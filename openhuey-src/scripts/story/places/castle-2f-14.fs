\ story/places/castle-2f-14.fs - the suspended bridge's hall in Belli Castle's 2F ($2F): the first time
\ in, the screen back (the crows' file); the raised bridge in turn; Hewie, pointed across the gap,
\ leaps over to the Medallion and brings it back (it had: he lies down over there); on the far side
\ called back, he leaps back; the stalker grabbing him from behind on the plank (then he comes back,
\ or he is loose); climbing down to the great hall (Hewie left here), the statue in turn, the
\ Medallion's sparkle, what Hewie barks at and looks up at, the map's pages. A place (story/kit.fs).
\ The original's scripts, for checking: story/rooms-ref/castle-2f-14.fs (converted from the game's,
\ not loaded).
IN: story.places.castle-2f-14
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects story.kit hewie.anims ;

own-reset

\ ---- what is said here ----
0 constant bridge-msg      \ "It's a suspended bridge."
1 constant lower-it-msg    \ "If I lower the bridge somehow, I can use it to cross."
2 constant climb-msg       \ "If I choose my footing carefully, I should be able to get down this way... Climb down?..."
3 constant statue-msg      \ "The body of a beast and the head of a bird... What a strange-looking statue."
4 constant gargoyles-msg   \ "Some ancient castles used to line their walls with gargoyles, as it was believed to..."

\ ---- its own state (a visit) ----
own: bridge-remarked  own: statue-remarked  own: hewie-noticed
own: crossing              \ the stalker on the plank, its floor opened for him

\ ---- the room's things ----
1 constant medallion-fx   0 constant plank-floor   0 constant hewie-motions   0 constant climb-sound
4 constant bridge-area   9 constant climb-area   $A constant statue-area   7 constant hewie-gap-area   8 constant his-plank-area
$16 constant hall-track   $82 constant climb-way   117 constant grabbed-act   118 constant over-there-act
create holes  $13 , $124 , $6B , $69 , $54 ,   \ (things lying about fall through)
: over-the-gap? ( -- flag )   \ (Hewie on the far side or the plank)
    hewie body-tri  dup $2B =  over $140 = or  swap $1DF = or ;
: plank-open ( -- )  plank-floor solid-floor floor-off  plank-floor keeps-fiona-off keeps-pursuers-off or floor-on ;
: plank-shut ( -- )  plank-floor solid-floor floor-on  plank-floor keeps-fiona-off keeps-pursuers-off or floor-off ;
: hewie-waits-playing ( frames anim -- )  hewie send hewie-yelp ;   \ (out of sight, what he plays while the frames count)

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  hall-track 1e background
    c2f-13-medallion fact? 0= if  medallion-fx -151.78e 97.684e -50.579e flicker-at  then
    5 0 do  holes i cells + @ hole-floor floor-tri-on  loop ;

\ ---- the first time in: the screen back, the crows' file ----
: first-in ( -- )
    yield  camera-restart  world-held state-off  15 screen:from-black with-music or fade
    found-file-227 fact!  0 prepare-exit  wait-fade  $227 give-file  stalkers-stay state-off ;

\ ---- Hewie over the gap: back across it (its plank opened for him) ----
: leaps-back ( -- )
    hewie $140 -90 -1 walk-move -149e -51e go-to  plank-open
    hewie $139 100 -195e 0.5e -51e hewie-leap  hewie wait-move  plank-shut ;
: called-back ( -- )  hewie hold  hewie wait-move  hewie-bark  hewie wait-move  leaps-back ;
: crosses-back ( -- )  hewie hold  hewie wait-move  leaps-back  0 crossing ! ;
: fetches ( -- )   \ (pointed across: over the gap to the Medallion, back with it - had before, he lies down over there)
    hewie hold  hewie hewie-motions motions-for  hewie-bark  hewie wait-move
    hewie $AE 0 -1 walk-path-move -152.54e -106.55e go-to  hewie use-motions  hewie true noclip
    hewie room-motion 5 start-blend8  hewie wait-anim  plank-floor keeps-fiona-off keeps-pursuers-off or floor-off  hewie false noclip
    c2f-13-medallion fact? if
        hewie $140 -90 -1 walk-move -149e -51e go-to  hewie lie-down-anim play  hewie lie-anim play  5 lie-anim hewie-waits-playing  exit
    then
    fiona-half-hidden state?  fiona held? 0= or if
        medallion item-full? 0= if  10 trust+  then
        medallion c2f-13-medallion medallion-fx takes-up
    then
    leaps-back ;
