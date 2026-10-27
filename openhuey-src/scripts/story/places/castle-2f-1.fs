\ story/places/castle-2f-1.fs - the music room in Belli Castle's 2F ($A): the third moon dial (turned
\ by the stick; set right with castle-1f-12's two, the moonlight scene and down to it; brought back
\ up by the forced way, the moon door unlocked and the creatures loose), the moonlight's scene
\ here as she comes up from castle-1f-12 with its dials right; the door unlocked from this side; the
\ piano, the speakers, the Refined Magnesia Hewie sniffs out, the Lavender, what Hewie looks up at,
\ the walls' shade. A place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/
\ castle-2f-1.fs (converted from the game's, not loaded).
IN: story.places.castle-2f-1
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects story.kit fiona.anims scenes ;

own-reset

\ ---- what is said here ----
0 constant device-msg      \ "Huh? What is this? Looks like it pivots at the base..."
1 constant moonlight-msg   \ "A device that attracts moonlight? How bizarre."
2 constant rotate-msg      \ "Rotate by pressing Right or Left on the left analog stick."
3 constant speakers-msg    \ "Loads of speakers line the walls in a haphazard fashion. I can feel a piercing stare co..."
4 constant piano-msg       \ "Tinkering out a few chords may soothe my nerves a bit... But now is hardly the time or..."

\ ---- its own state (a visit) ----
own: device-remarked
own: solved-here           \ the dials set right here, this visit: down to castle-1f-12 after the scene

\ ---- the room's things ----
0 constant magnesia-fx   1 constant lavender-fx
6 constant device-area   7 constant piano-area   8 constant speakers-area
1 constant moon-movie   0 constant moon-cutscene   $22 constant moon-door   $15 constant this-side-door
0 constant dial-base   3 constant right-number
: dial ( -- addr len var )  s" syuukouki" 3 ;
: moon-let-through? ( -- flag )  c1f-12-flag1 fact? ;
: solved? ( -- flag )  0 var@ 5 =  1 var@ 2 = and  3 var@ 3 = and ;   \ (castle-1f-12's two dials, this one)

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  moon-let-through? 0= if  room-sounds  then
    got-lunar-refractor fact?  c2f-1-flag1 fact? 0= and if  c2f-1-flag1 fact!  castle-2f-1 0 43 4 15 creature-in  then
    c2f-1-refined-magnesia-found fact?  c2f-1-refined-magnesia fact? 0= and if  magnesia-fx 39.5e 81e -38.8e flicker-at  then
    c2f-1-lavender fact? 0= if  lavender-fx 41.5e 69e 31e flicker-at  then
    dial dial-base dial-set  1 $2300 sound-reverb! ;

\ ---- the moonlight's scene: up from castle-1f-12 with its dials right (the stalker held through it),
\ or set right here - then down there by the forced way (or back, set from there) ----
: moon-scene ( -- )
    moon-movie scenes:keyed-movie moon-cutscene load-scene  castle-1f-12 preload  wait-fade
    world-frozen state!  $10 $FF movie-key  panic-eased  if  -1 run-scene  then
    world-held state!  world-frozen state-off ;
: stalker-kept ( -- )  stalker hold  begin  yield  again ;
: moon-comes ( -- )   \ (up from castle-1f-12)
    fiona hold  stalker-in? if  ['] stalker-kept start drop  yield  then  world-frozen state!
    moon-scene  solved-here @ if  forced-exit  else  forced-exit 1 or  then  take-exit ;
: moon-set ( -- )   \ (set right here: to black, the scene, down)
    -1 solved-here !  force-calm state!  15 screen:to-black-2 with-music or fade
    fiona hold  moon-scene  forced-exit take-exit ;
: moon-door-open ( -- )   \ (back up from castle-1f-12, set right: the moon door unlocked, the creatures loose)
    fiona hold  moon-door unlock  c1f-12-flag1 fact!  5 0 s" creatures" actor-named send creature-count
    force-calm state-off  stalkers-stay state-off  fiona let-go  camera-restart  0 prepare-exit
    15 screen:from-black with-music or fade ;

\ ---- the device: its dial turned by the stick; all three right, the moon ----
: the-device ( -- )
    stalkers-stay state!  fiona wait-move
    moon-let-through? if  moonlight-msg say  stalkers-stay state-off exit  then
    device-remarked @ 0= if  device-msg say  -1 device-remarked !  8 frames  then
    world-frozen state!  fiona $E6 62 -43.61e 1.45e place-at  1 11e 0e 0e 0e shot
    dial right-number dial-base rotate-msg  -36e -102e 5e  -2.88e  dial-turning
    fiona $EB 52 -42.2e -4.35e place-at  text-gone  shot-off  world-frozen state-off
    solved? if  moon-set exit  then
    stalkers-stay state-off ;

\ ---- what she may do or say here ----
: piano ( -- )  fiona wait-move  fiona 18e -16e turn-to-xz  piano-msg say ;
: speakers ( -- )  fiona wait-move  fiona 180 turn  speakers-msg say ;
: pick-magnesia ( -- )  refined-magnesia c2f-1-refined-magnesia magnesia-fx 39.5e -38.8e picks-up ;
: pick-lavender ( -- )  lavender c2f-1-lavender lavender-fx 41.5e 31e picks-up ;
: locked-exit ( -- exit )  this-side-door castle-2f-1 door-exit-in ;   \ (the door to unlock from this side)
: unlocks-it ( -- )  locked-exit unlocks ;
: spots ( -- )
    locked-exit before-locked? if  ['] unlocks-it any-time offer  then
    fiona device-area 50 faces-area? if  ['] the-device unhurried offer  then
    fiona piano-area 50 faces-area? if  ['] piano unhurried offer  then
    fiona speakers-area in-area?  fiona 90 50 heading? and if  ['] speakers unhurried offer  then
    c2f-1-refined-magnesia-found fact?  c2f-1-refined-magnesia fact? 0= and  fiona 39.5e 81e -38.8e 5e 5e touches? and if
        ['] pick-magnesia calm-only offer  then
    c2f-1-lavender fact? 0=  fiona 41.5e 69e 31e 5e 5e touches? and if  ['] pick-lavender calm-only offer  then ;

\ ---- Hewie: the Refined Magnesia he sniffs out, his looks up ----
create looks
    -35.43e 90e 3.42e  16e 12e 12e look,
    16.78e 80e -16.27e  23e 8e 8e look,
: magnesia-sniffed ( -- )  c2f-1-refined-magnesia-found c2f-1-refined-magnesia magnesia-fx  39.5e 81e -38.8e hewie-finds ;
: hewie-sniffs ( -- )
    c2f-1-refined-magnesia-found fact?  scrambling? 0= or if  exit  then
    hewie 39.5e 80e -38.8e 10e 5e touches? 0= if  exit  then
    ['] magnesia-sniffed start-only ;

\ ---- the walls' shade ----
: walls-shade ( -- )
    -3e 80e 3e  -10e 80e -10e  -2.1e 65e 3e  -10e 65e -10e  light-doorway
    10e 80e 10e  -3e 80e 3e  10e 65e 10e  -3e 65e 3e  light-doorway
    50e 100e 10e  10e 100e 10e  50e 40e 10e  10e 40e 10e  light-doorway
    -10e 80e -10e  -10e 80e -30e  -10e 30e -10e  -10e 30e -30e  light-doorway
    -26e 120e 8e  -26e 120e 13e  -26e 90e 8e  -26e 90e 13e  light-doorway ;

\ ---- the movie replayed (the extras): the moonlight - then on to castle-1f-12's ----
: replayed ( -- )
    fiona hold  fiona false shown
    moon-movie scenes:keyed-movie moon-cutscene load-scene  castle-1f-12 preload  $10 $FF movie-key
    panic-eased  if  -1 run-scene  then
    fiona true shown  forced-exit take-exit  1 $2C var! ;

\ ---- every frame ----
create cameras  4 , 0 , 0 ,   5 , 1 , 1 ,   9 , 0 , 0 ,   $A , 2 , -1 ,   \ (area, set, path)
create ahead  2 , 0 ,   3 , 1 ,   \ (area, exit)
create entries  1 , 1 ,   0 , 0 ,   \ (by exit 0, 1: the camera)
: each-frame ( -- )
    events-held state? if  exit  then
    2 exits-out  cameras 4 cameras-at  ahead 2 ways-out  hewie-sniffs  looks 2 hewie-looks-at  spots  walls-shade ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    entries 2 ways-in
    $81 forced-way? if
        fiona $EB 50 -41.573e -3.787e place-at  1 1 player-camera
        came-by @ forced-exit = if  ['] moon-comes  else  ['] moon-door-open  then  start drop
    then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-2f-1 place!
