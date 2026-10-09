\ fiona/hurt.fs - Fiona struck or caught (F4; src/game/fiona.c Fiona_Reaction, Fiona_React and
\ the states after: Fiona_StateKnockedDown, Fiona_StateThrown, Fiona_StateCaught). A `hit` from
\ a stalker: what it makes of her by what she is doing - knocked down (a blow; on a step or in a
\ doorway the short falls), thrown (a hard blow: forwards or backwards by where it came from),
\ caught (a hold: she flinches in his grip and pulls free) - the fright it carries, and getting
\ up again. How she stands for an attack (`fiona-plight`) is told each frame.
IN: fiona.hurt
USING: engine game-state actors messages common facts flag-names fiona.state fiona.model fiona.moving fiona.fear ;

defer to-idle ( -- )
: danger-id ( -- id )  s" danger" actor-named ;

\ ---- how she stands for a stalker's attack (Pursuer_FionaState): out of reach while struck
\ (her reactions, mode 4) or seized; the panic's stages 4 / 5; fleeing (mode $B); panicking
\ (fear over 90, or the panic attack) ----
: plight ( -- n )
    her-mode @ 4 =  her-sub @ $10 = or if  3 exit  then
    her-panic-stage @ dup 4 = swap 5 = or if  her-panic-stage @ exit  then
    her-mode @ $B = if  1 exit  then
    her-fear f@ 90e f>  her-doing @ $E = or if  2 exit  then
    0 ;

