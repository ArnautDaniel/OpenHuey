\ story/places/old-mansion-b1-4.fs - the workshop of the old mansion's B1: a cabinet to hide in (the
\ kit's: Hewie beside her; the stalker's grab, or his opening it - a mandragora she carries cries
\ out); the first time Daniella is here working at the kiln - her back to Fiona (the cabinet no use
\ now: "She's right there"), and going up to her, her scene: Daniella turns, and gives chase as the
\ next of her; the wax figures, the chess board, the hourglass, the models, the flask, the kiln, the
\ still; six candles; the file once its flag is set; Hewie tense at her, his looks; its hum. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-b1-4.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.old-mansion-b1-4
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit fiona.anims scenes ;

own-reset

2 constant wax-msg       \ "A pair of creepy wax figures. These things aren't going to come to life like in the mov..."
3 constant chess-msg     \ "A chess board? Here of all places? Guess they used this to take their mind off things."
4 constant hourglass-msg \ "A massive hourglass... Turning this over would be quite a task."
5 constant models-msg    \ "There are models and their accompanying sketches. I haven't seen any of these bugs befo..."
6 constant flask-msg     \ "There is a flask... I wonder what kind of liquid is inside?"
7 constant kiln-msg      \ "An industrial-sized kiln... It burns fiery hot."
8 constant still-msg     \ "A device for distilling things..."
9 constant right-there-msg \ "She's right there, stupid. What good is hiding in the closet going to do now?"
own: waiting   \ Daniella at the kiln, her back to her
own: her-noticed  own: sounds-on  own: hewie-sent
2 constant shriek-sound   3 constant hum-sound   4 constant presence-sound   $C constant cabinet-area
7 constant kiln-tri   5 constant kiln-motions   7 constant turns-movie   6 constant turns-cutscene
omb1-4-flag1 $11 0  $7B 0  $23 0  $23 0  $3C 28
    19.42e 25.02e  24.09e 21.5e  21.86e 24.14e  14.4e 16.6e  cabinet: the-cabinet

\ ---- Daniella at the kiln ----
: at-the-kiln ( -- )   \ (an activity: her back to the room, at work)
    stalker-present  stalker 2 2 camera-set  stalker hold  stalker true noclip  stalker kiln-tri -90 -25.05e -20.33e place-at  1 barred-floor floor-on
    stalker kiln-motions motions-for  stalker use-motions  1 her-back
    begin  presence-sound room-bank stalker sound-by  stalker room-motion play  again ;
: waiting-due? ( -- flag )   \ (the first time, Daniella the stalker not yet after her)
    x-37-flag1 fact?  partner-kind dup daniella-kind = swap kind34-kind = or 0= or  stalker-told? or if  false exit  then
    om1f-13-flag2 fact? 0=  white-mandragora-used fact? or ;
