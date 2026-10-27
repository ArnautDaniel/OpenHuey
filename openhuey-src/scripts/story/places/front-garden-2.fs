\ story/places/front-garden-2.fs - the yard before Belli Castle ($13): the grate bolted from the
\ other side until, Hewie found in the wire, it swings open (a scene); three crates, a Medallion in
\ one; the windows' light; at night Hewie's whine from the wire, and - Hewie still caught - she
\ won't go on toward the cage but turns back for him. A place (story/kit.fs). The original's
\ scripts, for checking: story/rooms-ref/front-garden-2.fs (converted from the game's, not loaded).
IN: story.places.front-garden-2
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects story.kit fiona.anims scenes ;

own-reset

\ ---- what is said here ----
0 constant bolted-msg     \ "It is bolted shut from the other side. I'm not going to be able to open this..."
1 constant flicker-msg    \ "I wonder what kind of a room is on the other side? I can see the light flicker from can..."

\ ---- its own state (a visit) ----
own: turned-back           \ she has turned back for Hewie (once a visit)
own: whine-frames          \ frames to Hewie's next whine (caught in the wire)

\ ---- the room's things ----
0 constant shut-grate-group   1 constant open-grate-group   \ (the floor barred while the grate is shut, and once it is open)
6 constant no-hint-part   7 constant hint-part              \ (the Saturnus hint, once the key is used)
0 constant to-cage-area   2 constant to-exit-2-area   3 constant near-exit-2-area   4 constant near-exit-0-area
5 constant near-exit-2b-area   6 constant near-exit-1-area   $E constant grate-area   $F constant by-grate-area
$10 constant window-area   $11 constant window-2-area
0 constant medallion-fx   15 constant whine-sound   $B constant grate-door
1 constant grate-movie   0 constant grate-cutscene
fg-2-breakable1-broken 0 1  131e 60e -244e 5e 8e  breakable: crate-1
fg-2-breakable2-broken 2 3  130e 60e -237e 5e 8e  breakable: crate-2   \ (the Medallion in it)
fg-2-breakable3-broken 4 5  137e 60e -240e 5e 8e  breakable: crate-3
: crate ( bk group -- )   \ (their shards, their floor, their noise)
    over $80303030 0 6 shard-look  over 0e 1e 0.187e 0.312e shard-uv  over swap frees-floor  $1E4 noise-from ;
crate-1 2 crate   crate-2 3 crate   crate-3 4 crate
: night? ( -- flag )  c2f-3-flag1 fact? ;
: wire-night? ( -- flag )   \ (Hewie caught in the wire, not yet freed: she turns back for him)
    c2f-3-flag2 fact? 0=  hewie-found-in-wire fact? and  hewie-freed fact? 0= and ;

\ ---- as she comes in ----
: night-specks ( -- )
    $10 $A $80 $80 $80 $30 rgba 152e 92.2e -60e specks   $E $C $80 $80 $80 $30 rgba 128e 92.2e -60e specks
    $10 $A $80 $80 $80 $30 rgba 152e 56.7e 12.9e specks  $A $A $80 $80 $80 $30 rgba 128e 56.7e 12.9e specks ;
: grate ( open? -- )   \ (shut, or swung open a quarter turn - the floor barred before it, or behind it)
    dup if  pi f2/ fnegate  else  0e  then  s" kousi" turn-y object!
    if  shut-grate-group barred-floor floor-off  open-grate-group barred-floor floor-on
    else  open-grate-group barred-floor floor-off  shut-grate-group barred-floor floor-on  then ;
: whines ( -- )  begin  yield  whine-frames whine-due? if  whine-sound 228e 5e 78e room-sound  then  again ;
: came-in ( exit -- )
    drop  room-sounds
    night? if  night-specks  else  $AC $60 $26 $50 $19 $1F $12 $80 room-tint  then
    fg-2-flag1 fact? grate
    saturnus-hint-on fact? if  hint-part  else  no-hint-part  then  show-part
    night?  hewie-found-in-wire fact? 0= and if  30 whine-frames !  ['] whines start drop  then
    crate-1 as-it-stands  crate-2 as-it-stands  crate-3 as-it-stands
    crate-2 broken?  fg-2-medallion fact? 0= and if  medallion-fx 130e 61e -237e flicker-at  then ;

\ ---- the grate swung open: Hewie found in the wire, she near it (a scene) ----
: grate-opens ( -- )
    fiona hold  stalkers-stay state!  15 screen:to-black-2 with-music or fade
    grate-movie scenes:unseen-movie grate-cutscene load-scene  fiona wait-move  wait-fade  panic-eased
    if  -1 run-scene  then  wait-fade
    fiona $20A -35 133.34e -76.62e place-at  fiona 0 idle  camera-restart
    fg-2-flag1 fact!  grate-door unlock  true grate  stalkers-stay state-off
    15 screen:from-black with-music or fade ;
: grate-near ( -- )   \ (once: crossing the area by it, the stalker not here)
    hewie-found-in-wire fact?  fg-2-flag1 fact? 0= and 0= if  exit  then
    fiona by-grate-area entered?  fiona by-grate-area left? or  stalker-in? 0= and 0= if  exit  then
    fg-2-flag1 fact!  panic-eased  ['] grate-opens start drop ;
\ ---- at night, Hewie still caught: heading for the cage she turns back (to the bedroom by the wire) ----
: turns-back ( -- )
    fiona hold  stalkers-stay state!  15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade
    world-held state!  scene-locked state!  0 prepare-exit  stalkers-stay state-off
    forced-exit 1 or take-exit  0e sounds-scaled ;
: toward-the-cage ( -- )
    wire-night? 0=  turned-back @ or if  exit  then
    fiona to-cage-area entered? 0= if  exit  then
    -1 turned-back !  panic-eased  ['] turns-back start drop ;

\ ---- what she may do or say here ----
: bolted ( -- )  fiona wait-move  fiona hands-clasped-anim 10 start-blend  bolted-msg say  fiona wait-anim ;
: flickering ( F: x z -- )  fiona wait-move  fiona turn-to-xz  flicker-msg say ;
: window-1 ( -- )  70e -160e flickering ;
: window-2 ( -- )  100e -210e flickering ;
: pick-medallion ( -- )  medallion fg-2-medallion medallion-fx 130e -237e picks-up ;
: medallion-shows ( -- )  medallion-fx 130e 61e -237e flicker-at ;
: spots ( -- )
    fg-2-flag1 fact? 0=  fiona grate-area in-area? and  fiona -45 50 heading? and if  ['] bolted any-time offer  then
    fiona window-area in-area?  fiona 90 60 heading? and if  ['] window-1 unhurried offer  then
    fiona window-2-area in-area?  fiona -45 60 heading? and if  ['] window-2 unhurried offer  then
    crate-1 breaks? drop  crate-3 breaks? drop
    crate-2 broken? if
        fg-2-medallion fact? 0=  fiona 130e 61e -237e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then
    else  crate-2 breaks? if  medallion-shows  then  then ;

\ ---- the windows' light: where the walls shade, no shadows ----
: window-light ( -- )
    151.9e 66.3e -63.6e  151.9e 66.3e -43.5e  151.9e 44.1e -63.6e  151.9e 44.1e -43.8e  light-doorway
    148e 97e -176e  175e 97e -176e  148e 97e -150e  175e 97e -150e  light-doorway
    109e 124e -176e  148e 97e -176e  109e 124e -150e  148e 97e -150e  light-doorway
    148e 101e -176e  175e 101e -176e  148e 97e -176e  175e 97e -176e  light-doorway
    109e 128e -176e  148e 101e -176e  109e 124e -176e  148e 97e -176e  light-doorway ;

\ ---- the movie replayed (the extras' theatre: its one scene here) ----
: replay-grate ( -- )
    fiona hold  night-specks  grate-movie scenes:unseen-movie grate-cutscene load-scene  panic-eased
    if  -1 run-scene  then  replay-over ;

\ ---- every frame ----
create cameras  7 , 0 , 0 ,   8 , 1 , 1 ,   9 , 2 , 2 ,   $A , 1 , 1 ,   $B , 1 , 1 ,   $C , 3 , -1 ,   \ (area, set, path)
create ahead  near-exit-2-area , 2 ,   near-exit-0-area , 0 ,   near-exit-2b-area , 2 ,   near-exit-1-area , 1 ,
create entries  2 , 2 ,   1 , 1 ,   3 , -1 ,                    \ (by exit 0, 1, 2: the camera)
: ways ( -- )
    wire-night? 0= if  0 exit-out  then  1 exit-out  to-exit-2-area 2 leaves-by
    cameras 6 cameras-at  ahead 4 ways-out ;
: each-frame ( -- )
    events-held state? if  window-light exit  then
    ways  grate-near  toward-the-cage  spots  window-light ;
: person-in ( i -- )
    fiona-p = if  events-held state? if  hewie send part-from-fiona  ['] replay-grate start drop exit  then
        entries 3 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went front-garden-2 place!
