\ castle-1f-6's place (scripts/story/places/castle-1f-6.fs): the long dining room - the thirteen
\ candles, the plate-key on the table, the candlesticks by day and night, the tapestry, the
\ Medallion, the door unlocked from this side, Daniella's scene (brought in by the forced way), the
\ replay. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_1f_6.fs
IN: test-castle-1f-6
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-1f-6 doors items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: room-afresh ( -- )  castle-1f-6 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)
: flames ( -- n )  [: 0  #candles 0 do  i effect @ alive? if  1+  then  loop ;] in-place ;
create spot 12 allot
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the thirteen candles lit, the plate-key on the table
got-cracked-plate-key-2 story-flag-clear  c2f-3-flag1 story-flag-clear  room-afresh
T{ flames  plate-key-part part? -> #candles -1 }T

testing the candlesticks (by day, by night), the tapestry
T{ table-area -90 at  said -> candles-msg }T
c2f-3-flag1 story-flag-set  room-afresh
T{ table-2-area 90 at  said -> candles-night-msg }T
c2f-3-flag1 story-flag-clear
T{ tapestry-area 180 at  said -> tapestry-msg }T

testing the plate-key on the table: reached, taken
room-afresh  cracked-plate-key-2 items:items-of before !
0 4e 0e -16e put  circle press  300 read-on
T{ got-cracked-plate-key-2 story-flag?  cracked-plate-key-2 items:items-of  plate-key-part part? -> -1 before @ 1+ 0 }T

testing the Medallion Hewie found: it shows; picked up
c1f-6-medallion-found story-flag-set  c1f-6-medallion story-flag-clear  room-afresh
T{ [: medallion-fx effect @ 0<> ;] in-place -> -1 }T
medallion items:items-of before !
34.2e 0e 60.5e picked
T{ c1f-6-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T

testing the door locked from this side (exit 0, door 3): before it, the button - unlocked
3 s" doors" actor-named send lock  room-afresh
0 0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames
T{ circle press  [: window-shown unlocked-msg = ;] 300 soon -> -1 }T
300 read-on
T{ 3 door-locked? -> 0 }T

testing brought in by the forced way: Daniella's scene, then out by the way loaded ahead
c1f-6-flag1 story-flag-clear  castle-1f-6 forced-exit rooms send go-to-room  5 run-frames
T{ [: movie-skipped state-flag-set  c1f-6-flag1 story-flag? ;] 1500 soon -> -1 }T
T{ [: room-id castle-1f-6 <> ;] 600 soon -> -1 }T
free-play  10 run-frames  0 game:fiona send costume

testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  castle-1f-6 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
