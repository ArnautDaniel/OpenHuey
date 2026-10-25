\ subscreen/extras.fs - the mini-game (the extras room's door 0: Hewie guiding Fiona against the
\ clock). Its menu (the scripts' mode $A, src/game/subscreen.c SubScreen_StateExtrasMenu): the
\ introduction, challenges A and B, C once A has been cleared and D once B has, and exit - up and
\ down round them, each one's note and (a challenge) its three best times; the cross picks a
\ challenge or exit (the story's variable $2E: 0..3, $FF none), the triangle leaves. Its results
\ (mode $C, Results_Setup / Results_State): clearing A or B unlocks C or D (resident flags $52 /
\ $53), the time taken placed among the challenge's three best; shown a step every 30 frames -
\ the banner, the three times (the new one lit, a mark by a new best), then a challenge newly
\ unlocked - the cross or the triangle ends it. The texts are the extras room's own messages
\ ($60 + entry its name, $66 + entry its note). The menu's art is SUBSCR/SUBMG (in the background
\ group: the parts' textures 1..3 are its - the list's frame, the introduction's panel, each
\ challenge's picture of its stage); the results' pictures are the avoid prompt's
\ (SYSTEM/AVOID.TEX parts 0..4, as the original's).
IN: subscreen.extras
USING: engine game-state actors facts keys ps2-screen subscreen.state subscreen.art subscreen.input ;

: var@ ( n -- v )  progress pr.vars + c@ ;
: var! ( v n -- )  progress pr.vars + c! ;
: room-line ( msg line -- addr len )  0 swap line-of ;   \ (the room's own message, its line)
$2E constant challenge-var
$52 constant c-unlocked   $53 constant d-unlocked      \ (the system data's +0x2C bits 18 / 19)
$60 constant msg-entries   $66 constant msg-notes
0 constant intro-entry   5 constant exit-entry

\ ---- the menu ----
: entry ( i -- e )  cells mg-entries + @ ;
: entries! ( -- )   \ (SubScreen_Open's mode $A)
    0 {: n :}
    6 0 do
        i dup 3 = if  c-unlocked resident-flag? 0= if  drop -1  then  then
        dup 4 = if  d-unlocked resident-flag? 0= if  drop -1  then  then
        dup 0< if  drop  else  n cells mg-entries + !  n 1+ to n  then
    loop  n mg-n !  0 mg-at ! ;
: time$ ( h m s -- addr len )   \ (mm:ss; past an hour 59:59)
    rot if  2drop 59 59  then
    spell-reset  swap 10 /mod n>s +spell n>s +spell  s" :" +spell  10 /mod n>s +spell n>s +spell  spelled ;
: none? ( a -- flag )  dup c@ 99 =  over 1+ c@ 59 = and  swap 2 + c@ 59 = and ;
: rank$ ( i -- addr len )  case  0 of  s" 1st"  endof  1 of  s" 2nd"  endof  >r s" 3rd" r>  endcase ;
: best-times ( challenge y lit -- )   \ (the three: rank, mm:ss or -----; the lit one in palette 1)
    {: c y0 lit :}
    3 0 do
        i lit = 1 and {: hue :}   \ (the lit one in palette 1, added: Task_PrintfEx's colour)
        i rank$  $E0  y0 i $18 * +  hue inked
        c i record dup none? if  drop s" -----"  $121  else  dup c@ over 1+ c@ rot 2 + c@ time$  $124  then
        y0 i $18 * +  hue inked
    loop ;
: list-frame ( -- )   \ (its top, a piece a row, its foot: one more row past three)
    $40 $10 $1D $80 part  0 $40 $20 $80 part
    0 $70 $21 $80 part  0 $90 $21 $80 part  0 $B0 $21 $80 part
    mg-n @ case
        4 of  0 $D0 $22 $80 part  endof
        5 of  0 $D0 $21 $80 part  0 $F0 $22 $80 part  endof
        6 of  0 $D0 $21 $80 part  0 $F0 $21 $80 part  0 $110 $22 $80 part  endof
    endcase ;
: entry-art ( e -- )   \ (the introduction's panel; a challenge's picture and its times' frame)
    dup intro-entry = if  drop
        $160 $40 $23 $80 part  $C0 $40 $24 $80 part
        6 0 do  $160 $80 i $20 * + $25 $80 part  $C0 $80 i $20 * + $26 $80 part  loop
        $160 $140 $27 $80 part  $C0 $140 $28 $80 part  $160 $150 $29 $80 part  $C0 $150 $2A $80 part  exit
    then
    dup exit-entry < 0= if  drop exit  then
    dup 1 = swap 3 = or if  $1E  else  $1F  then  $C0 $40 rot $80 part
    $C0 $140 $20 $80 part  $C0 $170 $21 $80 part  $C0 $190 $22 $80 part ;
: extras-view ( -- )
    list-frame  mg-at @ entry entry-art
    mg-n @ 0 ?do
        i entry msg-entries + 0 room-line  $20  $68 i $20 * +  i mg-at @ = 2 and inked
    loop
    mg-at @ entry {: e :}
    e intro-entry = if  msg-notes 0 room-line $E0 $68 box-ink inked  msg-notes 1 room-line $E0 $80 box-ink inked  exit  then
    e exit-entry < if  msg-notes e + 0 room-line $E0 $70 box-ink inked  e 1- $15A -1 best-times  then ;
: challenge-picked ( v -- )  challenge-var var!  se-decide  close-menu ;
: extras-keys ( -- )
    up? if  mg-at @ 1- mg-n @ wrap mg-at !  se-cursor  then
    down? if  mg-at @ 1+ mg-n @ wrap mg-at !  se-cursor  then
    back? if  $FF challenge-var var!  se-cancel  close-menu exit  then
    confirm? if
        mg-at @ entry dup intro-entry = if  drop exit  then
        dup exit-entry = if  drop $FF  else  1-  then  challenge-picked
    then ;
: start-extras ( -- )  entries! ;

\ ---- the results ----
: run-time ( -- h m s )   \ (the story's clock_save: an hour at most)
    run-took @ 30 /  dup 3600 >= if  drop 0 59 59 exit  then  0 swap  60 /mod swap ;
: seconds-of ( h m s -- n )  rot 60 * rot + 60 * + ;
: place-time ( -- )   \ (among the challenge's three best: the rest moved down)
    challenge-var var@ dup 3 > if  drop exit  then  {: c :}
    run-time {: h m s :}  h m s seconds-of {: t :}
    3 0 do
        c i record {: a :}  t  a c@ a 1+ c@ a 2 + c@ seconds-of < if
            i res-rank !
            2 begin  dup i > while  c over 1- record  c 2 pick record 3 move  1-  repeat  drop
            h a c!  m a 1+ c!  s a 2 + c!  unloop exit
        then
    loop ;
: start-results ( -- )   \ (Results_Setup)
    0 res-step !  30 res-t !  -1 res-rank !  0 res-new !  $40 res-pulse !  0 res-up !
    challenge-var var@ case
        0 of  c-unlocked resident-flag? 0= if  c-unlocked resident-flag-set  -1 res-new !  then  endof
        1 of  d-unlocked resident-flag? 0= if  d-unlocked resident-flag-set  -1 res-new !  then  endof
    endcase
    place-time ;
$2D constant prompt-group   \ (the avoid prompt's art: avoid.fs)
create prompt-parts   0 , 192 , 368 , 32 , 2 ,   0 , 224 , 112 , 32 , 2 ,   0 , 128 , 416 , 64 , 1 ,
    448 , 32 , 64 , 112 , 3 ,   448 , 144 , 64 , 112 , 3 ,   \ (D_00429E00: u v w h palette)
: prompt-part ( x y i alpha -- )
    {: x y i alpha | a :}  i 5 * cells prompt-parts + to a
    x y  a 2 cells + @  a 3 cells + @  picture
    prompt-group 0  a 4 cells + @  a @  a cell+ @  a 2 cells + @  a 3 cells + @  alpha from ;
: results-view ( -- )
    $30 $70 2 res-pulse @ prompt-part
    res-step @ 0= if  exit  then
    $110 $100 3 $80 prompt-part  $C0 $100 4 $80 prompt-part
    res-rank @ 0= if  $50 $110 1 $80 prompt-part  then
    challenge-var var@ 3 min $110 res-rank @ best-times
    res-step @ 2 < if  exit  then
    res-new @ if  $50 $D0 0 $80 prompt-part  then ;
: results-keys ( -- )
    res-step @ 2 < if  -1 res-t +!  res-t @ 0= if  1 res-step +!  30 res-t !  then  then
    res-up @ if  -4 res-pulse +!  res-pulse @ $60 = if  0 res-up !  then
    else  4 res-pulse +!  res-pulse @ $80 = if  -1 res-up !  then  then
    res-step @ 2 < if  exit  then
    confirm? back? or if  close-menu  then ;
