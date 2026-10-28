\ off-map-d0's place (the clock run's goal in the forest, Hewie played): in by exit 1 (placed
\ at it, the camera); its cameras; the room beyond exit 1 loaded ahead as Hewie nears it; out by
\ exit 1; Fiona reaching the goal - the time kept (the clock shown frozen), the results, out to the
\ extras room by its second forced way, the run over (Fiona played again). Headless:
\   build/openhuey --test tests/places/test_off_map_d0.fs
IN: test-off-map-d0
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.clock floor-flags subscreen story.places.off-map-d0 ;
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
: by ( exit -- )  off-map-d0 swap room-by!  60 run-frames ;
testing in by exit 1: at it, its camera (set 0) - exit 0 (camera 3) isn't in the room's data: no area, no spot
T{ 1 by  room-id  his-camera@  0 exit-area -> off-map-d0 0 0 $FFFF }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing its cameras: area 2 set 0, 3 set 1, 5 set 2, 6 set 4, 7 set 5, 8 set 3
T{ 2 hinto  his-camera@  3 hinto  his-camera@  5 hinto  his-camera@ -> 0 0 1 1 2 2 }T
T{ 6 hinto  his-camera@  7 hinto  his-camera@  8 hinto  his-camera@ -> 4 4 5 5 3 3 }T
testing two floor triangles let through ($16F, $174)
T{ $16F nav-flags floor-flags:solid-floor and  $174 nav-flags floor-flags:solid-floor and -> 0 0 }T
testing the room beyond exit 1 loaded ahead as he crosses into area 2
T{ 1 by  2 hinto  spare@ -> off-map-d0 1 leads }T
testing out by exit 1 from area 1
T{ 1 by  1 0 hat  [: room-id off-map-d0 <> ;] 300 soon  room-id -> -1 off-map-d0 1 leads }T
testing Fiona reaching the goal: the time kept; the results; out to the extras room, Fiona played
1 by  $10 0 at   \ (she put there: the goal's area)
T{ [: [: reached @ ;] in-place ;] 60 soon  run-took @ 0> -> -1 -1 }T
T{ [: subscreen-wanted state-flag? ;] 300 soon -> -1 }T
: menus-shut ( -- )  20 0 do  subscreen-wanted state-flag? if  cross press  then  30 run-frames  loop ;
T{ menus-shut  [: room-id off-map-37 = ;] 600 soon  hewie-played? -> -1 0 }T
test-summary
