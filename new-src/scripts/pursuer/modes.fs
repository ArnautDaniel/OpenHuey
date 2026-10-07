\ pursuer/modes.fs - the stalker's mode (+0x16C8: 0 after Fiona, 1 heading for her room, 2
\ searching it, 3 waiting about, 4 held off a while) and how long he keeps to it (+0x1660),
\ his search route through a room (+0x15E0: 8 stops of a triangle and a flag, +0x1620 the next,
\ +0x1621 the end) and where he heads (+0x1594 the room, +0x1598 the side): src/game/pursuer.c
\ Pursuer_ModesSearching and its helpers.
IN: pursuer.modes
USING: engine game-state events.core events.words chars relations fiona.doors partner.route pursuer.core pursuer.stubs pursuer.npc ;

: mode ( -- m )  $16C8 pu-c@ ;   : mode! ( m -- )  $16C8 pu-c! ;
: wait-t ( -- n )  $1660 pu-l@ ;   : wait-t! ( n -- )  $1660 pu-l! ;
\ the mode timers (vtable +0x2BC .. +0x2C8): after her (+0x16D4), searching 10 s (Debilitas),
\ waiting 15 s (1 in the countdown), held off for `t` (900 for 0)
:noname  $16D4 pu-l@ wait-t! ; $2BC vt!
:noname  600 wait-t! ; $2C0 vt!
:noname  countdown? if  1  else  900  then  wait-t! ; $2C4 vt!
:noname ( t -- )  ?dup 0= if  900  then  wait-t! ; $2C8 vt!

\ ---- which side of a door a room's exit is (Rooms_ExitSideFlag, checked: the def's flags
\ 0x40 / 0x80 say the side is watched; then 0x10 / 0x8) ----
: exit-side-flag ( room exit -- side | -1 )
    over swap room-exit-door dup 0< if  2drop -1 exit  then          ( room d )
    dup sides! event-door-flags >r
    dup r0 @ = if  r@ $40 and if  drop r> $10 and 0<> 1 and exit  then  then
    r1 @ = if  r@ $80 and if  r> 8 and 0<> 1 and exit  then  then
    r> drop -1 ;
\ the exit a character came in by (c.door, +0x14D4): his own; Fiona's the event's
: p-door ( -- exit )  $14D4 pu-c@ ;   : p-door! ( exit -- )  $14D4 pu-c! ;
defer her-door ( -- exit )   :noname  event-state ev.exit sl@ ; is her-door
\ Npc_CharSideBehind (Fiona) / Npc_ExitSideBehind (him)
: her-side-behind ( -- side )  her c-room her-door $FF and exit-side-flag ;
: side-behind ( -- side )  p-room p-door exit-side-flag ;
\ Npc_ReachedRoom: in the room he heads for (+0x1594), on its side (+0x1598) if one is asked
: reached-room? ( -- flag )
    $1594 pu-l@ p-room <> if  false exit  then
    $1598 pu-l@ -1 = if  true exit  then
    side-behind dup -1 = if  drop true  else  $1598 pu-l@ =  then ;
\ Npc_NearRoom: character `cs` in his room or one next to it
: near-room? ( cs -- flag )
    c-room dup p-room = if  drop true exit  then
    8 0 do  dup p-room i room-exit-leads drop = if  drop true unloop exit  then  loop  drop false ;

\ ---- his search route (Npc_RouteAdd / Npc_RouteDrop / Npc_RouteAim) ----
: stop# ( k -- addr )  8 * $15E0 + pu ;
: route-first ( -- k )  $1620 pu-c@ ;   : route-end ( -- k )  $1621 pu-c@ ;
: route-clear ( -- )   \ Pursuer_ClearRoute
    8 0 do  -1 i stop# l!  0 i stop# 4 + c!  loop  $FF $1620 pu-c!  $FF $1621 pu-c!  0 $1794 pu-l! ;
