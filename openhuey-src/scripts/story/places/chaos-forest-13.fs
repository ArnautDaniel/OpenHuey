\ story/places/chaos-forest-13.fs - the Chaos Forest's heart ($10B): once Hewie has been found, the
\ first time she steps in, the scene (Riccardo; the ash rising between its cues, the doorway's light
\ after) - then on: dressed anew (her bare feet's sounds) to the water tower (or, the cage's garden
\ seen, the old mansion's 2F); with Hewie never bandaged and mistreated, out the other way. The room
\ ahead loaded as she crosses toward the way out; the maze dealt; the replay. A place (story/kit.fs).
\ The original's scripts, for checking: story/rooms-ref/chaos-forest-13.fs (converted from the
\ game's, not loaded).
IN: story.places.chaos-forest-13
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    effects story.kit story.places.forest scenes ;

own-reset  forest-owns owns !

0 constant heart-msg       \ "Why are you doing this? What did I do? You inherited your father's Azoth. That..." (the scene's)
own: ash-on                \ the ash rising (between the scene's cues)
own: doorway-lit           \ the doorway's light on (from the scene's late cue)
$19 constant forest-track   0 constant ash-fx   1 constant heart-area   3 constant ahead-area
1 constant heart-movie   0 constant heart-cutscene   $17 constant heart-figure   3 constant heart-slot
3 constant tower-costume   $C0 constant tower-way   $59 constant mansion-way   $34 constant other-way

: came-in ( exit -- )  drop  forest-track 1e background  maze-in ;
: kept-well? ( -- flag )   \ (Hewie bandaged, or never mistreated: the tower's way)
    cf-10-flag1 fact?  progress pr.hewie-mistreated l@ 100 < or ;
: way-on ( -- room )  kept-well? if  fg-3-flag3 fact? if  mansion-way  else  tower-way  then  else  other-way  then ;

\ ---- the scene ----
: ash-rises ( on -- )  dup ash-on !  if  effects:ash ash-fx effect!  else  ash-fx effect-off  then ;
: till-shot ( n -- )  begin  cutscene-frame cutscene-shot-at over <>  while  yield  repeat  drop ;
: heart-cues ( -- )  1 till-shot  true ash-rises  2 till-shot  false ash-rises  $18 till-shot  -1 doorway-lit ! ;
: heart-scene ( -- )
    heart-movie scenes:opaque-when-movie heart-cutscene load-scene drop
    heart-slot figure-on  5 cutscene-keep-model  ['] heart-cues start drop  0 ash-on !
    panic-eased  heart-msg run-scene  ['] heart-cues stop-doing ;
: the-heart ( -- )
    fiona hold  summoner-on state-off  stalker let-go  0 summon-take  hewie-in? if  hewie let-go  then
    fiona wait-move  15 screen:to-black-2 with-music or fade  no-track 1e background  wait-fade
    items-40-as-70 fact? if  creatures-on state-off  spirits-gone  then
    0 s" creatures" actor-named send creatures-clear
    cf-13-flag1 fact!  heart-figure heart-slot figure-in  yield
    heart-scene
    stalker let-go  0 summon-take  0 doorway-lit !  ash-on @ if  false ash-rises  then
    world-held state!
    kept-well? if
        tower-costume dress  steps-in
        way-on dup preload  tower-way = if  heart-slot figure-out  then
    else
        heart-slot figure-out  other-way preload
    then
    forced-exit take-exit ;
: replayed ( -- )
    fiona hold  riccardo-kind stalker-ready  heart-figure heart-slot figure-in  yield
    heart-scene  stalker-unloaded  heart-slot figure-out  replay-over ;

\ ---- every frame ----
: each-frame ( -- )
    0 0 leaves-by
    fiona 2 entered? if  0 prepare-exit  then
    fiona ahead-area entered? if  way-on preload  then
    fiona heart-area entered?  found-file-247 fact? and  cf-13-flag1 fact? 0= and if
        4 figure-out  5 figure-out  cf-13-flag1 fact!  panic-eased  ['] the-heart start drop
    then
    maze-frame
    doorway-lit @ if  -60e 40e -30e  10e 46.5e -46e  -60e 0e -40e  10e -10e -59e  light-doorway  then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    0 0 0 way-in ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went chaos-forest-13 place!
