\ fiona/controls.fs - the stick as a direction in the room (Fiona_MoveInput). Camera-relative;
\ after a camera cut the old camera keeps steering while the stick is held (3 frames for
\ sure, then until it moves), then she turns toward the old camera's heading while the stick
\ holds within 15 degrees. On the keyboard: W A S D the stick, Shift the run button.
IN: fiona.controls
USING: engine keys common fiona.state ;

fvariable sx  fvariable sz         \ the stick: x right, z down (as the pad's)
: hands-off? ( -- flag )  her-scripted @ her-reading @ or her-in-scene @ or ;   \ (a script or a cutscene has her, or a message is up)
: read-stick ( -- )
    0e sx f!  0e sz f!
    hands-off? if  exit  then
    key: D held? if  1e sx f!  then  key: A held? if  sx f@ 1e f- sx f!  then
    key: S held? if  1e sz f!  then  key: W held? if  sz f@ 1e f- sz f!  then ;
: run-held? ( -- flag )  hands-off? 0=  cross button-down? and ;   \ (Fiona_Move: cross held)
: cam-yaw ( F: -- a )  camera cam.yaw sf@ ;
\ a stick direction turned into the room by a camera heading (0 looks along -z)
fvariable tn-x  fvariable tn-z
: turned ( F: cam-yaw nx nz -- dx dz )
    tn-z f!  tn-x f!  fdup fcos tn-x f@ f*  fover fsin tn-z f@ f* f-
    fswap fdup fsin tn-x f@ f*  fswap fcos tn-z f@ f* f+ ;
fvariable nx  fvariable nz   \ (the stick normalized)
: small? ( F: x z -- flag )  fabs 0.5e f<= fabs 0.5e f<= and ;
: dir! ( F: x z -- )  her-dir 8 + sf!  0e her-dir 4 + sf!  her-dir sf! ;
: dir-by ( F: yaw -- )  nx f@ nz f@ turned dir! ;
: stick-heading ( F: -- a )  her-dir sf@ her-dir 8 + sf@ fatan2 ;
\ (a cut: she faces where she was going, and with the stick held the old camera steers)
: after-cut ( -- )
    0 her-turn-mode !
    her-mode @ dup 0= swap 10 = or 0= if  exit  then
    her-heading f@ her-yaw f!  0 her-still !
    nx f@ nz f@ small? if  exit  then
    3 her-lock !  1 her-turn-mode !  cam-yaw her-steer-yaw f!
    cam-yaw nx f@ nz f@ turned fswap fatan2 her-heading f! ;
defer settled-for-stand ( -- how )   \ (2 once her animation has faded in, else 1: fiona.moving)
\ how the direction is taken: 0 the camera now, 1 her facing, 2 none, 3 the steering camera
: how ( moving -- moving how )
    her-turn-mode @ case
        0 of  dup if  0  else  settled-for-stand  then  endof
        1 of  dup 0= if
                  2  her-still @ 6 = if  0 her-turn-mode !  then
              else
                  3
                  her-lock @ if  -1 her-lock +!  her-cam-yaw f@ her-steer-yaw f!
                  else
                      sx f@ her-stick sf@ f-  sz f@ her-stick 8 + sf@ f-  vlen 0.01e f< if
                          2 her-turn-mode !  0.5e deg>rad her-turn-rate f!  cam-yaw her-steer-yaw f!
                      then
                  then
                  her-steer-yaw f@ nx f@ nz f@ turned fswap fatan2 her-turn-to f!
              then  endof
        2 of  dup if
                  0  nx f@ nz f@ fatan2  her-last sf@ her-last 8 + sf@ fatan2 f- angle-wrap fabs
                  15e deg>rad f> if  0 her-turn-mode !  then
              else
                  2  her-still @ 6 = if  0 her-turn-mode !  then
              then  endof
        >r 0 r>
    endcase ;
: read-controls ( -- )
    read-stick
    sx f@ sz f@ vlen fdup f0> if  fdup sx f@ fswap f/ nx f!  sz f@ fswap f/ nz f!  else  fdrop 0e nx f!  0e nz f!  then
    her-turn-mode @ 3 =  her-cut @ or if  after-cut  then  0 her-cut !
    sx f@ sz f@ small? dup if  1 her-still +!  her-still @ 6 min her-still !  else  0 her-still !  then  0=
    how nip
    case
        3 of  her-cam-yaw f@ dir-by  endof
        2 of  0e 0e dir!  endof
        1 of  her-yaw f@ fdup fsin fswap fcos dir!  endof
        0 of  cam-yaw dir-by  endof
    endcase
    sx f@ her-stick sf!  0e her-stick 4 + sf!  sz f@ her-stick 8 + sf!
    her-turn-mode @ 0= if  cam-yaw her-cam-yaw f!  then
    her-turn-mode @ 2 <> if  nx f@ her-last sf!  0e her-last 4 + sf!  nz f@ her-last 8 + sf!  then
    run-held? her-run? ! ;
