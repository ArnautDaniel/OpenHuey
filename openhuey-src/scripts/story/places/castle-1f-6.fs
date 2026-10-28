\ story/places/castle-1f-6.fs - the long dining room in Belli Castle's 1F ($11): thirteen candles on
\ the table, each flame stirring as someone passes; the second cracked plate-key on it; the
\ Medallion Hewie sniffs out; the tapestry; the door to unlock from this side; at night Daniella
\ about beyond (exit 1); brought in by the forced way: Daniella's scene. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/castle-1f-6.fs in the history (converted from
\ the game's; gone with the old machinery in 0.2.20).
IN: story.places.castle-1f-6
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects story.kit fiona.anims scenes ;

own-reset

\ ---- what is said here ----
2 constant daniella-msg       \ "My creator said he made me the perfect woman... But I cannot taste or experienc..."
6 constant candles-msg        \ "Why are there so many candlesticks on this table?"
7 constant candles-night-msg  \ "There are just so many candlesticks on this table... I don't think I'm imagining th..."
8 constant tapestry-msg       \ "A large tapestry is suspended here. I wonder if this is an embroidery of one of the cas..."

\ ---- the room's things ----
13 constant #candles   $E constant medallion-fx   0 constant daniella-part   1 constant plate-key-part
2 constant to-exit-0-area   3 constant from-exit-1-area   5 constant table-area   6 constant table-2-area
7 constant tapestry-area   0 constant locked-exit   3 constant figure-slot   $1D constant scene-kind
1 constant daniella-movie   0 constant daniella-cutscene   $E8 constant brought-tri
: night? ( -- flag )  c2f-3-flag1 fact? ;
: daniella-about? ( -- flag )  hewie-found-in-wire fact?  found-file-208 fact? 0= and ;   \ (Daniella beyond exit 1)

\ ---- the candles: their flames (effects 0..12), each stirred as someone steps into its draught ----
: candle, ( F: x z -- )  {: F: x z :}  x f,  z f, ;
create candles
    1.2e -24.813e candle,   -1.26e -24.365e candle,   4.387e -6.264e candle,   2.327e -4.824e candle,
    -3.559e -5.985e candle,   -1.887e -0.304e candle,   1.651e 3.417e candle,   2.473e 5.786e candle,
    -2.85e 13.116e candle,   -0.431e 13.794e candle,   3.347e 19.167e candle,   1.908e 24.905e candle,
    -0.597e 25.104e candle,
: candle@ ( k -- ) ( F: -- x y z )  2* floats candles +  dup f@  15.55e  float+ f@ ;
: candles-lit ( -- )  #candles 0 do  i 0  i candle@  flame-at  loop ;
: draughts ( -- )  #candles 0 do  i  i candle@  8e -4e  flame-stirred  loop ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  candles-lit
    got-cracked-plate-key-2 fact? if  plate-key-part hide-part  else  plate-key-part show-part  then
    c1f-6-medallion-found fact?  c1f-6-medallion fact? 0= and if  medallion-fx 34.2e 1e 60.5e flicker-at  then
    night? 0= if  $84 $60 $3C $50 $24 $2A $2A $46 room-tint  then ;

\ ---- brought in by the forced way: Daniella's scene, then out to the room loaded ahead ----
: daniella-scene ( -- )   \ (her figure in, the movie keyed; her figure out)
    scene-kind figure-slot figure-in  yield
    daniella-movie scenes:keyed-movie daniella-cutscene load-scene  $32 $FF movie-key  daniella-part show-part
    panic-eased  if  daniella-msg run-scene  then
    figure-slot figure-out ;
: meets-daniella ( -- )
    fiona hold  text-gone  locked-exit exit-door false doors-actor send door-opened  doors-redone  world-held state!
    0 prepare-exit  daniella-scene  wait-fade  world-held state!
    fiona brought-tri 0 place-on-tri  0 0 player-camera  camera-restart  c1f-6-flag1 fact!
    daniella-part hide-part  forced-exit take-exit ;

