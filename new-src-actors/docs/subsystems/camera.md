# Camera

## Purpose

The game's fixed-angle camera. Each room has camera *sets* (an eye, a point looked at, a view
angle) and camera *paths* (splines it slides along). The story says which set and path apply
to a character in each part of the room. The camera follows one character, usually the
controlled one, and uses that character's setup: it cuts on a new set, slides along a path
staying nearest the character, or turns to keep the character in view.

## API

| Message | Stack | To | Meaning |
|---|---|---|---|
| `camera-setup` | `( who set path -- )` | camera | `who`'s setup from now on (path -1: none). The story sends it as characters go into camera areas or arrive by exits. |
| `follow` | `( who -- )` | camera | Follow this actor (-1: nobody; the camera stays). |
| `event-camera` | `( on -- )` | camera | A scene drives the camera: the director stops (*to detail with cutscenes*). |
| `camera-cut` | `( -- )` | broadcast | It cut to a new set (Fiona's controls turn with the view only after a cut). |

**Facts:** the room's sets and paths (C, from the room file); the followed actor's body
(position). The mechanics stay in C (`camdirector.c`, the original's `CamDirector_*`): easing,
the path, the view angle dipping after a cut, the 4:3 to widescreen view.

## State

Who is followed; each character's setup (set, path); event mode.

## Rules

From `camera.c` (`CamDirector_*`), `Progress_CameraFollow`, `Progress_CameraSetup`, the play
sub-state.

1. Each frame, after the characters have acted: the followed character's setup is given to the
   director, which takes it if it changed (cut or new path), then places the camera (`Track`,
   `ModeNormal`). `Ease` runs at the start of the frame.
2. Who may be followed (`Progress_CameraFollow`): Fiona always; another character only in the
   played room and with a setup. Otherwise the camera goes back to Fiona.
3. A new room: no set, no path, no target (`NewRoom`). Once the room is in, its sets and paths
   are taken (`RoomStart`). Arriving characters drop their setups, and the room's scripts give
   new ones.
4. While a scene drives the camera (event camera), the characters' controls freeze (the
   frame flags 0x3F0), unless the scene says otherwise.

## Design notes

- The setups come from the story (the converted room scripts): `char-camera` / `area-camera`
  become `camera-setup` messages.
- The director follows a body (an actor's position), not a model.
- Debug: a free camera (the old `freecam`) as a console command on the camera actor.

## Status

**Built** (`scripts/camera.fs`). The C director now follows a point (`cam-target!`), set each
frame from the followed actor's body, not a model.

*Stand-in until the story:* nobody gives setups yet, so a character without one gets the
room's camera set whose look-at point is nearest it (no path). It goes when the story sends
`camera-setup`.

Left: who may be followed (Fiona's rule), the event camera (with cutscenes), a debug free
camera, a test with the room scripts' setups.
