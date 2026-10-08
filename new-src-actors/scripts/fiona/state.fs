\ fiona/state.fs - Fiona's state: one actor's fields (docs/subsystems/fiona.md). `fiona enter`
\ at the console shows them.
IN: fiona.state
USING: actors paths ;

state: fiona-state
  cell field her-model          \ her model (an Actor)
  cell field her-doing          \ what she is busy with: 0 free (see `busy-*` in fiona.fs)
  cell field her-mode           \ how she moves (0 on her own; more with the later phases)
  cell field her-sub            \ within it: 0 standing, 1 walking, 2 running, 5 pushing...
  12 field her-at               \ where she is (3 single floats: x y z)
  12 field her-was-at           \ and was at the frame's start
  cell field her-tri            \ her nav triangle (-1 none)
  cell field her-was-tri
  1 floats field her-yaw        \ her heading
  1 floats field her-yaw-was
  1 floats field her-heading    \ the heading she turns to
  \ the controls (Fiona_MoveInput)
  12 field her-dir              \ the stick as a direction in the room (x 0 z)
  12 field her-stick            \ the stick last frame (x 0 z)
  12 field her-last             \ its direction, kept while the old camera steers
  cell field her-still          \ frames the stick has been still (6: let go)
  cell field her-turn-mode      \ 0 camera-relative, 1 the old camera steers, 2 turning to its heading
  cell field her-lock           \ frames the old camera keeps steering regardless
  1 floats field her-cam-yaw    \ the camera heading the controls use
  1 floats field her-turn-to    \ (mode 1 / 2) where to turn
  1 floats field her-turn-rate
  1 floats field her-steer-yaw  \ the camera heading steering her after a cut
  cell field her-run?           \ the run button this frame
  cell field her-cut            \ the camera cut since the controls last looked
  1 floats field her-stick-k    \ how much of her root motion goes (0..1: eases off when let go)
  \ her animations
  cell field her-variant        \ the playing animation's variant (-1 none)
  1 floats field her-blend-w    \ the variant weight last set
  cell field her-rest           \ frames rested
  cell field her-run-t          \ frames run
  \ the danger around her (told by the danger actor: 0 calm, 1 followed, 2 chased) and the panic
  cell field her-danger
  1 floats field her-panic-level   \ 0..100
  \ her feelings (F2: fear and panic - zero until then)
  1 floats field her-fear       \ 0..99
  cell field her-recovery       \ frames since a fright
  cell field her-fear-bits      \ 1 out of breath, 2 panicking
  1 floats field her-tired-w
  \ her footsteps
  cell field her-step-l   cell field her-step-r   cell field her-steps
  \ what she is doing, run each frame (an xt: the original's state), and its part
  cell field her-act
  \ walking to a spot (Fiona_DoorFrame): the way, the spot and how she comes round to it
  /path field her-path
  12 field her-spot   cell field her-spot-tri
  cell field her-wk         \ the walk's flags (fiona/spots.fs)
  cell field her-wk-n
  1 floats field her-wk-step   1 floats field her-wk-turn   1 floats field her-wk-speed
  \ at a door
  cell field her-door-exit      \ the exit (-1 none)
  cell field her-door-side      \ the side she came from (1 in front)
  cell field her-door-kind      \ its animation (0..7 of FIN_D000.MTN)
  cell field her-door-anim      \ hers (0x600 + kind; 0x608.. trying a locked one)
  cell field her-door-opens     \ she opens it (else shuts it)
  cell field her-rattle         \ frames after trying a locked one
  cell field her-armed          \ (the stand-in exit check: she has been out of every exit's area since arriving)
end-state
