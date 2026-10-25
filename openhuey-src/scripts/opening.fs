\ opening.fs - the start of a game (docs/subsystems/story.md): the title, then the opening
\ movie, then the new game.
\ - The title: Hewie lying on the stone circle in the front garden (room $2D, its scenery only), by
\   the menu - NEW GAME / LOAD GAME / OPTIONS, and once a game has been cleared HARD MODE (a new
\   game in hard mode: story flag $AF) and EXTRAS (straight into the extras room, $37); QUIT
\   (Up / Down, the cross; SceneTitle_SeqMenu). OpenHuey's own: MINI-GAME (with EXTRAS), MODS (when
\   there are mods), UPDATE (a release build's: updater.fs), and mods' entries (title-menu-hook).
\   A new game: he gets up and barks.
\   Load Game: the save slots (as the save point's: their places and times), the cross loads one
\   in use, the triangle goes back.
\ - The opening movie, as the original's title plays it after New Game (SceneTitle_StateNewGame:
\   OPENING.SFD; the action or start button skips it).
\ - The new game's entry (SceneGame_StateEntry, entry $2A): the new game's sound set, the world
\   held, play started (state flags 3, 8, $28), Fiona in the slip she wakes in (costume
\   variable $26 = 1, Hewie's $27 = 0), room $2A - whose entering script does the rest.
\ Without a window (tests, screenshots) the game starts at once.
IN: opening
USING: engine game-state actors messages keys vectors room-names facts flag-names ps2-screen saves subscreen.art subscreen.saving mods.menu updater ;

6 constant movie-layer   7 constant menu-layer   \ (over everything)

\ its stages: the menu; he gets up; he barks; the opening movie; the game started; the save slots;
\ the options up; the mods menu; the updater
enum: opening-stage  on-menu getting-up barking opening-movie started on-slots on-options on-mods on-update ;

state: opening-state
  cell field stage       \ (enum: opening-stage)
  cell field picked      \ the menu's item
  cell field slot-at     \ (the save slots) the one chosen
  cell field dog         \ the title's Hewie (a model of its own)
  cell field titled      \ the title is up (its lights and Hewie to put away)
end-state

variable hard   \ the new game chosen is a hard one (the title's HARD MODE; new-game runs after the title has gone)
\ the story's variables for the costumes (Fiona's, Hewie's): 1 her slip
: costume-vars! ( fiona hewie -- )  progress pr.vars $27 + c!  progress pr.vars $26 + c! ;
: flag-on ( n -- )  state-flag-set ;
: new-game ( -- )   \ SceneGame_StateEntry, entry $2A (messages: from an actor or at load)
    0 hewie-played !   \ (SceneGame_ctor: Fiona the one played)
    new-game-sounds flag-on  world-held flag-on  in-play flag-on
    1 sound-set!   \ (Progress_LoadSoundSet by new-game-sounds: her bare feet's set)
    hard @ if  hard-mode-flag story-flag-set  then   \ (entry $4000002A: progress +0x30 bit 15)
    1 0 costume-vars!
    1 s" fiona" actor-named send costume
    front-garden-3 -1 s" rooms" actor-named send go-to-room ;

: extras ( -- )   \ (SceneGame_StateEntry, entry $37: the extras room - Fiona in her clothes)
    0 hewie-played !
    world-held flag-on  in-play flag-on
    0 sound-set!   \ (new-game-sounds off: the boots' set)
    0 0 costume-vars!
    0 s" fiona" actor-named send costume
    off-map-37 -1 s" rooms" actor-named send go-to-room ;

\ ---- the title: Hewie lying on the stone circle in the front garden ($2D - only its scenery: no
\ story, nobody else) in its own light; the camera back far enough for all of him, the scarecrows
\ behind him, him three-quarters to it a little right of centre (the menu on the left) ----
$002 constant m-lie          \ lying, settled (Hewie_StepToPose's pose 2)
$103 constant m-get-up       \ lying to standing (sPoseInto[0][2])
$1B00 constant m-bark        \ a bark, standing (Hewie_Bark)
$5A constant bark-sound       \ his calm bark, standing (Hewie_Bark's $1B00 at its key: Hewie_MakeSound $5A; $65 was his yelp)
: him ( -- a )  dog @ actor ;
: play ( anim loop? -- )   \ as new-src's title: the motion from its start, looped or once, nothing else
    0<> 1 and him act.loop l!  dog @ swap motion! ;
create spot  -258e f, -10e f, 174e f,   \ (on the circle, between the scarecrows)
: spot@ ( F: -- x y z )  spot f@  spot float+ f@  spot 2 floats + f@ ;
1.75e fconstant round   \ (from his front: the scarecrows behind him)
: frame-him ( -- )   \ the camera 36 away, 13 up, looking a little over his back; turned so he
    \ sits right of centre
    spot@ {: F: x y z :}
    x round fsin 36e f* f+  y 13e f+  z round fcos 36e f* f+   ( F: eye )
    x  y 4e f+  z  cam-place
    camera cam.yaw sf@ -0.22e f+ camera cam.yaw sf! ;
: title-start ( -- )
    0 stage !  0 picked !  -1 titled !
    front-garden-5 room  cam-new-room  cam-room-start   \ (the scenery: no `arrived` - no story, nobody else)
    s" O_HEW/HEW_000" actor-load dog !   \ (no motion table: as new-src's title, the motions played as they are)
    spot@ him act.z sf!  him act.y sf!  him act.x sf!  round 0.65e f- him act.yaw sf!  1e him act.shadow sf!   \ (three-quarters to the camera)
    1 him act.visible l!  m-lie -1 play  frame-him ;
: title-end ( -- )   \ (his model gone, the room's lights back)
    titled @ 0= if  exit  then  0 titled !
    dog @ actor-free  0 stage-lights  0.06e 0.06e 0.08e clear-color  menu-layer ui-clear ;
: title-gone ( -- )  title-end  started stage !  self kill ;   \ (leaving the title for the game: a mod's entry)

\ the menu, low on the left; the game's name above
\ the entries: the last two only once a game has been cleared (the first clear's unlock)
\ (MINI-GAME, hga's own: straight into the extras room's mini-game - its menu up, as at its door)
\ (MODS, hga's own: only when there are mods in mods/ - the mods menu, mods/menu.fs)
\ (UPDATE, OpenHuey's own: a release build's only - updater.fs)
enum: menu-entry  e-new e-load e-options e-hard e-extras e-mini e-mods e-update e-quit e-mod ;
: cleared? ( -- flag )  0 resident-flag? ;
create entries 16 cells allot   variable #entries
: +entry ( e -- )  #entries @ 16 < if  entries #entries @ cells + !  1 #entries +!  else  drop  then ;

\ ---- the hook (docs/library-and-mods.md): mods' own entries, after the game's (before MODS, UPDATE and
\ QUIT). A mod sets its own with `is`, calls what it replaced (`action-of title-menu-hook`), and
\ adds its entries with `title-entry`: a name, and what picking it does (run by the title's actor:
\ `title-gone` first if it leaves the title) ----
8 constant most-mod-entries
create mod-names most-mod-entries 2 cells * allot   create mod-picks most-mod-entries cells allot
variable #mod-entries
: title-entry ( addr len xt -- )
    #mod-entries @ most-mod-entries < if
        mod-picks #mod-entries @ cells + !  mod-names #mod-entries @ 2 cells * + 2!
        e-mod #mod-entries @ +  +entry  1 #mod-entries +!
    else  drop 2drop  then ;
defer title-menu-hook ( -- )

: entries! ( -- )   \ the menu's entries, as the title comes up
    0 #entries !  0 #mod-entries !  e-new +entry  e-load +entry  e-options +entry
    cleared? if  e-hard +entry  e-extras +entry  e-mini +entry  then
    title-menu-hook
    mods-menu-count 0> if  e-mods +entry  then
    updater? if  e-update +entry  then
    e-quit +entry ;
: entry ( i -- e )  cells entries + @ ;
: #items ( -- n )  #entries @ ;
: item ( i -- addr len )
    entry case  e-new of  s" NEW GAME"  endof  e-load of  s" LOAD GAME"  endof  e-options of  s" OPTIONS"  endof
                e-hard of  s" HARD MODE"  endof  e-extras of  s" EXTRAS"  endof  e-mini of  s" MINI-GAME"  endof
                e-mods of  s" MODS"  endof  e-update of  s" UPDATE"  endof
                e-quit of  s" QUIT"  endof
                >r  r@ e-mod - 2 cells * mod-names + 2@  r>   \ (a mod's)
            endcase ;
\ the save slots, to load one: each its number, place and time played
: slot-colour ( i -- rgba )  dup slot-at @ = if  drop $FFFFFFFF  else  slot-used? if  $B0A8A0FF  else  $605C58FF  then  then ;
: slot-draw ( n -- )   \ (a word of its own: a local inside a do loop would hide the loop's index)
    {: n | sw sh y left :}  screen-size to sh to sw
    sh 4 /  n char-size nip 2* *  +  to y   sw 10 /  to left
    left 3 char-size drop * 2* +  {: col :}   \ (where the place goes: 3 characters on)
    menu-layer  n 1+ n>s  left  y  n slot-colour 2 ui-text
    n slot-used? if
        menu-layer  n slot-place@ menu-line  col  y  n slot-colour 2 ui-text
        menu-layer  n slot-frames@ clock$  sw 6 * 10 /  y  n slot-colour 2 ui-text
    else
        menu-layer  s" - empty -"  col  y  n slot-colour 2 ui-text
    then ;
: slots-draw ( -- )
    screen-size {: sw sh :}  3 pen-scale
    menu-layer  s" LOAD GAME"  sw 10 /  sh 8 /  $D8D0C8FF 3 ui-text
    2 pen-scale
    #slots 0 do  i slot-draw  loop
    menu-layer  s" >"  sw 10 / char-size drop 2* 2* -  sh 4 / slot-at @ char-size nip 2* * +  $FFFFFFFF 2 ui-text ;
: entry-y ( i -- y )   \ (the pen's size set) from 7/10 down - higher when they wouldn't all fit
    char-size nip 3 2 */ {: i h :}  screen-size nip {: sh :}
    sh 7 * 10 /  sh #items 1+ h * -  min  i h * + ;
: menu-draw ( -- )
    menu-layer ui-clear
    stage @ on-slots = if  slots-draw exit  then
    stage @ on-mods = if  menu-layer mods.menu:draw exit  then
    stage @ on-update = if  menu-layer updater:draw exit  then
    stage @ on-menu <> if  exit  then
    screen-size {: sw sh :}  4 pen-scale
    menu-layer  s" HAUNTING GROUND"  sw 2/ 15 char-size drop * 2/ -  sh 8 /  $D8D0C8FF 4 ui-text
    3 pen-scale
    #items 0 do
        menu-layer  i item  sw 10 /  i entry-y
        i picked @ = $FFFFFFFF $8C8880FF ?  3 ui-text
    loop
    menu-layer  s" >"  sw 10 / char-size drop 2* -  picked @ entry-y  $FFFFFFFF 3 ui-text ;
: menu-keys ( -- )
    key: Up key-pressed? if  picked @ 1- 0 max picked !  then
    key: Down key-pressed? if  picked @ 1+ #items 1- min picked !  then
    cross button-pressed? if   \ (confirm: cross)
        picked @ entry case
            e-new of  0 hard !  m-get-up 0 play  getting-up stage !  endof
            e-hard of  -1 hard !  m-get-up 0 play  getting-up stage !  endof
            e-load of  0 slot-at !  on-slots stage !  endof
            e-options of  5 s" subscreen" actor-named send menu-mode  subscreen-wanted flag-on  on-options stage !  endof
            e-extras of  title-end  started stage !  extras  self kill  endof
            e-mini of  title-end  started stage !  -1 mini-game-wanted !  extras  self kill  endof
            e-mods of  mods.menu:open  on-mods stage !  endof
            e-update of  updater:open  on-update stage !  endof
            e-quit of  bye  endof
            >r  r@ e-mod - cells mod-picks + @ execute  r>   \ (a mod's: what it said picking it does)
        endcase
    then ;
: load-it ( -- )   \ (the slot's game: the title away, the saves actor puts it back)
    title-end  started stage !  slot-at @ s" saves" actor-named send load-from-slot  self kill ;
: slots-keys ( -- )
    key: Up key-pressed? if  slot-at @ 1- #slots + #slots mod slot-at !  then
    key: Down key-pressed? if  slot-at @ 1+ #slots mod slot-at !  then
    cross button-pressed? slot-at @ slot-used? and if  load-it exit  then
    triangle button-pressed? if  on-menu stage !  then ;

\ ---- the opening movie over the whole window, 4:3 in the middle, black round it ----
: draw-opening-movie ( -- )
    movie-layer ui-clear
    stage @ opening-movie <> if  exit  then
    movie-layer  0 0 screen-size  $000000FF ui-rect
    movie-layer  middle-4:3 ui-movie ;
: movie-start ( -- )   \ (none to play: straight into the game)
    title-end  opening-movie stage !
    s" OPENING.SFD" movie-open 0= if  started stage !  new-game  self kill  then ;
: movie-over ( -- )  movie-close  movie-layer ui-clear  started stage !  new-game  self kill ;   \ (started now: its last words leave it be)
: ended? ( -- flag )  dog @ motion-done? ;   \ (played once, at its last frame)
: title-up? ( -- flag )  stage @ opening-movie <  stage @ started > or ;   \ (the menu, his getting up and barking, the slots, the options)

behaviour opening-the-game
  on spawned ( -- )
      self subscribe tick  self subscribe frame-end
      hidden? if  started stage !  self kill  else  title-start  entries!  then ;   \ (headless: game.fs started it)
  on tick ( -- )
      stage @ case
          on-menu of  menu-keys  endof
          on-slots of  slots-keys  endof
          on-mods of  mods.menu:keys if  on-menu stage !  then  endof
          on-update of  updater:keys if  on-menu stage !  then  endof
          on-options of  subscreen-wanted state-flag? 0= if  on-menu stage !  then  endof   \ (back to the menu once they close)
          getting-up of  ended? if  m-bark 0 play  bark-sound common-sound  barking stage !  then  endof
          barking of  ended? if  movie-start  then  endof
          opening-movie of  movie-status 1 <>  start-button button-pressed? or if  movie-over  then  endof   \ (SceneTitle: Start skips)
      endcase ;
  on frame-end ( -- )  title-up? if  frame-him  then  menu-draw  draw-opening-movie ;
  on killed ( -- )   \ (free play: the title or the movie cut short)
      title-up? if  title-end  then  stage @ opening-movie = if  movie-close  movie-layer ui-clear  then ;
end-behaviour

: opening-spawn ( -- id )  opening-the-game opening-state s" opening" spawn ;
