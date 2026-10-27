\ story/places/old-mansion-1f-7.fs - the extractor's room of the old mansion's 1F: the extractor (its
\ plaque, and the file of it; the Prima Materia put in, Sulfuric Ore out - anything else, nothing);
\ the large device; the high windows ("If I get caught now..."); brought in by the forced way the
\ first time, the scene ("come with me...") and the chapter's turn - out by the forced way, the
\ mansion's doors anew, the stalker in; the Album Medallion Hewie sniffs out; his looks; its hum.
\ A place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-1f-7.fs
\ (converted from the game's, not loaded).
IN: story.places.old-mansion-1f-7
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    items.names story.kit fiona.anims scenes ;

own-reset

0 constant come-msg          \ "Miss Fiona... Now, if you would please come with me... No, I don't want to. Ple..."
1 constant plaque-msg        \ "Extractor Born from the swirling sands of chaos, the primordial element, Prima Mate..."
2 constant extractor-msg     \ "An extractor? This place is full of odd inventions that I've never heard of. It app..."
3 constant nothing-msg       \ "No extraction occurred. I guess it didn't work... Maybe I should try a different item?"
4 constant caught-msg        \ "Why is this happening to me?! Dear god! If I get caught now, I'm sure they will kil..."
5 constant no-luck-msg       \ "No luck. I can't see any higher than this."
6 constant changed-msg       \ "The composition of this item has changed!"
7 constant device-msg        \ "It's quite a large and complicated device. It looks like nutrients are fed in throu..."
8 constant device-2-msg      \ "However, come to think of it, maybe nutrients are not fed in... Perhaps they are su..."
own: plaque-read  own: caught-remarked  own: device-remarked  own: sounds-on  own: pod-noticed  own: put-in
0 constant medallion-fx   0 constant hum-sound   3 constant change-sound   $242 constant extractor-file
1 constant come-movie   0 constant come-cutscene   5 constant kept-figure
1 constant changes-it   2 constant nothing-from-it   \ (the items' events here: items.fs)

\ ---- the extractor: its plaque; the Prima Materia put in, Sulfuric Ore out; else nothing ----
: at-extractor ( -- )  stalkers-stay state!  fiona hold  fiona wait-move  fiona $50 -90 -57e 55e walk-to ;
: the-extractor ( -- )
    at-extractor
    plaque-read @ if
        fiona hand-to-mouth-anim 10 start-blend  extractor-msg say  fiona wait-anim  0 plaque-read !
    else
        15 screen:to-dim fade wait-fade  plaque-msg say  -1 plaque-read !  15 screen:from-dim fade wait-fade
    then
    extractor-file give-file  stalkers-stay state-off ;
: extracting ( -- )  change-sound room-bank fiona sound-by  120 frames  fiona look-close-anim play ;
: given-back ( item -- )  dup item-named  show-item-tab  fanfare  obtained-2-msg say ;
: changes ( -- )
    at-extractor  extracting  changed-msg say
    fiona reach-out-anim play  prima-materia spend  sulfuric-ore item-named  sulfuric-ore 1 give-item
    sulfuric-ore show-item-tab  fanfare  obtained-2-msg say  fiona reach-back-anim play  stalkers-stay state-off ;
: nothing-comes ( -- )
    at-extractor  extracting  fiona reach-out-anim play  put-in @ given-back  fiona reach-back-anim play
    nothing-msg say  stalkers-stay state-off ;
: items-used ( who ev var value -- handled? )
    {: who ev var it :}  who 0<> if  false exit  then
    ev changes-it = if  ['] changes start drop  true exit  then
    ev nothing-from-it = if  it put-in !  ['] nothing-comes start drop  true exit  then
    false ;

\ ---- what she may do or say here ----
: high-window ( tri deg F: x z -- )   \ (the first time stood up to it, her fear; then no luck)
    {: tri deg F: x z :}
    fiona wait-move  fiona 0e 0e turn-to-xz
    caught-remarked @ if  no-luck-msg say exit  then
    4 frames  fiona tri deg x z place-at  1 10e -10e 0e 3e shot  world-frozen state!
    caught-msg say  -1 caught-remarked !  4 frames  world-frozen state-off  shot-off ;
: left-window ( -- )  $BD -62 12.41e -5.83e high-window ;
: right-window ( -- )  $C0 121 -11.799e 6.147e high-window ;
: the-device ( -- )
    fiona wait-move  fiona 12.8e 28.7e turn-to-xz  fiona look-close-anim start-anim  40 frames
    device-remarked @ if  device-2-msg  else  -1 device-remarked !  device-msg  then  say ;
: take-medallion ( -- )  album-medallion om1f-7-album-medallion medallion-fx -30.81e -21.85e picks-up ;
: medallion-sniffed ( -- )  om1f-7-album-medallion-found om1f-7-album-medallion medallion-fx  -30.81e 1e -21.85e  hewie-finds ;
: wary-of-it ( -- )  -1.06e 0e -0.54e hewie-stands-by ;

\ ---- brought in by the forced way the first time: the scene, the chapter's turn ----
: the-scene ( -- )
    come-movie scenes:unseen-movie come-cutscene load-scene  yield yield
    old-mansion-1f-7 145 stalker-to  kept-figure cutscene-keep-model
    panic-eased  if  come-msg run-scene  then ;
create locked-doors   $44 , $47 , $56 , $5D , $7D , $34 ,
create his-rooms   $60 , $63 , $10B ,
: each-cell ( addr n xt -- )  {: a n xt :}  n 0 ?do  a i cells + @ xt execute  loop ;
: kept-from ( room -- )  stalker-slot 1 doors-into ;
: doors-anew ( -- )
    locked-doors 6 ['] lock each-cell  $79 closed-off  exits-redone  his-rooms 3 ['] kept-from each-cell ;
: come-with-me ( -- )
    stalkers-stay state!  fiona hold  hewie let-go  stalker-unloaded  fiona wait-move
    riccardo-kind stalker-ready  yield  in-play state-off
    the-scene
    world-held state!  stalker $91 125 -13.81e 11.69e place-at  0 prepare-exit  forced-exit take-exit
    -1 music-part  2 music-part  2 0 0 stage-music  3 0 0 stage-music  4 0 0 stage-music
    doors-anew  summoner-on state!  his-own-time ;
: replayed ( -- )
    fiona hold  riccardo-kind stalker-ready  yield  the-scene  stalker-unloaded  replay-over ;

\ ---- as she comes in ----
: medallion-shows ( -- )
    om1f-7-album-medallion-found fact?  om1f-7-album-medallion fact? 0= and if  medallion-fx -30.81e 1e -21.85e flicker-at  then ;
: came-in ( exit -- )  drop  room-sounds  medallion-shows  1 $2300 reverb ;

create cameras  4 , 0 , 0 ,   5 , 1 , 1 ,   6 , 1 , 1 ,   7 , 2 , 2 ,   \ (area, set, path)
create entries  2 , 2 ,   0 , 0 ,   \ (by exit 0, 1: the camera)
create ahead  2 , 0 ,   3 , 1 ,   \ (area, exit loaded ahead)
create far-look  -68.08e 0e 55.74e  30e 18e 18e look,
create pod-look  -1.06e 0e -0.54e  38e 17e 17e look,
: hum ( -- )
    sounds-on @ if  hum-sound 0e 11e 0e keep-sound
    else  room-sounds-in? if  hum-sound 0e 11e 0e room-sound  -1 sounds-on !  then  then ;
: each-frame ( -- )
    hum  2 exits-out  cameras 4 cameras-at  ahead 2 ways-out
    far-look 1 hewie-looks-at
    pod-noticed 50 -1.06e 0e -0.54e 38e 17e notices? if  ['] wary-of-it start drop  then
    pod-look 1 hewie-looks-unchased
    om1f-7-album-medallion-found fact? 0=  scrambling? and  hewie -30.81e 0e -21.85e 10e 5e touches? and if  ['] medallion-sniffed start-only  then
    fiona 8 in-area?  fiona -45 50 heading? and if  ['] the-extractor unhurried offer  then
    fiona 9 in-area?  fiona 50 0e 0e faces-point? and if  ['] left-window unhurried offer  then
    fiona $A in-area?  fiona 50 0e 0e faces-point? and if  ['] right-window unhurried offer  then
    fiona $B in-area?  fiona -25 60 heading? and  fiona $C in-area?  fiona 45 60 heading? and or if  ['] the-device unhurried offer  then
    om1f-7-album-medallion-found fact?  om1f-7-album-medallion fact? 0= and  fiona -30.81e 0e -21.85e 5e 5e touches? and if
        ['] take-medallion calm-only offer
    then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop  exit  then
    entries 2 ways-in
    came-by @ forced-exit =  over-music-c fact? 0= and if
        world-held state!  0e sounds-scaled  over-music-c fact!  ['] come-with-me start drop
    then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-7 place!
' items-used old-mansion-1f-7 items-here
