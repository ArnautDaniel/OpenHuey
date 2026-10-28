\ story/places/old-mansion-1f-16.fs - the reservoir hall of the old mansion's 1F: the light rippling
\ off the water (the ceiling's glass; once drained, low down, drops falling); the first time she
\ stands by the way in, she goes on; at the door to the waterway, Daniella's scene (the creatures
\ gone, out by it); the locked door with its odd marking (the Mars Key opens it); the water pooled
\ below; the Refined Magnesia; the map's pages. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/old-mansion-1f-16.fs in the history (converted from the game's; gone
\ with the old machinery in 0.2.20).
IN: story.places.old-mansion-1f-16
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names effects story.kit fiona.anims scenes ;

own-reset

0 constant locked-msg    \ "The door is locked..."
1 constant marking-msg   \ "There's an odd marking on the door. What could it mean?"
2 constant water-msg     \ "A large amount of water is pooled below. I wonder if this room is used as a reservoir o..."
own: locked-remarked  own: stood-in
0 constant magnesia-fx   $1B constant glass-fx   0 constant key-sound   $609 constant key-turn-anim
4 constant marked-exit   $63 constant marked-door   $62 constant waterway-door   1 constant waterway-exit
2 constant waterway-movie   1 constant waterway-cutscene   0 constant her-motions

: ripples ( drained? -- )
    if  3 $80808080  0e -5.9e 0e  160e 0.5e 0.1e  effects:caustic drop  effects:rain  exit  then
    glass-fx false $80  30e 45e -30e  -30e 45e -30e  30e 45e 30e  -30e 45e 30e  glass-in
    3 $80808080  0e 45.1e 0e  160e 0.5e 0.1e  effects:caustic drop ;
: came-in ( exit -- )
    drop  room-sounds  $11 1e background
    om2f-12-flag1 fact? if  1 show-part  true ripples  else  0 show-part  false ripples  then
    om1f-16-refined-magnesia fact? 0= if  magnesia-fx 65.56e 94.52e 19.77e flicker-at  then
    1 $3FFF reverb ;

\ ---- the first time by the way in (before the 3F's turn): on by it ----
: goes-on ( -- )  stalkers-stay state!  fiona hold  15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade  world-held state!  2 take-exit ;

\ ---- the door to the waterway: Daniella's scene, out by it ----
: waterway-scene ( -- )
    waterway-movie scenes:keyed-movie waterway-cutscene load-scene  $5A $FF movie-key  0 hide-part
    panic-eased  if  -1 run-scene  then ;
: by-the-waterway ( -- )
    stalkers-stay state!  fiona hold  fiona waterway-exit to-door
    15 screen:to-black-2 with-music or fade  no-track 1e background
    waterway-movie scenes:keyed-movie waterway-cutscene load-scene  wait-fade
    hewie let-go  0 s" creatures" actor-named send creatures-clear  $5A $FF movie-key  0 hide-part
    panic-eased  if  -1 run-scene  then
    world-held state!  waterway-door unlock  waterway-exit take-exit ;

\ ---- the marked door: locked (then its marking); the Mars Key ----
: tries-door ( -- )
    locked-remarked @ 0=  chased? or  in-panic? or if  marked-exit false locked-msg tries-locked  -1 locked-remarked !  exit  then
    fiona wait-move  4 frames  fiona $18C -89 -64.3e 0e place-at  world-frozen state!  1 20e 5e 0e 4e shot  4 frames
    marking-msg say  4 frames  shot-off  fiona $18C -89 -64.3e 1.25e place-at  world-frozen state-off ;
: key-used ( -- )  marked-exit marked-door mars-key key-turn-anim key-sound room-bank key-turns ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0<> if  drop false exit  then
    2 = if  ['] key-used start drop  true  else  false  then ;

\ ---- what she may do here ----
: the-water ( -- )
    fiona wait-move  fiona $19F 166 -6.04e 30.79e walk-to  fiona true noclip  yield  world-frozen state!
    fiona $19E 166 -4.65e 24.47e place-at  1 35e 55e 0e 0e shot  water-msg say  16 frames
    fiona false noclip  yield  world-frozen state-off  fiona $19F 166 -6.04e 30.79e place-at  shot-off ;
: take-magnesia ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona 65.56e 19.77e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  refined-magnesia om1f-16-refined-magnesia magnesia-fx takes-up  fiona reach-back-anim play ;

\ ---- the movie replayed (the extras) ----
: replayed ( -- )  fiona hold  false ripples  waterway-scene  replay-over ;

create entries  0 , 0 ,   0 , 0 ,   1 , 1 ,   1 , 1 ,   1 , 1 ,   \ (by exit 0..4: the camera)
create ahead  5 , 0 ,   6 , 1 ,   7 , 2 ,   8 , 3 ,   9 , 3 ,   $A , 4 ,   \ (area, exit loaded ahead)
: map-pages ( -- )
    fiona 6 entered?  fiona 8 entered? or  fiona 9 entered? or if  2 map-page  then
    fiona 6 left?  fiona 8 left? or if  3 map-page  then   fiona 9 left? if  1 map-page  then ;
: each-frame ( -- )
    events-held state? if  exit  then
    0 exit-out  1 exit-out  3 exit-out  4 exit-out
    om3f-2-flag1 fact? 0=  stood-in @ 0= and if
        fiona 2 in-area? if  -1 stood-in !  panic-eased  ['] goes-on start drop  then
    else  2 exit-out  then
    ahead 6 ways-out  map-pages
    fiona 5 entered?  found-file-233 fact? and  found-file-234 fact? 0= and if  partner her-motions motions-for  then
    found-file-234 fact? 0=  fiona waterway-exit before-door? and if  ['] by-the-waterway any-time offer  then
    5 locked-request?  fiona marked-exit before-door? and if  ['] tries-door any-time offer  then
    fiona $B in-area?  fiona 166 60 heading? and if  ['] the-water unhurried offer  then
    om1f-16-refined-magnesia fact? 0=  fiona 65.56e 94.52e 19.77e 5e 5e touches? and if  ['] take-magnesia calm-only offer  then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop  exit  then
    entries 5 ways-in
    came-by @ dup 0 =  over 2 = or if  3 map-page  then  dup 1 =  swap 3 = or if  2 map-page  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-16 place!
' items-used old-mansion-1f-16 items-here
