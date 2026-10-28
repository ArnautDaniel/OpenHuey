\ story/places/off-map-c4.fs - room $C4 (on no map page): in by exit 1, out by it from area 1; its
\ cameras; the rooms beyond exits 0 and 1 loaded ahead as she nears them. A place (story/kit.fs).
\ The original's scripts, for checking: story/rooms-ref/off-map-c4.fs (converted from the game's,
\ not loaded).
IN: story.places.off-map-c4
USING: engine game-state actors messages common flag-names room-names story.kit ;

: came-in ( exit -- )  drop ;

\ ---- every frame ----
create cameras  2 , 0 , 0 ,  3 , 1 , 1 ,  4 , 1 , 1 ,  5 , 2 , 2 ,  6 , 3 , 3 ,  7 , 2 , 2 ,  8 , 2 , 2 ,
    9 , 4 , 4 ,  $A , 2 , 2 ,  $B , 5 , 5 ,  $C , 4 , 4 ,  $D , 5 , 5 ,  $E , 0 , 0 ,  $F , 6 , 6 ,   \ (area, set, path)
create ahead  4 , 0 ,  5 , 1 ,   \ (area, exit loaded ahead)
: each-frame ( -- )  1 1 leaves-by  cameras 14 cameras-at  ahead 2 ways-out ;
: person-in ( i -- )  player person-of <> if  exit  then  1 5 5 way-in ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went off-map-c4 place!
