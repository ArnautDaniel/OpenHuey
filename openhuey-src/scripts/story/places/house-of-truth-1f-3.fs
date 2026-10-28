\ story/places/house-of-truth-1f-3.fs - the House of Truth's 1F hall of candles ($8A): four flames,
\ stirred as anyone passes; the Golden Candlestick (its file, but in the other game); the barred
\ corridor; the Rubrum Medallion; the Viola Medallion Hewie sniffs out, his look at the candlestick;
\ the first time she goes on from the hall, the scene (Lorenzo let loose after her); the replay. A
\ place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/house-of-truth-1f-3.fs
\ in the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.house-of-truth-1f-3
USING: engine game-state actors messages common facts room-names flag-names story-names items.names
    story.kit fiona.anims scenes ;

own-reset

0 constant bars-msg        \ "This is connected to another corridor but... these foreboding bars seem to keep out all..."
own: hewie-sent-off        \ Hewie was here as the scene began: back after it
0 constant candlestick-part   4 constant rubrum-fx   5 constant viola-fx   4 constant #flames
$A constant candlestick-area   $B constant bars-area   9 constant hall-area
1 constant hall-movie   0 constant hall-cutscene   $45 constant hall-seen   $B constant scene-figure
$24E constant candlestick-file   $C constant candlestick-sound
create flames  -42.5e f, 45.4e f, -36.9e f,   -72.4e f, 17.2e f, -7.1e f,   -42.5e f, 45.4e f, 87.3e f,   -72.4e f, 17.2e f, 117.2e f,
: flame@ ( k -- ) ( F: -- x y z )  3 * floats flames +  dup f@  dup float+ f@  2 floats + f@ ;
: lit ( -- )  #flames 0 do  i 1  i flame@  flame-at  loop ;
: came-in ( exit -- )
    drop  lit
    got-golden-candlestick fact? 0= if  candlestick-part show-part  then
    ht1f-3-rubrum-medallion fact? 0= if  rubrum-fx -23.41e 1e -14.78e flicker-at  then
    ht1f-3-viola-medallion-found fact?  ht1f-3-viola-medallion fact? 0= and if  viola-fx 3e 1e 168e flicker-at  then
    1 $2300 reverb ;

\ ---- the first time she goes on from the hall: the scene ----
: hall-scene ( -- )  hall-movie scenes:unseen-movie hall-cutscene load-scene drop  panic-eased  -1 run-scene ;
: in-the-hall ( -- )
    stalkers-stay state!  fiona hold  stalker let-go  0 summon-take
    15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade
    hewie-in? dup hewie-sent-off !  if  hewie let-go  hewie false shown  then
    hall-scene
    fiona $BC 0 -12.31e 49.96e place-at
    hewie-sent-off @ if  hewie true shown  hewie activate  hewie $1E4 0 -15.99e 63.61e place-at  0 hewie-sent-off !  then
    partner activate  this-room @ 480 stalker-to  stalker $1E0 0 -11.74e 29.25e place-at  stalker 0 0 camera-set
    summoner-on state!  his-own-time  0 0 player-camera  camera-restart
    15 fade-in  hall-seen resident-flag-set  stalkers-stay state-off  ht1f-3-flag1 fact! ;
: replayed ( -- )
    fiona hold  lit  scene-figure 3 figure-in  yield  3 figure-on  hall-scene  3 figure-out  replay-over ;

\ ---- what she may do or say here ----
: takes-candlestick ( -- )   \ (in the other game neither its file nor its chime)
    fiona wait-move  fiona -62.5e -1e turn-to-xz  fiona reach-out-anim play  candlestick-part hide-part
    items-40-as-70 fact? 0= if  candlestick-file give-file  then
    golden-candlestick got-golden-candlestick -1 takes-the  fiona reach-back-anim play
    items-40-as-70 fact? 0= if  candlestick-sound fanfare-bank bank-sound  then ;
: the-bars ( -- )
    fiona wait-move  fiona $4F 90 -33e -24e place-at  1 42e 10e -160e 0e shot  bars-msg say  16 frames  shot-off ;
: pick-rubrum ( -- )  rubrum-medallion ht1f-3-rubrum-medallion rubrum-fx -23.41e -14.78e picks-up ;
: pick-viola ( -- )  viola-medallion ht1f-3-viola-medallion viola-fx 3e 168e picks-up ;
: viola-sniffed ( -- )  ht1f-3-viola-medallion-found ht1f-3-viola-medallion viola-fx  3e 1e 168e  hewie-finds ;
create candlestick-look  -61.25e 0e -4.27e  17e 15e 15e look,
: spots ( -- )
    got-golden-candlestick fact? 0=  fiona candlestick-area in-area? and  fiona 0 50 heading? and if  ['] takes-candlestick calm-only offer  then
    fiona bars-area in-area?  fiona 45 50 heading? and if  ['] the-bars unhurried offer  then
    ht1f-3-rubrum-medallion fact? 0=  fiona -23.41e 1e -14.78e 5e 5e touches? and if  ['] pick-rubrum calm-only offer  then
    ht1f-3-viola-medallion-found fact?  ht1f-3-viola-medallion fact? 0= and  fiona 3e 1e 168e 5e 5e touches? and if  ['] pick-viola calm-only offer  then ;

\ ---- every frame ----
create entries  0 , 0 ,  0 , 0 ,  1 , 1 ,  2 , 2 ,   \ (by exit 0 .. 3: the camera)
create cameras  7 , 0 , 0 ,  8 , 1 , 1 ,   \ (area, set, path)
create ahead  4 , 0 ,  5 , 1 ,  6 , 2 ,   \ (area, exit loaded ahead)
: each-frame ( -- )
    0 exit-out  1 1 leaves-by  2 2 leaves-by  3 3 leaves-by  cameras 2 cameras-at  ahead 3 ways-out
    ht1f-3-flag1 fact? 0=  fiona hall-area left? and if
        ht1f-3-flag1 fact!  panic-eased  ['] in-the-hall fiona start-holding drop
    then
    #flames 0 do  i  i flame@  8e -4e  flame-stirred  loop
    got-golden-candlestick fact? 0= if  candlestick-look 1 hewie-looks-at  then
    ht1f-3-viola-medallion-found fact? 0=  scrambling? and  hewie 3e 0e 168e 10e 5e touches? and if  ['] viola-sniffed start-only  then
    spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    entries 4 ways-in ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went house-of-truth-1f-3 place!
