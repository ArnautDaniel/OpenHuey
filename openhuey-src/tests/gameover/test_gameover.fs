\ The game over (docs/subsystems/gameover.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/gameover/test_gameover.fs
IN: test-gameover
USING: tester engine game-state actors messages room-names game debug keys facts flag-names gameover ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: step? ( -- n )  game:gameover enter  gameover:step @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
create words 16 allot
: tint ( -- w )  $1F words look@ drop  words l@ ;

free-play  10 frames  front-garden-2 room!  10 frames
testing caught: the game over begins - the flag taken, the world still (the panic held)
T{ step? -> -1 }T
caught state-flag-set  3 frames
T{ step? 0< 0=  caught state-flag?  panic-held state-flag? -> -1 0 -1 }T
testing the characters hidden, the room drawn alone (GameOver: each one's +0x29)
T{ game:fiona enter  fiona.state:her-model @ actor act.visible l@  leave-actor -> 0 }T
testing the tint goes purple over 60 frames (after the music's start)
: purple? ( -- flag )  tint $AA0000C8 = ;
T{ ' purple? 400 soon -> -1 }T
testing a face button cuts the blue drift short; then black, and the game is over (a fresh game asked for)
: done? ( -- flag )  step? 12 = ;
: press-on ( -- flag )  cross debug:press  done? ;
T{ ' press-on 30000 soon -> -1 }T   \ (the movie plays in real time: headless frames are quicker)
testing the story's end (kind 2): over at once - the ending follows (ending.fs)
game:gameover enter  -1 gameover:step !  leave-actor  0 still!  panic-held state-flag-clear  world-held state-flag-clear
2 game:gameover send game-over-kind  2 frames  caught state-flag-set  2 frames
T{ step? -> 12 }T

testing a death in a movie (kind 0): the world not stilled, the movie stopped; the tint purple as the others
game:gameover enter  -1 gameover:step !  leave-actor  panic-held state-flag-clear  world-held state-flag-clear
s" OPENING.SFD" movie-open drop  10 frames   \ (a movie on)
0 game:gameover send game-over-kind  2 frames  caught state-flag-set  3 frames
T{ step? 0< 0=  panic-held state-flag?  movie-status 1 = -> -1 0 0 }T
T{ ' purple? 400 soon -> -1 }T

testing the mini-game's run (Hewie played): over, it starts again in place - the extras room by
testing its exit $81, Fiona played again, the stalker gone (SceneGame_SubTransition)
game:gameover enter  -1 gameover:step !  leave-actor  movie-close  0 still!  panic-held state-flag-clear  world-held state-flag-clear
-1 hewie-played !  2 frames  caught state-flag-set  2 frames
T{ step? 0< 0= -> -1 }T
T{ ' press-on 30000 soon -> -1 }T
20 frames
T{ room-id  hewie-played?  panic-held state-flag? -> off-map-37 0 0 }T

test-summary
