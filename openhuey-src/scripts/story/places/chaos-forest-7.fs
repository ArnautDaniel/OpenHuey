\ story/places/chaos-forest-7.fs - a room of the Chaos Forest's maze ($105): the bush to crouch in
\ (story/places/forest.fs). A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/chaos-forest-7.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.chaos-forest-7
USING: engine game-state actors messages common facts room-names story.kit story.places.forest ;

own-reset  forest-owns owns !

$19 constant forest-track
create entries  1 , 1 ,  2 , 2 ,  2 , 2 ,  0 , 0 ,   \ (by exit 0 .. 3: the camera)
create ahead  5 , 1 ,  6 , 2 ,  7 , 3 ,   \ (area, exit loaded ahead)
create cameras  8 , 0 , 0 ,  9 , 2 , 2 ,   \ (area, set, path)
: came-in ( exit -- )  drop  room-sounds  forest-track 1e background  0 $1E $E0 0 -4.49e -66.28e -2.89e -89.23e bush  leaves-still ;
: each-frame ( -- )  4 0 do  i i leaves-by  loop  cameras 2 cameras-at  ahead 3 ways-out  bush-frame ;
: person-in ( i -- )
    dup stalker-p = if  drop bush-searched exit  then
    fiona-p = if  entries 4 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went chaos-forest-7 place!
