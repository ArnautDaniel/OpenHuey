\ off-map-0d's place: in by exit 1 (placed at it, its camera); out by it from area 0; her camera by
\ the floor she stands on. Headless:
\   build/openhuey --test tests/places/test_off_map_0d.fs
IN: test-off-map-0d
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.places.off-map-0d ;
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
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing in by exit 1: at it, its camera (set 5); the reverb
off-map-0d 1 room-by!  60 run-frames
T{ room-id  her-camera@ -> off-map-0d 5 -1 }T
testing her camera by the floor: $BB / $1C7 set 2, $1D0 / $E0 set 4, $1F1 / $1ED / $EA set 5
T{ $BB on-tri  her-camera@  $1C7 on-tri  her-camera@ -> 2 -1 2 -1 }T
T{ $1D0 on-tri  her-camera@  $E0 on-tri  her-camera@ -> 4 -1 4 -1 }T
T{ $BB on-tri  $1F1 on-tri  her-camera@  $BB on-tri  $1ED on-tri  her-camera@  $BB on-tri  $EA on-tri  her-camera@ -> 5 -1 5 -1 5 -1 }T
testing out by exit 1 from area 0
T{ 0 0 at  [: room-id off-map-0d <> ;] 300 soon -> -1 }T
test-summary
