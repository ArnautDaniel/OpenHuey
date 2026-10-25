\ The opening as a new game plays it (docs/subsystems/story.md): the cage room's entering script
\ (front-garden-3's place: the doors locked, the world held, the opening - its music and figures,
\ the fade in) and then she is the player's, to find what the room offers - the first, "Where am
\ I?" (area $11). Not free-play: from the new game itself. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_opening.fs
IN: test-opening
USING: tester engine game-state actors messages keys room-names game debug flag-names story-names story.state story.words ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: within ( n xt -- flag )   \ (within n frames xt says so)
    swap 0 ?do  game-tick  dup execute if  drop true unloop exit  then  loop  drop false ;

testing a new game: the cage room, the opening begun (its entering script, Fiona's act01)
T{ 30 [: opening-done story-flag? ;] within  room-id -> -1 front-garden-3 }T
testing after the fade the world is hers: not held, and she walks
60 frames
T{ world-held state-flag? -> 0 }T
create was 12 allot  game:fiona body-pos was vec!
T{ key: W true key-hold  20 frames  key: W false key-hold  was game:fiona body-at vec-dist 1e f> -> -1 }T
testing "Where am I?" - in area $11 the room offers its action 4; taken, its message, and the flag set
T{ fg-3-flag1 story-flag? -> 0 }T
T{ $11 0 tp-area-facing  3 frames  [: story.kit:offer-xt @ ;] in-room -> ' story.places.front-garden-3:waking }T
T{ circle press  300 [: shown 6 = ;] within -> -1 }T   \ (her waking animation first: about 170 frames)
T{ cross press  120 [: fg-3-flag1 story-flag? ;] within -> -1 }T

test-summary
