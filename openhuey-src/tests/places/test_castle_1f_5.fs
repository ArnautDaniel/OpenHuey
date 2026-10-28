\ castle-1f-5's place (scripts/story/places/castle-1f-5.fs): the long gallery - the flames, the
\ remarks (the dim corner, the box in turn, the twin paintings), the vase (the Antimony Powder),
\ Debilitas's entrance and his shove, the chapter turning (brought in after Daniella's scene: the
\ doors anew, Daniella the stalker) and her weakness after it. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_1f_5.fs
IN: test-castle-1f-5
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names floor-flags fiona.state story.kit story.places.castle-1f-5 doors items.names ;

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
: room-afresh ( -- )  castle-1f-5 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
: struck ( xt F: x y z -- )  blow-at  in-place  [: body-point-off ;] as-her  10 run-frames ;
: flames-up ( -- n )  [: 0  3 0 do  i effect @ alive? if  1+  then  loop ;] in-place ;
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the three flames, the vase whole
c1f-5-breakable-broken story-flag-clear  over-music-b story-flag-clear  c1f-6-flag1 story-flag-clear  c1f-5-flag1 story-flag-set
room-afresh
T{ flames-up  0 part? -> 3 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)

testing the remarks: the dim corner, the box in turn, the paintings in turn, their twin
T{ dim-area 180 at  said -> dim-msg }T
T{ 0 41e 0e -15e put  said  0 41e 0e -15e put  said -> empty-box-msg dust-box-msg }T
T{ painting-area 90 at  said  painting-area 90 at  said -> painting-msg painting-2-msg }T
T{ twin-area 90 at  said -> twins-msg }T

testing the vase broken: the Antimony Powder; picked up
' spots 41e 1e -19e struck
T{ c1f-5-breakable-broken story-flag?  [: antimony-fx effect @ 0<> ;] in-place -> -1 -1 }T
antimony-powder items:items-of before !
180 41e 0e -23e put  circle press  300 read-on   \ (facing away from the box)
T{ c1f-5-antimony-powder story-flag?  antimony-powder items:items-of -> -1 before @ 1+ }T

testing Debilitas coming in (the golem done, she back by exit 1, he loaded but not yet in the game)
golem-done story-flag-set  c1f-5-flag1 story-flag-clear  2 story.base:partner-load  5 run-frames  game:debilitas send stalker-out  5 run-frames
castle-1f-5 1 rooms send go-to-room  60 run-frames
T{ c1f-5-flag1 story-flag?  game:debilitas body?  game:debilitas body-room -> -1 -1 castle-1f-5 }T
600 run-frames
T{ [: his-entrance @ ;] in-place -> 0 }T

testing his shove: the rubble down, his scene over
[: ['] shoves start drop ;] in-place
T{ [: [: his-scene @ ;] in-place ;] 120 soon  [: [: his-scene @ 0= ;] in-place ;] 900 soon -> -1 -1 }T
game:debilitas send stalker-out  60 run-frames

testing the chapter turning (brought in after Daniella's scene): the doors anew, Daniella the stalker, she weak
c1f-6-flag1 story-flag-set  over-music-b story-flag-clear  castle-1f-5 1 rooms send go-to-room  60 run-frames
T{ [: over-music-b story-flag? ;] 300 soon  $D doors:door-locked?  1 doors:door-locked?  story.base:slot2-kind @ -> -1 0 -1 daniella-kind }T
300 run-frames
testing ... she weak: reaching to act, she flags - "I don't feel so hot"
saturnus-hint-on story-flag-clear  room-afresh
T{ square press  [: window-shown not-hot-msg = ;] 300 soon -> -1 }T
test-summary
