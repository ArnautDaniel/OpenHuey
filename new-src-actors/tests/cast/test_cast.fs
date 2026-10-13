\ The event characters (docs/subsystems/cast.md): the story's figures in slots 3..5 - loaded by
\ the scripts' 0x12 / 0xB9, found by their id, placed, moved by the story, out of the scene, one
\ wearing another's model (0xBA), removed. Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/cast/test_cast.fs
IN: test-cast
USING: tester engine game-state actors messages room-names game debug story.words story.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: c3 ( xt -- x )  game:cast3 enter  execute  leave-actor ;
: c4 ( xt -- x )  game:cast4 enter  execute  leave-actor ;
: model3 ( -- m )  [: cast.private:model @ ;] c3 ;
: anim3 ( -- a )  [: cast.private:anim @ ;] c3 ;
\ (floats f>cell, as the story sends them)
: put3 ( F: x y z yaw -- )  f>cell >r f>cell >r f>cell >r f>cell r> r> r>  game:cast3 send place ;
: move3 ( kind a b F: x y z yaw -- )
    f>cell >r f>cell >r f>cell >r f>cell r> r> r>  game:cast3 send scripted-move ;
fvariable fx  fvariable fz

free-play  front-garden-2 room!  20 frames
game:story send story-stop  deliver
testing the scripts' 0x12: character $1C (its kind's figure O_HMA) into slot 3, not placed
$1C 3 $FF char-load  2 frames
T{ $1C char-slot  3 $1C char-loaded?  3 char-actor  $1C cast-model model3 = -> 3 -1 -1 -1 }T
T{ model3 model-file s" O_HMA/HMA_000" compare -> 0 }T
testing placed by the story: in the room, seen
game:fiona body-pos fz f! fdrop fx f!
fx f@ 10e f+  game:fiona body-pos fdrop fswap fdrop  fz f@  0e put3  3 frames
T{ 3 char-actor game:cast3 =  game:cast3 body-room room-id =  model3 actor act.visible l@ -> -1 -1 1 }T
testing the story's moves: an animation (7) - done at once
7 0 -1  0e 0e 0e 0e move3  3 frames
T{ anim3  [: cast.private:mv-done @ ;] c3 -> 0 -1 }T
testing walking to a spot (5): there, facing the move's way, done
5 -1 -1  fx f@ 10e f+  game:fiona body-pos fdrop fswap fdrop  fz f@ 8e f+  0e move3  1 frames
T{ [: cast.private:mv-done @ ;] c3 -> 0 }T
T{ [: [: cast.private:mv-done @ ;] c3 ;] 300 soon  game:cast3 body-pos frot fdrop fswap fdrop  fz f@ 8e f+ f- fabs 1e f< -> -1 -1 }T

testing the scripts' 0x14: out of the scene - gone from the room, still loaded
$1C char-done  2 frames
T{ 3 char-actor  3 $1C char-loaded? -> -1 -1 }T

testing 0xB9 / 0xBA: the crow ($F) into slot 4, then it wears slot 3's figure
$F 4 char-load-2  2 frames
T{ 4 $F char-loaded?  [: cast.private:model @ model-file s" O_CRW/CRW_000" compare ;] c4 -> -1 0 }T
4 3 char-hand-over  2 frames
T{ [: cast.private:model @ model-file s" O_HMA/HMA_000" compare  cast.private:who @ ;] c4 -> 0 $F }T

testing the scripts' 0x40: removed - its slot empty, its figure let go
3 0 char-remove  4 0 char-remove  2 frames
T{ 3 $1C char-loaded?  $1C char-slot  $1C cast-model  model3 -> 0 -1 -1 -1 }T
test-summary
