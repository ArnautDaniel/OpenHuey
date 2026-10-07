\ pursuer/grab.fs - the stalker seizing Fiona (src/game/pursuer.c): facing her and closing on
\ her (Pursuer_StateFaceFiona, Pursuer_CloseOnFiona); in reach and facing her, a joint action
\ asked of her (Relation_Request kind 9: type 6 by the hand - his taunt 0x1900.., type 8
\ walking - his grab 0x1A01); she refused it (his request 7): his next attack
\ (Pursuer_PickAttack).
IN: pursuer.grab
USING: engine game-state events.core events.words chars relations pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps pursuer.behave pursuer.search pursuer.moves pursuer.chase pursuer.debchase pursuer.attack ;

:noname ( F: -- r )  10e ; $300 vt!   \ Pursuer_FrightSeen
:noname ( F: -- r )  5e ; $304 vt!    \ Pursuer_FrightAttack

\ ---- Pursuer_PickAttack (vtable +0x134): his table for the panic (stage 4 and up: 7, else 6)
\ rolled; the attack behaviour (+0x288) with it next ----
: pick-attack ( -- )
    panic @ 4 >= if  7  else  6  then  $130 vcall
    100e rnd01 f* pick-roll f!
    $1718 pu-l@  begin  dup 8 + exe-f@ pick-roll f@ f<  while  12 +  repeat  pk-row !
    pk-row @ exe-l@
    dup $17 = if  row-arg p-104!  then
    dup $13 = if  row-arg $1728 pu-l!  0 $172C pu-c!  then
    [: $288 vcall ;] behaviour!  $118 vcall ;
' pick-attack $134 vt!

\ ---- after her answer: refused (7) - his next attack; else on ----
: answered? ( -- refused? )   \ (his state block: 7 her refusal)
    0 p-req 7 = if  0 0 p-req!  $134 vcall  true exit  then  0 0 p-req!  false ;
: state-step-to-end ( -- )   \ Pursuer_StateStepToEnd
    root-move-masked  p-ended? if  1 step-done!  1 step-next!  then ;

\ the grab (0x1A01, his +0x2D set)
: state-grab-start ( -- )
    answered? if  exit  then
    $1A01 dup p-entry 8 or p-start          \ (Motion_PlayOwnBlend)
    1 p-2d!  $19 p-sub!  ['] state-step-to-end behave ;
\ the taunt (0x1900, then 0x1901 again and again with the fright he puts on her; 6 of them, or
\ her caught: his hold 0x1904 / 0x1903)
: state-taunt-hold ( -- )
    root-move-masked
    p-ended? p-anim $1903 = and if
        1 $1760 pu-c!  $1748 pu-l@ ?dup if  exe-l@  else  90  then  $178C pu-l!
        1 step-next!  -1 p-104!  1 step-done!
    then ;
: state-taunt-repeat ( -- )
    root-move-masked
    p-ended? if
        $304 vcall fiona.panic:panic-fright  p-104 1+ p-104!
        0 p-req 7 <> if
            p-104 6 >= if  $1904 play  ['] state-taunt-hold behave exit  then
            $1901 play
        else  0 0 p-req!  $1903 play  ['] state-taunt-hold behave  then
    then ;
: state-taunt ( -- )
    p-ended? if  $1901 play  ['] state-taunt-repeat behave  $300 vcall fiona.panic:panic-fright exit  then
    root-move-masked ;
: state-taunt-start ( -- )
    answered? if  exit  then
    $1900 dup p-entry 8 or p-start  $19 p-sub!  ['] state-taunt behave ;
\ ---- asking her (Pursuer_StateRelation1 / 2): the joint action, then her answer ----
: state-relation ( type next -- )
    swap >r 1 9 0 r> me 0e ask  behave ;

\ ---- Pursuer_CloseOnFiona: hiding (her mode 3): his next attack; in reach (+0x2E4) and facing
\ her (+0x2EC degrees): seizing her if she may be (Pursuer_MayGo; her floor free), else his
\ table for being near (3); else on to her (0x16). While his walk fades in, turning to her ----
: close-on-fiona ( type next -- )
    walk-on? if
        2drop  me her c-pos c-heading-to $A0 vcall turn-toward fdrop  exit
    then
    her c-mode 3 = if  2drop $134 vcall exit  then
    d-fiona $2E4 vcall f<  d-fiona f0< 0= and if
        me p-target c-pos c-heading-to p-yaw f- angle-wrap fabs  3.1415927e $2EC vcall f* 180e f/ f< if
            may-go? if  state-relation exit  then
            2drop  3 $130 vcall  pick-from-table exit
        then
    then
    2drop  $16 $114 vcall ;
: state-close-a ( -- )  answered? 0= if  ['] state-grab-start behave  then ;
: state-close-b ( -- )  answered? 0= if  ['] state-taunt-start behave  then ;
: state-close-on-fiona-a ( -- )  8 ['] state-close-a close-on-fiona ;
: state-close-on-fiona-b ( -- )  6 ['] state-close-b close-on-fiona ;
\ Pursuer_StateFaceFiona (action 0x14): his stand once his walk's faded in, then closing on her
\ for the hand (B)
: state-face-fiona ( -- )
    0 $16EC pu-c!  0 step-next!
    walk-on? if  exit  then
    $320 vcall play-anim  her p-target!  0 p-104!  0 $1784 pu-l!
    ['] state-close-on-fiona-b behave  state-close-on-fiona-b ;
' state-face-fiona is st.Pursuer_StateFaceFiona
