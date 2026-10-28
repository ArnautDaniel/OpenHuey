\ Fiona's costumes (fiona.fs wear): each costume's model, and her clothes worn as the story goes
\ (the variants by story flags $41 .. $95). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/fiona/test_costumes.fs
IN: test-costumes
USING: tester engine game-state actors messages room-names game facts fiona.state fiona.looks debug ;

: model ( -- id )  fiona enter  her-model @  leave-actor ;
: name ( -- addr len )  fiona enter  fiona:model-name  leave-actor  13 ;
: wearing? ( k -- flag )  fiona send costume  deliver  2 run-frames  model 0< 0= ;

2 run-frames
testing each costume loads: her clothes, the slip, and the others (2, 3, 6, 7, 8)
T{ 0 wearing?  1 wearing?  2 wearing?  3 wearing?  6 wearing?  7 wearing?  8 wearing? -> -1 -1 -1 -1 -1 -1 -1 }T
testing her clothes worn by the story: none yet, then variant 3 ($82), 6 ($95)
T{ 0 wearing? drop  name s" O_FIN/FIN_000" compare -> 0 }T
$82 story-flag-set
T{ 0 wearing? drop  name s" O_FIN/FIN_003" compare -> 0 }T
$95 story-flag-set
T{ 2 wearing? drop  name s" O_FIH/FIH_006" compare -> 0 }T
T{ 1 wearing? drop  name s" O_FIS/FIS_000" compare -> 0 }T   \ (the slip: none)
testing each costume's head and the bone below it (its model class's +0x8B0 / +0x8B4): her head, on
testing ... her neck, as high as a head is (13 to 16 over her feet)
: bones ( -- head neck )  fiona enter  fiona.looks:her-head-bones  leave-actor ;
: high ( bone -- ) ( F: -- h )  model swap bone-pos fdrop fswap fdrop  fiona body-pos fdrop fswap fdrop  f- ;
: headed? ( k -- flag )
    wearing? drop  10 run-frames  bones {: h n :}
    h high 13e f>  h high 16e f<  and  h high n high f>  and ;
T{ 0 wearing? drop bones  1 wearing? drop bones  2 wearing? drop bones  3 wearing? drop bones
   -> $21 $17 $35 $2B $21 $17 $1D $13 }T
T{ 6 wearing? drop bones  7 wearing? drop bones  8 wearing? drop bones -> $14 $A $16 $A $13 $B }T
full-suite   \ (on: waits, scenes, walks - the full run's)
T{ 0 headed?  1 headed?  2 headed?  3 headed?  6 headed?  7 headed?  8 headed? -> -1 -1 -1 -1 -1 -1 -1 }T

test-summary