: route-add ( tri -- flag )
    route-end $FF = if  0 $1621 pu-c!  0 $1620 pu-c!  then
    route-end 8 <  over 0< 0= and  over nav-tris < and if  route-end stop# l!  route-end 1+ $1621 pu-c!  true
    else  drop false  then ;
: route-drop ( -- tri | -1 )
    route-end route-first <  route-end $FF = or if  -1 exit  then
    route-end 0 ?do  i 1+ stop# l@ i stop# l!  i 1+ stop# 4 + c@ i stop# 4 + c!  loop
    route-end 1- $1621 pu-c!  route-first 0> if  route-first 1- $1620 pu-c!  then
    route-end route-first >=  route-end $FF <> and if  route-first stop# sl@  else  -1  then ;
: route-aim ( -- )   \ his goal (+0x15A4 / +0x15B0): the next stop
    route-first route-end < if  route-first stop# sl@ dup $15A4 pu-l!  tri-center $15B0 pu vec!  then ;
\ Npc_RandomTri: a triangle of the room he may stand on (not both 0x100000 and 0x200000)
: random-tri ( -- tri )
    p-room played <> if  -1 exit  then
    begin
        nav-tris s>f rnd01 f* f>s
        dup tri-flags p-mask and 0= over tri-flags $300000 and $300000 <> and if  exit  then  drop
    again ;
\ the room's search spots (gEvents +0x64 +0x38: up to 8, -1 ended; with the rooms' events)
defer search-spots ( -- addr n )   :noname  0 0 ; is search-spots
variable as-spots  variable as-n  variable as-used
\ Pursuer_AddSearchStops: n stops, each by 60% the room's next search spot (+0x1738 the next;
\ flag 0) while it has more, else a random triangle (flag 1)
: add-search-stops ( n -- )
    dup 0> 0= if  drop exit  then
    search-spots as-n !  as-spots !  0 as-used !
    0 do
        $1738 pu-l@ as-n @ >= if  0 $1738 pu-l!  then
        as-used @ as-n @ <  100e rnd01 f* 60e f<= and if
            $1738 pu-l@ dup 1+ $1738 pu-l!  cells as-spots @ + @ route-add drop  1 as-used +!
            0 route-end stop# 4 + c!
        else
            random-tri route-add drop  1 route-end stop# 4 + c!
        then
    loop ;
\ the stops to have left: heading for a room 2..5, searching 1..2
: stops-wanted ( -- n )
    mode case
        2 of  4e rnd01 f* f>s 2 +  endof
        3 of  2e rnd01 f* f>s 1+  endof
        >r $FF r>
    endcase ;
\ Pursuer_SearchRoom: in her room the route's done stops dropped and the rest topped up (none:
\ the route cleared unless he's reached the room he heads for), 30 s to grow back; elsewhere
\ in the room he heads for 5 s a stop to wait
: search-room ( -- )
    stops-wanted >r
    in-played-room? if
        begin  route-first 0<>  route-end $FF <> and  while  route-drop drop  repeat
        $16CA pu-c@ dup 2 <> swap 7 <> and  reached-room? 0= and if  r> drop route-clear exit  then
        route-end route-first - r@ < if
            r@ route-end route-first - - add-search-stops
            r@ 0>  r@ $FF <> and if  1800  else  0  then  $1794 pu-l!
        else  1800 $1794 pu-l!  then
        r> drop
    else reached-room? if  r> 150 * $17B4 pu-l!
    else  r> drop  0 $17B4 pu-l!  route-clear  then then ;

\ ---- seeing her again (Pursuer_ResightTest): in the room he heads for, and she is behind the
\ same side as he came in by (or either side unknown) ----
: resight? ( -- flag )
    her-side-behind side-behind  reached-room? 0= if  2drop false exit  then
    2dup = >r  2 u< 0= swap 2 u< 0= or r> or ;   \ (unsigned: -1 counts as unknown)
\ the countdown's seconds left (progress +0x764 +4: frames) - with the countdown
defer countdown-seconds ( -- n )   ' false is countdown-seconds

