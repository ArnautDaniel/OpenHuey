\ story/places/house-of-truth-1f-7.fs - the House of Truth's 1F hall of the trap ($8F): file 255
\ found, Hewie barking at the wall - her look, his scene (the wall opened); the Cane of Caduceus
\ behind it; her track on, the first time she steps in - the scene: the trap (she struggles against
\ it, shaking free at 100 - or Lorenzo reaches her first: the game over); the hiding place (the
\ kit's) and the stalker's search; the door unlocked from this side; the Antimony Tube; a creature
\ let out; the tremors, the stones, the drone; five flames; the walls' shade; the replays. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/house-of-truth-1f-7.fs in
\ the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.house-of-truth-1f-7
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit story.places.house-of-truth fiona.anims fiona.sounds stalker.anims
    gameover scenes ;

own-reset  truth-owns owns !

0 constant barking-msg     \ "Hewie's barking... Is he trying to tell me something about this wall?"
1 constant cane-msg        \ "This cane... I don't know how or why, but for some reason I feel like I've seen it some..."
own: hewie-at-wall         \ Hewie barking at the wall (his doing)
own: pushed                \ she has shaken free of the trap
own: searched              \ the stalker has searched her hiding place (as he found her hidden)
own: trapped               \ the trap scene's struggle under way
own: drone-on   own: cool-down   own: pushing   own: last-shakes
0 constant cane-part   5 constant tube-fx   $12 constant wall-area   $14 constant hiding-area   $11 constant trap-area
$13 constant creature-area   $10 constant wall-hewie-area   $17 constant near-trap-area
0 constant trap-start-sound   1 constant trap-close-sound   $30 constant drone-sound   $47 constant drone-bank
1 constant wall-movie   0 constant wall-cutscene   7 constant trap-movie   6 constant trap-cutscene
$25 constant searches-var   5 constant trap-motions   8 constant hide-motions   $E04 constant reach-anim
create flames  -59.1e f, 18.5e f, -16e f,   -10.4e f, 17.5e f, -26.3e f,   84.1e f, 19.8e f, -2.3e f,
    -57.4e f, 20e f, 24.4e f,   115.9e f, 19.5e f, 33.2e f,
: flame@ ( k -- ) ( F: -- x y z )  3 * floats flames +  dup f@  dup float+ f@  2 floats + f@ ;
: lit ( -- )  5 0 do  i 1  i flame@  flame-at  loop ;
: stones-fall ( -- )   \ (the ceiling coming down: stones of three kinds falling from 50)
    0 50e 0e 0e 1e story.base:stones-falling  1 50e 0e 0e 1e story.base:stones-falling  6 50e 0e 0e 1e story.base:stones-falling ;
: wall ( opened? -- )  if  2 object-hide  3 object-hide  else  2 object-show  3 object-show  0 solid-floor floor-on  then ;
: drone ( -- )  drone-sound story-volume or drone-bank bank-sound  -1 drone-on ! ;
: came-in ( exit -- )
    drop  room-sounds
    got-cane-of-caduceus fact? if  cane-part hide-part  else  cane-part show-part  then
    ht1f-7-flag1 fact? wall  4 object-show
    over-music-f fact? if  1 solid-floor floor-on  1 show-part  2 hide-part  else  1 hide-part  2 show-part  then
    lit
    tremor-warned fact? if  5 1e background  then
    tremors  ht1f-9-flag2 fact? if  stones-fall  then
    ht1f-7-antimony-tube fact? 0= if  tube-fx 13.05e 6.54e 108.47e flicker-at  then
    1 $2300 reverb ;

\ ---- Hewie at the wall; his scene (her face for the scene in two of her costumes) ----
: barks-at-wall ( -- )   \ (his doing: turned to it, barking, till she is chased)
    hewie hold  -1 hewie-at-wall !  hewie wait-move  hewie -89e 75e turn-to-xz
    begin  hewie-bark  hewie wait-move  chased? until  0 hewie-at-wall ! ;
: her-face ( -- )   \ (costume 3's, 2's)
    costume-var var@ case  3 of  0 0 s" O_FIO\FIO_201.TEX" face-on  endof  2 of  0 0 s" O_FIH\FIH_201.TEX" face-on  endof  endcase ;
: face-cue ( -- )  begin  cutscene-frame cutscene-shot-at 2 <> while  yield  repeat  her-face ;
: wall-scene ( -- )
    wall-movie scenes:keyed-movie wall-cutscene load-scene drop  ['] face-cue start drop
    $14 $64 movie-key  panic-eased  -1 run-scene  ['] face-cue stop-doing ;
: wall-opens ( -- )
    stalkers-stay state!  fiona wait-move  fiona -89e 75e turn-to-xz  fiona look-close-anim start-anim  16 frames
    barking-msg say  fiona wait-anim  15 screen:to-black-2 with-music or fade  wait-fade
    ['] barks-at-wall stop-doing  hewie let-go  wall-scene
    true wall  0 solid-floor floor-off  ht1f-7-flag1 fact!
    fiona $B3 -91 -74.86e 74.73e place-at  hewie activate  hewie $B5 -80 -51.58e 76.38e place-at  50 trust+
    1 1 player-camera  camera-restart  15 fade-in  $4C resident-flag-set  stalkers-stay state-off ;
: the-cane ( -- )
    fiona wait-move  fiona -89e 75e turn-to-xz  fiona look-close-anim start-anim  40 frames  cane-msg say  fiona wait-anim
    fiona reach-out-anim play  4 frames  cane-part hide-part
    cane-of-caduceus got-cane-of-caduceus -1 takes-the  fiona reach-back-anim play ;

\ ---- the trap: she struggles free - shaking, 100 - or Lorenzo reaches her ----
: grunts ( -- )   \ (her struggle this frame: new shakes counted, a grunt when the cool-down is out)
    cool-down @ if  -1 cool-down +!  then
    shakes-now @ last-shakes @ <> if
        cool-down @ 0= if  2 random if  kick-sound 45  else  push-sound 60  then  cool-down !  common-bank fiona sound-by  then
        shakes-now @ last-shakes !
    then
    shakes-now @ 100 >= if  -1 pushed !  then ;
: trap-loaded ( -- )  trap-movie scenes:unseen-movie trap-cutscene load-scene drop ;
: trap-scene ( -- )  trap-loaded  panic-eased  -1 run-scene ;
: struggles ( -- )
    stalkers-stay state!  fiona hold  stalkers-blind state!
    stalker-in? if  stalker hold  then  hewie-in? if  hewie hold  then
    ['] slight-tremors stop-doing  ['] violent-tremors stop-doing
    15 screen:to-black-2 fade  trap-loaded  wait-fade
    stalker false shown  hewie false shown  panic-eased  -1 run-scene
    $50 resident-flag-set  4 object-show  1 solid-floor floor-on  ht1f-7-flag2 fact!
    fiona trap-motions motions-for  fiona use-motions  fiona $192 180 -37.91e 65.69e place-at
    7 -1 player-camera  camera-restart  stalkers-stay state-off
    0 shakes-now !  0 last-shakes !  0 cool-down !  true fiona send count-shakes
    fiona 0 room-motion + start-anim  4 1 0 object  trap-start-sound -37e 0e 63e room-sound
    stalker let-go  hewie let-go  stalker true shown  hewie true shown
    stalker 1 on-group? if  stalker $194 put-on-tri  then  hewie 1 on-group? if  hewie $18B put-on-tri  then
    15 screen:from-black fade  tremors  drone  -1 trapped !  0 pushed !
    begin
        fiona went-round? if  fiona 0 room-motion + start-anim  4 1 0 object  then
        grunts  yield  pushed @
    until
    false fiona send count-shakes  fiona wait-anim  push-sound common-bank fiona sound-by
    fiona 1 room-motion + start-anim  4 1 1 object  fiona wait-anim
    stalker let-go  0 trapped !  stalkers-blind state-off ;
: caught-in-it ( -- )
    capture-no-end state!  fiona wait-move  caught-sound common-bank fiona sound-by
    fiona held-anim play  30 frames  faded-out game-over ;
: reaches-her ( -- )   \ (Lorenzo: to her, his reach - she is caught - the trap closing)
    stalker hold  stalker wait-move  trapped @ 0= if  exit  then
    stalker $18E -102 -25.136e 68.404e walk-to  trapped @ 0= if  exit  then
    stalker reach-anim start-anim  20 frames  panic-eased  ['] caught-in-it fiona start-holding drop
    4 1 2 object  27 frames  trap-close-sound -37e 0e 63e room-sound  begin  yield  again ;

\ ---- the hiding place (the kit's); his search as he finds her hidden ----
: found-there ( -- )
    stalker hold  0 door-for-him  1 door-for-him  $FE 180 45.21e 54.9e finds-her-at ;
: searched-hidden ( -- )   \ (by the odds - his searches counted - or seen, or a creature giving her away)
    -1 searched !  -1 came-while-hidden !
    searches-var mansion-odds chance?  stalker unseen? 0= or  2 creature-moving? or if  ['] found-there start drop
    else  120 summoner-sooner  then
    0 stalker-alert? if  2 stalker-order  then
    searches-var var@ 1+ searches-var var! ;
: hides ( -- )  0 searched !  -1 hide-unseen !  hide-motions $FE 180 6 50.28e 39.2e hides-in ;
: his-in-view? ( -- flag )   \ (the original's cond00: in play, about his own way off screen - or not on it)
    stalker-told? 0= if  false exit  then  st-mode-now @ 3 =  stalker unseen? or ;

\ ---- the door unlocked from this side (then the others locked) ----
: unlocks-door ( -- )  0 unlocks  $E6 lock  $E9 lock ;
: pick-tube ( -- )
    fiona wait-move  fiona 13.05e 108.47e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  antimony-tube ht1f-7-antimony-tube tube-fx takes-up  fiona reach-back-anim play ;
: spots ( -- )
    ht1f-7-flag3 fact?  fiona hiding-area in-area? and  fiona 180 60 heading? and if
        stalker-in?  his-in-view? and if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then
    then
    8 locked-request?  fiona 0 before-door? and if  ['] unlocks-door any-time offer  then
    fiona wall-area in-area?  fiona -90 50 heading? and if
        ht1f-7-flag1 fact? if  got-cane-of-caduceus fact? 0= if  ['] the-cane unhurried offer  then
        else  hewie-at-wall @ if  ['] wall-opens unhurried offer  then  then
    then
    ht1f-7-antimony-tube fact? 0=  fiona 13.05e 6.54e 108.47e 5e 5e touches? and if  ['] pick-tube calm-only offer  then ;

\ ---- the movies replayed (the extras) ----
: replay-wall ( -- )  fiona hold  cane-part show-part  2 show-part  lit  wall-scene  replay-over ;
: replay-trap ( -- )  fiona hold  true wall  1 show-part  lit  stones-fall  trap-scene  replay-over ;

\ ---- every frame ----
create cameras  4 , 0 , 0 ,  5 , 1 , 1 ,  6 , 1 , 1 ,  7 , 2 , 2 ,  8 , 2 , 2 ,  9 , 3 , 3 ,  $A , 3 , 3 ,  $B , 4 , 4 ,
    $C , 4 , 4 ,  $D , 5 , 5 ,  $E , 5 , 5 ,  $F , 0 , 0 ,   \ (area, set, path)
create near-cameras  $15 , 2 , 2 ,  $16 , 2 , 2 ,
create ahead  2 , 0 ,  3 , 1 ,
: shade ( -- )
    -72.3e 18e 56e  -85e 18e 56.4e  -72.3e -2e 51.4e  -85e -2e 51.4e  light-doorway
    72e 16e -0.1e  91.2e 16e -0.1e  72e 0e -2e  91.2e 0e -2e  light-doorway ;
: hewie-at-it ( -- )   \ (file 255 found, the wall not yet opened: Hewie by it barks - down, he is helped up)
    found-file-255 fact?  ht1f-7-flag1 fact? 0= and  hewie wall-hewie-area in-area? and  hewie-at-wall @ 0= and  chased? 0= and if
        hewie-cond-now @ down-cond = if  1 hewie send heal  else  ['] barks-at-wall start drop  then
    then ;
: each-frame ( -- )
    drone-on @ if  drone-sound story-volume or keep-playing or drone-bank bank-sound  then
    2 exits-out  cam-set@ 7 = if  near-cameras 2 cameras-at  then  cameras 12 cameras-at  ahead 2 ways-out
    hewie-at-it
    over-music-f fact?  ht1f-7-flag2 fact? 0= and  fiona trap-area entered? and if
        ht1f-7-flag2 fact!  panic-eased  ['] struggles fiona start-holding drop
    then
    ht1f-9-flag2 fact? 0=  ht1f-7-flag3 fact? 0= and  fiona creature-area entered? and if
        ht1f-7-flag3 fact!  this-room @ 0 235 $88 0 creature-in
    then
    st-cond-now @ down-cond <>  fiona-hidden state? and  searched @ 0= and  stalker-in? and  stalker held? 0= and if  searched-hidden  then
    trapped @  stalker near-trap-area in-area? and  ['] reaches-her running? 0= and if  ['] reaches-her start drop  then
    slam-shake  shade
    ht1f-9-flag2 fact?  drone-on @ 0= and if  drone  then
    spots ;
: person-in ( i -- )
    dup stalker-p = if  drop  fiona-hidden state? if  searched-hidden  else  0 came-while-hidden !  then  exit  then
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  replay-act $F = if  ['] replay-wall  else  ['] replay-trap  then  start drop exit  then
    0 0 0 way-in  1 4 4 way-in ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went house-of-truth-1f-7 place!
