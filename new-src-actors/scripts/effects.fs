\ effects.fs - the rooms' effects (docs/subsystems/effects.md; src/game/effects.c): flames,
\ flickering sprites, sparks... Each is an actor: a room's scripts make one in one of the
\ room's 32 effect slots (the room actor keeps them: story.words), or an effect makes another
\ (a flame's sparks, which end by themselves). An effect steps itself each tick and writes its
\ sprites (the engine's sprite batches: `sprites`, `sprite!`) at the frame's end; killed, its
\ last words free its batch.
\   effect-start ( x y z command kind )   its place (f>cell) and how it starts (RoomEffects_Send)
\   effect-command ( v )                  a command later (the scripts' 0x87: 1 / 2)
IN: effects
USING: engine actors messages common vectors ;

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
  #parts floats field p-d  #parts floats field p-e  #parts floats field p-f
  #parts cells field p-alpha          \ each one's own alpha
  1 floats field spread
  \ (shards) each one's box: 8 corners' x y z, its scale, turn and spin (x y z each), alive
  16 24 * floats field box
  #parts 3 * floats field p-scale  #parts 3 * floats field p-turn  #parts 3 * floats field p-spin
  #parts cells field p-alive
  32 floats field fv                  \ a class's own numbers (named below for each)
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

\ ---- scene effects (the original's EffectMgr: ended by themselves, or when the next room loads -
\ EffectMgr_Reset) ----
behaviour a-scene-effect
  extends an-effect
  on arrived ( room exit -- )  2drop  self kill ;
end-behaviour
: scene-subscribe ( -- )  -1 batch !  new-batch  self subscribe tick  self subscribe frame-end  self subscribe arrived ;

\ ---- SpeckSwarm (the scripts' 0x9F): up to 16 specks buzzing about a centre within a spread -
\ each now and then (1 in 10) a new heading pulled back to the centre; hidden with the camera
\ within 10 of it, dim within 20. A 4 x 4 cell at (238, 78), palette 2 ----
: speck-heading ( i -- )   \ (swarm_heading: up to 0.05 x the spread each way, pulled back by an eighth)
    >r
    rnd 0.5e f- spread f@ f* 0.1e f*  fdup r@ p-a fx f!  -0.125e f* r@ p-d fx f!
    rnd 0.5e f- spread f@ f* 0.1e f*  fdup r@ p-b fx f!  -0.125e f* r@ p-e fx f!
    rnd 0.5e f- spread f@ f* 0.1e f*  fdup r@ p-c fx f!  -0.125e f* r> p-f fx f! ;
\ one axis: the velocity pulled, the speck moved, the pull turned back once past the centre
: speck-axis ( pos vel acc -- ) ( F: centre -- )
    >r  dup f@ r@ f@ f+ dup f!  f@                       ( pos ) ( F: centre v )
    dup f@ f+ dup f!  f@ fswap f-                        ( F: pos-centre )
    r@ f@ f0< if  f0< if  r@ f@ fnegate r@ f!  then  else  f0> if  r@ f@ fnegate r@ f!  then  then  r> drop ;
: swarm-start ( count spread rgba -- )   \ SpeckSwarm_SetParams
    >r  s>f spread f!  16 min #shown !
    #shown @ 0 ?do
        r@ 24 rshift $FF and 8 -  16 random +  c255
        r@ 16 rshift $FF and 8 -  16 random +  c255
        r@ 8 rshift $FF and 8 -  16 random +  c255
        r@ $FF and 4 -  8 random +  $80 min  dup i p-alpha pc !  i col!
        at f@  rnd 0.5e f- spread f@ f* f+
        at float+ f@  rnd 0.5e f- spread f@ f* f+
        at 2 floats + f@  rnd 0.5e f- spread f@ f* f+  i place!
        rnd spread f@ f* 0.01e f* 0.05e f+  fdup  i size!
        rnd-turn i p-rot fx f!  0 i p-frame pc !  i speck-heading
    loop  r> drop ;
: swarm-step ( -- )   \ SpeckSwarm_Update
    cam-pos  at 2 floats + f@ f- fsq  fswap at float+ f@ f- fsq f+  fswap at f@ f- fsq f+   ( F: d2 )
    fdup 100e f< if  fdrop 8  else  400e f< if  1  else  0  then  then                         ( shift )
    #shown @ 0 ?do
        rnd 0.1e f< if  i speck-heading  then
        i p-x fx  i p-a fx  i p-d fx  at f@  speck-axis
        i p-y fx  i p-b fx  i p-e fx  at float+ f@  speck-axis
        i p-z fx  i p-c fx  i p-f fx  at 2 floats + f@  speck-axis
        rnd-turn i p-rot fx f!
        i p-alpha pc @ over rshift  i 3 col !
    loop  drop ;
behaviour swarming
  extends a-scene-effect
  on spawned ( -- )
      scene-subscribe  $10 1 2 tex!  $EE $4E 4 4 512 256 1 cells!  $20 $19 flags!  0 #shown ! ;
  on specks-start ( x y z count spread rgba -- )  >r >r >r  cell>at  r> r> r> swarm-start ;
  on tick ( -- )  swarm-step ;
end-behaviour

\ ---- Effect6FF60 (the scripts' 0x8C): 16 shards - small random boxes thrown up from a point
\ that tumble, fall and bounce on its height until they settle. Kind 0 / 3 a gentle spray, 2 / 3
\ bigger, slower turning pieces thrown higher, 4 thrown down. Textured from the room's bank by
\ a zone rectangle of the script's (its id the texture, its corners the cell); one colour ----
: v3 ( i k addr -- addr' )  >r swap 3 * + floats r> + ;   \ (a shard's x y z triple, k 0..2)
: corner ( i c -- addr )  swap 24 * swap 3 * + floats box + ;
: shard-sign ( c k -- n )   \ (the box's corners: x - on odd, y - on 2 3 6 7, z - on 4..7)
    case  0 of  1 and  endof  1 of  2 and  endof  >r 4 and r>  endcase  if  -1  else  1  then ;
: shard ( i -- )   \ Effect6FF60_Shard
    8 0 do  3 0 do  rnd  j i shard-sign 0< if  fnegate  then  dup j corner i floats + f!  loop  loop
    3 0 do  rnd 0.9e f* 0.1e f+  dup i p-scale v3 f!  loop
    3 0 do  rnd 0.5e f- 6e f*  dup i p-turn v3 f!  loop
    at f@  rnd 0.5e f- 4e f* f+  at float+ f@  rnd 8e f* f+  at 2 floats + f@  rnd 0.5e f- 4e f* f+  dup place!
    3 0 do  rnd 22.5e f*  dup i p-spin v3 f!  loop
    -1 over p-alive pc !
    kind @ 2 - 2 u< if
        3 0 do  rnd 0.5e f+  dup i p-scale v3 f!  loop
        at float+ f@  rnd 15e f* f+  dup p-y fx f!
        3 0 do  rnd 10e f*  dup i p-spin v3 f!  loop
    then
    \ the box's lopsidedness against its size: how hard it is thrown up
    0e  8 0 do  dup i corner f@  i 1 and if  f-  else  f+  then  loop  dup 0 p-scale v3 f@ f*     ( F: xs*sx )
    0e  8 0 do  dup i corner float+ f@  i 2 and if  f-  else  f+  then  loop  dup 1 p-scale v3 f@ f* f+
    0e  8 0 do  dup i corner 2 floats + f@  i 4 and if  f-  else  f+  then  loop  dup 2 p-scale v3 f@ f* f+   ( F: dot )
    21e fswap f-                                                                                       ( F: 21-dot )
    rnd 0.5e f-  kind @ dup 0= swap 3 = or if  0.4e f*  then  dup p-a fx f!
    kind @ case
        4 of  -0.02e f*  dup p-b fx f!  rnd 0.5e f-  dup p-c fx f!  endof
        0 of  0.05e f* 0.1e f+  dup p-b fx f!  rnd 0.5e f- 0.4e f*  dup p-c fx f!  endof
        3 of  0.05e f* 0.1e f+  dup p-b fx f!  rnd 0.5e f- 0.4e f*  dup p-c fx f!  endof
        >r  0.01e f* 0.1e f+  dup p-b fx f!  rnd 0.5e f-  dup p-c fx f!  r>
    endcase  drop ;
: shards-step ( -- any? )   \ Effect6FF60_Update
    false
    16 0 do
        i p-alive pc @ if
            drop true
            3 0 do  j i p-turn v3 f@  pi j i p-spin v3 f@ f* 180e f/ f+  fdup pi f> if  2e pi f* f-  then  j i p-turn v3 f!  loop
            i p-x fx f@ i p-a fx f@ f+ i p-x fx f!
            i p-y fx f@ i p-b fx f@ f+ i p-y fx f!
            i p-z fx f@ i p-c fx f@ f+ i p-z fx f!
            i p-b fx f@ 0.1e f- i p-b fx f!
            i p-y fx f@ at float+ f@ f< if
                i p-b fx f@ -0.5e f< if
                    at float+ f@ i p-y fx f!
                    i p-a fx f@ 0.5e f* i p-a fx f!
                    i p-b fx f@ fnegate 0.3e f* i p-b fx f!
                    i p-c fx f@ 0.5e f* i p-c fx f!
                else  0 i p-alive pc !  then
            then
        then
    loop ;
\ a corner in the world: scaled, turned (x, then y, then z: sceVu0RotMatrix), moved
fvariable wx  fvariable wy  fvariable wz  fvariable ca  fvariable sa  fvariable tmp
: turn-xyz ( i -- )   \ (wx wy wz turned by shard i's turn)
    dup 0 p-turn v3 f@ fdup fcos ca f! fsin sa f!
    wy f@ ca f@ f*  wz f@ sa f@ f* f-  wz f@ ca f@ f*  wy f@ sa f@ f* f+  wz f! wy f!
    dup 1 p-turn v3 f@ fdup fcos ca f! fsin sa f!
    wx f@ ca f@ f*  wz f@ sa f@ f* f+  wz f@ ca f@ f*  wx f@ sa f@ f* f-  wz f! wx f!
    2 p-turn v3 f@ fdup fcos ca f! fsin sa f!
    wx f@ ca f@ f*  wy f@ sa f@ f* f-  wx f@ sa f@ f*  wy f@ ca f@ f* f+  wy f! wx f! ;
create world 8 3 * floats allot
: shard-corners ( i -- )
    8 0 do
        dup i corner f@  over 0 p-scale v3 f@ f* wx f!
        dup i corner float+ f@  over 1 p-scale v3 f@ f* wy f!
        dup i corner 2 floats + f@  over 2 p-scale v3 f@ f* wz f!
        dup turn-xyz
        i 3 * floats world +  >r
        wx f@ over p-x fx f@ f+ r@ f!  wy f@ over p-y fx f@ f+ r@ float+ f!  wz f@ over p-z fx f@ f+ r> 2 floats + f!
    loop  drop ;
create faces  0 c, 1 c, 2 c, 3 c,  1 c, 5 c, 3 c, 7 c,  5 c, 4 c, 7 c, 6 c,  4 c, 0 c, 6 c, 2 c,  0 c, 1 c, 4 c, 5 c,  6 c, 7 c, 2 c, 3 c,
: world@ ( c -- ) ( F: -- x y z )  3 * floats world +  dup f@  dup float+ f@  2 floats + f@ ;
: shards-draw ( -- )
    batch @ 0< if  exit  then
    0                                                     ( n )
    16 0 do
        i p-alive pc @ if
            i shard-corners
            6 0 do
                4 0 do  faces j 4 * + i + c@ world@  loop
                batch @ over  slow @  sprite-quad!  1+
            loop
        then
    loop
    batch @ swap sprites-count ;
fvariable fl  fvariable uu  fvariable vv  fvariable du  fvariable dv
behaviour shattering
  extends a-scene-effect
  on spawned ( -- )  scene-subscribe  $100 $19 flags! ;
  on effect-start ( x y z colour kind -- )   \ (the colour a GS word: R G B A from the low byte; kept in `slow`)
      kind !  dup $FF and 24 lshift  over 8 rshift $FF and 16 lshift or  over 16 rshift $FF and 8 lshift or  swap 24 rshift $FF and or  slow !
      cell>at ;
  on shards-start ( tex u v du dv floor -- )   \ (Effect6FF60_SetParams: thrown, the origin raised by the floor)
      cell>f fl f!  cell>f dv f!  cell>f du f!  cell>f vv f!  cell>f uu f!  ( tex )
      0 swap -1 tex!  batch @  uu f@ vv f@  uu f@ du f@ f+  vv f@ dv f@ f+  sprites-uv
      16 #shown !  16 0 do  i shard  loop
      at float+ f@ fl f@ f+ at float+ f! ;
  on tick ( -- )  shards-step 0= if  self kill  then ;
  on frame-end ( -- )  shards-draw ;
end-behaviour
\ ---- SinkingSprite (a butterfly's dust; EffectMgr's): one mote slowly growing and sinking,
\ fading 0..3 a frame; gone the frame after it's faded out. A 32 x 32 cell at (64, 64) ----
: v@ ( k -- ) ( F: -- x )  fv fx f@ ;
: v! ( k -- ) ( F: x -- )  fv fx f! ;
fvariable ex  fvariable ey  fvariable ez
variable dc  create d5 3 floats allot  create d6 3 floats allot
behaviour sinking
  extends a-scene-effect
  on spawned ( -- )  scene-subscribe  $10 1 -1 tex!  $40 $40 32 32 512 256 1 cells!  $20 $19 flags!  0 slow ! ;
  on dust-start ( rgba x y z vx vy vz -- )
      cell>f 2 v!  cell>f 1 v!  cell>f 0 v!  cell>at  0 at@ place!
      dc !  dc @ 24 rshift $FF and  dc @ 16 rshift $FF and  dc @ 8 rshift $FF and  dc @ $FF and  0 col!
      rnd 0.2e f* 0.2e f+ fdup 0 size!  0e 0 p-rot fx f!  0 0 p-frame pc !  1 #shown ! ;
  on tick ( -- )   \ SinkingSprite_Update
      slow @ if  self kill exit  then
      1 slow !
      0 3 col @ 0> if
          0 slow !
          0 3 col @ 4 random - 0 max 0 3 col !
          1 v@ 0.025e f- 1 v!
          1 v@ -0.05e f< if  rnd 0.5e f- 0.02e f* 0.05e f- 1 v!  then
          0 p-x fx f@ 0 v@ f+ 0 p-x fx f!  0 p-y fx f@ 1 v@ f+ 0 p-y fx f!  0 p-z fx f@ 2 v@ f+ 0 p-z fx f!
          0 p-w fx f@ 0.001e f+ fdup 0 p-w fx f! 0 p-h fx f!
      then ;
end-behaviour
: dust ( rgba -- ) ( F: x y z vx vy vz -- )
    d6 2 floats + f!  d6 float+ f!  d6 f!  d5 2 floats + f!  d5 float+ f!  d5 f!
    d5 f@ f>cell  d5 float+ f@ f>cell  d5 2 floats + f@ f>cell  d6 f@ f>cell  d6 float+ f@ f>cell  d6 2 floats + f@ f>cell
    sinking fx-state s" dust" spawn send dust-start ;

\ ---- Butterflies (the scripts' 0x35; a room effect): butterflies circling a centre that makes
\ for a target, while a point they follow wanders about it; bobbing, banking, flapping; now and
\ then one sheds a mote of dust. Two wings each (a 40 x 64 cell of GAME_FIX's first sheet),
\ coloured from the executable's table (D_00412710). (Their size factor scales a homogeneous
\ point on the PS2 - no change - so all are one size.) ----
0 constant bC   3 constant bW   6 constant bT   9 constant bStarted  10 constant bTilt  11 constant bHead
12 constant bRz  13 constant bPh  16 constant bStep  19 constant bCirc  22 constant bCount  23 constant bLast
: wrap ( F: a -- a' )  angle-wrap ;
: turn-y ( F: x z a -- x' z' )   \ (sceVu0RotMatrixY: (0, 0, 1) to (sin a, 0, cos a))
    fdup fcos ca f!  fsin sa f!  tmp f!  fdup ca f@ f*  tmp f@ sa f@ f* f+  fswap fnegate sa f@ f*  tmp f@ ca f@ f* f+ ;
: butterflies-begin ( -- )   \ Butterflies_Start (and Init: 8 of them)
    32 0 do  0e i v!  loop
    3 0 do  rnd 2e f* pi f* pi f-  bPh i + v!  loop
    3 0 do  rnd pi f* pi f2/ f+ wrap  bStep i + v!  loop
    3 0 do  rnd 2e f* 2e f+  bCirc i + v!  loop
    8e bCount v! ;
: butterflies-step ( -- )   \ Butterflies_Update
    bT v@ bC v@ f-  bT 1+ v@ bC 1+ v@ f-  bT 2 + v@ bC 2 + v@ f-  ex f! ey f! ez f!   \ (ez x, ey y, ex z: the difference)
    ez f@ fsq ey f@ fsq f+ ex f@ fsq f+  fdup 1e f> if
        fsqrt 0.4e fswap f/  fdup ez f@ f* bC v@ f+ bC v!  fdup ey f@ f* bC 1+ v@ f+ bC 1+ v!  ex f@ f* bC 2 + v@ f+ bC 2 + v!
    else  f0> if  bT v@ bC v!  bT 1+ v@ bC 1+ v!  bT 2 + v@ bC 2 + v!  then  then
    pi rnd 10e f* 5e f- f* 180e f/  fdup bTilt v@ f+ fabs 0.17453e f> if  fnegate  then  bTilt v@ f+ bTilt v!
    bC v@ bW v@ f-  bC 2 + v@ bW 2 + v@ f-  fatan2  bHead v@ f- wrap  f0> if  1e  else  -1e  then
    pi rnd 20e f* 15e f+ f* 180e f/ f*  bHead v@ f+ wrap bHead v!
    rnd 0.4e f* 0.4e f+  fdup bHead v@ fsin f* bW v@ f+ bW v!  bHead v@ fcos f* bW 2 + v@ f+ bW 2 + v!
    bPh v@  rnd 20e f* 1.0472e f+ f+ wrap bPh v!
    bPh 1+ v@  pi rnd 9e f* 2e f+ f* 180e f/ f+ wrap bPh 1+ v!
    bPh 2 + v@  pi rnd 20e f* 40e f+ f* 180e f/ f+ wrap bPh 2 + v! ;
\ a point of a butterfly: turned by (tx ty tz: x, then y, then z), moved to (ax ay az)
fvariable tx  fvariable ty  fvariable tz  fvariable ax  fvariable ay  fvariable az
: turn3 ( F: x y z -- x' y' z' )
    wz f! wy f! wx f!
    tx f@ fdup fcos ca f! fsin sa f!  wy f@ ca f@ f*  wz f@ sa f@ f* f-  wz f@ ca f@ f*  wy f@ sa f@ f* f+  wz f! wy f!
    ty f@ fdup fcos ca f! fsin sa f!  wx f@ ca f@ f*  wz f@ sa f@ f* f+  wz f@ ca f@ f*  wx f@ sa f@ f* f-  wz f! wx f!
    tz f@ fdup fcos ca f! fsin sa f!  wx f@ ca f@ f*  wy f@ sa f@ f* f-  wx f@ sa f@ f*  wy f@ ca f@ f* f+  wy f! wx f!
    wx f@ ax f@ f+  wy f@ ay f@ f+  wz f@ az f@ f+ ;
fvariable flap  fvariable bob  fvariable wingx  fvariable wingy  variable sgn  variable nq
: wing-colour ( i -- rgba )   \ (the table's RGBA words: R in the low byte)
    7 and 4 * $412710 + 4 exe-bytes  dup 0= if  drop $80808080 exit  then  l@
    dup $FF and 24 lshift  over 8 rshift $FF and 16 lshift or  over 16 rshift $FF and 8 lshift or  swap 24 rshift $FF and or ;
: wing ( rgba F: side -- )   \ one wing (side -1 left, 1 right): (0 0 1) (side*x y 1) (0 0 -0.4) (side*x y -0.4)
    fdup wingx f@ f*  tmp f!  fdrop
    0e 0e 1e turn3  tmp f@ wingy f@ 1e turn3  0e 0e -0.4e turn3  tmp f@ wingy f@ -0.4e turn3
    batch @ nq @ rot sprite-quad!  1 nq +! ;
: butterflies-draw ( -- )   \ Butterflies_Draw
    batch @ 0< if  exit  then
    0 nq !
    bCount v@ f>s 0 ?do
        i 1 and 2* 1- sgn !   i i 1+ * s>f                                                  ( F: ii )
        fdup bStep v@ 0.92502e f+ f* wrap sgn @ s>f f*                                     ( F: ii a )
        fover bStep 1+ v@ 0.82030e f+ f* wrap sgn @ s>f f*                                 ( F: ii a b )
        frot bStep 2 + v@ 1.16937e f+ f* wrap sgn @ s>f f*                                 ( F: a b c )
        frot fdup bPh v@ bStep v@ f+ f+ wrap fsin 1.0472e f* flap f!                       ( F: b c a )
        frot bPh 1+ v@ bStep v@ f+ f+ wrap fsin 0.8e f*                                     ( F: c a bob1 )
        frot bPh 2 + v@ bStep v@ f+ f+ wrap fsin 0.4e f* f+ bob f!                         ( F: a )
        bCirc v@ bCirc 2 + v@ frot wrap turn-y  fswap bC v@ f+ ax f!  bC 2 + v@ f+ az f!   ( F: )
        bCirc 1+ v@ bC 1+ v@ f+ bob f@ f+ ay f!
        i 1+ s>f sgn @ s>f f* 0.87266e f*  fdup                                              ( F: turn turn )
        bW v@ bC v@ f-  bW 2 + v@ bC 2 + v@ f-  frot wrap turn-y  az f@ f+ az f!  ax f@ f+ ax f!   ( F: turn )
        bTilt v@ tx f!  bHead v@ f+ wrap ty f!  bRz v@ tz f!
        flap f@ fcos 0.7e f* wingx f!  flap f@ fsin wingy f!
        i wing-colour dup  -1e wing  1e wing
        flap f@ -1e f<  rnd 0.1e f< and  bCount v@ 2e f< 0= and  bLast v@ bTilt v@ f= 0= and if
            bTilt v@ bLast v!
            32 random $80 +  24 lshift  32 random 16 lshift or  32 random $80 + 8 lshift or  16 random $70 + or
            ax f@  ay f@ rnd 0.5e f* f+  az f@  ax f@ bC v@ f- 0.005e f*  rnd 0.05e f* 0.05e f+  az f@ bC 2 + v@ f- 0.005e f*
            dust
        then
    loop
    batch @ nq @ sprites-count ;
behaviour fluttering
  extends an-effect
  on spawned ( -- )
      -1 batch !  new-batch  self subscribe tick  self subscribe frame-end
      $10 0 0 tex!  0 0 40 64 512 256 1 cells!  2 1 flags!  butterflies-begin ;
  on butterflies-start ( x y z count -- )   \ Butterflies_SetParams
      ?dup if  s>f bCount v!  then
      s>f bT 2 + v!  s>f bT 1+ v!  s>f bT v!
      bStarted v@ f0= if  1e bStarted v!  3 0 do  bT i + v@ fdup bC i + v!  bW i + v!  loop  then ;
  on tick ( -- )  butterflies-step ;
  on frame-end ( -- )  butterflies-draw ;
end-behaviour

: specks ( count spread rgba -- ) ( F: x y z -- )   \ a speck swarm there
    >r >r >r  f>cell f>cell f>cell swap rot  r> r> r>  swarming fx-state s" specks" spawn  send specks-start ;

\ an effect of a behaviour: spawned, then started (the room's slots keep them)
: effect ( command kind beh -- id ) ( F: x y z -- )
    fx-state s" effect" spawn >r  >r >r  f>cell f>cell f>cell swap rot  r> r>  r@ send effect-start  r> ;
