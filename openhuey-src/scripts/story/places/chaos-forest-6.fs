\ story/places/chaos-forest-6.fs - a room of the Chaos Forest's maze ($104): the bush to crouch
\ in (story/places/forest.fs). A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/chaos-forest-6.fs (converted from the game's, not loaded).
IN: story.places.chaos-forest-6
USING: engine game-state actors messages common facts room-names story.kit story.places.forest ;

own-reset  forest-owns owns !

$19 constant forest-track
create entries  0 , 0 ,  0 , 0 ,  0 , 0 ,  1 , 1 ,   \ (by exit 0 .. 3: the camera)
create ahead  4 , 0 ,  5 , 1 ,  6 , 2 ,   \ (area, exit loaded ahead)
: came-in ( exit -- )  drop  room-sounds  3 $1D $E8 -180 3.4e 62.55e 3.84e 83.43e bush  leaves-still ;
: each-frame ( -- )  4 0 do  i i leaves-by  loop  ahead 3 ways-out  bush-frame ;
: person-in ( i -- )
    dup stalker-p = if  drop bush-searched exit  then
    fiona-p = if  entries 4 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went chaos-forest-6 place!
