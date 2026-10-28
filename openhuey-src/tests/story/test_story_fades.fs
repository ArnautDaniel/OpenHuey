\ The story's fades for the panic, against the original (src/game/event.c, fiona.c, scene_game.c):
\ a place's fade (the kit's) - the screen's step scripts (D_003D6A40) setting the panic's overlay
\ level (ScreenFade_Step) and keeping it from full while they go (Events_ScenePlaying); a level
\ under full stilling the camera and the heartbeat (ScreenFade_Frame); her panic's start holding
\ the place's doings (SceneGame +0xF6CD24 bit 3).
\ Headless:  build/openhuey-src/openhuey --test openhuey-src/tests/story/test_story_fades.fs
IN: test-story-fades
USING: tester engine game-state actors messages common room-names game debug facts flag-names story.kit ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: panic@ ( xt -- x )  game:panic enter  execute  leave-actor ;
: overlay ( -- thousandths )  [: panic:fade f@ 1000e f* fround f>s ;] panic@ ;
: room-fading ( -- flag )  [: fading @ ;] in-room ;
: fade! ( frames kind -- )  [: fade ;] in-room  deliver ;   \ (its first step at once, as in the frame a script starts it)
1 seed !

free-play  front-garden-2 room!  20 run-frames
stalkers-blind state-flag-set

testing a fade out (kind 0, 15 frames): the overlay level 0.58 at its first step, 0.04 less each (alpha 8 a step), then 0
T{ 15 0 fade!  deliver  overlay -> 580 }T
T{ 1 run-frames  overlay -> 540 }T
T{ 20 run-frames  overlay  room-fading -> 0 0 }T
testing ... its script goes on: the level not set back to full (Events_ScenePlaying)
T{ 300 game:panic send panic-overlay  30 run-frames  overlay -> 300 }T
testing a level under full: the heartbeat cut short, the camera still (ScreenFade_Frame)
T{ [: 0 panic:pulse ! ;] panic@  1 run-frames  [: panic:pulse @ ;] panic@ -> $80 }T
T{ [: calming-down panic:stage !  100 panic:panic-left ! ;] panic@  1 run-frames  [: panic:jolting @ ;] panic@ -> 0 }T
[: composed panic:stage !  0 panic:panic-left ! ;] panic@  0 game:panic send panic-level!

testing a fade in (kind 1): 0.44 at its first step, full at its last two (16 steps); then its step script ends
T{ 15 1 fade!  deliver  overlay -> 440 }T
T{ 13 run-frames  overlay -> 960 }T
T{ 1 run-frames  overlay  room-fading -> 1000 -1 }T
T{ 1 run-frames  room-fading -> -1 }T
T{ 1 run-frames  room-fading -> 0 }T
testing ... in play the level back to full each frame; the heartbeat and the camera's shake then go on
T{ 300 game:panic send panic-overlay  1 run-frames  overlay -> 1000 }T
T{ [: 0 panic:pulse ! ;] panic@  1 run-frames  [: panic:pulse @ ;] panic@ -> 4 }T
T{ [: calming-down panic:stage !  100 panic:panic-left ! ;] panic@  1 run-frames  [: panic:jolting @ ;] panic@ -> -1 }T
[: composed panic:stage !  0 panic:panic-left ! ;] panic@  0 game:panic send panic-level!

testing the dim kinds (6 / 7) leave the level; their scripts go on as the others'
T{ 10 6 fade!  15 run-frames  overlay -> 1000 }T
T{ 10 7 fade!  15 run-frames  overlay -> 1000 }T

testing to-black-2 (kind 4, the rooms' $44): over 15 frames after its steps ($BC)
20 4 fade!
T{ 22 run-frames  room-fading  overlay -> -1 0 }T
T{ 13 run-frames  room-fading -> -1 }T
T{ 1 run-frames  room-fading -> 0 }T
testing ... a room entered: its step script gone, the level full again
T{ 300 game:panic send panic-overlay  5 run-frames  overlay -> 300 }T
T{ room-id -1 game:screen send arrived  1 run-frames  overlay -> 1000 }T
testing from-black-2 (kind 5, $BD): its first 15 frames held, over no sooner than its 17th frame
3 5 fade!
T{ 15 run-frames  room-fading -> -1 }T
T{ 1 run-frames  room-fading -> 0 }T
testing her panic's start: the place's doings held; as its 30 frames pass, on again
variable turns
: counting ( -- )  begin  1 turns +!  yield  again ;
[: ['] counting start drop ;] in-room  3 run-frames
T{ turns @  2 run-frames  turns @ swap - -> 2 }T
T{ -1 broadcast panic-onset  1 run-frames  turns @  2 run-frames  turns @ swap - -> 0 }T
T{ 0 broadcast panic-onset  2 run-frames  turns @  2 run-frames  turns @ swap - -> 2 }T
[: ['] counting stop-doing ;] in-room

test-summary
