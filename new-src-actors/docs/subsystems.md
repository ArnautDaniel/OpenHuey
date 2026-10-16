# The game's subsystems

A map of the game: what each part owns, where it is, and where the original's rules are. Each
subsystem's page (`docs/subsystems/<name>.md`) has its messages, rules and status; this page
only points there.

## Engine services (C, not actors)

| Service | Provides (facts and words) | Original |
|---|---|---|
| assets | rooms (PAC), nav meshes, models with their motions and textures, sound banks, movies, the executable's tables | files, `char_load.c`, `model.c` |
| world | the room graph: exits, doors' two sides, routes between rooms; door positions; nav triangles (centres, flags, neighbours), floors, rays | `room_map.c`, `navmesh.c`, `RoutePlanner_*` |
| bodies and models | where each actor stands, its posed, animated model: motions, events, bones, visibility | `model.c`, `actor.c` |
| render | the picture, the camera's view | (GS / VU) |
| audio | voices, 3D sound, music streams, reverb | `sound.c`, SNDDRV |
| cutscenes | scene files played back on models, the room's objects and the camera | `cutscene.c` |

## Actors

| Subsystem | Owns | Script | Original | Page |
|---|---|---|---|---|
| Story | story flags, script variables, the rooms' phase scripts and the characters' action scripts (the original's event scripts, converted to Forth once) | `story.fs`, `story/` | `event.c` | [story](subsystems/story.md) |
| Rooms | the room being played, going from room to room | `rooms.fs` | `scene_game.c`, `room.c` | [rooms](subsystems/rooms.md) |
| Doors | each door's lock, open state, who holds it, its swing | `doors.fs` | `doors.c` | [doors](subsystems/doors.md) |
| Obstacles | the things pushed about in the played room, their squares and the floor under them | `obstacles.fs` | `room_map.c` (`Obstacle_*`) | [obstacles](subsystems/obstacles.md) |
| Things | what Fiona throws, drops or sets down, and what they do | `things.fs` | `placed.c` | [things](subsystems/things.md) |
| Camera | the director: whom it follows, each character's setup, cuts and eases (in C), the story's shot | `camera.fs` | `camera.c`, `camdirector.c` | [camera](subsystems/camera.md) |
| Fiona | her moves and actions (doors, ladders, items, pushing, shoving, kicking), being struck and seized, her fear | `fiona.fs`, `fiona/` | `fiona.c` | [fiona](subsystems/fiona.md) |
| Panic | the panic meter: level, stage, her breath and scream | `panic.fs` | `Panic_*` | [panic](subsystems/panic.md) |
| Hewie | his trust, her commands, his moods, his own mind | `hewie.fs`, `hewie/` | `hewie.c` | [hewie](subsystems/hewie.md) |
| Stalkers | Debilitas, Daniella, Riccardo, Lorenzo: senses, search, chase, attacks, being struck | `stalker.fs`, `stalker/` | `pursuer.c`, `debilitas*.c`, `daniella.c`, `riccardo.c`, `lorenzo.c` | [stalker](subsystems/stalker.md) |
| Summoner | bringing a stalker into play and taking him out | `summoner.fs` | `Summoner_*` | [summoner](subsystems/summoner.md) |
| Creatures | spirits and homunculi | `creatures.fs`, `creatures/` | `creature.c` | [creatures](subsystems/creatures.md) |
| Cast | the story's other characters: the crows, the figures in scenes | `cast.fs` | `story_chars.c`, `char_load.c` | [cast](subsystems/cast.md) |
| Acoustics | the noises made this frame, and who hears them | `acoustics.fs` | `Noise_Make`, `Character_Hearing` | [acoustics](subsystems/acoustics.md) |
| Danger | calm / followed / chased | `danger.fs` | `SceneGame_Danger` | [danger](subsystems/danger.md) |
| Music | the background track, the stage music, volumes and fades | `music.fs` | `music.c`, `MusicDir_*` | [music](subsystems/music.md) |
| Screen | the fade to and from black | `screen.fs` | `Events_Fade` | [screen](subsystems/screen.md) |
| Window | the message window and its choices | `window.fs` | `text.c` | [window](subsystems/window.md) |
| Pause | the play and movie pauses | `pause.fs` | `pause.c` | [pause](subsystems/pause.md) |
| Items and the menu | the inventory, item use, the files, the map, the options, the word plate, the slot machine, the wardrobe | `items.fs`, `subscreen.fs`, `subscreen/` | `items.c`, `subscreen.c` | [items](subsystems/items.md) |
| Effects | flames, sparks, flickering sprites, shards, butterflies; the rooms' own | `effects.fs` | `effects.c` | [effects](subsystems/effects.md) |
| Game over | the three kinds of game over | `gameover.fs` | `gameover.c` | [gameover](subsystems/gameover.md) |
| Ending | the staff roll, the results, the unlocks | `ending.fs` | `scene_ending.c` | [ending](subsystems/ending.md) |
| Saving | F5 / F9 quick save, the 8 save slots (the save points), the system data (the unlocks) | `saves.fs` | `SceneGame_Save` | (below) |

Also: the title and the opening (`opening.fs`), the avoid prompt (`avoid.fs`), the pad's rumble
(`rumble.fs`), the graphics options (`graphics.fs`), the debug words and overlay (`debug.fs`).

**Saving.** A save is a Forth file in the user's folder, read back by running it. `saves` writes
the progress, then broadcasts `write-save` and each actor (Fiona, Hewie, the stalker) prints its
own line. Loading puts the progress back, plays the room again and puts them where they were;
anything else (a scene playing, the room's own script state) starts afresh, as in the original.

## Not yet

The sections where Hewie is the one played, and the extras' menus (the movies, the galleries).
Two story stubs are left (they print "not written yet" when reached): a room command of the
unused off-map-5a, and `char-load` of a figure outside slots 3..5.
