\ The stalkers, P1: Debilitas in the house (docs/subsystems/stalker.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/stalker/test_stalker.fs
IN: test-stalker
USING: tester engine game-state actors messages room-names game debug stalker stalker.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: danger-room ( -- room )  game:danger enter  danger:s-room @  leave-actor ;   \ (the room he last told)
: shown? ( -- flag )  [: him act.visible l@ 0<> ;] his ;

free-play  20 frames
testing out of the game at first: no body, not shown
T{ game:debilitas body?  shown? -> 0 0 }T
testing into Fiona's room: placed, shown, waiting about (mode 3), full health
s" fiona" actor-named body-tri debug:debilitas-tri  2 frames
T{ game:debilitas body?  game:debilitas body-room room-id =  shown? -> -1 -1 -1 }T
T{ [: my-mode @  hp @  hp-max @ ;] his -> 3 70 70 }T
testing he tells the danger he is about
T{ danger-room -> room-id }T
testing in another room he isn't shown
front-garden-2 room!  5 frames
T{ shown? -> 0 }T
testing out of the game
debug:debilitas-out  2 frames
T{ game:debilitas body?  shown? -> 0 0 }T

test-summary
