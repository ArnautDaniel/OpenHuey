\ story/places/old-mansion-2f-6.fs - the laboratory of the old mansion's 2F: two lockers - chased by
\ Daniella or Riccardo (him not in the room), she hides in one: its scene (by the near one he comes
\ and goes; by the far one Daniella is knocked down and she slips out, Riccardo taken off); else
\ their shelves' remarks (the medicine, the chemicals); the water (a recovery point); the measuring
\ device; the locked cabinet; the canister; the containers; the operating table; the Lavender; back
\ by the forced way from the forest, the "2 or 3 days" scene; the replays. A place (story/kit.fs).
\ The original's scripts, for checking: story/rooms-ref/old-mansion-2f-6.fs (converted from the
\ game's, not loaded). (The stalker's model's springs, its cmd 0, aren't simulated.)
IN: story.places.old-mansion-2f-6
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    items.names story.kit fiona.anims scenes ;

own-reset

0 constant days-msg        \ "Another 2 or 3 days, at most..."
3 constant medicine-msg    \ "There's some kind of medicine bottle on the shelf."
4 constant chemicals-msg   \ "Several bottles containing various chemicals have been left out."
5 constant acetic-msg      \ "The label says Acetic Acid on it."
6 constant sulfuric-msg    \ "The words Sulfuric Acid are written on one bottle."
7 constant water-msg       \ "I hope this water won't make me sick if I drink it."
8 constant refreshed-msg   \ "Ahh. That hit the spot."
9 constant device-msg      \ "There is a device which appears to be used for measuring things... I wonder what th..."
$A constant cabinet-msg    \ "I wonder if this could serve as a hiding spot if I'm ever in a pinch? Oh darn. It'..."
$B constant locked-msg     \ "I don't think I'll be able to open it. Maybe they have locked some important chemi..."
$C constant canister-msg   \ "I wonder what's inside? It's not like it's going to explode or anything... right?"
$D constant containers-msg \ "Looks like plastic containers used to store chemicals. I can make out the word 'Ethano..."
$E constant modern-msg     \ "For some reason this room seems to contain comparatively modern equipment. Still t..."
$F constant table-msg      \ "A rather dirty and unsanitary operating table. No matter how serious the illness, you..."
own: medicine-remarked  own: chemicals-remarked  own: rested  own: hewie-away  own: from-forest
own: device-remarked  own: cabinet-remarked
0 constant lavender-fx   1 constant calm-sound   $A constant her-motions
8 constant near-locker   9 constant far-locker   $C constant days-movie   $B constant days-cutscene

\ ---- as she comes in ----
: back-from-forest? ( -- flag )  cf-13-flag1 fact?  x-37-flag2 fact? 0= and  came-by @ forced-exit = and ;
: came-in ( exit -- )
    drop  room-sounds  back-from-forest? from-forest !
    om2f-6-flag2 fact? if  near-locker object-hide  then  om2f-6-flag1 fact? if  far-locker object-hide  then
    0 hide-part
    from-forest @ 0=  om2f-6-lavender fact? 0= and if  lavender-fx 4.58e 9e 14.22e flicker-at  then ;

