\ story/places/water-tower-1f-1.fs - the Water Tower's 1F ($C1): the water (a recovery point, once a
\ visit; then its second remark); the clock; the Medallion Hewie sniffs out; Riccardo taken off when
\ his time is up and she can't see him; its four sounds in turn; the holes in its floor; the
\ doorway's shade. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/water-tower-1f-1.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.water-tower-1f-1
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit fiona.anims ;

0 constant water-msg       \ "I wonder if this water is safe to drink... Guess I'll never know if I don't try."
1 constant helped-msg      \ "Ahh, that helped."
own: rested                \ she has drunk here (once a visit)
0 constant medallion-fx   1 constant calm-sound   0 constant her-motions   5 constant first-ambient
$B constant clock-area   $C constant water-area
create holes  $1F , $2F , $89 ,
: came-in ( exit -- )
    drop  room-sounds
    tintstalker-kind partner-is? if  no-stalker-camera state!  then
    wt1f-1-medallion-found fact?  wt1f-1-medallion fact? 0= and if  medallion-fx -39.66e 1e 0.99e flicker-at  then
    1 $2300 reverb
    3 0 do  holes i cells + @ hole-floor floor-tri-on  loop ;

\ ---- what she may do or say here ----
: the-water ( -- )   \ (a recovery point: once a visit it calms her)
    rested @ if  fiona wait-move  helped-msg say exit  then
    -1 rested !  water-msg her-motions calm-sound $32 -45 -64.094e 52.322e recovery-point ;
: pick-medallion ( -- )  medallion wt1f-1-medallion medallion-fx -39.66e 0.99e picks-up ;
: medallion-sniffed ( -- )  wt1f-1-medallion-found wt1f-1-medallion medallion-fx  -39.66e 1e 0.99e  hewie-finds ;
: spots ( -- )
    fiona clock-area 50 faces-area? if  ['] save-clock calm-hewie-up offer  then
    fiona water-area in-area?  fiona -44 50 heading? and if  ['] the-water unhurried offer  then
    wt1f-1-medallion-found fact?  wt1f-1-medallion fact? 0= and  fiona -39.66e 0e 0.99e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then ;

\ ---- every frame ----
create entries  2 , -1 ,  0 , -1 ,  1 , 1 ,   \ (by exit 0 .. 2: the camera)
create cameras  7 , 0 , -1 ,  8 , 1 , 1 ,  9 , 1 , 1 ,  $A , 2 , -1 ,   \ (area, set, path)
create ahead  3 , 0 ,  4 , 1 ,  5 , 0 ,  6 , 2 ,   \ (area, exit loaded ahead)
: each-frame ( -- )
    0 exit-out  1 1 leaves-by  2 2 leaves-by
    cameras 4 cameras-at  ahead 4 ways-out
    time-up-leaves
    wt1f-1-medallion-found fact? 0=  scrambling? and  hewie -39.66e 0e 0.99e 10e 5e touches? and if  ['] medallion-sniffed start-only  then
    room-sounds-in? if  first-ambient 25e 10e 136.5e ambient-sounds  then
    spots
    -83.5e 3e -34.7e  -18e 3e -20.4e  -83.5e -12e -34.7e  -18e -12e -20.4e  light-doorway ;
: person-in ( i -- )  fiona-p = if  entries 3 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went water-tower-1f-1 place!
