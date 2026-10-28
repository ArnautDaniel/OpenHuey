\ story/places/off-map-d0.fs - the clock run's goal in the forest ($D0, chaos-forest-1's ground
\ again): two of its floor's triangles let through; its specks; Fiona reaching the goal's area - the
\ time kept and shown, the results, out to the extras room (story/places/clock-run.fs); the room
\ beyond exit 1 loaded ahead as Hewie nears it; out by exit 1; the clock. A place (story/kit.fs).
\ The original's scripts, for checking: story/rooms-ref/off-map-d0.fs in the history (converted from
\ the game's; gone with the old machinery in 0.2.20).
IN: story.places.off-map-d0
USING: engine game-state actors messages common flag-names facts room-names story-names floor-flags effects
    story.kit story.clock story.places.clock-run ;

own-reset
own: reached   \ Fiona has reached the goal (her and Hewie's reaction made): the time shown as kept

$19 constant forest-track   $10 constant goal-area
: came-in ( exit -- )
    drop  $16F solid-floor floor-tri-off  $174 solid-floor floor-tri-off  forest-track 1e background
    $10 $E $80 $80 $80 $40 rgba 54.3e 20.2e 154.5e specks   $10 $E $80 $80 $80 $40 rgba -53.5e 20.2e 154.5e specks ;

\ ---- every frame ----
create entries  3 , 3 ,  0 , 0 ,   \ (by exit 0, 1: the camera)
create cameras  2 , 0 , 0 ,  3 , 1 , 1 ,  4 , 1 , 1 ,  5 , 2 , 2 ,  6 , 4 , 4 ,  7 , 5 , 5 ,  8 , 3 , 3 ,
    9 , 3 , 3 ,  $A , 5 , -1 ,  $B , 2 , 2 ,  $C , 5 , 5 ,  $D , 4 , 4 ,  $E , 5 , -1 ,   \ (area, set, path)
: each-frame ( -- )
    1 1 leaves-by  cameras 13 cameras-at  2 1 his-way
    reached @ 0=  fiona goal-area in-area? and if  -1 reached !  true at-goal  then
    reached @ clock-draw-saved ;
: person-in ( i -- )  player person-of = if  entries 2 ways-in  then ;
: went ( -- )  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-d0 place!
