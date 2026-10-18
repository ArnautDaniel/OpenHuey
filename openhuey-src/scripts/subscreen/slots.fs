\ subscreen/slots.fs - the slot machine, the synthesizer (the scripts' mode 3; src/game/items.c
\ SlotMachine_*, sy_finish, ItemFound_*).
\ She puts a medallion in. Ten rows are rolled in turn: the top one drawn by the medallion's odds,
\ then each row's reel (picked by the medallion's odds) spins until the cross stops it - or it
\ stops by itself once it is spinning at full speed. Then the cells between the rows are scored:
\ a cell matches when its two rows show the same symbol (5 is wild). The symbols' counts give a
\ result, and the result an item - forged, in an amount by how many cells matched.
IN: subscreen.slots
USING: engine actors common messages keys items items.names subscreen.state subscreen.art subscreen.input subscreen.tables ;

: items-id ( -- id )  s" items" actor-named ;
5 constant wild   4 constant blank
magist-medallion constant fallback   \ (when nothing is left to forge)
\ the machine's states: choosing a medallion, none to put in, rolling, what came out
enum: slot-machine  choosing-medal no-medals rolling came-out ;

\ ---- the medallions she can put in ----
: medallion-id ( m -- id )  medallion + ;   \ (the six in order from the plain one)
: held-of ( m -- n )  cells stock + @ ;
: count-stock ( -- any? )  0  6 0 do  i medallion-id items-of  dup i cells stock + !  0<> or  loop ;
: offered ( -- n )  0  6 0 do  i held-of 0<> 1 and +  loop ;   \ (those she has, listed)
: chosen ( -- m | -1 )   \ the one at the cursor
    0  6 0 do  i held-of if  dup slot-cursor @ = if  drop i unloop exit  then  1+  then  loop  drop -1 ;

\ ---- the rows and the reel ----
: row@ ( i -- sym )  cells rows + @ ;
: row! ( sym i -- )  cells rows + ! ;
: reel-symbol ( pos -- sym )  reel @ 35 * + cells reels + @ ;
: reel-step ( -- )  0 reel-tick !  reel-pos @ 1+ 35 mod reel-pos ! ;
: by-chance ( addr n -- i )   \ which of n chances (percentages) a roll of 100 falls in (-1: none)
    {: chances n | F: sum :}  rnd 100e f* {: F: roll :}
    n 0 ?do  chances i cells + @ s>f sum f+ to sum  roll sum f<= if  i unloop exit  then  loop  -1 ;
: odds ( table per n -- i )   \ by-chance in the medallion's row of a table (n chances; none: n)
    {: table per n :}  table medal @ per * cells +  n by-chance  dup 0< if  drop n  then ;
: clear-rows ( -- )
    10 0 do  -1 i row!  loop  22 0 do  -1 i cells matches + !  loop
    -1 result !  -1 row-at !  0 reel-turning !  0 stopping !  0 all-rolled ! ;
: start-slots ( -- )  0 slot-cursor !  clear-rows  count-stock choosing-medal no-medals ? slot-state ! ;

\ ---- rolling (SlotMachine_StateRoll) ----
: top-row ( -- )   \ the first row: drawn straight from the medallion's odds
    first-chances 6 5 odds  0 row!  0 row-at !  1 6 bank-sound ;
: next-reel ( -- )   \ the next row's reel, at a place by chance, spinning from slow
    reel-chances 11 10 odds  cells reels-picked + @ reel !
    rnd 35e f* f>s 34 min reel-pos !  0 reel-speed !  0 reel-time !  0 reel-tick !  -1 reel-turning ! ;
: spin ( -- )   \ a symbol on every 5 frames; faster each 31 frames, 8 levels
    1 reel-tick +!  reel-tick @ 5 >= if  reel-step  then
    1 reel-time +!  reel-time @ 31 >= if  0 reel-time !  reel-speed @ 1+ 8 min reel-speed !  then ;
: settle ( -- )   \ stopping: eases on onto the next symbol, or back onto this one
    reel-tick @ 0= if  exit  then
    reel-tick @ 3 >= if  1 reel-tick +!  reel-tick @ 5 >= if  reel-step  then  else  -1 reel-tick +!  then ;
: stopped ( -- )   \ the row takes the reel's symbol
    0 stopping !  reel-pos @ reel-symbol  row-at @ 1+ row!  1 row-at +!
    row-at @ 9 >= if  -1 all-rolled !  else  0 reel-turning !  then ;

