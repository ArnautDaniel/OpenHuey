\ stalker/moving.fs - how a stalker moves (src/game/pursuer.c): by his animations' root motion,
\ along his way a stride at a time (Npc_WalkPathStride: turned toward the point a stride on
\ and stepped onto it), turning on the spot ($400..$403) when the way goes off to one side,
\ his walk for his mode, standing.
IN: stalker.moving
USING: engine game-state actors facts flag-names paths stalker.state stalker.senses ;

: model ( -- a )  my-model @ actor ;
: sound ( id bank -- )   \ Pursuer_Sound: at him (not while the world is held)
    world-held state-flag? if  2drop exit  then  >r >r  my-at vec@  r> r>  bank-sound-at ;
: yaw ( F: -- a )  self body-yaw ;
: yaw! ( F: a -- )  body-turn ;

\ ---- his animations (Pursuer_PlayAnim: blended as its table says, not restarted while it
\ plays) ----
: play-now ( anim -- )   \ (Motion_PlayTable)
    dup anim !  my-model @ swap  2dup motion-entry nip  motion-play ;
: play ( anim -- )  dup anim @ = if  drop exit  then  play-now ;
: stand-anim ( -- a )  cond @ 1 = if  2  else  0  then ;   \ (vtable +0x320: hurt, 2)
\ his walks (vtable +0x324 / +0x328): hurt $202 / $203; else $200, and the slow one - held off
\ $203, searching or heading $201, after her $206
: walk-fast ( -- a )  cond @ 1 = if  $202  else  $200  then ;
: walk-slow ( -- a )
    my-mode @ 4 =  cond @ 1 = or if  $203 exit  then
    my-mode @ if  $201  else  $206  then ;
: turn-base ( -- a )  cond @ 1 = if  $405  else  $400  then ;   \ (Pursuer_TurnOnSpot: hurt, $405..)
: ended? ( -- flag )  model act.mflags l@ $20 and 0<> ;   \ came to its end this frame
: settled? ( -- flag )  model act.fade sf@ 0e f<= ;       \ faded in

\ ---- his root motion this frame (Motion_RootMovement): turned and moved by it, along walls ----
fvariable rm-turn  fvariable rm-x  fvariable rm-y  fvariable rm-z
: root@ ( -- )  my-model @ root-delta  rm-z f!  rm-y f!  rm-x f!  rm-turn f! ;
: root-move ( -- )  root@  yaw rm-turn f@ f+ angle-wrap yaw!  rm-x f@ rm-z f@ body-move-local ;
: stride ( F: -- d )  root@ rm-z f@ ;

\ ---- turning (Npc_TurnTowardPos: toward a point by at most `step`; how far is left) ----
: heading-to ( v -- ) ( F: -- a )  self swap body-heading-to ;
: turn-to ( v -- ) ( F: step -- left )  heading-to fswap body-turn-toward ;
\ the turn rate (vtable +0xA0; Debilitas 3 degrees a frame)
: turn-rate ( F: -- r )  0.052359879e ;
\ Pursuer_TurnOnSpot via Npc_TurnWayTo: past 80 degrees off he turns on the spot - $400 left,
\ $401 right, +2 a half turn (past 160); $FF none
fvariable wp-a  fvariable wp-b
: way-past ( v -- way ) ( F: a b -- )   \ (Npc_TurnWayTo: past a off, b for a half turn)
    wp-b f!  wp-a f!  heading-to yaw f- angle-wrap                   ( F: d )
    fdup fabs wp-a f@ f<= if  fdrop $FF exit  then
    fdup f0< if  0  else  1  then
    fabs wp-b f@ f> if  2 or  then ;
: way-to ( v -- way )  1.3962634e 2.7925268e way-past ;

\ ---- along his way: planned to (tri, v) over his floor; how far is left ----
: plan ( tri v -- n )  >r >r  my-path self body-tri my-at r> r> self body-mask path-plan ;
: way-left ( F: -- d )  my-path path-left? if  my-path my-at path-rest  else  0e  then ;
\ Npc_WalkPathStride: the point a stride on; well off his heading (45 degrees, with 4 or more
\ left) he only turns, twice as fast; else turned to it and put there. True at the end
create sp-at 12 allot
: walk-stride ( -- end? )
    my-path path-left? 0= if  true exit  then
    stride fdup f0< if  fdrop false exit  then  0.01e fmax
    my-path my-at path-ahead  sp-at ahead vec-copy                  ( i )
    sp-at heading-to yaw f- angle-wrap fabs 0.7853982e f< 0=
    way-left 4e f< 0= and if
        drop  sp-at turn-rate 2e f* turn-to fdrop  false exit
    then
    sp-at turn-rate turn-to fdrop
    self body-tri >r  sp-at vec@ body-place-at                       \ (just off the floor: his
    self body-tri 0< if  room-id r@ sp-at vec@ body-place  then  r> drop   \ triangle kept)
    my-path path-i!
    my-path path-left? 0= ;

\ Npc_StepToward: turned toward a point (past 45 degrees only turning, twice as fast); a stride
\ at it - onto it when it is within the stride
: step-toward ( v -- )
    stride fdup f0< if  fdrop drop exit  then                        ( v ) ( F: s )
    dup heading-to yaw f- angle-wrap fabs 0.7853982e f< 0= if
        fdrop  turn-rate 2e f* turn-to fdrop exit
    then
    dup turn-rate turn-to fdrop
    my-at over vec-dist-xz fover f<= if  fdrop vec@ body-place-at exit  then
    drop  0e fswap body-move-local ;
