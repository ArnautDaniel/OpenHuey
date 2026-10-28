\ story/places/old-mansion-b1-1.fs - a hall of the old mansion's B1 with a hopping figure (the kit's
\ hopper - now and then its call); the Album Medallion up on the ledge, fetched by Hewie at her "go
\ there" (she stands on the far side, it out of sight); the vase (the Chicken Jerky in it); brought
\ in by the forced way, her costume and Hewie's coat put on afresh, Hewie by her, the stalker sent
\ off to the 1F, the file; Hewie wary of the figure, barking at the ledge, his looks; the map's
\ pages; the doorways' shade. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/old-mansion-b1-1.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.old-mansion-b1-1
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names hewie-names story.kit fiona.anims ;

own-reset

own: call-frames  own: figure-noticed  own: ledge-noticed  own: fetching
0 constant jerky-fx   1 constant medallion-fx   0 constant lash-sound   1 constant hop-sound   2 constant first-call
$13 constant hall-track   $23F constant hall-file   $C constant below-ledge-tri   $129 constant ledge-tri
create holes  $51 , $50 , $EF , $DB , $FA , $F9 , $55 , $DA , $F3 , $F8 ,
omb1-1-breakable-broken 0 1  60.5e -0.1e -33.4e 5e 8e  breakable: the-vase
the-vase $80303030 0 6 shard-look  the-vase 0.812e 0.687e 0.187e 0.312e shard-uv  the-vase 0 frees-floor  the-vase $123 noise-from
$900A $900B lash-sound hop-sound  39.3e 0e 50.4e -90e  hopper: the-figure

