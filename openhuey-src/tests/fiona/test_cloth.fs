\ Fiona's hanging parts (src/game/cloth.c; the original's spring sets): the slip's strips hang
\ from her hips and trail as she walks, her hair hangs from her head. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/fiona/test_cloth.fs
IN: test-cloth
USING: tester engine actors game fiona.state debug ;

: model ( -- m )  game:fiona enter  her-model @  leave-actor ;
: y-of ( b -- ) ( F: -- y )  model swap bone-pos fdrop fswap fdrop ;
: xz-apart ( b1 b2 -- ) ( F: -- d )   \ (how far apart across the floor)
    >r model swap bone-pos fswap fdrop  model r> bone-pos fswap fdrop
    frot f-  fsq  frot frot f-  fsq  f+ fsqrt ;

20 run-frames   \ (a new game: the slip she wakes in)
testing the slip: a strip hangs from her hip, a bone a unit (bones $B .. $E)
T{ $B y-of  $E y-of  f-  2.5e f> -> -1 }T
testing her hair hangs from her head (bones $39 .. $3C, under it)
T{ $39 y-of  $3C y-of  f> -> -1 }T
testing walking, the strip's end trails behind her hip
key: W true key-hold  20 run-frames
T{ $B $E xz-apart  0.5e f> -> -1 }T
key: W false key-hold  60 run-frames
testing standing again, it hangs back down
T{ $B $E xz-apart  1e f< -> -1 }T
testing put somewhere by the story mid-stride (Fiona_PlaceOn), it is settled afresh there, coming
\ in with her pose as her stride fades (1.1 apart by the third frame) - not flung on from where it
\ was (on out to 1.8, unsettled)
: put-by ( F: dx -- )   \ (a few units over, as the story's scripts place her)
    fiona body-pos {: F: dx x y z :}  x dx f+ y z 0e  f>cell >r f>cell >r f>cell >r f>cell r> r> r>  game:fiona send place ;
key: W true key-hold  20 run-frames  key: W false key-hold
T{ 5e put-by  deliver  3 run-frames  $B $E xz-apart  1.5e f< -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
test-summary
