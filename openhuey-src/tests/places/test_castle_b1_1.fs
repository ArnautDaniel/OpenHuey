\ castle-b1-1's place (scripts/story/places/castle-b1-1.fs): the kitchen - the Castle Sketch; the
\ remarks; the vase's jerky, the shelf's jerky, the Lavender; the plywood door broken through; the
\ cupboard to hide in and the stalker finding her; Daniella at the stove (her scene, then her remark);
\ the scene as she steps in with the sketch. Headless:
\   build/openhuey --test tests/places/test_castle_b1_1.fs
IN: test-castle-b1-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.castle-b1-1 ;
full-suite   \ (on: waits, scenes, walks - the full run's)
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] her  10 run-frames ;
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
: room-afresh ( -- )  castle-b1-1 room!  60 run-frames ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: kitchen-afresh ( -- )   \ (no wire, no night: the plain kitchen)
    hewie-found-in-wire story-flag-clear  c2f-3-flag1 story-flag-set  room-afresh ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear

testing the Castle Sketch: taken from the wall (the cupboard's place, before it)
got-castle-sketch story-flag-clear  kitchen-afresh  castle-sketch items:items-of before !
-90 -30e -50e -30e put  circle press  300 read-on
T{ got-castle-sketch story-flag?  castle-sketch items:items-of  sketch-part part? -> -1 before @ 1+ 0 }T

testing the remarks
T{ 9 90 at  said  9 90 at  said -> china-msg polished-msg }T
T{ $B 90 at  said -> box-msg }T
T{ $C 180 at  said  $C 180 at  said -> wine-msg father-msg }T
T{ $D 0 at  said -> sink-msg }T
T{ $E 0 at  said -> empty-pot-msg }T

testing the plywood door: its remark; her blow breaks it
cb1-1-breakable2-broken story-flag-clear  kitchen-afresh
T{ 0 22e -40e -38e put  said -> plywood-msg }T

testing the shelf's jerky: taken
cb1-1-chicken-jerky-2 story-flag-clear  kitchen-afresh  chicken-jerky items:items-of before !
0 -13e 0e 6e put  circle press  300 read-on
T{ cb1-1-chicken-jerky-2 story-flag?  chicken-jerky items:items-of -> -1 before @ 1+ }T

testing the vase broken: its jerky picked up
cb1-1-breakable1-broken story-flag-set  cb1-1-chicken-jerky story-flag-clear  kitchen-afresh  chicken-jerky items:items-of before !
0 -52.5e -60e -5.5e put  circle press  300 read-on
T{ cb1-1-chicken-jerky story-flag?  chicken-jerky items:items-of -> -1 before @ 1+ }T

testing the Lavender Hewie sniffed out: picked up
cb1-1-lavender-found story-flag-set  cb1-1-lavender story-flag-clear  kitchen-afresh  lavender items:items-of before !
0 -18.1e -60e -12e put  circle press  300 read-on
T{ cb1-1-lavender story-flag?  lavender items:items-of -> -1 before @ 1+ }T

testing the cupboard (the sketch had): in, hidden; out by the button
got-castle-sketch story-flag-set  kitchen-afresh
-90 -30e -50e -30e put  circle press  [: hidden? ;] 600 soon drop
T{ hidden? -> -1 }T
circle press  [: doing-busy? 0= ;] 600 soon drop
T{ hidden?  free? -> 0 -1 }T

testing Daniella at the stove (Hewie in the wire): the first time near, her scene and its file; after, her remark
hewie-found-in-wire story-flag-set  found-file-208 story-flag-clear  cb1-1-flag1 story-flag-clear
daniella-kind 3 story.kit:figure-in  room-afresh
2 game:fiona body-tri 0 cam-setup drop  0 16.5e 0e -12e put  0 16.5e 0e 2e put  1500 play-on
T{ cb1-1-flag1 story-flag?  $226 filed? -> -1 -1 }T

testing the stalker coming in while she hides: he finds her (but on his alert 4)
hewie-found-in-wire story-flag-clear  got-castle-sketch story-flag-set  2 story.base:partner-load  5 run-frames  kitchen-afresh
-90 -30e -50e -30e put  circle press  [: hidden? ;] 600 soon drop
castle-b1-1 $1FE game:debilitas send stalker-in
T{ [: [: found-her @ ;] in-place ;] 900 soon -> -1 }T
game:debilitas send stalker-out  600 play-on

testing with the sketch, before nightfall: stepping in - Riccardo's scene, out by the near door
c2f-3-flag1 story-flag-clear  room-afresh
[: ['] the-scene start drop ;] in-place  1200 play-on   \ (its trigger, the step into area 5, not placed: no test spot found)
T{ room-id castle-b1-1 = -> 0 }T
test-summary
