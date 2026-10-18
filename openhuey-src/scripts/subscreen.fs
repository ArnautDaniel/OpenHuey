\ subscreen.fs - the sub screen, the in-game menu (docs/subsystems/items.md; src/game/subscreen.c).
\ Select opens it while Fiona is free (SceneGame_SubPlay): the world is held (`pause!`) and only
\ this actor has frames (`held-tick`). Its pages - the item lists, the map, the files, the
\ options - and the scripts' machines (the word plate, the slot machine) are in subscreen/; this
\ file opens and closes it, gives the keys to what is showing and draws it. Also the tab that
\ slides in when the story hands her an item (the scripts' 0xBE, SubScreen_DrawTab).
IN: subscreen
USING: engine game-state actors messages keys facts flag-names items items.tables ps2-screen subscreen.state subscreen.art subscreen.input subscreen.box subscreen.lists subscreen.map subscreen.options subscreen.plate subscreen.slots subscreen.saving subscreen.costumes subscreen.extras subscreen.galleries ;

<PRIVATE
\ ---- which art is in the background group: the menu's, the plate's, the slot machine's ----
: background ( n file len -- )  back-group load-bank  back-bank ! ;
: menu-art ( -- )
    art-loaded @ 0= if  s" SUBSCR/SUBBASE" base-group load-bank  -1 art-loaded !  then
    back-bank @ if  0 s" SUBSCR/SUBBACK" background  then ;

\ ---- opening and closing ----
: may-open? ( -- flag )   \ Select, she free, the panic below 4, no scene, nothing held
    select-button button-pressed?
    subscreen-locked state-flag? 0= and  world-held state-flag? 0= and
    her-mode @ 0= and  the-panic @ 4 < and  cutscene-active? 0= and ;   \ (her mode only: out of breath, pushing - it opens)
: open-menu ( -- )
    menu-art  opening stage !  0 openness !  back-to-page  0 quiet !  0 opt-editing !  back-to-her
    se-open  1 pause! ;
: open-plate ( -- )  open-menu  1 s" SUBSCR/PLATE" background  on-plate mode !  start-plate ;
: open-slots ( -- )  open-menu  2 s" SUBSCR/SYN_SLOT" background  on-slots mode !  start-slots ;
: open-saving ( -- )  open-menu  on-saving mode !  start-saving ;
: open-costumes ( -- )  open-menu  on-costumes mode !  start-costumes ;
: open-options ( -- )  open-menu  pg-options page !  -1 options-only ! ;   \ (the title's: SceneTitle_SeqOptions)
: open-extras ( -- )  open-menu  3 s" SUBSCR/SUBMG" background  on-extras mode !  start-extras ;   \ (the mini-game's menu: mode $A)
: open-results ( -- )  open-menu  on-results mode !  start-results ;   \ (its results: mode $C)
: open-gallery ( kind -- )   \ (the extras room's galleries: modes 8, $B, $D, $E, $F)
    open-menu  4 over gallery-file background  start-gallery ;
: open-played ( -- )  hewie-played? if  open-options  else  open-menu  then ;   \ (in the run, the options alone)
: asked-open ( -- )   \ (the scripts' 0x6D: its mode; the rest - not built - shut at once)
    opened-as @ case
        0 of  open-played  endof
        1 of  open-saving  endof
        $A of  open-extras  endof
        $C of  open-results  endof
        5 of  open-options  endof
        7 of  open-costumes  endof
        2 of  open-plate  endof
        3 of  open-slots  endof
        8 of  gal-movies open-gallery  endof
        $B of  gal-models open-gallery  endof
        $D of  gal-music open-gallery  endof
        $E of  gal-art open-gallery  endof
        $F of  gal-types open-gallery  endof
        >r subscreen-wanted state-flag-clear r>
    endcase ;
: shut-menu ( -- )   \ back to play: the world's frames again, the scripts' wait over
    options-only @ if  0 options-only !  pg-special page !  then
    gal-kind @ if  gallery-shut  then
    shut stage !  0 openness !  0 pause!  subscreen-wanted state-flag-clear  art-layer ui-clear  word-layer ui-clear ;

\ ---- what is showing: its keys and its drawing ----
: menu-keys ( -- )
    read-arrows
    mode @ case
        on-actions of  action-keys  endof
        on-box of  box-keys  endof
        on-plate of  plate-keys-read  endof
        on-slots of  slots-keys  endof
        on-saving of  saving-keys  endof
        on-costumes of  costumes-keys  endof
        on-extras of  extras-keys  endof
        on-results of  results-keys  endof
        on-gallery of  gallery-keys  endof
        on-page of
            page @ case  pg-files of  files-keys  endof  pg-map of  map-keys  endof
                         pg-options of  options-keys  endof  >r list-keys r>  endcase  endof
    endcase ;
\ the machines' pages are the whole screen (nothing of the world behind): beyond the 4:3 frame
\ on a wider window, black - faded in with the page
: window-black ( -- )  art-layer 0 0 screen-size  openness @ 2* 255 min  ui-rect ;
: draw-page ( -- )
    gal-kind @ if  gallery-view  mode @ on-box = if  box-view  then  exit  then
    mode @ on-plate = if  window-black  plate-view exit  then
    mode @ on-slots = if  window-black  slots-view exit  then
    mode @ on-saving = if  saving-view exit  then
    mode @ on-costumes = if  costumes-view exit  then
    mode @ on-extras = if  window-black  extras-view exit  then
    mode @ on-results = if  window-black  results-view exit  then
    page @ case  pg-files of  files-view  endof  pg-map of  map-view  endof
                 pg-options of  options-view  endof  >r list-view r>  endcase
    mode @ on-actions = if  actions-view  then
    mode @ on-box = if  box-view  then ;
: draw-tab ( -- )   \ the tab and the item's picture, sliding in from the left (fading with it)
    tab-slide @ >r
    tab @ case
        2 of  $AC $40 - r@ +  $60 $C r@ 2* part   $C0 $40 - r@ +  $8C $B r@ part  endof
        3 of  $AC $60 $C $80 part   $C0 $8C $B $40 part  endof
        4 of  $EC r@ -  $60 $C r@ 2* part   $100 r@ -  $8C $B r@ part  endof
    endcase  r> drop ;
: draw ( -- )
    frame!  art-layer ui-clear  word-layer ui-clear
    stage @ shut <> if  draw-page exit  then
    tab @ if  $80 openness !  draw-tab  0 openness !  then ;

\ ---- a frame with the world held ----
: held-frame ( -- )
    1 frames +!
    stage @ case
        opening of  16 openness +!  openness @ 128 >= if  128 openness !  up stage !  then  endof
        up of  menu-keys  select-button button-pressed? gal-kind @ 0= and if  close-menu  then  endof   \ (in a gallery select is its own)
        closing of
            openness @ 128 = quiet @ 0= and if  se-close  then
            -32 openness +!  openness @ 0> 0= if  shut-menu exit  then  endof
    endcase
    draw ;

\ ---- the tab: told who asked once it is in or out ----
: tab-frame ( -- )
    tab @ 2 = if  tab-slide @ $3D < if  4 tab-slide +!  else  3 tab !  tab-asker @ send tab-done  then  then
    tab @ 4 = if  tab-slide @ 4 >= if  -4 tab-slide +!  else  0 tab !  tab-asker @ send tab-done  then  then ;
: tab-item ( id -- )   \ its picture, and the menu's art for the tab
    -1 shown-picture !  menu-art  item-picture dup if  picture-group load-bank  else  2drop  then ;
PRIVATE>

behaviour menu
  on spawned ( -- )
      shut stage !  pg-special page !  -1 shown !  -1 shown-picture !  0 tab !  -1 stalker-room !  defaults
      1 back-bank !  -1 her-map !  -1 her-pg !  -1 her-room !  -1 map-loaded !
      self subscribe tick  self subscribe held-tick  self subscribe frame-end  self subscribe arrived
      self subscribe fiona-doing  self subscribe danger  self subscribe panic  self subscribe stalker-doing ;
  on tick ( -- )
      1 frames +!  tab-frame
      stage @ shut = if
          may-open? if  open-played exit  then
          subscreen-wanted state-flag? if  asked-open  then
      then ;
  on held-tick ( -- )  held-frame ;
  on frame-end ( -- )  draw ;
  on menu-mode ( mode -- )  opened-as ! ;
  on game-saved ( -- )  saved-now ;
  on arrived ( room exit -- )  drop find-room ;        \ (Map_FindRoom as each room comes in)
  on map-turn-to ( page -- )  show-on-page ;            \ (the scripts' 0xD1)
  on item-used ( id result -- )  mode @ awaiting-use = if  used  else  2drop  then ;
  on fiona-doing ( mode sub cond cmd doing -- )  her-doing !  2drop drop  her-mode ! ;
  on danger ( level -- )  the-danger ! ;
  on panic ( stage level -- )  drop the-panic ! ;
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )  2drop 2drop 2drop 2drop  stalker-room ! ;
  on tab-op ( op item -- )   \ (the scripts' 0xBE: 0 the item's, 1 its picture in, 2 slide in, 3 shown, 4 slide out)
      sender tab-asker !
      swap case
          0 of  tab-item  0 tab-slide !  tab-asker @ send tab-done  endof
          2 of  drop  2 tab !  endof
          4 of  drop  4 tab !  endof
          3 of  drop  3 tab !  $40 tab-slide !  tab-asker @ send tab-done  endof
          >r drop  tab-asker @ send tab-done  r>
      endcase ;
end-behaviour

: subscreen-spawn ( -- id )  menu subscreen-state s" subscreen" spawn ;
