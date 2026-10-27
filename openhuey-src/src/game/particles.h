/* Particles: the rooms' sprite effects (the original's room effects and EffectMgr's, in
 * src/game/effects.c) - flames, flickering sprites, sparks, specks, splashes, dust, shards,
 * butterflies. An effect actor (scripts/effects.fs) says what and where, and when it ends; this
 * moves and draws it. Each particle system keeps a sprite batch and is moved each tick
 * (particles_tick); its sprites are written as it goes.
 *
 * Some make others as they go - a flame's sparks, a butterfly's motes of dust. Those belong
 * to the engine: moved by the same tick and gone when done, when what made them is freed (a
 * flame's lasting sparks), or when the room changes. */
#ifndef PARTICLES_H
#define PARTICLES_H

#include <stdint.h>

#include "../core/mathx.h"

#define PARTICLE_SYSTEMS 96

/* EvEffect7F (the scripts' 0x7F): a grey flickering sprite that plays its 16 frames, rests
 * 0.5 .. 1.2 s (slow: 2.5 .. 4) and plays again at a new turn */
int particles_flicker(Vec3 at, int slow);
/* EvEffect86 (0x86): a flame - kind 0 a candle, others a fire (1 with lasting sparks above it,
 * 3 another colour, 4 as 1 turned a quarter) */
int particles_flame(Vec3 at, int kind);
/* SpeckSwarm (0x9F): up to 16 specks buzzing about the point within `spread` */
int particles_swarm(Vec3 at, int count, float spread, uint32_t rgba);
/* Splash (0xA0): up to 10 specks thrown up and away from `from`, bouncing on at's height */
int particles_splash(Vec3 at, Vec3 from, int count, float spread, uint32_t rgba);
/* SpriteBurst (0xC8, dust): 16 sprites - kinds 1 swirling motes that brighten to `top`, 2 dust
 * spreading at a speed by `top`, 3 a rising puff (pale if r is not 0), others dust drifting up */
int particles_burst(Vec3 at, int kind, int r, int g, int b, int top);
/* Effect6FF60 (0x8C): 16 shards thrown from the point, tumbling and bouncing on its height
 * raised by `floor`; textured by a cell (u v .. u+du v+dv) of the room's texture `tex`, coloured
 * by the GS word `gs` (R in the low byte). Kinds: 0 / 3 gentle, 2 / 3 bigger and higher, 4 down */
int particles_shards(Vec3 at, uint32_t gs, int kind, int tex, float u, float v, float du, float dv, float floor);
/* falling stones (Effect79B00): 32 boxes dropped from `height` onto the room's floor, over and over;
 * textured by the room's texture `tex`, its part (u, v) .. (u + du, v + dv); colour a GS word */
int particles_stones(float height, uint32_t gs, int tex, float u, float v, float du, float dv);
/* debris falling down the water tower (Debris): kind 0..2 a spot, 3..5 the same in small pieces */
int particles_debris(int kind);
/* the smoke rising from castle 2F's kiln (RisingSmoke) */
int particles_rising_smoke(void);
/* the dust drifting down the old mansion's shaft of light (DustShaft) */
int particles_dust_shaft(void);
/* the smoke spiralling up from the old mansion's fire, with its glow (SpiralSmoke) */
int particles_spiral_smoke(void);
/* water dripping into the old mansion's flooded room, ripples and sounds (Drips) */
int particles_drips(void);
/* a footstep in water (Character_WaterStep): a ring and a spray of droplets (`big` larger) */
void particles_water_step(Vec3 at, int big);
/* a drop now and then from a branch in the Chaos Forest, a ring and a droplet where it lands (OneDrip) */
int particles_one_drip(void);
/* motes rising in castle 1F's hall (RisingMotes) */
int particles_rising_motes(void);
/* embers drifting up castle 1F's fireplace (OrangeSparks); `ending` >= 0 as a first command
 * (particles_command: 0 a gust, 1 flung away as they go out) */
