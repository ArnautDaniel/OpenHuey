\ old-mansion-1f-22's place: the cabinet - in (shut on her, hidden), out by the button (left open);
\ in again from open, out; the Refined Antimony. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_22.fs
IN: test-old-mansion-1f-22
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-22 ;
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
: room-afresh ( -- )  old-mansion-1f-22 room!  60 run-frames ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing the cabinet shut: in, hidden; out by the button, left open
om1f-22-flag1 story-flag-set  room-afresh
T{ 0 part? -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
cabinet-area 45 at  circle press  [: hidden? ;] 900 soon drop
T{ hidden?  cab-steps @ -> -1 1 }T
circle press  [: doing-busy? 0= ;] 900 soon drop
T{ hidden?  free?  om1f-22-flag1 story-flag? -> 0 -1 0 }T
testing the cabinet open: in again, hidden; out
cabinet-area 45 at  circle press  [: hidden? ;] 900 soon drop
T{ hidden? -> -1 }T
circle press  [: doing-busy? 0= ;] 900 soon drop
T{ hidden?  free? -> 0 -1 }T
testing the Refined Antimony: taken
om1f-22-refined-antimony story-flag-clear  room-afresh  refined-antimony items:items-of before !
0 147.17e 0e 82.89e put  circle press  300 read-on
T{ om1f-22-refined-antimony story-flag?  refined-antimony items:items-of -> -1 before @ 1+ }T
test-summary
