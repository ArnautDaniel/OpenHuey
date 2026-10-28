\ castle-1f-14's place (scripts/story/places/castle-1f-14.fs): the corridor - the three vases broken
\ (the Chicken Jerky and the Antimony Powder behind two), the Viride Medallion Hewie finds, the door
\ unlocked from this side, the map's pages, the creature in the other game, the movie replayed, and
\ brought here caught: the game over. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_1f_14.fs
IN: test-castle-1f-14
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-1f-14 doors items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: room-afresh ( -- )  castle-1f-14 room!  60 run-frames ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )   \ (her blow's point there)
    blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)
: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: across ( gate side -- )   \ (stepped over the gate's line: from the other side to this one, 4 from it)
    {: g side | F: ax F: ay F: az F: bx F: bz F: ox F: oz :}
    g 0 area-corner drop  to az to ay to ax   g 1 area-corner drop  to bz fdrop to bx
    bz az f- to ox  bx ax f- fnegate to oz   ox ox f* oz oz f* f+ fsqrt 4e fswap f/
    fdup ox f* side s>f f* to ox  oz f* side s>f f* to oz
    0  ax bx f+ 2e f/ ox f-  ay  az bz f+ 2e f/ oz f-  put
    0  ax bx f+ 2e f/ ox f+  ay  az bz f+ 2e f/ oz f+  put  10 run-frames ;
: map-page@ ( -- page )  game:subscreen enter  subscreen.state:her-pg @  leave-actor ;
create spot 12 allot
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing the vases: whole, each broken by a blow in its place - the jerky and the antimony showing
c1f-14-breakable1-broken story-flag-clear  c1f-14-breakable2-broken story-flag-clear  c1f-14-breakable3-broken story-flag-clear
c1f-14-chicken-jerky story-flag-clear  c1f-14-antimony-powder story-flag-clear  room-afresh
T{ 0 part?  2 part?  4 part?  [: jerky-fx effect @  antimony-fx effect @ ;] in-place -> -1 -1 -1 0 0 }T
T{ -61e 1e -52e blow-at  [: vase-1 breaks? ;] in-place -> -1 }T
-68e 1e -58e blow-at  [: spots ;] in-place
-66e 1e 55e blow-at  [: spots ;] in-place  [: body-point-off ;] as-her
T{ c1f-14-breakable2-broken story-flag?  c1f-14-breakable3-broken story-flag?  1 part?  3 part?  5 part? -> -1 -1 -1 -1 -1 }T
T{ [: jerky-fx effect @ 0<>  antimony-fx effect @ 0<> ;] in-place -> -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)

testing the jerky and the antimony picked up
chicken-jerky items:items-of before !
-68e 0e -58e picked
T{ c1f-14-chicken-jerky story-flag?  chicken-jerky items:items-of -> -1 before @ 1+ }T
antimony-powder items:items-of before !
-66e 0e 55e picked
T{ c1f-14-antimony-powder story-flag?  antimony-powder items:items-of -> -1 before @ 1+ }T
testing ... broken, as she comes in again: the pieces, nothing to pick up
room-afresh
T{ 1 part?  3 part?  5 part?  0 part?  [: jerky-fx effect @  antimony-fx effect @ ;] in-place -> -1 -1 -1 0 0 0 }T

testing the Viride Medallion: Hewie finds it (it shows), she picks it up
hewie-freed story-flag-set  hewie-commandable state-flag-set  c1f-14-viride-medallion-found story-flag-clear
c1f-14-viride-medallion story-flag-clear  room-afresh  game:hewie send join-fiona  60 run-frames
T{ [: medallion-fx effect @ ;] in-place -> 0 }T
[: ['] medallion-sniffed start-only ;] in-place  600 run-frames
T{ c1f-14-viride-medallion-found story-flag?  [: medallion-fx effect @ 0<> ;] in-place -> -1 -1 }T
viride-medallion items:items-of before !
-70e 54.1e 54.1e picked
T{ c1f-14-viride-medallion story-flag?  viride-medallion items:items-of -> -1 before @ 1+ }T

testing the map: at the stairs her floor's next page, back from them this one's
T{ stairs-area 1 across  map-page@  stairs-area -1 across  map-page@  2dup <>  rot rot + -> -1 stairs-page this-floor-page + }T   \ (one way the stairs' page, the other back)

testing the door locked from this side (exit 0): before it, the button - unlocked, said so
variable the-door  [: locked-exit exit-door ;] in-place the-door !
the-door @ s" doors" actor-named send lock  room-afresh
0 0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames
T{ circle press  [: window-shown unlocked-msg = ;] 300 soon -> -1 }T
300 read-on
T{ the-door @ door-locked? -> 0 }T

testing the other game's creature: let out once
items-40-as-70 story-flag-set  c1f-14-flag1 story-flag-clear  room-afresh
T{ c1f-14-flag1 story-flag? -> -1 }T
items-40-as-70 story-flag-clear

testing brought here caught (the forced way in): the scene, the game over (the Azoth ending)
: over@ ( -- kind running? )  game:gameover enter  gameover:kind @  gameover:step @ gameover:not-running <>  leave-actor ;
castle-1f-14 forced-exit rooms send go-to-room  5 run-frames
T{ [: movie-skipped state-flag-set  over@ nip ;] 900 soon  over@ drop  ending-var progress pr.vars + c@ -> -1 caught-over azoth-ending }T

free-play  10 run-frames  0 game:fiona send costume   \ (afresh)
testing the movie replayed (the extras): Lorenzo's scene, then back by the forced way
events-held state-flag-set  castle-1f-14 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
200 run-frames

test-summary
