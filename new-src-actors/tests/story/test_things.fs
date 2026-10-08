\ The story, S5a: the room's things - placed objects, the doors' parts, lights, the look
\ (docs/subsystems/story.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/story/test_things.fs
IN: test-things
USING: tester engine game-state actors messages room-names game debug flag-names story.state story.words story.strings ;

: frames ( n -- )  0 ?do  game-tick  loop ;
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

3 frames
castle-b1-6 room!  5 frames
testing entering castle-b1-6: its door shows its part (group 22), its object 6 is shown
T{ 0 22 door-group?  0 0 door-group?  0 23 door-group? -> -1 -1 0 }T
T{ 6 object-shown? -> -1 }T
testing the scripts' words: an object hidden, a door part off
T{ [: 6 0 object-show  1 0 $14 door-bits ;] in-room  6 object-shown?  0 22 door-group? -> 0 0 }T

testing a light scaled and back to the room's own
T{ 0 light@ fswap f0> -> -1 }T   fdrop
0 light@ fdrop fconstant own
T{ [: 1 0 0.5e light ;] in-room  0 light@ fdrop  own 0.5e f* f- fabs 0.001e f< -> -1 }T
T{ [: 0 0 1e light ;] in-room  0 light@ fdrop own f- fabs 0.001e f< -> -1 }T

testing the look: fog and the depth range on and off
T{ [: $40424C74 $4244489C 1 8.54e 25.61e fog  1e 1e 1e 48.6e depth-range ;] in-room  $1D look-on?  $1C look-on? -> -1 -1 }T
T{ [: 0 0 0 0e 0e fog  depth-range-off ;] in-room  $1D look-on?  $1C look-on? -> 0 0 }T

test-summary
