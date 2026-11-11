\ story/places/water-tower-b1-1.fs - the Water Tower's B1 ($C0), Riccardo's dungeon: brought in by
\ the forced way from the old mansion (the "2 or 3 days" scene), she wakes in the cell - its scene
\ (movie 1), Riccardo let loose as the tinted stalker; the cell - the key on the desk, the cell's door
\ she can't force, Hewie sent for the key (its scene, movie 5) or rattling the door; out of it the
\ first time by the far side, Riccardo's scene (movie 3, his light by its cues); three rusted pipes
\ that burst at a blow (shards, a noise; the first knocks her over), two leaving a thing to
\ pick up; the map and the memo, the figurine, the disc; the Viride Medallion Hewie digs up; the
\ Sublimatum container (what she lays in it changed: Items_Offer's event $14); the gap she crawls
\ into to hide, Riccardo searching it; the hole into the hidden room ($27) and back; the locked door;
\ the flames stirring as someone passes; the ceiling dripping; Hewie's looks; the replays. A place
\ (story/kit.fs). The original's scripts were never converted (its tables had no names in the symbol
\ file): ported from the game's bytecode (Room C0's, 0x42CA20..) and src/game/rooms/room_C0.c.
IN: story.places.water-tower-b1-1
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names vectors
    floor-flags items.names story.kit fiona.anims hewie.anims hewie-names stalker.anims effects scenes ;

own-reset

