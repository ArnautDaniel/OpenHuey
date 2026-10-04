#!/usr/bin/env python3
"""Generate build.ninja for the Haunting Ground (USA) decomp.

Usage:
    ./configure.py            # write build.ninja (runs splat if asm/ is missing)
    ./configure.py --split    # force a fresh splat run first
    ninja                     # build both ELFs, check the asm-only one matches

Outputs:
    build/SLUS_210.75.elf         pure asm; must stay byte-identical to the baserom
    build/SLUS_210.75.decomp.elf  the game with decompiled C (src/**) replacing asm
    build/SLUS_210.75.shift-*.elf decomp build with padding inserted (shiftability tests)
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
DECOMP_LD_SCRIPT = f"build/{BASENAME}.decomp.ld"
DECOMP_ELF = f"build/{BASENAME}.decomp.elf"
CODE_ASM = ("asm/game.s", "asm/sinit.s")  # asm files decompiled functions are removed from
BIN = f"build/{BASENAME}.bin"
EXTRA_LD = "config/extra_syms.ld"

ASFLAGS = "-EL -march=r5900 -mabi=n32 -msingle-float -G 0 -no-pad-sections -I include"
CFLAGS = (
    # EABI, like the original code: int and float arguments fill a0.. and f12.. in order
    # (n32 assigns registers by argument position, which breaks calls into the asm)
    "-EL -march=r5900 -mabi=eabi -mlong32 -G 0 -O2 -fno-common -ffreestanding -fno-strict-aliasing "
    "-fno-builtin -fno-pic -mno-abicalls -fno-delete-null-pointer-checks "
    "-fno-isolate-erroneous-paths-dereference -mno-check-zero-division -I include -I src"
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
    subprocess.run([sys.executable, "tools/align_data.py"], cwd=ROOT, check=True)


# Where `ninja shift-<name>` inserts padding (anchor line in the linker script).
SHIFT_POINTS = {
    "all": "        build/asm/game.s.o(.text);\n",  # everything after crt0 moves
    "data": "        main_DATA_START = .;\n",        # data, rodata, sinit, bss move
    "sinit": "        sinit_TEXT_START = .;\n",      # sinit code/data and bss move
    "bss": "        sinit_BSS_START = .;\n",         # only bss moves
    "irx": "        build/assets/cdvdman_irx.bin.o(.data);\n",
    "rodata": "        main_RODATA_START = .;\n",
}


def make_ld_script(out_path: str = LD_SCRIPT, shift: int = 0, at: str = "all") -> list[str]:
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
    # Input sections keep their own alignment (tools/align_data.py); splat's
    # SUBALIGN(16) would override it and break 64-byte aligned objects on a shift.
    text = text.replace(" SUBALIGN(16)", "")
    # Load address = run address (splat's AT(ROM_START) is meant for cart ROMs)
    text = re.sub(r" AT\([A-Za-z0-9_]+\)", "", text)
    # _gp is defined relative to BSS in config/extra_syms.ld
    text = re.sub(r"\n    _gp = 0x[0-9A-Fa-f]+;", "", text)
    if shift:
        # Test build: pad before the anchor so everything after it moves.
        anchor = SHIFT_POINTS[at]
        assert anchor in text, f"shift anchor for {at!r} not found"
        text = text.replace(anchor, f"        . += 0x{shift:X};\n" + anchor, 1)
    text = "ENTRY(_start)\n" + text
    out = ROOT / out_path
    out.parent.mkdir(parents=True, exist_ok=True)
    if not out.exists() or out.read_text() != text:
        out.write_text(text)
    return sorted(set(re.findall(r"(build/\S+?\.o)\(", text)))


FUNC_DEF_RE = re.compile(
    r"^(?!static\b|typedef\b|extern\b)[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{", re.M
)


def decomp_sources() -> list[str]:
    return sorted(str(p.relative_to(ROOT)) for ext in ("*.c", "*.cpp") for p in (ROOT / "src").rglob(ext))


def without_native_only(text: str) -> str:
    """Drop the lines only the PC build compiles (#ifdef HG_NATIVE branches, #ifndef's #else):
    a PC-only replacement of a function doesn't replace its asm in the PS2 build."""
    out, stack = [], []   # stack: per open #if, whether its current branch is PC-only
    for line in text.splitlines():
        d = line.strip()
        if d.startswith("#if"):
            native = re.match(r"#\s*(ifdef\s+HG_NATIVE|if\s+defined\s*\(?\s*HG_NATIVE\b)", d)
            stack.append("native" if native else ("pc_else" if re.match(r"#\s*ifndef\s+HG_NATIVE", d) else None))
        elif d.startswith("#else") and stack:
            stack[-1] = {"native": "ps2", "pc_else": "native"}.get(stack[-1], stack[-1])
        elif d.startswith("#endif") and stack:
            stack.pop()
        elif "native" not in stack:
            out.append(line)
    return "\n".join(out)


def decompiled_funcs(sources: list[str]) -> dict[str, list[str]]:
    """asm file -> functions defined in C that replace asm there."""
    defined = set()
    for src in sources:
        defined.update(FUNC_DEF_RE.findall(without_native_only((ROOT / src).read_text())))
    out: dict[str, list[str]] = {}
    for asm in CODE_ASM:
        names = set(re.findall(r"^\s*(?:glabel|alabel) (\S+)", (ROOT / asm).read_text(), re.M))
        out[asm] = sorted(defined & names)
    return out


def make_decomp_ld_script(base_text: str, sources: list[str], stripped: dict[str, list[str]], out_path: str) -> None:
    """Swap stripped asm objects for build/decomp/ copies and link the C objects next to them."""
    text = base_text
    for asm, funcs in stripped.items():
        if funcs:
            text = text.replace(f"build/{asm}.o(", f"build/decomp/{Path(asm).name}.o(")
    objs = [f"build/{s}.o" for s in sources]
    anchor_obj = "build/decomp/game.s.o" if stripped.get("asm/game.s") else "build/asm/game.s.o"
    for sect, pats in (
        (".text", ".text .text.*"),
        (".data", ".data .data.* .sdata .sdata.*"),
        (".rodata", ".rodata .rodata.*"),
        (".bss", ".bss .bss.* .sbss .sbss.* COMMON"),
    ):
        line = f"        {anchor_obj}({sect});\n"
        assert line in text, f"anchor {line!r} missing"
        text = text.replace(line, line + "".join(f"        {o}({pats});\n" for o in objs), 1)
    out = ROOT / out_path
    if not out.exists() or out.read_text() != text:
        out.write_text(text)


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
    """build/asm/foo.s.o -> asm/foo.s, build/src/a.c.o -> src/a.c,
    build/decomp/game.s.o -> build/decomp/game.s (generated by strip_funcs)"""
    if obj.startswith("build/decomp/"):
        return obj[: -len(".o")]
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
    undef = filter_undefined(objs)
    sources = decomp_sources()
    stripped = decompiled_funcs(sources)
    make_decomp_ld_script((ROOT / LD_SCRIPT).read_text(), sources, stripped, DECOMP_LD_SCRIPT)
    for at in SHIFT_POINTS:
        make_ld_script(f"build/{BASENAME}.shift-{at}.ld.tmp", args.shift, at)
        tmp = ROOT / f"build/{BASENAME}.shift-{at}.ld.tmp"
        make_decomp_ld_script(tmp.read_text(), sources, stripped, f"build/{BASENAME}.shift-{at}.ld")
        tmp.unlink()
    decomp_objs = sorted(set(re.findall(r"(build/\S+?\.o)\(", (ROOT / DECOMP_LD_SCRIPT).read_text())))

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
            f"{PY} tools/eecc.py ${{tc}}gcc -c $cflags -MMD -MF $out.d -o $out $in",
            description="CC $in",
            depfile="$out.d",
            deps="gcc",
        )
        n.rule(
            "cxx",
            f"{PY} tools/eecc.py ${{tc}}g++ -c $cxxflags -MMD -MF $out.d -o $out $in",
            description="CXX $in",
            depfile="$out.d",
            deps="gcc",
        )
        n.rule(
            "bin2o",
            # objcopy -I binary output lacks the n32 ABI flags, so wrap via as
            # The blob gets global labels named after the file (assets/foo.bin -> foo, foo_bin_end).
            r"""printf '.section .data\n.balign 16\n.globl %s, %s_bin_end\n%s:\n.incbin "%s"\n%s_bin_end:\n'"""
            " $sym $sym $sym $in $sym"
            " | ${tc}as $asflags -o $out -",
            description="BIN $in",
        )
        n.rule(
            "ld",
            # --no-warn-mismatch: C objects are EABI, the asm is assembled as n32 (only its
            # register names and ELF tag differ; the code itself follows the original's ABI)
            f"${{tc}}ld -EL -m elf32lr5900n32 --no-warn-mismatch -T $ldscript -T {EXTRA_LD} $undef_scripts "
            "--emit-relocs -Map $mapfile -o $out",
            description="LD $out",
        )
        n.rule("objcopy", "${tc}objcopy -O binary $in $out", description="OBJCOPY $out")
        n.rule(
            "strip_funcs",
            f"{PY} tools/strip_funcs.py $in $out $funcs",
            description="STRIP $in",
        )
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

        for asm, funcs in stripped.items():
            if funcs:
                n.build(
                    f"build/decomp/{Path(asm).name}",
                    "strip_funcs",
                    asm,
                    implicit=["tools/strip_funcs.py"],
                    variables={"funcs": " ".join(funcs)},
                )
        for obj in sorted(set(objs) | set(decomp_objs)):
            src = source_for(obj)
            if src.endswith(".s") and src.startswith("build/decomp/"):
                n.build(obj, "as", src)
            elif src.endswith(".s"):
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
        n.build(
            DECOMP_ELF,
            "ld",
            [],
            implicit=decomp_objs + [DECOMP_LD_SCRIPT, EXTRA_LD, *undef],
            variables={
                "ldscript": DECOMP_LD_SCRIPT,
                "undef_scripts": " ".join(f"-T {u}" for u in undef),
                "mapfile": f"build/{BASENAME}.decomp.map",
            },
        )
        # Shiftability tests: decomp build, padding inserted at SHIFT_POINTS
        for at in SHIFT_POINTS:
            elf = f"build/{BASENAME}.shift-{at}.elf"
            ld = f"build/{BASENAME}.shift-{at}.ld"
            n.build(
                elf,
                "ld",
                [],
                implicit=decomp_objs + [ld, EXTRA_LD, *undef],
                variables={
                    "ldscript": ld,
                    "undef_scripts": " ".join(f"-T {u}" for u in undef),
                    "mapfile": f"build/{BASENAME}.shift-{at}.map",
                },
            )
            n.build(f"shift-{at}", "phony", elf)
        n.build("shift", "phony", [f"shift-{at}" for at in SHIFT_POINTS])
        n.build("build/check.ok", "check", BIN)
        n.build(
            "build.ninja",
            "configure",
            implicit=[
                "configure.py",
                *sources,
                LD_SCRIPT_SPLAT,
                "linker/undefined_syms_auto.txt",
                "linker/undefined_funcs_auto.txt",
            ],
        )
        n.default(["build/check.ok", DECOMP_ELF])


if __name__ == "__main__":
    main()
