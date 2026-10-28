\ story/places/chaos-forest-12.fs - the Chaos Forest's ($10A) way to its heart: the Camomile; the
\ wisp (story/places/forest.fs) startled by a loud noise; once Hewie is found shot, the stalker
\ loaded ahead as she heads for the way on (exit 2); the maze dealt. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/chaos-forest-12.fs in the history (converted
\ from the game's; gone with the old machinery in 0.2.20).
IN: story.places.chaos-forest-12
USING: engine game-state actors messages common facts room-names flag-names story-names items.names
    story.kit story.places.forest ;

own-reset  forest-owns owns !

$19 constant forest-track   0 constant camomile-fx   $17 constant him-ahead   3 constant ahead-slot
create entries  1 , 1 ,  0 , 0 ,  0 , 0 ,  0 , 0 ,   \ (by exit 0 .. 3: the camera)
create cameras  $C , 0 , 0 ,  $D , 1 , 1 ,   \ (area, set, path)
: came-in ( exit -- )
    drop  room-sounds  forest-track 1e background
    cf-12-flag1 fact? 0= if  -25.61e 38.13e -136.96e 1e wisp-in  then
    maze-in
    cf-12-camomile fact? 0= if  camomile-fx -253.83e 5.54e -217.28e flicker-at  then ;
: his-scene-ahead? ( -- flag )  found-file-247 fact?  cf-13-flag1 fact? 0= and ;   \ (the forest's heart's scene to come)
: him-loaded ( -- )  his-scene-ahead? if  him-ahead ahead-slot figure-in  then ;
: ways ( -- )
    0 0 leaves-by  1 1 leaves-by  3 3 leaves-by
    fiona 2 in-area?  fiona held? 0= and if  him-loaded  2 2 leaves-by  then
    cameras 2 cameras-at
    fiona 4 entered? if  0 prepare-exit  then
    fiona 5 entered?  fiona 6 entered? or  fiona 8 entered? or if  1 prepare-exit  then
    fiona 7 entered? if  him-loaded  2 prepare-exit  then
    fiona 9 entered? if  3 prepare-exit  then
    fiona 7 left?  his-scene-ahead? and if  ahead-slot figure-out  then ;
: pick-camomile ( -- )  camomile cf-12-camomile camomile-fx -253.83e -217.28e picks-up ;
: each-frame ( -- )
    ways  maze-frame
    wisp-up @  cf-12-flag1 fact? 0= and  loud-here? and if  cf-12-flag1 startled  then
    cf-12-camomile fact? 0=  fiona -253.83e 5.54e -217.28e 5e 5e touches? and if  ['] pick-camomile calm-only offer  then ;
: person-in ( i -- )  fiona-p = if  entries 4 ways-in  then ;
: went ( -- )  wisp-slot figure-out ;

' came-in ' person-in ' each-frame ' went chaos-forest-12 place!
