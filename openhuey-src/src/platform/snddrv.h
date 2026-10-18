/* The sound effects: Capcom's sound driver (SNDDRV.IRX, its commands 0x26 play / 0x28 stop /
 * 0x29 voices off / 0x35 voice volumes; the decomp's native/platform/snddrv.c) with the game's
 * side of it (src/game/sound.c: SndDriver_Play / PlayPlaced / StopSound, and SndLib_Place's 3-D
 * placement), playing on the sound processor's core 1 voices (spu.c).
 *
 * Eight banks, as the game numbers them: 4 the sound set (D_n000), 5 the common sounds (C_0000),
 * 6 the room's (ST_xxx/ST1_xxx), 7 the pursuer's; each NAME.HD / .SDT / .BD. A sound is a run
 * of entries in the bank's table, each a program / split of its header on a voice of its own,
 * at a priority: a sound takes its voice from one playing at a lower priority (or the same, if
 * its entry says so), never from a higher one. */
#ifndef SNDDRV_H
#define SNDDRV_H

#include <stddef.h>
#include <stdint.h>

/* the tables only the executable has: the 16 distance curves (0x500 bytes at 0x41D820), the
 * stereo pan by angle (1025 floats each: right 0x3DF580, left 0x3E0580) and the positioned
 * sounds (8 x bank, voice - 24: 0x3D8990) */
void snddrv_tables(const uint8_t *curves, const uint8_t *pan_r, const uint8_t *pan_l, const uint8_t *positioned);

/* the reverb as the driver starts it (command 0x15: both cores in mode 3, studio B, silent),
 * libsd's presets read from the game's LIBSD.IRX (0 if it isn't the expected one: no reverb) */
int snddrv_reverb_setup(const uint8_t *libsd, size_t n);
/* SndDriver_SetVolume: core k's reverb comes back at v (0..0x3FFF) */
void snddrv_reverb_volume(int k, int v);

/* bank k holds NAME (loaded if it doesn't already; NULL or "" empties it); 0 if it can't be */
int snddrv_bank(int k, const char *name);
const char *snddrv_bank_name(int k);
int snddrv_bank_loaded(int k);

/* sound `id` of bank k, heard plainly (SndDriver_Play); id bit 31: its voices not keyed (a
 * playing sound's settings changed) */
void snddrv_play(uint32_t id, int k);
/* placed at a point seen from the listener, both in the camera's view (x right, z ahead):
 * SndDriver_PlayPlaced with Sound_SetPosition's block. vol, pitch: offsets (vol: x / 128 more
 * or less; pitch: semitones); id bit 30: softer by the scripts' scale too */
void snddrv_play_placed(uint32_t id, int k, int vol, int pitch, const float at[3], const float ahead[3]);
/* stop sound `id` of bank k (its voices release) */
void snddrv_stop(uint32_t id, int k);
/* the voices in the masks off (core 0's 24, core 1's 24) */
void snddrv_stop_masks(uint32_t core0, uint32_t core1);
void snddrv_stop_all(void);
/* the positioned sounds' voices' own volume (0..255; the pause screen fades them) */
void snddrv_placed_volume(int v);
/* the sound effects' volume (sound x master, 0..1 each) */
void snddrv_volume(float sound, float master);
/* stereo (1) or mono (0) */
void snddrv_stereo(int on);
/* the placed sounds' loudness (0..1; the driver's +0x1D4) */
void snddrv_placed_scale(float s);
/* the scripts' scale (progress +0x1118) on the placed sounds whose id has bit 30 */
void snddrv_progress_scale(float s);

/* voice v's record: the bank and first entry playing (-1 none), its priority, the volumes set;
 * 0 if it is silent */
int snddrv_voice(int v, int *bank, int *entry, int *pri, int *l, int *r);

#endif
