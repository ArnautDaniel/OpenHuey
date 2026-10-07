\ doors.fs - going from room to room. The game's door table says which exit of a room leads to
\ which exit of another; each exit has spots (exit-spot: from its door, or the room table):
\ "out" in front of the door, "in" in the doorway beyond it, "through" further on. Standing at
\ an exit's spot, Space goes through.
IN: doors
USING: engine vectors views state player ;

\ the exit she is standing at (one that leads somewhere), or -1
variable t
: spot-tri ( exit which -- tri )  exit-spot fdrop fdrop fdrop ;
: at-exit? ( exit -- flag )  dup 0 spot-tri t @ =  swap 2 spot-tri t @ =  or ;
: leads? ( exit -- flag )  exit-leads drop 0< 0= ;
: exit-under ( -- exit | -1 )
    her-pos nav-tri t !  t @ 0< if  -1 exit  then
    8 0 do  i at-exit? i leads? and if  i unloop exit  then  loop  -1 ;

\ arriving: at the exit's "out" spot, facing away from the doorway (the "in" spot)
fvariable ax  fvariable az
: face-away ( exit -- )
    dup 1 exit-spot fswap fdrop az f! ax f!  0< if  drop exit  then   \ the doorway
    0 exit-spot fswap fdrop  0< if  fdrop fdrop exit  then         ( F: ox oz )
    az f@ f- fnegate fswap ax f@ f- fnegate fswap fatan2 her act.yaw sf! ;
: arrive ( exit -- )
    dup 0 exit-spot  0< if  fdrop fdrop fdrop drop place-player exit  then
    place-fiona  face-away ;

: go-through ( exit -- )
    exit-leads over 0< if  2drop exit  then        ( room exit' )
    leaving-room  dup came-in-by !
    swap room .room  -1 view !
    arrive ;

: doors
    playing @ 0= if  exit  then
    exit-under dup 0< if  drop 0 0 hud exit  then
    dup exit-locked? if  drop s" locked" hud exit  then
    s" Space: go through" hud
    key: Space key-pressed? if  use-exit  0 0 hud  else  drop  then ;
' doors on-tick
' go-through is use-exit

\ where this room's exits lead (for the console)
: .exits
    8 0 do
        i exit-leads over 0< if  2drop  else
            ." exit " i .  ." -> room " swap hex . decimal  ." exit " . cr
        then
    loop ;
