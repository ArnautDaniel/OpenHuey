\ story/places/castle-1f-8.fs - the telephone room in Belli Castle's 1F ($18): her first steps in
\ (uneasy - the telephone rings: the voice warns her of Riccardo, and Debilitas comes in after the
\ call), the line cut since, a place to hide (the kit's; the stalker coming in searches - finding
\ her more likely each time he comes), four flames stirring as someone passes, the chair, the
\ ornaments, the Magnesia, the Album Medallion Hewie sniffs out, the walls' shade by the doors. A
\ place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/castle-1f-8.fs
\ (converted from the game's, not loaded).
IN: story.places.castle-1f-8
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects story.kit fiona.anims scenes ;

own-reset

\ ---- what is said here ----
0 constant call-msg        \ "Fiona... Who is this? You must pay attention now, Fiona. Be wary of Riccardo......"
7 constant line-cut-msg    \ "The line must have been cut... There is no dial tone."
8 constant chair-msg       \ "There is an oddly placed chair here as well. This place continues to weird me out."
9 constant nothing-msg     \ "There is nothing out of the ordinary here."
$A constant ornaments-msg  \ "Various ornaments have been placed here. They all look quite dusty, though."

\ ---- its own state (a visit) ----
own: his-entrance          \ the stalker coming in after the call (left meanwhile: his order 0)

\ ---- the room's things ----
4 constant album-fx   5 constant magnesia-fx   0 constant receiver-part
4 constant receiver-a-object   5 constant receiver-b-object
2 constant call-area   3 constant to-exit-0-area   8 constant chair-area   9 constant nothing-area
$A constant ornaments-area   $B constant phone-area   $C constant hiding-area
0 constant uneasy-sound   2 constant her-motions   3 constant his-motions   $D constant search-var
1 constant call-movie   0 constant call-cutscene   3 constant figure-slot   $19 constant voice-kind
$11 constant call-door   $208 constant call-file
: called? ( -- flag )  c1f-8-flag1 fact? ;

\ ---- the flames (effects 0..3), each stirred as someone steps into its draught ----
create flames  34.2e 18.3e 14.2e f, f, f,   34.2e 18.3e -14.2e f, f, f,
    -34.2e 18.3e -14.2e f, f, f,   -34.2e 18.3e 14.2e f, f, f,
: flame@ ( k -- ) ( F: -- x y z )  3 * floats flames +  dup 2 floats + f@  dup float+ f@  f@ ;
: flames-lit ( -- )  4 0 do  i 1  i flame@  flame-at  loop ;
: draughts ( -- )  4 0 do  i  i flame@  8e -4e  flame-stirred  loop ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  called? 0= if  room-sounds  then
    flames-lit
    c1f-8-album-medallion-found fact?  c1f-8-album-medallion fact? 0= and if  album-fx -25.6e 1e 27.6e flicker-at  then
    c1f-8-magnesia fact? 0= if  magnesia-fx 0e 8.5e 25e flicker-at  then
    receiver-part show-part  1 $2300 sound-reverb! ;

\ ---- the call: the voice's warning (the movie, a figure in it), then the stalker in after her ----
: shadow-shot ( shot -- )  8 = if  0 true no-shadow  then ;   \ (her shadow off from its eighth shot)
: the-call ( -- )   \ (the movie, keyed; her shadow back after)
    voice-kind figure-slot figure-in  yield
    call-movie scenes:unseen-movie call-cutscene load-scene  ['] shadow-shot at-shots ;
: call-over ( -- )  world-held state!  0 false no-shadow  figure-slot figure-out ;
: comes-in ( -- )   \ (the stalker, in the game: his camera; by the far door, through the near one)
    yield  stalker 0 -1 camera-set  2 stalker-order
    stalker hold  -1 his-entrance !  stalker $DA 100 place-on-tri  stalker 0 open-door  0 his-entrance ! ;
: sent-in ( -- )  partner dup 0< if  drop exit  then  dup activate  >r castle-1f-8 218 r> send stalker-in ;
: hewie-through-it ( -- down? )   \ (Hewie down: kept out of sight through it; else let be)
    hewie-in? 0= if  false exit  then
    hewie-cond-now @ down-cond = if  hewie hold  hewie false shown  true  else  hewie let-go  false  then ;
: hewie-after ( down? -- )   \ (down, shown again; else put by her)
    if  hewie true shown  hewie let-go exit  then
    hewie activate  castle-1f-8 299 0 hewie-to  hewie $12B -100 73.05e 6.36e place-at ;
: telephone-rings ( -- )
    {: | was-in down :}  fiona hold  stalkers-stay state!
    begin  room-sounds-in? 0= while  yield  repeat
    uneasy-sound 22e 10e -24e room-sound  fiona uneasy-anim 10 play-blend
    15 screen:to-black-2 with-music or fade
    the-call  wait-fade
    hewie-in? to was-in  hewie-through-it to down
    receiver-part hide-part  panic-eased  if  call-msg run-scene  then
    call-over  receiver-part show-part
    receiver-a-object object-hide  receiver-b-object object-hide  receiver-a-object object-reset  receiver-b-object object-reset
    was-in if  down hewie-after  then
    fiona 0 idle  fiona $48 150 19e -19e place-at  2 -1 player-camera
    c1f-8-flag1 fact!  call-door door-shut  call-door lock
    summoner-on state!  sent-in  his-own-time  ['] comes-in start drop
    found-file-208 fact!  5 0 0 stage-music
    15 screen:from-black music-cut or fade  wait-fade
    call-file give-file  stalkers-stay state-off ;

\ ---- the hiding place (the kit's): in, lying low, calming; out by the button - or found, and
\ hauled out ----
: hides ( -- )  her-motions $141 180 3 -0.803e -15.664e hides-in ;

\ ---- the stalker coming in while she hides: he searches ----
: finds-her ( -- )
    stalker hold  stalker wait-move  0 door-for-him  1 door-for-him  $B2 180 0.866e 0.417e finds-her-at ;
: stalker-came ( -- )  came-on-her? if  search-var his-motions ['] finds-her searches  then ;

\ ---- what she may do or say here ----
: telephone ( -- )   \ (since the call: the line dead)
    fiona $48 150 -1 walk-move 19e -19e go-to
    world-frozen state!  1 1e 10e 0e 0e shot  line-cut-msg say  16 frames  shot-off  world-frozen state-off ;
: chair ( -- )  fiona wait-move  chair-msg say ;
: nothing-here ( -- )  fiona wait-move  nothing-msg say ;
: ornaments ( -- )  fiona wait-move  ornaments-msg say ;
: pick-album ( -- )  album-medallion c1f-8-album-medallion album-fx -25.6e 27.6e picks-up ;
: reach-magnesia ( -- )
    fiona wait-move  fiona 0e 25e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  magnesia c1f-8-magnesia magnesia-fx takes-up  fiona reach-back-anim play ;
: spots ( -- )
    c1f-8-magnesia fact? 0=  fiona 0e 8.5e 25e 5e 5e touches? and if  ['] reach-magnesia calm-only offer  then
    fiona hiding-area in-area?  fiona 90 60 heading? and if
        stalker-in? if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then
    then
    called?  fiona phone-area in-area? and  fiona 75 45 heading? and if  ['] telephone unhurried offer  then
    fiona chair-area in-area?  fiona 45 45 heading? and if  ['] chair unhurried offer  then
    fiona nothing-area 45 faces-area? if  ['] nothing-here unhurried offer  then
    fiona ornaments-area in-area?  fiona -67 45 heading? and if  ['] ornaments unhurried offer  then
    c1f-8-album-medallion-found fact?  c1f-8-album-medallion fact? 0= and  fiona -25.6e 1e 27.6e 5e 5e touches? and if
        ['] pick-album calm-only offer  then ;

\ ---- Hewie: the medallion he sniffs out (scrambling about by it) ----
: album-sniffed ( -- )  c1f-8-album-medallion-found c1f-8-album-medallion album-fx  -25.6e 1e 27.6e hewie-finds ;
: hewie-sniffs ( -- )
    c1f-8-album-medallion-found fact?  scrambling? 0= or if  exit  then
    hewie -25.6e 0e 27.6e 10e 5e touches? 0= if  exit  then
    ['] album-sniffed start-only ;

\ ---- the walls' shade by the doors ----
: walls-shade ( -- )
    34e 20e -15e  42e 20e -15e  34e 0e -15e  42e 0e -15e  light-doorway
    42e 20e 15e  34e 20e 15e  42e 0e 15e  34e 0e 15e  light-doorway
    -43e 20e -15e  -35e 20e -15e  -43e 0e -15e  -35e 0e -15e  light-doorway
    -35e 20e 15e  -43e 20e 15e  -35e 0e 15e  -43e 0e 15e  light-doorway ;

\ ---- the movie replayed (the extras) ----
: replayed ( -- )
    fiona hold  the-call  flames-lit  receiver-part hide-part  panic-eased  if  call-msg run-scene  then
    call-over  replay-over ;

\ ---- every frame ----
create cameras  4 , 1 , 0 ,   5 , 0 , -1 ,   6 , 2 , -1 ,   \ (area, set, path)
create entries  0 , -1 ,   1 , 0 ,             \ (by exit 0, 1: the camera)
: ways ( -- )
    2 exits-out  cameras 3 cameras-at  7 1 0 camera-out
    fiona call-area entered? if  1 prepare-exit  then
    fiona to-exit-0-area entered? if  0 prepare-exit  then
    called? 0=  fiona call-area left? and if  c1f-8-flag1 fact!  panic-eased  ['] telephone-rings start drop  then ;
: each-frame ( -- )
    events-held state? if  draughts exit  then
    ways  draughts  hewie-sniffs  spots  walls-shade ;
: person-in ( i -- )
    case
        fiona-p of
            events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
            entries 2 ways-in  endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- )  his-entrance @ if  0 stalker-order  then ;

' came-in ' person-in ' each-frame ' went castle-1f-8 place!
