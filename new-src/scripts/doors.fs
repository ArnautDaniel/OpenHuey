\ doors.fs - going from room to room. The game's door table says which exit of a room leads to
\ which exit of another; each exit has nav triangles: "out" and "through" (at the doorway) and
\ "in" (where someone arriving stands). Standing at an exit, Space goes through.
IN: doors
USING: engine vectors views state player ;

\ the exit she is standing at (one that leads somewhere), or -1
variable t
: at-exit? ( exit -- flag )  dup 0 exit-tri t @ =  swap 2 exit-tri t @ =  or ;
: leads? ( exit -- flag )  exit-leads drop 0< 0= ;
: exit-under ( -- exit | -1 )
    her-pos nav-tri t !  t @ 0< if  -1 exit  then
    8 0 do  i at-exit? i leads? and if  i unloop exit  then  loop  -1 ;

\ arriving: at the exit's "in" triangle, facing away from the doorway
fvariable ax  fvariable az
: face-away ( exit -- )
    dup 2 exit-tri dup 0< if  2drop exit  then
    tri-center fswap fdrop az f! ax f!              \ the doorway
    1 exit-tri dup 0< if  drop exit  then
    tri-center fswap fdrop                         ( F: ix iz )
    az f@ f- fswap ax f@ f- fswap fatan2 her act.yaw sf! ;
: arrive ( exit -- )
    dup 1 exit-tri dup 0< if  drop dup 0 exit-tri  then
    dup 0< if  2drop place-player exit  then
    tri-center place-fiona  face-away ;

: go-through ( exit -- )
    exit-leads over 0< if  2drop exit  then        ( room exit' )
    swap room .room  -1 view !
    arrive ;

: doors
    playing @ 0= if  exit  then
    exit-under dup 0< if  drop 0 0 hud exit  then
    s" Space: go through" hud
    key: Space key-pressed? if  go-through  0 0 hud  else  drop  then ;
' doors on-tick

\ where this room's exits lead (for the console)
: .exits
    8 0 do
        i exit-leads over 0< if  2drop  else
            ." exit " i .  ." -> room " swap hex . decimal  ." exit " . cr
        then
    loop ;
