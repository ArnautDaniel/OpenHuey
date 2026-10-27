\ story/places/old-mansion-2f-10.fs - the old mansion's 2F room of the bull statues: the vase (broken by
\ a blow: the Weeping Stone in it), the two statues' remark in turn, the flame and its draught; once the
\ 1F's way is open, a creature let out next door; Hewie's looks. A place (story/kit.fs). The original's
\ scripts, for checking: story/rooms-ref/old-mansion-2f-10.fs (converted from the game's, not loaded).
IN: story.places.old-mansion-2f-10
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags items.names story.kit fiona.anims ;

own-reset

0 constant statue-msg   \ "A bronze statue with a cow-like head. My Greek mythology is kind of rusty, but I th..."
1 constant alive-msg    \ "It looks like it could come to life at any second."
own: statue-remarked
0 constant stone-fx   1 constant flame-fx
om2f-10-breakable-broken 0 1  -10.5e 0e -0.4e 5e 8e  breakable: the-vase
the-vase $80404040 0 2 shard-look  the-vase 0.812e 0.687e 0.187e 0.312e shard-uv
the-vase 0 frees-floor  the-vase $7B noise-from

: stone-shows ( -- )  om2f-10-weeping-stone fact? 0= if  stone-fx -10.5e 1e -0.4e flicker-at  then ;
: came-in ( exit -- )
    drop  the-vase as-it-stands  the-vase broken? if  stone-shows  then
    om1f-12-flag1 fact?  om2f-10-flag1 fact? 0= and if  om2f-10-flag1 fact!  $5F 0 259 4 8 creature-in  then
    flame-fx 1 12.4e 17.3e -45e flame-at  1 $2300 reverb ;
: take-stone ( -- )  weeping-stone om2f-10-weeping-stone stone-fx -10.5e -0.4e picks-up ;
: statue ( F: x z -- )
    fiona wait-move  fiona turn-to-xz  fiona look-at-anim start-anim  40 frames
    statue-remarked @ if  alive-msg  else  -1 statue-remarked !  statue-msg  then  say  fiona wait-anim ;
: statue-1 ( -- )  -67.5e -11.5e statue ;
: statue-2 ( -- )  -67.5e 11.5e statue ;
create looks-calm  5.8e 0e 15.45e  15e 13e 13e look,
create looks  -65.87e 0e 11.35e  11e 15e 15e look,   -66.99e 0e -11.47e  11e 15e 15e look,
create cameras  6 , 0 , 0 ,   7 , 1 , 1 ,   8 , 0 , 0 ,   9 , 2 , 2 ,   \ (area, set, path)
create entries  2 , 2 ,   1 , 1 ,   0 , 0 ,   \ (by exit 0..2: the camera)
create ahead  3 , 0 ,   4 , 1 ,   5 , 2 ,   \ (area, exit loaded ahead)
: doorways-shade ( -- )
    -13.7e 15e -28e  -13.7e 15e -11.3e  -13.7e 0e -28e  -13.7e 0e -11.3e  light-doorway
    -13.7e 15e 14e  -13.7e 15e 30.7e  -13.7e 0e 14e  -13.7e 0e 30.7e  light-doorway ;
: each-frame ( -- )
    3 exits-out  cameras 4 cameras-at  ahead 3 ways-out
    looks-calm 1 hewie-looks-at  looks 2 hewie-looks-unchased
    flame-fx 12.4e 17.3e -45e 8e -4e flame-stirred
    the-vase broken? if
        om2f-10-weeping-stone fact? 0=  fiona -10.5e 1e -0.4e 5e 5e touches? and if  ['] take-stone calm-only offer  then
    else  the-vase breaks? if  stone-shows  then  then
    fiona -67.5e -11.5e 8e centre-across?  fiona 60 -67e -11e faces-point? and if  ['] statue-1 unhurried offer  then
    fiona -67.5e 11.5e 8e centre-across?  fiona 60 -67e 11e faces-point? and if  ['] statue-2 unhurried offer  then
    doorways-shade ;
: person-in ( i -- )  fiona-p = if  entries 3 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-2f-10 place!
