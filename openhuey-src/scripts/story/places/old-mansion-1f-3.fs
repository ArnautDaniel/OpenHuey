\ story/places/old-mansion-1f-3.fs - a room of the old mansion's 1F with a hopping figure (kind $1C):
\ it hops in place, calling now and then; struck, it hops back - the fifth time it lashes out (her
\ fright up, the stalker told to come); the medicine cart; the Medallion on the shelf, the Refined
\ Antimony Hewie sniffs out; the door unlocked from this side; brought in by the forced way, she lies
\ and gets up; Hewie sitting by one spot, wary of the figure; the doorways' shade. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-1f-3.fs (converted
\ from the game's, not loaded).
IN: story.places.old-mansion-1f-3
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names story.kit fiona.anims fiona.sounds ;

own-reset

0 constant cart-msg   \ "A cart with some medicine on it. There may be some medicine here that I can use, but al..."
own: spot-noticed  own: figure-noticed
0 constant medallion-fx   1 constant antimony-fx
0 constant lash-sound   1 constant hop-sound   2 constant call-sound
$D constant cart-area

\ ---- the figure (the kit's hopper) ----
$9002 $9003 lash-sound hop-sound  6.38e 0e -112.79e 170e  hopper: the-figure
: hopping-about ( -- )  the-figure true hops-about ;

\ ---- as she comes in ----
: antimony-shows ( -- )
    om1f-3-refined-antimony-found fact?  om1f-3-refined-antimony fact? 0= and if  antimony-fx -19.52e 1e -13.68e flicker-at  then ;
: came-in ( exit -- )
    drop  room-sounds  ['] hopping-about start drop  hunted state!
    om2f-3-flag1 fact? if
        hewie-in?  hewie 0 on-group? and if  hewie $13 put-on-tri  then
        0 show-part  0 barred-floor floor-on
    then
    om1f-3-medallion fact? 0= if  medallion-fx -10.64e 9.5e -116.64e flicker-at  then
    antimony-shows  1 $2300 reverb ;

\ ---- brought in by the forced way: she lies, then gets up ----
: wakes ( -- )
    fiona hold  fiona wait-move  fiona send recovery-reset  2 panic-to  3 stalker-order
    fiona lying-anim 0 start-blend  yield  15 screen:from-black with-music or fade
    fiona get-up-anim play  fiona -1 idle  7 frames  wait-fade ;

\ ---- what she may do here ----
: the-cart ( -- )
    fiona wait-move  fiona 2e -125e turn-to-xz  fiona look-low-anim start-anim  40 frames  cart-msg say ;
: take-medallion ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona -10.64e -116.64e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  medallion om1f-3-medallion medallion-fx takes-up  fiona reach-back-anim play ;
: take-antimony ( -- )  refined-antimony om1f-3-refined-antimony antimony-fx -19.52e -13.68e picks-up ;
: antimony-sniffed ( -- )  om1f-3-refined-antimony-found om1f-3-refined-antimony antimony-fx  -19.52e 1e -13.68e  hewie-finds ;
: unlocks-it ( -- )  0 unlocks ;
: sits-by ( -- )  -25.44e 0e -54.79e hewie-sits-by ;
: wary-of-it ( -- )  7.31e 0e -110.05e hewie-stands-by ;

create cameras  7 , 1 , 1 ,   8 , 3 , -1 ,   9 , 3 , -1 ,   $A , 0 , 0 ,   $B , 0 , 0 ,   $C , 2 , 2 ,   \ (area, set, path)
create entries  3 , -1 ,   1 , 1 ,   2 , 2 ,   \ (by exit 0..2: the camera)
create ahead  3 , 0 ,   5 , 0 ,   4 , 1 ,   6 , 2 ,   \ (area, exit loaded ahead)
create looks  7.31e 0e -110.05e  30e 6e 6e look,
: doorways-shade ( -- )
    -10e 12e -89e  6e 12e -89e  -10e 0e -89e  6e 0e -89e  light-doorway
    -37e 8e -83.5e  -31e 8e -83.5e  -37e 0e -83.5e  -31e 0e -83.5e  light-doorway
    -37e 17e -83.5e  -31e 17e -83.5e  -37e 8e -83.5e  -31e 8e -83.5e  light-doorway
    -40e 8e -83.5e  -34e 8e -83.5e  -40e 0e -83.5e  -34e 0e -83.5e  light-doorway
    -40e 17e -83.5e  -34e 17e -83.5e  -40e 8e -83.5e  -34e 8e -83.5e  light-doorway ;
: each-frame ( -- )
    3 exits-out  cameras 6 cameras-at  ahead 4 ways-out
    the-figure hopper-still dup 7 =  swap 47 = or if  call-sound hopper-sound  then
    om1f-3-refined-antimony-found fact? 0=  scrambling? and  hewie -19.52e 0e -13.68e 10e 5e touches? and if  ['] antimony-sniffed start-only  then
    spot-noticed 30 -25.44e 0e -54.79e 12e 10e notices? if  ['] sits-by start drop  then
    figure-noticed 80 7.31e 0e -110.05e 30e 6e notices? if  ['] wary-of-it start drop  then
    looks 1 danger-now @ followed = hewie-looks-if
    1 locked-request?  fiona 0 before-door? and if  ['] unlocks-it any-time offer  then
    fiona cart-area in-area?  fiona -140 60 heading? and if  ['] the-cart unhurried offer  then
    om1f-3-medallion fact? 0=  fiona -10.64e 9.5e -116.64e 5e 5e touches? and if  ['] take-medallion calm-only offer  then
    om1f-3-refined-antimony-found fact?  om1f-3-refined-antimony fact? 0= and  fiona -19.52e 1e -13.68e 5e 5e touches? and if
        ['] take-antimony calm-only offer
    then
    doorways-shade ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    entries 3 ways-in
    came-by @ forced-exit = if  fiona $1A 160 place-on-tri  1 1 player-camera  ['] wakes start drop  then ;
: went ( -- )  fiona 2 in-area? if  hopper-gone  then ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-3 place!
