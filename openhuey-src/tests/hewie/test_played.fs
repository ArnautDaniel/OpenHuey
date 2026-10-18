\ Hewie played: the mini-game's run (hewie/steered.fs; docs/subsystems/hewie.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/hewie/test_played.fs
IN: test-played
USING: tester engine game-state actors messages common vectors facts keys room-names hewie-names game hewie.state hewie.body hewie.anims hewie.model ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: his@ ( xt -- x )  hewie enter  execute  leave-actor ;
: action ( -- n )  [: his-action @ ;] his@ ;
: press ( key -- )  dup true key-hold  game-tick  false key-hold ;
create was /vec allot
: moved ( F: -- d )  hewie body-at was vec-dist-xz ;
: mark ( -- )  was hewie body-at vec-copy ;
1 seed !

front-garden-2 -1 rooms send go-to-room  5 frames
hewie send join-fiona  5 frames
testing played: afresh, steered (the run's clock: Hewie_Restart)
-1 hewie-played !  hewie send hewie-restart  3 frames
T{ action -> steered-act }T
T{ [: his-still @ ;] his@ -> 6 }T

testing the stick runs him; cross held, flat out
mark  key: W true key-hold  45 frames
T{ moved 10e f>  [: anim@ ;] his@ -> -1 trot-anim }T
key: X true key-hold  20 frames
T{ [: anim@ ;] his@ -> run-anim }T
key: X false key-hold
key: W false key-hold  10 frames
T{ [: his-still @ ;] his@ -> 6 }T
mark  20 frames
T{ moved 1e f< -> -1 }T   \ (the stick let go: he stops)

testing a gesture barks, then he is steered again
key: I press
T{ [: his-gesture @ ;] his@ -> 0 }T   \ (up: read the frame it came)
5 frames
T{ action steered-act <> -> -1 }T
200 frames
T{ action -> steered-act }T

testing given back: as he was
0 hewie-played !

test-summary
