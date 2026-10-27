\ story/places/old-mansion-2f-1.fs - the old mansion's 2F landing ($45): the curtain, the barred
\ way; once its floor has fallen (old-mansion-b1-5's flag), her remark at the gap from either side.
\ A place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-2f-1.fs
\ (converted from the game's, not loaded).
IN: story.places.old-mansion-2f-1
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags story.kit ;

own-reset

0 constant collapsed-msg   \ "The floor has collapsed! But I literally just crossed through here seconds ago..."
1 constant curtain-part   1 constant barred-group   3 constant halves-group

: came-in ( exit -- )
    drop  curtain-part show-part  barred-group barred-floor floor-on  halves-group both-halves floor-on ;
: collapsed ( deg -- )  fiona wait-move  fiona swap turn  collapsed-msg say ;
: from-here ( -- )  0 collapsed ;
: from-there ( -- )  180 collapsed ;
create entries  2 , 2 ,   \ (by exit 0: the camera)
: each-frame ( -- )
    0 exit-out
    omb1-5-flag3 fact? if
        fiona $10 in-area?  fiona 0 50 heading? and if  ['] from-here unhurried offer  then
        fiona $11 in-area?  fiona 180 50 heading? and if  ['] from-there unhurried offer  then
    then ;
: person-in ( i -- )  fiona-p = if  entries 1 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-2f-1 place!
