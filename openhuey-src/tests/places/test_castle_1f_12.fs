\ castle-1f-12's place (scripts/story/places/castle-1f-12.fs): the moon dials' room - the dials set
\ and turning, the lens's reflection and glint, the wall's remarks, a dial turned by the stick, the
\ device with its refractor missing, the Lunar Refractor set (the scene), the dials right (out to
\ castle-2f-1), the moonlight back here (the doors open), the vase's Medallion, the locked door,
\ the replays. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_1f_12.fs
IN: test-castle-1f-12
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-1f-12 doors items.names ;

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
: room-afresh ( -- )  castle-1f-12 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;

: dial-2-turn ( F: -- deg )  s" syuukouki2" turn-y object@ rad>deg ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
: barred? ( group -- flag )
    nav-tris 0 ?do  i over nav-in-group? if  drop i nav-flags barred-floor and 0<> unloop exit  then  loop  drop false ;
: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
create spot 12 allot
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the doors shut (their floor barred), the device's lens there (no refractor)
c1f-12-flag1 story-flag-clear  lunar-refractor-used story-flag-clear  c1f-12-breakable-broken story-flag-clear
0 0 var!  0 1 var!  room-afresh
T{ doors-floor barred?  lens-part part? -> -1 -1 }T

testing the dials: set from their variable (3: 0 degrees), turning a degree a frame to 5 (60)
3 0 var!
T{ [: dial-2 dial-base dial-set ;] in-place  dial-2-turn 0e ~= -> -1 }T
5 0 var!
: turns ( -- n )  [: 0  begin  1+  dial-2 dial-base dial-turns  over 200 > or  until ;] in-place ;
T{ turns 60 - abs 2 <  dial-2-turn 60e f- fabs 1.5e f< -> -1 -1 }T
0 0 var!

testing the lens: her reflection in it (an effect), gone; its glint
T{ [: lens-fx $60 0 cast-model s" lens" 640e -560e 1000e object-mirror  lens-fx effect @ 0<> ;] in-place -> -1 }T
T{ [: lens-fx effect-off  lens-fx effect @ ;] in-place -> 0 }T
T{ [: s" syuukouki1" 0e catches-light ;] in-place  2 run-frames  s" window-flash" actor-named 0< -> 0 }T
full-suite   \ (on: waits, scenes, walks - the full run's)

testing the wall: its remarks in turn
T{ wall-area -45 at  said  wall-area -45 at  said -> wall-msg moon-msg }T

testing the second dial turned by the stick: one number on, then done
room-afresh  dial-1-area 180 at  circle press
T{ [: window-shown device-msg = ;] 300 soon -> -1 }T
: past-remark ( -- )  begin  window-shown device-msg = while  cross press  10 run-frames  repeat ;
past-remark  [: window-shown rotate-msg = ;] 300 soon drop  key: Right tap  120 run-frames  cross press  30 run-frames
T{ 1 var@ -> 1 }T

testing the first device without its refractor: something missing from it
T{ dial-area -90 at  said -> missing-msg }T

testing the Lunar Refractor set in it: the scene - spent, the lens away
lunar-refractor 1 items send give-item  10 run-frames
dial-area -90 at  4 use-on-place  900 play-on
T{ lunar-refractor-used story-flag?  lunar-refractor items:items-of  lens-part part? -> -1 0 0 }T

testing the vase broken: its Medallion picked up
c1f-12-medallion story-flag-clear  c1f-12-breakable-broken story-flag-set  room-afresh
T{ [: medallion-fx effect @ 0<> ;] in-place -> -1 }T
medallion items:items-of before !
-73.5e -103e 55.5e picked
T{ c1f-12-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T

testing the door locked from this side (exit 0): before it, the button - unlocked
variable the-door  [: locked-exit exit-door ;] in-place the-door !
the-door @ s" doors" actor-named send lock  room-afresh
0 0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames
T{ circle press  [: window-shown unlocked-msg = ;] 300 soon -> -1 }T
300 read-on
T{ the-door @ door-locked? -> 0 }T

testing the dials right: out to castle-2f-1 (the moon let through there)
5 0 var!  2 1 var!  3 3 var!  room-afresh  dial-1-area 180 at  circle press  60 read-on  cross press
T{ [: room-id castle-2f-1 = ;] 900 soon -> -1 }T
free-play  10 run-frames  0 game:fiona send costume

testing the moonlight back here (castle-2f-1's dial unset there): the moon door unlocked, the doors open
c1f-12-flag1 story-flag-clear  castle-1f-12 forced-exit 1 or rooms send go-to-room  5 run-frames
T{ [: movie-skipped state-flag-set  c1f-12-flag1 story-flag? ;] 1500 soon -> -1 }T
120 play-on
T{ moon-door doors:door-locked?  s" autodoor0" turn-y object@ rad>deg -75e ~=  doors-floor barred? -> 0 -1 0 }T
free-play  10 run-frames  0 game:fiona send costume

testing the movies replayed (the extras): played, then back by the forced way
events-held state-flag-set  castle-1f-12 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
