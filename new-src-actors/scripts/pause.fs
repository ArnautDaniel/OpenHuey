\ pause.fs - the pause screen (docs/subsystems/pause.md; the original's SceneGame +0x73EBA0,
\ src/game/pause.c). Two kinds:
\ - in play, Start: the world held, the screen dimmed, "PAUSE"; Start goes back, Select the menu
\   - "back to the game" / "quit"; quitting asks "yes / no", then darkens to the title. The
\   sound is turned down as far as the screen is open.
\ - while a scene's movie plays, Start: the movie paused, "Unpause game" / "Skip"; Start goes
\   back, Cancel skips - the screen darkens to black and movie-skipped is set for the scene's
\   script, which ends the scene and closes it (pause-wanted).
IN: pause
USING: engine game-state actors messages keys flag-names ps2-screen ;

5 constant pause-layer   \ (the UI layers: 0 the movie, 1 the fade, 2 the window, 3 the prompt, 4 subtitles, 5 this)

\ its stages: closed; the movie pause's; the play pause's
0 constant closed
1 constant movie-opening  2 constant movie-open  3 constant skipping  4 constant movie-closing
5 constant opening  6 constant open  7 constant closing  8 constant on-menu  9 constant menu-closing
10 constant asking  11 constant quitting

state: pause-state
  1 floats field t        \ how far open (0..1: 0.2 a frame)
  cell field stage
  cell field choice       \ 0 the first line, 1 the second
end-state

