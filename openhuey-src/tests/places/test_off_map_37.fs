\ off-map-37's place (the extras room): as she comes in - Hewie at her command, not to attack, the
\ unlocks that follow, both doors locked, the fan turning; from the title (exit 0) - the "Secret
\ Room" document the first time, the costumes earned given; back from a replay ($80) - at the
\ theatre, its menu; back from the run ($81) - at the door; each corner named as she faces it;
\ its menus: the galleries, the changing area, the theatre (nothing chosen), the mini-game (nothing
\ chosen), a replay chosen - out to its room; the exit - out to the title. Headless:
\   build/openhuey --test tests/places/test_off_map_37.fs
IN: test-off-map-37
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names gameover doors story.places.off-map-37 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: his ( xt -- x )  game:hewie enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )   \ (on the floor, on a triangle she walks - else the nearest the mesh has)
    on-floor s>f tp-facing  [: her-at $28020018 v-tri-in  dup 0< if  drop her-at vec@ nav-tri  then  her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: hput ( deg F: x y z -- )  on-floor  s>f deg>rad xyzw>cells  game:hewie send place  10 run-frames ;   \ (Hewie, there on the floor)
: hat ( area deg -- )  >r area-middle drop r> hput ;
: his-camera@ ( -- set path )  game:hewie cam-setup-of ;
: his-tri ( -- tri )  [: self body-tri ;] his ;
: on-tri ( tri -- )  0 swap tri-center put ;
: shown-soon ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
: said ( -- msg )
    circle press  shown-soon
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: spare@ ( -- room )  game:rooms enter  rooms:spare @  leave-actor ;   \ (the room loaded ahead; -1 none)
: her-camera@ ( -- set path )  game:fiona cam-setup-of ;
: across ( gate side -- )   \ (stepped over the gate's line: from the other side to this one, 4 from it)
    {: g side | F: ax F: ay F: az F: bx F: bz F: ox F: oz :}
    g 0 area-corner drop  to az to ay to ax   g 1 area-corner drop  to bz fdrop to bx
    bz az f- to ox  bx ax f- fnegate to oz   ox ox f* oz oz f* f+ fsqrt 4e fswap f/
    fdup ox f* side s>f f* to ox  oz f* side s>f f* to oz
    0  ax bx f+ 2e f/ ox f-  ay  az bz f+ 2e f/ oz f-  put
    0  ax bx f+ 2e f/ ox f+  ay  az bz f+ 2e f/ oz f+  put  10 run-frames ;
: hacross ( gate side -- )   \ (Hewie stepped over the gate's line, as across)
    {: g side | F: ax F: ay F: az F: bx F: bz F: ox F: oz :}
    g 0 area-corner drop  to az to ay to ax   g 1 area-corner drop  to bz fdrop to bx
    bz az f- to ox  bx ax f- fnegate to oz   ox ox f* oz oz f* f+ fsqrt 4e fswap f/
    fdup ox f* side s>f f* to ox  oz f* side s>f f* to oz
    0  ax bx f+ 2e f/ ox f-  ay  az bz f+ 2e f/ oz f-  hput
    0  ax bx f+ 2e f/ ox f+  ay  az bz f+ 2e f/ oz f+  hput  10 run-frames ;
: hinto ( area -- )   \ (a gate: crossed one way - the one that gives him a camera, if either does - else back too)
    dup area-kind if  0 hat exit  then
    game:hewie 99 99 cam-char-setup  dup 1 hacross  his-camera@ 99 = swap 99 = and if  -1 hacross  else  drop  then ;
: into ( area -- )   \ (a gate crossed both ways; else its middle)
    dup area-kind 0= if  dup 1 across  -1 across  else  0 at  then ;
: leads ( room exit -- room' )  room-exit-leads drop ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: replay-for ( act -- )   \ (the theatre's variable set to a movie its room replays by that act)
    {: act :}  110 0 do  i $2A var!  [: replay-act ;] in-place act = if  unloop exit  then  loop ;
: over-kind ( -- kind )  game:gameover enter  gameover:kind @  leave-actor ;
: played-till ( xt n -- flag )   \ (played on - its messages read, its movies skipped - till xt holds)
    0 ?do  dup execute if  drop true unloop exit  then
        window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop
    drop false ;
: quiet ( -- )  events-held state-flag-clear  off-map-0d 1 room-by!  30 run-frames  world-held state-flag-set ;   \ (between replays: a room of no scenes)
variable before
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
: resident-flag-clear ( n -- )  progress pr.resident bit-clear ;
: menu-up? ( -- flag )  subscreen-wanted state-flag? ;
: menu-shut ( -- )  [: menu-up? ;] 600 soon drop  10 run-frames  [: triangle press  menu-up? 0= ;] 300 soon drop  60 run-frames ;
: in-by ( exit -- )  off-map-37 swap room-by!  60 run-frames ;
: at-corner ( area deg -- )  at  circle press ;
: fan@ ( F: -- a )  [: s" fan" turn-y placed-f@ ;] in-place ;
testing from the title (exit 0): placed, Hewie at her command and not to attack, both doors locked
1 resident-flag-set  2 resident-flag-set  3 resident-flag-set  4 resident-flag-set  $15 resident-flag-clear
$10 resident-flag-clear  $13 resident-flag-clear  doc-secret-room items:items-of before !
T{ 0 in-by  [: her-tri @ ;] her  hewie-commandable state-flag?  hewie-no-attack state-flag?  $10C doors:door-locked?  $10D doors:door-locked? -> $15 -1 -1 -1 -1 }T
testing ... the unlocks that follow (all four of the first marked: $15); the fan turning
T{ $15 resident-flag?  fan@  10 run-frames  fan@ f= -> -1 0 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing ... the "Secret Room" document, the first time; the costume earned ($13 by the first's mark)
T{ 600 read-on  $10 resident-flag?  doc-secret-room items:items-of  $13 resident-flag?  $25C filed? -> -1 before @ 1+ -1 -1 }T
testing ... again: the document not said again (given once more: still the one)
T{ 0 in-by  [: window-shown secret-room-msg = ;] 200 soon  300 read-on  doc-secret-room items:items-of -> 0 before @ 1+ }T
testing a corner named as she faces it: the character gallery's label up; turned away, down
T{ 5 180 at  window-shown  5 0 at  window-shown -> character-label -1 }T
testing the galleries: the models' (5), the music (4), the art (6), the play types ($C) - each its menu
T{ 5 180 at-corner  [: menu-up? ;] 600 soon -> -1 }T
menu-shut
T{ 4 180 at-corner  [: menu-up? ;] 600 soon -> -1 }T
menu-shut
T{ 6 -90 at-corner  [: menu-up? ;] 600 soon -> -1 }T
menu-shut
T{ $C 90 at-corner  [: menu-up? ;] 600 soon -> -1 }T
menu-shut
testing the changing area ($15): the costumes' menu, then she as she was
T{ $15 180 at-corner  [: menu-up? ;] 600 soon -> -1 }T
menu-shut
testing the theatre: its menu; nothing chosen - the room's track, the replays' page back to 0
variable what
: doing ( xt -- )  what !  [: what @ fiona start-holding drop ;] in-place ;
T{ 0 in-by  300 read-on  ' the-theatre doing  [: menu-up? ;] 600 soon -> -1 }T
$FF $2A var!  5 $2B var!  menu-shut   \ (the triangle: nothing chosen - $2A $FF)
T{ $2B var@  [: doing-busy? 0= ;] 300 soon drop  room-id -> 0 off-map-37 }T
testing ... a movie chosen (the eighth, seen): its room loaded ahead ($25), out by the forced way, the theatre held
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: unlock-all ( -- )  96 0 do  i resident-flag-set  loop ;
unlock-all  7 $2B var!
T{ ' the-theatre doing  [: menu-up? ;] 600 soon -> -1 }T
30 run-frames  key: X tap
T{ [: room-id off-map-37 <> ;] 600 soon  room-id  $2A var@  events-held state-flag? -> -1 $25 7 -1 }T
testing back from a replay ($80): at the theatre, its menu again
events-held state-flag-clear
T{ forced-exit in-by  [: menu-up? ;] 600 soon  [: her-tri @ ;] her -> -1 $11B }T
$FF $2A var!  menu-shut
testing the mini-game: its menu; nothing chosen - she stays, the room's track
T{ ' mini-game doing  [: menu-up? ;] 600 soon -> -1 }T
menu-shut
T{ $2E var@  [: doing-busy? 0= ;] 300 soon drop  room-id -> $FF off-map-37 }T
testing back from the run ($81): at the door, Hewie by her
off-map-37 267 0 game:hewie send hewie-place  60 run-frames   \ (Hewie in the room)
T{ $81 in-by  300 read-on  [: her-tri @ ;] her  [: self body-tri ;] his -> $13 $C4 }T
testing the exit: out - faded, she walked there, the world held, the quit asked for
T{ ' out doing  [: quit-wanted state-flag? ;] 600 soon  world-held state-flag? -> -1 -1 }T
quit-wanted state-flag-clear
test-summary
