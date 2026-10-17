\ fiona/looks.fs - where her head looks (Fiona_Head, Fiona_HeadLook; the motion's look-at and
\ tilt: Motion_LookAt, Motion_EaseTilt, HumanModel_AdjustBone). On her own she looks at Hewie
\ (within 50, ahead of her, reachable) - the stalker first, within 200, with them - or else
\ toward where she is turning; after a command at him; under a script where it says (the
\ story's moves 12 / 13). The head bone ($35) takes the pitch and half the turn, the bone below
\ it ($2B) the other half.
IN: fiona.looks
USING: engine messages vectors actors common fiona.state fiona.anims fiona.model fiona.slam ;

: head-bone ( -- b )  her-costume @ slip-dress = if  $35  else  $21  then ;   \ (FionaModel / EventHumanModel +0x8B0)
: neck-bone ( -- b )  her-costume @ slip-dress = if  $2B  else  $17  then ;   \ (+0x8B4)
14e fconstant eye-up
: dog-ok? ( -- flag )  dog-here @ 2 = ;            \ Hewie with her (the original's +0x1AD5D5)
: hewie-id ( -- id )  s" hewie" actor-named ;

\ Motion_LookAt: from her eye (14 up) to the point - the pitch (its rise over the level distance)
\ and the turn from her heading (negative to the left)
create eye /vec allot
: look-at ( v -- ) ( F: -- pitch turn )
    {: v :}  eye her-at vec-copy  eye v.y sf@ eye-up f+ eye v.y sf!
    v v.y sf@ eye v.y sf@ f-  v eye vec-dist-xz fatan2
    eye v vec-heading her-yaw f@ f- angle-wrap ;

\ fiona_sees: within range, ahead of her (the way to it against her forward, unnormalised, over
\ 0.6), and she could walk straight there
$40080 constant sight-stops   \ (the floors her eye can't pass over)
: in-sight? ( id -- flag )  her-tri @ her-at rot body-at sight-stops v-walk 0< 0= ;   \ fiona_in_sight
: sees? ( id -- flag ) ( F: range -- )
    {: id F: range :}
    id body? 0= if  false exit  then
    id body-at her-at vec-dist range f< 0= if  false exit  then
    id body-pos {: F: x y z :}   \ (the way to them against her forward, unnormalised)
    x her-at v.x sf@ f- her-yaw f@ fsin f*  z her-at v.z sf@ f- her-yaw f@ fcos f* f+  0.6e f> 0= if  false exit  then
    id in-sight? ;

: clamp-pitch ( F: p -- p' )  -18e deg>rad fmax 45e deg>rad fmin ;
: ease ( addr -- ) ( F: target step -- )   \ (Motion_EaseTilt: toward the target by at most step)
    fabs {: a F: goal step :}
    a f@ goal f- fabs step f<= if  goal a f! exit  then
    a f@ goal f> if  a f@ step f-  else  a f@ step f+  then  a f! ;

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
    body-head her-look-pt swap vec-copy  true ;

\ Fiona_HeadLook (a script has her): at whom / where the script said, or ahead; eased 10%
: head-look ( -- )
    0e 0e {: F: pitch yaw :}
    her-look-on @ if  look-point drop  then
    her-look-on @ if
        her-look-pt look-at  -126e deg>rad fmax 126e deg>rad fmin to yaw  clamp-pitch to pitch
    then
    pitch her-head-pitch f@ f- 0.1e f* her-head-pitch f@ f+ her-head-pitch f!
    yaw her-head-yaw f@ f- 0.1e f* her-head-yaw f@ f+ her-head-yaw f!
    turn-head ;

\ Fiona_Head (on her own): the look decided afresh each frame
: look-cleared ( -- )  her-scripted @ 0= if  0 her-look-on !  then ;   \ (Fiona_Update's start)
: still? ( -- flag )  her-doing @ dup attack-doing = over push-doing = or swap breathless-doing = or ;
: head ( -- )   \ (the look was cleared as her frame began: a state may have set it since)
    0e 0e {: F: pitch yaw :}
    her-look-hold @ if  -1 her-look-hold +!  1 her-look-on !  then
    her-look-on @ 0=  her-mode @ own-mode = and  still? 0= and  anim@ rest-anim <> and  her-fear-bits @ 2 and 0= and if   \ (not panicking)
        stalker-in-room dup 0< 0= if  dup 200e sees? if  her-look-who !  1 her-look-on !  else  drop  then  else  drop  then
        her-look-on @ 0= dog-ok? and if  hewie-id dup 50e sees? if  her-look-who !  1 her-look-on !  else  drop  then  then
    then
    her-look-on @ if
        look-point if
            her-look-pt look-at  fswap clamp-pitch to pitch                    ( F: turn )
            her-look-hold @  her-mode @ command-mode = or  her-mode @ react-mode = her-sub @ caught-r = and  or if
                -108e deg>rad fmax 108e deg>rad fmin
            else  fdup fabs pi f2/ f> if  fdrop 0e  then  then  to yaw
        then
    else
        her-mode @ own-mode <> still? or  her-fear-bits @ 2 and or 0= if  her-heading f@ her-yaw f@ f- angle-wrap to yaw  then
    then
    her-head-pitch  pitch  pitch her-head-pitch f@ f- fabs 0.1e f*  ease
    her-head-yaw  yaw
    yaw her-head-yaw f@ f- fabs 0.1e f*  her-yaw f@ her-yaw-was f@ f- angle-wrap fabs f+  ease
    turn-head ;
: looks ( -- )  her-scripted @ if  head-look  else  head  then ;
\ the head as it was posed last, for whoever looks at her
: tell-head ( -- )  her-model @ head-bone bone-pos body-head! ;
