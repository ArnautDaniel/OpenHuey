\ pursuer/npc.fs - what the stalker knows of the room and the others (src/game/pursuer.c, the
\ NPC layer Npc_* / NPC_* / Eye_*): his floor, the way to a place and how long it is, whom he
\ sees and can reach (+0x1544 Fiona, +0x1545 Hewie, +0x1546 a noise), how far they are on foot
\ (+0x1588 Fiona, +0x158C Hewie, +0x1590 the target) and a door in his way.
IN: pursuer.npc
USING: engine game-state events.core events.words chars relations fiona.doors noises pursuer.core pursuer.stubs ;

\ ---- his floor ----
: tri-flags ( tri -- flags )  dup 0< if  drop 0 exit  then  nav-flags ;
: p-mask ( -- mask )  me c-mask ;
\ NavMesh +0x10 == 3: the point lies on the triangle (any floor under it: that one)
: on-tri? ( tri v -- flag )  0 v-tri-in = ;
: tri-blocked? ( tri -- flag )  tri-flags p-mask and 0<> ;          \ Npc_TriBlocked
\ Npc_TriIfStandable: the triangle, or (blocked for him) the nearest he may stand on near it
\ (the original's planner kind 7: cheapest first within 30 of it - here the nearest by edges)
create tis-q 64 cells allot  variable tis-n
: standable? ( tri -- flag )  tri-flags $A8 vcall and 0= ;
: tri-if-standable ( tri -- tri' )
    dup 0< if  exit  then
    dup standable? if  exit  then
    tis-q !  1 tis-n !
    0 begin  dup tis-n @ < over 32 < and  while
        dup cells tis-q + @
        3 0 do
            dup i nav-next dup 0< 0= if
                dup standable? if  nip nip unloop exit  then
                tis-n @ 64 < if  dup tis-n @ cells tis-q + !  1 tis-n +!  else  drop  then
            else  drop  then
        loop  drop  1+
    repeat  drop -1 ;
\ Npc_TriAtDirection: the triangle `dist` from him along `heading`: -1 none, -2 blocked for him,
\ -3 a step (flag 1), -4 at a door, -5 at a ladder
create tad-at 12 allot
: tri-at-direction ( -- tri ) ( F: heading dist -- )
    fswap angle-wrap fswap tad-at p-pos vec-ahead
    me tad-at 0 c-tri-to dup 0< if  exit  then
    dup tri-flags p-mask and if  drop -2 exit  then
    dup tri-flags 1 and if  drop -3 exit  then
    8 0 do  i tad-at vec@ door-near? if  drop -4 unloop exit  then  loop
    nav-links 0 ?do  tad-at i nav-link-at? if  drop -5 unloop exit  then  loop ;

\ ---- the way somewhere and its length (the planner: planned, measured, let go) ----
create pl-a 12 allot  create pl-b 12 allot  fvariable pl-len
\ Character_PathLength: the way from him to (tri, v) within `mask`: its length, -1 none
: path-length ( tri v mask -- ) ( F: -- len )
    >r  pl-b swap vec-copy  >r  p-tri p-pos r> pl-b r> v-path                 ( n )
    dup 0= if  drop -1e exit  then
    0e pl-len f!  pl-a p-pos vec-copy
    0 do  i nav-path-point pl-b vec!  pl-a pl-b vec-dist-xz pl-len f@ f+ pl-len f!  pl-a pl-b vec-copy  loop
    pl-len f@ ;
\ Rooms_ExitLoops: the exit leads back into this room
: exit-loops? ( exit -- flag )  exit-leads drop played = ;
\ Npc_NearestWalkable: for a place on a triangle blocked for him, the nearest he may stand on
\ toward it - in a doorway (Doors_InArea 0) the exit's triangle through, else from the nearest
\ standable one walking toward it the edge he'd meet, 0.1 short of it. out: the point; its
\ triangle (-1 none)
create nw-c 12 allot  fvariable nw-h  fvariable nw-d  variable nw-pos  variable nw-out
: nearest-walkable ( tri pos out -- tri' )
    nw-out !  nw-pos !
    dup tri-blocked? 0= if  nw-out @ nw-pos @ vec-copy  exit  then
    -1  8 0 do
        i exit-loops? 0= if  nw-pos @ vec@ i 0 door-in-area? if  drop i 2 exit-tri  leave  then  then
    loop                                                         ( tri t )
    dup -1 = if  drop tri-if-standable  else  nip  then
    dup 0< if  exit  then
    dup tri-center nw-c vec!
    nw-c nw-pos @ vec-heading nw-h f!  nw-c nw-pos @ vec-dist-xz nw-d f!
    dup nw-c p-mask  nw-h f@ nw-d f@ v-free                        ( t ) ( F: free )
    fdup nw-d f@ f< if  nw-out @ nw-c nw-h f@ fswap vec-ahead
    else  fdrop  nw-out @ nw-pos @ vec-copy  then
    nw-out @ nw-out @ nw-h f@ 3.1415927e f+ 0.1e vec-ahead      \ (0.1 back toward the middle)
    drop  nw-out @ p-mask v-tri-in ;
\ Npc_PathLength's kind (Character_PlanPathKind + Character_WaypointsCurve): the way planned as
\ his path, its length (-1 none)
create pk-v 12 allot
: plan-length ( tri v -- ) ( F: -- len )
    >r >r me r> r> p-mask c-plan 0= if  -1e exit  then
    0e pl-len f!  pl-a p-pos vec-copy
    p-path-n 0 do  me i path-point pl-b swap vec-copy  pl-a pl-b vec-dist-xz pl-len f@ f+ pl-len f!  pl-a pl-b vec-copy  loop
    pl-len f@ ;
\ NPC_PathLengthTo (vtable +0xD4): to (tri, v) (a blocked one: its nearest walkable)
create plt-v 12 allot
: path-length-to ( tri v -- ) ( F: -- len )
    over tri-blocked? if  plt-v nearest-walkable  else  plt-v swap vec-copy  then
    dup 0< if  drop -1e exit  then
    plt-v $A8 vcall path-length ;
' path-length-to $D4 vt!

\ Npc_DistanceTo: how far `cs` is on foot (straight when nothing is between them), -1 when
\ it isn't in his room
create dt-v 12 allot
: distance-to ( cs -- ) ( F: -- d )
    dup c-room p-room <> if  drop -1e exit  then
    dup c-tri over c-pos on-tri? 0= if                            \ (its place off its triangle:)
        dup c-tri tri-center dt-v vec!
        dup c-tri  me dt-v p-mask c-tri-to = if  me swap c-pos c-dist-to exit  then
    then
    dup c-tri swap c-pos $D4 vcall ;
\ his path left to walk (the planner's +0x3C along the waypoints)
: path-left ( F: -- d )  p-path-left? if  me path-rest  else  0e  then ;
\ Npc_PathClear: the way ahead `d` along his path is on his floor
: path-clear? ( F: d -- flag )
    me path-ahead drop  pa-pos me c-mask v-tri-in 0< 0= ;
\ the target's place: Fiona +0x1588, Hewie +0x158C, the path +0x1590
: d-fiona ( F: -- d )  $1588 pu-f@ ;   : d-fiona! ( F: d -- )  $1588 pu-f! ;
: d-hewie ( F: -- d )  $158C pu-f@ ;   : d-hewie! ( F: d -- )  $158C pu-f! ;
: d-path ( F: -- d )  $1590 pu-f@ ;    : d-path! ( F: d -- )  $1590 pu-f! ;
\ NPC_PathLengthChar (vtable +0xE0): the way to `cs` (its nearest walkable place): 1 if there is one
create plc-v 12 allot
: path-length-char ( cs -- flag )
    dup 0< if  drop p-target  then
    dup c-tri over c-pos on-tri? 0= if  dup c-tri tri-center plc-v vec!
    else  plc-v over c-pos vec-copy  then
    dup c-tri plc-v plc-v nearest-walkable  plc-v plan-length  d-path!
    dup her = if  drop d-path d-fiona!  else  dog = if  d-path d-hewie!  then  then
    d-path f0< 0= ;
' path-length-char $E0 vt!
\ NPC_PathLengthSpot (+0xDC: to +0x15C4 / +0x15D0) and NPC_PathLengthGoal (+0xD8: +0x15A4 / +0x15B0)
create pls-v 12 allot
: path-length-place ( tri pos -- flag )
    >r r@ pls-v nearest-walkable  dup 0< if  r> 2drop  -1e d-path!  false exit  then
    pls-v plan-length  d-path!
    d-path f0< if  r> drop false exit  then
    pls-v r> vec-dist-xz  d-path f+ d-path!  true ;
: path-length-spot ( -- flag )  $15C4 pu-l@ $15D0 pu path-length-place ;
: path-length-goal ( -- flag )  $15A4 pu-l@ $15B0 pu path-length-place ;
' path-length-spot $DC vt!   ' path-length-goal $D8 vt!

\ ---- sight (+0x1574 his heading with his head's turn, +0x1580 the range, +0x1584 half the
\ angle) ----
fvariable cs-half  fvariable cs-range  fvariable cs-head
\ Eye_CanSee: `to` within the range and the half angle of `heading` from `from`
: can-see? ( from to -- flag ) ( F: heading range half -- )
    cs-half f!  cs-range f!  cs-head f!
    2dup vec-dist-xz f0= if  2drop false exit  then              \ (the same place: no)
    2dup vec-dist-xz cs-range f@ f> if  2drop false exit  then
    vec-heading cs-head f@ f- angle-wrap fabs cs-half f@ f<= ;
\ the floor the sight goes over (progress flags 9 / 0xA: low walls stop it too)
: sight-mask ( -- mask )  9 state-flag? $A state-flag? or if  $40088  else  $40080  then ;
\ Npc_SeesChar: `cs` in his sight and the way straight to it (or to a point beside it) clear
create sc-at 12 allot  create sc-me 12 allot  create sc-off 12 allot  create sc-pt 12 allot
fvariable sc-ang
: sees-char? ( cs -- flag )
    dup c-tri over c-pos on-tri? 0= if  dup c-tri tri-center sc-at vec!  else  sc-at over c-pos vec-copy  then
    p-tri p-pos on-tri? 0= if  p-tri tri-center sc-me vec!  else  sc-me p-pos vec-copy  then
    p-pos over c-pos  $1574 pu-f@ $1580 pu-f@ $1584 pu-f@ can-see? 0= if  drop false exit  then
    p-tri sc-me sc-at sight-mask v-walk over c-tri = if  drop true exit  then
    \ (round it: points its radius off the line to it, every 22.5 degrees)
    sc-me sc-at vec-heading  1.5707964e f+ sc-ang f!
    9 0 do
        sc-pt sc-at sc-ang f@ 2 pick c-radius vec-ahead
        me sc-pt 0 c-tri-to dup 0< 0= if
            >r  over c-tri sc-at sc-pt sight-mask v-walk 0< r@ 0< 0= and
            p-tri sc-me sc-pt sight-mask v-walk r> = and if  drop true unloop exit  then
        else  drop  then
        sc-ang f@ 0.3926991e f+ sc-ang f!
    loop  drop false ;
\ Npc_SeesPoint
: sees-point? ( tri pos -- flag )
    p-pos over $1574 pu-f@ $1580 pu-f@ $1584 pu-f@ can-see? 0= if  2drop false exit  then
    >r  p-tri p-pos r> 0 v-walk = ;
\ Fiona's hiding (her +0x1AD630: with the hiding places)
defer her-hidden? ( -- flag )   ' false is her-hidden?
\ NPC_FionaInReach (vtable +0xC0): seen, or (progress flag 9 off) near on foot - 10 with flag 0xA,
\ else 20 - and straight there
: fiona-in-reach? ( -- flag )
    her-hidden? if  false exit  then
    her sees-char? if  true exit  then
    9 state-flag? if  false exit  then
    $A state-flag? if  10e  else  20e  then  d-fiona fswap f<  d-fiona f0> and
    p-2b 0= and if  me her c-pos $40080 c-tri-to her c-tri =  else  false  then ;
\ NPC_HewieInReach (+0xC4): seen, or (flag 0xB off) within 20 on foot and straight there
: hewie-in-reach? ( -- flag )
    dog sees-char? if  true exit  then
    $B state-flag? if  false exit  then
    d-hewie 20e f<  d-hewie f0> and  p-2b 0= and if  me dog c-pos $40080 c-tri-to dog c-tri =  else  false  then ;
\ NPC_HearNoise (+0xC8): a noise heard (the noise slots: with them)
: heard-noise? ( -- flag )  me c-heard-slot $FF <> ;
' fiona-in-reach? $C0 vt!   ' hewie-in-reach? $C4 vt!   ' heard-noise? $C8 vt!

\ his step this frame (Motion_RootMovement): how far
: root-step ( F: -- d )  p-actor dup 0< if  drop 0e exit  then  root-delta fswap fdrop fsq fswap fsq f+ fsqrt fswap fdrop ;
\ Npc_Senses modes 2 / 3 (walking his path): the one not watched this frame straight; while
\ walking, the way ahead (30, then this frame's step) not clear: mode 3 the goal's way
\ (+0xD8), else the spot's (+0xDC) if there is one; else the path left
: senses-path ( mode -- flag )
    $15A0 pu-c@ if  p-room her c-room = if  her distance-to  else  -1e  then  d-fiona!
    else  p-room dog c-room = if  dog distance-to  else  -1e  then  d-hewie!  then
    p-path-left? if
        30e path-clear? if  root-step fdup f0< if  fdrop false  else  path-clear?  then  else  false  then
        0= if  3 = if  $D8 vcall  else  $15C4 pu-l@ -1 <> if  $DC vcall  else  false  then  then  exit  then
    then  drop
    path-left fdup d-path!  f0< 0= ;
\ ---- Npc_Senses: how far they are on foot, by his sense mode (+0x15C0: 0 Fiona, 1 Hewie, 2 / 3
\ his path, else straight), alternating frames (+0x15A0); with `fiona-room` only in her room ----
fvariable sn-v
: senses ( fiona-room -- flag )
    p-room played <> if  drop  -1e d-path!  -1e d-hewie!  -1e d-fiona!  false exit  then
    $15C0 pu-c@ case
        0 of
            $15A0 pu-c@ if
                0= p-room her c-room = or if  her $E0 vcall  else  -1e d-path!  -1e d-fiona!  false  then
            else
                p-room dog c-room = if  dog distance-to  else  -1e  then  d-hewie!
                0= p-room her c-room = or if  path-left fdup d-path! d-fiona!  else  -1e d-path!  -1e d-fiona!  then
                d-hewie f0>
            then
        endof
        1 of  drop
            $15A0 pu-c@ if
                p-room dog c-room = if  dog $E0 vcall  else  -1e d-path!  -1e d-hewie!  false  then
            else
                p-room her c-room = if  her distance-to  else  -1e  then  d-fiona!
                p-room dog c-room = if  path-left fdup d-path! d-hewie!  else  -1e d-path!  -1e d-hewie!  then
                d-fiona f0>
            then
        endof
        2 of  drop  2 senses-path  endof
        3 of  drop  3 senses-path  endof
        >r drop
        $15A0 pu-c@ if  p-room her c-room = if  her distance-to  else  -1e  then  d-fiona!
        else  p-room dog c-room = if  dog distance-to  else  -1e  then  d-hewie!  then
        p-target her = if  d-fiona  else  d-hewie  then  d-path!  false
        r>
    endcase
    $15A0 pu-c@ 0= 1 and $15A0 pu-c! ;
\ Npc_SensesWatching (and Npc_SensesFiona: during the countdown, only in her room)
: senses-watching ( -- flag )  countdown? senses ;

\ ---- a door in his way (Npc_DoorOnWay): of the exits with a door held open (state bit 0) -
\ looping exits: him at its side (4) not as Fiona is (0x10 differs), or on its sides (8) on a
\ triangle blocked for him; others: him in its area (1) or on its side 0. 0xFF none ----
: group-fields ( exit slot -- f )  flags-of ;   \ PursuerGroup_Fields
: door-held? ( exit -- flag )  exit-door dup 0< if  drop false exit  then  door-word dup if  l@ 1 and 0<>  then ;
: door-on-way ( -- exit | $FF )
    8 0 do
        i exit-door 0< 0= i has-door? and i door-held? and if
            i me group-fields
            i exit-loops? if
                dup 4 and over i her group-fields xor $10 and 0<> and if  drop i unloop exit  then
                8 and if  p-tri tri-flags p-mask and if  i unloop exit  then  then
            else
                1 and  i 0 p-tri door-on-side? or if  i unloop exit  then
            then
        then
    loop  $FF ;

\ ---- who is about (Npc_WhoAround / Npc_WhoAroundEnding): +0x1574 his sight's heading (his
\ head's turn added), Fiona +0x1544 (a door in his way counts as her), Hewie +0x1545, a noise
\ +0x1546, then his target (vtable +0xCC) ----
fvariable head-yaw   \ (his motion's +0x858: the head's turn)
: who-around ( -- )
    p-room played <> if  0 $1544 pu-c!  0 $1545 pu-c!
    else
        p-yaw head-yaw f@ f+ angle-wrap $1574 pu-f!
        p-mode 2 <> door-on-way $FF <> and if  1                    \ (during the countdown only in
        else countdown? p-room her c-room <> and if  0              \ her room: Npc_WhoAround)
        else  $C0 vcall 1 and  then then  $1544 pu-c!
        played dog c-room <> if  0  else  $C4 vcall 1 and  then  $1545 pu-c!
    then
    $C8 vcall 1 and $1546 pu-c!  $CC vcall drop ;
