\ camera.fs - the game's camera (docs/subsystems/camera.md). The director in C (game/camdirector.c)
\ keeps whom it follows and each character's setup, and does the cuts, the paths and the easing;
\ the story calls it itself where the original does (story/words.fs: cam-char-setup, cam-on,
\ cam-shake!, cam-restart; story/room.fs: the room's start). This actor runs it each frame in the
\ original's order (SceneGame's play frame: CamDirector_Ease first, then whom it follows and their
\ setup, CamDirector_Track, CamDirector_Update) and takes the story's own shot of the one followed.
IN: camera
USING: engine actors messages common ;

state: camera-state
  cell field held                     \ the game over: the room's next set, nobody followed, no setups taken
  \ the story's shot (EventCam_Frame): from `dist` away at `pitch` degrees up, `yaw` degrees round
  \ from the heading the one followed had as it started, looking at the point it started on (its
  \ middle, raised by `rise`)
  cell field shot                     \ none, wanted, or taken
  1 floats field dist   1 floats field pitch   1 floats field yaw   1 floats field rise
  3 floats field look   1 floats field heading   1 floats field toward
  \ the room's depth range (look slot $1C), off during the shot (CamDirector_HoldEffect1C)
  cell field fog-held   16 field fog
end-state
0 constant no-shot   1 constant shot-wanted   2 constant shot-taken

: thousandths ( n -- ) ( F: -- r )  s>f 1000e f/ ;
: v! ( addr -- ) ( F: x y z -- )  dup 2 floats + f!  dup float+ f!  f! ;
: v@ ( addr -- ) ( F: -- x y z )  dup f@  dup float+ f@  2 floats + f@ ;

\ ---- the shot ----
: take-shot ( -- )   \ (its first frame: the point and the heading fixed)
    cam-target  dup body-pos look v!
    look float+ f@  10e f+  rise f@ f+  look float+ f!     \ (followed 10 up, then raised)
    body-yaw heading f!  shot-taken shot ! ;
: across ( -- ) ( F: -- d )  dist f@  pitch f@ deg>rad fcos f* ;   \ how far off, along the floor
: above ( -- ) ( F: -- d )  dist f@  pitch f@ deg>rad fsin f* ;    \ how far up
: shoot ( -- )   \ (each frame: dist, pitch and yaw as they are now; the eye behind along the heading)
    heading f@  yaw f@ deg>rad f+  toward f!
    look f@             toward f@ fsin across f* f-
    look float+ f@      above f+
    look 2 floats + f@  toward f@ fcos across f* f-
    look v@ cam-place ;
: hold-fog ( -- )   \ (kept and taken off, if the room has one)
    fog-held @ if  exit  then
    $1C fog look@ 0> if  -1 fog-held !  $1C 0 0 look-set  then ;
: give-fog ( -- )  fog-held @ 0= if  exit  then  0 fog-held !  $1C fog 16 look-set ;
: shot-over ( -- )   \ (the setup taken anew)
    false cam-event!  shot @ no-shot <> if  no-shot shot !  cam-retake  then ;

: fiona ( -- id )  s" fiona" actor-named ;
: fresh-start ( -- )   \ (the characters made anew: their setups none - hers as the save had it)
    fiona cam-setup-of  cam-forget-setups  fiona -rot cam-char-setup
    cam-new-room  0e cam-shake!  no-shot shot !  0 fog-held !  fiona cam-on ;

behaviour filming
  on spawned ( -- )  cam-forget-setups  self subscribe tick  self subscribe frame-end  self subscribe arrived ;
  \ a fresh start (a new game, a load - SceneGame_RoomIn: CamDirector_NewRoom, the camera on
  \ Fiona); from room to room the director goes on - its shake, its shot, whom it follows - and
  \ only the characters' setups go. The room's script starts it (story/room.fs).
  on arrived ( room exit -- )
      nip 0 held !  cam-stop
      0< if  fresh-start  else  cam-forget-setups  then ;
  on camera-restart ( -- )  cam-restart ;
  \ the story's shot (0x82: EventCam_Set, CamDirector_HoldEffect1C): on - the distance and angles
  \ (thousandths), the point fixed as it starts; again while on - the new distance and angles only
  on event-shot ( on dist pitch yaw rise -- )
      thousandths rise f!  thousandths yaw f!  thousandths pitch f!  thousandths dist f!
      if  shot @ no-shot = if  shot-wanted shot !  then  true cam-event!  hold-fog exit  then
      give-fog  shot-over ;
  \ the game over (GameOver: CamDirector_NextSetup): the room's next set, from there on
  on camera-hold ( -- )  -1 held !  cam-next-set ;
  on tick ( -- )  cam-started? if  cam-ease  then ;
  on frame-end ( -- )   \ (after everyone has moved)
      cam-started? 0= if  exit  then
      held @ 0= if  cam-take-followed  then
      cam-track
      cam-target 0< if  shot-over  then   \ (following nobody: no shot)
      cam-update
      shot @ shot-wanted = if  take-shot  then
      shot @ if  shoot  then
      cam-cut? if  broadcast camera-cut  then ;
end-behaviour

: camera-spawn ( -- id )  filming camera-state s" camera" spawn ;
