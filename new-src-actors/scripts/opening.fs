\ opening.fs - the start of a game (docs/subsystems/story.md): the title, then the opening
\ movie, then the new game.
\ - The title (from new-src's title.fs): Hewie lying in the dark, close up and warmly lit, by
\   the menu - NEW GAME / QUIT (Up / Down, the action button). New Game: he gets up and barks.
\ - The opening movie, as the original's title plays it after New Game (SceneTitle_StateNewGame:
\   OPENING.SFD; the action or start button skips it).
\ - The new game's entry (SceneGame_StateEntry, entry $2A): the new game's sound set, the world
\   held, play started (state flags 3, 8, $28), Fiona in the slip she wakes in (costume
\   variable $26 = 1, Hewie's $27 = 0), room $2A - whose entering script does the rest.
\ Without a window (tests, screenshots) the game starts at once.
IN: opening
USING: engine game-state actors messages keys vectors room-names flag-names ;

6 constant movie-layer   7 constant menu-layer   \ (over everything)

state: opening-state
  cell field stage       \ 0 the menu, 1 he gets up, 2 he barks, 3 the movie, 4 the game started
  cell field picked      \ the menu's item
  cell field dog         \ the title's Hewie (a model of its own)
  cell field titled      \ the title is up (its lights and Hewie to put away)
end-state

: flag-on ( n -- )  progress pr.state over 5 rshift 4 * + dup l@ rot 31 and 1 swap lshift or swap l! ;
: new-game ( -- )   \ SceneGame_StateEntry, entry $2A (messages: from an actor or at load)
    new-game-sounds flag-on  world-held flag-on  in-play flag-on
    1 progress pr.vars $26 + c!  0 progress pr.vars $27 + c!
    1 s" fiona" actor-named send costume
    front-garden-3 -1 s" rooms" actor-named send go-to-room ;

\ ---- the title: no room, just him in the dark, warm lights on him; the camera close, at an
\ angle round him, him a little right of centre (the menu on the left) ----
$002 constant m-lie          \ lying, settled (Hewie_StepToPose's pose 2)
$103 constant m-get-up       \ lying to standing (sPoseInto[0][2])
$1B00 constant m-bark        \ a bark, standing (Hewie_Bark)
$65 constant bark-sound      \ the common bank's loud bark (Hewie_MakeSound)
: him ( -- a )  dog @ actor ;
: play ( anim loop? -- )   \ with his table's fade, looped or once (the title's own: new-src's)
    >r  dog @ swap  2dup motion-entry nip  1 invert and  r> 1 and or  motion-play ;
: warm-lights ( -- )
    \ the key: a warm lamp-light, high in front of him to the side; behind, a low orange glow
    -10e 22e 26e   170e 115e 65e   160e  0 stage-light
     24e 18e -20e   60e 35e 18e    120e  1 stage-light
    2 stage-lights  10e 8e 6e stage-ambient ;
fvariable tx  fvariable ty  fvariable tz
: look-at ( F: x y z -- )   \ (the camera turned to look there)
    tz f! ty f! tx f!
    tx f@ camera cam.x sf@ f-  tz f@ camera cam.z sf@ f- fnegate
    fover fover fatan2 camera cam.yaw sf!
    fsq fswap fsq f+ fsqrt  ty f@ camera cam.y sf@ f- fswap fatan2 camera cam.pitch sf! ;
: frame-him ( -- )   \ the camera 20 away, 0.9 round from his front, 7 up, looking at his chest
    0.9e fdup fsin 20e f*  fswap fcos 20e f*                  ( F: cx cz )
    fswap 7e frot cam-at
    0e 3e 0e look-at
    camera cam.yaw sf@ -0.25e f+ camera cam.yaw sf!  0.55e camera cam.fov sf! ;
: title-start ( -- )
    0 stage !  0 picked !  -1 titled !
    0e 0e 0e clear-color  warm-lights
    s" O_HEW/HEW_000" actor-load dup dog !  $3D5F90 motion-table
    0e him act.x sf!  0e him act.y sf!  0e him act.z sf!  0e him act.yaw sf!  0e him act.shadow sf!
    1 him act.visible l!  m-lie -1 play  frame-him ;
: title-end ( -- )   \ (his model gone, the room's lights back)
    titled @ 0= if  exit  then  0 titled !
    dog @ actor-free  0 stage-lights  0.06e 0.06e 0.08e clear-color  menu-layer ui-clear ;

\ the menu, low on the left; the game's name above
: item ( i -- addr len )  0= if  s" NEW GAME"  else  s" QUIT"  then ;
variable sw  variable sh
: menu-draw ( -- )
    menu-layer ui-clear
    stage @ 0<> if  exit  then
    screen-size sh ! sw !  4 pen-scale
    menu-layer  s" HAUNTING GROUND"  sw @ 2/ 15 char-size drop * 2/ -  sh @ 8 /  $D8D0C8FF 4 ui-text
    3 pen-scale
    2 0 do
        menu-layer  i item  sw @ 10 /  sh @ 7 * 10 / i char-size nip 3 2 */ * +
        i picked @ = if  $FFFFFFFF  else  $8C8880FF  then  3 ui-text
    loop
    menu-layer  s" >"  sw @ 10 / char-size drop 2* -  sh @ 7 * 10 / picked @ char-size nip 3 2 */ * +  $FFFFFFFF 3 ui-text ;
: menu-keys ( -- )
    key: Up key-pressed? if  picked @ 1- 0 max picked !  then
    key: Down key-pressed? if  picked @ 1+ 1 min picked !  then
    circle button-pressed? if  picked @ 0= if  m-get-up 0 play  1 stage !  else  bye  then  then ;

\ ---- the opening movie over the whole window, 4:3 in the middle, black round it ----
variable mx  variable my  variable mw  variable mh
: movie-draw ( -- )
    movie-layer ui-clear
    stage @ 3 <> if  exit  then
    screen-size  over 3 * 4 / over min  dup mh !  4 * 3 / mw !   mh @ - 2/ my !  mw @ - 2/ mx !
    movie-layer  0 0 screen-size  $000000FF ui-rect
    movie-layer  mx @ my @ mw @ mh @ ui-movie ;
: movie-start ( -- )   \ (none to play: straight into the game)
    title-end  3 stage !
    s" OPENING.SFD" movie-open 0= if  4 stage !  new-game  self kill  then ;
: movie-over ( -- )  movie-close  movie-layer ui-clear  4 stage !  new-game  self kill ;   \ (stage 4 now: its last words leave it be)
: ended? ( -- flag )  dog @ motion-done? ;   \ (played once, at its last frame)

behaviour opening-the-game
  on spawned ( -- )
      self subscribe tick  self subscribe frame-end
      hidden? if  4 stage !  self kill  else  title-start  then ;   \ (headless: game.fs started it)
  on tick ( -- )
      stage @ case
          0 of  menu-keys  endof
          1 of  ended? if  m-bark 0 play  bark-sound common-sound  2 stage !  then  endof
          2 of  ended? if  movie-start  then  endof
          3 of  movie-status 1 <>  circle button-pressed? or  start-button button-pressed? or if  movie-over  then  endof
      endcase ;
  on frame-end ( -- )  stage @ 3 < if  frame-him  then  menu-draw  movie-draw ;
  on killed ( -- )   \ (free play: the title or the movie cut short)
      stage @ 3 < if  title-end  then  stage @ 3 = if  movie-close  movie-layer ui-clear  then ;
end-behaviour

: opening-spawn ( -- id )  opening-the-game opening-state s" opening" spawn ;
