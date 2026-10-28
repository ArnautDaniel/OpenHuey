\ story/places/off-map-33.fs - room $33 (on no map page), an ending's scene: brought in by the
\ forced way once the forest's heart is behind her - the world held, her panic eased; the scene (her
\ double's - the TintStalker - figure without its shadow, the room's part away from its fourth
\ shot), then the game over of the story's end (the ending D); replayed from the extras: the same
\ scene, then back. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-33.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.off-map-33
USING: engine game-state actors messages common flag-names facts room-names story-names stalker-names
    story.kit scenes ;

1 constant the-movie   0 constant the-cutscene   0 constant room-part   4 constant part-away-shot
3 constant ending-d   2 constant story-over   4 constant seen-flag   3 constant her-double   4 constant other-slot
: came-in ( exit -- )  drop  room-part show-part ;

\ ---- the scene ----
: part-away ( -- )   \ (the room's part gone from its fourth shot, to the end)
    yield  begin  part-away-shot scene-shot? if  room-part hide-part  then  scene-near-end? 0= while  yield  repeat ;
: the-scene ( -- )   \ (her double without its shadow; the movie)
    fiona false shown  tintstalker-kind true no-shadow
    the-movie scenes:unseen-movie the-cutscene load-scene  ['] part-away start drop
    panic-eased  if  -1 run-scene  then ;
: the-end ( -- )
    seen-flag resident-flag? 0= if  no-pause state!  then
    stalker let-go  hewie let-go  fiona hold  fiona wait-move
    the-scene  her-double figure-out  other-slot figure-out
    ending-d ending-var var!  world-held state!  story-over game-over
    tintstalker-kind false no-shadow ;
: replayed ( -- )
    fiona hold  fiona wait-move  room-part show-part  tintstalker-kind her-double figure-in  yield
    the-scene  off-map-37 preload  her-double figure-out  other-slot figure-out
    tintstalker-kind false no-shadow  replay-over ;

\ ---- every frame ----
: each-frame ( -- ) ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    cf-13-flag1 fact?  came-by @ forced-exit = and if
        fiona 0 0 place-on-tri  world-held state!  panic-eased  ['] the-end fiona start-holding drop
    then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went off-map-33 place!
