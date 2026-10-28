\ story/places/old-mansion-3f-1.fs - the glass-floored hall of the old mansion's 3F: four boxes
\ pushed onto their squares (each one stopping there - the floor's light up a step, the hum begun);
\ the first time she steps in, the scene ("Blood, Flesh Woman") - Daniella after her, the door shut
\ behind; all four set, Daniella stepping onto the middle - the glass gives, and her death (the
\ scene; the files, Hewie's trust); after, her body pinned (its remarks), the Mars Key; the locked
\ door ("Damn!"); the doorways' shade; the replays. A place (story/kit.fs). The original's scripts,
\ for checking: story/rooms-ref/old-mansion-3f-1.fs in the history (converted from the game's; gone
\ with the old machinery in 0.2.20). Its acts 2 and 4 (the same death another way) nothing starts;
\ the stalker's springs and the corners' cones of light aren't drawn.
IN: story.places.old-mansion-3f-1
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names effects story.kit fiona.anims scenes ;

own-reset

1 constant locked-msg   \ "Damn! The door is locked!"
2 constant pinned-msg   \ "A shard of glass pierces through her body and pins her to the ground. I can't belie..."
3 constant cold-msg     \ "When she was alive, her facial expressions always seemed ice-cold and devoid of fee..."
own: set-boxes  own: stopped-mask  own: stepped-in  own: pinned-remarked  own: stage  own: was-dead  own: hum-on
0 constant key-fx   1 constant glow-fx   1 constant box-sound   2 constant hum-sound   3 constant his-sound
1 constant intro-movie   0 constant intro-cutscene   3 constant death-movie   2 constant death-cutscene
$70 constant hall-door   $1D constant scene-figure   $1D constant her-motions
create box-homes  $20A , $153 , $15C , $213 ,   \ (each box's square)
create box-starts  $217 , $4E2 ,  $12D , $3F3 ,  $249 , $519 ,  $272 , $544 ,
create box-ends  $20A , $4D8 ,  $153 , $415 ,  $15C , $41E ,  $213 , $4DE ,

\ ---- the floor's light: up a step for each box set ----
: floor-light ( -- )   \ (parts 3..6 its steps, 8 none)
    3 hide-part  4 hide-part  5 hide-part  6 hide-part  8 hide-part
    set-boxes @ case  1 of 3  endof  2 of 4  endof  3 of 5  endof  4 of 6  endof  >r 8 r>  endcase  show-part ;
: strength ( -- k )  set-boxes @ case  1 of $1E endof  2 of $3C endof  3 of $5A endof  4 of $80 endof  >r 0 r>  endcase ;
: shine-at ( k -- )   \ (the reflecting floor, its glow told the same)
    {: k :}  $1B false k  10e -0.2e -10e  -10e -0.2e -10e  10e -0.2e 10e  -10e -0.2e 10e  glass-in
    k if  k glow-fx tell-effect  then ;
: shine ( -- )  strength shine-at ;   \ (by the boxes set)
: glow-off ( -- )  $1B effect-off  glow-fx effect-off ;
: all-hidden ( -- )  $1C 4 do  i object-hide  loop ;   \ (its loose shards)
: boxes-at ( table -- )  {: t :}  4 0 do  i i 0  t i 2* cells +  dup @ swap cell+ @  obstacle-in  loop ;   \ (box i on its square)

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  9 hide-part  $A hide-part  0 set-boxes !
    om3f-1-flag2 fact? 0= if
        0 show-part  1 hide-part  box-starts boxes-at  7 hide-part
    else
        0 hide-part  1 show-part  box-ends boxes-at  4 0 do  i obstacle-fixed  loop  7 show-part
        0 solid-floor floor-on  om3f-1-flag3 fact? if  $A  else  9  then  show-part
        got-mars-key fact? 0= if  key-fx 1.29e 1e -17.02e flicker-at  then
    then
    om3f-1-flag2 fact? if  $F  else  0  then  stopped-mask !   \ (all four stopped already; the light at none)
    2 hide-part  all-hidden  floor-light  effects:floor-glow glow-fx effect!  shine  1 $2300 reverb ;

\ ---- the boxes pushed home ----
: boxes ( -- )
    4 0 do
        stopped-mask @ i rshift 1 and 0= if
            i  box-homes i cells + @  obstacle-on-tri? if
                i obstacle-fixed  1 i lshift stopped-mask @ or stopped-mask !
                set-boxes @ 0= if  -1 hum-on !  then  1 set-boxes +!  box-sound room-bank fiona sound-by
            then
        then
    loop ;
: all-set? ( -- flag )  stopped-mask @ $F = ;

\ ---- the first time in: the scene - Daniella after her, the door shut behind ----
: blends ( from to -- )  {: a b :}  begin  cutscene-frame a < while  yield  repeat  begin  cutscene-frame b < while  $2D frame-blend  yield  repeat ;
: till-shot ( n -- )  begin  cutscene-frame cutscene-shot-at over <> while  yield  repeat  drop ;
: intro-looks ( -- )   \ (along the scene: the picture blended at its cues, her face at the end)
    $468 $477 blends  $484 $499 blends  $49D $4B3 blends  $4B4 $4D2 blends  $4D7 $4F2 blends
    $19 till-shot  partner-kind 0 s" O_DNL\DNL_203.TEX" face-on ;
: intro-scene ( -- )
    intro-movie scenes:unseen-movie intro-cutscene load-scene  wait-fade
    scene-figure 3 figure-in  4 cutscene-keep-model  ['] intro-looks start drop  2 show-part
    panic-eased  if  0 run-scene  then  2 hide-part  3 figure-out ;
: stepped-in-as ( daniella? -- )
    {: dan :}  stalker-unloaded  stalkers-stay state!  fiona hold  hewie-no-attack state!  stalkers-blind state!  force-calm state!
    dan if  35 stalker-ready  om3f-1-flag3 fact-off  else  36 stalker-ready  om3f-1-flag3 fact!  then
    summoner-on state-off  yield
    15 screen:to-black-2 with-music or fade  world-held state!
    hewie activate  old-mansion-3f-1 433 0 hewie-to  old-mansion-3f-1 1571 stalker-to  stalker 2 2 camera-set  stalker let-go
    intro-scene  world-held state!
    fiona $247 180 -0.592e 1.575e place-at  hewie $2CF 180 0.346e -5.781e place-at  stalker $50D 0 -1.47e -23.64e place-at
    hall-door false open-mark  hall-door lock  doors-redone  scene-5-pending state!  om3f-1-flag1 fact!  summoner-on state-off
    stalker 2 2 camera-set  force-calm state-off  8 1e background  15 screen:from-black fade wait-fade
    stalkers-stay state-off  hewie-no-attack state-off  stalkers-blind state-off ;
: stepped-in-first ( -- )  partner-kind daniella-kind = stepped-in-as ;

\ ---- all four set, Daniella on the middle: the glass gives - her death ----
: her-fall ( -- )   \ (her part of it: the motion of her end)
    stalker hold  stalker her-motions motions-for  stalker use-motions  1 stage !
    begin  stage @ 2 < while  yield  repeat
    stalker $1AB -90 0.25e 0.25e place-at  stalker room-motion start-anim  150 frames  his-sound room-bank stalker sound-by
    247 frames  stalker wait-anim  3 stage !  begin  yield  again ;
: death-looks ( -- )   \ (along the scene: shards gone, the floor's light out, the hum stopped)
    2 till-shot  $1E object-hide  4 till-shot  $1E object-hide
    5 till-shot  glow-off  0 set-boxes !  floor-light
    begin  cutscene-frame $369 < while  yield  repeat  hum-sound room-bank stop-sound  0 hum-on !
    $A till-shot  $1E object-hide ;
: after-death ( -- )   \ (back in the hall, her body pinned; Hewie well again)
    fiona true shown  scene-5-pending state-off  hewie-no-attack state-off  stalkers-blind state-off  force-calm state-off
    stalker-unloaded  7 show-part  0 set-boxes !  floor-light  all-hidden  0 panic-to
    got-mars-key fact? 0= if  key-fx 1.29e 1e -17.02e flicker-at  then
    hewie activate  hewie full-health
    fiona $1E6 27 -22.22e -47.34e place-at  fiona 0 idle  hewie $30 21 -14.71e -55.27e place-at  $29 0 hewie-do
    2 2 player-camera  camera-restart  0 solid-floor floor-on  glow-off  fiona follow ;
: glass-gives ( -- )
    15 screen:to-black fade  stalkers-stay state!  hewie-no-attack state!  stalkers-blind state!  fiona hold  wait-fade
    world-held state!  yield  0 stage !  ['] her-fall start drop  begin  stage @ 1 < while  yield  repeat
    fiona false shown  hewie let-go  no-track 1e background  fiona 3 -1 camera-set  0 panic-to
    2 stage !  15 screen:from-black fade  begin  stage @ 3 < while  yield  repeat  wait-fade
    15 screen:to-black-2 fade  death-movie scenes:keyed-movie death-cutscene load-scene  wait-fade
    0 hide-part  1 show-part  ['] death-looks start drop  $1E object-hide  om3f-1-flag3 fact? if  $A  else  9  then  show-part
    $1E $C8 movie-key  panic-eased  if  -1 run-scene  then  wait-fade
    ['] her-fall stop-doing  hum-sound room-bank stop-sound  $1E object-hide
    after-death  15 fade-in
    $23E give-file  $8282 give-file  trust-file  stalkers-stay state-off
    hall-door unlock  $60 stalker-slot 0 doors-into  $63 stalker-slot 0 doors-into ;
: death-due ( -- )
    om3f-1-flag1 fact?  om3f-1-flag2 fact? 0= and  all-set? and 0= if  exit  then
    stalker 0e 0e 0e 10e 10e touches? if
        om3f-1-flag2 fact!  om1f-6-flag1 fact!  force-calm state!  panic-eased  ['] glass-gives start drop
    then ;

\ ---- what she may do or say here ----
: the-body ( -- )
    fiona wait-move  fiona 0e 0e turn-to-xz  fiona look-close-anim start-anim
    pinned-remarked @ if  fiona look-close-anim 10 play-blend  cold-msg  0  else  pinned-msg  -1  then  pinned-remarked !  say ;
: take-key ( -- )
    fiona wait-move  fiona 0e -17e turn-to-xz  fiona pick-up-anim play  4 frames
    mars-key got-mars-key key-fx takes-the  50 trust+  fiona up-from-pick-anim play ;
: the-door ( -- )  locked-msg show ;

\ ---- the replays (the extras) ----
: replay-intro ( -- )
    fiona hold  9 hide-part  $A hide-part  8 show-part  0 show-part  1 hide-part  box-starts boxes-at  7 hide-part
    all-hidden  35 stalker-ready  yield  intro-scene  stalker-unloaded  replay-over ;
: replay-death ( -- )
    fiona hold  9 hide-part  $A hide-part  0 hide-part  1 show-part  box-ends boxes-at  4 0 do  i obstacle-fixed  loop
    7 hide-part  2 hide-part  all-hidden  4 set-boxes !  floor-light  shine
    35 stalker-ready  yield  fiona false shown  fiona 3 -1 camera-set
    death-movie scenes:keyed-movie death-cutscene load-scene  0 hide-part  1 show-part  ['] death-looks start drop
    $1E object-hide  9 show-part  $1E $C8 movie-key  panic-eased  if  -1 run-scene  then
    stalker-unloaded  replay-over ;

create cameras  1 , 0 , 0 ,   2 , 1 , 1 ,   3 , 1 , 1 ,   4 , 2 , 2 ,   \ (area, set, path)
: each-frame ( -- )
    0 exit-out  cameras 4 cameras-at
    hum-on @  room-sounds-in? and if  hum-sound 0e 0e 0e keep-sound  then
    om3f-1-flag2 fact? 0= if  boxes  then  floor-light  shine
    om3f-1-flag1 fact? 0=  stepped-in @ 0= and  fiona 4 entered? and if  -1 stepped-in !  panic-eased  ['] stepped-in-first start drop  then
    death-due
    om3f-1-flag1 fact?  om3f-1-flag2 fact? 0= and if
        st-cond-now @ down-cond = if  -1 was-dead !  then
        was-dead @  st-cond-now @ down-cond <> and if  $23D give-file  then
    then
    scene-5-pending state?  1 locked-request? and if  ['] the-door any-time offer  then
    om3f-1-flag2 fact?  fiona 5 45 faces-area? and if  ['] the-body unhurried offer  then
    got-mars-key fact? 0=  om3f-1-flag2 fact? and  fiona 1.29e 0e -17.02e 5e 5e touches? and if  ['] take-key calm-only offer  then
    50e -29e -143e  -8.8e 9.3e -143e  50e -50e -143e  -8.8e -10e -143e  light-doorway
    66e -39e -143e  50e -29e -143e  69e -55e -143e  50e -50e -143e  light-doorway ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if
        hewie send part-from-fiona  replay-act $A = if  ['] replay-intro  else  ['] replay-death  then  start drop  exit
    then
    0 0 0 way-in ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-3f-1 place!
