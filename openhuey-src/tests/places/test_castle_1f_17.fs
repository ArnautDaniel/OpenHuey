\ castle-1f-17's place (scripts/story/places/castle-1f-17.fs): the specimen room - the pendulums,
\ the rug, the anatomical model, the insects, the pool table, the lavender, the door unlocked from
\ this side. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_1f_17.fs
IN: test-castle-1f-17
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-1f-17 doors items.names ;

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
: room-afresh ( -- )  castle-1f-17 room!  60 run-frames ;
: facing ( area F: px pz -- )   \ (in the area's middle, facing the point)
    {: area F: px pz :}  area area-middle drop {: F: x y z :}
    px x f-  pz z f-  fatan2 rad>deg f>s  x y z put ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )   \ (her blow's point there)
    blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)

: pendulum-turn ( k -- ) ( F: -- t )  [: hanging turn-y object@ ;] in-place ;
create spot 12 allot

free-play  10 run-frames  0 game:fiona send costume
testing the pendulums swinging (each its own period and swing)
room-afresh
fvariable was  0 pendulum-turn was f!  20 run-frames
T{ 0 pendulum-turn was f@ f- fabs 0.0001e f> -> -1 }T

testing the remarks: the rug, the anatomical model, the insects (then the other), the pool table (in turn)
T{ rug-area 0 at  said -> rug-msg }T
T{ model-area 0 at  said -> model-msg }T
T{ insects-area -45 at  said  insects-area -45 at  said -> insects-msg insects-again }T
: at-table ( -- )  pool-table-area area-middle drop {: F: x y z :}  0 x y z 6e f- put ;
c1f-17-lavender story-flag-set  room-afresh   \ (the lavender taken: its reach is by the table, offered first)
T{ at-table  said  at-table  said  at-table  said -> pool-table-msg pool-table-again pool-table-msg }T

testing the lavender up on the shelf: reached, taken
c1f-17-lavender story-flag-clear  room-afresh
variable before  lavender items:items-of before !
0 -6e 0e -1e put  circle press  300 read-on
T{ c1f-17-lavender story-flag?  lavender items:items-of -> -1 before @ 1+ }T

testing the door locked from this side (exit 1, $19): before it, the button - unlocked, said so
$19 s" doors" actor-named send lock  room-afresh
1 0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames
T{ circle press  [: window-shown unlocked-msg = ;] 300 soon -> -1 }T
300 read-on
T{ $19 door-locked? -> 0 }T
test-summary
