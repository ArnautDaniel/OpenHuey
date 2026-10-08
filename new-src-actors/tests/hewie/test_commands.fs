\ Hewie, phase H2 (Fiona F3): her gestures and commands, how he takes them, the meeting by his
\ side (docs/subsystems/hewie.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/hewie/test_commands.fs
IN: test-commands
USING: tester engine game-state actors messages common facts room-names game hewie.state hewie.body hewie.model hewie.states fiona.state fiona.model flag-names ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: his@ ( xt -- x )  hewie enter  execute  leave-actor ;
: his! ( xt -- )  hewie enter  execute  leave-actor ;
: her@ ( xt -- x )  fiona enter  execute  leave-actor ;
: action ( -- n )  [: his-action @ ;] his@ ;
: apart ( F: -- d )  hewie body-at fiona body-at vec-dist ;
: press ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;   \ (read, then acted on)
\ within n frames he takes up action a
: takes? ( n a -- flag )  swap 0 ?do  game-tick  dup action = if  drop true unloop exit  then  loop  drop false ;
: her-anim? ( n anim -- flag )  swap 0 ?do  game-tick  dup [: fiona.model:anim@ ;] her@ = if  drop true unloop exit  then  loop  drop false ;
: set-flag ( n -- )  dup 5 rshift 4 * progress pr.state +  swap 31 and 1 swap lshift  over l@ or swap l! ;
1 seed !

5 frames
hewie send join-fiona  3 frames
\ (calm, trusting, obeying, by her: the default action)
: ready ( -- )  [: 1000 add-trust  obeys  0 -1 set-mood  0 0 set-action ;] his!  30 frames ;
ready

testing without state flag 0xD her gestures do nothing
T{ key: 2 press  60 frames  [: her-mode @ ;] her@ -> 0 }T
T{ [: his-cmd-was @ ;] his@ -> 0 }T
hewie-commandable set-flag

testing what she tells: Hewie's broadcast, read by her
T{ [: dog-here @ dog-action @ ;] her@  -> 2 action }T

testing "come back" (down) on the spot: a gesture (mode 0xD), the order heard (0x2C), he answers (0x1D) and comes (0xD)
ready
T{ key: 2 press  [: her-mode @ her-sub @ ;] her@ -> $D $2C }T
T{ 200 $1D takes? -> -1 }T
T{ [: his-cmd-was @ ;] his@ -> $2C }T
T{ 200 $D takes? -> -1 }T
120 frames

testing "go there" (up): 0x23, he answers and makes for the spot she showed (0x63)
ready
T{ key: 1 press  [: her-sub @ ;] her@ -> $23 }T
T{ 300 $63 takes? -> -1 }T
T{ [: his-to-tri @ ;] his@ 0< -> 0 }T
300 frames

\ (the cage room's floor by her is mostly 0x80001, where he won't meet: to front-garden-2's open floor)
front-garden-2 -1 rooms send go-to-room  5 frames
hewie send part-from-fiona  2 frames  hewie send join-fiona  3 frames
testing the meeting by his side (right, close): praised close up - he turns with her (0x48), sits, is petted (0x4A)
ready  [: 0 his-cooldown ! ;] his!
\ (him put by her, well within 15, on open floor: not flags 0x80001, where he won't meet)
create try-at 12 allot
fvariable ox  fvariable oz
: open-by-her? ( F: angle -- flag )
    fdup fsin 7e f* ox f!  fcos 7e f* oz f!
    fiona body-at vec@  oz f@ f+  frot ox f@ f+  frot frot  try-at vec!
    try-at $29020008 v-tri-in dup 0< if  drop false exit  then  nav-flags $80001 and 0= ;
: close ( -- )
    false  36 0 do  i 10 * s>f deg>rad open-by-her? if  drop true leave  then  loop
    0= if  ." no open floor by her" cr  then
    [: try-at vec@  his-place  1 0 set-action ;] his!  2 frames ;   \ (by her: listening)
close
T{ apart 15e f< -> -1 }T
T{ key: 4 press  [: her-sub @ ;] her@ -> $28 }T
T{ 200 $48 takes? -> -1 }T
T{ 400 $4A takes? -> -1 }T
T{ 200 $C07 her-anim? -> -1 }T
400 frames

testing her cry for help when panicking: a command 0x30 he weighs
ready
T{ [: 2 her-fear-bits !  0 her-cmd ! ;] fiona enter execute leave-actor  2 frames  [: his-cmd-was @ ;] his@ -> $30 }T
[: 0 her-fear-bits ! ;] fiona enter execute leave-actor

testing a reaction: while he waits (does as he likes) her doings cost his patience
T{ [: 1 his-waiting !  1000 his-obey ! ;] his!  5 hewie send reaction  deliver  [: his-obey @ ;] his@ 1000 < -> -1 }T

test-summary
