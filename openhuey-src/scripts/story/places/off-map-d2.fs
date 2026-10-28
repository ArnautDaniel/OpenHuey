\ story/places/off-map-d2.fs - the clock run's forest ($D2): four ways, each by its own area; the
\ room behind each loaded ahead as Hewie nears it; the clock. A place (story/kit.fs,
\ story/places/clock-run.fs). The original's scripts, for checking: story/rooms-ref/off-map-d2.fs in
\ the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.off-map-d2
USING: engine game-state actors messages common flag-names facts room-names story.kit story.clock
    story.places.clock-run ;

: came-in ( exit -- )  drop ;

\ ---- every frame ----
create entries  1 , 1 ,  1 , 1 ,  0 , 0 ,  0 , 0 ,   \ (by exit 0 .. 3: the camera)
create ahead  4 , 0 ,  5 , 1 ,  6 , 2 ,  7 , 3 ,   \ (area, exit loaded ahead)
: each-frame ( -- )  4 by-areas  ahead 4 his-ways  clock-shown ;
: person-in ( i -- )  player person-of = if  entries 4 ways-in  then ;
: went ( -- )  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-d2 place!
