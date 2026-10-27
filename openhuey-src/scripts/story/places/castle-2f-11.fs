\ story/places/castle-2f-11.fs - the corridor of the slanted crest in Belli Castle's 2F ($1F): the
\ hanging doll and swing rocking (the first creaking each turn), the crest, the specks, what Hewie
\ looks up at. A place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/
\ castle-2f-11.fs (converted from the game's, not loaded).
IN: story.places.castle-2f-11
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    effects story.kit fiona.anims ;

own-reset

\ ---- what is said here ----
0 constant crest-msg   \ "This crest... Why is it slanted like this?"

\ ---- the room's things ----
4 constant crest-area   0 constant creak-sound   2 constant #hung
: hung ( i -- addr len )  this-room @ story.strings:room-string ;   \ (0 the doll, 1 the swing)
: creak ( -- )  creak-sound room-bank 0 0  -366e 30e -25e sound-at ;

\ ---- the doll and the swing rocking 4 degrees, their phase on 6 a frame - the doll creaking each turn ----
: still-one ( i -- )
    dup hung {: i a n :}  a n placed-here? 0= if  exit  then
    0e a n swing-phase object!  0e a n turn-z object!  i 0= if  creak  then ;
: rock-one ( i -- )
    dup hung {: i a n :}  a n placed-here? 0= if  exit  then
    6e a n swing-phase object+!
    a n swing-phase object@ 360e f< 0= if  -360e a n swing-phase object+!  i 0= if  creak  then  then
    a n swing-phase object@ deg>rad fsin 4e f* deg>rad a n turn-z object! ;
: hung-still ( -- )  #hung 0 do  i still-one  loop ;
: rocking ( -- )  begin  #hung 0 do  i rock-one  loop  yield  again ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  hung-still  ['] rocking start drop
    $A $C $60 $60 $60 $20 rgba -33.7e 23.2e -126e specks   $C $C $60 $60 $60 $20 rgba -92.2e 23.2e -92e specks
    $A $C $60 $60 $60 $20 rgba -118e 23.2e -48.5e specks
    1 $2300 sound-reverb! ;

\ ---- what she may do or say here ----
: crest ( -- )  fiona wait-move  fiona 60 turn  crest-msg say ;
: spots ( -- )  fiona crest-area in-area?  fiona 30 50 heading? and if  ['] crest unhurried offer  then ;

\ ---- every frame ----
create looks  -366.29e 0e -24.86e  35e 29e 29e look,
create ahead  2 , 0 ,   3 , 1 ,   \ (area, exit)
create entries  0 , 0 ,   0 , 0 ,   \ (by exit 0, 1: the camera)
: each-frame ( -- )  2 exits-out  ahead 2 ways-out  looks 1 hewie-looks-at  spots ;
: person-in ( i -- )  fiona-p = if  entries 2 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-2f-11 place!
