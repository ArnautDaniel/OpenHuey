\ The story's human figures' heads (cast.fs look; the original's Pursuer_Think: a scripted look
\ - the story's moves 12 / 13 - turns the head toward whom the moves last named, else it eases
\ back; HumanModel_AdjustBone turns the head and the bone below it as the model is posed): a
\ Fiona double (kind 13, EventHumanModel, head $21) turns hers. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/cast/test_cast_head.fs
IN: test-cast-head
USING: tester engine game-state actors messages room-names game debug ;
: char-load ( id slot exit -- )  drop story.kit:figure-in ;   \ (a figure into a story slot: the kit's)

: c3 ( xt -- x )  game:cast3 enter  execute  leave-actor ;
: model3 ( -- m )  [: cast.private:model @ ;] c3 ;
: head-yaw3 ( F: -- yaw )  [: cast.private:head-yaw f@ ;] c3 ;
: put3 ( F: x y z yaw -- )  f>cell >r f>cell >r f>cell >r f>cell r> r> r>  game:cast3 send place ;
: move3 ( kind a b F: x y z yaw -- )
    f>cell >r f>cell >r f>cell >r f>cell r> r> r>  game:cast3 send scripted-move  deliver ;
\ the head's forward (its bone's z, level): x and z
: ahead ( F: -- x z )
    model3 $21 0e 0e 1e bone-point  model3 $21 bone-pos {: F: ax ay az hx hy hz :}  ax hx f-  az hz f- ;
fvariable px  fvariable py  fvariable pz

free-play  castle-1f-1 room!  20 run-frames
game:story send story-stop  deliver
testing the kinds' heads: Lorenzo's hanging figure $2B / $22, Fiona's doubles $21 / $17, a plain figure none
T{ 9 cast.private:kind-head  13 cast.private:kind-head  18 cast.private:kind-head  28 cast.private:kind-head
   -> $2B $22 $21 $17 $21 $17 -1 -1 }T
13 3 $FF char-load  2 run-frames
439 tri-center  pz f! py f! px f!
px f@ py f@ pz f@ 0e put3  10 run-frames
testing a double facing +z, no look: her head ahead
T{ ahead fswap fabs 0.01e f<  0.99e f> -> -1 -1 }T
testing a look at a point to her side (13): turned 8 degrees a frame toward it ...
look-point-move 0 -1  px f@ 20e f+  py f@ 16e f+  pz f@  0e move3  1 run-frames
T{ head-yaw3 8e deg>rad f- fabs 0.001e f< -> -1 }T
testing ... till her head faces it
20 run-frames
T{ head-yaw3 90e deg>rad f- fabs 0.001e f<  ahead fswap 0.99e f>  fabs 0.1e f< -> -1 -1 -1 }T
testing the look off (12 at nobody): eased back ahead
look-move -1 $FF  0e 0e 0e 0e move3  20 run-frames
T{ head-yaw3 f0=  ahead 0.99e f>  fabs 0.01e f< -> -1 -1 -1 }T
testing a look at Fiona (12 at slot 0): toward her
look-move game:fiona 0  0e 0e 0e 0e move3  40 run-frames
: to-fiona ( F: -- turn )  game:cast3 game:fiona body-at body-heading-to  game:cast3 body-yaw f-  angle-wrap ;
T{ head-yaw3 to-fiona f- fabs 0.001e f<  to-fiona fabs 0.2e f> -> -1 -1 }T
testing a look at a point after it: still at Fiona (the original's +0x100 left as the last look set it)
look-point-move 0 -1  px f@ 20e f+  py f@ 16e f+  pz f@  0e move3  40 run-frames
T{ head-yaw3 to-fiona f- fabs 0.001e f< -> -1 }T
testing a door's use naming exit 5 since: the point now
look-move -1 $FF  0e 0e 0e 0e move3  open-door-move 5 -1  0e 0e 0e 0e move3
look-point-move 0 -1  px f@ 20e f+  py f@ 16e f+  pz f@  0e move3  40 run-frames
T{ head-yaw3 90e deg>rad f- fabs 0.001e f< -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
test-summary
