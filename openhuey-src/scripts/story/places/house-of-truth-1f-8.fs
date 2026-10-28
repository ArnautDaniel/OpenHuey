\ story/places/house-of-truth-1f-8.fs - the House of Truth's 1F corridor of flames ($91): brought here by
\ the forced way, the first tremor (she crouches; "I should crouch down during large tremors..." - the
\ house's tremors from then on, Lorenzo sent ahead); the door snagged on the far side (broken through by
\ her blows, or Hewie leaping in at her "go for it"); once the house is coming down, its stones falling,
\ the drone, and her first step in taking her on; the three flames stirred; the Magist Medallion Hewie
\ sniffs out; the clock; two creatures let out; its sounds in turn; the slam's shake. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/house-of-truth-1f-8.fs
\ (converted from the game's, not loaded).
IN: story.places.house-of-truth-1f-8
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names effects story.kit story.places.house-of-truth fiona.anims fiona.sounds ;

own-reset  truth-owns owns !

0 constant snagged-msg     \ "Something seems to be snagged on the other side of the door, keeping it from opening. D..."
2 constant tremor-msg      \ "Whoa! What was that? It felt like a tremor... I should crouch down during large tremors..."
own: snag-tried            \ she has tried the snagged door (her rattle, once a visit)
own: going-on              \ her first step in, the house coming down: on (once a visit)
own: drone-on              \ the drone going
3 constant magist-fx   $A constant clock-area   $B constant creatures-area   6 constant going-area
$EB constant snagged-door   $91 constant snag-sound   5 constant first-ambient   5 constant tremor-track
$30 constant drone-sound   $47 constant drone-bank   $1FC constant brought-tri   0 constant snag-var
create flames  72.4e f, 17.2e f, 117.2e f,   101.9e f, 66e f, 87.1e f,   97.6e f, 29.5e f, 144.8e f,   101.9e f, 66e f, 87.4e f,
create flame-kinds  1 , 4 , 1 , 4 ,
create flame-slots  0 , 1 , 2 , 4 ,
: flame@ ( k -- ) ( F: -- x y z )  3 * floats flames +  dup f@  dup float+ f@  2 floats + f@ ;
: stones-fall ( -- )   \ (the ceiling coming down: stones of three kinds falling from 70)
    1 70e 0e 0e 0.5e story.words:stones-falling  3 70e 0e 0e 0.5e story.words:stones-falling
    11 70e 0e 0.5e 0.5e story.words:stones-falling ;
: drone ( -- )  drone-sound story-volume or drone-bank bank-sound  -1 drone-on ! ;
: came-in ( exit -- )
    room-sounds
    4 0 do  flame-slots i cells + @  flame-kinds i cells + @  i flame@  flame-at  loop
    ht1f-9-flag2 fact? if  0 barred-floor floor-on  0 show-part  then
    tremor-warned fact? if  tremor-track 1e background  then
    ht1f-8-magist-medallion-found fact?  ht1f-8-magist-medallion fact? 0= and if  magist-fx 86e 1e 68e flicker-at  then
    forced-exit = if  -1 drone-on !  else  tremors  ht1f-9-flag2 fact? if  stones-fall  then  then ;

\ ---- brought here: the first tremor ----
: shakes ( -- )   \ (an activity: growing, the thump, dying - slight then, till stopped)
    1 $80 30 rumble-pad  30 0.5e shaken  0 0 30 rumble-pad  1 $FF 30 rumble-pad  quake-sound quake-bank bank-sound
    30 1e shaken  1 $80 30 rumble-pad  30 0.5e shaken
    begin  0.1e cam-shake!  yield  again ;
: first-tremor ( -- )
    fiona hold  0 prepare-exit  fiona wait-move  over-music-f fact!
    this-room @ 493 0 hewie-to  hewie $1ED 45 place-on-tri
    partner activate  house-of-truth-1f-9 -1 stalker-to  0 stalker-order  house-of-truth-1f-5 stalker-slot 1 doors-into
    hewie-no-attack state!  camera-restart  stones-fall  drone
    15 screen:from-black fade  wait-fade  ['] shakes start drop  8 frames
    seized-sound common-bank fiona sound-by  fiona crouch-down-anim play  fiona crouching-anim start-anim  30 frames
    fiona crouch-up-anim start-anim  tremor-msg say  tremor-warned fact!  tremor-track 1e background  tremor-warned-2 fact!
    ['] shakes stop-doing  tremors  5 snag-var var!  stalkers-stay state-off ;

\ ---- the house coming down: her first step in takes her on ----
: goes-on ( -- )
    stalkers-stay state!  fiona hold  2 prepare-exit  15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade
    world-held state!  forced-exit take-exit ;

\ ---- the snagged door: her blows wear it through, or Hewie leaps in at it ----
: opened ( -- )  snagged-door unlock  0 true doors-actor send set-door-opened  snagged-door true open-mark ;
: snag-shut? ( -- flag )  0 exit-open? 0= ;
: struck ( -- )   \ (her blow at it: worn by it - 1 the weak blow, else 5 - through at 0)
    snag-shut? if
        snag-var var@  fiona-sub-now @ story.words:weak-blow-sub = if  1  else  5  then  -  0 max  snag-var var!
        snag-sound common-bank -37e 10e 177e sound-there
        snag-var var@ 0= if  opened  then
    then  16 frames ;
: leaps-in ( -- )   \ (Hewie, at her "go for it": a bark, round to it, a leap - through)
    hewie hold  hewie-bark  hewie wait-move
    hewie $202 -73 -1 walk-path-move -4.422e 167.237e go-to
    $90 50 -32e 0.5e 177.5e hewie-leap  8 frames
    snag-shut? if  snagged-door unlock  snag-sound common-bank -37e 10e 177e sound-there  opened  then  hewie wait-move ;
: tries-snagged ( -- )   \ (the first time: to it, her rattle; then its remark)
    snag-tried @ 0= if
        fiona 0 to-door  fiona rattle-behind-slow-anim play  fiona -1 idle  7 frames  -1 snag-tried !
    then  snagged-msg say ;
: snagged ( -- )
    snagged-door doors:door-locked? 0= if  exit  then
    -37e 0e 177e 7e 20e her-blow-in?  ['] struck running? 0= and if  ['] struck start drop  then
    hewie-in?  hewie-cond-now @ down-cond <> and  fiona $C in-area? and  hewie $C in-area? and
    fiona 60 -37e 177e faces-point? and  fiona-cmd-now @ 0= and  hewie-breakable-now @ and if  ['] leaps-in start drop  then ;

\ ---- what she may do here ----
: pick-magist ( -- )  magist-medallion ht1f-8-magist-medallion magist-fx 86e 68e picks-up ;
: magist-sniffed ( -- )  ht1f-8-magist-medallion-found ht1f-8-magist-medallion magist-fx  86e 1e 68e  hewie-finds ;
: spots ( -- )
    fiona clock-area in-area?  fiona -22 50 heading? and if  ['] save-clock calm-hewie-up offer  then
    8 locked-request?  fiona 0 before-door? and if  ['] tries-snagged any-time offer  then
    ht1f-8-magist-medallion-found fact?  ht1f-8-magist-medallion fact? 0= and  fiona 86e 1e 68e 5e 5e touches? and if  ['] pick-magist calm-only offer  then ;

\ ---- every frame ----
create entries  0 , 0 ,  0 , 0 ,  2 , 2 ,   \ (by exit 0 .. 2: the camera)
create cameras  6 , 0 , 0 ,  7 , 1 , 1 ,  8 , 1 , 1 ,  9 , 2 , 2 ,   \ (area, set, path)
create ahead  3 , 0 ,  4 , 1 ,  5 , 2 ,   \ (area, exit loaded ahead)
: each-frame ( -- )
    drone-on @ if  drone-sound story-volume or keep-playing or drone-bank bank-sound  then
    3 exits-out  cameras 4 cameras-at  ahead 3 ways-out
    ht1f-9-flag2 fact?  over-music-f fact? 0= and  going-on @ 0= and  fiona going-area entered? and if
        -1 going-on !  panic-eased  ['] goes-on fiona start-holding drop
    then
    snagged
    3 0 do  flame-slots i cells + @  i flame@  8e -4e  flame-stirred  loop
    ht1f-8-flag1 fact? 0=  fiona creatures-area entered? and if
        ht1f-8-flag1 fact!  this-room @ 0 467 $88 0 90 creature-facing  this-room @ 0 131 $89 0 -90 creature-facing
    then
    ht1f-8-magist-medallion-found fact? 0=  scrambling? and  hewie 86e 0e 68e 10e 5e touches? and if  ['] magist-sniffed start-only  then
    slam-shake
    room-sounds-in? if  first-ambient 48e 0e 115e ambient-sounds  then
    ht1f-9-flag2 fact?  drone-on @ 0= and if  drone  then
    spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    entries 3 ways-in
    came-by @ forced-exit = if
        fiona brought-tri -90 22e 100e place-at  0 0 player-camera  0e sounds-scaled
        ['] first-tremor start drop
    then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went house-of-truth-1f-8 place!
