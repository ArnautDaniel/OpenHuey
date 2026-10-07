\ pursuer/debilitas.fs - Debilitas (src/game/debilitas.c, debilitas_body.inc: kind 2, the
\ stalker of the first chapters): his setup and the Pursuer functions he overrides, over the
\ Pursuer's own (Pursuer_Setup, NPC_Reset / Pursuer_Reset, Pursuer_Activate).
IN: pursuer.debilitas
USING: engine game-state events.core events.words chars relations fiona.doors pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps pursuer.behave pursuer.frame pursuer.debact ;

\ the hard setting (progress +0x30 bit 0x8000: his stats and files differ)

: alt? ( -- flag )  hard-mode @ 0<> ;

\ ---- his constants (vtable +0xA0 .. +0x2FC) ----
:noname ( F: -- r )  0.052359879e ; $A0 vt!    \ Debilitas_TurnRate: 3 degrees
:noname ( F: -- r )  0.13962634e ; $A4 vt!     \ Debilitas_TurnRateFast: 8 degrees
:noname ( -- mask )  $2C020068 ; $A8 vt!       \ Debilitas_BlockFlags
:noname ( F: -- r )  24e ; $2F4 vt!            \ Debilitas_AttackRange
:noname ( F: -- r )  16e ; $2F0 vt!            \ Debilitas_ReachHewie
:noname ( F: -- r )  20e ; $2EC vt!            \ Debilitas_AttackAngle
:noname ( F: -- r )  10e ; $2E8 vt!            \ Debilitas_Dist2E8
:noname ( F: -- r )  20e ; $2E4 vt!            \ Debilitas_ReachFiona
:noname ( F: -- r )  0.12e ; $2E0 vt!          \ Debilitas_LookSwing
:noname ( F: -- r )  60e ; $2DC vt!            \ Debilitas_LookFrames
:noname ( F: -- r )  1.4e ; $2F8 vt!           \ Debilitas_SpeedTop
:noname ( F: -- r )  0.6e ; $2FC vt!           \ Debilitas_SpeedBase
:noname ( -- n )  $D ; $30C vt!                \ Debilitas_AttackAnimA
:noname ( -- n )  $E ; $310 vt!                \ Debilitas_AttackAnimB
:noname ( -- )  1 step-done! ; $200 vt!        \ Debilitas_ExitDone
\ Debilitas_DoorOffset (+0x9C): where he stands at a door, by its animation kind
: door-offset ( kind out -- )
    swap case
        0 of  -6.2115e  endof  1 of  10.4e  endof  2 of  6.2885e  endof  3 of  -7.4404e  endof
        >r drop r> exit
    endcase  >r 0e 0e frot r> vec! ;
' door-offset $9C vt!
\ Debilitas_ActionOffsets (+0x2D8): where he stands for an action, by its kind
: action-offsets ( kind out -- )
    swap case
        10 of  0.9433e 11.2509e  endof  11 of  0.9433e 11.2509e  endof
        12 of  2.1745e 15.0419e  endof  13 of  2.1745e 15.0419e  endof
        14 of  -1.6117e -4.3418e  endof  15 of  0.0182e -1.0008e  endof
        >r drop r> exit
    endcase  >r 0e fswap r> vec! ;
' action-offsets $2D8 vt!
\ the walk / stand animations (+0x320 the stand, +0x324 the walk, +0x328 the slow walk)
: stand-anim ( -- anim )  p-cond 1 <> if  $16B8 pu-l@ 2 = if  3  else  0  then  else  2  then ;
: walk-anim ( -- anim )  p-cond 1 <> if  $16B8 pu-l@ 2 = if  $204  else  $200  then  else  $202  then ;
: slow-walk-anim ( -- anim )   \ Debilitas_SlowWalkAnim over Pursuer_SlowWalkAnim
    mode 4 = if  $203 exit  then
    p-cond 1 = if  $203 exit  then
    $16B8 pu-l@ 2 = if  $205 exit  then
    mode if  $201  else  $206  then ;
' stand-anim $320 vt!   ' walk-anim $324 vt!   ' slow-walk-anim $328 vt!
\ Debilitas_StandAnim (vtable +0x128): his walk for his mode - slow after her (the way to her
\ shorter than +0x17E8, she not hiding) or waiting with nothing drawing him; else the normal
: debilitas-walk ( -- )
    mode case
        0 of  d-path $17E8 pu-f@ f<=  d-fiona f0< 0= and  her c-mode 3 <> and  endof
        1 of  false  endof  2 of  false  endof  4 of  false  endof
        3 of  $16C9 pu-c@ dup 0= swap 2 = or  endof
        >r drop r> exit
    endcase
    if  $324  else  $328  then  vcall 0 play-anim-if drop ;
