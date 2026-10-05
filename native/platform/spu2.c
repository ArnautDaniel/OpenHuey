/* The PS2 sound processor (SPU2) on PC, behind Sony's libsd interface the IOP sound modules
 * use (sceSdSetParam / SetSwitch / SetAddr / Note2Pitch ...): 2 cores x 24 voices playing
 * PS-ADPCM from sound memory (spu_ram, iop_mem.h) with the hardware's pitch (0x1000 = 48 kHz),
 * ADSR envelope and volumes, mixed into the SDL3 output (snd_mix.c). Reverb is not done. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iop_mem.h"
#include "snd.h"

#define NVOICE 48

/* libsd parameter / switch / address codes (entry = code | voice << 1 | core) */
#define SD_VP_VOLL 0x0000
#define SD_VP_VOLR 0x0100
#define SD_VP_PITCH 0x0200
#define SD_VP_ADSR1 0x0300
#define SD_VP_ADSR2 0x0400
#define SD_VP_ENVX 0x0500
#define SD_VP_VOLXL 0x0600
#define SD_VP_VOLXR 0x0700
#define SD_S_KON 0x1500
#define SD_S_KOFF 0x1600
#define SD_S_ENDX 0x1C00
#define SD_VA_SSA 0x2040
#define SD_VA_LSAX 0x2140
#define SD_VA_NAX 0x2240

enum { ENV_OFF, ENV_ATTACK, ENV_DECAY, ENV_SUSTAIN, ENV_RELEASE };

typedef struct Voice {
    unsigned short voll, volr, pitch, adsr1, adsr2;
    unsigned ssa, lsa, nax;     /* byte addresses in sound memory */
    int lsaSet;                 /* the loop start was given (not taken from the sample) */
    int env, level, counter;
    unsigned pos;               /* 12-bit fraction of the position in the block (28 samples) */
    short buf[28];
    int s1, s2;                 /* the decoder's history */
    int prev, cur, idx;         /* interpolation: the two samples around the position */
    unsigned mixL, mixR;
} Voice;

static Voice sVoice[NVOICE];
static unsigned sEndx[2];
static unsigned sSwitch[2][0x20];   /* other switch registers, by (code >> 8) - 0x10 */
static unsigned short sCoreParam[2][0x10];

static int voice_of(unsigned entry) {
    int v = (int)((entry >> 1) & 0x1F) + (int)(entry & 1) * 24;

    return v < NVOICE ? v : 0;
}

/* ---- PS-ADPCM ---- */

static const int sFilter[5][2] = {{0, 0}, {60, 0}, {115, -52}, {98, -55}, {122, -60}};

/* decode the block at nax into buf (and see its loop flags) */
static void decode_block(Voice *v) {
    const unsigned char *b = spu_ram + (v->nax & (SPU_RAM_SIZE - 1) & ~0xFu);
    int shift = b[0] & 0xF, f = (b[0] >> 4) & 7, i;

    if (shift > 12) {
        shift = 9;
    }
    if (f > 4) {
        f = 0;
    }
    if ((b[1] & 4) && !v->lsaSet) {
        v->lsa = v->nax;
    }
    for (i = 0; i < 28; i++) {
        int n = (b[2 + i / 2] >> ((i & 1) * 4)) & 0xF;
        int s = (short)(n << 12) >> shift;

        s += (v->s1 * sFilter[f][0] + v->s2 * sFilter[f][1]) / 64;
        if (s > 32767) {
            s = 32767;
        } else if (s < -32768) {
            s = -32768;
        }
        v->buf[i] = (short)s;
        v->s2 = v->s1;
        v->s1 = s;
    }
}

/* after the block: on to the next, the loop, or the end */
static void next_block(Voice *v) {
    const unsigned char *b = spu_ram + (v->nax & (SPU_RAM_SIZE - 1) & ~0xFu);
    int core = (int)(v - sVoice) / 24, bit = (int)(v - sVoice) % 24;

    if (b[1] & 1) {
        sEndx[core] |= 1u << bit;
        if (b[1] & 2) {
            v->nax = v->lsa;
        } else {
            v->env = ENV_OFF;
            v->level = 0;
            v->nax += 16;
        }
    } else {
        v->nax += 16;
    }
    decode_block(v);
}

/* ---- the envelope (as the hardware: psx-spx) ---- */

