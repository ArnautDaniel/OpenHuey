\ debug.fs - labels over the room for reporting problems: `labels` (from the console) turns
\ them on and off. Shown:
\   top left    the room, Fiona's nav triangle and its flags, the event areas she is in
\   yellow      the exits: "exit 1 d2>0xD/1 shut a1" - exit 1, door 2, to room 0xD's exit 1, the
\               door's state, its event area (at the "out" spot; "in" / "thru" mark the doorway
\               and the far spot)
\   green/cyan  the event areas (green: Fiona is inside), outlined, with their number
\   orange      the scripts' zones
\   magenta     the placed objects (grey while hidden)
\   white       the characters: slot, script id, move mode / move; Hewie his action and behaviour
IN: debug
USING: engine state game-state events.core partner.core ;

variable labels-on
: labels ( -- )  labels-on @ 0= labels-on ! ;
: fiona-actor ( -- id | -1 )  0 character char.actor sl@ ;

\ ---- writing: a line of pieces from (tx, ty) ----
variable tx  variable ty
: put ( addr len -- )  tuck tx @ ty @ draw-text  char-size drop * tx +! ;
: putn ( n -- )  n>s put ;
: puth ( n -- )  h>s put ;
: sp ( -- )  s"  " put ;
\ a label at a point of the room: the pen there (false: off screen)
: at-point ( -- flag ) ( F: x y z -- )
    to-screen 0= if  fdrop fdrop false exit  then
    f>s ty !  f>s tx !
    tx @ 0 screen-size drop within  ty @ 0 screen-size nip within and ;
: backing ( n -- )   \ a dark strip under n characters from the pen
    $000000A0 pen-color  tx @ 2 - ty @ 1- rot char-size drop * 4 + char-size nip 2 + draw-rect ;

\ ---- exits ----
: door-state ( exit -- )
    event-exit-door dup 0< if  drop exit  then
    dup event-door-flags 1 and if  drop s"  doorway" put exit  then
    dup door-locked if  drop s"  locked" put exit  then
    dup 2 door-bit? if  s"  open" put  else  s"  shut" put  then
    1 door-bit? if  s"  held" put  then ;
: exit-label ( exit -- )
    dup exit-leads nip 0< if  drop exit  then
    dup 0 exit-spot drop at-point if
        30 backing  $FFE040FF pen-color
        s" exit " put  dup putn
        s"  d" put  dup event-exit-door putn
        s" >" put  dup exit-leads swap puth s" /" put putn
        dup door-state
        s"  a" put  dup exit-area putn
    then
    dup 1 exit-spot drop at-point if  $FFE040FF pen-color  s" in " put  dup putn  then
    2 exit-spot drop at-point if  $C0A030FF pen-color  s" thru" put  then ;

\ ---- event areas: outlined with dots, their number at the middle ----
fvariable ax  fvariable ay  fvariable az  fvariable bx  fvariable by  fvariable bz
: dot ( F: x y z -- )  at-point if  tx @ 1- ty @ 1- 3 3 draw-rect  then ;
: edge ( area k -- )
    2dup area-corner 0= if  2drop fdrop fdrop fdrop exit  then  az f! ay f! ax f!
    1+ 3 and area-corner 0= if  fdrop fdrop fdrop exit  then  bz f! by f! bx f!
    9 0 do
        i s>f 8e f/  fdup bx f@ ax f@ f- f* ax f@ f+
        fover by f@ ay f@ f- f* ay f@ f+
        frot bz f@ az f@ f- f* az f@ f+  dot
    loop ;
: her-in-area? ( area -- flag )
    fiona-actor dup 0< if  2drop false exit  then  actor dup act.x sf@ dup act.y sf@ act.z sf@ area-in? ;
: area-label ( area -- )
    dup area-kind 1 <> if  drop exit  then
    dup her-in-area? if  $40FF40FF  else  $40C0FFFF  then  pen-color
    4 0 do  dup i edge  loop
    dup area-middle if  at-point if  8 backing  dup her-in-area? if  $40FF40FF  else  $40C0FFFF  then  pen-color
        s" area " put  dup putn  then  else  fdrop fdrop fdrop  then  drop ;

\ ---- zones (the scripts' cylinders) ----
: zone-label ( z -- )
    dup zone# dup l@ 0= if  2drop exit  then
    dup 8 + sf@  dup 12 + sf@  16 + sf@  at-point if  8 backing  $FF9020FF pen-color  s" zone " put  putn  else  drop  then ;

\ ---- placed objects ----
: placed-label ( i -- )
    placed-info  at-point if
        dup if  $FF40FFFF  else  $909090FF  then  pen-color
        >r  dup 8 + backing  put  r> 0= if  s"  (hidden)" put  then
    else  drop 2drop  then ;

\ ---- characters ----
: char-label ( cs -- )
    dup character char.present sl@ 0= if  drop exit  then
    dup character char.actor sl@ dup 0< if  2drop exit  then
    actor dup act.x sf@ dup act.y sf@ 18e f+ act.z sf@  at-point 0= if  drop exit  then
    30 backing  $FFFFFFFF pen-color
    s" slot " put  dup putn  s"  id " put  dup character char.id sl@ putn
    s"  mode " put  dup character char.mode sl@ puth
    s"  move " put  dup character char.move sl@ putn
    dup character char.scripted sl@ if  s"  scripted" put  then
    1 = if
        tx @ >r  ty @ char-size nip + ty !  r> 30 char-size drop * - 0 max tx !
        30 backing  $FFFFFFFF pen-color
        s" action " put  h-action @ puth  sp  h-state @ ?dup if  xt>name put  then
    then ;

\ ---- the corner ----
: corner ( -- )
    8 tx !  8 ty !  60 backing  $FFFFFFFF pen-color
    s" room " put  room-id puth
    fiona-actor dup 0< if  drop exit  then  actor dup act.x sf@ dup act.y sf@ act.z sf@ nav-tri
    s"   tri " put  dup putn  dup 0< 0= if  s"  flags " put  nav-flags puth  else  drop  then
    s"   in areas" put
    area-count 0 ?do  i her-in-area? if  sp i putn  then  loop ;

: draw-labels ( -- )
    labels-on @ 0= if  exit  then
    2 pen-scale
    area-count 0 ?do  i area-label  loop
    zones 0 do  i zone-label  loop
    placed-count 0 ?do  i placed-label  loop
    8 0 do  i exit-label  loop
    characters 0 do  i char-label  loop
    corner ;
' draw-labels on-draw
