\ old-mansion-1f-6's place: the hopping figure; the scene's figure by the far way (turning, walking
\ off; a homunculus in its place); the hiding place; the sheet, the cage; the Medallion. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_6.fs
IN: test-old-mansion-1f-6
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-6 ;
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
: room-afresh ( -- )  old-mansion-1f-6 room!  60 run-frames ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: toward ( area F: px pz -- )   \ (at the area's middle, facing the point)
    {: a F: px pz :}  a area-middle drop {: F: x y z :}  px x f-  pz z f-  fatan2 rad>deg f>s  x y z put ;
: facing-middle ( area -- )  area-middle drop {: F: x y z :}  90  x 3e f-  y z put ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
om1f-6-flag1 story-flag-set
testing coming in: the figure hopping, the hall hunted
room-afresh
T{ [: hopper-slot figure 0< 0= ;] in-place  hunted state-flag? -> -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing the sheet, the cage
T{ $C facing-middle  said -> sheet-msg }T
T{ $D 45 at  said -> cage-msg }T
testing the hiding place: in, hidden; out by the button
hide-area -45 at  circle press  [: hidden? ;] 600 soon drop
T{ hidden? -> -1 }T
circle press  [: doing-busy? 0= ;] 600 soon drop
T{ hidden?  free? -> 0 -1 }T
testing the Medallion: taken
om1f-6-medallion story-flag-clear  room-afresh  medallion items:items-of before !
0 -119.67e 0e 147.64e put  circle press  300 read-on
T{ om1f-6-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T
testing by the far way before the 3F's turn: the figure there; by the cage, a homunculus instead
om1f-6-flag1 story-flag-clear  om3f-1-flag2 story-flag-clear
$21 4 story.kit:figure-in  20 run-frames  old-mansion-1f-6 1 rooms send go-to-room  60 run-frames
T{ [: walker-slot figure 0< 0= ;] in-place -> -1 }T
[: -1 in-further ! ;] in-place  120 run-frames   \ (as she steps further in: area $B a gate)
T{ [: walker-slot figure body-yaw angle-wrap 1.6e f< ;] in-place -> -1 }T
test-summary
