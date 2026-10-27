\ castle-2f-13's place (scripts/story/places/castle-2f-13.fs): the suspended bridge's hall before the
\ balcony fell - coming in (the sparkles), the remarks (the bridge, the statue), Hewie wary on the
\ balcony, fetching the Medallion across the gap (and, had, lying down over there), called back; out
\ on the balcony a while: it gives (she; the stalker after her). Headless:
\   build/openhuey --test tests/places/test_castle_2f_13.fs
IN: test-castle-2f-13
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names floor-flags fiona.state story.kit story.places.castle-2f-13 story.places.bridge doors items.names ;

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
: room-afresh ( -- )  castle-2f-13 room!  60 run-frames ;
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
testing coming in: the sparkles
c2f-13-flag1 story-flag-clear  c2f-13-medallion story-flag-clear  found-file-227 story-flag-set  room-afresh
T{ [: sparkle-fx effect @ 0<>  medallion-fx effect @ 0<> ;] in-place -> -1 -1 }T

testing the remarks: the bridge in turn, the statue turn and turn about
T{ bridge-area 90 at  said  bridge-area 90 at  said -> bridge-msg lower-it-msg }T
T{ statue-area 45 at  said  statue-area 45 at  said  statue-area 45 at  said -> statue-msg gargoyles-msg statue-msg }T

testing Hewie: wary on the balcony (once a visit)
hewie-freed story-flag-set  hewie-commandable state-flag-set  game:hewie send join-fiona  room-afresh
castle-2f-13 -231e 60e 30e nav-tri 0 game:hewie send hewie-place  60 run-frames
T{ [: [: wary-noticed @ ;] in-place ;] 90 soon -> -1 }T
300 run-frames

testing Hewie pointed across: he leaps over to the Medallion and brings it back
room-afresh  castle-2f-13 $13C 0 game:hewie send hewie-place  60 run-frames
medallion items:items-of before !
[: ['] fetches start drop ;] in-place
T{ [: window-shown 0< 0= if  cross press  then  c2f-13-medallion story-flag? ;] 1500 soon  medallion items:items-of -> -1 before @ 1+ }T
T{ [: window-shown 0< 0= if  cross press  then  [: ['] fetches running? 0= ;] in-place ;] 900 soon -> -1 }T   \ (back across)
testing ... the Medallion had: over there he lies down; called back, he leaps back
[: ['] fetches start drop ;] in-place
T{ [: [: ['] fetches running? 0= ;] in-place ;] 900 soon  hewie-tri $1EA = -> -1 -1 }T
[: come-back-cmd started-now ! ;] in-place
T{ [: hewie-tri dup $1C3 <> over $1D0 <> and swap $1EA <> and ;] 900 soon -> -1 }T
game:hewie send part-from-fiona

testing out on the balcony a while (before the great hall's balcony fell): it gives - down by the forced way
found-file-227 story-flag-set  c2f-13-flag1 story-flag-set  room-afresh  found-file-227 story-flag-clear
balcony-area 0 at
T{ [: movie-skipped state-flag-set  room-id castle-1f-13 = ;] 600 soon -> -1 }T
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing ... the stalker on it after her: it gives - down the other forced way (seized)
2 story.words:partner-load  10 run-frames  found-file-227 story-flag-clear  room-afresh
castle-2f-13  balcony-area area-middle drop nav-tri  game:debilitas send stalker-in
T{ [: movie-skipped state-flag-set  room-id castle-2f-13 <> ;] 900 soon -> -1 }T
test-summary
