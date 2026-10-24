\ pause.fs - the pause screen (docs/subsystems/pause.md; the original's SceneGame +0x73EBA0,
\ src/game/pause.c). Two kinds:
\ - in play, Start: the world held, the screen dimmed, "PAUSE"; Start goes back, Select the menu
\   - "back to the game" / "quit"; quitting asks "yes / no", then darkens to the title. The
\   sound is turned down as far as the screen is open.
\ - while a scene's movie plays, Start: the movie paused, "Unpause game" / "Skip"; Start goes
\   back, Cancel skips - the screen darkens to black and movie-skipped is set for the scene's
\   script, which ends the scene and closes it (pause-wanted).
IN: pause
USING: engine game-state actors messages keys facts flag-names ps2-screen ;

5 constant pause-layer   \ (the UI layers: 0 the movie, 1 the fade, 2 the window, 3 the prompt, 4 subtitles, 5 this)

\ its stages: closed; the movie pause's; the play pause's
enum: pause-stage  closed
    movie-opening movie-open skipping movie-closing
    opening open closing on-menu menu-closing asking quitting ;

\ its sounds (the common bank) and lines (system messages)
$96 constant open-sound   $97 constant back-sound   $2A constant move-sound
$8088 constant pause-msg                                  \ "PAUSE"
$8081 constant back-to-game-msg   $8082 constant quit-msg   \ the menu
$8083 constant exit-to-title-msg   $8084 constant yes-msg   $8085 constant no-msg
$8089 constant unpause-msg   $808A constant skip-msg      \ the movie pause

state: pause-state
  1 floats field t        \ how far open (0..1: 0.2 a frame)
  cell field stage
  cell field choice       \ 0 the first line, 1 the second
end-state

