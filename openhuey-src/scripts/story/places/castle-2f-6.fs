\ story/places/castle-2f-6.fs - the study in Belli Castle's 2F ($22): the desk (locked - her blows on
\ it; the Study Key used: the drawer open, the Lunar Refractor - and the stalker stronger, the summoner
\ on), the board (the Luminessants), the shelves, the massive book and the desktop in turn, the
\ Lavender, what Hewie barks at and looks up at, the far view's depth. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/castle-2f-6.fs (converted from the game's, not
\ loaded).
IN: story.places.castle-2f-6
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects story.kit fiona.anims ;

own-reset

\ ---- what is said here ----
3 constant locked-msg      \ "The desk is locked."
8 constant shelves-msg     \ "Old scholarly masterpieces fill up the shelves. It's almost as if someone has been..."
9 constant study-msg       \ "Not to state the obvious, but this is clearly the castle study."
$A constant book-msg       \ "A massive book stands stalwart. It looks quite old. Perhaps it's an antique?"
$B constant greek-msg      \ "The script looks almost Latin in nature... But unfortunately, it's all Greek to me."
$C constant board-msg      \ "There is something scribbled on the board..."
$D constant no-clue-msg    \ "'Luminessants'...'Azoth'... No clue what any of this means."
$E constant fireflies-msg  \ "Luminessants... judging by the ring of that, I'll bet these are those lightning bu..."
$F constant luminessants-msg  \ "'Luminessants' Luminessants are tiny creatures that respond to the ethereal energies g..."
$10 constant desktop-msg   \ "Various formulae and computations have been scribbled on the desktop. This goes way b..."

\ ---- its own state (a visit) ----
own: shelves-remarked  own: book-remarked  own: hewie-noticed

\ ---- the room's things ----
0 constant lavender-fx   0 constant drawer-object   1 constant desk-motions
5 constant desk-area   6 constant board-area   $E constant book-area   $F constant desktop-area
0 constant open-sound   1 constant obtained-sound   2 constant strike-sound   3 constant reach-sound
: refractor-had? ( -- flag )  got-lunar-refractor fact? ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  refractor-had? 0= if  room-sounds  then
    c2f-3-flag1 fact? 0= if  $84 $60 $3C $50 $24 $2A $2A $46 room-tint  then
    c2f-6-lavender fact? 0= if  lavender-fx -22.26e 8.5e -41.95e flicker-at  then ;

\ ---- the desk: locked (her blows on it); the Study Key used - the drawer, the Lunar Refractor ----
: at-desk ( F: z -- )   \ (to her spot at it, its motions hers)
    {: F: z :}  fiona desk-motions motions-for  fiona $55 0 -1 walk-move 3.622e z go-to  fiona use-motions ;
: desk-locked ( -- )
    stalkers-stay state!  32.279e at-desk
    fiona room-motion 2 + 10 start-blend  22 frames  strike-sound room-bank fiona sound-by
    11 frames  strike-sound room-bank fiona sound-by  8 frames  strike-sound room-bank fiona sound-by
    fiona wait-anim  locked-msg say  stalkers-stay state-off ;
