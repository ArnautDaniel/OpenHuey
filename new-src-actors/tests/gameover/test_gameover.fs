\ The game over (docs/subsystems/gameover.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/gameover/test_gameover.fs
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
testing the tint goes purple over 60 frames (after the music's start)
: purple? ( -- flag )  tint $AA0000C8 = ;
T{ ' purple? 400 soon -> -1 }T
testing a face button cuts the blue drift short; then black, and the game is over (a fresh game asked for)
: done? ( -- flag )  step? 12 = ;
: press-on ( -- flag )  cross debug:press  done? ;
T{ ' press-on 3000 soon -> -1 }T
testing a continue (kind 2): over at once
game:gameover enter  -1 gameover:step !  leave-actor  0 still!  panic-held state-flag-clear  world-held state-flag-clear
2 game:gameover send game-over-kind  2 frames  caught state-flag-set  2 frames
T{ step? -> 12 }T

test-summary
