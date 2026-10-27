\ castle-2f-11's place (scripts/story/places/castle-2f-11.fs): the corridor of the slanted crest - the doll and the swing rocking, the crest. Headless:
\   build/openhuey --test tests/places/test_castle_2f_11.fs
IN: test-castle-2f-11
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-2f-11 doors items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: false  #activities 0 do  i act-slot @ 0<>  i cells act-xts + @ ['] rocking <> and  or  loop ;] in-place ;   \ (the rocking: always)
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: room-afresh ( -- )  castle-2f-11 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: replayed? ( -- flag )
    [: cutscene-active? movie-status 1 = or ;] 200 soon
    [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon and ;


: swing-turn ( F: -- t )  s" swing" turn-z object@ ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the doll and the swing rocking
room-afresh
T{ swing-turn  5 run-frames  swing-turn f- fabs 0.0001e f> -> -1 }T

testing the crest
T{ crest-area 30 at  said -> crest-msg }T
test-summary
