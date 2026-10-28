\ castle-1f-3's place (scripts/story/places/castle-1f-3.fs): the library - the remarks (the shelf from
\ before and behind, the books, the harp, the creepy thing in turn), her thought pushing the shelf,
\ the Camomile, the awakening at the Garden Key and the key taken, Hewie wary by the table, the hint
\ (chased, Debilitas elsewhere) and its while, the shelf knocked down on him, the replays. Headless:
\   build/openhuey --test tests/places/test_castle_1f_3.fs
IN: test-castle-1f-3
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names floor-flags fiona.state story.kit story.places.castle-1f-3 doors items.names ;

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
: room-afresh ( -- )  castle-1f-3 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: hewie-spot ( F: x y z -- )  on-floor f>cell f>cell f>cell swap rot  0 game:hewie send place  5 run-frames ;
: replays ( v -- )   \ (the extras' theatre: that movie, here)
    $2A var!  events-held state-flag-set  castle-1f-3 -1 rooms send go-to-room  5 run-frames ;
: replayed? ( -- flag )
    [: cutscene-active? movie-status 1 = or ;] 200 soon
    [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon and ;
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in (awakened, the shelf up): the key and the thing on the table, the Camomile showing
c1f-3-flag1 story-flag-set  c1f-3-bookshelf-examined story-flag-clear  got-garden-key story-flag-clear
c1f-3-camomile story-flag-clear  room-afresh
T{ key-part part?  thing-part part?  table-part part?  fallen-part part?  [: camomile-fx effect @ 0<> ;] in-place -> -1 -1 0 0 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)

testing the remarks: the shelf from before and behind, the books in turn, the harp in turn
T{ shelf-area 180 at  said  behind-shelf-area 0 at  said -> shelf-msg shelf-msg }T
T{ books-area -134 at  said  books-area -134 at  said -> books-msg books-2-msg }T
T{ harp-area 90 at  said  harp-area 90 at  said -> harp-msg harp-2-msg }T
testing ... the creepy thing: in turn (awakened); before, snooping
T{ thing-area -90 at  said  thing-area -90 at  said  thing-area -90 at  said -> creepy-msg creepy-2-msg creepy-2-msg }T
c1f-3-flag1 story-flag-clear
T{ thing-area -90 at  said -> snooping-msg }T
c1f-3-flag1 story-flag-set

testing pushing the shelf: her thought
room-afresh  [: -1 straining-now ! ;] in-place
T{ [: window-shown knock-msg = ;] 120 soon -> -1 }T
[: 0 straining-now ! ;] in-place  120 read-on

testing the Camomile: reached, taken
camomile items:items-of before !
180 -6e 0e -42e put  circle press  300 read-on
T{ c1f-3-camomile story-flag?  camomile items:items-of  [: camomile-fx effect @ ;] in-place -> -1 before @ 1+ 0 }T

testing the Garden Key the first time: the awakening - the thing on the table, then the key taken
c1f-3-flag1 story-flag-clear  room-afresh  0 10e 0e -21e put  circle press  900 play-on
T{ c1f-3-flag1 story-flag?  thing-part part?  table-part part?  got-garden-key story-flag? -> -1 -1 0 0 }T
garden-key items:items-of before !
0 10e 0e -21e put  circle press  300 read-on
T{ got-garden-key story-flag?  key-part part?  garden-key items:items-of -> -1 0 before @ 1+ }T

testing Hewie by the table: wary once a visit
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh
castle-1f-3 13.13e 0e -30.63e nav-tri 0 game:hewie send hewie-place  30 run-frames  13.13e 0e -30.63e hewie-spot
T{ [: [: hewie-wary @ ;] in-place ;] 120 soon -> -1 }T
300 run-frames

testing chased, Debilitas elsewhere, coming in: the hint - the stalkers kept off, then let go
2 story.base:partner-load  5 run-frames  castle-1f-4 100 game:debilitas send stalker-in  10 run-frames
c1f-3-bookshelf-hint story-flag-clear  force-chased state-flag-set  room-afresh
T{ c1f-3-bookshelf-hint story-flag?  [: window-shown hint-msg = ;] 120 soon  [: chase-scene @ ;] in-place -> -1 -1 -1 }T
500 read-on
T{ [: chase-scene @ ;] in-place  force-chased state-flag? -> 0 0 }T

testing ... in behind the shelf: knocked down on him - he lies under it, she runs out
castle-1f-4 100 game:debilitas send stalker-in  10 run-frames   \ (out of the game meanwhile: back)
force-chased state-flag-set  room-afresh  behind-shelf-area 0 at
T{ [: c1f-3-bookshelf-examined story-flag?  room-id castle-1f-3 <> and  movie-skipped state-flag-set ;] 1500 soon -> -1 }T
T{ game:debilitas body-room  shelf-seen resident-flag? -> castle-1f-3 -1 }T
T{ game:debilitas enter  stalker.state:hidden @  leave-actor -> 0 }T   \ (hidden before it, shown again: the scene had him)
force-chased state-flag-clear  game:debilitas send stalker-out  free-play  10 run-frames  0 game:fiona send costume
room-afresh
T{ fallen-part part? -> -1 }T

testing the movies replayed (the extras): the awakening, the shelf by day, by night
T{ 2 replays  replayed? -> -1 }T
free-play  10 run-frames  0 game:fiona send costume
T{ 8 replays  replayed? -> -1 }T
free-play  10 run-frames  0 game:fiona send costume
T{ 9 replays  replayed? -> -1 }T
test-summary
