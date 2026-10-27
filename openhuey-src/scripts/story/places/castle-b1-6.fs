\ story/places/castle-b1-6.fs - the chapel in Belli Castle's B1 ($C): the goddess statue's
\ inscription (Hermes' cane: the Saturnus Key's scene, the castle opened up); with the key, stepping
\ in - Debilitas comes for her (a scene, the door locked behind); the two stands that hold the
\ chandelier's ropes, struck (by her or by him) till they break, swinging as they're hit - both
\ broken, the chandelier falls on him (two scenes, his end); or he goes down - the way out (a scene);
\ "Oh no! It's locked!" while he's after her; the pulley, the chandelier, him lying there; Hewie's
\ look up at the statue; the five movies replayed. A place (story/kit.fs). The original's scripts,
\ for checking: story/rooms-ref/castle-b1-6.fs (converted from the game's, not loaded).
IN: story.places.castle-b1-6
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects story.kit fiona.anims scenes ;

own-reset

\ ---- what is said here ----
2 constant calm-down-msg      \ "It's OK, Hewie... He can't come after us anymore... Good boy. Calm down, Hewie..."
6 constant letters-msg        \ "There are some letters engraved here..."
7 constant inscription-msg    \ "To the seeker of the sacred technique of Ars, Obey the will of Hermes' divine cane and..."
8 constant cane-msg           \ "A cane? Hmm, curious. I'll bet this is related to that goddess statue... Search mor..."
$A constant deity-msg         \ "If this is the chapel... This statue must symbolize the deity worshipped here."
$B constant locked-msg        \ "Oh no! It's locked!!"
$C constant pulley-msg        \ "A pulley system used to wind up rope... Looks like this is used to raise and lower..."
$D constant fastened-msg      \ "It is tightly fastened... Must be to make sure the chandelier doesn't accidentally..."
$E constant broken-msg        \ "It's broken beyond repair..."
$F constant chandelier-msg    \ "What a fancy chandelier. It must have been quite expensive."
$10 constant killed-msg       \ "I-I could have been killed... I guess fate was smiling on me today... for now."
$11 constant not-moving-msg   \ "He's not moving... I-I think he's dead..."

\ ---- its own state (a visit) ----
own: pulley-remarked   own: chandelier-remarked
own: met               \ his coming made, this visit
own: strength-1  own: strength-2   \ the stands' strength (25: broken at 0)

\ ---- the room's things (its strings: movies, cutscenes, objects) ----
1 constant statue-movie   0 constant statue-cutscene   3 constant calm-movie   2 constant calm-cutscene
5 constant comes-movie    4 constant comes-cutscene    8 constant falls-movie   7 constant falls-cutscene
$C constant down-movie    $B constant down-cutscene
6 constant ev-door   9 constant chandelier   $A constant chandelier-down   $D constant stand-1   $E constant stand-2
$F constant left-lever   $10 constant right-lever   $11 constant rope-1   $12 constant rope-2
0 constant cane-part   1 constant door-part   3 constant stand-2-part   5 constant stand-1-part
6 constant shard-part  7 constant fallen-part
$28 constant chapel-door   $11 constant far-door   0 constant fallen-floor
0 constant hit-sound   1 constant break-sound   $15 constant chapel-track   $1B constant debilitas-27
7 constant step-in-area   5 constant statue-area   $C constant pulley-1-area   $D constant pulley-2-area   $E constant chandelier-area
25 constant stand-strength

\ ---- as she comes in ----
: thing$ ( i -- addr len )  this-room @ story.strings:room-string ;
: stands-shown ( -- )
    cb1-6-flag4 fact? if  stand-1 object-hide  stand-1-part show-part  else  stand-strength strength-1 !  then
    cb1-6-flag5 fact? if  stand-2 object-hide  stand-2-part show-part  else  stand-strength strength-2 !  then ;
: chandelier-shown ( -- )
    found-file-20b fact? if
        chandelier object-hide  chandelier-down object-show  fallen-floor solid-floor floor-on
    else
        chandelier object-show  chandelier-down object-hide
        cb1-6-flag2 fact? if  fallen-part show-part  fallen-floor solid-floor floor-on  then
    then ;
: came-in ( exit -- )
    drop  room-sounds  chapel-track 1e background
    got-saturnus-key fact? 0= if  cane-part show-part  then  door-part show-part  ev-door object-hide
    stands-shown  chandelier-shown  shard-part hide-part  1 $2300 reverb ;

\ ---- the stands: struck, they swing; their strength down (her weak blow 1, her others 5, his by
\ his blow) while he's after her; at 0 broken ----
: swing-set ( pair -- )   \ (its three things - the stand, its lever, its rope - set swinging, half a turn apart)
    3 0 do
        dup i 2* + 13 + thing$ 2dup placed-here? if
            2dup place-x object@  2dup $30 object!   0.5e i s>f f*  2dup $34 object!
            i 2 <> if  0.75e  else  0.5e  then  $3C object!
        else  2drop  then
    loop  drop ;
: swings-on ( addr len -- moving? )   \ (one of them on 60 degrees, its swing down 0.1; still - at x 10)
    {: a n :}  a n placed-here? 0= if  false exit  then
    60e deg>rad a n $34 object+!  a n $34 object@ pi f> if  -2e pi f* a n $34 object+!  then
    -0.1e a n $3C object+!
    a n $30 object@  a n $3C object@  a n $34 object@ fsin f* f+  a n place-x object!
    a n $3C object@ f0> dup 0= if  a n place-x 10e object!  then ;
: swing-step ( pair -- still? )
    0  3 0 do  over i 2* + 13 + thing$ swings-on or  loop  nip 0= ;
: swinging-1 ( -- )  0 swing-set  begin  0 swing-step 0= while  yield  repeat  20 frames ;
: swinging-2 ( -- )  1 swing-set  begin  1 swing-step 0= while  yield  repeat  20 frames ;
: fighting? ( -- flag )  cb1-6-flag1 fact?  cb1-6-flag2 fact? 0= and ;
: blow-strength ( his? -- n )  if  his-blow  else  fiona-sub-now @ $1A = if  1  else  5  then  then ;
: weaker ( strength his? -- )   \ (while he's after her: down by the blow, not below 0)
    fighting? 0= if  2drop exit  then
    blow-strength dup 0> if  over @ swap - 0 max swap !  else  2drop  then ;
: stand-1-struck ( his? -- )
    strength-1 swap weaker
    strength-1 @ 0= if
        $80303030 3 8  10e 10e -35e  0e 0e 0.25e 0.25e  -9e shards
        stand-1 object-hide  stand-1-part show-part  cb1-6-flag4 fact!  break-sound 10e 10e -40e room-sound
    else  hit-sound 10e 10e -40e room-sound  then
    ['] swinging-1 stop-doing  ['] swinging-1 start drop ;
: stand-2-struck ( his? -- )
    strength-2 swap weaker
    strength-2 @ 0= if
        $80303030 3 8  10e 10e 35e  0e 0e 0.25e 0.25e  -9e shards
        stand-2 object-hide  stand-2-part show-part  cb1-6-flag5 fact!  break-sound 10e 10e 40e room-sound
    else  hit-sound 10e 10e 40e room-sound  then
    ['] swinging-2 stop-doing  ['] swinging-2 start drop ;
: stands-struck ( -- )   \ (a blow in its place - his, else hers)
    cb1-6-flag4 fact? 0= if
        stalker 10e 0e -37e 7e 20e blow-in? if  true stand-1-struck
        else  fiona 10e 0e -37e 7e 20e blow-in? if  false stand-1-struck  then  then
    then
    cb1-6-flag5 fact? 0= if
        stalker 10e 0e 37e 7e 20e blow-in? if  true stand-2-struck
        else  fiona 10e 0e 37e 7e 20e blow-in? if  false stand-2-struck  then  then
    then ;

\ ---- the scenes ----
: hewie-back ( tri face F: x z -- )  {: tri face F: x z :}  hewie activate  castle-b1-6 78 0 hewie-to  hewie tri face x z place-at ;
: comes-scene ( -- )   \ (Debilitas in for it: unshadowed)
    debilitas-27 stalker-ready  yield
    comes-movie scenes:unseen-movie comes-cutscene load-scene  wait-fade
    door-part hide-part  hewie activate  castle-b1-6 209 0 hewie-to  castle-b1-6 206 stalker-to
    stalker 0 -1 camera-set  partner activate  partner-kind true no-shadow
    panic-eased  if  -1 run-scene  then  partner-kind false no-shadow ;
: he-comes ( -- )   \ (stepping in with the key: Debilitas after her, the door locked behind)
    stalkers-stay state!  fiona hold  force-calm state!  fiona wait-move  fiona -1 idle
    no-track 1e background  15 screen:to-black-2 with-music or fade
    stalker-unloaded  summoner-on state-off  comes-scene
    world-held state!  found-file-209 fact!  far-door unlock  door-part show-part  ev-door object-hide
    chapel-door false open-mark  chapel-door lock  doors-redone  scene-5-pending state!
    fiona 3 -90 place-on-tri  hewie activate  castle-b1-6 209 0 hewie-to  hewie $D1 -90 place-on-tri
    castle-b1-6 206 stalker-to  partner activate  2 stalker-order  0 stalker-clock  stalker $CE 90 place-on-tri  stalker full-health
    cb1-6-flag1 fact!  2 1 player-camera  camera-restart  world-held state-off  force-calm state-off
    1 1e background  15 screen:from-black fade  wait-fade  stalkers-stay state-off ;
: doors-open ( -- )   \ (after it, the door open)
    1 0 player-camera  camera-restart  chapel-door false open-mark  chapel-door unlock  doors-redone ;
: he-goes-down ( -- )   \ (Debilitas down: the way out, a scene)
    no-track 1e background  stalkers-stay state!  fiona hold  fiona wait-move
    15 screen:to-black-2 fade  down-movie scenes:unseen-movie down-cutscene load-scene  wait-fade
    hewie let-go  panic-eased  if  -1 run-scene  then
    scene-5-pending state-off  stalker-unloaded  50 trust+
    fiona $11C -45 56.239e -3.659e place-at  $4E 0 57.799e 0.951e hewie-back
    fallen-part show-part  fallen-floor solid-floor floor-on  doors-open
    chapel-track 1e background  force-calm state-off  15 fade-in
    $28 resident-flag-set  $22F give-file  $8297 give-file  trust-file  stalkers-stay state-off ;
: falls-shots ( -- )   \ (the far view between its third and fourth shots; past $4AE the screen darkening)
    begin  cutscene-frame cutscene-shot-at 3 <> while  yield  repeat
    begin  cutscene-frame cutscene-shot-at 4 <> while  46e 46e 116.5e 116.5e depth-range  yield  repeat
    begin  cutscene-frame $4AE < while  yield  repeat
    begin  cutscene-frame 0< 0= while  cutscene-frame $4AE - 5 lshift $80 min screen-dark  yield  repeat ;
: calm-shots ( -- )   \ (the shards shown through its fifth shot)
    begin  cutscene-frame cutscene-shot-at  dup 5 = if  shard-part show-part  then  6 = if  shard-part hide-part  then
        cutscene-near? 0= while  yield  repeat ;
: ropes-down ( -- )  chandelier object-hide  left-lever object-show  right-lever object-show  rope-1 object-show  rope-2 object-show ;
: falls-scene ( -- )  falls-movie scenes:unseen-movie falls-cutscene load-scene  ['] falls-shots start drop  panic-eased  if  -1 run-scene  then ;
: calm-scene ( -- )  calm-movie scenes:unseen-movie calm-cutscene load-scene  ['] calm-shots start drop  panic-eased  if  calm-down-msg run-scene  then ;
: chandelier-falls ( -- )   \ (both stands broken: on him - two scenes)
    no-track 1e background  stalkers-stay state!  fiona hold  fiona wait-move
    hewie-no-attack state!  stalkers-blind state!
    15 screen:to-black-2 fade  falls-movie scenes:unseen-movie falls-cutscene load-scene  wait-fade
    hewie let-go  ['] falls-shots start drop  panic-eased  if  -1 run-scene  then  wait-fade
    $26 resident-flag-set  world-held state!  ropes-down  hewie let-go  stalker let-go  16 frames
    calm-scene  shard-part hide-part
    scene-5-pending state-off  hewie-no-attack state-off  stalkers-blind state-off  50 trust+  stalker-unloaded
    fiona $11C -45 56.239e -3.659e place-at  $4E 0 57.799e 0.951e hewie-back  doors-open
    chandelier-down object-show  fallen-floor solid-floor floor-on  found-file-20b fact!  panic-over
    chapel-track 1e background  force-calm state-off  15 fade-in
    $20B give-file  $8296 give-file  trust-file  $27 resident-flag-set  stalkers-stay state-off ;
: his-end ( -- )   \ (each frame, while he's after her: down - the way out; both stands broken - the chandelier)
    fighting? 0= if  exit  then
    st-cond-now @ down-cond =  stalker-told? and if
        chapel-door unlock  cb1-6-flag2 fact!  force-calm state!  panic-eased  ['] he-goes-down start drop  exit
    then
    cb1-6-flag4 fact?  cb1-6-flag5 fact? and if
        chapel-door unlock  cb1-6-flag2 fact!  force-calm state!  panic-eased  ['] chandelier-falls start drop
    then ;

\ ---- the statue: its inscription; the cane - the Saturnus Key (a scene: the castle opened up) ----
: key-scene ( -- )
    statue-movie scenes:keyed-movie statue-cutscene load-scene  wait-fade
    hewie let-go  creatures-on state-off  spirits-gone
    c1f-1-flag1 fact!  got-doc-luminessants fact!  cb1-6-flag3 fact!  found-file-227-2 fact!
    c2f-2-flag3 fact!  c2f-1-flag1 fact!  c1f-13-flag1 fact!
    $28 $C8 movie-key  panic-eased  if  -1 run-scene  then ;
: the-statue ( -- )
    stalkers-stay state!  fiona wait-move  fiona 160e 0e turn-to-xz  fiona look-low-anim 10 play-blend
    letters-msg say  15 screen:to-dim fade wait-fade  inscription-msg say  15 screen:from-dim fade wait-fade
    fiona look-at-anim start-anim  40 frames
    got-saturnus-key fact? if  deity-msg say  fiona wait-anim  stalkers-stay state-off  exit  then
    cane-msg yes?  fiona wait-anim  if
        no-track 1e background  15 screen:to-black-2 with-music or fade  key-scene
        fiona 0 idle  fiona $CD -90 place-on-tri  camera-restart
        hewie activate  castle-b1-6 401 0 hewie-to  hewie $191 0 place-on-tri  hewie full-health  hewie well
        chapel-door true open-mark  doors-redone  cane-part hide-part  got-saturnus-key fact!
        chapel-track 1e background  15 fade-in
        saturnus-key item-named  saturnus-key 1 give-item  saturnus-key show-item-tab  fanfare  obtained-2-msg say
        30 trust+
    then
    stalkers-stay state-off ;

\ ---- what she may say here ----
: the-chandelier ( -- )   \ (fallen: her first remark, then how near she came)
    chandelier-remarked @ 0= if  -1 chandelier-remarked !  chandelier-msg say exit  then
    fiona look-low-anim 10 start-blend  killed-msg say  fiona wait-anim ;
: lying-there ( -- )  fiona wait-move  fiona hands-clasped-anim 10 start-blend  not-moving-msg say  fiona wait-anim ;
: the-pulley ( -- )
    fiona wait-move
    found-file-20b fact? if  broken-msg say exit  then
    pulley-remarked @ if  0 pulley-remarked !  fastened-msg  else  -1 pulley-remarked !  pulley-msg  then  say ;
: spots ( -- )
    scene-5-pending state?  1 locked-request? and if  locked-msg any-time offer-remark  then
    fiona statue-area in-area?  fiona 90 50 heading? and if  ['] the-statue unhurried offer  then
    cb1-6-flag1 fact? 0=  cb1-6-flag2 fact? or if
        fiona pulley-1-area in-area?  fiona 50 10e -40e faces-point? and if  ['] the-pulley unhurried offer  then
        fiona pulley-2-area in-area?  fiona 50 10e 40e faces-point? and if  ['] the-pulley unhurried offer  then
    then
    fiona chandelier-area 50 faces-area? if
        found-file-20b fact? if  ['] the-chandelier unhurried offer
        else  cb1-6-flag2 fact? if  ['] lying-there unhurried offer  then  then
    then ;

\ ---- stepping in with the key, the first time a visit: he comes ----
: stepping-in ( -- )
    got-saturnus-key fact?  cb1-6-flag1 fact? 0= and  cb1-6-flag2 fact? 0= and  met @ 0= and
    fiona step-in-area entered? and if  -1 met !  panic-eased  ['] he-comes start drop  then ;

\ ---- the movies replayed (the extras) ----
: replayed ( -- )  stalker-unloaded  replay-over ;
: replay-statue ( -- )
    fiona hold  cane-part show-part  door-part show-part  ev-door object-hide
    chandelier object-show  chandelier-down object-hide  shard-part hide-part
    fiona look-at-anim start-anim  40 frames
    statue-movie scenes:keyed-movie statue-cutscene load-scene  $28 $C8 movie-key  panic-eased  if  -1 run-scene  then
    cane-part hide-part  replay-over ;
: replay-comes ( -- )
    fiona hold  chandelier object-show  chandelier-down object-hide  shard-part hide-part  fiona -1 idle
    comes-scene  door-part show-part  ev-door object-hide  replayed ;
: replay-falls ( -- )
    fiona hold  stand-1 object-hide  stand-1-part show-part  stand-2 object-hide  stand-2-part show-part
    chandelier object-show  chandelier-down object-hide  shard-part hide-part
    debilitas-27 stalker-ready  yield  falls-scene  replayed ;
: replay-calm ( -- )
    fiona hold  shard-part hide-part  debilitas-27 stalker-ready  yield  ropes-down
    hewie true shown  hewie let-go  stalker let-go  16 frames  calm-scene  shard-part hide-part  replayed ;
: replay-down ( -- )
    fiona hold  chandelier object-show  chandelier-down object-hide  shard-part hide-part
    debilitas-27 stalker-ready  yield
    down-movie scenes:unseen-movie down-cutscene load-scene  panic-eased  if  -1 run-scene  then  replayed ;
: replaying ( -- )
    replay-act case
        $C of  ['] replay-statue  endof   $D of  ['] replay-comes  endof   $E of  ['] replay-falls  endof
        $F of  ['] replay-calm  endof    $10 of  ['] replay-down  endof   >r 0 r>
    endcase  ?dup if  start drop  then ;

\ ---- every frame ----
create cameras  1 , 0 , -1 ,   2 , 1 , 0 ,   3 , 1 , 0 ,   4 , 2 , 1 ,   8 , 0 , -1 ,   9 , 1 , 0 ,   $A , 0 , -1 ,   $B , 1 , 0 ,   \ (area, set, path)
create entries  0 , -1 ,   \ (by exit 0: the camera)
create looks  166.49e 6e 0.6e  36e 36e 36e look,   \ (the statue)
: each-frame ( -- )
    events-held state? if  exit  then
    0 exit-out  cameras 8 cameras-at
    stepping-in  stands-struck  his-end  spots  looks 1 hewie-looks-at ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  replaying exit  then
    entries 1 ways-in ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-b1-6 place!
