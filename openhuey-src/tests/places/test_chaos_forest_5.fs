\ chaos-forest-5's place: its bush (story/places/forest.fs: crouching at L1, up again, the stalker\n\ finding her - his searches counted here) and its ways - out by an open one (the maze as
\ dealt), in by each. Headless:
\   build/openhuey --test tests/places/test_chaos_forest_5.fs
IN: test-chaos-forest-5
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.forest story.places.chaos-forest-5 ;
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
: room-afresh ( -- )  chaos-forest-5 room!  60 run-frames ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: open-way ( -- exit )  [: -1  4 0 do  i story.kit:exit-open? if  drop i leave  then  loop ;] in-place ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
room-afresh
: by-bush ( -- )  0 70.5e 9e -5.5e put ;
3 story.words:partner-load  5 run-frames
testing "no time" by it as he has come by its side (exit 1)
chaos-forest-5 $E7 game:daniella send stalker-in  30 run-frames  by-bush  10 run-frames
T{ [: 1 st-came-now !  1 st-age !  0 offer-msg !  0 offer-xt !  stalker-in? if  bush-frame  offer-msg @  else  -2  then ;] in-place -> no-time-message }T
game:daniella send stalker-out  10 run-frames
testing its bush: crouching at L1, up again at circle
by-bush
T{ l1 press  [: hidden? ;] 300 soon -> -1 }T
60 run-frames  circle press  [: hidden? 0= ;] 300 soon drop  [: doing-busy? 0= ;] 300 soon drop
T{ hidden?  free? -> 0 -1 }T
testing the stalker coming to search it: his searches counted (here $1C); found, she is hauled out
chaos-forest-5 $E7 game:daniella send stalker-in  30 run-frames
by-bush  l1 press  [: hidden? ;] 300 soon drop  60 run-frames
T{ [: 3 $1C var!  bush-search ;] in-place  $1C progress pr.vars + c@ -> 4 }T
: try-find ( -- flag )  [: found-her @ ;] in-place  dup 0= if  [: 3 $1C var!  ['] bush-found running? 0= if  bush-search  then ;] in-place  then ;
T{ [: try-find ;] 3000 soon  [: hidden? 0= ;] 600 soon -> -1 -1 }T
[: doing-busy? 0= ;] 900 soon drop  [: 0 panic-to ;] in-place  120 run-frames
game:daniella send stalker-out  10 run-frames
testing its ways: out by an open one
room-afresh
T{ open-way 0< 0= -> -1 }T
T{ open-way 0 at  [: room-id chaos-forest-5 <> ;] 300 soon -> -1 }T
testing ... in by each of its four: came by it
: in-by ( exit -- n )  chaos-forest-5 swap rooms send go-to-room  60 run-frames  [: came-by @ ;] in-place ;
T{ 0 in-by  1 in-by  2 in-by  3 in-by -> 0 1 2 3 }T
test-summary
