\ story/places/house-of-truth-1f-1.fs - the House of Truth's 1F hall of the blasted wall ($81):
\ Lorenzo following her and the Golden Candlestick in hand, as she comes to the wall - the scene
\ (the wall blown open, the candlestick spent, the way through unlocked; Lorenzo laid in the rubble,
\ the summoner off); the water (a recovery point); the "Extremely Flammable" sign, the fire before
\ the wall, the wreckage after; the Album Medallion Hewie sniffs out; the flame and its sounds; the
\ holes in its floor; the walls' shade; the other game's creatures; the replay. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/house-of-truth-1f-1.fs in
\ the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.house-of-truth-1f-1
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit story.places.house-of-truth fiona.anims scenes ;

own-reset  truth-owns owns !

0 constant fire-msg        \ "The fire spouts showers of sparks into the air."
1 constant flammable-msg   \ "'Caution. Extremely Flammable' I wonder if there are explosives or the like inside. If..."
2 constant wreckage-msg    \ "Wreckage from the wall lies in a mass on the floor. What a powerful explosion."
3 constant water-msg       \ "Life-giving water..."
4 constant good-water-msg  \ "Ahh. Now that is some good water."
own: rested                \ she has drunk here (once a visit)
own: hewie-sent-off        \ Hewie was here as the scene began: back after it
own: blasted               \ the wall blown this visit: Lorenzo laid there (let go as she leaves)
own: steady-on   own: steady-2-on   own: flame-lit
0 constant flame-fx   1 constant album-fx   1 constant calm-sound   0 constant her-motions
2 constant flame-sound   6 constant rubble-sound
$19 constant water-area   $1A constant sign-area   $1B constant wall-area   $1C constant wreckage-area
2 constant blast-movie   1 constant blast-cutscene   $E0 constant blasted-door   $44 constant blast-seen
3 constant his-motions   0 constant his-lying
create holes  $51 , $21C , $2F , $54 , $23B , $43 , $22C , $44 , $22D , $45 , $22E , $46 , $22F , $40 , $229 ,
    $3D , $226 , $3A , $224 , $28 , $216 , $2D , $21A , $53 , $21E , $31 , $56 , $23D , $4B , $234 ,
