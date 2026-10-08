\ fiona/looks.fs - where her head looks (Fiona_Head, Fiona_HeadLook; the motion's look-at and
\ tilt: Motion_LookAt, Motion_EaseTilt, HumanModel_AdjustBone). On her own she looks at Hewie
\ (within 50, ahead of her, reachable) - the stalker first, within 200, with them - or else
\ toward where she is turning; after a command at him; under a script where it says (the
\ story's moves 12 / 13). The head bone ($35) takes the pitch and half the turn, the bone below
\ it ($2B) the other half.
IN: fiona.looks
USING: engine actors common fiona.state fiona.model ;

$35 constant head-bone   $2B constant neck-bone   14e fconstant eye-up
: dog-ok? ( -- flag )  dog-here @ 2 = ;            \ Hewie with her (the original's +0x1AD5D5)
: hewie-id ( -- id )  s" hewie" actor-named ;

\ Motion_LookAt: from her eye (14 up) to the point - the pitch (its rise over the level distance)
\ and the turn from her heading (negative to the left)
create eye 12 allot
: look-at ( v -- ) ( F: -- pitch turn )
    her-at eye 12 move  eye 4 + dup sf@ eye-up f+ sf!
    dup 4 + sf@ eye 4 + sf@ f-  dup eye vec-dist-xz fatan2
    eye swap vec-heading her-yaw f@ f- angle-wrap ;

\ fiona_sees: within range, ahead of her (the way to it against her forward, unnormalised, over
\ 0.6), and she could walk straight there
: sees? ( id F: range -- flag )
    dup body? 0= if  drop fdrop false exit  then
    dup body-at her-at vec-dist fswap f< 0= if  drop false exit  then
    dup body-pos fswap fdrop  her-at 8 + sf@ f-  fswap her-at sf@ f-          ( F: dz dx )
    her-yaw f@ fsin f*  fswap her-yaw f@ fcos f* f+                           ( F: the way . her forward )
    0.6e f> 0= if  drop false exit  then
    >r  her-tri @ her-at r@ body-at $40080 v-walk  r> drop  0< 0= ;

fvariable want-p  fvariable want-y  fvariable e-to  fvariable e-step
: clamp-pitch ( F: p -- p' )  -0.31415927e fmax 0.7853982e fmin ;
: ease ( addr -- ) ( F: target step -- )   \ (Motion_EaseTilt: toward the target by at most step)
    fabs e-step f!  e-to f!
    dup f@ e-to f@ f- fabs e-step f@ f<= if  e-to f@ f! exit  then
    dup f@ e-to f@ f> if  dup f@ e-step f@ f- f!  else  dup f@ e-step f@ f+ f!  then ;

\ the head's bones turned by the angles (each frame, as she is posed)
: turn-head ( -- )
    her-model @ turns-clear
    her-model @ neck-bone  0e  her-head-yaw f@ 0.5e f*  turn+
    her-model @ head-bone  her-head-pitch f@ fnegate  her-head-yaw f@ 0.5e f*  turn+ ;

\ whom she looks at: their head (the actor gone: the look off)
: look-point ( -- ok? )
    her-look-who @ dup 0< if  drop true exit  then
    dup alive? 0= if  drop 0 her-look-on !  false exit  then
    dup body? 0= if  drop 0 her-look-on !  false exit  then
    body-head her-look-pt 12 move  true ;

\ Fiona_HeadLook (a script has her): at whom / where the script said, or ahead; eased 10%
: head-look ( -- )
    0e want-p f!  0e want-y f!
    her-look-on @ if  look-point drop  then
    her-look-on @ if
        her-look-pt look-at  -2.1991148e fmax 2.1991148e fmin want-y f!  clamp-pitch want-p f!
    then
    want-p f@ her-head-pitch f@ f- 0.1e f* her-head-pitch f@ f+ her-head-pitch f!
    want-y f@ her-head-yaw f@ f- 0.1e f* her-head-yaw f@ f+ her-head-yaw f!
    turn-head ;

\ Fiona_Head (on her own): the look decided afresh each frame
: look-cleared ( -- )  her-scripted @ 0= if  0 her-look-on !  then ;   \ (Fiona_Update's start)
: still? ( -- flag )  her-doing @ dup $E = over 1 = or swap $F = or ;
: head ( -- )   \ (the look was cleared as her frame began: a state may have set it since)
    her-look-hold @ if  -1 her-look-hold +!  1 her-look-on !  then
    her-look-on @ 0= her-mode @ 0= and  still? 0= and  anim@ 1 <> and if
        dog-ok? if  hewie-id dup 50e sees? if  her-look-who !  1 her-look-on !  else  drop  then  then
    then
    her-look-on @ if
        look-point if
            her-look-pt look-at  fswap clamp-pitch want-p f!                    ( F: turn )
            her-look-hold @ her-mode @ $D = or  her-mode @ $B = her-sub @ $20 = and or if
                -1.8849556e fmax 1.8849556e fmin
            else  fdup fabs pi f2/ f> if  fdrop 0e  then  then  want-y f!
        else  0e want-p f!  0e want-y f!  then
    else
        0e want-p f!
        her-mode @ 0<> still? or if  0e  else  her-heading f@ her-yaw f@ f- angle-wrap  then  want-y f!
    then
    her-head-pitch  want-p f@  want-p f@ her-head-pitch f@ f- fabs 0.1e f*  ease
    her-head-yaw  want-y f@
    want-y f@ her-head-yaw f@ f- fabs 0.1e f*  her-yaw f@ her-yaw-was f@ f- angle-wrap fabs f+  ease
    turn-head ;
: looks ( -- )  her-scripted @ if  head-look  else  head  then ;
\ the head as it was posed last, for whoever looks at her
: tell-head ( -- )  her-model @ head-bone bone-pos body-head! ;
