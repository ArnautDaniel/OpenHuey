\ story/places/off-map-d5.fs - the clock run's forest (, chaos-forest-11's crossing again): five
\ ways; a rabbit now and then, off into the trees; the wisp startled as she or Hewie comes near it;
\ the rooms ahead loaded; the clock.
\ A place (story/kit.fs, story/places/clock-run.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-d5.fs (converted from the game's, not loaded).
IN: story.places.off-map-d5
USING: engine game-state actors messages common flag-names facts room-names story-names story.kit story.clock
    story.places.forest story.places.clock-run ;

own-reset  forest-owns owns !

$19 constant forest-track
create entries  0 , 0 ,  3 , 3 ,  1 , 1 ,  2 , 2 ,  2 , 2 ,   \ (by exit 0 .. 4: the camera)
create cameras  $A , 3 , 3 ,  $B , 1 , 1 ,  $C , 3 , 3 ,  $D , 2 , 2 ,  $E , 0 , 0 ,  $F , 3 , 3 ,   \ (area, set, path)
create ahead  5 , 0 ,  6 , 1 ,  7 , 2 ,  8 , 3 ,  9 , 4 ,   \ (area, exit loaded ahead)
: rabbit ( -- )  1 180 3e 235e -6e 257e runs-off ;
: came-in ( exit -- )
    drop  room-sounds  forest-track 1e background
    30 chance? if  rabbit-kind rabbit-slot figure-in  ['] rabbit start drop  then
    x-d5-flag1 fact? 0= if  -21.17e 31.84e -137.07e 1e wisp-in  then ;
: near-wisp? ( id -- flag )  -24e -132e 25e reaches-across? ;
: each-frame ( -- )
    5 by-areas  cameras 6 cameras-at  ahead 5 his-ways
    wisp-up @  x-d5-flag1 fact? 0= and  fiona near-wisp?  hewie near-wisp? or and if  x-d5-flag1 startled  then
    clock-shown ;
: person-in ( i -- )  player person-of = if  entries 5 ways-in  then ;
: went ( -- )  rabbit-slot figure-out  wisp-slot figure-out  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-d5 place!
