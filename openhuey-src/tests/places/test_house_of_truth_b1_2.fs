\ house-of-truth-b1-2's place: brought here the first time - Lorenzo's words (the scene, the file,
\ the doors shut); the door locked from the far side, tried; the other door the first time - through
\ to Lorenzo; the clock's save; the Lavender; its sounds in turn; the replay. Headless:
\   build/openhuey --test tests/places/test_house_of_truth_b1_2.fs
IN: test-house-of-truth-b1-2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-b1-2 ;
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
: room-afresh ( -- )  house-of-truth-b1-2 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
found-file-252 story-flag-clear  over-music-e story-flag-clear  htb1-2-lavender story-flag-clear
testing brought here the first time: Lorenzo's words - its fact, its file; the doors shut
house-of-truth-b1-2 forced-exit rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  belli-seen resident-flag? ;] 2000 soon  found-file-252 story-flag?  belli-file filed? -> -1 -1 -1 }T
T{ hall-door doors:door-locked?  b1-1-door doors:door-locked? -> -1 -1 }T
300 play-on
testing the Lavender (facing away from the clock: it is offered first)
room-afresh  lavender items:items-of before !
T{ 180 -45.77e 5.75e 19.99e put  circle press  300 read-on  htb1-2-lavender story-flag?  lavender items:items-of -> -1 before @ 1+ }T
testing its sounds in turn (every 30 frames)
T{ [: story.kit:ambient-step @ ;] in-place  31 run-frames  [: story.kit:ambient-step @ ;] in-place = -> 0 }T
testing the door locked from the far side: her try at it (her request there)
0 by-door
T{ [: 0 offer-xt !  0 offer-msg !  2 request-now !  spots  offer-xt @ ;] in-place  ' tries-it = -> -1 }T
testing the clock: the save asked (no)
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  10 run-frames ;
: picked ( -- n )  game:window enter  window:picked @  leave-actor ;
T{ clock-area 0 at  [: fiona clock-area 50 faces-area? ;] in-place -> -1 }T
T{ circle press  shown-soon -> clock-msg }T
[: key: Right tap  picked 1 = dup 0= if  cross press  20 run-frames  then ;] 30 soon drop  cross press  300 read-on
testing the other door the first time: through it - Lorenzo's room (B1-1)
room-afresh  1 by-door
T{ [: fiona 1 before-door? ;] in-place -> -1 }T
T{ circle press  [: room-id house-of-truth-b1-1 = ;] 900 soon -> -1 }T
1200 play-on  1e sound-scale!  summoner-on state-flag-clear
testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  house-of-truth-b1-2 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon -> -1 }T
test-summary
