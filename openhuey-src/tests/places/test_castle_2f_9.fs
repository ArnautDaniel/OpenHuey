\ castle-2f-9's place (scripts/story/places/castle-2f-9.fs): the stair landing - the first time in, Daniella cleaning (spoken to: she stands; her strangeness; taken as she leaves). Headless:
\   build/openhuey --test tests/places/test_castle_2f_9.fs
IN: test-castle-2f-9
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names floor-flags fiona.state story.kit story.places.castle-2f-9 doors items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: false  #activities 0 do  i act-slot @ 0<>  i cells act-xts + @ dup ['] cleans <> swap ['] she-stands <> and and  or  loop ;] in-place ;   \ (her cleaning: always)
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: room-afresh ( -- )  castle-2f-9 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: replayed? ( -- flag )
    [: cutscene-active? movie-status 1 = or ;] 200 soon
    [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon and ;



: daniella-in ( -- )  daniella-kind story.words:partner-load  10 run-frames ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing the first time in: the screen back
c2f-9-flag1 story-flag-clear  c1f-14-flag2 story-flag-clear  room-afresh
T{ [: doing-busy? 0= ;] 120 soon  c2f-9-flag1 story-flag? -> -1 -1 }T

testing Daniella cleaning (after the Saturnus hint): in, on her knees - her floor kept from her
daniella-in  c1f-14-flag2 story-flag-set  c3f-2-flag1 story-flag-clear  c2f-9-flag2 story-flag-clear  room-afresh  30 run-frames
T{ game:daniella body?  game:daniella body-room  [: stalker held? ;] in-place -> -1 castle-2f-9 -1 }T
testing ... spoken to: she stands - "It's cleaning time now"; then her strangeness
180 -4.85e -46e -45e put
T{ said  c2f-9-flag2 story-flag? -> cleaning-msg -1 }T
180 -4.85e -46e -45e put
T{ said -> attacked-msg }T
testing ... out of the room: she is taken (out of the game)
castle-2f-10 room!  30 run-frames
T{ game:daniella body? -> 0 }T
test-summary
