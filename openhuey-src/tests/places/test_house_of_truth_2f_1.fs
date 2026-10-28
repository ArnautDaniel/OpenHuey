\ house-of-truth-2f-1's place: coming in - the crates whole, the flames lit; the locked door's remark
\ in turn with the sun's; the Sol Key turned in it (spent but while the other door is locked); the barred
\ corridor; the crates broken - the Viola Medallion, a creature let out; the Medallion Hewie sniffs out;
\ her first steps in taking her on; back by the forced way (her clothes, the doors shut, Lorenzo loose,
\ the file). Headless:
\   build/openhuey --test tests/places/test_house_of_truth_2f_1.fs
IN: test-house-of-truth-2f-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-2f-1 ;
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
: room-afresh ( -- )  house-of-truth-2f-1 room!  60 run-frames ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] her ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
ht2f-1-breakable1-broken story-flag-clear  ht2f-1-breakable2-broken story-flag-clear  ht2f-1-breakable3-broken story-flag-clear
ht2f-1-viola-medallion story-flag-clear  ht2f-1-medallion-found story-flag-clear  ht2f-1-medallion story-flag-clear
over-music-d story-flag-set  room-afresh
testing coming in: the crates whole, the flames lit
T{ [: crate-1 broken?  crate-2 broken?  0 effect @ 0<>  9 effect @ 0<> ;] in-place -> 0 0 -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing the locked door: its remark, then the sun carved in it, in turn
T{ [: ['] the-door fiona start-holding drop ;] in-place  shown-soon -> locked-msg }T
300 read-on  [: doing-busy? 0= ;] 300 soon drop
T{ [: ['] the-door fiona start-holding drop ;] in-place  shown-soon -> sun-msg }T
300 read-on  [: doing-busy? 0= ;] 300 soon drop
testing the Sol Key turned in it: the door unlocked, the key spent (the other door open)
sol-key [: 1 give-item ;] in-place  5 run-frames  sun-door [: lock ;] in-place  other-door [: unlock ;] in-place  5 run-frames
sol-key items:items-of before !
T{ 2 use-on-place  [: sun-door doors:door-locked? 0= ;] 1200 soon  5 run-frames  sol-key items:items-of -> -1 before @ 1- }T
300 read-on  [: doing-busy? 0= ;] 300 soon drop
testing ... with the other door locked: kept
sun-door [: lock ;] in-place  other-door [: lock ;] in-place  5 run-frames  sol-key items:items-of before !
T{ 2 use-on-place  [: sun-door doors:door-locked? 0= ;] 1200 soon  5 run-frames  sol-key items:items-of -> -1 before @ }T
300 read-on  [: doing-busy? 0= ;] 300 soon drop
testing the barred corridor
T{ corridor-area 45 at  said -> corridor-msg }T
testing the crates: the second - the Viola Medallion behind it, taken; the third - a creature let out
T{ -17e 71e -41e blow-at  [: crate-2 breaks? dup if  viola-shows  then ;] in-place -> -1 }T
[: body-point-off ;] her  viola-medallion items:items-of before !
T{ 0 -17e 71e -41e put  circle press  300 read-on  ht2f-1-viola-medallion story-flag?  viola-medallion items:items-of -> -1 before @ 1+ }T
T{ -3e 71e -41e blow-at  [: crate-3 breaks? ;] in-place  ht2f-1-breakable3-broken story-flag? -> -1 -1 }T
[: body-point-off ;] her
testing the Medallion: Hewie sniffs it out; taken
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  game:hewie send join-fiona  60 run-frames
[: ['] medallion-sniffed start drop ;] in-place
T{ [: ht2f-1-medallion-found story-flag? ;] 600 soon -> -1 }T
400 run-frames  medallion items:items-of before !
T{ 0 -50e 71e -15e put  circle press  300 read-on  ht2f-1-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T
testing back by the forced way: her clothes as they were, the doors shut on her, Lorenzo loose, the file
3 costume-var [: var! ;] in-place  lorenzo2-kind story.base:partner-load  5 run-frames
house-of-truth-2f-1 forced-exit rooms send go-to-room
T{ [: hall-file filed? ;] 900 soon  other-door doors:door-locked?  summoner-on state-flag?  new-game-sounds state-flag? -> -1 -1 -1 -1 }T
300 play-on  summoner-on state-flag-clear  game:lorenzo2 send stalker-out  10 run-frames
testing her first steps in (her track not on): on, by the forced way
over-music-d story-flag-clear  room-afresh  [: 0 prepare-exit ;] in-place
T{ 0 0 at  [: room-id house-of-truth-2f-1 <> world-held state-flag? or ;] 600 soon -> -1 }T
600 play-on
test-summary
