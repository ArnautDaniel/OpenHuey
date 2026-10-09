\ The stalkers, P1: Debilitas in the house (docs/subsystems/stalker.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/stalker/test_stalker.fs
IN: test-stalker
USING: tester engine game-state actors messages room-names game debug stalker stalker.state ;
fvariable dx  fvariable dy  fvariable dz  fvariable dyaw

: frames ( n -- )  0 ?do  game-tick  loop ;
: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: danger-room ( -- room )  game:danger enter  danger:s-room @  leave-actor ;   \ (the room he last told)
: shown? ( -- flag )  [: him act.visible l@ 0<> ;] his ;

free-play  20 frames
testing out of the game at first: no body, not shown
T{ game:debilitas body?  shown? -> 0 0 }T
testing into Fiona's room: placed, shown, waiting about (mode 3), full health
s" fiona" actor-named body-tri debug:debilitas-tri  2 frames
T{ game:debilitas body?  game:debilitas body-room room-id =  shown? -> -1 -1 -1 }T
T{ [: my-mode @  hp @  hp-max @ ;] his -> 3 70 70 }T
testing he tells the danger he is about
T{ danger-room -> room-id }T
testing in another room he isn't shown
front-garden-2 room!  5 frames
T{ shown? -> 0 }T
testing out of the game
debug:debilitas-out  2 frames
T{ game:debilitas body?  shown? -> 0 0 }T

testing his senses (P1b): seen ahead in his view, within reach close behind, not far behind; a noise heard
front-garden-2 room!  10 frames
s" fiona" actor-named body-tri debug:debilitas-tri  3 frames
game:debilitas body-pos dz f! dy f! dx f!  game:debilitas body-yaw dyaw f!
: there ( F: d -- )   \ (Fiona put d along his heading: negative behind him)
    fdup dyaw f@ fsin f* dx f@ f+  dy f@  fswap dyaw f@ fcos f* dz f@ f+  tp  4 frames ;
: seen ( -- flag )  [: sees-fiona @ ;] his ;
T{ 40e there  seen  [: d-fiona f@ ;] his 40e f- fabs 1e f< -> -1 -1 }T
T{ -15e there  seen -> -1 }T
T{ -60e there  seen -> 0 }T
: heard-any ( -- flag )   \ (a noise each frame; heard on the frame after its end)
    0 10 0 do  $14 room-id s" fiona" actor-named body-tri -1 0 game:acoustics send noise  game-tick  [: did-hear @ ;] his or  loop ;
T{ heard-any -> -1 }T
debug:debilitas-out  2 frames

test-summary
