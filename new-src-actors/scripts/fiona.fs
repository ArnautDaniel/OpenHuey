\ fiona.fs - Fiona, the player (docs/subsystems/fiona.md). Phase F1: her body and model, the
\ controls, standing, walking, running and turning, her footsteps, going out by exits.
IN: fiona
USING: engine actors messages common paths doors fiona.state fiona.model fiona.controls fiona.moving fiona.spots fiona.doors fiona.fear fiona.commands fiona.moves ;

: rooms-id ( -- id )  s" rooms" actor-named ;
: camera-id ( -- id )  s" camera" actor-named ;

\ ---- arriving (by an exit: on its outside spot - Rooms_ExitPointOut - her heading kept from
\ going through, as the original's: Fiona_Vt34 saves it, the scripts' 0x04 places without
\ turning) ----
fvariable ix  fvariable iy  fvariable iz
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
: arrive-at ( exit -- )   \ (the room scripts' char-to-exit: its outside spot, her heading kept)
    dup 0 exit-spot 0< if  fdrop fdrop fdrop  drop arrive-anywhere exit  then
    drop  her-yaw f@ put ;
: idle ( -- )   \ Fiona_ToIdle: free, standing, her moves her own
    0 her-doing !  0 her-mode !  her-yaw f@ her-heading f!  0 her-rest !  0 her-turn-mode !
    -1 idle-anim  ['] idle-move her-act ! ;
' idle is fiona.doors:to-idle   ' idle is fiona.fear:to-idle   ' idle is fiona.commands:to-idle   ' idle is fiona.moves:to-idle
: fresh ( -- )   \ standing, the controls as new
    0 her-doing !  0 her-mode !  0 her-sub !  6 her-still !  0 her-turn-mode !  0 her-lock !
    1e her-stick-k f!  -1 her-variant !  1e her-tired-w f!  0 her-rest !  0 her-run-t !
    -1 her-cmd !  0 her-busy-t !  0 her-scripted !  0 her-move !  -1 her-move-done !  1 her-step-l !  1 her-step-r !  -1 her-door-exit !  her-path path-clear  stand  idle ;

\ ---- walking to a point (go-to: the console's, later the story's moves): her way over the nav
\ mesh, arriving facing the way she walked ----
create goal 12 allot
: going ( -- )  walk-to-spot dup 0< if  drop idle exit  then  if  exit  then  idle ;
: go-to-point ( F: x y z -- )
    goal vec!  goal blocked-floor v-tri-in dup 0< if  drop exit  then
    goal vec@  her-at goal vec-heading  walk-spot
    2 her-mode !  $14 her-doing !  ['] going her-act ! ;

\ ---- each frame (Fiona_Update, the parts built so far) ----
: frame ( -- )
    her-at her-was-at 12 move  her-tri @ her-was-tri !  her-yaw f@ her-yaw-was f!
    feel-the-panic
    read-controls
    her-mode @ 0= if  panic-controls  else  false  then  0=  hands-off? 0= and if  control-command  then
    hands-off? if  -1 her-cmd !  else  read-command  try-flee  try-doors  then
    her-act @ execute
    command-sounds
    footsteps
    show-her ;

behaviour free
  on spawned ( -- )
      s" O_FIN/FIN_000" actor-load dup her-model !  0 over cast-as  $3D5CC0 motion-table   \ (CharModel_SecondaryMotion: her fades and flags)
      1 her act.visible l!  2e 15e body-size
      self subscribe tick  self subscribe arrived  self subscribe camera-cut
      self subscribe danger  self subscribe panic  self subscribe hewie-doing  self subscribe frame-end  self subscribe text-shown  self subscribe scene ;
  on arrived ( room exit -- )  nip arrive-at  fresh  self camera-id send follow ;   \ (the story's scripts place her too)
  on to-exit ( exit -- )  arrive-at  fresh ;
  \ examining (Progress_PlayerButtons): an action offered here, taken with the action button
  \ while she is free; a script of the room has her, or lets go; a message on screen
  on offer ( scene arg -- )
      her-mode @ 0= her-doing @ 0= and  hands-off? 0= and  action-button? and
      if  sender send take-offer  else  2drop  then ;
  on scripted ( on -- )  idle  her-scripted ! ;
  on text-shown ( on -- )  her-reading ! ;
  on scene ( on -- )  her-in-scene ! ;   \ (a cutscene has her)
  on go-to ( x y z -- )  >r >r cell>f r> cell>f r> cell>f  go-to-point ;
  on place ( x y z yaw -- )  >r >r >r cell>f r> cell>f r> cell>f r> cell>f  put  fresh ;   \ (the story puts her)   \ (the room's scripts: Rooms_ExitPointOut, facing in)
  on camera-cut ( -- )  -1 her-cut ! ;
  on danger ( level -- )  her-danger ! ;
  on panic ( stage level -- )  cell>f her-panic-level f!  her-panic-stage ! ;
  on door-held ( room exit -- )  2drop  got-hold ;
  on door-refused ( room exit -- )  2drop  idle ;
  on tick ( -- )  self body? if  frame  then ;
  on frame-end ( -- )   \ what she is doing, as the frame left her; her move
      self body? 0= if  exit  then
      her-mode @ her-sub @ her-cond @ her-cmd @ broadcast fiona-doing
      her-move-done @  ended?  her-model @ 0 0 1 motion-events  broadcast moving ;
  \ the story's moves (fiona.moves)
  on scripted-move ( kind a b x y z yaw -- )
      cell>f her-move-yaw f!  >r >r cell>f r> cell>f r> cell>f her-move-at vec!
      her-move-b !  her-move-a !  dup her-move !  start-move ;
  on hold-anim ( anim blend -- )   \ (the scripts' 0x9D: in it, held)
      -1 play-blend  2 her-mode !  $11 her-doing !  ['] root-move her-act !  done ;
  on show ( on -- )  0<> 1 and her act.visible l! ;
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
