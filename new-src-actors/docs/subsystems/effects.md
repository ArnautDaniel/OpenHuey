# The rooms' effects

## Purpose

The flames, sparks, flickering sprites, dust, shards and butterflies the rooms' scripts set going
(`src/game/effects.c`). Two kinds, as in the original:

- **Room effects** (RoomEffects, 32 slots): made in a slot by the room's scripts, kept until
  removed or the room is left: the flame (0x86), the flickering sprite (0x7F, 0xB1), the
  butterflies (0x35). The room actor keeps the slots (`fx-ids`, and where each was made).
- **Scene effects** (EffectMgr): made and forgotten - they end by themselves, or when the next
  room comes in (`arrived`; the original resets the manager on a room load): the specks (0x9F),
  the shards (0x8C), a flame's sparks, a butterfly's dust.

Each effect is an actor (`scripts/effects.fs`). It steps itself each tick and writes its sprites
at the frame's end; killed, its last words (`on killed`) free its sprite batch.

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

| Message | Stack | Meaning |
|---|---|---|
| `effect-start` | `( x y z command kind -- )` | Its place (f>cell) and how it starts (RoomEffects_Send's record). |
| `effect-command` | `( v -- )` | A command later: the scripts' 0x87 (1 the character stood still, 2 it moved). |
| `specks-start` | `( x y z count spread rgba -- )` | A speck swarm. |
| `shards-start` | `( tex u v du dv floor -- )` | The shards' texture and cell (a zone rectangle of the script's), the floor offset; thrown. |
| `butterflies-start` | `( x y z count -- )` | Where they make for; how many (0: as they are). |
| `dust-start` | `( rgba x y z vx vy vz -- )` | A mote of dust. |

The story's words: `effect-86`, `flicker-sprite`, `flicker-sprite-var`, `effect-remove`,
`zone-at-effect`, `char-effect-moving`, `specks`, `scene-effect-8C`, `butterflies`.

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
- **Butterflies**: circling a centre that makes for the target while a point they follow wanders
  about it, bobbing, banking, flapping; now and then one sheds a mote of dust (**SinkingSprite**:
  growing, sinking, fading).

## Status

**Built** (S5b): the classes above. Not yet: the glow pass ($80: drawn plain), the shards' and
butterflies' PS2-only behaviour (a shard with a corner off screen not drawn; the butterflies' size
factor, which scales a homogeneous point - no visible change - is left out), the butterflies'
dust held back on a camera cut, Effect71000 (0xA9), and the rooms' own effects (S5c: wisps,
smoke, drips, the mirror...). Checked by `tests/story/test_effects.fs`; seen on screen: the
torches of castle-1f-8, shards in castle-1f-14.
