\ front-garden-2's place (scripts/story/places/front-garden-2.fs): the yard - the bolted grate and
\ its scene, the windows, three crates and the Medallion, the night's whine, turning back for
\ Hewie. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_front_garden_2.fs
IN: test-front-garden-2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.front-garden-2 items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: room-afresh ( -- )  front-garden-2 room!  60 run-frames ;
: facing ( area F: px pz -- )   \ (in the area's middle, facing the point)
    {: area F: px pz :}  area area-middle drop {: F: x y z :}
    px x f-  pz z f-  fatan2 rad>deg f>s  x y z put ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )   \ (her blow's point there)
    blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)

: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
: grate-turn ( F: -- t )  s" kousi" turn-y object@ ;
: barred? ( group -- flag )   \ (the first of its triangles barred)
    nav-tris 0 ?do  i over nav-in-group? if  drop i nav-flags barred-floor and 0<> unloop exit  then  loop  drop false ;
: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;

free-play  10 run-frames  0 game:fiona send costume
testing the grate bolted: shut, the floor before it barred; facing it, she says so (whatever goes on)
fg-2-flag1 story-flag-clear  hewie-found-in-wire story-flag-clear  room-afresh
T{ shut-grate-group barred?  open-grate-group barred?  grate-turn 0e ~= -> -1 0 -1 }T
T{ grate-area -90 at  said -> bolted-msg }T
testing the windows: the flickering light, from either
T{ window-area 180 at  said  window-2-area -90 at  said -> flicker-msg flicker-msg }T

testing Hewie found in the wire: crossing by the grate, it swings open (the scene) - the door unlocked, the floor behind it barred
hewie-found-in-wire story-flag-set  room-afresh
by-grate-area 0 at  10 run-frames
T{ [: fg-2-flag1 story-flag? ;] 60 soon -> -1 }T
600 play-on
T{ grate-turn pi f2/ fnegate ~=  shut-grate-group barred?  open-grate-group barred?  grate-door doors:door-locked? -> -1 0 -1 0 }T
T{ grate-area -90 at  said -> -1 }T   \ (open now: nothing said)

testing the crates: each broken by a blow in its place; the second has the Medallion
fg-2-breakable1-broken story-flag-clear  fg-2-breakable2-broken story-flag-clear  fg-2-breakable3-broken story-flag-clear
fg-2-medallion story-flag-clear  room-afresh
T{ 0 part?  2 part?  4 part? -> -1 -1 -1 }T
T{ 131e 61e -244e blow-at  [: crate-1 breaks? ;] in-place -> -1 }T
T{ 137e 61e -240e blow-at  [: crate-3 breaks? ;] in-place -> -1 }T
[: body-point-off ;] as-her
T{ [: medallion-fx effect @ ;] in-place -> 0 }T
130e 61e -237e blow-at  [: spots ;] in-place  [: body-point-off ;] as-her
T{ fg-2-breakable2-broken story-flag?  [: medallion-fx effect @ 0<> ;] in-place  1 part?  3 part?  5 part? -> -1 -1 -1 -1 -1 }T
variable before  medallion items:items-of before !
180 130e 60e -233e put  circle press  300 read-on   \ (facing off the window, so its remark isn't offered first)
T{ fg-2-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T

testing the night, Hewie not yet found: his whine now and then (the activity), the specks
c2f-3-flag1 story-flag-set  hewie-found-in-wire story-flag-clear  room-afresh
T{ [: whine-frames @ 0> ;] in-place -> -1 }T
testing ... Hewie in the wire, not freed: heading for the cage she turns back for him (to the bedroom, by the wire)
hewie-found-in-wire story-flag-set  hewie-freed story-flag-clear  c2f-3-flag2 story-flag-clear  room-afresh
to-cage-area 0 at
T{ [: room-id front-garden-2 <> ;] 200 soon  room-id -> -1 front-garden-2 0 room-exit-leads drop }T   \ (exit 0's room - castle-2f-3 once a new game has closed the way)

testing the movie replayed (the extras): the grate's scene, then back by the forced way
events-held state-flag-set  front-garden-2 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