int particles_embers(int ending);
/* flecks drifting across an area (DriftingFlecks): half its width, its depth; placed, turned (degrees) */
int particles_flecks(Vec3 at, float half_w, float depth, Vec3 turn);
/* smoke puffs rising in castle B1 (SmokePuffs) */
int particles_smoke_puffs(void);
/* a soft shadow on the old mansion's wall (WallShadow) */
int particles_wall_shadow(void);
/* a fire at one of the old mansion B1's spots (Fire): spot 0..3, small or big; `particles_command`
 * -1 puts it out */
int particles_fire(int spot, int big);
/* white puffs and a glow rising at one of the old mansion B1's spots (Room66Effect); `particles_command`
 * -1 dies it down */
int particles_steam(int spot);
/* the old mansion 3F's floor light's glow (FloorGlow); `particles_command` its level */
int particles_floor_glow(void);
/* a TV in castle 2F showing the screen (TvScreenA 0 / B 1); `particles_command` nonzero turns it on */
int particles_tv(int which);
/* the old mansion 1F's fire: smoke, flames and a glow (BigFire); `particles_command` nonzero puts it out */
int particles_big_fire(void);
/* the old mansion B1's beam of light and its motes (LightShaft); `particles_command` 1 / 2 its haze on /
 * off, -1 stopped */
int particles_light_shaft(void);
/* a glint on actor `actor`'s bone 6 (Glint) */
int particles_glint(int actor);
/* the glow and sparks on a stalker's model (Marker; the second kind: Lorenzo's, sometimes a spark
 * on `other`); particles_command -1 done, -2 no sparks a frame, else its size in thousandths */
int particles_marker(int model, int other, float size, int second);
/* a dial's lens catching the light: five frames of a white ring at `at`, turned `turn` (WindowFlash) */
int particles_window_flash(Vec3 at, float turn);
/* smoke rising from thing `thing` till it is gone (SmokeTrail; the house of truth's traps) */
int particles_smoke_trail(int thing);
/* a magnesia going off at `at` (the refined one's burst; `prima`: the prima's, three across the view), its
 * ring laid on the floor of normal `floor_up` */
int particles_mag_burst(Vec3 at, int prima, Vec3 floor_up);
/* the old mansion 1F's haze: rising motes, the screen wavering (Room49Effect) */
int particles_haze(void);
/* a glow standing at a point (Effect737D0): its colour ($RRGGBBAA), its size */
int particles_glow(Vec3 at, uint32_t rgba, float size);
/* a column of wisps rising up the House of Truth's middle (WispColumn); `particles_command` 0 fades it out */
int particles_wisp_column(void);
/* sparks sprayed across the House of Truth from one side (SparkSpray: 0 left, else right);
 * `particles_command` 0 turns it off */
int particles_spark_spray(int side);
/* a heat haze over the House of Truth (Room92Effect); `particles_command` its wave's mark (+256: at once) */
int particles_heat_haze(void);
/* embers (and flames) rising from the House of Truth's pit (Embers); `particles_command` 0 off */
int particles_pit_embers(int flames);
/* leaves blowing across the front garden (Room2AWisps) */
int particles_leaves(void);
/* one looping sprite at a point (LoopingSprite), moved with particles_place */
int particles_loop_sprite(Vec3 at);
/* drops falling over the old mansion's square room, splashing (FallingDrops) */
int particles_rain(void);
/* ash flakes carried up on a wind in the Chaos Forest (AshFlakes) */
int particles_ash(void);
/* a glow on the floor under a planet of water-tower-7f-1 (ObjectGlow: idx 0..7 its size); `particles_command`
 * 1 on, 0 off (fading out), $FF gone */
int particles_object_glow(Vec3 at, int idx);
/* a caustic: the room's texture `tex` laid flat as two wobbling grids of `size` at a point, added at 1/8
 * in a colour; its phase on `step`, its grids turned by `turn` a frame (0: unturned) */
int particles_caustic(Vec3 at, int tex, float size, uint32_t rgba, float step, float turn);
/* Butterflies (0x35): `count` butterflies (0: 8) making for `to`; coloured by `colours` (8
 * GS words, NULL: grey). Told again: a new place to make for */
