# Camera

## Purpose

The game's fixed-angle camera. Each room has camera *sets* (an eye, a point looked at, a view
angle) and camera *paths* (splines it slides along). The story says which set and path apply
to a character in each part of the room. The camera follows one character, usually the
controlled one, and uses that character's setup: it cuts on a new set, slides along a path
staying nearest the character, or turns to keep the character in view.

## API

The director is C (`src/game/camdirector.c`, the original's `CamDirector_*` with the
progress' `Progress_Camera*`): it keeps whom it follows and each character's setup, so the story
calls it directly, at the points the original does.

| Word | Stack | Meaning |
|---|---|---|
| `cam-char-setup` | `( who set path -- )` | `who`'s setup from now on (path -1: none). The story's `char-camera` / `area-camera`. |
| `cam-on` | `( who -- )` | Follow `who` and take their setup (-1: nobody; someone gone, elsewhere or without a setup: Fiona). The story's `camera-follow` (0x28). |
| `cam-followed` | `( -- who )` | Whom it follows (the story's `camera-on`). |
| `cam-shake!` | `( F: a -- )` | The eye shaken by up to `a` each way - for this frame: the panic stills it first each frame (0xD3). |
| `cam-restart` | `( -- )` | Start over on the setup (0x4A). |
| `cam-scene!` / `cam-scene?` | `( flag -- )` / `( -- flag )` | A cutscene has the camera (from its start of play to its signal 8): the director idle, `cam-set@` -1. Others wait on it (the summoner, the butterflies' motes). |
| `cam-set@` / `cam-changed?` | `( -- set )` / `( -- flag )` | Conditions 0x25 and 0x2C (the phase 3 scripts' per-set depth ranges). |
| `out-of-view?` | `( -- flag ) ( F: x y z -- )` | Off the camera's 4:3 picture, or nearer than its near or beyond its far (`Camera_OutOfView`, through its clip matrix; behind the eye always out): condition 0x45, the director's own jump test. |

| Message | Stack | To | Meaning |
|---|---|---|---|
| `event-shot` | `( on dist pitch yaw rise -- )` | camera | The story's own shot of the one followed (0x82, thousandths), or back. |
| `camera-hold` | `( -- )` | camera | The game over: the room's next set, nobody followed, from there on. |
| `camera-cut` | `( -- )` | broadcast | It cut to another set (Fiona's controls keep the old view's directions). |

## Rules

From `camera.c` (`CamDirector_*`), `Progress_CameraFollow` / `CameraOn` / `CameraSetup`, the play
sub-state and the room change.

1. Each frame: `Ease` first (the view angle, the path toward the one followed, the turn to keep
   them in view); after the characters have acted, whom it follows checked and their setup
   taken, `Track` (a cut on a new set or path, or when a newly followed one is out of view),
   `Update` (the setup remembered, the camera placed). A cut to another set is broadcast.
2. Who may be followed (`Progress_CameraFollow`): Fiona always; another character only in the
   played room and with a setup. Otherwise the camera goes back to Fiona.
3. From room to room the director goes on (its shake, its shot, whom it follows); only the
   characters' setups go. The room starts it between its first scripts and its phase 3
   (`RoomStart`, then `CameraOn`): the view angle from 0.6 of the set's, out to it over 60
   frames, and the phase 3 scripts see the setup changed. A fresh start (a new game, a load:
   `RoomIn`) resets it and cuts on the first frame; Fiona's setup is the save's.
4. A cut wipes the glow's trails (`camdir_glow_cut`; Renderer_GlowClear, the renderer's +0x5C):
   `Track`'s cuts, the room start, a restart, a cutscene taking the camera or giving it back, the
   story's shot coming in, and a cutscene's own cuts (its signal 0). Not while state flag $28 is
   set (`in-play`: from the game's entry till the opening is done - the original's check).
5. The camera's range (`RoomStart`: Camera +0xB0 / +0xB4): near 1, far 2000 as each room
   starts; the story's 0x0B (`camera-value`) changes them after (water-tower-8f-1's far 20000).
   Characters are drawn as the original's microprograms clip (CLIP, FCAND, the ADC bit;
   `render_mesh_ps2_clip`): a triangle with a corner nearer than the near plane, beyond the far
   one or past the GS's drawing space round the screen (about 8 screens across) isn't drawn at all
   - OpenGL alone would cut it at the screen, showing a model's inside to a camera in it.
6. The panic stills the camera at the start of each frame and shakes it (0.2) through a panic's
   later stretch (`Panic_Stage`), unless a cutscene has the camera or the panic is held.
7. The story's shot: the point and heading fixed as it starts, its distance and angles as told;
   the room's depth range off meanwhile. Following nobody ends it.

## Debug

- `room!` / `free-play` jump into a room with no exit to arrive by - nothing the original can do:
  Fiona gets the room's set looking nearest her when the room's scripts give her none.
- `.director` prints the director's state; F3 frees the camera.

## Status

**Built** (`scripts/camera.fs`, `src/game/camdirector.c`; `tests/camera/test_camera.fs`).
The director reads the followed actor's body directly. Its random shake isn't the original's
generator.
