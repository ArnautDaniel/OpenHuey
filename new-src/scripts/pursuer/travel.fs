\ pursuer/travel.fs - the stalker from room to room (src/game/pursuer.c): the exits as he sees
\ them (Npc_ExitKind), his route through the doors (+0x138C, +0x1388 the step, +0x1384 their
\ number), going through one (Pursuer_ThroughDoor), his moves out of sight (+0x17A0: a way
\ counted down at his pace - Pursuer_TravelOffscreen - then the next door) and coming into
\ the room being played (Pursuer_EnterRoom). His behaviour at the doors (Pursuer_BehaviourDoors).
IN: pursuer.travel
USING: engine game-state events.core events.words chars relations fiona.doors partner.route pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps pursuer.behave pursuer.search pursuer.moves pursuer.target pursuer.chase pursuer.hit ;

defer start-search   ' noop is start-search   \ Pursuer_StartSearch (below)
\ ---- his route ----
: route-step ( -- n )  $1388 pu-l@ ;   : route-steps ( -- n )  $1384 pu-l@ ;
: route-door ( i -- d )  cells pursuer.target:p-route + @ ;
: next-door ( -- d )  route-step route-door ;
: door-exit-of ( d -- exit )  p-room partner.route:door-exit $FF and ;     \ Rooms_DoorExit (0xFF: not here)
: exit-target ( -- exit )  $17B0 pu-c@ ;   : exit-target! ( exit -- )  $17B0 pu-c! ;
: through-room ( exit -- room )  p-room swap room-exit-leads drop ;   \ Rooms_OtherRoom
: through-exit ( exit -- exit' )  p-room swap room-exit-leads nip ;   \ Rooms_OtherExit
: exit-door# ( exit -- d )  p-room swap room-exit-door ;              \ Rooms_ExitDoor
\ the doors that let him down (+0x148C, a bit a door)
: avoid-door ( d -- )  pursuer.target:p-avoid bit! ;
: unavoid-door ( d -- )
    dup 0< if  drop exit  then
    dup 5 rshift cells pursuer.target:p-avoid +  swap 31 and 1 swap lshift invert  over @ and swap ! ;
: avoid-clear ( -- )  pursuer.target:p-avoid 13 cells 0 fill ;

\ ---- a door as he finds it (the door state word: bit 0 held, 1 open, 2 barred, 3 locked; 4..7
\ the sides it's locked from) ----
: exit-word ( exit -- w )  exit-door# dup 0< if  drop 0 exit  then  door-word dup if  l@  then ;
: exit-held? ( exit -- flag )  exit-word 1 and 0<> ;          \ Progress_CurRoomFlag
: exit-doorway? ( exit -- flag )                              \ (the door def's flag 1: no door)
    exit-door# dup 0< if  drop true exit  then  event-door-flags 1 and 0<> ;
\ Npc_ExitKind: 2 held, 3 not for him (locked from his side, locked, barred), 4 a way through
\ (a doorway, open), 5 a shut door
: exit-kind ( exit -- k )
    dup exit-held? if  drop 2 exit  then
    dup exit-word 4 rshift 4 and if  drop 3 exit  then
    dup exit-word 8 and if  drop 3 exit  then
    dup exit-doorway? if  drop 4 exit  then
    dup exit-word 2 and if  drop 4 exit  then
    exit-word 4 and if  not-yet" Npc_ExitKind: a barred door (DoorHold_Usable / Rooms_ExitHasSideFlag)"  3 exit  then
    5 ;
\ NPC_CanUseExit (vtable +0xEC): 1 for 4 5 6, 2 for 2
:noname ( exit -- n )  exit-kind case  2 of 2 endof  4 of 1 endof  5 of 1 endof  6 of 1 endof  >r 0 r>  endcase ; $EC vt!
\ DoorHold_Take (his side: the sides 2..5 lock) / DoorHold_Release: true if it couldn't
: hold-take ( exit -- failed? )
    dup exit-doorway? if  drop true exit  then
    exit-door# door-word dup 0= if  drop true exit  then
    dup l@ 4 rshift 4 and if  drop true exit  then
    dup l@ 1 and if  drop true exit  then
    dup l@ 1 or swap l!  false ;
: hold-release ( exit -- )
    dup exit-doorway? if  drop exit  then
    exit-door# door-word dup 0= if  drop exit  then
    dup l@ 8 and over l@ 1 and 0= or if  drop exit  then
    dup l@ 4 invert and 1 invert and swap l! ;

\ ---- Pursuer_ExitClosed: that exit closed to him: his route anew without it ----
: exit-closed ( exit -- )
    dup $FF = if  drop exit-target  then
    avoid-clear  exit-door# avoid-door
    $1594 pu-l@ char-route -1 =  mode dup 0<> swap 2 <> and if  pmv-wait set-move drop exit  then
    if  pmv-idle  else  pmv-plan  then  set-move ;
\ Npc_ExitWhatToDo: 2 held; 6 (barred) taken and let go; 4 / 5 through (5: his door arg +0xF0)
: exit-what-to-do ( exit -- n )
    dup exit-kind case
        2 of  drop 2  endof
        6 of  dup hold-take drop  dup hold-release  dup exit-doorway? if  drop 1 exit  then  $F0 vcall 1  endof
        4 of  dup exit-doorway? if  drop 1 exit  then  $F0 vcall 1  endof
        5 of  $F0 vcall 1  endof
        >r drop 0 r>
    endcase ;
:noname ( exit -- )  drop ; $F0 vt!   \ NPC_ExitArg (the door he goes through: with the door animations)
\ Pursuer_MayUseExit: not held; to another room than hers - by what to do; into hers - a way
\ through, or a door he opens
: may-use-exit? ( exit -- flag )
    dup exit-held? if  drop false exit  then
    dup through-room played <> if
        dup exit-what-to-do case
            1 of  exit-door# unavoid-door  true  endof
            0 of  $17AC pu-l@ 4 <> if  exit-closed  else  drop  then  false  endof
            >r drop false r>
        endcase exit
    then
    dup exit-kind case
        4 of  exit-door# unavoid-door  true  endof
        3 of  $17AC pu-l@ 4 <> if  exit-closed  else  drop  then  false  endof
        2 of  drop false  endof
        >r  dup exit-door# unavoid-door  dup through-exit p-who!  dup hold-take drop  hold-release  true  r>
    endcase ;

\ ---- Pursuer_ThroughDoor: through the exit into the next room - his behaviour by his mode,
\ out of sight (+0x29 hidden, +0x2A); into her room: he'll come in; his way in is the exit ----
: through-door ( exit -- )
    countdown? if  drop not-yet" Pursuer_ThroughDoorEnding"  exit  then
    0 $1530 pu-l!  0 $1538 pu-l!  0 $1534 pu-l!
    dup through-room swap through-exit                         ( room exit' )
    route-clear  p-path-end
    0 $1761 pu-c!  0 $1760 pu-c!  0 $178C pu-l!  -1 $1764 pu-l!  0 p-freeze!
    0 $16F5 pu-c!  0 $16F8 pu-c!  0 $16F7 pu-c!  0 $1794 pu-l!  0 $1798 pu-l!  0 $16F3 pu-c!
    in-played-room? if
        p-cond 2 = if  [: $2A4 vcall ;] behaviour!  pmv-idle set-move  then
        $175C pu-l@ $20 = if  $1802 play
        else
            mode case
                0 of  [: $290 vcall ;] behaviour!  pmv-plan set-move  endof
                2 of  [: $290 vcall ;] behaviour!  pmv-plan set-move  endof
                4 of  $1388 pu-1+  [: $2B4 vcall ;] behaviour!  pmv-wait set-move  endof
                >r  $1388 pu-1+  [: $290 vcall ;] behaviour!
                    over $1594 pu-l@ =  route-step route-steps >= and if  pmv-idle  else  pmv-wait  then  set-move  r>
            endcase
            $16C9 pu-c@ 2 = if
                0 $16C9 pu-c!  0 $16CB pu-c!  0 $16CA pu-c!  0 $16CC pu-c!  -1 $179C pu-l!
                0 $16F1 pu-c!  0 $16F3 pu-c!  0 $16F2 pu-c!  0 $16F4 pu-c!
            then
        then
        0 $1624 pu-l!  0 $1628 pu-l!  0 $162C pu-l!  0 $1630 pu-l!
        1 p-char char.disabled l!  1 p-2a!  0 p-2b!  0 p-2d!  0 $1544 pu-c!  0 $1545 pu-c!
    else
        $1388 pu-1+
        over her c-room = if  1 $16ED pu-c!  0 p-2a!
        else
            $320 vcall play-anim
            mode case
                0 of  pmv-plan set-move  endof   2 of  pmv-plan set-move  endof
                4 of  pmv-wait set-move  endof
                >r  over $1594 pu-l@ =  route-step route-steps >= and if  pmv-idle  else  pmv-wait  then  set-move  r>
            endcase
        then
    then
    dup p-door!  exit-target!  p-room!
    mode 1 = reached-room? and if  2 mode!  then
    $148 vcall ;

\ ---- out of sight: his moves (+0x17A0) ----
: count-down ( F: by -- )  $14C4 pu-f@ fswap f- $14C4 pu-f! ;
\ Pursuer_TravelOffscreen (+0x168): the way to the next door counted down at his pace (after
\ her 1..1.2, searching / waiting 0.6, heading / held off 1; flag 9: 2); at it, through - unless
\ it's into her room through a shut door (he'll knock)
: own-room? ( room -- flag )   \ Pursuer_IsOwnRoom (his rooms, +0x314: with the rooms he keeps to)
    drop false ;
: travel-offscreen ( -- )
    $1664 pu-l@ $17B4 pu-l@ or if  exit  then
    mode 4 = p-room own-room? and if  exit  then
    $14C4 pu-f@ f0> if
        9 state-flag? 0= if
            mode case
                0 of  1e rnd01 0.2e f* f+ count-down  0.6e count-down  endof
                2 of  0.6e count-down  endof   3 of  0.6e count-down  endof
                1 of  1e count-down  endof   4 of  1e count-down  endof
            endcase
        else  2e count-down  then
    then
    $14C4 pu-f@ f0> if  exit  then
    next-door door-exit-of dup exit-target!
    dup 8 < if
        dup through-room played =  over exit-kind 5 = and  mode 4 <> and if  drop 1 step-done! exit  then
        dup may-use-exit? if  through-door  else  drop  then
    else  drop pmv-wait set-move  then ;
' travel-offscreen $168 vt!
\ the way's length to a door (Npc_NodeDistance: the doors' numbers in the room - the original's
\ own measure, Rooms_DoorTri)
: node-distance ( a b -- ) ( F: -- d )
    dup p-room door-tri dup 0< if  2drop drop -1e exit  then  s>f
    2dup = if  2drop exit  then  drop p-room door-tri dup 0< if  drop fdrop -1e exit  then  s>f f+ ;
\ Pursuer_PlanWayOn (kPursuerMove, +0x158): his way on (none: a fresh route, else the search)
: plan-way-on ( -- )
    1 step-next!  p-path-end
    route-step route-steps >=  mode 2 = and if
        -1 $1598 pu-l!
        $16C9 pu-c@ dup 2 = over 4 = or over 1 = or swap 5 = or if
            0 $16C9 pu-c!  0 $16CB pu-c!  0 $16CA pu-c!  0 $16CC pu-c!  -1 $179C pu-l!
            0 $16F1 pu-c!  0 $16F3 pu-c!  0 $16F2 pu-c!  0 $16F4 pu-c!
        then
    then
    p-room p-door room-exit-door                                   ( node )
    plan-where 0= if
        avoid-clear
        $1594 pu-l@ char-route -1 = if  drop  0 route-door door-exit-of exit-target!  start-search exit  then
    then
    0 route-door node-distance $14C4 pu-f!
    0 route-door door-exit-of exit-target!
    ['] travel-offscreen p-move !
    0 $1530 pu-l!  0 $1538 pu-l!  0 $1534 pu-l!  0 route-door $14C0 pu-w!  0 $1784 pu-l! ;
' plan-way-on $158 vt!
\ Pursuer_PlanToGoal (kPursuerWaitMove, +0x15C): at his route's end, where next (none: idle);
\ else the route anew
: plan-to-goal ( -- )
    1 step-next!  p-path-end
    route-step route-steps >= if
        -1 $1598 pu-l!  avoid-clear
        $16C9 pu-c@ dup 1 = over 3 = or over 4 = or swap 5 = or if
            0 $16C9 pu-c!  0 $16CB pu-c!  0 $16CA pu-c!  0 $16CC pu-c!  -1 $179C pu-l!
            0 $16F1 pu-c!  0 $16F3 pu-c!  0 $16F2 pu-c!  0 $16F4 pu-c!
        then
        plan-where 0= if
            pmv-idle set-move  mode 2 <> if  1 $16F4 pu-c!  then  exit
        then
    else $1594 pu-l@ char-route 0> 0= if  route-step $1384 pu-l!  1 $16F4 pu-c!  exit  then then
    0 route-door door-exit-of exit-target!  0 route-door $14C0 pu-w!
    p-door exit-door# 0 route-door node-distance $14C4 pu-f!
    0 $1530 pu-l!  0 $1538 pu-l!  0 $1534 pu-l!
    ['] travel-offscreen p-move !  0 $1784 pu-l! ;
' plan-to-goal $15C vt!
\ Pursuer_Plan16C (kPursuerIdleMove): waiting about (Pursuer_Plan170: out of sight, an exit at
\ random when his waits are over; in sight: his step done)
: plan-170 ( -- )
    in-played-room? if  1 $16ED pu-c!  exit  then
    $17B4 pu-l@ $1664 pu-l@ or 0=  $FF random-exit $FF <> and if  1 step-done!  then ;
' plan-170 $170 vt!
: plan-16c ( -- )
    p-path-left? if  $16  else  $17  then  p-sub!
    0 $1530 pu-l!  0 $1538 pu-l!  0 $1534 pu-l!  ['] plan-170 p-move !  plan-170 ;
' plan-16c $16C vt!
\ Pursuer_KnockAtDoor (kPursuerMoveB): at a shut door into her room - a knock (his table +0x16B0
\ of sounds), heard there; Fiona near it is frightened (6 / 2); then waiting 60..120 frames
: wait-room-flag ( -- )   \ Pursuer_WaitRoomFlag (+0x178)
    exit-target exit-held? if  0 $1624 pu-l!  then
    $1624 pu-l@ 0> if  $1624 pu-1-  else  1 step-done!  then ;
' wait-room-flag $178 vt!
: knock-at-door ( -- )
    60e rnd01 f* 60e f+ f>s $1624 pu-l!
    $21  $16B0 pu-l@ ?dup if
        100e rnd01 f*  begin  dup 4 + exe-f@ fover f< while  8 +  repeat  fdrop  nip exe-sw@
    then  7 0 0 p-sound   \ (heard in her room at the exit's point: the 3D sound of his room - not kept)
    exit-target through-exit her group-fields 4 and if  1 6 0 2 me 5e ask  then
    ['] wait-room-flag p-move ! ;
' knock-at-door $174 vt!

\ ---- Pursuer_StartSearch: in her room the search, from here; elsewhere waiting about ----
: start-search-now ( -- )
    p-cond 2 = if  0 p-cond!  0 $1664 pu-l!  then
    in-played-room? if
        step-next? p-scripted 0= and if
            $175C pu-l@ $C = if  [: $280 vcall ;] behaviour!  $C $118 vcall  else  [: $280 vcall ;] behaviour!  then
        then
        route-clear  1 $1620 pu-c!  1 $1621 pu-c!
    else
        pmv-wait set-move  [: $290 vcall ;] behaviour!  route-step $1384 pu-l!  1 step-next!  0 $17B4 pu-l!
    then
    mode 0= if
        0 $16C9 pu-c!  0 $16CB pu-c!  0 $16CA pu-c!  0 $16CC pu-c!  -1 $179C pu-l!
        0 $16F1 pu-c!  0 $16F3 pu-c!  0 $16F2 pu-c!  0 $16F4 pu-c!
    then
    route-steps $1388 pu-l!  3 mode!  $2C4 vcall ;
' start-search-now is start-search

\ ---- Pursuer_OffscreenUpdate (+0x298) / OffscreenStep (+0x294): his move, and after it ----
: offscreen-update ( -- )
    $16ED pu-c@ 1 = if  exit  then
    p-move run
    $17AC pu-l@ case
        4 of  step-done? if  p-path-left? if  pmv-a  else  pmv-wait  then  set-move  0 step-done!  then  endof
        5 of  step-done? $1664 pu-l@ 0= and  exit-target may-use-exit? and if  0 step-done!  exit-target through-door  then  endof
        >r
        step-done? if  pmv-b set-move  0 step-done!  0 $17B4 pu-l!
        else $17B4 pu-l@ if  pmv-idle set-move  then then
        r>
    endcase ;
: offscreen-step ( -- )
    mode 0= in-played-room? 0= and if
        $1594 pu-l@ her c-room <>  side-behind her-side-behind <> or if
            p-path-end  $B0 vcall  0 $1538 pu-l!  0 $1530 pu-l!  0 $1534 pu-l!  0 $17B4 pu-l!
        then
        p-room her c-room =  side-behind her-side-behind = and if  150 $17B4 pu-l!  then
    then
    offscreen-update ;
' offscreen-step $294 vt!   ' offscreen-update $298 vt!

\ ---- Pursuer_EnterRoom (vtable +0x148): coming into a room - out of sight his search's
\ length there; into hers: at the exit's inner triangle, facing in, shown; his behaviour by how
\ he came; a door shut behind him held, or (it won't) he gives up on it ----
create er-at 12 allot  create er-in 12 allot
: exit-heading ( exit -- ) ( F: -- yaw )   \ Rooms_ExitHeading: from the way through to the way in
    dup 2 exit-spot drop er-at vec!  1 exit-spot drop er-in vec!  er-at er-in vec-heading ;
: enter-room ( -- )
    in-played-room? 0= if  -1 p-tri!  p-cond 2 <> if  not-yet" Pursuer_SearchRouteIn"  then  exit  then
    0 p-char char.disabled l!
    p-door exit-heading
    p-door 1 exit-spot dup 0< if  drop fdrop fdrop fdrop  p-door 2 exit-spot drop  then  me c-place!
    3.1415927e f+ angle-wrap p-yaw!
    mode 4 <> if   \ (from his moves out of sight: his stance - Pursuer_StanceStep - and next step)
        stance-by-fiona
        mode 0= $1544 pu-c@ 0= and if  2 mode!  $2C0 vcall  3 $16C9 pu-c!  4 $16CA pu-c!
        else mode 1 = reached-room? and if
            $159C pu-l@ -2 = if  -1 $159C pu-l!  then
            $16C9 pu-c@ 2 = if
                0 $16C9 pu-c!  0 $16CB pu-c!  0 $16CA pu-c!  0 $16CC pu-c!  -1 $179C pu-l!
                0 $16F1 pu-c!  0 $16F3 pu-c!  0 $16F2 pu-c!  0 $16F4 pu-c!
            then  2 mode!
        then then
        not-yet" Pursuer_SearchRouteIn"  $13C vcall  1 $16F6 pu-c!
    else
        [: $2AC vcall ;] behaviour!  $26 $118 vcall  0 $16F6 pu-c!
    then
    1 step-next!
    p-door has-door? if
        p-door exit-word 2 and if
            mode 2 - 2 u<  $16C9 pu-c@ dup 4 <> over 1 <> and swap 5 <> and and if  $28 $118 vcall  then
        else p-door hold-take if  $C $118 vcall  1 p-2d!  then then
    then ;
' enter-room $148 vt!
