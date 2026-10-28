\ story/places/old-mansion-2f-5.fs - a hall of the old mansion's 2F with a hopping figure (kind
\ $1C): it hops in place, calling as it comes round; struck, it hops back - the fifth time it lashes
\ out (her fright up, the stalker told to come); a hiding place (the kit's; the stalker's search -
\ he finds her by the odds); two vases (the Medallion in one); after the 3F's turn three homunculi
\ dropped in, once; the Viride Medallion Hewie sniffs out; Hewie wary of the figure, his look. A
\ place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-2f-5.fs in
\ the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.old-mansion-2f-5
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit fiona.anims fiona.sounds ;

own-reset

own: figure-noticed
0 constant medallion-fx   1 constant viride-fx
0 constant lash-sound   1 constant hop-sound   2 constant call-sound
0 constant her-motions   1 constant daniella-motions   $15 constant search-var   6 constant hide-area   6 constant far-exit
om2f-5-breakable1-broken 0 1  -10.3e 0e -93.8e 5e 8e  breakable: far-vase
far-vase $80202030 0 3 shard-look  far-vase 0e 0.5e 0.312e 0.187e shard-uv  far-vase 0 frees-floor  far-vase $E1 noise-from
om2f-5-breakable2-broken 2 3  10.5e 0e -28e 5e 8e  breakable: near-vase
near-vase $80202030 0 3 shard-look  near-vase 0e 0.5e 0.312e 0.187e shard-uv  near-vase 1 frees-floor  near-vase $1AD noise-from

\ ---- the figure (the kit's hopper) ----
$9004 $9005 lash-sound hop-sound  -3e 0e 43e -45e  hopper: the-figure
: hopping-about ( -- )  the-figure true hops-about ;

\ ---- as she comes in ----
: medallion-shows ( -- )  om2f-5-medallion fact? 0= if  medallion-fx 10.5e 1e -28e flicker-at  then ;
: came-in ( exit -- )
    drop  room-sounds  far-vase as-it-stands  near-vase as-it-stands  near-vase broken? if  medallion-shows  then
    ['] hopping-about start drop  hunted state!
    om3f-1-flag2 fact?  om2f-5-flag1 fact? 0= and if
        om2f-5-flag1 fact!
        old-mansion-2f-5 0 291 $81 3 0 0e creature-dropped  old-mansion-2f-5 0 426 $81 3 0 0e creature-dropped
        old-mansion-2f-5 0 205 $81 3 0 0e creature-dropped
    then
    om2f-5-viride-medallion-found fact?  om2f-5-viride-medallion fact? 0= and if  viride-fx 7.38e 1e 20.58e flicker-at  then
    1 $2300 reverb ;

\ ---- the hiding place (the kit's); the stalker's search ----
: hides ( -- )  her-motions $106 180 2 -4.6e -231.1e hides-in ;
: her-looker? ( -- flag )  st-kind-now @ dup daniella-kind = swap kind34-kind = or ;
: looks-in ( -- )
    stalker hold  stalker wait-move  0 door-for-him  far-exit door-for-him
    her-looker? if  stalker daniella-motions motions-for  then
    stalker $106 180 -4.6e -222e walk-to
    her-looker? if  stalker use-motions  1 her-back  stalker room-motion play  0 her-back
    else  stalker stalker.anims:there-anim play  then
    -1 found-her !  stalker 10 idle  begin  fiona held? while  yield  repeat  0 stalker-order ;
: stalker-came ( -- )
    came-on-her? 0= if  exit  then
    search-var mansion-odds chance?  2 creature-moving? or if  ['] looks-in start drop  else  120 summoner-sooner  then
    search-var var@ 1+ search-var var! ;

\ ---- what she may take here ----
: take-medallion ( -- )  medallion om2f-5-medallion medallion-fx 10.5e -28e picks-up ;
: take-viride ( -- )  viride-medallion om2f-5-viride-medallion viride-fx 7.38e 20.58e picks-up ;
: viride-sniffed ( -- )  om2f-5-viride-medallion-found om2f-5-viride-medallion viride-fx  7.38e 1e 20.58e  hewie-finds ;
: wary-of-it ( -- )  -2.18e 0e 42.94e hewie-stands-by ;

create cameras  4 , 0 , 0 ,   5 , 1 , -1 ,   \ (area, set, path)
create ahead  2 , 0 ,   3 , 6 ,   \ (area, exit loaded ahead)
create looks  -2.18e 0e 42.94e  32e 11e 11e look,
: each-frame ( -- )
    0 exit-out  far-exit exit-out  cameras 2 cameras-at  ahead 2 ways-out
    far-vase breaks? drop
    the-figure hopper-still 1 = if  call-sound hopper-sound  then
    figure-noticed 80 -2.18e 0e 42.94e 32e 11e notices? if  ['] wary-of-it start drop  then
    looks 1 danger-now @ followed = hewie-looks-if
    om2f-5-viride-medallion-found fact? 0=  scrambling? and  hewie 7.38e 0e 20.58e 10e 5e touches? and if  ['] viride-sniffed start-only  then
    fiona hide-area in-area?  fiona 90 60 heading? and if
        stalker-in? if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then
    then
    near-vase broken? if
        om2f-5-medallion fact? 0=  fiona 10.5e 0e -28e 5e 5e touches? and if  ['] take-medallion calm-only offer  then
    else  near-vase breaks? if  medallion-shows  then  then
    om2f-5-viride-medallion-found fact?  om2f-5-viride-medallion fact? 0= and  fiona 7.38e 0e 20.58e 5e 5e touches? and if
        ['] take-viride calm-only offer
    then ;
: person-in ( i -- )
    case
        fiona-p of  0 0 0 way-in  far-exit 1 -1 way-in  endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- )  hopper-gone ;

' came-in ' person-in ' each-frame ' went old-mansion-2f-5 place!
