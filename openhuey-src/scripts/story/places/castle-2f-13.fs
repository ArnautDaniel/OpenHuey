\ story/places/castle-2f-13.fs - the suspended bridge's hall in Belli Castle's 2F ($28), before the
\ great hall's balcony fell: out on the balcony a while (she, or the stalker after her), it gives -
\ to black, the doors set anew, down to the great hall by the forced way (with the crows; Hewie sent
\ there or kept here); the raised bridge in turn; Hewie, pointed across the gap, leaps over to the
\ Medallion and brings it back (it had: he lies down over there); called back he leaps back; the
\ stalker grabbing him from behind on the plank; the statue in turn, the sparkles, what Hewie barks
\ at, stands wary of and looks up at, the map's pages. A place (story/kit.fs). The original's
\ scripts, for checking: story/rooms-ref/castle-2f-13.fs (converted from the game's, not loaded).
\ (castle-2f-14 is the same hall after.)
IN: story.places.castle-2f-13
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects story.kit fiona.anims hewie.anims ;

own-reset

\ ---- what is said here ----
0 constant bridge-msg      \ "It's a suspended bridge."
1 constant lower-it-msg    \ "If I lower the bridge somehow, I can use it to cross."
3 constant statue-msg      \ "The body of a beast and the head of a bird... What a strange-looking statue."
4 constant gargoyles-msg   \ "Some ancient castles used to line their walls with gargoyles, as it was believed to..."

\ ---- its own state (a visit) ----
own: bridge-remarked  own: statue-remarked  own: medallion-noticed  own: wary-noticed
own: crossing              \ the stalker on the plank, its floor opened for him
own: balcony-gives         \ out on the balcony a while: it gives (once a visit)
own: her-frames  own: his-frames   \ (on the balcony so far)
own: was-idle              \ she stood idle as it gave

\ ---- the room's things ----
0 constant sparkle-fx   1 constant medallion-fx   0 constant plank-floor   0 constant hewie-motions
4 constant bridge-area   6 constant balcony-area   $A constant statue-area   7 constant hewie-gap-area   8 constant his-plank-area
$16 constant hall-track   117 constant grabbed-act   118 constant over-there-act
: over-the-gap? ( -- flag )   \ (Hewie on the far side or the plank)
    hewie body-tri  dup $1C3 =  over $1D0 = or  swap $1EA = or ;
: plank-open ( -- )  plank-floor solid-floor floor-off  plank-floor keeps-fiona-off keeps-pursuers-off or floor-on ;
: plank-shut ( -- )  plank-floor solid-floor floor-on  plank-floor keeps-fiona-off keeps-pursuers-off or floor-off ;
: hewie-waits-playing ( frames anim -- )  hewie send hewie-yelp ;   \ (out of sight, what he plays while the frames count)

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  hall-track 1e background
    c2f-13-flag1 fact? 0= if  sparkle-fx -231e 61e 24e flicker-at  then
    c2f-13-medallion fact? 0= if  medallion-fx -151.78e 97.684e -50.579e flicker-at  then ;

\ ---- the balcony gives: to black, the doors set anew, down to the great hall by the forced way ----
: stalker-kept ( -- )  stalker hold  begin  yield  again ;
: it-gives ( -- )
    30 frames  music send bgm-resume  15 screen:to-black-2 with-music or fade  wait-fade  world-held state!
    stalker-in? if  ['] stalker-kept start drop  then
    $35 $36 door-as  $36 closed-off  exits-redone ;
: down-with-it ( way hewie-room rest? -- )   \ (her on it - idle, she rests a moment - or the stalker)
    {: way hroom rest :}  stalkers-stay state!  stalkers-blind state!  hewie-no-attack state!  fiona hold
    castle-1f-13 preload  cast:crow 3 figure-in  cast:crow-2 4 figure-in  fiona wait-move
    rest if  fiona rest-anim start-anim  then
    it-gives
    hewie-in? if  hroom -1 0 hewie-to  then
    stalkers-blind state-off  hewie-no-attack state-off  way take-exit ;
: she-gives-way ( -- )  forced-exit castle-1f-13 was-idle @ down-with-it ;
: he-gives-way ( -- )   \ (the stalker after her: the camera on him)
    stalker 0 0 camera-set  stalker follow  forced-exit 1 or castle-2f-14 false down-with-it ;
: on-the-balcony ( -- )   \ (before the great hall's balcony fell: 30 frames out on it, she or he)
    found-file-227 fact?  balcony-gives @ or if  exit  then
    fiona balcony-area in-area? if  1 her-frames +!  else  0 her-frames !  then
    stalker balcony-area in-area? if  1 his-frames +!  else  0 his-frames !  then
    her-frames @ 30 = if
        -1 balcony-gives !  panic-eased  fiona-mode-now @ own-mode = was-idle !  ['] she-gives-way start drop  exit
    then
    his-frames @ 30 = if  -1 balcony-gives !  panic-eased  ['] he-gives-way start drop  then ;

\ ---- Hewie over the gap: back across it (its plank opened for him) ----
: leaps-back ( -- )
    hewie $1EA -90 -1 walk-move -149e -51e go-to  plank-open
    hewie $139 100 -195e 0.5e -51e hewie-leap  hewie wait-move  plank-shut ;
: called-back ( -- )  hewie hold  hewie wait-move  hewie-bark  hewie wait-move  leaps-back ;
: crosses-back ( -- )  hewie hold  hewie wait-move  leaps-back  0 crossing ! ;
: fetches ( -- )   \ (pointed across: over the gap to the Medallion, back with it - had before, he lies down over there)
    hewie hold  hewie hewie-motions motions-for  hewie-bark  hewie wait-move
    hewie $AE 0 -1 walk-path-move -152.54e -106.55e go-to  hewie use-motions  hewie true noclip
    hewie room-motion 5 start-blend8  hewie wait-anim  plank-floor keeps-fiona-off keeps-pursuers-off or floor-off  hewie false noclip
    c2f-13-medallion fact? if
        hewie $1EA -90 -1 walk-move -149e -51e go-to  hewie lie-down-anim play  hewie lie-anim play  5 lie-anim hewie-waits-playing  exit
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

\ ---- what she may do or say here ----
: the-bridge ( -- )
    fiona wait-move  bridge-remarked @ if  lower-it-msg  else  -1 bridge-remarked !  bridge-msg  then  say ;
: the-statue ( -- )   \ (its remarks turn and turn about)
    fiona wait-move  fiona 90 turn
    statue-remarked @ if  0 statue-remarked !  gargoyles-msg  else  -1 statue-remarked !  statue-msg  then  say ;
: spots ( -- )
    fiona bridge-area in-area?  fiona 90 50 heading? and if  ['] the-bridge unhurried offer  then
    fiona statue-area in-area?  fiona 45 50 heading? and if  ['] the-statue unhurried offer  then ;

\ ---- Hewie: wary on the balcony, his bark at the Medallion across the gap (each once a visit), his looks ----
create looks
    -149.92e 60e -50.71e  54e 36e 36e look,
    -228.31e 60e 25.69e  35e 10e 0e look,
: wary-on-balcony ( -- )  -231.29e 60e 30.58e hewie-stands-by ;
: barks-across ( -- )  -149.92e 60e -50.71e hewie-barks-at ;
: at-his-command? ( -- flag )  hewie-in?  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and ;
: hewie-notices ( -- )
    at-his-command? 0= if  exit  then
    wary-noticed @ 0=  hewie -231.29e 60e 30.58e 50e 20e centred-in? and if  -1 wary-noticed !  ['] wary-on-balcony start drop  then
    c2f-13-medallion fact? 0=  hewie hewie-gap-area in-area? and  medallion-noticed @ 0= and
    hewie -149.92e 60e -50.71e 54e 36e centred-in? and if  -1 medallion-noticed !  ['] barks-across start drop  then ;

\ ---- every frame ----
create cameras  2 , 2 , 2 ,   3 , 1 , 1 ,   $B , 0 , 0 ,   $C , 2 , 2 ,   \ (area, set, path)
: each-frame ( -- )
    0 exit-out  cameras 4 cameras-at
    fiona 2 entered? if  3 map-page  then   fiona 3 entered? if  4 map-page  then
    on-the-balcony  hewie-by-the-gap  hewie-notices  looks 2 hewie-looks-at  spots ;
: person-in ( i -- )
    case
        fiona-p of  0 0 0 way-in  endof
        hewie-p of  over-there-act hewie-doing? 0=  over-the-gap? and if  hewie $13C -90 place-on-tri  then  endof
    endcase ;
: went ( -- )   \ (Hewie left over the gap: lying low there, or back on this side)
    hewie-in? 0= if  exit  then
    over-there-act hewie-doing? if
        over-the-gap? if  hewie $1EA -90 -149e -51e place-at  150 lie-anim hewie-waits-playing  then
    else  hewie 0 on-group?  over-the-gap? or if  hewie $13C put-on-tri  then  then ;

' came-in ' person-in ' each-frame ' went castle-2f-13 place!
