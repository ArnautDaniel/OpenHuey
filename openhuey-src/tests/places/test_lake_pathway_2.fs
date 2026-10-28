\ lake-pathway-2's place (scripts/story/places/lake-pathway-2.fs): the bridge there, then gone -
\ its far end barred, her two remarks at the gap, Hewie's look at it. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_lake_pathway_2.fs
IN: test-lake-pathway-2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.lake-pathway-2 ;

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
: room-afresh ( -- )  lake-pathway-2 room!  60 run-frames ;
: gap-barred? ( -- flag )   \ (the first of the gap's triangles barred)
    nav-tris 0 ?do  i gap-group nav-in-group? if  i nav-flags barred-floor and 0<> unloop exit  then  loop  false ;

free-play  10 run-frames  0 game:fiona send costume
testing the bridge there: shown, its floor free, nothing said at its end
over-music-d story-flag-clear  room-afresh
T{ bridge-part part?  gap-barred? -> -1 0 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
T{ bridge-end-area 180 at  said -> -1 }T

testing the bridge gone: its floor barred; at its end facing the gap, the remark - then the other one
over-music-d story-flag-set  room-afresh
T{ gap-barred? -> -1 }T
T{ bridge-end-area 180 at  said -> bridge-gone-msg }T
T{ bridge-end-area 180 at  said -> no-way-back-msg }T
testing ... a new visit: the first remark again
room-afresh
T{ bridge-end-area 180 at  said -> bridge-gone-msg }T
testing ... facing away: nothing
T{ bridge-end-area -90 at  said -> -1 }T

testing Hewie by the gap (with her, at her command): he looks at it once a visit
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh
game:hewie send join-fiona  60 run-frames
: hewie-spot ( F: x y z -- )  on-floor f>cell f>cell f>cell swap rot  0 game:hewie send place  5 run-frames ;
T{ [: gap-noticed @ ;] in-place -> 0 }T
-2.26e 38e -25.57e hewie-spot  10 run-frames
T{ [: gap-noticed @ ;] in-place -> -1 }T
test-summary
