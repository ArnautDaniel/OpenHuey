\ castle-1f-4's place (scripts/story/places/castle-1f-4.fs): the hall by the puppet room - the door
\ with no knob and the broken marionette, the floor plate and its mechanism, the weak wall's
\ antimony, the plate-key's slot (REST, a wrong word, the cracked key), the chairs, Riccardo at
\ night, back by the trap. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_1f_4.fs
IN: test-castle-1f-4
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    fiona.state story.kit story.places.castle-1f-4 ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: play-on ( n -- )
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
: room-afresh ( -- )  castle-1f-4 room!  60 run-frames ;

free-play  10 run-frames  0 game:fiona send costume  no-flee state-flag-clear
puppet-door s" doors" actor-named send lock  slot-door s" doors" actor-named send lock   \ (as a new game has them)
room-afresh
testing by day: the puppet door shut, the hall's door at exit 1 its own (part 3), the plate key lying there
T{ puppet-door-part part?  exit-1-door-part part?  plate-key-part part?  puppet-door doors:door-locked? -> -1 -1 -1 -1 }T

testing the puppet door: no knob (once, then the words on it - the puppet room's file); pushing it, it won't open
T{ -90 -16e 0e 14e put  said  puppet-file filed? -> no-knob-msg -1 }T
T{ -90 -16e 0e 14e put  said -> pupa-cella-msg }T
-1 broadcast fiona-straining  2 run-frames  0 broadcast fiona-straining
T{ [: window-shown wont-open-msg = ;] 200 soon -> -1 }T
200 play-on

testing the chairs, the floor
T{ chair-area 180 at  said -> chair-msg }T
T{ chair-area 180 at  said -> chair-again }T
T{ chair-2-area 0 at  said -> same-chair-msg }T
T{ plate-area 0 at  said -> floor-msg }T
testing ... standing on the floor plate: pressed, castle-1f-7's gate rising; off it, let go
T{ floor-plate-pressed story-flag? -> -1 }T
0 0e 0e 30e put  60 run-frames
T{ floor-plate-pressed story-flag? -> 0 }T

testing the broken marionette on its hook: the scene, the puppet door open, the marionette spent
items.names:broken-marionette 1 items send give-item  10 run-frames
1 use-on-place  900 play-on
T{ broken-marionette-used story-flag?  puppet-door doors:door-locked?  puppet-door-part part?  free? -> -1 0 0 -1 }T
T{ items.names:broken-marionette items:items-of -> 0 }T

testing the plate key, taken; the slot (empty); a wrong word, the cracked key, REST
0 131.392e -30e 145e put  circle press  300 play-on
T{ got-cracked-plate-key story-flag?  plate-key-part part? -> -1 0 }T
T{ -90 120e 0e 154e put  said -> slot-msg }T   \ (by the slot, where she stands to look)
T{ $17 use-on-place  [: window-shown nothing-msg = ;] 600 soon -> -1 }T
200 play-on
T{ $16 use-on-place  [: window-shown reforge-msg = ;] 600 soon -> -1 }T
200 play-on
T{ $14 use-on-place  400 play-on  slot-door doors:door-locked?  key-in-slot-part part? -> 0 -1 }T
T{ -90 120e 0e 154e put  said -> key-in-msg }T

testing the weak wall, broken: its bugs, the refined antimony taken
weak-wall-broken story-flag-set  room-afresh
T{ broken-wall-part part?  [: sparkle-shown @ ;] in-place -> -1 -1 }T
variable before  refined-antimony items:items-of before !
0 140e 0e 89e put  circle press  300 play-on
T{ c1f-4-refined-antimony story-flag?  refined-antimony items:items-of -> -1 before @ 1+ }T

testing at night, back with the castle's sketch: Riccardo - the scene, then out by the forced way
got-castle-sketch story-flag-set  room-afresh
T{ exit-1-door-part part? -> 0 }T
1200 play-on
T{ room-id castle-1f-4 <> -> -1 }T   \ (out to the room after it)
test-summary
