\ castle-1f-8's place (scripts/story/places/castle-1f-8.fs): the telephone room - the flames, the
\ remarks (the dead line, the chair, nothing much, the ornaments), the Magnesia, the Album Medallion
\ (Hewie sniffing it out; picked up), the hiding place (in, out; found), the stalker coming while
\ she hides (his search), the call (the voice, the stalker in after it), the replay. Headless:
\   build/openhuey --test tests/places/test_castle_1f_8.fs
IN: test-castle-1f-8
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-1f-8 doors items.names ;

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
: room-afresh ( -- )  castle-1f-8 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)
: across ( gate side -- )   \ (stepped over the gate's line: from the other side to this one, 4 from it)
    {: g side | F: ax F: ay F: az F: bx F: bz F: ox F: oz :}
    g 0 area-corner drop  to az to ay to ax   g 1 area-corner drop  to bz fdrop to bx
    bz az f- to ox  bx ax f- fnegate to oz   ox ox f* oz oz f* f+ fsqrt 4e fswap f/
    fdup ox f* side s>f f* to ox  oz f* side s>f f* to oz
    0  ax bx f+ 2e f/ ox f-  ay  az bz f+ 2e f/ oz f-  put
    0  ax bx f+ 2e f/ ox f+  ay  az bz f+ 2e f/ oz f+  put  10 run-frames ;
: filed? ( doc -- flag )
    128 0 do  progress pr.files i 2* + w@ over = if  drop true unloop exit  then  loop  drop false ;
: flames-up ( -- n )  [: 0  4 0 do  i effect @ alive? if  1+  then  loop ;] in-place ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in (after the call): the four flames, the Magnesia showing, the receiver on its hook
c1f-8-flag1 story-flag-set  c1f-8-magnesia story-flag-clear  c1f-8-album-medallion-found story-flag-clear  room-afresh
T{ flames-up  [: magnesia-fx effect @ 0<>  album-fx effect @ ;] in-place  receiver-part part? -> 4 -1 0 -1 }T

testing the remarks: the dead line, the chair, nothing much, the ornaments
T{ 75 19e 0e -19e put  said -> line-cut-msg }T   \ (the area's middle is by the table, on blocked floor)
T{ chair-area 45 at  said -> chair-msg }T
T{ 53 -8e 0e 22e put  said -> nothing-msg }T   \ (facing its middle, clear of the Magnesia)
T{ ornaments-area -67 at  said -> ornaments-msg }T

testing the Magnesia: reached, taken
magnesia items:items-of before !
0 0e 0e 21e put  circle press  300 read-on
T{ c1f-8-magnesia story-flag?  magnesia items:items-of  [: magnesia-fx effect @ ;] in-place -> -1 before @ 1+ 0 }T

testing the Album Medallion Hewie sniffs out: found, it shows; picked up
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh
castle-1f-8 299 0 game:hewie send hewie-place  60 run-frames
[: ['] album-sniffed start drop ;] in-place
T{ [: c1f-8-album-medallion-found story-flag? ;] 600 soon  [: album-fx effect @ 0<> ;] in-place -> -1 -1 }T
120 read-on
album-medallion items:items-of before !
-25.6e 0e 27.6e picked
T{ c1f-8-album-medallion story-flag?  album-medallion items:items-of -> -1 before @ 1+ }T

testing the hiding place: in (hidden), out by the button
room-afresh  hiding-area 90 at  circle press  300 run-frames
T{ hidden? -> -1 }T
circle press  200 run-frames
T{ hidden? -> 0 }T
testing ... found: hauled out
hiding-area 90 at  circle press  300 run-frames
T{ hidden? -> -1 }T
[: -1 found-her ! ;] in-place  200 run-frames
T{ hidden? -> 0 }T

testing the stalker coming while she hides: his comings counted
2 story.base:partner-load  5 run-frames
hiding-area 90 at  circle press  300 run-frames
search-var var@ before !
castle-1f-8 $B2 game:debilitas send stalker-in  60 run-frames
T{ hidden?  [: came-while-hidden @ ;] in-place  search-var var@ -> -1 -1 before @ 1+ }T
game:debilitas send stalker-out  300 run-frames  circle press  200 run-frames

testing the call (stepping out of the doorway, the first time): the voice, the door locked, the
testing ... file, the stalker in after her
c1f-8-flag1 story-flag-clear  found-file-208 story-flag-clear  call-door s" doors" actor-named send unlock  room-afresh
call-area 1 across  call-area -1 across
T{ [: movie-skipped state-flag-set  c1f-8-flag1 story-flag?  doing-busy? 0= and ;] 1500 soon -> -1 }T
T{ found-file-208 story-flag?  call-door door-locked?  call-file filed?  summoner-on state-flag? -> -1 -1 -1 -1 }T
T{ game:debilitas body?  game:debilitas body-room -> -1 castle-1f-8 }T
game:debilitas send stalker-out  summoner-on state-flag-clear  60 run-frames

testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  castle-1f-8 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
