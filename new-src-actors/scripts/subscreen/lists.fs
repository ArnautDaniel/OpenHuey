\ subscreen/lists.fs - the item lists and an item's actions (src/game/subscreen.c
\ SubScreen_StateItems, SubScreen_DrawItemGrid, SubScreen_StateItemActions, item_use), and the
\ files page (SubScreen_StateFiles).
\ A list: 16 items a page in two columns of 8 - each its icon, its name (light if she wears it)
\ and how many. On one the cross opens its actions: its picture slides in with its name and its
\ note, beside the actions its class allows (use, wear / take off, throw away) and "back".
IN: subscreen.lists
USING: engine actors messages keys facts items items.names items.tables subscreen.state subscreen.art subscreen.input subscreen.box ;

: items-id ( -- id )  s" items" actor-named ;
: list ( -- l )  page @ ;
: cursor ( -- addr )  list cells cursors + ;
: under-cursor ( -- id )  list cursor @ item-at ;
: in-list ( -- n )  list listed ;
: list-pages ( -- n )  in-list 1- 0 max 16 / 1+ ;
: list-page ( -- n )  cursor @ 16 / ;

\ ---- a list page ----
: arrows-at ( y -- )   \ the blinking page arrows (more than one page)
    >r  $168 r@ $1A blink part  $1B3 r> $1B blink part ;
variable slot-x  variable slot-y   \ (the place drawn)
: place-of ( i -- )  dup 8 / $DA * slot-x !  8 mod 35 * slot-y ! ;
: draw-item ( list k -- )   \ its icon, its name (light if she wears it), how many
    {: list k | id :}  list k item-at to id
    slot-x @ $20 +  slot-y @ $5E +  id item-kind  $80 part
    id word-plate-key = if  3  list k held-at letters 8  message-param$  then   \ (a plate: its word)
    id name  slot-x @ $46 +  slot-y @ $64 +  list k equip-state 1 =  text
    id item-held 1 = if  list k held-at n@ count$  slot-x @ $DE +  slot-y @ $64 +  false text  then ;
