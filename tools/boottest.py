#!/usr/bin/env python3
"""Boot an ELF in PCSX2 against the game ISO and check what's on screen.

    tools/boottest.py build/SLUS_210.75.shift-all.elf            # default: 21 s, check frame at 20 s
    tools/boottest.py ELF --secs 92 --shots 30,60,90 --keep      # keep screenshots in build/boottest/

Uses a private PCSX2 data dir (build/pcsx2/) seeded from ~/.config/PCSX2 so your own
settings and memory cards are untouched: EE console logging on, no shutdown prompt.
Only PCSX2's own windows are captured (xdotool + ImageMagick `import`).

Reports TLB misses from the emulator log and, for the frame at --check seconds
(the Capcom logo at ~20 s), mean R/G/B: BLACK, TINTED (G >> R) or OK.
"""
import argparse
import os
import shutil
import signal
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ISO = os.environ.get("HG_ISO", str(ROOT.parent / "Haunting Ground (USA).iso"))
DATA = ROOT / "build" / "pcsx2"
OUT = ROOT / "build" / "boottest"


def setup() -> None:
    ini = DATA / "PCSX2" / "inis" / "PCSX2.ini"
    if ini.exists():
        return
    src = Path.home() / ".config/PCSX2/inis/PCSX2.ini"
    ini.parent.mkdir(parents=True, exist_ok=True)
    text = src.read_text()
    repl = {
        "Bios = bios": f"Bios = {Path.home() / '.config/PCSX2/bios'}",
        "EnableFileLogging = false": "EnableFileLogging = true",
        "EnableEEConsole = false": "EnableEEConsole = true",
        "EnableIOPConsole = false": "EnableIOPConsole = true",
        "ConfirmShutdown = true": "ConfirmShutdown = false",
    }
    for a, b in repl.items():
        text = text.replace(a, b)
    ini.write_text(text)


def windows(pid: int) -> list[str]:
    r = subprocess.run(["xdotool", "search", "--onlyvisible", "--pid", str(pid)], capture_output=True, text=True)
    return r.stdout.split()


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("elf")
    ap.add_argument("--secs", type=int, default=21)
    ap.add_argument("--shots", default="20", help="comma-separated seconds to screenshot")
    ap.add_argument("--check", type=int, default=20, help="which shot to analyse")
    ap.add_argument("--keep", action="store_true")
    args = ap.parse_args()

    setup()
    OUT.mkdir(parents=True, exist_ok=True)
    log = DATA / "PCSX2" / "logs" / "emulog.txt"
    log.unlink(missing_ok=True)
    tag = Path(args.elf).stem
    shots = {int(x) for x in args.shots.split(",")}
    p = subprocess.Popen(
        ["pcsx2-qt", "-datapath", str(DATA), "-batch", "-fastboot", "-elf", str(Path(args.elf).resolve()), "--", ISO],
        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
    )
    files = {}
    try:
        for t in range(1, args.secs + 1):
            time.sleep(1)
            if p.poll() is not None:
                break
            if t in shots:
                for i, w in enumerate(windows(p.pid)):
                    f = OUT / f"{tag}_{t}s_{i}.png"
                    subprocess.run(["import", "-window", w, str(f)], capture_output=True)
                    files.setdefault(t, f)  # first window is the game window
    finally:
        p.send_signal(signal.SIGTERM)
        try:
            p.wait(10)
        except subprocess.TimeoutExpired:
            p.kill()

    misses = log.read_text(errors="replace").count("TLB Miss") if log.exists() else -1
    print(f"{tag}: TLB misses {misses}")
    f = files.get(args.check)
    if f:
        r = subprocess.run(
            ["magick", str(f), "-crop", "90%x80%+0+40", "-format", "%[fx:mean.r] %[fx:mean.g] %[fx:mean.b]", "info:"],
            capture_output=True, text=True,
        )
        rr, gg, bb = map(float, r.stdout.split())
        verdict = "BLACK" if gg < 0.2 else "TINTED" if gg - rr > 0.05 else "OK"
        print(f"{tag}: frame@{args.check}s R={rr:.3f} G={gg:.3f} B={bb:.3f} {verdict}")
    if not args.keep:
        for f in OUT.glob(f"{tag}_*.png"):
            if f != files.get(args.check):
                f.unlink()
    sys.exit(0 if misses == 0 else 1)


if __name__ == "__main__":
    main()
