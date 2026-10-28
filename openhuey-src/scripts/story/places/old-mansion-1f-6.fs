\ story/places/old-mansion-1f-6.fs - the caged hall of the old mansion's 1F: a hopping figure (the
\ kit's hopper, its calls); before the 3F's turn, coming in by the far way, the scene's figure there
\ - as she steps further in it turns round and walks off, and once she is by the cage a homunculus
\ drops in in its place; a hiding place (the kit's; the stalker he-searches it once while she hides
\ - he finds her by the odds); the sheet over the thing; the cage; two candles (their flames stirred
\ by whoever passes); the Medallion; Hewie wary of the figure, his look; the map's pages; the
\ doorway's shade. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/old-mansion-1f-6.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.old-mansion-1f-6
USING: engine game-state actors messages common vectors facts room-names flag-names story-names stalker-names
    items.names story.kit fiona.anims ;

own-reset

0 constant sheet-msg   \ "There's a sheet draped over this... thing. Why are they trying to hide everything away..."
1 constant cage-msg    \ "It's an enormous cage. What in the world could they have been keeping in here?"
own: call-frames  own: figure-noticed  own: in-further  own: searched  own: search-started
0 constant medallion-fx   1 constant flame-fx   2 constant flame-2-fx
0 constant lash-sound   1 constant hop-sound   2 constant first-call
0 constant her-motions   1 constant daniella-motions   $22 constant search-var   $F constant hide-area
4 constant walker-slot   $200 constant walking
$9006 $9007 lash-sound hop-sound  20e 0e -105e 0e  hopper: the-figure