' debilitas-walk $128 vt!

\ ---- Pursuer_Setup + Debilitas_Setup (vtable +0xF4): his stats and tables ----
: setup ( -- )
    \ (Pursuer_Setup)
    me 5e 20e c-size!  100 p-hp-max!  p-hp-max p-hp!  12 p-hear!
    0 $16B4 pu-c!  0 own-steps !  0 $16AC pu-l!  0 $16B0 pu-l!  0 $171C pu-l!  0 $1720 pu-l!
    0 $1724 pu-l!  0 $1734 pu-l!  0 $1730 pu-l!  0 $1740 pu-l!  0 $173C pu-l!  $3EC8E0 $1744 pu-l!
    0 $1748 pu-l!  150e $1580 pu-f!  1.0471976e $1584 pu-f!
    100 $16DC pu-l!  10e $16E8 pu-f!  300 $16D4 pu-l!  1800 $16D8 pu-l!  1800 $16D0 pu-l!
    9000 $16E0 pu-l!  150 $16E4 pu-l!
    \ (Debilitas_Setup)
    alt? if  110  else  70  then  p-hp-max!
    $3AF4D0 $171C pu-l!  $3AFA90 $1730 pu-l!  $3AFAF0 $1740 pu-l!  $3AFB10 $173C pu-l!
    $3AFB58 $1748 pu-l!  $3AFB70 $17F0 pu-l!   \ (str_Z_2: his chase table)
    20 $16DC pu-l!  10e $16E8 pu-f!                   \ (Hewie's bites he takes)
    alt? if  540  else  300  then  $16D4 pu-l!  1800 $16D8 pu-l!
    alt? if  1350  else  1800  then  $16D0 pu-l!  4500 $16E0 pu-l!  90 $16E4 pu-l!
    alt? if  40e  else  50e  then  $17E4 pu-f!
    80e $17E8 pu-f!                                     \ (the slow walk when the way to her is shorter)
    me 5e 20e c-size!  p-hp-max p-hp!  0 p-hear!  0 $16B4 pu-c!
    deb-actions own-steps !
    $3AF6F0 $1720 pu-l!  $3AF730 $1724 pu-l!  $3AF340 $16AC pu-l!  $3AF4A0 $16B0 pu-l!
    $47A900 $1734 pu-l!  0 $17EC pu-l!
    8e $1694 pu-f!  1.5e $169C pu-f!  12e $1698 pu-f!  1.5e $16A0 pu-f! ;
' setup $F4 vt!

\ ---- NPC_Reset + Pursuer_Reset (vtable +0xC): as he is loaded ----
: npc-reset ( -- )
    0 p-cond!  char-reset  $A8 vcall me c-mask!
    $148C pu 13 4 * 0 fill  $FF p-door!  $FFFF $14C0 pu-w!  0 $14C4 pu-l!
    -1 $15A4 pu-l!  -1 $15C4 pu-l!  -1 $1594 pu-l!  -1 $1598 pu-l!  -1 $159C pu-l!  $FF $15C0 pu-c!
    p-yaw $1570 pu-f!  0 $1568 pu-l!  0 $1544 pu-c!  0 $1545 pu-c!  0 $1546 pu-c! ;
: reset ( -- )
    npc-reset
    0 $16C9 pu-c!  0 $16CB pu-c!  0 $16CA pu-c!  0 $16CC pu-c!  -1 $179C pu-l!
    0 $16F1 pu-c!  0 $16F3 pu-c!  0 $16F2 pu-c!  0 $16F4 pu-c!
    route-clear  0 $1794 pu-l!
    behaviour-clear
    0 $1784 pu-l!  0 $1780 pu-l!  0 $1664 pu-l!  0 $1790 pu-l!  0 $17B4 pu-l!  0 $1794 pu-l!  0 $1798 pu-l!
    mode case
        0 of  $2BC vcall  endof  1 of  $2C0 vcall  endof  2 of  $2C0 vcall  endof
        3 of  $2C4 vcall  endof  4 of  0 $2C8 vcall  endof
    endcase
    0 $16A4 pu-l!  0 $16BC pu-l!  0 $16C0 pu-l!  0 $16C4 pu-l!  -1 $1788 pu-l!
    2 mode!  $2C0 vcall
    her p-target!  0 $16B8 pu-l!  $FF $17B0 pu-c!
    8 $114 vcall  pmv-wait set-move
    [: $25C vcall ;] behaviour!
    0 $1718 pu-l!
    $F4 vcall ;
' reset $C vt!

\ ---- Pursuer_Activate (vtable +0x5C): coming into the room ----
: activate ( -- )
    $A8 vcall me c-mask!
    0 play-table
    $FF $17B0 pu-c!  0 $1664 pu-l!  0 $1544 pu-c!  0 $1545 pu-c!
    3 mode!  $2C4 vcall
    0 $16C9 pu-c!  0 $16CB pu-c!  0 $16CA pu-c!  0 $16CC pu-c!  -1 $179C pu-l!
    0 $16F1 pu-c!  0 $16F3 pu-c!  0 $16F2 pu-c!  0 $16F4 pu-c!
    p-hp 0<= if  p-hp-max p-hp!  then
    0 $31C vcall ;
' activate $5C vt!
\ Pursuer_SetRage (+0x31C): +0x16B8 2 / 0
:noname ( on -- )  if  2  else  0  then  $16B8 pu-l! ; $31C vt!
\ ---- his head (Pursuer_HeadLook) by his look (+0x1710): 1 at Fiona, 2 at Hewie (5 above him) -
\ when he sees them; 3 at his goal (+0x15B0) when it's in view and within 30 degrees; 5 at
\ +0x1700; else (and 0 as it was) back ahead. Eased at his head's speed (+0xA4); +0x1574 his
\ sight's heading. His model turns it (HumanModel_AdjustBone): the head (0x16) its pitch and
\ half its turn, the neck (0xF) the other half ----
fvariable head-pitch   \ (his motion's +0x854)
create hl-eye 12 allot  create hl-at 12 allot
: look-angles ( v -- ) ( F: -- pitch turn )   \ Motion_LookAt from his eye (the model's +0x860: 16 up)
    hl-eye p-pos vec-copy  hl-eye 4 + dup sf@ 16e f+ sf!
    dup 4 + sf@  hl-eye 4 + sf@ f-  dup hl-eye swap vec-dist-xz  fatan2
    hl-eye swap vec-heading p-yaw f- angle-wrap ;
fvariable e-v  fvariable e-to  fvariable e-s
: ease ( F: v to step -- v' )   \ (Motion_EaseTilt: by at most step)
    fabs e-s f!  e-to f!  e-v f!
    e-to f@ e-v f@ f- fabs e-s f@ f<= if  e-to f@ exit  then
    e-to f@ e-v f@ f<= if  e-v f@ e-s f@ f-  else  e-v f@ e-s f@ f+  then ;
fvariable he-ty  fvariable he-sp  fvariable he-sy
: head-ease ( F: pitch turn sp-pitch sp-turn -- )
    he-sy f!  he-sp f!  he-ty f!
    head-pitch f@ fswap he-sp f@ ease head-pitch f!
    head-yaw f@ he-ty f@ he-sy f@ ease head-yaw f! ;
: sight-heading ( -- )  p-yaw head-yaw f@ f+ angle-wrap $1574 pu-f! ;
: head-at ( v -- )   \ (Pursuer_LookAt: the turn only, small ones none)
    look-angles fswap fdrop  fdup fabs 0.05e f< if  fdrop 0e  then
    0e fswap 0e $A4 vcall head-ease  sight-heading ;
: look-ahead ( -- )   \ (the motion's +0x5C: back to none at the head's speed)
    0e 0e $A4 vcall fdup head-ease  sight-heading ;
: head-look ( -- )
    $1710 pu-c@ case
        0 of  exit  endof
        1 of  $1544 pu-c@ if  her c-pos head-at exit  then  endof
        2 of  $1545 pu-c@ if
                  hl-at dog c-pos vec-copy  hl-at 4 + dup sf@ 5e f+ sf!
                  hl-at look-angles  $A4 vcall fdup head-ease  sight-heading exit
              then  endof
        3 of  $15B0 pu $40080 v-tri-in $15A4 pu-l@ =  if
                  p-pos $15B0 pu  $1574 pu-f@ $1580 pu-f@ $1584 pu-f@ can-see? if
                      me $15B0 pu c-heading-to p-yaw f- angle-wrap fabs 0.5235988e f< if  $15B0 pu head-at  then
                      exit
                  then
              then  endof
        5 of  $1700 pu head-at exit  endof
    endcase
    look-ahead ;
: head-pose ( -- )
    p-actor dup 0< if  drop exit  then
    dup turns-clear
    head-pitch f@ fnegate  head-yaw f@ 0.5e f*  dup $16 turn+
    0e  head-yaw f@ 0.5e f*  $F turn+ ;
\ his model's frame (Pursuer_ModelUpdate, +0x40): the engine moves his model; his head's look
:noname ( -- )  head-look  head-pose ; $40 vt!
\ Debilitas_TurnRateFast (+0xA4): his head's speed, 8 degrees a frame
:noname ( F: -- r )  0.13962634e ; $A4 vt!
