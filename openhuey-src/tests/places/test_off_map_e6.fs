\ off-map-e6's place (the clock run, Hewie played): in by either way (placed at it, camera 0); the barred floor;
\ the lattice down; its cameras (5, 6 crossed out of); the rooms ahead loaded as Hewie nears their
\ ways (out of area 2, into 3); out by exit 0 (its door open), by exit 1 from area 1. Headless:
\   build/openhuey --test tests/places/test_off_map_e6.fs
IN: test-off-map-e6
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.clock story.places.off-map-e6 ;
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
: by ( exit -- )  off-map-e6 swap room-by!  60 run-frames ;
testing in by exit 0, 1: at it, camera 0; the lattice down
T{ 0 by  room-id  his-camera@  [: s" kousi" place-y object@ ;] in-place -25.3e f- fabs 0.001e f< -> off-map-e6 0 0 -1 }T
T{ 1 by  his-camera@ -> 0 0 }T
testing the barred floor (group 0); part 0 shown, 1 not
T{ [: 0 part?  1 part? ;] in-place -> -1 0 }T
testing its cameras: area 4 / 7 set 0; 5, 6 crossed out of - set 1
T{ 5 both-ways  his-camera@  4 hinto  his-camera@  6 both-ways  his-camera@  7 hinto  his-camera@ -> 1 -1 0 0 1 -1 0 0 }T
testing the rooms ahead: out of area 2 exit 0's, into area 3 exit 1's
T{ 0 by  3 hinto  spare@ -> off-map-e6 1 leads }T
T{ 2 both-ways  spare@ -> off-map-e6 0 leads }T
testing out by exit 0 (its door open); by exit 1 from area 1
T{ 0 by  0 opened  0 exit-area 0 hat  [: room-id off-map-e6 <> ;] 300 soon  room-id -> -1 off-map-e6 0 leads }T
T{ 1 by  1 0 hat  [: room-id off-map-e6 <> ;] 300 soon  room-id -> -1 off-map-e6 1 leads }T
0 hewie-played !
test-summary
