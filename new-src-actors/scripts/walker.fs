\ walker.fs - a debug stand-in for Fiona until she is built: her model, walked with the keys
\ over the nav mesh, out by an exit with Space while in its area. It is the controlled
\ character: it tells the camera to follow it and the rooms when it goes out.
\   W A S D  walk (relative to the view)     Shift  run     Space  go out by this exit
IN: walker
USING: engine keys vectors actors messages ;

$000 constant m-idle   $200 constant m-walk   $202 constant m-run   \ (her motions)
0.5e fconstant walk-speed    1.4e fconstant run-speed     \ units a frame
0.36e fconstant turn-speed                                \ radians a frame
8e fconstant step-up   3e fconstant clearance
$28020018 constant blocking                               \ the floor she can't stand on (the game's mask)

state: walker-state
  cell field model      \ her model (an Actor)
  cell field moving     \ the motion playing
end-state

: rooms ( -- id )  s" rooms" actor-named ;
: camera ( -- id )  s" camera" actor-named ;
: me ( -- addr )  model @ actor ;

\ the body where the model is, and the model where the body is
: model-to-body ( -- )
    room-id  me act.x sf@ me act.y sf@ me act.z sf@ nav-tri  >r
    me act.x sf@ me act.y sf@ me act.z sf@  r> body-place
    me act.yaw sf@ body-turn ;
fvariable px  fvariable py  fvariable pz
: put ( F: x y z yaw -- )
    me act.yaw sf!  pz f! py f! px f!
    px f@ me act.x sf!  pz f@ me act.z sf!
    px f@ py f@ step-up f+ pz f@ floor-below if  me act.y sf!  else  py f@ me act.y sf!  then
    model-to-body ;

: arrive-anywhere ( -- )   \ (no exit: by the room's first camera's target, else its middle)
    room-cameras if
        0 room-camera  pz f! py f! px f!  fdrop fdrop fdrop fdrop  px f@ py f@ pz f@
    else  0e 0e 0e  then
    nav-tris if  nav-nearest  then  0e put ;
\ arriving by an exit: on its spot inside, facing in from its spot outside
fvariable qx  fvariable qy  fvariable qz
: arrive-by ( exit -- )
    dup 0 exit-spot 0< if  fdrop fdrop fdrop  drop arrive-anywhere exit  then
    pz f! fdrop px f!
    1 exit-spot 0< if  fdrop fdrop fdrop  px f@ 0e pz f@ 0e put exit  then
    qz f! qy f! qx f!
    qx f@ qy f@ qz f@  qx f@ px f@ f-  qz f@ pz f@ f-  fatan2  put ;

\ ---- walking ----
fvariable mx  fvariable mz
: wanted ( -- )
    0e mx f!  0e mz f!
    key: W held? if  yaw fsin mx f!  yaw fcos fnegate mz f!  then
    key: S held? if  yaw fsin fnegate mx f!  yaw fcos mz f!  then
    key: D held? if  mx f@ yaw fcos f+ mx f!  mz f@ yaw fsin f+ mz f!  then
    key: A held? if  mx f@ yaw fcos f- mx f!  mz f@ yaw fsin f- mz f!  then ;
: fwrap ( F: a -- a' )  fdup pi f+ pi f2* f/ ffloor pi f2* f* f- ;
: turn-toward ( F: yaw -- )
    me act.yaw sf@ f- fwrap  turn-speed fnegate turn-speed fclamp  me act.yaw sf@ f+ fwrap  me act.yaw sf! ;
: step ( F: distance -- )
    nav-tris 0= if  fdrop exit  then
    fdup me act.yaw sf@ fsin f* px f!  me act.yaw sf@ fcos f* pz f!
    blocking nav-block!
    me act.x sf@ me act.y sf@ me act.z sf@  px f@ pz f@ step-up clearance nav-move
    0 nav-block!
    me act.z sf!  me act.y sf!  me act.x sf! ;
: play ( motion -- )  dup moving @ = if  drop exit  then  dup moving !  model @ swap motion! ;
: walk ( -- )
    wanted
    mx f@ f0= mz f@ f0= and if  m-idle play exit  then
    mx f@ mz f@ fatan2 turn-toward
    key: Left_Shift held? if  run-speed step  m-run play  else  walk-speed step  m-walk play  then ;

\ ---- going out ----
: exit-here ( -- exit | -1 )   \ the exit whose area it stands in
    8 0 do
        i exit-area dup area-count < if
            me act.x sf@ me act.y sf@ me act.z sf@  area-in? if  i unloop exit  then
        else  drop  then
    loop  -1 ;

behaviour walking
  on spawned ( -- )
      s" O_FIN/FIN_000" actor-load model !  1 me act.visible l!  -1 moving !
      2e 15e body-size  self subscribe tick  self subscribe arrived ;
  on arrived ( room exit -- )  nip  dup 0< if  drop arrive-anywhere  else  arrive-by  then
      m-idle play  self camera send follow ;
  on tick ( -- )
      self body? 0= if  exit  then
      walk  model-to-body
      key: Space pressed? if  exit-here dup 0< if  drop  else  rooms send go-through  then  then ;
end-behaviour

: walker-spawn ( -- id )  walking walker-state s" walker" spawn ;
