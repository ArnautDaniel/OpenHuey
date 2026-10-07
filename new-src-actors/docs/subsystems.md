# The game's subsystems

A map, from what the decompilation showed. Each entry says what the subsystem owns, the
messages it takes and sends (a first draft: the API is settled on its own page,
`docs/subsystems/<name>.md`, before it is built) and where the original's rules are.

**Status:** `spec` page written · `built` · `checked` against the C.

## Engine services (C, not actors)

| Service | Provides (facts and words) | Original |
|---|---|---|
| assets | rooms (PAC), nav meshes, figures (models + motions + textures), sound banks, movies, the executable's tables | files, `char_load.c`, `model.c` |
| world | the room graph: exits, doors' two sides, routes between rooms; door positions; nav triangles (centres, flags, neighbours), floors, rays | `room_map.c`, `navmesh.c`, `RoutePlanner_*` |
| figures | a posed, animated model: motions, events, bones, visibility | `model.c` |
| render | the picture, the camera's view | (GS / VU) |
| audio | voices, 3D sound, music streams, reverb | `sound.c`, SNDDRV |
| cutscenes | scene files played back on figures and the camera | `cutscene.c` |

## Actors

### The story (`story`)
Owns: story flags, script variables, the room's phase scripts and the characters' action
scripts.
Takes: `entered-room`, `left-room`, `exit-taken`, `action-pressed`, `area-entered`, `scene-cue`.
Sends commands to the cast and the world: `place`, `walk-to`, `play-motion`, `say`, `lock`,
`music`, `bring-in`, `take-out`...
Original: `event.c` (commands, conditions), the converted room scripts.
Note: the room scripts are data. They are converted once (as in new-src) into Forth that sends
messages.

### Rooms (`rooms`)  — **built** (`subsystems/rooms.md`)
Owns: the room being played, and going from room to room.
Takes: `go-to-room`. Broadcasts `room-loaded`, `entered-room`, `left-room`.
Original: `scene_game.c` (room change), `room.c`.

### Doors (`doors`)  — **spec** (`subsystems/doors.md`)
Owns: each door's lock, open state, who holds it and its swing.
Takes: `open`, `shut`, `slam`, `lock`, `unlock`, `hold`, `let-go`.
Sends: `noise` (opened / shut / slammed), `hit` (slammed on someone), `door-state` broadcasts.
Original: `doors.c`, `Progress_*Door*`.

### Camera (`camera`)  — **built** (`subsystems/camera.md`)
Owns: the room's camera setups, who it follows, cuts and eases.
Takes: `follow`, `use-setup`, `event-camera`, `release`.
Original: `camera.c`, `camdirector.c`.

### Fiona (`fiona`)
Owns: her moves, her actions (doors, ladders, items, pushing), her health, fear and panic.
Takes: `input`, `hit`, `seize`, `lead`, `let-go`, `frighten`, `heal`, `place`, `play-motion`.
Sends: `noise` (steps, screams, falls), `kick`/`shove` (as `hit` to whoever she struck),
commands to Hewie, `open`/`shut` to doors, `panic-stage` broadcasts.
Original: `fiona.c`, `Panic_*`.

### Hewie (`hewie`)
Owns: his trust, his commands, his moods, his own wandering.
Takes: `command` (from Fiona), `praise`, `scold`, `hit`, `heard`, `tick`.
Sends: `noise` (barks), `bite` (a `hit`), `noticed` (he's alert to a stalker).
Original: `hewie.c`.

### The stalkers (`stalker`)
One family for Debilitas, Daniella, Riccardo and Lorenzo: the same senses, search, chase and
attack, with each one's numbers, motions and special moves.
Owns: its mode (searching / after her / held back / waiting), what it knows (where it saw her,
what it heard), its route, its health.
Takes: `tick`, `heard`, `hit`, `bitten`, `door-slammed`, `place-in-room`, `knock-down`,
`set-mode`, `search-delay`.
Sends: `noise` (steps), `hit`/`seize`/`lead` to Fiona, `open` to doors, `stalker-mode`
broadcasts (for danger and the music).
Original: `pursuer.c`, `debilitas*.c`, `daniella.c`, `riccardo.c`, `lorenzo.c`.

### Event characters (`cast`)
The story's other characters and things: the crows, the figures in scenes. Mostly placed and
put in motions by the story.
Takes: `place`, `play-motion`, `hold-motion`, `show`, `hide`.
Original: `story_chars.c`, `char_load.c`.

### Sound in the house (`acoustics`)  — **built, checked**
Owns: the noises made this frame.
Takes: `noise`, `listen`, `stop-listening`. Sends `heard` to each listener that hears one.
Original: `Noise_Make`, `Character_Hearing`, `Progress_PursuerRequest`.
Page: `docs/subsystems/acoustics.md`.

### Danger (`danger`)
Owns: calm / tense / chased and its hold timers.
Takes: `stalker-mode`, `stalker-room`, `fiona-room`, `creature-near`, `hunted`, story overrides.
Broadcasts: `danger` (music, Fiona's fear, Hewie, the scripts' conditions read it).
Original: `SceneGame_Danger`.

### The summoner (`summoner`)
Brings the stalker into play and takes him out, by the story's stage and where Fiona is.
Original: `Summoner_*`.

### Music (`music`)
Owns: the background track, the chase music stages, volumes and fades.
Takes: `music`, `music-stage`, `danger`.
Original: `music.c`, `BgmCtl`.

### Items and the sub screen (`items`, `subscreen`)
Owns: the inventory, item use, the files, the map.
Original: `items.c`, `subscreen.c`.

### Messages on screen (`messages`)
Owns: the text window and the choices.
Takes: `say`, `ask`. Sends `answered`, `closed`.
Original: `text.c`, messages.

### Effects (`effects`)
Room and scene effects (dust, specks, flicker, splashes).
Original: `effects.c`, `effectmgr.c`.

### Game over, title, ending (`scenes`)
Original: `gameover.c`, `title.c`, the ending scene.

## Order

1. The kernel and `acoustics`: the prototype.
2. `rooms`, `doors`, `camera`: the world working with actors, walked by a debug `walker`
   (a body moved by the keys over the nav mesh, going through exits) until Fiona replaces it.
3. `fiona`, `hewie`: the player's side.
4. `story` (the converted scripts sending messages).
5. `stalker` (Debilitas first), `danger`, `summoner`, `music`.
6. The rest.
