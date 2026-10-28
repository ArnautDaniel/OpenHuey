\ off-map-d6's place (the clock run, Hewie played): in by any of its four ways (placed at it, the
\ camera); out by each from its own area; the rooms behind exits 0, 2, 3 loaded ahead as Hewie
\ nears them; the bush - Fiona (come by exit 1) crouches in it at his command as he stands by it,
\ out at his next; the stalker coming in by her side while she hides searches (counted); the
\ leaves swing as Hewie pushes through; left behind in it, she's out at its front. Headless:
\   build/openhuey --test tests/places/test_off_map_d6.fs
IN: test-off-map-d6
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.clock floor-flags story.places.forest story.places.off-map-d6 ;
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
: by ( exit -- )  off-map-d6 swap room-by!  60 run-frames ;
: out-by ( exit -- room )  dup by  0 hat  [: room-id off-map-d6 <> ;] 300 soon drop  room-id ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: command ( -- )  key: I tap ;   \ (his gesture: up)
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: her-here ( -- )  0 75e 0e -10e put ;   \ (Fiona by the bush, free)
testing in by exit 0 (camera 0), 1 (1), 2, 3 (0): at it
T{ 0 by  room-id  his-camera@ -> off-map-d6 0 0 }T
T{ 1 by  his-camera@  2 by  his-camera@  3 by  his-camera@ -> 1 1 0 0 0 0 }T
testing the room behind exit 0, 2, 3 loaded ahead as Hewie crosses into area 4, 6, 7 (area 5: none)
T{ 1 by  4 hinto  spare@ -> off-map-d6 0 leads }T
T{ 6 hinto  spare@  5 hinto  spare@ -> off-map-d6 2 leads  off-map-d6 2 leads }T
T{ 7 hinto  spare@ -> off-map-d6 3 leads }T
testing the leaves: still as he comes in; swinging as Hewie pushes through
T{ 1 by  [: 0 hanging swing-left object@ ;] in-place f0> -> 0 }T
: swinging? ( -- flag )  [: false  3 0 do  i hanging swing-left object@ f0> or  loop ;] in-place ;
: through ( -- flag )   \ (a step of 1 at a time through its middle: any swinging on the way)
    false  8 0 do  0 70.5e 0e i s>f -9e f+ hput  swinging? or  loop ;
T{ through -> -1 }T
testing the bush: Hewie by it, she free and here - at his command she crouches in it, hidden
1 by  her-here  0 70.5e 0e -5.5e hput
T{ [: fiona here? ;] in-place  hidden? -> -1 0 }T
T{ command  [: hidden? ;] 400 soon -> -1 }T
testing ... at his next command, out again
60 run-frames
T{ command  [: hidden? 0= ;] 200 soon  [: doing-busy? 0= ;] 200 soon -> -1 -1 }T
testing ... not without his command; not by exit 0's way
T{ 0 70.5e 0e -5.5e hput  100 run-frames  hidden? -> 0 }T
0 by  her-here  0 70.5e 0e -5.5e hput
T{ command  100 run-frames  hidden? -> 0 }T
testing the stalker coming in by her side while she hides: counted (he searches by the odds)
1 by  her-here  0 70.5e 0e -5.5e hput  command  [: hidden? ;] 400 soon drop
0 $2F var!  debilitas-kind stalker-ready  30 run-frames  off-map-d6 $10 game:debilitas send stalker-in  10 run-frames
: by-her-side ( -- )  [: 1 st-came-now !  searched ;] in-place  5 run-frames ;   \ (as he tells he came by exit 1)
T{ hidden?  by-her-side  [: came-while-hidden @ ;] in-place  $2F var@ -> -1 -1 1 }T
testing ... not by the other side: no search
T{ [: 0 st-came-now !  searched ;] in-place  [: came-while-hidden @ ;] in-place  $2F var@ -> 0 1 }T
[: stalker-unloaded ;] in-place  10 run-frames
testing left behind in it: he leaves by exit 0 - she's out, at its front
T{ 0 0 hat  [: room-id off-map-d6 <> ;] 300 soon  hidden?  stalkers-stay state-flag? -> -1 0 0 }T
testing out by each exit from its own area
T{ 0 out-by -> off-map-d6 0 leads }T
T{ 1 out-by -> off-map-d6 1 leads }T
T{ 2 out-by -> off-map-d6 2 leads }T
T{ 3 out-by -> off-map-d6 3 leads }T
0 hewie-played !
test-summary
