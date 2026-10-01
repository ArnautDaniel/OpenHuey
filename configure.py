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

ASFLAGS = "-EL -march=r5900 -mabi=n32 -G 0 -no-pad-sections -I include"
CFLAGS = (
    "-EL -march=r5900 -mabi=n32 -G 0 -O2 -fno-common -ffreestanding "
    "-fno-builtin -fno-pic -mno-abicalls -I include -I src"
)
CXXFLAGS = CFLAGS + " -fno-exceptions -fno-rtti"


def run_splat() -> None:
    subprocess.run(
        [sys.executable, "-m", "splat", "split", YAML], cwd=ROOT, check=True
    )


def make_ld_script() -> list[str]:
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
    text = text.replace(
        "    main_BSS_SIZE = ABSOLUTE(main_BSS_END - main_BSS_START);\n    }\n",
        "    main_BSS_SIZE = ABSOLUTE(main_BSS_END - main_BSS_START);\n    }\n    _end = .;\n",
    )
    # Load address = run address (splat's AT(ROM_START) is meant for cart ROMs)
    text = re.sub(r" AT\([A-Za-z0-9_]+\)", "", text)
    text = "ENTRY(_start)\n" + text
    out = ROOT / LD_SCRIPT
    out.parent.mkdir(parents=True, exist_ok=True)
    if not out.exists() or out.read_text() != text:
        out.write_text(text)
    return sorted(set(re.findall(r"(build/\S+?\.o)\(", text)))


def source_for(obj: str) -> str:
    """build/asm/foo.s.o -> asm/foo.s, build/src/a.c.o -> src/a.c, etc."""
    return obj[len("build/") : -len(".o")]


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--split", action="store_true", help="re-run splat")
    args = ap.parse_args()

    if args.split or not (ROOT / LD_SCRIPT_SPLAT).exists():
        run_splat()

    objs = make_ld_script()

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
            r"""printf '.section .data\n.incbin "%s"\n' $in | ${tc}as $asflags -o $out -""",
            description="BIN $in",
        )
        n.rule(
            "ld",
            "${tc}ld -EL -m elf32lr5900n32 -T $ldscript -T linker/undefined_syms_auto.txt "
            "-T linker/undefined_funcs_auto.txt -Map $mapfile -o $out",
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
                n.build(obj, "bin2o", src)
            else:
                raise SystemExit(f"don't know how to build {obj}")
        n.newline()

        n.build(
            ELF,
            "ld",
            [],
            implicit=objs
            + [LD_SCRIPT, "linker/undefined_syms_auto.txt", "linker/undefined_funcs_auto.txt"],
            variables={"ldscript": LD_SCRIPT, "mapfile": f"build/{BASENAME}.map"},
        )
        n.build(BIN, "objcopy", ELF)
        n.build("build/check.ok", "check", BIN)
        n.build(
            "build.ninja",
            "configure",
            implicit=["configure.py", LD_SCRIPT_SPLAT],
        )
        n.default("build/check.ok")


if __name__ == "__main__":
    main()
