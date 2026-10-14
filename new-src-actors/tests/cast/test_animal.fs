\ A story animal (cast.fs, Kind26_Behaviour): the rabbit (character $1A, kind 26) set grazing
\ (`animal-go`) - grazing, alert as one comes within 30, off to its spot as one comes within 20;
\ there it is done. Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/cast/test_animal.fs
IN: test-animal
USING: tester engine game-state actors messages common room-names game debug story.words story.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: c3 ( xt -- x )  game:cast3 enter  execute  leave-actor ;
: doing ( -- n )  [: cast.private:an-doing @ ;] c3 ;
: done? ( -- flag )  [: cast.private:mv-done @ ;] c3 ;
: put3 ( F: x y z yaw -- )  f>cell >r f>cell >r f>cell >r f>cell r> r> r>  game:cast3 send place ;
fvariable fx  fvariable fz  fvariable fy
1 seed !

free-play  front-garden-2 room!  20 frames
game:story send story-stop  deliver
game:fiona body-pos fz f! fy f! fx f!
$1A 3 $FF char-load  2 frames
fx f@ 60e f+ fy f@ fz f@ 0e put3  3 frames      \ (60 from her)
: go ( F: x z -- )  f>cell >r f>cell r>  2 -rot game:cast3 send animal-go  deliver ;
fx f@ 120e f+  fz f@ go  2 frames

testing grazing while nobody is near ($9000), not done
T{ doing  done?  [: cast.private:anim @ ;] c3 -> 0 0 $9000 }T
testing she comes within 30: alert (2); within 20: off it runs (3) to its spot, and is done
fx f@ 35e f+ fy f@ fz f@ 0e tp-facing
T{ [: doing 2 = ;] 30 soon -> -1 }T
fx f@ 45e f+ fy f@ fz f@ 0e tp-facing
T{ [: doing 3 = ;] 30 soon -> -1 }T
T{ [: done? ;] 900 soon  game:cast3 body-at sf@ fx f@ 120e f+ f- fabs 2e f< -> -1 -1 }T
test-summary
