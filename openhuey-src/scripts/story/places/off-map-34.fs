\ story/places/off-map-34.fs - room $34 (on no map page), Lorenzo's scene: brought in by the forced
\ way once the forest's heart is behind her - the world held, her panic eased; the scene ("Ah,
\ you're up. Fiona..."; the room's six parts shown and hidden by its cues), the figure and $33's
\ figure loaded, on to $33 by the forced way; replayed from the extras: the same, its double loaded
\ for it, on to $33's replay. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-34.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20; its act $4D is read past the room's table and never runs).
IN: story.places.off-map-34
USING: engine game-state actors messages common flag-names facts room-names story-names stalker-names
    story.kit scenes ;

0 constant lorenzo-msg   \ "Ah, you're up. Fiona... I've decided you shall give birth to me. Don't worry, I..."
1 constant the-movie   0 constant the-cutscene   4 constant seen-flag   3 constant her-double   4 constant next-slot
$20 constant next-figure   $2C constant replaying-var
: came-in ( exit -- )  drop ;

\ ---- the scene: the room's parts by its cues ----
: parts! ( p0 p1 p2 p3 p4 p5 -- )  6 0 do  5 i - swap part  loop ;   \ (each on or off, part 0 first)
create cues  $547 , $569 , $587 , $592 , $59E , $5AE , $5D4 , $5EE , $610 ,
create looks   \ (the parts after each cue: 0 .. 5)
    -1 , 0 , 0 , 0 , -1 , 0 ,    -1 , -1 , 0 , 0 , 0 , 0 ,    0 , -1 , 0 , 0 , 0 , -1 ,
    0 , 0 , 0 , -1 , 0 , -1 ,    -1 , 0 , -1 , 0 , 0 , 0 ,    -1 , 0 , 0 , -1 , 0 , 0 ,
    0 , -1 , 0 , -1 , 0 , -1 ,   -1 , 0 , 0 , 0 , -1 , 0 ,    -1 , -1 , 0 , 0 , 0 , 0 ,
: look ( k -- )  {: k :}  6 0 do  k 6 * i + cells looks + @  loop  parts! ;
: by-cues ( -- )
    -1 -1 0 0 0 0 parts!
    9 0 do  begin  i cells cues + @ scene-at? 0= while  yield  repeat  i look  loop
    begin  scene-near-end? 0= while  yield  repeat ;
: the-scene ( -- )
    the-movie scenes:unseen-movie the-cutscene load-scene  ['] by-cues start drop
    panic-eased  if  lorenzo-msg run-scene  then ;
: meets-him ( -- )
    seen-flag resident-flag? 0= if  no-pause state!  then
    stalker let-go  hewie let-go  fiona hold  fiona wait-move
    the-scene  world-held state!  stalker let-go
    next-figure next-slot figure-in  off-map-33 preload  forced-exit take-exit ;
: replayed ( -- )
    fiona hold  fiona wait-move  tintstalker-kind her-double figure-in  yield
    the-scene  next-figure next-slot figure-in  off-map-33 preload  her-double figure-out
    forced-exit take-exit  1 replaying-var var! ;

\ ---- every frame ----
: each-frame ( -- ) ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    cf-13-flag1 fact?  came-by @ forced-exit = and if
        fiona 0 0 place-on-tri  world-held state!  panic-eased  ['] meets-him fiona start-holding drop
    then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went off-map-34 place!
