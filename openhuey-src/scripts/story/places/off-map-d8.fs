\ story/places/off-map-d8.fs - the clock run's forest (, chaos-forest-12's ground again): four ways,
\ each by its own area; the wisp startled by a loud noise here; its cameras; the rooms ahead loaded
\ as Hewie nears them; the clock. A place (story/kit.fs, story/places/clock-run.fs). The original's
\ scripts, for checking: story/rooms-ref/off-map-d8.fs in the history (converted from the game's;
\ gone with the old machinery in 0.2.20).
IN: story.places.off-map-d8
USING: engine game-state actors messages common flag-names facts room-names story-names story.kit story.clock
    story.places.forest story.places.clock-run ;

own-reset  forest-owns owns !

$19 constant forest-track
create entries  1 , 1 ,  0 , 0 ,  0 , 0 ,  0 , 0 ,   \ (by exit 0 .. 3: the camera)
create cameras  $C , 0 , 0 ,  $D , 1 , 1 ,   \ (area, set, path)
create ahead  4 , 0 ,  5 , 1 ,  6 , 1 ,  8 , 1 ,  7 , 2 ,  9 , 3 ,   \ (area, exit loaded ahead)
: came-in ( exit -- )
    drop  room-sounds  forest-track 1e background
    x-d8-flag1 fact? 0= if  -25.61e 38.13e -136.96e 1e wisp-in  then ;
: each-frame ( -- )
    4 by-areas  cameras 2 cameras-at  ahead 6 his-ways
    wisp-up @  x-d8-flag1 fact? 0= and  loud-here? and if  x-d8-flag1 startled  then
    clock-shown ;
: person-in ( i -- )  player person-of = if  entries 4 ways-in  then ;
: went ( -- )  wisp-slot figure-out  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-d8 place!
