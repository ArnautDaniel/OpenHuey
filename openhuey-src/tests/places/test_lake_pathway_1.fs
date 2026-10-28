\ lake-pathway-1's place (scripts/story/places/lake-pathway-1.fs): the path collapsed, then the way
\ across open; the body's remarks, the path's, the lavender, Hewie's look at the drop. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_lake_pathway_1.fs
IN: test-lake-pathway-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.lake-pathway-1 items.names ;

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
: room-afresh ( -- )  lake-pathway-1 room!  60 run-frames ;
: path-barred? ( -- flag )   \ (the first of the fallen path's triangles barred)
    nav-tris 0 ?do  i path-group nav-in-group? if  i nav-flags barred-floor and 0<> unloop exit  then  loop  false ;


free-play  10 run-frames  0 game:fiona send costume
testing before the books' second event: the path collapsed (its floor barred), its two remarks in turn
c7-second-event story-flag-clear  over-music-d story-flag-clear  room-afresh
T{ path-barred?  way-part part? -> -1 0 }T
T{ path-end-area 0 at  said  path-end-area 0 at  said  path-end-area 0 at  said -> collapsed-msg the-light-msg collapsed-msg }T
T{ body-side-area 180 at  said -> -1 }T   \ (the body not offered yet)

testing the way across open: shown, the floor free; the body - by it, then up close, then again
c7-second-event story-flag-set  room-afresh
T{ path-barred?  way-part part? -> 0 -1 }T
T{ by-body-area 0 at  said  by-body-area 0 at  said  by-body-area 0 at  said -> twisted-msg leathery-msg twisted-msg }T
testing ... from its side, turned to the drop: the other remark
room-afresh
T{ body-side-area 180 at  said -> the-fall-msg }T
testing ... the bridge gone: the path barred again, the body still there to see
over-music-d story-flag-set  room-afresh
T{ path-barred?  body-side-area 180 at  said -> -1 the-fall-msg }T
over-music-d story-flag-clear

testing the lavender by the water: picked up once
variable before  lavender items:items-of before !
lp-1-lavender story-flag-clear  room-afresh
T{ [: lavender-fx effect @ 0<> ;] in-place -> -1 }T
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
0 19.59e 39e -243e put  circle press  300 read-on
T{ lp-1-lavender story-flag?  lavender items:items-of  [: lavender-fx effect @ ;] in-place -> -1 before @ 1+  0 }T

testing Hewie by the drop (the path collapsed; with her, at her command): he looks at it once a visit
c7-second-event story-flag-clear  hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh
game:hewie send join-fiona  60 run-frames
: hewie-spot ( F: x y z -- )  on-floor f>cell f>cell f>cell swap rot  0 game:hewie send place  5 run-frames ;
T{ [: drop-noticed @ ;] in-place -> 0 }T
-4.8e 38e -171.89e hewie-spot  10 run-frames
T{ [: drop-noticed @ ;] in-place -> -1 }T
test-summary
