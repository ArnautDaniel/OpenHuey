\ house-of-truth-1f-7's place: coming in - the cane there, the wall whole, the flames, the Antimony
\ Tube; file 255 found, Hewie barking at the wall; her look - the scene, the wall opened; the Cane of
\ Caduceus; her track on, the trap - its scene, she shakes free at 100; the hiding place; the stalker's
\ search finding her; the door unlocked from this side; the tube; a creature let out; the replays;
\ caught in the trap (the game over). Headless:
\   build/openhuey --test tests/places/test_house_of_truth_1f_7.fs
IN: test-house-of-truth-1f-7
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-1f-7 ;
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
: room-afresh ( -- )  house-of-truth-1f-7 room!  60 run-frames ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: replays ( n -- flag )
    replay-var var!  events-held state-flag-set  house-of-truth-1f-7 -1 rooms send go-to-room  5 run-frames
    [: cutscene-active? movie-status 1 = or ;] 600 soon  [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon and ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
got-cane-of-caduceus story-flag-clear  ht1f-7-flag1 story-flag-clear  ht1f-7-flag2 story-flag-clear  ht1f-7-flag3 story-flag-clear
ht1f-7-antimony-tube story-flag-clear  over-music-f story-flag-clear  ht1f-9-flag2 story-flag-clear  found-file-255 story-flag-set
room-afresh
testing coming in: the cane there, the wall whole, the flames lit, the tube's sparkle
T{ cane-part [: part? ;] in-place  [: 0 effect @ 0<>  4 effect @ 0<>  tube-fx effect @ 0<> ;] in-place -> -1 -1 -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing Hewie by the wall: he barks at it; her look there - the scene, the wall opened
hewie-freed story-flag-set  hewie-commandable state-flag-set  game:hewie send join-fiona  60 run-frames
house-of-truth-1f-7 -68e 0e 75e nav-tri 0 game:hewie send hewie-place  30 run-frames
T{ [: [: hewie-at-wall @ ;] in-place ;] 300 soon -> -1 }T
T{ wall-area -90 at  circle press  [: window-shown 0< 0= if  cross press  then  movie-skipped state-flag-set  ht1f-7-flag1 story-flag? ;] 2000 soon -> -1 }T
T{ [: $4C resident-flag? ;] 600 soon -> -1 }T
300 play-on
testing the Cane of Caduceus behind it
cane-of-caduceus items:items-of before !
T{ wall-area -90 at  circle press  300 read-on  got-cane-of-caduceus story-flag?  cane-of-caduceus items:items-of -> -1 before @ 1+ }T
testing a creature let out as she crosses on (once)
0 49e 0e 40e put
T{ 0 29e 0e 40e put  ht1f-7-flag3 story-flag? -> -1 }T
testing her track on, the trap: the first time in - its scene, she shakes free (100)
over-music-f story-flag-set  room-afresh  0 -27e 0e 70e put
T{ 0 -47e 0e 70e put  ht1f-7-flag2 story-flag? -> -1 }T
T{ [: movie-skipped state-flag-set  $50 resident-flag? ;] 2000 soon  [: [: trapped @ ;] in-place ;] 600 soon -> -1 -1 }T
T{ [: 100 shakes-now ! ;] in-place  [: [: trapped @ 0= ;] in-place ;] 600 soon -> -1 }T
300 play-on
testing the hiding place: in at her examine, out at circle
room-afresh
T{ hiding-area 180 at  circle press  [: hidden? ;] 900 soon -> -1 }T
60 run-frames  circle press  [: hidden? 0= ;] 600 soon drop  [: doing-busy? 0= ;] 600 soon drop
T{ hidden? -> 0 }T
testing the stalker coming on her hidden: his search - found, hauled out
lorenzo2-kind story.base:partner-load  5 run-frames
hiding-area 180 at  circle press  [: hidden? ;] 900 soon drop  60 run-frames
house-of-truth-1f-7 $FE game:lorenzo2 send stalker-in  30 run-frames
: try-find ( -- flag )  [: found-her @ ;] in-place  dup 0= if  [: 3 searches-var var!  0 searched !  ['] found-there running? 0= if  searched-hidden  then ;] in-place  then ;
T{ [: try-find ;] 3000 soon  [: hidden? 0= ;] 600 soon -> -1 -1 }T
[: doing-busy? 0= ;] 900 soon drop  game:lorenzo2 send stalker-out  10 run-frames  [: 0 panic-to ;] in-place  60 run-frames
testing the door: unlocked from this side (her request at it)
room-afresh  0 by-door
T{ [: 0 offer-xt !  0 offer-msg !  2 request-now !  spots  offer-xt @ ;] in-place  ' unlocks-door = -> -1 }T
testing the Antimony Tube
antimony-tube items:items-of before !
T{ 0 13.05e 6.54e 108.47e put  circle press  300 read-on  ht1f-7-antimony-tube story-flag?  antimony-tube items:items-of -> -1 before @ 1+ }T
testing the movies replayed (the extras)
T{ 50 replays  51 replays -> -1 -1 }T
testing caught in the trap: the game over
over-music-f story-flag-set  ht1f-7-flag2 story-flag-clear  room-afresh
T{ [: -1 trapped !  ['] caught-in-it fiona start-holding drop ;] in-place  [: caught state-flag? ;] 600 soon -> -1 }T
test-summary
