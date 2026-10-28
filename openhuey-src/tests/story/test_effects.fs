\ The story, S5b: a place's effects as actors in its slots (the kit's; docs/subsystems/effects.md).
\ Headless:  build/openhuey-src/openhuey --test openhuey-src/tests/story/test_effects.fs
IN: test-effects
USING: tester engine game-state actors messages room-names game debug flag-names effects story.kit ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: slot ( k -- id )  [: effect @ ;] in-room ;
: system ( id -- p )  enter  effects:particles @  leave-actor ;   \ (its particle system)
: kind-of ( id -- n )  system particles-info 2drop ;
: frame-of ( id -- n )  system particles-info drop nip ;
: rests ( -- n )  9 slot system particles-info nip nip ;
: sparks# ( -- n )   \ (the torches' sparks going: the engine's, theirs)
    0  4 0 do  i slot dup 0<>  over alive? and if  system particles-children +  else  drop  then  loop ;   \ (0: an empty slot)

free-play  20 run-frames
castle-1f-8 room!  10 run-frames
testing four torches (castle-1f-8's, its place lights them): fires (kind 1) in slots 0..3, each with its lasting sparks
T{ 0 slot alive?  3 slot alive?  4 slot -> -1 -1 0 }T
T{ 0 slot kind-of  sparks# -> 1 4 }T
testing a fire's 16 frames run on
0 slot frame-of constant f0
T{ 1 run-frames  0 slot frame-of  f0 1+ 16 mod = -> -1 }T

testing commanded (the scripts' 0x87), a fire throws sparks that end by themselves
T{ 2 0 slot send effect-command  deliver  sparks# -> 8 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
T{ 120 run-frames  sparks# -> 4 }T

testing an effect removed (its actor gone)
0 slot constant torch0
T{ [: 0 effect-off ;] in-room  1 run-frames  torch0 alive?  0 slot -> 0 0 }T

testing a flickering sprite: made in a slot, its 16 frames, resting between runs
[: 9 0e 10e 0e flicker-at ;] in-room  1 run-frames
T{ 9 slot alive?  9 slot kind-of -> -1 0 }T
: plays? ( -- flag )  false  150 0 do  game-tick  rests 0= or  loop ;   \ (rests 0.5 .. 1.2 s, then plays)
T{ rests 0>  plays? -> -1 -1 }T

testing a thing wandering about its place (0xA9), in a slot; it doesn't end by itself
[: 3  0e 10e 0e  4e 4e  0e 0e 0e  wanderer-at ;] in-room  60 run-frames
: wanderer-id ( -- id )  3 slot ;
T{ wanderer-id alive?  wanderer-id system particles-alive? -> -1 -1 }T
wanderer-id constant the-wanderer

testing leaving the room: its effects go with it
1 slot constant torch1  9 slot constant flick
front-garden-2 room!  10 run-frames
T{ torch1 alive?  flick alive?  sparks#  the-wanderer alive? -> 0 0 0 0 }T

test-summary
