\ fiona/state.fs - Fiona's state: one actor's fields (docs/subsystems/fiona.md). `fiona enter`
\ at the console shows them.
IN: fiona.state
USING: actors paths vectors ;

state: fiona-state
  cell field her-model          \ her model (an Actor)
  cell field her-costume        \ which (progress variable $26: 0 her clothes O_FIN, 1 the slip she wakes in O_FIS)
  cell field her-doing          \ what she is busy with: 0 free (see `busy-*` in fiona.fs)
  cell field her-mode           \ how she moves (0 on her own; more with the later phases)
  cell field her-sub            \ within it: 0 standing, 1 walking, 2 running, 5 pushing...
  /vec field her-at               \ where she is (3 single floats: x y z)
  /vec field her-was-at           \ and was at the frame's start
  cell field her-tri            \ her nav triangle (-1 none)
  cell field her-was-tri
  1 floats field her-yaw        \ her heading
  1 floats field her-yaw-was
  1 floats field her-heading    \ the heading she turns to
  \ the controls (Fiona_MoveInput)
  /vec field her-dir              \ the stick as a direction in the room (x 0 z)
  /vec field her-stick            \ the stick last frame (x 0 z)
  /vec field her-last             \ its direction, kept while the old camera steers
  1 floats field stick-x  1 floats field stick-z   \ the stick this frame: x right, z down (as the pad's)
  1 floats field way-x  1 floats field way-z       \ ... as a direction (length 1; 0 0 when still)
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
  cell field her-panic-stage       \ (told by the panic: 0 calm .. 4 panicking, 5 calming)
  cell field her-cond              \ her condition: 0 well, 1 hurt, 2 down (F4)
  \ her feelings (F2: fear and panic - zero until then)
  1 floats field her-fear       \ 0..99
  cell field her-recovery       \ frames still to calm after a fright (1800 at one; under 360 she may flee)
  cell field her-fear-bits      \ 1 out of breath, 2 panicking
  1 floats field her-tired-w
  cell field her-panic-t      \ frames her panic run (or being out of breath) lasts
  cell field her-stumble-t    \ frames before she may fall again
  cell field her-attack-t     \ the panic attack's frames
  1 floats field her-fall-yaw  cell field her-fall-turn  cell field her-wall-side
  \ struck or caught (F4: fiona/hurt.fs): by whom (+0x100: an actor, -1 nobody), the blow's
  \ detail (the grip of a hold, $8000 a stumble), thrown backwards
  cell field her-hit-who  cell field her-hit-how  cell field her-throw-back
  \ led away by a stalker (F4b: fiona/seized.fs): led on (till `let-go`); the spot by him
  \ (+0x1AD6F0 its triangle, +0x1AD700, +0x1AD6F4 its heading), the step and turn to it; the
  \ drags, the shakes (Fiona_Shakes: the stick's last way, out from rest), broken free; her cry
  \ and the game's end marked once
  cell field her-led  cell field her-led-tri  /vec field her-led-at  1 floats field her-led-face
  1 floats field her-led-step  1 floats field her-led-turn
  cell field her-drags  cell field her-shakes  cell field her-free
  1 floats field her-shake-a  cell field her-shake-rest
  cell field her-shake-count  \ (the story's count of her shakes - old-mansion-b1-3: -1 not counting)
  cell field her-cried  cell field her-ended
  \ defending (F4: fiona/defend.fs): whom her blow struck so far (1 the stalker, 2 Hewie), held
  \ still a moment by it (+0x14D0)
  cell field her-struck  cell field her-recoil
  \ her footsteps
  cell field her-step-l   cell field her-step-r   cell field her-steps
  \ what she is doing, run each frame (an xt: the original's state), and its part
  cell field her-act
  \ walking to a spot (Fiona_DoorFrame): the way, the spot and how she comes round to it
  /path field her-path
  /vec field her-spot   cell field her-spot-tri
  cell field her-wk         \ the walk's flags (fiona/spots.fs)
  cell field her-wk-n
  1 floats field her-wk-step   1 floats field her-wk-turn   1 floats field her-wk-speed
  \ at a door
  cell field her-door-exit      \ the exit (-1 none)
  cell field her-door-hit       \ the door (exit) that threw her last (her request 5)
  1 floats field her-letgo-yaw  \ letting go of a thing: the way she turns to (FI 0x1AD6D0), aimed at him, gone from her hand
  cell field her-aimed  cell field her-let-gone
  cell field her-st-cond        \ the stalker's condition and mode, as he last told (stalker-doing): 2 down
  cell field her-st-mode
  cell field her-noclip         \ the story's 0x20: through blocked floor (+0x2B)
  cell field her-aside          \ the story's 0x21: left alone - nobody attacks her (+0x2D)
  cell field her-ladder         \ the ladder she uses (+0x100: the room's nav link; -1 none)
  cell field her-push           \ +0x1AD560: the obstacle she pushes (-1 none)
  /vec field her-push-dir         \ +0x1AD570: the push (x 0 z)
  cell field her-ladder-side    \ +0x104: the side she came at it from (0 the foot, 1 the top)
  cell field her-door-side      \ the side she came from (1 in front)
  cell field her-door-kind      \ its animation (0..7 of FIN_D000.MTN)
  cell field her-door-anim      \ hers (0x600 + kind; 0x608.. trying a locked one)
  cell field her-door-opens     \ she opens it (else shuts it)
  cell field her-rattle         \ frames after trying a locked one
  \ Hewie (F3): her command this frame (-1 none: 0 up, 1 down, 2 R3, 3 right, 4 left), the
  \ command's code and where it was given, frames the pad isn't read, the praise's repeats
  cell field her-cmd   cell field her-busy-t
  cell field her-act-tri  1 floats field her-act-yaw  12 field her-act-at
  cell field her-held-n
  \ her hands off the controls: a script of the room has her (the original's +0xE0), or a
  \ message is on screen
  cell field her-scripted  cell field her-reading  cell field her-in-scene
  \ the story's move (kind 0: none): its numbers, the spot and heading; done; its walk's step and turn
  cell field her-move  cell field her-move-a  cell field her-move-b  cell field her-move-done
  /vec field her-move-at  1 floats field her-move-yaw
  1 floats field her-move-step  1 floats field her-move-turn
  \ Hewie as he tells it (hewie-doing): here (0 out of the game, 1 elsewhere, 2 with her), his
  \ action, move mode and its part, condition, mood, pose group
  cell field dog-here  cell field dog-action  cell field dog-mode  cell field dog-sub
  cell field dog-cond  cell field dog-mood  cell field dog-group
  \ her head's look (+0x1AD5FC on, +0x1AD600 at whom - an actor, -1 a point - +0x1AD610 the point,
  \ +0x1AD620 frames to keep it, +0x1AD664 whom to face after a command), the head's angles
  \ as eased (the motion's +0x854 pitch, +0x858 turn)
  cell field her-look-on  cell field her-look-who  /vec field her-look-pt  cell field her-look-hold
  cell field her-look-after  1 floats field her-head-pitch  1 floats field her-head-yaw          \ (the stand-in exit check: she has been out of every exit's area since arriving)
  \ out of sight: item $89 worn (the equipment's slot 1), standing still 60 frames (Fiona_Frame's area fade)
  cell field her-unseen  cell field her-still-t
  cell field her-fade           \ 0..$80: how much of her shows (fading out while unseen, back in after)
  cell field her-hold  cell field her-hold-k   \ held by homunculi: the hold's frames, the shakes that may still count
  cell field her-carried-t      \ frames since a homunculus in her room said it was holding (move 8)
  10 cells field her-creatures    \ each creature slot's actor, as it last told her (creature-doing; -1 none)
  cell field her-blow-n        \ her blows, numbered (a creature takes each once: Fiona_BlowCreatures' skip)
end-state
