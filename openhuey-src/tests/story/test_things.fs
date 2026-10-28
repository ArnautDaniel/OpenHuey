\ The story, S5a: the room's things - placed objects, the doors' parts, lights, the look
\ (docs/subsystems/story.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_things.fs
IN: test-things
USING: tester engine game-state actors messages room-names game debug flag-names story.strings story.kit ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
\ a placed object by the room's string: shown?
variable want-n  variable want-a
: object-shown? ( obj -- flag )
    room-id room-string want-n ! want-a !
    placed-count 0 ?do
        i placed-info >r fdrop fdrop fdrop
        want-a @ want-n @ compare 0= if  r> unloop exit  then  r> drop
    loop  0 ;

3 run-frames
castle-b1-6 room!  5 run-frames
testing entering castle-b1-6 (its place): its door shows its part (group 22), its object 6 (the scenes' door) is hidden
T{ 0 22 door-group?  0 0 door-group?  0 23 door-group? -> -1 -1 0 }T
T{ 6 object-shown? -> 0 }T
testing the kit's words: an object shown (its +0 0), a part off (group $14 + 1 + part)
T{ [: 6 object-show  1 hide-part ;] in-room  6 object-shown?  0 22 door-group? -> -1 0 }T

testing a light scaled and back to the room's own
T{ 0 light@ fswap f0> -> -1 }T   fdrop
0 light@ fdrop fconstant own-level
T{ [: 0 0.5e light-brighter ;] in-room  0 light@ fdrop  own-level 0.5e f* f- fabs 0.001e f< -> -1 }T
T{ [: 0 light-back ;] in-room  0 light@ fdrop own-level f- fabs 0.001e f< -> -1 }T

testing the look: fog and the depth range on and off
T{ [: $40424C74 $4244489C 8.54e 25.61e fog  1e 1e 1e 48.6e depth-range ;] in-room  $1D look-on?  $1C look-on? -> -1 -1 }T
T{ [: fog-off  depth-range-off ;] in-room  $1D look-on?  $1C look-on? -> 0 0 }T
full-suite   \ (on: waits, scenes, walks - the full run's)

test-summary
