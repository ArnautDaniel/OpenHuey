\ story/places/chaos-forest-2.fs - a room of the Chaos Forest's maze ($100): its four ways. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/chaos-forest-2.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.chaos-forest-2
USING: engine game-state actors messages common facts room-names story.kit ;

own-reset

$19 constant forest-track
create entries  1 , 1 ,  0 , 0 ,  1 , 1 ,  0 , 0 ,   \ (by exit 0 .. 3: the camera)
create ahead  4 , 0 ,  5 , 1 ,  6 , 2 ,  7 , 3 ,   \ (area, exit loaded ahead)
: came-in ( exit -- )  drop  forest-track 1e background ;
: each-frame ( -- )  4 0 do  i i leaves-by  loop  ahead 4 ways-out ;
: person-in ( i -- )  fiona-p = if  entries 4 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went chaos-forest-2 place!
