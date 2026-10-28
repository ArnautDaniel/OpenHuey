\ Hewie out by an exit on screen (hewie/exits.fs): off by the nearest open way out he can get to
\ (0x25); drawing the stalker off while she hides (0x7B) - ahead of it on its way to an exit,
\ on guard, barking. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/hewie/test_exits.fs
IN: test-exits
USING: tester engine game-state actors messages common facts room-names game debug flag-names hewie.state hewie.body hewie.model hewie.states hewie.offscreen hewie.exits fiona.state stalker.state stalker.senses stalker.moving stalker.search stalker.chase stalker.bitten ;

: his@ ( xt -- x )  game:hewie enter  execute  leave-actor ;
: his! ( xt -- )  game:hewie enter  execute  leave-actor ;
: deb! ( xt -- )  game:debilitas enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: act? ( xt -- flag )  [: his-act @ ;] his@ = ;
: calm ( -- )  [: 1000 add-trust  obeys  0 -1 set-mood  100 hp!  0 cond!  10000 his-wait ! ;] his! ;
1 seed !

free-play  castle-1f-2 room!  20 run-frames   \ (a room he can leave freely)
0 1 exit-tri tri-center 0e tp-facing  5 run-frames  game:hewie send join-fiona  20 run-frames  calm

testing 0x25: for the nearest open exit he can get to, and out by it - gone from her room
T{ [: self 0 flee-exit ;] his@ 0< 0= -> -1 }T
[: $25 0 want ;] his!
T{ ' st-go-to-exit act? -> -1 }T
T{ [: game:hewie body-room room-id <> ;] 900 soon -> -1 }T
game:hewie send join-fiona  20 run-frames  calm

testing 0x7B: she hides, the stalker here - he makes for the stalker's way to an exit
stalkers-blind state-flag-set  game:hewie body-tri debug:debilitas-tri  3 run-frames  [: 2 mode!  0 temper ! ;] deb!
fiona-hidden state-flag-set
[: $7B 0 want ;] his!
T{ [: his-action @ ;] his@ -> $7B }T
T{ [: ['] st-run-spot act?  ['] st-guard act? or  ['] st-pursuer-near act? or ;] 60 soon -> -1 }T
testing ... she out of hiding: himself again
fiona-hidden state-flag-clear
T{ [: ['] st-run-spot act?  ['] st-guard act? or  ['] st-pursuer-near act? or  ['] st-plan-pursuer act? or  0= ;] 30 soon -> -1 }T

testing in a door's gap, on its exit's own area, the door shut (Hewie_FirstDoor): out through it ($66)
game:hewie send join-fiona  20 run-frames  calm
[: room-id 660 dup tri-center body-place  0 0 want ;] his!
T{ [: [: his-room room-id <> ;] his@ ;] 30 soon -> -1 }T
game:hewie send join-fiona  20 run-frames  calm
testing in a door's gap, not on its exit's area (Hewie_FirstDoor, StateSqueeze): he squeezes out ($65) - off the gap, then himself again
[: room-id 680 dup tri-center body-place  0 0 want ;] his!
T{ [: [: his-action @ $65 = ;] his@ ;] 10 soon -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
T{ [: [: on-gap? 0=  his-action @ $65 <> and ;] his@ ;] 120 soon -> -1 }T
test-summary
