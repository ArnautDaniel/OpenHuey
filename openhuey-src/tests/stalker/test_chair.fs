\ Lorenzo in his chair (src/game/chair.c; LorenzoModel_BodyFrames / LorenzoModel_AdjustBone): on
\ the castle's steps his body tilts with the floor - its front up or down the slope, as the floor 8
\ ahead and 8 behind lie - and on the level floor it doesn't. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/stalker/test_chair.fs
IN: test-stalker-chair
USING: story.words tester engine game-state actors messages common room-names game debug facts flag-names stalker.state stalker ;

: lorenzo-id ( -- id )  s" lorenzo" actor-named ;
: his ( xt -- x )  lorenzo-id enter  execute  leave-actor ;
: model ( -- id )  [: my-model @ ;] his ;
: facing ( yaw -- )   \ (turned there where he stands)
    {: y :}  lorenzo-id body-pos  y s>f deg>rad  f>cell >r f>cell >r f>cell >r f>cell r> r> r>  lorenzo-id send place  deliver  4 run-frames ;
\ the most his body's and his chair front's forward axes rise (a unit's), of four headings
fvariable most-body  fvariable most-front
: tilts ( -- ) ( F: -- body front )
    0e most-body f!  0e most-front f!
    4 0 do  i 90 * facing  model chair-rises  fabs most-front f@ fmax most-front f!  fabs most-body f@ fmax most-body f!  loop
    most-body f@ most-front f@ ;
: tilted? ( F: body front -- ) ( -- flag )  0.1e f>  0.1e f> and ;
: level? ( F: body front -- ) ( -- flag )  0.001e f<  0.001e f< and ;

free-play  castle-1f-1 room!  20 run-frames
stalkers-blind state-flag-set  stalkers-stay state-flag-set
testing his model is in the chair (O_LRO)
$B partner-load  deliver  2 run-frames
T{ [: chaired @ ;] his -> -1 }T
testing on the steps his body and his chair's front tilt with the floor (facing up or down them)
room-id 342 lorenzo-id send stalker-in  deliver  40 run-frames
T{ tilts tilted? -> -1 }T
testing on a level floor (the bedroom's, where Fiona stands) neither does
castle-2f-5 room!  20 run-frames
room-id s" fiona" actor-named body-tri lorenzo-id send stalker-in  deliver  40 run-frames
T{ tilts level? -> -1 }T
test-summary