static void env_step(Voice *v, int exp, int dec, int shift, int step, int target) {
    int cycles = 1 << (shift - 11 > 0 ? shift - 11 : 0);
    int delta = (dec ? -8 + step : 7 - step) << (11 - shift > 0 ? 11 - shift : 0);

    if (exp && !dec && v->level > 0x6000) {
        cycles *= 4;
    }
    if (exp && dec) {
        delta = delta * v->level >> 15;
    }
    if (++v->counter < cycles) {
        return;
    }
    v->counter = 0;
    v->level += delta;
    if (v->level > 0x7FFF) {
        v->level = 0x7FFF;
    }
    if (v->level < 0) {
        v->level = 0;
    }
    (void)target;
}

static void env_tick(Voice *v) {
    unsigned a1 = v->adsr1, a2 = v->adsr2;

    switch (v->env) {
    case ENV_ATTACK:
        env_step(v, a1 >> 15, 0, (a1 >> 10) & 0x1F, (a1 >> 8) & 3, 0x7FFF);
        if (v->level >= 0x7FFF) {
            v->env = ENV_DECAY;
            v->counter = 0;
        }
        break;
    case ENV_DECAY:
        env_step(v, 1, 1, (a1 >> 4) & 0xF, 0, 0);
        if (v->level <= (int)(((a1 & 0xF) + 1) * 0x800)) {
            v->env = ENV_SUSTAIN;
            v->counter = 0;
        }
        break;
    case ENV_SUSTAIN:
        env_step(v, a2 >> 15, (a2 >> 14) & 1, (a2 >> 8) & 0x1F, (a2 >> 6) & 3, 0);
        break;
    case ENV_RELEASE:
        env_step(v, (a2 >> 5) & 1, 1, a2 & 0x1F, 0, 0);
        if (v->level <= 0) {
            v->env = ENV_OFF;
        }
        break;
    }
}

static int sLog = -1;

static void key_on(Voice *v) {
    if (sLog < 0) {
        sLog = getenv("HG_SNDLOG") != NULL;
    }
    if (sLog) {
        fprintf(stderr, "spu: key on %d ssa %05X pitch %04X adsr %04X %04X vol %04X %04X\n", (int)(v - sVoice), v->ssa,
                v->pitch, v->adsr1, v->adsr2, v->voll, v->volr);
        fprintf(stderr, "spu:   data %02X %02X %02X %02X %02X %02X %02X %02X\n", spu_ram[v->ssa], spu_ram[v->ssa + 1],
                spu_ram[v->ssa + 2], spu_ram[v->ssa + 3], spu_ram[v->ssa + 16], spu_ram[v->ssa + 17], spu_ram[v->ssa + 18],
                spu_ram[v->ssa + 19]);
    }
    v->nax = v->ssa;
    v->lsaSet = 0;
    v->lsa = v->ssa;
    v->s1 = v->s2 = 0;
    v->pos = 0;
    v->idx = 0;
    v->prev = v->cur = 0;
    v->env = ENV_ATTACK;
    v->level = 0;
    v->counter = 0;
    decode_block(v);
    v->cur = v->buf[0];
}

/* a 15-bit volume (or a sweep, taken at its level) as a gain */
static float vol_gain(unsigned short r) {
    int x;

    if (r & 0x8000) {   /* sweep mode: not modelled, half way */
        return 0.5f;
    }
    x = r & 0x7FFF;
    if (x & 0x4000) {
        x -= 0x8000;
    }
    return x / 16384.0f;
}

void spu_render(float *out, int frames) {
    int i, n;

    for (n = 0; n < NVOICE; n++) {
        Voice *v = &sVoice[n];
        float gl, gr;

        if (v->env == ENV_OFF) {
            continue;
        }
        gl = vol_gain(v->voll);
        gr = vol_gain(v->volr);
        for (i = 0; i < frames && v->env != ENV_OFF; i++) {
            unsigned step = v->pitch > 0x3FFF ? 0x3FFF : v->pitch;
            float s;

            v->pos += step;
            while (v->pos >= 0x1000) {
                v->pos -= 0x1000;
                v->prev = v->cur;
                if (++v->idx >= 28) {
                    v->idx = 0;
                    next_block(v);
                    if (v->env == ENV_OFF) {
                        break;
                    }
                }
                v->cur = v->buf[v->idx];
            }
            s = (v->prev + (v->cur - v->prev) * (v->pos / 4096.0f)) / 32768.0f * (v->level / 32767.0f);
            out[2 * i] += s * gl;
            out[2 * i + 1] += s * gr;
            env_tick(v);
        }
    }
}

/* ---- libsd ---- */

