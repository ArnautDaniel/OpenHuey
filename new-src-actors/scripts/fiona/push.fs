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
USING: engine game-state actors messages common obstacles rumble fiona.state fiona.model fiona.moving fiona.slam ;

defer to-idle   ' noop is to-idle        \ (fiona.fs)
<PRIVATE
: obstacles-id ( -- id )  s" obstacles" actor-named ;
$800000 constant pushable                \ (NAV_PUSHABLE)
: play-own ( anim -- )  dup entry 8 or >r >r -1 r> r> start ;   \ (Motion_PlayOwnBlend)
: play-cut ( anim -- )  dup entry >r >r -1 r> r> start ;        \ (Motion_Play)

\ ---- something to push ahead (Fiona_FindPushable) ----
create probe 12 allot
: probe! ( F: side reach -- )   \ from where she was this frame, in her frame
    her-yaw f@ rotate-by  her-was-at 8 + sf@ f+ probe 8 + sf!  her-was-at sf@ f+ probe sf!
    her-was-at 4 + sf@ probe 4 + sf! ;
fvariable reach  fvariable wall-yaw
: side-tri ( F: side -- ) ( -- tri | -1 )   \ the floor there, reached straight over the mesh: pushable
    reach f@ probe!  her-tri @ her-at probe 0 v-walk
    dup 0< if  exit  then  dup nav-flags pushable and 0= if  drop -1  then ;
: lowest ( bits -- k )  0  begin  over 1 and 0=  while  1+ swap 1 rshift swap  repeat  nip ;
PRIVATE>
: find-pushable ( pick? -- ok? ) ( F: reach -- )
    reach f!
    group  dup 1 =  over 2 = or  swap 6 = or 0= if  drop false exit  then
    0e reach f@ probe!  her-tri @ her-at probe blocked-floor v-wall 0= if  fdrop drop false exit  then  wall-yaw f!
    -2e side-tri dup 0< if  2drop false exit  then  ring-mask
    2e side-tri dup 0< if  drop 2drop false exit  then  ring-mask and                ( pick? both )
    \ the push: square to the wall, from her side of it
    wall-yaw f@ pi f2/ f+ angle-wrap
    fdup her-yaw f@ f- angle-wrap fabs pi f2/ f> if  pi f+ angle-wrap  then
    fdup fsin her-push-dir sf!  0e her-push-dir 4 + sf!  fcos her-push-dir 8 + sf!
    -1 her-push !
    swap if  ?dup if  lowest her-push !  then  else  drop  then  true ;

<PRIVATE
: stick-along ( F: -- dot )   \ the stick against the push
    her-push-dir sf@ her-dir sf@ f*  her-push-dir 8 + sf@ her-dir 8 + sf@ f* f+ ;
: let-go? ( -- flag )  stick-along 0.6e f<= ;
: held? ( -- flag )   \ (Fiona_PushBlocked: her panic attack on her, or the stalker touching her - she lets go)
    her-fear-bits @ 2 and if  true exit  then
    stalker-in-room dup 0< if  drop false exit  then  self swap 0e 0e bodies-touching? ;
: can-go? ( -- flag )  her-push @  her-push-dir sf@ her-push-dir 8 + sf@  can-push? ;
\ the floor 2.2 ahead of where she was pushable, she pushing (Fiona_StatePushStep)
: floor-ahead? ( -- flag )
    her-mode @ 0<>  group 6 <> or if  false exit  then
    0e 2.2e probe!  her-tri @ her-at probe 0 v-walk
    dup 0< if  drop false exit  then  nav-flags pushable and 0<> ;
: strain ( -- )  $1203 play-cut ;
: push-it ( -- )   \ the obstacle sent on two squares, her push
    her-push @  her-push-dir sf@ f>cell  her-push-dir 8 + sf@ f>cell  obstacles-id send push-obstacle
    $1201 play-cut ;

\ ---- letting go (Fiona_StatePushLetGo): free as it comes round ----
: letting-go ( -- )  held? if  to-idle exit  then  root-move  ended? if  to-idle  then ;
: let-go ( -- )  $1202 play-cut  ['] letting-go her-act ! ;
\ ---- pushing on (Fiona_StatePushStep) ----
: push-key? ( -- flag )  her-model @ 0 0 1 motion-events 2 and 0<> ;   \ (a push's step: its key)
: pushing-on ( -- )
    held? if  to-idle exit  then
    push-key? if  $40 16 fiona-jolt  then
    root-move
    ended? 0= if  exit  then
    anim@ $1201 = if  her-push @ obstacles-id send obstacle-moved  then
    let-go? if  let-go exit  then
    floor-ahead? if  can-go? if  push-it exit  then  then
    strain ;
\ ---- leaning on it (Fiona_StatePush) ----
: pushing ( -- )
    held? if  to-idle exit  then
    root-move
    let-go? if  settled? if  anim@ $1203 = if  let-go  else  to-idle  then  then  exit  then
    ended? 0= if  exit  then
    her-push @ 0< if  true 2.2e find-pushable 0= if  let-go exit  then  then
    her-push @ 0<  if  strain exit  then
    can-go? 0= if  strain exit  then
    push-it  $45 5 0 0 her-pos sound-at  ['] pushing-on her-act ! ;
\ ---- turning to it (Fiona_StateTurnStick) ----
: turning-to ( -- )
    root-move
    let-go? if  to-idle exit  then
    her-push-dir sf@ her-push-dir 8 + sf@ fatan2 her-heading f!
    10e deg>rad turn-to-heading f0= if  $1200 play-own  ['] pushing her-act !  then ;
PRIVATE>
\ walking or running into it (Fiona_StateIdleMove: her step's reach ahead): she turns to push
: try-push ( F: reach -- )
    her-doing @ $F =  settled? 0= or  her-fear-bits @ 2 and or if  fdrop exit  then   \ (not panicking)
    false find-pushable 0= if  exit  then
    1 her-doing !  5 her-sub !  ['] turning-to her-act ! ;
