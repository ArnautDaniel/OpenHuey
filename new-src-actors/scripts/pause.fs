\ pause.fs - the pause screen (docs/subsystems/pause.md; the original's SceneGame +0x73EBA0,
\ src/game/pause.c). For now its movie half: Start while a scene's movie plays opens it - the
\ movie paused, the screen dimmed, "Unpause game" / "Skip"; Start goes back, Cancel skips: the
\ screen darkens to black and movie-skipped is set for the scene's script, which ends the scene
\ and closes it (pause-wanted). (The play pause - "back" / "quit" - and the sound turned down
\ while it is open: later.)
IN: pause
USING: engine game-state actors messages keys flag-names ;

5 constant pause-layer   \ (the UI layers: 0 the movie, 1 the fade, 2 the window, 3 the prompt, 4 subtitles, 5 this)

state: pause-state
  1 floats field t        \ how far open (0..1: 0.2 a frame)
  cell field stage        \ 0 closed, 1 opening, 2 open, 3 skipping, 4 closing
end-state

: flag-word ( n -- mask addr )  1 over 31 and lshift  swap 5 rshift 4 * progress pr.state + ;
: flag? ( n -- flag )  flag-word l@ and 0<> ;
: flag-on ( n -- )  flag-word dup l@ rot or swap l! ;
: flag-off ( n -- )  flag-word dup l@ rot invert and swap l! ;
: sound ( id -- )  5 bank-sound ;   \ (the driver's +0x14, bank 5)
: t+ ( F: d -- )  t f@ f+ 0e fmax 1e fmin t f! ;
: wanted? ( -- flag )   \ (SceneGame_SubPlay: Start, while a scene with a movie has the camera)
    start-button button-pressed?  world-held flag? 0= and  no-pause flag? 0= and
    cutscene-active? and  movie-status 1 = and ;
: open ( -- )  0e t f!  1 stage !  1 movie-pause  $96 sound ;   \ Pause_Open, mode 1
: closed ( -- )  0 stage !  0e t f! ;
: step ( -- )
    stage @ case
        0 of  wanted? if  open  then  endof
        1 of  0.2e t+  t f@ 1e f>= if  2 stage !  then  endof   \ MoviePause_StateOpening
        2 of                                                     \ MoviePause_StateOpen
            cross button-pressed? if  0e t f!  3 stage !  $97 sound  exit  then
            start-button button-pressed? if  0 movie-pause  4 stage !  $97 sound  then  endof
        3 of  0.2e t+  t f@ 1e f>= if  movie-skipped flag-on  then  endof   \ MoviePause_StateSkip
        4 of  -0.2e t+  t f@ f0> 0= if  closed  then  endof      \ MoviePause_StateClosing
    endcase ;

\ ---- drawing: the dimming (alpha 63 open, to 255 skipping, of 255 here twice the PS2's 128
\ scale) and the two lines at (320, 400) / (320, 420) of 512 x 448, fading with it ----
variable sw  variable sh
: dim ( -- a )
    stage @ 3 = if  63e 192e t f@ f* f+  else  63e t f@ f*  then  f>s 2* 255 min ;
: line ( msg y -- )
    >r  message-layout drop  pause-layer  0 0 message-line
    sw @ 320 * 512 /  sh @ r> * 448 /
    stage @ 3 = if  1e t f@ f-  else  t f@  then  255e f* f>s $FFFFFF00 or  2 ui-text ;
: draw ( -- )
    pause-layer ui-clear
    stage @ 0= if  exit  then
    screen-size sh ! sw !  2 pen-scale
    pause-layer  0 0 sw @ sh @  dim  ui-rect
    $8089 400 line  $808A 420 line ;

behaviour pausing
  on spawned ( -- )  0 stage !  self subscribe tick  self subscribe frame-end ;
  on tick ( -- )  step ;
  on frame-end ( -- )   \ (flag 6, back to play: the scene's script sets it to close the skip; cleared each frame)
      pause-wanted flag? stage @ 3 = and if  closed  then  pause-wanted flag-off  draw ;
end-behaviour

: pause-spawn ( -- id )  pausing pause-state s" pause" spawn ;
