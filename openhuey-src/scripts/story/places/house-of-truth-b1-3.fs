\ story/places/house-of-truth-b1-3.fs - a corridor of the House of Truth's B1 ($86): the first time
\ in, Lorenzo leaves her be (the summoner off); its echo; his slam's shake. A place (story/kit.fs).
\ The original's scripts, for checking: story/rooms-ref/house-of-truth-b1-3.fs in the history
\ (converted from the game's; gone with the old machinery in 0.2.20). Never run in the original: its
\ act00 (nothing in it).
IN: story.places.house-of-truth-b1-3
USING: engine game-state actors messages common facts room-names flag-names story-names story.kit
    story.places.house-of-truth ;

own-reset  truth-owns owns !

create entries  1 , 1 ,  0 , 0 ,   \ (by exit 0, 1: the camera)
create cameras  8 , 0 , 0 ,  9 , 1 , 1 ,   \ (area, set, path)
create ahead  4 , 0 ,  5 , 1 ,   \ (area, exit loaded ahead)
: came-in ( exit -- )
    drop
    htb1-3-flag1 fact? 0= if  summoner-on state-off  htb1-3-flag1 fact!  stalker let-go  1 summon-take  then
    1 $2300 reverb ;
: each-frame ( -- )  0 exit-out  1 1 leaves-by  cameras 2 cameras-at  ahead 2 ways-out  slam-shake ;
: person-in ( i -- )  fiona-p = if  entries 2 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went house-of-truth-b1-3 place!