: sound ( id -- )  5 bank-sound ;   \ (the driver's +0x14, bank 5)
: t+ ( F: d -- )  t f@ f+ 0e fmax 1e fmin t f! ;
: opened? ( -- flag )  t f@ 1e f>= ;
: shut? ( -- flag )  t f@ f0> 0= ;
: in-movie? ( -- flag )  stage @ movie-opening skipping 1+ within  stage @ movie-closing = or ;

\ ---- opening (SceneGame_SubPlay: Start, the world not held, pausing allowed) ----
: on-title? ( -- flag )  s" opening" actor-named 0< 0= ;   \ (the title and the opening: their own Start)
: may-pause? ( -- flag )
    start-button button-pressed?  on-title? 0= and  world-held state-flag? 0= and  no-pause state-flag? 0= and ;
: movie-wanted? ( -- flag )  cutscene-active? movie-status 1 = and ;
: play-wanted? ( -- flag )  cutscene-active? 0=  movie-status 1 <> and ;
: open-movie ( -- )  0e t f!  movie-opening stage !  1 movie-pause  open-sound sound ;   \ Pause_Open, mode 1
: open-play ( -- )  0e t f!  opening stage !  0 choice !  1 pause!  open-sound sound ;   \ mode 0: the world held
: shut ( -- )   \ back to play
    in-movie? 0= if  0 pause!  then  closed stage !  0e t f! ;

\ ---- the sound turned down as far as it is open (duck: the effects' master, the stream, the
\ stage music and the movie) ----
: music-duck! ( F: v -- )  f>cell s" music" actor-named send music-duck ;
: duck-by ( F: down -- )   \ (pause.c duck)
    1e fswap f- 0e fmax 1e fmin  {: F: v :}
    1e v sound-volume!  v music-duck!  v stage-music-volume!  v movie-volume! ;
: duck ( -- )  t f@ duck-by ;
: unduck ( -- )  0e duck-by ;

\ ---- choosing: up / down the first / second line (a sound when it changes) ----
: arrow ( k1 k2 -- flag )  key-pressed? swap key-pressed? or ;
: choose ( -- )
    choice @
    key: Up key: W arrow if  0 choice !  then
    key: Down key: S arrow if  1 choice !  then
    choice @ <> if  move-sound sound  then ;

\ ---- the play pause, a frame of each stage (Pause_State*) ----
: play-frame ( -- )
    stage @ case
        opening of  0.2e t+  duck  opened? if  open stage !  then  endof
        open of
            start-button button-pressed? if  closing stage !  back-sound sound  exit  then
            select-button button-pressed? if  0 choice !  on-menu stage !  open-sound sound  then  endof
        closing of  -0.2e t+  duck  shut? if  unduck shut  then  endof
        on-menu of
            choose
            cross button-pressed? if
                choice @ 0= if  menu-closing stage !  back-sound sound
                else  1 choice !  asking stage !  open-sound sound  then
            then  endof
        menu-closing of  -0.2e t+  duck  shut? if  unduck shut  then  endof
        asking of
            choose
            cross button-pressed? if
                choice @ 0= if  0e t f!  quitting stage !  back-sound sound
                else  open stage !  open-sound sound  then  exit
            then
            triangle button-pressed? if  open stage !  back-sound sound  then  endof
        quitting of  0.2e t+  opened? if  unduck  0 pause!  closed stage !  soft-reset  then  endof
    endcase ;

\ ---- the movie pause (MoviePause_State*) ----
: movie-frame ( -- )
    stage @ case
        movie-opening of  0.2e t+  duck  opened? if  movie-open stage !  then  endof
        movie-open of
            triangle button-pressed? if  0e t f!  skipping stage !  back-sound sound  exit  then   \ (Cancel: MoviePause_StateOpen)
            start-button button-pressed? if  0 movie-pause  movie-closing stage !  back-sound sound  then  endof
        skipping of  0.2e t+  1e t f@ f- duck-by  opened? if  movie-skipped state-flag-set  then  endof   \ (the sound back as it darkens)
        movie-closing of  -0.2e t+  duck  shut? if  shut  then  endof
    endcase ;

: step ( -- )
    stage @ closed = if
        may-pause? 0= if  exit  then
        movie-wanted? if  open-movie exit  then
        play-wanted? if  open-play  then  exit
    then
    in-movie? if  movie-frame  else  play-frame  then ;

\ ---- drawing (pause.c): the dimming (alpha 63 of 128 open; darkening to 255 skipping or
\ quitting - here twice the PS2's 128 scale); the lines in the game's font at its size (a pen of
\ height / 224: its 16 x 21 glyphs of 448 lines) and their colours, centred on x 256 of the
\ PS2's 512 - "PAUSE" and the line chosen in a highlight (GAME_FIX's texture 2 box, palette 5,
\ twice the line's width, 32 high); the movie pause's from x 320; all fading with it ----
: darkening? ( -- flag )  stage @ skipping =  stage @ quitting = or ;
: dim ( -- a )  darkening? if  63e 192e t f@ f* f+  else  63e t f@ f*  then  f>s 2* 255 min ;
: alpha ( -- a )   \ the words' (fading out as it darkens)
    darkening? if  1e t f@ f-  else  t f@  then  255e f* f>s ;
variable pen
$D8D0C400 constant plain-colour   \ (the font's own, palette 0: its white tinted as the window's)
: text-of ( msg -- addr len )  message-layout drop  0 0 message-line ;
: width ( msg -- w )   \ (Task_MessageWidth: in the PS2's units)
    text-of pen @ text-width  512 *  ps2-screen:fw @ 1 max / ;
: line-at ( msg x y -- )   \ its first line from x, y (the PS2's), run by run of a colour
    {: msg x y | col :}  msg message-layout drop
    begin  0 0 col 0 message-run  {: a n c :}  n 0>  while
        pause-layer a n  x sx  0 0 message-line drop col pen @ text-width +  y sy
        c if  $FFFFFF00  else  plain-colour  then  alpha or  pen @ c ui-text-in
        col n + to col
    repeat ;
: centred ( msg y -- )  {: msg y :}  msg  256 msg width 2/ -  y line-at ;
: highlight ( msg y -- )   \ (highlight_w)
    {: msg y :}  msg width {: w :}
    pause-layer  256 w - sx  y sy  w 2* sw  $20 sy  $10 2 5  $30 $20 $20 $20  $FFFFFF00 alpha or  ui-image ;
: chosen ( first second y0 step -- )   \ the highlight on the line chosen
    {: a b y0 step :}  choice @ if  b  else  a  then  y0 choice @ step * +  highlight ;
: play-lines ( -- )
    stage @ case
        opening of  pause-msg $D2 highlight  pause-msg $D6 centred  endof
        open of  pause-msg $D2 highlight  pause-msg $D6 centred  endof
        closing of  pause-msg $D2 highlight  pause-msg $D6 centred  endof
        on-menu of  back-to-game-msg quit-msg $C4 $20 chosen  back-to-game-msg $C8 centred  quit-msg $E8 centred  endof
        menu-closing of  back-to-game-msg quit-msg $C4 $20 chosen  back-to-game-msg $C8 centred  quit-msg $E8 centred  endof
        >r  exit-to-title-msg $C8 centred  yes-msg no-msg $E4 $14 chosen  yes-msg $E8 centred  no-msg $FC centred  r>   \ (asking, quitting)
    endcase ;
: draw ( -- )
    pause-layer ui-clear
    stage @ closed = if  exit  then
    frame!  screen-size nip 224 / 1 max pen !
    pause-layer  0 0 screen-size  dim  ui-rect
    in-movie? if  unpause-msg $140 $190 line-at  skip-msg $140 $1A4 line-at  else  play-lines  then ;

behaviour pausing
  on spawned ( -- )  closed stage !  self subscribe tick  self subscribe held-tick  self subscribe frame-end ;
  on tick ( -- )  step ;
  on held-tick ( -- )  stage @ closed <> if  step  draw  then ;   \ (the world held: its own frames)
  on frame-end ( -- )   \ (flag 6, back to play: the scene's script sets it to close the skip; cleared each frame)
      pause-wanted state-flag? stage @ skipping = and if  shut  then  pause-wanted state-flag-clear  draw ;
end-behaviour

: pause-spawn ( -- id )  pausing pause-state s" pause" spawn ;
