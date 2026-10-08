\ camera.fs - the game's camera (docs/subsystems/camera.md): who it follows, and each
\ character's camera setup; the director in C does the rest (cuts, paths, easing).
IN: camera
USING: engine actors messages ;

256 constant #ids   \ (actor ids: the kernel's HACTOR_MAX)

state: camera-state
  cell field followed                 \ an actor id (-1: nobody)
  cell field started                  \ a room has started it (nothing to direct before)
  #ids 2 * cells field setups         \ each actor's set and path (-1 -1: none)
end-state

: setup ( who -- addr )  2 * cells setups + ;
: forget-setups ( -- )  #ids 0 do  -1 i setup !  -1 i setup cell+ !  loop ;

\ (stand-in until the story gives setups: the room's set looking nearest the one followed)
fvariable bx  fvariable bz  fvariable best-d  variable best
: nearest-set ( who -- set )
    body-pos bz f! fdrop bx f!
    -1 best !  1e30 best-d f!
    room-cameras 0 ?do
        i room-camera  bz f@ f- fsq  fswap fdrop  fswap bx f@ f- fsq f+   ( F: ex ey ez fov d2 )
        fswap fdrop fswap fdrop fswap fdrop fswap fdrop
        fdup best-d f@ f< if  best-d f!  i best !  else  fdrop  then
    loop  best @ ;
: wanted ( who -- set path )
    dup setup @ 0< if  nearest-set -1 exit  then
    dup setup @ swap setup cell+ @ ;

behaviour filming
  on spawned ( -- )  -1 followed !  forget-setups
      self subscribe tick  self subscribe frame-end  self subscribe arrived  self subscribe entered-room ;
  on follow ( who -- )  dup followed !  cam-follow ;
  on camera-setup ( who set path -- )  rot setup tuck cell+ !  ! ;
  on camera-restart ( -- )  cam-restart ;
  on arrived ( room exit -- )  2drop  forget-setups  cam-new-room  0 started ! ;
  on entered-room ( room exit -- )  2drop  cam-room-start  -1 started ! ;
  on tick ( -- )  started @ if  cam-ease  then ;
  on frame-end ( -- )   \ after everyone has moved: the director takes the setup and places the camera
      started @ 0= if  exit  then
      followed @ dup body? 0= if  drop exit  then
      dup body-pos cam-target!
      wanted cam-setup  cam-track  cam-update
      cam-changed? if  broadcast camera-cut  then ;
end-behaviour

: camera-spawn ( -- id )  filming camera-state s" camera" spawn ;
