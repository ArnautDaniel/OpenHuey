\ old-mansion-2f-3's place: the decomposer (its plaque and file; the Handful of Ash changed,
\ anything else nothing); the crawl space; the notes and their file; the weak floor (its remark;
\ giving way under the stalker; fallen, its remark); the gear and the Album Medallion inside. Headless:
\   build/openhuey --test tests/places/test_old_mansion_2f_3.fs
IN: test-old-mansion-2f-3
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-2f-3 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )   \ (on the floor, on a triangle she walks - else the nearest the mesh has)
    on-floor s>f tp-facing  [: her-at $28020018 v-tri-in  dup 0< if  drop her-at vec@ nav-tri  then  her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: shown-soon ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
: said ( -- msg )
    circle press  shown-soon
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
create spot 12 allot
: by-door ( exit -- )  0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames ;
variable before
: room-afresh ( -- )  old-mansion-2f-3 room!  60 run-frames ;
: toward ( area F: px pz -- )   \ (at the area's middle, facing the point)
    {: a F: px pz :}  a area-middle drop {: F: x y z :}  px x f-  pz z f-  fatan2 rad>deg f>s  x y z put ;
: facing-middle ( area -- )   \ (in it, a little off its middle, facing it)
    area-middle drop {: F: x y z :}  90  x 3e f-  y z put ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
om2f-3-flag1 story-flag-clear  om2f-2-flag1 story-flag-clear  room-afresh
testing the decomposer: its plaque, then her wonder; its file
T{ 4 0 at  said  decomposer-file filed? -> plaque-msg -1 }T
T{ 4 0 at  said -> decomposer-msg }T
testing the Handful of Ash put in: the Prima Materia out
handful-of-ash 1 items send give-item  10 run-frames  prima-materia items:items-of before !
0 transforms -1 0 broadcast item-event  600 play-on
T{ handful-of-ash items:items-of  prima-materia items:items-of -> 0 before @ 1+ }T
testing ... anything else: nothing comes of it
silver-ore 1 items send give-item  10 run-frames  silver-ore items:items-of before !
0 nothing-from-it 0 silver-ore broadcast item-event  600 play-on
T{ silver-ore items:items-of -> before @ }T
testing the crawl space: the draught, then too tight; the furnace lit
T{ 5 -45 at  said -> draught-msg }T
T{ 5 -45 at  said -> too-tight-msg }T
testing the notes on the dog: their file
T{ 6 90 at  said  notes-file filed? -> notes-msg -1 }T
300 read-on
testing the weak floor: her remark
T{ 0 35e 0e 0e put  said -> weak-floor-msg }T
testing the gear: its remarks; the Album Medallion inside, then nothing
T{ 7 facing-middle  said -> gear-msg }T
om2f-3-album-medallion story-flag-clear  album-medallion items:items-of before !
T{ 8 5e 37e toward  said -> inside-msg }T
300 read-on
T{ om2f-3-album-medallion story-flag?  album-medallion items:items-of -> -1 before @ 1+ }T
testing the weak floor giving way under the stalker: gone, its remark
T{ [: gives-way ;] in-place  [: doing-busy? 0= ;] 600 soon drop  om2f-3-flag1 story-flag?  0 part? -> -1 0 }T
room-afresh
T{ 3 facing-middle  said -> fell-out-msg }T
om2f-2-flag1 story-flag-set  room-afresh
T{ 5 -45 at  said -> furnace-msg }T
test-summary
