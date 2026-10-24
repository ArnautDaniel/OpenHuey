\ base.fs - OpenHuey's own words for every script, loaded right after the language's prelude
\ (ClankForth: external/clankforth, lib/prelude.fs).

\ seconds into frames: 30 a second, the game's rate (`wait` and `yield` give way to the rest of
\ the frame)
: seconds  30 * ;
