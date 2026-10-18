\ fiona/controls.fs - the stick as a direction in the room (Fiona_MoveInput). Camera-relative;
\ after a camera cut the old camera keeps steering while the stick is held (3 frames for
\ sure, then until it moves), then she turns toward the old camera's heading while the stick
\ holds within 15 degrees. On the keyboard: W A S D the stick, Shift the run button; a gamepad's left stick as it is pushed.
IN: fiona.controls
USING: engine keys common vectors messages facts fiona.state fiona.anims ;

: hands-off? ( -- flag )  her-scripted @ her-reading @ or her-in-scene @ or ;   \ (a script or a cutscene has her, or a message is up)
: sx ( F: -- x )  stick-x f@ ;   : sz ( F: -- z )  stick-z f@ ;
: nx ( F: -- x )  way-x f@ ;     : nz ( F: -- z )  way-z f@ ;
: read-stick ( -- )
    0e stick-x f!  0e stick-z f!
    hands-off? if  exit  then
    stick@ stick-z f!  stick-x f! ;
: run-held? ( -- flag )  hands-off? 0=  cross button-down? and ;   \ (Fiona_Move: cross held)
: cam-yaw ( F: -- a )  camera cam.yaw sf@ ;
: turned ( F: cam-yaw nx nz -- dx dz )  stick-turned ;
: small? ( F: x z -- flag )  fabs 0.5e f<= fabs 0.5e f<= and ;
: dir! ( F: x z -- )  her-dir v.z sf!  0e her-dir v.y sf!  her-dir v.x sf! ;
: dir-by ( F: yaw -- )  nx nz turned dir! ;
: stick-heading ( F: -- a )  her-dir v.x sf@ her-dir v.z sf@ fatan2 ;
\ how she turns to the stick (her-turn-mode): by the camera now; the old camera steering after a
\ cut; turning round to the old camera's heading; through a door with the stick held (fiona.fs)
enum: turn-mode  camera-relative steered turning-round door-steered ;
\ (a cut: she faces where she was going, and with the stick held the old camera steers)
: after-cut ( -- )
    0 her-turn-mode !
    her-mode @ dup own-mode = swap down-mode = or 0= if  exit  then
    her-heading f@ her-yaw f!  0 her-still !
    nx nz small? if  exit  then
    3 her-lock !  steered her-turn-mode !  cam-yaw her-steer-yaw f!
    cam-yaw nx nz turned fswap fatan2 her-heading f! ;
defer settled-for-stand ( -- how )   \ (2 once her animation has faded in, else 1: fiona.moving)
\ how the direction is taken: the camera now, her facing, none, the steering camera
enum: direction-from  by-camera by-facing by-none by-steering ;
: how ( moving -- moving how )
    her-turn-mode @ case
        camera-relative of  dup if  by-camera  else  settled-for-stand  then  endof
        steered of  dup 0= if
                  by-none  her-still @ 6 = if  camera-relative her-turn-mode !  then
              else
                  by-steering
                  her-lock @ if  -1 her-lock +!  her-cam-yaw f@ her-steer-yaw f!
                  else
                      sx her-stick v.x sf@ f-  sz her-stick v.z sf@ f-  vlen 0.01e f< if
                          turning-round her-turn-mode !  0.5e deg>rad her-turn-rate f!  cam-yaw her-steer-yaw f!
                      then
                  then
                  her-steer-yaw f@ nx nz turned fswap fatan2 her-turn-to f!
              then  endof
        turning-round of  dup if
                  by-camera  nx nz fatan2  her-last v.x sf@ her-last v.z sf@ fatan2 f- angle-wrap fabs
                  15e deg>rad f> if  camera-relative her-turn-mode !  then
              else
                  by-none  her-still @ 6 = if  camera-relative her-turn-mode !  then
              then  endof
        >r by-camera r>
    endcase ;
: read-controls ( -- )
    read-stick
    sx sz vlen fdup f0> if  fdup sx fswap f/ way-x f!  sz fswap f/ way-z f!  else  fdrop 0e way-x f!  0e way-z f!  then
    her-turn-mode @ door-steered =  her-cut @ or if  after-cut  then  0 her-cut !
    sx sz small? dup if  1 her-still +!  her-still @ 6 min her-still !  else  0 her-still !  then  0=
    how nip
    case
        by-steering of  her-cam-yaw f@ dir-by  endof
        by-none of  0e 0e dir!  endof
        by-facing of  her-yaw f@ fdup fsin fswap fcos dir!  endof
        by-camera of  cam-yaw dir-by  endof
    endcase
    sx her-stick v.x sf!  0e her-stick v.y sf!  sz her-stick v.z sf!
    her-turn-mode @ camera-relative = if  cam-yaw her-cam-yaw f!  then
    her-turn-mode @ turning-round <> if  nx her-last v.x sf!  0e her-last v.y sf!  nz her-last v.z sf!  then
    run-held? her-run? ! ;

\ ---- her struggling (Fiona_Shakes): this frame's shakes - the stick swung past 120 degrees, or out
\ from rest; the buttons pressed (keys.fs) ----
: shakes-now ( -- n )   \ (Fiona_Shakes: the stick swung past 120 degrees, or out from rest; the buttons - not in the run)
    hewie-played? if  0 exit  then
    0  her-stick v.x sf@ her-stick v.z sf@ vlen                               ( n ) ( F: len )
    fdup 0.8e f<= if  0.2e f< if  -1 her-shake-rest !  then
    else
        fdrop  her-stick v.x sf@ her-stick v.z sf@ fatan2
        her-shake-rest @ if  1+  0 her-shake-rest !  her-shake-a f!
        else  fdup her-shake-a f@ f- angle-wrap fabs 2.0943952e f> if  1+  her-shake-a f!  else  fdrop  then  then
    then
    buttons-pressed + ;
\ the story's test of her shaking (Progress_FionaFlag: Fiona_Shakes as from rest) - the stick out
\ past 0.8, or a button; her own count left as it is
: shook-from-rest ( -- n )  hewie-played? if  0 exit  then  her-stick v.x sf@ her-stick v.z sf@ vlen 0.8e f> 1 and  buttons-pressed + ;
