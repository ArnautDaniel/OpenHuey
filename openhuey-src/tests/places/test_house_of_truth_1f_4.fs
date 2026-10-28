\ house-of-truth-1f-4's place: coming in - the rubble, the generator's jolts to come; the dynamo's
\ shake and the belt's step; the generator's and the demolisher's remarks in turn; the generator jolted:
\ the first time the scene (the door locked behind - "No! It's locked!" - its file), the belt running;
\ the locked door's remark in turn with its symbol (its file); Lorenzo under the belt - crushed (the
\ Sol Key there, the summoner off); the Sol Key taken, turned in the door; Hewie moved off its way; the
\ replays; she under the belt - caught (the game over). Headless:
\   build/openhuey --test tests/places/test_house_of_truth_1f_4.fs
IN: test-house-of-truth-1f-4
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-1f-4 ;
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
: room-afresh ( -- )  house-of-truth-1f-4 room!  60 run-frames ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
: replays ( n -- flag )
    replay-var var!  events-held state-flag-set  house-of-truth-1f-4 -1 rooms send go-to-room  5 run-frames
    [: cutscene-active? movie-status 1 = or ;] 600 soon  [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon and ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
found-file-250 story-flag-clear  ht1f-4-flag1 story-flag-clear  got-sol-key story-flag-clear  room-afresh
testing coming in: the rubble there, three jolts to come, followed
T{ [: jolts @ ;] in-place  force-followed state-flag? -> 3 -1 }T
testing the dynamo's shake: its size down a quarter a frame; the belt's step: 0.4 along, the wheel 0.05 round
T{ [: dynamo placed-here?  belt-there? ;] in-place -> -1 -1 }T
T{ [: belt-set  belt-step  belt place-x object@ 0.4e ~=  roller turn-y object@ 0.05e ~= ;] in-place -> -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing the generator's remarks in turn; the demolisher's
T{ generator-area 180 at  said  generator-area 180 at  said -> dead-gen-msg jolt-msg }T
T{ demolisher-area 0 at  [: fiona demolisher-area 50 faces-area? ;] in-place -> -1 }T
T{ said  demolisher-area 0 at  said -> demolisher-msg not-running-msg }T
testing a full blow at it (one jolt left): the first time the scene - its file, the door locked behind, the belt running
lorenzo2-kind story.base:partner-load  5 run-frames
T{ [: 1 jolts !  ['] jolted start drop ;] in-place  [: movie-skipped state-flag-set  found-file-250 story-flag? ;] 2000 soon -> -1 }T
T{ [: [: belt-running @ ;] in-place ;] 600 soon  study-door doors:door-locked?  scene-5-pending state-flag?  demolisher-file filed? -> -1 -1 -1 -1 }T
300 play-on
testing "No! It's locked!" at the door then
T{ [: 0 offer-xt !  0 offer-msg !  2 request-now !  spots  offer-xt @ ;] in-place  ' the-door = -> -1 }T
testing Lorenzo under it: crushed - its fact, the door unlocked, the Sol Key there, the summoner off
summoner-on state-flag-set
T{ [: ['] crushed fiona start-holding drop ;] in-place  [: movie-skipped state-flag-set  ht1f-4-flag1 story-flag? ;] 2000 soon -> -1 }T
T{ [: doing-busy? 0= ;] 600 soon  study-door doors:door-locked?  summoner-on state-flag?  [: sol-fx effect @ 0<> ;] in-place -> -1 0 0 -1 }T
300 play-on  game:lorenzo2 send stalker-out  10 run-frames  scene-5-pending state-flag-clear
testing the locked door: its remark in turn with the symbol carved in it - its file
room-afresh  0 by-door
T{ [: ['] the-door fiona start-holding drop ;] in-place  shown-soon -> locked-msg }T
300 read-on  [: doing-busy? 0= ;] 300 soon drop
T{ symbol-file filed? -> -1 }T
T{ [: ['] the-door fiona start-holding drop ;] in-place  shown-soon -> symbol-msg }T
300 read-on  [: doing-busy? 0= ;] 300 soon drop
testing the Sol Key taken
sol-key items:items-of before !
T{ 0 0e -3.2e 65e put  circle press  300 read-on  got-sol-key story-flag?  sol-key items:items-of -> -1 before @ 1+ }T
testing it turned in the door: the other door unlocked, spent (the sun door open)
other-door [: lock ;] in-place  sun-door [: unlock ;] in-place  5 run-frames  sol-key items:items-of before !
T{ 8 use-on-place  [: other-door doors:door-locked? 0= ;] 1200 soon  5 run-frames  sol-key items:items-of -> -1 before @ 1- }T
300 read-on  [: doing-busy? 0= ;] 300 soon drop
testing Hewie moved off the belt's way as she comes back
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  game:hewie send join-fiona  60 run-frames
T{ [: -1 hewie-way-open !  hewie-moved  hewie-way-open @ ;] in-place  5 run-frames  [: hewie body-tri ;] in-place  dup $146 =  over $130 = or  swap $11C = or -> 0 -1 }T
testing the movies replayed (the extras)
T{ 23 replays -> -1 }T
testing she under the belt: caught - the game over
found-file-250 story-flag-set  ht1f-4-flag1 story-flag-clear  room-afresh
T{ [: ['] she-caught fiona start-holding drop ;] in-place  [: movie-skipped state-flag-set  $47 resident-flag? ;] 2000 soon  caught state-flag? -> -1 -1 }T
test-summary
