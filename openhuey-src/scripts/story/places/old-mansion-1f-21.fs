\ story/places/old-mansion-1f-21.fs - a long room of the old mansion's 1F with four vases: each
\ broken by a blow (the first holds the Album Medallion); the door unlocked from this side; the
\ scene's figure loaded and let go by its ways; after the 3F's turn, two homunculi dropped in, once;
\ Hewie sitting by the medallion's vase a while; the map's pages, the doorways' shade. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-1f-21.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.old-mansion-1f-21
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names story.kit fiona.anims ;

own-reset

own: vase-noticed
0 constant medallion-fx   $1C constant scene-figure   3 constant figure-slot
om1f-21-breakable1-broken 0 1  20e 0e -72.5e 5e 8e  breakable: vase-1
om1f-21-breakable2-broken 2 3  6.4e 0e -132.4e 5e 8e  breakable: vase-2
om1f-21-breakable3-broken 4 5  -63.9e 0e -147.4e 5e 8e  breakable: vase-3
om1f-21-breakable4-broken 6 7  -68.8e 0e -156.4e 5e 8e  breakable: vase-4
: vase-look ( bk group tri -- )   \ (their shards, their floors, their noises)
    {: bk group tri :}  bk $80202020 0 6 shard-look  bk 0.345e 0.687e 0.187e 0.312e shard-uv
    bk group frees-floor  bk tri noise-from ;
vase-1 0 $35 vase-look   vase-2 3 $100 vase-look   vase-3 2 $112 vase-look   vase-4 1 $115 vase-look

: medallion-shows ( -- )  om1f-21-album-medallion fact? 0= if  medallion-fx 20e 1e -72.5e flicker-at  then ;
: came-in ( exit -- )
    drop  vase-1 as-it-stands  vase-1 broken? if  medallion-shows  then
    vase-2 as-it-stands  vase-3 as-it-stands  vase-4 as-it-stands
    om3f-1-flag2 fact?  om1f-21-flag1 fact? 0= and if
        om1f-21-flag1 fact!
        old-mansion-1f-21 0 65 $80 3 $FF80 44e creature-dropped  old-mansion-1f-21 0 36 $80 3 $81 44e creature-dropped
    then
    1 $2300 reverb ;

: take-medallion ( -- )  album-medallion om1f-21-album-medallion medallion-fx 20e -72.5e picks-up ;
: unlocks-it ( -- )  0 unlocks ;
: sits-by-vase ( -- )  19.93e 0e -73.33e hewie-sits-by ;
create cameras  $A , 0 , -1 ,   $B , 1 , 0 ,   $C , 0 , -1 ,   $D , 2 , 1 ,   $E , 2 , 1 ,   $F , 3 , -1 ,   \ (area, set, path)
create entries  2 , 1 ,   3 , -1 ,   0 , -1 ,   1 , 0 ,   \ (by exit 0..3: the camera)
: doorways-shade ( -- )
    38e 10e -99e  68e 30e -99e  38e 0e -99e  68e 0e -99e  light-doorway
    68e 30e -99e  90e 50e -99e  68e 0e -99e  90e 0e -99e  light-doorway ;
: each-frame ( -- )
    2 exits-out  2 2 leaves-by  3 3 leaves-by  cameras 6 cameras-at
    fiona 4 entered? if  0 prepare-exit  scene-figure figure-slot 0 figure-in-by  then
    fiona 6 entered? if  1 prepare-exit  scene-figure figure-slot 1 figure-in-by  then
    fiona 5 entered?  fiona 7 entered? or  fiona 8 entered? or if  2 prepare-exit  then
    fiona 5 entered?  fiona 7 entered? or if  figure-slot figure-out  then
    fiona 9 entered? if  3 prepare-exit  2 map-page  then   fiona 9 left? if  1 map-page  then
    vase-noticed 30 19.93e 0e -73.33e 13e 10e notices? if  ['] sits-by-vase start drop  then
    vase-2 breaks? drop  vase-3 breaks? drop  vase-4 breaks? drop
    1 locked-request?  fiona 0 before-door? and if  ['] unlocks-it any-time offer  then
    vase-1 broken? if
        om1f-21-album-medallion fact? 0=  fiona 20e 1e -72.5e 5e 5e touches? and if  ['] take-medallion calm-only offer  then
    else  vase-1 breaks? if  medallion-shows  then  then
    doorways-shade ;
: person-in ( i -- )
    fiona-p <> if  exit  then  entries 4 ways-in  came-by @ 3 = if  2 map-page  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-21 place!
