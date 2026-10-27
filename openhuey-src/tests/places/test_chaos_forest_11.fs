\ chaos-forest-11's place: the rabbit grazing, off into the trees as she comes near; the wisp startled as she comes near it - its
\ fact, risen and gone; its five ways. Headless:
\   build/openhuey --test tests/places/test_chaos_forest_11.fs
IN: test-chaos-forest-11
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.forest story.places.chaos-forest-11 ;
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
: room-afresh ( -- )  chaos-forest-11 room!  60 run-frames ;
: open-way ( -- exit )  [: -1  5 0 do  i story.kit:exit-open? if  drop i leave  then  loop ;] in-place ;
: wisp-hovers? ( -- flag )  [: wisp-up @  wisp 0< 0= and ;] in-place ;
: loud ( -- )  $40 room-id broadcast noise-was  2 run-frames ;
: wisp-gone? ( -- flag )  [: wisp 0< ;] in-place ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
cf-11-flag1 story-flag-clear  room-afresh
testing the rabbit (now and then as she comes in): grazing there; she comes near - off into the trees and gone
T{ [: rabbit-kind rabbit-slot figure-in  ['] rabbit start drop ;] in-place  [: [: rabbit-slot figure 0< 0= ;] in-place ;] 30 soon -> -1 }T
T{ 0 12e 31.5e 228e put  [: [: rabbit-slot figure 0< ;] in-place ;] 900 soon -> -1 }T
testing the wisp hovering; she comes near it: startled - its fact, risen and gone
T{ wisp-hovers? -> -1 }T
T{ 0 -24e 8.31e -132e put  cf-11-flag1 story-flag?  [: wisp-gone? ;] 300 soon -> -1 -1 }T
testing ... not there again
room-afresh
T{ wisp-hovers? -> 0 }T
testing its ways: out by an open one; in by each of its five
T{ open-way 0< 0= -> -1 }T
T{ open-way 0 at  [: room-id chaos-forest-11 <> ;] 300 soon -> -1 }T
: in-by ( exit -- n )  chaos-forest-11 swap rooms send go-to-room  60 run-frames  [: came-by @ ;] in-place ;
T{ 0 in-by  1 in-by  2 in-by  3 in-by  4 in-by -> 0 1 2 3 4 }T
test-summary
