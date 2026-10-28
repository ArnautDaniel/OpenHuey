\ story/places/old-mansion-b1-6.fs - the mud maze of the old mansion's B1: four walls of flame (each
\ gate's steam; stepping close, her fright and "A wall of flame..."); the golem raced through the
\ maze by a word plate set in the altar - at each turning the word's next L or R; the right way
\ takes it into a gate's fire, which it puts out sinking (a door unlocked), a wrong one into a blaze
\ (the circle button makes it run); SALTATIO and it dances (it calms her); the plate pressing
\ machine; the corpse on the altar (its tablet's text); the golem statue; "METH" burned into the
\ ground; three Medallions; the figures kept by its far ways; the candles; Hewie's looks; its two
\ hums; the replay. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/old-mansion-b1-6.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.old-mansion-b1-6
USING: engine game-state actors messages common vectors facts room-names flag-names story-names stalker-names
    floor-flags items.names effects keys story.kit fiona.anims fiona.sounds scenes ;

own-reset

0 constant sacrifice-msg  \ "The golem sacrifice must be offered to the ever-burning wall of Hades that hinders your..."
1 constant meth-msg       \ "An unusual figure has been burned into the ground... It reads: 'METH'"
3 constant flame-wall-msg \ "A wall of flame is shooting up from the ground!"
5 constant golem-msg      \ "I wonder if this is the same type of golem statue that I saw in the main castle? Look!..."
6 constant run-msg        \ "? Button: Run"
7 constant altar-msg      \ "A corpse lies on the altar clutching a stone tablet in a death grip."
8 constant calmer-msg     \ "...... That got my mind off things. I feel a little better now."
9 constant staring-msg    \ "...... Just staring at this all day isn't going to get me out of this place any soo..."
$F constant no-plates-msg \ "There's a plate pressing machine here. There are no remaining plates currently so..."
$10 constant returned-msg \ "Returned a plate to the plate pressing machine."
$11 constant press-msg    \ "It's a plate pressing machine."
$12 constant none-left-msg \ "There are no remaining plates left. I'll have to get rid of some plate-keys befor..."
own: golem-runs  own: turned  own: race-done  own: lfoot  own: rfoot  own: read-at  own: in-17  own: in-4
own: wall-reacting  own: calmed  own: press-remarked  own: hums-on  own: noticed-4  own: pending-5  own: pending-4  own: pulse
0 constant medallion-fx   1 constant viola-fx   2 constant magist-fx   7 constant sink-fx   8 constant blaze-fx   $10 constant first-steam
3 constant walk-sound   4 constant run-sound   3 constant creature-sound   7 constant hum-sound   8 constant hum-2-sound
9 constant creature-2-sound   $A constant creature-3-sound   6 constant unlocked-sound
8 constant golem   3 constant golem-slot   2 constant press-screen
$200 constant walking   $201 constant running-anim   $400 constant turning   $401 constant turning-back
9 constant plate-back   $A constant golem-race   $11 constant golem-dance   \ (the items' events here: items.fs)

\ ---- the golem: by its animation, its steps heard ----
: fig ( -- id )  golem-slot figure ;
: fig-anim ( anim loop? fade -- )  {: a l f :}  golem-slot a l f figure-play ;
: coord ( axis -- ) ( F: -- v )   \ (0 its x, 1 its z)
    fig body-at vec@  if  frot fdrop fswap fdrop  else  fdrop fdrop  then ;
: feet-of ( model -- )   \ (a step as either foot comes down: a run's or a walk's)
    {: m | l r :}  m 1 foot-down? to l  m 0 foot-down? to r
    l lfoot @ 0= and  r rfoot @ 0= and or if  golem-runs @ if  run-sound  else  walk-sound  then  room-bank fig sound-by  then
    l lfoot !  r rfoot ! ;
: feet ( -- )  golem cast-model dup 0< if  drop exit  then  feet-of ;
create camera-areas  $D , 3 , 1 ,   $11 , 5 , 3 ,   $13 , 6 , 4 ,   $F , 4 , 2 ,
: camera-by ( row -- )  {: a :}  fig a @ entered? if  fig a cell+ @ a 2 cells + @ camera-set  then ;
: golem-cameras ( -- )  4 0 do  camera-areas i 3 * cells + camera-by  loop ;   \ (crossing into the maze's far parts: the camera there on it)
: golem-step ( -- )
    turned @ if  running-anim 1 10 fig-anim  0 turned !  then  feet  golem-cameras  yield ;
: until>= ( axis F: v -- )  {: ax F: v :}  begin  ax coord v f< while  golem-step  repeat ;
: until< ( axis F: v -- )  {: ax F: v :}  begin  ax coord v f< 0= while  golem-step  repeat ;

\ ---- the word's letters, the next L or R ----
: letter ( i -- c )  dup 4 < if  word-vars @  else  4 -  word-vars cell+ @  then  swap 8 * rshift $FF and ;
: lr? ( c -- flag )  dup [char] L =  swap [char] R = or ;
: letter? ( right? -- flag )   \ (from where it read last, the next L or R: the one asked for - read on)
    {: rt :}  begin  read-at @ 8 <  if  read-at @ letter lr? 0=  else  false  then  while  1 read-at +!  repeat
    read-at @ 8 < 0= if  false exit  then
    read-at @ letter  rt if  [char] R  else  [char] L  then  = dup if  1 read-at +!  then ;

\ ---- its turnings, its ends ----
: turns ( anim -- )   \ (round at a turning, then walking - or running - on)
    creature-sound room-bank fig sound-by  0 10 fig-anim
    begin  fig went-round? 0= while  feet  yield  repeat
    golem-runs @ if  running-anim  else  walking  then  1 4 fig-anim ;
: passes ( -- )  fig true noclip ;
: blaze-up ( spot -- )   \ (a wrong way: into a blaze at the spot)
    golem-runs @ if  walking 1 8 fig-anim  then  text-gone
    1 effects:fire blaze-fx effect!  creature-3-sound room-bank fig sound-by ;
: blaze-out ( -- )  blaze-fx dies-down  -1 race-done ! ;
: place-golem ( F: x y z deg -- )  {: F: x y z d :}  golem-slot x y z d figure-at ;
: lowered ( -- )  fig body-at vec@  fswap 0.1e f- fswap  fig body-yaw rad>deg  place-golem ;   \ (0.1 lower)
: golem-y ( -- ) ( F: -- y )  fig body-at v.y sf@ ;
: sinks ( spot -- )   \ (into the gate's fire: sinking 0.1 a frame below -25 - a fire as it starts, out below -24)
    {: k | started :}
    begin
        fig dup 0< if  drop exit  then  body? 0= if  exit  then  lowered
        started 0= if  -1 to started  k 0 effects:fire sink-fx effect!  then
        golem-y -24e f< if  sink-fx dies-down  then
        golem-y -25e f< 0= while  yield
    repeat ;
: gate-steam ( k -- )  first-steam + ;
: into-gate ( k fact door other -- )   \ (the gate's fire put out by it: the door beyond unlocked)
    {: k f door other :}
    f fact? 0= if  k gate-steam dies-down  then
    creature-sound room-bank fig sound-by  0 1 8 fig-anim  8 frames
    other fact? if  k 2 < if  hum-sound  else  hum-2-sound  then  room-bank stop-sound  then
    f fact? 0= if  creature-2-sound room-bank fig sound-by  then  creature-3-sound room-bank fig sound-by
    k sinks
    f fact? 0= if  door 0< 0= if  door unlock  then  unlocked-sound room-bank bank-sound  90 frames  then
    f fact!  k barred-floor floor-off  -1 race-done ! ;

\ ---- the legs of its way (each its next, 0 the end) ----
defer leg-01  defer leg-02  defer leg-03  defer leg-04  defer leg-05  defer leg-06  defer leg-07
: leg-00 ( -- next )
    0 read-at !  1 80e until>=  true letter? if  ['] leg-01 exit  then
    passes  1 100e until>=  0 blaze-up  1 110e until>=  blaze-out  0 ;
:noname ( -- next )
    turning turns  0 -80e until<  true letter? if  ['] leg-02 exit  then
    passes  0 -100e until<  1 blaze-up  0 -110e until<  blaze-out  0 ; is leg-01
:noname ( -- next )
    turning turns  1 20e until<  false letter? if  ['] leg-03 exit  then
    1 -40e until<  true letter? if  ['] leg-06 exit  then
    passes  1 -70e until<  2 blaze-up  1 -80e until<  blaze-out  0 ; is leg-02
:noname ( -- next )
    -1 in-17 !  turning-back turns  0 -140e until<  true letter? if  ['] leg-04 exit  then
    passes  0 -170e until<  0 omb1-6-flag2 $5E omb1-6-flag3 into-gate  0 ; is leg-03
:noname ( -- next )
    -1 in-4 !  turning turns  1 -110e until<  true letter? if  ['] leg-05 exit  then
    passes  1 -140e until<  3 blaze-up  1 -150e until<  blaze-out  0 ; is leg-04
:noname ( -- next )
    turning turns  passes  0 -60e until>=  3 omb1-6-flag5 $76 omb1-6-flag4 into-gate  0 ; is leg-05
:noname ( -- next )
    turning turns  0 -20e until>=  true letter? if  ['] leg-07 exit  then
    passes  0 15e until>=  1 omb1-6-flag3 -1 omb1-6-flag2 into-gate  0 ; is leg-06
:noname ( -- next )
    turning turns  passes  1 -5e until>=  2 omb1-6-flag4 $7B omb1-6-flag5 into-gate  0 ; is leg-07
: its-way ( -- )   \ (the golem's activity: leg by leg - once it has its body)
    begin  fig dup 0< 0= if  body?  then  0= while  yield  repeat
    fig hold  ['] leg-00  begin  ?dup while  execute  repeat ;

\ ---- the race: the golem set going (the first time its scene); the button to run ----
: hewie-tucked ( -- )  hewie hold  hewie false shown  begin  yield  again ;
: race ( -- )
    stalkers-stay state!  fiona hold  0 golem-runs !  0 turned !  0 race-done !  -1 lfoot !  -1 rfoot !
    15 screen:to-black-2 with-music or fade  golem golem-slot figure-in  yield
    omb1-6-flag1 fact? 0= if
        1 scenes:keyed-movie 0 load-scene  wait-fade  0 hide-part  4 barred-floor floor-off
        hewie-in? if  ['] hewie-tucked start drop  then
        $28 $78 movie-key  panic-eased  if  -1 run-scene  then  wait-fade
        panic-held state!  fiona 0 idle  fiona $56 -28 83.405e -17.137e place-at
        golem-slot 74.6e 0e 13.8e 0e figure-at  fig 3 1 camera-set  omb1-6-flag1 fact!
    else
        wait-fade  panic-eased  panic-held state!  hewie-in? if  ['] hewie-tucked start drop  then
        0 hide-part  4 barred-floor floor-off
        fiona $5E -62 92.522e -37.951e place-at  golem-slot 75e 0e -40e 0e figure-at  fig 1 -1 camera-set
    then
    golem-slot figure-on  walking 1 0 fig-anim  fig follow  camera-restart  ['] its-way start drop
    run-msg show  15 screen:from-black fade wait-fade
    begin  race-done @ 0= while
        circle button-pressed?  golem-runs @ 0= and if  text-gone  -1 golem-runs !  -1 turned !  then  yield
    repeat
    15 screen:to-black-2 fade wait-fade  panic-held state-off  text-gone
    ['] hewie-tucked running? if  ['] hewie-tucked stop-doing  hewie true shown  hewie let-go  then
    ['] its-way stop-doing  golem-slot figure-out  0 show-part  4 barred-floor floor-on
    fiona follow  camera-restart  15 fade-in  stalkers-stay state-off ;

\ ---- SALTATIO: it dances ----
: dance ( -- )
    stalkers-stay state!  fiona hold  15 screen:to-black-2 with-music or fade  golem golem-slot figure-in
    $37 1e background  wait-fade  no-pause state!  panic-eased  panic-held state!
    fiona $5D 180 75e -30e place-at  1 30e -10e 0e 4e shot  fiona false shown  0 hide-part
    golem-slot figure-on  4 barred-floor floor-off  golem-slot 75e 0e -40e 0e figure-at  4 barred-floor floor-on
    15 screen:from-black fade  golem-slot $1300 0 0 figure-play  0 pulse !
    pulse @ $5A $44 $46 $52 $4B pulse-tint  1 pulse +!  10 frames  no-track 1e background
    begin  golem-slot figure-came-round? 0= while
        pulse @ $5A $44 $46 $52 $4B pulse-tint  pulse @ 1+ dup $60 > if  drop 0  then  pulse !  yield
    repeat
    15 screen:to-black fade wait-fade
    $7C $74 $78 $5A $44 $46 $52 $4B room-tint  0 show-part  golem-slot figure-out  fiona true shown  shot-off
    panic-held state-off  no-pause state-off  15 fade-in
    calmed @ if  staring-msg say
    else
        fiona hand-to-mouth-anim start-anim  calmer-msg say  fiona wait-anim
        $E4 ease-panic  fiona send calm-reset  3 avoid-prompt  calmed-sound common-bank fiona sound-by  -1 calmed !  30 frames
    then  stalkers-stay state-off ;

\ ---- the plate pressing machine; a plate given back ----
: the-press ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move
    plate-pressable? if
        press-remarked @ 0= if  press-msg say  -1 press-remarked !  then
        plate-made fact-off  press-screen open-subscreen  yield
        plate-made fact? if  word-plate-key item-named  word-plate-key show-item-tab  fanfare  obtained-2-msg say  then
        plate-made fact!
    else  press-remarked @ if  none-left-msg  else  -1 press-remarked !  no-plates-msg  then  say  then
    stalkers-stay state-off ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0<> if  drop false exit  then
    case
        plate-back of  [: fiona wait-move  returned-msg say ;]  endof
        golem-race of  ['] race  endof
        golem-dance of  ['] dance  endof
        >r false r>
    endcase  dup if  start drop  true  then ;

\ ---- the walls of flame: her fright ----
: recoils ( a1 a2 a3 -- )   \ (by her heading: startled, looking behind, staggering, else doubled over)
    {: d1 d2 d3 :}  -1 wall-reacting !  fiona hold  fiona wait-move
    fiona d1 60 heading? if  startled-anim  else  fiona d2 60 heading? if  look-behind-anim
    else  fiona d3 30 heading? if  stagger-anim  else  doubled-over-anim  then  then  then  fiona swap start-anim
    stalker-in? 0= if  16 frames  flame-wall-msg show  then
    fiona wait-anim  fiona -1 idle  7 frames  0 wall-reacting ! ;
: wall-0 ( -- )  -45 45 0 recoils ;   : wall-1 ( -- )  45 -45 90 recoils ;
: wall-2 ( -- )  0 90 45 recoils ;     : wall-3 ( -- )  45 -45 90 recoils ;
: walls ( -- )
    wall-reacting @ if  exit  then
    omb1-6-flag2 fact? 0=  fiona $1C entered? and if  panic-eased  ['] wall-0 start drop  then
    omb1-6-flag3 fact? 0=  fiona $1E entered? and if  panic-eased  ['] wall-1 start drop  then
    omb1-6-flag4 fact? 0=  fiona $20 entered? and if  panic-eased  ['] wall-2 start drop  then
    omb1-6-flag5 fact? 0=  fiona $22 entered? and if  panic-eased  ['] wall-3 start drop  then ;

\ ---- what she may look at or take here ----
: the-altar ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move  altar-msg say
    15 screen:to-dim fade wait-fade  sacrifice-msg say  15 screen:from-dim fade wait-fade  stalkers-stay state-off ;
: the-golem ( -- )  fiona wait-move  fiona look-at-anim start-anim  16 frames  golem-msg say  fiona wait-anim ;
: meth ( F: x z -- )  fiona wait-move  fiona turn-to-xz  fiona look-low-anim start-anim  16 frames  meth-msg say  fiona wait-anim ;
: take-medallion ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona 2.06e -99.68e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  medallion omb1-6-medallion medallion-fx takes-up  fiona reach-back-anim play ;
: take-viola ( -- )  viola-medallion omb1-6-viola-medallion viola-fx -138.94e -114.31e picks-up ;
: take-magist ( -- )  magist-medallion omb1-6-magist-medallion magist-fx 39.98e -40.67e picks-up ;
: viola-sniffed ( -- )  omb1-6-viola-medallion-found omb1-6-viola-medallion viola-fx  -138.94e 1e -114.31e  hewie-finds ;
: sits-by ( -- )  73.86e 0e -40.16e hewie-sits-by ;

\ ---- as she comes in ----
create flames  117.5e f, 11.7e f, -21.8e f,   -217e f, 20.1e f, 20e f,   -175e f, 20.1e f, 4.7e f,   -70e f, 20.1e f, -124.7e f,
    -20e f, 20.1e f, -96.7e f,   11.7e f, 20.1e f, -55.8e f,   43e f, 20.1e f, -26.5e f,   -33.2e f, 20.1e f, -6.2e f,
    89.3e f, 20.1e f, 50e f,   -12.4e f, 28.3e f, 24.6e f,   -40e f, 20.1e f, 95.6e f,   -65.5e f, 20.1e f, 0e f,
: candles-lit ( -- )  $C 0 do  i 3 +  i 0= if  0  else  1  then  flames i 3 * floats +  dup f@  dup float+ f@  2 floats + f@  flame-at  loop ;
: steams ( -- )   \ (the gates still burning: their steam)
    omb1-6-flag2 fact? 0= if  0 barred-floor floor-on  0 effects:steam 0 gate-steam effect!  then
    omb1-6-flag3 fact? 0= if  1 barred-floor floor-on  1 effects:steam 1 gate-steam effect!  then
    omb1-6-flag4 fact? 0= if  2 barred-floor floor-on  2 effects:steam 2 gate-steam effect!  then
    omb1-6-flag5 fact? 0= if  3 barred-floor floor-on  3 effects:steam 3 gate-steam effect!  then ;
: came-in ( exit -- )
    drop  room-sounds  0 show-part  4 barred-floor floor-on  steams
    omb1-6-medallion fact? 0= if  medallion-fx 2.06e 6.5e -99.68e flicker-at  then
    omb1-6-magist-medallion fact? 0= if  magist-fx 39.98e 1e -40.67e flicker-at  then
    omb1-6-viola-medallion-found fact?  omb1-6-viola-medallion fact? 0= and if  viola-fx -138.94e 1e -114.31e flicker-at  then
    candles-lit  1 $2300 reverb ;

\ ---- the figures kept by its far ways ----
: figures ( -- )
    fiona 5 entered? if  -1 pending-5 !  then
    fiona 5 left? if  pending-5 @ 0= if  3 figure-out  then  0 pending-5 !  then
    fiona 4 entered? if  -1 pending-4 !  then
    fiona 4 left? if  pending-4 @ 0= if  3 figure-out  4 figure-out  5 figure-out  then  0 pending-4 !  then
    pending-5 @ if  $1C 3 1 figure-in-by  0 pending-5 !  then
    pending-4 @ if  $14 3 figure-in  $15 4 figure-in  $16 5 figure-in  0 pending-4 !  then ;

\ ---- its two hums (by which gates are out, and where she is) ----
: hums ( -- )
    fiona $11 entered? if  -1 in-17 !  then  fiona $11 left? if  0 in-17 !  then
    fiona 4 entered? if  -1 in-4 !  then  fiona 4 left? if  0 in-4 !  then
    hums-on @ 0= if
        room-sounds-in? 0= if  exit  then
        omb1-6-flag2 fact? 0= if  hum-sound -185e 10e 20e room-sound  else  omb1-6-flag3 fact? 0= if  hum-sound 30e 10e -42e room-sound  then  then
        omb1-6-flag4 fact? 0= if  hum-2-sound -20e 10e 10e room-sound  else  omb1-6-flag5 fact? 0= if  hum-2-sound -40e 10e -110e room-sound  then  then
        -1 hums-on !  exit
    then
    omb1-6-flag2 fact? 0=  omb1-6-flag3 fact? 0= or if
        in-17 @  omb1-6-flag2 fact? 0= and  omb1-6-flag3 fact? or if  hum-sound -185e 10e 20e keep-sound  then
        in-17 @ 0=  omb1-6-flag3 fact? 0= and  omb1-6-flag2 fact? or if  hum-sound 30e 10e -42e keep-sound  then
    then
    omb1-6-flag4 fact? 0=  omb1-6-flag5 fact? 0= or if
        in-4 @ 0=  omb1-6-flag4 fact? 0= and  omb1-6-flag5 fact? or if  hum-2-sound -20e 10e 10e keep-sound  then
        in-4 @  omb1-6-flag5 fact? 0= and  omb1-6-flag4 fact? or if  hum-2-sound -40e 10e -110e keep-sound  then
    then ;

\ ---- the replay (the extras) ----
: replayed ( -- )
    fiona hold  steams  candles-lit  golem golem-slot figure-in  yield
    1 scenes:keyed-movie 0 load-scene  $28 $78 movie-key  panic-eased  if  -1 run-scene  then  golem-slot figure-out  replay-over ;

create cameras  8 , 0 , -1 ,   9 , 1 , -1 ,   $A , 0 , -1 ,   $B , 2 , 0 ,   $C , 1 , -1 ,   $D , 3 , 1 ,
    $E , 3 , 1 ,   $F , 4 , 2 ,   $10 , 3 , 1 ,   $11 , 5 , 3 ,   $12 , 5 , 3 ,   $13 , 6 , 4 ,   \ (area, set, path)
create entries  6 , 4 ,   5 , 3 ,   4 , 2 ,   2 , 0 ,   \ (by exit 0..3: the camera)
create ahead  4 , 0 ,   5 , 1 ,   6 , 2 ,   7 , 3 ,   \ (area, exit loaded ahead)
create calm-looks  144.66e 0e -5.88e  18e 12e 12e look,   73.86e 0e -40.16e  25e 20e 20e look,
: each-frame ( -- )
    hums  0 exit-out  1 1 leaves-by  2 2 leaves-by  3 3 leaves-by  cameras $C cameras-at  ahead 4 ways-out  figures
    walls  calm-looks 2 hewie-looks-at
    noticed-4 30 73.86e 0e -40.16e 12e 10e notices? if  ['] sits-by start drop  then
    omb1-6-viola-medallion-found fact? 0=  scrambling? and  hewie -138.94e 0e -114.31e 10e 5e touches? and if  ['] viola-sniffed start-only  then
    3 117.5e 11.7e -21.8e 8e -4e flame-stirred
    fiona $14 50 faces-area? if  ['] the-press unhurried offer  then
    fiona $15 50 faces-area? if  ['] the-altar unhurried offer  then
    fiona $16 in-area?  fiona 50 75e -40e faces-point? and if  ['] the-golem unhurried offer  then
    fiona $1B in-area? if  [: -170e 20e meth ;] unhurried offer  then
    fiona $1D in-area? if  [: 12e -41e meth ;] unhurried offer  then
    fiona $1F in-area? if  [: -19e -6e meth ;] unhurried offer  then
    fiona $21 in-area? if  [: -60e -110e meth ;] unhurried offer  then
    omb1-6-medallion fact? 0=  fiona 2.06e 6.5e -99.68e 5e 5e touches? and if  ['] take-medallion calm-only offer  then
    omb1-6-viola-medallion-found fact?  omb1-6-viola-medallion fact? 0= and  fiona -138.94e 0e -114.31e 5e 5e touches? and if
        ['] take-viola calm-only offer
    then
    omb1-6-magist-medallion fact? 0=  fiona 39.98e 0e -40.67e 5e 5e touches? and if  ['] take-magist calm-only offer  then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop  exit  then
    entries 4 ways-in ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-b1-6 place!
' items-used old-mansion-b1-6 items-here