: wall ( blown? -- )   \ (whole: its floor solid; blown: the rubble's)
    if  0 hide-part  1 show-part  0 solid-floor floor-off  1 solid-floor floor-on
    else  0 show-part  1 hide-part  1 solid-floor floor-off  0 solid-floor floor-on  then ;
: flame ( -- )  flame-fx 1 -24.6e 22.1e -26.4e flame-at ;
: came-in ( exit -- )
    drop  room-sounds  ht1f-1-flag1 fact? dup wall  flame-lit !  flame
    ht1f-1-album-medallion-found fact?  ht1f-1-album-medallion fact? 0= and if  album-fx -78e 1e 39e flicker-at  then
    items-40-as-70 fact?  over-music-d fact? and  ht1f-1-flag2 fact? 0= and if
        ht1f-1-flag2 fact!  this-room @ 0 624 4 10 creature-in  this-room @ 0 321 4 10 creature-in
    then
    30 0 do  holes i cells + @ hole-floor floor-tri-on  loop ;

\ ---- the wall blown: the scene ----
: blast-cues ( -- )   \ (at its shot 6 the wall blown)
    begin  cutscene-frame cutscene-shot-at 6 <> while  yield  repeat  true wall ;
: blast-scene ( -- )
    blast-movie scenes:keyed-movie blast-cutscene load-scene drop  ['] blast-cues start drop
    $10 $FF movie-key  panic-eased  -1 run-scene  ['] blast-cues stop-doing ;
: lorenzo-lies ( -- )   \ (an activity: in the rubble, shown, for good)
    stalker hold  stalker false hush  stalker true shown  stalker his-lying room-motion + start-anim
    begin  yield  again ;
: blast ( -- )
    scene-locked state!  stalkers-stay state!  fiona hold
    15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade
    hewie-in? dup hewie-sent-off !  if  hewie let-go  hewie false shown  then
    stalker let-go  0 summon-take
    flame-fx effect-off  blast-scene
    fiona $EF 50 -71.08e 32.24e place-at
    hewie-sent-off @ if  hewie true shown  hewie activate  hewie $20F 70 -2.73e -54.08e place-at  0 hewie-sent-off !  then
    stalker let-go  0 summon-take  stalker his-motions motions-for  stalker use-motions
    0 solid-floor floor-off  1 keeps-fiona-off keeps-hewie-off or floor-on
    partner activate  this-room @ 457 stalker-to  stalker $1C9 -82 -53.111e 33.905e place-at  stalker activate
    stalker true hush  ['] lorenzo-lies start drop
    no-stalker-camera state!  stalker-no-fear state!  0 0 player-camera  true wall
    blasted-door unlock  golden-candlestick spend  -1 blasted !  summoner-on state-off  force-calm state!
    flame  -1 flame-lit !  15 fade-in
    blast-seen resident-flag-set  stalkers-stay state-off  scene-locked state-off ;
: replayed ( -- )
    fiona hold  false wall  lorenzo-kind 3 figure-in  yield  3 figure-on  blast-scene  3 figure-out  replay-over ;

\ ---- what she may do or say here ----
: the-water ( -- )   \ (a recovery point: once a visit it calms her)
    rested @ if  fiona wait-move  fiona 0e 5e turn-to-xz  good-water-msg say exit  then
    -1 rested !  water-msg her-motions calm-sound $117 0 0e -1.1e recovery-point ;
: look-and-say ( msg anim F: x z -- )
    {: msg anim F: x z :}  fiona wait-move  fiona x z turn-to-xz  fiona anim start-anim  16 frames  msg say  fiona wait-anim ;
: the-sign ( -- )  flammable-msg look-low-anim -32e 34e look-and-say ;
: the-fire ( -- )
    fiona $10C 90 -1 walk-move -30e -26e go-to  fiona look-at-anim start-anim  16 frames  fire-msg say  fiona wait-anim ;
: the-wreckage ( -- )  wreckage-msg look-low-anim -53e 34e look-and-say ;
: pick-album ( -- )  album-medallion ht1f-1-album-medallion album-fx -78e 39e picks-up ;
: album-sniffed ( -- )  ht1f-1-album-medallion-found ht1f-1-album-medallion album-fx  -78e 1e 39e  hewie-finds ;
: spots ( -- )
    fiona water-area in-area?  fiona 0 50 heading? and if  ['] the-water unhurried offer  then
    ht1f-1-flag1 fact? 0= if
        fiona sign-area 50 faces-area? if  ['] the-sign unhurried offer  then
        fiona wall-area in-area?  fiona 45 50 heading? and if  ['] the-fire unhurried offer  then
    else
        fiona wreckage-area in-area?  fiona 30 50 heading? and  blasted @ and if  ['] the-wreckage unhurried offer  then
    then
    ht1f-1-album-medallion-found fact?  ht1f-1-album-medallion fact? 0= and  fiona -78e 1e 39e 5e 5e touches? and if  ['] pick-album calm-only offer  then ;

\ ---- every frame ----
create entries  1 , 1 ,  4 , 4 ,  5 , 5 ,  0 , 0 ,  2 , 2 ,   \ (by exit 0 .. 4: the camera)
create cameras  7 , 0 , 0 ,  8 , 2 , 2 ,  9 , 1 , 1 ,  $A , 2 , 2 ,  $D , 2 , 2 ,  $E , 4 , 4 ,  $F , 2 , 2 ,  $10 , 5 , 5 ,
    $11 , 2 , 2 ,  $12 , 2 , 2 ,  $13 , 2 , 2 ,  $14 , 6 , 6 ,  $15 , 6 , 6 ,  $16 , 6 , 6 ,   \ (area, set, path)
: shade ( -- )
    14e 74e -180e  14e 74e -160e  14e 59e -180e  14e 59e -160e  light-doorway
    14e 74e -160e  10e 74e -80e  10e 39e -160e  7e 39e -80e  light-doorway
    10e 73e -85.5e  43e 73e -85.5e  10e 40e -85.5e  43e 40e -85.5e  light-doorway
    -43e 73e -85.5e  -10e 73e -85.5e  -43e 40e -85.5e  -10e 40e -85.5e  light-doorway
    -70e 55e -60.2e  -38e 78e -60.2e  -70e 40e -60.2e  -38e 60e -60.2e  light-doorway ;
: ways ( -- )
    3 exits-out  3 3 leaves-by  $17 4 leaves-by  cameras 14 cameras-at
    fiona 4 entered? if  0 prepare-exit  then
    fiona $E entered?  fiona $10 entered? or if  1 prepare-exit  then
    fiona 6 entered? if  3 prepare-exit  then
    fiona $18 entered?  fiona $D entered? or  fiona $F entered? or  fiona 5 entered? or if  4 prepare-exit  then
    fiona $1D entered? if  1 map-page  then   fiona $1D left? if  2 map-page  then ;
: each-frame ( -- )
    ways
    ht1f-1-flag1 fact? 0=  fiona wall-area entered? and  danger-now @ calm <> and  golden-candlestick 1 has-items? and if
        ht1f-1-flag1 fact!  panic-eased  ['] blast fiona start-holding drop
    then
    ht1f-1-album-medallion-found fact? 0=  scrambling? and  hewie -78e 0e 39e 10e 5e touches? and if  ['] album-sniffed start-only  then
    steady-on flame-sound -24.6e 22.1e -26.4e steady
    flame-lit @ if  steady-2-on rubble-sound -21.86e 0e 33.51e steady  then
    shade  spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    entries 5 ways-in
    came-by @ dup 0 3 within  swap 4 = or if  2 map-page  then ;
: went ( -- )   \ (leaving after the blast: Lorenzo let go, her calm and the camera as they were)
    blasted @ 0= if  exit  then
    force-calm state-off  ['] lorenzo-lies stop-doing  stalker let-go  3 summon-take
    no-stalker-camera state-off  stalker-no-fear state-off ;

' came-in ' person-in ' each-frame ' went house-of-truth-1f-1 place!
