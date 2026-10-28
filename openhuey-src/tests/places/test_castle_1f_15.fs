\ castle-1f-15's place (scripts/story/places/castle-1f-15.fs): the armour gallery - the remarks (the
\ diorama, the spear, the armour and the handrail in turn), the hole (no; down it), landing from it
\ above, the Camomile and the Medallion (Hewie sniffing them out; picked up), his bark at the high
\ window, the balcony's encounter (caught; saved; failed), the replays. Headless:
\   build/openhuey --test tests/places/test_castle_1f_15.fs
IN: test-castle-1f-15
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names floor-flags fiona.state story.kit story.places.castle-1f-15 doors items.names ;

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
: room-afresh ( -- )  castle-1f-15 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)
: hewie-spot ( F: x y z -- )  on-floor f>cell f>cell f>cell swap rot  0 game:hewie send place  5 run-frames ;
: hewie-here ( F: x y z -- )  {: F: x y z :}  castle-1f-15 x y z nav-tri 0 game:hewie send hewie-place  30 run-frames  x y z hewie-spot ;
: map-page@ ( -- page )  game:subscreen enter  subscreen.state:her-pg @  leave-actor ;
: filed? ( doc -- flag )
    128 0 do  progress pr.files i 2* + w@ over = if  drop true unloop exit  then  loop  drop false ;
: replays ( v -- )   \ (the extras' theatre: that movie, here)
    $2A var!  events-held state-flag-set  castle-1f-15 -1 rooms send go-to-room  5 run-frames ;
: replayed? ( -- flag )
    [: cutscene-active? movie-status 1 = or ;] 200 soon
    [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon and ;
: hunted-down ( -- )   \ (Debilitas in the game elsewhere, she chased - not yet in a panic)
    0e f>cell s" panic" actor-named send threat  composed s" panic" actor-named send panic-stage!
    castle-1f-4 100 game:debilitas send stalker-in  10 run-frames  force-chased state-flag-set ;
: encounter-over? ( -- flag )  [: [: encounter-done @ ;] in-place  movie-skipped state-flag-set ;] 1500 soon ;
: at-choice? ( -- flag )  game:window enter  window:choosing? window:page-typed? and  leave-actor ;
: answers-no ( -- )   \ (the pages read to the choice, then the second option)
    100 0 do  at-choice? if  leave  then  cross press  10 run-frames  loop
    key: Down tap  10 run-frames  cross press  100 run-frames ;
: hidden? ( -- flag )  game:debilitas enter  stalker.state:hidden @  leave-actor ;
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the Camomile and the Medallion Hewie found showing
c1f-15-camomile-found story-flag-set  c1f-15-camomile story-flag-clear
c1f-15-medallion-found story-flag-set  c1f-15-medallion story-flag-clear  room-afresh
T{ [: camomile-fx effect @ 0<>  medallion-fx effect @ 0<> ;] in-place -> -1 -1 }T

testing the remarks: the diorama, the spear in turn, the armour in turn, the handrail in turn
T{ 180 -0.195e 50e -27.107e put  said -> diorama-msg }T   \ (where she reads it: the area's middle is on blocked floor)
T{ 45 45e 50e -99e put  said  45 45e 50e -99e put  said -> spear-msg spear-2-msg }T   \ (facing its middle)
T{ armour-area 180 at  said  armour-2-area 180 at  said -> armour-msg armour-2-msg }T
T{ handrail-area -90 at  said  handrail-area -90 at  said -> handrail-msg handrail-2-msg }T

testing the Camomile and the Medallion: picked up
camomile items:items-of before !
34.3e 0e -13.7e picked
T{ c1f-15-camomile story-flag?  camomile items:items-of -> -1 before @ 1+ }T
medallion items:items-of before !
56.3e 50e -71.5e picked
T{ c1f-15-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T

testing Hewie: the Camomile and the Medallion sniffed out; a bark at the high window once a visit
c1f-15-camomile-found story-flag-clear  c1f-15-camomile story-flag-clear
c1f-15-medallion-found story-flag-clear  c1f-15-medallion story-flag-clear
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  34.3e 0e -9e hewie-here
[: ['] camomile-sniffed start drop ;] in-place
T{ [: c1f-15-camomile-found story-flag? ;] 600 soon  [: camomile-fx effect @ 0<> ;] in-place -> -1 -1 }T
200 run-frames  [: ['] medallion-sniffed start drop ;] in-place
T{ [: c1f-15-medallion-found story-flag? ;] 600 soon  [: medallion-fx effect @ 0<> ;] in-place -> -1 -1 }T
200 run-frames  room-afresh  -88e 50e -94.617e hewie-here   \ (afresh: at her command again)
T{ [: [: hewie-noticed @ ;] in-place ;] 60 soon -> -1 }T
200 run-frames

testing the hole: not down it; down it (to the room below, loaded ahead)
room-afresh  hole-area -90 at  circle press  [: window-shown hole-msg = ;] 300 soon drop
answers-no
T{ room-id -> castle-1f-15 }T
T{ doing-busy? -> 0 }T
T{ stalkers-stay state-flag? -> 0 }T
below-room s" rooms" actor-named send room-preload  hole-area -90 at
circle press
T{ [: window-shown 0< 0= if  cross press  then  room-id below-room = ;] 900 soon -> -1 }T   \ (yes)
free-play  10 run-frames  0 game:fiona send costume

testing dropped in from the hole above: she lands - the map's page
2 [: subscreen.state:her-pg ! ;] game:subscreen enter execute leave-actor
castle-1f-15 hole-exit rooms send go-to-room  60 run-frames
T{ [: doing-busy? 0= ;] 300 soon  map-page@  stalkers-stay state-flag? -> -1 3 0 }T

testing the balcony's encounter, no Hewie: caught - left lying, frightened
2 story.base:partner-load  5 run-frames  game:hewie send part-from-fiona  room-afresh  hunted-down
balcony-area 0 at
T{ encounter-over?  caught-seen resident-flag?  game:debilitas body-room -> -1 -1 castle-1f-15 }T
120 read-on  force-chased state-flag-clear  game:debilitas send stalker-out

testing ... with Hewie: saved - Debilitas knocked down, the file
0 saves-var var!  room-afresh  49e 50e 70e hewie-here  hunted-down  balcony-area 0 at
T{ encounter-over? -> -1 }T
T{ saved-seen resident-flag? -> -1 }T
T{ saves-var var@ -> 1 }T
T{ saved-file $7FFF and filed? -> 0 }T   \ (the file only in the other game)
T{ hidden? -> 0 }T
120 read-on  force-chased state-flag-clear  game:debilitas send stalker-out

testing ... with Hewie, failing: left lying
room-afresh  49e 50e 70e hewie-here  [: ['] failed start drop ;] in-place
T{ encounter-over?  failed-seen resident-flag?  game:debilitas body-room -> -1 -1 castle-1f-15 }T
120 read-on  game:debilitas send stalker-out  free-play  10 run-frames  0 game:fiona send costume

testing the movies replayed (the extras): saved, failed, caught
T{ 4 replays  replayed? -> -1 }T
free-play  10 run-frames  0 game:fiona send costume
T{ 5 replays  replayed? -> -1 }T
free-play  10 run-frames  0 game:fiona send costume
T{ 25 replays  replayed? -> -1 }T
test-summary
