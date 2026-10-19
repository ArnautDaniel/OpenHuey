\ first-person: the game through Fiona's eyes. The mods' camera (mod-camera) sits at her head as
\ she tells it each frame (body-head) and the mouse looks round - held in the window - or the
\ arrow keys; she walks where you look (her controls go by the camera, as ever). Look down and
\ there are her feet: the camera's near plane (3) leaves her own head out of the picture. F4
\ turns it off and on. In a cutscene or an event's shot the game's camera has it, as before.
\
\ The sample of the mods' camera: an actor of its own; it reads the game (her body) and moves
\ only its own camera.
IN: first-person
USING: engine actors messages vectors ;

0.0025e fconstant mouse-turn     \ radians a pixel of the mouse
0.04e fconstant key-turn         \ radians a frame an arrow key held
1.4e fconstant steepest          \ how far up or down it looks (radians)
1.6e fconstant ahead             \ the eye this much in front of her head's middle
3e fconstant nearest               \ nothing nearer the eye drawn: her own head (round its middle, 1.5) left out

state: eyes-state
  cell field through-her-eyes    \ (F4)
  cell field holding             \ the mouse held
  cell field had                 \ its camera had the view last frame (back from a scene: facing her way again)
  1 floats field yaw  1 floats field pitch
end-state

: fiona ( -- id )  s" fiona" actor-named ;
: her-head? ( -- flag )  fiona dup 0< if  drop false exit  then  body? ;
: cam ( -- addr )  mod-camera ;
: look ( -- )   \ the mouse, or the arrows
    mouse-dx mouse-turn f*  yaw f@ f+  yaw f!
    pitch f@  mouse-dy mouse-turn f* f-  pitch f!
    key: Left key-down? if  yaw f@ key-turn f- yaw f!  then
    key: Right key-down? if  yaw f@ key-turn f+ yaw f!  then
    key: Up key-down? if  pitch f@ key-turn f+ pitch f!  then
    key: Down key-down? if  pitch f@ key-turn f- pitch f!  then
    pitch f@ steepest fnegate fmax steepest fmin pitch f! ;
: place ( -- )   \ at her eyes: her head, a little ahead the way she faces
    fiona body-head {: v :}  fiona body-yaw {: F: face :}
    v v.x sf@  face fsin ahead f* f+  cam cam.x sf!
    v v.y sf@                         cam cam.y sf!
    v v.z sf@  face fcos ahead f* f-  cam cam.z sf!
    yaw f@ cam cam.yaw sf!  pitch f@ cam cam.pitch sf!
    1.15e cam cam.fov sf!  nearest cam cam.near sf! ;   \ (a wider view than the game's shots: about 66 degrees up and down)
: hold ( flag -- )   \ the mouse held in the window while it looks
    dup holding @ = if  drop exit  then  dup holding !  mouse-capture ;
: face-her-way ( -- )   \ (her yaw and the camera's turn the same way: x by sin, z by -cos)
    fiona body-yaw yaw f!  0e pitch f! ;
: start ( -- )  -1 mod-camera!  0 had ! ;
: frame ( -- )
    key: F4 key-pressed? if  through-her-eyes @ 0= through-her-eyes !  through-her-eyes @ if  start  else  0 mod-camera!  then  then
    through-her-eyes @ her-head? and 0= if  false hold  0 mod-camera!  exit  then
    -1 mod-camera!  mod-camera? hold   \ (its camera: not while a cutscene's or an event's shot has it)
    mod-camera? had @ 0= and if  face-her-way  then  mod-camera? had !
    mod-camera? if  look place  then ;

behaviour seeing
  on spawned ( -- )  -1 through-her-eyes !  0 holding !  start  self subscribe tick ;
  on tick ( -- )  frame ;
end-behaviour

seeing eyes-state s" first-person" spawn drop
