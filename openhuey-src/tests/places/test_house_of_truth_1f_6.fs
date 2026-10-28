\ house-of-truth-1f-6's place: shut in (the door slammed, his voice, the time started); his taunts by
\ stage - the locked door tried (2, 3, then 4), a spot examined, hidden in the cabinet and out again,
\ the shelf falling, time passing, standing still; late, the far door open, stepping away - "Come to me"
\ (the door opened), then on by the forced way; with her track on, his game over; the replay. Headless:
\   build/openhuey --test tests/places/test_house_of_truth_1f_6.fs
IN: test-house-of-truth-1f-6
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-1f-6 ;
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
: room-afresh ( -- )  house-of-truth-1f-6 room!  60 run-frames ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: taunt-shown ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
over-music-e story-flag-clear  room-afresh
testing shut in: the door slammed, his voice; the time started
T{ shown-soon -> naughty-msg }T
300 read-on  [: doing-busy? 0= ;] 300 soon drop
T{ slammed-door doors:door-locked?  [: time-here @ 0< 0= ;] in-place -> -1 -1 }T
testing his taunt as time passes (450 frames)
T{ [: 449 time-here ! ;] in-place  taunt-shown  [: stage @ ;] in-place -> 9 9 }T
300 read-on
T{ [: stage @ ;] in-place -> -1 }T
testing the locked door tried: his taunts 2, 3, then 4
0 by-door
: tried ( -- msg )  [: ['] tries-door fiona start-holding drop ;] in-place  taunt-shown  300 read-on  [: doing-busy? 0= ;] 300 soon drop ;
T{ tried  tried  tried  tried -> 2 3 4 4 }T
testing a spot she examines: his taunt 7
T{ spot-area 0 at  circle press  taunt-shown -> 7 }T
300 read-on
testing standing still 300 frames: his last taunt
T{ [: 0 waited-out !  no-stage stage !  299 still ! ;] in-place  taunt-shown -> 18 }T   \ (its once again - idle frames before may have had it)
300 read-on
testing the cabinet: hidden - his taunt 5; out at circle
T{ hiding-area -45 at  circle press  [: hidden? ;] 900 soon -> -1 }T
T{ taunt-shown -> 5 }T
300 read-on
circle press  [: hidden? 0= ;] 300 soon drop  [: doing-busy? 0= ;] 600 soon drop
T{ hidden? -> 0 }T
testing the shelf falling as she comes to it (the far door open): his taunt 8
T{ [: -1 far-open !  ['] shelf-scene fiona start-holding drop ;] in-place  taunt-shown -> 8 }T
300 read-on  [: doing-busy? 0= ;] 600 soon drop
T{ [: shelf-done @  shelf-down @ ;] in-place -> -1 -1 }T
testing late, the far door open, stepping away: "Come to me" - the door opened
room-afresh  300 read-on  [: doing-busy? 0= ;] 300 soon drop
[: -1 late !  -1 far-open ! ;] in-place  0 0e 0e -37e put
T{ 0 0e 0e -17e put  [: [: ['] come-to-me running? ;] in-place ;] 30 soon -> -1 }T
T{ [: movie-skipped state-flag-set  come-seen resident-flag? ;] 2000 soon  slammed-door doors:door-locked? -> -1 0 }T
300 play-on
testing after it, her step to the door: on by the forced way
[: house-of-truth-b1-2 preload ;] in-place
T{ 0 0 at  [: room-id house-of-truth-1f-6 <> world-held state-flag? or ;] 600 soon -> -1 }T
600 play-on
testing her track on: his game over - no taunts, the door hers
over-music-e story-flag-set  room-afresh
T{ [: ['] taunts running? ;] in-place -> 0 }T
testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  house-of-truth-1f-6 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon -> -1 }T
test-summary
