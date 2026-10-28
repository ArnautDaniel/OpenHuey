\ castle-1f-10's place (scripts/story/places/castle-1f-10.fs): the doll's room - the remarks, the
\ beef jerky, the stalker with his doll (the scene, the Smudged Key, the dance), his chase scene,
\ the Bisque Doll. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_1f_10.fs
IN: test-castle-1f-10
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
 floor-flags fiona.state story.kit story.places.castle-1f-10 items.names ;
full-suite   \ (on: waits, scenes, walks - the full run's)

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: room-afresh ( -- )  castle-1f-10 room!  60 run-frames ;
: facing ( area F: px pz -- )   \ (in the area's middle, facing the point)
    {: area F: px pz :}  area area-middle drop {: F: x y z :}
    px x f-  pz z f-  fatan2 rad>deg f>s  x y z put ;
: facing-from ( area F: x z -- )   \ (there, facing the area's middle)
    {: area F: x z :}  area area-middle drop {: F: mx my mz :}
    mx x f-  mz z f-  fatan2 rad>deg f>s  x 0e z put ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )   \ (her blow's point there)
    blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)

: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing the remarks: the doll on show, the bed, the chest
found-file-20b story-flag-clear  c1f-10-flag3 story-flag-clear  room-afresh
T{ 0 6.04e 0e 17e put  said -> doll-on-show-msg }T   \ (the areas' middles are on the furniture: standing spots in them)
T{ -45 -22e 0e 6e put  said -> not-lying-msg }T
T{ 45 11e 0e 14e put  said -> out-of-sight-msg }T

testing the beef jerky on the shelf: reached, taken
variable before  beef-jerky items:items-of before !
c1f-10-beef-jerky story-flag-clear  room-afresh
0 11e 0e -54e put  circle press  300 read-on
T{ c1f-10-beef-jerky story-flag?  beef-jerky items:items-of -> -1 before @ 1+ }T

testing the stalker with his doll (the file found): sitting there; beside him, the scene - the Smudged Key; then his doll's dance
found-file-20b story-flag-set  c1f-10-flag1 story-flag-clear  c1f-10-flag2 story-flag-clear  room-afresh
castle-1f-2 room!  30 run-frames  [: 2 3 0 figure-in-by ;] in-place  10 run-frames   \ (castle-1f-2 loads his figure on her way)
castle-1f-10 room!  60 run-frames
T{ [: him here?  he-sits? ;] in-place -> -1 -1 }T
smudged-key items:items-of before !
: beside-him ( -- )  [: him ;] in-place body-pos {: F: x y z :}  -90 x 12e f+ y z put ;
beside-him  circle press  900 play-on
T{ c1f-10-flag2 story-flag?  smudged-key items:items-of -> -1 before @ 1+ }T
beside-him  circle press  200 play-on
T{ [: doll-danced @ ;] in-place -> -1 }T
found-file-20b story-flag-clear  room-afresh

testing the Bisque Doll on show (she has it from castle-2f-2, none lying about, she hasn't it): taken
got-bisque-doll story-flag-set  room-afresh
T{ bisque-part part?  [: doll-to-take @ ;] in-place -> -1 -1 }T
bisque-doll items:items-of before !
: at-bisque ( -- )  bisque-area -2.94e 11.39e facing-from ;
at-bisque  circle press  300 read-on
T{ bisque-doll items:items-of  bisque-part part? -> before @ 1+ 0 }T
test-summary
