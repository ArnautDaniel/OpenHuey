\ Her legs on the floor (src/game/humanlegs.c; the original's HumanModel_BodyFrames and
\ HumanModel_PlantFeet, Fiona_AnimUpdate): her body set at the floor under the lower of her feet,
\ her legs bent to her feet on the floor; off her feet (down, seized, on a ladder) none of it.
\ Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/fiona/test_legs.fs
IN: test-legs
USING: tester engine actors room-names game debug keys fiona.state fiona.model fiona.anims fiona ;

: hers ( xt -- x )  game:fiona enter  execute  leave-actor ;
: model ( -- m )  [: her-model @ ;] hers ;
: her-y ( F: -- y )  [: her-at 4 + sf@ ;] hers ;
: height ( F: -- y )  model legs-height ;
: sunk ( F: -- d )  her-y height f- ;   \ (how far her body is below her place)
: bone-y ( b -- ) ( F: -- y )  model swap bone-pos fdrop fswap fdrop ;
: hold ( key -- )  true key-hold ;   : release ( key -- )  false key-hold ;

free-play  castle-1f-1 room!  20 run-frames
426 tri-center 0e tp-facing  30 run-frames   \ (the level floor at the top of its steps)
testing standing on level floor, her body at her place (eased in: 1/4 of the way a frame)
T{ sunk fabs 0.05e f< -> -1 }T
testing her ankles (bones 4, 8) as high above it as her model has them, both alike
T{ 4 bone-y her-y f-  0.5e f>  4 bone-y 8 bone-y f- fabs 0.01e f< -> -1 -1 }T
342 tri-center 180e tp-facing  10 run-frames
testing walking down the steps (a slope of the floor), her body down at the lower of her feet
key: S hold  20 run-frames
T{ sunk 0.5e f>  sunk 3e f< -> -1 -1 }T
testing her ankles: the higher foot's leg bent - its ankle higher above her body than the other's
T{ 4 bone-y 8 bone-y f- fabs 0.5e f> -> -1 }T
key: S release  30 run-frames
testing standing across the slope, one foot lower: her body below her place
342 tri-center 0e tp-facing  30 run-frames
T{ sunk 0.2e f>  sunk 3e f< -> -1 -1 }T

testing on her feet: the states that aren't (Fiona_AnimUpdate's +0x1AD5BC cleared)
T{ ' fiona.ladder:on-ladder feet-off-act?  ' fiona.seized:dragged feet-off-act?  ' fiona.fear:fallen feet-off-act? -> -1 -1 -1 }T
T{ ' fiona.moving:root-move feet-off-act?  ' fiona.hurt:spirit-fall feet-off-act? -> 0 0 }T
testing the points along her step on a slope: 11 back and 1 ahead; running 5 and 0.5
T{ [: walk-anim -1 play-table  feet-way ;] hers  -1e f=  11e f= -> -1 -1 }T
T{ [: run-anim -1 play-table  feet-way ;] hers  0.5e f=  5e f= -> -1 -1 }T
test-summary
