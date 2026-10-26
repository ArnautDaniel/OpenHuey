\ castle-1f-1's place (scripts/story/places/castle-1f-1.fs): the entrance hall - the gate shut,
\ the pond's splashes, Hewie fetching the Refined Magnesia, the crouch evasion tip, the creature let
\ out once, the way in from the bookshelf. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_1f_1.fs
IN: test-castle-1f-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-1f-1 items.names ;

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
: room-afresh ( -- )  castle-1f-1 room!  60 run-frames ;
: facing ( area F: px pz -- )   \ (in the area's middle, facing the point)
    {: area F: px pz :}  area area-middle drop {: F: x y z :}
    px x f-  pz z f-  fatan2 rad>deg f>s  x y z put ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )   \ (her blow's point there)
    blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)

: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;

free-play  10 run-frames  0 game:fiona send costume
testing the gate shut (castle-1f-2's way not yet open): shown, its floor solid; facing it, she says so
c1f-2-flag1 story-flag-clear  room-afresh
T{ gate-shut-part part?  gate-area -67 at  said -> -1 gate-shut-msg }T
testing ... open: the other part, nothing said
c1f-2-flag1 story-flag-set  room-afresh
T{ gate-open-part part?  gate-area -67 at  said -> -1 -1 }T

testing the pond: a splash as she steps in (once a visit)
room-afresh
T{ [: splashed-1 @ ;] in-place -> 0 }T
0 -46e -16e 90e put  0 -46e -16e 70e put  5 run-frames
T{ [: splashed-1 @ ;] in-place -> -1 }T

testing the crouch evasion tip: in the hall's crossing, free - the tip, its file
got-doc-crouch-evasion story-flag-clear  room-afresh
tip-area 0 at  300 read-on
T{ got-doc-crouch-evasion story-flag?  crouch-doc items:items-of 0> -> -1 -1 }T

testing the creature let out once, the garden's door done
garden-door-done story-flag-set  c1f-1-flag1 story-flag-clear  room-afresh
T{ c1f-1-flag1 story-flag? -> -1 }T

testing Hewie sent for the Refined Magnesia (her "go for it", facing it, the spot off the screen): he fetches it
hewie-freed story-flag-set  hewie-commandable state-flag-set  c1f-1-refined-magnesia story-flag-clear  room-afresh
game:hewie send join-fiona  60 run-frames
variable before  refined-magnesia items:items-of before !
[: ['] fetches start drop ;] in-place  1500 play-on
T{ c1f-1-refined-magnesia story-flag?  refined-magnesia items:items-of -> -1 before @ 1+ }T

testing back by the bookshelf's way: she and Hewie in, the file, the music on
c1f-3-bookshelf-examined story-flag-set  c1f-1-flag2 story-flag-clear  room-afresh
300 play-on
T{ c1f-1-flag2 story-flag?  [: hewie-arrived @ ;] in-place  arrived-file filed? -> -1 -1 -1 }T
test-summary