\ ---- what came out (sy_finish, SlotMachine_Result) ----
: count@ ( sym -- n )  cells matched + @ ;
: score-cell ( c -- )   \ its two rows: the same symbol, or one wild (wild with wild counts as 0)
    {: c | one other sym :}
    c 2* cells cell-rows + @ row@ to one   c 2* 1+ cells cell-rows + @ row@ to other
    one other =  one wild =  other wild =  or or 0= if  -1 c cells matches + !  exit  then
    one wild = if  other  else  one  then  to sym   sym wild = if  0 to sym  then
    sym c cells matches + !  1 sym cells matched + +! ;
: score ( -- )  5 0 do  0 i cells matched + !  loop  22 0 do  i score-cell  loop ;
: matched-total ( -- n )  0 count@ 1 count@ + 2 count@ + 3 count@ + ;
: kinds ( -- n )  0  4 0 do  i count@ 0> 1 and +  loop ;
: kinds-but-0 ( -- n )  kinds  0 count@ 0> 1 and - ;
: verdict ( -- r )   \ the more cells and kinds the better: 0 best .. 15; -1 none
    kinds 4 >= matched-total 8 >= and  matched-total 22 >= or if  0 exit  then
    kinds 4 >=  matched-total 18 >= or if  1 exit  then
    kinds-but-0 3 >=  matched-total 15 >= or if  2 exit  then
    3 count@ 7 >= if  $10 exit  then   3 count@ 5 >= if  3 exit  then   1 count@ 5 >= if  4 exit  then
    3 count@ 3 >= if  5 exit  then    kinds 3 >= if  6 exit  then
    3 count@ 2 >= if  7 exit  then    1 count@ 2 >= if  8 exit  then    2 count@ 2 >= if  9 exit  then
    0 count@ 3 >= if  10 exit  then   3 count@ 0> if  11 exit  then    1 count@ 0> if  12 exit  then
    2 count@ 0> if  13 exit  then    0 count@ 2 >= if  14 exit  then   0 count@ 0> if  15  else  -1  then ;
: size ( -- s | -1 )   \ the haul by how many cells matched: 8+, 6+, 4+, any
    matched-total  dup 8 >= if  drop 0 exit  then  dup 6 >= if  drop 1 exit  then
    dup 4 >= if  drop 2 exit  then  0> if  3  else  -1  then ;
: result-item ( k -- id )  result @ 9 * + cells results + @ ;   \ (0..7 its items, 8 its amounts' row)
: haul ( -- n )  size dup 0< if  drop 0 exit  then  8 result-item 4 * + cells amounts + @ ;
create candidates 8 cells allot   variable #candidates
: forgeable? ( id -- flag )   \ (-1 none; the boots, earrings and chokers only once)
    dup 0< if  drop false exit  then  dup $80 $90 within if  items-of 0=  else  drop true  then ;
: gather-candidates ( -- )
    0 #candidates !
    8 0 do  i result-item dup forgeable? if  #candidates @ cells candidates + !  1 #candidates +!  else  drop  then  loop ;
: forge ( -- )
    score  verdict result !  0 amount !
    result @ 0< 0= if  haul amount !  amount @ 0= if  -1 result !  then  then
    result @ 0< 0= if
        gather-candidates
        #candidates @ 0= if  fallback forged !  1 amount !
        else  #candidates @ s>f rnd f* f>s  #candidates @ 1- min  cells candidates + @ forged !  then
    then
    0 found-t !  0 found-heard !  came-out slot-state ! ;
: roll-frame ( -- )   \ (SlotMachine_StateRoll, a frame)
    row-at @ 0< if  top-row  then
    stopping @ if  settle  reel-tick @ 0= if  stopped  then  then
    stopping @ if  exit  then
    reel-turning @ 0= if  next-reel  then
    all-rolled @ if  forge exit  then
    spin
    confirm?  reel-speed @ 8 = or if  2 6 bank-sound  -1 stopping !  then ;

\ ---- drawing (SlotMachine_DrawRow, DrawCell, MaterialList, ItemFound_Step) ----
create symbol-palettes  6 , 3 , 4 , 5 , 0 , 7 ,
: palette-of ( sym -- pal )  dup 0 6 within if  cells symbol-palettes + @  else  drop 1  then ;
: draw-row ( i sym -- )   \ its symbol, 32 x 32 at its place
    {: i sym :}  i 2* cells row-places + @ 16 +  i 2* 1+ cells row-places + @ 16 +  32 32 picture
    back-group 4 sym palette-of  $60 0 32 32  $80 from ;
: draw-cell ( c -- )   \ its symbol's piece of the machine, flipped as its place needs
    {: c | u v w h :}  c 8 * cells cell-draws + piece !
    0 column to u  1 column to v  2 column to w  3 column to h
    4 column 5 column w h picture
    7 column if  u w + to u  w negate to w  then   \ (flipped left-right)
    6 column if  v h + to v  h negate to h  then   \ (flipped up-down)
    back-group 4  c cells matches + @ palette-of  u v w h  $80 from ;
: blinker ( -- )   \ the start prompt's light: 128 down to 64 and back, over 60 frames
    blink-t @ 1+ 60 mod blink-t !
    blink-t @ 30 - abs 64 * 30 / $40 + $80 min  {: alpha :}
    $1B8 $100 $40 $30 picture  back-group 4 8 $1A0 0 $40 $30 alpha from ;
: say ( msg -- )   \ the machine's line (the room's own messages), in a box at the bottom
    $100 $172 $1C0 $40 shade  0 0 line-of  $50 $164 false text ;