: drawer-opened ( -- )
    fiona hold  stalkers-stay state!  31e at-desk
    stalker-unloaded  debilitas-7-kind stalker-ready
    fiona reach-out-anim play  reach-sound room-bank fiona sound-by  fiona reach-back-anim play
    study-key item-named  used-msg say
    fiona $55 0 3.622e 32.279e place-at  1 -17e -10e 30e 0e shot  8 frames
    drawer-object 1 0 object  fiona room-motion 10 start-blend  20 frames  open-sound room-bank fiona sound-by
    fiona wait-anim  16 frames  yield
    got-lunar-refractor fact!  study-key spend
    lunar-refractor item-named  lunar-refractor 1 give-item  lunar-refractor show-item-tab  fanfare  obtained-2-msg say
    1 0 0 stage-music  obtained-sound room-bank fiona sound-by
    drawer-object 1 1 object  fiona room-motion 1 + 0 play-blend  16 frames  shot-off
    summoner-on state!  stalkers-stay state-off ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0= 0= if  drop false exit  then
    5 = if  ['] drawer-opened start drop  true  else  false  then ;

\ ---- what she may do or say here ----
: the-board ( -- )
    stalkers-stay state!  fiona wait-move  board-msg say
    15 screen:to-dim fade  wait-fade  luminessants-msg say  15 screen:from-dim fade  wait-fade
    got-doc-luminessants fact? if  fireflies-msg say
    else  fiona hand-to-mouth-anim 10 start-blend  no-clue-msg say  fiona wait-anim  then
    stalkers-stay state-off ;
: shelves ( -- )
    fiona wait-move  shelves-remarked @ if  study-msg  else  -1 shelves-remarked !  shelves-msg  then  say ;
: the-book ( -- )
    fiona wait-move
    book-remarked @ if  fiona look-low-anim 10 start-blend  greek-msg say  fiona wait-anim
    else  -1 book-remarked !  book-msg say  then ;
: desktop ( -- )  fiona wait-move  fiona -135 turn  desktop-msg say ;
: reach-lavender ( -- )
    fiona wait-move  fiona -22.26e -41.95e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  lavender c2f-6-lavender lavender-fx takes-up  fiona reach-back-anim play ;
: spots ( -- )
    c2f-6-lavender fact? 0=  fiona -22.26e 8.5e -41.95e 5e 5e touches? and if  ['] reach-lavender calm-only offer  then
    refractor-had? 0=  fiona desk-area in-area? and  fiona 0 45 heading? and if  ['] desk-locked unhurried offer  then
    fiona board-area in-area?  fiona -45 45 heading? and if  ['] the-board unhurried offer  then
    fiona desktop-area in-area?  fiona -67 50 heading? and if  ['] desktop unhurried offer  then
    fiona 7 in-area?  fiona 9 in-area? or  fiona -45 45 heading? and if  ['] shelves unhurried offer  then
    fiona 8 in-area?  fiona $B in-area? or  fiona 0 45 heading? and if  ['] shelves unhurried offer  then
    fiona $A in-area?  fiona $D in-area? or  fiona 45 45 heading? and if  ['] shelves unhurried offer  then
    fiona $C in-area?  fiona 90 45 heading? and if  ['] shelves unhurried offer  then
    fiona book-area 50 faces-area? if  ['] the-book unhurried offer  then ;

\ ---- Hewie: a bark at the desk once a visit (the refractor in it still), his looks up ----
create looks
    3.07e 0e 30.96e  13e 35e 35e look,
    -28.72e 0e -20.26e  13e 12e 12e look,
: barks-at-desk ( -- )  3.5e 0e 33e hewie-barks-at ;
: hewie-notices ( -- )
    refractor-had?  hewie-noticed @ or  hewie-in? 0= or  hewie-cond-now @ well-cond <> or  chased? or  hewie-cmdable-now @ 0= or if  exit  then
    hewie 3.5e 0e 33e 30e 20e centred-in? 0= if  exit  then
    -1 hewie-noticed !  ['] barks-at-desk start drop ;

\ ---- the far view's depth (camera set 0) ----
: depth-by-camera ( -- )
    cam-changed? 0= if  exit  then
    cam-set@ 0= if  20e 30e 2000e 2000e depth-range  else  depth-range-off  then ;

\ ---- every frame ----
create cameras  1 , 0 , 0 ,   2 , 1 , -1 ,   3 , 1 , -1 ,   4 , 2 , -1 ,   \ (area, set, path)
: each-frame ( -- )  0 exit-out  cameras 4 cameras-at  hewie-notices  looks 2 hewie-looks-at  spots  depth-by-camera ;
: person-in ( i -- )  fiona-p = if  0 1 -1 way-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-2f-6 place!
' items-used castle-2f-6 items-here