\ ---- the reaction to a blow of `kind` (Fiona_Reaction): none while one is under way (mode 4,
\ floored $A) or the world is held; on a ladder (mode 3) her fall from it comes with the
\ ladders (F5) ----
: on-step? ( -- flag )   \ (her floor a step or a doorway's: the short falls)
    her-tri @ dup 0< if  drop false exit  then  nav-flags $80003 and 0<> ;
: reaction ( kind -- r | -1 )
    world-held state-flag? if  drop -1 exit  then
    her-mode @ dup 4 = over $A = or swap 3 = or if  drop -1 exit  then
    case
        1 of  on-step? if  $C  else  $E  then  endof
        2 of  on-step? if  $D  else  $F  then  endof
        4 of  on-step? if  $D  else  $A  then  endof
        6 of  her-mode @ 0= her-doing @ $F = and if  -1  else  $20  then  endof   \ (not while out of breath)
        >r -1 r>
    endcase ;

\ ---- where the blow came from: the turn from her heading to whoever struck ----
fvariable side
: striker? ( -- flag )  her-hit-who @ dup 0< if  drop false exit  then  dup alive? if  body?  else  drop false  then ;
: blow-side ( -- )
    striker? if  her-at her-hit-who @ body-at vec-heading her-yaw f@ f- angle-wrap  else  0e  then  side f! ;
\ a fall by side: from in front (within 60 degrees) `front`, from behind (past 120) +1, from
\ her left +3, her right +2
: fall-by-side ( front -- )
    side f@ fabs 60e deg>rad f< if  -1 play-table exit  then
    side f@ fabs 120e deg>rad f> if  1+ -1 play-table exit  then
    side f@ f0< if  3 +  else  2 +  then  -1 play-table ;

\ ---- knocked down (Fiona_StateKnockedDown): her cry, the fall, then up - a stumble's while
\ before she may fall again ----
: up-again ( -- )  ended? if  30 her-stumble-t !  to-idle exit  then  root-move ;
: knocked ( -- )
    settled? if
        blow-side
        her-sub @ case
            $C of  $3E 0 her-sound  $100E -1 play-table  endof
            $E of  $3E 0 her-sound  $1000 fall-by-side  endof
            $D of  $3F 0 her-sound  $100F -1 play-table  endof
            $F of  $3F 0 her-sound  $1004 fall-by-side  endof
        endcase
        $5F noise-here  ['] up-again her-act !
    then
    root-move ;

\ ---- thrown (Fiona_StateThrown): facing him, back along her heading ($1008); from behind,
\ forwards ($100B; panicking she is flung down $B04). Moved by the motion turned to the way he
\ struck from, turning there 20 degrees a frame (pull_back_move); then up ($100A / $100D), or
\ panicking she stays down and gets up as from a panic fall ----
: pull-back ( -- )
    her-fall-turn @ -1 = if  root-move exit  then
    root@ rm-x f@ rm-z f@ her-fall-yaw f@ rotate-by move-by
    her-fall-turn @ 0= if
        her-yaw f@ her-fall-yaw f@ 20e deg>rad turn-toward f0= if  1 her-fall-turn !  then  her-yaw f!
    then ;
: got-up ( -- )  ended? if  to-idle exit  then  root-move ;
: floored ( -- )  $B01 -1 play-table  $A her-mode !  $B her-doing !  ['] getting-up her-act ! ;
: after-throw ( -- )
    ended? if
        her-throw-back @ 0= if
            anim@ $1008 = if
                her-fear-bits @ 2 and if  floored exit  then
                $100A -1 play-table  ['] got-up her-act ! exit
            then
        else
            anim@ $B04 = if  floored exit  then
            anim@ $100B = if  $100D -1 play-table  ['] got-up her-act ! exit  then
        then
    then
    pull-back  along-wall ;
: thrown ( -- )
    settled? if
        striker? if  0 her-fall-turn !  her-at her-hit-who @ body-at vec-heading her-fall-yaw f!
        else  -1 her-fall-turn !  her-yaw f@ her-fall-yaw f!  then
        her-fall-yaw f@ her-yaw f@ f- angle-wrap fabs pi f2/ f< if
            0 her-throw-back !  $1008 -1 play-table
        else
            -1 her-throw-back !  her-fall-yaw f@ pi f+ angle-wrap her-fall-yaw f!
            her-fear-bits @ 2 and if  -1 her-fall-turn !  her-fall-yaw f@ her-yaw f!  $B04  else  $100B  then
            -1 play-table
        then
        her-fear-bits @ 2 and if  $42 0 her-sound  $6F noise-here  else  $40 0 her-sound  $5F noise-here  then
        ['] after-throw her-act !
    then
    pull-back ;

\ ---- caught (Fiona_StateCaught: a hold - her gasp, a flinch in his grip by how he holds her:
\ an arm $F02 looking at him, $F04, from behind $F03, $F05; then free) ----
: watch-him ( -- )   \ (her head on him while caught by the arm or from behind)
    her-hit-how @ dup 1 = swap 3 = or striker? and if  1 her-look-on !  her-hit-who @ her-look-who !  then ;
: after-caught ( -- )  ended? if  to-idle exit  then  watch-him  root-move ;
: caught ( -- )
    settled? if
        $43 0 her-sound
        her-hit-how @ case  4 of $F05 endof  1 of $F02 endof  2 of $F04 endof  >r $F03 r>  endcase
        -1 play-table  ['] after-caught her-act !
    then
    root-move ;
\ his grip: by the arm (1) only on open floor, facing him (else from behind, 3)
: grip ( how -- how' )
    dup 1 <> if  exit  then  drop
    her-tri @ dup 0< 0= if  nav-flags $80001 and if  3 exit  then  else  drop  then
    striker? 0= if  3 exit  then
    her-at her-hit-who @ body-at vec-heading her-yaw f@ f- angle-wrap fabs pi f2/ f< if  1  else  3  then ;

\ ---- Fiona_React: the blow taken - its fright to her panic, what she was doing dropped, the
\ reaction started; struck, the danger is told (a stumble resets her recovery) ----
: struck ( kind how who -- taken? ) ( F: fright -- )
    her-hit-who !  her-hit-how !
    reaction dup 0< if  drop fdrop false exit  then
    f>cell s" panic" actor-named send fright
    -1 her-fall-turn !  0 her-busy-t !
    4 her-mode !  $A her-doing !  dup her-sub !
    dup $20 <> if  1 danger-id send danger-signal  then   \ (Progress_SetCondBit 1: she's been struck)
    her-hit-how @ $8000 and if  0 her-recovery !  then
    case
        $20 of
            her-hit-how @ 5 = if  -1 her-hit-who !  ['] thrown her-act !
            else  $B her-mode !  8 her-doing !  her-hit-how @ grip her-hit-how !  ['] caught her-act !  then
        endof
        $A of  ['] thrown her-act !  endof
        >r ['] knocked her-act ! r>
    endcase
    true ;
