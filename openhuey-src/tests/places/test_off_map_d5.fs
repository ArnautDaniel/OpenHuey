\ off-map-d5's place (the clock run, Hewie played): in by any of its five ways (placed at it, the
\ camera); its cameras; out by each from its own area; the rooms behind them loaded ahead as
\ Hewie nears them; a rabbit now and then, off into the trees; the wisp, startled as Hewie (or
\ she) comes near - once; gone as he leaves; the clock. Headless:
\   build/openhuey --test tests/places/test_off_map_d5.fs
IN: test-off-map-d5
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.clock story.places.forest story.places.off-map-d5 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: his ( xt -- x )  game:hewie enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )   \ (on the floor, on a triangle she walks - else the nearest the mesh has)
    on-floor s>f tp-facing  [: her-at $28020018 v-tri-in  dup 0< if  drop her-at vec@ nav-tri  then  her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: hput ( deg F: x y z -- )  on-floor  s>f deg>rad xyzw>cells  game:hewie send place  10 run-frames ;   \ (Hewie, there on the floor)
: hat ( area deg -- )  >r area-middle drop r> hput ;
: his-camera@ ( -- set path )  game:hewie cam-setup-of ;
: his-tri ( -- tri )  [: self body-tri ;] his ;
: on-tri ( tri -- )  0 swap tri-center put ;
: shown-soon ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
: said ( -- msg )
    circle press  shown-soon
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: spare@ ( -- room )  game:rooms enter  rooms:spare @  leave-actor ;   \ (the room loaded ahead; -1 none)
: her-camera@ ( -- set path )  game:fiona cam-setup-of ;
: across ( gate side -- )   \ (stepped over the gate's line: from the other side to this one, 4 from it)
    {: g side | F: ax F: ay F: az F: bx F: bz F: ox F: oz :}
    g 0 area-corner drop  to az to ay to ax   g 1 area-corner drop  to bz fdrop to bx
    bz az f- to ox  bx ax f- fnegate to oz   ox ox f* oz oz f* f+ fsqrt 4e fswap f/
    fdup ox f* side s>f f* to ox  oz f* side s>f f* to oz
    0  ax bx f+ 2e f/ ox f-  ay  az bz f+ 2e f/ oz f-  put
    0  ax bx f+ 2e f/ ox f+  ay  az bz f+ 2e f/ oz f+  put  10 run-frames ;
: hacross ( gate side -- )   \ (Hewie stepped over the gate's line, as across)
    {: g side | F: ax F: ay F: az F: bx F: bz F: ox F: oz :}
    g 0 area-corner drop  to az to ay to ax   g 1 area-corner drop  to bz fdrop to bx
    bz az f- to ox  bx ax f- fnegate to oz   ox ox f* oz oz f* f+ fsqrt 4e fswap f/
    fdup ox f* side s>f f* to ox  oz f* side s>f f* to oz
    0  ax bx f+ 2e f/ ox f-  ay  az bz f+ 2e f/ oz f-  hput
    0  ax bx f+ 2e f/ ox f+  ay  az bz f+ 2e f/ oz f+  hput  10 run-frames ;
: hinto ( area -- )   \ (a gate: crossed one way - the one that gives him a camera, if either does - else back too)
    dup area-kind if  0 hat exit  then
    game:hewie 99 99 cam-char-setup  dup 1 hacross  his-camera@ 99 = swap 99 = and if  -1 hacross  else  drop  then ;
: into ( area -- )   \ (a gate crossed both ways; else its middle)
    dup area-kind 0= if  dup 1 across  -1 across  else  0 at  then ;
: leads ( room exit -- room' )  room-exit-leads drop ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
clock-start  30 run-frames   \ (the run: Hewie the one played)
: by ( exit -- )  off-map-d5 swap room-by!  60 run-frames ;
: out-by ( exit -- room )  dup by  0 hat  [: room-id off-map-d5 <> ;] 300 soon drop  room-id ;
: wisp? ( -- flag )  [: wisp-slot figure 0< 0= ;] in-place ;
: rabbit? ( -- flag )  [: rabbit-slot figure 0< 0= ;] in-place ;
x-d5-flag1 story-flag-set
testing in by exit 0 (camera 0), 1 (3), 2 (1), 3, 4 (2): at it
T{ 0 by  room-id  his-camera@ -> off-map-d5 0 0 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
T{ 1 by  his-camera@  2 by  his-camera@  3 by  his-camera@  4 by  his-camera@ -> 3 3 1 1 2 2 2 2 }T
testing its cameras: area $A / $C set 3, $B set 1, $D set 2, $E set 0 ($F's, set 3: no corners in its data)
T{ $A hinto  his-camera@  $B hinto  his-camera@  $D hinto  his-camera@  $E hinto  his-camera@ -> 3 3 1 1 2 2 0 0 }T
T{ $C hinto  his-camera@  $F 0 area-corner >r fdrop fdrop fdrop r> -> 3 3 0 }T
testing the room behind exit 0 .. 4 loaded ahead as Hewie crosses into area 5 .. 9
T{ 0 by  5 hinto  spare@ -> off-map-d5 0 leads }T
T{ 6 hinto  spare@ -> off-map-d5 1 leads }T
T{ 7 hinto  spare@ -> off-map-d5 2 leads }T
T{ 8 hinto  spare@ -> off-map-d5 3 leads }T
T{ 9 hinto  spare@ -> off-map-d5 4 leads }T
testing a rabbit now and then (3 in 10): in 30 visits some, not every one
variable rabbits  0 rabbits !
: visits ( -- )  30 0 do  0 by  rabbit? if  1 rabbits +!  then  loop ;
T{ visits  rabbits @ 0>  rabbits @ 30 < -> -1 -1 }T
testing the wisp (not yet startled): hovering; Hewie nearing it - startled, its flag set, gone
x-d5-flag1 story-flag-clear  off-map-d4 0 room-by!  30 run-frames  1 by   \ (by exit 1: away from it - by exit 0 he's near enough to startle it)
T{ wisp?  x-d5-flag1 story-flag? -> -1 0 }T
T{ 0 -24e 8.31e -132e hput  [: x-d5-flag1 story-flag? ;] 60 soon  [: wisp? 0= ;] 200 soon -> -1 -1 }T
testing ... once startled, not there again
T{ off-map-d4 0 room-by!  30 run-frames  1 by  wisp? -> 0 }T
testing out by each exit from its own area
T{ 0 out-by -> off-map-d5 0 leads }T
T{ 1 out-by -> off-map-d5 1 leads }T
T{ 2 out-by -> off-map-d5 2 leads }T
T{ 3 out-by -> off-map-d5 3 leads }T
T{ 4 out-by -> off-map-d5 4 leads }T
0 hewie-played !
test-summary
