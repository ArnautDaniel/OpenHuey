\ story/places/castle-1f-14.fs - the corridor round Belli Castle's 1F west ($0E): three vases to
\ break (the Chicken Jerky behind one, the Antimony Powder behind another), the Viride Medallion
\ Hewie sniffs out, the door to unlock from this side, the map turning to the next floor's page by
\ the stairs, the walls' shade; brought here caught (a forced way in): the game over's scene. A
\ place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/castle-1f-14.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.castle-1f-14
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects story.kit fiona.anims scenes ;

own-reset

\ ---- the room's things ----
0 constant jerky-fx   1 constant medallion-fx   2 constant antimony-fx
3 constant near-exit-0-area   4 constant near-exit-1-area   5 constant near-exit-2-area   $D constant stairs-area
2 constant this-floor-page   3 constant stairs-page   0 constant locked-exit
1 constant caught-movie   0 constant caught-cutscene   0 constant caught-msg   \ "Fiona! Fiona! Fio...! Don't go, Fiona... My life... My Azoth... Fiona..."
3 constant lorenzo-figure   4 constant chair-figure   $B constant lorenzo-kind   $10 constant chair-kind
2 constant azoth-ending   2 constant caught-over   3 constant ending-seen   $37 constant caught-way
c1f-14-breakable1-broken 0 1  -61e 0e -52e 5e 8e  breakable: vase-1
c1f-14-breakable2-broken 2 3  -68e 0e -58e 5e 8e  breakable: vase-2   \ (the Chicken Jerky behind it)
c1f-14-breakable3-broken 4 5  -66e 0e 55e 5e 8e  breakable: vase-3    \ (the Antimony Powder behind it)
: vase ( bk group rgba tri -- )   \ (their shards, their floor, their noise)
    {: bk g c tri :}  bk c 0 4 shard-look  bk 0e 0.687e 0.187e 0.312e shard-uv  bk g frees-floor  bk tri noise-from ;
vase-1 0 $80303030 $55 vase   vase-2 1 $80303030 $2BF vase   vase-3 2 $80202020 $295 vase
: jerky-shows ( -- )  jerky-fx -68e 1e -58e flicker-at ;
: antimony-shows ( -- )  antimony-fx -66e 1e 55e flicker-at ;
: medallion-found? ( -- flag )  c1f-14-viride-medallion-found fact?  c1f-14-viride-medallion fact? 0= and ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop
    medallion-found? if  medallion-fx -70e 61e 54.1e flicker-at  then
    vase-1 as-it-stands  vase-2 as-it-stands  vase-3 as-it-stands
    vase-2 broken?  c1f-14-chicken-jerky fact? 0= and if  jerky-shows  then
    vase-3 broken?  c1f-14-antimony-powder fact? 0= and if  antimony-shows  then
    1 $2300 sound-reverb!   \ (the effects' reverb: the corridor's)
    items-40-as-70 fact?  c1f-14-flag1 fact? 0= and if
        c1f-14-flag1 fact!  castle-1f-14 0 829 $80 10 $FF80 creature-facing
    then ;

\ ---- brought here caught (the forced way in): Lorenzo's scene, and the game is over ----
: caught-scene ( -- )   \ (the figures in, the movie; the figures out)
    lorenzo-kind lorenzo-figure figure-in  chair-kind chair-figure figure-in
    caught-movie scenes:unseen-movie caught-cutscene load-scene  wait-fade  yield
    panic-eased  if  caught-msg run-scene  then
    lorenzo-figure figure-out  chair-figure figure-out ;
: caught-here ( -- )
    ending-seen resident-flag? 0= if  no-pause state!  then
    world-held state!  fiona hold  fiona wait-move  fiona false shown
    stalker let-go  hewie let-go
    caught-scene  azoth-ending ending-var var!  caught-over game-over ;

