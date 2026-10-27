\ story/places/castle-1f-11.fs - the drawbridge room in Belli Castle's 1F ($03): the clamp on the
\ winch - struck, its lever jolts; the third blow and the drawbridge falls (a scene), Debilitas on
\ the far side shoving the rubble down, throwing her if she is in his way; three vases (the Beef
\ Jerky and the Magnesia behind two); the Medallion Hewie fetches across the gap at her "go for it";
\ the map's pages; the walls' shade. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/castle-1f-11.fs (converted from the game's, not loaded).
IN: story.places.castle-1f-11
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects story.kit fiona.anims hewie.anims scenes ;

own-reset

\ ---- what is said here ----
0 constant clamp-msg       \ "A clamp is fixed to a wire winding device. It's rusted solid but if I hit it wi..."
1 constant loosened-msg    \ "The wire seems to have loosened a bit."
2 constant clamp-off-msg   \ "The clamp has already come off..."

\ ---- its own state (a visit) ----
own: hits                  \ the clamp struck so far (the bridge falls at the third)
own: his-scene             \ his shove under way (at the room's end he is put back)

\ ---- the room's things ----
0 constant jerky-fx   1 constant magnesia-fx   2 constant medallion-fx
0 constant to-exit-0-area   2 constant clamp-area   7 constant page-2-area   8 constant page-3-area
9 constant page-4-area   $A constant page-3b-area
0 constant clamp-sound   11 constant thud-sound   $16 constant hall-track
1 constant bridge-movie   0 constant bridge-cutscene   2 constant his-motions   0 constant his-shove
0 constant go-for-it-gesture   $18 constant near-door   $35 constant far-door   $22B constant bridge-file   $2F constant hewie-far-room
: bridge-down? ( -- flag )  c1f-11-flag1 fact? ;
c1f-11-breakable1-broken 0 1  72.5e 48e 8.5e 5e 8e  breakable: vase-1      \ (the Beef Jerky behind it)
c1f-11-breakable2-broken 3 2  53e 113e -113.5e 5e 8e  breakable: vase-2    \ (the Magnesia behind it)
c1f-11-breakable3-broken 5 4  53e 113e -107e 5e 8e  breakable: vase-3
: vase ( bk group rgba tri -- )   \ (their shards, their floor, their noise)
    {: bk g c tri :}  bk c 0 0 shard-look  bk 0.812e 0.687e 0.187e 0.312e shard-uv  bk g frees-floor  bk tri noise-from ;
vase-1 0 $80505050 $111 vase   vase-2 2 $80404040 $122 vase   vase-3 1 $80404040 $123 vase
: jerky-shows ( -- )  jerky-fx 72.5e 49e 8.5e flicker-at ;
: magnesia-shows ( -- )  magnesia-fx 53e 114e -113.5e flicker-at ;

\ ---- the drawbridge: its two halves and the lever down; the lever's tilt (-10..0 degrees) ----
: lever ( -- addr len )  s" kanagu" ;
: bridge-lies ( -- )
    -60e deg>rad s" hanebasi1" turn-x object!  -60e deg>rad s" hanebasi2" turn-x object!  -90e deg>rad lever turn-z object! ;
: lever-tilt ( F: deg -- )  0e fmin -10e fmax deg>rad lever turn-z object! ;
: lever-back ( -- )  lever placed-here? 0= if  exit  then  lever turn-z object@ rad>deg 2e f+ lever-tilt ;   \ (2 degrees a frame)
: lever-jolts ( -- )   \ (pulled to -10, a grey puff at it)
    lever placed-here? 0= if  exit  then  -10e lever-tilt
    lever place-x object@  lever place-y object@  lever place-z object@  dust ;

\ ---- as she comes in ----
: specks-about ( -- )
    $A $10 $80 $80 $80 $40 rgba 3.5e 30.7e 49.9e specks   $C $10 $80 $80 $80 $60 rgba 16e 129e 35.7e specks ;
: came-in ( exit -- )
    drop  room-sounds  hall-track 1e background
    bridge-down? if  bridge-lies  then
    vase-1 as-it-stands  vase-2 as-it-stands  vase-3 as-it-stands
    vase-1 broken?  c1f-11-beef-jerky fact? 0= and if  jerky-shows  then
    vase-2 broken?  c1f-11-magnesia fact? 0= and if  magnesia-shows  then
    specks-about
    c1f-11-medallion fact? 0= if  medallion-fx 9.646e 0.163e 27.266e flicker-at  then ;

\ ---- the bridge falls: the scene; then Debilitas on the far side, shoving the rubble down ----
: shoves ( -- )   \ (into place unseen, then his shove - throwing her if she is in his way; the dust as it lands)
    -1 his-scene !  stalker hold  stalker his-motions motions-for  stalker use-motions  stalker true noclip  yield
    stalker $132 180 -37.43e 44.367e place-at  stalker 0 0 camera-set  stalker true shown  stalker false hush
    stalker $13C 90 -1 walk-path-move -45.739e -73.201e go-to
    stalker room-motion his-shove + 5 start-blend8
    0  begin
        stalker fiona 0 close-to? if  stalker throws-her  then
        dup 89 = if  5e 115e -70e dust  5e 115e -76e dust  thud-sound room-bank stalker sound-by  then
        1+  stalker went-round? 0= while  yield
    repeat  drop
    0 stalker-order  stalker false noclip  0 his-scene !  his-own-time ;
: hewie-back ( -- )   \ (held: let go, by the bridge's end)
    hewie held? if  hewie let-go  hewie $CC 60 32.509e 56.979e place-at  stalkers-stay state-off  then ;
: bridge-falls ( -- )
    hewie-back  fiona hold  stalker let-go  fiona wait-anim  fiona -1 idle  7 frames
    15 screen:to-black-2 with-music or fade  no-track 1e background
    bridge-movie scenes:keyed-movie bridge-cutscene load-scene  $10 $FF movie-key  panic-eased
    if  -1 run-scene  then
    world-held state!  fiona 0 idle  fiona $8C -161 1.471e -112.581e place-at  0 0 player-camera  yield  camera-restart
    bridge-lies  c1f-11-flag1 fact!  far-door near-door door-as  far-door closed-off  exits-redone
    hewie body?  hewie body-room hewie-far-room = and if  $30 0 -1 hewie-to  then
    stalker activate  castle-1f-11 153 stalker-to  his-own-time
    stalker -1 -1 camera-set  stalker false shown  stalker true hush  ['] shoves start drop
    hall-track 1e background  15 fade-in  bridge-file give-file  stalkers-stay state-off
    castle-1f-11 stalker-slot 0 doors-into ;

\ ---- the clamp: struck (her blow there), the lever jolts; at the third blow the bridge falls ----
: struck-again ( -- )  clamp-sound room-bank fiona sound-by  begin  fiona-free? 0= while  yield  repeat  1 hits +! ;
: struck-off ( -- )
    stalkers-stay state!  clamp-sound room-bank fiona sound-by  begin  fiona-free? 0= while  yield  repeat
    panic-eased  ['] bridge-falls start drop ;
: clamp ( -- )
    bridge-down? if  exit  then
    lever-back
    -11e 113e -87e 5e 12e her-blow-in? 0= if  exit  then
    lever-jolts  1 $FF 4 rumble-pad
    hits @ 2 = if  1 hits +!  ['] struck-off start drop  else  ['] struck-again start drop  then ;

\ ---- the Medallion across the gap: her "go for it" toward it - Hewie fetches it ----
: fetches ( -- )
    hewie hold  hewie bark-move 0 0  0e 0e 0e 0e  move  hewie wait-move
    hewie $13 -90 -1 walk-path-move 53.585e 39.199e go-to  hewie true noclip
    $148 40 0e 1.5e 43.21e hewie-leap  hewie wait-move
    hewie $145 180 -1 walk-move 9.804e 32.761e go-to  hewie sit-a-while-anim 10 play-blend
    c1f-11-medallion fact? 0= if
        fiona-half-hidden state?  fiona held? 0= or if
            medallion item-full? 0= if  10 trust+  then
            medallion c1f-11-medallion medallion-fx takes-up
        then
    then
    hewie $148 66 -1 walk-move -7.96e 39.28e go-to  $CC 50 32.509e 1.5e 56.979e hewie-leap  hewie wait-move
    hewie false noclip  stalkers-stay state-off ;
: sends-him ( -- )
    fiona hold  stalkers-stay state!  fiona wait-move
    hewie held? if  stalkers-stay state-off exit  then
    ['] fetches start drop
    fiona go-for-it-gesture-anim start-anim  fiona 1 wait-mark  $2E common-bank fiona sound-by
    fiona wait-anim  fiona -1 idle  7 frames ;
: medallion-way ( -- )
    c1f-11-medallion fact? if  exit  then
    hewie-in?  hewie-cond-now @ down-cond <> and  hewie held? 0= and  chased? 0= and 0= if  exit  then
    fiona one-half on-floor-of? if  exit  then
    fiona-cmd-now @ go-for-it-gesture <> if  exit  then
    4e 0e 45e point-unseen?  fiona unseen? 0= and  hewie unseen? 0= and 0= if  exit  then
    fiona 50 4e 45e faces-point?  hewie-breakable-now @ and if  ['] sends-him start drop  then ;
create medallion-look  9.646e 0.163e 27.266e 30e 6e 6e look,

\ ---- what she may do or say here ----
: at-the-clamp ( -- )
    fiona wait-move
    bridge-down? if  clamp-off-msg say  exit  then
    world-frozen state!  fiona $8E -144 -6.625e -82.651e place-at  1 10e 15e 0e 0e shot
    hits @ if  loosened-msg  else  clamp-msg  then  say  16 frames  world-frozen state-off  shot-off ;
: pick-jerky ( -- )  beef-jerky c1f-11-beef-jerky jerky-fx 72.5e 8.5e picks-up ;
: pick-magnesia ( -- )  magnesia c1f-11-magnesia magnesia-fx 53e -113.5e picks-up ;
: spots ( -- )
    vase-1 broken? if
        c1f-11-beef-jerky fact? 0=  fiona 72.5e 49e 8.5e 5e 5e touches? and if  ['] pick-jerky calm-only offer  then
    else  vase-1 breaks? if  jerky-shows  then  then
    vase-2 broken? if
        c1f-11-magnesia fact? 0=  fiona 53e 114e -113.5e 5e 5e touches? and if  ['] pick-magnesia calm-only offer  then
    else  vase-2 breaks? if  magnesia-shows  then  then
    vase-3 breaks? drop
    fiona clamp-area in-area?  fiona 90 50 heading? and if  ['] at-the-clamp unhurried offer  then ;

\ ---- the map's pages ----
: map-pages ( -- )
    fiona page-2-area entered? if  2 map-page  then   fiona page-3-area entered? if  3 map-page  then
    fiona page-4-area entered? if  4 map-page  then   fiona page-3b-area entered? if  3 map-page  then ;
: walls-shade ( -- )  78e 127e -100e  78e 127e -19e  78e 90e -100e  78e 90e -19e  light-doorway ;

\ ---- the movie replayed (the extras) ----
: replayed ( -- )
    fiona hold  specks-about  bridge-movie scenes:keyed-movie bridge-cutscene load-scene  $10 $FF movie-key
    panic-eased  if  -1 run-scene  then  replay-over ;

\ ---- every frame ----
create bridge-cameras  3 , 1 , 1 ,   4 , 1 , 1 ,   5 , 0 , 0 ,   6 , 0 , 0 ,   \ (before the bridge is down)
create cameras  7 , 2 , 2 ,   8 , 3 , 3 ,   9 , 0 , 0 ,   $A , 3 , 3 ,        \ (area, set, path)
: ways ( -- )
    to-exit-0-area 0 leaves-by
    bridge-down? 0= if  bridge-cameras 4 cameras-at  then  cameras 4 cameras-at  map-pages ;
: each-frame ( -- )  events-held state? if  walls-shade exit  then
    ways  clamp  medallion-way  medallion-look 1 hewie-looks-at  spots  walls-shade ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    0 2 2 way-in ;
: went ( -- )
    hewie-back
    stalker held?  his-scene @ and if  stalker $8F 90 2.4e -76.445e place-at  0 his-scene !  then ;

' came-in ' person-in ' each-frame ' went castle-1f-11 place!
