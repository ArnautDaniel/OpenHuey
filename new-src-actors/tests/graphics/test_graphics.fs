\ The graphics menu (scripts/graphics.fs): F1 opens it. Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/graphics/test_graphics.fs
IN: test-graphics
USING: tester engine game graphics ;

testing F1 opens the menu
key: F1 true key-hold  game-tick  key: F1 false key-hold  game-tick
T{ menu-open @ 0<> -> -1 }T
test-summary
