#!/usr/bin/env python3
"""Compare the built flat binary against the loadable image in the baserom."""
import hashlib
import sys

BASEROM = "baserom/SLUS_210.75"
IMAGE_START, IMAGE_END = 0x80, 0x37B280  # file range loaded at 0x00100000
VRAM = 0x00100000

built = open(sys.argv[1], "rb").read()
want = open(BASEROM, "rb").read()[IMAGE_START:IMAGE_END]

if built == want:
    print(f"OK: {sys.argv[1]} matches ({hashlib.sha1(built).hexdigest()})")
    sys.exit(0)

print(f"MISMATCH: built {len(built):#x} bytes, expected {len(want):#x}")
for i, (a, b) in enumerate(zip(built, want)):
    if a != b:
        print(f"first difference at vram {VRAM + i:#010x} (file {IMAGE_START + i:#x})")
        break
sys.exit(1)
