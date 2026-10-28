\ castle-2f-10's place (scripts/story/places/castle-2f-10.fs): the room of the four boxes - coming in (the boxes kept), the four on their squares (the way opened), leaving (kept), the replay. Headless:
\   build/openhuey --test tests/places/test_castle_2f_10.fs
IN: test-castle-2f-10
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-2f-10 doors items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: room-afresh ( -- )  castle-2f-10 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: replayed? ( -- flag )
    [: cutscene-active? movie-status 1 = or ;] 200 soon
    [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon and ;


: placed? ( box -- flag )  obstacle-placed? ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the four boxes where they were kept
room-afresh
T{ 0 placed?  3 placed? -> -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)

testing leaving: the boxes kept where they stand (out by exit 0)
variable the-door  room-afresh  0 exit-door the-door !   \ (its door open)
the-door @ s" doors" actor-named send unlock  the-door @ true s" doors" actor-named send door-opened  10 run-frames  to-exit-0-area 0 at
T{ [: room-id castle-2f-10 <> ;] 300 soon -> -1 }T
free-play  10 run-frames  0 game:fiona send costume

testing the four on their squares: fixed, the scene - the way opened, out by the forced way
$1C s" doors" actor-named send lock  $1D s" doors" actor-named send lock
[: 0 0 0 $455 $CA5 obstacle-in  1 1 0 $459 $CCD obstacle-in  2 2 0 $473 $CE7 obstacle-in  3 3 0 $4B8 $D2C obstacle-in ;] in-place
T{ [: movie-skipped state-flag-set  room-id castle-2f-10 <> ;] 1500 soon  $1C door-locked?  $1D door-locked? -> -1 0 0 }T
free-play  10 run-frames  0 game:fiona send costume

testing the movie replayed (the extras)
events-held state-flag-set  castle-2f-10 -1 rooms send go-to-room  5 run-frames
T{ replayed? -> -1 }T
test-summary
