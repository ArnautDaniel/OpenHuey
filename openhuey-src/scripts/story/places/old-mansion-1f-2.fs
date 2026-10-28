\ story/places/old-mansion-1f-2.fs - the hall of the scales in the old mansion's 1F: the statue's
\ scales (a coin on one pan); an ore laid on them - its scene, the ore gone, a Handful of Ash; a
\ Godstone laid on them - its scene: the scales level, the great door unlocked (the Godstone may be
\ taken back while she's still here); the great door (its inscription and the file; pushed, it won't
\ give); two save clocks; the Camomile; eight candles; Hewie's look; its sounds; the replays. A
\ place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-1f-2.fs in
\ the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.old-mansion-1f-2
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit fiona.anims scenes ;

own-reset

0 constant scales-msg     \ "A pair of scales hangs mysteriously from the statue's hand. One of the dishes i..."
1 constant coin-msg       \ "There is a coin of sorts resting on one of the trays."
2 constant take-back-msg  \ "I may not be able to use this anymore, but who knows... Take the Godstone? YES NO"
3 constant door-msg       \ "A large door stands unnervingly still. Letters have been etched into the door."
4 constant inscription-msg \ "Pursuer of truths, entrust the accumulation of knowledge to the judgment of the god..."
5 constant wont-open-msg  \ "Damn, it just won't open. This could be my way out. Open, damn you!"
own: scales-remarked  own: inscription-read  own: placed  own: laid  own: hewie-away  own: sounds-on
own: ambient-at  own: ambient-wait
0 constant camomile-fx   1 constant godstone-fx   0 constant hum-sound   5 constant first-ambient   $A constant first-ambient-2
1 constant godstone-movie   0 constant godstone-cutscene   3 constant ore-movie   2 constant ore-cutscene
$5D constant great-door   $246 constant inscription-file   $38 constant ore-seen   $1C constant scene-figure
0 constant godstone-laid   1 constant ore-laid   \ (the items' events here: items.fs)

\ ---- the scales: the stones on them (the room's objects 4..8 - by the item), level or tipped ----
: stone-object ( item -- obj )   \ (isi_ichi, ishi_iou, ishi_gin for the ores 8..$A; ishi_shiro, ishi_aka the Godstones)
    dup white-godstone = if  drop 7 exit  then  dup red-godstone = if  drop 8 exit  then  4 - ;
: stones-hidden ( -- )  9 4 do  i object-hide  loop ;
: chains-shown ( -- )  $F $B do  i object-show  loop ;   \ (kusari, sara_l, sara_r, tenbin)
: pans-level ( -- )   \ (each pan and the beam straight; the beam turned 25 degrees when tipped)
    s" sara_l" turn-x 0e object!  s" sara_l" turn-y 0e object!  s" sara_l" turn-z 0e object!
    s" sara_r" turn-x 0e object!  s" sara_r" turn-y 0e object!  s" sara_r" turn-z 0e object!
    s" tenbin" turn-x 0e object!  s" tenbin" turn-y 0e object!  s" tenbin" 4.6193e 20.6e 17.082e object-to ;
: scales ( tipped? -- )
    pans-level
    if    s" sara_l" 6.3504e 20.6e 17.1e object-to  s" sara_r" 3.594e 19.25e 17.1e object-to  s" tenbin" turn-z 25e deg>rad object!
    else  s" sara_l" 6.2004e 19.8e 17.1e object-to  s" sara_r" 3.049e 19.8e 17.1e object-to  s" tenbin" turn-z 0e object!  then ;
create flames  -19.7e f, 22.5e f,  -19.7e f, 25e f,  -17.2e f, 22.4e f,  -17.2e f, 25e f,
    17.2e f, 22.4e f,  17.2e f, 25e f,  19.7e f, 22.5e f,  19.7e f, 25e f,
: candles-lit ( -- )  8 0 do  i 2 +  0  flames i 2* floats +  dup f@  18.3e  float+ f@  flame-at  loop ;
: came-in ( exit -- )
    drop  room-sounds  0 ambient-at !  0 ambient-wait !  true scales  stones-hidden
    medallion-40-done fact? if  0 show-part  9 object-hide  $A object-hide  else  0 barred-floor floor-on  then
    om1f-2-camomile fact? 0= if  camomile-fx -72.34e 59e -34.25e flicker-at  then
    candles-lit  1 $2300 reverb  scene-figure 3 figure-in ;

\ ---- Hewie through a scene: down, hidden away till it's over; else let go ----
: hewie-held ( -- )  hewie hold  hewie false shown  begin  yield  again ;
: hewie-for-scene ( -- )   \ (here: down, held hidden; else let go - not here, away)
    hewie-in? 0= dup hewie-away !  if  exit  then
    hewie-cond-now @ down-cond = if  ['] hewie-held start drop  else  hewie let-go  then ;
: hewie-back ( -- )  ['] hewie-held stop-doing  hewie true shown  hewie let-go ;

\ ---- a Godstone laid on the scales: level, the great door unlocked ----
: godstone-shown ( -- )   \ (through its scene: the stone on the pan)
    begin  stones-hidden  laid @ stone-object object-show  yield  again ;
: godstone-scene ( -- )
    godstone-movie scenes:unseen-movie godstone-cutscene load-scene  ['] godstone-shown start drop
    panic-eased  if  -1 run-scene  then ;
: lays-godstone ( -- )
    stalkers-stay state!  fiona hold  stalker let-go  summoner-on state-off
    15 screen:to-black-2 with-music or fade  wait-fade  hewie-for-scene
    godstone-scene  wait-fade
    fiona 0 idle  fiona $158 83 3e 24e place-at  camera-restart
    hewie-away @ 0= if
        hewie-cond-now @ down-cond = if  hewie-back
        else  hewie activate  old-mansion-1f-2 229 1 hewie-to  hewie $E5 -129 26.55e 54.18e place-at  then
    then
    ['] godstone-shown stop-doing  stones-hidden  laid @ stone-object object-show  laid @ spend
    chains-shown  false scales  0 barred-floor floor-off  0 show-part  9 object-hide  $A object-hide
    -1 placed !  godstone-fx 6.2e 18e 17.1e flicker-at  medallion-40-done fact!  great-door unlock
    15 fade-in  stalkers-stay state-off ;

\ ---- an ore laid on the scales: tipped, the ore gone - a Handful of Ash ----
: ore-shown ( -- )   \ (through its scene, to its cue: the ore on the pan)
    begin  cutscene-frame $1CC < while  stones-hidden  laid @ stone-object object-show  yield  repeat  stones-hidden ;
: ore-scene ( -- )
    ore-movie scenes:keyed-movie ore-cutscene load-scene  ['] ore-shown start drop  $10 $FF movie-key
    panic-eased  if  -1 run-scene  then ;
: lays-ore ( -- )
    stalkers-stay state!  fiona hold  15 screen:to-black-2 with-music or fade  wait-fade
    hewie-in?  hewie-cond-now @ down-cond = and if  ['] hewie-held start drop  then
    ore-scene  wait-fade  ['] ore-shown stop-doing
    laid @ spend  stones-hidden  chains-shown  true scales
    fiona 0 idle  fiona $158 180 5.733e 23.039e place-at  camera-restart  hewie-back
    15 fade-in  ore-seen resident-flag-set
    fiona reach-out-anim play  handful-of-ash item-named  handful-of-ash 1 give-item  handful-of-ash show-item-tab
    fanfare  obtained-2-msg say  got-handful-of-ash fact!  fiona reach-back-anim play  stalkers-stay state-off ;
: items-used ( who ev var value -- handled? )
    {: who ev var it :}  who 0<> if  false exit  then
    ev godstone-laid = if  it laid !  ['] lays-godstone start drop  true exit  then
    ev ore-laid = if  it laid !  ['] lays-ore start drop  true exit  then
    false ;

\ ---- the replays (the extras) ----
: replay-candles ( -- )  candles-lit ;
: replay-godstone ( -- )  fiona hold  replay-candles  white-godstone laid !  godstone-scene  replay-over ;
: replay-ore ( -- )  fiona hold  replay-candles  sulfuric-ore laid !  ore-scene  replay-over ;

\ ---- what she may do or say here ----
: the-scales ( -- )   \ (the coin on them; laid, the Godstone may be taken back)
    fiona wait-move
    placed @ 0= if
        fiona 4e 17e turn-to-xz
        scales-remarked @ if  coin-msg  else  -1 scales-remarked !  scales-msg  then  say  exit
    then
    take-back-msg yes? 0= if  exit  then
    fiona $158 180 4.619e 23.5e walk-to  fiona reach-out-anim play  4 frames
    0 placed !  godstone-fx effect-off  true scales  laid @ stone-object object-hide
    laid @ item-named  laid @ 1 give-item  laid @ show-item-tab  fanfare  obtained-2-msg say  fiona reach-back-anim play ;
: the-great-door ( -- )
    inscription-read @ 0= if
        fiona wait-move  fiona 0 turn  fiona look-at-anim play  door-msg say
        stalkers-stay state!  fiona hold  15 screen:to-dim fade wait-fade  inscription-msg say  -1 inscription-read !
        15 screen:from-dim fade wait-fade  inscription-file give-file  stalkers-stay state-off  exit
    then
    fiona wait-move  fiona $1B9 0 -3.806e 112.773e walk-to
    fiona lean-anim play  fiona strain-anim play  16 frames  fiona let-go-push-anim play  fiona -1 idle  7 frames
    wont-open-msg say  0 inscription-read ! ;
: take-camomile ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona -72.34e -34.25e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  camomile om1f-2-camomile camomile-fx takes-up  fiona reach-back-anim play ;

\ ---- its sounds: the hum once the door's open; two sets of four in turn ----
: ambient ( -- )
    room-sounds-in? 0= if  exit  then
    1 ambient-wait +!  ambient-wait @ 30 < if  exit  then  0 ambient-wait !
    first-ambient ambient-at @ +  -109.95e 50e -5.34e room-sound
    first-ambient-2 ambient-at @ +  -42e 20e 117e room-sound
    ambient-at @ 1+ 4 mod ambient-at ! ;
: hum ( -- )
    medallion-40-done fact? 0= if  exit  then
    sounds-on @ if  hum-sound 0e 0e 130e keep-sound
    else  room-sounds-in? if  hum-sound 0e 0e 130e room-sound  -1 sounds-on !  then  then ;

create cameras  $B , 0 , 0 ,   $C , 3 , 3 ,   $D , 1 , 1 ,   $E , 2 , 2 ,   $F , 1 , 1 ,   $10 , 4 , -1 ,   \ (area, set, path)
create ahead  6 , 5 ,   7 , 0 ,   8 , 1 ,   9 , 2 ,   $A , 4 ,   \ (area, exit loaded ahead)
create looks  0.56e 0e 14.62e  35e 22e 22e look,
: each-frame ( -- )
    0 exit-out  1 exit-out  2 exit-out  4 exit-out  5 5 leaves-by  cameras 6 cameras-at  ahead 5 ways-out
    looks 1 hewie-looks-at  hum  ambient
    fiona $11 in-area?  fiona 90 50 heading? and if  ['] the-scales unhurried offer  then
    medallion-40-done fact? 0=  fiona $12 in-area? and  fiona 0 50 heading? and if  ['] the-great-door unhurried offer  then
    fiona $13 in-area?  fiona 0 50 heading? and  fiona $14 in-area?  fiona 0 50 heading? and or if  ['] save-clock calm-hewie-up offer  then
    om1f-2-camomile fact? 0=  fiona -72.34e 59e -34.25e 5e 5e touches? and if  ['] take-camomile calm-only offer  then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if
        hewie send part-from-fiona  replay-act 7 = if  ['] replay-godstone  else  ['] replay-ore  then  start drop  exit
    then
    0 1 1 way-in  1 1 1 way-in  2 0 0 way-in  4 3 3 way-in  5 2 2 way-in
    came-by @ dup 2 =  swap 4 = or if  2 map-page  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-2 place!
' items-used old-mansion-1f-2 items-here
