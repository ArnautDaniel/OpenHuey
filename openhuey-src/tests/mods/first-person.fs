\ The mods' camera (mod-camera, API 1.3) and its sample mod (examples/mods/first-person): the
\ camera at Fiona's eyes, her yaw; she walks where it looks; an event's shot takes the camera
\ back, and once it is over the mod's has it again; F4 gives it back to the game. Run with the
\ mod on (CMakeLists.txt).
IN: test-first-person
USING: tester engine game-state actors messages keys debug game vectors ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: fi ( -- id )  s" fiona" actor-named ;
: cam ( -- addr )  engine:camera ;
: ahead-of-her ( -- F: d )   \ (how far the camera is in front of her, the way she faces: x by
    \ sin, z by +cos of her yaw)
    fi body-at {: v :}  fi body-yaw {: F: f :}
    cam cam.x sf@ v v.x sf@ f- f fsin f*  cam cam.z sf@ v v.z sf@ f- f fcos f* f+ ;
: looking-her-way? ( -- flag )   \ (the camera looks x by sin, z by -cos of its yaw: hers turned round)
    pi fi body-yaw f- cam cam.yaw sf@ f- fsin fabs 0.01e f< ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;

game:free-play  30 frames

testing the mod on, its camera at her eyes (1.6 in front of her), looking her way
T{ s" first-person" mod-on?  mod-camera? -> -1 -1 }T
T{ ahead-of-her 1.6e f- fabs 0.1e f<  looking-her-way? -> -1 -1 }T
testing she runs where it looks - the camera still in front of her, not a frame behind
fvariable x0  fvariable z0  fi body-at v.x sf@ x0 f!  fi body-at v.z sf@ z0 f!
key: W true key-hold  key: X true key-hold  30 frames
T{ fi body-at v.x sf@ x0 f@ f-  cam cam.yaw sf@ fsin f*  fi body-at v.z sf@ z0 f@ f-  cam cam.yaw sf@ fcos fnegate f*  f+  10e f> -> -1 }T
T{ ahead-of-her 1.6e f- fabs 0.1e f<  looking-her-way? -> -1 -1 }T
key: W false key-hold  key: X false key-hold
testing an event's shot has the camera; over, the mod's again
T{ true cam-event!  mod-camera? -> 0 }T
T{ false cam-event!  mod-camera? -> -1 }T
testing F4: the game's camera back, and again the mod's
T{ key: F4 tap  mod-camera? -> 0 }T
T{ key: F4 tap  mod-camera? -> -1 }T

test-summary