: medal-list ( -- )   \ the medallions she has, with how many; the one at the cursor lit
    {: | k y :}
    $2 say
    0 $40 $F8 $30 picture  back-group 4 2 $108 $30 $F8 $30 $80 from
    offered 0 ?do  0 $70 i $20 * + $F8 $20 picture  back-group 4 2 $108 $50 $F8 $20 $80 from  loop
    0 $70 offered 1- 0 max $20 * + $F8 $20 picture  back-group 4 2 $108 $70 $F8 $20 $80 from
    6 0 do  i held-of if
        k $20 * $60 + to y
        i medallion-id name  $20 y  k slot-cursor @ =  text
        i held-of n>s  $B1 y  false text
        k 1+ to k
    then  loop ;
\ the panels as what came out shows: over the rows (in 3 frames, out over 12), then the item's
: covering ( t -- alpha up? )   \ the panel over the rows: in over 3 frames, out over 12 (then up)
    dup 4 < if  128 * 3 /  false exit  then
    3 - dup 13 >=  swap 12 min  12 swap - 128 * 12 /  swap ;
: item-shown ( t -- alpha done? )   \ the item's panel: on from the 1st frame, out over 13 from the 10th
    dup 10 < if  0<> 128 and  false exit  then
    9 - dup 14 >=  swap 13 min  13 swap - 128 * 13 /  swap ;
: found-panels ( -- up? done? )
    found-t @ covering {: alpha up :}
    $170 $A0 $40 $B0 picture  back-group 4 $A $90 $20 $40 $B0 alpha from
    result @ 0< if  up up exit  then
    found-t @ item-shown {: alpha2 done :}
    $180 $B0 $38 $90 picture  back-group 4 9 $D0 $20 $38 $90 alpha2 from  up done ;
: found-view ( -- )
    10 0 do  i i row@ draw-row  loop
    22 0 do  i cells cell-order + @  dup cells matches + @ dup 0< swap blank = or if  drop  else  draw-cell  then  loop
    found-panels drop if
        result @ 0< if  5 say  else
            found-heard @ 0= if  3 6 bank-sound  -1 found-heard !  then
            $100 $172 $100 $28 shade  4 0 0 line-of  $60 $164 false text
            forged @ name  $A8 $164 true text
            amount @ 1 <> if  amount @ count$  $140 $164 false text  then
        then
    then
    found-t @ 50 < if  1 found-t +!  then ;
: rolling-view ( -- )
    10 0 do
        i row@ dup 0< 0= if  i swap draw-row
        else  drop  i row-at @ 1+ = if  i reel-pos @ reel-symbol draw-row  then  then
    loop  $10 say ;
: slots-view ( -- )
    6 page-art  blinker
    slot-state @ case
        choosing-medal of  medal-list  endof
        no-medals of  3 say  endof
        rolling of  rolling-view  endof
        came-out of  found-view  endof
    endcase ;

\ ---- the keys ----
: put-in ( -- )   \ the medallion at the cursor: one spent, the rolling begins
    chosen dup 0< if  drop exit  then  se-decide
    dup medal !  medallion-id items-id send take-item  clear-rows  rolling slot-state ! ;
: take-forged ( -- )   \ (what came out done, then confirmed: it is hers; again, or none left)
    result @ 0< 0= if  forged @ amount @ items-id send give-item  then
    0 slot-cursor !  clear-rows  count-stock choosing-medal no-medals ? slot-state ! ;
: done-showing? ( -- flag )   \ (the panels gone: the item's at frame 23, with none the first's at 16)
    result @ 0< if  found-t @ 16 >=  else  found-t @ 23 >=  then ;
: slots-keys ( -- )
    slot-state @ case
        choosing-medal of
            slot-cursor @
            up? if  slot-cursor @ 1- offered wrap slot-cursor !  then
            down? if  slot-cursor @ 1+ offered wrap slot-cursor !  then
            slot-cursor @ <> if  se-cursor exit  then
            back? if  close-menu exit  then
            confirm? if  put-in  then  endof
        no-medals of  confirm? back? or if  close-menu  then  endof
        rolling of  roll-frame  endof
        came-out of  done-showing? confirm? and if  take-forged  then  endof
    endcase ;
