\ The story, S4: fades, music, a scene (its movie and cutscene) and skipping it from the movie
\ pause (docs/subsystems/story.md, screen.md, music.md, pause.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/story/test_scene.fs
IN: test-scene
USING: tester engine game-state actors messages keys room-names game debug flag-names fiona.state story.state story.words screen music pause ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: screen-level ( -- n )  game:screen enter  screen:level @  leave-actor ;
: bgm-playing ( -- track )  game:music enter  bgm-cur @  leave-actor ;
: pause-stage ( -- n )  game:pause enter  pause:stage @  leave-actor ;
: in-scene? ( -- flag )  game:fiona enter  her-in-scene @  leave-actor ;
: msg? ( -- flag )  [: msg @ ;] in-room 0< 0= ;
: play ( n -- )  0 ?do  msg? if  circle press  then  game-tick  loop ;   \ (the messages read on)
: within ( n xt -- flag )   \ (within n frames xt says so)
    swap 0 ?do  msg? if  circle press  then  game-tick  dup execute if  drop true unloop exit  then  loop  drop false ;

60 frames
testing a new game: the cage room's opening - its stage music made, the background music, faded in
T{ bgm-playing  game:music enter sm-stage @ sm-step @ leave-actor -> $16 0 4 }T
T{ screen-level [: story.state:fading @ ;] in-room -> 0 0 }T

testing a fade out from the room's scripts: black after its frames, the room told it is over
[: 20 4 story.words:fade ;] in-room
T{ 10 frames  screen-level dup 0> swap 1000 < and  [: story.state:fading @ ;] in-room -> -1 -1 }T
T{ 15 frames  screen-level [: story.state:fading @ ;] in-room -> 1000 0 }T
[: 0 1 story.words:fade ;] in-room  2 frames
T{ screen-level -> 0 }T

testing the scene (act00): the movie, the cutscene, Fiona in it, the subtitles prepared
world-held state-flag-clear   \ (as the opening left it, before her first scene)
[: 0 0 0 action ;] in-room
T{ 300 [: cutscene-active? ;] within  movie-status  in-scene?  [: prepared-msg @ ;] in-room -> -1 1 -1 0 }T
T{ 90 [: bgm-playing $FF = ;] within -> -1 }T   \ (faded out with the scene's fade)

testing the movie pause: Start opens it, Cancel skips - the scene's script ends it
T{ start-button press  10 frames  pause-stage -> 2 }T
T{ cross press  10 frames  movie-skipped state-flag? -> -1 }T
T{ 30 [: cutscene-active? 0= ;] within  movie-status  pause-stage -> -1 0 0 }T
T{ 300 [: in-scene? 0= ;] within  200 [: bgm-playing $16 = ;] within -> -1 -1 }T

test-summary
