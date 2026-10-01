#!/usr/bin/env python3
"""Generate build.ninja for the Haunting Ground (USA) decomp.

Usage:
    ./configure.py            # write build.ninja (runs splat if asm/ is missing)
    ./configure.py --split    # force a fresh splat run first
    ninja                     # build build/SLUS_210.75.elf and check it
"""
import argparse
import re
import shlex
import shutil
import subprocess
import sys
from pathlib import Path

import ninja_syntax

ROOT = Path(__file__).parent.resolve()
PY = shlex.quote(sys.executable)
BASENAME = "SLUS_210.75"
YAML = f"config/{BASENAME}.yaml"
TOOLCHAIN = "tools/ps2dev/ps2dev/ee/bin/mips64r5900el-ps2-elf-"

LD_SCRIPT_SPLAT = f"linker/{BASENAME}.ld"
LD_SCRIPT = f"build/{BASENAME}.ld"
ELF = f"build/{BASENAME}.elf"
BIN = f"build/{BASENAME}.bin"
EXTRA_LD = "config/extra_syms.ld"
SHIFT_LD_SCRIPT = f"build/{BASENAME}.shift.ld"
SHIFT_ELF = f"build/{BASENAME}.shift.elf"

ASFLAGS = "-EL -march=r5900 -mabi=n32 -G 0 -no-pad-sections -I include"
CFLAGS = (
    "-EL -march=r5900 -mabi=n32 -G 0 -O2 -fno-common -ffreestanding "
    "-fno-builtin -fno-pic -mno-abicalls -I include -I src"
)
CXXFLAGS = CFLAGS + " -fno-exceptions -fno-rtti"


def run_splat() -> None:
    # splat never deletes outputs; stale files would linger after layout changes
    for d in ("asm", "assets"):
        shutil.rmtree(ROOT / d, ignore_errors=True)
    subprocess.run(
        [sys.executable, "-m", "splat", "split", YAML], cwd=ROOT, check=True
    )
    subprocess.run([sys.executable, "tools/ptrpatch.py"], cwd=ROOT, check=True)
    subprocess.run([sys.executable, "tools/offpatch.py"], cwd=ROOT, check=True)


def make_ld_script(out_path: str = LD_SCRIPT, shift: int = 0) -> list[str]:
    """Turn splat's script into one that links a bootable ELF.

    splat emits the ELF header as an output section at address 0; we drop it
    (the linker writes its own header), make LMA == VMA, and add an entry point.
    Returns the list of object files the script references.
    """
    text = (ROOT / LD_SCRIPT_SPLAT).read_text()
    text = re.sub(
        r"\n    elf_header_ROM_START.*?elf_header_VRAM_END = \.;\n",
        "\n",
        text,
        flags=re.S,
    )
    # _end marks the top of BSS; crt0 and the heap setup in main use it.
    # Put it after the last BSS output section.
    m = list(re.finditer(r"\w+_BSS_SIZE = ABSOLUTE\(.*\);\n    }\n", text))[-1]
    text = text[: m.end()] + "    _end = .;\n" + text[m.end() :]
    # Load address = run address (splat's AT(ROM_START) is meant for cart ROMs)
    text = re.sub(r" AT\([A-Za-z0-9_]+\)", "", text)
    # _gp is defined relative to BSS in config/extra_syms.ld
    text = re.sub(r"\n    _gp = 0x[0-9A-Fa-f]+;", "", text)
    if shift:
        # Test build: pad after crt0 so every later function and datum moves.
        text = text.replace(
            "build/asm/crt0.s.o(.text);\n",
            f"build/asm/crt0.s.o(.text);\n        . += 0x{shift:X};\n",
        )
    text = "ENTRY(_start)\n" + text
    out = ROOT / out_path
    out.parent.mkdir(parents=True, exist_ok=True)
    if not out.exists() or out.read_text() != text:
        out.write_text(text)
    return sorted(set(re.findall(r"(build/\S+?\.o)\(", text)))


LABEL_RE = re.compile(r"^\s*(?:glabel|alabel|dlabel|jlabel|ehlabel)\s+(\S+)", re.M)


def filter_undefined(objs: list[str]) -> list[str]:
    """Drop symbols the asm defines from splat's undefined_*_auto.txt.

    A linker-script assignment overrides the object's definition, which would
    pin the symbol to its original address and break shiftability.
    """
    defined: set[str] = set()
    for obj in objs:
        src = ROOT / source_for(obj)
        if src.suffix == ".s":
            defined.update(LABEL_RE.findall(src.read_text()))
        elif src.suffix == ".bin":
            defined.add(src.stem)
    defined.update(re.findall(r"^(\w+)\s*=", (ROOT / EXTRA_LD).read_text(), re.M))
    outs = []
    for name in ("undefined_syms_auto.txt", "undefined_funcs_auto.txt"):
        lines = (ROOT / "linker" / name).read_text().splitlines(keepends=True)
        kept = [l for l in lines if l.split("=")[0].strip() not in defined]
        out = ROOT / "build" / name
        text = "".join(kept)
        if not out.exists() or out.read_text() != text:
            out.write_text(text)
        outs.append(f"build/{name}")
    return outs


