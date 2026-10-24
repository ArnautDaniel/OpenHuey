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
    {: y :}  $168 y $1A blink part  $1B3 y $1B blink part ;
: place-of ( i -- x y )  dup 8 / $DA *  swap 8 mod 35 * ;   \ (two columns of 8)
: draw-item ( list k x y -- )   \ its icon, its name (light if she wears it), how many
    {: list k x y | id :}  list k item-at to id
    x $20 +  y $5E +  id item-kind  $80 part
    id word-plate-key = if  3  list k held-at letters 8  message-param$  then   \ (a plate: its word)
    list k equip-state worn = ink {: c :}   \ (its count in its colour too)
    id name  x $46 +  y $64 +  c inked
    id stacks? if  list k held-at n@ count$  x $DE +  y $64 +  c inked  then ;
: grid ( -- )
    list-page list-pages 1- > if  in-list 1- 0 max cursor !  then
    list-page 1+ list-pages of$  $186 $176 false text
    list-pages 1 > if  $170 arrows-at  then
    {: | x y k :}
    16 0 do
        i place-of to y to x  list-page 16 * i + to k
        list k item-at 0< 0=  mode @ on-actions = i 8 < and 0= and if   \ (the picture's frame covers the left column)
            list k x y draw-item
        then
        cursor @ 16 mod i = if  x $20 +  y $5E +  $1C $80 part  then
    loop ;
: help ( msg -- )  menu-line  $46 $186 false text ;
: list-view ( -- )   \ (the special list's panels, the others')
    list case  pg-special of  0  endof  pg-items of  8  endof  >r 9 r>  endcase  page-art
    grid  mode @ on-actions = 0= if  list pg-items = if  $5B  else  $5D  then  help  then ;

\ ---- an item's actions: use, wear, throw away (its class's bits, in that order); 0 back ----
0 constant go-back
create acts 4 cells allot   variable #acts
: act+ ( act -- )  #acts @ cells acts + !  1 #acts +! ;
: gather ( -- )   \ (those its class allows, then back)
    {: | bits :}  0 #acts !
    under-cursor dup 0< if  drop  else
        item-actions to bits
        can-discard can-equip can-use  3 0 do  dup bits and if  act+  else  drop  then  loop   \ (use first)
    then
    go-back act+ ;
: act@ ( i -- act )  cells acts + @ ;
: act-word ( act -- msg )   \ (use $50, wear $58 / take off $59, throw away $51, back $5A)
    case  can-use of  $50  endof  can-equip of  list cursor @ equip-state worn = $59 $58 ?  endof
          can-discard of  $51  endof  >r $5A r>  endcase ;
: load-picture ( -- )   \ its picture into the cache's picture group
    under-cursor dup shown-picture @ = if  drop exit  then
    dup shown-picture !  0 slide !  item-picture dup if  picture-group load-bank  else  2drop  then ;
: note ( -- msg )   \ (its note; one whose note changes, not used yet: "?")
    under-cursor {: id :}
    id item-actions note-changes and 0<>  id item-bit? 0= and if  $413D  else  id $100 + $4000 or  then ;
: actions-view ( -- )
    $10 $30 $A $80 part                                        \ the picture's frame
    slide @ $40 < if  8 slide +!  then  $50 $70 $B slide @ part
    under-cursor name  $38 $49 false text
    under-cursor stacks? if  list cursor @ held-at n@ count$  $D2 $49 false text  then
    $100 $16E $19C $66 shade
    note dup message-layout 0> if  0 message-lines 0 ?do  dup 0 i line-of  $32  $142 i 22 * +  box-ink inked  loop  then  drop
    $1A9  #acts @ 10 * $DD +  $46  #acts @ 20 *  shade
    #acts @ 0 ?do  i act@ act-word menu-line  $190  $D8 i $14 * +  box-ink inked  loop
    hand 1  $17C  action @ 20 * $D8 +  cursor-ink inked ;   \ (the hand before the action picked)

\ ---- using (item_use) ----
: back ( answer -- )  drop  back-to-page ;
: usable-now? ( -- flag )   \ (a key thing can't be used in a chase, nor with the stalker in her room)
    the-danger @ chased <>  stalker-room @ room-id <> and ;
: mark-used ( id -- )  items-id send item-bit ;
\ what came of it (the items actor's answer): nothing (a hint: no buzzer); an event - the menu
\ closes onto it quietly; used up (a remedy, a mark) - said so; else back to the list
: used ( id result -- )
    {: id r :}
    r no-use =  r hinted = or if  r hinted <> if  se-buzzer  then  $4052 ['] back ask exit  then
    r event-started and if  r hinted and 0= if  se-decide  then  id mark-used  -1 quiet !  close-menu exit  then
    r marked and  r used-up and  id item-kind remedy = and  or if
        id mark-used  1 id message-param!  se-used  $4055 ['] back ask exit
    then
    back-to-page ;
: read-doc ( answer -- )  drop  list cursor @ items-id send use-item  back-to-page ;
: use-it ( -- )
    under-cursor item-kind remedy <  usable-now? 0= and if  se-buzzer  $4054 ['] back ask exit  then   \ (a key thing, the plate)
    under-cursor item-kind document = if  under-cursor $4000 or ['] read-doc ask exit  then
    awaiting-use mode !  list cursor @ items-id send use-item ;
: thrown ( answer -- )
    if  back-to-page exit  then
    1 under-cursor message-param!  list cursor @ items-id send discard-item  $4057 ['] back ask ;
: worn-instead ( answer -- )  0= if  list cursor @ items-id send equip-item  then  back-to-page ;
: wear ( -- )
    list cursor @ equip-state case
        other-worn of  1  list cursor @ slot-of equipped  message-param!  $405C ['] worn-instead ask  endof   \ ask
        worn of  list cursor @ items-id send unequip-item  back-to-page  endof
        slot-free of  list cursor @ items-id send equip-item  back-to-page  endof
    endcase ;
: take-action ( -- )
    se-decide
    action @ act@ case
        can-use of  use-it  endof
        can-equip of  wear  endof
        can-discard of  1 under-cursor message-param!  $4056 ['] thrown ask  endof
        >r back-to-page r>
    endcase ;

\ ---- the keys ----
: open-item ( -- )
    under-cursor 0< if  exit  then  se-decide
    under-cursor item-kind document = if  under-cursor $4000 or ['] read-doc ask exit  then   \ (a document: read)
    on-actions mode !  0 action !  gather  load-picture ;
: list-keys ( -- )   \ up / down along the column, left / right across the columns and pages
    confirm? if  open-item exit  then
    cursor @ {: at :}   \ (round the column; across the columns and pages, round)
    up? if  at 8 mod if  at 1-  else  at 7 +  then  cursor !  then
    down? if  at 8 mod 7 = if  at 7 -  else  at 1+  then  cursor !  then
    left? if  at 8 < if  at list-pages 16 * + 8 -  else  at 8 -  then  cursor !  then
    right? if  at 8 + 16 / list-pages 1- > if  at 8 mod  else  at 8 +  then  cursor !  then
    at cursor @ <> if  se-cursor  then
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
            file@ $4000 or  dup message-layout 0> if   \ (its text in MSG_SUB, as the items' notes)
                0 message-lines 0 ?do  dup 0 i line-of  $2D  j $A0 * $54 + i 22 * +  false text  loop
            then
        then  drop
    loop ;
: files-keys ( -- )
    file-page @
    left? if  file-page @ 1- file-pages wrap file-page !  then
    right? if  file-page @ 1+ file-pages wrap file-page !  then
    file-page @ <> if  se-cursor  then
    page-keys ;
