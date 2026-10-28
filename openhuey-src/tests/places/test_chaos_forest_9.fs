\ chaos-forest-9's place: the crate on the rock broken by a blow (in pieces, the Refined Antimony's
\ sparkle), picked up; her two remarks at the lake; the wisp, hovering, startled by a loud noise -
\ its fact, it rises and is gone; the maze dealt as she comes in; its way. Headless:
\   build/openhuey --test tests/places/test_chaos_forest_9.fs
IN: test-chaos-forest-9
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.forest story.places.chaos-forest-9 ;
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
: room-afresh ( -- )  chaos-forest-9 room!  60 run-frames ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] her ;
: wisp-hovers? ( -- flag )  [: wisp-up @  wisp 0< 0= and ;] in-place ;
: loud ( -- )  $40 room-id broadcast noise-was  2 run-frames ;
: wisp-gone? ( -- flag )  [: wisp 0< ;] in-place ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
cf-9-breakable-broken story-flag-clear  cf-9-refined-antimony story-flag-clear  cf-9-flag1 story-flag-clear
$1F progress pr.vars + 0 swap c!  room-afresh
testing coming in: the maze dealt (never before); the crate whole; the wisp hovering
T{ $1F progress pr.vars + c@  whole-part [: part? ;] in-place  wisp-hovers? -> 1 -1 -1 }T
testing the crate: a blow elsewhere misses it; in its place it breaks - in pieces, the antimony's sparkle
T{ -250e 72e -365e blow-at  [: the-crate breaks? ;] in-place -> 0 }T
T{ -298.4e 72e -365.4e blow-at  [: the-crate breaks? dup if  antimony-shows  then ;] in-place -> -1 }T
T{ cf-9-breakable-broken story-flag?  [: antimony-fx effect @ 0<> ;] in-place -> -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
[: body-point-off ;] her
testing the Refined Antimony picked up
refined-antimony items:items-of before !
T{ 0 -298.4e 72.6e -365.4e put  circle press  300 read-on  cf-9-refined-antimony story-flag?  refined-antimony items:items-of -> -1 before @ 1+ }T
testing her remarks at the lake: the tower, then no way on (and on)
T{ lake-area 180 at  said  lake-area 180 at  said  lake-area 180 at  said -> lake-msg no-way-msg no-way-msg }T
testing the wisp startled by a loud noise: its fact, risen and gone
T{ loud  cf-9-flag1 story-flag?  [: wisp-gone? ;] 300 soon -> -1 -1 }T
testing ... not there again
room-afresh
T{ wisp-hovers? -> 0 }T
testing its way out
T{ 0 0 at  [: room-id chaos-forest-9 <> ;] 300 soon -> -1 }T
test-summary
