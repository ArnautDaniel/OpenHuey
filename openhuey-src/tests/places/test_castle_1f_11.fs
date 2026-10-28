\ castle-1f-11's place (scripts/story/places/castle-1f-11.fs): the drawbridge room - the lever's
\ tilt, the clamp's remarks, the clamp struck three times (the bridge falls: the scene, the far door
\ closed off, the file), the vases (the jerky and the magnesia behind two), Hewie fetching the
\ Medallion across the gap, the replay. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_1f_11.fs
IN: test-castle-1f-11
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-1f-11 doors items.names ;

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
: room-afresh ( -- )  castle-1f-11 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )   \ (her blow's point there, for a frame)
    blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
: struck ( xt F: x y z -- )   \ (the place's word run with her blow there - as each frame does - then the blow gone)
    blow-at  in-place  [: body-point-off ;] as-her  10 run-frames ;
: tilt ( F: -- deg )  s" kanagu" turn-z object@ rad>deg ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
: filed? ( doc -- flag )
    128 0 do  progress pr.files i 2* + w@ over = if  drop true unloop exit  then  loop  drop false ;
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the bridge up, the vases whole, the Medallion's sparkle
c1f-11-flag1 story-flag-clear  c1f-11-breakable1-broken story-flag-clear  c1f-11-breakable2-broken story-flag-clear
c1f-11-breakable3-broken story-flag-clear  c1f-11-beef-jerky story-flag-clear  c1f-11-magnesia story-flag-clear
c1f-11-medallion story-flag-clear  room-afresh
T{ 0 part?  3 part?  5 part?  [: medallion-fx effect @ 0<> ;] in-place -> -1 -1 -1 -1 }T

testing the lever: jolted to -10 degrees, back 2 a frame, never past 0
T{ [: lever-jolts ;] in-place  tilt -10e ~=  [: lever-back ;] in-place  tilt -8e ~= -> -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)

testing the clamp's remark (facing it: the room's 90, cond 0x05's half-degrees, is 180 - at 90 nothing)
T{ clamp-area 90 at  circle press  60 run-frames  window-shown 0<  doing-busy? -> -1 0 }T
T{ clamp-area 180 at  said -> clamp-msg }T

testing the vases: broken by her blows - the jerky and the magnesia showing; picked up
' spots 72.5e 49e 8.5e struck  ' spots 53e 114e -113.5e struck  ' spots 53e 114e -107e struck
T{ c1f-11-breakable1-broken story-flag?  c1f-11-breakable2-broken story-flag?  c1f-11-breakable3-broken story-flag? -> -1 -1 -1 }T
T{ [: jerky-fx effect @ 0<>  magnesia-fx effect @ 0<> ;] in-place -> -1 -1 }T
beef-jerky items:items-of before !
72.5e 48e 8.5e picked
T{ c1f-11-beef-jerky story-flag?  beef-jerky items:items-of -> -1 before @ 1+ }T
magnesia items:items-of before !
53e 113e -113.5e picked
T{ c1f-11-magnesia story-flag?  magnesia items:items-of -> -1 before @ 1+ }T

testing Hewie fetching the Medallion across the gap
hewie-freed story-flag-set  hewie-commandable state-flag-set  game:hewie send join-fiona  60 run-frames
castle-1f-11 $13 0 game:hewie send hewie-place  60 run-frames
medallion items:items-of before !
[: ['] fetches start drop ;] in-place
T{ [: c1f-11-medallion story-flag? ;] 1500 soon  medallion items:items-of -> -1 before @ 1+ }T
300 read-on

testing the clamp struck: twice loosening it; the third time the bridge falls - the far door closed off, the file
room-afresh  0 -13e 113e -86e put
' clamp -11e 118e -87e struck  60 run-frames  ' clamp -11e 118e -87e struck  60 run-frames
T{ [: hits @ ;] in-place -> 2 }T
' clamp -11e 118e -87e struck
T{ [: movie-skipped state-flag-set  c1f-11-flag1 story-flag? ;] 1500 soon -> -1 }T
300 play-on
T{ far-door doors:door-locked?  s" hanebasi1" turn-x object@ rad>deg -60e ~=  bridge-file filed? -> -1 -1 -1 }T
T{ clamp-area 180 at  said -> clamp-off-msg }T
free-play  10 run-frames  0 game:fiona send costume

testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  castle-1f-11 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
