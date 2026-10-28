\ story/places/chaos-forest-4.fs - a room of the Chaos Forest's maze ($102): the bush to crouch in
\ (story/places/forest.fs). A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/chaos-forest-4.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.chaos-forest-4
USING: engine game-state actors messages common facts room-names story.kit story.places.forest ;

own-reset  forest-owns owns !

$19 constant forest-track
create entries  0 , 0 ,  0 , 0 ,  1 , 1 ,  0 , 0 ,   \ (by exit 0 .. 3: the camera)
create ahead  4 , 0 ,  5 , 1 ,  7 , 3 ,   \ (area, exit loaded ahead)
: came-in ( exit -- )  drop  room-sounds  forest-track 1e background  2 $1B $E0 90 -61.3e 0.24e -83.04e 1.02e bush  leaves-still ;
: each-frame ( -- )  4 0 do  i i leaves-by  loop  ahead 3 ways-out  bush-frame ;
: person-in ( i -- )
    dup stalker-p = if  drop bush-searched exit  then
    fiona-p = if  entries 4 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went chaos-forest-4 place!