: flag-word ( n -- mask addr )  1 over 31 and lshift  swap 5 rshift 4 * progress pr.state + ;
: flag? ( n -- flag )  flag-word l@ and 0<> ;
: flag-on ( n -- )  flag-word dup l@ rot or swap l! ;
: flag-off ( n -- )  flag-word dup l@ rot invert and swap l! ;
: sound ( id -- )  5 bank-sound ;   \ (the driver's +0x14, bank 5)
: t+ ( F: d -- )  t f@ f+ 0e fmax 1e fmin t f! ;
: opened? ( -- flag )  t f@ 1e f>= ;
: shut? ( -- flag )  t f@ f0> 0= ;
: in-movie? ( -- flag )  stage @ movie-opening skipping 1+ within  stage @ movie-closing = or ;

\ ---- opening (SceneGame_SubPlay: Start, the world not held, pausing allowed) ----
: on-title? ( -- flag )  s" opening" actor-named 0< 0= ;   \ (the title and the opening: their own Start)
: may-pause? ( -- flag )
    start-button button-pressed?  on-title? 0= and  world-held flag? 0= and  no-pause flag? 0= and ;
: movie-wanted? ( -- flag )  cutscene-active? movie-status 1 = and ;
: play-wanted? ( -- flag )  cutscene-active? 0=  movie-status 1 <> and ;
: open-movie ( -- )  0e t f!  movie-opening stage !  1 movie-pause  $96 sound ;   \ Pause_Open, mode 1
: open-play ( -- )  0e t f!  opening stage !  0 choice !  1 pause!  $96 sound ;   \ mode 0: the world held
: shut ( -- )   \ back to play
    in-movie? 0= if  0 pause!  then  closed stage !  0e t f! ;

\ ---- the sound turned down as far as it is open (duck: the effects, the stream and the stage
\ music) ----
: duck ( -- )  1e  1e t f@ f-  sound-volume!   1e t f@ f- music-volume!  1e t f@ f- stage-music-volume! ;
: unduck ( -- )  1e 1e sound-volume!  1e stage-music-volume! ;   \ (the stream: its own again as the world goes on)

\ ---- choosing: up / down the first / second line (a sound when it changes) ----
: arrow ( k1 k2 -- flag )  key-pressed? swap key-pressed? or ;
: choose ( -- )
    choice @
    key: Up key: W arrow if  0 choice !  then
    key: Down key: S arrow if  1 choice !  then
    choice @ <> if  $2A sound  then ;

\ ---- the play pause, a frame of each stage (Pause_State*) ----
: play-frame ( -- )
    stage @ case
        opening of  0.2e t+  duck  opened? if  open stage !  then  endof
        open of
            start-button button-pressed? if  closing stage !  $97 sound  exit  then
            select-button button-pressed? if  0 choice !  on-menu stage !  $96 sound  then  endof
        closing of  -0.2e t+  duck  shut? if  unduck shut  then  endof
        on-menu of
            choose
            cross button-pressed? if
                choice @ 0= if  menu-closing stage !  $97 sound
                else  1 choice !  asking stage !  $96 sound  then
            then  endof
        menu-closing of  -0.2e t+  duck  shut? if  unduck shut  then  endof
        asking of
            choose
            cross button-pressed? if
                choice @ 0= if  0e t f!  quitting stage !  $97 sound
                else  open stage !  $96 sound  then  exit
            then
            triangle button-pressed? if  open stage !  $97 sound  then  endof
        quitting of  0.2e t+  opened? if  unduck  0 pause!  closed stage !  soft-reset  then  endof
    endcase ;

\ ---- the movie pause (MoviePause_State*) ----
: movie-frame ( -- )
    stage @ case
        movie-opening of  0.2e t+  opened? if  movie-open stage !  then  endof
        movie-open of
            triangle button-pressed? if  0e t f!  skipping stage !  $97 sound  exit  then   \ (Cancel: MoviePause_StateOpen)
            start-button button-pressed? if  0 movie-pause  movie-closing stage !  $97 sound  then  endof
        skipping of  0.2e t+  opened? if  movie-skipped flag-on  then  endof
        movie-closing of  -0.2e t+  shut? if  shut  then  endof
    endcase ;

: step ( -- )
    stage @ closed = if
        may-pause? 0= if  exit  then
        movie-wanted? if  open-movie exit  then
        play-wanted? if  open-play  then  exit
    then
    in-movie? if  movie-frame  else  play-frame  then ;

\ ---- drawing: the dimming (alpha 63 of 128 open; darkening to 255 skipping or quitting - here
\ twice the PS2's 128 scale), the lines centred at the PS2's heights (of 448), the chosen one
\ lit; all fading with it. The words scale with the screen's height (a pen of height / 224) ----
: darkening? ( -- flag )  stage @ skipping =  stage @ quitting = or ;
: dim ( -- a )  darkening? if  63e 192e t f@ f* f+  else  63e t f@ f*  then  f>s 2* 255 min ;
: alpha ( -- a )   \ the words' (fading out as it darkens)
    darkening? if  1e t f@ f-  else  t f@  then  255e f* f>s ;
variable pen
$D8D0C400 constant plain-colour  $F0C87800 constant lit-colour  $F0EBE000 constant title-colour
: text-of ( msg -- addr len )  message-layout drop  0 0 message-line ;
: words ( addr len x y rgb -- )  alpha or >r >r >r >r >r  pause-layer r> r> r> r> r> pen @ ui-text ;
: line ( msg y rgb -- )   \ a line centred on the screen's middle (256 of 512)
    {: msg y rgb :}  msg text-of  2dup pen @ text-width 2/ 256 sx swap -  y sy  rgb words ;
: centred ( msg y -- )  plain-colour line ;
: option ( msg y lit? -- )  if  lit-colour  else  plain-colour  then  line ;
: title ( msg y -- )  pen @ >r  r@ 2* pen !  title-colour line  r> pen ! ;
: play-lines ( -- )
    stage @ case
        opening of  $8088 $D0 title  endof
        open of  $8088 $D0 title  endof
        closing of  $8088 $D0 title  endof
        on-menu of  $8081 $C8 choice @ 0= option  $8082 $E8 choice @ 0<> option  endof
        menu-closing of  $8081 $C8 centred  $8082 $E8 centred  endof
        >r  $8083 $BC centred  $8084 $E0 choice @ 0= option  $8085 $F8 choice @ 0<> option  r>
    endcase ;
: draw ( -- )
    pause-layer ui-clear
    stage @ closed = if  exit  then
    frame!  screen-size nip 224 / 1 max pen !
    pause-layer  0 0 screen-size  dim  ui-rect
    in-movie? if  $8089 $180 centred  $808A $194 centred  else  play-lines  then ;

behaviour pausing
  on spawned ( -- )  closed stage !  self subscribe tick  self subscribe held-tick  self subscribe frame-end ;
  on tick ( -- )  step ;
  on held-tick ( -- )  stage @ closed <> if  step  draw  then ;   \ (the world held: its own frames)
  on frame-end ( -- )   \ (flag 6, back to play: the scene's script sets it to close the skip; cleared each frame)
      pause-wanted flag? stage @ skipping = and if  shut  then  pause-wanted flag-off  draw ;
end-behaviour

: pause-spawn ( -- id )  pausing pause-state s" pause" spawn ;
