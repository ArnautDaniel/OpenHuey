\ pursuer/react.fs - the stalker reacting to what drew him (src/game/pursuer.c
\ Pursuer_ReactToEvent, action 0x1C): Fiona seen (7: 0x1601, looking at her), lost (4: 0x1602,
\ his search's worth), something about (1 / 5 / 6: he stands looking at where it is, his growl
\ 0x21, for 90 frames - Pursuer_StepCount)
IN: pursuer.react
USING: engine game-state events.core events.words chars relations pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps pursuer.behave pursuer.moves ;

\ Pursuer_StepCount (vtable +0x240): his root motion, +0x104 frames
: step-count ( -- )  root-move-masked  p-104 1- p-104!  p-104 0<= if  1 step-done!  then ;
' step-count $240 vt!
: end-or-move ( -- )
    p-ended? if  1 step-done!  1 step-next!  else  root-move-masked  then ;
: react-to-event ( -- )
    0 $16EC pu-c!
    $16CA pu-c@ dup 4 <> swap 7 <> and 1 and step-next!
    walk-on? if  exit  then
    0 $1784 pu-l!
    $16CA pu-c@ case
        7 of  $1601 play-anim  1 $1710 pu-c!  endof
        4 of  $1602 play-anim
              0 $16C9 pu-c!  0 $16CB pu-c!  0 $16CA pu-c!  0 $16CC pu-c!  -1 $179C pu-l!
              0 $16F1 pu-c!  0 $16F3 pu-c!  0 $16F2 pu-c!  0 $16F4 pu-c!  3 $16CA pu-c!  2 $16C9 pu-c!  endof
        dup 1 = over 5 = or over 6 = or if
            0 play-anim  5 $1710 pu-c!
            $1594 pu-l@ p-room = if  $1700 pu $15B0 pu vec-copy
            else  not-yet" ReactToEvent: the exit's point (rooms +0x30 / +0x3C)"  then
            90 p-104!  $21 7 0 0 p-sound
            [: $240 vcall ;] behave  $240 vcall  drop exit
        then
        1 step-done!  drop exit
    endcase
    ['] end-or-move behave  end-or-move ;
' react-to-event $23C vt!