int particles_butterflies(Vec3 to, int count, const uint8_t *colours);

/* Effect71000 (0xA9): a flat quad wandering within (rx, rz) of the point, the whole turned by `turn`
 * (radians about x, y, z) */
int particles_wander(Vec3 at, float rx, float rz, Vec3 turn);
/* a spirit (CreatureA's look): its glow and trail, bobbing; `red` its colour, `bob` how high it
 * floats to begin (0; kind $24 -9). Told each frame where it is and whether it shows; told to
 * vanish - seen (it touched her: a burst of light, then it sinks and fades) or not (it fades) -
 * it ends once gone */
/* a creature's vanishing at `at`: its glow (in that colour) and 8 sparks (CreatureVanish) */
int particles_vanish(Vec3 at, int r, int g, int b, int a);
int particles_spirit(Vec3 at, int red, float bob);
void particles_spirit_at(int p, Vec3 at, int shown);
void particles_spirit_vanish(int p, int seen);

/* a command later: a flame's (the scripts' 0x87: 1 / 2 - the candle burns down, the fire throws
 * sparks); butterflies: somewhere new (at) and how many (0: as they are) */
void particles_command(int p, int v);
/* a system moved there (its parts with it); a flickering sprite's rest set */
void particles_place(int p, Vec3 at);
void particles_rest(int p, int frames);
void particles_butterflies_to(int p, Vec3 to, int count);
/* butterflies: where they wander about now (an effect's +0x20, a sound there); 0 not butterflies */
int particles_butterflies_at(int p, Vec3 *at);

/* every system on a tick (the camera where it is: specks hide near it) */
/* BloodSpray (HitEffect_Spawn): where a blow lands, 32 drops falling to the floor of triangle
 * `tri` (-1: none), half leaving a splat there; heavy: bigger, faster, brighter. The engine's:
 * gone by themselves */
int particles_spray(Vec3 at, int tri, int heavy);
/* DropletFlash (Riccardo_Debris): 32 droplets of colour r g b (0..127) flung from `at` and
 * slowing, a flash there first - the debris where his shot strikes. The engine's */
int particles_drop_flash(Vec3 at, int r, int g, int b);
/* BoneSmoke: sixteen wisps of smoke rising from `at` (Riccardo's gun as it fires). The engine's */
int particles_bone_smoke(Vec3 at);
/* LorenzoSpark: two white sparks at `at`, rising 0.1 x rise, growing (growth - 1) x size a frame,
 * fading over `life` frames. The engine's */
int particles_spark2(Vec3 at, float rise, float size, float growth, int life);
/* LightRing: a ring of light spreading from `at` and sparks spiralling down about it (rising: up,
 * no ring) - where Lorenzo's other class sinks away or comes up. The engine's */
int particles_sink_ring(Vec3 at, int rising);
/* where the leaving homunculus sinks away (Cr19Bubbles): bubbles welling up and drops thrown up
 * at `at`, a stain on triangle `tri`'s floor at frame 100 (-1: nothing); the engine's */
int particles_welling(Vec3 at, int tri);
/* the system `p` the engine's from now on: freed once it has ended (made from the scripts and let go) */
void particles_engines(int p);
/* ThingBurst (ShoveBurst_Init): 16 brown pieces thrown out and 16 puffs - where a thing breaks
 * or someone is shoved hard. The engine's */
int particles_puffs(Vec3 at);

void particles_tick(Vec3 camera);
/* still going (made, not ended by itself) */
int particles_alive(int p);
/* how many systems of the effect that `p` is are going, made by the engine (tests: sparks) */
int particles_children(int p);
/* (tests, the console) its kind, its first sprite's frame, frames left of its rest; 0 if none */
int particles_info(int p, int *kind, int *frame, int *rest);
void particles_free(int p);
/* a new room: the engine's own (sparks, motes) go */
void particles_new_room(void);
void particles_free_all(void);

#endif