: grid ( -- )
    list-page list-pages 1- > if  in-list 1- 0 max cursor !  then
    list-page 1+ list-pages of$  $186 $176 false text
    list-pages 1 > if  $170 arrows-at  then
    16 0 do
        i place-of
        list  list-page 16 * i +                              ( list k )
        2dup item-at 0< 0=  mode @ on-actions = i 8 < and 0= and if   \ (the picture's frame covers the left column)
            draw-item
        else  2drop  then
        cursor @ 16 mod i = if  slot-x @ $20 +  slot-y @ $5E +  $1C $80 part  then
    loop ;
: help ( msg -- )  menu-line  $46 $186 false text ;
: list-view ( -- )   \ (the special list's panels, the others')
    list case  pg-special of  0  endof  pg-items of  8  endof  >r 9 r>  endcase  page-art
    grid  mode @ on-actions = 0= if  list pg-items = if  $5B  else  $5D  then  help  then ;

\ ---- an item's actions: use 1, wear 2, throw away 4; 0 back ----
create acts 4 cells allot   variable #acts
: gather ( -- )   \ (those its class allows, then back)
    0 #acts !
    under-cursor dup 0< if  drop  else
        item-actions  3 0 do  dup 1 i lshift and if  1 i lshift  #acts @ cells acts + !  1 #acts +!  then  loop  drop
    then
    0 #acts @ cells acts + !  1 #acts +! ;
: act@ ( i -- act )  cells acts + @ ;
: act-word ( act -- msg )   \ (use $50, wear $58 / take off $59, throw away $51, back $5A)
    case  1 of  $50  endof  2 of  list cursor @ equip-state 1 = if  $59  else  $58  then  endof
          4 of  $51  endof  >r $5A r>  endcase ;
: load-picture ( -- )   \ its picture into the cache's picture group
    under-cursor dup shown-picture @ = if  drop exit  then
    dup shown-picture !  0 slide !  item-picture dup if  picture-group load-bank  else  2drop  then ;
: note ( -- msg )   \ (its note; one whose note changes, not used yet: "?")
    under-cursor  dup $100 + $4000 or
    over item-actions $80000000 and 0<>  rot item-bit? 0= and if  drop $413D  then ;
: actions-view ( -- )
    $10 $30 $A $80 part                                        \ the picture's frame
    slide @ $40 < if  8 slide +!  then  $50 $70 $B slide @ part
    under-cursor name  $38 $49 false text
    under-cursor item-held 1 = if  list cursor @ held-at n@ count$  $D2 $49 false text  then
    $100 $16E $19C $66 shade
    note dup message-layout 0> if  0 message-lines 0 ?do  dup 0 i line-of  $32  $142 i 18 * +  false text  loop  then  drop
    $1A9  #acts @ 10 * $DD +  $46  #acts @ 20 *  shade
    #acts @ 0 ?do  i act@ act-word menu-line  $190  $D8 i $14 * +  i action @ =  text  loop
    s" >"  $17C  action @ 20 * $D8 +  true text ;

\ ---- using (item_use) ----
: back ( answer -- )  drop  back-to-page ;
: usable-now? ( -- flag )   \ (a key thing can't be used in a chase, nor with the stalker in her room)
    the-danger @ 2 <>  stalker-room @ room-id <> and ;
: mark-used ( id -- )  items-id send item-bit ;
\ what came of it (the items actor's answer): 0 nothing (8 a hint: no buzzer); 4 an event - the
\ menu closes onto it quietly; used up (1 a remedy, 2 a mark) - said so; else back to the list
: used ( id result -- )
    dup 0= over 8 = or if  8 <> if  se-buzzer  then  drop  $4052 ['] back ask exit  then
    dup 4 and if  8 and 0= if  se-decide  then  mark-used  -1 quiet !  close-menu exit  then
    dup 2 and  swap 1 and  2 pick item-kind 2 = and  or if
        dup mark-used  1 swap message-param!  se-used  $4055 ['] back ask exit
    then
    drop  back-to-page ;
: read-doc ( answer -- )  drop  list cursor @ items-id send use-item  back-to-page ;
: use-it ( -- )
    under-cursor item-kind 2 <  usable-now? 0= and if  se-buzzer  $4054 ['] back ask exit  then
    under-cursor item-kind 7 = if  under-cursor $4000 or ['] read-doc ask exit  then
    awaiting-use mode !  list cursor @ items-id send use-item ;
: thrown ( answer -- )
    if  back-to-page exit  then
    1 under-cursor message-param!  list cursor @ items-id send discard-item  $4057 ['] back ask ;
: worn-instead ( answer -- )  0= if  list cursor @ items-id send equip-item  then  back-to-page ;
: wear ( -- )
    list cursor @ equip-state case
        2 of  1  list cursor @ slot-of equipped  message-param!  $405C ['] worn-instead ask  endof   \ another worn: ask
        1 of  list cursor @ items-id send unequip-item  back-to-page  endof
        0 of  list cursor @ items-id send equip-item  back-to-page  endof
    endcase ;
: take-action ( -- )
    se-decide
    action @ act@ case
        1 of  use-it  endof
        2 of  wear  endof
        4 of  1 under-cursor message-param!  $4056 ['] thrown ask  endof
        >r back-to-page r>
    endcase ;

\ ---- the keys ----
: open-item ( -- )
    under-cursor 0< if  exit  then  se-decide
    under-cursor item-kind 7 = if  under-cursor $4000 or ['] read-doc ask exit  then   \ (a document: read)
    on-actions mode !  0 action !  gather  load-picture ;
: list-keys ( -- )   \ up / down along the column, left / right across the columns and pages
    confirm? if  open-item exit  then
    cursor @ >r
    up? if  r@ 8 mod if  r@ 1-  else  r@ 7 +  then  cursor !  then
    down? if  r@ 8 mod 7 = if  r@ 7 -  else  r@ 1+  then  cursor !  then
    left? if  r@ 8 < if  r@ list-pages 16 * + 8 -  else  r@ 8 -  then  cursor !  then
    right? if  r@ 8 + 16 / list-pages 1- > if  r@ 8 mod  else  r@ 8 +  then  cursor !  then
    r> cursor @ <> if  se-cursor  then
    list pg-items = square button-pressed? and if  list items-id send sort-items  se-decide  then
    page-keys ;
: step-item ( d -- )   \ (among the actions: the list's next or last item)
    cursor @ + in-list wrap cursor !  0 action !  gather  load-picture  se-cursor ;
: action-keys ( -- )
    confirm? if  take-action exit  then
    up? if  action @ 1- #acts @ wrap action !  se-cursor  then
    down? if  action @ 1+ #acts @ wrap action !  se-cursor  then
    next-page? if  1 step-item  then
    prev-page? if  -1 step-item  then
    back? if  #acts @ 1- action !  back-to-page  se-cancel  then ;

\ ---- the files: two to a page, each its lines; left / right turn them ----
: file-pages ( -- n )  files 1- 0 max 2/ 1+ ;
: files-view ( -- )
    2 page-art
    file-page @ 1+ file-pages of$  $186 $186 false text
    file-pages 1 > if  $180 arrows-at  then
    2 0 do
        file-page @ 2* i +  dup files < if
            file@ dup message-layout 0> if
                0 message-lines 0 ?do  dup 0 i line-of  $2D  j $A0 * $54 + i 18 * +  false text  loop
            then
        then  drop
    loop ;
: files-keys ( -- )
    file-page @
    left? if  file-page @ 1- file-pages wrap file-page !  then
    right? if  file-page @ 1+ file-pages wrap file-page !  then
    file-page @ <> if  se-cursor  then
    page-keys ;
