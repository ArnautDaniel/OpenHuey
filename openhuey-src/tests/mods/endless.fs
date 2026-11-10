\ A mod whose script never ends as it loads (tests/mods/endless-mod): stopped with an error and
\ turned off, the game loaded all the same. Run with the mod (CMakeLists.txt):
\   build/openhuey-src/openhuey --mod openhuey-src/tests/mods/endless-mod --test openhuey-src/tests/mods/endless.fs
IN: test-endless-mod
USING: tester engine game-state actors messages game debug ;

2 run-frames
testing the game runs, its actors there; nothing of the mod's vocabulary past the error is in use
T{ game:fiona alive? -> -1 }T
T{ 5 run-frames  game:fiona alive? -> -1 }T
test-summary
