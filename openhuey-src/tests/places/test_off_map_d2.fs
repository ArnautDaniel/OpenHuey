\ off-map-d2's place (the clock run, Hewie played): in by any of its four ways (placed at it, the
\ camera); out by each from its own area; the rooms behind the first three loaded ahead as Hewie nears them (exit 3's area 7 isn't in the room's data);
\ the clock. Headless:
\   build/openhuey --test tests/places/test_off_map_d2.fs
IN: test-off-map-d2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.clock story.places.off-map-d2 ;
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
: by ( exit -- )  off-map-d2 swap room-by!  60 run-frames ;
: out-by ( exit -- room )  dup by  0 hat  [: room-id off-map-d2 <> ;] 300 soon drop  room-id ;
testing in by exit 0, 1 (camera set 1), 2, 3 (set 0): at it
T{ 0 by  room-id  his-camera@ -> off-map-d2 1 1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
T{ 1 by  his-camera@  2 by  his-camera@  3 by  his-camera@ -> 1 1 0 0 0 0 }T
testing the room behind exit 0 .. 2 loaded ahead as Hewie crosses into area 4 .. 6
T{ 0 by  4 hinto  spare@ -> off-map-d2 0 leads }T
T{ 5 hinto  spare@ -> off-map-d2 1 leads }T
T{ 6 hinto  spare@ -> off-map-d2 2 leads }T
testing ... exit 3's never: the room's area 7 has no corners in its data - Hewie never crosses it
T{ 7 0 area-corner >r fdrop fdrop fdrop r>  7 hinto  spare@ -> 0 off-map-d2 2 leads }T
testing out by each exit from its own area
T{ 0 out-by -> off-map-d2 0 leads }T
T{ 1 out-by -> off-map-d2 1 leads }T
T{ 2 out-by -> off-map-d2 2 leads }T
T{ 3 out-by -> off-map-d2 3 leads }T
0 hewie-played !
test-summary
