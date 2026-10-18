\ The dressing scene (castle-2f-3, her action $D): she changes into her clothes, the instructions'
\ pages are dimmed in (fade kind 6) and out (kind 7), and she is the player's again - the screen
\ clear (docs/subsystems/screen.md). Headless (its movie skipped, as from the pause):
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_dressing.fs
IN: test-dressing
USING: tester engine game-state actors messages keys room-names game debug flag-names fiona.state story.state story.words screen ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: screen-level ( -- n )  game:screen enter  screen:level @  leave-actor ;
: msg? ( -- flag )  [: msg @ ;] in-room 0< 0= ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: scene-running? ( -- flag )  [: 1 slot-task @ 0<> ;] in-room ;   \ (her action script's slot: 1 + her character slot)
: play ( n -- )   \ (its messages read on - the first answer yes - and its movie skipped)
    0 ?do  msg? if  i 20 mod 0= if  cross press  then  then  game-tick
        i 200 = if  movie-skipped state-flag-set  then  loop ;

60 frames
castle-2f-3 -1 rooms send go-to-room  60 frames
testing the scene runs to its end: the screen clear, Fiona free in her clothes
[: 0 0 $D action ;] in-room
T{ scene-running? -> -1 }T
600 play
T{ scene-running?  screen-level  world-held state-flag? -> 0 0 0 }T
T{ [: her-scripted @ her-mode @ her-costume @ ;] her -> 0 0 0 }T
testing ... and she walks
key: W true key-hold  30 frames  key: W false key-hold
T{ [: her-sub @ ;] her -> 1 }T
test-summary