def source_for(obj: str) -> str:
    """build/asm/foo.s.o -> asm/foo.s, build/src/a.c.o -> src/a.c, etc."""
    return obj[len("build/") : -len(".o")]


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--split", action="store_true", help="re-run splat")
    ap.add_argument(
        "--shift",
        type=lambda x: int(x, 0),
        default=0x1000,
        help="padding for the `ninja shift` test build (default 0x1000)",
    )
    args = ap.parse_args()

    if args.split or not (ROOT / LD_SCRIPT_SPLAT).exists():
        run_splat()

    objs = make_ld_script()
    make_ld_script(SHIFT_LD_SCRIPT, args.shift)
    undef = filter_undefined(objs)

    with open(ROOT / "build.ninja", "w") as f:
        n = ninja_syntax.Writer(f, width=120)
        n.variable("tc", TOOLCHAIN)
        n.variable("asflags", ASFLAGS)
        n.variable("cflags", CFLAGS)
        n.variable("cxxflags", CXXFLAGS)
        n.newline()

        n.rule("as", "${tc}as $asflags -o $out $in", description="AS $in")
        n.rule(
            "cc",
            "${tc}gcc -c $cflags -MMD -MF $out.d -o $out $in",
            description="CC $in",
            depfile="$out.d",
            deps="gcc",
        )
        n.rule(
            "cxx",
            "${tc}g++ -c $cxxflags -MMD -MF $out.d -o $out $in",
            description="CXX $in",
            depfile="$out.d",
            deps="gcc",
        )
        n.rule(
            "bin2o",
            # objcopy -I binary output lacks the n32 ABI flags, so wrap via as
            # The blob gets global labels named after the file (assets/foo.bin -> foo, foo_bin_end).
            r"""printf '.section .data\n.globl %s, %s_bin_end\n%s:\n.incbin "%s"\n%s_bin_end:\n'"""
            " $sym $sym $sym $in $sym"
            " | ${tc}as $asflags -o $out -",
            description="BIN $in",
        )
        n.rule(
            "ld",
            f"${{tc}}ld -EL -m elf32lr5900n32 -T $ldscript -T {EXTRA_LD} $undef_scripts "
            "--emit-relocs -Map $mapfile -o $out",
            description="LD $out",
        )
        n.rule("objcopy", "${tc}objcopy -O binary $in $out", description="OBJCOPY $out")
        n.rule(
            "check",
            f"{PY} tools/check.py $in && touch $out",
            description="CHECK $in",
        )
        n.rule(
            "configure",
            f"{PY} configure.py",
            generator=True,
            description="CONFIGURE",
        )
        n.newline()

        for obj in objs:
            src = source_for(obj)
            if src.endswith(".s"):
                n.build(obj, "as", src)
            elif src.endswith(".c"):
                n.build(obj, "cc", src)
            elif src.endswith((".cpp", ".cc")):
                n.build(obj, "cxx", src)
            elif src.startswith("assets/"):
                n.build(obj, "bin2o", src, variables={"sym": Path(src).stem})
            else:
                raise SystemExit(f"don't know how to build {obj}")
        n.newline()

        n.build(
            ELF,
            "ld",
            [],
            implicit=objs
            + [LD_SCRIPT, EXTRA_LD, *undef],
            variables={
                "ldscript": LD_SCRIPT,
                "undef_scripts": " ".join(f"-T {u}" for u in undef),
                "mapfile": f"build/{BASENAME}.map",
            },
        )
        n.build(BIN, "objcopy", ELF)
        # Shiftability test: same objects, padding inserted after crt0
        n.build(
            SHIFT_ELF,
            "ld",
            [],
            implicit=objs + [SHIFT_LD_SCRIPT, EXTRA_LD, *undef],
            variables={
                "ldscript": SHIFT_LD_SCRIPT,
                "undef_scripts": " ".join(f"-T {u}" for u in undef),
                "mapfile": f"build/{BASENAME}.shift.map",
            },
        )
        n.build("shift", "phony", SHIFT_ELF)
        n.build("build/check.ok", "check", BIN)
        n.build(
            "build.ninja",
            "configure",
            implicit=[
                "configure.py",
                LD_SCRIPT_SPLAT,
                "linker/undefined_syms_auto.txt",
                "linker/undefined_funcs_auto.txt",
            ],
        )
        n.default("build/check.ok")


if __name__ == "__main__":
    main()
