\ story/places/off-map-e8.fs - the clock run's castle ($E8, castle-1f-14's ground again): two ways out;
\ its cameras; the rooms ahead loaded as Hewie nears their ways; the walls' shade; the clock. A place
\ (story/kit.fs, story/places/clock-run.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-e8.fs (converted from the game's, not loaded).
IN: story.places.off-map-e8
USING: engine game-state actors messages common flag-names facts room-names story.kit story.clock
    story.places.clock-run ;

: came-in ( exit -- )  drop  1 $2300 reverb ;

\ ---- every frame ----
create cameras  6 , 0 , 0 ,  7 , 0 , 0 ,  8 , 2 , 2 ,  9 , 3 , 3 ,  $A , 0 , 0 ,  $B , 1 , 1 ,  $C , 1 , 1 ,
    $D , 2 , 2 ,  $E , 2 , 2 ,  $F , 4 , 4 ,   \ (area, set, path)
create ahead  4 , 1 ,  5 , 2 ,   \ (area, exit loaded ahead)
: walls-shade ( -- )
    30e 54.1e -51.5e  -45e 54.1e -51.5e  30e 54.1e -90e  -45e 54.1e -90e  light-doorway
    70e 54.1e -23e  57e 54.1e -56e  100e 54.1e -30e  80e 54.1e -65e  light-doorway
    77e 54.1e 18e  70e 54.1e -23e  100e 54.1e 30e  100e 54.1e -30e  light-doorway
    52e 54.1e 48e  77e 54.1e 18e  82e 54.1e 65e  100e 54.1e 30e  light-doorway
    -78e 54.1e -20e  -77e 54.1e 20e  -100e 54.1e -25e  -100e 54.1e 20e  light-doorway
    -60e 54.1e -50e  -78e 54.1e -20e  -80e 54.1e -60e  -100e 54.1e -25e  light-doorway
    65e 20e 18e  75e 20e 13e  65e 0e 18e  75e 0e 13e  light-doorway
    74.5e 20e -19.5e  68.5e 20e -24.5e  74.5e 0e -19.5e  68.5e 0e -24.5e  light-doorway
    53e 20e -48e  53e 20e -60e  53e 0e -48e  53e 0e -60e  light-doorway
    -55e 20e -62e  -55e 20e -47e  -55e 0e -62e  -55e 0e -47e  light-doorway
    -70e 20e -26e  -78e 20e -22e  -70e 0e -26e  -78e 0e -22e  light-doorway
    -78e 20e 20e  -69e 20e 25e  -78e 0e 20e  -69e 0e 25e  light-doorway
    -55e 20e 47e  -55e 20e 54e  -55e 0e 47e  -55e 0e 54e  light-doorway
    52e 20e 57e  52e 20e 47e  52e 0e 57e  52e 0e 47e  light-doorway ;
: each-frame ( -- )  1 exit-out  2 exit-out  cameras 10 cameras-at  ahead 2 his-ways  walls-shade  clock-shown ;
: person-in ( i -- )  player person-of = if  1 0 0 way-in  2 0 0 way-in  then ;
: went ( -- )  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-e8 place!
