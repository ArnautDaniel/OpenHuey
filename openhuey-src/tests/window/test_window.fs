\ The message window (window.fs; src/game/text.c Task_State*): a page typed out, then turned by
\ any face button, a shoulder or Select; a choice's keys and sounds; a message closing itself.
\ Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/window/test_window.fs
IN: test-window
USING: tester engine game-state actors messages game debug window ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: press ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: its ( xt -- x )  game:window enter  execute  leave-actor ;
: shown? ( -- msg )  [: window:shown @ ;] its ;
: page? ( -- n )  [: window:page @ ;] its ;
: picked? ( -- n )  [: window:picked @ ;] its ;
: show ( msg -- )  game:window send show-text  deliver  2 frames ;
free-play  20 frames

testing a page turned by circle, L1 closing the last (the pad's 0xFC01, not only cross)
$8010 show
T{ shown? page? -> $8010 0 }T
key: C press
T{ page? -> 1 }T
key: Q press
T{ shown? -> -1 }T
testing Start doesn't turn it
$8010 show  key: Return press
6 frames
T{ page? shown? -> 0 $8010 }T  key: Return press  6 frames
key: V press  key: E press
T{ shown? -> -1 }T

testing a choice: up / down round the options, the triangle to the last (cancel allowed: flags 0)
$8006 show  key: X press
T{ page? picked? -> 1 0 }T
key: Up press
T{ picked? -> 1 }T
key: Down press
T{ picked? -> 0 }T
key: Right press   \ (both on one line: along it)
T{ picked? -> 1 }T
key: Right press
T{ picked? -> 0 }T
key: V press
T{ picked? shown? -> 1 $8006 }T
key: X press
T{ shown? -> -1 }T
testing a choice that may not be cancelled (flags bit 0)
T{ $8006 message-layout drop message-choice-flags 1 and  $403C message-layout drop message-choice-flags 1 and -> 0 0 }T

testing a message closing itself 62 frames after it is typed (0x10 62)
T{ $8015 message-layout drop message-close-after -> 62 }T
$8015 show  55 frames
T{ shown? -> $8015 }T
10 frames
T{ shown? -> -1 }T

testing typed at a speed (0x0C) with a pause (0x09): the triangle types to the pause; no turning till typed
create typing 16 allot
\ "AB" at speed 2 (4 frames each, and one), a pause of 10, "C"
$0C typing c!  2 typing 1+ c!  char A typing 2 + c!  char B typing 3 + c!
9 typing 4 + c!  10 typing 5 + c!  char C typing 6 + c!
0 typing 7 message-param$
T{ $8007 message-layout drop  0 message-typed  0 0 message-skip  0 10 message-skip  0 12 message-skip -> 31 10 10 31 }T   \ (the rest of it at speed 2 too)
$8007 game:window send show-text  deliver
T{ [: window:typed @ ;] its -> 0 }T
1 frames
key: V press
T{ [: window:typed @ ;] its  shown? -> 12 $8007 }T
key: X press
T{ shown? -> $8007 }T
25 frames  key: X press
T{ shown? -> -1 }T
testing the box's place (0x0A n, Task_Layout): none given, low and boxed; a tutorial's 0x81 - the middle, no box
T{ $8010 message-layout drop  0 message-place  $40A5 message-layout drop  0 message-place -> -1 $81 }T
: box-centre ( -- y )  [: window:box-y @  window:box-h @ 2/ + ;] its ;
: screen-h ( -- h )  screen-size nip ;
$8010 show  2 frames
T{ [: window:boxed? ;] its  box-centre 448 * screen-h /  370 6 + - abs 3 < -> -1 -1 }T
key: X press  key: X press
$40A5 show  2 frames
T{ [: window:boxed? ;] its  box-centre 448 * screen-h /  224 6 + - abs 3 < -> 0 -1 }T
key: X press  key: X press  key: X press  20 frames
T{ shown? -> -1 }T

testing a line to the right (0x14) and the options taking no room (Task_CollectChoices: the hand before them)
T{ $8006 message-layout drop  1 1 message-align  1 0 message-align -> 2 0 }T
T{ 1 message-option-at  1 1 message-line nip -> 1 1 6 8 }T   \ ("YES   NO")
test-summary
