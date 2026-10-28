\ story/places/old-mansion-b1-5.fs - the trap workshop of the old mansion's B1 (B1-4's double): a
\ cabinet to hide in (the kit's, its doors objects 8 and 9; the stalker's grab, or his opening it);
\ the first time, Daniella waiting inside it - going to hide there, she bursts out at her; six
\ things to search "more thoroughly": the wax figures, the chess board, the hourglass each a scene
\ that opens a way (and something moves - the door unlocked again); the models, the flask, the kiln,
\ the still a trick - the door slams and locks; tried while it's locked, the trap's end (the game
\ over); Hewie wary, tense, his looks; six candles; its hum; the replays. A place (story/kit.fs).
\ The original's scripts, for checking: story/rooms-ref/old-mansion-b1-5.fs in the history
\ (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.old-mansion-b1-5
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit fiona.anims fiona.sounds scenes ;

own-reset

0 constant searched-msg  \ "I've already searched this. No need to do it again."
1 constant moved-msg     \ "Did something just move?"
2 constant wax-msg       \ "A pair of creepy wax figures. These things aren't going to come to life like in the..."
3 constant chess-msg     \ "A chess board? Here of all places? Guess they used this to take their mind off thin..."
4 constant hourglass-msg \ "A massive hourglass... Turning this over would be quite a task. Search more thoroug..."
5 constant models-msg    \ "There are models and their accompanying sketches. I haven't seen any of these bugs befo..."
6 constant flask-msg     \ "There is a flask... I wonder what kind of liquid is inside? Search more thoroughly? YES NO"
7 constant kiln-msg      \ "An industrial-sized kiln... It burns fiery hot. Search more thoroughly? YES NO"
8 constant still-msg     \ "A device for distilling things... Search more thoroughly? YES NO"
own: waiting  own: hum-on  own: noticed-0  own: noticed-1  own: hewie-away  own: burst-step
2 constant shriek-sound   3 constant hum-sound   $C constant cabinet-area   $52 constant trap-door
1 constant door-swing-anim
omb1-5-flag4 $12 0  $7B 180  $7D 180  $7B 180  $1D 153
    16.59e -25.02e  11.91e -21.5e  14.14e -24.14e  9.84e -11.27e  cabinet: the-cabinet
the-cabinet 8 cabinet-doors

\ ---- as she comes in ----
create flames  -2.8e f, 31.7e f, -5.1e f,   -5.7e f, 31.4e f, -0.3e f,   -3.3e f, 31.4e f, 4.3e f,
    2.7e f, 31.7e f, -5e f,   5.6e f, 31.5e f, -0.5e f,   2.9e f, 31.2e f, 4.4e f,
: candles-lit ( -- )  6 0 do  i 0  flames i 3 * floats +  dup f@  dup float+ f@  2 floats + f@  flame-at  loop ;
: waiting-due? ( -- flag )   \ (the first time, Daniella the stalker not yet after her)
    omb1-5-flag5 fact?  omb1-5-flag4 fact? or  partner-kind dup daniella-kind = swap kind34-kind = or 0= or  stalker-told? or if  false exit  then
    om1f-13-flag2 fact? 0=  white-mandragora-used fact? or ;
: came-in ( exit -- )
    drop  room-sounds  the-cabinet cabinet-shown
    waiting-due? dup waiting !  if  force-followed state!  then
    candles-lit  $650 fact? if  $238 give-file  then ;

\ ---- Hewie through a scene: down, hidden away; else let go, back by her after ----
: hewie-held ( -- )  hewie hold  hewie false shown  begin  yield  again ;
: hewie-for-scene ( -- )
    hewie-in? 0= dup hewie-away !  if  exit  then
    hewie-cond-now @ down-cond = if  ['] hewie-held start drop  else  hewie let-go  then ;
: hewie-down-hidden ( -- )  hewie-in?  hewie-cond-now @ down-cond = and if  ['] hewie-held start drop  then ;
: hewie-back ( -- )  ['] hewie-held running? if  ['] hewie-held stop-doing  then  hewie true shown  hewie let-go ;

\ ---- the things to search: a spot of its own, its shot; the question ----
: spot-at ( tri deg F: x z -- )
    {: tri deg F: x z :}  stalkers-stay state!  fiona hold  fiona wait-move  4 frames  world-frozen state!  fiona tri deg x z place-at ;
: asks ( msg F: dist pitch yaw rise -- yes? )  1 shot  16 frames  ask  8 frames  0= ;
: unasked ( -- )  world-frozen state-off  shot-off ;
: something-moved ( -- )   \ (back from a scene: "Did something just move?" - the door unlocked again)
    shot-off  15 fade-in  moved-msg say
    trap-door doors:door-locked? if  16 frames  unlock-sound common-bank 0e 0e -40e sound-there  trap-door unlock  then ;
: stalker-off-to-b1-2 ( -- )   \ (him about but elsewhere: off to the B1's hall)
    stalker-in? 0=  stalker-told? and  st-cond-now @ down-cond <> and if  old-mansion-b1-2 -1 stalker-to  then ;
: way-scene ( movie class cutscene -- )
    {: m cls c :}  15 screen:to-black-2 with-music or fade  m cls c load-scene  wait-fade ;
: wax ( -- )
    omb1-5-flag1 fact? if  fiona wait-move  searched-msg say exit  then
    $5F 0 18e 25.5e spot-at  wax-msg 15e 10e 0e 0e asks 0= if  unasked  stalkers-stay state-off exit  then
    1 scenes:unseen-movie 0 way-scene  world-frozen state-off  hewie-for-scene
    panic-eased  if  -1 run-scene  then  wait-fade  things-clear
    fiona 0 idle  fiona $27 -2 15.929e 21.234e place-at  camera-restart
    hewie-away @ 0= if
        hewie-cond-now @ down-cond = if  hewie-back
        else  hewie activate  old-mansion-b1-5 86 0 hewie-to  hewie $56 -2 1.98e 5.93e place-at  then
    then
    $4D $68 door-as  $68 closed-off  $4E $69 door-as  $69 closed-off  $50 $6A door-as  $6A closed-off  $53 $6B door-as  $6B closed-off
    exits-redone  hewie body-room $4D = if  $68 -1 0 hewie-to  then
    stalker-off-to-b1-2  omb1-5-flag1 fact!  something-moved  stalkers-stay state-off ;
: chess ( -- )
    omb1-5-flag2 fact? if  fiona wait-move  searched-msg say exit  then
    $1E 90 12.5e 3e spot-at  2 2 player-camera  chess-msg 1e 45e 0e 5e asks 0= if  unasked  stalkers-stay state-off exit  then
    3 scenes:unseen-movie 2 way-scene  hewie-down-hidden  world-frozen state-off
    panic-eased  if  -1 run-scene  then  wait-fade
    fiona 0 idle  fiona $55 57 10.203e -1.688e place-at  0 0 player-camera  camera-restart  hewie-back
    $58 $82 door-as  $82 closed-off  $5A $83 door-as  $83 closed-off  exits-redone
    $5F hewie-slot 0 doors-into  $5F stalker-slot 0 doors-into
    omb1-5-flag2 fact!  $2B resident-flag-set  something-moved  stalkers-stay state-off ;
: hourglass ( -- )
    omb1-5-flag3 fact? if  fiona wait-move  searched-msg say exit  then
    6 -90 -30e 33.5e spot-at  hourglass-msg 10e 5e 0e 2e asks 0= if  unasked  stalkers-stay state-off exit  then
    5 scenes:keyed-movie 4 way-scene  hewie-down-hidden  world-frozen state-off  things-clear
    $10 $78 movie-key  panic-eased  if  -1 run-scene  then  wait-fade
    fiona 0 idle  fiona 6 -92 -24.369e 32.751e place-at  camera-restart  hewie-back
    $78 unlock  $5C $88 door-as  $5C reopened  $88 closed-off  exits-redone
    stalker-off-to-b1-2  omb1-5-flag3 fact!  something-moved  stalkers-stay state-off ;
: slams ( -- )   \ (a trick: the door slams shut and locks)
    fiona reach-out-anim start-anim
    0 exit-open? if
        0 door-swing-anim story.base:door-play  fiona wait-anim  fiona look-point-move 0 0  0e 10e -40e 0e  move  yield
        fiona reach-back-anim start-anim  0 story.base:door-wait  16 frames
    else
        fiona wait-anim  fiona look-point-move 0 0  0e 10e -40e 0e  move  yield  fiona reach-back-anim start-anim
    then
    unlock-sound common-bank 0e 0e -40e sound-there  trap-door false open-mark  doors-redone  trap-door lock
    fiona wait-anim  fiona -1 idle  16 frames  fiona -1 look-at  yield ;
: a-trick ( tri deg msg F: x z dist pitch yaw rise -- )
    {: tri deg msg F: x z d p y r :}  tri deg x z spot-at  msg d p y r asks  unasked  if  slams  then  stalkers-stay state-off ;
: models ( -- )  $71 -90 models-msg -13e -21e 15e -5e 0e 5e a-trick ;
: flask ( -- )  $55 -90 flask-msg -1e -3e 5e 20e 0e 5e a-trick ;   \ (her camera the room's 0 for it)
: kiln ( -- )  7 -90 kiln-msg -28e 20e 15e 10e 0e 2e a-trick ;
: still ( -- )  $19 0 still-msg -2e 24e 20e 20e 0e 1e a-trick ;

\ ---- the locked door tried: the trap's end ----
: till-shot ( n -- )  begin  cutscene-frame cutscene-shot-at over <> while  yield  repeat  drop ;
: shadowless ( -- )  3 till-shot  0 true no-shadow  4 till-shot  0 false no-shadow ;   \ (her shadow off through two shots)
: trap-shut ( -- )
    capture-no-end state!  no-pause state!  stalkers-stay state!  fiona hold  fiona trap-door to-door
    15 screen:to-black-2 with-music or fade  7 scenes:keyed-movie 6 load-scene  wait-fade
    hewie let-go  stalker let-go  ['] shadowless start drop
    $10 $28 movie-key  panic-eased  if  -1 run-through  then
    $2C resident-flag-set  0 false no-shadow  gameover:movie-death game-over ;

\ ---- the cabinet (the kit's); Daniella waiting inside it the first time ----
: hides ( -- )  shriek-sound hide-shriek !  the-cabinet hides-in-cabinet ;
: stalker-came ( -- )  0 waiting !  force-followed state-off  the-cabinet cabinet-stalker-came ;
: bursts-out ( -- )   \ (Daniella's part: out of the cabinet at her)
    stalker-present  stalker hold  stalker true noclip  stalker $77 0 15.58e -33.18e place-at
    stalker $C motions-for  stalker true shown  stalker false hush  0 hide-part  stalker use-motions
    omb1-5-flag4 fact!  0 barred-floor floor-on  1 burst-step !
    8 object-show  9 object-show  8 1 6 object  9 1 6 object
    stalker room-motion 3 + start-anim  7 frames  0 room-bank stalker sound-by  stalker wait-anim
    begin  burst-step @ 2 < while  yield  repeat  stalker false noclip ;
: she-was-inside ( -- )
    stalkers-stay state!  force-followed state!  fiona hold  fiona $D motions-for  fiona wait-move
    fiona $7B 180 16.59e -25.02e walk-to
    partner activate  old-mansion-b1-5 7 stalker-to  stalker-present  stalker 0 0 camera-set  his-own-time
    stalker false shown  stalker true hush  fiona use-motions  0 burst-step !  ['] bursts-out start drop
    begin  burst-step @ 1 < while  yield  repeat
    seized-sound common-bank fiona sound-by  fiona room-motion 10 play-blend  fiona room-motion 3 + 10 play-blend
    2 burst-step !  stalkers-stay state-off  force-followed state-off  0 waiting !  omb1-5-flag5 fact! ;

\ ---- the replays (the extras) ----
: replay ( movie class cutscene -- )
    {: m cls c :}  fiona hold  candles-lit  m cls c load-scene  panic-eased  if  -1 run-scene  then  replay-over ;
: replay-wax ( -- )  1 scenes:unseen-movie 0 replay ;
: replay-chess ( -- )  3 scenes:unseen-movie 2 replay ;
: replay-hourglass ( -- )
    fiona hold  candles-lit  5 scenes:keyed-movie 4 load-scene  $10 $78 movie-key  panic-eased  if  -1 run-scene  then  replay-over ;
: replay-trap ( -- )
    fiona hold  7 scenes:keyed-movie 6 load-scene  ['] shadowless start drop  $10 $28 movie-key  panic-eased  if  -1 run-scene  then  replay-over ;

\ ---- Hewie, its hum ----
: wary ( -- )  17.96e 0e -27.69e hewie-stands-by ;
: tense ( -- )  1.58e 0e -9.28e hewie-barks-tense ;
create wary-look  17.96e 0e -27.69e  15e 18e 18e look,
create looks  17.16e 0e 30.67e  22e 18e 18e look,   -33.99e 0e 32.91e  22e 18e 18e look,   15.24e 0e -0.28e  17e 14e 14e look,
: hums ( -- )
    hum-on @ if  hum-sound -32e 0e 21e keep-sound
    else  room-sounds-in? if  hum-sound -32e 0e 21e room-sound  -1 hum-on !  then  then ;

create cameras  1 , 0 , 0 ,   2 , 2 , 2 ,   3 , 1 , 1 ,   4 , 2 , 2 ,   \ (area, set, path)
: facing? ( area deg -- flag )  {: a d :}  fiona a in-area?  fiona d 50 heading? and ;
: each-frame ( -- )
    hums  0 exit-out  cameras 4 cameras-at
    omb1-5-flag5 fact? 0=  omb1-5-flag4 fact? 0= and if
        noticed-0 100 17.96e 0e -27.69e 15e 18e notices? if  ['] wary start drop  then
        wary-look 1 hewie-looks-unchased
    then
    noticed-1 100 1.58e 0e -9.28e 16e 8e notices? if  ['] tense start drop  then
    looks 3 hewie-looks-at
    5 0 facing? if  ['] wax unhurried offer  then
    6 45 facing? if  ['] chess unhurried offer  then
    7 -45 facing? if  ['] hourglass unhurried offer  then
    8 -45 facing? if  ['] models unhurried offer  then
    9 -45 facing? if  ['] flask unhurried offer  then
    $A -45 facing? if  ['] kiln unhurried offer  then
    $B 0 facing? if  ['] still unhurried offer  then
    8 locked-request? if  ['] trap-shut any-time offer  then
    fiona cabinet-area in-area?  fiona 90 60 heading? and if
        stalker-in? if  no-time-message offer-remark-now
        else  waiting @  trap-door doors:door-locked? 0= and if  ['] she-was-inside  else  ['] hides  then  calm-only offer  then
    then ;
: person-in ( i -- )
    case
        fiona-p of
            events-held state? if
                hewie send part-from-fiona
                replay-act case  $14 of  ['] replay-wax  endof  $15 of  ['] replay-chess  endof  $16 of  ['] replay-hourglass  endof
                    >r ['] replay-trap r>  endcase  start drop  exit
            then
            0 0 0 way-in
        endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- )  force-followed state-off ;

' came-in ' person-in ' each-frame ' went old-mansion-b1-5 place!
