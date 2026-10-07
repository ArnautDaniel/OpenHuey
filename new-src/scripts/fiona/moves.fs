\ fiona/moves.fs - how Fiona moves (src/game/fiona.c): the stick as the original reads it
\ (Fiona_MoveInput), her animation's root motion, standing, walking, running and turning
\ (Fiona_StateIdleMove and the turning states).
\
\ The pad on the keyboard: W A S D the left stick, Shift the run button (cross).
IN: fiona.moves
USING: engine keys vectors game-state events.core events.words chars fiona.core ;

\ ---- the left stick: the keys (as the d-pad: a key adds a full step) ----
fvariable sx  fvariable sz         \ the stick: x right, z down (as the pad's)
: read-stick ( -- )
    0e sx f!  0e sz f!
    key: D held? if  1e sx f!  then  key: A held? if  sx f@ 1e f- sx f!  then
    key: S held? if  1e sz f!  then  key: W held? if  sz f@ 1e f- sz f!  then ;
: run-held? ( -- flag )  key: Left_Shift held?  key: Right_Shift held? or ;

\ a stick direction turned by a camera heading into the room: up is where the camera looks
\ (the camera's yaw: 0 looks along -z; its right is (cos, sin))
fvariable tn-x  fvariable tn-z  fvariable tn-c  fvariable tn-s
: turned ( F: cam-yaw nx nz -- dx dz )
    tn-z f!  tn-x f!  fdup fcos tn-c f!  fsin tn-s f!
    tn-c f@ tn-x f@ f*  tn-s f@ tn-z f@ f* f-
    tn-s f@ tn-x f@ f*  tn-c f@ tn-z f@ f* f+ ;
: cam-yaw ( F: -- a )  camera cam.yaw sf@ ;

\ ---- Fiona_MoveInput: each frame, the stick as a direction in the room (f-dir), camera
\ relative; after a camera cut the old camera keeps steering while the stick is held (mode 1,
\ then 2 while the direction holds within 15 degrees); the run button ----
fvariable nx  fvariable nz  fvariable rot-yaw     \ (the normalized stick; the camera turn last used)
: vlen ( F: x z -- l )  fsq fswap fsq f+ fsqrt ;
: small? ( F: x z -- flag )  fabs 0.5e f<= fabs 0.5e f<= and ;
: dir! ( F: x z -- )  f-dir 8 + sf!  0e f-dir 4 + sf!  f-dir sf! ;
: dir-by ( F: yaw -- )  nx f@ nz f@ turned dir! ;
defer camera-cut? ( -- flag )  ' false is camera-cut?
: move-input ( -- )
    0 f-go !
    hewie-control @ if  6 f-still !  exit  then   \ (the special mode's walking: with it)
    read-stick
    sx f@ sz f@ vlen fdup f0> if  fdup sx f@ fswap f/ nx f!  sz f@ fswap f/ nz f!  else  fdrop 0e nx f!  0e nz f!  then
    f-turn-mode @ 3 =  camera-cut? or if
        0 f-turn-mode !
        f-mode dup 0= swap 10 = or if
            f-heading f@ f-yaw!  0 f-still !
            nx f@ nz f@ small? 0= if
                3 f-lock !  1 f-turn-mode !  cam-yaw rot-yaw f!
                cam-yaw nx f@ nz f@ turned fswap fatan2 f-heading f!
            then
        then
    then
    sx f@ sz f@ small? dup if  1 f-still +!  f-still @ 6 min f-still !  else  0 f-still !  then  0=   ( moving )
    0                                                                  ( moving how )
    f-turn-mode @ case
        0 of  drop dup if  0  else  f-settled? if  2  else  1  then  then  endof
        1 of  drop over 0= if
                  2  f-still @ 6 = if  0 f-turn-mode !  then
              else
                  3
                  f-lock @ if  -1 f-lock +!  f-camyaw f@ rot-yaw f!
                  else
                      sx f@ f-stick sf@ f- sz f@ f-stick 8 + sf@ f- vlen 0.01e f< if
                          2 f-turn-mode !  0.5e deg>rad f-turn-rate f!  cam-yaw rot-yaw f!
                      then
                  then
                  rot-yaw f@ nx f@ nz f@ turned fswap fatan2 f-turn-to f!
              then  endof
        2 of  drop over if
                  0  nx f@ nz f@ fatan2  f-last sf@ f-last 8 + sf@ fatan2 f- f-wrap-abs 15e deg>rad f> if  0 f-turn-mode !  then
              else
                  2  f-still @ 6 = if  0 f-turn-mode !  then
              then  endof
    endcase                                                            ( moving how )
    case
        3 of  f-camyaw f@ dir-by  endof
        2 of  0e 0e dir!  endof
        1 of  f-yaw fdup fsin fswap fcos dir!  endof
        0 of  cam-yaw dir-by  endof
    endcase  drop
    sx f@ f-stick sf!  0e f-stick 4 + sf!  sz f@ f-stick 8 + sf!
    f-turn-mode @ 0= if  cam-yaw f-camyaw f!  then
    f-turn-mode @ 2 <> if  nx f@ f-last sf!  0e f-last 4 + sf!  nz f@ f-last 8 + sf!  then
    run-held? if  1 f-go !  then ;
: stick-heading ( F: -- a )  f-dir sf@ f-dir 8 + sf@ fatan2 ;   \ FIONA_STICK_HEADING

\ ---- her animation's root motion (Motion_RootMovement / Motion_RootRotation) ----
fvariable rm-turn  fvariable rm-x  fvariable rm-y  fvariable rm-z
: root@ ( -- )
    f-actor dup 0< if  drop 0e rm-turn f!  0e rm-x f!  0e rm-y f!  0e rm-z f!  exit  then
    root-delta  rm-z f!  rm-y f!  rm-x f!  rm-turn f! ;
\ Character_ApplyRootTurn: the animation's turn added to her heading
: root-turn ( -- )  root@  f-yaw rm-turn f@ f+ angle-wrap f-yaw! ;
\ Model_SlopeKeep (the motion's +0x44): on a slope (motion flag 4) the part of a step kept:
\ 1 - (1 - ny^2) x |her facing along the floor's fall|
fvariable sk-x  fvariable sk-y  fvariable sk-z
: slope-keep ( F: -- k )
    f-actor dup 0< if  drop 1e exit  then  actor act.mflags l@ 4 and 0= if  1e exit  then
    f-tri dup 0< if  drop 1e exit  then  tri-normal  sk-z f!  sk-y f!  sk-x f!
    sk-y f@ 1e f< 0= if  1e exit  then
    sk-x f@ sk-z f@ vlen fdup f0= if  fdrop 1e exit  then             ( F: l )
    sk-x f@ fover f/ f-yaw fsin f*  sk-z f@ frot f/ f-yaw fcos f* f+ fabs   ( F: d )
    1e sk-y f@ fsq f- f*  1e fswap f- ;
\ a move by (x, z) in the room, as far as her blocked floor allows (Actor_Move)
: f-move ( F: x z -- )  me c-move ;
\ Character_RootMove / RootMoveMasked: the root motion turned by her heading
\ (x, z) turned by a heading (sceVu0RotMatrixY: x' = x cos + z sin, z' = z cos - x sin)
fvariable ry-x  fvariable ry-z  fvariable ry-a
: rotate-by ( F: x z yaw -- x' z' )
    ry-a f!  ry-z f!  ry-x f!
    ry-x f@ ry-a f@ fcos f*  ry-z f@ ry-a f@ fsin f* f+
    ry-z f@ ry-a f@ fcos f*  ry-x f@ ry-a f@ fsin f* f- ;
: f-root-move ( -- )  root-turn  rm-x f@ rm-z f@ f-yaw rotate-by f-move ;

\ ---- the states that move her ----
defer turn-stick   ' noop is turn-stick          \ Fiona_StateTurnStick (pushing: with the rooms)
defer find-pushable ( F: reach -- ) ( -- 0|-1 )   :noname fdrop -1 ; is find-pushable

\ Fiona_StateIdleMove: standing, resting, walking, running, the panic run, turning; the root
\ motion (less of it while the stick points away from where she faces); pushing when she
\ stops against something
fvariable mv-x  fvariable mv-z  fvariable mv-dz
: released ( g -- )   \ the stick let go
    case
        0 of  game-mode @ 1 = f-fear-bits @ 0= and f-fear f@ 20e f< and f-recovery @ 360 < and if
                  1 f-rest +!  f-rest @ 90 >= if  1 -1 f-play-table  else  -1 idle-anim  then
              else  stand  then  0 f-run-t !  endof
        5 of  f-end? if  stand  then  0 f-run-t !  endof
        1 of  f-still @ 6 < if  walk-look  else  stand  then  0 f-run-t !  endof
        2 of  f-still @ 6 < if  run-look  else  0 f-rest !  0 f-run-t !  -1 idle-anim  then  endof
        >r  0 f-rest !  0 f-run-t !  -1 idle-anim  r>
    endcase ;
: held ( -- )   \ the stick held: walk, or run with the run button
    stick-heading f-heading f!
    $1E state-flag? 0=  f-go @ 1 = and if
        f-fear-bits @ 1 and if
            -1 f-panic-t +!  f-panic-t @ 0< 0= if  1 f-run-t +!  run-look  else  exhausted  0 f-run-t !  then
        else  1 f-run-t +!  run-look  then
    else  0 f-run-t !  walk-look  then ;
: st-idle-move ( -- )
    f-still @ 0= if  1e  else  f-stick-k f@ 0.05e f- 0e fmax  then  f-stick-k f!
    f-group                                                            ( g )
    f-settled? if
        f-act @ 0= if
            f-fear-bits @ 2 and 0= if
                f-still @ if  dup released  else  held  then
            else   \ panicking: running until out of breath
                1e f-stick-k f!
                -1 f-panic-t +!  f-panic-t @ 0< 0= if  run-look  f-still @ 0= if  stick-heading f-heading f!  then
                else  exhausted  then
                0 f-run-t !
            then
        else
            1e f-stick-k f!
            f-end? if
                0 f-act !
                f-fear-bits @ 2 and 0= if
                    f-still @ if  f-still @ 6 >= if  stand  then  else  run-look  then
                else  run-look  then
            then
        then
    then  drop
    root-turn
    f-turn-mode @ 2 = if   \ the accelerating turn toward the old camera's heading
        f-turn-rate f@ 0.0013083e f+ 3e deg>rad fmin f-turn-rate f!
        f-turn-to f@ f-turn-rate f@ f-turn-toward f-turn-rate f@ f< if  0 f-turn-mode !  then
        f-yaw f-heading f!
    else
        f-heading f@ 10e deg>rad f-turn-toward fdrop
    then
    \ the root motion by her heading-to-be, less the further the stick points from her facing
    rm-z f@ slope-keep f* mv-dz f!
    rm-x f@ mv-dz f@ f-heading f@ rotate-by  mv-z f!  mv-x f!
    f-heading f@ f-yaw f- fcos 1e f+ 2e f/ f-stick-k f@ f*           ( F: k )
    fdup mv-x f@ f* mv-x f!  mv-z f@ f* mv-z f!
    mv-x f@ mv-z f@ f-move
    \ moved against the step (pushed back along a wall corner): where she was
    f-pos sf@ f-prev sf@ f- mv-x f@ f*  f-pos 8 + sf@ f-prev 8 + sf@ f- mv-z f@ f* f+ f0< if
        f-prev vec@ me c-pos vec!  f-prev-tri @ me c-tri!  me c-sync
    then
    f-fear-bits @ 2 and 0=  f-act @ $F <> and  f-settled? and if
        mv-dz f@ find-pushable 0= if  1 f-act !  5 f-sub!  ['] turn-stick behave  then
    then ;
' st-idle-move is idle-move-state

\ Fiona_StateIdleStep: a standing animation played out marks a scripted move done; another goes
\ back to idle; the root motion within her blocked floor
: st-idle-step ( -- )
    f-settled? if  f-group 0= if  1 f-e1 !  else  -1 idle-anim  then  then
    f-root-move ;
' st-idle-step is idle-step-state

\ Fiona_StateTurnStanding: turning to f-heading (10 degrees a frame) between animations: in a
\ move she goes idle within 90 degrees; turned all the way her action ends and she stands
: st-turn-standing ( -- )
    f-heading f@ 10e deg>rad f-turn-toward                            ( F: left )
    f-settled? 0= if  fdrop exit  then
    f-group 0<> if  90e deg>rad f< if  -1 idle-anim  then  exit  then
    f0= 0= if  exit  then
    0 f-e1 !  0 f-act !  0 f-mode!  f-yaw f-heading f!  0 f-rest !  0 f-turn-mode !
    f-busy? 0= if  0 f-2d !  -1 idle-anim  ['] st-idle-move behave
    else  ['] st-idle-step behave  then
    $2B state-flag-clear ;
\ Fiona_StateTurnOnSpot: turning to f-heading; the turn left / right animation (0x400 / 0x401)
\ between animations; there: idle
: st-turn-on-spot ( -- )
    f-heading f@ 10e deg>rad f-turn-toward                            ( F: left )
    f-settled? if
        fdup f0= if  -1 idle-anim
        else f-heading f@ f-yaw-was f@ f- angle-wrap f0< if  f-anim@ $400 <> if  $400 -1 f-play-table  then
        else  f-anim@ $401 <> if  $401 -1 f-play-table  then  then then
    then  fdrop
    ['] st-turn-standing behave ;
\ Fiona_StateStep: a scripted step (+0x2B: her feet not fitted)
: st-step ( -- )  f-2b @ 1 = if  0 f-cam-on !  then  f-root-move ;
