\ pursuer/debchase.fs - Debilitas after Fiona (src/game/debilitas.c, debilitas_body.inc): his
\ chase in her room - closing in (action 1), holding back (5) or circling (6) by chance and how
\ she faces him (his table +0x17F0), a lunge or grab by the threat's chance every 90 frames, his
\ strike once she's in reach (his attack table for her state) - and the Pursuer's chase
\ behaviours around it (Pursuer_ChaseFiona, Debilitas_BehaviourRun).
IN: pursuer.debchase
USING: engine game-state events.core events.words chars relations fiona.doors pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps pursuer.behave pursuer.search pursuer.moves pursuer.target pursuer.chase ;

\ his chase table (+0x17F0: str_Z_2): frames and chances
: zt@ ( off -- n )  $17F0 pu-l@ + exe-l@ ;
: zt-f@ ( off -- ) ( F: -- r )  $17F0 pu-l@ + exe-f@ ;
: hold-t ( -- n )  $162C pu-l@ ;   : hold-t! ( n -- )  $162C pu-l! ;
: wait2 ( -- n )  $1630 pu-l@ ;    : wait2! ( n -- )  $1630 pu-l! ;
: close-reach ( F: -- d )  $17E4 pu-f@ ;   \ (+0x17E4: 50, 40 hard)

\ Eye_ActorSees(target, him, her heading, his range, 90 degrees): she faces him (Debilitas_Seen)
: seen? ( -- flag )
    p-target c-pos p-pos  p-target c-yaw $1580 pu-f@ 1.5707964e can-see? ;
\ Pursuer_ThresholdEntry: the chance for how near she is (D_003AF4B0: 15 100, 30 40, 60 20)
: threshold-chance ( -- n )
    3 0 do  d-fiona i 8 * $3AF4B0 + exe-f@ f<= if  i 8 * $3AF4B4 + exe-l@ unloop exit  then  loop  0 ;
\ Debilitas_BackOrHold: hold back (5) or circle (6) by his table's chance for situation i
: back-or-hold ( i -- ) ( F: roll -- )
    dup 4 * $24 + zt-f@ f<= if  5 $114 vcall  8 * zt@ hold-t!
    else  6 $114 vcall  8 * 4 + zt@ hold-t!  then ;
\ Debilitas_StrikeOrWait: a lunge or grab by the threat's chance (his table 0xA), else back or
\ hold by how she faces him. True: he strikes
: strike-or-wait? ( -- flag )
    100e rnd01 f* threshold-chance s>f f< if  $A $130 vcall  pick-from-table  true exit  then
    100e rnd01 f*
    seen? 0= if  d-fiona close-reach f<= 1 and  else  2  then  back-or-hold  false ;

\ ---- her state for his attack tables (Pursuer_FionaState): 0 calm, 1 fleeing (mode 0xB), 2
\ panicking (Npc_FionaPanicking: fear over 90 or a panic attack), 3 out of reach, 4 / 5 the
\ panic's stages 4 / 5 ----
: fiona-panicking? ( -- flag )  fiona.core:f-fear f@ 90e f>  fiona.core:f-act @ $E = or ;
: fiona-state ( -- n )
    fiona.core:f-2d @ 1 <>  her character char.sub sl@ $10 <> and  $E state-flag? 0= and
    0= if  3 exit  then   \ (her +0x2D: hidden away - with the hiding)
    her character char.sub sl@ dup $A = swap $B = or                      ( hiding? )
    panic @ case
        4 of  0= if  4  else  5  then  endof
        5 of  drop 5  endof
        >r  0= if  her c-mode $B = if  1  else  fiona-panicking? if  2  else  0  then  then
            else  3  then  r>
    endcase ;
\ Pursuer_GroundGained: how much nearer he'll be in 5 frames (his strides, less hers when she
\ doesn't face him), and his reach for Hewie (+0x2F0; the panic's stage 5: 10)
: ground-gained ( F: -- d )
    stride 5e f*
    p-target c-pos p-pos p-target c-yaw 150e 1.5707964e can-see? 0= if  not-yet" GroundGained: her strides"  then
    panic @ 5 = p-target her = and if  10e f+  else  $2F0 vcall f+  then ;
