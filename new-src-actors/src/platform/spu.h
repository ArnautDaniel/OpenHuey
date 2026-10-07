/* The PS2's sound processor (SPU2) as the game's sound driver drives it: 48 voices (core 0:
 * 0..23, the music's; core 1: 24..47, the sound effects'), each playing PS-ADPCM samples at a
 * pitch, through an ADSR envelope, at a left / right volume, mixed into the sound output.
 * The music sequencer (seq.c) and the sound effects driver (snddrv.c) both play on it.
 *
 * Sample memory is not one block here: a voice plays from the bank (NAME.BD) it was given.
 * Everything is called with the lock held (spu_lock), from the game or from the tick hook. */
#ifndef SPU_H
#define SPU_H

#include <stddef.h>
#include <stdint.h>

#define SPU_VOICES 48

void spu_lock(void);
void spu_unlock(void);

/* a voice's sample: `ssa` bytes into `mem` (the start of its 16-byte block) */
void spu_voice_sample(int v, const uint8_t *mem, size_t size, size_t ssa);
/* its pitch (0x1000: 48 kHz, up to 0x3FFF), envelope (the ADSR1 / ADSR2 words) and volumes
 * (-0x4000..0x3FFF a side, as the SPU's: 0x3FFF full, negative inverted) */
void spu_voice_pitch(int v, unsigned pitch);
void spu_voice_adsr(int v, unsigned adsr1, unsigned adsr2);
void spu_voice_volume(int v, int l, int r);
/* its mix (the HD sample's spuAttr byte): bit 0 / 1 dry left / right, 2 / 3 into the reverb
 * left / right (libsd's VMIXL / VMIXR / VMIXEL / VMIXER) */
void spu_voice_mix(int v, int mix);
/* key on (from the start, the envelope attacking) / off (releasing) the voices in the mask */
void spu_key_on(uint64_t mask);
void spu_key_off(uint64_t mask);
/* its envelope's level (the SPU's ENVX: 0 silent) and whether it is still sounding */
int spu_voice_level(int v);
int spu_voice_busy(int v);
/* the voices playing from `mem` stopped dead (their bank is going) */
void spu_forget(const uint8_t *mem);

/* each core's reverb, the PS1's algorithm at half the rate: the presets as libsd has them (10
 * modes x 32 registers, dAPF1 .. vRIN; the work areas' sizes in 8-byte units), a core's mode
 * (0 off .. 9 pipe; sceSdSetEffectAttr) and the level it comes back at (EVOL: 0..0x7FFF) */
void spu_reverb_presets(const uint16_t regs[10][32], const uint32_t size[10]);
void spu_reverb_mode(int core, int mode);
void spu_reverb_volume(int core, int l, int r);

/* a hook run (lock held) before each slice of output is mixed: `sec` seconds of it */
void spu_on_tick(void (*fn)(double sec));
/* open (on the sound output's stream; done by the first use) */
void spu_start(void);

#endif
