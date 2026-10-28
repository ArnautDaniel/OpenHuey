\ story/places/water-tower-4f-1.fs - the Water Tower's 4F ($C3): the walkway (see walkway.fs) turned
\ by its lever - its doors open only while it's across, creatures let out the first time each visit
\ it gets there; the drum (the object doramukan) falling as she first goes on - a jolt, a fright,
\ and Riccardo come for her; after, back 1800 frames after each time he left; taken off as she comes
\ in, and when his time is up and she can't see him; the summoner off while she's here; the box (the
\ Camomile in it); the Medallion; Hewie's bark at the walkway's far end (till it first turns); the
\ map's pages. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/water-tower-4f-1.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.water-tower-4f-1
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit story.places.walkway fiona.anims ;

own-reset  walkway-owns owns !

own: summoner-was-on       \ the summoner was on as she came in (on again as she leaves)
own: return-due            \ Riccardo gone: his coming back counted down
own: return-wait           \ ... the frames left
own: hewie-noticed         \ Hewie barked at the walkway's far end (once a visit)
0 constant medallion-fx   1 constant camomile-fx
3 constant drum-sound   $13 constant growl-sound   \ (the room's; his kind's)
4 constant drum-object   0 constant drum-anim
5 constant box-part   6 constant box-broken-part
$FF constant near-door   $101 constant far-door
$C constant lever-area   $D constant on-area   $E constant on-2-area   $F constant first-step-area
$15 constant map-4-area   $16 constant map-5-area   $17 constant map-6-area
1800 constant return-frames
: drum ( -- addr len )  s" doramukan" ;

