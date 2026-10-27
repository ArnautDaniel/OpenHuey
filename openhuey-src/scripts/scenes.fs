\ scenes.fs - a room's scene: its movie, the director moving the characters' models through its
\ shots, its subtitles (docs/rooms-design.md). A place asks for one to be made ready, does what
\ it must while the screen is black, then has it run; it is told each new shot and, at the end -
\ played through or skipped - that it is over, the world held, the camera given back.
\ (The original's scenes: the event scripts' idiom 0x60 / 0x61 / 0x62, src/game/director.c.)
IN: scenes
USING: engine game-state actors messages facts flag-names story.strings ;

\ how the movie's picture shows (Progress_PlayMovie's kinds): not drawn - only the scene's timing
\ and sound; keyed by its brightness; opaque only while movie-opaque; by its own alpha; not
\ drawn here; opaque; a strip
enum: movie-class  unseen-movie keyed-movie opaque-when-movie alpha-movie unseen-4-movie opaque-movie strip-movie ;
: class>compo ( class -- compo )   \ (Sofdec's composition: luminance key, alpha, three-level, opaque)
    case  keyed-movie of $31 endof  alpha-movie of $21 endof  strip-movie of $41 endof  $11 swap  endcase ;
1 constant movie-running   2 constant movie-ended   \ (movie-status)
2 constant director-ready  4 constant director-ending  5 constant director-over   \ (cutscene-status)
$B constant opaque-signal  8 constant page-signal   \ (the director's signals)
15 constant scene-fade   \ (frames: the fade in as it starts, out as it nears its end)

enum: scene-step  no-scene readying ready running ending ;
state: scenes-state
  cell field step
  cell field asked-by      \ the place it is for
  cell field class         \ the movie's
  cell field subtitles     \ the message shown as its subtitles (-1 none)
  cell field page          \ the page of it shown (-1 none yet)
  cell field cue  cell field cue-was
  cell field shot-was      \ the last shot told
  cell field fading
  cell field movie-up      \ a movie is open
  cell field through       \ played through: not skipped, its last frame left up (the game overs' movies)
end-state

\ ---- the movie ----
: movie-state ( -- n )  movie-status movie-running <> if  -1  else  movie-paused? if  2  else  1  then  then ;   \ (-1 none, 1 running, 2 paused)
: screen-actor ( -- id )  s" screen" actor-named ;
: window-actor ( -- id )  s" window" actor-named ;
: fade-to ( kind -- )  -1 fading !  scene-fade swap screen-actor send fade ;

\ ---- the steps ----
: asker ( -- id | -1 )  asked-by @ dup alive? 0= if  drop -1  then ;   \ (the place that asked, still there: else none - gone with its room)
: tell ( -- )   \ (the place told of each new shot)
    cutscene-frame cutscene-shot-at  dup shot-was @ = if  drop exit  then
    dup shot-was !  asker dup 0< if  2drop  else  send scene-shot  then ;
: cue-step ( -- )   \ (the director kept with the movie's frame; its signal flips movie-opaque)
    cue @ cue-was !  movie-frame cue !  cue @ cutscene-frame!
    opaque-signal cutscene-signals 1 and if
        movie-opaque state-flag? if  movie-opaque state-flag-clear  else  movie-opaque state-flag-set  then
    then ;
: run-frame ( -- over? )
    movie-skipped state-flag?  through @ 0= and if  true exit  then
    cue-step  cutscene-near? if  screen:to-black fade-to  then
    page-signal cutscene-signals 0 ?do  1 page +!  loop
    cutscene-run  cutscene-status director-over <>  cutscene-in-shot? and if  cutscene-update  then
    tell
    movie-state 0<  cutscene-status dup director-over = swap director-ending = or  or ;
: scene-ends ( -- )   \ (the end: black, the movie stopped, the world held, the camera back)
    $FFFF window-actor send close-text  -1 subtitles !
    black-frame  movie-close  0 movie-up !  world-held state-flag-set
    pause-wanted state-flag-set
    false cam-scene!  cutscene-end  movie-opaque state-flag-clear  0 broadcast scene
    no-scene step !  asker dup 0< if  drop  else  send scene-over  then ;
: through-ends ( -- )   \ (played through: its subtitles gone, the movie and the director left as they are)
    $FFFF window-actor send close-text  -1 subtitles !  0 through !
    no-scene step !  asker dup 0< if  drop  else  send scene-over  then ;
: stop-now ( -- )   \ (the place gone: nothing told)
    movie-up @ if  movie-close  0 movie-up !  then
    cutscene-active? if  false cam-scene!  cutscene-end  movie-opaque state-flag-clear  0 broadcast scene  then
    -1 subtitles !  no-scene step ! ;

\ ---- drawing: the movie on layer 0, the subtitles' page on layer 4 ----
4 constant subtitle-layer
variable mx  variable my  variable mw  variable mh
: movie-area ( -- )   \ (the 4:3 picture in the window)
    screen-size  over 3 * 4 / over min  dup mh !  4 * 3 / mw !
    mh @ - 2/ my !  mw @ - 2/ mx ! ;
: shown? ( -- flag )  movie-up @  movie-status movie-running = and ;
: draw-movie ( -- )
    0 ui-clear
    shown? 0= if  exit  then
    class @ dup unseen-movie = swap unseen-4-movie = or if  exit  then
    class @ opaque-when-movie = movie-opaque state-flag? 0= and if  exit  then
    movie-area
    class @ strip-movie = if
        0  mx @ mw @ 64 * 512 / +  my @ mh @ 176 * 448 / +  mw @ 384 * 512 /  mh @ 96 * 448 /  ui-movie  exit
    then
    class @ dup keyed-movie <> swap alpha-movie <> and if  0  0 0 screen-size  $000000FF ui-rect  then
    0  mx @ my @ mw @ mh @ ui-movie ;
variable sw  variable sh  variable lh
: draw-subtitles ( -- )
    subtitle-layer ui-clear
    subtitles @ 0<  page @ 0< or if  exit  then
    subtitles @ message-layout page @ > 0= if  exit  then
    screen-size sh ! sw !  2 pen-scale  char-size nip 5 * 4 / lh !
    page @ message-lines 0 ?do
        subtitle-layer  page @ i message-line  sw @ 8 /  sh @ 7 * 8 / 8 +  i lh @ * +  $FFFFFFFF 2 ui-text
    loop ;

: run-from ( subtitles through? -- )   \ faded in, a frame at a time until the movie or the director ends (or it is skipped)
    through !  dup $FFFF = if  drop -1  then  subtitles !  -1 page !
    -1 cue-was !  -1 cue !  cutscene-go  cutscene-run
    1e movie-volume!  screen:from-black fade-to  world-held state-flag-clear  0 movie-pause   \ (coming in: the world let go - it may be paused, and skipped)
    movie-skipped state-flag-clear  running step ! ;
behaviour scening
  on spawned ( -- )  self subscribe tick  self subscribe frame-end  -1 subtitles !  no-scene step ! ;
  on scene-load ( room movie class cutscene -- )   \ made ready: its movie open and paused, the director loaded
      {: room movie cls cut :}  sender asked-by !  cls class !  -1 shot-was !
      movie room room-string movie-open movie-up !  1 movie-pause  cls class>compo $10 $20 movie-compose
      cut room room-string cutscene-load drop  -1 broadcast scene
      readying step ! ;
  on scene-run ( subtitles -- )  false run-from ;   \ played (or skipped)
  on scene-run-through ( subtitles -- )  true run-from ;   \ played through, its last frame left up
  on scene-stop ( -- )  stop-now ;
  on fade-done ( -- )  0 fading ! ;
  on tick ( -- )
      step @ case
          readying of  cutscene-run  cutscene-status director-ready = if
                           ready step !  movie-up @ asker dup 0< if  2drop  else  send scene-ready  then  then  endof
          running of  run-frame if  ending step !  then  endof
          ending of  through @ if  through-ends  else  scene-ends  then  endof
      endcase ;
  on frame-end ( -- )  draw-movie  draw-subtitles ;
end-behaviour

: scenes-spawn ( -- id )  scening scenes-state s" scenes" spawn ;
