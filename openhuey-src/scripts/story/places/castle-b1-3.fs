\ story/places/castle-b1-3.fs - the store room in Belli Castle's B1 ($16): the Metal Boots on the
\ shelf (had already: a Magist Medallion there instead), the Castle Gate Key once she has her
\ costumes (and hasn't it); the doorways' shade. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/castle-b1-3.fs (converted from the game's, not loaded).
IN: story.places.castle-b1-3
USING: engine game-state actors messages common facts flag-names story-names room-names items.names
    story.kit fiona.anims ;

own-reset

\ ---- the room's things ----
0 constant boots-fx   1 constant key-fx

\ ---- as she comes in ----
: key-lies-here? ( -- flag )  fg-3-flag3 fact?  got-castle-gate-key fact? 0= and  castle-gate-key items:items-of 0= and ;
: came-in ( exit -- )
    drop  1 $2300 sound-reverb!
    got-metal-boots fact? 0= if  boots-fx -2e 9e -2e flicker-at  then
    key-lies-here? if  key-fx -6e 9e 3e flicker-at  then ;

\ ---- what she may take here ----
: reach-to ( F: x z -- )  fiona wait-move  fiona turn-to-xz  fiona reach-out-anim play  4 frames ;
: the-boots ( -- )   \ (had already: a Magist Medallion there - not in a panic)
    metal-boots items:items-of 0= if
        -2e -2e reach-to  metal-boots got-metal-boots boots-fx takes-the  fiona reach-back-anim play  exit
    then
    fiona wait-move  fiona -2e -2e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  magist-medallion got-metal-boots boots-fx takes-up  fiona reach-back-anim play ;
: the-key ( -- )  -6e 3e reach-to  castle-gate-key got-castle-gate-key key-fx takes-the  fiona reach-back-anim play ;
: spots ( -- )
    got-metal-boots fact? 0=  fiona -2e 9e -2e 5e 5e touches? and if  ['] the-boots calm-only offer  then
    got-castle-gate-key fact? 0=  key-fx effect @ 0<> and  fiona -6e 9e 3e 5e 5e touches? and if  ['] the-key calm-only offer  then ;

\ ---- every frame ----
create cameras  3 , 0 , 0 ,   4 , 1 , -1 ,   5 , 1 , -1 ,   6 , 2 , -1 ,   \ (area, set, path)
create entries  0 , 0 ,   \ (by exit 0: the camera)
: doorways-shade ( -- )
    -6.7e 20e 24.7e  -3.5e 20e 24.7e  -6.7e 0e 24.7e  -3.5e 0e 24.7e  light-doorway
    -3.5e 20e 24.8e  -6.7e 20e 24.8e  -3.5e 0e 24.8e  -6.7e 0e 24.8e  light-doorway
    12.7e 20e 24.7e  15.9e 20e 24.7e  12.7e 0e 24.7e  15.9e 0e 24.7e  light-doorway
    15.9e 20e 24.8e  12.7e 20e 24.8e  15.9e 0e 24.8e  12.7e 0e 24.8e  light-doorway ;
: each-frame ( -- )  0 exit-out  cameras 4 cameras-at  spots  doorways-shade ;
: person-in ( i -- )  fiona-p = if  entries 1 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-b1-3 place!
