\ pursuer/attack.fs - the stalker's attacks (src/game/pursuer.c): a combo of his moves (+0x1724:
\ 4 move numbers a combo, -1 ending it; +0x1728 the combo, +0x172C the step) from his move
\ table (+0x171C: 0x24 a move - its animation, the bones that strike, their reach, the blow's
\ kind, damage, threat, the chance of a stumble, turning with it, its cry), each blow a request
\ to those it reaches (Relation_Request); the behaviour around it (Pursuer_BehaviourAttack) and
\ his next behaviour after (Pursuer_PickStep).
IN: pursuer.attack
USING: engine game-state events.core events.words chars relations pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps pursuer.behave pursuer.search pursuer.moves pursuer.chase pursuer.debchase ;

\ ---- his move table ----
: combo# ( -- addr )   \ the move number of the combo's step
    $1728 pu-l@ 4 * $172C pu-c@ + $1724 pu-l@ + ;
: move-no ( -- n )  combo# exe-c@ dup $80 and if  $100 -  then ;
: next-move-no ( -- n )  combo# 1+ exe-c@ dup $80 and if  $100 -  then ;
: move# ( -- e )  move-no $24 * $171C pu-l@ + ;
: e-l@ ( off -- n )  move# + exe-l@ ;    : e-c@ ( off -- n )  move# + exe-c@ ;
: e-w@ ( off -- n )  move# + exe-w@ ;    : e-f@ ( off -- ) ( F: -- r )  move# + exe-f@ ;

\ ---- the threat he puts on her (Pursuer_Threat, vtable +0x12C; Threat_Raise on the panic) ----
: threat-raise ( F: amount -- )
    fdup f0< if  fdrop exit  then
    fdup 10e f< if  fiona.panic:pn-d2 f@ f+ fiona.panic:pn-d2 f!  exit  then
    0.5e f*  30 fiona.panic:pn-hold !
    fdup fiona.panic:pn-d2 f@ f+ fiona.panic:pn-d2 f!  fiona.panic:pn-a2 f@ f+ fiona.panic:pn-a2 f! ;
: threat ( F: amount -- )
    d-fiona f0< if  fdrop exit  then
    d-fiona 10e f< if  threat-raise exit  then
    0.75e f*  d-fiona 20e f< if  threat-raise exit  then
    0.75e f*  d-fiona 30e f< if  threat-raise exit  then
    0.75e f*  d-fiona 40e f< if  threat-raise exit  then  fdrop ;
' threat $12C vt!

\ ---- whom a blow reaches (Npc_WhoReachable): the others near the point (Progress_CharNear:
\ within their height and radius, widened by `reach`) in front of him (90 degrees) ----
fvariable cn-m  fvariable cn-y  fvariable cn-by
: char-near? ( pos cs -- flag ) ( F: margin -- )
    cn-m f!
    dup c-active? 0= if  2drop false exit  then
    dup character char.disabled sl@ if  2drop false exit  then
    over 4 + sf@ cn-y f!  dup c-pos 4 + sf@ cn-by f!
    cn-y f@ cn-by f@ cn-m f@ f- f<= if  2drop false exit  then
    cn-y f@ cn-by f@ dup c-height f+ cn-m f@ f+ f< 0= if  2drop false exit  then
    dup c-pos rot swap vec-dist-xz  c-radius cn-m f@ f+ f< ;
fvariable wr-reach
: who-reachable ( pos -- bits ) ( F: reach -- )
    wr-reach f!  0
    3 0 do
        i me <> if
            over i wr-reach f@ char-near? if
                p-yaw me i c-pos c-heading-to f- angle-wrap fabs 1.5707964e f< if  1 i lshift or  then
            then
        then
    loop  nip ;
\ Pursuer_BonePositions (vtable +0x138): where the move's bones are
create other-at 12 allot
: bone-positions ( -- )
    p-actor 4 e-l@ bone-pos $1770 pu vec!
    8 e-l@ 0< 0= if  p-actor 8 e-l@ bone-pos other-at vec!  then ;
\ Npc_CanWalkStraight: nothing but floor between him and the point (true: blocked)
: walk-blocked? ( v -- flag )
    me over 0 c-tri-to dup 0< if  2drop false exit  then
    >r  p-tri p-pos rot 0 v-walk r> <> ;

