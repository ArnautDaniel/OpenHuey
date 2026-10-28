\ castle-1f-7's place (scripts/story/places/castle-1f-7.fs): the breaker room - the gate shut or
\ held up, falling once let go; the remarks (the gate, the doll in turn); the breaker lowered (the
\ door unlocked); the locked door tried; Hewie barking at the breaker; thrown back in by the forced
\ way; in among the wires: the game over; the replay. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_1f_7.fs
IN: test-castle-1f-7
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-1f-7 doors items.names ;

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
: room-afresh ( -- )  castle-1f-7 room!  60 run-frames ;
: by-exit ( exit -- )  castle-1f-7 swap rooms send go-to-room  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: barred? ( group -- flag )   \ (the first of its triangles barred)
    nav-tris 0 ?do  i over nav-in-group? if  drop i nav-flags barred-floor and 0<> unloop exit  then  loop  drop false ;
: gate-y ( F: -- y )  s" kousi" place-y object@ ;
: filed? ( doc -- flag )
    128 0 do  progress pr.files i 2* + w@ over = if  drop true unloop exit  then  loop  drop false ;
: over@ ( -- kind running? )  game:gameover enter  gameover:kind @  gameover:step @ gameover:not-running <>  leave-actor ;
create spot 12 allot

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in (not by the gate's way): the gate shut, its floor barred; the breaker up, humming
breaker-lowered story-flag-clear  floor-plate-pressed story-flag-clear  gate-rising story-flag-clear  room-afresh
T{ gate-floor barred?  gate-y -25.3e f- fabs 0.01e f<  handle-part part?  spark-part part? -> -1 -1 0 -1 }T

testing the gate up, then falling: settling shut at -25.3, the gate-rising fact cleared on landing
gate-rising story-flag-set
: settles ( -- n )  [: gate-up  0  begin  1+  falls  over 300 > or  until ;] in-place ;
T{ settles 300 <  gate-y -25.3e f- fabs 0.001e f<  gate-rising story-flag? -> -1 -1 0 }T

testing the remarks: the gate shut, the doll in turn
T{ gate-area -90 at  said -> gate-msg }T
full-suite   \ (on: waits, scenes, walks - the full run's)
T{ $A -90 at  said  $B 180 at  said  $E 0 at  said -> doll-msg doll-2-msg doll-3-msg }T

testing the door (exit 0, 9) locked: tried, it won't open
breaker-door s" doors" actor-named send lock  room-afresh
0 0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames
T{ said -> locked-msg }T

testing the breaker: lowered (yes) - the hum gone, the door unlocked, the file only in the other game
T{ breaker-area -90 at  said -> breaker-msg }T
300 read-on
T{ breaker-lowered story-flag?  breaker-door doors:door-locked?  spark-part part?  breaker-file $7FFF and filed? -> -1 0 0 0 }T
T{ breaker-area -90 at  said -> lowered-msg }T

testing the gate held up as she comes by castle-1f-4's way (exit 1, the plate pressed): up, then - nobody on the plate - falling shut
floor-plate-pressed story-flag-set  1 by-exit
T{ [: floor-plate-pressed story-flag? 0= ;] 120 soon  gate-floor barred? -> -1 -1 }T
T{ [: gate-y -25.3e f- fabs 0.01e f< ;] 600 soon  gate-rising story-flag? -> -1 0 }T

testing Hewie, the breaker up: over by it, barking at it
breaker-lowered story-flag-clear  c1f-7-flag1 story-flag-clear  hewie-freed story-flag-set  hewie-commandable state-flag-set
room-afresh  game:hewie send join-fiona  castle-1f-7 room!  60 run-frames
T{ [: c1f-7-flag1 story-flag? ;] 900 soon -> -1 }T
300 run-frames

testing brought back in by the forced way: the gate rattling shut, and back to castle-1f-4
castle-1f-7 forced-exit rooms send go-to-room
T{ [: room-id castle-1f-4 = ;] 900 soon -> -1 }T
free-play  10 run-frames  0 game:fiona send costume

testing in among the wires, the breaker up: the game over (its movie's)
breaker-lowered story-flag-clear  room-afresh
-90 -10e 0e 0e put  -90 -28e 0e 0e put   \ (stepping in among them)
T{ [: over@ nip ;] 3000 soon  over@ drop  $1C resident-flag? -> -1 gameover:movie-death -1 }T
free-play  10 run-frames  0 game:fiona send costume

testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  castle-1f-7 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
