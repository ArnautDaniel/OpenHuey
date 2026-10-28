\ story/places/off-map-e1.fs - the clock run's castle ($E1, castle-1f-5's ground again): the day's
\ light in the run's second kind; three flames, each stirred as someone steps into its draught; two
\ ways out; its cameras; the rooms ahead loaded as Hewie nears their ways; the walls' shade; the
\ clock. A place (story/kit.fs, story/places/clock-run.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-e1.fs (converted from the game's, not loaded).
IN: story.places.off-map-e1
USING: engine game-state actors messages common flag-names facts room-names story.kit story.clock
    story.places.clock-run ;

$2E constant run-kind-var
: daylight ( -- )  run-kind-var var@ 1 = if  $84 $60 $3C $50 $24 $2A $2A $46 room-tint  then ;
create flames  6e 21.8e -3.2e f, f, f,   29e 41.8e 41.9e f, f, f,   -46.8e 93.8e 18.5e f, f, f,
: flame@ ( k -- ) ( F: -- x y z )  3 * floats flames +  dup 2 floats + f@  dup float+ f@  f@ ;
: flames-lit ( -- )  3 0 do  i 0  i flame@  flame-at  loop ;
: draughts ( -- )  3 0 do  i  i flame@  8e -4e  flame-stirred  loop ;
: came-in ( exit -- )  drop  daylight  flames-lit  1 $2300 reverb ;

\ ---- every frame ----
create cameras  7 , 2 , 2 ,  8 , 1 , 1 ,  9 , 0 , 0 ,  $A , 1 , 1 ,  $D , 0 , 0 ,  $15 , 3 , -1 ,  $16 , 4 , 3 ,  $17 , 3 , -1 ,   \ (area, set, path)
: walls-shade ( -- )
    -8e 82e -5e  -8e 82e 45e  -8e 30e -5e  -8e 30e 45e  light-doorway
    -5.3e 54e -4.1e  -5.3e 54e 47e  -58e 54e -4.1e  -58e 54e 47e  light-doorway
    15e 30e -3e  45e 30e -3e  15e 15e -3e  45e 15e -3e  light-doorway
    15e 15e -3e  45e 15e -3e  15e 0e -3e  45e 0e -3e  light-doorway
    -11e 30e -3e  15e 30e -3e  -11e 15e -3e  15e 15e -3e  light-doorway
    -11e 15e -3e  15e 15e -3e  -11e 0e -3e  15e 0e -3e  light-doorway ;
: each-frame ( -- )
    1 exit-out  2 exit-out  cameras 8 cameras-at
    hewie 4 left?  hewie 5 entered? or if  2 prepare-exit  then  6 1 his-way-back
    draughts  walls-shade  clock-shown ;
: person-in ( i -- )  player person-of = if  1 0 0 way-in  2 4 3 way-in  then ;
: went ( -- )  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-e1 place!
