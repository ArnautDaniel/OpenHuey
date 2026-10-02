#!/usr/bin/env python3
"""Convert every character/object model of the game to .glb (see tools/hg_model.py).

    tools/hg_export_all.py <extracted DATA.CVM folder> [-o build/models]

For each O_<NAME> folder and each <NAME>_nnn.PCK in it: textures from <NAME>_nnn.TEX if it
exists, else <NAME>_200.TEX for 2xx models if it exists, else <NAME>_000.TEX; motions from the
model's own bank plus the folder's .MTN files (and O_<NAME>_M; Fiona's O_FIN_M for all
her outfits FI*). The *_D000.MTN files are not motion banks (door data) and are skipped.
Writes <out>/<NAME>/<NAME>_nnn.glb.
"""
import argparse
import sys
import traceback
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hg_model  # noqa: E402


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("data", type=Path, help="folder with the O_* folders (extracted DATA.CVM)")
    ap.add_argument("-o", "--out", type=Path, default=Path("build/models"))
    ap.add_argument("--no-mtn", action="store_true", help="only each model's own motion bank")
    a = ap.parse_args()

    ok = failed = 0
    for folder in sorted(a.data.glob("O_*")):
        name = folder.name[2:]
        if name.endswith("_M"):
            continue
        pcks = sorted(folder.glob("*.PCK"))
        if not pcks:
            continue
        # (the *_D000.MTN files are something else: door data, not motion banks)
        shared = "FIN" if name.startswith("FI") else name    # Fiona's outfits share her motions
        mtns = [] if a.no_mtn else [m for m in sorted(folder.glob("*.MTN")) + sorted((a.data / f"O_{shared}_M").glob("*.MTN"))
                                    if not m.stem.endswith("D000")]
        for pck in pcks:
            stem = pck.stem
            tex = folder / f"{stem}.TEX"
            if not tex.exists() and stem.split("_")[-1].startswith("2"):
                tex = folder / f"{name}_200.TEX"
            if not tex.exists():
                tex = folder / f"{name}_000.TEX"
            out = a.out / name / f"{stem}.glb"
            out.parent.mkdir(parents=True, exist_ok=True)
            args = [str(pck), "-o", str(out)]
            if tex.exists():
                args += ["--tex", str(tex)]
            for m in mtns:
                args += ["--mtn", str(m)]
            try:
                hg_model.main(args)
                ok += 1
            except (Exception, SystemExit) as e:   # keep going: report and continue
                failed += 1
                print(f"FAILED {pck.name}: {e}")
                traceback.print_exc(limit=1)
    print(f"{ok} converted, {failed} failed")


if __name__ == "__main__":
    main()
