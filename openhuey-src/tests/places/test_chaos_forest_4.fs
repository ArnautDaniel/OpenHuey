\ chaos-forest-4's place (and what the forest's bush rooms share, story/places/forest.fs): the
\ leaves still as she comes in, swinging as she pushes through; crouching in the bush (L1, or her
\ examine), getting up again (circle), hauled out in a panic; the stalker's search - found, or not,
\ his searches counted; "no time" as he comes by the bush's side; its ways. Headless:
\   build/openhuey --test tests/places/test_chaos_forest_4.fs
IN: test-chaos-forest-4
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.forest story.places.chaos-forest-4 ;
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
: room-afresh ( -- )  chaos-forest-4 room!  60 run-frames ;
: leaf@ ( field -- ) ( F: -- v )  >r  [: 0 hanging ;] in-place r> object@ ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: by-bush ( -- )  0 -61.3e 9e 0.24e put ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
room-afresh
testing coming in: the leaves still (pushed 0.9 already)
T{ swing-pushed leaf@ 0.9e f- fabs 0.01e f<  swing-left leaf@ f0= -> -1 -1 }T

testing the leaves swing as she pushes through, the first rustling
0 -70e 9e 0.24e put  by-bush
T{ swing-left leaf@ f0>  [: rustled @ ;] in-place -> -1 -1 }T
0 -70e 9e 0.24e put

testing crouching in the bush at her examine; up again at circle
by-bush  circle press  [: hidden? ;] 300 soon drop
T{ hidden?  [: fiona held? ;] in-place -> -1 -1 }T
60 run-frames  circle press  [: hidden? 0= ;] 300 soon drop  [: doing-busy? 0= ;] 300 soon drop
T{ hidden?  free? -> 0 -1 }T

testing crouching at L1 by it
by-bush
T{ l1 press  [: hidden? ;] 300 soon -> -1 }T
testing a panic while hidden: hauled out (her panic as the place hears of it)
T{ [: [: panicking panic-stage ! ;] in-place  hidden? 0= ;] 300 soon  [: doing-busy? 0= ;] 600 soon -> -1 -1 }T   \ (the place told she panics)
60 run-frames

testing the stalker's search: his searches counted; found - she is hauled out (the avoid prompt)
3 story.base:partner-load  5 run-frames
chaos-forest-4 $E0 game:daniella send stalker-in  30 run-frames
by-bush  circle press  [: hidden? ;] 300 soon drop  60 run-frames
$1B progress pr.vars + c@ before !
T{ [: 3 $1B var!  bush-search ;] in-place  $1B progress pr.vars + c@ -> 4 }T   \ (75 in 100 - try till found)
: try-find ( -- flag )  [: found-her @ ;] in-place  dup 0= if  [: 3 $1B var!  ['] bush-found running? 0= if  bush-search  then ;] in-place  then ;
T{ [: try-find ;] 3000 soon -> -1 }T
T{ [: hidden? 0= ;] 600 soon -> -1 }T
[: doing-busy? 0= ;] 900 soon drop

testing "no time" by the bush as he has come by its side (him in, by exit 2)
chaos-forest-4 $E0 game:daniella send stalker-in  30 run-frames  by-bush  10 run-frames
T{ [: 2 st-came-now !  1 st-age !  0 offer-msg !  0 offer-xt !  stalker-in? if  bush-frame  offer-msg @  else  -2  then ;] in-place -> no-time-message }T
game:daniella send stalker-out  10 run-frames

testing its ways: out by an open one (the maze as dealt); in by exit 2
room-afresh
: open-way ( -- exit )  [: -1  4 0 do  i story.kit:exit-open? if  drop i leave  then  loop ;] in-place ;
T{ open-way 0< 0= -> -1 }T
T{ open-way 0 at  [: room-id chaos-forest-4 <> ;] 300 soon -> -1 }T
chaos-forest-4 2 rooms send go-to-room  60 run-frames
T{ room-id  [: came-by @ ;] in-place -> chaos-forest-4 2 }T
test-summary
