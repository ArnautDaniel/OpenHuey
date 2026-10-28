\ off-map-35's place: by the first forced way: "Come on, boy!", on to room $E; by the second -
\ Hewie's file found: "Hewie!", on to $36; else "Wait up, boy!" and the ending B; each replayed. Headless:
\   build/openhuey --test tests/places/test_off_map_35.fs
IN: test-off-map-35
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names gameover story.places.off-map-35 ;
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
: replay-for ( act -- )   \ (the theatre's variable set to a movie its room replays by that act)
    {: act :}  110 0 do  i $2A var!  [: replay-act ;] in-place act = if  unloop exit  then  loop ;
: over-kind ( -- kind )  game:gameover enter  gameover:kind @  leave-actor ;
: played-till ( xt n -- flag )   \ (played on - its messages read, its movies skipped - till xt holds)
    0 ?do  dup execute if  drop true unloop exit  then
        window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop
    drop false ;
: quiet ( -- )  events-held state-flag-clear  off-map-0d 1 room-by!  30 run-frames  world-held state-flag-set ;   \ (between replays: a room of no scenes)
variable before
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing by the first forced way: "Come on, boy!" - on to room $E
world-held state-flag-set
off-map-35 forced-exit room-by!  5 run-frames
T{ doing-busy?  [: room-id off-map-35 <> ;] 1500 played-till  room-id -> -1 -1 $E }T
testing by the second, Hewie's file found: "Hewie!" - on to $36
found-file-20b story-flag-set  world-held state-flag-set
T{ off-map-35 $81 room-by!  5 run-frames  [: room-id off-map-35 <> ;] 1500 played-till  room-id -> -1 off-map-36 }T
testing ... not found: "Wait up, boy!" - the story's end (ending B)
found-file-20b story-flag-clear  world-held state-flag-set  0 $2D var!
off-map-35 $81 room-by!
T{ [: caught state-flag? ;] 1500 played-till  $2D var@  over-kind -> -1 1 2 }T
testing replayed: "Come on" (act 6) and "Hewie!" (4) on to the next room's replay
caught state-flag-clear  0 game:gameover send game-over-kind  quiet
events-held state-flag-set  6 replay-for  0 $2C var!
T{ off-map-35 forced-exit room-by!  5 run-frames  [: room-id off-map-35 <> ;] 1500 played-till  room-id  $2C var@  events-held state-flag? -> -1 $E 1 -1 }T
quiet  events-held state-flag-set  4 replay-for  0 $2C var!
T{ off-map-35 forced-exit room-by!  5 run-frames  [: room-id off-map-35 <> ;] 1500 played-till  room-id  $2C var@ -> -1 off-map-36 1 }T
testing ... "Wait up" (5): the replay over, the theatre let go - back by the forced way to the room it came from
quiet  events-held state-flag-set  5 replay-for
T{ off-map-35 forced-exit room-by!  5 run-frames  [: room-id off-map-35 <> ;] 1500 played-till  room-id  events-held state-flag? -> -1 off-map-0d 0 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
test-summary