: she-turns ( -- )   \ (her scene: she turns - the next of her after Fiona)
    x-37-flag1 fact!  0 waiting !  stalkers-stay state!  fiona hold
    15 screen:to-black-2 with-music or fade  turns-movie scenes:keyed-movie turns-cutscene load-scene  wait-fade
    ['] at-the-kiln stop-doing  4 cutscene-keep-model
    hewie-in? dup hewie-sent !  if  hewie let-go  then
    $10 $FF movie-key  panic-eased  if  -1 run-scene  then
    world-held state!  stalker let-go  costume-var var@ dress  yield
    fiona $57 -108 -1.12e -15.98e place-at  camera-restart  1 barred-floor floor-off
    stalker-unloaded  kind34-kind stalker-ready  yield  partner activate  old-mansion-b1-4 7 stalker-to
    stalkers-stay state-off  force-followed state-off  stalker-no-fear state-off  no-stalker-camera state-off
    stalker kiln-tri -68 -24.52e -20.6e place-at  stalker 2 2 camera-set  world-held state-off
    hewie-sent @ if  hewie activate  old-mansion-b1-4 30 0 hewie-to  then
    15 fade-in  $2D resident-flag-set  force-followed state-off ;
: right-there ( -- )  fiona wait-move  fiona stalker look-at  yield  right-there-msg say ;
: tense ( -- )  -27.99e 0e -19.65e hewie-barks-tense ;

\ ---- as she comes in ----
create flames  -3.3e f, 31.4e f, -4.3e f,   -5.7e f, 31.4e f, 0.3e f,   -2.8e f, 31.7e f, 5e f,
    2.9e f, 31.1e f, -4.4e f,   5.6e f, 31.5e f, 0.5e f,   2.7e f, 31.7e f, 5e f,
: candles-lit ( -- )  6 0 do  i 0  flames i 3 * floats +  dup f@  dup float+ f@  2 floats + f@  flame-at  loop ;
: came-in ( exit -- )
    drop  room-sounds  the-cabinet cabinet-shown  1 barred-floor floor-off
    waiting-due? waiting !
    waiting @ if
        stalkers-stay state!  force-followed state!  stalker-no-fear state!  no-stalker-camera state!
        partner activate  old-mansion-b1-4 7 stalker-to  ['] at-the-kiln start drop
    then
    candles-lit
    $651 fact? if  $238 give-file  then ;

\ ---- the cabinet (the kit's) ----
: hides ( -- )  shriek-sound hide-shriek !  the-cabinet hides-in-cabinet ;
: stalker-came ( -- )  the-cabinet cabinet-stalker-came ;

\ ---- what she may look at here (each from a spot of its own, its own shot) ----
: spot-at ( tri deg F: x z -- )   \ (from a spot of its own, the world still)
    {: tri deg F: x z :}  fiona wait-move  4 frames  world-frozen state!  fiona hold  fiona tri deg x z place-at ;
: framed ( F: dist pitch yaw rise -- )  1 shot  16 frames ;
: said-there ( msg -- )  say  8 frames  world-frozen state-off  shot-off ;
: wax ( -- )  $5F 180 18e -25.5e spot-at  15e 10e 0e 0e framed  wax-msg said-there ;
: chess ( -- )  $1E 90 12.5e -3e spot-at  1 1 player-camera  1e 45e 0e 5e framed  chess-msg said-there ;
: hourglass ( -- )  6 -90 -30e -33.5e spot-at  10e 5e 0e 2e framed  hourglass-msg said-there ;
: models ( -- )  $71 -90 -13e 21e spot-at  15e -5e 0e 5e framed  models-msg said-there ;
: flask ( -- )  $55 -90 -1e 3e spot-at  0 0 player-camera  5e 20e 0e 5e framed  flask-msg said-there ;
: kiln ( -- )  kiln-tri -90 -28e -20e spot-at  15e 10e 0e 2e framed  kiln-msg said-there ;
: still ( -- )  $19 180 -2e -24e spot-at  20e 20e 0e 1e framed  still-msg said-there ;
: replayed ( -- )   \ (the extras: her scene)
    fiona hold  candles-lit  daniella-kind stalker-ready  yield  1 her-back
    turns-movie scenes:keyed-movie turns-cutscene load-scene  4 cutscene-keep-model  $10 $FF movie-key
    panic-eased  if  -1 run-scene  then  0 her-back  stalker-unloaded  replay-over ;

\ ---- its hum; Daniella's at the kiln ----
: hums ( -- )
    sounds-on @ if  hum-sound -32e 0e -21e keep-sound
    else  room-sounds-in? if  hum-sound -32e 0e -21e room-sound  -1 sounds-on !  then  then ;

create cameras  1 , 0 , 0 ,   2 , 1 , 1 ,   3 , 1 , 1 ,   4 , 2 , 2 ,   \ (area, set, path)
create her-look  -27.99e 0e -19.65e  30e 8e 8e look,
create looks  19.24e 0e -32.32e  22e 18e 18e look,   -32.619e 0e -33.47e  22e 18e 18e look,   19.13e 0e -1.45e  17e 14e 14e look,
: facing? ( area deg -- flag )  {: a d :}  fiona a in-area?  fiona d 50 heading? and ;
: each-frame ( -- )
    hums  0 exit-out  cameras 4 cameras-at
    waiting @ if
        her-noticed 100 -27.99e 0e -19.65e 30e 8e notices? if  ['] tense start drop  then
        her-look 1 danger-now @ followed = hewie-looks-if
        room-sounds-in?  partner-kind daniella-kind = and if  presence-sound keep-playing or room-bank stalker sound-by  then
    then
    looks 3 hewie-looks-at
    5 90 facing? if  ['] wax unhurried offer  then
    6 45 facing? if  ['] chess unhurried offer  then
    7 -45 facing? if  ['] hourglass unhurried offer  then
    8 -45 facing? if  ['] models unhurried offer  then
    9 -45 facing? if  ['] flask unhurried offer  then
    waiting @ 0=  $A -45 facing? and if  ['] kiln unhurried offer  then
    $B 90 facing? if  ['] still unhurried offer  then
    fiona cabinet-area in-area?  fiona 0 60 heading? and if
        waiting @ if  ['] right-there unhurried offer
        else  stalker-in? if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then  then
    then
    waiting @ 0=  stalker-in? 0= or if  exit  then
    fiona stalker 20 close-to?  fiona 45 stalker body-pos fswap fdrop faces-point? and if  ['] she-turns unhurried offer  then ;
: person-in ( i -- )
    case
        fiona-p of
            events-held state? if  hewie send part-from-fiona  ['] replayed start drop  exit  then
            0 0 0 way-in
        endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- )
    waiting @ 0= if  exit  then
    0 her-back  ['] at-the-kiln stop-doing  stalker let-go
    stalkers-stay state-off  no-stalker-camera state-off  stalker-no-fear state-off  force-followed state-off ;

' came-in ' person-in ' each-frame ' went old-mansion-b1-4 place!
