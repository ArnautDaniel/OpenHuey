\ story/places/castle-2f-4.fs - the little TV room in Belli Castle's 2F ($31): brought in by the forced
\ way from the bedroom's TV, her nightmare (a scene - "No---!! This isn't real" - the lit quad over
\ the picture at its shots, her layer and shadow by them) and out by the way loaded ahead; the TV, the
\ fan. A place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/castle-2f-4.fs
\ (converted from the game's, not loaded).
IN: story.places.castle-2f-4
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    effects story.kit scenes ;

own-reset

\ ---- what is said here ----
9 constant dream-msg   \ "No---------!! No...This isn't real... It's just a dream... This can't be happen..."

\ ---- the room's things ----
1 constant tv-fx   0 constant screen-part   1 constant off-part   $1B constant lit-fx
1 constant dream-movie   0 constant dream-cutscene   $21 constant beyond-room

: fan ( -- )  begin  s" fan" 0.0200713e object-turn  yield  again ;   \ (round 1.15 degrees a frame)

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  0e 0e 0e 0 effects:tv-screen tv-fx effect!  screen-part show-part  0 tv-fx tell-effect  off-part hide-part
    ['] fan start drop ;

\ ---- her nightmare: the scene's shots - her layer over, her shadow off, the lit quad over the picture ----
: lit-quad ( -- )  lit-fx $40  0.491e 22e 69.502e  9.502e 22e 60.491e  0.491e 0e 69.502e  9.502e 0e 60.491e  lit-over-all ;
: shot-wait ( shot -- )  begin  cutscene-frame cutscene-shot-at over <> while  yield  repeat  drop ;
: dream-shots ( -- )
    4 shot-wait  0 before-reflections-layer layer
    5 shot-wait  0 usual-layer layer  0 true no-shadow
    6 shot-wait  0 false no-shadow
    9 shot-wait  lit-quad  0 before-reflections-layer layer
    $A shot-wait  lit-fx effect-off  0 usual-layer layer
    begin  cutscene-frame 0< 0= while  yield  repeat  wait-fade ;
: dream-scene ( ahead? -- )   \ (the room beyond loaded ahead: brought in, not replayed)
    dream-movie scenes:opaque-when-movie dream-cutscene load-scene  wait-fade  ['] dream-shots start drop
    if  beyond-room preload  then  movie-opaque state!  panic-eased  if  dream-msg run-scene  then ;
: nightmare ( -- )
    fiona hold  c2f-3-tv-examined fact-off
    true dream-scene  world-held state!  forced-exit take-exit  0 usual-layer layer ;

\ ---- the movie replayed (the extras) ----
: replayed ( -- )  fiona hold  false dream-scene  replay-over ;

\ ---- every frame ----
create cameras  $17 , 0 , 0 ,   $18 , 1 , 1 ,   $19 , 0 , 0 ,   $1A , 2 , 2 ,   \ (area, set, path)
create entries  1 , 1 ,   0 , 0 ,   \ (by exit 0, 1: the camera)
: each-frame ( -- )  cameras 4 cameras-at ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    entries 2 ways-in
    came-by @ forced-exit = if  2 2 player-camera  fiona $A8 45 place-on-tri  world-held state!  ['] nightmare start drop  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-2f-4 place!
