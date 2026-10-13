IN: gcheck
USING: tester engine game graphics ;
key: F1 true key-hold  game-tick  key: F1 false key-hold  game-tick
T{ menu-open @ 0<> -> -1 }T
test-summary
