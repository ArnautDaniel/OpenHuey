\ Fiona's hanging parts (src/game/cloth.c; the original's spring sets): the slip's strips hang
\ from her hips and trail as she walks, her hair hangs from her head. Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/fiona/test_cloth.fs
IN: test-cloth
USING: tester engine actors game fiona.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: model ( -- m )  game:fiona enter  her-model @  leave-actor ;
: y-of ( b -- ) ( F: -- y )  model swap bone-pos fdrop fswap fdrop ;
: xz-apart ( b1 b2 -- ) ( F: -- d )   \ (how far apart across the floor)
    >r model swap bone-pos fswap fdrop  model r> bone-pos fswap fdrop
    frot f-  fsq  frot frot f-  fsq  f+ fsqrt ;

20 frames   \ (a new game: the slip she wakes in)
testing the slip: a strip hangs from her hip, a bone a unit (bones $B .. $E)
T{ $B y-of  $E y-of  f-  2.5e f> -> -1 }T
testing her hair hangs from her head (bones $39 .. $3C, under it)
T{ $39 y-of  $3C y-of  f> -> -1 }T
testing walking, the strip's end trails behind her hip
key: W true key-hold  20 frames
T{ $B $E xz-apart  0.5e f> -> -1 }T
key: W false key-hold  60 frames
testing standing again, it hangs back down
T{ $B $E xz-apart  1e f< -> -1 }T
test-summary
