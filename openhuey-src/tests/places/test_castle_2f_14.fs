\ castle-2f-14's place (scripts/story/places/castle-2f-14.fs): the suspended bridge's hall - the first
\ time in, the remarks (the bridge, the statue), climbing down (no; yes), Hewie fetching the Medallion
\ across the gap (and, had, lying down over there), called back. Headless:
\   build/openhuey --test tests/places/test_castle_2f_14.fs
IN: test-castle-2f-14
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names floor-flags fiona.state story.kit story.places.castle-2f-14 story.places.bridge doors items.names ;

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
: room-afresh ( -- )  castle-2f-14 room!  60 run-frames ;
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

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing the first time in: the screen back, the crows' file; the Medallion's sparkle across the gap
found-file-227 story-flag-clear  c2f-13-medallion story-flag-clear  room-afresh
T{ [: doing-busy? 0= ;] 300 soon  found-file-227 story-flag?  $227 filed?  [: medallion-fx effect @ 0<> ;] in-place -> -1 -1 -1 -1 }T

testing the remarks: the bridge in turn, the statue turn and turn about
T{ bridge-area 180 at  said  bridge-area 180 at  said -> bridge-msg lower-it-msg }T
T{ statue-area 90 at  said  statue-area 90 at  said  statue-area 90 at  said -> statue-msg gargoyles-msg statue-msg }T

testing climbing down: not now; then down to the great hall (Hewie left here)
climb-area 0 at  circle press  answers-no
T{ room-id  stalkers-stay state-flag? -> castle-2f-14 0 }T
climb-area 0 at  circle press
T{ [: window-shown 0< 0= if  cross press  then  room-id castle-1f-13 = ;] 900 soon -> -1 }T
free-play  10 run-frames  0 game:fiona send costume

testing Hewie pointed across: he leaps over to the Medallion and brings it back
hewie-freed story-flag-set  hewie-commandable state-flag-set  game:hewie send join-fiona  room-afresh
castle-2f-14 $13C 0 game:hewie send hewie-place  60 run-frames
medallion items:items-of before !
[: ['] fetches start drop ;] in-place
T{ [: window-shown 0< 0= if  cross press  then  c2f-13-medallion story-flag? ;] 1500 soon  medallion items:items-of -> -1 before @ 1+ }T
T{ [: window-shown 0< 0= if  cross press  then  [: ['] fetches running? 0= ;] in-place ;] 900 soon -> -1 }T   \ (back across)

testing ... the Medallion had: over there he lies down
[: ['] fetches start drop ;] in-place
T{ [: [: ['] fetches running? 0= ;] in-place ;] 900 soon  hewie-tri $140 = -> -1 -1 }T
testing ... called back from over there: he leaps back
[: come-back-cmd started-now ! ;] in-place
T{ [: hewie-tri dup $140 <> over $2B <> and swap $1DF <> and ;] 900 soon -> -1 }T
test-summary
