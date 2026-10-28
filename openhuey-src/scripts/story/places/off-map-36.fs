\ story/places/off-map-36.fs - room $36 (on no map page), an ending's scene: brought in by the forced
\ way - the world held, Hewie let go; the scene, then the game over of the story's end (the ending
\ A); replayed from the extras: the same scene, she unseen, then back. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/off-map-36.fs (converted from the game's, not
\ loaded).
IN: story.places.off-map-36
USING: engine game-state actors messages common flag-names facts room-names story-names
    story.kit scenes ;

1 constant the-movie   0 constant the-cutscene   1 constant seen-flag   3 constant figure-slot
0 constant ending-a   2 constant story-over
: came-in ( exit -- )  drop ;

: the-scene ( -- )
    the-movie scenes:unseen-movie the-cutscene load-scene  wait-fade
    panic-eased  if  -1 run-scene  then ;
: the-end ( -- )
    seen-flag resident-flag? 0= if  no-pause state!  then
    world-held state!  hewie let-go  fiona hold
    the-scene  figure-slot figure-out  ending-a ending-var var!  story-over game-over ;
: replayed ( -- )
    fiona hold  fiona wait-move  fiona false shown
    the-movie scenes:unseen-movie the-cutscene load-scene  wait-fade  off-map-37 preload
    panic-eased  if  -1 run-scene  then
    figure-slot figure-out  fiona true shown  replay-over ;

\ ---- every frame ----
: each-frame ( -- ) ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    came-by @ forced-exit = if  ['] the-end fiona start-holding drop  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went off-map-36 place!
