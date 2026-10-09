\ The stalkers, P1: Debilitas in the house (docs/subsystems/stalker.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/stalker/test_stalker.fs
IN: test-stalker
USING: tester engine game-state actors messages room-names game debug paths doors stalker stalker.state stalker.senses stalker.moving stalker.search stalker.travel ;
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
: her-y ( F: -- y )  s" fiona" actor-named body-pos fdrop fswap fdrop ;
140.64e her-y -217.33e 0e tp-facing  2 frames   \ (an open spot: him there, facing +z)
s" fiona" actor-named body-tri debug:debilitas-tri  3 frames
: face ( -- )   \ (him facing 340 degrees: open floor 40 ahead and 15 behind, a wall 30 behind)
    [: 340e deg>rad body-turn ;] his  1 frames
    game:debilitas body-pos dz f! dy f! dx f!  game:debilitas body-yaw dyaw f! ;
face
fvariable dd
: there ( F: d -- )   \ (Fiona put d along his heading: negative behind him)
    dd f!  dd f@ dyaw f@ fsin f* dx f@ f+  dy f@  dd f@ dyaw f@ fcos f* dz f@ f+  tp  4 frames ;
: on-floor? ( -- flag )  s" fiona" actor-named body-tri 0< 0= ;
: seen ( -- flag )  [: sees-fiona @ ;] his ;
T{ 40e there  on-floor?  seen  [: d-fiona f@ ;] his 40e f- fabs 1e f< -> -1 -1 -1 }T
T{ -15e there  on-floor?  seen -> -1 -1 }T
T{ -30e there  on-floor?  seen -> -1 0 }T
: heard-any ( -- flag )   \ (a noise each frame; heard on the frame after its end)
    0 10 0 do  $14 room-id s" fiona" actor-named body-tri -1 0 game:acoustics send noise  game-tick  [: did-hear @ ;] his or  loop ;
T{ heard-any -> -1 }T
debug:debilitas-out  2 frames

testing searching (P1c): he sets off for a stop; his route over, he waits, then searches the room (2..5 stops)
front-garden-2 room!  10 frames
140.64e her-y -217.33e 0e tp-facing  2 frames
s" fiona" actor-named body-tri  -500e her-y -500e tp  2 frames   \ (her well out of his sight)
debug:debilitas-tri  2 frames
create p0 12 allot  p0 game:debilitas body-at vec-copy
: moved ( F: -- d )  game:debilitas body-at p0 vec-dist ;
: wait-for ( xt n -- flag )   \ (up to n frames for xt, run as him, to be true)
    0 ?do  dup his if  drop true unloop exit  then  game-tick  loop  drop false ;
T{ 120 frames  moved 3e f>  [: route-end @ 1 >= ;] his -> -1 -1 }T
T{ [: route-done @ ;] 300 wait-for  [: my-mode @ ;] his -> -1 3 }T
[: 1 mode-t ! ;] his  3 frames
T{ [: my-mode @  route-end @ dup 2 >= swap 5 <= and  search-phase @ ;] his -> 2 -1 1 }T
T{ [: route-next @ 1 >= ;] 900 wait-for -> -1 }T
testing seeing her he's after her (mode 0): closer on foot
140.64e her-y -217.33e 0e tp-facing  2 frames   \ (the open spot again: him there, her away)
s" fiona" actor-named body-tri  -500e her-y -500e tp  2 frames  debug:debilitas-tri  1 frames
face  40e there
T{ on-floor?  [: my-mode @  sees-fiona @ ;] his -> -1 0 -1 }T
: d-her ( F: -- d )  s" fiona" actor-named body-at  game:debilitas body-at vec-dist ;
fvariable d0  d-her d0 f!  90 frames
T{ d-her d0 f@ 5e f- f< -> -1 }T
debug:debilitas-out  2 frames

testing out of sight (P1d): heading for her room he comes through the doors into it, on its floor, shown, searching
front-garden-3 room!  10 frames
front-garden-2 0 game:debilitas send stalker-in  2 frames
T{ game:debilitas body-room  game:debilitas body-tri  shown? -> front-garden-2 -1 0 }T
[: 1 mode! ;] his
T{ [: my-room front-garden-3 = ;] 900 wait-for  game:debilitas body-tri 0< 0=  shown? -> -1 -1 -1 }T
T{ [: my-mode @ dup 0= swap 2 = or ;] his -> -1 }T
testing she leaves his room: out of sight, after her he follows her into the next
[: 0 mode! ;] his  front-garden-2 room!  2 frames
T{ game:debilitas body-tri  shown?  [: goal-room @ ;] his -> -1 0 front-garden-2 }T
T{ [: my-room front-garden-2 = ;] 900 wait-for  game:debilitas body-tri 0< 0= -> -1 -1 }T
testing she comes into his room: he's put on its floor
debug:debilitas-out  2 frames  front-garden-3 0 game:debilitas send stalker-in  2 frames
T{ game:debilitas body-tri shown? -> -1 0 }T
front-garden-3 room!  3 frames
T{ game:debilitas body-tri 0< 0=  shown?  game:debilitas body-room -> -1 -1 front-garden-3 }T
debug:debilitas-out  2 frames

testing at a shut door into her room he knocks, waits, then comes in and it swings open
castle-1f-2 room!  10 frames
castle-1f-10 debug:debilitas-hunt  2 frames
T{ [: knock-t @ 0> ;] 900 wait-for  [: away? ;] his -> -1 -1 }T
T{ [: my-room castle-1f-2 = ;] 200 wait-for  game:debilitas body-tri 0< 0= -> -1 -1 }T
T{ 30 frames  32 state@ opened-bit and 0<> -> -1 }T
debug:debilitas-out  2 frames

test-summary
