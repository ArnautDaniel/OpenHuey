\ story/places/old-mansion-2f-2.fs - the old mansion's 2F corridor by the furnace room: its door
\ blocked from inside ("It won't budge", then the tiny window); Hewie, hidden in old-mansion-2f-3's
\ hole, come through to the furnace room - barking inside ("Hey! Hewie is inside!"); her call: his
\ scene, the way opened (the wood gone), the door unlocked, his file; the Viride Medallion's
\ sparkle, the Medallion Hewie sniffs out. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/old-mansion-2f-2.fs in the history (converted from the game's; gone with
\ the old machinery in 0.2.20).
IN: story.places.old-mansion-2f-2
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names hewie-names story.kit fiona.anims ;

own-reset

0 constant budge-msg     \ "It won't budge. It appears to be blocked by something on the other side of the..."
1 constant window-msg    \ "I can just barely see inside from this tiny window... There is a huge furnace i..."
2 constant inside-msg    \ "Hey! Hewie is inside!"
own: blocked-remarked
own: hewie-inside        \ Hewie came through to the furnace room
own: bark-frames         \ frames to his next bark (90, 150 or 30)
0 constant viride-fx   1 constant medallion-fx   2 constant wood-object   0 constant way-part
$47 constant furnace-door   1 constant hewie-movie   0 constant hewie-cutscene   $8283 constant hewie-file
$31 constant call-sound

: came-in ( exit -- )
    drop  1 both-halves floor-on
    om2f-2-flag1 fact? if  way-part show-part  wood-object object-hide  then
    om2f-4-viride-medallion fact? 0= if  viride-fx 5.64e 9.5e 5.58e flicker-at  then
    om2f-2-medallion-found fact?  om2f-2-medallion fact? 0= and if  medallion-fx -49.36e 1e -47.3e flicker-at  then ;

\ ---- Hewie in the furnace room: barking, now and then ----
: barking ( -- )
    hewie hold  hewie wait-move  hewie model-of dup 0< if  drop  else  0 swap actor act.doorway-fade l!  then   \ (no fading in the doorway)
    begin
        bark-frames @ 0= if
            hewie-bark  hewie wait-move
            50 chance? if  90  else  50 chance? if  150  else  30  then  then  bark-frames !
        else  -1 bark-frames +!  yield  then
    again ;
: through-to-furnace ( -- )   \ (a moment on, as Hewie tells where he is: hidden in old-mansion-2f-3's hole)
    2 frames
    om2f-2-flag1 fact? 0=  hewie body-room old-mansion-2f-3 = and  hewie-act-now @ hidden-act = and if
        old-mansion-2f-2 65 0 hewie-to  -1 hewie-inside !  ['] barking start drop
    then ;

\ ---- the blocked door ----
: to-window ( -- )  fiona $CB 30 -38e -30e walk-to ;
: the-door ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move
    hewie-inside @ if  to-window  inside-msg say  0 bark-frames !  stalkers-stay state-off  exit  then
    blocked-remarked @ 0= if
        fiona 0 to-door  fiona rattle-behind-anim play  fiona -1 idle  15 frames
        fiona $CB 30 -38e -30e place-at  5 4 player-camera  4 frames  budge-msg say  -1 blocked-remarked !
    else  to-window  5 4 player-camera  4 frames  window-msg say  then
    4 frames  3 2 player-camera  stalkers-stay state-off ;

\ ---- her call: his scene - the way opened, the door unlocked ----
: called ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move
    fiona call-gesture-anim start-anim  fiona 1 wait-mark  call-sound common-bank fiona sound-by  fiona wait-anim
    fiona -1 idle  7 frames
    15 screen:to-black-2 with-music or fade  hewie-movie scenes:unseen-movie hewie-cutscene load-scene  wait-fade
    panic-eased  if  -1 run-scene  then  wait-fade
    world-held state!  50 trust+  way-part show-part  wood-object object-hide
    fiona 0 idle  fiona $CB 0 -38.75e -31.7e place-at  camera-restart  furnace-door unlock
    om2f-2-flag1 fact!  0 hewie-inside !  ['] barking stop-doing  old-mansion-2f-4 -1 0 hewie-to
    15 fade-in  hewie-file give-file  stalkers-stay state-off ;
: calls-him? ( -- flag )   \ (at the door, facing it - her call, Hewie inside, nobody after her)
    om2f-2-flag1 fact? 0=  fiona 0 in-area? and  fiona 0 80 heading? and
    fiona-cmd-now @ 1 = and  hewie-inside @ and  stalker-in? 0= and  fiona-mode-now @ own-mode = and ;

\ ---- the Medallion Hewie sniffs out ----
: take-medallion ( -- )  medallion om2f-2-medallion medallion-fx -49.36e -47.3e picks-up ;
: medallion-sniffed ( -- )  om2f-2-medallion-found om2f-2-medallion medallion-fx  -49.36e 1e -47.3e  hewie-finds ;

\ ---- the movie replayed (the extras) ----
: replayed ( -- )
    fiona hold  hewie-movie scenes:unseen-movie hewie-cutscene load-scene  panic-eased  if  -1 run-scene  then  replay-over ;

create cameras  $B , 3 , 2 ,   $C , 4 , 3 ,   \ (area, set, path)
create ahead  9 , 0 ,   $A , 2 ,   \ (area, exit loaded ahead)
: each-frame ( -- )
    events-held state? if  exit  then
    0 exit-out  2 2 leaves-by  cameras 2 cameras-at  ahead 2 ways-out
    calls-him? if  ['] called start drop  then
    om2f-2-medallion-found fact? 0=  scrambling? and  hewie -49.36e 0e -47.3e 10e 5e touches? and if  ['] medallion-sniffed start-only  then
    chased? 0=  in-panic? 0= and  om2f-2-flag1 fact? 0= and  fiona 0 before-door? and if  ['] the-door any-time offer  then
    om2f-2-medallion-found fact?  om2f-2-medallion fact? 0= and  fiona -49.36e 1e -47.3e 5e 5e touches? and if  ['] take-medallion calm-only offer  then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop  exit  then
    0 3 2 way-in  2 4 3 way-in  ['] through-to-furnace start drop ;
: went ( -- )   \ (Hewie, inside still: back in the hole)
    om2f-2-flag1 fact? 0=  hewie-in? and  hewie-inside @ and if  old-mansion-2f-3 -1 0 hewie-to  18000 2 hewie send hewie-yelp  then ;

' came-in ' person-in ' each-frame ' went old-mansion-2f-2 place!
