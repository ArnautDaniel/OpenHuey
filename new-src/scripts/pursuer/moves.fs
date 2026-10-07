\ pursuer/moves.fs - how the stalker moves (src/game/pursuer.c): by his animations' root
\ motion, along his planned path a stride at a time (Npc_WalkPathStride: he turns toward the
\ point a stride ahead and steps onto it), turning on the spot (0x400..0x403), walking up to a
\ goal (Pursuer_CloseInGoal) or a search stop (Pursuer_StepBack), and a walk with a gesture.
IN: pursuer.moves
USING: engine game-state events.core events.words chars relations pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps pursuer.behave pursuer.search ;

\ ---- his root motion (Motion_RootMovement) ----
fvariable rm-turn  fvariable rm-x  fvariable rm-y  fvariable rm-z
: root@ ( -- )
    p-actor dup 0< if  drop 0e rm-turn f!  0e rm-x f!  0e rm-y f!  0e rm-z f!  exit  then
    root-delta  rm-z f!  rm-y f!  rm-x f!  rm-turn f! ;
\ Character_RootMoveMasked: turned and moved by it, within his floor
: root-move-masked ( -- )
    root@  p-yaw rm-turn f@ f+ angle-wrap p-yaw!  me rm-x f@ rm-z f@ c-move-local ;
