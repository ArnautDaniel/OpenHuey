\ stalker/head.fs - where a stalker's head looks (src/game/pursuer.c Pursuer_HeadLook,
\ Pursuer_LookAt; Motion_LookAt, Motion_EaseTilt, Model_EaseTilt, HumanModel_AdjustBone): by what
\ he is doing - each action's look mode, the action table's (kPursuerSteps +0x19) - at Fiona
\ while he sees her, at Hewie while he sees him (5 above him), at his goal while it's in view
\ within 30 degrees of his heading; else eased back ahead. It turns at his fast rate (8 degrees
\ a frame) and his sight looks where it does. The head bone takes the tilt and half the turn,
\ the bone below it the other half.
IN: stalker.head
USING: engine game-state actors common stalker.state stalker.senses stalker.moving ;

\ ---- the look mode of each action (kPursuerSteps +0x19, actions 0..$29): set as it starts ----
create look-modes
    0 c, 1 c, 0 c, 1 c, 3 c, 1 c, 1 c, 2 c, 3 c, 0 c, 4 c, 4 c, 4 c, 4 c, 3 c, 0 c,    \ 0..$F
    1 c, 4 c, 3 c, 4 c, 4 c, 4 c, 4 c, 4 c, 4 c, 4 c, 1 c, 1 c, 3 c, 1 c, 4 c, 4 c,    \ $10..$1F
    4 c, 4 c, 4 c, 4 c, 4 c, 3 c, 3 c, 3 c, 3 c, 3 c,                                  \ $20..$29
: action-look! ( action -- )   \ (Pursuer_StartAction: an action's look; 0 - as it was)
    dup 0 $2A within 0= if  drop exit  then  look-modes + c@  ?dup if  look-mode !  then ;

\ ---- his bones by his model (the models' Loaded: +0x8B0 / +0x8B4) ----
: head-bones! ( kind -- )
    case
        2 of  $16 $F  endof   6 of  $16 $F  endof   7 of  $16 $F  endof   27 of  $16 $F  endof   \ DebilitasModel
        3 of  $1F $15  endof  34 of  $1F $15  endof  35 of  $1F $15  endof  36 of  $1F $15  endof  \ DaniellaModel
        4 of  $23 $1A  endof                                                                     \ RiccardoModel
        10 of  $2B $22  endof  39 of  $2B $22  endof                                             \ Lorenzo2Model
        12 of  $13 $B  endof                                                                     \ Kind12Model
        >r -1 -1 r>                                                       \ (Lorenzo's, the tinted ones': none)
    endcase  neck-bone !  head-bone ! ;

\ ---- Motion_LookAt: from his eye (16 up) the tilt (rise over the level distance) and the turn
\ from his heading; Motion_EaseTilt: each toward its target by at most the step ----
16e fconstant eye-up
create eye 12 allot
: look-at ( v -- ) ( F: -- pitch turn )
    my-at eye 12 move  eye 4 + dup sf@ eye-up f+ sf!
    dup 4 + sf@ eye 4 + sf@ f-  dup eye vec-dist-xz fatan2
    eye swap vec-heading yaw f- angle-wrap ;
: rate ( F: -- r )  0.13962634e ;   \ (vtable +0xA4: 8 degrees a frame - the family's and Debilitas's)
fvariable e-to
: ease ( addr -- ) ( F: target step -- )
    fabs  fswap e-to f!                                                ( F: step )
    dup f@ e-to f@ f- fabs fover f<= if  fdrop e-to f@ f! exit  then
    dup f@ e-to f@ f> if  dup f@ fswap f- f!  else  dup f@ f+ f!  then ;
: look-ahead ( -- )  0e rate head-pitch ease  0e rate head-yaw ease ;      \ (Model_EaseTilt)
: turn-only ( v -- )   \ (Pursuer_LookAt: the turn alone, small ones none; the tilt left)
    look-at  fswap fdrop  fdup fabs 0.05e f< if  fdrop 0e  then  rate head-yaw ease ;
create hewie-pt 12 allot
: at-hewie ( -- )   \ (5 above him: the tilt too)
    hewie body-at hewie-pt 12 move  hewie-pt 4 + dup sf@ 5e f+ sf!
    hewie-pt look-at  rate head-yaw ease  rate head-pitch ease ;
: goal-in-view? ( -- flag )   \ (his goal on the triangle a walk there reaches, in his sight, within 30 degrees)
    goal-tri @ dup 0< if  drop false exit  then
    self body-tri my-at goal-at $40080 v-walk <> if  false exit  then
    goal-at in-view? ;
: near-ahead? ( -- flag )  goal-at heading-to yaw f- angle-wrap fabs 0.5235988e f< ;

\ ---- each frame (Pursuer_HeadLook), then his sight's heading, then the bones ----
: head-look ( -- )
    look-mode @ case
        0 of  exit  endof
        1 of  sees-fiona @ if  fiona body-at turn-only  else  look-ahead  then  endof
        2 of  sees-hewie @ if  at-hewie  else  look-ahead  then  endof
        3 of  goal-in-view? if  near-ahead? if  goal-at turn-only  then  else  look-ahead  then  endof
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
