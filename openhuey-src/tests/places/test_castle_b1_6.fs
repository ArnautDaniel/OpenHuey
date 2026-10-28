\ castle-b1-6's place (scripts/story/places/castle-b1-6.fs): the chapel - the statue's inscription and
\ the Saturnus Key's scene; stepping in with the key, Debilitas comes; the stands weakened by his blows
\ (and hers), broken; both broken, the chandelier falls on him; the remarks; the movies replayed. Headless:
\   build/openhuey --test tests/places/test_castle_b1_6.fs
IN: test-castle-b1-6
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.castle-b1-6 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] her  10 run-frames ;
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
: room-afresh ( -- )  castle-b1-6 room!  60 run-frames ;
: his-blow! ( n -- )   \ (as he tells: in here, his blow under way)
    castle-b1-6 0 0 0 100 100 debilitas-kind 0 -1 broadcast stalker-doing  broadcast stalker-blow  1 run-frames ;
: reset-chapel ( -- )
    got-saturnus-key story-flag-clear  cb1-6-flag1 story-flag-clear  cb1-6-flag2 story-flag-clear
    cb1-6-flag4 story-flag-clear  cb1-6-flag5 story-flag-clear  found-file-20b story-flag-clear ;
: middle-put ( area deg -- )  >r area-middle drop r> put ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear

testing the pulley in turn, before he's come
reset-chapel  room-afresh
T{ pulley-1-area 180 at  said  pulley-1-area 180 at  said -> pulley-msg fastened-msg }T
full-suite   \ (on: waits, scenes, walks - the full run's)

testing the stands weakened only while he's after her: his blow of 7 takes 25 to 18, then 11; none, nothing
room-afresh  cb1-6-flag1 story-flag-set
T{ 7 his-blow!  [: strength-1 true weaker  strength-1 @  strength-1 true weaker  strength-1 @ ;] in-place -> 18 11 }T
T{ -1 his-blow!  [: strength-1 true weaker  strength-1 @ ;] in-place -> 11 }T
T{ [: strength-2 false weaker  strength-2 @ ;] in-place -> 20 }T   \ (her blow: 5)
cb1-6-flag1 story-flag-clear  room-afresh
T{ 7 his-blow!  [: strength-1 true weaker  strength-1 @ ;] in-place -> 25 }T
testing ... broken at 0: the stand gone, its fact
[: 0 strength-1 !  false stand-1-struck ;] in-place  30 run-frames
T{ cb1-6-flag4 story-flag?  stand-1-part part? -> -1 -1 }T

testing the statue without the key: the inscription, the cane - the Saturnus Key's scene, the key hers
reset-chapel  room-afresh
saturnus-key items:items-of before !
statue-area 90 at  circle press  2400 play-on
T{ got-saturnus-key story-flag?  saturnus-key items:items-of  cane-part part? -> -1 before @ 1+ 0 }T
testing ... with it (the fight over): its letters again, the deity after
cb1-6-flag2 story-flag-set  room-afresh
T{ statue-area 90 at  said -> letters-msg }T

testing stepping in with the key: Debilitas comes - the door locked, the fight on
reset-chapel  got-saturnus-key story-flag-set  room-afresh
-90 120e 0e 0e put  -90 96e 0e 0e put  1500 play-on   \ (in across its line, from the door)
T{ cb1-6-flag1 story-flag?  chapel-door doors:door-locked?  scene-5-pending state-flag? -> -1 -1 -1 }T
testing ... both stands broken: the chandelier falls on him - the door open, his file
cb1-6-flag4 story-flag-set  cb1-6-flag5 story-flag-set  3000 play-on
T{ found-file-20b story-flag?  cb1-6-flag2 story-flag?  chapel-door doors:door-locked?  $20B filed? -> -1 -1 0 -1 }T
testing ... the chandelier after: her remark, then how near she came; the pulley broken
T{ chandelier-area 0 at  said  chandelier-area 0 at  said -> chandelier-msg killed-msg }T
T{ pulley-1-area 180 at  said -> broken-msg }T

testing the movies replayed (the extras): played, back to the extras room
: replays ( v -- room held )  off-map-37 room!  30 run-frames  $2A var!  events-held state-flag-set  room-afresh  1500 play-on  room-id  events-held state-flag? ;   \ (from the extras room)
T{ 4 replays -> off-map-37 0 }T   \ (4: the extras list's act $C, the statue's)
test-summary
