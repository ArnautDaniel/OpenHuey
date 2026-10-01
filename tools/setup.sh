#!/bin/sh
# One-time setup: Python venv + ps2dev EE toolchain (mips64r5900el-ps2-elf-*).
set -e
cd "$(dirname "$0")/.."
python3 -m venv .venv
.venv/bin/pip install -r requirements.txt
if [ ! -x tools/ps2dev/ps2dev/ee/bin/mips64r5900el-ps2-elf-as ]; then
    mkdir -p tools/ps2dev
    curl -L https://github.com/ps2dev/ps2dev/releases/download/v2.0.0/ps2dev-ubuntu-latest.tar.gz \
        | tar xz -C tools/ps2dev
fi
if [ ! -d tools/m2c ]; then
    git clone --depth 1 https://github.com/matt-kempster/m2c.git tools/m2c
    .venv/bin/pip install -e tools/m2c
fi
