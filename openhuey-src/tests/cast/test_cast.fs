\ The event characters (docs/subsystems/cast.md): the story's figures in slots 3..5 - loaded by
\ the scripts' 0x12 / 0xB9, found by their id, placed, moved by the story, out of the scene, one
\ wearing another's model (0xBA), removed. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/cast/test_cast.fs
IN: test-cast
USING: tester engine game-state actors messages room-names game debug story.words story.state vectors ;

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
testing 0x12 into a slot taken: nothing (CharLoad_EventChar)
$F 3 $FF char-load  2 frames
T{ 3 $1C char-loaded?  3 $F char-loaded?  model3 model-file s" O_HMA/HMA_000" compare -> -1 0 0 }T
testing placed by the story: in the room, seen
game:fiona body-pos fz f! fdrop fx f!
fx f@ 10e f+  game:fiona body-pos fdrop fswap fdrop  fz f@  0e put3  3 frames
T{ 3 char-actor game:cast3 =  game:cast3 body-room room-id =  model3 actor act.visible l@ -> -1 -1 1 }T
testing the story's moves: an animation (7) - done at once
7 0 -1  0e 0e 0e 0e move3  3 frames
T{ anim3  [: cast.private:mv-done @ ;] c3 -> 0 -1 }T
: yaw3 ( -- deg )  game:cast3 body-yaw 180e f* pi f/ fround f>s ;
testing blended (8): a faded in over b frames, done at once
7 $9000 -1  0e 0e 0e 0e move3  2 frames   \ (playing something first: a fade needs one to fade from)
8 $9001 12  0e 0e 0e 0e move3  1 frames
T{ anim3  [: cast.private:mv-done @  cast.private:faded-in? ;] c3 -> $9001 -1 0 }T
testing an idle (16): its stand (0) over b frames - b no animation (this figure has no 0: as it was)
16 0 $9002  0e 0e 0e 0e move3  1 frames
T{ anim3  [: cast.private:mv-done @ ;] c3 -> $9001 -1 }T
testing turning to a heading (15): 4 degrees a frame; turned, standing, done once that has faded in
[: 0e body-turn ;] c3
15 0 -1  0e 0e 0e  pi f2/ move3  3 frames
T{ yaw3 dup 0> swap 13 <  [: cast.private:mv-done @ ;] c3 -> -1 -1 0 }T
T{ [: [: cast.private:mv-done @ ;] c3 ;] 100 soon  yaw3 -> -1 90 }T
testing the walks' animations (Pursuer_ScriptMove): 5 / 6 b if one of the $2xx, else $200; 10 / 11 $201
T{ [: walk-move cast.private:mv-kind !  $204 cast.private:mv-b !  cast.private:walk-anim-for
      -1 cast.private:mv-b !  cast.private:walk-anim-for
      run-path-move cast.private:mv-kind !  $204 cast.private:mv-b !  cast.private:walk-anim-for ;] c3 -> $204 $200 $201 }T

testing walking (5, 6, 11: Pursuer_WalkToGoal / CloseInGoal / StateArrive - as the stalkers'): the golem (8: its
testing ... walks $200 / $201) along its way to the spot - there turned to the move's heading at 4 degrees a frame,
testing ... then standing, done once its stance has faded in
: c5 ( xt -- x )  game:cast5 enter  execute  leave-actor ;
: put5 ( F: x y z yaw -- )  f>cell >r f>cell >r f>cell >r f>cell r> r> r>  game:cast5 send place ;
: move5 ( kind a b F: x y z yaw -- )
    f>cell >r f>cell >r f>cell >r f>cell r> r> r>  game:cast5 send scripted-move ;
: done5? ( -- flag )  [: cast.private:mv-done @ ;] c5 ;
: anim5 ( -- a )  [: cast.private:anim @ ;] c5 ;
: yaw5 ( -- deg )  game:cast5 body-yaw 180e f* pi f/ fround f>s ;
variable spot-tri  create spot /vec allot
8 5 $FF char-load  2 frames
fx f@ 10e f+  game:fiona body-pos fdrop fswap fdrop  fz f@  0e put5  3 frames
fx f@ 10e f+  game:fiona body-pos fdrop fswap fdrop  fz f@ 20e f+  spot vec!  spot v-tri spot-tri !
5 spot-tri @ -1  spot vec@  pi f2/ move5  5 frames
T{ done5?  anim5 $FF00 and  dup $200 = swap $400 = or -> 0 -1 }T
T{ ' done5? 600 soon  game:cast5 body-at spot vec-dist-xz 1e f<  yaw5 -> -1 -1 90 }T
testing ... to a triangle slowly (11): done as soon as it is on it
11 game:fiona body-tri -1  0e 0e 0e 0e move5  5 frames
T{ done5?  anim5 $FF00 and dup $201 = swap $400 = or -> 0 -1 }T
T{ ' done5? 600 soon  game:cast5 body-tri game:fiona body-tri = -> -1 -1 }T
5 slot-clear  2 frames

testing the scripts' 0x14: out of the scene - gone from the room, still loaded
$1C char-done  2 frames
T{ 3 char-actor  3 $1C char-loaded? -> -1 -1 }T

testing 0xB9 / 0xBA: the crow ($F) into slot 4, then it wears slot 3's figure
$F 4 figure-load  2 frames
T{ 4 $F char-loaded?  [: cast.private:model @ model-file s" O_CRW/CRW_000" compare ;] c4 -> -1 0 }T
testing the crow's own: its motion table (its fades); in $9000 / $9001 part 5 shown, 3 and 4 hidden
: model4 ( -- m )  [: cast.private:model @ ;] c4 ;
: part? ( n -- flag )  1+ 1 swap lshift  model4 actor act.parts-shown l@ and 0<> ;
T{ model4 $9000 motion-entry -> 10 0 1 }T   \ (a 10-frame fade; the plain model has none: 0 0 0)
T{ [: $9000 cast.private:play-now ;] c4  2 frames  5 part?  3 part?  4 part? -> -1 0 0 }T
T{ [: $9002 cast.private:play-now ;] c4  2 frames  5 part?  3 part?  4 part? -> 0 -1 -1 }T
testing the scripts' 0x9D on the crow: its take-off ($9002, looping, faded in over 0 frames) at once - not over its table's 10
$9002 1 0 game:cast4 send hold-anim  deliver  1 frames
T{ [: cast.private:anim @ ;] c4  model4 actor act.fade sf@ 0e f<=  model4 actor act.loop l@ -> $9002 -1 1 }T
4 3 char-hand-over  2 frames
T{ [: cast.private:model @ model-file s" O_HMA/HMA_000" compare  cast.private:who @ ;] c4 -> 0 $F }T

testing the scripts' 0x40: removed - its slot empty, its figure let go
3 0 char-remove  4 0 char-remove  2 frames
T{ 3 $1C char-loaded?  $1C char-slot  $1C cast-model  model3 -> 0 -1 -1 -1 }T

testing Lorenzo's hanging figure (kind 9) on the hard setting: its own textures (LRM_000H), its motion table
$AF story-flag-set   \ (a hard game)
9 5 figure-load  2 frames
T{ 5 9 char-loaded?  9 cast-model 0< 0=  9 cast-model 0 motion-entry 2drop 0<> -> -1 -1 -1 }T
5 0 char-remove  $AF story-flag-clear  2 frames
test-summary
