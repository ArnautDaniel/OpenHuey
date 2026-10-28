\ story/places/house-of-truth-1f-2.fs - the House of Truth's 1F entrance hall ($88): in from the B1
\ by the forced way (the colours done: the summoner on); the water to drink (a recovery point - its
\ document the first time - once a visit, then "Ah, that tasted good."); the hole (in at her yes,
\ and out of it here); the crate on the ledge (broken by a blow: the Refined Antimony); the Refined
\ Magnesia; the Prima Magnesia Hewie sniffs out; not going back; Hewie's bark at the hole, his look;
\ its holes in the floor; its steady sound and four in turn; the walls' shade; the slam's shake; the
\ map's pages. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/house-of-truth-1f-2.fs in the history (converted from the game's; gone with the
\ old machinery in 0.2.20).
IN: story.places.house-of-truth-1f-2
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names story.kit story.places.house-of-truth fiona.anims ;

own-reset  truth-owns owns !

0 constant water-msg       \ "I hope this water is pure..."
1 constant tasted-msg      \ "Ah, that tasted good."
2 constant hole-msg        \ "Unsettling sounds and smells waft forth from the eerie hole. Enter the hole? YES NO"
3 constant go-back-msg     \ "If I went back I'm sure I would get lost. Better to keep moving forward."
own: rested                \ she has drunk here (once a visit)
own: steady-on             \ its steady sound started
own: hole-barked           \ Hewie has barked at the hole (once a visit)
0 constant magnesia-fx   1 constant antimony-fx   2 constant prima-fx   1 constant calm-sound   0 constant her-motions
$12 constant water-area   $13 constant clock-area   $14 constant hole-area   1 constant back-area
8 constant steady-sound   9 constant first-ambient
0 constant whole-part   1 constant broken-part   0 constant crate-group
ht1f-2-breakable-broken whole-part broken-part  -0.4e 70e -123.2e 5e 8e  breakable: the-crate
the-crate $80202020 0 1 shard-look  the-crate 0.812e 0.687e 0.187e 0.312e shard-uv  the-crate crate-group frees-floor  the-crate $34E noise-from
create holes  $51 , $21C , $2F , $54 , $23B , $43 , $22C , $350 , $41 , $22D , $354 , $232 , $355 , $34C , $35C , $351 ,
    $48 , $235 , $356 , $234 , $4B , $23D , $56 , $31 , $21E , $53 ,
: antimony-shows ( -- )  antimony-fx -0.4e 71e -123.2e flicker-at ;
: came-in ( exit -- )
    drop  room-sounds  the-crate as-it-stands
    the-crate broken?  ht1f-2-refined-antimony fact? 0= and if  antimony-shows  then
    ht1f-2-refined-magnesia fact? 0= if  magnesia-fx -33.17e 71e 33.19e flicker-at  then
    ht1f-2-prima-magnesia-found fact?  ht1f-2-prima-magnesia fact? 0= and if  prima-fx 35e 71e 33e flicker-at  then
    26 0 do  holes i cells + @ hole-floor floor-tri-on  loop ;

\ ---- in from the B1 (the colours done) ----
: up-from-below ( -- )
    fiona hold  fiona wait-move  1 map-page  off-map-27 preload  15 fade-in  summoner-on state! ;

\ ---- what she may do here ----
: the-water ( -- )   \ (a recovery point: once a visit it calms her)
    rested @ if  fiona wait-move  fiona 0e 5e turn-to-xz  tasted-msg say exit  then
    -1 rested !  water-msg her-motions calm-sound $117 0 0e -1.1e recovery-point ;
: not-back ( -- )
    fiona hold  fiona wait-move  fiona 90 turn  fiona turn-round-anim play  fiona -1 idle  go-back-msg say ;
