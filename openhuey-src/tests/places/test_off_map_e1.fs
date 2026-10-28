\ off-map-e1's place (the clock run, Hewie played): in by exit 1 (camera 0), 2 (4, path 3); the
\ flames lit; the day's light only in the run's second kind; its cameras; the room behind exit 2
\ loaded ahead as Hewie leaves area 4 or enters 5, exit 1's as he leaves 6; out by either. Headless:
\   build/openhuey --test tests/places/test_off_map_e1.fs
IN: test-off-map-e1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.clock story.places.off-map-e1 ;
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
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
variable ex
: opened ( exit -- )  ex !  [: ex @ exit-door 1 doors-actor send door-opened ;] in-place  5 run-frames ;
: both-ways ( area -- )  dup 1 hacross  -1 hacross ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: command ( -- )  key: I tap ;   \ (his gesture: up)
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: stalker-here ( room tri -- )  debilitas-kind stalker-ready  30 run-frames  game:debilitas send stalker-in  10 run-frames ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
clock-start  30 run-frames   \ (the run: Hewie the one played)
: by ( exit -- )  off-map-e1 swap room-by!  60 run-frames ;
: out-by ( exit -- room )  dup by  dup opened  exit-area 0 hat  [: room-id off-map-e1 <> ;] 300 soon drop  room-id ;
: lit? ( -- n )  [: 0  3 0 do  i effect @ 0<> -  loop ;] in-place ;
testing in by exit 1 (camera 0), 2 (4, path 3): at it; the three flames lit
T{ 1 by  room-id  his-camera@  lit? -> off-map-e1 0 0 3 }T
T{ 2 by  his-camera@ -> 4 3 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing its cameras: area 7 set 2, 8 / $A set 1, 9 / $D set 0, $16 set 4 (path 3)
T{ 7 hinto  his-camera@  8 hinto  his-camera@  9 hinto  his-camera@ -> 2 2 1 1 0 0 }T
T{ $16 hinto  his-camera@  $A hinto  his-camera@  $D hinto  his-camera@ -> 4 3 1 1 0 0 }T
testing the room behind exit 2 loaded ahead (out of area 4, into area 5), exit 1's (out of area 6)
T{ 1 by  5 hinto  spare@ -> off-map-e1 2 leads }T
T{ 6 hinto  spare@ -> off-map-e1 1 leads }T
T{ 4 1 hacross  4 -1 hacross  spare@ -> off-map-e1 2 leads }T   \ (both ways: a camera's gate by it)
testing out by exit 1, by exit 2 - their doors shut (the run's start shut them): not till one is open
T{ 1 by  1 exit-area 0 hat  60 run-frames  room-id -> off-map-e1 }T
T{ 1 out-by -> off-map-e1 1 leads }T
T{ 2 out-by -> off-map-e1 2 leads }T
0 hewie-played !
test-summary
