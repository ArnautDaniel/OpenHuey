\ story/places/old-mansion-1f-20.fs - the long corridor of the old mansion's 1F: a hopping figure (the
\ kit's hopper; struck the fifth time it lashes out at her - thrown down, her panic up); the rotted
\ door (its knob gone; kicked in, the way through); the scales; the Weeping Stone; three Medallions
\ and the Prima Magnesia Hewie sniffs out; after the 3F's turn, stepping in the first time, she is
\ taken out by the forced way; Hewie wary of the figure, his look; the map's pages. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-1f-20.fs
\ (converted from the game's, not loaded).
IN: story.places.old-mansion-1f-20
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names effects story.kit fiona.anims fiona.sounds ;

own-reset

2 constant knob-msg     \ "The knob has rotted off. I can't open the door like this."
3 constant rotted-msg   \ "The door is completely rotted through and through. A good kick should knock the doo..."
4 constant scales-msg   \ "A pair of scales lies in the center of the corridor. A balance in a corridor... Just on..."
own: knob-remarked  own: call-frames  own: figure-noticed  own: taken-out
0 constant stone-fx   1 constant medallion-fx   2 constant medallion-2-fx   3 constant medallion-3-fx   4 constant magnesia-fx
0 constant lash-sound   1 constant hop-sound   2 constant first-call   5 constant break-sound
$900E constant lashing   $64 constant rotted-door
$900C $900D lash-sound hop-sound  12e 0e 0e -90e  hopper: the-figure

\ ---- the figure: struck it hops back - the fifth time it lashes out at her ----
: lashes-out ( -- )   \ (her in its reach: thrown down, her panic up)
    hopper-slot lashing 0 3 figure-play  16 frames  0 the-figure hp.hops !
    fiona 7e 11e 0e 5e 10e touches? if
        lash-sound hopper-sound  hopper-slot figure true fiona send throw-down
        panic-stage @ 3 = if  4  else  3  then  panic-to
    then
    hopper-slot figure-wait ;
: jumps-back ( h -- )
    {: a :}  0 a hp.still !  1 a hp.hops +!
    a hp.hops @ 3 = if  33 chance? if  5 a hp.hops !  then  then
    a hp.hops @ 4 = if  50 chance? if  5 a hp.hops !  then  then
    hop-sound hopper-sound  shove-hit-sound common-bank fiona sound-by
    hopper-slot a hp.back @ 0 3 figure-play  hopper-slot figure-wait
    a hp.hops @ 5 = if  lashes-out  then
    hopper-slot a hp.hop @ 0 3 figure-play  0 a hp.still ! ;
: hopping-about ( -- )
    the-figure true hopper-in
    begin
        the-figure hopper-struck? if  the-figure jumps-back  then
        hopper-slot figure-came-round? if  hopper-slot the-figure hp.hop @ 0 0 figure-play  0 the-figure hp.still !
        else  1 the-figure hp.still +!  then
        yield
    again ;

\ ---- the rotted door: kicked in ----
: kicked-in ( -- )
    break-sound room-bank fiona sound-by  0 hide-part  1 show-part  0 barred-floor floor-off  rotted-door unlock
    om1f-20-breakable-broken fact!
    $80303030 2 6  -53e 52e 357e  0.25e 0.5e 0.25e 0.5e  0e shards
    $80404040 2 6  -50e 50e 357e  0.25e 0.5e 0.25e 0.5e  0e shards
    $80303030 2 6  -47e 50e 357e  0.25e 0.5e 0.25e 0.5e  0e shards
    -55e 55e 357e dust  -50e 52e 357e dust  -45e 54e 357e dust ;
: the-door ( -- )
    fiona wait-move  fiona 0 turn
    knob-remarked @ if  0 knob-remarked !  rotted-msg  else  -1 knob-remarked !  knob-msg  then  say ;

\ ---- after the 3F's turn, the first time in: taken out by the forced way ----
: taken-away ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move  15 screen:to-black-2 with-music or fade  wait-fade
    world-held state!  scene-locked state!  hewie let-go  0 prepare-exit  stalkers-stay state-off  forced-exit take-exit ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  ['] hopping-about start drop  hunted state!
    om1f-20-breakable-broken fact? if  1 show-part  else  0 show-part  0 barred-floor floor-on  then
    om1f-20-weeping-stone fact? 0= if  stone-fx 12.3e 8.5e 177.61e flicker-at  then
    om1f-20-medallion-found fact?  om1f-20-medallion fact? 0= and if  medallion-fx 0.34e 1e 62e flicker-at  then
    om1f-20-medallion-sniffed-2 fact?  om1f-20-medallion-2 fact? 0= and if  medallion-2-fx 13.99e 1e 94.5e flicker-at  then
    om1f-20-medallion-sniffed-3 fact?  om1f-20-medallion-3 fact? 0= and if  medallion-3-fx -13.99e 1e 126.99e flicker-at  then
    om1f-20-prima-magnesia-found fact?  om1f-20-prima-magnesia fact? 0= and if  magnesia-fx -42.23e 51e 236.98e flicker-at  then
    1 $2300 reverb ;

\ ---- what she may do or say here ----
: the-scales ( -- )  fiona wait-move  fiona 0.5e 56.7e turn-to-xz  scales-msg say ;
: take-stone ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona 12.3e 177.61e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  weeping-stone om1f-20-weeping-stone stone-fx takes-up  fiona reach-back-anim play ;
: take-medallion ( -- )  medallion om1f-20-medallion medallion-fx 0.34e 62e picks-up ;
: take-medallion-2 ( -- )  medallion om1f-20-medallion-2 medallion-2-fx 13.99e 94.5e picks-up ;
: take-medallion-3 ( -- )  medallion om1f-20-medallion-3 medallion-3-fx -13.99e 126.99e picks-up ;
: take-magnesia ( -- )  prima-magnesia om1f-20-prima-magnesia magnesia-fx -42.23e 236.98e picks-up ;
: medallion-sniffed ( -- )  om1f-20-medallion-found om1f-20-medallion medallion-fx  0.34e 1e 62e  hewie-finds ;
: medallion-2-sniffed ( -- )  om1f-20-medallion-sniffed-2 om1f-20-medallion-2 medallion-2-fx  13.99e 1e 94.5e  hewie-finds ;
: medallion-3-sniffed ( -- )  om1f-20-medallion-sniffed-3 om1f-20-medallion-3 medallion-3-fx  -13.99e 1e 126.99e  hewie-finds ;
: magnesia-sniffed ( -- )  om1f-20-prima-magnesia-found om1f-20-prima-magnesia magnesia-fx  -42.23e 51e 236.98e  hewie-finds ;
: wary-of-it ( -- )  11.58e 0e -0.41e hewie-stands-by ;

create cameras  $A , 0 , 0 ,   $B , 2 , 2 ,   $C , 2 , 2 ,   $D , 3 , 3 ,   $E , 0 , 0 ,   $F , 1 , 1 ,   \ (area, set, path)
create entries  2 , 2 ,   1 , 1 ,   3 , 3 ,   0 , 0 ,   \ (by exit 0..3: the camera)
create ahead  4 , 3 ,   6 , 3 ,   5 , 0 ,   8 , 0 ,   7 , 1 ,   9 , 2 ,   \ (area, exit loaded ahead)
create looks  11.58e 0e -0.41e  23e 16e 16e look,
: sniffed ( -- )
    scrambling? 0= if  exit  then
    om1f-20-medallion-found fact? 0=  hewie 0.34e 0e 62e 10e 5e touches? and if  ['] medallion-sniffed start-only  then
    om1f-20-medallion-sniffed-2 fact? 0=  hewie 13.99e 0e 94.5e 10e 5e touches? and if  ['] medallion-2-sniffed start-only  then
    om1f-20-medallion-sniffed-3 fact? 0=  hewie -13.99e 0e 126.99e 10e 5e touches? and if  ['] medallion-3-sniffed start-only  then
    om1f-20-prima-magnesia-found fact? 0=  hewie -42.23e 50e 236.98e 10e 5e touches? and if  ['] magnesia-sniffed start-only  then ;
: pick-ups ( -- )
    om1f-20-weeping-stone fact? 0=  fiona 12.3e 8.5e 177.61e 5e 5e touches? and if  ['] take-stone calm-only offer  then
    om1f-20-medallion-found fact?  om1f-20-medallion fact? 0= and  fiona 0.34e 0e 62e 5e 5e touches? and if  ['] take-medallion calm-only offer  then
    om1f-20-medallion-sniffed-2 fact?  om1f-20-medallion-2 fact? 0= and  fiona 13.99e 0e 94.5e 5e 5e touches? and if  ['] take-medallion-2 calm-only offer  then
    om1f-20-medallion-sniffed-3 fact?  om1f-20-medallion-3 fact? 0= and  fiona -13.99e 0e 126.99e 5e 5e touches? and if  ['] take-medallion-3 calm-only offer  then
    om1f-20-prima-magnesia-found fact?  om1f-20-prima-magnesia fact? 0= and  fiona -42.23e 50e 236.98e 5e 5e touches? and if  ['] take-magnesia calm-only offer  then ;
: each-frame ( -- )
    over-music-c fact? if  0 exit-out  then  1 exit-out  2 2 leaves-by  3 3 leaves-by
    cameras 6 cameras-at  ahead 6 ways-out
    fiona $11 entered? if  2 map-page  then  fiona $11 left? if  1 map-page  then
    om1f-20-breakable-broken fact? 0=  -50e 50e 355e 6e 12e her-blow-in? and if  kicked-in  then
    the-figure call-frames first-call hopper-calls
    figure-noticed 80 11.58e 0e -0.41e 23e 16e notices? if  ['] wary-of-it start drop  then
    looks 1 danger-now @ followed = hewie-looks-if
    sniffed
    om3f-1-flag2 fact?  over-music-c fact? 0= and  taken-out @ 0= and  fiona 0 entered? and if
        -1 taken-out !  panic-eased  ['] taken-away start drop
    then
    om1f-20-breakable-broken fact? 0=  fiona -50e 50e 355e 6e 12e touches? and  fiona 0 50 heading? and if  ['] the-door unhurried offer  then
    fiona $10 60 faces-area? if  ['] the-scales unhurried offer  then
    pick-ups ;
: person-in ( i -- )  fiona-p = if  entries 4 ways-in  came-by @ 2 = if  2 map-page  then  then ;
: went ( -- )  hopper-gone ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-20 place!
