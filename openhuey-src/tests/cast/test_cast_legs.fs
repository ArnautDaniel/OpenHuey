\ The story's human figures' feet on the floor (cast.fs fit-legs, src/game/humanlegs.c; the
\ original's Pursuer_ModelUpdate for slots 3..5: the model's +0x40 with its stance's points - none
\ set for these kinds, 0 0 - and +0x4C): a Fiona double (kind 13, EventHumanModel) on the castle's
\ steps has her feet planted - put where the floor is under each - but not while a script walks
\ her through blocked floor; the plain figures' models have no legs to fit. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/cast/test_cast_legs.fs
IN: test-cast-legs
USING: tester engine game-state actors messages room-names game debug story.words story.state ;

: model3 ( -- m )  [: cast.private:model @ ;] game:cast3 enter execute leave-actor ;
: put3 ( F: x y z yaw -- )  f>cell >r f>cell >r f>cell >r f>cell r> r> r>  game:cast3 send place ;
: foot-y ( bone -- ) ( F: -- y )  model3 swap bone-pos fdrop fswap fdrop ;
: feet ( F: -- right left )  5 foot-y  9 foot-y ;
fvariable r1  fvariable l1

free-play  castle-1f-1 room!  20 run-frames
game:story send story-stop  deliver
testing the kinds' legs: Lorenzo's hanging figure's left from $12, Fiona's doubles 2 and 6, a plain figure none
T{ 9 cast.private:kind-legs  13 cast.private:kind-legs  18 cast.private:kind-legs  28 cast.private:kind-legs
   -> 2 $12 2 6 2 6 -1 -1 }T
testing a Fiona double on the steps, facing up them: her feet planted
13 3 $FF char-load  2 run-frames
439 tri-center  0e put3  30 run-frames
feet l1 f! r1 f!
testing ... and while a script walks her through blocked floor, not: her feet where her pose has them
-1 game:cast3 send scripted  -1 game:cast3 send noclip  deliver  10 run-frames
T{ feet  l1 f@ f- fabs  fswap r1 f@ f- fabs  fmax 0.2e f> -> -1 }T
testing let go again, planted as before
0 game:cast3 send scripted  0 game:cast3 send noclip  deliver  10 run-frames
T{ feet  l1 f@ f- fabs  fswap r1 f@ f- fabs  fmax 0.01e f< -> -1 }T
test-summary
