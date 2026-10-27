\ story/places/chaos-forest-10.fs - the Chaos Forest's glade where Hewie lies shot ($108): once he is
\ found (chaos-forest-8's scene) and until the heart's scene, he lies here hurt (his hurt face, unless
\ he wears his other coat), whining now and then; the first time she comes up, the scene (her
\ bandage: the files, his trust; the stalker sent in after her); then her word to him; the crate
\ broken by a blow, the Medallion behind it; the maze dealt; the replay. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/chaos-forest-10.fs (converted from the game's,
\ not loaded).
IN: story.places.chaos-forest-10
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names effects story.kit story.places.forest fiona.anims hewie.sounds scenes ;

own-reset  forest-owns owns !

0 constant shot-msg        \ "You've been shot. You'll be all right. This will stop the bleeding." (the scene's)
1 constant stay-still-msg  \ "You'll be alright, boy. Just stay still, OK?"
own: whine-frames          \ frames to his next whine
own: hewie-lying           \ he is lying hurt (his doing under way)
$19 constant forest-track   0 constant medallion-fx   1 constant hurt-whine   $D constant reaching-area
1 constant shot-movie   0 constant shot-cutscene   $1004 constant shot-lying   $118 constant lying-tri
$39 constant shot-seen   $259 constant shot-file   $8285 constant shot-other-file
0 constant whole-part   1 constant broken-part   0 constant crate-group
cf-10-breakable-broken whole-part broken-part  76.5e -26.5e 134.5e 5e 8e  breakable: the-crate
the-crate shard-grey 0 9 shard-look  the-crate 0.812e 0.687e 0.187e 0.312e shard-uv  the-crate crate-group frees-floor  the-crate $262 noise-from

: hurt? ( -- flag )  found-file-247 fact?  cf-13-flag1 fact? 0= and ;   \ (found, and the heart's scene not yet)
: own-coat? ( -- flag )  coat-var var@ 0= ;   \ (his own: his hurt face fits it)
: face-from ( addr len -- )  {: a n :}  1 0 a n face-on  1 1 a n face-on ;   \ (both his face's textures)
: first-face ( -- )  s" O_HEW\HEW_201.TEX" face-from ;
: hurt-face ( -- )   \ (before the scene the first file, after it the bandaged one)
    cf-10-flag1 fact? if  s" O_HEW\HEW_202.TEX" face-from  else  first-face  then ;
: face-back ( -- )  own-coat? if  1 face-off  then ;
: lies ( -- )   \ (his doing: down, through anything, his face; for good)
    hewie hold  hewie true noclip  hewie wait-move  hewie shot-lying 0 start-blend
    own-coat? if  hurt-face  then  -1 hewie-lying !
    begin  yield  again ;
: lying-here ( -- )  hewie activate  hewie lying-tri 180 4.7e 54.573e place-at  ['] lies start drop ;
: medallion-shows ( -- )  medallion-fx 76.5e -25.5e 134.5e flicker-at ;
: came-in ( exit -- )
    drop  room-sounds  forest-track 1e background  maze-in
    the-crate as-it-stands
    the-crate broken?  cf-10-medallion fact? 0= and if  medallion-shows  then
    hurt? if  lying-here  then ;

\ ---- the first time she reaches him: the scene ----
: shot-scene ( -- )
    shot-movie scenes:unseen-movie shot-cutscene load-scene drop  panic-eased  shot-msg run-scene ;
: bandaged ( -- )
    stalkers-stay state!  fiona hold  stalker let-go  0 summon-take  fiona wait-move
    15 screen:to-black-2 with-music or fade  no-track 1e background  wait-fade
    0 hewie-lying !  ['] lies stop-doing  yield
    shot-scene  30 trust+  world-held state!  cf-10-flag1 fact!
    fiona $9E -7 12.16e 49.91e place-at  face-back  lying-here
    begin  hewie-lying @ 0=  while  yield  repeat
    partner activate  chaos-forest-10 305 stalker-to  stalker 3 3 camera-set  2 stalker-order
    summoner-on state!  his-own-time  camera-restart
    15 fade-in  forest-track 1e background
    shot-seen resident-flag-set  shot-file give-file  shot-other-file give-file
    stalkers-stay state-off  plain-commands state-off  hewie-commandable state-off ;
: replayed ( -- )
    fiona hold  own-coat? if  first-face  then  shot-scene  face-back  replay-over ;

\ ---- what she may do or say here ----
: stay-still ( -- )
    fiona wait-move  fiona crouch-down-anim play  stay-still-msg say
    cowed-whine common-bank hewie sound-by  fiona crouch-up-anim play ;
: pick-medallion ( -- )  medallion cf-10-medallion medallion-fx 76.5e 134.5e picks-up ;
: spots ( -- )
    hurt?  cf-10-flag1 fact? and  fiona hewie 10 45 touches-facing? and if  ['] stay-still unhurried offer  then
    the-crate broken? if
        cf-10-medallion fact? 0=  fiona 76.5e -25.5e 134.5e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then
    else  the-crate breaks? if  medallion-shows  then  then ;

\ ---- every frame ----
create cameras  8 , 2 , 2 ,  9 , 0 , 0 ,  $A , 0 , 0 ,  $B , 3 , 3 ,  $C , 0 , 0 ,  $D , 4 , 4 ,   \ (area, set, path)
create ahead  4 , 0 ,  5 , 1 ,  6 , 2 ,  7 , 3 ,   \ (area, exit loaded ahead)
create entries  2 , 2 ,  1 , 1 ,  1 , 1 ,  3 , 3 ,   \ (by exit 0 .. 3: the camera)
: each-frame ( -- )
    4 0 do  i i leaves-by  loop  cameras 6 cameras-at  ahead 4 ways-out
    fiona reaching-area entered?  found-file-247 fact? and  cf-10-flag1 fact? 0= and if
        cf-10-flag1 fact!  panic-eased  ['] bandaged start drop
    then
    maze-frame
    hurt? if  whine-frames whine-due? if  hurt-whine room-bank hewie sound-by  then  then
    spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    entries 4 ways-in ;
: went ( -- )  hurt? if  hewie let-go  face-back  then ;

' came-in ' person-in ' each-frame ' went chaos-forest-10 place!
