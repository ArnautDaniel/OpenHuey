\ The story's tackle (Hewie's 0x75: Hewie_StateTargetPursuer, StateLeapLaunch / LeapTakeOff /
\ LeapAtPursuer; the stalker's side Pursuer_GrabOrder's slot command 2 / 7) and what the stalker
\ tells of what holds him (stalker-ties: +0xE0, +0x2A, Progress_IsLinked) - Hewie's scuffle off
\ screen, Fiona kept apart. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/hewie/test_tackle.fs
IN: test-tackle
USING: tester engine game-state actors messages common facts vectors room-names game debug flag-names hewie-names hewie.state hewie.body hewie.model hewie.states hewie.fight fiona.state stalker.state stalker.senses stalker.moving stalker.search stalker.travel stalker.chase stalker.grab stalker.bitten stalker ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: his@ ( xt -- x )  game:hewie enter  execute  leave-actor ;
: his! ( xt -- )  game:hewie enter  execute  leave-actor ;
: deb@ ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: deb! ( xt -- )  game:debilitas enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: blind ( on -- )  if  stalkers-blind state-flag-set  else  stalkers-blind state-flag-clear  then ;
1 seed !

free-play  front-garden-2 room!  20 frames
140.64e 124e -217.33e 0e tp-facing  5 frames  game:hewie send join-fiona  20 frames
: deb-in ( -- )
    true blind  s" fiona" actor-named body-tri debug:debilitas-tri  3 frames
    [: 3 mode!  0 temper !  70 hp ! ;] deb!  3 frames ;
: tackle! ( -- )  [: 1000 add-trust  obeys  0 -1 set-mood  100 hp!  0 cond!  tackle-act 0 set-action ;] his! ;

testing the tackle (0x75): asked of him - taken, his behaviour cut short, the spot by his leg; the leap ($1E01)
deb-in  tackle!
T{ [: [: his-act @ ['] st-tackle-take-off = ;] his@ ;] 10 soon  [: anim@ ;] his@ -> -1 $1E01 }T
T{ [: his-mode @  his-2d @ ;] his@ -> engaged-mode 1 }T
testing ... the leg's bite faded in ($2218) over the frames left; reaching him he is bitten at the leg (15)
T{ [: [: his-act @ ['] st-tackle-lands = ;] his@ ;] 60 soon  [: anim@ ;] his@ -> -1 $2218 }T
T{ [: [: bitten @ ;] deb@ leg-attack = ;] 60 soon -> -1 }T
testing ... landed: the bite of 30, holding on with one bite left - then he lets go ($2219) and the stalker is up again
T{ [: [: his-act @ ['] st-bite-hold = ;] his@ ;] 60 soon  [: hp @ ;] deb@  [: his-bites @ ;] his@ -> -1 40 1 }T
T{ [: [: anim@ $2219 = ;] his@ ;] 300 soon -> -1 }T
T{ [: [: bitten @ 0= ;] deb@ ;] 600 soon -> -1 }T
testing ... his hold by his mood (never rolled against): angry 110, upset none, else (his feeling + 10) x 5
T{ [: 3 his-mood !  tackle-hold  2 his-mood !  tackle-hold  0 his-mood !  0 feel !  tackle-hold ;] his@ -> 110 0 50 }T
debug:debilitas-out  5 frames

testing refused - the stalker not in the room: his default (0x00)
deb-in  [: tackle-act his-action !  ['] st-await-tackle his-act ! ;] his!
front-garden-3 room!  1 frames  game:debilitas [: send dog-tackle ;] his!  3 frames
T{ [: his-act @ ['] st-await-tackle = ;] his@ -> 0 }T
debug:debilitas-out  front-garden-2 room!  20 frames
140.64e 124e -217.33e 0e tp-facing  5 frames  game:hewie send join-fiona  20 frames

testing what holds him (stalker-ties): a script of the room has him (`scripted`); out of contact (+0x2A); a joint action pending
deb-in
: told ( -- scripted apart linked )  [: script-held @ in-scene @ or 0<>  apart @ away? or 0<>  linked? ;] deb@ ;
T{ told -> 0 0 0 }T
-1 game:debilitas send scripted  2 frames
T{ told  [: st-scripted @ ;] his@ -> -1 0 0 -1 }T
0 game:debilitas send scripted  2 frames
T{ told  [: st-scripted @ ;] his@ -> 0 0 0 0 }T
T{ [: -1 apart ! ;] deb!  told  [: 0 apart ! ;] deb! -> 0 -1 0 }T
T{ [: 1 leading !  no-answer answer ! ;] deb!  told  [: 0 leading ! ;] deb! -> 0 0 -1 }T
testing ... his tackle taken: linked the frame he takes it
[: tackle-act his-action !  ['] st-await-tackle his-act ! ;] his!
game:debilitas [: send dog-tackle ;] his!  deliver
T{ told -> 0 0 -1 }T
2 frames
T{ told -> 0 0 0 }T
debug:debilitas-out  5 frames

testing Fiona kept apart (Fiona_KeepApart): not pushed out of him while he is out of contact (his +0x2A)
deb-in  false blind
create her-was /vec allot
: on-her ( -- )   \ (him 2 off her: overlapping)
    her-was s" fiona" actor-named body-at vec-copy  [: room-id self body-tri  her-was vec@ {: F: x y z :}  x 2e f+ y z body-place ;] deb! ;
: her-moved ( -- f )  s" fiona" actor-named body-at her-was vec-dist ;
: held-apart ( n -- )  0 ?do  [: -1 apart ! ;] deb!  game-tick  loop ;
2 held-apart   \ (as he tells it: by her next frame)
T{ on-her  3 held-apart  her-moved 0.01e f< -> -1 }T
T{ on-her  [: 0 apart ! ;] deb!  3 frames  her-moved 0.5e f> -> -1 }T
true blind  debug:debilitas-out  5 frames

testing off screen: no scuffle with a stalker a script has (Hewie_Scuffle's +0xE0)
2 seed !
front-garden-2 room!  10 frames  140.64e 124e -217.33e 0e tp-facing  5 frames  game:hewie send join-fiona  20 frames
front-garden-3 room!  2 frames
[: 0 his-scuffled !  0 his-hold-call ! ;] his!  -1 game:debilitas send scripted  1 frames
true blind  front-garden-2 0 game:debilitas send stalker-in  2 frames  [: 2 mode! ;] deb!
T{ [: [: his-scuffled @ ;] his@ ;] 30 soon -> 0 }T
0 game:debilitas send scripted  2 frames
T{ [: [: his-scuffled @ ;] his@ ;] 30 soon -> -1 }T
debug:debilitas-out  5 frames
false blind
test-summary
