\ front-garden-4's place (scripts/story/places/front-garden-4.fs): the kennels - the cages, the
\ boarded door, the large door, the crate, the camomile, the Album Medallion, the rabbit, the
\ stalker shutting the door on her. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_front_garden_4.fs
IN: test-front-garden-4
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.front-garden-4 items.names ;
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
: room-afresh ( -- )  front-garden-4 room!  60 run-frames ;
: facing ( area F: px pz -- )   \ (in the area's middle, facing the point)
    {: area F: px pz :}  area area-middle drop {: F: x y z :}
    px x f-  pz z f-  fatan2 rad>deg f>s  x y z put ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )   \ (her blow's point there)
    blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)

free-play  10 run-frames  0 game:fiona send costume
testing the remarks: the cages (then the feathers), the boarded door, the large door - a look; once dressed, she tries it
found-file-202 story-flag-clear  room-afresh
T{ cages-area -140e 0e facing  said  cages-area -140e 0e facing  said -> cages-msg feathers-msg }T
T{ boarded-area 180 at  said -> boarded-msg }T
T{ large-door-area 0 at  said -> large-door-msg }T
found-file-202 story-flag-set
T{ large-door-area 0 at  said -> rusted-msg }T

testing the crate: whole; a blow in its place breaks it (once)
fg-4-breakable-broken story-flag-clear  room-afresh
T{ whole-part part?  broken-part part? -> -1 0 }T
T{ -120e 1e -13e blow-at  [: the-crate breaks? ;] in-place  [: the-crate breaks? ;] in-place -> -1 0 }T
T{ fg-4-breakable-broken story-flag?  whole-part part?  broken-part part? -> -1 0 -1 }T
[: body-point-off ;] as-her

testing the camomile: sparkling, picked up once
variable before  camomile items:items-of before !
fg-4-camomile story-flag-clear  room-afresh
T{ [: camomile-fx effect @ 0<> ;] in-place -> -1 }T
-202.89e 0e -6.76e picked
T{ fg-4-camomile story-flag?  camomile items:items-of  [: camomile-fx effect @ ;] in-place -> -1 before @ 1+ 0 }T

testing the Album Medallion: not there till Hewie finds it; found, sparkling, picked up
album-medallion items:items-of before !
fg-4-album-medallion-found story-flag-clear  fg-4-album-medallion story-flag-clear  room-afresh
T{ [: medallion-fx effect @ ;] in-place -> 0 }T
fg-4-album-medallion-found story-flag-set  room-afresh
T{ [: medallion-fx effect @ 0<> ;] in-place -> -1 }T
-283.8e 0e 15.5e picked
T{ fg-4-album-medallion story-flag?  album-medallion items:items-of -> -1 before @ 1+ }T

testing the rabbit: grazing by the gate; as she comes near it runs off past the cages - its call there - and is gone
room-afresh
[: ['] rabbit-runs start drop ;] in-place  10 run-frames
T{ [: rabbit-in? ;] in-place -> -1 }T
0 -255e 0e 43e put
T{ [: [: rabbit-in? 0= ;] in-place ;] 900 soon  [: rabbit-heard @ ;] in-place -> -1 -1 }T

testing the stalker, finding her crouched by the open large door: he looks about, backs off and shuts it - once
fg-4-flag1 story-flag-clear  room-afresh
stalkers-blind state-flag-set  summoner-on state-flag-clear   \ (the summoner kept from bringing him elsewhere)
big-door true s" doors" actor-named send door-opened  5 run-frames
variable his-tri  s" fiona" actor-named body-tri his-tri !   \ (he comes in where she came in)
by-the-door-area 0 at  fiona-half-hidden state-flag-set
debug:debilitas-out  2 run-frames  his-tri @ debug:debilitas-tri  5 run-frames
T{ [: his-scene @ 0<> ;] in-place -> -1 }T
T{ [: fg-4-flag1 story-flag? ;] 1200 soon  0 exit-open? -> -1 0 }T
testing ... she comes out of hiding mid-way: his scene over
fg-4-flag1 story-flag-clear  debug:debilitas-out  room-afresh
big-door true s" doors" actor-named send door-opened  5 run-frames
by-the-door-area 0 at  fiona-half-hidden state-flag-set
debug:debilitas-out  2 run-frames  his-tri @ debug:debilitas-tri  20 run-frames
fiona-half-hidden state-flag-clear  5 run-frames
T{ [: his-scene @ ;] in-place  fg-4-flag1 story-flag? -> 0 0 }T
debug:debilitas-out
test-summary