\ the stride this frame along his path (the root motion's forward part)
: stride ( F: -- d )  root@ rm-z f@ ;

\ ---- turning (Npc_TurnToward / Npc_TurnTowardPos: toward a heading by at most `step`; the
\ way left) ----
: turn-toward ( F: heading step -- rest )  me c-turn-toward ;
fvariable tt-h  fvariable tt-a  fvariable tt-b
: turn-toward-pos ( v -- ) ( F: step -- rest )
    me swap c-heading-to tt-h f!                                     ( F: step )
    tt-h f@ p-yaw f- fabs fover f< if  fdrop tt-h f@ p-yaw!  0e exit  then
    tt-h f@ p-yaw f- angle-wrap fabs tt-a f!
    tt-h f@ p-yaw head-yaw f@ f+ f- angle-wrap fabs tt-b f!
    tt-a f@ 2.0943952e f<= tt-a f@ tt-b f@ f<= or if
        tt-h f@ p-yaw f- angle-wrap f0> if  1e  else  -1e  then
    else  head-yaw f@ f0> if  1e  else  -1e  then  then
    f* p-yaw f+ angle-wrap p-yaw!
    tt-h f@ p-yaw f- angle-wrap ;
\ Npc_TurnWay / Npc_TurnWayTo: which way to turn for a heading: $FF none (within a), 0 left,
\ 1 right; +2 past b (a half turn)
: turn-way ( F: heading a b -- way )
    fabs tt-b f!  fabs tt-a f!  p-yaw f- angle-wrap                  ( F: d )
    fdup tt-a f@ f<= if  fdup tt-a f@ fnegate f< 0= if  fdrop $FF exit  then  0  else  1  then
    tt-b f@ tt-a f@ f< if  fdrop exit  then
    fabs tt-b f@ f> if  2 or  then ;
: turn-way-to ( v -- way ) ( F: a b -- )  me swap c-heading-to frot frot turn-way ;
\ Pursuer_TurnOnSpot: the turn animation for a way (hurt: 0x405..)
: turn-on-spot ( way -- flag )
    dup 3 u> if  drop false exit  then
    p-cond 1 = if  $405  else  $400  then  +  0 start-anim? ;

\ ---- his path (Character_WaypointAhead, Npc_StepPath, Npc_WalkPathStride) ----
\ the point `d` ahead along his path: its triangle (his own when he's at its end, or it is off
\ his floor), the point in pa-pos, and the waypoint it is on
: waypoint-ahead ( F: d -- next tri )
    p-path-left? 0= if  fdrop  pa-pos p-pos vec-copy  p-path-n p-tri exit  then
    me path-ahead                                                   ( next )
    pa-pos p-mask v-tri-in dup 0< if  drop  pa-pos p-pos vec-copy  drop p-path-n p-tri  then ;
: step-path ( F: d -- next tri )   \ (d <= 0: this frame's stride)
    fdup f0> 0= if  fdrop stride  then
    fdup f0< if  fdrop -1 -1 exit  then  waypoint-ahead ;
: walk-path-stride ( -- done? )
    stride fdup f0< if  fdrop false exit  then
    waypoint-ahead                                                   ( next tri )
    over 0< if  2drop false exit  then
    me pa-pos c-heading-to p-yaw f- angle-wrap fabs 0.7853982e f< 0=   \ (a sharp turn: turn first,
    me path-rest 4e f< 0= and if                                      \ twice as fast)
        2drop  pa-pos $A0 vcall 2e f* turn-toward-pos fdrop
    else
        pa-pos $A0 vcall turn-toward-pos fdrop
        p-tri!  p-pos pa-pos vec-copy  me c-sync  me path-i!
    then
    p-path-left? 0= ;

\ ---- Pursuer_WalkOn: while his animation is still fading in, he keeps walking (his path, or his
\ root motion when he has no sense mode); true then ----
: walk-on? ( -- flag )
    p-faded? if  false exit  then
    $15C0 pu-c@ $FF <> if  p-path-left? if  walk-path-stride drop  then
    else  root-move-masked  then  true ;

\ ---- Pursuer_CloseInGoal (vtable +0x19C): along his path to the goal; turned on the spot when
\ the next point is well to one side; the goal re-measured once he can see its triangle ----
variable cg-tri
: close-in-goal ( -- )
    d-path f0< if  target-out-of-reach? 0= if  1 $16EF pu-c!  then  exit  then
    -1 cg-tri !
    d-path 4e f< 0= if  0.1e step-path cg-tri ! drop  then
    $1788 pu-l@ $400 <> if
        p-faded? cg-tri @ -1 <> and if
            pa-pos 1.3962634e 2.7925268e turn-way-to dup $FF <> if  turn-on-spot drop  else  drop  then
        then
        p-path-left? if
            $1628 pu-l@ 0=  $15A4 pu-l@  me $15B0 pu -1 c-tri-to = and if  $D8 vcall drop  1 $1628 pu-l!  then
            walk-path-stride
        else d-path f0= 0= if  1 $16EF pu-c!  false  else  true  then then
    else
        root-move-masked
        p-faded? if
            p-ended?  me pa-pos c-heading-to p-yaw f- angle-wrap fabs 0.1e f< or if
                $1624 pu-l@ play-anim  $175C pu-l@ $114 vcall exit
            then
        then  false
    then
    if  1 step-done!  0 $1624 pu-l!  0 $1628 pu-l!  then ;
' close-in-goal $19C vt!

\ ---- Pursuer_StepBack (vtable +0x198): setting off to the goal: turned on the spot toward the
\ path's next point, or his walk (scripted: the walk he was given, +0x104[1]); then +0x19C ----
variable sb-tri  variable sb-way
: step-back ( -- )
    walk-on? if  exit  then
    -1 sb-tri !  $FF sb-way !
    d-path 4e f< 0= if
        0.1e step-path sb-tri ! drop
        sb-tri @ -1 <> if  pa-pos 1.3962634e 2.7925268e turn-way-to sb-way !  then
    then
    p-scripted if
        $108 pu-l@ -1 = if  $328 vcall $108 pu-l!  then
        sb-way @ $FF = if  $108 pu-l@ play-anim  else  sb-way @ turn-on-spot drop  then
        $108 pu-l@
    else sb-way @ $FF = if
        p-target dog <> if  $128 vcall  else  $328 vcall play-anim  then  p-anim
    else
        sb-way @ turn-on-spot drop
        p-target dog <> mode 3 = and if  $324 vcall  else  $328 vcall  then
    then then
    $1624 pu-l!
    $15A4 pu-l@  me $15B0 pu -1 c-tri-to = 1 and $1628 pu-l!
    [: $19C vcall ;] behave  $19C vcall ;
' step-back $198 vt!

\ ---- Pursuer_StateWalkGesture (action 0x17..0x19): the gesture of his table (+0x1720, by
\ +0x104) once his walk's fade is over, then to its end (Pursuer_StateEndStep) ----
: state-end-step ( -- )   \ Pursuer_StateEndStep: to its end
    p-ended? if  1 step-done!  1 step-next!  else  root-move-masked  then ;
: state-walk-gesture ( -- )
    0 $16EC pu-c!  0 step-next!
    p-104 0< if  1 step-done!  1 step-next!  exit  then
    walk-on? if  exit  then
    p-104 8 * $1720 pu-l@ +  dup exe-l@ play-anim  4 + exe-c@ if  1 step-next!  then
    0 $1784 pu-l!  ['] state-end-step behave
    p-ended? if  1 step-done!  1 step-next!  else  root-move-masked  then ;
' state-walk-gesture is st.Pursuer_StateWalkGesture
