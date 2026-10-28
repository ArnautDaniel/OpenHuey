\ castle-2f-7's place (scripts/story/places/castle-2f-7.fs): the plate press room - coming in, the
\ birdcage, the ruin, the press (the first time; again), someone at the door (a fright), a plate
\ given back, Daniella's scene, the replays. Headless:
\   build/openhuey --test tests/places/test_castle_2f_7.fs
IN: test-castle-2f-7
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names floor-flags fiona.state story.kit story.places.castle-2f-7 doors items.names ;

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
: room-afresh ( -- )  castle-2f-7 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: replayed? ( -- flag )
    [: cutscene-active? movie-status 1 = or ;] 200 soon
    [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon and ;



: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
: screen-up? ( -- flag )  subscreen-wanted state-flag? ;
: screen-left ( -- )   \ (the press's screen up: left without a plate - the triangle)
    [: screen-up? ;] 900 soon drop  10 run-frames  [: triangle press  screen-up? 0= ;] 300 soon drop  30 run-frames ;
: replays ( v -- )   \ (the extras' theatre: that movie, here)
    $2A var!  events-held state-flag-set  castle-2f-7 -1 rooms send go-to-room  5 run-frames ;
: cube-shown? ( -- flag )  s" cube" placed-shown? ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the thing wandering, the cube away, the room as before Daniella came
room-afresh
T{ [: wander-fx effect @ 0<> ;] in-place  cube-shown?  before-part part?  after-part part? -> -1 0 -1 0 }T

testing the remarks: the birdcage in turn, the ruin
T{ 90 -42e 0e 33e put  said  90 -42e 0e 33e put  said -> birdcage-msg animals-msg }T   \ (facing its middle)
T{ 45 -4e 0e -7e put  said -> ruined-msg }T

testing the press the first time: its scene, its screen (left without a plate)
c2f-7-flag1 story-flag-clear  plate-made story-flag-clear  room-afresh
45 30e 0e 18e put  circle press
[: movie-skipped state-flag-set  window-shown 0< 0= if  cross press  then  screen-up? ;] 1500 soon drop  screen-left
T{ [: doing-busy? 0= ;] 300 soon  c2f-7-flag1 story-flag? -> -1 -1 }T
testing ... again: its remark, the screen
45 30e 0e 18e put  circle press  [: window-shown press-msg = ;] 300 soon drop  cross press  screen-left
T{ [: doing-busy? 0= ;] 300 soon -> -1 }T

testing the golem done, someone trying the door from beyond (Debilitas loaded, out of the game): a fright
golem-done story-flag-set  c2f-7-flag2 story-flag-clear  2 story.base:partner-load  10 run-frames  room-afresh
[: door-0 true open-mark ;] in-place  10 run-frames
45 30e 0e 18e put  circle press
T{ [: c2f-7-flag2 story-flag? ;] 300 soon -> -1 }T
200 run-frames  golem-done story-flag-clear

testing a plate given back: her word
T{ 5 use-on-place  [: window-shown returned-msg = ;] 300 soon -> -1 }T
200 read-on

testing brought in by the forced way: Daniella's scene, out again
off-map-0d room!  30 run-frames   \ (from another room: the forced way out goes back to the room left)
castle-2f-7 forced-exit rooms send go-to-room  5 run-frames
T{ [: movie-skipped state-flag-set  room-id castle-2f-7 <> ;] 1500 soon  old-man-seen resident-flag? -> -1 -1 }T
free-play  10 run-frames  0 game:fiona send costume

testing the movies replayed (the extras): the press and the plate; Daniella's scene
T{ 3 replays  replayed? -> -1 }T
free-play  10 run-frames  0 game:fiona send costume
T{ 21 replays  replayed? -> -1 }T
test-summary
