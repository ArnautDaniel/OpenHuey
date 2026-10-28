\ old-mansion-b1-3's place: the plates (stood on: pressed in); two at once put out a flame, side by
\ side all lit again; all four out - the statue's scene; the Venus Key; the lever and the iron
\ maiden (spikes; hidden inside, out); the remarks; the Camomile; the replays. Headless:
\   build/openhuey --test tests/places/test_old_mansion_b1_3.fs
IN: test-old-mansion-b1-3
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-b1-3 ;
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
: room-afresh ( -- )  old-mansion-b1-3 room!  60 run-frames ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: ~= ( F: a b -- flag )  f- fabs 0.01e f< ;
: toward ( area F: px pz -- )
    {: a F: px pz :}  a area-middle drop {: F: x y z :}  px x f-  pz z f-  fatan2 rad>deg f>s  x y z put ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
game:debilitas send stalker-out  5 run-frames
omb1-3-flag1 story-flag-set  omb1-3-flag2 story-flag-clear  omb1-3-flag3 story-flag-clear  got-venus-key story-flag-clear  room-afresh
testing a plate stood on: pressed in
[: fiona $89 0 place-on-tri ;] in-place  20 run-frames
T{ [: plates-on @  s" fumi0" place-y object@ -0.7e ~= ;] in-place -> 1 -1 }T
testing two at once put out a flame; side by side, all lit again
T{ [: 6 plates-on !  flames-puzzle  flames-out @ ;] in-place -> 1 }T
T{ [: 9 plates-on !  flames-puzzle  flames-out @ ;] in-place -> 3 }T
T{ [: 3 plates-on !  flames-puzzle  flames-out @ ;] in-place -> 0 }T
testing all four out: the statue's scene
T{ [: $F flames-out !  omb1-3-flag2 fact!  ['] all-out start drop ;] in-place  [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
[: movie-skipped state-flag-set  doing-busy? 0= ;] 2000 soon drop  200 play-on
T{ doing-busy?  free? -> 0 -1 }T   \ (its file the other game's: not here)
testing the Venus Key from the corpse; then its remarks
4 10e 0e toward  circle press  300 read-on
T{ got-venus-key story-flag?  venus-key items:items-of 0> -> -1 -1 }T
T{ 4 10e 0e toward  said -> drained-msg }T
T{ 4 10e 0e toward  said -> peaceful-msg }T
testing the tool and its engraving
T{ 3 180 at  said -> tool-msg }T
300 play-on
testing the oven, the birdcage, the stretcher, the sabers, the breaker, the tools
T{ 7 110 at  said -> oven-msg }T
T{ 8 90 at  said -> birdcage-msg }T
T{ 8 90 at  said -> person-msg }T
T{ 9 0 at  said -> stretcher-msg }T
T{ $A -44 at  said -> sabers-msg }T
T{ $B -90 at  said -> breaker-msg }T
T{ $B -90 at  said -> chair-msg }T
T{ $C -90 at  said -> tools-msg }T
testing the iron maiden: its spikes; the lever lowers them; inside, hidden; out
T{ maiden-area 90 at  said -> spikes-msg }T
6 0 at  circle press  [: doing-busy? 0= ;] 600 soon drop
T{ omb1-3-flag3 story-flag? -> -1 }T
maiden-area 90 at  circle press  [: hidden? ;] 900 soon drop
T{ hidden? -> -1 }T
circle press  [: doing-busy? 0= ;] 900 soon drop
T{ hidden?  free? -> 0 -1 }T
testing the Camomile: taken
omb1-3-camomile story-flag-clear  room-afresh  camomile items:items-of before !
0 -20.96e 0e 1.57e put  circle press  300 read-on
T{ omb1-3-camomile story-flag?  camomile items:items-of -> -1 before @ 1+ }T
testing the movies replayed (the extras): played, then back by the forced way
200 run-frames  events-held state-flag-set  old-mansion-b1-3 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 300 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 3000 soon -> -1 }T
test-summary
