\ house-of-truth-1f-5's place: before the file - the door welded shut, the doors locked from this side
\ tried; her track on, the file not found: the stalkers held till she steps further in - then the scene
\ (Lorenzo's end: its file, the door shut behind her, the stalkers let go); after it "I have to get out
\ of here now!", his body, the door of mystical power (open: by the forced way); the tremors; the
\ replay. Headless:
\   build/openhuey --test tests/places/test_house_of_truth_1f_5.fs
IN: test-house-of-truth-1f-5
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-1f-5 ;
full-suite   \ (on: waits, scenes, walks - the full run's)
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
: room-afresh ( -- )  house-of-truth-1f-5 room!  60 run-frames ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  10 run-frames ;
: picked ( -- n )  game:window enter  window:picked @  leave-actor ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
found-file-257 story-flag-clear  over-music-f story-flag-clear  ht1f-9-flag2 story-flag-clear  room-afresh
testing before the file: the door welded shut; the doors locked from this side, tried
T{ door-area 0 at  said -> welded-msg }T
0 by-door
T{ [: 0 offer-xt !  0 offer-msg !  2 request-now !  spots  offer-xt @ ;] in-place  ' tries-front = -> -1 }T
testing her track on, the file not found: the stalkers held as she comes in
over-music-f story-flag-set  room-afresh
T{ stalkers-stay state-flag?  [: held-back @ ;] in-place -> -1 -1 }T
testing ... further in: the scene - Lorenzo's end; its file, the door shut behind her, the stalkers let go
0 45e 0e -20e put
T{ 0 28e 0e -20e put  [: [: ['] lorenzo-ends running? ;] in-place ;] 30 soon -> -1 }T
T{ [: movie-skipped state-flag-set  found-file-257 story-flag? ;] 2000 soon  end-seen resident-flag?  lorenzo-file filed? -> -1 -1 -1 }T
T{ hall-door doors:door-locked?  stalkers-stay state-flag? -> -1 0 }T
300 play-on
testing after it: "I have to get out of here now!", his body
room-afresh
T{ clock-area 134 at  said -> get-out-msg }T
T{ 0 0e 0e 74.25e put  said -> dead-msg }T
testing the tremors - the house coming down (the stalker dealt with: slight)
ht1f-9-flag2 story-flag-set  room-afresh
T{ [: ['] slight-tremors running? ;] in-place -> -1 }T
ht1f-9-flag2 story-flag-clear
testing the door of mystical power: open (yes) - by the forced way, its sounds stopped
room-afresh
T{ door-area 0 at  circle press  [: [: shown-msg @ mystic-msg = ;] in-place ;] 600 soon -> -1 }T
[: picked if  key: Left tap  then  cross press  20 run-frames  room-id house-of-truth-1f-5 <>  dup 0= if  drop [: shown-msg @ mystic-msg <> ;] in-place  then ;] 30 soon drop
T{ [: room-id off-map-35 = ;] 900 soon -> -1 }T
600 play-on  over-music-f story-flag-clear
testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  house-of-truth-1f-5 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon -> -1 }T
test-summary
