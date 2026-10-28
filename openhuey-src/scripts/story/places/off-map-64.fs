\ story/places/off-map-64.fs - room $64 (on no map page): two ways through, each room beyond loaded
\ ahead as she nears it. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-64.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.off-map-64
USING: engine game-state actors messages common flag-names room-names story.kit ;

: came-in ( exit -- )  drop ;

\ ---- every frame ----
create entries  0 , -1 ,  0 , -1 ,   \ (by exit 0, 1: the camera)
create ahead  2 , 0 ,  3 , 1 ,       \ (area, exit loaded ahead)
: each-frame ( -- )  2 exits-out  ahead 2 ways-out ;
: person-in ( i -- )  player person-of <> if  exit  then  entries 2 ways-in ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went off-map-64 place!
