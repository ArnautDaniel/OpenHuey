\ story/places/castle-3f-1.fs - the 3F hall of Belli Castle ($6), over the room of the four boxes:
\ the boxes seen below (put back to their first squares as she comes in by exit 1, else as they were
\ left; once the way is open, where they ended); the door with the strange symbol, locked (the
\ Saturnus Key's file) - the key used there before the chapter turns, Daniella's "Dinner is served"
\ (out by the forced way), after it unlocking it; once she has rested, Daniella here, busy, before
\ she stands in the corridor beyond; the doorways' shade; the movie replayed. A place (story/kit.fs).
\ The original's scripts, for checking: story/rooms-ref/castle-3f-1.fs (converted from the game's,
\ not loaded).
IN: story.places.castle-3f-1
USING: engine game-state actors messages common facts flag-names story-names room-names stalker-names
    items.names story.kit fiona.anims scenes ;

own-reset

\ ---- what is said here ----
0 constant dinner-msg     \ "Dinner is served, Miss. Dinner is served, Miss."
1 constant locked-msg     \ "The door is locked."
2 constant symbol-msg     \ "There is a strange symbol etched on the door. What could it mean?"

\ ---- its own state (a visit) ----
own: locked-remarked

\ ---- the room's things ----
0 constant symbol-door-exit   $34 constant symbol-door   0 constant scene-part
1 constant dinner-movie   0 constant dinner-cutscene   2 constant her-motions
0 constant key-sound   $609 constant key-turn-anim   $258 constant symbol-file
4 constant #boxes

\ ---- the boxes below (the obstacles of the room of the four boxes) ----
create first-squares  $584 , $DF8 ,   $65C , $ED0 ,   $76A , $FDE ,   $36E , $BE2 ,
: boxes-ended ( -- )   \ (their models where the way opened left them)
    s" oshi00" -35e -46e -55e object-to   s" oshi01" -25e -46e -55e object-to
    s" oshi02" -25e -46e 55e object-to    s" oshi03" 75e -46e 35e object-to ;
: boxes-below ( exit -- )
    c2f-9-flag1 fact? if  drop boxes-ended exit  then
    1 = if  #boxes 0 do  i  first-squares i 2* cells +  dup @ swap cell+ @  obstacle-kept-at  loop
    else  #boxes 0 do  i i obstacle-model-home  loop  then ;

\ ---- Daniella here, once she has rested (till the corridor): busy at her task, unafraid of, no camera ----
: daniella-due? ( -- flag )  c1f-14-flag2 fact?  c3f-2-flag1 fact? 0= and ;
: busy-here ( -- )
    stalker-present  stalker hold  stalker true noclip  stalker $129 180 -4.85e -47.03e place-at  stalker -1 -1 camera-set
    stalker wait-move  stalker use-motions  begin  stalker room-motion 1+ play  again ;
: daniella-busy ( -- )
    force-followed state!  stalker-no-fear state!  no-stalker-camera state!
    partner activate  partner true hush  castle-3f-1 236 stalker-to  hands-empty
    stalker her-motions motions-for  ['] busy-here start drop ;

\ ---- as she comes in ----
: came-in ( exit -- )
    room-sounds  boxes-below  daniella-due? if  daniella-busy  then  1 $2300 sound-reverb! ;

\ ---- the symbol door: locked (its file) ----
: tries-door ( -- )
    locked-remarked @ 0=  chased? or  in-panic? or if  symbol-door-exit false locked-msg tries-locked  -1 locked-remarked !
    else
        fiona symbol-door-exit to-door
        4 frames  fiona $12E 180 0e -76.5e place-at  world-frozen state!  1 20e 5e 0e 4e shot  4 frames
        symbol-msg say  4 frames  shot-off  fiona $12E 180 -1.25e -75.3e place-at  world-frozen state-off
    then
    symbol-file give-file ;

\ ---- the Saturnus Key at it: before the chapter turns, Daniella's scene; after, it unlocks ----
: dinner-scene ( -- )
    daniella-kind stalker-ready  yield
    dinner-movie scenes:unseen-movie dinner-cutscene load-scene ;
: dinner-served ( movie? -- )  scene-part show-part  panic-eased  if  dinner-msg run-scene  then ;
: dinner ( -- )   \ (out by the forced way after it)
    stalkers-stay state!  fiona hold  fiona symbol-door-exit to-door
    15 screen:to-black-2 with-music or fade  scene-locked state!
    dinner-scene  wait-fade  hewie let-go  castle-1f-6 preload
    dinner-served  wait-fade  world-held state!  stalker let-go  forced-exit take-exit  scene-part hide-part
    stalkers-stay state-off ;
: key-used ( -- )
    over-music-b fact? if  symbol-door-exit symbol-door saturnus-key key-turn-anim key-sound room-bank key-turns
    else  dinner  then ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0= 0= if  drop false exit  then
    1 = if  ['] key-used start drop  true  else  false  then ;

\ ---- the movie replayed (the extras) ----
: replay-dinner ( -- )
    fiona hold  dinner-scene  dinner-served  world-held state!  stalker let-go  scene-part hide-part
    stalker-unloaded  replay-over ;

\ ---- every frame ----
create cameras  8 , 1 , -1 ,   9 , 0 , -1 ,   $E , 1 , -1 ,   $F , 2 , -1 ,   $10 , 1 , -1 ,   $11 , 2 , -1 ,   \ (area, set, path)
create entries  2 , -1 ,   0 , -1 ,   1 , -1 ,   0 , -1 ,   \ (by exit 0..3: the camera)
create ahead  4 , 0 ,   $C , 0 ,   6 , 1 ,   $B , 1 ,   5 , 2 ,   $A , 2 ,   7 , 3 ,   $D , 3 ,   \ (area, exit loaded ahead)
: doorways-shade ( -- )
    0e -2.8e -39e  -83e -2.8e -39e  0e -2.8e -60e  -83e -2.8e -60e  light-doorway
    40e -2.8e -39e  0e -2.8e -39e  40e -2.8e -60e  0e -2.8e -60e  light-doorway
    83e -2.8e -39e  40e -2.8e -39e  83e -2.8e -60e  40e -2.8e -60e  light-doorway
    0e -2.8e 39e  40e -2.8e 39e  0e -2.8e 60e  40e -2.8e 60e  light-doorway
    40e -2.8e 39e  83e -2.8e 39e  40e -2.8e 60e  83e -2.8e 60e  light-doorway
    -83e -2.8e 39e  0e -2.8e 39e  -83e -2.8e 60e  0e -2.8e 60e  light-doorway
    30e -2e -57e  -20e -2e -57e  30e -2e -80e  -20e -2e -80e  light-doorway
    38.7e 19.3e -60.1e  20.2e 19.3e -60.1e  38.7e -4e -60.1e  20.2e -4e -60.1e  light-doorway ;
: each-frame ( -- )
    events-held state? if  doorways-shade exit  then
    2 exits-out  2 2 leaves-by  3 3 leaves-by  cameras 6 cameras-at  ahead 8 ways-out
    4 locked-request? if  ['] tries-door any-time offer  then
    doorways-shade ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replay-dinner start drop exit  then
    entries 4 ways-in ;
: went ( -- )   \ (Daniella let be again; her fear and camera back once she's out by the corridors)
    force-followed state-off
    daniella-due? 0= if  exit  then
    hands-back  partner false hush  stalker let-go
    fiona 0 in-area?  fiona 1 in-area? or if  stalker-no-fear state-off  no-stalker-camera state-off  then ;

' came-in ' person-in ' each-frame ' went castle-3f-1 place!
' items-used castle-3f-1 items-here
