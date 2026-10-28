\ story/places/off-map-65.fs - room $65 (on no map page): three ways through, each room beyond
\ loaded ahead as she nears it. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-65.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.off-map-65
USING: engine game-state actors messages common flag-names room-names story.kit ;

: came-in ( exit -- )  drop ;

\ ---- every frame ----
create entries  0 , -1 ,  0 , -1 ,  0 , -1 ,   \ (by exit 0, 1, 2: the camera)
create ahead  3 , 0 ,  4 , 1 ,  5 , 2 ,        \ (area, exit loaded ahead)
: each-frame ( -- )  3 exits-out  ahead 3 ways-out ;
: person-in ( i -- )  player person-of <> if  exit  then  entries 3 ways-in ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went off-map-65 place!
