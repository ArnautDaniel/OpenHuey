\ chaos-forest-12's place: the Camomile picked up; the wisp startled by a loud noise; once Hewie has
\ been found shot (file 247), the stalker loaded ahead by the way on and let go again; its ways.
\ Headless:
\   build/openhuey --test tests/places/test_chaos_forest_12.fs
IN: test-chaos-forest-12
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.forest story.places.chaos-forest-12 ;
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
: room-afresh ( -- )  chaos-forest-12 room!  60 run-frames ;
: open-way ( -- exit )  [: -1  4 0 do  i story.kit:exit-open? if  drop i leave  then  loop ;] in-place ;
: wisp-hovers? ( -- flag )  [: wisp-up @  wisp 0< 0= and ;] in-place ;
: loud ( -- )  $40 room-id broadcast noise-was  2 run-frames ;
: wisp-gone? ( -- flag )  [: wisp 0< ;] in-place ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
cf-12-flag1 story-flag-clear  cf-12-camomile story-flag-clear  room-afresh
testing coming in: the Camomile's sparkle; the wisp hovering
T{ [: camomile-fx effect @ 0<> ;] in-place  wisp-hovers? -> -1 -1 }T
testing the Camomile picked up
camomile items:items-of before !
T{ 0 -253.83e 5.54e -217.28e put  circle press  300 read-on  cf-12-camomile story-flag?  camomile items:items-of -> -1 before @ 1+ }T
testing the wisp startled by a loud noise: its fact, risen and gone
T{ loud  cf-12-flag1 story-flag?  [: wisp-gone? ;] 300 soon -> -1 -1 }T
testing the stalker loaded ahead by the way on (Hewie found, the heart's scene to come), let go again
found-file-247 story-flag-set  cf-13-flag1 story-flag-clear  room-afresh
T{ 7 0 at  [: ahead-slot figure 0< 0= ;] in-place -> -1 }T
T{ 0 -253.83e 5.54e -217.28e put  [: ahead-slot figure 0< ;] in-place -> -1 }T
found-file-247 story-flag-clear
testing its ways: out by an open one; in by each of its four
room-afresh
T{ open-way 0< 0= -> -1 }T
T{ open-way 0 at  [: room-id chaos-forest-12 <> ;] 300 soon -> -1 }T
: in-by ( exit -- n )  chaos-forest-12 swap rooms send go-to-room  60 run-frames  [: came-by @ ;] in-place ;
T{ 0 in-by  1 in-by  2 in-by  3 in-by -> 0 1 2 3 }T
test-summary
