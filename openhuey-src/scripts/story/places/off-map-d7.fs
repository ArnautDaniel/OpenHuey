\ story/places/off-map-d7.fs - the clock run's forest (, chaos-forest-9's lakeside again): the
\ wisp hovering (never startled here: the original's hover never marks its scene begun, so its
\ cry never comes); the tower's light far off; out by exit 0; the clock.
\ A place (story/kit.fs, story/places/clock-run.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-d7.fs (converted from the game's, not loaded).
IN: story.places.off-map-d7
USING: engine game-state actors messages common flag-names facts room-names story-names effects story.kit
    story.clock story.places.forest story.places.clock-run ;

own-reset  forest-owns owns !

$19 constant forest-track
: far-light ( -- )  $C $80808080  -225e -63.62e -1125e  1400e 0.5e 0.1e  effects:caustic drop ;   \ (the tower's light far off, turning)
: came-in ( exit -- )
    drop  room-sounds  forest-track 1e background
    x-d7-flag1 fact? 0= if  -339.86e 125.95e -391.22e 2e wisp-in  then
    far-light ;
: each-frame ( -- )  0 0 leaves-by  clock-shown ;
: person-in ( i -- )  player person-of = if  0 0 0 way-in  then ;
: went ( -- )  wisp-slot figure-out  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-d7 place!