: grabbed ( -- )   \ (the stalker's grab from behind on the plank: after it, back across - or loose)
    grabbed-act 0 hewie-do  begin  grabbed-act hewie-doing? while  yield  repeat
    over-the-gap? if  ['] crosses-back start drop  exit  then
    plank-shut  0 crossing !  5 trust+ ;
: over-there ( -- )   \ (Hewie over the gap, free: called back; the stalker by the plank - his grab tried)
    started-now @ come-back-cmd = if  ['] called-back start drop  exit  then
    st-cond-now @ down-cond =  stalker his-plank-area in-area? 0= or  stalker-free? 0= or  crossing @ or if  exit  then
    plank-open  grabs-hewie? if  -1 crossing !  ['] grabbed start drop  else  plank-shut  then ;
: pointed-across? ( -- flag )   \ (her gesture up, facing the far side - out of the picture - both of them seen)
    chased? 0=  fiona-cmd-now @ 0= and  -163e 97e -50e point-unseen? and  fiona unseen? 0= and  hewie unseen? 0= and
    fiona 50 -163e -50e faces-point? and ;
: hewie-by-the-gap ( -- )
    hewie-in? 0=  hewie-cond-now @ down-cond = or  hewie held? or if  exit  then
    over-the-gap? if  over-there exit  then
    pointed-across?  hewie-breakable-now @ and if  ['] fetches start drop  then ;

\ ---- climbing down to the great hall (Hewie left here) ----
: climbs-down ( -- )
    stalkers-stay state!  castle-1f-13 preload  fiona wait-move
    climb-msg yes? if
        15 screen:to-black-2 fade  wait-fade  world-held state!  scene-locked state!
        climb-sound $86 0e 0e 0e sound-there  180 frames
        hewie-in? if  castle-2f-14 288 0 hewie-to  then
        climb-way take-exit
    else  0 prepare-exit  stalkers-stay state-off  then ;

\ ---- what she may do or say here ----
: the-bridge ( -- )
    fiona wait-move  bridge-remarked @ if  lower-it-msg  else  -1 bridge-remarked !  bridge-msg  then  say ;
: the-statue ( -- )   \ (its remarks turn and turn about)
    fiona wait-move  fiona 90 turn
    statue-remarked @ if  0 statue-remarked !  gargoyles-msg  else  -1 statue-remarked !  statue-msg  then  say ;
: spots ( -- )
    found-file-227 fact?  fiona climb-area in-area? and if  ['] climbs-down unhurried offer  then
    fiona bridge-area in-area?  fiona 90 45 heading? and if  ['] the-bridge unhurried offer  then
    fiona statue-area in-area?  fiona 45 50 heading? and if  ['] the-statue unhurried offer  then ;

\ ---- Hewie's bark at the Medallion across the gap (once a visit), his looks up there ----
create looks  -149.92e 60e -50.71e  54e 36e 36e look,
: barks-across ( -- )  -149.92e 60e -50.71e hewie-barks-at ;
: hewie-notices ( -- )
    c2f-13-medallion fact?  hewie hewie-gap-area in-area? 0= or  hewie-noticed @ or  hewie-in? 0= or if  exit  then
    hewie-cond-now @ well-cond <>  chased? or  hewie-cmdable-now @ 0= or if  exit  then
    hewie -149.92e 60e -50.71e 54e 36e centred-in? 0= if  exit  then
    -1 hewie-noticed !  ['] barks-across start drop ;

\ ---- every frame ----
create cameras  2 , 2 , 2 ,   3 , 1 , 1 ,   $B , 0 , 0 ,   $C , 2 , 2 ,   \ (area, set, path)
: each-frame ( -- )
    0 exit-out  cameras 4 cameras-at
    fiona 2 entered? if  3 map-page  then   fiona 3 entered? if  4 map-page  then
    hewie-by-the-gap  hewie-notices  looks 1 hewie-looks-at  spots ;
: person-in ( i -- )
    case
        fiona-p of
            0 0 0 way-in
            found-file-227 fact? 0= if
                fiona $4D -45 place-on-tri  hewie activate  hewie-in? if  hewie $120 -45 place-on-tri  then
                0 0 player-camera  camera-restart  ['] first-in start drop
            then  endof
        hewie-p of  over-there-act hewie-doing? 0=  over-the-gap? and if  hewie $13C -90 place-on-tri  then  endof
    endcase ;
: went ( -- )   \ (Hewie left over the gap: lying low there, or back on this side)
    hewie-in? 0= if  exit  then
    over-there-act hewie-doing? if
        over-the-gap? if  hewie $140 -90 -149e -51e place-at  150 lie-anim hewie-waits-playing  then
    else  hewie 0 on-group?  over-the-gap? or if  hewie $13C put-on-tri  then  then ;

' came-in ' person-in ' each-frame ' went castle-2f-14 place!
