\ pursuer/debact.fs - Debilitas's own actions (src/game/debilitas.c, his table +0x1714:
\ 0x1000 wander, 0x1001 turn to Fiona and strike, 0x1002 lunge, 0x1003 grab)
IN: pursuer.debact
USING: engine game-state events.core events.words chars relations pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps pursuer.behave pursuer.search pursuer.moves ;

\ ---- 0x1000: Debilitas_StartWander - a triangle at random he may stand on, on his floor
\ level (0x300000 alike), 20 or more away on foot: walked to (60 frames at least) ----
: state-look-walk ( -- )   \ Debilitas_StateLookWalk
    not-yet" Pursuer_RaiseThreat"
    d-path f0< if  1 $16EF pu-c!  then
    p-path-left? if  walk-path-stride
    else d-path f0= if  true  else  1 $16EF pu-c!  false  then then
    0= $1624 pu-l@ 0> and if  $1624 pu-1-  exit  then
    1 step-done! ;
: state-start-walk ( -- )   \ Debilitas_StateStartWalk
    walk-on? if  exit  then
    $328 vcall 0 play-anim-if drop  60 $1624 pu-l!  ['] state-look-walk behave  state-look-walk ;
create sw-pos 12 allot
: start-wander ( -- )
    0 $16EC pu-c!  0 step-next!
    nav-tris 0 ?do
        nav-tris s>f rnd01 f* f>s                                   ( tri )
        dup tri-flags p-mask and 0=
        over tri-flags $300000 and  p-tri tri-flags $300000 and = and
        over tri-flags $300000 and $300000 <> and if
            dup tri-center sw-pos vec!  dup sw-pos path-length-to 20e f< 0= if
                $15A4 pu-l!  $15A4 pu-l@ tri-center $15B0 pu vec!  $D8 vcall drop
                0 $1784 pu-l!  ['] state-start-walk behave  state-start-walk  unloop exit
            then
        then  drop
    loop
    1 $16EF pu-c!  1 step-next! ;

\ ---- 0x1001: turning to her, then his blow (Debilitas_StateBlow) ----
defer state-blow   :noname  not-yet" Debilitas_StateBlow" ; is state-blow
: state-turn-to-fiona ( -- )
    p-ended? if  $205 0 play-anim-if drop  $1C p-sub!  ['] state-blow behave  state-blow exit  then
    root-move-masked
    me her c-pos c-heading-to $A0 vcall turn-toward fdrop ;
: start-turn-to-fiona ( -- )
    0 $16EC pu-c!  0 step-next!
    walk-on? if  exit  then
    $1304 0 play-anim-if drop  1 $16F7 pu-c!  0 $1784 pu-l!
    ['] state-turn-to-fiona behave  state-turn-to-fiona ;

\ ---- 0x1002: the lunge (0x1306); in the panic's stage 5 on to his attack's next step ----
defer attack-next-step   :noname  not-yet" Pursuer_AttackNextStep" ; is attack-next-step
: state-lunge ( -- )
    root-move-masked
    p-ended? if
        panic @ 5 <> if  1 step-done!  1 step-next!  exit  then
        8 $1728 pu-l!  ['] attack-next-step behave  8 p-mode!  attack-next-step
    then ;
: start-lunge ( -- )
    0 $16EC pu-c!  0 step-next!
    walk-on? if  exit  then
    $1306 0 play-anim-if drop  0 $1784 pu-l!  ['] state-lunge behave  state-lunge ;

create dn-at 12 allot
: dog-near? ( F: x y z -- flag )   \ (progress +0x2C: Hewie within 5 of the point)
    dn-at vec!  dog c-active? 0= if  false exit  then
    dn-at 4 + sf@  dog c-pos 4 + sf@ 5e f- f>  dn-at 4 + sf@ dog c-pos 4 + sf@ dog c-height f+ 5e f+ f< and
    dn-at dog c-pos vec-dist-xz dog c-radius 5e f+ f< and ;
\ ---- 0x1003: the grab (0xE06): at its key, Hewie near his hand (bone 0x1E, 5) - asked to be
\ held (6), once; over at its end, losing her, or far (60) ----
: state-grab ( -- )
    root-move-masked
    p-events 2 and  $1760 pu-c@ 1 and 0= and if
        p-actor $1E bone-pos dog-near? if  1 6 0 3 me 10e ask  $1760 pu-c@ 1 or $1760 pu-c!  then
    then
    p-ended?  $1544 pu-c@ 0= or  d-path 60e f< 0= or  d-path f0< or if  1 step-done!  1 step-next!  then ;
: start-grab ( -- )
    0 $16EC pu-c!  0 step-next!
    walk-on? if  exit  then
    $E06 0 play-anim-if drop  0 $1784 pu-l!  ['] state-grab behave  state-grab ;

\ his table (+0x1714): state, id, mode, sub, sense mode, look (Debilitas_Actions)
create deb-actions
    ' start-wander ,         $1000 , 0 , 2 , 3 , 3 ,
    ' start-turn-to-fiona ,  $1001 , 8 , 2 , $FF , 4 ,
    ' start-lunge ,          $1002 , 0 , 0 , $FF , 4 ,
    ' start-grab ,           $1003 , 8 , 0 , $FF , 4 ,
