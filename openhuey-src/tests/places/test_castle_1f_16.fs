\ castle-1f-16's place (scripts/story/places/castle-1f-16.fs): the TV room - the fan, the TV on (the
\ first time a fright) and off, the mirror, the painting in turn, the Magnesia, the clock, the hiding
\ place (in, out; found), the stalker coming while she hides (Debilitas turning the TV off; his
\ search). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_1f_16.fs
IN: test-castle-1f-16
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-1f-16 items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: 0  #activities 1 do  i act-slot @ 0<> or  loop ;] in-place ;   \ (the fan has the first)
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: room-afresh ( -- )  castle-1f-16 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: fan-turn ( F: -- t )  s" fan" turn-y object@ ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the TV off (its dark screen up), the fan turning
c1f-16-flag1 story-flag-clear  c1f-16-flag2 story-flag-clear  c1f-16-magnesia story-flag-clear  room-afresh
T{ fan-turn  10 run-frames  fan-turn f- fabs 0.1e f>  tv-off-part part?  [: tv-fx effect @ 0<> ;] in-place -> -1 -1 -1 }T

testing the remarks: the mirror, the painting in turn
T{ mirror-area -144 at  said -> mirror-msg }T
T{ painting-area 0 at  said  painting-area 0 at  said -> painting-msg painting-2-msg }T

testing the Magnesia: reached, taken
magnesia items:items-of before !
90 -31e 0e 13e put  circle press  300 read-on
T{ c1f-16-magnesia story-flag?  magnesia items:items-of -> -1 before @ 1+ }T

testing the TV: on (the first time a fright), then off
0 9 progress pr.vars + c!
120 16.67e 0e 10.12e put  circle press  400 read-on   \ (the area's middle is on the furniture)
T{ c1f-16-flag2 story-flag?  c1f-16-flag1 story-flag?  tv-off-part part? -> -1 -1 0 }T
120 16.67e 0e 10.12e put  circle press  300 read-on
T{ c1f-16-flag2 story-flag?  tv-off-part part? -> 0 -1 }T

testing the hiding place: in (hidden), out by the button
hiding-area 90 at  circle press  300 run-frames
T{ fiona-hidden state-flag? -> -1 }T
circle press  200 run-frames
T{ fiona-hidden state-flag? -> 0 }T

testing ... found: pulled out, the avoid prompt
hiding-area 90 at  circle press  300 run-frames
T{ fiona-hidden state-flag? -> -1 }T
[: -1 found-her ! ;] in-place  200 run-frames
T{ fiona-hidden state-flag? -> 0 }T

testing the stalker coming while she hides, the TV on: Debilitas turns it off (its offs counted)
c1f-16-flag2 story-flag-set  room-afresh  2 story.base:partner-load  5 run-frames
hiding-area 90 at  circle press  300 run-frames
castle-1f-16 $CE game:debilitas send stalker-in  600 run-frames
T{ c1f-16-flag2 story-flag?  9 progress pr.vars + c@ -> 0 1 }T
testing ... the TV off: he searches (his comings counted)
8 progress pr.vars + c@ before !
game:debilitas send stalker-out  60 run-frames  castle-1f-16 $CE game:debilitas send stalker-in  60 run-frames
T{ 8 progress pr.vars + c@ -> before @ 1+ }T
game:debilitas send stalker-out  300 run-frames  circle press  200 run-frames
test-summary
