\ house-of-truth-1f-2's place: its steady sound, its holes in the floor; the water - a recovery point,
\ then its second remark; the clock asked (no); the crate broken by a blow, the Refined Antimony in it;
\ the Refined Magnesia; the Prima Magnesia Hewie sniffs out; not going back; Hewie's bark at the hole;
\ the hole - in, and out of it here; in from the B1 by the forced way (the summoner on). Headless:
\   build/openhuey --test tests/places/test_house_of_truth_1f_2.fs
IN: test-house-of-truth-1f-2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-1f-2 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )   \ (on the floor, on a triangle she walks - else the nearest the mesh has)
    on-floor s>f tp-facing  [: her-at $28020018 v-tri-in  dup 0< if  drop her-at vec@ nav-tri  then  her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: shown-soon ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
: said ( -- msg )
    circle press  shown-soon
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
create spot 12 allot
: by-door ( exit -- )  0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames ;
variable before
: room-afresh ( -- )  house-of-truth-1f-2 room!  60 run-frames ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  10 run-frames ;
: picked ( -- n )  game:window enter  window:picked @  leave-actor ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] her ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
ht1f-2-breakable-broken story-flag-clear  ht1f-2-refined-antimony story-flag-clear  ht1f-2-refined-magnesia story-flag-clear
ht1f-2-prima-magnesia-found story-flag-clear  ht1f-2-prima-magnesia story-flag-clear  got-doc-recovery story-flag-clear
room-afresh
testing coming in: its steady sound, its holes in the floor
T{ [: steady-on @ ;] in-place  $51 nav-flags floor-flags:hole-floor and 0<> -> -1 -1 }T
testing the water: a recovery point (its document the first time); then "Ah, that tasted good."
T{ water-area 0 at  circle press  [: window-shown 0< 0= if  cross press  then  got-doc-recovery story-flag? ;] 900 soon -> -1 }T
300 read-on  [: doing-busy? 0= ;] 600 soon drop
T{ water-area 0 at  said -> tasted-msg }T
testing the clock: the save asked (no)
T{ clock-area 0 at  circle press  shown-soon -> clock-msg }T
[: key: Right tap  picked 1 = dup 0= if  cross press  20 run-frames  then ;] 30 soon drop  cross press  300 read-on
testing the crate broken by a blow: the Refined Antimony in it, taken
T{ -0.4e 71e -123.2e blow-at  [: the-crate breaks? dup if  antimony-shows  then ;] in-place -> -1 }T
[: body-point-off ;] her  refined-antimony items:items-of before !
T{ 0 -0.4e 71e -123.2e put  circle press  300 read-on  ht1f-2-refined-antimony story-flag?  refined-antimony items:items-of -> -1 before @ 1+ }T
testing the Refined Magnesia
refined-magnesia items:items-of before !
T{ 0 -33.17e 71e 33.19e put  circle press  300 read-on  ht1f-2-refined-magnesia story-flag?  refined-magnesia items:items-of -> -1 before @ 1+ }T
testing not going back: as she steps back into the way she came
room-afresh  0 -5e 70e 50e put
T{ 0 -5e 70e 25e put  shown-soon -> go-back-msg }T
300 read-on
testing the Prima Magnesia: Hewie sniffs it out; taken
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  game:hewie send join-fiona  60 run-frames
[: ['] prima-sniffed start drop ;] in-place
T{ [: ht1f-2-prima-magnesia-found story-flag? ;] 600 soon -> -1 }T
400 run-frames  prima-magnesia items:items-of before !
T{ 0 35e 71e 33e put  circle press  300 read-on  ht1f-2-prima-magnesia story-flag?  prima-magnesia items:items-of -> -1 before @ 1+ }T
testing Hewie's bark at the hole (once a visit)
house-of-truth-1f-2 -61.4e 0e 46.49e nav-tri 0 game:hewie send hewie-place   \ (the lower floor's)  30 run-frames
T{ [: [: hole-barked @ ;] in-place ;] 120 soon -> -1 }T
300 run-frames
testing the hole: in (yes) - by the forced way; out of it here
room-afresh  [: off-map-27 preload ;] in-place
T{ hole-area 0 at  circle press  [: [: shown-msg @ hole-msg = ;] in-place ;] 600 soon -> -1 }T
[: picked if  key: Left tap  then  cross press  20 run-frames  room-id house-of-truth-1f-2 <>  dup 0= if  drop [: shown-msg @ hole-msg <> ;] in-place  then ;] 30 soon drop
T{ [: room-id off-map-27 = ;] 900 soon -> -1 }T
600 play-on
house-of-truth-1f-2 hole-way rooms send go-to-room  [: room-id house-of-truth-1f-2 = ;] 300 soon drop  [: doing-busy? 0= ;] 600 soon drop
T{ [: fiona body-tri ;] in-place  free? -> $28 -1 }T
testing in from the B1 by the forced way: at its spot, the summoner on
house-of-truth-1f-2 forced-exit rooms send go-to-room
T{ [: summoner-on state-flag? ;] 600 soon  [: fiona body-tri ;] in-place -> -1 $3F }T
summoner-on state-flag-clear
test-summary
