\ story/places/chaos-forest-5.fs - a room of the Chaos Forest's maze ($103): the bush to crouch
\ in (story/places/forest.fs). A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/chaos-forest-5.fs (converted from the game's, not loaded).
IN: story.places.chaos-forest-5
USING: engine game-state actors messages common facts room-names story.kit story.places.forest ;

own-reset  forest-owns owns !

$19 constant forest-track
create entries  0 , 0 ,  1 , 1 ,  0 , 0 ,  0 , 0 ,   \ (by exit 0 .. 3: the camera)
create ahead  4 , 0 ,  6 , 2 ,  7 , 3 ,   \ (area, exit loaded ahead)
: came-in ( exit -- )  drop  room-sounds  forest-track 1e background  1 $1C $E7 -90 70.5e -5.5e 90.5e -4.5e bush  leaves-still ;
: each-frame ( -- )  4 0 do  i i leaves-by  loop  ahead 3 ways-out  bush-frame ;
: person-in ( i -- )
    dup stalker-p = if  drop bush-searched exit  then
    fiona-p = if  entries 4 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went chaos-forest-5 place!
