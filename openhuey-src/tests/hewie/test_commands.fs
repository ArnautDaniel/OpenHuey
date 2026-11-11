\ Hewie, phase H2 (Fiona F3): her gestures and commands, how he takes them, the meeting by his
\ side (docs/subsystems/hewie.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/hewie/test_commands.fs
IN: test-commands
USING: tester engine game-state actors messages common facts room-names game hewie.commands hewie.state hewie.body hewie.model hewie.states fiona.state fiona.model flag-names debug ;

: his@ ( xt -- x )  hewie enter  execute  leave-actor ;
: his! ( xt -- )  hewie enter  execute  leave-actor ;
: her@ ( xt -- x )  fiona enter  execute  leave-actor ;
: action ( -- n )  [: his-action @ ;] his@ ;
: apart ( F: -- d )  hewie body-at fiona body-at vec-dist ;
: press-key ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;   \ (read, then acted on)
\ within n frames he takes up action a
: takes? ( n a -- flag )  swap 0 ?do  game-tick  dup action = if  drop true unloop exit  then  loop  drop false ;
: her-anim? ( n anim -- flag )  swap 0 ?do  game-tick  dup [: fiona.model:anim@ ;] her@ = if  drop true unloop exit  then  loop  drop false ;
: set-flag ( n -- )  dup 5 rshift 4 * progress pr.state +  swap 31 and 1 swap lshift  over l@ or swap l! ;
1 seed !

5 run-frames
0 fiona send costume  1 run-frames   \ (her clothes: these are her mechanics, not the new game's slip)
hewie send join-fiona  3 run-frames
\ (calm, trusting, obeying, by her: the default action)
: ready ( -- )  [: 1000 add-trust  obeys  0 -1 set-mood  0 0 set-action ;] his!  30 run-frames ;
ready

testing without state flag 0xD her gestures do nothing
T{ key: K press-key  60 run-frames  [: her-mode @ ;] her@ -> 0 }T
T{ [: his-cmd-was @ ;] his@ -> 0 }T
hewie-commandable set-flag

testing what she tells: Hewie's broadcast, read by her
T{ [: dog-here @ dog-action @ ;] her@  -> 2 action }T

testing "come back" (down) on the spot: a gesture (mode 0xD), the order heard (0x2C), he answers (0x1D) and comes (0xD)
ready
T{ key: K press-key  [: her-mode @ her-sub @ ;] her@ -> $D $2C }T
T{ 200 $1D takes? -> -1 }T
T{ [: his-cmd-was @ ;] his@ -> $2C }T
T{ 200 $D takes? -> -1 }T

testing "come back" while the story hides him away: its time cut short and he picks anew (Hewie_ActOnCommand: +0xF3559)
T{ [: 5 his-yelp !  0 his-pending !  $2C act-away  his-yelp @  his-pending @ ;] his@ -> 0 -1 1 }T
[: -1 his-yelp !  0 his-pending ! ;] his!  ready
testing she leaves while a script has him: it lets go first (Hewie_Vt34: Hewie_FullStop), so he follows her
T{ [: -1 his-busy !  -1 hewie.offscreen:fiona-left  his-busy @ ;] his@ -> 0 }T
ready
testing placed by the story in another room: his default action there, not what he was about (Hewie_PlaceInRoom)
[: hewie-names:through-act his-action ! ;] his!
T{ off-map-0d -1 2 hewie send hewie-place  deliver  action hewie-names:through-act = -> 0 }T
room-id fiona body-tri 2 hewie send hewie-place  deliver  ready
testing out of sight he crosses a room at 1.6 a frame - her cry's 10 only to his first door; 0.38 hurt (Hewie_State1920)
T{ [: hewie-names:to-her-cry-act his-action !  hewie.offscreen:between-speed 100e f* f>s ;] his@ -> 160 }T
ready
testing a door out of sight: open - his way; locked against him - not (Progress_ExitPassable, slot 1)
T{ 15 true doors-id send door-opened  deliver  [: 15 hewie.offscreen:door-usable? ;] his@ -> -1 }T
T{ 15 hewie-kind true doors-id send lock-for  deliver  [: 15 hewie.offscreen:door-usable? ;] his@ -> 0 }T
15 hewie-kind false doors-id send lock-for  15 false doors-id send door-opened  deliver
testing the keyboard's praise (L) is one a press, as the stick's one a push - not one a frame while held
T{ key: L true key-hold  game-tick  keys:gesture@  game-tick  keys:gesture@  key: L false key-hold  game-tick -> 3 -1 }T
60 run-frames  ready
full-suite   \ (on: waits, scenes, walks - the full run's)
120 run-frames

testing "come back" on the move (walking): no stop for a gesture; she glances at him (30 frames)
ready
key: W true key-hold  10 run-frames
T{ key: K press-key  [: her-mode @  her-look-who @ game:hewie =  her-look-hold @ 0> ;] her@ -> 0 -1 -1 }T
key: W false key-hold  60 run-frames

testing "go there" (up): 0x23, he answers and makes for the spot she showed (0x63)
ready
T{ key: I press-key  [: her-sub @ ;] her@ -> $23 }T
T{ 300 $63 takes? -> -1 }T
T{ [: his-to-tri @ ;] his@ 0< -> 0 }T
300 run-frames

\ (the cage room's floor by her is mostly 0x80001, where he won't meet: to front-garden-2's open floor)
front-garden-2 -1 rooms send go-to-room  5 run-frames
hewie send part-from-fiona  2 run-frames  hewie send join-fiona  3 run-frames
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
    [: try-at vec@  his-place  1 0 set-action ;] his!  2 run-frames ;   \ (by her: listening)
close
T{ apart 15e f< -> -1 }T
T{ key: L press-key  [: her-sub @ ;] her@ -> $28 }T
T{ 200 $48 takes? -> -1 }T
T{ 400 $4A takes? -> -1 }T
T{ 200 $C07 her-anim? -> -1 }T
400 run-frames

testing her cry for help when panicking: a command 0x30 he weighs
ready
T{ [: 2 her-fear-bits !  0 her-cmd ! ;] fiona enter execute leave-actor  2 run-frames  [: his-cmd-was @ ;] his@ -> $30 }T
[: 0 her-fear-bits ! ;] fiona enter execute leave-actor

testing a reaction: while he waits (does as he likes) her doings cost his patience
T{ [: 1 his-waiting !  1000 his-obey ! ;] his!  5 hewie send reaction  deliver  [: his-obey @ ;] his@ 1000 < -> -1 }T

testing from another room: her cry for help brings him to her ($39), "come back" too ($2D)
: next-room ( -- room )  room-id 0 room-exit-leads drop ;
: away-cmd ( code -- )
    next-room -1 0 hewie send hewie-place  deliver  2 run-frames
    [: 0 his-cooldown !  $DF his-cmd ! ;] his!   \ (as waiting off screen: 0x2E)
    -1 0 0 0 hewie send command  deliver ;   \ (code tri yaw x z)
T{ $30 away-cmd  [: his-away @ 0<>  his-action @ ;] his@ -> -1 $39 }T
T{ [: 0 0 set-action ;] his!  $2C away-cmd  [: his-action @ ;] his@ -> $2D }T

test-summary
