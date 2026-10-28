\ off-map-d7's place (the clock run, Hewie played): in by exit 0 (placed at it, its camera); the
\ wisp hovering - never startled here, not even by a loud noise (the original's hover never marks
\ its scene begun); gone as he leaves; out by exit 0. Headless:
\   build/openhuey --test tests/places/test_off_map_d7.fs
IN: test-off-map-d7
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.clock story.places.forest story.places.off-map-d7 ;
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
: by ( exit -- )  off-map-d7 swap room-by!  60 run-frames ;
: wisp? ( -- flag )  [: wisp-slot figure 0< 0= ;] in-place ;
: loud ( -- )  $40 room-id broadcast noise-was  2 run-frames ;
x-d7-flag1 story-flag-clear  x-d5-flag1 story-flag-set  x-d8-flag1 story-flag-set   \ (the rooms beyond: no wisp of theirs)
testing in by exit 0: at it, its camera (set 0); the wisp hovering
T{ 0 by  room-id  his-camera@  wisp?  [: wisp-up @ ;] in-place -> off-map-d7 0 0 -1 -1 }T
testing a loud noise: the wisp stays, its flag unset
T{ loud  100 run-frames  wisp?  x-d7-flag1 story-flag? -> -1 0 }T
testing ... once its flag is set: no wisp
T{ x-d7-flag1 story-flag-set  off-map-d6 0 room-by!  30 run-frames  0 by  wisp? -> 0 }T
x-d7-flag1 story-flag-clear  x-d5-flag1 story-flag-set  x-d8-flag1 story-flag-set   \ (the rooms beyond: no wisp of theirs)
testing out by exit 0 from its area: the wisp gone with the room
T{ off-map-d6 0 room-by!  30 run-frames  0 by  0 0 hat  [: room-id off-map-d7 <> ;] 300 soon  room-id -> -1 off-map-d7 0 leads }T
T{ [: 5 figure 0< ;] in-place -> -1 }T
0 hewie-played !
test-summary
