\ story/places/off-map-e2.fs - the clock run's castle ($E2, castle-1f-6's long dining room again):
\ thirteen candles, each flame stirring as someone passes; the plate-key's part away; the day's
\ light in the run's second kind; two ways out; its cameras; the rooms ahead loaded as Hewie nears
\ their ways; the clock. A place (story/kit.fs, story/places/clock-run.fs). The original's
\ scripts, for checking: story/rooms-ref/off-map-e2.fs (converted from the game's, not loaded).
IN: story.places.off-map-e2
USING: engine game-state actors messages common flag-names facts room-names story.kit story.clock
    story.places.clock-run ;

$2E constant run-kind-var   13 constant #candles   1 constant plate-key-part
: candle, ( F: x z -- )  {: F: x z :}  x f,  z f, ;
create candles
    1.2e -24.813e candle,   -1.26e -24.365e candle,   4.387e -6.264e candle,   2.327e -4.824e candle,
    -3.559e -5.985e candle,   -1.887e -0.304e candle,   1.651e 3.417e candle,   2.473e 5.786e candle,
    -2.85e 13.116e candle,   -0.431e 13.794e candle,   3.347e 19.167e candle,   1.908e 24.905e candle,
    -0.597e 25.104e candle,
: candle@ ( k -- ) ( F: -- x y z )  2* floats candles +  dup f@  15.55e  float+ f@ ;
: candles-lit ( -- )  #candles 0 do  i 0  i candle@  flame-at  loop ;
: draughts ( -- )  #candles 0 do  i  i candle@  8e -4e  flame-stirred  loop ;
: came-in ( exit -- )
    drop  candles-lit  plate-key-part hide-part
    run-kind-var var@ 1 = if  $84 $60 $3C $50 $24 $2A $2A $46 room-tint  then ;

\ ---- every frame ----
create entries  0 , 0 ,  1 , 1 ,   \ (by exit 0, 1: the camera)
create cameras  2 , 0 , 0 ,  8 , 1 , 1 ,  9 , 2 , 2 ,   \ (area, set, path)
: each-frame ( -- )
    2 exits-out  cameras 3 cameras-at  3 1 1 camera-out
    2 0 his-way  3 1 his-way-back  draughts  clock-shown ;
: person-in ( i -- )  player person-of = if  entries 2 ways-in  then ;
: went ( -- )  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-e2 place!
