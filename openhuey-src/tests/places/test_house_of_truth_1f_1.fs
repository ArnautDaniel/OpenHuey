\ house-of-truth-1f-1's place: coming in - the wall whole, the flame, the holes in the floor, its
\ steady sound; the water (a recovery point, then its second remark); the sign; the fire; the blast
\ (the scene: the wall blown, the candlestick spent, the door unlocked, Lorenzo laid in the rubble, the
\ summoner off) and the wreckage after; leaving after it - Lorenzo let go; the Album Medallion Hewie
\ sniffs out; the replay. Headless:
\   build/openhuey --test tests/places/test_house_of_truth_1f_1.fs
IN: test-house-of-truth-1f-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-1f-1 ;
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
: room-afresh ( -- )  house-of-truth-1f-1 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
ht1f-1-flag1 story-flag-clear  ht1f-1-album-medallion-found story-flag-clear  ht1f-1-album-medallion story-flag-clear
got-doc-recovery story-flag-set  room-afresh
testing coming in: the wall whole, the flame, the holes in the floor, its steady sound
T{ 0 [: part? ;] in-place  1 [: part? ;] in-place  [: flame-fx effect @ 0<>  steady-on @ ;] in-place  $51 nav-flags floor-flags:hole-floor and 0<> -> -1 0 -1 -1 -1 }T
testing the water: a recovery point; then its second remark
T{ water-area 0 at  circle press  [: window-shown 0< 0= if  cross press  then  [: rested @ ;] in-place  doing-busy? 0= and ;] 900 soon -> -1 }T
T{ water-area 0 at  said -> good-water-msg }T
testing the sign; the fire before the wall
T{ sign-area 0 at  [: fiona sign-area 50 faces-area? ;] in-place -> -1 }T
T{ said  wall-area 45 at  said -> flammable-msg fire-msg }T
testing the blast: the wall blown, the candlestick spent, the door unlocked; Lorenzo laid there, the summoner off
golden-candlestick [: 1 give-item ;] in-place  5 run-frames  golden-candlestick items:items-of before !
lorenzo2-kind story.words:partner-load  5 run-frames  summoner-on state-flag-set  blasted-door [: lock ;] in-place
[: ['] blast start drop ;] in-place
T{ [: movie-skipped state-flag-set  blast-seen resident-flag? ;] 2000 soon -> -1 }T
T{ 1 [: part? ;] in-place  golden-candlestick items:items-of  blasted-door doors:door-locked?  summoner-on state-flag? -> -1 before @ 1- 0 0 }T
T{ [: ['] lorenzo-lies running?  flame-lit @ ;] in-place  force-calm state-flag? -> -1 -1 -1 }T
300 play-on
testing then the wreckage; its second steady sound
ht1f-1-flag1 story-flag-set
T{ wreckage-area 30 at  said  [: steady-2-on @ ;] in-place -> wreckage-msg -1 }T
testing leaving after it: Lorenzo let go, her calm and the camera as they were
T{ 3 0 at  [: room-id house-of-truth-1f-1 <> ;] 300 soon  force-calm state-flag?  no-stalker-camera state-flag? -> -1 0 0 }T
300 play-on
testing the Album Medallion: Hewie sniffs it out; taken
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  game:hewie send join-fiona  60 run-frames
[: ['] album-sniffed start drop ;] in-place
T{ [: ht1f-1-album-medallion-found story-flag? ;] 600 soon -> -1 }T
400 run-frames  album-medallion items:items-of before !
T{ 0 -78e 1e 39e put  circle press  300 read-on  ht1f-1-album-medallion story-flag?  album-medallion items:items-of -> -1 before @ 1+ }T
testing the movie replayed (the extras): played, then back by the forced way
ht1f-1-flag1 story-flag-clear  events-held state-flag-set  house-of-truth-1f-1 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon -> -1 }T
test-summary
