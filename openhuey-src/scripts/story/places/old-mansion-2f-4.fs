\ story/places/old-mansion-2f-4.fs - the old mansion's laboratory: the Purifier (its plaque, the
\ Sulfuric Ore laid in it turned to Silver Ore; another ore - nothing happens, handed back), the
\ inscription naming Torva and Weltigo, the desk, the passage's warm wind, the Viride Medallion; a
\ figure loaded on the way to the far door; Hewie's look. A place (story/kit.fs). The original's
\ scripts, for checking: story/rooms-ref/old-mansion-2f-4.fs (converted from the game's, not loaded).
IN: story.places.old-mansion-2f-4
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags items.names
    story.kit fiona.anims ;

own-reset

3 constant plaque-msg       \ "Purifier The impure yellow mass of filth will set free the shining glory of the sac..."
4 constant purifier-msg     \ "A purifier? I've never heard of a device like this before. There is a space to inse..."
5 constant no-change-msg    \ "No transformation occurred. I guess it didn't work. Maybe I should try a different item?"
6 constant transformed-msg  \ "Oh my! It actually transformed..."
7 constant desk-msg         \ "The desk is covered in strange research equipment. What kind of research could possibly..."
8 constant passage-msg      \ "There seems to be a passage to another one here. A warm wind is blowing in from that di..."
9 constant written-msg      \ "There is something written here... I... think I can just about make it out..."
$A constant names-msg       \ "Torva Temporarily gives anxiety to those who use it, making them feel panicked. Weltig..."
$B constant effects-msg     \ "Names for chemicals or medicine or something... Looks like their effects are liste..."
own: plaque-read   own: laid-item
0 constant medallion-fx   3 constant change-sound   $243 constant purifier-file   2 constant wood-object
$1C constant scene-figure   3 constant figure-slot

: came-in ( exit -- )
    drop  room-sounds  0 both-halves floor-on
    om2f-2-flag1 fact? if  0 show-part  wood-object object-hide  then
    om2f-4-viride-medallion fact? 0= if  medallion-fx 5.64e 9.5e 5.58e flicker-at  then ;

\ ---- the Purifier ----
: to-purifier ( -- )  stalkers-stay state!  fiona hold  fiona wait-move  fiona $A4 -35 -36e 41e walk-to ;
: purifier ( -- )   \ (its plaque the first time - then in turn its remark; its file)
    to-purifier
    plaque-read @ if  0 plaque-read !  fiona hand-to-mouth-anim 10 start-blend  purifier-msg say  fiona wait-anim
    else  15 screen:to-dim fade wait-fade  plaque-msg say  -1 plaque-read !  15 screen:from-dim fade wait-fade  then
    purifier-file give-file  stalkers-stay state-off ;
: handed-out ( item -- )  dup item-named  show-item-tab  fanfare  obtained-2-msg say ;
: transforms ( -- )   \ (the Sulfuric Ore laid in: Silver Ore)
    to-purifier  change-sound room-bank fiona sound-by  120 frames  fiona look-close-anim play
    transformed-msg say  fiona reach-out-anim play
    sulfuric-ore spend  silver-ore item-named  silver-ore 1 give-item  silver-ore show-item-tab  fanfare  obtained-2-msg say
    fiona reach-back-anim play  stalkers-stay state-off ;
: nothing-happens ( -- )   \ (another: handed back)
    to-purifier  change-sound room-bank fiona sound-by  120 frames  fiona look-close-anim play  fiona reach-out-anim play
    laid-item @ dup 8 = over $A $D within or swap $10 = or if  laid-item @ handed-out  then
    fiona reach-back-anim play  no-change-msg say  stalkers-stay state-off ;
: items-used ( who ev var value -- handled? )
    {: who ev v value :}  who 0<> if  false exit  then
    ev 0 = if  ['] transforms start drop  true exit  then
    ev 3 = if  value laid-item !  ['] nothing-happens start drop  true exit  then
    false ;

\ ---- the rest ----
: the-writing ( -- )
    stalkers-stay state!  fiona wait-move  fiona 180 turn  written-msg say
    15 screen:to-dim fade wait-fade  names-msg say  15 screen:from-dim fade wait-fade
    $48 items:item-bit? 0=  $49 items:item-bit? 0= or if
        fiona hand-to-mouth-anim start-anim  effects-msg say  fiona wait-anim
        $48 items send item-bit  $49 items send item-bit
    then
    stalkers-stay state-off ;
: the-desk ( -- )  fiona wait-move  fiona 0.6e 5.2e turn-to-xz  fiona look-low-anim start-anim  desk-msg say ;
: the-passage ( -- )  fiona wait-move  fiona 46.8e 0e turn-to-xz  fiona look-low-anim start-anim  passage-msg say ;
: take-medallion ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona 5.64e 5.58e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  viride-medallion om2f-4-viride-medallion medallion-fx takes-up  fiona reach-back-anim play ;

create looks  -44.47e 0e 54.87e  30e 16e 18e look,
create cameras  5 , 0 , 0 ,   6 , 2 , -1 ,   7 , 1 , 1 ,   8 , 2 , -1 ,   \ (area, set, path)
create entries  2 , -1 ,   0 , 0 ,   \ (by exit 0, 1: the camera)
: each-frame ( -- )
    2 exits-out  cameras 4 cameras-at
    fiona 3 entered? if  0 prepare-exit  figure-slot figure-out  then
    fiona 4 entered? if  1 prepare-exit  scene-figure figure-slot 1 figure-in-by  then
    looks 1 hewie-looks-at
    fiona $D in-area?  fiona -30 50 heading? and if  ['] purifier unhurried offer  then
    fiona $F in-area?  fiona 40 60 heading? and if  ['] the-desk unhurried offer  then
    fiona $10 in-area?  fiona 90 60 heading? and if  ['] the-passage unhurried offer  then
    om2f-4-viride-medallion fact? 0=  fiona 5.64e 9.5e 5.58e 5e 5e touches? and if  ['] take-medallion calm-only offer  then
    fiona $11 in-area?  fiona 180 50 heading? and if  ['] the-writing unhurried offer  then ;
: person-in ( i -- )  fiona-p = if  entries 2 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-2f-4 place!
' items-used old-mansion-2f-4 items-here