\ ---- the figure (the kit's hopper; out of sight coming by the near ways): its calls ----
: hopping-about ( -- )  the-figure  came-by @ dup 0 =  swap 3 = or 0=  hops-about ;
: calls ( -- )  the-figure call-frames first-call hopper-calls ;

\ ---- as she comes in ----
: jerky-shows ( -- )  omb1-1-chicken-jerky fact? 0= if  jerky-fx 60.5e 0.9e -33.4e flicker-at  then ;
: came-in ( exit -- )
    drop  the-vase as-it-stands  the-vase broken? if  jerky-shows  then
    0 call-frames !  ['] hopping-about start drop  hunted state!
    omb1-1-album-medallion fact? 0= if  medallion-fx -6.17e 39.94e -29.06e flicker-at  then
    1 $2300 reverb  1 solid-floor floor-on
    10 0 do  holes i cells + @ hole-floor floor-tri-on  loop ;

\ ---- brought in by the forced way: dressed afresh, Hewie by her, the stalker off to the 1F ----
: arrives ( -- )
    fiona hold  costume-var var@ dress  yield  coat-var var@ coat  yield
    fiona 1 at-exit  hewie activate  old-mansion-b1-1 161 1 hewie-to  hewie 1 at-exit
    old-mansion-1f-7 -1 stalker-to  his-own-time  0 stalker-order
    3 3 player-camera  yield  camera-restart  room-sounds  hall-track 1e background
    15 fade-in  hall-file give-file  scene-locked state-off  stalkers-stay state-off ;

\ ---- the Album Medallion on the ledge: Hewie fetches it at her "go there" ----
: medallion-in-sight? ( -- flag )  -6e 40e -29e point-seen? ;
: fiona-far-side? ( -- flag )  fiona body-tri dup 0< if  drop false exit  then  nav-flags other-half and 0<> ;
: sent-up? ( -- flag )
    hewie-in? 0=  hewie-cond-now @ down-cond = or  hewie held? or  chased? or  hewie-way-now @ 0<> or if  false exit  then
    fiona-far-side? 0=  medallion-in-sight? 0= or  fiona unseen? or  hewie unseen? or if  false exit  then
    fiona 60 -6e -29e faces-point?  started-now @ go-there-cmd = and  hewie-breakable-now @ and ;
: fetches ( -- )
    hewie hold  stalkers-stay state!  -1 fetching !  hewie-bark  hewie wait-move
    hewie below-ledge-tri 90 -1 walk-path-move -59.57e -30.97e go-to
    1 solid-floor floor-off  1 keeps-fiona-off keeps-pursuers-off or floor-on
    hewie leap-move ledge-tri 50  -11.17e 0.3e -29.06e 0e  move  hewie wait-move
    hewie hewie.anims:sit-a-while-anim 10 play-blend
    omb1-1-album-medallion fact? 0=  fiona-half-hidden state?  fiona held? 0= or and if
        album-medallion item-full? 0= if  10 trust+  then
        album-medallion omb1-1-album-medallion medallion-fx takes-up
    then
    hewie ledge-tri -90 -1 walk-move -11.46e -30.1e go-to
    hewie leap-move below-ledge-tri 50  -59.57e 2e -30.97e 0e  move  hewie wait-move
    1 solid-floor floor-on  0 fetching !  stalkers-stay state-off ;
: wary-of-it ( -- )  38.73e 0e 50.28e hewie-stands-by ;
: barks-at-ledge ( -- )  -9.31e 39.5e -32.27e hewie-barks-at ;

\ ---- what she may take here ----
: take-jerky ( -- )  chicken-jerky omb1-1-chicken-jerky jerky-fx 60.5e -33.4e picks-up ;

create cameras  8 , 0 , 0 ,   9 , 1 , 1 ,   $A , 0 , 0 ,   $B , 2 , 2 ,   $C , 3 , 3 ,   $D , 4 , 4 ,   \ (area, set, path)
create entries  2 , 2 ,   3 , 3 ,   4 , 4 ,   1 , 1 ,   \ (by exit 0..3: the camera)
create ahead  4 , 0 ,   5 , 3 ,   6 , 1 ,   7 , 2 ,   \ (area, exit loaded ahead)
create figure-look  38.73e 0e 50.28e  26e 16e 16e look,
create ledge-look  -9.31e 39.5e -32.27e  50e 13e 0e look,
: map-pages ( -- )
    fiona 5 entered? if  0 map-page  then  fiona 5 left? if  1 map-page  then
    fiona $A entered? if  1 map-page  then  fiona $A left? if  2 map-page  then ;
: hewie-about ( -- )
    sent-up? if  ['] fetches start drop  then
    figure-noticed 80 38.73e 0e 50.28e 26e 16e notices? if  ['] wary-of-it start drop  then
    figure-look 1 danger-now @ followed = hewie-looks-if
    omb1-1-album-medallion fact? if  exit  then
    ledge-noticed 100 -9.31e 39.5e -32.27e 50e 13e notices? if  ['] barks-at-ledge start drop  then
    ledge-look 1 danger-now @ followed = hewie-looks-if ;
: each-frame ( -- )
    3 exits-out  3 3 leaves-by  cameras 6 cameras-at  ahead 4 ways-out  map-pages
    calls  hewie-about
    the-vase broken? if
        omb1-1-chicken-jerky fact? 0=  fiona 60.5e -0.1e -33.4e 5e 5e touches? and if  ['] take-jerky calm-only offer  then
    else  the-vase breaks? if  jerky-shows  then  then
    37e -50e 50e  82.5e -50e 50e  37e -90e 50e  82.5e -90e 50e  light-doorway
    52e -32e 45e  68e -32e 45e  52e -90e 45e  68e -90e 45e  light-doorway
    -29e -4e 50e  37e -48e 50e  -8e -80e 50e  37e -80e 50e  light-doorway
    5e 0e -40e  -65e 0e -40e  5e -80e -40e  -65e -80e -40e  light-doorway
    -50e 18.5e -40e  -50e 18.5e 5.5e  -50e -50e -40e  -50e -50e 5.5e  light-doorway ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    entries 4 ways-in
    came-by @ forced-exit = if  world-held state!  0e sounds-scaled  ['] arrives start drop
    else  room-sounds  hall-track 1e background  then
    came-by @ dup 1 =  swap 2 = or if  1 map-page  then  came-by @ 0= if  2 map-page  then ;
: went ( -- )
    hopper-gone
    fetching @ if  hewie let-go  hewie $76 -90 -40.52e -28e place-at  stalkers-stay state-off  then ;

' came-in ' person-in ' each-frame ' went old-mansion-b1-1 place!
