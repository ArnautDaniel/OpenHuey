\ house-of-truth-b1-1's place: as it stands; brought in by the forced way - Lorenzo's scene: its file
\ and fact, the door to B1-2 open, her clothes as they were; the replay. Headless:
\   build/openhuey --test tests/places/test_house_of_truth_b1_1.fs
IN: test-house-of-truth-b1-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-b1-1 ;
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
: room-afresh ( -- )  house-of-truth-b1-1 room!  60 run-frames ;
: opened? ( door -- flag )  doors:opened-bit doors:has? ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
over-music-e story-flag-clear  room-afresh
testing as it stands: his part shown
T{ lorenzo-part [: part? ;] in-place -> -1 }T
testing brought in by the forced way: his scene - the fact, its file, the door to B1-2 open, her costume as it was
3 costume-var [: var! ;] in-place  b1-2-door [: lock ;] in-place
house-of-truth-b1-1 forced-exit rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  lorenzo-seen resident-flag? ;] 2000 soon  over-music-e story-flag?  lorenzo-file filed? -> -1 -1 -1 }T
T{ b1-2-door doors:door-locked?  b1-2-door opened?  new-game-sounds state-flag? -> 0 -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
300 play-on  1e sound-scale!  summoner-on state-flag-clear
testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  house-of-truth-b1-1 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon -> -1 }T
test-summary
