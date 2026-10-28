\ story/places/water-tower-2f-1.fs - the Water Tower's 2F ($C2): the walkway (the object move00)
\ turned by its lever from one landing to the other - Hewie on it brought along; Riccardo heard
\ prowling above, then come for her (once, by her way up; after, 1800 frames after each time he
\ left); taken off as she comes in, and when his time is up and she can't see him; the summoner off
\ while she's here; the crate (a creature let out); the Medallion Hewie sniffs out; the Refined
\ Antimony; Hewie's bark at the walkway's far end (till it first turns); the map's pages. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/water-tower-2f-1.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.water-tower-2f-1
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit story.places.walkway fiona.anims ;

own-reset  walkway-owns owns !

own: summoner-was-on       \ the summoner was on as she came in (on again as she leaves)
own: prowl-begun           \ she left the landing (area $D): Riccardo heard above
own: return-due            \ Riccardo gone: his coming back counted down
own: return-wait           \ ... the frames left
own: hewie-noticed         \ Hewie barked at the walkway's far end (once a visit)
own: hewie-aboard          \ Hewie on the walkway's floor last frame - for leaving (the next room's floor is in by then)
0 constant medallion-fx   1 constant antimony-fx
$27 constant growl-sound   0 constant step-sound   1 constant step-2-sound   \ (his kind's sounds)
4 constant crate-part   5 constant crate-broken-part
$C constant lever-area   $D constant landing-area   $E constant way-up-area   5 constant top-area   7 constant stairs-area
$A constant hewie-far-area   $F constant map-2-area   $10 constant map-3-area
1800 constant return-frames

\ ---- the walkway: to 90 degrees, at 152; its edge 45 out; the lever at $13A; Hewie's spots ----
: edge ( F: rad -- x z )  fdup fcos -45e f*  fswap fsin 45e f* ;
: first-across ( -- )  wt2f-1-flag4 fact! ;
wt2f-1-flag3 1 ' edge 4 152e walkway: the-way
the-way $13A -90 -0.47e 39.8e lever-at   the-way $3B6 120 -22.18e 0.43e hewie-out   the-way $3C3 -160 2.56e 19.55e hewie-home
the-way $3B6 $3BF 1 hewie-ends   the-way ' first-across when-across

wt2f-1-breakable-broken crate-part crate-broken-part  -16.5e 152e -47.5e 5e 8e  breakable: crate
crate $80202020 0 0 shard-look   crate 0.812e 0.687e 0.187e 0.312e shard-uv   crate $2B9 noise-from
: came-in ( exit -- )
    room-sounds
    case  0 of  wt2f-1-flag3 fact-off  endof  1 of  wt2f-1-flag3 fact!  endof  endcase
    walkway-floor barred-floor floor-off  the-way walkway-here
    tintstalker-kind partner-is? if  no-stalker-camera state!  then
    summoner-on state? if  -1 summoner-was-on !  summoner-on state-off  then
    stalker let-go  1 summon-take
    wt2f-1-medallion-found fact?  wt2f-1-medallion fact? 0= and if  medallion-fx 4.12e 105e -18.51e flicker-at  then
    wt2f-1-refined-antimony fact? 0= if  antimony-fx -19.9e 153e 74.61e flicker-at  then
    crate as-it-stands
    1 $2300 reverb ;

\ ---- Riccardo: heard prowling above (his growl, then his steps while he's not here) ----
: riccardo-in? ( -- flag )  riccardo? stalker-in? and ;
: prowls ( -- )   \ (an activity, till she reaches the top)
    growl-sound stalker-bank 54e 112e -56e sound-there  60 frames
    begin
        riccardo-in? 0= if  step-sound stalker-bank 54e 112e -56e sound-there  then  30 frames
        riccardo-in? 0= if  step-2-sound stalker-bank 54e 112e -56e sound-there  then  30 frames
    again ;
: sent-in ( tri -- )  this-room @ swap stalker-to  0 partner send stalker-order  his-own-time ;
: comes-first ( tri -- )  sent-in  wt2f-1-flag1 fact! ;
: comes-back ( -- )   \ (each frame after his first coming: 1800 frames after he left, back - by the landing the walkway reaches)
    return-due @ 0= if  return-frames return-wait !  stalker-told? 0= if  -1 return-due !  then  exit  then
    stalker-told? if  0 return-due !  exit  then
    return-wait @ if  -1 return-wait +!  exit  then
    turning @  stalkers-stay state? or if  exit  then
    turned? if  888  else  473  then  sent-in  0 return-due ! ;
: riccardo-hunts ( -- )
    wt2f-1-flag2 fact?  c7-second-event fact? 0= and  riccardo? and  stalker-in? 0= and 0= if  exit  then
    wt2f-1-flag1 fact? if  comes-back exit  then
    prowl-begun @ 0=  fiona landing-area left? and if  -1 prowl-begun !  ['] prowls start drop  then
    fiona top-area entered? if  ['] prowls stop-doing  then
    fiona way-up-area entered? if  825 comes-first  then
    fiona stairs-area entered? if  816 comes-first  then ;

\ ---- what she may do or say here ----
: pick-medallion ( -- )  medallion wt2f-1-medallion medallion-fx 4.12e -18.51e picks-up ;
: pick-antimony ( -- )  refined-antimony wt2f-1-refined-antimony antimony-fx -19.9e 74.61e picks-up ;
: medallion-sniffed ( -- )  wt2f-1-medallion-found wt2f-1-medallion medallion-fx  4.12e 105e -18.51e  hewie-finds ;
: barks-at-end ( -- )  1.97e 152e 46.65e hewie-barks-at ;
: spots ( -- )
    lever-area -90 lever-offer
    wt2f-1-medallion-found fact?  wt2f-1-medallion fact? 0= and  fiona 4.12e 104e -18.51e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then
    wt2f-1-refined-antimony fact? 0=  fiona -19.9e 152e 74.61e 5e 5e touches? and if  ['] pick-antimony calm-only offer  then ;
: hewie-bits ( -- )
    wt2f-1-medallion-found fact? 0=  scrambling? and  hewie 4.12e 104e -18.51e 10e 5e touches? and if  ['] medallion-sniffed start-only  then
    wt2f-1-flag4 fact? 0=  hewie-noticed @ 0= and  hewie-in? and  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and
    hewie 1.97e 152e 46.65e 38e 10e centred-in? and if  -1 hewie-noticed !  ['] barks-at-end start drop  then ;

\ ---- every frame ----
create entries  0 , 0 ,  4 , 4 ,   \ (by exit 0, 1: the camera)
create cameras  4 , 0 , 0 ,  5 , 1 , 1 ,  6 , 1 , 1 ,  7 , 2 , 2 ,  8 , 2 , 2 ,  9 , 3 , 3 ,  $A , 2 , 2 ,  $B , 4 , 4 ,   \ (area, set, path)
create ahead  2 , 0 ,  3 , 1 ,   \ (area, exit loaded ahead)
: map-pages ( -- )
    fiona map-2-area entered? if  2 map-page  then
    fiona map-2-area left?  fiona map-3-area entered? or if  3 map-page  then
    fiona map-3-area left? if  4 map-page  then ;
: each-frame ( -- )
    0 0 leaves-by  1 1 leaves-by  cameras 8 cameras-at  ahead 2 ways-out
    hewie-in? if  hewie hewie-far-area entered? if  1 hewie-side  then   hewie hewie-far-area left? if  0 hewie-side  then  then
    map-pages  riccardo-hunts  time-up-leaves
    crate breaks? if  this-room @ 1 291 $24 5 6 creature-slot-in  then
    hewie-in?  hewie walkway-floor on-group? and  hewie-aboard !
    hewie-bits  spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    entries 2 ways-in  came-by @ 1 = if  4 map-page  then ;
: went ( -- )   \ (Hewie left on the walkway's floor: by the landing it reaches; the camera and the summoner as they were)
    hewie-aboard @ if  turned? if  this-room @ 887 0  else  this-room @ 874 1  then  hewie-to  then
    no-stalker-camera state-off
    summoner-was-on @ if  summoner-on state!  then ;

' came-in ' person-in ' each-frame ' went water-tower-2f-1 place!