variable sw-n  variable sw-have
\ ---- Pursuer_ModesSearching (vtable +0x120): the mode each frame ----
: mode-after ( -- )   \ (back after her: mode 0, sees her, her timer)
    0 mode!  $2BC vcall  6 $16C9 pu-c! ;
: modes-searching ( -- )
    countdown? if  $124 vcall exit  then
    p-cond 2 = if                                                    \ (knocked down: waiting)
        mode 3 <> if
            3 mode!  $2C4 vcall
            0 $16C9 pu-c!  0 $16CB pu-c!  0 $16CA pu-c!  0 $16CC pu-c!  -1 $179C pu-l!
            0 $16F1 pu-c!  0 $16F3 pu-c!  0 $16F2 pu-c!  0 $16F4 pu-c!
        then  exit
    then
    mode case
        0 of
            $1544 pu-c@ if  6 $16C9 pu-c!  7 $16CA pu-c!  $2BC vcall  0 $16F4 pu-c!  exit  then
            5 $16C9 pu-c!
            p-mode dup 2 = swap 3 = or if  exit  then
            wait-t if
                $16F4 pu-c@ if  3 mode!  $2C4 vcall  2 $16C9 pu-c!  3 $16CA pu-c!  0 $16F4 pu-c!
                else  $1660 pu-1-  then
            else in-played-room? 0= if  1 mode!  2 $16C9 pu-c!  3 $16CA pu-c!
            else
                2 mode!  $2C0 vcall
                stops-wanted sw-n !  route-end route-first - sw-have !
                sw-have @ sw-n @ < if  sw-n @ sw-have @ - add-search-stops
                else sw-n @ sw-have @ < if
                    begin  route-first 1+ $1620 pu-c!  sw-n @ route-end route-first - =  until
                then then
                sw-n @ 0>  sw-n @ $FF <> and if  1800  else  0  then  $1794 pu-l!
                3 $16C9 pu-c!  4 $16CA pu-c!
            then then
        endof
        1 of
            $1544 pu-c@ 1 = if
                resight? if  mode-after  then  0 $16F2 pu-c!
            else reached-room? if  2 mode!
            else $16F4 pu-c@ if  3 mode!  $2C4 vcall  0 $16F4 pu-c!
            else in-played-room? 0= countdown-seconds 0= and if  3 mode!  $2C4 vcall
            then then then then
            $16F2 pu-c@ if  search-room  0 $16F2 pu-c!  then
        endof
        4 of
            wait-t 0= if
                0 mode!  $2BC vcall
                route-clear  her c-room $1594 pu-l!
                6 $16C9 pu-c!  1 $16F6 pu-c!  0 $16F2 pu-c!  0 $16F4 pu-c!
                not-yet" ModesSearching 4: the room memory (+0x148C) and the route's step (+0x1384)"
            then
        endof
        \ 2 / 3: searching, waiting
        $1544 pu-c@ 1 = if
            resight? if  mode-after  else  1 mode!  then
            route-clear  0 $16F2 pu-c!  0 $16F4 pu-c!
        else mode 2 = if
            $16F2 pu-c@ if
                reached-room? 0= if  1 mode!  search-room
                else in-played-room? 0= route-first 0<> or if  search-room  then then
                0 $16F2 pu-c!  0 $16F4 pu-c!
            else $16F4 pu-c@  $17B4 pu-l@ 0= in-played-room? 0= and  or if
                3 mode!  $2C4 vcall  0 $16F4 pu-c!
            then then
        else $16F2 pu-c@ if
            reached-room? if  2  else  1  then  mode!  search-room  0 $16F2 pu-c!  0 $16F4 pu-c!
        else $16F4 pu-c@ if  0 $16F4 pu-c!
        else in-played-room? 0= if
            wait-t if  $1660 pu-1-
            else 0 near-room? 0= if
                $B0 vcall  reached-room? if  2  else  1  then  mode!  search-room
            then then
        then then then then then
    endcase ;
' modes-searching $120 vt!