\ ---- what is said here ----
0 constant capture-msg      \ "Hewie? You saved me again." (the waking scene's subtitles)
2 constant riccardo-msg     \ "Fiona! Damn you, you worthless mutt! Ah... The formula is working, then..."
3 constant key-scene-msg    \ "There you are boy! Hewie... Thank you Hewie!"
4 constant key-msg          \ "Maybe the key attached to the side of the desk unlocks this cell? ..."
5 constant scared-msg       \ "There has to be a way to get that key. This place is really scaring me..."
6 constant not-strong-msg   \ "I'm not that strong! I'll need to find another way."
7 constant map-msg          \ "This looks like a map of sorts..."
8 constant memo-msg         \ "There's a memo here..."
9 constant hole-msg         \ "Creaking, ominous sounds ... Enter the hole? YES NO"
$A constant locked-msg      \ "It's locked. I can't open it. Another exit... There has to be one somewhere!"
$B constant container-msg   \ "Sublimatum Container  May the wise sage, seer of all, show the testament..."
$C constant mansion-msg     \ "Sublimatum? It looks like that same device that I saw in the mansion before..."
$D constant changing-msg    \ "It-it's changing... transforming!"
$E constant pipe-msg        \ "This water pipe is rusted badly. I bet even I could break this..."

\ ---- the room's things ----
$FE constant hole-way   $27 constant hidden-room   $FA constant cell-door   $103 constant locked-door   $FF constant old-door
1 constant waking-movie   0 constant waking-cutscene   \ (the room's strings: EV0023, its movie)
3 constant come-movie   2 constant come-cutscene          \ (his scene: movie 3)
5 constant key-movie   4 constant key-cutscene            \ (Hewie's key: movie 5)
0 constant key-part   1 constant cell-open-part   8 constant disc-part   9 constant map-part   $A constant figurine-part
6 constant miniature-fx   7 constant viride-fx   8 constant album-fx   9 constant jerky-fx
$23 constant searches-var   \ (the story's variable: how often he has come while she hid here)
$3F constant hidden-room-file   $8286 constant cell-file   $8287 constant rattle-file   $8288 constant key-file   $8289 constant riccardo-file
$24C constant dungeon-file
3 constant changing-sound   5 constant key-sound   6 constant rattle-sound   7 constant burst-sound   $2E constant command-sound
$7E constant slide-sound   $43 constant cry-sound
own: rattles          \ Hewie sent at the cell's door (three times at most: the original's script variable 0)
own: key-asked        \ she looked at the key on the desk once (her remark the second time)
own: container-asked  \ the container read (its name, then her own remark, in turn)
own: dog-noticed      \ Hewie barked at the far corner (once a visit)
own: sound-started    \ the room's steady sound begun
own: laid-item        \ what she lays in the container
variable key-count    \ the original's event counter: Hewie's fetching done

\ ---- the cell's door: "ori", swung about its rest (Room C0's cmd 0) ----
create ori-rest  /vec allot
fvariable ori-phase   fvariable ori-swing
: ori ( -- addr len )  s" ori" ;
: ori-at-rest ( -- )   \ (byte 0: from where it is, a quarter of a unit)
    ori place-x object@  ori place-y object@  ori place-z object@  ori-rest vec!
    0e ori-phase f!  0.25e ori-swing f! ;
: ori-step ( -- swinging? )   \ (byte 1: 60 degrees on, a twentieth less, at rest + swing x sin both ways)
    ori-phase f@ 1.0472e f+  fdup pi f> if  2e pi f* f-  then  ori-phase f!
    ori-swing f@ 0.05e f- ori-swing f!
    ori-rest vec@ fdrop fswap fdrop  ori-swing f@ ori-phase f@ fsin f* f+  ori place-x object!
    ori-rest vec@ fswap fdrop fswap fdrop  ori-swing f@ ori-phase f@ fsin f* f+  ori place-z object!
    ori-swing f@ f0> ;
: rattles-shut ( -- )  ori-at-rest  begin  ori-step while  yield  repeat ;
: ori-open ( -- )  -16.5e ori place-x object!  -5.6017e ori place-z object! ;   \ (byte 2: swung open)

\ ---- as she comes in ----
create flames   74.12e f, 19.12e f, -207.3e f,   28.1e f, 18.9e f, -256.9e f,   -34.25e f, 19.17e f, -268.5e f,
                -73.7e f, 19.5e f, -232.1e f,   -42.14e f, 19.57e f, -186.96e f,   23.85e f, 18.77e f, -187.24e f,
: flame@ ( k -- ) ( F: -- x y z )  3 * floats flames +  dup f@  dup float+ f@  2 floats + f@ ;
: lit ( -- )  6 0 do  i 1  i flame@ flame-at  loop ;   \ (six flames, kind 1)
: two-parts ( first burst? -- )   \ (a pipe whole - its first part - or burst - its second)
    {: k on :}  on if  k hide-part  k 1+ show-part  else  k show-part  k 1+ hide-part  then ;
: came-in ( exit -- )
    drop  room-sounds  0 sound-started !
    out-of-cell fact? if  cell-open-part show-part  ori-open  then
    wtb1-disc fact? 0= if  disc-part show-part  then
    wtb1-map fact? 0= if  map-part show-part  then
    wtb1-figurine fact? 0= if  figurine-part show-part  then
    lit
    2 wtb1-pipe-1-burst fact? two-parts
    4 wtb1-pipe-2-burst fact? two-parts
    wtb1-pipe-2-burst fact?  wtb1-miniature fact? 0= and if  miniature-fx -48e 1e -296e flicker-at  then
    6 wtb1-pipe-3-burst fact? two-parts
    wtb1-pipe-3-burst fact?  wtb1-album-medallion fact? 0= and if  album-fx 84e 1e -220e flicker-at  then
    tintstalker-kind partner-is? if  no-stalker-camera state!  then
    wtb1-viride-found fact?  wtb1-viride fact? 0= and if  viride-fx -42.27e 1e -5.28e flicker-at  then
    wtb1-beef-jerky fact? 0= if  jerky-fx 15.77e 1e -102.69e flicker-at  then
    effects:ceiling-drips
    1 $3FFF reverb ;

\ ---- the scenes ----
: waking-cues ( -- )   \ (act $1E, along the waking scene: dark through its first shot, lifting from frame 120 till its second)
    begin  1 scene-shot? 0= while  $80 screen-dark  yield  repeat
    begin  2 scene-shot? 0= while  $80  cutscene-frame 120 - 4 lshift -  0 max  screen-dark  yield  repeat ;
: his-light ( a -- )   \ (Room C0's cmd 1: full - his own layer; else between, his tint and the blend layer)
    dup $80 >= if  drop  partner-kind usual-layer layer exit  then
    0 max  partner-kind $23 layer  stalker swap 24 lshift $808080 or tint ;
: light-from ( from F: rate -- )  cutscene-frame swap - s>f f* f>s his-light ;
: light-to ( to F: rate -- )  cutscene-frame - s>f f* f>s his-light ;
: his-light-off ( -- )  partner-kind $11 layer  stalker $80808080 tint ;
: riccardo-cues ( -- )   \ (act $C, along his scene: his light up and down at its cues)
    his-light-off
    begin  $16A scene-at? 0= while  yield  repeat
    begin  $1A9 scene-at? 0= while  361 2.04e light-from  yield  repeat
    begin  $22B scene-at? 0= while  yield  repeat
    begin  $25E scene-at? 0= while  605 2.51e light-to  yield  repeat
    begin  $475 scene-at? 0= while  yield  repeat
    begin  $48C scene-at? 0= while  1140 5.57e light-from  yield  repeat
    begin  $49D scene-at? 0= while  yield  repeat
    begin  $4B4 scene-at? 0= while  1203 5.57e light-to  yield  repeat ;
: waking-scene ( -- )   \ (movie 1: the key shown on the desk)
    key-part show-part
    waking-movie scenes:unseen-movie waking-cutscene load-scene  ['] waking-cues along-scene
    panic-eased  if  capture-msg run-scene  then ;
: riccardo-scene ( -- )
    come-movie scenes:unseen-movie come-cutscene load-scene  ['] riccardo-cues along-scene
    panic-eased  if  riccardo-msg run-scene  then  his-light-off ;
: key-scene ( -- )
    cell-open-part show-part
    key-movie scenes:unseen-movie key-cutscene load-scene  panic-eased  if  key-scene-msg run-scene  then ;

\ ---- brought here: she wakes in the cell (act 0) ----
: wakes ( -- )
    cell-door true open-mark  doors-redone  fiona send calm-reset  0 panic-to
    stalker let-go  hewie let-go  x-37-flag2 fact!  1 0 0 stage-music
    waking-scene
    locked-door lock  old-door closed-off  exits-redone
    fiona $46 100 -24.85e -2.25e place-at
    hewie activate  water-tower-b1-1 $1B3 0 hewie-to  hewie $1B3 -90 -5.63e -7.33e place-at   \ (he found her: in here, by the cell)
    6 6 player-camera  camera-restart
    0 barred-floor floor-on
    stalker-unloaded  tintstalker-kind stalker-ready
    cell-door false open-mark  doors-redone
    15 fade-in
    $3B resident-flag-set  dungeon-file give-file  cell-file give-file
    plain-commands state-off  hewie-commandable state! ;

\ ---- the cell ----
: the-key ( -- )   \ (act $A: the key on the desk - the first time a look at it)
    fiona wait-move
    key-asked @ if  scared-msg say exit  then
    4 frames  fiona 7 -1 camera-set  4 frames  key-msg say  4 frames  6 6 player-camera  -1 key-asked ! ;
: rattle ( -- )  ['] rattles-shut start drop ;
: tries-the-door ( -- )   \ (act $B: kicked - the door rattles; too strong for her)
    rattle  fiona wait-move  fiona wait-anim  fiona -1 idle  7 frames  not-strong-msg say ;
: fetches-key ( -- )   \ (act 3, Hewie's: a bark, to the desk, the key; the original's event counter)
    hewie hold  hewie wait-move  hewie-bark  hewie wait-move
    hewie $223 90 -1 walk-move -2.55e -13.07e go-to
    key-part hide-part  key-sound room-bank hewie sound-by
    30 frames  prison-key item-named  prison-key 1 give-item  prison-key show-item-tab  fanfare  obtained-2-msg say
    30 frames  1 key-count ! ;
: sends-for-key ( -- )   \ (act 2: her command - he fetches it; the door opened, out)
    0 key-count !  fiona wait-move
    fiona go-for-it-gesture-anim start-anim  fiona 1 wait-mark  command-sound common-bank fiona sound-by  fiona wait-anim
    fiona -1 idle  7 frames
    ['] fetches-key hewie start-holding drop
    begin  key-count @ 1 < while  yield  repeat
    15 screen:to-black-2 with-music or fade  wait-fade
    stalker-unloaded
    key-scene
    50 trust+  prison-key spend
    0 barred-floor floor-off
    fiona $A7 99 -20.21e 10.24e place-at  hewie $1C2 -83 -8.71e 10.17e place-at
    out-of-cell fact!  ori-open
    6 6 player-camera  camera-restart
    cell-door true open-mark  doors-redone
    15 fade-in
    key-file give-file  $3C resident-flag-set ;
: door-rattled ( -- )   \ (act 5: a moment, then the door swung)
    $16 frames  rattle-sound room-bank hewie sound-by  rattles-shut ;
: runs-at-door ( -- )   \ (act 4, Hewie's: a bark, at the door - it rattles - and back)
    hewie hold  hewie wait-move  hewie-bark  hewie wait-move
    hewie $BC -35 -1 walk-path-move 7.04e -29.24e go-to
    ['] door-rattled start drop
    $1B3 10 -7.13e 1e -9.05e hewie-leap  hewie wait-move
    1 rattles +!  10 trust+  rattle-file give-file ;
: in-the-cell ( -- )   \ (phase 1 while in it: her commands to Hewie; the door she kicks)
    out-of-cell fact? if  exit  then
    started-now @ go-there-cmd =  hewie held? 0= and  hewie-breakable-now @ and if   \ (cond $31: Hewie_MayBreakOff - he may be taken off what he does)
        ['] sends-for-key fiona start-holding drop
    then
    started-now @ come-back-cmd =  rattles @ 3 <> and  hewie-breakable-now @ and if
        ['] runs-at-door hewie start-holding drop
    then
    fiona -17e 0e 8e 10e 15e blow-in?  fiona held? 0= and  ['] tries-the-door running? 0= and if   \ (her kick at it: the rooms' zones test a character's point)
        rattle-sound room-bank fiona sound-by  panic-eased  ['] tries-the-door fiona start-holding drop
    then ;

\ ---- his scene: out of the cell's side the first time (act 1) ----
: riccardo-comes ( -- )
    riccardo-come fact!  stalker let-go  stalkers-stay state!  fiona hold
    15 screen:to-black-2 with-music or fade  wait-fade  world-held state!
    water-tower-b1-1 88 stalker-to  stalker let-go
    riccardo-scene
    fiona $337 130 82.6e -122.02e place-at  hewie $1DB 0 93.53e -113.35e place-at
    stalker activate  water-tower-b1-1 88 stalker-to  stalker 2 2 camera-set  his-own-time
    camera-restart  15 fade-in
    $3D resident-flag-set  riccardo-file give-file  stalkers-stay state-off
    summoner-on state!
    water-tower-7f-1 stalker-slot 1 doors-into  water-tower-7f-2 stalker-slot 1 doors-into  water-tower-8f-1 stalker-slot 1 doors-into ;

\ ---- the pipes: rusted through, they burst at a blow - hers, Hewie's, the stalker's - shards, a noise ----
: bursts ( first loud tri F: x z -- )   \ (its parts swapped; the room's way under it open; its shards, sound, noise)
    {: k loud tri F: x z :}
    0 keeps-people-off floor-off  k true two-parts
    $80202030 0 7  x 0e z  0e 0.218e 0.437e 0.156e  0e effects:shards
    burst-sound x 0e z room-sound  loud tri room-noise ;
: knocked-over ( -- )   \ (act $1D: the drifting flecks, a fright, the pad shaken, her cry - over she goes)
    -33e 2e -165e  50e 100e  0e -180e 0e  effects:flecks
    $19 frames  90 fright+  1 $FF 8 rumble-pad  cry-sound common-bank fiona sound-by
    fiona startled-anim play  fiona -1 idle  7 frames ;
: pipes ( -- )
    wtb1-pipe-1-burst fact? 0= if
        -33e 0e -167e 5e 8e struck-in? if  wtb1-pipe-1-burst fact!  2 $40 $19A -33e -167e bursts  then
        fiona -33e 0e -167e 5e 8e blow-in? if  ['] knocked-over fiona start-holding drop  then
    then
    wtb1-pipe-2-burst fact? 0= if
        -48e 0e -296e 5e 8e struck-in? if
            wtb1-pipe-2-burst fact!  4 $40 $1CA -48e -296e bursts  miniature-fx -48e 1e -296e flicker-at
        then
    then
    wtb1-pipe-3-burst fact? 0= if
        84e 0e -220e 5e 8e struck-in? if
            wtb1-pipe-3-burst fact!  6 $40 $278 84e -220e bursts  album-fx 84e 1e -220e flicker-at
        then
    then ;

\ ---- what she may take or look at here ----
: obtained ( item -- )  dup item-named  dup 1 give-item  dup show-item-tab  fanfare  drop obtained-2-msg say ;
: the-map ( -- )   \ (act 6: crouched to it - the Tower Sketch, then the memo under it)
    stalkers-stay state!  fiona wait-move  fiona -32e -15e turn-to-xz
    fiona crouch-down-anim play  fiona crouching-anim start-anim
    map-msg say  tower-sketch obtained  16 frames
    memo-msg say  folded-memo obtained
    wtb1-map fact!  map-part hide-part
    fiona crouch-up-anim play  fiona -1 idle  7 frames  stalkers-stay state-off ;
: on-the-wall ( item fact part F: x z -- )   \ (acts 7, 8: reached up for it)
    {: it f p F: x z :}  fiona wait-move  fiona x z turn-to-xz
    fiona reach-out-anim play  p hide-part  f fact!  it obtained  fiona reach-back-anim play ;
: the-figurine ( -- )  bronze-figurine wtb1-figurine figurine-part -11e -69e on-the-wall ;
: the-disc ( -- )  silver-disc wtb1-disc disc-part 18e -267e on-the-wall ;
: the-miniature ( -- )  wooden-miniature wtb1-miniature miniature-fx -48e -296e picks-up ;
: the-viride ( -- )  viride-medallion wtb1-viride viride-fx -42.27e -5.28e picks-up ;
: the-album ( -- )  album-medallion wtb1-album-medallion album-fx 84e -220e picks-up ;
: the-jerky ( -- )  beef-jerky wtb1-beef-jerky jerky-fx 15.77e -102.69e picks-up ;
: the-pipe ( -- )   \ (act $19)
    fiona wait-move  fiona look-low-anim start-anim  40 frames  pipe-msg say  fiona wait-anim ;
: the-locked-door ( -- )   \ (act $13: tried - in a panic or chased, a hurried rattle and nothing said)
    fiona $5D 0 -1 walk-move 96.497e -39.125e go-to  fiona wait-move
    in-panic? chased? or if  fiona rattle-behind-slow-anim play  fiona -1 idle  7 frames  exit  then
    fiona rattle-behind-anim play  fiona -1 idle  7 frames  locked-msg say ;
: at-container ( -- )  fiona $1E3 65 -1 walk-move 37.5e -56.6e go-to  fiona wait-move ;
: the-container ( -- )   \ (act $15: its words, read in the half dark; then her own remark)
    stalkers-stay state!  at-container
    container-asked @ 0= if
        15 screen:to-dim fade  wait-fade  container-msg say  -1 container-asked !
        15 screen:from-dim fade  wait-fade
    else
        fiona hand-to-mouth-anim 10 start-blend  mansion-msg say  fiona wait-anim  0 container-asked !
    then  stalkers-stay state-off ;

\ ---- the Sublimatum container: what she lays in it, changed (act $14) ----
create changes   \ (laid in, and what it becomes)
    withered-flowers , nigred ,   smudged-key , rubrum-medallion ,   prima-materia , refined-magnesia ,
    sulfuric-ore , refined-magnesia ,   silver-ore , refined-magnesia ,   white-godstone , refined-magnesia ,
    jupiter-key , album-medallion ,   film-reel , weeping-stone ,   cracked-plate-key , refined-antimony ,
    cracked-plate-key-2 , refined-magnesia ,   cathedral-secrets , magna-esca ,   occult-philosophy , sylvesta-esca ,
    wisemans-crafts , bennenam-esca ,   hebels-perfume , recreatio ,   castle-gate-key , magist-medallion ,
here changes - 2 cells / constant #changes
: changed-into ( item -- item' | -1 )   \ (the Red Godstone: the Philosopher Earrings, the Magist Medallion once she has them)
    dup red-godstone = if  drop  philosopher-earrings items:items-of 0> if  magist-medallion  else  philosopher-earrings  then  exit  then
    #changes 0 do  changes i 2* cells + @ over = if  drop changes i 2* 1+ cells + @  unloop exit  then  loop  drop -1 ;
: lays-in ( -- )
    laid-item @ {: it :}  stalkers-stay state!  at-container
    changing-sound room-bank fiona sound-by  $78 frames
    fiona look-close-anim play  changing-msg say
    fiona reach-out-anim play
    it changed-into dup 0< if  drop  else  it spend  obtained  then
    wtb1-container-used fact!  fiona reach-back-anim play  stalkers-stay state-off ;
: items-used ( who ev var value -- handled? )   \ (Items_Offer: event $14, the item in variable 1)
    {: who ev var value :}  who 0<>  ev $14 <> or  var 1 <> or if  false exit  then
    value laid-item !  ['] lays-in fiona start-holding drop  true ;

\ ---- the gap she crawls into to hide (act $F); hauled out, or out by herself (act $10) ----
: pulled-out ( -- )   \ (found, in a panic, or chased as she came out)
    found-her @ if  2 avoid-prompt  then
    fiona-hidden state-off  fiona follow  stalkers-stay state!
    fiona $E6 -90 out-of-breath-anim walk-move 36.42e -287e go-to
    fiona hug-self-anim $14 play-blend
    fiona false noclip  stalkers-stay state-off ;
: crawls-out ( -- )
    fiona-hidden state-off  fiona follow  stalker let-go
    chased? if  pulled-out exit  then
    stalkers-stay state!
    fiona $34F -90 $803 walk-move 59.29e -289.65e go-to
    fiona crouch-up-anim play  fiona false noclip  stalkers-stay state-off ;
: hides ( -- )
    stalkers-stay state!  0 came-while-hidden !  0 found-her !
    fiona wait-move  fiona $34F 90 -1 walk-move 59.29e -289.65e go-to
    fiona true noclip  fiona crouch-down-anim play
    fiona $2F6 90 $803 walk-move 77e -287e go-to  fiona $2F6 -90 $803 walk-move 77e -287e go-to
    fiona crouching-anim start-anim
    fiona-hidden state!  stalkers-stay state-off  0 avoid-prompt
    begin
        wants-out? if  crawls-out exit  then
        in-panic? found-her @ or if
            in-panic? 0= if  cry-sound common-bank fiona sound-by  then  pulled-out exit
        then
        2 ease-panic  6 calm-her  $1E recover
        came-while-hidden @  stalker-in? 0= and if  0 came-while-hidden !  1 avoid-prompt  then
        yield
    again ;
: finds-her ( -- )   \ (act $12, his: to the gap - "there!" - she is his)
    stalker hold  stalker wait-move
    stalker $351 117 -1 walk-path-move 52.99e -276.09e go-to  stalker wait-move
    stalker there-anim play  -1 found-her !  stalker 10 idle
    begin  fiona held? while  yield  repeat  0 stalker-order ;
: search-odds ( -- pct )  searches-var var@ case  1 of 25 endof  2 of 50 endof  3 of 75 endof  >r 0 r>  endcase ;
: stalker-came ( -- )   \ (act $11: in by the way she is by while she hides - by the odds he finds her)
    fiona-hidden state?  1 stalker-came-by? and 0= if  0 came-while-hidden !  exit  then
    -1 came-while-hidden !
    tintstalker-kind partner-is? 0= if  stalker follow  then
    search-odds chance?  2 creature-moving? or if  ['] finds-her start drop  then
    searches-var var@ 1+ searches-var var! ;

\ ---- the hole into the hidden room ($27) and back ----
: into-hole ( -- )   \ (act $D: a close look; yes - in, the world held)
    stalkers-stay state!  fiona wait-move  fiona -114e -114e turn-to-xz
    fiona look-close-anim 10 play-blend
    hole-msg yes? if
        15 screen:to-black fade  wait-fade  slide-sound common-bank fiona sound-by
        world-held state!  hole-way take-exit
    else  stalkers-stay state-off  then ;
: out-of-hole ( -- )   \ (act $E)
    fiona wait-move  camera-restart  slide-sound common-bank fiona sound-by  0 map-page
    15 screen:from-black fade  wait-fade  stalkers-stay state-off ;

\ ---- Hewie: his looks; the far corner's bark (once a visit); digging up the Viride Medallion ----
create looks
    22.18e 0e -273.16e 20e 15e 15e look,   -4.82e 0e -65.32e 20e 16e 16e look,
    37.5e 0e -56.6e 25e 18e 18e look,       -105.11e 0e -117.43e 20e 17e 17e look,
create pipe-2-look  -45.98e 0e -290.99e 13e 10e 0e look,
create pipe-1-look  -35.15e 0e -173.08e 13e 10e 0e look,
create pipe-3-look  78e 0e -221.58e 13e 10e 0e look,
: barks-at-corner ( -- )  -105.11e 17e -117.43e hewie-barks-at ;
: hewie-about ( -- )
    looks 4 hewie-looks-at
    wtb1-pipe-2-burst fact? 0= if  pipe-2-look 1 hewie-looks-at  then
    wtb1-pipe-1-burst fact? 0= if  pipe-1-look 1 hewie-looks-at  then
    wtb1-pipe-3-burst fact? 0= if  pipe-3-look 1 hewie-looks-at  then
    dog-noticed @ 0=  hewie-in? and  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and
    hewie -105.11e 0e -117.43e 20e 17e centred-in? and if
        -1 dog-noticed !  30 chance? if  ['] barks-at-corner start drop  then
    then ;
: viride-sniffed ( -- )  wtb1-viride-found wtb1-viride viride-fx  -42.27e 1e -5.28e  hewie-finds ;

\ ---- every frame ----
create cameras   \ (area, set, path)
    1 , 5 , 5 ,   2 , 0 , 0 ,   3 , 1 , 1 ,   4 , 1 , 1 ,   5 , 2 , 2 ,   6 , 3 , 3 ,   7 , 2 , 2 ,   8 , 2 , 2 ,
    9 , 4 , 4 ,   $A , 2 , 2 ,   $B , 5 , 5 ,   $C , 4 , 4 ,   $D , 5 , 5 ,   $E , 0 , 0 ,   $F , 6 , 6 ,
create ahead  $18 , 1 ,   \ (area, exit loaded ahead)
: facing? ( area deg within -- flag )  {: area deg within :}  fiona area in-area?  fiona deg within heading? and ;
: flames-stir ( -- )  6 0 do  i  i flame@ 8e -4e flame-stirred  loop ;
: room-sound-on ( -- )   \ (the room's steady sound: begun once its bank is in, then kept up)
    sound-started @ 0= if
        room-sounds-in? if  8 -112e 16.5e -114e room-sound  -1 sound-started !  then  exit
    then  8 -112e 16.5e -114e keep-sound ;
: spots ( -- )
    fiona $19 in-area? if
        stalker-in? if  no-time-message offer-remark-now  else  ['] hides hiding-spot offer  then
    then
    wtb1-pipe-2-burst fact?  wtb1-miniature fact? 0= and  fiona -48e 0e -296e 5e 5e touches? and if  ['] the-miniature calm-only offer  then
    wtb1-map fact? 0=  fiona $12 50 faces-area? and if  ['] the-map unhurried offer  then
    out-of-cell fact? 0=  $11 90 50 facing? and if  ['] the-key unhurried offer  then
    wtb1-figurine fact? 0=  $13 -104 50 facing? and if  ['] the-figurine calm-only offer  then
    wtb1-disc fact? 0=  $14 -54 50 facing? and if  ['] the-disc calm-only offer  then
    $15 -90 50 facing? if  ['] into-hole unhurried offer  then
    fiona $1A in-area? if  ['] the-locked-door unhurried offer  then
    $1B 64 50 facing? if  ['] the-container unhurried offer  then
    wtb1-pipe-3-burst fact? 0=  $1C 106 60 facing? and if  ['] the-pipe unhurried offer  then
    wtb1-pipe-2-burst fact? 0=  $1D -158 60 facing? and if  ['] the-pipe unhurried offer  then
    wtb1-pipe-1-burst fact? 0=  $1E 12 60 facing? and if  ['] the-pipe unhurried offer  then
    wtb1-viride-found fact?  wtb1-viride fact? 0= and  fiona -42.27e 0e -5.28e 5e 5e touches? and if  ['] the-viride calm-only offer  then
    wtb1-pipe-3-burst fact?  wtb1-album-medallion fact? 0= and  fiona 84e 0e -220e 5e 5e touches? and if  ['] the-album calm-only offer  then
    wtb1-beef-jerky fact? 0=  fiona 15.77e 0e -102.69e 5e 5e touches? and if  ['] the-jerky calm-only offer  then ;
: each-frame ( -- )
    events-held state? if  flames-stir exit  then
    1 1 leaves-by  cameras 15 cameras-at  ahead 1 ways-out
    fiona $16 entered?  fiona $17 entered? or if  hidden-room preload  then
    riccardo-come fact? 0=  fiona $10 entered? and if  panic-eased  ['] riccardo-comes fiona start-holding drop  then
    in-the-cell
    flames-stir
    pipes
    time-up-leaves
    wtb1-viride-found fact? 0=  hewie -42.27e 0e -5.28e 10e 5e touches? and
    hewie-act-now @ dup scramble-act = swap 100 = or and  hewie-in? and if  ['] viride-sniffed start-only  then
    hewie-about
    room-sound-on
    spots ;

\ ---- the replays (the extras) ----
: replay-waking ( -- )  fiona hold  waking-scene  replay-over ;
: replay-key ( -- )  fiona hold  key-scene  replay-over ;
: replay-riccardo ( -- )
    fiona hold  tintstalker-kind stalker-ready  yield
    riccardo-scene  stalker-unloaded  replay-over ;
: person-in ( i -- )
    case
        fiona-p of
            events-held state? if
                hewie send part-from-fiona
                replay-act case  $1A of  ['] replay-waking  endof  $1B of  ['] replay-key  endof  >r ['] replay-riccardo r>  endcase
                fiona start-holding drop  exit
            then
            1 5 5 way-in
            came-by @ forced-exit =  cf-13-flag1 fact? and  x-37-flag2 fact? 0= and if
                fiona $4B 0 place-on-tri  world-held state!  0e sounds-scaled  panic-eased  ['] wakes fiona start-holding drop
            then
            came-by @ hole-way = if
                fiona $100 90 -103e -114e place-at  5 5 player-camera  ['] out-of-hole fiona start-holding drop
            then
        endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went water-tower-b1-1 place!
' items-used water-tower-b1-1 items-here
