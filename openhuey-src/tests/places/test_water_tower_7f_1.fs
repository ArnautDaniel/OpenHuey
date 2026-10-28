\ water-tower-7f-1's place: coming in - Saturn's glow lit, hunted; the plates lit in turn while the
\ one before is stood on, and dark again down to the last stood on; Hewie sits on a plate he reaches,
\ and is sent to the next one she points him at; all lit - the scene (the way on opened, the door
\ behind closed off, on to 7F-2); the clock; the inscription; leaving - no longer hunted; the replay.
\ Headless:
\   build/openhuey --test tests/places/test_water_tower_7f_1.fs
IN: test-water-tower-7f-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.water-tower-7f-1 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )   \ (on the floor, on a triangle she walks - else the nearest the mesh has)
    on-floor s>f tp-facing  [: her-at $28020018 v-tri-in  dup 0< if  drop her-at vec@ nav-tri  then  her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: shown-soon ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
: said ( -- msg )
    circle press  shown-soon
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
create spot 12 allot
: by-door ( exit -- )  0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames ;
variable before
: room-afresh ( -- )  water-tower-7f-1 1 rooms send go-to-room  60 run-frames  0 0e 0e -60e put ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: answer-no ( -- )  60 run-frames  cross press  30 run-frames  key: Down tap  10 run-frames  cross press  100 run-frames ;
: lit? ( k -- flag )  [: live-effect nip ;] in-place ;
: geared ( -- k )  [: gear @ ;] in-place ;
: her-on ( k -- )  [: plate@ ;] in-place  0e fswap  0 put ;   \ (on that planet's plate)
: off-plates ( -- )  0 -90e 0e 0e put ;
: keeper ( -- )  hewie hold  begin  yield  again ;   \ (Hewie kept where he's put)
: keep-him ( -- )  [: ['] keeper running? 0= if  ['] keeper start drop  then ;] in-place  2 run-frames ;
: free-him ( -- )  [: ['] keeper stop-doing ;] in-place  2 run-frames ;
variable which
: hewie-on ( k -- )  which !  keep-him  [: hewie  which @ runs pair@ drop  0  which @ plate@  place-at ;] in-place  10 run-frames ;
: hewie-off ( -- )  keep-him  [: hewie $90 0 place-on-tri ;] in-place  10 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  game:hewie send join-fiona  60 run-frames
testing coming in: Saturn's glow lit, the next to light Uranus's; hunted
T{ 8 lit?  7 lit?  geared  hunted state-flag? -> -1 0 8 -1 }T
testing she on Saturn's plate: Uranus's lit; Hewie on it: Neptune's ... (each lit while the one before is stood on)
T{ 8 her-on  geared  7 lit? -> 7 -1 }T
T{ 7 hewie-on  geared  6 lit? -> 6 -1 }T
T{ 6 her-on  geared  5 lit? -> 5 -1 }T
testing ... Hewie sits on the plate he reaches
T{ free-him  [: 7 sat? ;] in-place -> -1 }T
testing the last lit left: dark again down to the highest still stood on
T{ off-plates  geared -> 6 }T
T{ hewie-off  geared -> 8 }T
testing Hewie sent to the next plate as she points him at it
free-him  8 her-on  geared drop  -176 57.25e 0e 57.25e put
T{ geared  [: go-there-cmd started-now !  sends-him  ['] runs-to running? ;] in-place -> 7 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
T{ [: [: ['] runs-to running? 0= ;] in-place ;] 600 soon  [: his-plate @ ;] in-place  geared -> -1 7 6 }T
testing ... not when she's facing away
off-plates  hewie-off  free-him  60 run-frames  8 her-on  4 57.25e 0e 57.25e put
T{ [: go-there-cmd started-now !  sends-him  ['] runs-to running? ;] in-place -> 0 }T
testing all lit: the scene - the way on opened, the door behind closed off; on to 7F-2
1 her-on  free-him
T{ [: 1 gear !  2 his-plate !  1 her-plate !  lights  lined-up @ ;] in-place -> -1 }T
T{ [: movie-skipped state-flag-set  solved-seen resident-flag? ;] 2000 soon -> -1 }T
T{ [: room-id water-tower-7f-2 = ;] 600 soon  way-on-door doors:door-locked?  behind-door doors:door-locked? -> -1 0 -1 }T
300 play-on
game:hewie send part-from-fiona  room-afresh
testing leaving: no longer hunted
T{ hunted state-flag?  water-tower-1f-1 room!  60 run-frames  hunted state-flag? -> -1 0 }T
room-afresh
testing the clock: saving (or Hewie not by her) - no
T{ clock-area 0 at  circle press  shown-soon  dup $8014 =  swap $8024 = or -> -1 }T
answer-no
T{ doing-busy? -> 0 }T
testing the inscription
T{ inscription-area 0 at  [: fiona inscription-area 50 faces-area? ;] in-place -> -1 }T
T{ said -> inscription-msg }T
testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  water-tower-7f-1 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon -> -1 }T
test-summary