\ ---- Pursuer_AttackNextStep (action 0x13): the combo's next move - while she's in his room and
\ near (his reach, 50, or the ground he gains) - its animation and how it moves (0x1A / 0x1B /
\ 0x1C; 6: held, StateAttackActive); the combo over: done ----
defer state-attack-step   defer state-attack-active
: combo-over ( -- )
    -1 p-104!  0 $172C pu-c!  -1 $1728 pu-l!  1 step-done!  0 $16F7 pu-c!  1 step-next! ;
: attack-next-step ( -- )
    0 $16EC pu-c!  0 step-next!
    walk-on? if  exit  then
    $171C pu-l@ 0= if  1 $16EF pu-c!  1 step-next!  exit  then
    $172C pu-c@ 4 < if
        move-no -1 <>  p-room p-target c-room = and if
            $10 e-c@ 6 <> if
                50e ground-gained fmax  me p-target c-pos c-dist-to f> 0=
            else  false  then  $10 e-c@ 6 = or if
                0 $1760 pu-c!  0 e-l@ play-anim  $1D e-c@ $16F7 pu-c!  0 $1784 pu-l!
                $10 e-c@ case
                    6 of  ['] state-attack-active behave  state-attack-active  exit  endof
                    1 of  $1A p-sub!  endof  2 of  $1B p-sub!  endof  4 of  $1C p-sub!  endof
                endcase
                0 p-104!  ['] state-attack-step behave  state-attack-step  exit
            then
        then
    then
    combo-over ;
' attack-next-step is st.Pursuer_AttackNextStep
' attack-next-step is pursuer.debact:attack-next-step

\ ---- Pursuer_StateAttackStep: the move to its end (then held off: hit his table's first word,
\ else its second; 90 / 30); turning with it; at its strike key (2) the blow - blocked by a
\ wall: his stand and 0x2B; else his cry (0x10) once, and those it reaches told (kind, damage,
\ a stumble by its chance, threat); after it, the next move while she's within 50 ----
: hold-off-after ( -- )
    $1760 pu-c@ if  $1748 pu-l@ ?dup if  exe-l@  else  90  then
    else  $1748 pu-l@ ?dup if  4 + exe-l@  else  30  then  $178C pu-l@ max  then  $178C pu-l! ;
: strike ( -- )
    $E state-flag? if  exit  then
    p-faded? if
        p-actor 4 e-l@ bone-pos other-at vec!
        $1788 pu-l@ 0<>  other-at walk-blocked? and if
            $320 vcall play-anim  0 $16F7 pu-c!  $2B 7 0 0 p-sound  exit
        then
    then
    p-104 1 <> if
        p-104 2 = if  0 $1760 pu-c!  then  $10 7 0 0 p-sound  1 p-104!
    then
    bone-positions
    $1770 pu  $C e-f@ who-reachable  $1760 pu-c@ invert and $FF and
    8 e-l@ 0< 0= if  other-at $C e-f@ who-reachable $1760 pu-c@ invert and $FF and or  then
    ?dup if
        $10 e-c@  $12 e-w@  100e rnd01 f* $18 e-f@ f<= if  -32768  else  0  then  me  $14 e-f@ ask
        $20 e-l@ $1764 pu-l!
    then ;
: state-attack-step-now ( -- )
    p-ended? if
        1 step-done!  1 step-next!  hold-off-after
        -1 p-104!  0 $172C pu-c!  -1 $1728 pu-l!  0 $1770 pu-l!  0 $1774 pu-l!  0 $1778 pu-l!  exit
    then
    $1C e-c@ if  me p-target c-pos c-heading-to $A0 vcall turn-toward fdrop  then
    p-freeze 0<= if  root-move-masked  then
    -1 p-events-at 2 and if  strike exit  then
    p-104 1 <> if  exit  then
    p-target her = $1760 pu-c@ 1 and 0= and if  $14 e-f@ 2e f/ $12C vcall  then
    $172C pu-c@ 4 <  next-move-no -1 <> and  me p-target c-pos c-dist-to 50e f< and if
        $172C pu-c@ 1+ $172C pu-c!  0 $1770 pu-l!  0 $1774 pu-l!  0 $1778 pu-l!
        ['] attack-next-step behave exit
    then
    2 p-104!  0 $16F7 pu-c! ;
' state-attack-step-now is state-attack-step
\ Pursuer_StateAttackActive (a held move, kind 6): at its key, everyone within its reach held
: state-attack-active-now ( -- )
    -1 p-events-at 2 and  $E state-flag? 0= and if
        3 0 do
            i me <> i c-active? and if
                me i c-pos c-dist-to $C e-f@ f< if  $1760 pu-c@ 1 i lshift or $1760 pu-c!  then
            then
        loop
        $1760 pu-c@ ?dup if  $10 e-c@ $12 e-w@ 4 e-l@ $FFFF and dup $8000 and if  $10000 -  then  me $14 e-f@ ask  then
    then
    p-ended? if
        $1760 pu-c@ if  $1748 pu-l@ ?dup if  8 + exe-l@  else  30  then  $178C pu-l!  then
        $172C pu-c@ 1+ $172C pu-c!  ['] attack-next-step behave exit
    then
    $1C e-c@ if  me p-target c-pos c-heading-to $A0 vcall turn-toward fdrop  then
    root-move-masked ;
' state-attack-active-now is state-attack-active

\ ---- the behaviour around his attacks ----
\ Pursuer_MayGoForTarget: she isn't hidden away (+0x2D), progress flag 0xE, her move 0x10
: may-go? ( -- flag )
    fiona.core:f-2d @ 1 =  $E state-flag? or if  false exit  then  her character char.sub sl@ $10 <> ;
\ Pursuer_RoomAround: nothing beside or behind within 2
: room-around? ( -- flag )
    3.1415927e 2e target-side-walkable? 0=  1.9198622e 2e target-side-walkable? 0= and
    -1.9198622e 2e target-side-walkable? 0= and ;
: attack-again ( -- )  $1760 pu-c@ if  $C  else  $D  then  $130 vcall  pick-from-table ;
: turn-dir ( -- way )  p-target c-pos 1.0471976e 2.6179939e turn-way-to ;
\ Pursuer_ChanceRoll: on Hewie - his table +0x1740 (chance, rise) by how things stand
: chance-roll ( -- flag )
    $1740 pu-l@ ?dup if
        dog c-cond 2 = if  0  else $1760 pu-c@ 2 and if  8  else $1761 pu-c@ if  $18  else  $10  then then then
        +  dup exe-f@ $16C0 pu-f@ f+  100e rnd01 f* f< 0= if
            drop 0 $16BC pu-l!  0e $16C0 pu-f!
        else  4 + exe-f@ $16C0 pu-f@ f+ $16C0 pu-f!  false exit  then
    else  0 $16BC pu-l!  0e $16C0 pu-f!  then
    0 $16F8 pu-c!  true ;
\ (BehaviourAttack's cases)
: attack-gesture ( -- )   \ 0x17 / 0x1B: a taunt - done; lost sight of her, or far: over
    step-done? if  0 step-done!  1 $16ED pu-c!  exit  then
    $175C pu-l@ $1B <>  p-104 8 * $1720 pu-l@ + 4 + exe-c@ 0= and if  exit  then
    p-target her = $1544 pu-c@ 0= and  p-target dog = $1545 pu-c@ 0= and or if  1 $16ED pu-c!  exit  then
    d-path 60e f< 0=  d-path f0< or if  1 $16ED pu-c!  then ;
: attack-turned ( -- )    \ 3 / 0x18 / 0x1A: a turn or step done - round to her, in, or back off
    step-done? 0= if  exit  then  0 step-done!
    may-go? her c-mode 4 <> and  $178C pu-l@ 0= and if  1 $16ED pu-c!  exit  then
    d-path 40e f<= 0=  d-path f0< or if  1 $16ED pu-c!  exit  then
    turn-dir dup $FF <> if  p-104!  3 $114 vcall  exit  then  drop
    d-path 20e f<= 0=  room-around? or if  30 $162C pu-l!  1 $114 vcall
    else  $1D $114 vcall  then ;
: attack-other ( -- )     \ any other: a blow over
    $16EF pu-c@ 1 = if  $16 $114 vcall  0 $16EF pu-c!  exit  then
    step-done? 0= if  exit  then  0 step-done!
    p-target dog = if  chance-roll drop  $16C9 pu-c@ 2 < if  2 $16C9 pu-c!  3 $16CA pu-c!  then  then
    $178C pu-l@ 0=  d-path 100e f<= 0= or if  1 $16ED pu-c!
    else d-path 20e f<= if  $1D $114 vcall  else  attack-again  then then ;
: behaviour-attack ( -- )
    p-state run
    $175C pu-l@ case
        1 of  $178C pu-l@ 0= if  1 $16ED pu-c!  else  $162C pu-l@ dup 1- $162C pu-l! 0<= if  attack-again  then  then  endof
        2 of  step-done? if  0 step-done!  1 $16ED pu-c!  then  endof
        $17 of  attack-gesture  endof   $1B of  attack-gesture  endof
        $1D of
            may-go? her c-mode 4 <> and  $178C pu-l@ 0= and if  1 $16ED pu-c!  then
            d-path 20e f<= 0=  room-around? or if  attack-again  then
        endof
        $18 of
            p-104 8 * $1720 pu-l@ + 4 + exe-c@ if
                turn-dir dup $FF <> if  p-104!  3 $114 vcall  0 $178C pu-l!  else  drop  then
            then  attack-turned
        endof
        3 of  attack-turned  endof   $1A of  attack-turned  endof
        $16 of
            step-done? if
                turn-dir dup $FF <> if  p-104!  3 $114 vcall
                else  drop  d-path 20e f<= 0= if  1 $16ED pu-c!
                else  $1748 pu-l@ ?dup if  4 + exe-l@  else  30  then  $178C pu-l!  $1D $114 vcall  then  then
                0 step-done!
            then
        endof
        >r attack-other r>
    endcase
    step-next? 0=  $16ED pu-c@ 1 <> or if  exit  then
    0 $1760 pu-c!  0 $178C pu-l!  0 $162C pu-l!
    p-target her = if
        $1544 pu-c@ 1 = if
            d-path fdup ground-gained f<  fdup 10e f< or  fdup f0> and  fdrop if
                me p-target c-pos c-heading-to p-yaw f- angle-wrap fabs  3.1415927e $2EC vcall f* 180e f/ f<
                p-target same-floor? and if  fiona-state $130 vcall  pick-from-table  exit  then
            then
        then
    else p-target dog = if
        may-go? 0= if  0 $16BC pu-l!  0 $16C0 pu-l!  0 $16F8 pu-c!  then
        $16DC pu-l@ $16BC pu-l@ u<  $16F3 pu-c@ 0= and if  [: $270 vcall ;] behaviour!  exit  then
    then then
    $13C vcall
    $16F3 pu-c@ if  0 $16F3 pu-c!  $1C $118 vcall  then ;
' behaviour-attack $28C vt!

\ ---- Pursuer_Behaviour288 (vtable +0x288): his attack's behaviour - the action asked for
\ (0x14 if none), fresh ----
: behaviour-288 ( -- )
    $14 begin-action  fresh  0 $1760 pu-c!  0 $162C pu-l!
    [: $28C vcall ;] behaviour!  $28C vcall ;
' behaviour-288 $288 vt!
:noname  $288 vcall ; is pursuer.search:table-behaviour

\ ---- Pursuer_PickStep (vtable +0x13C): his next behaviour by his mode - after her: his chase
\ if he sees her, else after her; heading for her: after her; searching: chasing where she
\ was seen (followed modes 1 4 5), else the search; waiting: after her when his route is
\ done, else the search; held off: back after her (+0x2AC); down: ended ----
: pick-step ( -- )
    countdown? if  $140 vcall exit  then
    p-cond 2 = if  [: $29C vcall ;] behaviour! exit  then
    mode case
        0 of  $C0 vcall 1 and dup $1544 pu-c!
              1 <> if  $B0 vcall  [: $278 vcall ;]  else  [: $264 vcall ;]  then  behaviour!  endof
        1 of  $16C9 pu-c@ 6 = if  $B0 vcall  then  [: $280 vcall ;] behaviour!  endof
        2 of  $16C9 pu-c@ dup 4 = over 1 = or swap 5 = or if
                  [: $278 vcall ;] behaviour!
                  $179C pu-l@ dup -1 <> if  dup $15A4 pu-l!  tri-center $15B0 pu vec!  else  drop  then
              else  [: $25C vcall ;] behaviour!  then  endof
        3 of  $1620 pu-c@ $1621 pu-c@ >=  $1621 pu-c@ $FF <> and if  [: $280 vcall ;]  else  [: $25C vcall ;]  then
              behaviour!  endof
        4 of  [: $2AC vcall ;] behaviour!  endof
        >r [: $25C vcall ;] behaviour! r>
    endcase ;
' pick-step $13C vt!
