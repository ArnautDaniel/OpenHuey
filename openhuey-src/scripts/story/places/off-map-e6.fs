\ story/places/off-map-e6.fs - the clock run's castle ($E6, castle-1f-7's passage again): its barred
\ floor; the lattice down out of the way; its parts; two ways out (one by an area); its cameras; the
\ rooms ahead loaded as Hewie nears their ways; the walls' shade; the clock. A place (story/kit.fs,
\ story/places/clock-run.fs). The original's scripts, for checking: story/rooms-ref/off-map-e6.fs in
\ the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.off-map-e6
USING: engine game-state actors messages common flag-names facts room-names floor-flags story.kit
    story.clock story.places.clock-run ;

$34 constant speed   \ (the lattice's fall: its object's +$34)
: lattice-down ( -- )   \ ("kousi": down out of the way)
    s" kousi" placed-here? 0= if  exit  then  -25.3e s" kousi" place-y object!  0e s" kousi" speed object! ;
: came-in ( exit -- )  drop  0 barred-floor floor-on  lattice-down  1 object-hide  0 show-part  1 hide-part ;

\ ---- every frame ----
create cameras  4 , 0 , 0 ,  7 , 0 , 0 ,   \ (area, set, path)
: walls-shade ( -- )
    33e 23e -5.9e  45e 23e -5.9e  33e 11e -5.9e  45e 11e -5.9e  light-doorway
    33e 11e -5.9e  45e 11e -5.9e  33e 0e -5.9e  45e 0e -5.9e  light-doorway
    45e 23e -5.1e  33e 23e -5.1e  45e 0e -5.1e  33e 0e -5.1e  light-doorway ;
: each-frame ( -- )
    0 exit-out  1 1 leaves-by  cameras 2 cameras-at  5 1 -1 camera-out  6 1 -1 camera-out
    2 0 his-way-back  3 1 his-way  walls-shade  clock-shown ;
: person-in ( i -- )  player person-of = if  0 0 0 way-in  1 0 0 way-in  then ;
: went ( -- )  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-e6 place!
