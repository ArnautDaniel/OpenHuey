\ old-mansion-3f-2's place: the first time in, its scene; the plates (a step onto one: the warning
\ shot); the armour's button (pushed: disarmed; then the trap remark); Hewie sent to the far end;
\ the replays. Headless:
\   build/openhuey --test tests/places/test_old_mansion_3f_2.fs
IN: test-old-mansion-3f-2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-3f-2 ;
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
: room-afresh ( -- )  old-mansion-3f-2 room!  60 run-frames ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  10 run-frames ;
: picked ( -- n )  game:window enter  window:picked @  leave-actor ;
: yes-it ( -- )   \ (its choice may start on no; the question on its last page)
    [: key: Left tap  picked 0= dup 0= if  cross press  20 run-frames  then ;] 30 soon drop  cross press ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing the first time in: its scene
om3f-2-flag1 story-flag-clear  armor-button-pushed story-flag-clear  room-afresh
T{ [: cutscene-active? movie-status 1 = or ;] 300 soon -> -1 }T
T{ [: movie-skipped state-flag-set  doing-busy? 0= ;] 1500 soon  om3f-2-flag1 story-flag? -> -1 -1 }T
testing a step onto a plate: the warning shot, its scene
room-afresh
T{ [: 1 plate-on !  0 plate-on !  0 shots !  2 plate-on ! ;] in-place  [: plate-now ;] in-place -> 0 }T
T{ [: ['] first-shot start drop  1 shots ! ;] in-place  [: cutscene-active? movie-status 1 = or ;] 300 soon -> -1 }T
T{ [: movie-skipped state-flag-set  doing-busy? 0= ;] 1500 soon  [: armor-on @ ;] in-place -> -1 0 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing the armour's button: pushed - the trap disarmed; then its remark
6 0 at  circle press  shown-soon drop  yes-it  300 play-on
T{ armor-button-pushed story-flag? -> -1 }T
T{ 6 0 at  said -> trap-msg }T
testing the movies replayed (the extras): played, then back by the forced way
200 run-frames  events-held state-flag-set  old-mansion-3f-2 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 3000 soon -> -1 }T
test-summary
