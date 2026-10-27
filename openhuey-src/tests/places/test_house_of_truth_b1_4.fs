\ house-of-truth-b1-4's place (and what the colour rooms share, story/places/house-of-truth.fs):
\ coming in - the colours undone, their doors' opened marks cleared; the inscription; crossing toward
\ a door - the colour room ahead loaded - and out through it into the dark (in there by the forced
\ way, the dark lifting); in by the forced way here - the first time the fact, then the file; the
\ voices in turn, by the frames spent here; Lorenzo waiting once they are over. Headless:
\   build/openhuey --test tests/places/test_house_of_truth_b1_4.fs
IN: test-house-of-truth-b1-4
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-b1-4 ;
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
: room-afresh ( -- )  house-of-truth-b1-4 room!  60 run-frames ;
: opened? ( door -- flag )  doors:opened-bit doors:has? ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
shared-flag1 story-flag-clear  shared-flag2 story-flag-clear  shared-flag3 story-flag-clear  shared-flag4 story-flag-clear
htb1-5-flag1 story-flag-set  htb1-7-flag1 story-flag-set  $F1 true [: open-mark ;] in-place  5 run-frames  found-file-254 story-flag-clear
room-afresh
testing coming in: the colours undone, their doors' opened marks cleared
T{ htb1-5-flag1 story-flag?  htb1-7-flag1 story-flag?  $F1 opened? -> 0 0 0 }T
testing the inscription by the wall
T{ inscription-area -45 at  said -> inscription-msg }T
testing crossing toward the first door: the red room loaded ahead; through the door - in there, the dark lifting
0 0e 1e 5e put
T{ 0 0e 1e 25e put  0 -139e 0e 135e put  [: room-id house-of-truth-b1-5 = ;] 600 soon -> -1 }T
T{ [: came-by @ ;] in-place  [: doing-busy? 0= ;] 300 soon  htb1-5-flag1 story-flag? -> forced-exit -1 -1 }T
testing in here by the forced way: the first time its fact; the next its file
house-of-truth-b1-4 forced-exit rooms send go-to-room  [: doing-busy? 0= room-id house-of-truth-b1-4 = and ;] 600 soon drop  60 run-frames
T{ found-file-254 story-flag?  white-file filed? -> -1 0 }T
house-of-truth-b1-4 forced-exit rooms send go-to-room  [: doing-busy? 0= room-id house-of-truth-b1-4 = and ;] 600 soon drop  60 run-frames
T{ white-file filed? -> -1 }T
testing the voices: after 1800 frames here the first - its line; the second at 4500; the third at 7200; then the last fact
room-afresh
T{ [: 1799 frames-here ! ;] in-place  shown-soon  shared-flag1 story-flag? -> 5 -1 }T
300 read-on
T{ [: 4499 frames-here ! ;] in-place  shown-soon  shared-flag2 story-flag? -> 6 -1 }T
300 read-on
T{ [: 7199 frames-here ! ;] in-place  shown-soon  shared-flag3 story-flag? -> 7 -1 }T
300 read-on
T{ [: 7499 frames-here ! ;] in-place  5 run-frames  shared-flag4 story-flag? -> -1 }T
testing then Lorenzo waiting here as she comes in
lorenzo2-kind story.words:partner-load  5 run-frames  room-afresh
T{ [: [: stalker-in? ;] in-place ;] 120 soon -> -1 }T
game:lorenzo2 send stalker-out  10 run-frames
test-summary