\ ---- Hewie through a scene: down, hidden away till it's over; else let go ----
: hewie-held ( -- )  hewie hold  hewie false shown  begin  yield  again ;
: hewie-for-scene ( -- )
    hewie-in? 0= dup hewie-away !  if  exit  then
    hewie-cond-now @ down-cond = if  ['] hewie-held start drop  else  hewie let-go  then ;
: hewie-after ( tri F: x z deg -- )   \ (down: shown, his own again; else back in by her)
    {: tri F: x z deg :}  hewie-away @ if  exit  then
    hewie-cond-now @ down-cond = if  ['] hewie-held stop-doing  hewie true shown  hewie let-go  exit  then
    hewie activate  old-mansion-2f-6 tri 0 hewie-to  hewie tri deg f>s x z place-at ;

\ ---- hiding in a locker from Daniella or Riccardo ----
: hers? ( -- flag )  partner-kind dup daniella-kind = swap kind34-kind = or ;   \ (Daniella, else Riccardo)
: may-hide? ( -- flag )   \ (one of them after her - chased, not in a panic, him not down)
    hers?  partner-kind riccardo-kind = or  chased? and  in-panic? 0= and  st-cond-now @ down-cond <> and ;
: knocks ( -- )  0 exit-open? 0= if  15 frames  0 knock  then ;   \ (him at the door)
: locker-scene ( movie cutscene kept -- )
    {: m c kept :}  15 screen:to-black-2 with-music or fade
    m scenes:keyed-movie c load-scene  kept cutscene-keep-model  wait-fade  hewie-for-scene ;
: near-locker-hides ( -- )   \ (he comes and goes)
    stalkers-stay state!  fiona hold
    hers? if  3 2 4  else  7 6 5  then  locker-scene  $10 $FF movie-key  ['] knocks start drop
    panic-eased  if  -1 run-scene  then  wait-fade
    near-locker object-hide  fiona 0 idle
    hers? if
        fiona $4A 90 -7.575e 18.619e place-at  old-mansion-2f-6 16 stalker-to  stalker 2 2 camera-set
        stalker $10 -90 place-on-tri  0 stalker-order  $35 resident-flag-set
    else
        fiona $4A 90 -7.719e 18.622e place-at  old-mansion-2f-6 130 stalker-to  stalker 2 2 camera-set
        stalker $82 -90 place-on-tri  0 stalker-order  $37 resident-flag-set
    then
    2 2 player-camera  camera-restart  3 -7.85e 29.94e 52e hewie-after
    om2f-6-flag2 fact!  15 fade-in  stalkers-stay state-off ;
: far-locker-hides ( -- )   \ (Daniella knocked down, she slips out; Riccardo taken off)
    stalkers-stay state!  fiona hold
    hers? if  1 0 4  else  5 4 5  then  locker-scene  $10 $FF movie-key
    force-followed state!  yield  force-followed state-off  ['] knocks start drop
    panic-eased  if  -1 run-scene  then  wait-fade  world-held state!
    far-locker object-hide  fiona 0 idle  $38 -4.97e 35.12e -145e hewie-after  om2f-6-flag1 fact!
    hers? if
        fiona $4A 180 place-on-tri  old-mansion-2f-6 16 stalker-to  stalker $10 -30 23.591e 11.878e place-at
        stalker activate  stalker send stalker-knock-out  $34 resident-flag-set  $80 false open-mark  forced-exit take-exit
    else
        fiona $4A -130 -6.968e 18.812e place-at  2 2 player-camera  camera-restart
        stalker let-go  3 summon-take  15 fade-in  $36 resident-flag-set  stalkers-stay state-off
    then ;

\ ---- back by the forced way from the forest: the "2 or 3 days" scene ----
: days-scene ( -- )
    days-movie scenes:unseen-movie days-cutscene load-scene  0 show-part
    panic-eased  if  days-msg run-scene  then  0 hide-part ;
: back-from-forest ( -- )
    stalker let-go  hewie let-go  fiona hold  days-scene
    3 figure-out  $C0 preload  world-held state!  $3A resident-flag-set  forced-exit take-exit ;

\ ---- the replays (the extras) ----
: replay-locker ( kind movie cutscene kept obj -- )
    {: kind m c kept obj :}  fiona hold  0 hide-part  kind stalker-ready  yield
    m scenes:keyed-movie c load-scene  kept cutscene-keep-model  $10 $FF movie-key  panic-eased  if  -1 run-scene  then
    obj object-hide  stalker-unloaded  replay-over ;
: replay-days ( -- )  fiona hold  0 hide-part  $17 3 figure-in  yield  days-scene  3 figure-out  replay-over ;

\ ---- what she may do or say here ----
: the-medicine ( -- )
    fiona wait-move  fiona 32e -3.5e turn-to-xz
    medicine-remarked @ if  0 medicine-remarked !  acetic-msg  else  -1 medicine-remarked !  medicine-msg  then  say ;
: the-chemicals ( -- )
    fiona wait-move  fiona -29e -1e turn-to-xz
    chemicals-remarked @ if  0 chemicals-remarked !  sulfuric-msg  else  -1 chemicals-remarked !  chemicals-msg  then  say ;
: the-water ( -- )   \ (a recovery point: once a visit it calms her)
    rested @ if  fiona wait-move  refreshed-msg say exit  then
    -1 rested !  water-msg her-motions calm-sound $7A 0 30e 40.5e recovery-point ;
: the-device ( -- )
    fiona wait-move
    fiona $A in-area? if  fiona -30 turn  fiona look-close-anim  else  fiona -16e -17e turn-to-xz  fiona look-low-anim  then  start-anim
    40 frames  device-remarked @ if  modern-msg  else  -1 device-remarked !  device-msg  then  say  fiona wait-anim ;
: the-cabinet ( -- )
    fiona wait-move  fiona -70 turn
    cabinet-remarked @ 0= if  -1 cabinet-remarked !  cabinet-msg say exit  then
    fiona rattle-front-slow-anim start-anim  11 frames  try-door-sound common-bank fiona sound-by  fiona wait-anim
    locked-msg show  fiona -1 idle  7 frames  wait-closed ;
: eyes-on ( x z anim msg -- )  {: anim msg F: x z :}  fiona wait-move  fiona x z turn-to-xz  fiona anim start-anim  40 frames  msg say  fiona wait-anim ;
: the-canister ( -- )  2e 45e look-low-anim canister-msg eyes-on ;
: the-containers ( -- )  14.2e 44.7e look-close-anim containers-msg eyes-on ;
: the-table ( -- )  6.5e 14.5e look-low-anim table-msg eyes-on ;
: take-lavender ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona 4.58e 14.22e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  lavender om2f-6-lavender lavender-fx takes-up  fiona reach-back-anim play ;

create cameras  1 , 0 , 0 ,   2 , 1 , 1 ,   3 , 1 , 1 ,   4 , 2 , 2 ,   5 , 1 , 1 ,   6 , 2 , 2 ,   \ (area, set, path)
: a-locker ( fact area deg hide-xt remark-xt -- )   \ (open still: hide in it when one of them's after her - else its shelf)
    {: f area deg hide remark :}
    f fact?  fiona area in-area? 0= or  fiona deg 50 heading? 0= or if  exit  then
    may-hide? if  stalker-in? if  no-time-message offer-remark-now  else  hide calm-only offer  then
    else  remark unhurried offer  then ;
: each-frame ( -- )
    0 exit-out  cameras 6 cameras-at
    om2f-6-flag2 7 45 ['] near-locker-hides ['] the-medicine a-locker
    om2f-6-flag1 8 -45 ['] far-locker-hides ['] the-chemicals a-locker
    fiona 9 in-area?  fiona 0 50 heading? and if  ['] the-water unhurried offer  then
    fiona $A in-area?  fiona -15 60 heading? and  fiona $B in-area?  fiona 60 -16e -17e faces-point? and or if  ['] the-device unhurried offer  then
    fiona $C in-area?  fiona -35 60 heading? and if  ['] the-cabinet unhurried offer  then
    fiona $E in-area?  fiona 0 60 heading? and if  ['] the-canister unhurried offer  then
    fiona $D in-area?  fiona 10 60 heading? and if  ['] the-containers unhurried offer  then
    om2f-6-lavender fact? 0=  fiona 4.58e 9e 14.22e 5e 5e touches? and if  ['] take-lavender calm-only offer  then
    fiona $F 60 faces-area? if  ['] the-table unhurried offer  then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if
        hewie send part-from-fiona
        replay-act case
            $E of  [: daniella-kind 1 0 4 far-locker replay-locker ;]  endof
            $F of  [: daniella-kind 3 2 4 near-locker replay-locker ;]  endof
            $10 of  [: riccardo-kind 5 4 5 far-locker replay-locker ;]  endof
            $11 of  [: riccardo-kind 7 6 5 near-locker replay-locker ;]  endof
            >r ['] replay-days r>
        endcase  start drop  exit
    then
    0 0 0 way-in
    from-forest @ if  fiona $4B 0 place-on-tri  world-held state!  0e sounds-scaled  panic-eased  ['] back-from-forest start drop  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-2f-6 place!
