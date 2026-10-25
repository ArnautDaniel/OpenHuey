\ The play pause (pause.fs; src/game/pause.c Pause_*): Start in play holds the world under a dim
\ screen; Start goes back; Select the menu - back / quit, quitting asked. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/pause/test_pause.fs
IN: test-pause
USING: tester engine game-state actors messages room-names game debug pause music ;

: press-key ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: stage? ( -- n )  game:pause enter  pause:stage @  leave-actor ;
: choice? ( -- n )  game:pause enter  pause:choice @  leave-actor ;
create was 12 allot
: still? ( -- flag )   \ held W for 20 frames, she hasn't moved
    game:fiona body-at was vec-copy
    key: W true key-hold  20 run-frames  key: W false key-hold
    game:fiona body-at was vec-dist 0.01e f< ;
free-play  20 run-frames

testing Start: open (5 frames), the world held - she doesn't move
key: Return press-key  6 run-frames
T{ stage? -> open }T
T{ still? -> -1 }T
testing the stream turned down with it, back as it closes (pause.c duck: Bgm +0x114)
: ducked ( -- n )  game:music enter  music:bgm-duck f@ 100e f* fround f>s  leave-actor ;
T{ ducked -> 0 }T
testing Start again: closed, the world going on
key: Return press-key  6 run-frames
T{ stage? still? ducked -> closed 0 100 }T

testing Select on it: the menu; "back" closes it
key: Return press-key  6 run-frames  key: Tab press-key
T{ stage? choice? -> on-menu 0 }T
key: X press-key  6 run-frames
T{ stage? -> closed }T

testing "quit" asks - "no" is first; no goes back to the pause; cancel too
key: Return press-key  6 run-frames  key: Tab press-key  key: Down press-key
T{ choice? -> 1 }T
key: X press-key
T{ stage? choice? -> asking 1 }T
key: X press-key
T{ stage? -> open }T
key: Tab press-key  key: Down press-key  key: X press-key  key: V press-key
T{ stage? -> open }T
key: Return press-key  6 run-frames
T{ stage? -> closed }T
test-summary
