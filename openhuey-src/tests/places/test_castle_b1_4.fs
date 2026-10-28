\ castle-b1-4's place (scripts/story/places/castle-b1-4.fs): the spiral stair - its flames, the
\ clock, the inscription. Headless:
\   build/openhuey --test tests/places/test_castle_b1_4.fs
IN: test-castle-b1-4
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.places.castle-b1-4 ;

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
: room-afresh ( -- )  castle-b1-4 room!  60 run-frames ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: answer-no ( -- )  60 run-frames  cross press  30 run-frames  key: Down tap  10 run-frames  cross press  100 run-frames ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear

testing coming in: its five flames
room-afresh
T{ [: 0  #flames 0 do  i effect @ 0<> -  loop ;] in-place -> 5 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing the inscription
T{ inscription-area 0 at  said -> darkness-msg }T
testing the clock: saving (or Hewie not by her) - no
T{ clock-area 0 at  circle press  shown-soon  dup $8014 =  swap $8024 = or -> -1 }T
answer-no
T{ doing-busy? -> 0 }T
test-summary
