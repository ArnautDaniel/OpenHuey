\ story/places/off-map-d3.fs - the clock run's forest (): four ways, each by its own area; its
\ cameras; the room behind each way loaded ahead as Hewie nears it; the clock.
\ A place (story/kit.fs, story/places/clock-run.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-d3.fs (converted from the game's, not loaded).
IN: story.places.off-map-d3
USING: engine game-state actors messages common flag-names facts room-names story.kit story.clock
    story.places.clock-run ;

$19 constant forest-track
: came-in ( exit -- )  drop  forest-track 1e background ;

\ ---- every frame ----
create entries  2 , 2 ,  1 , 1 ,  1 , 1 ,  3 , 3 ,   \ (by exit 0 .. 3: the camera)
create cameras  8 , 2 , 2 ,  9 , 0 , 0 ,  $A , 0 , 0 ,  $B , 3 , 3 ,  $C , 0 , 0 ,  $D , 4 , 4 ,   \ (area, set, path)
create ahead  4 , 0 ,  5 , 1 ,  6 , 2 ,  7 , 3 ,   \ (area, exit loaded ahead)
: each-frame ( -- )  4 by-areas  cameras 6 cameras-at  ahead 4 his-ways  clock-shown ;
: person-in ( i -- )  player person-of = if  entries 4 ways-in  then ;
: went ( -- )  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-d3 place!