\ ---- the figure (the kit's hopper) ----
: hopping-about ( -- )  the-figure true hops-about ;

\ ---- the scene's figure by the far way: turning round as she steps in, then walking off ----
: turned-to ( F: x y z yaw -- )  {: F: x y z yaw :}  walker-slot x y z yaw rad>deg figure-at ;
: walker-yaw ( turn -- ) ( F: -- yaw )  s>f 0.1e f*  pi fswap f- ;   \ (from facing away, 0.1 radian more for each)
: walker-turns ( turn -- )  walker-slot figure body-at vec@  walker-yaw 1.57e fmax  turned-to ;   \ (to a quarter turn at least)
: walker-x ( -- ) ( F: -- x )  walker-slot figure body-at v.x sf@ ;
: walks-off ( -- )
    begin  in-further @ 0= while  yield  repeat
    walker-slot walking 1 4 figure-play  0
    begin  dup walker-turns  dup walker-yaw 1.57e f> while  2 +  yield  repeat  drop
    begin  walker-x -84e f> 0= while  yield  repeat ;
: walker-in ( -- )   \ (loaded ahead by the room before: there, facing away)
    walker-slot figure 0< if  exit  then
    walker-slot figure-on  walker-slot -97e 0e 84e 180e figure-at  walker-slot 0 1 0 figure-play
    ['] walks-off start drop ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  ['] hopping-about start drop  hunted state!
    om1f-6-medallion fact? 0= if  medallion-fx -119.67e 9e 151.64e flicker-at  then
    flame-fx 0 -122.2e 13.1e 139e flame-at  flame-2-fx 0 -120.5e 12.8e 140.35e flame-at
    1 $2300 reverb ;
: walker-due? ( -- flag )  om3f-1-flag2 fact? 0=  om1f-6-flag1 fact? 0= and ;

\ ---- the hiding place (the kit's); the stalker's search - once while she hides ----
: hides ( -- )  0 searched !  fiona -1 look-at  her-motions $92 -90 4 -110.16e 146.09e hides-in ;
: her-looker? ( -- flag )  st-kind-now @ dup daniella-kind = swap kind34-kind = or ;
: looks-in ( -- )
    stalker hold  stalker wait-move
    search-started @ 0=  stalker 0 exit-area in-area? or if  0 door-for-him  then  0 search-started !
    her-looker? if  stalker daniella-motions motions-for  then
    stalker $20 -90 -100e 146.09e walk-to
    her-looker? if  stalker use-motions  1 her-back  stalker room-motion play  0 her-back
    else  stalker stalker.anims:there-anim play  then
    -1 found-her !  stalker 10 idle  begin  fiona held? while  yield  repeat  0 stalker-order ;
: he-searches ( -- )
    -1 searched !  -1 came-while-hidden !
    search-var mansion-odds chance?  2 creature-moving? or if  ['] looks-in start drop
    else  120 summoner-sooner  0 search-started !  then
    0 stalker-alert? if  2 stalker-order  then
    search-var var@ 1+ search-var var! ;
: stalker-came ( -- )  came-on-her? if  he-searches  then ;
: search-due ( -- )   \ (he is here, she hides: he he-searches, once)
    st-cond-now @ down-cond =  fiona-hidden state? 0= or  searched @ or  stalker-in? 0= or  stalker held? or if  exit  then
    -1 search-started !  he-searches ;
: stalker-busy? ( -- flag )  stalker-in?  st-mode-now @ 2 u<  stalker unseen? 0= or  and ;

\ ---- what she may do or say here ----
: take-medallion ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona -119.67e 151.64e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  medallion om1f-6-medallion medallion-fx takes-up  fiona reach-back-anim play ;
: the-sheet ( -- )  fiona wait-move  fiona 20e -120e turn-to-xz  sheet-msg say ;
: the-cage ( -- )  fiona wait-move  fiona 90 turn  fiona look-close-anim start-anim  40 frames  cage-msg say  fiona wait-anim ;
: wary-of-it ( -- )  20.8e 0e -104.26e hewie-stands-by ;

create cameras  4 , 0 , -1 ,   5 , 1 , 0 ,   6 , 1 , 0 ,   7 , 2 , -1 ,   8 , 2 , -1 ,   9 , 3 , -1 ,   \ (area, set, path)
create entries  0 , -1 ,   3 , -1 ,   \ (by exit 0, 1: the camera)
create ahead  2 , 0 ,   3 , 1 ,   \ (area, exit loaded ahead)
create looks  20.8e 0e -104.26e  30e 15e 15e look,
: the-walker ( -- )
    walker-due? 0= if  exit  then
    fiona $B entered? if  -1 in-further !  then
    fiona $A entered? if
        walker-slot figure-out  old-mansion-1f-6 0 82 $80 8 $B4 0e creature-dropped  om1f-6-flag1 fact!
    then ;
: each-frame ( -- )
    0 exit-out  1 1 leaves-by  cameras 6 cameras-at  ahead 2 ways-out
    fiona $E entered? if  1 map-page  then  fiona $E left? if  2 map-page  then
    the-walker  search-due
    the-figure call-frames first-call hopper-calls
    figure-noticed 80 20.8e 0e -104.26e 30e 15e notices? if  ['] wary-of-it start drop  then
    looks 1 danger-now @ followed = hewie-looks-if
    flame-fx -122.2e 13.1e 139e 8e -4e flame-stirred  flame-2-fx -120.5e 12.8e 140.35e 8e -4e flame-stirred
    om1f-6-medallion fact? 0=  fiona -119.67e 9e 151.64e 5e 5e touches? and if  ['] take-medallion calm-only offer  then
    fiona hide-area in-area?  fiona -45 60 heading? and if
        stalker-busy? if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then
    then
    fiona $C 60 faces-area? if  ['] the-sheet unhurried offer  then
    fiona $D in-area?  fiona 45 60 heading? and if  ['] the-cage unhurried offer  then
    cam-set@ 4 <> if  -111.5e 18e 114e  -94e 18e 93e  -111.5e 0e 114e  -94e 0e 93e  light-doorway  then ;
: person-in ( i -- )
    case
        fiona-p of
            entries 2 ways-in
            came-by @ 1 = if
                2 map-page  walker-due? if  walker-in  then
            else  walker-due? if  om1f-6-flag1 fact!  then  then
        endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- )  hopper-gone ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-6 place!
