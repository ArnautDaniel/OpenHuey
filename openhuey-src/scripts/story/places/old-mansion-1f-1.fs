\ story/places/old-mansion-1f-1.fs - the old mansion's entrance hall: the first time she steps in,
\ Daniella's scene (the glass over the picture, the far view opening out) - after it Daniella at her
\ task there (her sound, no fright, no camera on her) and Hewie wary of her; "She's gone utterly
\ mad..."; once the hall has flooded, its water; the map's pages. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/old-mansion-1f-1.fs in the history (converted
\ from the game's; gone with the old machinery in 0.2.20).
IN: story.places.old-mansion-1f-1
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags story.kit fiona.anims scenes ;

own-reset

0 constant mad-msg   \ "She's gone utterly mad. Best to keep my distance..."
own: stepped-in   own: mad-remarked   own: hewie-wary   own: busy-step
$1B constant glass-fx   5 constant presence-sound   2 constant her-motions
1 constant hall-movie   0 constant hall-cutscene   $233 constant hall-file

: glass ( over-all? -- )
    glass-fx swap $80  -15.959e 111e -2e  -15.959e 111e 6e  -15.959e 91e -2e  -15.959e 91e 6e  glass-in ;
: met? ( -- flag )  found-file-233 fact?  found-file-234 fact? 0= and ;   \ (her scene had, the hall not flooded)

\ ---- Daniella at her task in the hall ----
: busy ( -- )
    stalker-present  stalker hold  stalker her-motions motions-for  stalker true noclip  stalker $68 -101 -4.538e 3.132e place-at
    force-followed state!  stalker-no-fear state!  stalker use-motions
    begin  room-sounds-in? 0= while  yield  repeat
    stalker false hush  0 barred-floor floor-on  1 busy-step !
    presence-sound room-bank stalker sound-by
    begin  stalker room-motion play  yield  again ;
: daniella-in ( -- )  partner activate  old-mansion-1f-1 104 stalker-to  stalker 1 1 camera-set ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds
    found-file-234 fact? if  1 show-part  1 water-floor floor-on  exit  then
    false glass  0 show-part
    found-file-233 fact? if
        force-followed state!  stalker-no-fear state!  no-stalker-camera state!
        partner true hush  daniella-in  ['] busy start drop  0 keeps-hewie-off floor-on
    then ;

\ ---- her scene: the first time she steps in ----
: hall-shots ( -- )
    depth-range-off
    begin  cutscene-frame $10E < while  yield  repeat
    begin  cutscene-frame cutscene-shot-at 5 <> while  yield  repeat  true glass  yield
    begin  cutscene-frame $3A2 < while  yield  repeat  false glass
    begin  cutscene-frame $4B0 < while  yield  repeat
    begin  cutscene-frame $4EC < while  1e 21e 40e 80e depth-range  yield  repeat
    begin  cutscene-frame 0< 0= while   \ (opening out from frame 1260: 0.4 a frame, to 41 / 61 / 80 / 120)
        cutscene-frame 1260 - s>f 0.4e f*
        fdup 1e f+ 41e fmin  fover 21e f+ 61e fmin  frot fdup 40e f+ 80e fmin  fswap 80e f+ 120e fmin  depth-range
        yield
    repeat ;
: hall-scene ( -- )
    hall-movie scenes:unseen-movie hall-cutscene load-scene  ['] hall-shots start drop  4 cutscene-keep-model
    panic-eased  if  -1 run-scene  then ;
: her-scene ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move
    15 screen:to-black-2 with-music or fade  hall-movie scenes:unseen-movie hall-cutscene load-scene  wait-fade
    hewie full-health  hewie well  old-mansion-1f-1 49 0 hewie-to  $86 0 hewie-do
    ['] hall-shots start drop  4 cutscene-keep-model  panic-eased  if  -1 run-scene  then
    wait-fade  world-held state!
    fiona 0 idle  fiona $DF -79 1.052e -47.906e place-at  0 0 player-camera  camera-restart
    0 busy-step !  daniella-in  ['] busy start drop
    hewie let-go  hewie $31 30 place-on-tri  found-file-233 fact!  1 prepare-exit  false glass
    begin  busy-step @ 0= while  yield  repeat
    5 0 0 stage-music  15 fade-in  hall-file give-file
    stalkers-stay state-off  no-stalker-camera state! ;
: mad ( -- )
    fiona hold  -1 mad-remarked !  fiona wait-move  fiona 0 turn  fiona turn-round-anim play
    fiona stalker look-at  yield  mad-msg say  fiona -1 look-at  yield  0 mad-remarked ! ;
: wary-of-her ( -- )  -3.91e 90e 3.23e hewie-stands-by ;

\ ---- the movie replayed (the extras) ----
: replayed ( -- )
    fiona hold  0 show-part  daniella-kind stalker-ready  yield  hall-scene  false glass  stalker-unloaded  replay-over ;

create cameras  4 , 1 , 1 ,   5 , 0 , 0 ,   6 , 0 , 0 ,   7 , 1 , 1 ,   \ (area, set, path)
create entries  0 , 0 ,   0 , 0 ,   \ (by exit 0, 1: the camera)
create ahead  2 , 0 ,   3 , 1 ,   \ (area, exit loaded ahead)
create looks  -3.91e 90e 3.23e  80e 26e 15e look,
: each-frame ( -- )
    events-held state? if  exit  then
    2 exits-out  cameras 4 cameras-at  ahead 2 ways-out
    fiona 8 entered? if  3 map-page  then   fiona 8 left? if  2 map-page  then
    fiona $A entered? if  2 map-page  then   fiona $A left? if  1 map-page  then
    c3f-2-flag1 fact?  found-file-233 fact? 0= and if  0 stalker-alert? 0= if  0 stalker-order  then  then
    found-file-233 fact? 0= if
        stepped-in @ 0=  fiona 8 entered? and if  -1 stepped-in !  panic-eased  ['] her-scene start drop  then
    else  found-file-234 fact? 0= if
        fiona 9 entered?  mad-remarked @ 0= and if  panic-eased  ['] mad start drop  then
    then  then
    met? if
        hewie-wary 100 -3.91e 90e 3.23e 80e 26e notices? if  ['] wary-of-her start drop  then
        looks 1 danger-now @ followed = hewie-looks-if
    then
    room-sounds-in?  stalker-in? and if  presence-sound keep-playing or room-bank stalker sound-by  then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop  exit  then
    entries 2 ways-in  came-by @ 1 = if  3 map-page  then ;
: went ( -- )
    force-followed state-off
    met? if  stalker-no-fear state-off  no-stalker-camera state-off  partner false hush  stalker let-go  then ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-1 place!
