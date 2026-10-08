\ fiona.fs - Fiona, the player (docs/subsystems/fiona.md). Phase F1: her body and model, the
\ controls, standing, walking, running and turning, her footsteps, going out by exits.
IN: fiona
USING: engine actors messages common paths doors fiona.state fiona.model fiona.controls fiona.moving fiona.spots fiona.doors fiona.fear fiona.commands ;

: rooms-id ( -- id )  s" rooms" actor-named ;
: camera-id ( -- id )  s" camera" actor-named ;

\ ---- arriving (by an exit: on its spot inside, facing in from its spot outside) ----
fvariable ox  fvariable oz  fvariable ix  fvariable iy  fvariable iz
: put ( F: x y z yaw -- )
    fdup her-yaw f!  her-heading f!
    her-at 8 + sf!  her-at 4 + sf!  her-at sf!
    her-at blocked-floor v-tri-in dup 0< if  drop her-at v-tri  then  her-tri !
    her-at her-was-at 12 move  her-tri @ her-was-tri !
    show-her ;
: arrive-anywhere ( -- )   \ (no exit: by the room's first camera's target, else its middle)
    room-cameras if  0 room-camera  iz f! iy f! ix f!  fdrop fdrop fdrop fdrop  ix f@ iy f@ iz f@
    else  0e 0e 0e  then
    nav-tris if  nav-nearest  then  0e put ;
: arrive-at ( exit -- )   \ (the room scripts' char-to-exit: its outside spot, facing in)
    dup 0 exit-spot 0< if  fdrop fdrop fdrop  drop arrive-anywhere exit  then
    iz f! iy f! ix f!
    1 exit-spot 0< if  fdrop fdrop fdrop  ix f@ iy f@ iz f@ 0e put exit  then
    oz f! fdrop ox f!
    ix f@ iy f@ iz f@  ox f@ ix f@ f-  oz f@ iz f@ f-  fatan2  put ;
: idle ( -- )   \ Fiona_ToIdle: free, standing, her moves her own
    0 her-doing !  0 her-mode !  her-yaw f@ her-heading f!  0 her-rest !  0 her-turn-mode !
    -1 idle-anim  ['] idle-move her-act ! ;
' idle is fiona.doors:to-idle   ' idle is fiona.fear:to-idle   ' idle is fiona.commands:to-idle
: fresh ( -- )   \ standing, the controls as new
    0 her-doing !  0 her-mode !  0 her-sub !  6 her-still !  0 her-turn-mode !  0 her-lock !
    1e her-stick-k f!  -1 her-variant !  1e her-tired-w f!  0 her-rest !  0 her-run-t !
    -1 her-cmd !  0 her-busy-t !  1 her-step-l !  1 her-step-r !  -1 her-door-exit !  her-path path-clear  stand  idle ;

\ ---- each frame (Fiona_Update, the parts built so far) ----
: frame ( -- )
    her-at her-was-at 12 move  her-tri @ her-was-tri !  her-yaw f@ her-yaw-was f!
    feel-the-panic
    read-controls
    her-mode @ 0= if  panic-controls  else  false  then  0= if  control-command  then
    read-command
    try-flee
    try-doors
    her-act @ execute
    command-sounds
    footsteps
    show-her ;

behaviour free
  on spawned ( -- )
      s" O_FIN/FIN_000" actor-load her-model !  1 her act.visible l!  2e 15e body-size
      self subscribe tick  self subscribe arrived  self subscribe camera-cut
      self subscribe danger  self subscribe panic  self subscribe hewie-doing  self subscribe frame-end ;
  on arrived ( room exit -- )  nip arrive-at  fresh  self camera-id send follow ;   \ (the story's scripts place her too)
  on to-exit ( exit -- )  arrive-at  fresh ;   \ (the room's scripts: Rooms_ExitPointOut, facing in)
  on camera-cut ( -- )  -1 her-cut ! ;
  on danger ( level -- )  her-danger ! ;
  on panic ( stage level -- )  cell>f her-panic-level f!  her-panic-stage ! ;
  on door-held ( room exit -- )  2drop  got-hold ;
  on door-refused ( room exit -- )  2drop  idle ;
  on tick ( -- )  self body? if  frame  then ;
  on frame-end ( -- )   \ what she is doing, as the frame left her
      self body? if  her-mode @ her-sub @ her-cond @ her-cmd @ broadcast fiona-doing  then ;
  \ Hewie (F3)
  on hewie-doing ( here action mode sub cond mood group -- )
      dog-group !  dog-mood !  dog-cond !  dog-sub !  dog-mode !  dog-action !  dog-here ! ;
  on meet-at ( tri x y z face -- )
      asked? 0= if  2drop 2drop drop  hewie-id send meet-off exit  then
      >r >r >r cell>f  r> cell>f  r> cell>f  r> cell>f  meet-placed ;
  on meet-on ( -- )  her-act @ ['] meet-waiting = if  meet-started  else  hewie-id send meet-off  then ;
  on meet-refused ( -- )  her-act @ ['] meet-waiting = if  meet-failed  then ;
end-behaviour

: fiona-spawn ( -- id )  free fiona-state s" fiona" spawn ;