\ ---- the walkway: to -90 degrees, at 142; its edge 45 out; the lever at $166; Hewie's spot $260 ----
: edge ( F: rad -- x z )  fdup fsin 45e f*  fswap fcos 45e f* ;
: doors-by-it ( -- )   \ (open only while it's across)
    turned? if  near-door unlock  far-door unlock  else  near-door lock  far-door lock  then ;
: first-across ( -- )   \ (the creatures let out on the far side)
    this-room @ 0 1055 $A 4 creature-in  this-room @ 0 1053 $A 6 creature-in  this-room @ 0 618 $A 7 creature-in
    this-room @ 0 1051 $A 7 creature-in  this-room @ 0 607 $A 4 creature-in  this-room @ 0 615 $A 6 creature-in
    wt4f-1-flag4 fact! ;
wt4f-1-flag3 -1 ' edge 5 142e walkway: the-way
the-way $166 180 73.23e -0.48e lever-at   the-way $260 180 9.39e -18.29e hewie-out   the-way $260 180 9.39e -18.29e hewie-home
the-way $260 $260 -1 hewie-ends   the-way ' first-across when-across   the-way ' doors-by-it when-stopped

wt4f-1-breakable-broken box-part box-broken-part  -86.45e 46e -8e 5e 8e  breakable: box
box $80202020 0 0 shard-look   box 0.812e 0.687e 0.187e 0.312e shard-uv   box $131 noise-from
: camomile-shows ( -- )  camomile-fx -86.45e 47e -8e flicker-at ;
: came-in ( exit -- )
    room-sounds
    dup 1 = if  wt4f-1-flag3 fact!  then
    the-way walkway-here  0 2 within if  doors-by-it  then   \ (by either exit: the doors as it lies)
    wt4f-1-flag2 fact? if  drum-object object-hide  else  drum-object object-show  then
    tintstalker-kind partner-is? if  no-stalker-camera state!  then
    summoner-on state? if  -1 summoner-was-on !  summoner-on state-off  then
    stalker let-go  1 summon-take
    wt4f-1-medallion fact? 0= if  medallion-fx -71.41e 143e -7.67e flicker-at  then
    box as-it-stands
    box broken?  wt4f-1-camomile fact? 0= and if  camomile-shows  then
    1 $2300 reverb ;

\ ---- Riccardo: the drum falling as she first goes on - a jolt, her flinch and fright; him come for her ----
: drum-falls ( -- )   \ (an activity)
    drum-object 1 drum-anim object  17 frames  1 $FF 8 rumble-pad
    fiona send flinch  50 threat
    drum placed-here? if  drum-sound room-bank  drum place-x object@  drum place-y object@  drum place-z object@  sound-there  then
    60 frames
    wt4f-1-flag1 fact? 0= if
        growl-sound stalker-bank 54e 112e -56e sound-there
        this-room @ 830 stalker-to  0 partner send stalker-order  his-own-time  wt4f-1-flag1 fact!
    then ;
: comes-back ( -- )   \ (each frame after his first coming: 1800 frames after he left, back - by the walkway's side)
    return-due @ 0= if  return-frames return-wait !  stalker-told? 0= if  -1 return-due !  then  exit  then
    stalker-told? if  0 return-due !  exit  then
    return-wait @ if  -1 return-wait +!  exit  then
    turning @  stalkers-stay state? or if  exit  then
    this-room @  turned? if  944  else  249  then  stalker-to  0 partner send stalker-order  his-own-time  0 return-due ! ;
: riccardo-hunts ( -- )
    wt2f-1-flag2 fact?  c7-second-event fact? 0= and  riccardo? and  stalker-in? 0= and 0= if  exit  then
    wt4f-1-flag2 fact? 0= if
        fiona first-step-area entered? if  fiona 5 4 camera-set  then
        fiona on-area entered?  fiona on-2-area entered? or if  wt4f-1-flag2 fact!  ['] drum-falls start drop  then
        exit
    then
    wt4f-1-flag1 fact? if  comes-back  then ;

\ ---- what she may do or say here ----
: pick-medallion ( -- )  medallion wt4f-1-medallion medallion-fx -71.41e -7.67e picks-up ;
: pick-camomile ( -- )  camomile wt4f-1-camomile camomile-fx -86.45e -8e picks-up ;
: barks-at-end ( -- )  -47.23e 142e 1.87e hewie-barks-at ;
: spots ( -- )
    lever-area 180 lever-offer
    wt4f-1-medallion fact? 0=  fiona -71.41e 142e -7.67e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then
    box broken?  wt4f-1-camomile fact? 0= and  fiona -86.45e 46e -8e 5e 5e touches? and if  ['] pick-camomile calm-only offer  then ;
: hewie-at-end ( -- )
    wt4f-1-flag4 fact? 0=  hewie-noticed @ 0= and  hewie-in? and  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and
    hewie -47.23e 142e 1.87e 38e 10e centred-in? and if  -1 hewie-noticed !  ['] barks-at-end start drop  then ;

\ ---- every frame ----
create entries  0 , 3 ,  3 , 2 ,   \ (by exit 0, 1: the camera)
create cameras  4 , 0 , 3 ,  5 , 1 , 0 ,  6 , 0 , 3 ,  7 , 2 , 1 ,  8 , 0 , 3 ,  9 , 2 , 1 ,  $A , 2 , 1 ,  $B , 3 , 2 ,
    $14 , 1 , 0 ,  $13 , 6 , 5 ,   \ (area, set, path)
create ahead  2 , 0 ,  3 , 1 ,   \ (area, exit loaded ahead)
: map-pages ( -- )
    fiona map-4-area entered? if  4 map-page  then
    fiona map-4-area left?  fiona map-5-area entered? or if  5 map-page  then
    fiona map-5-area left?  fiona map-6-area entered? or if  6 map-page  then
    fiona map-6-area left? if  7 map-page  then ;
: each-frame ( -- )
    0 0 leaves-by  1 1 leaves-by  cameras 10 cameras-at  ahead 2 ways-out
    map-pages
    fiona $10 entered?  fiona $11 entered? or  fiona $12 entered? or if  fiona 0 3 camera-set  then
    riccardo-hunts  time-up-leaves  hewie-at-end
    spots
    box breaks? if  camomile-shows  then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    entries 2 ways-in  came-by @ 1 = if  7 map-page  then ;
: went ( -- )   \ (the camera and the summoner as they were)
    no-stalker-camera state-off
    summoner-was-on @ if  summoner-on state!  then ;

' came-in ' person-in ' each-frame ' went water-tower-4f-1 place!
