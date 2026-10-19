\ The mods' camera (mod-camera, API 1.3) and its sample mod (examples/mods/first-person): the
\ camera at Fiona's eyes, her yaw; she walks where it looks; an event's shot takes the camera
\ back, and once it is over the mod's has it again; F4 gives it back to the game. Run with the
\ mod on (CMakeLists.txt).
IN: test-first-person
USING: tester engine game-state actors messages keys debug game vectors ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: fi ( -- id )  s" fiona" actor-named ;
: cam ( -- addr )  engine:camera ;
: eye-to-head ( -- F: d )   \ (how far the camera is from her head, across the floor)
    fi body-head {: v :}  cam cam.x sf@ v v.x sf@ f- fsq  cam cam.z sf@ v v.z sf@ f- fsq f+ fsqrt ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;

game:free-play  30 frames

testing the mod on, its camera at her eyes (1.6 ahead of her head), her way
T{ s" first-person" mod-on?  mod-camera? -> -1 -1 }T
T{ eye-to-head 1.6e f- fabs 0.1e f< -> -1 }T
T{ cam cam.yaw sf@ fi body-yaw f- fabs 0.01e f< -> -1 }T
testing she walks where it looks
fvariable z0  fi body-at v.z sf@ z0 f!   \ (her yaw 0 looks along -z)
key: W true key-hold  30 frames  key: W false key-hold
T{ fi body-at v.z sf@ z0 f@ f< -> -1 }T
testing an event's shot has the camera; over, the mod's again
T{ true cam-event!  mod-camera? -> 0 }T
T{ false cam-event!  mod-camera? -> -1 }T
testing F4: the game's camera back, and again the mod's
T{ key: F4 tap  mod-camera? -> 0 }T
T{ key: F4 tap  mod-camera? -> -1 }T

test-summary