\ ---- what she may do here ----
: pick-jerky ( -- )  chicken-jerky c1f-14-chicken-jerky jerky-fx -68e -58e picks-up ;
: pick-medallion ( -- )  viride-medallion c1f-14-viride-medallion medallion-fx -70e 54.1e picks-up ;
: pick-antimony ( -- )  antimony-powder c1f-14-antimony-powder antimony-fx -66e 55e picks-up ;
: unlocks-it ( -- )  locked-exit unlocks ;
: spots ( -- )
    vase-1 breaks? drop
    vase-2 broken? if
        c1f-14-chicken-jerky fact? 0=  fiona -68e 1e -58e 5e 5e touches? and if  ['] pick-jerky calm-only offer  then
    else  vase-2 breaks? if  jerky-shows  then  then
    vase-3 broken? if
        c1f-14-antimony-powder fact? 0=  fiona -66e 1e 55e 5e 5e touches? and if  ['] pick-antimony calm-only offer  then
    else  vase-3 breaks? if  antimony-shows  then  then
    locked-exit before-locked? if  ['] unlocks-it any-time offer  then
    medallion-found?  fiona -70e 61e 54.1e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then ;

\ ---- Hewie: the medallion he sniffs out (scrambling about up there) ----
: medallion-sniffed ( -- )  c1f-14-viride-medallion-found c1f-14-viride-medallion medallion-fx  -70e 61e 54.1e  hewie-finds ;
: hewie-sniffs ( -- )
    c1f-14-viride-medallion-found fact?  scrambling? 0= or if  exit  then
    hewie -70e 60e 54.1e 10e 5e touches? 0= if  exit  then
    ['] medallion-sniffed start-only ;

\ ---- the map: at the stairs, the next floor's page ----
: map-pages ( -- )
    fiona stairs-area entered? if  stairs-page map-page  then
    fiona stairs-area left? if  this-floor-page map-page  then ;

\ ---- the walls' shade: no shadows where it falls ----
: wall-shade ( -- )
    30e 54.1e -51.5e  -45e 54.1e -51.5e  30e 54.1e -90e  -45e 54.1e -90e  light-doorway
    70e 54.1e -23e  57e 54.1e -56e  100e 54.1e -30e  80e 54.1e -65e  light-doorway
    77e 54.1e 18e  70e 54.1e -23e  100e 54.1e 30e  100e 54.1e -30e  light-doorway
    52e 54.1e 48e  77e 54.1e 18e  82e 54.1e 65e  100e 54.1e 30e  light-doorway
    65e 20e 18e  75e 20e 13e  65e 0e 18e  75e 0e 13e  light-doorway
    74.5e 20e -19.5e  68.5e 20e -24.5e  74.5e 0e -19.5e  68.5e 0e -24.5e  light-doorway
    53e 20e -48e  53e 20e -60e  53e 0e -48e  53e 0e -60e  light-doorway
    -55e 20e -62e  -55e 20e -47e  -55e 0e -62e  -55e 0e -47e  light-doorway
    -70e 20e -26e  -78e 20e -22e  -70e 0e -26e  -78e 0e -22e  light-doorway
    -78e 20e 20e  -69e 20e 25e  -78e 0e 20e  -69e 0e 25e  light-doorway
    -55e 20e 47e  -55e 20e 54e  -55e 0e 47e  -55e 0e 54e  light-doorway
    52e 20e 57e  52e 20e 47e  52e 0e 57e  52e 0e 47e  light-doorway ;

\ ---- the movie replayed (the extras) ----
: replayed ( -- )
    fiona hold  fiona false shown  caught-way preload  caught-scene  replay-over ;

\ ---- every frame ----
create cameras   \ (area, set, path)
    6 , 0 , 0 ,   7 , 0 , 0 ,   8 , 2 , 2 ,   9 , 3 , 3 ,   $A , 0 , 0 ,
    $B , 1 , 1 ,   $C , 1 , 1 ,   $D , 2 , 2 ,   $E , 2 , 2 ,   $F , 4 , 4 ,
create ahead  near-exit-0-area , 0 ,   near-exit-1-area , 1 ,   near-exit-2-area , 2 ,
create entries  2 , 2 ,   0 , 0 ,   0 , 0 ,             \ (by exit 0, 1, 2: the camera)
: ways ( -- )  3 exits-out  cameras 10 cameras-at  ahead 3 ways-out  map-pages ;
: each-frame ( -- )  events-held state? if  wall-shade exit  then  ways  hewie-sniffs  spots  wall-shade ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    entries 3 ways-in
    saturnus-hint-on fact?  c1f-14-flag2 fact? 0= and if  c1f-14-flag2 fact!  stalker let-go  then
    came-by @ 0= if  stairs-page map-page  then
    came-by @ forced-exit = if  panic-eased  ['] caught-here start drop  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-1f-14 place!
