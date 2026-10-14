\ Fiona's costumes (fiona.fs wear): each costume's model, and her clothes worn as the story goes
\ (the variants by story flags $41 .. $95). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/fiona/test_costumes.fs
IN: test-costumes
USING: tester engine game-state actors messages room-names game facts fiona.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: model ( -- id )  fiona enter  her-model @  leave-actor ;
: name ( -- addr len )  fiona enter  fiona:model-name  leave-actor  13 ;
: wearing? ( k -- flag )  fiona send costume  deliver  2 frames  model 0< 0= ;

2 frames
testing each costume loads: her clothes, the slip, and the others (2, 3, 6, 7, 8)
T{ 0 wearing?  1 wearing?  2 wearing?  3 wearing?  6 wearing?  7 wearing?  8 wearing? -> -1 -1 -1 -1 -1 -1 -1 }T
testing her clothes worn by the story: none yet, then variant 3 ($82), 6 ($95)
T{ 0 wearing? drop  name s" O_FIN/FIN_000" compare -> 0 }T
$82 story-flag-set
T{ 0 wearing? drop  name s" O_FIN/FIN_003" compare -> 0 }T
$95 story-flag-set
T{ 2 wearing? drop  name s" O_FIH/FIH_006" compare -> 0 }T
T{ 1 wearing? drop  name s" O_FIS/FIS_000" compare -> 0 }T   \ (the slip: none)

test-summary
