\ The story, S4: fades, music, a scene (its movie and cutscene) and skipping it from the movie
\ pause (docs/subsystems/story.md, screen.md, music.md, pause.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_scene.fs
IN: test-scene
USING: tester engine game-state actors messages keys room-names game debug flag-names story-names fiona.state facts screen music pause ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: screen-level ( -- n )  game:screen enter  screen:level @  leave-actor ;
: bgm-playing ( -- track )  game:music enter  bgm-cur @  leave-actor ;
: pause-stage ( -- n )  game:pause enter  pause:stage @  leave-actor ;
: in-scene? ( -- flag )  game:fiona enter  her-in-scene @  leave-actor ;
: msg? ( -- flag )  game:window enter  window:shown @  leave-actor  0< 0= ;
: play ( n -- )  0 ?do  msg? if  cross press  then  game-tick  loop ;   \ (the messages read on)
: within ( n xt -- flag )   \ (within n frames xt says so)
    swap 0 ?do  msg? if  cross press  then  game-tick  dup execute if  drop true unloop exit  then  loop  drop false ;

60 run-frames
testing a new game: the cage room's opening - its stage music made, the background music, faded in
T{ bgm-playing  game:music enter sm-stage @ sm-step @ leave-actor -> $16 0 4 }T
T{ screen-level [: story.kit:fading @ ;] in-room -> 0 0 }T   \ (front-garden-3: a place)

\ ---- a place's fades (the kit's fade: front-garden-2's place) ----
front-garden-2 -1 rooms send go-to-room  30 run-frames
testing a fade out from a place: black after its frames, the room told it is over (kind 4: 15 frames later, $BC)
[: 20 4 story.kit:fade ;] in-room
T{ 10 run-frames  screen-level dup 0> swap 1000 < and  [: story.kit:fading @ ;] in-room -> -1 -1 }T
T{ 15 run-frames  screen-level [: story.kit:fading @ ;] in-room -> 1000 -1 }T
T{ 15 run-frames  [: story.kit:fading @ ;] in-room -> 0 }T
[: 0 1 story.kit:fade ;] in-room  2 run-frames
T{ screen-level -> 0 }T
testing the other kinds: 0 out (to black), 6 to half dark and 7 back (the instructions' pages)
T{ [: 0 0 story.kit:fade ;] in-room  2 run-frames  screen-level -> 1000 }T
[: 0 1 story.kit:fade ;] in-room
T{ [: 10 6 story.kit:fade ;] in-room  15 run-frames  screen-level -> 500 }T
T{ [: 10 7 story.kit:fade ;] in-room  15 run-frames  screen-level -> 0 }T

testing the scene (Hewie freed from the wire, at night): the movie, the cutscene, Fiona in it, its subtitles
c2f-3-flag1 story-flag-set  0 game:fiona send costume  castle-2f-3 room!  30 run-frames
front-garden-3 room!  60 run-frames
228.1e 0e 72e 0e tp-facing  10 run-frames   \ (by Hewie in the wire, facing him)
: subtitles ( -- msg )  s" scenes" actor-named enter  scenes:subtitles @  leave-actor ;
circle press
T{ 600 [: cutscene-active? ;] within  movie-status  in-scene?  subtitles -> -1 1 -1 0 }T
T{ 90 [: bgm-playing $FF = ;] within -> -1 }T   \ (faded out with the scene's fade)

testing the movie pause: Start opens it, Cancel skips - the scene's script ends it
T{ start-button press  10 run-frames  pause-stage -> 2 }T
T{ triangle press  10 run-frames  movie-skipped state-flag? -> -1 }T   \ (Cancel skips)
T{ 30 [: cutscene-active? 0= ;] within  movie-status  pause-stage -> -1 0 0 }T
T{ 300 [: in-scene? 0= ;] within  200 [: bgm-playing $16 = ;] within -> -1 -1 }T

test-summary