\ Npc_SameFloor
: same-floor? ( cs -- flag )
    c-pos 4 + sf@ fdup p-pos 4 + sf@ 15e f+ f<=  10e f+ p-pos 4 + sf@ f< 0= and ;
\ Npc_TargetSideWalkable: the floor `dist` from him at `angle` off the way to his target
create tsw-at 12 allot
: target-side-walkable? ( F: angle dist -- flag )
    fswap me p-target c-pos c-heading-to f+ angle-wrap fswap  tsw-at p-pos vec-ahead
    me tsw-at p-mask c-tri-to dup 0< if  drop false exit  then  tri-flags p-mask and 0= ;
\ Npc_OpenDoorFionaHides: hiding (her mode 3), the ladder she's at (with the hiding)
: hide-door ( -- link | -1 )  her c-mode 3 = if  not-yet" Npc_OpenDoorFionaHides"  then  -1 ;

\ ---- Debilitas_Chase (his behaviour while after her in her room) ----
: chase-lunge-roll ( -- done? )   \ (every 90 frames: a lunge by the threat's chance)
    $1780 pu-l@ 1+ 90 mod 0= if
        100e rnd01 f* threshold-chance s>f f<= if  $A $130 vcall  pick-from-table  true exit  then
    then  false ;
: chase-closing ( -- done? )   \ action 1: closing in
    chase-lunge-roll if  true exit  then
    p-faded? if
        her c-pos 1.0471976e 2.6179939e turn-way-to dup $FF <> if  p-104!  3 $114 vcall  else  drop  then
    then
    100e rnd01 f*
    d-fiona f0< if  fdrop  target-out-of-reach? if  false exit  then  chase-fiona-here true exit  then
    seen? 0= if  1 back-or-hold
    else d-fiona close-reach 20e f+ f<= 0= if  2 back-or-hold
    else  fdrop  wait2 0> if  wait2 1- wait2!  else  5 $114 vcall  $18 zt@ hold-t!  then then then
    false ;
: chase-circling ( -- done? )   \ action 6: circling at a distance
    0 $16EF pu-c!
    d-fiona close-reach f<=  seen? and if
        wait2 0> if  1 $114 vcall  $20 zt@ wait2!  0 hold-t!
        else  5 $114 vcall  $18 zt@ hold-t!  then  false exit
    then
    hold-t 0>  d-fiona 100e f< 0= or if  hold-t 1- hold-t!  false exit  then
    strike-or-wait? ;
: chase-holding ( -- done? )   \ action 5: holding back
    0 $16EF pu-c!
    d-fiona close-reach f<=  seen? and  wait2 0> and if
        1 $114 vcall  $20 zt@ wait2!  0 hold-t!  false exit
    then
    hold-t 0> if
        d-fiona 100e f<= if  hold-t 1- hold-t!  else  6 $114 vcall  $1C zt@ hold-t!  then  false exit
    then
    strike-or-wait? ;
: chase-step-over ( -- )   \ 0x10 0x12 0xB 3 0x1C: a step over, back to closing in
    $16EF pu-c@ 1 = if  1 $114 vcall  0 $16EF pu-c!  0 step-done!
    else step-done? if  1 $114 vcall  0 step-done!  then then ;
: chase-gesture ( -- )   \ 0x19 / 0x1A: a gesture - far off (100) she's left: circling
    $175C pu-l@ $1A =  p-104 8 * $1720 pu-l@ + 4 + exe-c@ 0<> or  d-fiona 100e f<= 0= and if
        6 $114 vcall  $1C zt@ hold-t!  0 step-done!  0 $16EF pu-c!
    then
    step-done? $16EF pu-c@ 1 = or if  1 $114 vcall  0 step-done!  0 $16EF pu-c!  then ;
: chase-near ( -- )   \ 0x1D: beside her - she moves off, or no room beside her: closing in
    d-fiona close-reach f<= 0=
    3.1415927e 2e target-side-walkable? 0= 1.9198622e 2e target-side-walkable? 0= and
    -1.9198622e 2e target-side-walkable? 0= and  or if  1 $114 vcall  then
    d-fiona 10e f<  d-fiona f0> and  her c-mode 0= and  her character char.sub sl@ 0<> and if
        not-yet" Debilitas_Chase 0x1D: PursuerGroup_Find, action 0x1000"
    then ;
: debilitas-chase ( -- )
    p-state run
    $175C pu-l@ case
        $1A of  chase-gesture  endof   $19 of  chase-gesture  endof
        $10 of  chase-step-over  endof  $12 of  chase-step-over  endof  $B of  chase-step-over  endof
        3 of  chase-step-over  endof   $1C of  chase-step-over  endof
        $1000 of  0 $1544 pu-c!
            $16EF pu-c@ 1 = if  [: $288 vcall ;] behaviour!  0 $16EF pu-c!
            else step-done? if  1 $114 vcall  0 step-done!  then then  endof
        $1D of  chase-near  endof
        1 of  chase-closing if  exit  then  endof
        6 of  chase-circling if  exit  then  endof
        5 of  chase-holding if  exit  then  endof
    endcase
    step-next? $175C pu-l@ $12 <> and if
        hide-door dup -1 <> if  p-who!  -1 p-104!  $12 $114 vcall  else  drop  then
    then
    p-sub 6 =  her c-mode 3 <> and  d-fiona f0> and if  1 $114 vcall  then
    step-next? 0= if  exit  then
    $1544 pu-c@ 0= if  $B0 vcall  [: $278 vcall ;] behaviour!  0 hold-t!  exit  then
    panic @ 5 = if  [: $264 vcall ;] behaviour!  0 hold-t!  exit  then
    p-target c-mode 3 = if  me p-target c-pos c-dist-to  else  d-fiona  then     ( F: near )
    fdup $2F4 vcall f< 0=  fdup f0< or if  fdrop exit  then
    fdup ground-gained f<  fdup 10e f< or  fdrop if
        me p-target c-pos c-heading-to p-yaw f- angle-wrap fabs
        3.1415927e $2EC vcall f* 180e f/ f<  p-target same-floor? and if
            fiona-state $130 vcall  pick-from-table  0 hold-t!
        then
    then ;

\ ---- Debilitas_Behaviour: starting the chase - by how she faces him and how near she is:
\ hold back (5) or circle (6) for his table's frames, or close in (1); an action asked for
\ (+0x1758, 0x1C with a rumble) ----
: debilitas-behaviour ( -- )
    $16B4 pu-c@ 1 = if  [: $264 vcall ;] behaviour!  $264 vcall exit  then
    her p-target!  1 $16F6 pu-c!
    $1544 pu-c@ 0= if  $B0 vcall  [: $278 vcall ;] behaviour!  exit  then
    $1758 pu-l@ dup -1 = if  drop
        100e rnd01 f*
        seen? 0= if
            d-fiona close-reach f<= 0= if
                $24 zt-f@ f<= if  5 $114 vcall  0 zt@  else  6 $114 vcall  4 zt@  then
            else  8 zt@ s>f f<= if  5 $114 vcall  8 zt@  else  6 $114 vcall  $C zt@  then  then
        else d-fiona close-reach f<= 0= if
            $2C zt-f@ f<= if  5 $114 vcall  $10 zt@  else  6 $114 vcall  $14 zt@  then
        else  fdrop  1 $114 vcall  0  then then  hold-t!
    else dup -2 <> if  $114 vcall  else  drop  then then
    0 $16ED pu-c!  0 step-done!  0 $16EF pu-c!  -1 $1758 pu-l!  0 $1780 pu-l!
    $20 zt@ wait2!
    ['] debilitas-chase behaviour!  debilitas-chase ;

\ ---- Debilitas_ChaseDecision (vtable +0x264): his own chase (the Pursuer's when he holds back
\ +0x16B4, or in the panic's stage 5) ----
: chase-decision ( -- )
    $16B4 pu-c@  panic @ 5 = or if  not-yet" Pursuer_ChaseDecision"  exit  then
    $1758 pu-l@  ['] debilitas-behaviour behaviour!
    dup -2 = if  0 $16F3 pu-c!  -2 $1758 pu-l!
    else $16F3 pu-c@ 1 = if  0 $16F3 pu-c!  $1C $118 vcall
    else dup -1 <> if  dup $118 vcall  then then then  drop
    debilitas-behaviour ;
' chase-decision $264 vt!

\ ---- Pursuer_ChaseFiona (vtable +0x278): action 4 (to her place), Debilitas_BehaviourRun ----
: chase-fiona ( -- )
    4 begin-action  her p-target!  fresh
    [: $27C vcall ;] behaviour!  $27C vcall ;
' chase-fiona $278 vt!

\ ---- Pursuer_BehaviourRun: his state, then by how his action went: walking to her place (4:
\ she's gone - searching; there - his table for it), a table's step (40 / 41), a step round a
\ door or the like (0x1C 11 13 18), a gesture over (2 9 23), seen her (16). Then: seeing her,
\ after her (or his chase when he's there); his steps over, the next; or the reaction ----
: run-walk-to ( -- )   \ 4
    $16EF pu-c@ 1 = if
        2 mode!  2 $2C0 vcall  2 $16C9 pu-c!  3 $16CA pu-c!  search-room  1 $16ED pu-c!  0 $16EF pu-c!
    else d-path 10e f<  d-path f0< 0= and if
        0 step-done!
        mode 0= if  fiona.core:f-2a @ if  0 $1660 pu-l!  then
        else  $16C9 pu-c@ 3 <> if  2 $16C9 pu-c!  then  -1 $179C pu-l!
            $16CA pu-c@ 4 <> if  3 $16CA pu-c!  then  then
        $10 $130 vcall  pick-from-table
    then then ;
: run-table-step ( -- )   \ 40 / 41
    step-done? 0= if  exit  then
    0 step-done!
    $16C9 pu-c@ dup 4 = over 1 = or swap 5 = or if
        $179C pu-l@ dup -1 <> if  dup $15A4 pu-l!  tri-center $15B0 pu vec!  else  drop  then
        4 $114 vcall
    else
        $175C pu-l@ $28 = if  $E  else  $F  then  $130 vcall  4 $114 vcall  pick-from-table
    then ;
: run-step-over ( -- )   \ 0x1C 11 13 18
    $16EF pu-c@ 1 = if  4 $114 vcall  0 $16EF pu-c!
    else step-done? if  4 $114 vcall  0 step-done!  then then ;
: run-gesture-over ( -- )   \ 2 9 23
    step-done? if
        mode if  1 $16ED pu-c!  else  -1 $B4 vcall  4 $114 vcall  then  0 step-done!
    then ;
: behaviour-run ( -- )
    p-state run
    $175C pu-l@ case
        4 of  run-walk-to  endof
        40 of  run-table-step  exit  endof   41 of  run-table-step  exit  endof
        12 of  step-done? if  0 step-done!  4 $114 vcall  then  endof
        28 of  $1544 pu-c@ if  1 step-next!  then  run-step-over  endof
        11 of  run-step-over  endof   13 of  run-step-over  endof   18 of  run-step-over  endof
        2 of  run-gesture-over  endof   9 of  run-gesture-over  endof   23 of  run-gesture-over  endof
        16 of  step-done? if  1 step-next!  1 $1544 pu-c!  0 step-done!  then  endof
    endcase
    step-next? 0= if  exit  then
    $1544 pu-c@ 1 = if
        reached-room? if  [: $264 vcall ;]  else  [: $280 vcall ;]  then  behaviour!
    else $16ED pu-c@ 1 = if  $13C vcall
    else $16F3 pu-c@ 1 = if
        reached-room? if  $1C $114 vcall  else  [: $280 vcall ;] behaviour!  $1C $118 vcall  then
        0 $16F3 pu-c!  exit
    then then then
    $16F3 pu-c@ 1 = if  $1C $118 vcall  0 $16F3 pu-c!  then ;
\ Debilitas_BehaviourRun (vtable +0x27C): walking to her place, his walk by how near she is
: debilitas-behaviour-run ( -- )
    behaviour-run
    $175C pu-l@ 4 <>  p-faded? 0= or if  exit  then
    $16B4 pu-c@ 1 = if  $1788 pu-l@ $201 <> if  $328 vcall 0 play-anim-if drop  then
    else mode 0=  d-fiona $17E8 pu-f@ f< and if  $1788 pu-l@ $200 <> if  $324 vcall 0 play-anim-if drop  then
    else $1788 pu-l@ $201 <> if  $328 vcall 0 play-anim-if drop  then then then ;
' debilitas-behaviour-run $27C vt!
