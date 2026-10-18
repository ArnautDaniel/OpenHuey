#!/bin/sh
# The updater (src/platform/update.c), against a release made here (file:// - no internet): a
# newer build is put in place of what a release ships and the player's library/, user/ and mods/
# are left be; the same build is up to date; a broken download changes nothing.
#   tests/update/run.sh <openhuey> <its build name> <a scratch folder>
BIN="$1"
BUILD="$2"
T="$3/update-test"
fails=0
rm -rf "$T"
mkdir -p "$T/rel" "$T/inst"
ok() { echo "ok: $1"; }
bad() { echo "FAILED: $1"; fails=$((fails + 1)); }
check() { if eval "$2"; then ok "$1"; else bad "$1"; fi; }

release() {   # build-name [no-program]: rel/ holds that build
    name="openhuey-$1-linux-x86_64"
    rm -rf "$T/rel/"*
    mkdir -p "$T/rel/$name/scripts" "$T/rel/$name/user"
    [ "$2" = no-program ] || echo "new program" > "$T/rel/$name/openhuey"
    echo "new" > "$T/rel/$name/scripts/new.fs"
    echo "new readme" > "$T/rel/$name/README.txt"
    echo "not theirs" > "$T/rel/$name/user/settings.fs"   # (never put over the player's)
    tar czf "$T/rel/$name.tar.gz" -C "$T/rel" "$name"
    rm -rf "${T:?}/rel/$name"
    printf 'openhuey-version %s\n%s %s\nopenhuey-%s-windows-x64.zip 1\n' "$1" "$name.tar.gz" \
        "$(wc -c < "$T/rel/$name.tar.gz" | tr -d ' ')" "$1" > "$T/rel/version.txt"
}

installed() {   # the folder updated: an older build, the player's things
    rm -rf "$T/inst"
    mkdir -p "$T/inst/scripts" "$T/inst/user" "$T/inst/library" "$T/inst/mods"
    echo "old program" > "$T/inst/openhuey"
    echo "old" > "$T/inst/scripts/old.fs"
    echo "theirs" > "$T/inst/user/settings.fs"
    echo "manifest" > "$T/inst/library/manifest.txt"
    echo "a mod" > "$T/inst/mods/order.txt"
}

URL="file://$(printf %s "$T/rel/" | sed 's/ /%20/g')"   # (a URL: no spaces)
update() {
    OPENHUEY_UPDATE_URL="$URL" OPENHUEY_UPDATE_DIR="$T/inst" "$BIN" --update > "$T/out.txt" 2>&1
}

# a newer build
release 9.9.9-test
installed
check "a newer build: --update succeeds" "update"
check "the program replaced" "grep -q 'new program' '$T/inst/openhuey'"
check "scripts/ replaced whole" "[ -f '$T/inst/scripts/new.fs' ] && [ ! -e '$T/inst/scripts/old.fs' ]"
check "a new file put in" "grep -q 'new readme' '$T/inst/README.txt'"
check "user/ left be" "grep -q theirs '$T/inst/user/settings.fs'"
check "library/ and mods/ left be" "[ -f '$T/inst/library/manifest.txt' ] && [ -f '$T/inst/mods/order.txt' ]"
check "it says so" "grep -q 'updated to 9.9.9-test' '$T/out.txt'"
check "the old things gone" "[ ! -e '$T/inst/update/old' ]"

# this build
release "$BUILD"
installed
check "the same build: --update succeeds" "update"
check "up to date: nothing changed" "grep -q 'old program' '$T/inst/openhuey' && grep -q 'up to date' '$T/out.txt'"

# a download without the program
release 9.9.9-test no-program
installed
check "no program in it: --update fails" "! update"
check "nothing changed" "grep -q 'old program' '$T/inst/openhuey' && [ -f '$T/inst/scripts/old.fs' ]"

# a download cut short (its size not version.txt's)
release 9.9.9-test
sed -i 's/linux-x86_64.tar.gz [0-9]*/linux-x86_64.tar.gz 5/' "$T/rel/version.txt"
installed
check "a short download: --update fails" "! update"
check "nothing changed" "grep -q 'old program' '$T/inst/openhuey'"

# no release there
rm -rf "$T/rel/"*
installed
check "no release: --update fails" "! update"

rm -rf "$T"
[ $fails -eq 0 ] && echo "all passed" || echo "$fails failed"
exit $fails
