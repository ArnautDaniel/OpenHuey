\ castle-2f-12's place (scripts/story/places/castle-2f-12.fs): the nursery - coming in (the wardrobe,
\ the chairs, the box), the chairs rocking and dying down, the remarks, the box opened (and nothing),
\ the vase's Medallion, the Refined Antimony, Hewie watching the chair, the wardrobe (in, out; in a
\ panic), the stalker while she hides. Headless:
\   build/openhuey --test tests/places/test_castle_2f_12.fs
IN: test-castle-2f-12
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names floor-flags fiona.state story.kit story.places.castle-2f-12 doors items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: false  #activities 0 do  i act-slot @ 0<>  i cells act-xts + @ ['] rocking <> and  or  loop ;] in-place ;   \ (the chairs: always)
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: room-afresh ( -- )  castle-2f-12 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: replayed? ( -- flag )
    [: cutscene-active? movie-status 1 = or ;] 200 soon
    [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon and ;



: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;
: hewie-spot ( F: x y z -- )  on-floor f>cell f>cell f>cell swap rot  0 game:hewie send place  5 run-frames ;
: hewie-here ( F: x y z -- )  {: F: x y z :}  castle-2f-12 x y z nav-tri 0 game:hewie send hewie-place  30 run-frames  x y z hewie-spot ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
: lid-turn ( F: -- t )  s" left" turn-y object@ ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
: struck ( xt F: x y z -- )  blow-at  in-place  [: body-point-off ;] as-her  10 run-frames ;
fvariable shut-lid
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the wardrobe shut, the chairs rocking the first time; the box shut, and open (at once) once opened
c2f-12-flag1 story-flag-clear  c2f-12-flag2 story-flag-clear  got-jade-choker story-flag-clear  room-afresh
T{ doors-part part?  [: still-rocking? ;] in-place  c2f-12-flag2 story-flag? -> 0 -1 -1 }T
lid-turn shut-lid f!  got-jade-choker story-flag-set  room-afresh
T{ shut-lid f@ lid-turn f- 2.25e ~= -> -1 }T
got-jade-choker story-flag-clear

testing the chairs: set rocking, dying down
: rocks ( -- n )  [: 3 0 do  i chair-still  loop  false chairs-set  0  begin  1+  3 0 do  i chair-rocks  loop  still-rocking? 0=  over 5000 > or  until ;] in-place ;
T{ rocks 5000 < -> -1 }T
testing ... a blow by the first: rocking again
room-afresh  [: 3 0 do  i chair-still  loop ;] in-place  -4e 1e 4e blow-at   \ (the frame's check, as the blow lands)
T{ [: -4e 0e 4e 4e 7e struck-in? if  true chairs-set  then  still-rocking? ;] in-place -> -1 }T
[: body-point-off ;] as-her

testing the remarks: the caged doll in turn, the horse in turn, the doll in the chair, the box in turn, the crib
room-afresh
T{ 49 16e 0e -2e put  said  49 16e 0e -2e put  said -> caged-doll-msg so-real-msg }T
T{ horse-area 150 at  said  horse-area 150 at  said -> horse-msg childhood-msg }T
T{ 74 -6e 0e -3e put  said -> chair-doll-msg }T
T{ 180 2e 0e -33e put  said  180 2e 0e -33e put  said -> box-msg slot-msg }T   \ (off its middle)
T{ 53 -27e 0e -25e put  said -> crib-msg }T

testing Hewie watching the rocking chair
hewie-freed story-flag-set  hewie-commandable state-flag-set  game:hewie send join-fiona  room-afresh  [: false chairs-set ;] in-place  4e 0e -4e hewie-here
T{ [: [: watching @ ;] in-place ;] 120 soon -> -1 }T
200 run-frames  game:hewie send part-from-fiona

testing the box: the right thing in its slot - open: the Earrings and the Jade Choker; else nothing happens
jade-choker items:items-of before !
180 2e 0e -33e put  $B use-on-place  600 read-on
T{ got-jade-choker story-flag?  jade-choker items:items-of  inquisitor-earrings items:items-of 0> -> -1 before @ 1+ -1 }T
T{ $C use-on-place  [: window-shown nothing-msg = ;] 300 soon -> -1 }T
200 read-on
T{ 180 2e 0e -33e put  said -> box-open-msg }T

testing the vase broken: the Medallion; picked up
c2f-12-breakable-broken story-flag-clear  c2f-12-medallion story-flag-clear  room-afresh
' spots -17.5e 1e 47e struck
T{ c2f-12-breakable-broken story-flag?  [: medallion-fx effect @ 0<> ;] in-place -> -1 -1 }T
medallion items:items-of before !
-17.5e 0e 47e picked
T{ c2f-12-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T

testing the Refined Antimony: Hewie sniffing it out; picked up
c2f-12-refined-antimony-found story-flag-clear  c2f-12-refined-antimony story-flag-clear
hewie-freed story-flag-set  hewie-commandable state-flag-set  game:hewie send join-fiona  room-afresh  22.9e 0e -5e hewie-here
[: ['] antimony-sniffed start drop ;] in-place
T{ [: c2f-12-refined-antimony-found story-flag? ;] 600 soon  [: antimony-fx effect @ 0<> ;] in-place -> -1 -1 }T
300 read-on
refined-antimony items:items-of before !
-90 26.9e 0e -9.7e put  circle press  300 read-on   \ (east of it: the horse and the caged doll about)
T{ c2f-12-refined-antimony story-flag?  refined-antimony items:items-of -> -1 before @ 1+ }T

testing the wardrobe: in (hidden), out by the button
room-afresh  44 20e 0e 32e put  circle press  [: hidden? ;] 600 soon drop
T{ hidden? -> -1 }T
circle press  [: doing-busy? 0= ;] 600 soon drop
T{ hidden?  c2f-12-flag1 story-flag? -> 0 0 }T
testing ... in a panic: out, the doors left open
44 20e 0e 32e put  circle press  [: hidden? ;] 600 soon drop
[: out-in-panic ;] drop  [: ['] out-in-panic start drop ;] in-place  [: doing-busy? 0= ;] 600 soon drop
T{ hidden?  c2f-12-flag1 story-flag? -> 0 -1 }T

testing the stalker coming while she hides: his search counted
2 story.base:partner-load  5 run-frames  c2f-12-flag1 story-flag-clear  room-afresh
44 20e 0e 32e put  circle press  [: hidden? ;] 600 soon drop
search-var var@ before !
castle-2f-12 $3A game:debilitas send stalker-in  10 run-frames
T{ [: came-while-hidden @ ;] in-place  search-var var@ c2f-12-flag3 story-flag? or 0<> -> -1 -1 }T
game:debilitas send stalker-out  300 run-frames
test-summary
