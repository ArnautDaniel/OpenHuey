\ story/places/house-of-truth-2f-1.fs - the House of Truth's 2F hall ($82): her first steps in (her
\ track not yet on) take her on by the forced way (to Lorenzo's study); back from it, her clothes
\ and Hewie's coat as they were, the doors shut on her, Lorenzo let loose (the file); the locked
\ door - its remark in turn with the sun carved in it - and the Sol Key turned in it; the barred
\ corridor; three crates (broken by a blow: the Viola Medallion behind one, a creature let out
\ behind another); the Medallion Hewie sniffs out; eight flames, six stirred as anyone passes. A
\ place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/house-of-truth-2f-1.fs
\ in the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.house-of-truth-2f-1
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit story.places.house-of-truth fiona.anims ;

own-reset  truth-owns owns !

0 constant locked-msg      \ "The door is locked."
1 constant sun-msg         \ "A strange symbol is carved into the door. The symbol looks like a sun."
2 constant corridor-msg    \ "This room connects to another corridor... But it doesn't seem like I can get through."
own: locked-remarked       \ the locked remark made (in turn with the sun's)
own: taken-on              \ her first steps in: on (once a visit)
6 constant medallion-fx   7 constant viola-fx   $609 constant key-anim
$14 constant corridor-area   $E2 constant sun-door   $E6 constant other-door   $24F constant hall-file
$80202020 constant crate-grey
create flames  -102e f, 65.4e f, -31.4e f,   -43.2e f, 46e f, -31.2e f,   -72.4e f, 17.2e f, -1.75e f,
    -102e f, 66e f, 87.6e f,   -43e f, 46e f, 87.7e f,   -72.4e f, 17.2e f, 117.2e f,   -101.8e f, 66e f, -31e f,   -101.8e f, 66e f, 88e f,
create flame-kinds  4 , 1 , 1 , 4 , 1 , 1 , 4 , 4 ,
create flame-slots  0 , 1 , 2 , 3 , 4 , 5 , 8 , 9 ,
: flame@ ( k -- ) ( F: -- x y z )  3 * floats flames +  dup f@  dup float+ f@  2 floats + f@ ;
: crate-look ( bk group noise -- )
    {: bk group noise :}  bk crate-grey 0 5 shard-look  bk 0.812e 0.687e 0.187e 0.312e shard-uv  bk group frees-floor  bk noise noise-from ;
ht2f-1-breakable1-broken 0 1  -57.4e 70e 114.5e 5e 8e  breakable: crate-1     crate-1 0 $103 crate-look
ht2f-1-breakable2-broken 2 3  -17e 70e -41e 5e 8e  breakable: crate-2        crate-2 1 $95 crate-look
ht2f-1-breakable3-broken 4 5  -3e 70e -41e 5e 8e  breakable: crate-3         crate-3 2 $86 crate-look
: viola-shows ( -- )  viola-fx -17e 71e -41e flicker-at ;
: came-in ( exit -- )
    drop  crate-1 as-it-stands  crate-2 as-it-stands  crate-3 as-it-stands
    crate-2 broken?  ht2f-1-viola-medallion fact? 0= and if  viola-shows  then
    8 0 do  flame-slots i cells + @  flame-kinds i cells + @  i flame@  flame-at  loop
    ht2f-1-medallion-found fact?  ht2f-1-medallion fact? 0= and if  medallion-fx -50e 71e -15e flicker-at  then ;

\ ---- her first steps in: on, by the forced way ----
: taken ( -- )
    stalkers-stay state!  fiona hold  15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade
    0 s" creatures" actor-named send creatures-clear  scene-locked state!  0 prepare-exit  forced-exit take-exit ;
\ ---- back from the study: as she was; the doors shut on her, Lorenzo let loose ----
: back-again ( -- )
    fiona hold  fiona wait-move  camera-restart
    costume-var var@ dress  steps-in  coat-var var@ coat  yield
    other-door lock  $EA lock  $103 lock
    house-of-truth-1f-6 hewie-slot 1 doors-into  house-of-truth-1f-6 stalker-slot 1 doors-into  house-of-truth-1f-4 stalker-slot 1 doors-into
    partner activate  this-room @ 622 stalker-to  stalker $26E 180 place-on-tri  stalker 1 1 camera-set
    summoner-on state!  his-own-time  room-sounds  15 fade-in
    hall-file give-file  scene-locked state-off  stalkers-stay state-off ;

\ ---- the locked door: tried (its remark in turn with the sun's); the Sol Key turned in it ----
: the-door ( -- )
    fiona 0 to-door
    locked-remarked @ 0=  chased? or  in-panic? or if
        in-panic? chased? or if  fiona rattle-behind-slow-anim play  fiona -1 idle  7 frames
        else  fiona rattle-behind-anim play  fiona -1 idle  7 frames  locked-msg say  then
        -1 locked-remarked !  exit
    then
    4 frames  fiona $26E 0 -10e 135.3e place-at  world-frozen state!
    1 20e 0e 0e 2e shot  4 frames  sun-msg say  4 frames  shot-off  0 locked-remarked !  world-frozen state-off ;
: turns-key ( -- )  key-anim other-door sun-door sol-turned ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0<> if  drop false exit  then
    2 = if  ['] turns-key start drop  true  else  false  then ;

\ ---- what she may do or say here ----
: the-corridor ( -- )
    fiona wait-move  fiona $FB 90 -34e 99e place-at  1 46e 10e 150e 0e shot  corridor-msg say  16 frames  shot-off ;
: pick-medallion ( -- )  medallion ht2f-1-medallion medallion-fx -50e -15e picks-up ;
: pick-viola ( -- )  viola-medallion ht2f-1-viola-medallion viola-fx -17e -41e picks-up ;
: medallion-sniffed ( -- )  ht2f-1-medallion-found ht2f-1-medallion medallion-fx  -50e 71e -15e  hewie-finds ;
: spots ( -- )
    8 locked-request?  fiona 0 before-door? and if  ['] the-door any-time offer  then
    fiona corridor-area in-area?  fiona 45 50 heading? and if  ['] the-corridor unhurried offer  then
    ht2f-1-medallion-found fact?  ht2f-1-medallion fact? 0= and  fiona -50e 71e -15e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then
    crate-2 broken? if
        ht2f-1-viola-medallion fact? 0=  fiona -17e 71e -41e 5e 5e touches? and if  ['] pick-viola calm-only offer  then
    else  crate-2 breaks? if  viola-shows  then  then ;

\ ---- every frame ----
create entries  1 , 1 ,  0 , 0 ,  0 , 0 ,  6 , 6 ,  5 , 5 ,   \ (by exit 0 .. 4: the camera)
create cameras  8 , 0 , 0 ,  9 , 1 , 1 ,  $A , 0 , 0 ,  $B , 2 , 2 ,  $C , 2 , 2 ,  $D , 4 , 4 ,  $E , 4 , 4 ,  $F , 5 , 5 ,
    $10 , 6 , 6 ,  $11 , 7 , 7 ,  $12 , 3 , 3 ,  $13 , 7 , 7 ,   \ (area, set, path)
create ahead  5 , 0 ,  6 , 1 ,  7 , 4 ,   \ (area, exit loaded ahead)
: each-frame ( -- )
    over-music-d fact? if  0 exit-out  then  1 exit-out  2 exit-out  3 3 leaves-by  4 4 leaves-by
    cameras 12 cameras-at  ahead 3 ways-out
    over-music-d fact? 0=  taken-on @ 0= and  fiona 0 entered? and if
        -1 taken-on !  panic-eased  ['] taken fiona start-holding drop
    then
    crate-1 breaks? drop
    crate-3 breaks? if  this-room @ 1 147 $24 5 creature-in  then
    6 0 do  i  i flame@  8e -4e  flame-stirred  loop
    ht2f-1-medallion-found fact? 0=  scrambling? and  hewie -50e 70e -15e 10e 5e touches? and if  ['] medallion-sniffed start-only  then
    spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    entries 5 ways-in
    came-by @ forced-exit = if
        fiona $C3 180 -9e 95e place-at  1 1 player-camera
        this-room @ 194 1 hewie-to  hewie $C2 0 place-on-tri  ['] back-again start drop
    else  room-sounds  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went house-of-truth-2f-1 place!
' items-used house-of-truth-2f-1 items-here
