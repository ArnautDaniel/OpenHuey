\ story/places/old-mansion-1f-17.fs - a room of the old mansion's 1F with the "Blue Flame, Secret
\ Ways" picture and the candle's flame (its draught stirred by whoever passes); once the 2F's way
\ was found, the wall moved aside (brought in by it the first time: the movie, out by the forced
\ way); the lever by the way she came; the "doll" that is a mummy; two vases (the Magnesia in one);
\ the Chicken Jerky; the Medallion Hewie sniffs out; his looks. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/old-mansion-1f-17.fs in the history (converted
\ from the game's; gone with the old machinery in 0.2.20).
IN: story.places.old-mansion-1f-17
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names story.kit fiona.anims fiona.sounds scenes ;

own-reset

0 constant picture-msg     \ "'Blue Flame, Secret Ways' Strange title for a picture. Unless it could be some sort..."
1 constant lamp-msg        \ "A female is raising an oil lamp. A blue flame burns brightly from its tip. It's alm..."
2 constant mummy-msg       \ "This... isn't a doll! It's a mummy!"
3 constant mummy-2-msg     \ "Why in the world is this here!?"
own: picture-remarked  own: spot-noticed
0 constant jerky-fx   1 constant magnesia-fx   2 constant medallion-fx   3 constant flame-fx
3 constant vase-sound   1 constant secret-movie   0 constant secret-cutscene   $5A constant way-door
om1f-17-breakable1-broken 0 1  43e 0e -40e 5e 8e  breakable: the-vase
the-vase $80181818 0 8 shard-look  the-vase 0.562e 0.5e 0.25e 0.5e shard-uv
the-vase 1 frees-floor  the-vase $126 noise-from  the-vase vase-sound room-bank shatters-with
om1f-17-breakable2-broken 2 3  -138.5e -50e -7.5e 5e 8e  breakable: low-vase
low-vase $80282828 0 8 shard-look  low-vase 0.812e 0.687e 0.187e 0.312e shard-uv
low-vase 2 frees-floor  low-vase 5 noise-from

\ ---- the wall moved aside (the 2F's way found); the lever ----
: wall-aside ( -- )   \ (the plate, wall, picture and light: 60 to the left)
    s" plate" place-x -60e object+!  s" wall" place-x -60e object+!
    s" picture" place-x -60e object+!  s" light" place-x -60e object+! ;
: lever ( deg -- )  s>f deg>rad s" doll" turn-y object! ;
: lever-by-way ( -- )   \ (as the way she came by left it)
    came-by @ case  0 of  -60 lever  endof  3 of  0 lever  endof  1 of  60 lever  endof  2 of  60 lever  endof  endcase ;

\ ---- as she comes in ----
: magnesia-shows ( -- )  om1f-17-magnesia fact? 0= if  magnesia-fx 43e 1e -40e flicker-at  then ;
: came-in ( exit -- )
    drop  room-sounds
    way-door closed-off? if  4 show-part  3 solid-floor floor-on  then
    om2f-8-flag1 fact? if  wall-aside  else  0 barred-floor floor-on  then
    the-vase as-it-stands  the-vase broken? if  magnesia-shows  then  low-vase as-it-stands
    om1f-17-chicken-jerky fact? 0= if  jerky-fx -137.37e -41e -14.57e flicker-at  then
    om1f-17-medallion-found fact?  om1f-17-medallion fact? 0= and if  medallion-fx 68.99e 1e 19.99e flicker-at  then
    flame-fx 1 -84e -19e 13.7e flame-at
    1 $2300 reverb ;

\ ---- brought in by the way behind the wall the first time: the movie, out by the forced way ----
: secret-scene ( -- )
    secret-movie scenes:keyed-movie secret-cutscene load-scene  $14 $C8 movie-key
    panic-eased  if  -1 run-scene  then ;
: the-way-behind ( -- )
    fiona hold  world-frozen state!  secret-scene  wait-fade
    world-held state!  world-frozen state-off  forced-exit take-exit ;
: replayed ( -- )
    fiona hold  flame-fx 1 -84e -19e 13.7e flame-at  fiona false shown  secret-scene  fiona true shown  replay-over ;

\ ---- what she may do or say here ----
: the-picture ( -- )
    fiona wait-move  fiona -90 turn
    picture-remarked @ if  0 picture-remarked !  lamp-msg  else  -1 picture-remarked !  picture-msg  then  say ;
: the-mummy ( -- )
    fiona wait-move  fiona $137 90 57.06e 30.88e walk-to  fiona hold
    mummy-examined fact? 0= if
        seized-sound common-bank fiona sound-by  fiona pull-free-anim 10 start-blend  $5A fright+  1 $FF 8 rumble-pad
        fiona wait-anim  world-frozen state!  1 5e 10e 0e 0e shot  mummy-examined fact!  mummy-msg say
    else
        world-frozen state!  1 5e 10e 0e 0e shot  mummy-2-msg say
    then
    16 frames  world-frozen state-off  shot-off ;
: take-jerky ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona -137.37e -14.57e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  chicken-jerky om1f-17-chicken-jerky jerky-fx takes-up  fiona reach-back-anim play ;
: take-magnesia ( -- )  magnesia om1f-17-magnesia magnesia-fx 43e -40e picks-up ;
: take-medallion ( -- )  medallion om1f-17-medallion medallion-fx 68.99e 19.99e picks-up ;
: medallion-sniffed ( -- )  om1f-17-medallion-found om1f-17-medallion medallion-fx  68.99e 1e 19.99e  hewie-finds ;
: wary-of-it ( -- )  68.2e 0e 31.14e hewie-stands-by ;

create cameras  $A , 0 , 0 ,   $B , 1 , 1 ,   $C , 0 , 0 ,   $D , 2 , 2 ,   \ (area, set, path)
create ahead  4 , 0 ,   6 , 0 ,   7 , 1 ,   5 , 3 ,   \ (area, exit loaded ahead)
create wall-look  -15.99e 0e 0.03e  25e 22e 22e look,
create calm-looks  54.73e 0e -58.43e  19e 22e 22e look,   -125.41e -50e 1.03e  20e 14e 14e look,
create mummy-look  68.2e 0e 31.14e  20e 12e 12e look,
: hewie-about ( -- )
    om2f-8-flag1 fact? 0= if  wall-look 1 hewie-looks-at  then
    calm-looks 2 hewie-looks-at
    spot-noticed 50 68.2e 0e 31.14e 20e 12e notices? if  ['] wary-of-it start drop  then
    mummy-look 1 hewie-looks-unchased
    om1f-17-medallion-found fact? 0=  scrambling? and  hewie 68.99e 0e 19.99e 10e 5e touches? and if  ['] medallion-sniffed start-only  then ;
: map-pages ( -- )  fiona $11 entered? if  2 map-page  then  fiona $11 left? if  1 map-page  then ;
: each-frame ( -- )
    way-door closed-off? 0= if  0 exit-out  then  1 exit-out  3 3 leaves-by
    cameras 4 cameras-at  ahead 4 ways-out  map-pages
    low-vase breaks? drop  hewie-about
    flame-fx -84e -19e 13.7e 8e -4e flame-stirred
    om2f-8-flag1 fact? 0=  fiona $E in-area? and  fiona -45 50 heading? and if  ['] the-picture unhurried offer  then
    fiona $10 in-area?  fiona 45 60 heading? and if  ['] the-mummy unhurried offer  then
    om1f-17-chicken-jerky fact? 0=  fiona -137.37e -41e -14.57e 5e 5e touches? and if  ['] take-jerky calm-only offer  then
    om1f-17-medallion-found fact?  om1f-17-medallion fact? 0= and  fiona 68.99e 0e 19.99e 5e 5e touches? and if
        ['] take-medallion calm-only offer
    then
    the-vase broken? if
        om1f-17-magnesia fact? 0=  fiona 43e 0e -40e 5e 5e touches? and if  ['] take-magnesia calm-only offer  then
    else  the-vase breaks? if  magnesia-shows  then  then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop  exit  then
    0 2 2 way-in  1 0 0 way-in  3 1 1 way-in   \ (by exit 2 no camera of its own)
    came-by @ forced-exit = if  fiona $12C put-on-tri  1 -1 player-camera  ['] the-way-behind start drop  then
    lever-by-way  came-by @ 2 < if  2 map-page  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-17 place!
