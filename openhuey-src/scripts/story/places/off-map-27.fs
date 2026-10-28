\ story/places/off-map-27.fs - room $27 (on no map page), the garden's hidden room: brought in by
\ its way ($FE: the front garden's) - placed, the world let go and faded in; the first time with the
\ costumes unlocked (and the castle's not yet behind her), the "Costumes" document filed; the
\ machine that tops up her Antimony Powder and Magnesia to one each (its sparkle while there's
\ something for her; taken - "There is nothing here." after); the slot machine - the word plate's
\ words (ALCHYMIA, ADAMAS, POWDER, MORGAN: items.fs) each give a piece of jewellery once, the Magist
\ Medallion if she has it already; any other word nothing; its remarks; the inscription on it; the
\ book of combinations; the chests of clothes - the costumes' menu (and before the castle's outfit,
\ the castle's and the "Under the Scalpel" outfits in turn); out by its way, faded. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/off-map-27.fs in the history
\ (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.off-map-27
USING: engine game-state actors messages common facts flag-names story-names room-names items.names
    story.kit fiona.anims ;

own-reset
own: lacks          \ what she lacks of the two (each a byte: the Antimony Powder's low, the Magnesia's next)
own: taken          \ the machine's given what she lacked: nothing there now

\ ---- what is said here ----
$40AB constant costume-doc-msg   \ "'Costumes' You are now able to change costumes. Change into alternate costu..."
6 constant strange-msg           \ "What a strange machine... I wonder what they use it for?"
7 constant nothing-msg           \ "There is nothing here."
8 constant written-msg           \ "There is something written here."
9 constant variatio-msg          \ "'Variatio Machina' By using a medallions, one can actually transform items into new thi..."
$A constant slot-msg             \ "What on earth could this machine be for? There's a slot on it..."
$B constant plate-key-msg        \ "This must be a machine that dispenses something if a certain Plate-key is inserted."
$C constant nothing-happened-msg \ "Nothing happened. Nothing at all."
$D constant archaic-msg          \ "The book looks almost archaic in nature. Wait, something is written here..."
$E constant combinations-msg     \ "Quies Restores a small amount of stamina. Mundus Calms the nerves of those who use it."
$F constant wonder-msg           \ "I wonder what these combinations are supposed to mean?"
$40 constant chest-msg           \ "A chest used to store clothes."
$41 constant change-msg          \ "A chest used to store clothes. Change clothes? YES NO"
$57 constant castle-outfit-msg   \ "Is this the outfit I was wearing while in the castle? Change into this outfit? YE..."
$58 constant scalpel-msg         \ "The 'Under the Scalpel' outfit that I just removed is here. Change into this outf..."

\ ---- the room's things ----
$FE constant garden-way   $36 constant room-track   0 constant wrong-sound   4 constant reach-sound   5 constant reach-back-sound
0 constant topping-fx   1 constant slot-fx   0 constant chest-part   3 constant slots-menu
0 constant way-out-area   1 constant inscription-area   2 constant chest-area   3 constant second-chest-area   4 constant book-area
2 constant castle-outfit   3 constant scalpel-outfit   8 constant medallion-ev   9 constant no-medallion-ev
$42 constant quies-seen   $44 constant mundus-seen   \ (the sub screen's marks: the combinations read)
: chests-open? ( -- flag )  fg-3-flag3 fact? ;   \ (the front garden's: her costumes to hand)

\ ---- the topping-up machine: one of each she lacks, given ----
: lacking ( item -- n )  1 swap items:items-of - 0 max ;   \ (short of one)
: lacks-kept ( -- )  antimony-powder lacking  magnesia lacking 8 lshift or  lacks ! ;
: lacks-given ( -- gave1 gave2 )   \ (each she lacked, given; which were)
    lacks @ $FF and  lacks @ 8 rshift $FF and  {: a m :}
    a if  antimony-powder a items send give-item  then  m if  magnesia m items send give-item  then  a 0<>  m 0<> ;
: shows-tab ( item -- )  dup item-named  dup show-item-tab  fanfare  drop  obtained-msg say ;
: the-machine ( -- )
    fiona wait-move  fiona -28e 6e turn-to-xz
    lacks @ 0= if  strange-msg say exit  then
    taken @ if  nothing-msg say exit  then
    fiona reach-out-anim play  4 frames  lacks-given  topping-fx effect-off
    {: g1 g2 :}  g1 if  antimony-powder shows-tab  then  g2 if  16 frames  magnesia shows-tab  then
    -1 taken !  fiona reach-back-anim play ;

\ ---- as she comes in ----
: came-in ( exit -- )
    room-sounds  room-track 1e background
    garden-way = if  lacks-kept  lacks @ if  topping-fx -27.75e 10e 5.68e flicker-at  then  then
    chests-open? if  chest-part show-part  then ;
: arrives ( -- )   \ (placed by its way: the world let go, faded in; the costumes' document the first time)
    world-held state-off  camera-restart  15 screen:from-black fade  wait-fade
    chests-open? 0=  x-37-flag2 fact? and  got-doc-action fact? 0= and if
        15 screen:to-dim fade wait-fade  costume-doc-msg say  15 screen:from-dim fade wait-fade
        got-doc-action fact!  doc-costume filed
    then ;

\ ---- the slot machine: the word plate's words (items.fs), the jewellery each gives once ----
: to-the-slot ( -- )
    fiona $46 -160 -9e -13e walk-to
    fiona reach-out-anim play  reach-sound room-bank fiona sound-by  4 frames  fiona reach-back-anim play ;
: given-for ( item fact -- )   \ (the jewellery - or, had already, the Magist Medallion)
    {: it f :}  it items:items-of 1 < 0= if  magist-medallion  else  it  then
    dup item-named  dup 1 give-item  dup show-item-tab  fanfare  drop  obtained-2-msg say  f fact! ;
: medallion-given ( -- )   \ (the word right: one of the four, as items.fs named it)
    to-the-slot  reach-back-sound room-bank fiona sound-by
    slot-fx -11e 3e -17e flicker-at  46 frames
    fiona pick-up-anim play  slot-fx effect-off
    word-vars cell+ @ case
        alchemist-earrings of  alchemist-earrings x-27-magist-medallion given-for  endof
        diamond-choker of  diamond-choker x-27-magist-medallion-2 given-for  endof
        burst-shoes of  burst-shoes x-27-magist-medallion-3 given-for  endof
        fairy-earrings of  fairy-earrings x-27-magist-medallion-4 given-for  endof
    endcase
    fiona up-from-pick-anim play ;
: nothing-given ( -- )
    to-the-slot  30 frames  fiona hand-to-mouth-anim start-anim  nothing-happened-msg say  fiona wait-anim ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0<> if  drop false exit  then
    case
        medallion-ev of  ['] medallion-given  endof
        no-medallion-ev of  ['] nothing-given  endof
        >r false r>
    endcase  dup if  fiona start-holding drop  true  then ;
: any-medallion? ( -- flag )
    x-27-magist-medallion fact?  x-27-magist-medallion-2 fact? or  x-27-magist-medallion-3 fact? or  x-27-magist-medallion-4 fact? or ;
: the-slot ( -- )
    fiona wait-move  fiona -12e -22e turn-to-xz
    any-medallion? if  plate-key-msg say exit  then
    fiona hand-to-mouth-anim start-anim  slot-msg say  fiona wait-anim ;

\ ---- what she may read here ----
: the-inscription ( -- )
    fiona wait-move  written-msg say  15 screen:to-dim fade wait-fade  variatio-msg say  15 screen:from-dim fade wait-fade ;
: the-book ( -- )
    stalkers-stay state!  fiona wait-move  fiona 8e -4e turn-to-xz
    archaic-msg say  15 screen:to-dim fade wait-fade  combinations-msg say  15 screen:from-dim fade wait-fade
    quies-seen items:item-bit? 0=  mundus-seen items:item-bit? 0= or if
        fiona hand-to-mouth-anim start-anim  wonder-msg say  fiona wait-anim
        quies-seen items send item-bit  mundus-seen items send item-bit
    then  stalkers-stay state-off ;
: the-slots ( -- )  fiona wait-move  fiona -100 turn  slots-menu open-subscreen ;

\ ---- the chests of clothes ----
: outfit-on ( costume -- )   \ (one outfit put on there: faded out and in, her steps' sounds with it)
    15 screen:to-black-2 with-music or fade  wait-fade  world-held state!
    wrong-sound room-bank fiona sound-by  60 frames  dress  steps-in
    fiona 3 180 -0.912e -13.497e place-at  camera-restart  15 screen:from-black with-music or fade  wait-fade ;
: the-chest ( -- )   \ (before her costumes: the castle's outfit, then the one she took off)
    fiona wait-move  fiona 180 turn
    got-doc-action fact? 0= if  chest-msg say exit  then
    costume-var var@ scalpel-outfit = if
        castle-outfit-msg yes? if  castle-outfit outfit-on  then
    else  scalpel-msg yes? if  scalpel-outfit outfit-on  then  then ;
: the-costumes ( -- )
    fiona wait-move  fiona 180 turn  1 20e 10e 0e 0e shot
    change-msg yes? if
        no-track 1e background  costume-menu open-subscreen  shot-off  wrong-sound clothes-changed
        room-track 1e background  15 screen:from-black with-music or fade  wait-fade
    else  16 frames  shot-off  then ;

\ ---- the way out: faded, the world held ----
: leaves ( -- )  fiona wait-move  15 screen:to-black fade  wait-fade  world-held state!  garden-way take-exit ;

\ ---- every frame ----
: facing? ( area deg within -- flag )  {: area deg within :}  fiona area in-area?  fiona deg within heading? and ;
: spots ( -- )
    way-out-area 90 50 facing? if  ['] leaves unhurried offer  then
    inscription-area -180 55 facing? if  ['] the-inscription unhurried offer  then
    chest-area 180 55 facing? if  chests-open? if  ['] the-costumes  else  ['] the-chest  then  unhurried offer  then
    fiona -16.2e -5.68e 5e reaches-across?  fiona -100 60 heading? and if  ['] the-slots unhurried offer  then
    fiona -21.456e 6.443e 5e reaches-across?  fiona -90 60 heading? and if  ['] the-machine unhurried offer  then
    book-area 90 50 facing? if  ['] the-book unhurried offer  then
    chests-open?  second-chest-area 180 55 facing? and if  ['] the-slot unhurried offer  then ;
: each-frame ( -- )  spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    came-by @ garden-way = if  fiona $60 -90 place-on-tri  0 0 player-camera  ['] arrives fiona start-holding drop  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went off-map-27 place!
' items-used off-map-27 items-here
