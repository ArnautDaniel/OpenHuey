\ story/places/bridge.fs - the suspended bridge's hall in Belli Castle's 2F, as its three rooms have
\ it (castle-2f-13, -14, -15: the same hall at three times): the raised bridge and the statue in turn;
\ Hewie, pointed across the gap, leaps over to the Medallion and brings it back (it had: he lies down
\ over there); called back from over there he leaps back; the stalker by the plank grabbing him from
\ behind (then back across, or loose); his bark at the Medallion and his look up there; as she
\ leaves, Hewie over there lying low or put back on this side. Each room tells which triangles are
\ the far side's (`far-side!`) as it comes in.
IN: story.places.bridge
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects story.kit hewie.anims ;

\ ---- what is said here (the same in all three) ----
0 constant bridge-msg      \ "It's a suspended bridge."
1 constant lower-it-msg    \ "If I lower the bridge somehow, I can use it to cross."
3 constant statue-msg      \ "The body of a beast and the head of a bird... What a strange-looking statue."
4 constant gargoyles-msg   \ "Some ancient castles used to line their walls with gargoyles, as it was believed to..."

\ ---- the hall's things ----
1 constant medallion-fx   0 constant plank-floor   0 constant hewie-motions
4 constant bridge-area   $A constant statue-area   7 constant hewie-gap-area   8 constant his-plank-area
$16 constant hall-track   117 constant grabbed-act   118 constant over-there-act

\ ---- this room's far side: its triangles (the last the spot by the plank); the visit's own ----
variable far-1  variable far-2  variable far-spot
variable crossing          \ the stalker on the plank, its floor opened for him
variable bridge-remarked  variable statue-remarked  variable medallion-noticed
: far-side! ( tri tri spot -- )   \ (as the room comes in: its far side; the visit's remarks afresh)
    far-spot !  far-2 !  far-1 !  0 crossing !  0 bridge-remarked !  0 statue-remarked !  0 medallion-noticed ! ;
: over-the-gap? ( -- flag )   \ (Hewie on the far side or the plank)
    hewie body-tri  dup far-1 @ =  over far-2 @ = or  swap far-spot @ = or ;
: plank-open ( -- )  plank-floor solid-floor floor-off  plank-floor keeps-fiona-off keeps-pursuers-off or floor-on ;
: plank-shut ( -- )  plank-floor solid-floor floor-on  plank-floor keeps-fiona-off keeps-pursuers-off or floor-off ;
: hewie-waits-playing ( frames anim -- )  hewie send hewie-yelp ;   \ (out of sight, what he plays while the frames count)
: medallion-sparkle ( -- )  c2f-13-medallion fact? 0= if  medallion-fx -151.78e 97.684e -50.579e flicker-at  then ;

\ ---- Hewie over the gap: back across it (its plank opened for him) ----
: to-far-spot ( -- )  hewie far-spot @ -90 -1 walk-move -149e -51e go-to ;
: leaps-back ( -- )
    to-far-spot  plank-open  hewie $139 100 -195e 0.5e -51e hewie-leap  hewie wait-move  plank-shut ;
: called-back ( -- )  hewie hold  hewie wait-move  hewie-bark  hewie wait-move  leaps-back ;
: crosses-back ( -- )  hewie hold  hewie wait-move  leaps-back  0 crossing ! ;
: fetches ( -- )   \ (pointed across: over the gap to the Medallion, back with it - had before, he lies down over there)
    hewie hold  hewie hewie-motions motions-for  hewie-bark  hewie wait-move
    hewie $AE 0 -1 walk-path-move -152.54e -106.55e go-to  hewie use-motions  hewie true noclip
    hewie room-motion 5 start-blend8  hewie wait-anim  plank-floor keeps-fiona-off keeps-pursuers-off or floor-off  hewie false noclip
    c2f-13-medallion fact? if
        to-far-spot  hewie lie-down-anim play  hewie lie-anim play  5 lie-anim hewie-waits-playing  exit
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
: hewie-by-the-gap ( -- )   \ (each frame)
    hewie-in? 0=  hewie-cond-now @ down-cond = or  hewie held? or if  exit  then
    over-the-gap? if  over-there exit  then
    pointed-across?  hewie-breakable-now @ and if  ['] fetches start drop  then ;

\ ---- the bridge, the statue ----
: the-bridge ( -- )
    fiona wait-move  bridge-remarked @ if  lower-it-msg  else  -1 bridge-remarked !  bridge-msg  then  say ;
: the-statue ( -- )   \ (its remarks turn and turn about)
    fiona wait-move  fiona 90 turn
    statue-remarked @ if  0 statue-remarked !  gargoyles-msg  else  -1 statue-remarked !  statue-msg  then  say ;
: bridge-spots ( -- )
    fiona bridge-area in-area?  fiona 90 50 heading? and if  ['] the-bridge unhurried offer  then
    fiona statue-area in-area?  fiona 45 50 heading? and if  ['] the-statue unhurried offer  then ;

\ ---- Hewie's bark at the Medallion across the gap (once a visit), his look up there ----
create medallion-look  -149.92e 60e -50.71e  54e 36e 36e look,
: barks-across ( -- )  -149.92e 60e -50.71e hewie-barks-at ;
: at-his-command? ( -- flag )  hewie-in?  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and ;
: medallion-notice ( -- )
    c2f-13-medallion fact?  hewie hewie-gap-area in-area? 0= or  medallion-noticed @ or  at-his-command? 0= or if  exit  then
    hewie -149.92e 60e -50.71e 54e 36e centred-in? 0= if  exit  then
    -1 medallion-noticed !  ['] barks-across start drop ;

\ ---- Hewie coming in on the far side (not lying low there): put on this side; as she leaves ----
: hewie-comes-in ( -- )  over-there-act hewie-doing? 0=  over-the-gap? and if  hewie $13C -90 place-on-tri  then ;
: hewie-as-she-goes ( -- )   \ (over the gap: lying low there, or back on this side)
    hewie-in? 0= if  exit  then
    over-there-act hewie-doing? if
        over-the-gap? if  hewie far-spot @ -90 -149e -51e place-at  150 lie-anim hewie-waits-playing  then
    else  hewie 0 on-group?  over-the-gap? or if  hewie $13C put-on-tri  then  then ;

\ ---- climbing down to the great hall (castle-2f-14, -15: the balcony fallen) - Hewie left here ----
2 constant climb-msg       \ "If I choose my footing carefully, I should be able to get down this way... Climb down?..."
9 constant climb-area   0 constant climb-sound   $82 constant climb-way
: climbs-down ( -- )
    stalkers-stay state!  castle-1f-13 preload  fiona wait-move
    climb-msg yes? if
        15 screen:to-black-2 fade  wait-fade  world-held state!  scene-locked state!
        climb-sound $86 0e 0e 0e sound-there  180 frames
        hewie-in? if  room-id 288 0 hewie-to  then
        climb-way take-exit
    else  0 prepare-exit  stalkers-stay state-off  then ;
: climb-spot ( -- )  found-file-227 fact?  fiona climb-area in-area? and if  ['] climbs-down unhurried offer  then ;
