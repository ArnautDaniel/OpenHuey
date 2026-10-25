\ The camera (docs/subsystems/camera.md; src/game/camera.c CamDirector_*, Progress_Camera*). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/camera/test_camera.fs
IN: test-camera
USING: tester engine game-state actors messages room-names game fiona.state story.state story.words debug flag-names ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
create spot 12 allot
: to-area-of ( exit -- )   \ (Fiona put in the middle of the exit's area)
    exit-area area-middle drop  spot vec!
    fiona enter  her-at spot vec-copy  her-at $28020018 v-tri-in her-tri !  leave-actor ;
: about ( F: a b -- flag )  f- fabs 0.2e f< ;

3 run-frames
front-garden-2 -1 rooms send go-to-room  5 run-frames
testing from room to room (CamDirector_RoomStart): the view angle from 0.6 of the set's, out to it over 60 frames
2 to-area-of  1 run-frames
T{ room-id -> front-garden-2 2 room-exit-leads drop }T
T{ cam-angle 33e about -> -1 }T
30 run-frames  T{ cam-angle 44e about -> -1 }T
30 run-frames  T{ cam-angle 55e about -> -1 }T
testing ... and no cut to it, no setup changed after its first frame
T{ cam-cut?  cam-changed? -> 0 0 }T

testing a new set for her (Track): a cut her controls see, and the phase 3 scripts see it changed for the frame
fiona cam-setup-of  cam-set@ 1+ 3 mod  -1 fiona -rot cam-char-setup  1 run-frames
T{ cam-cut?  cam-changed? -> -1 -1 }T
1 run-frames  T{ cam-cut?  cam-changed? -> 0 0 }T
fiona -rot cam-char-setup  2 run-frames

testing whom it follows (Progress_CameraFollow): someone without a setup falls back on Fiona
game:hewie cam-on  T{ cam-followed -> fiona }T
testing nobody: no setup taken, the camera directed still
-1 cam-on  T{ cam-followed  cam-target -> -1 -1 }T
variable was  cam-set@ was !  2 run-frames  T{ cam-set@ -> was @ }T
fiona cam-on  2 run-frames

testing a cutscene has the camera (ModeEvent): no setup seen, then it is given back (ModeDefault)
true cam-scene!  T{ cam-set@  cam-scene? -> -1 -1 }T
false cam-scene!  1 run-frames  T{ cam-set@ 0< 0=  cam-changed? -> -1 -1 }T

testing the story's shot: the room's depth range off meanwhile, back after (CamDirector_HoldEffect1C)
create range 16 allot
[: 20e 30e 2000e 2000e depth-range ;] in-room
T{ $1C range look@ 0> -> -1 }T
[: 1 3000e 20e 0e 0e event-camera ;] in-room  2 run-frames
T{ cam-event?  $1C range look@ -> -1 0 }T
[: 0 0e 0e 0e 0e event-camera ;] in-room  2 run-frames
T{ cam-event?  $1C range look@ 0> -> 0 -1 }T

testing the game over (CamDirector_NextSetup): the room's next set, nobody followed, from there on
cam-set@ was !  s" camera" actor-named send camera-hold  3 run-frames
T{ cam-set@  cam-target -> was @ 1+ room-cameras mod  -1 }T


testing out of view as the original (Camera_OutOfView): from the camera's near to its far, not 1 to 2000
: ahead ( F: d -- x y z )   \ (d along the camera's view)
    {: F: d :}  engine:camera cam.yaw sf@ engine:camera cam.pitch sf@ {: F: yaw F: pitch :}
    engine:camera cam.x sf@  yaw fsin pitch fcos f* d f* f+
    engine:camera cam.y sf@  pitch fsin engine:camera cam.up sf@ f* d f* f+
    engine:camera cam.z sf@  yaw fcos pitch fcos f* d f* f- ;
engine:camera cam.far sf@ fvalue was-far
T{ 1000e ahead out-of-view?  3000e ahead out-of-view? -> 0 -1 }T
20000e engine:camera cam.far sf!
T{ 3000e ahead out-of-view?  -100e ahead out-of-view? -> 0 -1 }T
was-far engine:camera cam.far sf!

test-summary