\ ---- what she may do or say here ----
: candlesticks ( -- )  fiona wait-move  night? if  candles-night-msg  else  candles-msg  then  say ;
: tapestry ( -- )
    fiona wait-move  fiona 180 turn  fiona turn-round-anim play  fiona -1 idle  7 frames
    fiona look-at-anim play  tapestry-msg say ;
: reach-plate-key ( -- )   \ (on the table: reached, taken)
    fiona wait-move  fiona 4e -20e turn-to-xz  fiona reach-out-anim play  4 frames
    got-cracked-plate-key-2 fact!  plate-key-part hide-part
    cracked-plate-key-2 item-named  cracked-plate-key-2 1 give-item  cracked-plate-key-2 show-item-tab  fanfare
    obtained-2-msg say  fiona reach-back-anim play ;
: pick-medallion ( -- )  medallion c1f-6-medallion medallion-fx 34.2e 60.5e picks-up ;
: unlocks-it ( -- )  locked-exit unlocks ;
: spots ( -- )
    got-cracked-plate-key-2 fact? 0=  fiona 4e 8.5e -20e 5e 5e touches? and if  ['] reach-plate-key calm-only offer  then
    c1f-6-medallion-found fact?  c1f-6-medallion fact? 0= and  fiona 34.2e 1e 60.5e 5e 5e touches? and if
        ['] pick-medallion calm-only offer  then
    fiona table-area in-area?  fiona -90 60 heading? and if  ['] candlesticks unhurried offer  then
    fiona table-2-area in-area?  fiona 90 60 heading? and if  ['] candlesticks unhurried offer  then
    fiona tapestry-area in-area?  fiona 180 50 heading? and if  ['] tapestry unhurried offer  then
    locked-exit before-locked? if  ['] unlocks-it any-time offer  then ;

\ ---- Hewie: the medallion he sniffs out (scrambling about by it) ----
: medallion-sniffed ( -- )  c1f-6-medallion-found c1f-6-medallion medallion-fx  34.2e 1e 60.5e  hewie-finds ;
: hewie-sniffs ( -- )
    c1f-6-medallion-found fact?  scrambling? 0= or if  exit  then
    hewie 34.2e 0e 60.5e 10e 5e touches? 0= if  exit  then
    ['] medallion-sniffed start-only ;

\ ---- the movie replayed (the extras): Daniella in as the stalker for it ----
: replayed ( -- )
    fiona hold  candles-lit  daniella-kind stalker-ready  yield  daniella-scene  daniella-part hide-part
    stalker-unloaded  replay-over ;

\ ---- every frame ----
create cameras  2 , 0 , 0 ,   8 , 1 , 1 ,   9 , 2 , 2 ,   \ (area, set, path)
create entries  0 , 0 ,   1 , 1 ,             \ (by exit 0, 1: the camera)
: daniella-ahead ( -- )   \ (at night, Daniella about beyond exit 1: loaded as the way is open, gone heading the other way)
    daniella-about? 0= if  exit  then
    1 exit-open?  figure-slot figure 0< and if  daniella-kind figure-slot figure-in  then
    fiona to-exit-0-area entered? if  figure-slot figure-out  then
    fiona from-exit-1-area left?  figure-slot figure 0< and if  daniella-kind figure-slot figure-in  then ;
: ways ( -- )
    daniella-ahead  2 exits-out  cameras 3 cameras-at  3 1 1 camera-out
    fiona to-exit-0-area entered? if  0 prepare-exit  then
    fiona from-exit-1-area left? if  1 prepare-exit  then ;
: each-frame ( -- )  events-held state? if  draughts exit  then  ways  hewie-sniffs  draughts  spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    entries 2 ways-in
    came-by @ forced-exit = if  fiona brought-tri 0 place-on-tri  0 0 player-camera  ['] meets-daniella start drop  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-1f-6 place!
