\ castle-2f-4's place (scripts/story/places/castle-2f-4.fs): the little TV room - coming in (the TV, the fan), her nightmare brought in by the forced way, the replay. Headless:
\   build/openhuey --test tests/places/test_castle_2f_4.fs
IN: test-castle-2f-4
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-2f-4 doors items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: false  #activities 0 do  i act-slot @ 0<>  i cells act-xts + @ ['] fan <> and  or  loop ;] in-place ;   \ (the fan: always)
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: room-afresh ( -- )  castle-2f-4 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: replayed? ( -- flag )
    [: cutscene-active? movie-status 1 = or ;] 200 soon
    [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon and ;


: fan-turn ( F: -- t )  s" fan" turn-y object@ ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the TV's screen, its dark part away; the fan turning
room-afresh
T{ [: tv-fx effect @ 0<> ;] in-place  screen-part part?  off-part part?  fan-turn  10 run-frames  fan-turn f- fabs 0.1e f> -> -1 -1 0 -1 }T

testing brought in by the forced way (the bedroom's TV): her nightmare, out by the way loaded ahead
c2f-3-tv-examined story-flag-set  castle-2f-4 forced-exit rooms send go-to-room  5 run-frames
T{ [: movie-skipped state-flag-set  room-id beyond-room = ;] 1500 soon  c2f-3-tv-examined story-flag? -> -1 0 }T
free-play  10 run-frames  0 game:fiona send costume

testing the movie replayed (the extras)
events-held state-flag-set  castle-2f-4 -1 rooms send go-to-room  5 run-frames
T{ replayed? -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
test-summary
