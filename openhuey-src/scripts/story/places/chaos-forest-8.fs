\ story/places/chaos-forest-8.fs - the Chaos Forest's clearing ($106): coming in the first time,
\ the scene - Hewie found (the file, his trust); seven crates (broken by a blow - the Medallion behind
\ the first, the Chicken Jerky the fifth); the sundial (its remark, then the save); the drop falling
\ now and then; figures loaded ahead for the ways on; the maze dealt; the replay. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/chaos-forest-8.fs (converted
\ from the game's, not loaded).
IN: story.places.chaos-forest-8
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names effects story.kit story.places.forest fiona.anims scenes ;

own-reset  forest-owns owns !

0 constant sundial-msg     \ "This is a sundial. I think it's used to tell the time of day depending on the length an..."
$19 constant forest-track   0 constant medallion-fx   1 constant jerky-fx   6 constant sundial-area
1 constant found-movie   0 constant found-cutscene   $247 constant found-file   $34 constant heart-door
3 constant scene-slot   4 constant scene-slot-2   $E constant scene-figure   $F constant scene-figure-2
$1A constant ahead-figure   $E constant ahead-figure-2   5 constant ahead-slot-2
$80404040 constant crate-grey

: crate-look ( bk group noise -- )   \ (one of the seven: their shards' look, its floor, its noise)
    {: bk group noise :}  bk crate-grey 0 9 shard-look  bk 0.812e 0.687e 0.187e 0.312e shard-uv  bk group frees-floor  bk noise noise-from ;
cf-8-breakable1-broken 0 1  -367e 0e -118.7e 5e 8e  breakable: crate-1     crate-1 2 $E8 crate-look
cf-8-breakable2-broken 2 3  -376e 0e -115.9e 5e 8e  breakable: crate-2     crate-2 1 $F0 crate-look
cf-8-breakable3-broken 4 5  -381e 0e -106.4e 5e 8e  breakable: crate-3     crate-3 0 $E6 crate-look
cf-8-breakable4-broken 7 6  -275.5e 0e -105e 5e 8e  breakable: crate-4     crate-4 3 $84 crate-look
cf-8-breakable5-broken 9 8  -275.5e 0e -96e 5e 8e  breakable: crate-5      crate-5 4 $FE crate-look
cf-8-breakable6-broken $B $A  -244.5e 0e -105e 5e 8e  breakable: crate-6   crate-6 5 $8E crate-look
cf-8-breakable7-broken $D $C  -244.5e 0e -96e 5e 8e  breakable: crate-7    crate-7 6 $F7 crate-look
create crates  ' crate-1 , ' crate-2 , ' crate-3 , ' crate-4 , ' crate-5 , ' crate-6 , ' crate-7 ,
: crate@ ( i -- bk )  cells crates + @ execute ;

: medallion-shows ( -- )  medallion-fx -367e 1e -118.7e flicker-at ;
: jerky-shows ( -- )  jerky-fx -275.5e 1e -96e flicker-at ;
: came-in ( exit -- )
    drop  found-file-247 fact? if  room-sounds  forest-track 1e background  then
    maze-in
    7 0 do  i crate@ as-it-stands  loop
    crate-1 broken?  cf-8-medallion fact? 0= and if  medallion-shows  then
    crate-5 broken?  cf-8-chicken-jerky fact? 0= and if  jerky-shows  then
    effects:one-drip ;

\ ---- the first time in: Hewie found (the scene) ----
: found-scene ( -- )   \ (its figures loaded on the way: chaos-forest-1's)
    found-movie scenes:unseen-movie found-cutscene load-scene drop
    scene-slot figure-on  scene-slot-2 scene-slot figure-dressed-as  panic-eased  -1 run-scene ;
: hewie-found ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move
    heart-door lock  chaos-forest-13 stalker-slot 1 doors-into
    room-sounds  found-scene  wait-fade
    50 trust+  hewie-in? if  hewie let-go  then
    scene-slot figure-out  scene-slot-2 figure-out
    fiona 0 idle  fiona $BD -73 -78.171e -25.963e place-at  camera-restart  found-file-247 fact!
    forest-track 1e background  15 fade-in  found-file give-file
    stalkers-stay state-off  plain-commands state! ;
: replayed ( -- )
    fiona hold  effects:one-drip  scene-figure scene-slot figure-in  scene-figure-2 scene-slot-2 figure-in  yield
    found-scene  scene-slot figure-out  scene-slot-2 figure-out  replay-over ;

\ ---- what she may do or say here ----
: the-sundial ( -- )  fiona wait-move  sundial-msg say  sundial-examined fact! ;
: pick-medallion ( -- )  medallion cf-8-medallion medallion-fx -367e -118.7e picks-up ;
: pick-jerky ( -- )  chicken-jerky cf-8-chicken-jerky jerky-fx -275.5e -96e picks-up ;
: spots ( -- )
    fiona sundial-area 50 faces-area? if
        sundial-examined fact? if  ['] save-clock calm-hewie-up offer  else  ['] the-sundial unhurried offer  then
    then
    crate-1 broken? if
        cf-8-medallion fact? 0=  fiona -367e 1e -118.7e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then
    else  crate-1 breaks? if  medallion-shows  then  then
    crate-5 broken? if
        cf-8-chicken-jerky fact? 0=  fiona -275.5e 1e -96e 5e 5e touches? and if  ['] pick-jerky calm-only offer  then
    else  crate-5 breaks? if  jerky-shows  then  then
    crate-2 breaks? drop  crate-3 breaks? drop  crate-4 breaks? drop  crate-6 breaks? drop  crate-7 breaks? drop ;

\ ---- every frame ----
create cameras  4 , 0 , 0 ,  5 , 1 , 1 ,   \ (area, set, path)
create entries  0 , 0 ,  1 , 1 ,   \ (by exit 0, 1: the camera)
: ways ( -- )
    0 0 leaves-by  1 1 leaves-by  cameras 2 cameras-at
    fiona 2 entered? if  0 prepare-exit  then
    fiona 3 entered? if   \ (the way on: what is there loaded ahead)
        ahead-figure scene-slot-2 1 figure-in-by  ahead-figure-2 ahead-slot-2 1 figure-in-by
        found-file-247 fact? if  scene-slot figure-out  then  1 prepare-exit
    then
    fiona 3 left? if   \ (back: the edge's figure ahead instead)
        found-file-247 fact? if  hopper-kind hopper-slot figure-in  then
        scene-slot-2 figure-out  ahead-slot-2 figure-out
    then ;
: each-frame ( -- )  ways  maze-frame  spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    entries 2 ways-in
    found-file-247 fact? 0= if  ['] hewie-found start drop  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went chaos-forest-8 place!
