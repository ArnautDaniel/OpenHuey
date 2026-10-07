\ pursuer/steps.fs - generated (scratchpad tools/steps.py): the actions (kPursuerSteps: their
\ state, id, move mode and sub, sense mode +0x15C0, look +0x1710) and the off-screen moves.
\ A state not ported yet is a defer saying so; a port fills it: `' w is st.Pursuer_X`.
IN: pursuer.steps
USING: engine chars pursuer.core pursuer.stubs ;

defer st.Pursuer_AttackNextStep  :noname  s" Pursuer_AttackNextStep" (not-yet) ; is st.Pursuer_AttackNextStep
defer st.Pursuer_StateDoorAhead  :noname  s" Pursuer_StateDoorAhead" (not-yet) ; is st.Pursuer_StateDoorAhead
defer st.Pursuer_StateFaceFiona  :noname  s" Pursuer_StateFaceFiona" (not-yet) ; is st.Pursuer_StateFaceFiona
defer st.Pursuer_StateFlinch  :noname  s" Pursuer_StateFlinch" (not-yet) ; is st.Pursuer_StateFlinch
defer st.Pursuer_StateHewieBites  :noname  s" Pursuer_StateHewieBites" (not-yet) ; is st.Pursuer_StateHewieBites
defer st.Pursuer_StateHitAtDoor  :noname  s" Pursuer_StateHitAtDoor" (not-yet) ; is st.Pursuer_StateHitAtDoor
defer st.Pursuer_StateHurt  :noname  s" Pursuer_StateHurt" (not-yet) ; is st.Pursuer_StateHurt
defer st.Pursuer_StateKnockedDown  :noname  s" Pursuer_StateKnockedDown" (not-yet) ; is st.Pursuer_StateKnockedDown
defer st.Pursuer_StateLookAround  :noname  s" Pursuer_StateLookAround" (not-yet) ; is st.Pursuer_StateLookAround
defer st.Pursuer_StateSidestepRoom  :noname  s" Pursuer_StateSidestepRoom" (not-yet) ; is st.Pursuer_StateSidestepRoom
defer st.Pursuer_StateSpecialAnim  :noname  s" Pursuer_StateSpecialAnim" (not-yet) ; is st.Pursuer_StateSpecialAnim
defer st.Pursuer_StateStand  :noname  s" Pursuer_StateStand" (not-yet) ; is st.Pursuer_StateStand
defer st.Pursuer_StateWalkGesture  :noname  s" Pursuer_StateWalkGesture" (not-yet) ; is st.Pursuer_StateWalkGesture
defer st.Pursuer_StateWalkOn  :noname  s" Pursuer_StateWalkOn" (not-yet) ; is st.Pursuer_StateWalkOn
defer st.Pursuer_StateWalkThen404  :noname  s" Pursuer_StateWalkThen404" (not-yet) ; is st.Pursuer_StateWalkThen404
defer st.Pursuer_StateWalkThenAnim  :noname  s" Pursuer_StateWalkThenAnim" (not-yet) ; is st.Pursuer_StateWalkThenAnim

: action! ( xt id mode sub sense look i -- )  /action * actions + >r
    r@ 5 cells + !  r@ 4 cells + !  r@ 3 cells + !  r@ 2 cells + !  r@ cell+ !  r> ! ;
