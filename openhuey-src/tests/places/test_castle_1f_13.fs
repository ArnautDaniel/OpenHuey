\ castle-1f-13's place (scripts/story/places/castle-1f-13.fs): the great hall - coming in (the door
\ shown open or shut, the balcony fallen or up), the remarks (the fallen balcony, the false doors in
\ turn), the Chicken Jerky, the Antimony Powder (Hewie sniffing it out; picked up), the hanging thing
\ swinging, the hiding place (in, out; found), the stalker searching while she hides, Debilitas back
\ from castle-b1-7, brought in by the forced way (the balcony falling, him above; seized), the
\ replays. Headless:
\   build/openhuey --test tests/places/test_castle_1f_13.fs
IN: test-castle-1f-13
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names floor-flags fiona.state story.kit story.places.castle-1f-13 doors items.names ;

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
: room-afresh ( -- )  castle-1f-13 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)
: hewie-spot ( F: x y z -- )  on-floor f>cell f>cell f>cell swap rot  0 game:hewie send place  5 run-frames ;
: hewie-here ( F: x y z -- )  {: F: x y z :}  castle-1f-13 x y z nav-tri 0 game:hewie send hewie-place  30 run-frames  x y z hewie-spot ;
: filed? ( doc -- flag )
    128 0 do  progress pr.files i 2* + w@ over = if  drop true unloop exit  then  loop  drop false ;
: replays ( v -- )   \ (the extras' theatre: that movie, here)
    $2A var!  events-held state-flag-set  castle-1f-13 -1 rooms send go-to-room  5 run-frames ;
: replayed? ( -- flag )
    [: cutscene-active? movie-status 1 = or ;] 200 soon
    [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon and ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: to-hide ( -- )  -110 -266e 0e -134e put  circle press  [: hidden? ;] 900 soon drop ;   \ (the area's middle: by the hiding spot, on its own floor)
: crows-in ( -- )  [: cast:crow 3 figure-in  cast:crow-2 4 figure-in ;] in-place  10 run-frames ;   \ (as the room before loads them)
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in (the balcony fallen): the door shut, the Jerky and the Antimony Powder Hewie found showing
found-file-227 story-flag-set  found-file-227-2 story-flag-set  c1f-13-chicken-jerky story-flag-clear
c1f-13-antimony-powder-found story-flag-set  c1f-13-antimony-powder story-flag-clear  room-afresh
T{ door-shut-part part?  door-open-part part?  [: jerky-fx effect @ 0<>  antimony-fx effect @ 0<> ;] in-place -> -1 0 -1 -1 }T
testing ... its door open
hall-door s" doors" actor-named send unlock  [: hall-door true open-mark ;] in-place  room-afresh
T{ door-open-part part? -> -1 }T
[: hall-door false open-mark ;] in-place

testing the remarks: the fallen balcony in turn, the false doors (each in turn)
T{ 90 -258e 0e 25e put  said  90 -258e 0e 25e put  said -> balcony-msg balcony-2-msg }T   \ (facing it)
T{ false-door-area 45 at  said -> a-door-msg }T
300 read-on
T{ false-door-area 45 at  said -> prank-msg }T
T{ false-door-2-area 45 at  said -> a-door-msg }T
300 read-on

testing the Jerky and the Antimony Powder: picked up
chicken-jerky items:items-of before !
-283.73e 0e 2.22e picked
T{ c1f-13-chicken-jerky story-flag?  chicken-jerky items:items-of -> -1 before @ 1+ }T
antimony-powder items:items-of before !
-254.5e 0e -95e picked
T{ c1f-13-antimony-powder story-flag?  antimony-powder items:items-of -> -1 before @ 1+ }T

testing Hewie sniffing out the Antimony Powder
c1f-13-antimony-powder-found story-flag-clear  c1f-13-antimony-powder story-flag-clear
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  -254.5e 0e -90e hewie-here
[: ['] antimony-sniffed start drop ;] in-place
T{ [: c1f-13-antimony-powder-found story-flag? ;] 600 soon  [: antimony-fx effect @ 0<> ;] in-place -> -1 -1 }T
200 run-frames  game:hewie send part-from-fiona

testing the hanging thing: her steps by it set it swinging (its creak)
room-afresh  0 -277e 0e -145e put   \ (a long step in: past the push)
T{ [: hanger-rung @ ;] in-place -> -1 }T
0 -277e 0e -141e put  0 -277e 0e -145e put   \ (two of 4: past it again - the creak off)
T{ [: hanger-rung @ ;] in-place -> 0 }T

testing the hiding place: crouched behind it (hidden), out by the button
room-afresh  to-hide
T{ hidden? -> -1 }T
circle press  [: doing-busy? 0= ;] 600 soon drop
T{ hidden?  doing-busy? -> 0 0 }T
testing ... found: away
to-hide
T{ hidden? -> -1 }T
[: -1 found-her ! ;] in-place  [: doing-busy? 0= ;] 600 soon drop
T{ hidden?  doing-busy? -> 0 0 }T

testing the stalker coming while she hides: his comings counted
2 story.words:partner-load  5 run-frames
to-hide
search-var var@ before !
castle-1f-13 $1A1 game:debilitas send stalker-in  60 run-frames
T{ hidden?  [: came-while-hidden @ ;] in-place  search-var var@ -> -1 -1 before @ 1+ }T
game:debilitas send stalker-out  300 run-frames  circle press  200 run-frames

testing Debilitas back from castle-b1-7 (loaded, not in the game): in here, searching
summoner-on state-flag-set  cb1-7-flag2 story-flag-set  room-afresh
T{ cb1-7-flag2 story-flag?  game:debilitas body?  game:debilitas body-room -> 0 -1 castle-1f-13 }T
game:debilitas send stalker-out  summoner-on state-flag-clear  60 run-frames

testing brought in by the forced way, the first time: the balcony falls - Debilitas above, the file
found-file-227 story-flag-clear  found-file-227-2 story-flag-clear
crows-in  castle-1f-13 forced-exit rooms send go-to-room  5 run-frames
T{ [: movie-skipped state-flag-set  found-file-227 story-flag? ;] 1500 soon -> -1 }T
T{ [: doing-busy? 0= ;] 900 soon  crows-file filed?  game:debilitas body-room -> -1 -1 castle-1f-13 }T
game:debilitas send stalker-out  60 run-frames
testing ... again: she just comes to
crows-in  castle-1f-13 forced-exit rooms send go-to-room  5 run-frames
T{ [: doing-busy? 0= ;] 300 soon  scene-locked state-flag? -> -1 0 }T

testing brought in seized: the movie, out by the way loaded ahead
crows-in  castle-1f-13 $81 rooms send go-to-room  5 run-frames
T{ [: movie-skipped state-flag-set  room-id beyond-room = ;] 1500 soon  seized-seen resident-flag? -> -1 -1 }T
free-play  10 run-frames  0 game:fiona send costume

testing the movies replayed (the extras): seized, the balcony falling
T{ 25 replays  replayed? -> -1 }T
free-play  10 run-frames  0 game:fiona send costume
T{ 27 replays  replayed? -> -1 }T
test-summary