void sceSdSetParam(unsigned short entry, unsigned short value) {
    Voice *v;

    snd_lock();
    if ((entry & 0xFF00) <= SD_VP_VOLXR) {
        v = &sVoice[voice_of(entry)];
        switch (entry & 0xFF00) {
        case SD_VP_VOLL: v->voll = value; break;
        case SD_VP_VOLR: v->volr = value; break;
        case SD_VP_PITCH: v->pitch = value; break;
        case SD_VP_ADSR1: v->adsr1 = value; break;
        case SD_VP_ADSR2: v->adsr2 = value; break;
        case SD_VP_ENVX: v->level = value; break;
        }
    } else {
        sCoreParam[entry & 1][(entry >> 8) & 0xF] = value;
    }
    snd_unlock();
}

unsigned short sceSdGetParam(unsigned short entry) {
    Voice *v = &sVoice[voice_of(entry)];

    switch (entry & 0xFF00) {
    case SD_VP_VOLL: return v->voll;
    case SD_VP_VOLR: return v->volr;
    case SD_VP_PITCH: return v->pitch;
    case SD_VP_ADSR1: return v->adsr1;
    case SD_VP_ADSR2: return v->adsr2;
    case SD_VP_ENVX: return v->env == ENV_OFF ? 0 : (unsigned short)v->level;
    case SD_VP_VOLXL: return (unsigned short)(v->level * vol_gain(v->voll));
    case SD_VP_VOLXR: return (unsigned short)(v->level * vol_gain(v->volr));
    }
    return sCoreParam[entry & 1][(entry >> 8) & 0xF];
}

void sceSdSetSwitch(unsigned short entry, unsigned value) {
    int core = entry & 1, i;

    snd_lock();
    switch (entry & 0xFF00) {
    case SD_S_KON:
        for (i = 0; i < 24; i++) {
            if (value & (1u << i)) {
                key_on(&sVoice[core * 24 + i]);
                sEndx[core] &= ~(1u << i);
            }
        }
        break;
    case SD_S_KOFF:
        for (i = 0; i < 24; i++) {
            if ((value & (1u << i)) && sVoice[core * 24 + i].env != ENV_OFF) {
                sVoice[core * 24 + i].env = ENV_RELEASE;
                sVoice[core * 24 + i].counter = 0;
            }
        }
        break;
    case SD_S_ENDX:
        sEndx[core] = value;
        break;
    default:
        sSwitch[core][((entry >> 8) - 0x10) & 0x1F] = value;
        break;
    }
    snd_unlock();
}

unsigned sceSdGetSwitch(unsigned short entry) {
    int core = entry & 1;

    switch (entry & 0xFF00) {
    case SD_S_ENDX:
        return sEndx[core];
    case SD_S_KON:
    case SD_S_KOFF:
        return 0;
    }
    return sSwitch[core][((entry >> 8) - 0x10) & 0x1F];
}

void sceSdSetAddr(unsigned short entry, unsigned value) {
    Voice *v = &sVoice[voice_of(entry)];

    snd_lock();
    switch (entry & 0xFF00) {
    case SD_VA_SSA & 0xFF00:
        v->ssa = value & (SPU_RAM_SIZE - 1);
        break;
    case SD_VA_LSAX & 0xFF00:
        v->lsa = value & (SPU_RAM_SIZE - 1);
        v->lsaSet = 1;
        break;
    case SD_VA_NAX & 0xFF00:
        v->nax = value & (SPU_RAM_SIZE - 1);
        break;
    }
    snd_unlock();
}

unsigned sceSdGetAddr(unsigned short entry) {
    Voice *v = &sVoice[voice_of(entry)];

    switch (entry & 0xFF00) {
    case SD_VA_SSA & 0xFF00: return v->ssa;
    case SD_VA_LSAX & 0xFF00: return v->lsa;
    case SD_VA_NAX & 0xFF00: return v->nax;
    }
    return 0;
}

/* the pitch playing `note` (+ `fine` 1/128ths) of a sample recorded at `cnote` (+ `cfine`) */
unsigned short sceSdNote2Pitch(unsigned short cnote, unsigned short cfine, unsigned short note, short fine) {
    double semis = ((int)note - (int)cnote) + ((int)fine - (int)cfine) / 128.0;
    double p = 4096.0 * pow(2.0, semis / 12.0);

    if (p > 0x3FFF) {
        p = 0x3FFF;
    }
    return (unsigned short)p;
}

/* all voices off (a reset) */
void spu_reset(void) {
    snd_lock();
    memset(sVoice, 0, sizeof(sVoice));
    memset(sEndx, 0, sizeof(sEndx));
    snd_unlock();
}