' st.Pursuer_StateStand  $0 $0 $0 $FF $0  $0 action!   \ Pursuer_StateStand
:noname $17C vcall ;  $1 $0 $0 $FF $1  $1 action!   \ Pursuer_Move17C
' st.Pursuer_StateWalkThenAnim  $2 $0 $0 $FF $0  $2 action!   \ Pursuer_StateWalkThenAnim
:noname $184 vcall ;  $3 $0 $0 $FF $1  $3 action!   \ Pursuer_Move184
:noname $194 vcall ;  $4 $0 $1 $3 $3  $4 action!   \ Pursuer_WalkToGoal
:noname $1A0 vcall ;  $5 $0 $1 $0 $1  $5 action!   \ Pursuer_Chase1A0
:noname $1A4 vcall ;  $6 $0 $2 $0 $1  $6 action!   \ Pursuer_Chase1A4
:noname $1A4 vcall ;  $7 $0 $2 $1 $2  $7 action!   \ Pursuer_Chase1A4
:noname $238 vcall ;  $8 $0 $1 $3 $3  $8 action!   \ Pursuer_SearchRoute
:noname $230 vcall ;  $9 $0 $0 $FF $0  $9 action!   \ Pursuer_WalkAside
:noname $1C8 vcall ;  $A $0 $1 $2 $4  $A action!   \ Pursuer_Door1C8
:noname $1CC vcall ;  $B $0 $1 $2 $4  $B action!   \ Pursuer_DoorWalkTo
:noname $1E4 vcall ;  $C $2 $14 $FF $4  $C action!   \ Pursuer_Door1E4
:noname $1F0 vcall ;  $D $0 $1 $2 $4  $D action!   \ Pursuer_Exit1F0
:noname $1F4 vcall ;  $E $0 $2 $3 $3  $E action!   \ Pursuer_ExitGoTo
:noname $200 vcall ;  $F $0 $0 $FF $0  $F action!   \ Debilitas_ExitDone
:noname $204 vcall ;  $10 $0 $1 $FF $1  $10 action!   \ Pursuer_DoorBackAway
:noname $210 vcall ;  $11 $4 $A $FF $4  $11 action!   \ Pursuer_DoorWalk
:noname $22C vcall ;  $12 $0 $6 $2 $3  $12 action!   \ Pursuer_DoorApproach
' st.Pursuer_AttackNextStep  $13 $8 $0 $FF $4  $13 action!   \ Pursuer_AttackNextStep
' st.Pursuer_StateFaceFiona  $14 $8 $18 $FF $4  $14 action!   \ Pursuer_StateFaceFiona
' st.Pursuer_StateWalkOn  $15 $8 $18 $FF $4  $15 action!   \ Pursuer_StateWalkOn
' st.Pursuer_StateLookAround  $16 $8 $2 $FF $4  $16 action!   \ Pursuer_StateLookAround
' st.Pursuer_StateWalkGesture  $17 $0 $0 $FF $4  $17 action!   \ Pursuer_StateWalkGesture
' st.Pursuer_StateWalkGesture  $18 $0 $0 $FF $4  $18 action!   \ Pursuer_StateWalkGesture
' st.Pursuer_StateWalkGesture  $19 $0 $0 $FF $4  $19 action!   \ Pursuer_StateWalkGesture
' st.Pursuer_StateSidestepRoom  $1A $0 $1 $3 $1  $1A action!   \ Pursuer_StateSidestepRoom
' st.Pursuer_StateSidestepRoom  $1B $0 $1 $3 $1  $1B action!   \ Pursuer_StateSidestepRoom
:noname $23C vcall ;  $1C $0 $0 $FF $3  $1C action!   \ Pursuer_ReactToEvent
' st.Pursuer_StateWalkThen404  $1D $0 $1 $FF $1  $1D action!   \ Pursuer_StateWalkThen404
' st.Pursuer_StateFlinch  $1E $4 $0 $FF $4  $1E action!   \ Pursuer_StateFlinch
' st.Pursuer_StateKnockedDown  $1F $4 $A $FF $4  $1F action!   \ Pursuer_StateKnockedDown
' st.Pursuer_StateHurt  $20 $4 $A $FF $4  $20 action!   \ Pursuer_StateHurt
' st.Pursuer_StateHitAtDoor  $21 $4 $D $FF $4  $21 action!   \ Pursuer_StateHitAtDoor
' st.Pursuer_StateDoorAhead  $22 $4 $A $FF $4  $22 action!   \ Pursuer_StateDoorAhead
' st.Pursuer_StateHewieBites  $23 $4 $9 $FF $4  $23 action!   \ Pursuer_StateHewieBites
' st.Pursuer_StateSpecialAnim  $24 $4 $11 $FF $4  $24 action!   \ Pursuer_StateSpecialAnim
:noname $244 vcall ;  $25 $0 $2 $3 $3  $25 action!   \ Pursuer_FollowPathExit
:noname $250 vcall ;  $26 $0 $2 $3 $3  $26 action!   \ Pursuer_OnToNextExit
:noname $254 vcall ;  $27 $0 $2 $3 $3  $27 action!   \ Pursuer_NextPathExit
:noname $258 vcall ;  $28 $0 $2 $3 $3  $28 action!   \ Pursuer_GoToDoor
:noname $258 vcall ;  $29 $0 $2 $3 $3  $29 action!   \ Pursuer_GoToDoor

\ the off-screen moves (kPursuerMove ..: a PTMF, an id +0x17AC, move mode 6, a sub)
create moves 6 2 * cells allot
:noname  $158 vcall ;  moves 0 2 * cells + !  $17 moves 0 2 * 1+ cells + !   \ kPursuerMove: Pursuer_PlanWayOn
:noname  $15C vcall ;  moves 1 2 * cells + !  $17 moves 1 2 * 1+ cells + !   \ kPursuerWaitMove: Pursuer_PlanToGoal
:noname  $164 vcall ;  moves 2 2 * cells + !  $16 moves 2 2 * 1+ cells + !   \ kPursuerMoveA: Pursuer_Pace
:noname  $160 vcall ;  moves 3 2 * cells + !  $17 moves 3 2 * 1+ cells + !   \ kPursuerStairsMove: Pursuer_FollowPlan
:noname  $16C vcall ;  moves 4 2 * cells + !  $17 moves 4 2 * 1+ cells + !   \ kPursuerIdleMove: Pursuer_Plan16C
:noname  $174 vcall ;  moves 5 2 * cells + !  $17 moves 5 2 * 1+ cells + !   \ kPursuerMoveB: Pursuer_KnockAtDoor
0 constant pmv-plan  1 constant pmv-wait  2 constant pmv-a  3 constant pmv-stairs  4 constant pmv-idle  5 constant pmv-b
\ Pursuer_SetMove
: set-move ( k -- )
    dup 2 * cells moves + @ p-move !  dup $17AC pu-l!  6 p-mode!  2 * 1+ cells moves + @ p-sub!
    0 $1530 pu-l!  0 $1538 pu-l!  0 $1534 pu-l! ;
