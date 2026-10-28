\ house-of-truth-b1-7's place (a colour room, story/places/house-of-truth.fs): its colour done as
\ she comes in; its inscription; the pickup; in by the forced way - at its spot, the dark
\ lifting; out through a door. Headless:
\   build/openhuey --test tests/places/test_house_of_truth_b1_7.fs
IN: test-house-of-truth-b1-7
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-b1-7 ;
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
: room-afresh ( -- )  house-of-truth-b1-7 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
shared-flag1 story-flag-set  shared-flag2 story-flag-set  shared-flag3 story-flag-set  shared-flag4 story-flag-clear
htb1-7-flag1 story-flag-clear  room-afresh
testing coming in: its colour done
T{ htb1-7-flag1 story-flag? -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing its inscription by the wall
T{ inscription-area -45 at  said -> inscription-msg }T
testing the pickup
htb1-7-beef-jerky story-flag-clear  room-afresh  beef-jerky items:items-of before !
T{ 0 -119.22e 1e -96.82e put  circle press  300 read-on  htb1-7-beef-jerky story-flag?  beef-jerky items:items-of -> -1 before @ 1+ }T
testing in by the forced way: at its spot, Hewie by her, the dark lifting
house-of-truth-b1-7 forced-exit rooms send go-to-room  [: doing-busy? 0= room-id house-of-truth-b1-7 = and ;] 600 soon drop  30 run-frames
T{ [: fiona body-tri arrival-tri = ;] in-place  free? -> -1 -1 }T
testing out through a door into the dark
room-afresh
T{ 0 0 at  [: room-id house-of-truth-b1-7 <> world-held state-flag? or ;] 600 soon -> -1 }T
300 play-on
test-summary
