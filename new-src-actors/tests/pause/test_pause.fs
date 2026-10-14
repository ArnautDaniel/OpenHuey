\ The play pause (pause.fs; src/game/pause.c Pause_*): Start in play holds the world under a dim
\ screen; Start goes back; Select the menu - back / quit, quitting asked. Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/pause/test_pause.fs
IN: test-pause
USING: tester engine game-state actors messages room-names game debug pause ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: press ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: stage? ( -- n )  game:pause enter  pause:stage @  leave-actor ;
: choice? ( -- n )  game:pause enter  pause:choice @  leave-actor ;
create was 12 allot
: still? ( -- flag )   \ held W for 20 frames, she hasn't moved
    game:fiona body-at was vec-copy
    key: W true key-hold  20 frames  key: W false key-hold
    game:fiona body-at was vec-dist 0.01e f< ;
free-play  20 frames

testing Start: open (5 frames), the world held - she doesn't move
key: Return press  6 frames
T{ stage? -> open }T
T{ still? -> -1 }T
testing Start again: closed, the world going on
key: Return press  6 frames
T{ stage? still? -> closed 0 }T

testing Select on it: the menu; "back" closes it
key: Return press  6 frames  key: Tab press
T{ stage? choice? -> on-menu 0 }T
key: X press  6 frames
T{ stage? -> closed }T

testing "quit" asks - "no" is first; no goes back to the pause; cancel too
key: Return press  6 frames  key: Tab press  key: Down press
T{ choice? -> 1 }T
key: X press
T{ stage? choice? -> asking 1 }T
key: X press
T{ stage? -> open }T
key: Tab press  key: Down press  key: X press  key: V press
T{ stage? -> open }T
key: Return press  6 frames
T{ stage? -> closed }T
test-summary
