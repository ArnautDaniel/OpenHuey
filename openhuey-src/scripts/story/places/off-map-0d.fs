\ story/places/off-map-0d.fs - room $D (on no map page): in by exit 1, out by it from area 0; her
\ camera by the floor she stands on. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-0d.fs (converted from the game's, not loaded).
IN: story.places.off-map-0d
USING: engine game-state actors messages common flag-names room-names story.kit ;

: came-in ( exit -- )  drop  1 $3FFF reverb ;

\ ---- every frame ----
: on-any? ( tri1 tri2 -- flag )  fiona swap stands-on?  fiona rot stands-on? or ;
: floor-cameras ( -- )   \ (her camera by the triangle she is on: each frame, as the original)
    $BB $1C7 on-any? if  2 -1 player-camera  then
    $1D0 $E0 on-any? if  4 -1 player-camera  then
    $1F1 $1ED on-any?  fiona $EA stands-on? or if  5 -1 player-camera  then ;
: each-frame ( -- )  0 1 leaves-by  floor-cameras ;
: person-in ( i -- )  player person-of <> if  exit  then  1 5 -1 way-in ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went off-map-0d place!
