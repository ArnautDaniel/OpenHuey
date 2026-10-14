\ subscreen/costumes.fs - the wardrobe (the scripts' mode 7; src/game/subscreen.c Costumes_Setup /
\ Costumes_State): the rows Fiona, Hewie (when he is here) and Exit; on a row the cross or right
\ opens its costumes - up / down among the unlocked ones (round), the cross picks one, the
\ triangle or left closes the list; the triangle on the rows goes to Exit, and Exit wears the
\ choices: her costume into variable $26, his into $27 (what she wore kept in $28 / $29 - the
\ room's script dresses them by the difference). The names and notes are the wardrobe room's
\ own messages ($42.. : the rows, $45 + i the names, $4E + i the notes; Hewie's after her six).
IN: subscreen.costumes
USING: engine game-state actors facts keys subscreen.state subscreen.art subscreen.input ;

\ her costumes in the list's order (the costume variable's values), and his; locked ones aren't
\ shown: her 7 until the first ending (A), her 8 until the third (C); his third until a hard game
\ is cleared with the TYPE Top Breeder or Dog Lover
create her-costumes  0 , 2 , 3 , 7 , 6 , 8 ,
6 constant #hers   3 constant #his
: her-costume ( i -- n )  cells her-costumes + @ ;
86 constant hard-cleared   8 constant type-top-breeder   12 constant type-dog-lover
: her-locked? ( i -- flag )
    her-costume case  7 of  1 resident-flag? 0=  endof  8 of  3 resident-flag? 0=  endof  >r false r>  endcase ;
: his-locked? ( i -- flag )
    2 = if  hard-cleared resident-flag?  type-top-breeder resident-flag? type-dog-lover resident-flag? or  and 0=  else  false  then ;
: var@ ( n -- v )  progress pr.vars + c@ ;
: var! ( v n -- )  progress pr.vars + c! ;

0 constant row-her   1 constant row-his   2 constant row-exit
$42 constant msg-rows   $45 constant msg-names   $4E constant msg-notes
: room-line ( msg line -- addr len )  0 swap line-of ;   \ (the room's own message, its line)

\ Hewie's row: he is up, in the dog's room ($37) or the story says so (its flag $18), not busy
: hewie-here? ( -- flag )
    s" hewie" actor-named  dup 0< if  drop false exit  then  dup body? 0= if  drop false exit  then
    body-room $37 =  $18 story-flag? or ;
: place-of-hers ( costume -- i | -1 )  #hers 0 do  dup i her-costume = if  drop i unloop exit  then  loop  drop -1 ;
: start-costumes ( -- )   \ (Costumes_Setup)
    $26 var@ $28 var!  $27 var@ $29 var!
    $26 var@ place-of-hers dup cost-her-worn !  cost-her-at !
    hewie-here? if  $27 var@ #his min  else  -1  then  dup cost-his-worn !  cost-his-at !
    row-her cost-row !  0 cost-open ! ;
: wear-chosen ( -- )   \ (costume_done) worn, the menu closes
    cost-her-worn @ her-costume $26 var!
    cost-his-worn @ 0< 0= if  cost-his-worn @ $27 var!  then
    se-decide  close-menu ;

: next-unlocked-costume ( i d n locked-xt -- i' )   \ round, past the locked
    {: i d n locked :}  i  begin  d + n wrap  dup locked execute 0= until ;
: wardrobe-row-step ( d -- )
    cost-row @ +  0 max  row-exit min                     ( row )
    dup row-his = cost-his-worn @ 0< and if  drop  cost-row @ row-her = if  row-exit  else  row-her  then  then
    dup cost-row @ <> if  cost-row !  se-cursor  else  drop  then ;
: wardrobe-list-step ( d -- )
    cost-row @ row-her = if  cost-her-at @ swap #hers ['] her-locked? next-unlocked-costume cost-her-at !
    else  cost-his-at @ swap #his ['] his-locked? next-unlocked-costume cost-his-at !  then  se-cursor ;
: choose-costume ( -- )
    cost-row @ row-her = if  cost-her-at @ cost-her-worn !  else  cost-his-at @ cost-his-worn !  then  se-decide  0 cost-open ! ;
: costumes-keys ( -- )
    cost-her-worn @ 0< if  close-menu exit  then   \ (no costume of hers known: it just ends)
    cost-open @ if
        up? if  -1 wardrobe-list-step exit  then   down? if  1 wardrobe-list-step exit  then
        confirm? if  choose-costume exit  then
        back? left? or prev-page? or if  0 cost-open !  se-cancel  then  exit
    then
    up? if  -1 wardrobe-row-step exit  then   down? if  1 wardrobe-row-step exit  then
    cost-row @ row-exit = if
        confirm? back? or if  wear-chosen  then  exit
    then
    confirm? right? or next-page? or if  1 cost-open !  se-decide  exit  then
    back? if  row-exit cost-row !  se-cancel  then ;

\ ---- drawing: the rows on the left, the row's costumes (the cursor's lit, the worn one marked),
\ the note of the one under the cursor ----
: wardrobe-rows ( -- )
    0 {: y :}
    msg-rows 0 room-line  $30 $40  cost-row @ row-her = text
    cost-his-worn @ 0< 0= if  msg-rows 1+ 0 room-line  $30 $60  cost-row @ row-his = text  $20 to y  then
    msg-rows 2 + 0 room-line  $30 $60 y +  cost-row @ row-exit = text ;
: wardrobe-list ( first n locked-xt at worn -- )
    {: first n locked at worn | y :}  $48 to y
    n 0 do
        i locked execute 0= if
            msg-names first + i + 0 room-line  $C0 y  i at = cost-open @ and  text
            i worn = if  s" *"  $B0 y  false text  then
            y $20 + to y
        then
    loop
    cost-open @ if  3 0 do  msg-notes first + at + i room-line  $30  $150 i 18 * +  false text  loop  then ;
: costumes-view ( -- )
    $100 $E0 $1E0 $190 shade
    wardrobe-rows
    cost-row @ case
        row-her of  0 #hers ['] her-locked? cost-her-at @ cost-her-worn @ wardrobe-list  endof
        row-his of  #hers #his ['] his-locked? cost-his-at @ cost-his-worn @ wardrobe-list  endof
    endcase ;
