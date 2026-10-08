\ The story, S5b: the rooms' effects as actors in the room's slots (docs/subsystems/effects.md).
\ Headless:  build/new-src-actors/hga --test new-src-actors/tests/story/test_effects.fs
IN: test-effects
USING: tester engine game-state actors messages room-names game debug flag-names story.state story.words effects ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: slot ( k -- id )  [: fx-id @ ;] in-room ;
: frame-of ( id -- n )  enter  0 p-frame pc @  leave-actor ;
: kind-of ( id -- n )  enter  kind @  leave-actor ;
: sparks# ( -- n )   \ (sparks alive)
    0  256 0 do  i alive? if  i actor-name s" sparks" compare 0= if  1+  then  then  loop ;

free-play  20 frames
castle-1f-8 room!  10 frames
testing castle-1f-8's torches: four fires (kind 1) in slots 0..3, each with its lasting sparks
T{ 0 slot alive?  3 slot alive?  4 slot -> -1 -1 -1 }T
T{ 0 slot kind-of  sparks# -> 1 4 }T
testing a fire's 16 frames run on
0 slot frame-of constant f0
T{ 1 frames  0 slot frame-of  f0 1+ 16 mod = -> -1 }T

testing commanded (the scripts' 0x87), a fire throws sparks that end by themselves
T{ 2 0 slot send effect-command  deliver  sparks# -> 8 }T
T{ 120 frames  sparks# -> 4 }T

testing a zone around an effect's place, an effect removed (its actor gone)
T{ [: 5 0 10 20 1 zone-at-effect  5 zone# l@  5 zone# 8 + sf@ ;] in-room  34.2e f- fabs 0.001e f< -> 1 -1 }T
0 slot constant torch0
T{ [: 0 effect-remove ;] in-room  torch0 alive?  0 slot -> 0 -1 }T

testing a flickering sprite: made in a slot, its 16 frames, resting between runs
[: 9 0e 10e 0e flicker-sprite ;] in-room  1 frames
T{ 9 slot alive?  9 slot kind-of -> -1 0 }T
: rests ( -- n )  9 slot enter  effects:rest @  leave-actor ;
: plays? ( -- flag )  false  150 0 do  game-tick  rests 0= or  loop ;   \ (rests 0.5 .. 1.2 s, then plays)
T{ rests 0>  plays? -> -1 -1 }T

testing leaving the room: its effects go with it
1 slot constant torch1  9 slot constant flick
front-garden-2 room!  10 frames
T{ torch1 alive?  flick alive?  sparks# -> 0 0 0 }T

test-summary
