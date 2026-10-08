\ effects.fs - the rooms' effects (docs/subsystems/effects.md; src/game/effects.c): flames,
\ flickering sprites, sparks... Each is an actor: a room's scripts make one in one of the
\ room's 32 effect slots (the room actor keeps them: story.words), or an effect makes another
\ (a flame's sparks, which end by themselves). An effect steps itself each tick and writes its
\ sprites (the engine's sprite batches: `sprites`, `sprite!`) at the frame's end; killed, its
\ last words free its batch.
\   effect-start ( x y z command kind )   its place (f>cell) and how it starts (RoomEffects_Send)
\   effect-command ( v )                  a command later (the scripts' 0x87: 1 / 2)
IN: effects
USING: engine actors messages common ;

64 constant #parts      \ sprites an effect keeps at most
: float+ ( addr -- addr' )  1 floats + ;

state: fx-state
  cell field batch                    \ its sprite batch (-1 none)
  3 floats field at                   \ where it is (+0x20)
  cell field command  cell field kind
  cell field timer  cell field rest  cell field slow
  cell field owner                    \ (sparks) the effect they belong to
  \ its sprites (the quad records: colour 4 x 0..0x80+, position, half sizes, turn, frame)
  #parts 4 * cells field p-col
  #parts floats field p-x  #parts floats field p-y  #parts floats field p-z
  #parts floats field p-w  #parts floats field p-h  #parts floats field p-rot
  #parts cells field p-frame
  \ what each moves by (the classes' own: speeds, angles, radii)
  #parts floats field p-a  #parts floats field p-b  #parts floats field p-c
  cell field #shown                   \ how many it draws
  cell field current                  \ (two-buffer effects: the one drawn - kept for the order)
end-state

\ ---- a sprite's parts ----
: fx ( i addr -- addr' )  swap floats + ;
: pc ( i addr -- addr' )  swap cells + ;
: col ( i k -- addr )  swap 4 * + cells p-col + ;
: col! ( r g b a i -- )  >r  r@ 3 col !  r@ 2 col !  r@ 1 col !  r> 0 col ! ;
: c255 ( v -- b )  0 max 255 min ;
: packed ( i -- rgba )
    >r  r@ 0 col @ c255 24 lshift  r@ 1 col @ c255 16 lshift or  r@ 2 col @ c255 8 lshift or  r> 3 col @ c255 or ;
: place! ( i F: x y z -- )  dup p-z fx f!  dup p-y fx f!  p-x fx f! ;
: size! ( i F: w h -- )  dup p-h fx f!  p-w fx f! ;
: rnd-turn ( F: -- a )  rnd 0.5e f- 2e f* pi f* ;   \ (-pi .. pi)

\ ---- the quad drawer: the batch's texture and cells, and the records written each frame ----
: new-batch ( -- )  sprites batch ! ;
: tex! ( group id palette -- )  >r >r >r batch @ r> r> r> sprites-texture ;
: cells! ( x y cw ch tw th frames -- )
    >r >r >r >r >r >r >r  batch @  r> r> r> r> r> r> r>  sprites-cells ;
: flags! ( flags layer -- )  >r >r batch @ r> r> sprites-flags ;
: offset! ( F: cx cy -- )  batch @ sprites-offset ;
: draw ( -- )   \ (at the frame's end)
    batch @ 0< if  exit  then
    #shown @ 0 ?do
        batch @ i  i packed  i p-frame pc @
        i p-x fx f@  i p-y fx f@  i p-z fx f@  i p-w fx f@  i p-h fx f@  i p-rot fx f@  sprite!
    loop
    batch @ #shown @ sprites-count ;
: gone ( -- )  batch @ 0< 0= if  batch @ sprites-free  -1 batch !  then ;

: at@ ( F: -- x y z )  at f@  at float+ f@  at 2 floats + f@ ;
: at! ( F: x y z -- )  at 2 floats + f!  at float+ f!  at f! ;
: cell>at ( x y z -- )  >r >r cell>f r> cell>f r> cell>f at! ;   \ (the cells as sent)

\ ---- the base: a fresh effect, its batch, ticks and draws; killed, its batch freed ----
behaviour an-effect
  on spawned ( -- )  -1 batch !  new-batch  -1 owner !  self subscribe tick  self subscribe frame-end ;
  on killed ( -- )  gone ;
  on frame-end ( -- )  draw ;
end-behaviour

\ a rest of 0.5 .. 1.2 s (slow: 2.5 .. 4 s)
: sprite-rest ( -- n )
    slow @ 0= if  0.7e rnd f* 0.5e f+  else  1.5e rnd f* 2.5e f+  then  30e f* f>s ;

\ ---- EvEffect7F (the scripts' 0x7F, 0xB1): a flickering animated sprite - grey, half
\ transparent, 1.6 across, the 16-frame strip of 32 x 32 cells (GAME_FIX); it plays through,
\ rests 0.5 .. 1.2 s and plays again at a new random turn ----
behaviour flickering
  extends an-effect
  on spawned ( -- )
      -1 batch !  new-batch  self subscribe tick  self subscribe frame-end
      $10 1 -1 tex!  0 0 32 32 512 256 16 cells!  0 $19 flags!
      $80 $80 $80 $40 0 col!  1.6e 1.6e 0 size!  0e 0 p-rot fx f!
      16 random 0 p-frame pc !  1 timer !  0 rest !  1 #shown ! ;
  on effect-start ( x y z command kind -- )   \ (EvEffect7F_SetParams: the command is "slow")
      drop slow !  cell>at  0 at@ place!  sprite-rest rest ! ;
  on tick ( -- )   \ EvEffect7F_Update
      rest @ if  -1 rest +!  exit  then
      -1 timer +!  timer @ if  exit  then
      1 timer !  1 0 p-frame pc +!
      0 p-frame pc @ 16 < if  exit  then
      0 0 p-frame pc !  rnd-turn 0 p-rot fx f!  sprite-rest rest ! ;
  on frame-end ( -- )  rest @ 0= if  1  else  0  then  #shown !  draw ;
end-behaviour

\ ---- Wisps (a flame's sparks; EffectMgr's): 4 orange sparks rising and swirling about a point.
\ Kind's low 12 bits 0: a lasting flame's (slow, coming back), else a burst; bit $8000: they end
\ with the effect they belong to (the original's room effect 0) ----
: lasting? ( -- flag )  kind @ $FFF and 0= ;
\ wisp i anew (again: coming back); a first one starts some way up (Wisps_Wisp)
: wisp ( i again -- )   \ (Wisps_Wisp; a first one's "some way up" is the int of a 0..1 random: none)
    >r
    lasting? if  0.1e rnd f* 0.2e f+  else  0.25e rnd f* 0.01e f+  then  dup p-a fx f!   \ rise speed
    360e rnd 0.5e f- f* pi f* 180e f/  dup p-b fx f!                                         \ angle
    0.5e rnd f*  dup p-c fx f!                                                               \ radius
    dup >r  $80 $40 $10  64 random $40 +  r> col!
    r> 0= if  dup p-a fx f@ 0.05e fmax  dup p-a fx f!  then
    dup p-b fx f@ fcos  dup p-c fx f@ f*  at f@ f+
    at float+ f@
    dup p-b fx f@ fsin  dup p-c fx f@ f*  at 2 floats + f@ f+
    dup place!
    0.1e rnd f* 0.2e f+  fdup  dup size!  rnd-turn  dup p-rot fx f!  0 swap p-frame pc ! ;
: wisps-step ( -- done? )   \ Wisps_Update
    true
    4 0 do
        i 3 col @ 0> if  i 3 col @  4 random 4 + -  i 3 col !  then
        i 3 col @ 0> 0= if
            lasting? if  i 1 wisp  drop false  else  0 i 3 col !  then
        else
            drop false
            i p-c fx f@  lasting? if  0.02e  else  0.04e  then  f+  i p-c fx f!
            i p-b fx f@  30e rnd 0.5e f- f* pi f* 180e f/ f+  angle-wrap  i p-b fx f!
            i p-a fx f@ -0.015e f+ 0.05e fmax  i p-a fx f!
            i p-y fx f@  i p-a fx f@ f+  i p-y fx f!
            i p-b fx f@ fcos i p-c fx f@ f*  at f@ f+  i p-x fx f!
            i p-b fx f@ fsin i p-c fx f@ f*  at 2 floats + f@ f+  i p-z fx f!
        then
    loop
    kind @ $8000 and  owner @ alive? 0= and if  drop true  then ;
behaviour wisping
  extends an-effect
  on spawned ( -- )
      -1 batch !  new-batch  self subscribe tick  self subscribe frame-end  -1 owner !
      $10 1 -1 tex!  $6C $4C 8 8 512 256 1 cells!  $40 $19 flags!  4 #shown ! ;
  on effect-start ( x y z command kind -- )   \ (Wisps_SetParams: kind is the command's word)
      kind !  drop  cell>at  sender owner !
      4 0 do  i  lasting? 0= wisp  loop ;
  on tick ( -- )  wisps-step if  self kill  then ;
end-behaviour
: sparks ( kind -- ) ( F: x y z -- )   \ a flame's sparks there
    >r  f>cell f>cell f>cell swap rot  0 r>  wisping fx-state s" sparks" spawn  send effect-start ;

\ ---- EvEffect86 (the scripts' 0x86): a flame. Kind 0 a candle (32 frames: the first half
\ loops; commanded, the second half plays at half speed - command 2 shrinking it away); others
\ a 16-frame fire (kind 4: kind 1 also turned a quarter; kind 3 another palette), kind 1 with
\ a lasting spark above it; commanded, the fire throws 2 / 4 sparks ----
: flame-start ( -- )   \ (EvEffect86_SetParams, command 0)
    $80 $80 $80 $80 0 col!  0 at@ place!  0e 0 p-rot fx f!  16 random 0 p-frame pc !  1 #shown !
    0e -1e offset!
    kind @ if
        1e 2e 0 size!
        kind @ 4 = if  $85  1 kind !  else  $81  then  $19 flags!
        $10 1  kind @ 3 = if  5  else  4  then  tex!  0 $80 16 32 512 256 16 cells!
        kind @ 1 = if  at f@  at float+ f@ 2e f+  at 2 floats + f@  $8000 sparks  then
    else
        0.25e 1e 0 size!  $81 $19 flags!  $10 1 1 tex!  0 $20 16 32 512 256 32 cells!
    then ;
: flame-command ( v -- )   \ (EvEffect86_SetParams, command 1 / 2)
    dup command !
    kind @ 0= if  drop 15 0 p-frame pc !  exit  then
    kind @ 1 <> if  drop exit  then
    2 = if  4  else  2  then  0 ?do  at f@  at float+ f@ 1.5e f+  at 2 floats + f@  1 sparks  loop ;
behaviour flaming
  extends an-effect
  on spawned ( -- )  -1 batch !  new-batch  self subscribe tick  self subscribe frame-end  1 timer ! ;
  on effect-start ( x y z command kind -- )
      over if  drop flame-command  2drop drop exit  then
      kind !  drop  cell>at  flame-start ;
  on effect-command ( v -- )  flame-command ;
  on tick ( -- )   \ EvEffect86_Update
      -1 timer +!
      kind @ if
          timer @ 0= if  1 timer !  1 0 p-frame pc +!  0 p-frame pc @ 16 < 0= if  0 0 p-frame pc !  then  then
          exit
      then
      timer @ 0= if
          1 timer !  1 0 p-frame pc +!
          command @ 0= if
              0 p-frame pc @ 16 < 0= if  0 0 p-frame pc !  then
          else
              2 timer !
              0 p-frame pc @ 32 < 0= if  0 0 p-frame pc !  1 timer !  0 command !  then
          then
      then
      command @ 2 = if  0 p-frame pc @ 12 - 2 rshift timer @ + s>f 0.125e f*  else  1e  then  0 p-h fx f! ;
end-behaviour

\ an effect of a behaviour: spawned, then started (the room's slots keep them)
: effect ( command kind beh -- id ) ( F: x y z -- )
    fx-state s" effect" spawn >r  >r >r  f>cell f>cell f>cell swap rot  r> r>  r@ send effect-start  r> ;
