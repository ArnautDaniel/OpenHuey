\ story/places/castle-1f-1.fs - Belli Castle's entrance hall ($01): the gate shut at first, the pond
\ where a splash goes up as someone steps in, the Refined Magnesia Hewie fetches from across it, the
\ crouch evasion tip, a creature let out once; back from the bookshelf's way (castle-1f-3), she and
\ Hewie come in here. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/castle-1f-1.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.castle-1f-1
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects story.kit fiona.anims hewie.anims ;

own-reset

\ ---- what is said here ----
0 constant too-dangerous-msg   \ "It's too dangerous to go back now..."
1 constant gate-shut-msg       \ "The gate is closed. There's no way through. I am going to have to find another way."
$40AA constant crouch-doc-msg  $AA constant crouch-doc   \ "'Crouch Evasion' In addition to evasion points..."

\ ---- its own state (a visit) ----
own: hewie-arrived          \ she and Hewie came in from the bookshelf's way (till the stalker is here)
own: splashed-1  own: splashed-2   \ each pool spot's splash made
own: pond-noticed           \ Hewie has barked at the pond

\ ---- the room's things ----
0 constant gate-open-part   1 constant gate-shut-part   0 constant gate-group
2 constant to-exit-2-area   3 constant near-exit-1-area   4 constant near-exit-0-area
5 constant near-exit-1b-area   6 constant near-exit-2-area   7 constant gate-area   $E constant tip-area
4 constant hall-track   1 constant hall-exit   $22A constant arrived-file
$28 constant arrival-sound   $2E constant go-for-it-call   0 constant go-for-it-gesture
: gate-open? ( -- flag )  c1f-2-flag1 fact? ;
: music-near ( -- )   \ (the hall's track, louder with exit 1's door open - till castle-1f-3's scene)
    c1f-3-flag1 fact? if  exit  then  hall-track  hall-exit exit-open? if  0.1e  else  0.01e  then  background ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop
    garden-door-done fact?  c1f-1-flag1 fact? 0= and if  c1f-1-flag1 fact!  1 0 443 4 8 creature-in  then
    music-near
    gate-open? if  gate-open-part show-part  else  gate-shut-part show-part  gate-group solid-floor floor-on  then
    c2f-3-flag1 fact? if
        $A $A $80 $80 $80 $20 rgba 103.4e 28.8e -51.9e specks   $10 $E $80 $80 $80 $50 rgba 83.3e 29e 14.8e specks
        $10 $E $80 $80 $80 $40 rgba -44.8e 13e -13e specks      $C $A $80 $80 $80 $40 rgba -87.5e 12.9e 36e specks
    else  $AC $60 $26 $50 $19 $1F $12 $80 room-tint  then ;

\ ---- in from the bookshelf's way (castle-1f-3): she and Hewie, the stage music on ----
: arrives ( -- )
    arrival-sound common-bank fiona sound-by  5 0 0 stage-music
    force-followed state!  yield  force-followed state-off  world-held state-off
    items-40-as-70 fact? 0= if  arrived-file -1 items send add-file  then
    15 screen:from-black with-music or fade  wait-fade
    items-40-as-70 fact? 0= if  $C $85 bank-sound  then ;
: by-the-bookshelf-way ( -- )
    c1f-3-bookshelf-examined fact?  c1f-1-flag2 fact? 0= and 0= if  exit  then
    0e sounds-scaled  0 -1 player-camera  fiona $B7 0 place-on-tri
    c1f-3-flag2 fact? if  1 306 0 hewie-to  hewie $132 0 place-on-tri  then
    c1f-1-flag2 fact!  -1 hewie-arrived !  ['] arrives start drop ;

\ ---- the Refined Magnesia across the pond: her "go for it" facing it - Hewie over, and back ----
: fetches ( -- )
    hewie hold  hewie wait-move  hewie bark-move 0 0  0e 0e 0e 0e  move  hewie wait-move
    hewie $1E0 -10 -1 walk-path-move -35.67e 31.26e go-to
    hewie true noclip  $1BD 45 -42.443e 1e 71.623e hewie-leap  hewie wait-move
    hewie $1BE 150 -1 walk-move -31.549e 70.055e go-to  hewie sit-a-while-anim 10 play-blend
    fiona-half-hidden state?  fiona held? 0= or if
        refined-magnesia item-full? 0= if  10 trust+  then
        refined-magnesia c1f-1-refined-magnesia -1 takes-up
    then
    hewie $1C0 176 -1 walk-move -42.94e 81.76e go-to
    $FE 45 -40.709e 1e 45.642e hewie-leap  hewie wait-move
    hewie false noclip  stalkers-stay state-off ;
: sends-him ( -- )
    stalkers-stay state!  fiona wait-move
    hewie held? if  stalkers-stay state-off exit  then
    ['] fetches start drop
    fiona go-for-it-gesture-anim start-anim  fiona 1 wait-mark  go-for-it-call common-bank fiona sound-by
    fiona wait-anim  fiona -1 idle  7 frames ;
: magnesia-way ( -- )
    c1f-1-refined-magnesia fact? if  exit  then
    hewie-in?  hewie-cond-now @ down-cond <> and  hewie held? 0= and  chased? 0= and 0= if  exit  then
    fiona-cmd-now @ go-for-it-gesture <> if  exit  then
    -40e -16e 60e point-seen?  fiona unseen? 0= and  hewie unseen? 0= and 0= if  exit  then   \ (the spot on the screen, they too)
    fiona 50 -40e 60e faces-point?  hewie-breakable-now @ and if  ['] sends-him start drop  then ;
: barks-at-pond ( -- )  -33.33e -16.93e 69.69e hewie-barks-at ;
create pond-look  -33.33e -16.93e 69.69e 40e 6e 6e look,
: hewie-pond ( -- )   \ (the magnesia there: once a visit he barks at it; he looks at it)
    c1f-1-refined-magnesia fact? if  exit  then
    pond-noticed @ 0=  hewie-in? and  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and
    hewie -33.33e -16.93e 69.69e 40e 6e centred-in? and if  -1 pond-noticed !  ['] barks-at-pond start drop  then
    pond-look 1 hewie-looks-at ;

\ ---- the pond: a splash as someone steps in (each spot once a visit) ----
: splash-by ( flag-addr F: x z -- )
    {: made F: x z :}  made @ if  exit  then
    fiona x -16e z 12e 4e steps-into? if  fiona 5 x -16e z splash-from  -1 made !  exit  then
    hewie x -16e z 12e 4e steps-into? if  hewie $A x -16e z splash-from  -1 made !  exit  then
    stalker x -16e z 12e 4e steps-into? if  stalker 5 x -16e z splash-from  -1 made !  then ;
: splashes ( -- )  splashed-1 -46e 70e splash-by  splashed-2 -23e 67e splash-by ;

\ ---- the crouch evasion tip (once: in the hall's crossing, free - not with the stalker near) ----
: crouch-tip ( -- )
    world-frozen state!  draw-when-frozen state!
    15 screen:to-dim fade wait-fade  crouch-doc-msg say  15 screen:from-dim fade wait-fade
    got-doc-crouch-evasion fact!  crouch-doc filed
    draw-when-frozen state-off  world-frozen state-off ;
: tip ( -- )
    got-doc-crouch-evasion fact?  fiona tip-area in-area? 0= or  fiona-free? 0= or if  exit  then
    chased?  fiona stalker 70 close-to? and if  exit  then
    got-doc-crouch-evasion fact!  panic-eased  ['] crouch-tip start drop ;

\ ---- what she may say here ----
: gate-shut ( -- )  fiona wait-move  fiona hands-clasped-anim 10 start-blend  gate-shut-msg say  fiona wait-anim ;
: spots ( -- )
    gate-open? 0=  fiona gate-area in-area? and  fiona -67 45 heading? and if  ['] gate-shut unhurried offer  then
    hall-exit exit-open? 0=  hewie-arrived @ and  fiona body-pos hall-exit 0 door-in-area? and if  too-dangerous-msg offer-remark-now  then ;

\ ---- every frame ----
create cameras  8 , 0 , -1 ,   9 , 1 , 0 ,   $A , 1 , 0 ,   $B , 2 , -1 ,   \ (area, set, path)
create entries  2 , -1 ,   0 , -1 ,   0 , -1 ,                 \ (by exit 0, 1, 2: the camera)
: ways ( -- )
    2 exits-out  to-exit-2-area 2 leaves-by  cameras 4 cameras-at  $C 0 -1 camera-out
    got-doc-crouch-evasion fact? 0= if  $D 3 -1 area-camera  then
    fiona near-exit-1-area entered?  gate-open? fiona near-exit-1b-area entered? and or if  1 prepare-exit  then
    fiona near-exit-0-area entered? if  0 prepare-exit  then
    gate-open?  fiona near-exit-2-area entered? and if  2 prepare-exit  then ;
: wall-shade ( -- )  -41.7e -6.2e 58e  -70.6e -6.2e 58e  -41.7e -18.6e 58e  -70.6e -18.6e 58e  light-doorway ;
: each-frame ( -- )
    hewie-arrived @  stalker-in? and if  0 hewie-arrived !  then
    ways  magnesia-way  hewie-pond  splashes  tip  music-near  spots  wall-shade ;
: person-in ( i -- )  fiona-p = if  entries 3 ways-in  by-the-bookshelf-way  then ;
: went ( -- )   \ (leaving: the stalker - down, out of sight since she came in with Hewie - taken; Hewie back by the pond)
    hewie-arrived @  stalker-in? 0= and  st-cond-now @ down-cond = and if  stalker let-go  2 summon-take  then
    hewie held? if  hewie let-go  hewie $FE 180 -40.709e 45.642e place-at  stalkers-stay state-off  then ;

' came-in ' person-in ' each-frame ' went castle-1f-1 place!
