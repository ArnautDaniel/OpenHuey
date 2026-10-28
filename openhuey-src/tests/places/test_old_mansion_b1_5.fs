\ old-mansion-b1-5's place: a search declined; the wax figures searched (the scene, a way opened);
\ again: already searched; a trick (the door slams and locks); the cabinet (in, out); Daniella
\ waiting inside it; the replays. Headless:
\   build/openhuey --test tests/places/test_old_mansion_b1_5.fs
IN: test-old-mansion-b1-5
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-b1-5 ;
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
: room-afresh ( -- )  old-mansion-b1-5 room!  60 run-frames ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  10 run-frames ;
: picked ( -- n )  game:window enter  window:picked @  leave-actor ;
variable wanted   \ (the choice to pick)
: choose ( n -- )   \ (the choice's n-th - the question on its last page)
    wanted !  [: wanted @ picked = if  true exit  then  wanted @ if  key: Right  else  key: Left  then  tap  wanted @ picked =  dup 0= if  cross press  20 run-frames  then ;] 30 soon drop  cross press ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
game:debilitas send stalker-out  5 run-frames
omb1-5-flag1 story-flag-clear  omb1-5-flag4 story-flag-clear  omb1-5-flag5 story-flag-set  room-afresh
testing the wax figures: declined
5 0 at  circle press  shown-soon drop  1 choose  200 read-on
T{ omb1-5-flag1 story-flag?  doing-busy? -> 0 0 }T
testing ... searched: the scene, a way opened; "Did something just move?"
5 0 at  circle press  shown-soon drop  0 choose
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
[: movie-skipped state-flag-set  window-shown 0< 0= ;] 2000 soon drop
T{ window-shown  omb1-5-flag1 story-flag? -> moved-msg -1 }T
300 read-on
T{ 5 0 at  said -> searched-msg }T
testing a trick: the door slams and locks
trap-door s" doors" actor-named send unlock  10 run-frames
8 -90 at  circle press  shown-soon drop  0 choose  600 play-on
T{ trap-door doors:door-locked? -> -1 }T
trap-door s" doors" actor-named send unlock  10 run-frames
testing the cabinet: in, hidden; out by the button
cabinet-area 180 at  circle press  [: hidden? ;] 900 soon drop
T{ hidden? -> -1 }T
circle press  [: doing-busy? 0= ;] 900 soon drop
T{ hidden?  free? -> 0 -1 }T
testing Daniella waiting inside it the first time: she bursts out at her
omb1-5-flag5 story-flag-clear  omb1-5-flag4 story-flag-clear  daniella-kind story.base:partner-load  10 run-frames  room-afresh
T{ [: waiting @ ;] in-place -> -1 }T
cabinet-area 180 at  circle press  [: doing-busy? 0= ;] 1200 soon drop
T{ omb1-5-flag5 story-flag?  omb1-5-flag4 story-flag? -> -1 -1 }T
partner send stalker-out  10 run-frames
testing the movies replayed (the extras): played, then back by the forced way
200 run-frames  events-held state-flag-set  old-mansion-b1-5 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 300 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 3000 soon -> -1 }T
test-summary
