\ story/places/water-tower-7f-2.fs - the Water Tower's 7F, past the planets ($C6): come in after the
\ planets' scene (by the forced way: Hewie gone ahead to his spot, the sounds silenced, the world
\ let go and faded in - the other game's file); back by the second forced way (Hewie by her); the
\ clock; the inscription; the Rubrum Medallion Hewie sniffs out; hunted while she's here; its four
\ sounds in turn; the map's pages. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/water-tower-7f-2.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.water-tower-7f-2
USING: engine game-state actors messages common facts room-names flag-names story-names
    items.names story.kit fiona.anims ;

0 constant inscription-msg   \ "All life on the surface of the planet stretches out below the stars' radiant light. All..."
0 constant medallion-fx   5 constant first-ambient   $30 constant hall-track   $828A constant planets-file
$C constant clock-area   $E constant inscription-area   $F constant map-7-area
$81 constant back-way   \ (the second forced way in)
: came-in ( exit -- )
    drop  room-sounds  hunted state!  hall-track 1e background
    wt7f-2-rubrum-medallion-found fact?  wt7f-2-rubrum-medallion fact? 0= and if  medallion-fx -9e 140.97e -10.23e flicker-at  then ;

\ ---- coming in by the forced ways ----
: after-planets ( -- )   \ (an activity: the next room ahead, the world let go, faded in)
    yield  camera-restart  1 prepare-exit  16 frames
    world-held state-off  15 fade-in  planets-file give-file  stalkers-stay state-off ;
: back-again ( -- )   \ (an activity: the world let go, faded in, she shown)
    world-held state-off  15 fade-in  wait-fade  stalkers-stay state-off  fiona true shown ;

\ ---- what she may do or say here ----
: the-inscription ( -- )
    stalkers-stay state!  fiona wait-move  fiona 0e -105e turn-to-xz  fiona look-close-anim start-anim  40 frames
    15 screen:to-dim fade wait-fade  inscription-msg say  15 screen:from-dim fade wait-fade  fiona wait-anim
    stalkers-stay state-off ;
: pick-medallion ( -- )  rubrum-medallion wt7f-2-rubrum-medallion medallion-fx -9e -10.23e picks-up ;
: medallion-sniffed ( -- )  wt7f-2-rubrum-medallion-found wt7f-2-rubrum-medallion medallion-fx  -9e 140.97e -10.23e  hewie-finds ;
: spots ( -- )
    fiona clock-area 50 faces-area? if  ['] save-clock calm-hewie-up offer  then
    fiona inscription-area 50 faces-area? if  ['] the-inscription unhurried offer  then
    wt7f-2-rubrum-medallion-found fact?  wt7f-2-rubrum-medallion fact? 0= and  fiona -9e 139.97e -10.23e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then ;

\ ---- every frame ----
create entries  3 , 1 ,  0 , -1 ,   \ (by exit 0, 1: the camera)
create cameras  4 , 0 , -1 ,  5 , 1 , -1 ,  6 , 1 , -1 ,  7 , 1 , -1 ,  8 , 1 , -1 ,  9 , 2 , 0 ,  $A , 2 , 0 ,  $B , 3 , 1 ,   \ (area, set, path)
create ahead  2 , 0 ,  3 , 1 ,   \ (area, exit loaded ahead)
: each-frame ( -- )
    0 exit-out  1 1 leaves-by  cameras 8 cameras-at  ahead 2 ways-out
    fiona map-7-area entered? if  7 map-page  then   fiona map-7-area left? if  8 map-page  then
    wt7f-2-rubrum-medallion-found fact? 0=  scrambling? and  hewie -9e 139.97e -10.23e 10e 5e touches? and if  ['] medallion-sniffed start-only  then
    room-sounds-in? if  first-ambient -127e 1.7e 10e ambient-sounds  then
    spots ;
: person-in ( i -- )
    case
        fiona-p of
            entries 2 ways-in
            came-by @ forced-exit = if
                fiona $1C2 105 -45.44e -52.11e place-at  1 -1 player-camera  0e sounds-scaled  ['] after-planets fiona start-holding drop
            then
            came-by @ back-way = if
                fiona activate  fiona 0 at-exit  3 1 player-camera
                hewie activate  this-room @ 836 0 hewie-to  hewie $344 0 6.81e -30.23e place-at
                0e sounds-scaled  ['] back-again fiona start-holding drop
            then
            came-by @ 0= if  8 map-page  then
        endof
        hewie-p of  came-by @ forced-exit = if  hewie $6D -30 -14.34e -72.11e place-at  then  endof
    endcase ;
: went ( -- )  hunted state-off ;

' came-in ' person-in ' each-frame ' went water-tower-7f-2 place!