: pick-magnesia ( -- )  refined-magnesia ht1f-2-refined-magnesia magnesia-fx -33.17e 33.19e picks-up ;
: pick-antimony ( -- )  refined-antimony ht1f-2-refined-antimony antimony-fx -0.4e -123.2e picks-up ;
: pick-prima ( -- )  prima-magnesia ht1f-2-prima-magnesia prima-fx 35e 33e picks-up ;
: prima-sniffed ( -- )  ht1f-2-prima-magnesia-found ht1f-2-prima-magnesia prima-fx  35e 71e 33e  hewie-finds ;
: spots ( -- )
    fiona water-area in-area?  fiona 0 50 heading? and if  ['] the-water unhurried offer  then
    fiona clock-area in-area?  fiona 0 50 heading? and if  ['] save-clock calm-hewie-up offer  then
    fiona hole-area in-area?  fiona 0 50 heading? and if  hole-msg -60e 55e hole-offer  then
    ht1f-2-refined-magnesia fact? 0=  fiona -33.17e 71e 33.19e 5e 5e touches? and if  ['] pick-magnesia calm-only offer  then
    the-crate broken? if
        ht1f-2-refined-antimony fact? 0=  fiona -0.4e 71e -123.2e 5e 5e touches? and if  ['] pick-antimony calm-only offer  then
    else  the-crate breaks? if  antimony-shows  then  then
    ht1f-2-prima-magnesia-found fact?  ht1f-2-prima-magnesia fact? 0= and  fiona 35e 71e 33e 5e 5e touches? and if  ['] pick-prima calm-only offer  then ;

\ ---- Hewie: his bark at the hole (once a visit, now and then), his look up there ----
: hole-bark ( -- )  -61.4e 0e 46.49e hewie-barks-at ;
create hole-look  -61.4e 0e 46.49e  20e 18e 18e look,

\ ---- every frame ----
create cameras  4 , 0 , 0 ,  5 , 2 , 2 ,  6 , 2 , 2 ,  7 , 3 , 3 ,  8 , 2 , 2 ,  9 , 1 , 1 ,  $A , 2 , 2 ,  $B , 4 , 4 ,
    $C , 2 , 2 ,  $D , 2 , 2 ,  $E , 2 , 2 ,  $F , 5 , 5 ,  $10 , 5 , 5 ,  $11 , 5 , 5 ,  $17 , 0 , 0 ,  $18 , 7 , 7 ,
    $19 , 3 , 3 ,  $1A , 6 , 6 ,   \ (area, set, path)
: shade ( -- )
    -20e 76e -132e  20e 76e -132e  -20e 50e -132e  20e 50e -132e  light-doorway
    38e 78e -60.2e  70e 55e -60.2e  38e 60e -60.2e  70e 40e -60.2e  light-doorway
    10e 73e -85.5e  43e 73e -85.5e  10e 40e -85.5e  43e 40e -85.5e  light-doorway
    -43e 73e -85.5e  -10e 73e -85.5e  -43e 40e -85.5e  -10e 40e -85.5e  light-doorway
    -70e 55e -60.2e  -38e 78e -60.2e  -70e 40e -60.2e  -38e 60e -60.2e  light-doorway ;
: each-frame ( -- )
    steady-on steady-sound -60e 20e 50e steady
    room-sounds-in? if  first-ambient -78e 21e 46.5e ambient-sounds  then
    0 exit-out  cameras 18 cameras-at
    fiona 2 entered? if  0 prepare-exit  then
    fiona 3 entered? if  off-map-27 preload  then
    fiona $15 entered?  fiona $16 entered? or if  1 map-page  then
    fiona $15 left?  fiona $16 left? or if  2 map-page  then
    hole-barked 30 -61.4e 0e 46.49e 20e 18e notices? if  ['] hole-bark start drop  then
    hole-look 1 hewie-looks-at
    fiona-doing-now @ 0=  in-panic? 0= and  fiona back-area entered? and  fiona held? 0= and if  ['] not-back fiona start-holding drop  then
    ht1f-2-prima-magnesia-found fact? 0=  scrambling? and  hewie 35e 70e 33e 10e 5e touches? and if  ['] prima-sniffed start-only  then
    slam-shake  shade  spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    0 6 6 way-in
    came-by @ hole-way = if  fiona $28 180 -60e 42e place-at  0 0 player-camera  ['] out-of-hole start drop  then
    came-by @ forced-exit = if
        fiona $3F -90 -23e 25e place-at  0 0 player-camera
        this-room @ 64 0 hewie-to  hewie $40 -90 place-on-tri  ['] up-from-below start drop
    then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went house-of-truth-1f-2 place!
