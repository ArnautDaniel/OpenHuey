\ fiona.fs - Fiona, the player (docs/subsystems/fiona.md). Phase F1: her body and model, the
\ controls, standing, walking, running and turning, her footsteps, going out by exits.
IN: fiona
USING: engine actors messages common paths doors fiona.state fiona.model fiona.controls fiona.moving fiona.spots fiona.doors ;

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
' idle is to-idle
: fresh ( -- )   \ standing, the controls as new
    0 her-doing !  0 her-mode !  0 her-sub !  6 her-still !  0 her-turn-mode !  0 her-lock !
    1e her-stick-k f!  -1 her-variant !  1e her-tired-w f!  0 her-rest !  0 her-run-t !
    1 her-step-l !  1 her-step-r !  -1 her-door-exit !  her-path path-clear  stand  idle ;

\ ---- going out (a stand-in until the story: the room scripts take an exit while she is in its
\ area and free - each frame. So that an arrival can't bounce her back out, only once she has
\ been out of every exit's area since arriving) ----
: in-exit-area ( -- exit | -1 )
    8 0 do
        i exit-area dup area-count < if  her-pos area-in? if  i unloop exit  then  else  drop  then
    loop  -1 ;
: went-out ( -- exit | -1 )
    in-exit-area  dup 0< if  -1 her-armed !  exit  then
    her-armed @ 0= her-doing @ 0<> or if  drop -1  then ;

\ ---- each frame (Fiona_Update, the parts built so far) ----
: frame ( -- )
    her-at her-was-at 12 move  her-tri @ her-was-tri !  her-yaw f@ her-yaw-was f!
    read-controls
    try-doors
    her-act @ execute
    footsteps
    show-her
    went-out dup 0< if  drop  else  rooms-id send go-through  then ;

behaviour free
  on spawned ( -- )
      s" O_FIN/FIN_000" actor-load her-model !  1 her act.visible l!  2e 15e body-size
      self subscribe tick  self subscribe arrived  self subscribe camera-cut ;
  on arrived ( room exit -- )  nip arrive-at  fresh  0 her-armed !  self camera-id send follow ;
  on camera-cut ( -- )  -1 her-cut ! ;
  on door-held ( room exit -- )  2drop  got-hold ;
  on door-refused ( room exit -- )  2drop  idle ;
  on tick ( -- )  self body? if  frame  then ;
end-behaviour

: fiona-spawn ( -- id )  free fiona-state s" fiona" spawn ;
