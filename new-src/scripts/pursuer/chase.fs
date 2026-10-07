\ pursuer/chase.fs - the stalker heading for Fiona (src/game/pursuer.c, debilitas.c): where he
\ heads (+0x1594 the room, +0x1598 the side, +0x15A4 / +0x15B0 the place), going after her
\ (Pursuer_GoAfterFiona), finding her gone from where he got to (Pursuer_ChaseFionaHere) or
\ back after her (Pursuer_GoForFionaStance0).
IN: pursuer.chase
USING: engine game-state events.core events.words chars relations fiona.doors partner.route pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps pursuer.behave pursuer.search pursuer.target ;

\ Npc_SameRoomOtherSide: `cs` in his room, on the other side of it (the doors they came in by)
: same-room-other-side? ( cs -- flag )
    dup c-room p-room <>  p-door 8 u< 0= or if  drop false exit  then
    her = 0= if  not-yet" Npc_SameRoomOtherSide for Hewie (his door)"  false exit  then
    her-door $FF and 8 u< 0= if  false exit  then
    side-behind dup -1 = if  drop false exit  then
    her-side-behind dup -1 = if  2drop false exit  then  <> ;
\ the side behind for any character (Hewie's door: not kept yet)
: side-behind-of ( cs -- side )  her = if  her-side-behind  else  -1  then ;

\ Debilitas_HeadFor (vtable +0xB4): his route to `cs`'s room (its side when it's on the other
\ side of his; in her room, its nearest walkable place) - held off (mode 4): Fiona here
defer chase-fiona-here ( -- )   ' noop is chase-fiona-here
: head-for ( cs -- )
    p-scripted 0= mode 4 = and if  drop chase-fiona-here exit  then
    dup -1 = if  drop p-target  then
    $1598 pu-l@ >r
    dup same-room-other-side? if  r> drop dup side-behind-of >r
    else
        dup c-room p-room =  $1594 pu-l@ 2 pick c-room <> or if  r> drop -1 >r  then
        dup c-room played = if  dup c-tri over c-pos $15B0 pu nearest-walkable $15A4 pu-l!  then
    then
    dup c-room char-route 0< 0= if  c-room $1594 pu-l!  r> $1598 pu-l!
    else  drop r> drop  $1594 pu-l@ char-route drop  then ;
' head-for $B4 vt!
\ Debilitas_HeadForFiona (vtable +0xB0) - held off: back after her
defer go-for-fiona-stance0 ( -- )   ' noop is go-for-fiona-stance0
: head-for-fiona ( -- )
    p-scripted 0= mode 4 = and if  go-for-fiona-stance0 exit  then
    her head-for ;
' head-for-fiona $B0 vt!
\ Debilitas_GoTo (vtable +0xAC): to (tri, pos) in `room` (-1: this one)
: go-to ( tri pos room -- )
    dup -1 = if  drop played  then  >r
    r@ played = if  $15B0 pu nearest-walkable $15A4 pu-l!  else  2drop  then
    $15A4 pu-l@ $15B0 pu on-tri? 0= if  $15A4 pu-l@ tri-center $15B0 pu vec!  then
    r@ char-route 0< 0= if  r> $1594 pu-l!  -1 $1598 pu-l!
    else  r> drop  $1594 pu-l@ char-route drop  then ;
' go-to $AC vt!

\ ---- Npc_RandomExit: an exit at random (on his side of the room; leading on; he may use it -
\ vtable +0xEC); none: `skip` ----
create re-list 8 cells allot
: random-exit ( skip -- exit )
    0  8 0 do
        i 3 pick <> if
            p-door 8 u< if  p-room i exit-side-flag side-behind <> if  0  else  1  then  else  1  then
            if  p-room i room-exit-leads drop 0< 0=  i $EC vcall 0<> and if  i over cells re-list + !  1+  then  then
        then
    loop                                                         ( skip n )
    dup 0= if  drop exit  then  nip
    dup 1 = if  drop re-list @ exit  then
    s>f rnd01 f* f>s cells re-list + @ ;

