\ player.fs - playing as Fiona. Tab switches between the free camera and her.
\   W A S D  walk (relative to the view)     Shift  run
\ The camera takes the room's own setup nearest to her and keeps her in sight - roughly what
\ the game's camera director does - or follows behind her in a room without setups.
IN: player
USING: engine keys vectors views state ;

variable fiona        -1 fiona !

$000 constant m-idle
$200 constant m-walk
$202 constant m-run

fvariable walk-speed   0.25e walk-speed f!     \ units a tick (60 ticks a second)
fvariable run-speed    0.7e run-speed f!
fvariable turn-speed   0.18e turn-speed f!     \ radians a tick
8e fconstant step-up                           \ the highest step she climbs
3e fconstant body                              \ how far she keeps from walls

: her ( -- addr ) fiona @ actor ;
: her-pos ( F: -- x y z )  her act.x sf@  her act.y sf@  her act.z sf@ ;

\ angles: wrap into -pi..pi
: fwrap ( F: a -- a' )  fdup pi f+ pi f2* f/ ffloor pi f2* f* f- ;

\ ---- where to stand ----
\ the floor under a point, from a little above it (flag false: nothing there)
: ground ( F: x y z -- y' ) ( -- flag )  frot frot step-up f+ frot floor-below ;

fvariable px  fvariable py  fvariable pz
: place-fiona ( F: x y z -- )
    pz f! py f! px f!
    px f@ her act.x sf!  pz f@ her act.z sf!
    px f@ py f@ pz f@ ground if  her act.y sf!  else  py f@ her act.y sf!  then ;

\ the first view's target: where the game expects someone to be
: spawn-fiona
    fiona @ 0< if  s" O_FIN/FIN_000" actor-load fiona !  then
    room-cameras if
        0 room-camera  tz f! ty f! tx f!  fdrop fdrop fdrop fdrop
        tx f@ ty f@ tz f@  nav-tris if  nav-nearest  then  place-fiona
    else
        cam-pos place-fiona
    then
    fiona @ m-idle motion! ;
' spawn-fiona is place-player

\ ---- moving ----
fvariable mx  fvariable mz      \ the wanted direction this tick (on the ground)

\ the keys as a direction relative to the view
: wanted
    0e mx f!  0e mz f!
    key: W held? if  yaw fsin mx f!  yaw fcos fnegate mz f!  then
    key: S held? if  yaw fsin fnegate mx f!  yaw fcos mz f!  then
    key: D held? if  mx f@ yaw fcos f+ mx f!  mz f@ yaw fsin f+ mz f!  then
    key: A held? if  mx f@ yaw fcos f- mx f!  mz f@ yaw fsin f- mz f!  then ;
: moving? ( -- flag )  mx f@ f0= mz f@ f0= and 0= ;

: turn-toward ( F: target-yaw -- )
    her act.yaw sf@ f- fwrap
    turn-speed f@ fnegate turn-speed f@ fclamp
    her act.yaw sf@ f+ fwrap her act.yaw sf! ;

\ a step along the direction she faces: on the nav mesh (sliding along walls), or - in a room
\ without one - anywhere there is floor that isn't a drop
: step-floor ( F: distance -- )
    fdup her act.yaw sf@ fsin f* her act.x sf@ f+ px f!
    her act.yaw sf@ fcos f* her act.z sf@ f+ pz f!
    px f@ her act.y sf@ pz f@ ground 0= if  exit  then     ( F: y' )
    fdup her act.y sf@ 30e f- f< if  fdrop exit  then
    her act.y sf!  px f@ her act.x sf!  pz f@ her act.z sf! ;

: step-nav ( F: distance -- )
    fdup her act.yaw sf@ fsin f* px f!  her act.yaw sf@ fcos f* pz f!
    $28020018 nav-block!                         \ (the triangles that stop her: the game's mask)
    her-pos px f@ pz f@ step-up body nav-move  0 nav-block!
    her act.z sf!  her act.y sf!  her act.x sf! ;

: step ( F: distance -- )  nav-tris if  step-nav  else  step-floor  then ;

: walk
    wanted
    moving? if
        mx f@ mz f@ fatan2 turn-toward
        key: Left_Shift held? if  run-speed f@ step  fiona @ m-run motion!
        else  walk-speed f@ step  fiona @ m-walk motion!  then
    else
        fiona @ m-idle motion!
    then ;

\ ---- the camera ----
: dist2 ( F: x y z -- d2 )  her act.z sf@ f- fsq frot her act.x sf@ f- fsq f+ fswap her act.y sf@ f- fsq f+ ;

variable best  fvariable best-d
: nearest-view ( -- n )
    -1 best !  1e30 best-d f!
    room-cameras 0 ?do
        i room-camera  dist2 fswap fdrop fswap fdrop fswap fdrop fswap fdrop   ( F: target-d2 )
        fdup best-d f@ f< if  best-d f!  i best !  else  fdrop  then
    loop best @ ;

: chase   \ behind and above her, looking at her
    her act.yaw sf@ fsin -60e f* her act.x sf@ f+
    her act.y sf@ 35e f+
    her act.yaw sf@ fcos -60e f* her act.z sf@ f+  cam-at ;

: follow
    room-cameras if
        nearest-view dup view @ <> if  dup view !  then
        room-camera  fdrop fdrop fdrop  camera cam.fov sf!  cam-at
    else
        chase
    then
    her-pos fswap 11e f+ fswap look-at ;    \ (her chest: 11 units up)

\ what moves the camera while playing (events/play.fs gives it to the game's camera director)
defer steer-camera  ' follow is steer-camera
: play  playing @ if  0 scripted? 0= if  walk  then  steer-camera  then ;
' play on-tick

\ ---- switching ----
: start-playing  fiona @ 0< if  place-player  then  -1 playing !  ." playing (Tab: free camera)" cr ;
: stop-playing  0 playing !  ." free camera (Tab: play)" cr ;
: play-keys
    title @ if  exit  then
    key: Tab key-pressed? if  playing @ if  stop-playing  else  start-playing  then  then ;
' play-keys on-tick
