\ off-map-e5's place (the clock run, Hewie played): in by exit 0 (placed at it, camera 0); the
\ curtain as it hangs; its cameras; the curtain to hide behind - Fiona crawls in at Hewie's command
\ as he stands by it (drawn across), out at his next (as it hangs again); the stalker coming in
\ while she hides searches (counted); left behind there, she's out; out by exit 0, its door open.
\ Headless:
\   build/openhuey --test tests/places/test_off_map_e5.fs
IN: test-off-map-e5
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.clock story.places.off-map-e5 ;
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
variable ex
: opened ( exit -- )  ex !  [: ex @ exit-door 1 doors-actor send door-opened ;] in-place  5 run-frames ;
: both-ways ( area -- )  dup 1 hacross  -1 hacross ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: command ( -- )  key: I tap ;   \ (his gesture: up)
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: stalker-here ( room tri -- )  debilitas-kind stalker-ready  30 run-frames  game:debilitas send stalker-in  10 run-frames ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
clock-start  30 run-frames   \ (the run: Hewie the one played)
: by ( exit -- )  off-map-e5 swap room-by!  60 run-frames ;
: by-it ( -- )  0 62.08e 0e -16.33e hput ;   \ (Hewie in front of it)
: her-here ( -- )  0 50e 0e -5e put ;
: curtain-t ( F: -- t )  [: s" Cartain" $7C object@ ;] in-place ;   \ (its morph's blend: +$7C)
testing in by exit 0: at it, camera 0; the curtain as it hangs
T{ 0 by  room-id  his-camera@  curtain-t f0= -> off-map-e5 0 0 -1 }T
testing its cameras: area 5 / 6 set 0, 7 set 2, 8 set 3, 9 set 1; area 4 crossed out of - set 0
T{ 7 hinto  his-camera@  8 hinto  his-camera@  9 hinto  his-camera@  5 hinto  his-camera@ -> 2 -1 3 -1 1 -1 0 0 }T
T{ 7 hinto  6 hinto  his-camera@ -> 0 0 }T
testing behind the curtain: he by it, she here and free - at his command she crawls in, it drawn across
0 by  her-here  by-it
T{ [: fiona here? ;] in-place  hidden? -> -1 0 }T
T{ command  [: hidden? ;] 800 soon  curtain-t 1e f= -> -1 -1 }T
testing ... at his next command, out again - it hangs
60 run-frames
T{ command  [: hidden? 0= ;] 300 soon  [: doing-busy? 0= ;] 300 soon  curtain-t f0= -> -1 -1 -1 }T
testing the stalker coming in while she hides: he searches (counted)
her-here  by-it  command  [: hidden? ;] 800 soon drop
0 $32 var!  off-map-e5 $10 stalker-here
T{ [: came-while-hidden @ ;] in-place  $32 var@ -> -1 1 }T
[: stalker-unloaded ;] in-place  10 run-frames
testing left behind there: he leaves by exit 0 - she's out
T{ 0 opened  0 exit-area 0 hat  [: room-id off-map-e5 <> ;] 300 soon  hidden?  stalkers-stay state-flag? -> -1 0 0 }T
testing out by exit 0 (its door open)
T{ 0 by  0 opened  0 exit-area 0 hat  [: room-id off-map-e5 <> ;] 300 soon  room-id -> -1 off-map-e5 0 leads }T
0 hewie-played !
test-summary
