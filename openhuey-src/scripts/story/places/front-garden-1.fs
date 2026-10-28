\ story/places/front-garden-1.fs - Belli Castle's gate and the drive before it ($00): the gate shut
\ and locked (the opening's scene there, at first), the castle gate key's way in; the broken
\ marionette up in the tree - her look at it (a scene) and Hewie fetching it; the lamps that flicker
\ at night; a breakable, two Medallions (one Hewie sniffs out), two spirits let out once. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/front-garden-1.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.front-garden-1
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects scenes story.kit fiona.anims hewie.anims ;

own-reset

\ ---- what is said here ----
$C constant gate-locked-msg     \ "The gate has been shut and locked solidly. It will be impossible to open it without a..."
$D constant in-the-tree-msg     \ (something up in the tree, before her clothes)
$E constant something-up-msg    \ "It looks like there is something up there..."
$F constant too-high-msg        \ "It's going to be too hard for me to get up there."

\ ---- its own state (a visit) ----
own: zone-noticed          \ Hewie has looked at the tree (once a visit)
own: fetched               \ Hewie has the marionette and is on his way back (her watch over)
own: lamp-0  own: lamp-1  own: lamp-2  own: lamp-3   \ each lamp's flicker (its activity; 0 none)

\ ---- the room's things ----
0 constant marionette-part   1 constant whole-part   2 constant broken-part   3 constant first-lamp-part
1 constant crate-group   0 constant tree-group
4 constant tree-area   5 constant gate-area   1 constant near-exit-0-area   2 constant near-exit-1-area
$A constant by-tree-area   $B constant hewie-tree-area   $C constant lamp-0-area   $D constant lamp-1-area
$E constant lamp-2-area   $F constant hewie-near-tree-area
0 constant medallion-fx   1 constant medallion-2-fx   2 constant first-glow   \ (the place's effects: the sparkles, the lamps' glows)
4 constant tree-movie   3 constant tree-night-movie   2 constant tree-cutscene
1 constant gate-movie   0 constant gate-cutscene
$222 constant gate-file   $223 constant tree-file   $8276 constant fetched-file
$1A constant gate-seen   $5E constant tree-seen   \ (resident flags: the ending's)
$35 constant castle-1f-hall   $F constant gate-key-event
fg-1-breakable-broken whole-part broken-part  -284e 60e -97e 5e 8e  breakable: the-crate
the-crate $80404040 0 4 shard-look  the-crate 0e 0.687e 0.187e 0.312e shard-uv
the-crate crate-group frees-floor  the-crate $185 noise-from
: night? ( -- flag )  c2f-3-flag1 fact? ;

\ ---- the lamps: lit at night, their glows; now and then one flickers ----
: lamp-spot ( g -- ) ( F: -- x z )   \ (the four glows' spots, 100.8 up)
    case  0 of  -332e -12e  endof  1 of  -368e 55e  endof  2 of  -368e 165e  endof  >r -332e 165e r>  endcase ;
: lamp ( g on -- )   \ (its part, its glow)
    {: g on :}  g first-lamp-part + on part
    on if  first-glow g +  $80808018  g lamp-spot {: F: x z :}  x 100.8e z 30e glow-at
    else  first-glow g + effect-off  then ;
: blinks ( g n -- )  {: g n :}  n 0 do  g true lamp  yield  g false lamp  yield  loop ;   \ (on and off, a frame each)
: flickers ( g -- )   \ (off, a blink, longer blinks by chance, on again)
    {: g :}  g false lamp  yield  g true lamp  yield  g false lamp  4 frames
    50 chance? if  g true lamp  2 frames  g false lamp  yield  g true lamp  yield  g false lamp  8 frames  then
    25 chance? if  g true lamp  2 frames  g false lamp  yield  g true lamp  yield  g false lamp  16 frames  then
    5 chance? if  g 2 blinks  32 frames  then
    g true lamp  yield  g false lamp  yield  g true lamp ;
: flicker-0 ( -- )  0 flickers ;   : flicker-1 ( -- )  1 flickers ;
: flicker-2 ( -- )  2 flickers ;   : flicker-3 ( -- )  3 flickers ;
: lamp-act ( g -- addr )  cells lamp-0 + ;   \ (own cells in a row)
: flicker-lamp ( g -- )   \ (afresh: one already flickering starts over)
    dup lamp-act @ ?dup if  stop-act  then
    dup case  0 of  ['] flicker-0  endof  1 of  ['] flicker-1  endof  2 of  ['] flicker-2  endof  >r ['] flicker-3 r>  endcase
    start swap lamp-act ! ;
: by-chance ( g -- )  30 chance? if  flicker-lamp  else  drop  then ;
: lamps-flicker ( -- )   \ (at night: as she passes under them; and, rarely, by itself)
    night? 0= if  exit  then
    fiona lamp-0-area entered?  fiona lamp-0-area left? or if  0 by-chance  then
    fiona lamp-1-area entered?  fiona lamp-1-area left? or if  1 by-chance  then
    fiona lamp-2-area entered? if  2 by-chance  then
    fiona lamp-2-area left? if  3 by-chance  then
    20 chance? 0= if  exit  then
    1 chance? if  0 flicker-lamp  else  1 chance? if  1 flicker-lamp  else  1 chance? if  2 flicker-lamp  else  1 chance? if  3 flicker-lamp  then  then  then  then ;
: night-look ( -- )   \ (the night's blend, the lamps lit, the specks under them)
    $80C0F2AA 0 screen-blend
    6 0 do  first-lamp-part i + show-part  loop  4 0 do  i true lamp  loop
    $10 $A $80 $80 $80 $40 rgba -368e 100e 55e specks      $A $E $80 $80 $80 $40 rgba -332e 100.8e -12e specks
    $10 $E $80 $80 $80 $40 rgba -332e 100.8e 165e specks   $10 $C $80 $80 $80 $40 rgba -368e 100.8e 165e specks
    $A $E $80 $80 $80 $50 rgba -327.5e 82e 236.3e specks   $A $E $80 $80 $80 $50 rgba -372.5e 82e 236.3e specks ;

\ ---- as she comes in ----
: spirits-out ( -- )   \ (the first time the way is open: two spirits let out here)
    0 doors:door-locked?  cb1-6-flag3 fact? or if  exit  then
    cb1-6-flag3 fact!  this-room @ 0 338 4 15 creature-in  this-room @ 0 320 4 15 creature-in ;
: came-in ( exit -- )
    drop  night? if  night-look  else  $AC $60 $26 $50 $19 $1F $12 $80 room-tint  then
    got-broken-marionette fact? if  marionette-part hide-part  else  marionette-part show-part  then
    the-crate as-it-stands
    fg-1-medallion-found fact?  fg-1-medallion fact? 0= and if  medallion-fx -331.4e 61e 239.2e flicker-at  then
    spirits-out
    fg-1-medallion-2 fact? 0= if  medallion-2-fx -366e 72e 26e flicker-at  then ;

\ ---- the scenes ----
: hewie-out-of-it ( -- )   \ (down here: hidden for the scene)
    hewie-in?  hewie-cond-now @ down-cond = and if  hewie false shown  then ;
: hewie-back ( -- )  hewie true shown  hewie let-go ;
: the-tree ( -- )   \ (her look up at it: the scene - the marionette up there)
    stalkers-stay state!  fiona wait-move  fiona -201.5e -121.8e turn-to-xz
    fiona look-at-anim start-anim  40 frames  something-up-msg show
    night? if  tree-night-movie  else  tree-movie  then  scenes:keyed-movie tree-cutscene load-scene
    wait-closed  15 screen:to-black-2 with-music or fade  wait-fade
    hewie-out-of-it  $28 $FF movie-key  panic-eased
    if  -1 run-scene  then
    fiona 0 idle  fiona $1B0 -173 -242.807e -79.976e place-at  camera-restart  hewie-back
    found-file-223 fact!  tree-seen resident-flag-set  15 fade-in  tree-file give-file  stalkers-stay state-off ;
: the-gate ( -- )   \ (at first: the opening's scene by the gate)
    stalkers-stay state!  15 screen:to-black-2 with-music or fade  fiona wait-move  scene-locked state!
    gate-movie scenes:unseen-movie gate-cutscene load-scene  wait-fade  hewie-out-of-it  panic-eased
    if  -1 run-scene  then
    scene-locked state-off  fiona 0 idle  fiona $148 -11 -350.123e 232.183e place-at  camera-restart  hewie-back
    found-file-222 fact!  15 fade-in  gate-seen resident-flag-set  gate-file give-file  stalkers-stay state-off ;
: into-the-castle ( -- )   \ (the castle gate key: by the forced way, into the hall)
    stalkers-stay state!  fiona wait-move  castle-1f-hall preload  15 screen:to-black-2 with-music or fade  wait-fade
    hewie let-go  stalker let-go  world-held state!  forced-exit take-exit ;
: key-used ( who ev var value -- handled? )
    2drop  swap 0= 0= if  drop false exit  then
    gate-key-event = dup if  ['] into-the-castle start drop  then ;

\ ---- the marionette: Hewie sent up for it ("go there" by the tree) - she watches ----
: brings-it ( -- )   \ (his: to the tree's foot, sitting there - got it - and back on his way)
    hewie hold  hewie wait-move  hewie bark-move 0 0  0e 0e 0e 0e  move  hewie wait-move
    tree-group solid-floor floor-off  tree-group keeps-fiona-off floor-on  tree-group keeps-pursuers-off floor-on
    hewie $208 138 -1 walk-path-move -206e -116e go-to  hewie sit-a-while-anim play
    got-broken-marionette fact!  10 trust+
    broken-marionette item-named  broken-marionette 1 give-item  broken-marionette show-item-tab  fanfare  obtained-2-msg say
    marionette-part hide-part  40 trust+  fiona follow
    hewie $54 -1 run-path-move path-to  tree-group solid-floor floor-on  -1 fetched ! ;
: watches ( -- )   \ (hers: her eyes on him, the tree, then him again till he is back)
    begin  fiona-free? 0= while  yield  repeat  panic-eased
    fiona hold  fiona wait-move  fiona hewie look-at  yield
    begin  hewie by-tree-area in-area? 0= while  yield  repeat
    3 -1 her-camera  fiona -1 look-at  yield
    fiona -201.5e -121.8e turn-to-xz  fiona look-at-anim 10 start-blend  fiona -1 idle  7 frames
    fiona hewie look-at  yield
    begin  fetched @ 0= while  yield  repeat
    fiona -1 look-at  yield  fetched-file give-file ;
: sent-up ( -- )
    got-broken-marionette fact?  started-now @ go-there-cmd <> or if  exit  then
    hewie-in?  hewie-cond-now @ down-cond <> and  0= if  exit  then
    fiona by-tree-area in-area?  hewie hewie-tree-area in-area? and  fiona 50 -204e -117e faces-point? and 0= if  exit  then
    0 fetched !  ['] watches start drop  ['] brings-it start drop ;

\ ---- what she may do or say here ----
: gate-locked ( -- )  fiona wait-move  fiona hands-clasped-anim 10 start-blend  gate-locked-msg say  fiona wait-anim ;
: up-there ( msg -- )  fiona wait-move  fiona -201.5e -121.8e turn-to-xz  say ;
: in-the-tree ( -- )  in-the-tree-msg up-there ;
: too-high ( -- )  too-high-msg up-there ;
: pick-medallion ( -- )  medallion fg-1-medallion medallion-fx -331.4e 239.2e picks-up ;
: reach-medallion ( -- )   \ (up on the ledge: she reaches for it)
    fiona wait-move  fiona -366e 26e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  medallion fg-1-medallion-2 medallion-2-fx takes-up  fiona reach-back-anim play ;
: spots ( -- )
    fiona gate-area in-area?  fiona 0 50 heading? and if
        found-file-222 fact? if  ['] gate-locked  else  ['] the-gate  then  unhurried offer
    then
    fiona held? 0=  got-broken-marionette fact? 0= and  fiona tree-area 45 faces-area? and if
        found-file-202 fact? 0= if  ['] in-the-tree  else  found-file-223 fact? 0= if  ['] the-tree  else  ['] too-high  then  then
        unhurried offer
    then
    fg-1-medallion-found fact?  fg-1-medallion fact? 0= and  fiona -331.4e 61e 239.2e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then
    fg-1-medallion-2 fact? 0=  fiona -366e 72e 26e 5e 5e touches? and if  ['] reach-medallion calm-only offer  then
    the-crate breaks? drop ;

\ ---- Hewie: the Medallion he sniffs out; the tree he looks up at (the marionette in it) ----
: medallion-sniffed ( -- )  fg-1-medallion-found fg-1-medallion medallion-fx  -331.4e 61e 239.2e  hewie-finds ;
: hewie-sniffs ( -- )
    fg-1-medallion-found fact?  scrambling? 0= or if  exit  then
    hewie -331.4e 60e 239.2e 10e 5e touches? 0= if  exit  then
    ['] medallion-sniffed start-only ;
: barks-at-tree ( -- )  -210e 60e -114e hewie-barks-at ;
: hewie-looks ( -- )   \ (by the tree, the marionette up there: once a visit he barks at it)
    got-broken-marionette fact?  zone-noticed @ or  hewie hewie-near-tree-area in-area? 0= or if  exit  then
    hewie-in?  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and 0= if  exit  then
    hewie -210e 60e -114e 75e 20e centred-in? 0= if  exit  then
    -1 zone-noticed !  ['] barks-at-tree start drop ;

\ ---- every frame: the far view's depth, the walls' shade, its ways out ----
: far-view? ( -- flag )  cam-set@ 2 = ;
: depth ( -- )  cam-changed? 0= if  exit  then  far-view? if  1e 1e 100e 200e depth-range  else  depth-range-off  then ;
: wall-shade ( -- )
    -370e 76e 178e  -370e 76e 109e  -370e 20e 178e  -370e 20e 109e  light-doorway
    far-view? if  -380e 74e 250e  -368e 74e 175e  -380e 22e 250e  -368e 22e 175e  light-doorway  then
    -370e 76e 110e  -370e 76e 41e  -370e 20e 110e  -370e 20e 41e  light-doorway
    -370e 76e 38e  -370e 76e -8e  -370e 20e 39e  -370e 20e -8e  light-doorway ;
create cameras  6 , 1 , 1 ,   7 , 0 , -1 ,   8 , 2 , 0 ,   9 , 0 , -1 ,   \ (area, set, path)
create ahead  near-exit-0-area , 0 ,   near-exit-1-area , 1 ,
create entries  0 , -1 ,   1 , 1 ,                                 \ (by exit 0, 1: the camera)
: ways ( -- )  2 exits-out  cameras 4 cameras-at  ahead 2 ways-out ;

\ ---- the movies replayed (the extras' theatre: the tree at night, the gate) ----
: replay-tree ( -- )
    fiona hold  night-look  marionette-part show-part
    tree-night-movie scenes:keyed-movie tree-cutscene load-scene  $28 $FF movie-key  panic-eased
    if  -1 run-scene  then  replay-over ;
: replay-gate ( -- )
    fiona hold  gate-movie scenes:unseen-movie gate-cutscene load-scene  night-look  marionette-part show-part  panic-eased
    if  -1 run-scene  then  replay-over ;
: replaying ( -- )   \ (the extras' list: its rows for these two)
    hewie send part-from-fiona  replay-var var@ 8 = if  ['] replay-tree  else  ['] replay-gate  then  start drop ;

: each-frame ( -- )
    events-held state? if  depth wall-shade exit  then
    ways  lamps-flicker  sent-up  hewie-sniffs  hewie-looks  spots  depth  wall-shade ;
: person-in ( i -- )
    fiona-p = if  events-held state? if  replaying exit  then  entries 2 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went front-garden-1 place!
' key-used front-garden-1 items-here
