\ story/places/castle-1f-9.fs - the hall of the half stairs in Belli Castle's 1F ($25): the stairs
\ stopping halfway (the Album Medallion up on them, fetched by Hewie at her "go there"), the sealed
\ well, the bench facing the wall, two vases (the Magnesia behind one), the Rubrum Medallion Hewie
\ sniffs out, the garden door - locked, the Garden Key opening it and the Luminessants' scene; her
\ first fright here; the motes rising, the tint by day, specks by night. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/castle-1f-9.fs (converted from the game's, not
\ loaded).
IN: story.places.castle-1f-9
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects story.kit fiona.anims hewie.anims scenes ;

own-reset

\ ---- what is said here ----
0 constant stairs-msg      \ "Why do the stairs stop halfway?"
1 constant well-msg        \ "This looks like an old well or something. The opening has been sealed up with a wooden..."
2 constant what-msg        \ "What the!"
3 constant what-was-msg    \ "What was that thing?!"
4 constant stairs-2-msg    \ "The longer I look over the edge, the more I feel like something is going to suck me..."
5 constant bench-msg       \ "The bench has been situated so it is facing the wall. No one can use it like this..."
6 constant locked-msg      \ "The door is locked. I will need to find a key if I am going to get through this doo..."
$40A9 constant luminessants-doc-msg   $A9 constant luminessants-doc

\ ---- its own state (a visit) ----
own: stairs-remarked       \ the stairs' first remark made
own: sound-started         \ the hall's sound begun (then kept up)
own: album-barked          \ Hewie has barked at the Album Medallion (once a visit)
own: hewie-up              \ Hewie up the stairs with the Album Medallion (she may look on)

\ ---- the room's things ----
0 constant rubrum-fx   1 constant magnesia-fx   2 constant album-fx
5 constant to-exit-0-area   6 constant to-exit-1-area   3 constant to-exit-1b-area   4 constant to-exit-2-area
$C constant stairs-area   $D constant well-area   $10 constant bench-area   $F constant stairs-top-area
1 constant garden-exit   6 constant garden-door   1 constant luminessants-movie   0 constant luminessants-cutscene
2 constant stairs-floor   3 constant drop-floor   0 constant hall-sound   $2E constant go-there-call
c1f-9-breakable1-broken 0 1  -30e 0e -85.4e 5e 8e  breakable: vase-1
c1f-9-breakable2-broken 2 3  -5.5e 0e -99e 5e 8e  breakable: vase-2   \ (the Magnesia behind it)
: vase ( bk group tri -- )   \ (their shards, their floor, their noise)
    {: bk g tri :}  bk $80202020 0 6 shard-look  bk 0.5e 1e 0.187e 0.312e shard-uv  bk g frees-floor  bk tri noise-from ;
vase-1 0 $1E7 vase   vase-2 1 $1F1 vase
: night? ( -- flag )  c2f-3-flag1 fact? ;
: magnesia-shows ( -- )  magnesia-fx -5.5e 1e -99e flicker-at ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds
    vase-1 as-it-stands  vase-2 as-it-stands
    vase-2 broken?  c1f-9-magnesia fact? 0= and if  magnesia-shows  then
    c1f-9-rubrum-medallion-found fact?  c1f-9-rubrum-medallion fact? 0= and if  rubrum-fx 60e 1e 37.2e flicker-at  then
    c1f-9-album-medallion fact? 0= if  album-fx -33e 43.5e 15e flicker-at  then
    effects:rising-motes
    night? if
        $10 $E $80 $80 $80 $40 rgba 20e 19.7e 36.4e specks   $10 $E $80 $80 $80 $40 rgba 30e 87e 124.7e specks
        $10 $A $80 $80 $80 $40 rgba -53.9e 28.8e -52e specks
    else  $AC $60 $26 $50 $19 $1F $12 $80 room-tint  then ;

