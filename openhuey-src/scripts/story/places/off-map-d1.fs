\ story/places/off-map-d1.fs - the clock run's forest (, chaos-forest-8's ground again): a drop
\ falling now and then; its two ways - as Hewie nears the way on, a rabbit and the wisp loaded at
\ it (gone again as he turns back); the rooms ahead loaded; the clock.
\ A place (story/kit.fs, story/places/clock-run.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-d1.fs (converted from the game's, not loaded).
IN: story.places.off-map-d1
USING: engine game-state actors messages common flag-names facts room-names effects story.kit story.clock
    story.places.clock-run ;

$19 constant forest-track   $1A constant ahead-figure   $E constant wisp   4 constant ahead-slot   5 constant wisp-slot
: came-in ( exit -- )  drop  room-sounds  forest-track 1e background  effects:one-drip ;

\ ---- every frame ----
create entries  0 , 0 ,  1 , 1 ,   \ (by exit 0, 1: the camera)
create cameras  4 , 0 , 0 ,  5 , 1 , 1 ,   \ (area, set, path)
: way-on ( -- )   \ (Hewie nearing it: what is beyond loaded at it; back: gone)
    hewie 3 entered? if  ahead-figure ahead-slot 1 figure-in-by  wisp wisp-slot 1 figure-in-by  1 prepare-exit  then
    hewie 3 left? if  ahead-slot figure-out  wisp-slot figure-out  then ;
: each-frame ( -- )  2 by-areas  cameras 2 cameras-at  2 0 his-way  way-on  clock-shown ;
: person-in ( i -- )  player person-of = if  entries 2 ways-in  then ;
: went ( -- )  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-d1 place!
