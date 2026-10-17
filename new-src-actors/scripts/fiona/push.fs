\ fiona/push.fs - Fiona pushing things (src/game/fiona.c Fiona_FindPushable, Fiona_StateTurnStick /
\ StatePush / StatePushStep / StatePushLetGo; the things: obstacles.fs). Walking or running up
\ against something to push - the walk toward the point her step reaches ahead meets a wall (an
\ edge, or her floor's blocks), the pushable floor ($800000: the ring round an obstacle) 2 to
\ each side of it - she turns square to the wall (10 degrees a frame) while the stick holds
\ that way and leans on it ($1200). As each move comes round, the stick still that way: the
\ obstacle on both sides (the lowest one) pushed if it can go (`can-push?`) - it moves two
\ squares (`push-obstacle`) as she pushes ($1201, her sound $45); else she strains ($1203).
\ Pushing on, the floor 2.2 ahead pushable: again, else strain. The stick let go: straining she
\ lets go ($1202), else free. The stalker touching her: free at once.
IN: fiona.push
USING: engine vectors game-state actors messages common obstacles rumble fiona.state fiona.anims fiona.sounds fiona.model fiona.moving fiona.slam ;

defer to-idle   ' noop is to-idle        \ (fiona.fs)
<PRIVATE
: obstacles-id ( -- id )  s" obstacles" actor-named ;
$800000 constant pushable                \ (NAV_PUSHABLE)

\ ---- something to push ahead (Fiona_FindPushable) ----
create probe /vec allot
: probe! ( F: side reach -- )   \ from where she was this frame, in her frame
    her-yaw f@ rotate-by  {: F: dx dz :}
    her-was-at vec@ {: F: x y z :}  x dx f+  y  z dz f+  probe vec! ;
: side-tri ( F: side reach -- ) ( -- tri | -1 )   \ the floor there, reached straight over the mesh: pushable
    probe!  her-tri @ her-at probe 0 v-walk
    dup 0< if  exit  then  dup nav-flags pushable and 0= if  drop -1  then ;
: lowest ( bits -- k )  0  begin  over 1 and 0=  while  1+ swap 1 rshift swap  repeat  nip ;
PRIVATE>
: find-pushable ( pick? -- ok? ) ( F: reach -- )
    {: pick? F: reach :}
    group dup walking-group =  over running-group = or  swap pushing-group = or 0= if  false exit  then
    0e reach probe!  her-tri @ her-at probe blocked-floor v-wall 0= if  fdrop false exit  then  {: F: wall :}
    -2e reach side-tri dup 0< if  drop false exit  then  obstacles-ring-mask
    2e reach side-tri dup 0< if  2drop false exit  then  obstacles-ring-mask and  {: both :}   \ (the obstacles on both sides)
    \ the push: square to the wall, from her side of it
    wall pi f2/ f+ angle-wrap
    fdup her-yaw f@ f- angle-wrap fabs pi f2/ f> if  pi f+ angle-wrap  then  {: F: push :}
    push fsin her-push-dir v.x sf!  0e her-push-dir v.y sf!  push fcos her-push-dir v.z sf!
    -1 her-push !
    pick? both 0<> and if  both lowest her-push !  then  true ;

<PRIVATE
: stick-along ( F: -- dot )   \ the stick against the push
    her-push-dir v.x sf@ her-dir v.x sf@ f*  her-push-dir v.z sf@ her-dir v.z sf@ f* f+ ;
: let-go? ( -- flag )  stick-along 0.6e f<= ;
: held? ( -- flag )   \ (Fiona_PushBlocked: her panic attack on her, or the stalker touching her - she lets go)
    her-fear-bits @ 2 and if  true exit  then
    stalker-in-room dup 0< if  drop false exit  then  self swap 0e 0e bodies-touching? ;
: can-go? ( -- flag )  her-push @  her-push-dir v.x sf@ her-push-dir v.z sf@  can-push? ;
\ the floor 2.2 ahead of where she was pushable, she pushing (Fiona_StatePushStep)
: floor-ahead? ( -- flag )
    her-mode @ own-mode <>  group pushing-group <> or if  false exit  then
    0e 2.2e probe!  her-tri @ her-at probe 0 v-walk
    dup 0< if  drop false exit  then  nav-flags pushable and 0<> ;
: strain ( -- )  strain-anim play-cut ;
: push-it ( -- )   \ the obstacle sent on two squares, her push
    her-push @  her-push-dir v.x sf@ f>cell  her-push-dir v.z sf@ f>cell  obstacles-id send push-obstacle
    push-anim play-cut ;

\ ---- letting go (Fiona_StatePushLetGo): free as it comes round ----
: letting-go ( -- )  held? if  to-idle exit  then  root-move  ended? if  to-idle  then ;
: let-go ( -- )  let-go-push-anim play-cut  ['] letting-go her-act ! ;
\ ---- pushing on (Fiona_StatePushStep) ----
: push-key? ( -- flag )  her-model @ 0 0 1 motion-events 2 and 0<> ;   \ (a push's step: its key)
: pushing-on ( -- )
    held? if  to-idle exit  then
    push-key? if  $40 16 fiona-jolt  then
    root-move
    ended? 0= if  exit  then
    anim@ push-anim = if  her-push @ obstacles-id send obstacle-moved  then
    let-go? if  let-go exit  then
    floor-ahead? if  can-go? if  push-it exit  then  then
    strain ;
\ ---- leaning on it (Fiona_StatePush) ----
: pushing ( -- )
    held? if  to-idle exit  then
    root-move
    let-go? if  settled? if  anim@ strain-anim = if  let-go  else  to-idle  then  then  exit  then
    ended? 0= if  exit  then
    her-push @ 0< if  true 2.2e find-pushable 0= if  let-go exit  then  then
    her-push @ 0<  if  strain exit  then
    can-go? 0= if  strain exit  then
    push-it  push-sound 0 her-sound  ['] pushing-on her-act ! ;
\ ---- turning to it (Fiona_StateTurnStick) ----
: turning-to ( -- )
    root-move
    let-go? if  to-idle exit  then
    her-push-dir v.x sf@ her-push-dir v.z sf@ fatan2 her-heading f!
    10e deg>rad turn-to-heading f0= if  lean-anim play-own  ['] pushing her-act !  then ;
PRIVATE>
\ walking or running into it (Fiona_StateIdleMove: her step's reach ahead): she turns to push
: try-push ( F: reach -- )
    her-doing @ breathless-doing =  settled? 0= or  her-fear-bits @ 2 and or if  fdrop exit  then   \ (not panicking)
    false find-pushable 0= if  exit  then
    push-doing her-doing !  pushing-sub her-sub !  ['] turning-to her-act ! ;
