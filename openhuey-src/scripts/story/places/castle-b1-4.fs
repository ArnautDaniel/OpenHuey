\ story/places/castle-b1-4.fs - the spiral stair down to Belli Castle's B1 ($7): its five flames
\ (stirred as someone steps into their draught), its sounds, the map's pages as she goes up and down,
\ the clock, the inscription ("The path beyond is the road of darkness..."). A place (story/kit.fs).
\ The original's scripts, for checking: story/rooms-ref/castle-b1-4.fs (converted from the game's,
\ not loaded).
IN: story.places.castle-b1-4
USING: engine game-state actors messages common facts flag-names story-names room-names story.kit fiona.anims ;

own-reset

\ ---- what is said here ----
0 constant darkness-msg   \ "'The path beyond is the road of darkness. Go not without a guide, lest ye lose yourself..."

\ ---- the room's things ----
2 constant to-exit-1-area   3 constant upper-area   6 constant clock-area   7 constant inscription-area
5 constant first-ambient   5 constant #flames
create flames  -11.28e f, 26.25e f, 0e f,   0e f, 35.2e f, 11.3e f,   11.3e f, 59.2e f, 0e f,
               0e f, 83.2e f, -11.3e f,   -11.28e f, 105.2e f, 0e f,
: flame@ ( k -- ) ( F: -- x y z )  3 * floats flames +  dup f@  dup float+ f@  2 floats + f@ ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  #flames 0 do  i 0  i flame@  flame-at  loop  1 $3FFF reverb ;

\ ---- what she may do here ----
: inscription ( -- )
    fiona wait-move  fiona 35e 10e turn-to-xz  fiona hand-to-mouth-anim 10 start-blend  darkness-msg say  fiona wait-anim ;
: spots ( -- )
    fiona clock-area 50 faces-area? if  ['] save-clock calm-hewie-up offer  then
    fiona inscription-area in-area?  fiona 0 50 heading? and if  ['] inscription unhurried offer  then ;

\ ---- every frame ----
create cameras  4 , 0 , 0 ,   5 , 1 , 1 ,   \ (area, set, path)
create entries  0 , 0 ,   1 , 1 ,   \ (by exit 0, 1: the camera)
create ahead  to-exit-1-area , 1 ,   upper-area , 0 ,   \ (area, exit loaded ahead)
: draughts ( -- )  #flames 0 do  i  i flame@  8e -4e  flame-stirred  loop ;
: each-frame ( -- )
    0 exit-out  0 1 leaves-by  cameras 2 cameras-at  ahead 2 ways-out
    fiona upper-area entered? if  1 map-page  then   fiona upper-area left? if  2 map-page  then
    draughts  room-sounds-in? if  first-ambient -34e 102e 100e ambient-sounds  then
    spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    entries 2 ways-in  came-by @ 1 = if  2 map-page  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-b1-4 place!
