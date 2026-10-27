\ front-garden-5's place (scripts/story/places/front-garden-5.fs): the garden's far corner - the
\ hung things swaying, the chimes, the crate, the pickups, the drop, the rusted door, the hiding
\ place, Hewie's ways over. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_front_garden_5.fs
IN: test-front-garden-5
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.front-garden-5 items.names ;

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
: room-afresh ( -- )  front-garden-5 room!  60 run-frames ;
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
: barred? ( group flags -- flag )   \ (the first of its triangles with those flags)
    {: g fl :}  nav-tris 0 ?do  i g nav-in-group? if  i nav-flags fl and 0<> unloop exit  then  loop  false ;
: obj@ ( obj off -- ) ( F: -- v )  >r  [: hanging ;] in-place  r> object@ ;

free-play  10 run-frames  0 game:fiona send costume
testing a hung thing struck: away from her 12 degrees, settling to still
room-afresh
T{ [: hanger-a fiona sway-start ;] in-place  hanger-a swing-offset obj@ 12e deg>rad ~= -> -1 }T
: settles-in ( -- n )  [: 0  begin  1+  hanger-a hanging sway-step  over 500 > or  until ;] in-place ;
T{ settles-in 500 <  hanger-a turn-x obj@ 0e ~= -> -1 -1 }T
testing the chimes: at rest; her steps among them past 5 - swinging, the first ringing
T{ [: chimes-rung @ ;] in-place  first-chime swing-left obj@ f0= -> 0 -1 }T
fvariable st-y  fvariable st-z
: step ( -- )   \ (a 3 step for her, then the chimes' frame)
    game:fiona body-pos  st-z f!  st-y f!  3e f+  st-y f@ st-z f@  0e tp-facing  deliver
    [: first-chime 2 false chimes-rung chime-on chime-off 5e chimes ;] in-place  1 run-frames ;
T{ step  [: chimes-rung @ ;] in-place  first-chime swing-left obj@ f0= -> 0 -1 }T
T{ step  [: chimes-rung @ ;] in-place  first-chime swing-left obj@ f0> -> -1 -1 }T

testing the crate: a blow breaks it, the beef jerky in it; picked up
fg-5-breakable-broken story-flag-clear  fg-5-beef-jerky story-flag-clear  room-afresh
-14e -9e 97e blow-at  [: spots ;] in-place
T{ fg-5-breakable-broken story-flag?  [: jerky-fx effect @ 0<> ;] in-place -> -1 -1 }T
[: body-point-off ;] as-her
variable before  beef-jerky items:items-of before !
-14e -10e 97e picked
T{ fg-5-beef-jerky story-flag?  beef-jerky items:items-of -> -1 before @ 1+ }T

testing the hung thing's drop: falling, gone - the Viola Medallion sparkling where it fell; picked up
fg-5-flag1 story-flag-clear  fg-5-viola-medallion story-flag-clear  room-afresh
[: ['] something-drops start drop ;] in-place
T{ [: fg-5-flag1 story-flag? ;] 300 soon  [: viola-fx effect @ 0<> ;] in-place -> -1 -1 }T
viola-medallion items:items-of before !
-265e -10e 160e picked
T{ fg-5-viola-medallion story-flag?  viola-medallion items:items-of -> -1 before @ 1+ }T

testing the Medallion Hewie found, and the ball: picked up
fg-5-medallion-found story-flag-set  fg-5-medallion story-flag-clear  got-ball story-flag-clear  room-afresh
medallion items:items-of before !
-228.4e -10e 103.2e picked
T{ fg-5-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T
ball items:items-of before !
-180e 5.61e 176e picked
T{ got-ball story-flag?  ball items:items-of -> -1 before @ 1+ }T

testing the rusted door
T{ door-area 90 at  said -> rusted-msg }T

testing the hiding place (her clothes found): in, crouched and hidden; out again by the button
found-file-202 story-flag-set  room-afresh
variable entry-tri  s" fiona" actor-named body-tri entry-tri !   \ (where she came in: the stalker's way in)
hiding-area 0 at  circle press  300 read-on
T{ fiona-hidden state-flag?  [: fiona held? ;] in-place -> -1 -1 }T
testing ... the stalker comes in, no odds (his first time): not found, his visits counted
$A 0 progress pr.vars + c!  stalkers-blind state-flag-set  summoner-on state-flag-clear
debug:debilitas-out  2 run-frames  entry-tri @ debug:debilitas-tri  5 run-frames
T{ [: searching @  came-while-hidden @ ;] in-place  $A progress pr.vars + c@ -> 0 -1 1 }T
debug:debilitas-out  2 run-frames
testing ... found by him: out, running
[: -1 found-her ! ;] in-place  120 read-on
T{ fiona-hidden state-flag?  [: fiona held? ;] in-place -> 0 0 }T
testing ... in again, out by the button: crouched up, free
hiding-area 0 at  circle press  300 read-on
T{ fiona-hidden state-flag? -> -1 }T
circle press  300 read-on
T{ fiona-hidden state-flag?  [: fiona held? ;] in-place -> 0 0 }T

testing Hewie over the left wall (sent from near it): to its foot, a leap - its floor kept from her meanwhile, then back
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh
game:hewie send join-fiona  60 run-frames
[: ['] left-from-near start drop ;] in-place
: hewie-x ( F: -- x )  game:hewie body-pos fdrop fdrop ;
T{ [: hewie-x -120e f< ;] 900 soon -> -1 }T   \ (over the wall: the far side of it)
T{ [: [: hewie held? 0= ;] in-place ;] 900 soon  0 keeps-fiona-off barred? -> -1 0 }T   \ (done: the floor hers again)
test-summary
