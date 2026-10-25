\ The rooms' breakables and a door burst open (story/words.fs door-bits, game/room.c): a vase
\ is the room mesh's own batches - whole in group $15, its shards in $16 - shown and hidden by
\ the room's scripts (part-shown / part-hidden); a shut door burst open from its area 4
\ (D_003E51A0, Fiona_SlamDoor). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_breakables.fs
IN: test-breakables
USING: tester engine game-state actors messages room-names game debug keys facts flag-names story-names vectors fiona.state fiona.slam ;

: frames ( n -- )  0 ?do  game-tick  loop ;

free-play  10 frames  0 game:fiona send costume  castle-1f-11 room!  200 frames
testing castle 1F's vase, not broken: whole (the room's group $15), no shards ($16)
T{ c1f-11-breakable1-broken story-flag?  $15 room-group?  $16 room-group? -> 0 -1 0 }T

testing front garden 2's door: area 4 before its shut leaf - she, there and facing it, bursts it open
front-garden-2 room!  200 frames
52e 124e -191e  0 door-turn rad>deg  debug:tp-facing  10 frames
T{ 0 4 game:fiona body-pos door-in-area?  [: 0 true barge? ;] game:fiona enter execute leave-actor -> -1 -1 }T

test-summary
