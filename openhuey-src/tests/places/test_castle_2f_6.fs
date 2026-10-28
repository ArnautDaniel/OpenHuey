\ castle-2f-6's place (scripts/story/places/castle-2f-6.fs): the study - the remarks (the shelves, the book, the desktop, the board), the desk locked, the Lavender, the Study Key used (the Lunar Refractor), Hewie's bark. Headless:
\   build/openhuey --test tests/places/test_castle_2f_6.fs
IN: test-castle-2f-6
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names floor-flags fiona.state story.kit story.places.castle-2f-6 doors items.names ;

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
: room-afresh ( -- )  castle-2f-6 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: replayed? ( -- flag )
    [: cutscene-active? movie-status 1 = or ;] 200 soon
    [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon and ;



: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the Lavender showing
c2f-6-lavender story-flag-clear  got-lunar-refractor story-flag-clear  room-afresh
T{ [: lavender-fx effect @ 0<> ;] in-place -> -1 }T

testing the remarks: the shelves in turn, the massive book in turn, the desktop, the board (the Luminessants)
T{ 7 -90 at  said  7 -90 at  said -> shelves-msg study-msg }T
full-suite   \ (on: waits, scenes, walks - the full run's)
T{ 90 -4e 0e -19e put  said  90 -4e 0e -19e put  said -> book-msg greek-msg }T   \ (facing its middle)
T{ desktop-area -134 at  said -> desktop-msg }T
got-doc-luminessants story-flag-clear
T{ board-area -90 at  circle press  [: window-shown board-msg = ;] 300 soon  [: window-shown 0< 0= if  window-shown no-clue-msg = if  true exit  then  cross press  then  false ;] 900 soon -> -1 -1 }T
300 read-on

testing the desk locked: her blows on it
T{ 0 -3e 0e 31e put  said -> locked-msg }T   \ (off its middle: the desk's floor)

testing the Lavender: reached, taken
lavender items:items-of before !
180 -26e 0e -42e put  circle press  300 read-on
T{ c2f-6-lavender story-flag?  lavender items:items-of -> -1 before @ 1+ }T

testing the Study Key used at the desk: the drawer, the Lunar Refractor - the stalker stronger, the summoner on
study-key 1 items send give-item  10 run-frames  lunar-refractor items:items-of before !
0 -3e 0e 31e put  5 use-on-place  600 read-on
T{ got-lunar-refractor story-flag?  lunar-refractor items:items-of  study-key items:items-of  summoner-on state-flag? -> -1 before @ 1+ 0 -1 }T
T{ story.base:slot2-kind @ -> debilitas-7-kind }T
summoner-on state-flag-clear

testing Hewie: a bark at the desk (the refractor in it still), once a visit
got-lunar-refractor story-flag-clear  hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh
castle-2f-6 3.5e 0e 30e nav-tri 0 game:hewie send hewie-place  30 run-frames
T{ [: [: hewie-noticed @ ;] in-place ;] 90 soon -> -1 }T
test-summary
