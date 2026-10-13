\ obstacles.fs - the things in a room that can be pushed about (docs/subsystems/obstacles.md;
\ src/game/room_map.c Obstacles_* / Obstacle_*, the room manager's +0x9380). Up to 5 in the
\ played room, each over the grid of the pushable floor: a square is a pair of nav triangles
\ (`nav-square-step`), an obstacle covers parts of squares (its kind's: offsets and sizes from a
\ reference square), the triangles under it blocked ($20400000) and the ring of squares round
\ each part marked - its sides $20800000 (walls to the characters, and the pushable floor:
\ Fiona pushes from it), its corners $20000000. Its model is the room's object "oshi0n".
\ Pushed (`push-obstacle`: Fiona's push), it goes two squares (5) along the push over the 30
\ frames of its move (OBSTACLE.MTN), its scrape heard at the sixth. The table is a fact for all
\ (the queries below: Fiona's and the story's); only this actor changes it.
IN: obstacles
USING: engine game-state actors messages ;

5 constant #obstacles
<PRIVATE
\ ---- an obstacle: a record of the table ----
0 cells constant o.on   1 cells constant o.stopped   2 cells constant o.model
3 cells constant o.a    4 cells constant o.b         \ its reference square
5 cells constant o.ta   6 cells constant o.tb        \ the square it is moving to (-1 none)
7 cells constant o.step  8 cells constant o.frames   \ its move: the frame, the frames
9 cells constant o.nparts
10 cells constant o.parts                            \ 3 parts: square a, b, dx, dz, w, d
6 cells constant /part
o.parts 3 /part * + constant o.ox                    \ floats: its model's offset x, z
o.ox 1 floats + constant o.oz
o.oz 1 floats + constant o.x                         \ its middle x y z
o.x 1 floats + constant o.y
o.y 1 floats + constant o.z
o.z 1 floats + constant o.dx                         \ the push x, z
o.dx 1 floats + constant o.dz
o.dz 1 floats + constant /obstacle
create table  #obstacles /obstacle * allot
: obst ( i -- o )  /obstacle * table + ;
: part ( o k -- p )  /part * o.parts + + ;
create saved  #obstacles 2 * cells allot             \ +0x580: the places kept (a, b)
create spots  #obstacles 3 * floats allot            \ +0x5B0: the spots kept (x y z)
: valid? ( i -- flag )  0 #obstacles within ;
: on? ( i -- flag )  dup valid? if  obst o.on + @  else  drop false  then ;

\ the move (OBSTACLE.MTN's one: 30 frames, 5 in all)
30 constant move-frames
create move-steps
  0e f, 0e f, 0e f, 0e f, 0e f, 0.070092e f, 0.095197e f, 0.142290e f, 0.211369e f, 0.285178e f,
  0.347676e f, 0.408945e f, 0.466564e f, 0.567652e f, 0.655631e f, 0.769039e f, 0.576050e f,
  0.290233e f, 0.093540e f, 0.021064e f, 0e f, 0e f, 0e f, 0e f, 0e f, 0e f, 0e f, 0e f, 0e f, 0e f,

\ its kinds (ObstacleKinds, in the executable: the model's offset x, z, the number of parts,
\ then each part's dx, dz, w, d)
$47A938 constant kinds-at
: kind@ ( kind -- addr | 0 )  4 * kinds-at + 4 exe-bytes dup if  l@ 64 exe-bytes  then ;
: s32@ ( addr -- n )  l@ dup $80000000 and if  $100000000 -  then ;

\ ---- squares ----
: step ( a b -- a' b' ) ( F: dx dz -- )  nav-square-step drop ;   \ (unchanged at the mesh's edge)
fvariable mx  fvariable my  fvariable mz
: sq-middle ( a b -- )   \ into mx my mz: the two triangles' middles' mean
    tri-center mz f! my f! mx f!  tri-center
    mz f@ f+ 2e f/ mz f!  my f@ f+ 2e f/ my f!  mx f@ f+ 2e f/ mx f! ;

\ the parts' squares from the reference square (Obstacle_PartSquares)
variable sa  variable sb  variable pp
: sign ( n -- ) ( F: -- s )  0< if  -1e  else  1e  then ;
: steps ( n -- ) ( F: dx dz -- )   \ sa sb that many squares along
    abs 0 ?do  fover fover  sa @ sb @ step  sb ! sa !  loop  fdrop fdrop ;
: part-squares ( o -- )
    dup o.nparts + @ 0 ?do
        dup i part pp !
        dup o.a + @ sa !  dup o.b + @ sb !
        pp @ 2 cells + @  dup sign 0e  steps
        pp @ 3 cells + @  0e dup sign  steps
        sa @ pp @ !  sb @ pp @ cell+ !
    loop  drop ;

\ ---- walking a part's squares and the ring round it, a visitor for each square ----
\ (the visitor: ( a b corner? -- stop? ); first `shift-n` squares along the push (pdx, pdz))
variable visitor  variable shift-n  fvariable pdx  fvariable pdz
: visit ( a b corner? -- a b stop? )  >r 2dup r> visitor @ execute ;
: shifted ( a b -- a' b' )  shift-n @ 0 ?do  pdx f@ pdz f@ step  loop ;
variable pw  variable pd  variable row-a  variable row-b
: area ( p -- )   \ (part_area: row by row)
    dup 4 cells + @ pw !  dup 5 cells + @ pd !
    dup @ swap cell+ @  shifted
    pd @ 0 ?do
        over row-a !  dup row-b !
        pw @ 0 ?do  false visit drop  1e 0e step  loop
        2drop  row-a @ row-b @  0e 1e step
    loop  2drop ;
fvariable rdx  fvariable rdz
: side ( k -- n )   \ side k's direction into rdx rdz; its length
    dup 2 < if  1e  else  -1e  then
    1 and if  rdz f!  0e rdx f!  pd @  else  rdx f!  0e rdz f!  pw @  then ;
: ring ( p -- stop? )   \ (part_ring: from the -x -z corner, each side then its corner)
    dup 4 cells + @ pw !  dup 5 cells + @ pd !
    dup @ swap cell+ @  shifted
    -1e 0e step  0e -1e step
    4 0 do
        i side 0 ?do  rdx f@ rdz f@ step  false visit if  2drop unloop unloop true exit  then  loop
        rdx f@ rdz f@ step  true visit if  2drop unloop true exit  then
    loop  2drop false ;
: each-area ( o xt -- )  visitor !  dup o.nparts + @ 0 ?do  dup i part area  loop  drop ;
: each-ring ( o xt -- stop? )
    visitor !  dup o.nparts + @ 0 ?do  dup i part ring if  drop unloop true exit  then  loop  drop false ;

\ ---- the floor's flags ----
variable fset  variable fbits
: flag-pair ( a b set? bits -- )
    fbits ! fset !  fset @ swap fbits @ nav-tri-flags!  fset @ swap fbits @ nav-tri-flags! ;
: block-sq ( a b corner? -- stop? )  drop true $20400000 flag-pair false ;
: unblock-sq ( a b corner? -- stop? )  drop false $20400000 flag-pair false ;
: mark-sq ( a b corner? -- stop? )  if  true $20000000  else  true $20800000  then  flag-pair false ;
: unmark-sq ( a b corner? -- stop? )  drop false $20800000 flag-pair false ;
: block ( o -- )  ['] block-sq each-area ;
: unblock ( o -- )  ['] unblock-sq each-area ;
: mark-ring ( o -- )  ['] mark-sq each-ring drop ;
: unmark-ring ( o -- )  ['] unmark-sq each-ring drop ;

\ ---- queries (anyone's) ----
variable wanted
: wanted-sq ( a b corner? -- stop? )  if  2drop false exit  then  wanted @ =  swap wanted @ = or ;
PRIVATE>
\ triangle `tri` is on the ring round obstacle i (Obstacle_OnRing: its sides)
: ring-has? ( i tri -- flag )  wanted !  obst  0 shift-n !  ['] wanted-sq each-ring ;
\ which obstacles have `tri` on the ring round them, a bit each (Obstacles_RingMask)
: ring-mask ( tri -- bits )
    0  #obstacles 0 do  i on? if  i 2 pick ring-has? if  1 i lshift or  then  then  loop  nip ;
\ `tri` is obstacle i's square - the one it is moving to on its move's last frame (+0x34)
: on-square? ( i tri -- flag )
    over on? 0= if  2drop false exit  then  swap obst >r
    r@ o.step + @  r@ o.frames + @ 1- = if  r@ o.ta + @ over =  r> o.tb + @ rot =
    else  r@ o.a + @ over =  r> o.b + @ rot =  then  or ;
\ where it stands: its middle by its model's offset (+0x50)
: obstacle-at ( i -- ) ( F: -- x y z )
    obst >r  r@ o.x + f@ r@ o.ox + f@ f+  r@ o.y + f@  r@ o.z + f@ r> o.oz + f@ f+ ;

<PRIVATE
\ can it be pushed along (dx, dz) (Obstacles_CanPush, Obstacle_CanPush): not stopped; nobody
\ but her on the ring round it moved two or one squares along (Obstacle_RingClear); and the
\ place two squares along, its own blocks lifted, the walkable floor ($100) without $440080
create others 12 cells allot  variable #others
: other-tris ( -- )   \ (the other characters in the room: their triangles)
    0 #others !
    s" hewie" actor-named  s" debilitas" actor-named  s" daniella" actor-named  s" riccardo" actor-named  s" lorenzo" actor-named  s" lorenzo2" actor-named  s" kind39" actor-named  s" kind37" actor-named  s" tintstalker" actor-named  s" kind12" actor-named
    10 0 do
        dup 0< 0= if  dup body? if  dup body-room room-id = if
            dup body-tri  #others @ cells others + !  1 #others +!
        then  then  then  drop
    loop ;
: someone-sq ( a b corner? -- stop? )
    drop  #others @ 0 ?do
        i cells others + @  dup 3 pick =  swap 2 pick = or if  2drop true unloop exit  then
    loop  2drop false ;
create own 200 cells allot  variable #own
: own-sq ( a b corner? -- stop? )  drop  #own @ cells own + !  1 #own +!  #own @ cells own + !  1 #own +!  false ;
: own? ( tri -- flag )  #own @ 0 ?do  i cells own + @ over = if  drop true unloop exit  then  loop  drop false ;
variable any  variable all
: test-tri ( tri -- )
    dup nav-flags  swap own? if  $400000 invert and  then
    dup any @ or any !  all @ and all ! ;
: test-sq ( a b corner? -- stop? )  drop  test-tri test-tri  false ;
: ring-clear? ( o -- flag )
    other-tris
    2 shift-n !  dup ['] someone-sq each-ring if  drop false exit  then
    1 shift-n !  ['] someone-sq each-ring 0= ;
PRIVATE>
: can-push? ( i -- flag ) ( F: dx dz -- )
    pdz f! pdx f!
    dup on? 0= if  drop false exit  then  obst
    dup o.stopped + @ if  drop false exit  then
    dup ring-clear? 0= if  drop false exit  then
    0 #own !  0 shift-n !  dup ['] own-sq each-area
    0 any !  $100 all !  2 shift-n !  ['] test-sq each-area
    any @ $440080 and 0=  all @ $100 and 0<> and ;

<PRIVATE
\ ---- the model: the room's object "oshi0n" ----
create model-name 8 allot
: model$ ( n -- addr len )  s" oshi00" model-name swap move  [char] 0 + model-name 5 + c!  model-name 6 ;
: model? ( n -- flag )
    model$  placed-count 0 ?do
        i placed-info drop fdrop fdrop fdrop  2over compare 0= if  2drop true unloop exit  then
    loop  2drop false ;
: follow ( o -- )   \ (Obstacles_ModelsFollow)
    >r  r@ o.model + @ model$  r@ o.x + f@ r@ o.ox + f@ f+  r@ o.y + f@  r@ o.z + f@ r> o.oz + f@ f+
    placed-at ;

\ ---- placed, pushed, moved ----
variable mo  variable ma  variable mb  variable mdef
: middle! ( o -- )  mo !  mo @ o.a + @ mo @ o.b + @ sq-middle  mx f@ mo @ o.x + f!  my f@ mo @ o.y + f!  mz f@ mo @ o.z + f! ;
: placed ( o -- )   \ (Obstacle_Placed: its middle, its parts' squares, blocked, its ring marked)
    dup middle!  dup part-squares  0 shift-n !  dup block  mark-ring ;
: reset ( o -- )
    dup /obstacle 0 fill  -1 over o.a + !  -1 over o.b + !  -1 over o.ta + !  -1 swap o.tb + ! ;
: make ( i n kind a b -- )   \ (obstacle_make, Obstacles_PlaceAt)
    mb ! ma !  kind@ dup 0= if  drop 2drop exit  then  mdef !                  ( i n )
    mdef @ 8 + l@ 1 4 within 0= if  2drop exit  then
    dup model? 0= if  2drop exit  then
    swap obst dup reset mo !  mo @ o.model + !
    mdef @ sf@ mo @ o.ox + f!  mdef @ 4 + sf@ mo @ o.oz + f!
    ma @ mo @ o.a + !  mb @ mo @ o.b + !
    mdef @ 8 + l@ 0 ?do
        mdef @ 12 + i 16 * +  mo @ i part                                     ( src p )
        over s32@ over 2 cells + !  over 4 + s32@ over 3 cells + !
        over 8 + l@ over 4 cells + !  swap 12 + l@ swap 5 cells + !
        1 mo @ o.nparts + +!
    loop
    -1 mo @ o.on + !  mo @ placed  mo @ follow ;
: moved ( o -- )   \ the move done: the square it went to its reference (Obstacle_Moved)
    dup o.ta + @ 0< if  drop exit  then  mo !
    mo @ o.ta + @ mo @ o.a + !  mo @ o.tb + @ mo @ o.b + !  -1 mo @ o.ta + !  -1 mo @ o.tb + !
    mo @ part-squares  mo @ middle!  mo @ o.frames + @ mo @ o.step + ! ;
: acoustics-id ( -- id )  s" acoustics" actor-named ;
: move-frame ( o -- )   \ (Obstacle_MoveFrame: its scrape and its noise at the sixth frame)
    dup o.step + @ over o.frames + @ = if  drop exit  then
    dup o.step + @ 6 = if
        dup o.x + f@  dup o.y + f@  dup o.z + f@  0 6 bank-sound-at
        $1F room-id  2 pick o.a + @  -1  fiona-noise  acoustics-id send noise
    then
    1 over o.step + +!
    dup o.step + @ over o.frames + @ = if  moved exit  then
    mo !  mo @ o.step + @ 1- floats move-steps + f@                         ( F: k )
    fdup mo @ o.dx + f@ f*  mo @ o.x + f@ f+ mo @ o.x + f!
    mo @ o.dz + f@ f*  mo @ o.z + f@ f+ mo @ o.z + f! ;
\ pushed along (dx, dz) (Fiona_StatePush: Obstacles_SetTarget, Pull, Settle, StartMove)
: pushed ( i -- ) ( F: dx dz -- )
    pdz f! pdx f!
    dup on? 0= if  drop exit  then  obst mo !
    pdx f@ mo @ o.dx + f!  pdz f@ mo @ o.dz + f!
    mo @ o.a + @ mo @ o.b + @  pdx f@ pdz f@ step  pdx f@ pdz f@ step  mo @ o.tb + !  mo @ o.ta + !
    0 shift-n !  mo @ unblock  mo @ unmark-ring
    #obstacles 0 do  i on? if  i obst mo @ <> if  0 shift-n !  i obst mark-ring  then  then  loop
    2 shift-n !  mo @ block  mo @ mark-ring
    0 mo @ o.step + !  move-frames mo @ o.frames + ! ;
: keep-spot ( i -- )  dup obstacle-at  3 * floats spots +  dup 2 floats + f!  dup 1 floats + f!  f! ;
: model-back ( i n -- )   \ (Obstacles_ModelBack: that model at its kept spot, its model)
    2dup swap obst o.model + !  model$ rot 3 * floats spots +  dup f@  dup 1 floats + f@  2 floats + f@  placed-at ;
: kept ( i -- a b )  2 * cells saved +  dup @ swap cell+ @ ;
: keep ( i a b -- )  rot 2 * cells saved +  tuck cell+ !  ! ;

\ ---- the actor ----
state: obstacles-state
end-state
behaviour pushables
  on spawned ( -- )  self subscribe tick  self subscribe leaving-room  #obstacles 0 do  i obst reset  loop ;
  on leaving-room ( room exit -- )  2drop  #obstacles 0 do  i obst reset  loop ;
  on tick ( -- )  #obstacles 0 do  i on? if  i obst dup move-frame follow  then  loop ;
  on place-obstacle ( i n kind a b -- )  4 pick valid? if  make  else  2drop 2drop drop  then ;
  on place-obstacle-kept ( i n kind -- )  2 pick valid? if  2 pick kept make  else  drop 2drop  then ;
  on stop-obstacle ( i -- )  dup on? if  obst o.stopped + -1 swap !  else  drop  then ;
  on keep-obstacle ( i -- )  dup valid? if  dup obst o.a + @  over obst o.b + @  keep  else  drop  then ;
  on keep-obstacle-at ( i a b -- )  2 pick valid? if  keep  else  drop 2drop  then ;
  on keep-obstacle-spot ( i -- )  dup valid? if  keep-spot  else  drop  then ;
  on obstacle-model-to-spot ( i n -- )  over valid? over model? and if  model-back  else  2drop  then ;
  on push-obstacle ( i dx dz -- )  >r cell>f r> cell>f  pushed ;
  on obstacle-moved ( i -- )  dup on? if  obst moved  else  drop  then ;
end-behaviour

PRIVATE>
: obstacles-spawn ( -- id )  pushables obstacles-state s" obstacles" spawn ;
