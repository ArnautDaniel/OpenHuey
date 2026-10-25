\ the mini-game's run from the title's MINI-GAME (docs/subsystems/hewie.md: Hewie played): the
\ extras room's mini-game menu up at once, challenge A picked - its room ($D9), the clock going,
\ Hewie the one played, Fiona with him, the stalker (kind 7) in the room next door. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/hewie/test_run.fs
IN: test-run
USING: tester engine game-state actors messages common vectors facts keys room-names hewie-names stalker-names game hewie.state fiona.state subscreen.state flag-names debug ;

: press-key ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: his@ ( xt -- x )  hewie enter  execute  leave-actor ;
: her@ ( xt -- x )  fiona enter  execute  leave-actor ;
: apart ( F: -- d )  hewie body-at fiona body-at vec-dist ;
1 seed !

testing the title's MINI-GAME: the extras room, its mini-game's menu up at once
-1 mini-game-wanted !  off-map-37 -1 rooms send go-to-room  70 run-frames
T{ room-id  subscreen-wanted state-flag? -> off-map-37 -1 }T

testing challenge A picked (down to it, the cross): its room, the clock started, Hewie played
key: Down press-key  key: X press-key
T{ progress pr.vars $2E + c@ -> 0 }T
600 run-frames
T{ room-id  hewie-played? -> off-map-d9 -1 }T
T{ [: his-action @ ;] his@ -> steered-act }T
T{ progress pr.play-frames l@ 0> -> -1 }T

testing Fiona there with him, by him; the stalker loaded as kind 7 next door
T{ fiona body-room  apart 50e f< -> off-map-d9 -1 }T
T{ stalker-in-game body-room -> off-map-d8 }T

testing the menu in the run (select): the options alone
: sub@ ( xt -- x )  s" subscreen" actor-named enter  execute  leave-actor ;
key: Tab press-key  20 run-frames
T{ [: stage @ shut <>  options-only @ ;] sub@ -> -1 -1 }T
key: Tab press-key  40 run-frames

0 hewie-played !
test-summary
