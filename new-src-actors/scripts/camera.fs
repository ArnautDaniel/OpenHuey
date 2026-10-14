\ camera.fs - the game's camera (docs/subsystems/camera.md): who it follows, and each
\ character's camera setup; the director in C does the rest (cuts, paths, easing). The story
\ can take a shot of its own (the event camera) and shake it.
IN: camera
USING: engine actors messages common ;

256 constant #ids   \ (actor ids: the kernel's HACTOR_MAX)

state: camera-state
  cell field followed                 \ an actor id (-1: nobody)
  cell field started                  \ a room has started it (nothing to direct before)
  #ids 2 * cells field setups         \ each actor's set and path (-1 -1: none)
  \ the story's shot (src/game/camera.c EventCam_Frame): from `dist` away at `pitch` degrees up,
  \ `yaw` degrees round from behind the one followed, looking at it raised by `rise`
  cell field shot                     \ none, wanted, or taken (where it was fixed the first frame)
  1 floats field dist   1 floats field pitch   1 floats field yaw   1 floats field rise
  3 floats field eye    3 floats field look    1 floats field toward
  1 floats field shake                \ the eye moved by up to this each way, each frame (0: still)
end-state
0 constant no-shot   1 constant shot-wanted   2 constant shot-taken

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

: thousandths ( n -- ) ( F: -- r )  s>f 1000e f/ ;
: v! ( addr -- ) ( F: x y z -- )  dup 2 floats + f!  dup float+ f!  f! ;
: v@ ( addr -- ) ( F: -- x y z )  dup f@  dup float+ f@  2 floats + f@ ;

: across ( -- ) ( F: -- d )  dist f@  pitch f@ deg>rad fcos f* ;   \ how far off, along the floor
: above ( -- ) ( F: -- d )  dist f@  pitch f@ deg>rad fsin f* ;    \ how far up
: take-shot ( who -- )   \ where it stands now fixes the shot: it doesn't follow after
    dup body-pos look v!
    look float+ f@  10e f+  rise f@ f+  look float+ f!     \ looking at its middle, raised
    body-yaw  yaw f@ deg>rad f+  toward f!                 \ the shot's heading
    look f@             toward f@ fsin across f* f-        \ the eye: behind, along that heading
    look float+ f@      above f+
    look 2 floats + f@  toward f@ fcos across f* f-
    eye v! ;
: shoot ( -- )  eye v@ look v@ cam-place ;
: shaken ( -- )   \ the eye nudged at random, up to `shake` each way
    shake f@ f0= if  exit  then
    3 0 do  rnd 2e f* 1e f-  shake f@ f*  loop  cam-nudge ;

behaviour filming
  on spawned ( -- )  -1 followed !  forget-setups
      self subscribe tick  self subscribe frame-end  self subscribe arrived  self subscribe entered-room ;
  on follow ( who -- )  dup followed !  cam-follow ;
  on camera-setup ( who set path -- )  rot setup tuck cell+ !  ! ;
  on camera-restart ( -- )  cam-restart ;
  on event-shot ( on dist pitch yaw rise -- )
      thousandths rise f!  thousandths yaw f!  thousandths pitch f!  thousandths dist f!
      if  shot-wanted shot !  true cam-event!  exit  then   \ (the director idle meanwhile: no stalker summoned)
      shot @ no-shot <> if  no-shot shot !  false cam-event!  cam-retake  then ;
  on shake-camera ( amount -- )  thousandths shake f! ;
  on arrived ( room exit -- )  2drop  forget-setups  cam-new-room  0 started !  no-shot shot !  false cam-event!  0e shake f! ;
  on entered-room ( room exit -- )  2drop  cam-room-start  -1 started ! ;
  on tick ( -- )  started @ if  cam-ease  then ;
  on frame-end ( -- )   \ after everyone has moved: the director takes the setup and places the camera
      started @ 0= if  exit  then
      followed @ dup body? 0= if  drop exit  then
      shot @ if
          shot @ shot-wanted = if  dup take-shot  shot-taken shot !  then
          drop shoot  shaken exit
      then
      dup body-pos cam-target!
      wanted cam-setup  cam-track  cam-update  shaken
      cam-changed? if  broadcast camera-cut  then ;
end-behaviour

: camera-spawn ( -- id )  filming camera-state s" camera" spawn ;
