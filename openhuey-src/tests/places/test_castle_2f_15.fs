\ castle-2f-15's place (scripts/story/places/castle-2f-15.fs): the suspended bridge's hall, its far way
\ open - coming in (the sparkles), the statue, the Prima Magnesia (sniffed out; picked up), Hewie's
\ fetch of the Medallion, climbing down, in by the far way (the map's page). Headless:
\   build/openhuey --test tests/places/test_castle_2f_15.fs
IN: test-castle-2f-15
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names floor-flags fiona.state story.kit story.places.castle-2f-15 story.places.bridge doors items.names ;

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
: room-afresh ( -- )  castle-2f-15 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: replayed? ( -- flag )
    [: cutscene-active? movie-status 1 = or ;] 200 soon
    [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon and ;



: filed? ( doc -- flag )
    128 0 do  progress pr.files i 2* + w@ over = if  drop true unloop exit  then  loop  drop false ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: at-choice? ( -- flag )  game:window enter  window:choosing? window:page-typed? and  leave-actor ;
: answers-no ( -- )   \ (the pages read to the choice, then the second option)
    100 0 do  at-choice? if  leave  then  cross press  10 run-frames  loop
    key: Down tap  10 run-frames  cross press  100 run-frames ;
: hewie-tri ( -- tri )  game:hewie body-tri ;
variable before

: map-page@ ( -- page )  game:subscreen enter  subscreen.state:her-pg @  leave-actor ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the Prima Magnesia Hewie found, the Medallion across the gap
c2f-15-prima-magnesia-found story-flag-set  c2f-15-prima-magnesia story-flag-clear  c2f-13-medallion story-flag-clear
found-file-227 story-flag-set  room-afresh
T{ [: magnesia-fx effect @ 0<>  medallion-fx effect @ 0<> ;] in-place -> -1 -1 }T

testing the statue turn and turn about (no remark on the bridge here)
T{ statue-area 45 at  said  statue-area 45 at  said -> statue-msg gargoyles-msg }T
T{ bridge-area 90 at  circle press  [: window-shown 0< 0= ;] 60 soon -> 0 }T

testing the Prima Magnesia: picked up; Hewie sniffing it out
prima-magnesia items:items-of before !
0 -105e 135e 27e put  circle press  300 read-on
T{ c2f-15-prima-magnesia story-flag?  prima-magnesia items:items-of -> -1 before @ 1+ }T
c2f-15-prima-magnesia-found story-flag-clear  c2f-15-prima-magnesia story-flag-clear
hewie-freed story-flag-set  hewie-commandable state-flag-set  game:hewie send join-fiona  room-afresh
castle-2f-15 -105e 135e 28e nav-tri 0 game:hewie send hewie-place  60 run-frames
[: ['] magnesia-sniffed start drop ;] in-place
T{ [: c2f-15-prima-magnesia-found story-flag? ;] 600 soon  [: magnesia-fx effect @ 0<> ;] in-place -> -1 -1 }T
300 read-on

testing Hewie pointed across: he brings the Medallion back
castle-2f-15 $13C 0 game:hewie send hewie-place  60 run-frames
medallion items:items-of before !
[: ['] fetches start drop ;] in-place
T{ [: window-shown 0< 0= if  cross press  then  c2f-13-medallion story-flag? ;] 1500 soon  medallion items:items-of -> -1 before @ 1+ }T
300 read-on  game:hewie send part-from-fiona

testing climbing down: to the great hall
climb-area 0 at  circle press
T{ [: window-shown 0< 0= if  cross press  then  room-id castle-1f-13 = ;] 900 soon -> -1 }T
free-play  10 run-frames  0 game:fiona send costume

testing in by the far way: the map's page
2 [: subscreen.state:her-pg ! ;] game:subscreen enter execute leave-actor
castle-2f-15 1 rooms send go-to-room  30 run-frames
T{ map-page@ -> 4 }T
test-summary
