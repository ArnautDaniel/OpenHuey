\ old-mansion-1f-7's place: the extractor - its plaque, its file; the Prima Materia changed, anything
\ else nothing; the high windows; the device; the Album Medallion; brought in the first time by the
\ forced way, the scene and the chapter's turn; the replay. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_7.fs
IN: test-old-mansion-1f-7
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-7 ;
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
: room-afresh ( -- )  old-mansion-1f-7 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
over-music-c story-flag-set
testing the extractor: its plaque, then her wonder; its file
room-afresh
T{ 8 -45 at  said  extractor-file filed? -> plaque-msg -1 }T
T{ 8 -45 at  said -> extractor-msg }T
testing the Prima Materia put in: Sulfuric Ore out
prima-materia 1 items send give-item  sulfuric-ore items:items-of before !  10 run-frames
0 changes-it -1 0 broadcast item-event  600 play-on
T{ prima-materia items:items-of  sulfuric-ore items:items-of -> 0 before @ 1+ }T
testing ... anything else: nothing comes of it
silver-ore 1 items send give-item  10 run-frames  silver-ore items:items-of before !
0 nothing-from-it 0 silver-ore broadcast item-event  600 play-on
T{ silver-ore items:items-of -> before @ }T
testing the high windows: her fear, then no luck
T{ 9 -62 at  said -> caught-msg }T
T{ $A 121 at  said -> no-luck-msg }T
testing the device: then her second thought
T{ $B -25 at  said -> device-msg }T
T{ $C 45 at  said -> device-2-msg }T
testing the Album Medallion Hewie sniffed out: taken
om1f-7-album-medallion-found story-flag-set  om1f-7-album-medallion story-flag-clear  room-afresh
album-medallion items:items-of before !
0 -30.81e 0e -25.85e put  circle press  300 read-on
T{ om1f-7-album-medallion story-flag?  album-medallion items:items-of -> -1 before @ 1+ }T
testing brought in by the forced way the first time: the scene, the doors anew, the stalker
over-music-c story-flag-clear  $44 doors:door-locked? 0= before !
old-mansion-1f-7 forced-exit rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  summoner-on state-flag? ;] 1500 soon  over-music-c story-flag?  $44 doors:door-locked? -> -1 -1 -1 }T
testing the movie replayed (the extras): played, then back by the forced way
200 run-frames  events-held state-flag-set  old-mansion-1f-7 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
