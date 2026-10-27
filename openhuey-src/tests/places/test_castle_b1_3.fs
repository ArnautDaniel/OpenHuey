\ castle-b1-3's place (scripts/story/places/castle-b1-3.fs): the store room - the Metal Boots (had
\ already: a Magist Medallion), the Castle Gate Key once she has her costumes. Headless:
\   build/openhuey --test tests/places/test_castle_b1_3.fs
IN: test-castle-b1-3
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.places.castle-b1-3 ;

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
: room-afresh ( -- )  castle-b1-3 room!  60 run-frames ;
: fx? ( k -- flag )  [: effect @ 0<> ;] in-place ;
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear

testing the Metal Boots on the shelf: sparkling, taken
got-metal-boots story-flag-clear  fg-3-flag3 story-flag-clear  room-afresh
T{ boots-fx fx?  key-fx fx? -> -1 0 }T
metal-boots items:items-of before !
180 -2e 0e 2e put  circle press  300 read-on
T{ got-metal-boots story-flag?  metal-boots items:items-of  boots-fx fx? -> -1 before @ 1+ 0 }T
testing ... had already: a Magist Medallion there instead
got-metal-boots story-flag-clear  room-afresh  magist-medallion items:items-of before !
180 -2e 0e 2e put  circle press  300 read-on
T{ got-metal-boots story-flag?  magist-medallion items:items-of -> -1 before @ 1+ }T

testing the Castle Gate Key once she has her costumes: sparkling, taken
fg-3-flag3 story-flag-set  got-castle-gate-key story-flag-clear  room-afresh
T{ key-fx fx? -> -1 }T
180 -6e 0e 7e put  circle press  300 read-on
T{ got-castle-gate-key story-flag?  castle-gate-key items:items-of  key-fx fx? -> -1 1 0 }T
test-summary
