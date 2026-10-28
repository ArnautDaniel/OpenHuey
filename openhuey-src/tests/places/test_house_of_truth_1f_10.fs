\ house-of-truth-1f-10's place: its steady sound; hiding under the hung things (in at her examine, out
\ at circle); "no time" with the stalker here; his search - found, hauled out; the hung things swinging
\ as she pushes by; the hole - in, by the forced way, and out of it here; the clock's save asked; the
\ Camomile; Hewie's bark at the hole. Headless:
\   build/openhuey --test tests/places/test_house_of_truth_1f_10.fs
IN: test-house-of-truth-1f-10
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-1f-10 ;
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
: room-afresh ( -- )  house-of-truth-1f-10 room!  60 run-frames ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  10 run-frames ;
: picked ( -- n )  game:window enter  window:picked @  leave-actor ;
: choose-answer ( n -- )  >r  [: key: Left tap  picked 0= dup 0= if  cross press  20 run-frames  then ;] r> if  drop [: key: Right tap  picked 1 = dup 0= if  cross press  20 run-frames  then ;]  then  30 soon drop  cross press ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
ht1f-10-camomile story-flag-clear  room-afresh
testing its steady sound started
T{ [: steady-on @ ;] in-place -> -1 }T
testing hiding at her examine; out at circle
T{ hiding-area 0 at  circle press  [: hidden? ;] 300 soon -> -1 }T
60 run-frames  circle press  [: hidden? 0= ;] 300 soon drop  [: doing-busy? 0= ;] 300 soon drop
T{ hidden?  free? -> 0 -1 }T
testing the hung things swinging as she pushes by
room-afresh  0 30.47e 0e 20e put   \ (out of their reach, then one step among them)
T{ 0 30.47e 0e -9.97e put  [: rung @ ;] in-place  [: 0 hanging swing-left object@ f0> ;] in-place -> -1 -1 }T
testing "no time" there with the stalker here
lorenzo2-kind story.words:partner-load  5 run-frames  house-of-truth-1f-10 $11F game:lorenzo2 send stalker-in  30 run-frames
hiding-area 0 at
T{ [: 0 offer-msg !  0 offer-xt !  stalker-in? if  spots  offer-msg @  else  -2  then ;] in-place -> no-time-message }T
testing his search as he comes in on her hidden: found - hauled out
game:lorenzo2 send stalker-out  10 run-frames  room-afresh
hiding-area 0 at  circle press  [: hidden? ;] 300 soon drop  60 run-frames
house-of-truth-1f-10 $11F game:lorenzo2 send stalker-in  30 run-frames
: try-find ( -- flag )  [: found-her @ ;] in-place  dup 0= if  [: 3 searches-var var!  ['] found-there running? 0= if  searched  then ;] in-place  then ;
T{ [: try-find ;] 3000 soon  [: hidden? 0= ;] 600 soon -> -1 -1 }T
[: doing-busy? 0= ;] 900 soon drop  game:lorenzo2 send stalker-out  10 run-frames  [: 0 panic-to ;] in-place  60 run-frames
testing the Camomile
room-afresh  camomile items:items-of before !
T{ 0 29.7e 5.25e 32.54e put  circle press  300 read-on  ht1f-10-camomile story-flag?  camomile items:items-of -> -1 before @ 1+ }T
testing the clock: the save asked (no)
T{ clock-area 90 at  circle press  shown-soon -> clock-msg }T
1 choose-answer  300 read-on
testing Hewie's bark at the hole (once a visit)
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  game:hewie send join-fiona  60 run-frames
house-of-truth-1f-10 60.49e 0e 45.99e nav-tri 0 game:hewie send hewie-place  30 run-frames
T{ [: [: hole-barked @ ;] in-place ;] 120 soon -> -1 }T
300 run-frames
testing the hole: in (yes) - by the forced way; out of it here
room-afresh  [: off-map-27 preload ;] in-place
T{ hole-area 0 at  circle press  [: [: shown-msg @ hole-msg = ;] in-place ;] 600 soon -> -1 }T   \ (after her look)
[: picked if  key: Left tap  then  cross press  20 run-frames  room-id house-of-truth-1f-10 <>  dup 0= if  drop [: shown-msg @ hole-msg <> ;] in-place  then ;] 30 soon drop   \ (yes - on its last page, where it starts)
T{ [: room-id off-map-27 = ;] 900 soon -> -1 }T
600 play-on
house-of-truth-1f-10 hole-way rooms send go-to-room  [: room-id house-of-truth-1f-10 = ;] 300 soon drop  [: doing-busy? 0= ;] 600 soon drop
T{ [: fiona body-tri ;] in-place  free? -> $7E -1 }T
test-summary
