\ story/places/castle-1f-12.fs - the moon dials' room in Belli Castle's 1F ($04): two devices with
\ dials, turned by the stick to a number (the first only once the Lunar Refractor is set in it - a
\ scene, its lens throwing her reflection); set right, with castle-2f-1's dial, the moonlight is let
\ through upstairs and down (scenes by way of castle-2f-1) and the five doors here open; a vase (a
\ Medallion), the moon on the wall, the door to unlock from this side. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/castle-1f-12.fs in the history (converted from
\ the game's; gone with the old machinery in 0.2.20).
IN: story.places.castle-1f-12
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects keys story.kit fiona.anims scenes ;

own-reset

\ ---- what is said here ----
0 constant device-msg      \ "Hmm, wonder what this device is for? Looks like it pivots at the base..."
1 constant refracts-msg    \ "A device that refracts moonlight... I've never seen anything like this before."
2 constant missing-msg     \ "I wonder what this does? Looks like there was something inserted here until just re..."
3 constant wall-msg        \ "This part of the wall has been designed differently from the rest of the wall."
4 constant moon-msg        \ "It's a picture of the moon... What could this mean?"
5 constant rotate-msg      \ "Rotate by pressing Right or Left on the left analog stick."

\ ---- its own state (a visit) ----
own: device-remarked       \ the device's first remark made
own: wall-remarked         \ the wall's first remark made

\ ---- the room's things ----
1 constant lens-object   2 constant dial-2-object   2 constant lens-part   0 constant medallion-fx
0 constant locked-exit   1 constant doors-floor   $1B constant lens-fx
6 constant lens-device-area   $C constant dial-area   $D constant dial-1-area   $E constant wall-area
3 constant to-exit-1-area   4 constant to-exit-0-area   5 constant to-exit-2-area
3 constant lens-movie   0 constant lens-cutscene   5 constant moon-movie   4 constant moon-cutscene
$22 constant moon-door   $C constant his-motions   -90 constant dial-base
: solved? ( -- flag )  0 var@ 5 =  1 var@ 2 = and  3 var@ 3 = and ;   \ (both dials here, castle-2f-1's: the moon let through)
: lens-set? ( -- flag )  lunar-refractor-used fact? ;
: opened? ( -- flag )  c1f-12-flag1 fact? ;
c1f-12-breakable-broken 0 1  -73.5e -103e 55.5e 5e 8e  breakable: the-vase
the-vase $80303030 0 4 shard-look  the-vase 0.812e 0.687e 0.187e 0.312e shard-uv
the-vase 0 frees-floor  the-vase $1E3 noise-from

\ ---- the two dials: the first ("syuukouki2", variable 0, its lens to 5), the second
\ ("syuukouki1", variable 1, to 2) ----
: dial-2 ( -- addr len var )  s" syuukouki2" 0 ;
: dial-1 ( -- addr len var )  s" syuukouki1" 1 ;
: doors-open ( -- )   \ (the five doors turned open, -75 / 75 degrees in turn)
    -75e deg>rad s" autodoor0" turn-y object!  75e deg>rad s" autodoor1" turn-y object!
    -75e deg>rad s" autodoor2" turn-y object!  75e deg>rad s" autodoor3" turn-y object!
    -75e deg>rad s" autodoor4" turn-y object! ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop
    opened? if  doors-open  doors-floor barred-floor floor-off  else  room-sounds  doors-floor barred-floor floor-on  then
    lens-set? if  lens-part hide-part  dial-2 dial-base dial-set  else  lens-part show-part  dial-2-object object-hide  then
    dial-1 dial-base dial-set
    the-vase as-it-stands
    the-vase broken?  c1f-12-medallion fact? 0= and if  medallion-fx -73.5e -102e 55.5e flicker-at  then
    1 $2300 sound-reverb! ;

\ ---- turning a dial: its number by the stick, a degree a frame to it; at a number, a click - its
\ lens catching the light at the right one; done, the moon let through if all are right ----
: moonlight ( -- )   \ (out to castle-2f-1, the light let through there; back by the forced way)
    stalkers-stay state!  15 screen:to-black-2 with-music or fade  castle-2f-1 preload  wait-fade
    force-calm state!  world-held state!  forced-exit take-exit ;
: turning ( addr len var right F: sx sy sz turn -- )   \ (at the dial till she is done; then, all right, the moon)
    dial-base rotate-msg dial-turning  text-gone  shot-off  world-frozen state-off
    solved? if  ['] moonlight start drop  then ;
: remarked-device ( -- )   \ (the first time: what it might be)
    device-remarked @ if  exit  then  -1 device-remarked !  device-msg say  8 frames ;
: at-dial-2 ( -- )
    stalkers-stay state!  fiona wait-move
    opened? if  refracts-msg say  stalkers-stay state-off exit  then
    lens-set? 0= if
        8 frames  world-frozen state!  fiona $73 -90 61e 50e place-at  1 13e 0e 0e 0e shot
        8 frames  missing-msg say  8 frames  shot-off  world-frozen state-off  stalkers-stay state-off exit
    then
    remarked-device  world-frozen state!  fiona $73 -90 61e 50e place-at  1 13e 0e 0e 0e shot
    dial-2 5  54e -90e 49e  pi f2/  turning  stalkers-stay state-off ;
: at-dial-1 ( -- )
    stalkers-stay state!  fiona wait-move
    opened? if  refracts-msg say  stalkers-stay state-off exit  then
    remarked-device  world-frozen state!  fiona $BE 180 30e -44e place-at  1 13e 0e 0e 0e shot
    dial-1 2  30e -90e -51e  0e  turning  stalkers-stay state-off ;

\ ---- the Lunar Refractor set in the first device (an item used there): a scene, its lens throwing
\ her reflection ----
: lens-reflects ( -- )   \ (the scene's own: from its frame $64 to near its end)
    begin  cutscene-frame $64 < while  yield  repeat
    begin  cutscene-near? 0= while  lens-fx $60 0 cast-model  s" lens" 640e -560e 1000e object-mirror  yield  repeat ;
: lens-scene ( -- )
    lens-movie scenes:keyed-movie lens-cutscene load-scene  ['] lens-reflects start drop  $1E $C8 movie-key
    panic-eased  if  -1 run-scene  then  lens-fx effect-off ;
: refractor-set ( -- )
    fiona hold  stalkers-stay state!  15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade
    hewie-in?  hewie-cond-now @ down-cond = and if  hewie hold  hewie false shown  then
    lens-scene
    lens-part hide-part  lens-object object-hide  dial-2-object object-show
    fiona 0 idle  fiona $73 -68 63.3e 50.28e place-at  hewie true shown  hewie let-go  camera-restart
    lunar-refractor spend  lunar-refractor-used fact!  15 fade-in  stalkers-stay state-off ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0= 0= if  drop false exit  then
    4 = if  ['] refractor-set start drop  true  else  false  then ;

\ ---- back by the forced way: the moonlight's scene here; then up to castle-2f-1 again, or - its
\ dial unset there - the moon door unlocked and these doors open ----
: moon-scene ( -- )  moon-movie scenes:keyed-movie moon-cutscene load-scene  $10 $FF movie-key ;
: moon-comes ( -- )
    fiona hold  stalker-in? if  stalker hold  yield  then  world-frozen state!
    moon-scene  came-by @ forced-exit = if  castle-2f-1 preload  else  castle-1f-7 preload  then  wait-fade
    panic-eased  if  -1 run-scene  then
    came-by @ forced-exit = if  world-held state!  world-frozen state-off  forced-exit 1 or take-exit exit  then
    moon-door unlock  c1f-12-flag1 fact!  dial-2 dial-base dial-set  dial-1 dial-base dial-set  doors-open
    doors-floor barred-floor floor-off  creatures-on state!  5 0 s" creatures" actor-named send creature-count
    camera-restart  force-calm state-off  stalkers-stay state-off  world-frozen state-off  15 fade-in ;

\ ---- what she may do or say here ----
: the-wall ( -- )
    fiona wait-move  4 frames  fiona $A0 -90 -76e -15e place-at  world-frozen state!  1 20e -10e 0e 0e shot  4 frames
    wall-remarked @ if  moon-msg  else  -1 wall-remarked !  wall-msg  then  say
    4 frames  world-frozen state-off  shot-off ;
: pick-medallion ( -- )  medallion c1f-12-medallion medallion-fx -73.5e 55.5e picks-up ;
: unlocks-it ( -- )  locked-exit unlocks ;
: spots ( -- )
    locked-exit before-locked? if  ['] unlocks-it any-time offer  then
    opened? 0=  fiona wall-area in-area? and  fiona -45 50 heading? and if  ['] the-wall unhurried offer  then
    fiona dial-area in-area?  fiona 50 54e 49e faces-point? and if  ['] at-dial-2 unhurried offer  then
    fiona dial-1-area in-area?  fiona 50 30e -51e faces-point? and if  ['] at-dial-1 unhurried offer  then
    the-vase broken? if
        c1f-12-medallion fact? 0=  fiona -73.5e -102e 55.5e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then
    else  the-vase breaks? if  medallion-fx -73.5e -102e 55.5e flicker-at  then  then ;

\ ---- the movies replayed (the extras): the lens set, the moonlight ----
: replay-lens ( -- )
    fiona hold  lens-part show-part  dial-2-object object-hide  lens-scene  replay-over ;
: replay-moon ( -- )
    fiona hold  fiona false shown  moon-scene  $37 preload  panic-eased  if  -1 run-scene  then
    fiona true shown  replay-over ;

\ ---- every frame ----
create cameras  6 , 0 , 0 ,   7 , 1 , 1 ,   8 , 1 , 1 ,   9 , 1 , 1 ,   $A , 2 , -1 ,   $B , 2 , -1 ,   \ (area, set, path)
create entries  0 , 0 ,   2 , -1 ,   0 , 0 ,     \ (by exit 0, 1, 2: the camera)
create looks  52.29e -103e 49.3e 30e 12e 12e look,   30.64e -103e -51.68e 30e 12e 12e look,
: ways ( -- )
    0 exit-out  0 1 leaves-by  2 2 leaves-by  cameras 6 cameras-at
    fiona to-exit-1-area entered? if  1 prepare-exit  then
    fiona to-exit-0-area entered? if  0 prepare-exit  then
    fiona to-exit-2-area entered? if
        c1f-14-flag2 fact?  c3f-2-flag1 fact? 0= and if  stalker his-motions motions-for  then  2 prepare-exit
    then ;
: each-frame ( -- )  ways  looks 2 hewie-looks-at  spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if
        hewie send part-from-fiona  replay-act $B = if  ['] replay-moon  else  ['] replay-lens  then  start drop exit
    then
    entries 3 ways-in
    came-by @ dup forced-exit =  swap forced-exit 1 or = or if   \ (by either forced way: $80, $81)
        fiona $56 -90 -68.279e -15.589e place-at  2 -1 player-camera  ['] moon-comes start drop
    then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-1f-12 place!
' items-used castle-1f-12 items-here
