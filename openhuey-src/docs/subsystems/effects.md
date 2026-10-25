# The rooms' effects

## Purpose

The flames, sparks, flickering sprites, dust, shards and butterflies the rooms' scripts set going
(`src/game/effects.c`). Two kinds, as in the original:

- **Room effects** (RoomEffects, 32 slots): made in a slot by the room's scripts, kept until
  removed or the room is left: the flame (0x86), the flickering sprite (0x7F, 0xB1), the
  butterflies (0x35). The room actor keeps the slots (`fx-ids`, and where each was made).
- **Scene effects** (EffectMgr): made and forgotten - they end by themselves, or when the next
  room comes in (`arrived`; the original resets the manager on a room load): the specks (0x9F),
  the shards (0x8C), a splash (0xA0), dust (0xC8).

The motion is the engine's: `src/game/particles.c` holds each effect's particle system - its
sprites, how they move (each class's update, number for number) - and moves them all on the
engine's tick. An effect actor (`scripts/effects.fs`) only says what and where (`flame-new`,
`swarm-new`, ...), passes on the story's commands (`effect-command`: a flame's 0x87), and ends:
a room effect when the story removes it, a scene effect once its system has ended
(`particles-alive?`) or the room changes. Killed, it frees its system.

Effects that make others - a flame's sparks, a butterfly's motes of dust - make them in the
engine: they go when done, when what made them is freed (a flame's lasting sparks), or when a
room loads (`particles_new_room`). `particles-children` counts them, `particles-info` shows a
system's kind, frame and rest (tests, the console).

## The engine's sprites

`src/game/sprites.c` is the original's quad drawer (`gl_sprites`): batches of textured quads,
drawn after the opaque scene without writing depth.

- **Textures** are taken from the texture cache as the original has it: index group + id; group
  $10 is `GAME_FIX.TEX` (three 512 x 256 sheets), 0 the room's bank (PAC section 9), $15 its
  second (11). A 16-colour texture is decoded in the palette the drawer names (the GS's CSA).
- **A batch:** its texture, its frame cells (along rows of the sheet), flags (`SPRITE_UPRIGHT` 1,
  `_CORNERS` 2, `_QUARTER` 4, `_ADD` $40, `_GLOW` $80 - not drawn yet, `_OPAQUE` $100), a draw layer.
- **A record:** colour (0x80 = 1.0), position, half sizes, turn about the view axis, frame - a
  camera-facing quad (the camera's right and down, as the PS2's screen is y-down); or a quad with
  its own four corners (`sprite-quad!`: the shards' faces, the butterflies' wings).

Words: `sprites`, `sprites-free`, `sprites-texture`, `-cells`, `-uv`, `-flags`, `-offset`,
`-corner`, `-count`, `sprite!`, `sprite-quad!`.

## API

Engine words (each makes a particle system and gives its number, -1 none): `flicker-new ( slow
F: x y z )`, `flame-new ( kind F: x y z )`, `swarm-new ( count spread rgba F: x y z )`,
`splash-new ( count spread rgba F: x y z from-x from-y from-z )`, `burst-new ( kind r g b top
F: x y z )`, `shards-new ( gs kind tex F: x y z u v du dv floor )`, `butterflies-new ( count F: x
y z )`; then `particles-command ( p v )`, `butterflies-to ( p count F: x y z )`,
`particles-alive? ( p -- flag )`, `particles-free ( p )`.

`effects.fs`: `flicker`, `flame`, `butterflies` (room effects: give the actor), `specks`,
`splash`, `burst`, `shards` (scene effects). Its actors take `effect-command ( v -- )` - the
scripts' 0x87 (1 the character stood still, 2 it moved).

The story's words: `effect-86`, `flicker-sprite`, `flicker-sprite-var`, `effect-remove`,
`zone-at-effect`, `char-effect-moving`, `specks`, `scene-effect-8C`, `butterflies`, `splash`, `dust`.

## Rules (each class)

- **EvEffect7F** (flickering sprite): grey, half transparent, 1.6 across, a 16-frame strip of 32 x
  32 cells; plays through, rests 0.5..1.2 s (slow: 2.5..4 s), plays again at a random turn.
- **EvEffect86** (flame): kind 0 a candle - 32 frames, the first half looping; commanded, the
  second half at half speed (command 2 shrinking it away). Others a 16-frame fire, upright, glowing
  (kind 4 also turned a quarter, kind 3 another palette); kind 1 has a lasting spark above it, and
  commanded throws 2 / 4 sparks.
- **Wisps** (sparks): 4 orange sparks rising and swirling; a lasting set comes back as each fades,
  a burst ends; with bit $8000 they end with the flame (the original: with room effect 0).
- **SpeckSwarm**: up to 16 specks buzzing about a centre within a spread, now and then (1 in 10) a
  new heading pulled back to the centre; hidden with the camera within 10, dim within 20.
- **Effect6FF60** (shards): 16 random boxes thrown up, tumbling, falling, bouncing on the origin's
  height until they settle; kinds 0 / 3 gentle, 2 / 3 bigger and higher, 4 thrown down.
- **Splash** (0xA0): up to 10 specks thrown up and away from a character, bouncing on the ground
  and fading a quarter a bounce.
- **SpriteBurst** (0xC8, dust): 16 sprites - swirling motes that brighten then fade, dust
  spreading or drifting up, a dark or pale puff rising.
- **Effect71000** (0xA9, `wander-new`): a flat quad wandering about its place within a range, turned
  the way it steps; waiting, a target now and then (or home).
- **Butterflies**: circling a centre that makes for the target while a point they follow wanders
  about it, bobbing, banking, flapping; now and then one sheds a mote of dust (**SinkingSprite**:
  growing, sinking, fading).

### The rooms' own (S5c)

Engine words each make one (`*-new`, giving the system); `effects.fs` wraps them as scene effects
(made and forgotten, or kept in a story variable), the TV as a room effect. Those that take
`effect-command`: the embers (0 a gust, 1 flung away), the fire and the steam (-1 put out / dying
down), the floor glow (its level), the TV (on / off). A room effect can be moved (`effect-place`,
`particles-place`) and a flicker's rest set (`effect-rest`, `particles-rest!`).

- **Effect79B00** (`stones-new`, House of Truth): 32 boxes dropped from a height onto random floor
  triangles over and over, tumbling, bouncing once off floors that let them, fading on the
  ground; unseen within 10 of the camera.
- **Debris** (`debris-new`, the water tower): 16 pieces thrown from a 4 x 4 grid up the tower,
  falling until the last is below 500.
- **RisingSmoke** (castle 2F's kiln), **SmokePuffs** (castle B1), **RisingMotes** (castle 1F's
  hall), **DustShaft** (256 motes down the old mansion's shaft of light), **SpiralSmoke** (the old
  mansion's fire, with its flickering glow), **OrangeSparks** (`embers-new`: castle 1F's
  fireplace), **DriftingFlecks** (`flecks-new`: flat flecks drifting across an area, turned the
  way they drift), **Drips** (five spots of the old mansion's flooded room: ripples and their
  sounds - `engine_sound_at`), **OneDrip** (a drop in the Chaos Forest, with a **SplashRing** and
  a **DropletSpray** where it lands), **WallShadow** (eight black quads stepping off a wall),
  **Fire** (64 flames at one of the old mansion B1's spots), **Room66Effect** (`steam-new`: white
  puffs and a glow), **FloorGlow** (a pool and a cone of light, white fading to nothing:
  `Sprite.shaded`, its corners' own colours).
- **TvScreenA / B** (`tv-new`, castle 2F): last frame's picture, grey, at half size
  (`post_tv_want` / `post_tv_texture` in render/post.c) on the TV's screen while it is on - the
  room's scripts switch it each frame by story flag $300 (`effect-string` on its slot).
- **Reflection** (room effect $1A / $1B, the lit quads: `reflect!`, `reflect-off`, the story's
  `lit-quad`): the characters on the eye's side drawn again mirrored in the quad's plane at its
  strength, only on the quad (the scene's stencil; its depth pushed back meanwhile and put back
  after) - or over the whole picture. The original renders from a mirrored camera and flips the
  picture; a reflection matrix gives the same for a flat mirror. Side panels: not drawn.

- **BigFire**, **Room49Effect** (`haze-new`) and **LightShaft** (with its haze: `post_haze`, the
  wavering columns), **Glint** (a white sprite on a model's bone 6), **Caustic** (the rooms'
  "ModelDraw" effects: a room texture laid flat as two wobbling 8 x 8 grids, added at 1/8),
  **Effect737D0** (`glow-new`), **WispColumn**, **SparkSpray**, **Room92Effect** (`heat-haze-new`),
  **Embers** (`pit-embers-new`), **Room2AWisps** (`leaves-new`), **LoopingSprite**, **ObjectGlow**,
  **FallingDrops** (`rain-new`), **AshFlakes** (`ash-new`).
- **Marker** (`marker-new`, Lorenzo in house-of-truth-1f-5): two or three sparks a frame from a
  model's bones (the second kind: a few chosen bones, now and then one on Fiona), each a glow and
  a flare drifting off, growing, fading by its size; and an orange glow at the model's root,
  pulsing. The original tints the model's draw layer and (the first kind) adds a screen glow
  round it, counting spheres where they show: here a glow sprite. `effect-command`: -1 done, -2
  no sparks a frame, else its size x 1000.
- **WindowFlash** (`window-flash-new`, the dials' lenses: the story's `lens-flash`): five frames of
  white fans through a ring at the object, turned with it. The original draws it into the
  bloom's mask and brightens the screen through it; there is no bloom pass yet, so the shape is
  added itself.
- **MirrorFragment** (the story's `mirror-fragment`): a reflection (as the lit quads') in the
  plane of a normal turned with an object, seen in an 8 x 8 square there (`mirror-square`; hg's
  approximation too - the original's extent is not known), reflecting only its character
  (`reflect-only`: Fiona in the lens, the stalker in the fragment).
- **Reflection side panels** (Reflection_SidePanels, castle 2F's door and window: the strength
  word's top bits, $20000000 / $10000000): two fixed quads beside the quad showing its reflection
  again at half, moved by the quad's width on screen (a clip-space shift of the mirrored draw).
- **SmokeTrail** (`smoke-trail-new`): 32 puffs rising from a thing (the House of Truth's traps),
  started again at it while it is there.

**The glow pass** (the original's renderer +0x58): batches flagged `SPRITE_GLOW` are drawn again,
after the scene's sprites, into a quarter-size buffer kept between frames (against the scene's
depth at that size: `post_glow_begin` / `_end`, `sprites_draw_glow`); on a frame that glows the
buffer fades to 3/4 (kept to what its 8 bits held) and is added over the screen at half - moving
glows leave trails. A new room clears it.

**The effects' blooms** (Bloom_Start): `post_bloom(rgba, subtract)` adds this frame's colour to
the room's own bloom (the bloom mask's areas, blurred); batches flagged `SPRITE_MASK` only mark
the mask (layer $26). The dials' flash (WindowFlash: white) and the object glows (ObjectGlow: the
disc; kind 0 brightens, $40808080) use them.

**The shine** (layer $14, renderer +0x78): the characters in that layer drawn again at half size;
their bright parts (green 0x40 of 0xFF and up) with half of eight round them added in the colour
the marker gives each frame (`post_shine`). Lorenzo's marker uses it in place of a glow.

**The room's fog** (room effect $1D: `Fog_SetParams`, `Fog_Draw`): two colours over its near to
far distance, held at both ends: the near colour in front of the near distance and the far one
beyond the far and where nothing was drawn (the original reads its Z buffer through a palette,
generator 2, whose ends are the two colours). The palette halves the colours' alpha: 0x80 is
half-way, 0x100 opaque. Its parameters are 26 bytes - the colours, the
range, +16 a word that moves the fog to layer 9 (else $21), +20..25 the characters' light groups
(`Fog_Update`: layer $B, after the fog as $A is - drawn no differently). Read from the room file
the original's way: 26 on, past the section into the next one's first bytes when the fog's are
the section's last (38 rooms have +16 set, many by those bytes). In layer 9 it fogs the room's
solid and see-through parts (layer 8) and not what follows - the characters, the "a_" objects,
the glows; at $21 everything before it. Done in the composite, with the scene's second target's
g: set where a depth-writing draw came after the fog's layer (`render_fog_after`). A cutscene
keeps the fog's colours and range only (+16 back to 0).

Untextured batches: group -1 (white). A batch can take a GPU texture of its own
(`sprites_raw_texture`).

## Status

**Built** (S5b): the classes above. Not yet: the shards' and
butterflies' PS2-only behaviour (a shard with a corner off screen not drawn; the butterflies' size
factor, which scales a homogeneous point - no visible change - is left out). The butterflies shed
no dust while an event's shot has the camera (the director's +0x38). The rooms' own (S5c) are built, with the glow pass, the effects'
blooms, the shine and the reflections' side panels. Checked by `tests/story/test_effects.fs`; seen on screen: the
torches of castle-1f-8, shards in castle-1f-14.
