\ castle-b1-5's place (scripts/story/places/castle-b1-5.fs): the dark passage - Hewie tense the
\ first time in by the near door; sent on ahead toward a way, he runs there and waits; stepping into
\ the pit, she falls - the game over. Headless:
\   build/openhuey --test tests/places/test_castle_b1_5.fs
IN: test-castle-b1-5
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.places.castle-b1-5 ;

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
: room-afresh ( -- )  castle-b1-5 room!  60 run-frames ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
hewie-freed story-flag-set  hewie-commandable state-flag-set

testing Hewie the first time in by the near door: tense at the dark, barking - once
cb1-5-flag1 story-flag-clear  room-afresh
game:hewie send join-fiona  0 game:hewie send to-exit  0 broadcast hewie-came   \ (as he comes through it)
castle-b1-5 game:fiona body-tri 0 game:hewie send hewie-place
T{ [: cb1-5-flag1 story-flag? ;] 900 soon -> -1 }T
200 run-frames
testing sent on ahead toward the far way: he runs there
90 0e 0e 0e put  60 run-frames  go-there-cmd broadcast fiona-started  5 run-frames
T{ [: guide-tri @ ;] in-place -> $9B }T
T{ [: game:hewie body-tri $9B = ;] 900 soon -> -1 }T

testing into the pit: she falls - caught, the game over
0 -28.33e 0e 4.56e put
T{ [: caught state-flag? ;] 1500 soon  capture-no-end state-flag? -> -1 -1 }T
test-summary
