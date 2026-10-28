\ story/places/chaos-forest-1.fs - the Chaos Forest's edge ($67): the hopping figure (the kit's
\ hopper - struck it hops back, the fifth time it lashes out) and its calls; the Forest Map on the
\ stump; Hewie wary of the figure, his look at it; the way on before Hewie is found - into
\ chaos-forest-8's scene. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/chaos-forest-1.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.chaos-forest-1
USING: engine game-state actors messages common facts room-names flag-names story-names items.names
    effects story.kit fiona.anims ;

own-reset

own: calls                 \ frames to the figure's next call
own: wary-noticed          \ Hewie has been wary of the figure (once a visit)
own: going-on              \ the way on taken: the figure left to the scene
$19 constant forest-track   0 constant map-part   $F constant map-area   1 constant way-on-area
2 constant first-call   $E constant scene-figure   $F constant scene-figure-2
$9008 $9009 0 1  22e -16e 145e 0e  hopper: the-figure

: hopping ( -- )  the-figure true hops-about ;
: music-after ( -- )   \ (the track once the figure is in)
    begin  hopper-slot figure 0<  while  yield  repeat  forest-track 1e background ;
: came-in ( exit -- )
    drop  room-sounds  ['] music-after start drop  ['] hopping start drop
    hunted state!
    got-forest-map fact? 0= if  map-part show-part  then
    $10 $E $80 $80 $80 $40 rgba 54.3e 20.2e 154.5e specks   $10 $E $80 $80 $80 $40 rgba -53.5e 20.2e 154.5e specks ;

\ ---- the way on, before Hewie is found: the scene's figures loaded, out by exit 1 ----
: goes-on ( -- )
    stalkers-stay state!  fiona hold  -1 going-on !
    ['] hopping stop-doing  hopper-slot figure-out
    scene-figure 3 figure-in  scene-figure-2 4 figure-in
    15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade  world-held state!  1 take-exit ;

\ ---- the Forest Map ----
: takes-map ( -- )
    fiona wait-move  fiona -324e 322e turn-to-xz
    fiona reach-out-anim play  4 frames  map-part hide-part
    forest-map got-forest-map -1 takes-the  fiona reach-back-anim play ;

\ ---- Hewie: wary of the figure (once a visit), his look at it ----
: wary ( -- )  22.39e -16e 144.46e hewie-stands-by ;
create figure-look  22.39e -16e 144.46e  21e 15e 15e look,
: hewie-here ( -- )
    wary-noticed 80 22.39e -16e 144.46e 21e 15e notices? if  ['] wary start drop  then
    figure-look 1 danger-now @ followed = hewie-looks-if ;

create cameras  2 , 0 , 0 ,  3 , 1 , 1 ,  4 , 1 , 1 ,  5 , 2 , 2 ,  6 , 4 , 4 ,  7 , 5 , 5 ,  8 , 3 , 3 ,
    9 , 3 , 3 ,  $A , 5 , -1 ,  $B , 2 , 2 ,  $C , 5 , 5 ,  $D , 4 , 4 ,  $E , 5 , -1 ,   \ (area, set, path)
create ahead  3 , 0 ,  2 , 1 ,   \ (area, exit loaded ahead)
: ways ( -- )
    0 0 leaves-by
    found-file-247 fact? if  way-on-area 1 leaves-by
    else  fiona way-on-area in-area?  going-on @ 0= and if  ['] goes-on start drop  then  then
    cameras 13 cameras-at  ahead 2 ways-out ;
: each-frame ( -- )
    ways  the-figure calls first-call hopper-calls  hewie-here
    got-forest-map fact? 0=  fiona map-area in-area? and  fiona 180 50 heading? and if  ['] takes-map calm-only offer  then ;
create entries  3 , 3 ,  0 , 0 ,   \ (by exit 0, 1: the camera)
: person-in ( i -- )  fiona-p = if  entries 2 ways-in  then ;
: went ( -- )  going-on @ 0= if  hopper-slot figure-out  then ;

' came-in ' person-in ' each-frame ' went chaos-forest-1 place!