\ ---- Pursuer_PlanWhere: his way on by his mode: after her - her room and side; heading or
\ searching - where he heads; waiting - a room through an exit at random (not the one he came
\ in by, if he can help it); held off - his destination (+0xE8). True: a way with doors ----
: plan-where ( -- flag )
    mode case
        0 of  her c-room $1594 pu-l!  her-side-behind $1598 pu-l!  $1594 pu-l@ char-route 0>  endof
        1 of  $1594 pu-l@ char-route 0>  endof
        2 of  $1594 pu-l@ char-route 0>  endof
        3 of
            $FF random-exit dup $FF = if  drop false
            else
                dup p-door = if  drop $FF random-exit  dup $FF = if  drop p-door  then  then
                p-room swap room-exit-leads drop $1594 pu-l!  -1 $1598 pu-l!
                $1594 pu-l@ char-route 0> 0= if
                    p-room p-door room-exit-leads drop $1594 pu-l!  $1594 pu-l@ char-route 0>
                else  true  then
            then
        endof
        4 of  $E8 vcall 0<>  endof
        >r false r>
    endcase ;

\ ---- Pursuer_ChaseFionaHere: where he got to - in her room the search there (his first
\ behaviour); elsewhere waiting about, his route to her ----
: chase-fiona-here-now ( -- )
    p-cond 2 = if  0 p-cond!  0 $1664 pu-l!  then
    in-played-room? if
        step-next? p-scripted 0= and if
            $175C pu-l@ $C = if  [: $25C vcall ;] behaviour!  $C $118 vcall
            else  [: $25C vcall ;] behaviour!  then
        then
        2 mode!  $2C0 vcall  p-room $1594 pu-l!  search-room
    else
        $1384 pu-l@ $1388 pu-l!  move-wait set-move
        0 $1530 pu-l!  0 $1538 pu-l!  0 $1534 pu-l!
        [: $290 vcall ;] behaviour!  1 mode!  $B0 vcall  1 step-next!
        not-yet" Pursuer_ChaseFionaHere off screen: the move's id, mode and sub (+0x17AC)"
    then
    0 $16C9 pu-c!  0 $16CB pu-c!  0 $16CA pu-c!  0 $16CC pu-c!  -1 $179C pu-l!
    0 $16F1 pu-c!  0 $16F3 pu-c!  0 $16F2 pu-c!  0 $16F4 pu-c! ;
' chase-fiona-here-now is chase-fiona-here

\ ---- Pursuer_GoForFionaStance0: back after her (mode 0): in her room his chase; elsewhere
\ his way on; his memory of doors and his route forgotten ----
: go-for-fiona-stance0-now ( -- )
    0 mode!  $2BC vcall
    p-cond 2 = if  0 p-cond!  0 $1664 pu-l!  then
    in-played-room? if
        step-next? p-scripted 0= and if
            $B0 vcall
            $175C pu-l@ $C <> if
                $1544 pu-c@ if  [: $264 vcall ;]  else  [: $278 vcall ;]  then  behaviour!
            else  [: $278 vcall ;] behaviour!  $C $118 vcall  then
        then
    else
        $17AC pu-l@ if  [: $290 vcall ;] behaviour!  move-plan set-move  then
        1 step-next!  0 $17B4 pu-l!
    then
    p-avoid 13 cells 0 fill  route-clear
    6 $16C9 pu-c!  7 $16CA pu-c!  1 $16F6 pu-c! ;
' go-for-fiona-stance0-now is go-for-fiona-stance0

\ ---- Pursuer_GoAfterFiona (vtable +0x280): after her - action 0x1C (when what drew him was
\ only worth 4) or 0x27; there already with no way on: the search here; else the door
\ behaviour (+0x284) ----
: go-after-fiona ( -- )
    $1758 pu-l@ dup -1 <> if  dup -2 <> if  $114 vcall  else  drop  then
    else  drop  $16CA pu-c@ 4 = if  $1C  else  $27  then  $114 vcall  then
    her p-target!  fresh
    reached-room? plan-where 0= and if  1 step-next!  chase-fiona-here exit  then
    0 $162C pu-l!  [: $284 vcall ;] behaviour!  $284 vcall ;
' go-after-fiona $280 vt!
