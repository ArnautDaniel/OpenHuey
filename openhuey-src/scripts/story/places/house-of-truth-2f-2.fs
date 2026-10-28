\ story/places/house-of-truth-2f-2.fs - the House of Truth's 2F study ($83): brought here by the forced
\ way, Lorenzo's scene ("Who's there? Poor Fiona...": his door shut behind, the music's stage on); the
\ Mystery Sketch; the painting's title; the closet; the bobbles; the bookshelves - the untitled book
\ (in turn with the first remark), the Rubrum Medallion wedged in another; the name of a medicine
\ (marked in her menu); the Viride Medallion Hewie sniffs out; the doorways' shade; the replay. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/house-of-truth-2f-2.fs
\ (converted from the game's, not loaded).
IN: story.places.house-of-truth-2f-2
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    items.names story.kit fiona.anims scenes ;

own-reset

0 constant lorenzo-msg     \ "Who's there? Poor Fiona... Riccardo put you through quite a bit, didn't he? Unl..." (the scene's)
1 constant title-msg       \ "'MAGNUS' Little Box' Is this the painting's title?"
2 constant closet-msg      \ "This closet seems strangely warped at the sides."
3 constant bobbles-msg     \ "Various bobbles used for experimental purposes. Some notebooks have been stacked here."
4 constant books-msg       \ "Stacks of books have been arranged neatly on the shelf. Some of the books are in Englis..."
5 constant wedged-msg      \ "What's this? These something wedged in between these two books..."
6 constant written-msg     \ "Something is written here..."
7 constant fortis-msg      \ "Fortis Strengthens body for a limited amount of time."
8 constant medicine-msg    \ "Looks like the name of some medicine... The medicine's effects are explained as well."
9 constant untitled-msg    \ "One book stands out from the others. It has no title."
$A constant earthen-msg    \ "Great earthen one, Born in the image of man, Granted an ever so transient life SAL..."
$B constant no-sense-msg   \ "Not that it should surprise me, but this book makes no sense."
own: books-remarked        \ the books' first remark made (in turn with the untitled book)
0 constant lorenzo-part   1 constant sketch-part   0 constant viride-fx   $81 constant brought-tri
4 constant study-movie   3 constant study-cutscene   $10 constant scene-figure   $E2 constant study-door
$43 constant study-seen   $28 constant gasp-sound   $4B constant fortis-bit
: room-set ( -- )  lorenzo-part show-part  3 0 do  i object-hide  loop ;
: came-in ( exit -- )
    drop  room-set
    got-mystery-sketch fact? 0= if  sketch-part show-part  then
    ht2f-2-viride-medallion-found fact?  ht2f-2-viride-medallion fact? 0= and if  viride-fx 37e 1e 84e flicker-at  then ;

\ ---- brought here: Lorenzo's scene ----
: no-shadow-cues ( -- )   \ (her shadow off from its shot 2 to 3)
    begin  cutscene-frame cutscene-shot-at 2 <> while  yield  repeat  0 true no-shadow
    begin  cutscene-frame cutscene-shot-at 3 <> while  yield  repeat  0 false no-shadow ;
: study-scene ( -- )
    study-movie scenes:unseen-movie study-cutscene load-scene drop  3 figure-on
    ['] no-shadow-cues start drop  panic-eased  lorenzo-msg run-scene  ['] no-shadow-cues stop-doing ;
: lorenzo-there ( -- )
    fiona hold  stalker let-go  0 summon-take
    scene-figure 3 figure-in  lorenzo-kind stalker-ready  lorenzo-part hide-part
    fiona wait-move  gasp-sound common-bank fiona sound-by  yield
    study-scene  3 figure-out  stalker let-go  0 summon-take
    over-music-d fact!  0 false no-shadow  forced-exit take-exit
    study-door false open-mark  study-door lock  study-seen resident-flag-set
    -1 music-part  3 music-part  2 0 0 stage-music  3 0 0 stage-music  4 0 0 stage-music ;
: replayed ( -- )
    fiona hold  lorenzo-kind 3 figure-in  scene-figure 4 figure-in  yield
    study-scene  3 figure-out  4 figure-out  replay-over ;

\ ---- what she may do or say here ----
: takes-sketch ( -- )
    fiona wait-move  fiona 40e 54e turn-to-xz  fiona reach-out-anim play  4 frames
    sketch-part hide-part  mystery-sketch got-mystery-sketch -1 takes-the  fiona reach-back-anim play ;
: the-title ( -- )
    fiona $9F -68 -1 walk-move 5.3e 80.92e go-to
    world-frozen state!  fiona hold  1 8e 0e 0e 4.5e shot  title-msg say  8 frames  shot-off  world-frozen state-off ;
: the-closet ( -- )  fiona wait-move  closet-msg say ;
: the-bobbles ( -- )  fiona wait-move  fiona look-close-anim 10 start-blend  bobbles-msg say  fiona wait-anim ;
: the-books ( -- )  fiona wait-move  books-msg say ;
: the-untitled ( -- )   \ (in turn: the books' remark, then the untitled book)
    stalkers-stay state!  fiona hold  fiona wait-move
    books-remarked @ 0= if  books-msg say  -1 books-remarked !
    else
        fiona look-close-anim start-anim  untitled-msg say  fiona wait-anim
        15 screen:to-dim fade wait-fade  earthen-msg say  15 screen:from-dim fade wait-fade
        fiona hand-to-mouth-anim start-anim  no-sense-msg say  fiona wait-anim  0 books-remarked !
    then  stalkers-stay state-off ;
: the-wedged ( -- )   \ (the Rubrum Medallion between two books; after it, the books)
    fiona wait-move
    ht2f-2-rubrum-medallion fact? if  books-msg say exit  then
    wedged-msg say  fiona reach-out-anim play  4 frames
    rubrum-medallion ht2f-2-rubrum-medallion -1 takes-up  fiona reach-back-anim play ;
: the-medicine ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move  fiona 25 turn
    written-msg say  15 screen:to-dim fade wait-fade  fortis-msg say  15 screen:from-dim fade wait-fade
    fortis-bit items:item-bit? 0= if
        fiona hand-to-mouth-anim start-anim  medicine-msg say  fiona wait-anim  fortis-bit items send item-bit
    then  stalkers-stay state-off ;
: pick-viride ( -- )  viride-medallion ht2f-2-viride-medallion viride-fx 37e 84e picks-up ;
: viride-sniffed ( -- )  ht2f-2-viride-medallion-found ht2f-2-viride-medallion viride-fx  37e 1e 84e  hewie-finds ;
: spots ( -- )
    got-mystery-sketch fact? 0=  fiona 3 in-area? and  fiona 52 50 heading? and if  ['] takes-sketch calm-only offer  then
    fiona 4 in-area?  fiona -34 50 heading? and if  ['] the-title unhurried offer  then
    fiona 5 in-area?  fiona 32 50 heading? and if  ['] the-closet unhurried offer  then
    fiona 6 in-area?  fiona -3 50 heading? and if  ['] the-bobbles unhurried offer  then
    fiona 7 in-area?  fiona 46 50 heading? and if  ['] the-books unhurried offer  then
    fiona 9 in-area?  fiona -35 50 heading? and if  ['] the-books unhurried offer  then
    fiona 8 in-area?  fiona -41 50 heading? and if  ['] the-untitled unhurried offer  then
    fiona $A in-area?  fiona -24 50 heading? and if  ['] the-wedged unhurried offer  then
    ht2f-2-viride-medallion-found fact?  ht2f-2-viride-medallion fact? 0= and  fiona 37e 1e 84e 5e 5e touches? and if  ['] pick-viride calm-only offer  then
    fiona $B in-area?  fiona 12 50 heading? and if  ['] the-medicine unhurried offer  then ;

\ ---- every frame ----
create cameras  1 , 0 , -1 ,  2 , 1 , 0 ,   \ (area, set, path)
: shade ( -- )   \ (the doorways' shade: the characters' shadows kept off it)
    40e 20e -9e  17e 20e -9e  40e 0e -9e  23.2e 0e -9e  light-doorway
    -20e 16e 40e  3e 16e 40e  -20e 0e 40e  3e 0e 40e  light-doorway
    30e 16e 32.4e  45e 16e 32.4e  30e 0e 32.4e  45e 0e 32.4e  light-doorway ;
: each-frame ( -- )
    0 exit-out  cameras 2 cameras-at  shade
    ht2f-2-viride-medallion-found fact? 0=  scrambling? and  hewie 37e 0e 84e 10e 5e touches? and if  ['] viride-sniffed start-only  then
    spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    0 0 -1 way-in
    came-by @ forced-exit = if
        fiona brought-tri 0 place-on-tri  house-of-truth-2f-1 -1 1 hewie-to  world-held state!
        ['] lorenzo-there fiona start-holding drop
    then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went house-of-truth-2f-2 place!
