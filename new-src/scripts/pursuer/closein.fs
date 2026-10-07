\ pursuer/closein.fs - the stalker closing in (src/game/pursuer.c, debilitas.c): stepping
\ straight at a point (Npc_StepToward), the chase walk to his target (Pursuer_CloseIn with
\ Debilitas_ChaseTarget: his path, straight at her once on her floor), standing (action 1:
\ Pursuer_Move17C / Move180) and stepping round her (Pursuer_StateSidestepRoom / Sidestep).
IN: pursuer.closein
USING: engine game-state events.core events.words chars relations pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps pursuer.behave pursuer.search pursuer.moves ;

\ ---- Npc_StepToward: turned toward it (twice as fast past 45 degrees - not stepping then), a
\ stride at it (onto it when it's within the stride); true within 1 ----
: dist-to ( v -- ) ( F: -- d )  me swap c-dist-to ;
: step-toward ( v -- near? )
    stride                                                          ( v ) ( F: s )
    me over c-heading-to p-yaw f- angle-wrap fabs 0.7853982e f< 0= if
        fdrop  dup $A0 vcall 2e f* turn-toward-pos fdrop  dist-to 1e f< exit
    then
    dup $A0 vcall turn-toward-pos fdrop                             ( v ) ( F: s )
    dup dist-to fover f<=  me 2 pick -1 c-tri-to 0< 0= and if
        fdrop  dup vec@ p-pos 8 + sf@ f- frot p-pos sf@ f- frot frot fswap fdrop  me c-move
    else
        me over c-heading-to fover fover fsin f* frot frot fcos f*  me c-move
    then
    dist-to 1e f< ;

\ ---- Debilitas_ChaseTarget (vtable +0x1A8): no way to her (in her room): his route re-made,
\ and given up unless there's a way round; else along his path - straight at her once she's on
\ the floor straight ahead (or her floor blocked and the path's end within 10 of her) ----
create ct-end 12 allot
: chase-target ( -- )
    d-path f0<  p-room p-target c-room = and if
        p-target $B4 vcall  p-room p-target c-room =  target-out-of-reach? 0= or if  1 $16EF pu-c!  then  exit
    then
    p-path-left? 0= if  exit  then
    p-target c-tri tri-blocked? 0= if
        me p-target c-pos -1 c-tri-to p-target c-tri = if  p-target c-pos step-toward drop exit  then
    else
        me dup path-n 1- path-point ct-end swap vec-copy
        me ct-end p-mask c-tri-to 0< 0= if
            me p-target c-pos c-dist-to  ct-end p-target c-pos vec-dist-xz f- 10e f<= if
                p-target c-pos step-toward drop exit
            then
        then
    then
    walk-path-stride drop ;
' chase-target $1A8 vt!

\ ---- Pursuer_CloseIn (vtable +0x1A0 his walk, +0x1A4 his slow walk): the way to his target
\ measured (+0xE0); then the walk and Debilitas_ChaseTarget; no way: his route there, given up
\ unless there's a way round ----
: close-in ( skip walk -- )
    walk-on? if  2drop exit  then
    -1 $E0 vcall drop
    d-path f0< if  2drop  p-target $B4 vcall  target-out-of-reach? 0= if  1 $16EF pu-c!  then  exit  then
    swap $1788 pu-l@ <> if  vcall play-anim  else  drop  then
    0 $1784 pu-l!  [: $1A8 vcall ;] behave  $1A8 vcall ;
:noname  1 $16EC pu-c!  1 step-next!  $200 $324 close-in ; $1A0 vt!
:noname  0 $16EC pu-c!  1 step-next!  $201 $328 close-in ; $1A4 vt!

\ ---- action 1: standing (Pursuer_Move17C: his stand; Pursuer_Move180: his root motion while
\ it fades in; scripted, his move done) ----
: move-180 ( -- )
    p-faded? 0= if  root-move-masked  else  p-scripted if  1 p-char char.move-done l!  then  then ;
' move-180 $180 vt!
: move-17c ( -- )
    1 $16EC pu-c!  1 step-next!
    walk-on? if  exit  then
    $1788 pu-l@ if  $320 vcall play-anim  then
    0 $1784 pu-l!  [: $180 vcall ;] behave  $180 vcall ;
' move-17c $17C vt!

\ ---- Npc_RoomToSide: floor `dist` to one side of the way to his target (a quarter turn either
\ way) ----
create rs-at 12 allot
: room-to-side? ( F: dist -- flag )
    2 0 do
        i if  -1.5707964e  else  1.5707964e  then
        me p-target c-pos c-heading-to f+ angle-wrap fover  rs-at p-pos vec-ahead
        me rs-at p-mask c-tri-to dup 0< 0= if
            tri-flags p-mask and 0= if  fdrop true unloop exit  then
        else  drop  then
    loop  fdrop false ;
\ ---- Pursuer_StateSidestep: round her at the same distance, 5 degrees a step the way +0x104
\ says (blocked: the other way, then done); done after 30 of it or 120 frames ----
create ss-at 12 allot  create ss-prev 12 allot
: sidestep-point ( -- )   \ (him turned round her by +0x104 x 5 degrees, at his distance)
    me p-target c-pos c-dist-to
    p-target c-pos p-pos vec-heading  p-104 s>f 0.08726647e f* f+     ( F: r h )
    fswap ss-at p-target c-pos vec-ahead ;
: sidestep-ok? ( -- flag )   \ (floor for him there, and straight from it to her)
    me ss-at 0 c-tri-to dup 0< if  drop false exit  then
    dup tri-blocked? if  drop false exit  then
    ss-at p-target c-pos 0 v-walk p-target c-tri = ;
: state-sidestep ( -- )
    2 0 do
        sidestep-point
        sidestep-ok? 0= if
            i if  1 step-done!  unloop exit  then
            0e $1634 pu-f!  p-104 negate p-104!
        else
            ss-prev p-pos vec-copy  ss-at step-toward drop
            ss-prev p-pos vec-dist-xz $1634 pu-f@ f+ $1634 pu-f!
            $1634 pu-f@ 30e f<= if
                $1784 pu-l@ 121 >= if  0e $1634 pu-f!  -1 p-104!  1 step-done!  then
            else  0e $1634 pu-f!  p-104 negate p-104!  then
            unloop exit
        then
    loop ;
: state-sidestep-room ( -- )
    0 $16EC pu-c!  1 step-next!
    8e room-to-side? 0= if  1 step-done!  exit  then
    walk-on? if  exit  then
    p-target dup c-pos p-pos vec-heading  c-yaw f- angle-wrap f0> if  1  else  -1  then  p-104!
    0 $1634 pu-l!
    $1788 pu-l@ $200 <> if  $324 vcall play-anim  then
    0 $1784 pu-l!  ['] state-sidestep behave  state-sidestep ;
' state-sidestep-room is st.Pursuer_StateSidestepRoom

\ ---- Pursuer_WalkToGoal (vtable +0x194, action 4's state): to his goal (+0x15A4 / +0x15B0;
\ none or off it: his target's place), measured; no way there - given up unless round a door,
\ standing; else the walk (+0x198) ----
: walk-to-goal ( -- )
    0 $16EC pu-c!  1 step-next!
    $15A4 pu-l@ dup 0<  swap nav-tris >= or if  -1 $B4 vcall  then
    $15A4 pu-l@ $15B0 pu on-tri? 0= if  -1 $B4 vcall  then
    $D8 vcall 0= if
        target-out-of-reach? 0= if  1 $16EF pu-c!  then
        root-move-masked exit
    then
    -1 $1624 pu-l!  0 $1784 pu-l!  [: $198 vcall ;] behave  $198 vcall ;
' walk-to-goal $194 vt!
