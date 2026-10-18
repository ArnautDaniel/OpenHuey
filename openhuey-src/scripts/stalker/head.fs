\ stalker/head.fs - where a stalker's head looks (src/game/pursuer.c Pursuer_HeadLook,
\ Pursuer_LookAt; Motion_LookAt, Motion_EaseTilt, Model_EaseTilt, HumanModel_AdjustBone): by what
\ he is doing - each action's look mode, the action table's (kPursuerSteps +0x19) - at Fiona
\ while he sees her, at Hewie while he sees him (5 above him), at his goal while it's in view
\ within 30 degrees of his heading; else eased back ahead. It turns at his fast rate (8 degrees
\ a frame) and his sight looks where it does. The head bone takes the tilt and half the turn,
\ the bone below it the other half.
IN: stalker.head
USING: engine game-state actors common vectors stalker-names stalker.state stalker.senses stalker.moving stalker.names ;

\ ---- the look mode of each action (kPursuerSteps +0x19, actions 0..$29): set as it starts ----
: v, ( mode -- )  c, ;
create look-modes
    as-was-look v, her-look v, as-was-look v, her-look v, goal-look v, her-look v, her-look v, hewie-look v,       \ 0..7
    goal-look v, as-was-look v, ahead-look v, ahead-look v, ahead-look v, ahead-look v, goal-look v, as-was-look v,   \ 8..$F
    her-look v, ahead-look v, goal-look v, ahead-look v, ahead-look v, ahead-look v, ahead-look v, ahead-look v,     \ $10..$17
    ahead-look v, ahead-look v, her-look v, her-look v, goal-look v, her-look v, ahead-look v, ahead-look v,         \ $18..$1F
    ahead-look v, ahead-look v, ahead-look v, ahead-look v, ahead-look v, goal-look v, goal-look v, goal-look v,     \ $20..$27
    goal-look v, goal-look v,                                                                                          \ $28..$29
: action-look! ( action -- )   \ (Pursuer_StartAction: an action's look; as it was - none)
    dup 0 $2A within 0= if  drop exit  then  look-modes + c@  ?dup if  look-mode !  then ;

\ ---- his bones by his model (the models' Loaded: +0x8B0 / +0x8B4): head, neck ----
: head-bones! ( kind -- )
    case
        debilitas-kind of  $16 $F  endof   debilitas-6-kind of  $16 $F  endof        \ DebilitasModel
        debilitas-7-kind of  $16 $F  endof   debilitas-27-kind of  $16 $F  endof
        daniella-kind of  $1F $15  endof  kind34-kind of  $1F $15  endof            \ DaniellaModel
        kind35-kind of  $1F $15  endof  kind36-kind of  $1F $15  endof
        riccardo-kind of  $23 $1A  endof                                             \ RiccardoModel
        lorenzo2-kind of  $2B $22  endof  kind39-kind of  $2B $22  endof            \ Lorenzo2Model
        kind12-kind of  $13 $B  endof                                                \ Kind12Model
        >r -1 -1 r>                                                       \ (Lorenzo's, the tinted ones': none)
    endcase  neck-bone !  head-bone ! ;

\ ---- Motion_LookAt: from his eye (16 up) the tilt (rise over the level distance) and the turn
\ from his heading; Motion_EaseTilt: each toward its target by at most the step ----
16e fconstant eye-up
create eye /vec allot
: look-at ( v -- ) ( F: -- pitch turn )
    {: v :}  eye my-at vec-copy  eye v.y sf@ eye-up f+ eye v.y sf!
    v v.y sf@ eye v.y sf@ f-  v eye vec-dist-xz fatan2
    eye v vec-heading yaw f- angle-wrap ;
: rate ( F: -- r )  8e deg>rad ;   \ (vtable +0xA4: the family's and Debilitas's)
: ease ( addr -- ) ( F: target step -- )
    fabs {: a F: goal step :}
    a f@ goal f- fabs step f<= if  goal a f! exit  then
    a f@ goal f> if  a f@ step f-  else  a f@ step f+  then  a f! ;
: look-ahead ( -- )  head-pitch 0e rate ease  head-yaw 0e rate ease ;      \ (Model_EaseTilt)
: turn-only ( v -- )   \ (Pursuer_LookAt: the turn alone, small ones none; the tilt left)
    look-at  fswap fdrop  fdup fabs 0.05e f< if  fdrop 0e  then  head-yaw rate ease ;
create hewie-pt /vec allot
: at-hewie ( -- )   \ (5 above him: the tilt too)
    hewie-pt hewie body-at vec-copy  hewie-pt v.y sf@ 5e f+ hewie-pt v.y sf!
    hewie-pt look-at {: F: pitch turn :}  head-yaw turn rate ease  head-pitch pitch rate ease ;
: goal-in-view? ( -- flag )   \ (his goal on the triangle a walk there reaches, in his sight, within 30 degrees)
    goal-tri @ dup 0< if  drop false exit  then
    self body-tri my-at goal-at sight-walls v-walk <> if  false exit  then
    goal-at in-view? ;
: near-ahead? ( -- flag )  goal-at heading-to yaw f- angle-wrap fabs 30e deg>rad f< ;

\ ---- each frame (Pursuer_HeadLook), then his sight's heading, then the bones ----
: head-look ( -- )
    look-mode @ case
        as-was-look of  exit  endof
        her-look of  sees-fiona @ if  fiona body-at turn-only  else  look-ahead  then  endof
        hewie-look of  sees-hewie @ if  at-hewie  else  look-ahead  then  endof
        goal-look of  goal-in-view? if  near-ahead? if  goal-at turn-only  then  else  look-ahead  then  endof
        >r look-ahead r>
    endcase ;
: turn-head ( -- )
    head-bone @ 0< if  exit  then
    my-model @ turns-clear
    my-model @ neck-bone @  0e  head-yaw f@ 0.5e f*  turn+
    my-model @ head-bone @  head-pitch f@ fnegate  head-yaw f@ 0.5e f*  turn+ ;
: sight-heading ( -- )  yaw head-yaw f@ f+ angle-wrap view-heading f! ;
: looking ( -- )   \ (each frame, in the room being played: his sight where his head is)
    head-look  sight-heading  turn-head ;
