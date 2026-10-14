\ opening.fs - the start of a game (docs/subsystems/story.md): the title, then the opening
\ movie, then the new game.
\ - The title (from new-src's title.fs): Hewie lying in the dark, close up and warmly lit, by
\   the menu - NEW GAME / LOAD GAME / OPTIONS, and once a game has been cleared HARD MODE (a new
\   game in hard mode: story flag $AF) and EXTRAS (straight into the extras room, $37); QUIT
\   (Up / Down, the cross; SceneTitle_SeqMenu). A new game: he gets up and barks.
\   Load Game: the save slots (as the save point's: their places and times), the cross loads one
\   in use, the triangle goes back.
\ - The opening movie, as the original's title plays it after New Game (SceneTitle_StateNewGame:
\   OPENING.SFD; the action or start button skips it).
\ - The new game's entry (SceneGame_StateEntry, entry $2A): the new game's sound set, the world
\   held, play started (state flags 3, 8, $28), Fiona in the slip she wakes in (costume
\   variable $26 = 1, Hewie's $27 = 0), room $2A - whose entering script does the rest.
\ Without a window (tests, screenshots) the game starts at once.
IN: opening
USING: engine game-state actors messages keys vectors room-names facts flag-names saves subscreen.art subscreen.saving ;

6 constant movie-layer   7 constant menu-layer   \ (over everything)

state: opening-state
  cell field stage       \ 0 the menu, 1 he gets up, 2 he barks, 3 the movie, 4 the game started, 5 the save slots
  cell field picked      \ the menu's item
  cell field slot-at     \ (the save slots) the one chosen
  cell field dog         \ the title's Hewie (a model of its own)
  cell field titled      \ the title is up (its lights and Hewie to put away)
end-state

: flag-on ( n -- )  progress pr.state over 5 rshift 4 * + dup l@ rot 31 and 1 swap lshift or swap l! ;
variable hard   \ the new game chosen is a hard one (the title's HARD MODE)
: new-game ( -- )   \ SceneGame_StateEntry, entry $2A (messages: from an actor or at load)
    new-game-sounds flag-on  world-held flag-on  in-play flag-on
    hard @ if  hard-mode-flag story-flag-set  then   \ (entry $4000002A: progress +0x30 bit 15)
    1 progress pr.vars $26 + c!  0 progress pr.vars $27 + c!
    1 s" fiona" actor-named send costume
    front-garden-3 -1 s" rooms" actor-named send go-to-room ;

: extras ( -- )   \ (SceneGame_StateEntry, entry $37: the extras room - Fiona in her clothes)
    world-held flag-on  in-play flag-on
    0 progress pr.vars $26 + c!  0 progress pr.vars $27 + c!
    0 s" fiona" actor-named send costume
    off-map-37 -1 s" rooms" actor-named send go-to-room ;

\ ---- the title: no room, just him in the dark, warm lights on him; the camera close, at an
\ angle round him, him a little right of centre (the menu on the left) ----
$002 constant m-lie          \ lying, settled (Hewie_StepToPose's pose 2)
$103 constant m-get-up       \ lying to standing (sPoseInto[0][2])
$1B00 constant m-bark        \ a bark, standing (Hewie_Bark)
$5A constant bark-sound       \ his calm bark, standing (Hewie_Bark's $1B00 at its key: Hewie_MakeSound $5A; $65 was his yelp)
: him ( -- a )  dog @ actor ;
: play ( anim loop? -- )   \ as new-src's title: the motion from its start, looped or once, nothing else
    0<> 1 and him act.loop l!  dog @ swap motion! ;
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
    s" O_HEW/HEW_000" actor-load dog !   \ (no motion table: as new-src's title, the motions played as they are)
    0e him act.x sf!  0e him act.y sf!  0e him act.z sf!  0e him act.yaw sf!  0e him act.shadow sf!
    1 him act.visible l!  m-lie -1 play  frame-him ;
: title-end ( -- )   \ (his model gone, the room's lights back)
    titled @ 0= if  exit  then  0 titled !
    dog @ actor-free  0 stage-lights  0.06e 0.06e 0.08e clear-color  menu-layer ui-clear ;

\ the menu, low on the left; the game's name above
\ the entries: the last two only once a game has been cleared (the first clear's unlock)
0 constant e-new   1 constant e-load   2 constant e-options   3 constant e-hard   4 constant e-extras   5 constant e-quit
: cleared? ( -- flag )  0 resident-flag? ;
: entry ( i -- e )  dup 3 >= cleared? 0= and if  drop e-quit  then ;
: #items ( -- n )  cleared? if  6  else  4  then ;
: item ( i -- addr len )
    entry case  e-new of  s" NEW GAME"  endof  e-load of  s" LOAD GAME"  endof  e-options of  s" OPTIONS"  endof
                e-hard of  s" HARD MODE"  endof  e-extras of  s" EXTRAS"  endof  >r s" QUIT" r>  endcase ;
variable sw  variable sh
\ the save slots, to load one: each its number, place and time played
: slot-colour ( i -- rgba )  dup slot-at @ = if  drop $FFFFFFFF  else  slot-used? if  $B0A8A0FF  else  $605C58FF  then  then ;
: slots-draw ( -- )
    screen-size sh ! sw !  3 pen-scale
    menu-layer  s" LOAD GAME"  sw @ 10 /  sh @ 8 /  $D8D0C8FF 3 ui-text
    2 pen-scale
    #slots 0 do
        sh @ 4 / i char-size nip 2* * +  {: y :}
        menu-layer  i 1+ n>s  sw @ 10 /  y  i slot-colour 2 ui-text
        i slot-used? if
            menu-layer  i slot-place@ menu-line  sw @ 10 / 3 char-size drop * 2* +  y  i slot-colour 2 ui-text
            menu-layer  i slot-frames@ clock$  sw @ 6 * 10 /  y  i slot-colour 2 ui-text
        else
            menu-layer  s" - empty -"  sw @ 10 / 3 char-size drop * 2* +  y  i slot-colour 2 ui-text
        then
    loop
    menu-layer  s" >"  sw @ 10 / char-size drop 2* 2* -  sh @ 4 / slot-at @ char-size nip 2* * +  $FFFFFFFF 2 ui-text ;
: menu-draw ( -- )
    menu-layer ui-clear
    stage @ 5 = if  slots-draw exit  then
    stage @ 0<> if  exit  then
    screen-size sh ! sw !  4 pen-scale
    menu-layer  s" HAUNTING GROUND"  sw @ 2/ 15 char-size drop * 2/ -  sh @ 8 /  $D8D0C8FF 4 ui-text
    3 pen-scale
    #items 0 do
        menu-layer  i item  sw @ 10 /  sh @ 7 * 10 / i char-size nip 3 2 */ * +
        i picked @ = if  $FFFFFFFF  else  $8C8880FF  then  3 ui-text
    loop
    menu-layer  s" >"  sw @ 10 / char-size drop 2* -  sh @ 7 * 10 / picked @ char-size nip 3 2 */ * +  $FFFFFFFF 3 ui-text ;
: menu-keys ( -- )
    key: Up key-pressed? if  picked @ 1- 0 max picked !  then
    key: Down key-pressed? if  picked @ 1+ #items 1- min picked !  then
    cross button-pressed? if   \ (confirm: cross)
        picked @ entry case
            e-new of  0 hard !  m-get-up 0 play  1 stage !  endof
            e-hard of  -1 hard !  m-get-up 0 play  1 stage !  endof
            e-load of  0 slot-at !  5 stage !  endof
            e-options of  5 s" subscreen" actor-named send menu-mode  subscreen-wanted flag-on  6 stage !  endof
            e-extras of  title-end  4 stage !  extras  self kill  endof
            >r bye r>
        endcase
    then ;
: load-it ( -- )   \ (the slot's game: the title away, the saves actor puts it back)
    title-end  4 stage !  slot-at @ s" saves" actor-named send load-from-slot  self kill ;
: slots-keys ( -- )
    key: Up key-pressed? if  slot-at @ 1- #slots + #slots mod slot-at !  then
    key: Down key-pressed? if  slot-at @ 1+ #slots mod slot-at !  then
    cross button-pressed? slot-at @ slot-used? and if  load-it exit  then
    triangle button-pressed? if  0 stage !  then ;

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
          5 of  slots-keys  endof
          6 of  subscreen-wanted state-flag? 0= if  0 stage !  then  endof   \ (the options up: back to the menu once they close)
          1 of  ended? if  m-bark 0 play  bark-sound common-sound  2 stage !  then  endof
          2 of  ended? if  movie-start  then  endof
          3 of  movie-status 1 <>  start-button button-pressed? or if  movie-over  then  endof   \ (SceneTitle: Start skips)
      endcase ;
  on frame-end ( -- )  stage @ 3 <  stage @ 5 = or  stage @ 6 = or if  frame-him  then  menu-draw  movie-draw ;
  on killed ( -- )   \ (free play: the title or the movie cut short)
      stage @ 3 <  stage @ 4 > or if  title-end  then  stage @ 3 = if  movie-close  movie-layer ui-clear  then ;
end-behaviour

: opening-spawn ( -- id )  opening-the-game opening-state s" opening" spawn ;
