\ pursuer/behave.fs - the stalker's behaviour plumbing (src/game/pursuer.c): resetting it, back
\ to normal after a script, back to his stance, the idle behaviour, and what his request block
\ asks (Pursuer_EventState: 4 a blow, 5 let go, 12 / 30 a grab's end, 7 held).
IN: pursuer.behave
USING: engine game-state events.core events.words chars relations fiona.doors pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps ;

\ Pursuer_ClearSteps: the steps' working values (+0x1624 .. +0x165C)
: clear-steps ( -- )
    $1624 pu 32 0 fill  0 $1650 pu-l!  0 $1654 pu-l!  0 $1658 pu-l!  0 $165C pu-l! ;
\ the behaviour's flags and values as each reset leaves them
: behaviour-clear ( -- )
    0 $16ED pu-c!  0 step-done!  0 $16EF pu-c!  0 $16F5 pu-c!  0 step-next!  0 $16EC pu-c!
    0 $1710 pu-c!  0 $16F9 pu-c!  0 $16F6 pu-c!  0 $16F8 pu-c!  0 $16F7 pu-c!  1 $15A0 pu-c!
    -1 $1758 pu-l!  0 $1761 pu-c!  0 $1760 pu-c!  0 $178C pu-l!  -1 $1764 pu-l!  0 p-freeze!
    0 $17B4 pu-l!  -1 $1728 pu-l!  0 $172C pu-c!  0 $1738 pu-l!  clear-steps
    0 $1700 pu-l!  0 $1704 pu-l!  0 $1708 pu-l!  0 $1770 pu-l!  0 $1774 pu-l!  0 $1778 pu-l! ;
\ Npc_LeaveDoor (a door he holds: with the doors)
defer leave-door ( exit -- )   ' drop is leave-door
\ Character_ResetBehaviour / Character_BackToNormal / Character_EventReset (the Character's part)
: char-reset ( -- )  p-path-end  0 p-2a!  0 p-2b!  0 p-2d!  0 0 p-req!  0 1 p-req! ;

\ Pursuer_ResetBehaviour (vtable +0x60)
: reset-behaviour ( -- )
    in-played-room? p-mode 2 = and if  $FF leave-door  then
    behaviour-clear  $7C vcall  char-reset ;
' reset-behaviour $60 vt!

\ Pursuer_BackToNormal (vtable +0x8C): after a script (Hewie / Fiona let go if he held them)
: back-to-normal ( -- )
    p-mode 2 = p-who door-held? and if  $FF leave-door  then
    p-sub 9 = if  not-yet" Pursuer_BackToNormal: Hewie let go (his +0x7C)"  then
    p-sub $18 - 2 u< if  not-yet" Pursuer_BackToNormal: Fiona let go (her +0x7C)"  then
    behaviour-clear  -1 p-who!  char-reset ;
' back-to-normal $8C vt!

\ Pursuer_EventReset (vtable +0x90): a script's end - held off: after her if he sees her, else
\ searching; up again (and Fiona told: request 3) if he was down
: event-reset ( -- )
    p-mode 2 = p-who door-held? and if  $FF leave-door  then
    mode 4 = if
        $C0 vcall if  0 mode!  $2BC vcall  6 $16C9 pu-c!  7 $16CA pu-c!
        else  2 mode!  search-room  2 $16C9 pu-c!  3 $16CA pu-c!  then
    then
    1 step-next!  1 $16F6 pu-c!  -1 p-who!  0 $1700 pu-l!  0 $1704 pu-l!  0 $1708 pu-l!
    char-reset
    p-cond 2 = if  0 p-cond!  p-hp-max p-hp!  4 3 0 0 her 0e ask  then ;
' event-reset $90 vt!

\ Pursuer_StateRunThenNext: his state; the step done, on to action 0
: run-then-next ( -- )
    p-state run
    step-done? if  0 step-done!  $175C pu-l@ if  0 $114 vcall  then  then ;

\ Pursuer_BackToStance (vtable +0x7C): action 1, the move for his mode; free: his next step
\ (+0x13C) in her room, the idle behaviour elsewhere; scripted: run-then-next
: back-to-stance ( -- )
    1 $114 vcall
    mode case
        0 of  pmv-plan set-move  endof
        2 of  pmv-idle set-move  endof
        >r pmv-wait set-move r>
    endcase
    p-scripted 0= if
        in-played-room? if  $13C vcall  else  [: $290 vcall ;] behaviour!  then
        1 step-next!
    else  ['] run-then-next behaviour!  then ;
' back-to-stance $7C vt!

defer hit   ' noop is hit   \ Pursuer_Hit (pursuer.hit)
\ Pursuer_EventState (vtable +0x84): his request block
: event-state ( -- )
    0 p-req case
        4 of  in-played-room? if  hit  else  not-yet" Pursuer_HitOffscreen"  then  endof
        5 of
            in-played-room? if
                p-sub 7 <> if  $8C vcall  $7C vcall
                else
                    $1624 pu-l@ $1628 pu-l@  behaviour-clear  $1628 pu-l! $1624 pu-l!
                    not-yet" Character_BackToNormal (stairs)"
                then
                ['] run-then-next behaviour!
            then  0 0 p-req!  0 1 p-req!
        endof
        12 of
            1 p-req 30 = if  [: $29C vcall ;] behaviour!  0 $118 vcall  then
            0 0 p-req!  0 1 p-req!
        endof
        7 of  p-mode 0= if  0 0 p-req!  0 1 p-req!  then  endof
    endcase ;
' event-state $84 vt!

\ Pursuer_BehaviourIdle (vtable +0x290): fresh; the off-screen step (+0x294; the countdown's
\ +0x298)
: behaviour-idle ( -- )
    1 $16F6 pu-c!  0 $16ED pu-c!  0 step-done!  0 $16EF pu-c!  -1 $1758 pu-l!  0 $1780 pu-l!
    countdown? if  [: $298 vcall ;] behaviour!  $298 vcall
    else  [: $294 vcall ;] behaviour!  $294 vcall  then ;
' behaviour-idle $290 vt!
\ Pursuer_ClearBehaviour (vtable +0x2A4): fresh, the give-up behaviour (+0x2A8)
: clear-behaviour ( -- )
    0 $16F6 pu-c!  0 $16ED pu-c!  0 step-done!  0 $16EF pu-c!  0 $1780 pu-l!
    [: $2A8 vcall ;] behaviour!  $2A8 vcall ;
' clear-behaviour $2A4 vt!
