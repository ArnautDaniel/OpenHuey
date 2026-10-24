\ subscreen/plate.fs - the word plate (the scripts' mode 2; src/game/subscreen.c
\ SubScreen_StateWordMake): a machine with a keyboard - QWERTY rows, and a done key - where she
\ spells a word of up to 8 letters and makes a plate bearing it (item $3F; at most 10 plates).
\ The cross types the key's letter (the 8th puts her on done); the triangle takes one back, or
\ leaves with none; Start jumps to done; L1 / R1 step through the alphabet. In the golem rooms
\ the plate is made quietly, with the machine's own sounds.
IN: subscreen.plate
USING: engine actors keys facts story-names room-names items items.names ps2-screen subscreen.state subscreen.art subscreen.input ;

\ the keys (D_0044B640): 3 rows of 10 - the letter and where it is; a wide key fills more places
create plate-keys
    'Q' , 32 , 161 ,  'W' , 74 , 161 ,  'E' , 116 , 161 ,  'R' , 158 , 161 ,  'T' , 200 , 161 ,
    'Y' , 242 , 161 ,  'U' , 284 , 161 ,  'I' , 326 , 161 ,  'O' , 368 , 161 ,  'P' , 410 , 161 ,
    'A' , 53 , 209 ,  'S' , 95 , 209 ,  'D' , 137 , 209 ,  'F' , 179 , 209 ,  'G' , 221 , 209 ,
    'H' , 263 , 209 ,  'J' , 305 , 209 ,  'K' , 347 , 209 ,  'L' , 389 , 209 ,  'L' , 389 , 209 ,
    'Z' , 74 , 257 ,  'X' , 116 , 257 ,  'C' , 158 , 257 ,  'V' , 200 , 257 ,  'B' , 242 , 257 ,
    'N' , 284 , 257 ,  'M' , 325 , 257 ,  '@' , 382 , 257 ,  '@' , 382 , 257 ,  '@' , 382 , 257 ,
'@' constant done-key
$27 constant done-place   \ (row 2, place 7)
: key ( place -- addr )  dup 4 rshift 10 *  swap $F and +  3 cells *  plate-keys + ;
: letter ( place -- c )  key @ ;
: row-of ( place -- row )  $F0 and ;
: col-of ( place -- col )  $F and ;

: along ( place d -- place' )   \ to the next key that way along its row, round (past a wide key's places)
    {: place d | from :}  place letter to from
    begin  place row-of  place col-of d + 10 wrap  +  to place   place letter from <> until  place ;
: up-down ( place d -- place' )   \ to the row above / below, round, at the same place along it
    over row-of 4 rshift +  3 wrap 4 lshift  swap col-of + ;
: find-letter ( c -- )   \ the cursor on the first key showing it
    {: c :}  30 0 do  i 10 /mod 4 lshift +  dup letter c = if  key-at !  unloop exit  then  drop  loop ;
: next-letter ( c d -- c' )   \ the letter after / before, round A..Z and done (the "@" before A)
    +  dup 'Z' > if  drop done-key  then  dup done-key < if  drop 'Z'  then ;

: golem-room? ( -- flag )   \ (the rooms with a golem's plate machine)
    room-id dup castle-2f-7 =  over old-mansion-1f-9 = or  swap old-mansion-b1-6 = or ;
: items-id ( -- id )  s" items" actor-named ;
: make-plate ( -- )   \ the plate made (if fewer than 10 and a word typed), and the menu closes
    plates 10 <  word-len @ 0>  and if
        word 8 0 do  dup i + c@ swap  loop  drop  items-id send give-plate
        3 word word-len @ message-param$
        golem-room? if  2 6 bank-sound  -1 quiet !  then
        plate-made story-flag-set
    then  close-menu ;
: type-letter ( c -- )
    word-len @ 8 = if  drop exit  then
    word word-len @ + c!  1 word-len +!
    word-len @ 8 = if  done-place key-at !  then
    golem-room? if  0 6 bank-sound  else  se-decide  then ;
: take-back ( -- )
    word-len @ 0= if  close-menu exit  then
    -1 word-len +!  0 word word-len @ + c!  se-cancel ;

: start-plate ( -- )  word 8 0 fill  0 word-len !  0 key-at ! ;
: plate-keys-read ( -- )
    confirm? if  key-at @ letter dup done-key = if  drop make-plate  else  type-letter  then exit  then
    back? if  take-back exit  then
    key-at @
    start-button button-pressed? if  done-place key-at !  then
    up? if  key-at @ -1 up-down key-at !  then
    down? if  key-at @ 1 up-down key-at !  then
    left? if  key-at @ -1 along key-at !  then
    right? if  key-at @ 1 along key-at !  then
    next-page? if  key-at @ letter 1 next-letter find-letter  then
    prev-page? if  key-at @ letter -1 next-letter find-letter  then
    key-at @ <> if  se-cursor  then ;

create one-letter 1 allot
: plate-view ( -- )
    4 page-art
    8 0 do  word i + c@ ?dup if   \ the word, in large letters
        one-letter c!  word-layer one-letter 1  $68 i $28 * + sx  $54 sy  box-ink  scale 2*  glyphs
    then  loop
    word-len @ 8 < if  word-len @ 40 * $60 +  $50 $F $80 part  then   \ where the next letter goes
    key-at @ key  dup cell+ @  swap 2 cells + @  $E $40 part           \ the key lit
    $180 $140 $D $80 part  10 plates - n>s  $198 $148 box-ink inked ;    \ the plates left