\ ---- the garden door: the Garden Key used at it - the Luminessants' scene, the door unlocked ----
: luminessants ( -- )
    fiona hold  stalkers-stay state!  15 screen:to-black-2 with-music or fade  fiona wait-move
    luminessants-movie scenes:keyed-movie luminessants-cutscene load-scene  wait-fade  $28 $FF movie-key
    panic-eased  if  -1 run-scene  then
    fiona 8 166 -19.086e -6.777e place-at  scene-locked state!  15 fade-in
    15 screen:to-dim fade wait-fade  luminessants-doc-msg say  15 screen:from-dim fade wait-fade
    garden-door-done fact!  luminessants-doc filed  yield  summoner-on state!
    got-doc-luminessants fact? 0= if  got-doc-luminessants fact!  castle-1f-9 0 217 3 6 creature-in  then
    garden-key spend  garden-door unlock  stalkers-stay state-off  scene-locked state-off ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0= 0= if  drop false exit  then
    4 = if  ['] luminessants start drop  true  else  false  then ;

\ ---- her first fright here: thrown, nobody behind it ----
: fright ( -- )
    what-msg show  begin  fiona-free? 0= while  yield  repeat
    panic-eased  fiona hold  what-was-msg say ;
: frightened ( -- )
    c1f-9-flag1 fact? if  exit  then
    fiona-mode-now @ thrown-mode =  struck-by-now @ 0< and if  c1f-9-flag1 fact!  ['] fright start drop  then ;

\ ---- the Album Medallion up the stairs: her "go there" toward it - Hewie fetches it ----
: fetches ( -- )
    hewie hold  hewie bark-move 0 0  0e 0e 0e 0e  move  hewie wait-move  hewie follow
    hewie $3E 180 -1 walk-path-move -30.81e 100e go-to
    stairs-floor solid-floor floor-off  stairs-floor keeps-fiona-off keeps-pursuers-off or floor-on
    $CF 50 -35e 0.2e 20.3e hewie-leap  hewie wait-move  hewie 2 -1 camera-set
    c1f-9-album-medallion fact? 0= if
        album-medallion item-full? 0= if  10 trust+  then
        album-medallion c1f-9-album-medallion album-fx takes-up
    then
    hewie $50 100 -1 walk-path-move -35.79e 15.06e go-to
    drop-floor solid-floor floor-off  drop-floor keeps-fiona-off keeps-pursuers-off or floor-on
    -1 hewie-up !  $196 100 18e 0.1e 6e hewie-leap  hewie wait-move
    stairs-floor solid-floor floor-on  drop-floor solid-floor floor-on  stalkers-stay state-off
    castle-1f-9 406 0 hewie-to ;
: sends-him ( -- )
    fiona hold  stalkers-stay state!  fiona wait-move  0 hewie-up !  ['] fetches start drop
    fiona go-there-gesture-anim start-anim  fiona 1 wait-mark  go-there-call common-bank fiona sound-by
    fiona wait-anim  fiona -1 idle  7 frames
    begin  hewie stairs-top-area in-area? 0= while  yield  repeat  hewie 2 -1 camera-set
    begin  hewie-up @ 0= while  yield  repeat  16 frames ;
: album-way ( -- )
    hewie-in?  hewie-cond-now @ down-cond <> and  hewie held? 0= and  chased? 0= and 0= if  exit  then
    hewie-way-now @ 1 =  fiona one-half on-floor-of? and 0= if  exit  then
    fiona 50 -37e 16e faces-point?  started-now @ go-there-cmd = and  hewie-breakable-now @ and if  ['] sends-him start drop  then ;
: barks-at-album ( -- )  -33.85e 42.7e 15.04e hewie-barks-at ;
create album-look  -33.85e 42.7e 15.04e 44e 30e 10e look,
: hewie-album ( -- )
    c1f-9-album-medallion fact? if  exit  then
    album-barked @ 0=  hewie-in? and  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and
    hewie -33.85e 42.7e 15.04e 44e 30e centred-in? and if  -1 album-barked !  ['] barks-at-album start drop  then
    album-look 1 hewie-looks-at ;

\ ---- the Rubrum Medallion: Hewie sniffs it out (scrambling about by it) ----
: rubrum-sniffed ( -- )  c1f-9-rubrum-medallion-found c1f-9-rubrum-medallion rubrum-fx  60e 1e 37.2e  hewie-finds ;
: hewie-sniffs ( -- )
    c1f-9-rubrum-medallion-found fact?  scrambling? 0= or if  exit  then
    hewie 60e 0e 37.2e 10e 5e touches? 0= if  exit  then
    ['] rubrum-sniffed start-only ;

\ ---- what she may do or say here ----
: the-stairs ( -- )
    fiona wait-move  fiona 90 turn  fiona look-low-anim start-anim  40 frames
    stairs-remarked @ if  stairs-2-msg  else  -1 stairs-remarked !  stairs-msg  then  say  fiona wait-anim ;
: the-well ( -- )  fiona wait-move  fiona 0e -60e turn-to-xz  well-msg say ;
: the-bench ( -- )  fiona wait-move  fiona 37e -44e turn-to-xz  bench-msg say ;
: rattles-locked ( -- )
    fiona garden-exit to-door
    in-panic?  chased? or if  fiona rattle-front-slow-anim play  fiona -1 idle  7 frames  exit  then
    fiona rattle-front-anim play  fiona -1 idle  7 frames  locked-msg say ;
: pick-rubrum ( -- )  rubrum-medallion c1f-9-rubrum-medallion rubrum-fx 60e 37.2e picks-up ;
: pick-magnesia ( -- )  magnesia c1f-9-magnesia magnesia-fx -5.5e -99e picks-up ;
: spots ( -- )
    c1f-9-rubrum-medallion-found fact?  c1f-9-rubrum-medallion fact? 0= and  fiona 60e 1e 37.2e 5e 5e touches? and if
        ['] pick-rubrum calm-only offer  then
    fiona stairs-area in-area?  fiona 45 50 heading? and if  ['] the-stairs unhurried offer  then
    fiona well-area 50 faces-area? if  ['] the-well unhurried offer  then
    fiona bench-area in-area?  fiona 50 35e -45e faces-point? and if  ['] the-bench unhurried offer  then
    garden-exit before-locked? if  ['] rattles-locked any-time offer  then
    vase-2 broken? if
        c1f-9-magnesia fact? 0=  fiona -5.5e 1e -99e 5e 5e touches? and if  ['] pick-magnesia calm-only offer  then
    else  vase-2 breaks? if  magnesia-shows  then  then ;

\ ---- the hall's sound; the walls' shade ----
: sounds ( -- )
    sound-started @ if  hall-sound 0e 71e 35e keep-sound exit  then
    room-sounds-in? if  hall-sound 0e 71e 35e room-sound  -1 sound-started !  then ;
: walls-shade ( -- )
    -37.6e 70e 29.5e  -9.4e 70e 29.5e  -37.6e 32.2e 29.5e  -9.4e 32.2e 29.5e  light-doorway
    6e 72e 63e  72e 72e 63e  6e -2e 63e  72e -2e 63e  light-doorway
    -39e 72e 38e  32e 72e 38e  -39e -2e 38e  32e -2e 38e  light-doorway ;

\ ---- the movie replayed (the extras) ----
: replayed ( -- )
    fiona hold  $AC $60 $26 $50 $19 $1F $12 $80 room-tint
    luminessants-movie scenes:keyed-movie luminessants-cutscene load-scene  $28 $FF movie-key
    panic-eased  if  -1 run-scene  then  replay-over ;

\ ---- every frame ----
create cameras   \ (area, set, path)
    3 , 1 , -1 ,   4 , 0 , 0 ,   7 , 1 , -1 ,   8 , 2 , -1 ,   9 , 1 , -1 ,   $A , 2 , -1 ,   $B , 1 , -1 ,
    $11 , 0 , 0 ,   $12 , 3 , 1 ,   $13 , 0 , 0 ,   $14 , 4 , 2 ,
create entries  1 , -1 ,   1 , -1 ,   3 , 1 ,     \ (by exit 0, 1, 2: the camera)
: ways ( -- )
    3 exits-out  cameras 11 cameras-at
    fiona to-exit-0-area entered? if  0 prepare-exit  then
    fiona to-exit-1-area entered?  fiona to-exit-1b-area entered? or if  1 prepare-exit  then
    fiona to-exit-2-area entered? if  2 prepare-exit  3 map-page  then
    fiona to-exit-1b-area entered? if  2 map-page  then ;
: each-frame ( -- )
    events-held state? if  walls-shade exit  then
    ways  vase-1 breaks? drop  hewie-sniffs  album-way  frightened  sounds  hewie-album  spots  walls-shade ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    entries 3 ways-in
    saturnus-hint-on fact?  c1f-14-flag2 fact? 0= and if  c1f-14-flag2 fact!  stalker let-go  then
    came-by @ 2 = if  3 map-page  then ;
: went ( -- )   \ (through either door once the garden door is done: her fright here no more)
    garden-door-done fact?  c1f-9-flag1 fact? 0= and if
        fiona body-pos 0 0 door-in-area?  fiona body-pos 1 0 door-in-area? or if  c1f-9-flag1 fact!  then
    then ;

' came-in ' person-in ' each-frame ' went castle-1f-9 place!
' items-used castle-1f-9 items-here
