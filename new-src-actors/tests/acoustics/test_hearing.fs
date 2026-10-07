\ acoustics: who hears which noise (docs/subsystems/acoustics.md, rules 1-6). Run headless:
\   build/new-src-actors/hga --test new-src-actors/tests/acoustics/test_hearing.fs
IN: test-hearing
USING: tester engine game-state actors messages acoustics game ;

\ ---- a listener that writes down what it was told ----
state: ear-state  cell field dummy  end-state
variable got   \ the source it heard last frame (-1 nothing), and the loudness
variable got-loud
behaviour ear
  on heard ( loud room tri door source -- )  got !  2drop drop got-loud ! ;
  on heard-nothing ( -- )  -1 got !  0 got-loud ! ;
end-behaviour

$13 room
ear ear-state s" ear" spawn constant ear
deliver
: put-ear ( tri -- )   \ stand it on a triangle of the played room
    ear enter  dup tri-center  room-id swap body-place  2e 15e body-size  leave ;
: ear-listens ( threshold source -- )  ear enter  acoustics send listen  leave  deliver ;
: make ( loud room tri door source -- )  acoustics send noise ;
: outcome ( -- source loud )  actors-frame  got @  got-loud @ ;

100 put-ear
0 stalker-noise ear-listens

testing a noise in the room: weakened by a tenth of the distance
T{ $40 $13 100 -1 fiona-noise make  outcome -> fiona-noise $40 }T          \ (its own triangle: 0 away)
T{ outcome -> -1 0 }T                                                     \ forgotten after the frame
fvariable ex  fvariable ey  fvariable ez
: dist ( tri -- n )   \ the weakening between triangle `tri` and the ear's
    ear body-pos ez f! ey f! ex f!
    tri-center  ez f@ f- fsq  fswap ey f@ f- fabs 3e f*  frot ex f@ f- fsq  frot f+ fsqrt  f+
    0.1e f* f>s ;
create far-tri 1 ,
: find-far ( -- )   \ a triangle 30..60 units of weakening away
    nav-tris 0 do  i dist 30 60 within if  i far-tri !  leave  then  loop ;
find-far
T{ far-tri @ dist 30 61 within -> -1 }T
T{ far-tri @ dist 1+  $13 far-tri @ -1 fiona-noise make  outcome nip -> far-tri @ dist 1+ }T   \ just loud enough
T{ far-tri @ dist     $13 far-tri @ -1 fiona-noise make  outcome -> -1 0 }T                 \ not quite

testing the order of the sources decides, not the loudness
T{ $20 $13 100 -1 fiona-noise make  $80 $13 100 -1 hewie-noise make  outcome -> fiona-noise $20 }T
testing its own source isn't heard
T{ $80 $13 100 -1 stalker-noise make  outcome -> -1 0 }T
T{ $80 $13 100 -1 stalker-noise make  $10 $13 100 -1 world-noise make  outcome -> world-noise $10 }T
testing the loudest of a source this frame
T{ $20 $13 100 -1 fiona-noise make  $30 $13 100 -1 fiona-noise make  $10 $13 100 -1 fiona-noise make  outcome
   -> fiona-noise $30 }T
testing nothing: no loudness, no room
T{ 0 $13 100 -1 fiona-noise make  $40 -1 100 -1 hewie-noise make  outcome -> -1 0 }T

testing the setting makes quiet noises quieter
T{ 2 acoustics send noise-setting  $40 $13 100 -1 fiona-noise make  outcome -> fiona-noise $40 }T   \ $40-$3F = 1
T{ 3 acoustics send noise-setting  $40 $13 100 -1 fiona-noise make  outcome -> -1 0 }T
T{ $80 $13 100 -1 fiona-noise make  outcome -> fiona-noise $80 }T              \ (not a quiet one)
T{ 0 acoustics send noise-setting  deliver -> }T

testing the threshold: louder than it
T{ 0 stalker-noise ear-listens  $10 stalker-noise ear-listens  $10 $13 100 -1 fiona-noise make  outcome -> -1 0 }T
T{ $11 $13 100 -1 fiona-noise make  outcome -> fiona-noise $11 }T
T{ 0 stalker-noise ear-listens -> }T

\ ---- next door: room $13's exit 0 has a real door (to $32), exit 2 a doorway (to $2A) ----
: door0 ( -- d )  $13 0 room-exit-door ;
: next0 ( -- room )  $13 0 room-exit-leads drop ;
: door2 ( -- d )  $13 2 room-exit-door ;
: shut ( d -- )  4 * progress pr.doors + dup l@ 2 invert and swap l! ;
: open ( d -- )  4 * progress pr.doors + dup l@ 2 or swap l! ;
testing through one door: 32 a door, below $41 the door must be open
door0 shut
T{ $13 next0 1 2 route -> 1 }T
T{ $1F next0 0 -1 fiona-noise make  outcome -> -1 0 }T
T{ $40 next0 0 -1 fiona-noise make  outcome -> -1 0 }T            \ door shut
T{ $41 next0 0 -1 fiona-noise make  outcome -> fiona-noise $41 }T
door0 open
T{ $20 next0 0 -1 fiona-noise make  outcome -> fiona-noise $20 }T   \ door open: 32 is enough
door0 shut
testing a doorway is always open
T{ $20 $2A 0 -1 fiona-noise make  outcome -> fiona-noise $20 }T

testing beyond two doors: only $60 and up
variable far-room  -1 far-room !
: find-far-room ( -- )
    room-count 0 do  $13 i 1 2 route -1 =  $13 i 1 -1 route 0> and if  i far-room !  leave  then  loop ;
find-far-room
T{ far-room @ 0< -> 0 }T
T{ $5F far-room @ 0 -1 fiona-noise make  outcome -> -1 0 }T
T{ $60 far-room @ 0 -1 fiona-noise make  outcome -> fiona-noise $60 }T

testing a door's noise reaches the room it is in
T{ $80 $2A -1 door2 hewie-noise make  outcome -> hewie-noise $80 }T

testing a listener with no body hears nothing (and no message)
ear enter body-off leave
T{ $80 $13 100 -1 fiona-noise make  -5 got !  0 got-loud !  outcome -> -5 0 }T

test-summary
