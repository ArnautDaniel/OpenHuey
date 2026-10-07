/* The sound processor's voices: see spu.h. (PS-ADPCM, pitch and the ADSR envelope as the
 * hardware does them: psx-spx.) */
#include "spu.h"

#include "sound.h"

#include <SDL3/SDL.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

enum { ENV_OFF, ENV_ATTACK, ENV_DECAY, ENV_SUSTAIN, ENV_RELEASE };

typedef struct Voice {
    const uint8_t *mem;
    size_t size;
    size_t ssa, lsa, nax;       /* bytes into mem: the start, the loop, the block playing */
    int lsa_set;
    unsigned pitch;
    unsigned adsr1, adsr2;
    int vl, vr;
    int mix;                    /* dry L 1, dry R 2, wet L 4, wet R 8 */
    int env, level, counter;
    unsigned pos;               /* 12-bit fraction */
    int16_t buf[28];
    int s1, s2, idx, prev, cur;
} Voice;

#define HOOKS 4
#define SLICE 192   /* 4 ms: how often the hooks (the sequencer) run */

/* a core's reverb (psx-spx "SPU Reverb Formula"): its work area a ring of samples */
typedef struct Reverb {
    int mode;
    float r[32];            /* the volumes (as fractions) */
    int off[32];            /* the addresses (in samples) */
    float *buf;
    int size, pos;
    float evl, evr;
    float in_l, in_r;       /* half a step's input */
    float out_l, out_r, last_l, last_r;
    int phase;
} Reverb;

enum {
    dAPF1, dAPF2, vIIR, vCOMB1, vCOMB2, vCOMB3, vCOMB4, vWALL, vAPF1, vAPF2, mLSAME, mRSAME, mLCOMB1, mRCOMB1,
    mLCOMB2, mRCOMB2, dLSAME, dRSAME, mLDIFF, mRDIFF, mLCOMB3, mRCOMB3, mLCOMB4, mRCOMB4, dLDIFF, dRDIFF, mLAPF1,
    mRAPF1, mLAPF2, mRAPF2, vLIN, vRIN
};

static struct {
    SDL_Mutex *lock;
    Voice voice[SPU_VOICES];
    Reverb rev[2];
    uint16_t presets[10][32];
    uint32_t sizes[10];
    int have_presets;
    void (*hook[HOOKS])(double sec);
    int started;
} S;

void spu_lock(void) {
    if (S.lock == NULL) {
        S.lock = SDL_CreateMutex();
    }
    SDL_LockMutex(S.lock);
}

void spu_unlock(void) { SDL_UnlockMutex(S.lock); }

/* ---- a voice ---- */

static const int kFilter[5][2] = {{0, 0}, {60, 0}, {115, -52}, {98, -55}, {122, -60}};

static void decode_block(Voice *v) {
    const uint8_t *b;
    int shift, f, i;

    if (v->mem == NULL || v->nax + 16 > v->size) {
        v->env = ENV_OFF;
        v->level = 0;
        return;
    }
    b = v->mem + (v->nax & ~(size_t)0xF);
    shift = b[0] & 0xF;
    f = (b[0] >> 4) & 7;
    shift = shift > 12 ? 9 : shift;
    f = f > 4 ? 0 : f;
    if ((b[1] & 4) && !v->lsa_set) {   /* the loop's start */
        v->lsa = v->nax;
    }
    for (i = 0; i < 28; i++) {
        int n = (b[2 + i / 2] >> ((i & 1) * 4)) & 0xF;
        int s = (int16_t)(n << 12) >> shift;

        s += (v->s1 * kFilter[f][0] + v->s2 * kFilter[f][1]) / 64;
        s = s > 32767 ? 32767 : s < -32768 ? -32768 : s;
        v->buf[i] = (int16_t)s;
        v->s2 = v->s1;
        v->s1 = s;
    }
}

static void next_block(Voice *v) {
    const uint8_t *b = v->mem + (v->nax & ~(size_t)0xF);

    if (b[1] & 1) {   /* the end: back to the loop, or silence */
        if (b[1] & 2) {
            v->nax = v->lsa;
        } else {
            v->env = ENV_OFF;
            v->level = 0;
            return;
        }
    } else {
        v->nax += 16;
    }
    decode_block(v);
}

static void env_step(Voice *v, int exp, int dec, int shift, int step) {
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
    v->level = v->level > 0x7FFF ? 0x7FFF : v->level < 0 ? 0 : v->level;
}

static void env_tick(Voice *v) {
    unsigned a1 = v->adsr1, a2 = v->adsr2;

    switch (v->env) {
    case ENV_ATTACK:
        env_step(v, a1 >> 15, 0, (a1 >> 10) & 0x1F, (a1 >> 8) & 3);
        if (v->level >= 0x7FFF) {
            v->env = ENV_DECAY;
            v->counter = 0;
        }
        break;
    case ENV_DECAY:
        env_step(v, 1, 1, (a1 >> 4) & 0xF, 0);
        if (v->level <= (int)(((a1 & 0xF) + 1) * 0x800)) {
            v->env = ENV_SUSTAIN;
            v->counter = 0;
        }
        break;
    case ENV_SUSTAIN:
        env_step(v, a2 >> 15, (a2 >> 14) & 1, (a2 >> 8) & 0x1F, (a2 >> 6) & 3);
        break;
    case ENV_RELEASE:
        env_step(v, (a2 >> 5) & 1, 1, a2 & 0x1F, 0);
        if (v->level <= 0) {
            v->env = ENV_OFF;
        }
        break;
    }
}

static void key_on(Voice *v) {
    v->nax = v->ssa;
    v->lsa = v->ssa;
    v->lsa_set = 0;
    v->s1 = v->s2 = 0;
    v->pos = 0;
    v->idx = 0;
    v->prev = 0;
    v->env = ENV_ATTACK;
    v->level = 0;
    v->counter = 0;
    decode_block(v);
    v->cur = v->buf[0];
}

static void key_off(Voice *v) {
    if (v->env != ENV_OFF) {
        v->env = ENV_RELEASE;
        v->counter = 0;
    }
}

/* ---- the game's side ---- */

void spu_voice_sample(int v, const uint8_t *mem, size_t size, size_t ssa) {
    if (v >= 0 && v < SPU_VOICES) {
        S.voice[v].mem = mem;
        S.voice[v].size = size;
        S.voice[v].ssa = ssa & ~(size_t)0xF;
    }
}

void spu_voice_pitch(int v, unsigned pitch) {
    if (v >= 0 && v < SPU_VOICES) {
        S.voice[v].pitch = pitch > 0x3FFF ? 0x3FFF : pitch;
    }
}

void spu_voice_adsr(int v, unsigned adsr1, unsigned adsr2) {
    if (v >= 0 && v < SPU_VOICES) {
        S.voice[v].adsr1 = adsr1 & 0xFFFF;
        S.voice[v].adsr2 = adsr2 & 0xFFFF;
    }
}

void spu_voice_volume(int v, int l, int r) {
    if (v >= 0 && v < SPU_VOICES) {   /* (15 bits, signed) */
        S.voice[v].vl = (int16_t)(l << 1) >> 1;
        S.voice[v].vr = (int16_t)(r << 1) >> 1;
    }
}

void spu_voice_mix(int v, int mix) {
    if (v >= 0 && v < SPU_VOICES) {
        S.voice[v].mix = mix & 0xF;
    }
}

void spu_key_on(uint64_t mask) {
    int v;

    for (v = 0; v < SPU_VOICES; v++) {
        if (mask >> v & 1) {
            key_on(&S.voice[v]);
        }
    }
}

void spu_key_off(uint64_t mask) {
    int v;

    for (v = 0; v < SPU_VOICES; v++) {
        if (mask >> v & 1) {
            key_off(&S.voice[v]);
        }
    }
}

int spu_voice_level(int v) { return v >= 0 && v < SPU_VOICES ? S.voice[v].level : 0; }

int spu_voice_busy(int v) {
    /* (just keyed on, it has not had a sample yet: sounding all the same) */
    return v >= 0 && v < SPU_VOICES && S.voice[v].env != ENV_OFF && (S.voice[v].level > 0 || S.voice[v].env == ENV_ATTACK);
}

void spu_forget(const uint8_t *mem) {
    int v;

    for (v = 0; v < SPU_VOICES; v++) {
        if (S.voice[v].mem == mem) {
            S.voice[v].env = ENV_OFF;
            S.voice[v].level = 0;
            S.voice[v].mem = NULL;
        }
    }
}

/* ---- the reverb ---- */

void spu_reverb_presets(const uint16_t regs[10][32], const uint32_t size[10]) {
    memcpy(S.presets, regs, sizeof(S.presets));
    memcpy(S.sizes, size, sizeof(S.sizes));
    S.have_presets = 1;
}

void spu_reverb_mode(int core, int mode) {
    Reverb *r;
    int i;

    if (core < 0 || core > 1 || mode < 0 || mode > 9 || !S.have_presets) {
        return;
    }
    r = &S.rev[core];
    free(r->buf);
    memset(r, 0, offsetof(Reverb, evl));
    r->mode = mode;
    r->size = (int)S.sizes[mode] * 4;   /* (8-byte units: four samples) */
    r->buf = mode != 0 && r->size > 0 ? calloc((size_t)r->size, sizeof(float)) : NULL;
    for (i = 0; i < 32; i++) {
        r->r[i] = (int16_t)S.presets[mode][i] / 32768.0f;
        r->off[i] = S.presets[mode][i] * 4;
    }
}

void spu_reverb_volume(int core, int l, int r) {
    if (core >= 0 && core <= 1) {
        S.rev[core].evl = (int16_t)l / 32768.0f;
        S.rev[core].evr = (int16_t)r / 32768.0f;
    }
}

static float *rv_at(Reverb *r, int off) {
    int i = (r->pos + off) % r->size;

    return &r->buf[i < 0 ? i + r->size : i];
}

static float rv_sat(float x) { return x > 1.0f ? 1.0f : x < -1.0f ? -1.0f : x; }

/* one step (every other output sample) on the input l, r: the output in out_l, out_r */
static void reverb_step(Reverb *r, float l, float rr) {
    const float *v = r->r;
    const int *o = r->off;
    float lin = v[vLIN] * l, rin = v[vRIN] * rr, lo, ro, t;

    *rv_at(r, o[mLSAME]) = rv_sat((lin + *rv_at(r, o[dLSAME]) * v[vWALL] - *rv_at(r, o[mLSAME] - 1)) * v[vIIR] + *rv_at(r, o[mLSAME] - 1));
    *rv_at(r, o[mRSAME]) = rv_sat((rin + *rv_at(r, o[dRSAME]) * v[vWALL] - *rv_at(r, o[mRSAME] - 1)) * v[vIIR] + *rv_at(r, o[mRSAME] - 1));
    *rv_at(r, o[mLDIFF]) = rv_sat((lin + *rv_at(r, o[dRDIFF]) * v[vWALL] - *rv_at(r, o[mLDIFF] - 1)) * v[vIIR] + *rv_at(r, o[mLDIFF] - 1));
    *rv_at(r, o[mRDIFF]) = rv_sat((rin + *rv_at(r, o[dLDIFF]) * v[vWALL] - *rv_at(r, o[mRDIFF] - 1)) * v[vIIR] + *rv_at(r, o[mRDIFF] - 1));
    lo = v[vCOMB1] * *rv_at(r, o[mLCOMB1]) + v[vCOMB2] * *rv_at(r, o[mLCOMB2]) + v[vCOMB3] * *rv_at(r, o[mLCOMB3]) + v[vCOMB4] * *rv_at(r, o[mLCOMB4]);
    ro = v[vCOMB1] * *rv_at(r, o[mRCOMB1]) + v[vCOMB2] * *rv_at(r, o[mRCOMB2]) + v[vCOMB3] * *rv_at(r, o[mRCOMB3]) + v[vCOMB4] * *rv_at(r, o[mRCOMB4]);
    t = *rv_at(r, o[mLAPF1] - o[dAPF1]);
    lo -= v[vAPF1] * t;
    *rv_at(r, o[mLAPF1]) = rv_sat(lo);
    lo = lo * v[vAPF1] + t;
    t = *rv_at(r, o[mRAPF1] - o[dAPF1]);
    ro -= v[vAPF1] * t;
    *rv_at(r, o[mRAPF1]) = rv_sat(ro);
    ro = ro * v[vAPF1] + t;
    t = *rv_at(r, o[mLAPF2] - o[dAPF2]);
    lo -= v[vAPF2] * t;
    *rv_at(r, o[mLAPF2]) = rv_sat(lo);
    lo = lo * v[vAPF2] + t;
    t = *rv_at(r, o[mRAPF2] - o[dAPF2]);
    ro -= v[vAPF2] * t;
    *rv_at(r, o[mRAPF2]) = rv_sat(ro);
    ro = ro * v[vAPF2] + t;
    r->pos = (r->pos + 1) % r->size;
    r->last_l = r->out_l;
    r->last_r = r->out_r;
    r->out_l = lo;
    r->out_r = ro;
}

/* the cores' reverbs over `frames` of wet input, added into out */
static void reverb_mix(float *out, float wet[2][SLICE * 2], int frames) {
    int c, i;

    for (c = 0; c < 2; c++) {
        Reverb *r = &S.rev[c];

        if (r->buf == NULL) {
            continue;
        }
        for (i = 0; i < frames; i++) {
            float l, rr;

            r->in_l += wet[c][2 * i] * 0.5f;
            r->in_r += wet[c][2 * i + 1] * 0.5f;
            if (r->phase ^= 1) {   /* (between steps: halfway from the last output to this one) */
                l = (r->last_l + r->out_l) * 0.5f;
                rr = (r->last_r + r->out_r) * 0.5f;
            } else {
                reverb_step(r, r->in_l, r->in_r);
                r->in_l = r->in_r = 0.0f;
                l = r->last_l;
                rr = r->last_r;
            }
            out[2 * i] += l * r->evl;
            out[2 * i + 1] += rr * r->evr;
        }
    }
}

/* ---- mixing ---- */

static void render(float *out, float wet[2][SLICE * 2], int frames) {
    int n, i;

    for (n = 0; n < SPU_VOICES; n++) {
        Voice *v = &S.voice[n];
        float gl = v->vl / 16384.0f, gr = v->vr / 16384.0f;
        float dl = v->mix & 1 ? gl : 0.0f, dr = v->mix & 2 ? gr : 0.0f;
        float wl = v->mix & 4 ? gl : 0.0f, wr = v->mix & 8 ? gr : 0.0f;
        float *w = wet[n / 24];

        for (i = 0; i < frames && v->env != ENV_OFF; i++) {
            float s;

            v->pos += v->pitch;
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
            if (v->env == ENV_OFF) {
                break;
            }
            s = (v->prev + (v->cur - v->prev) * (v->pos / 4096.0f)) / 32768.0f * (v->level / 32767.0f);
            out[2 * i] += s * dl;
            out[2 * i + 1] += s * dr;
            w[2 * i] += s * wl;
            w[2 * i + 1] += s * wr;
            env_tick(v);
        }
    }
}

static void mix(int16_t *out, int frames) {
    float buf[SLICE * 2], wet[2][SLICE * 2];
    int done = 0, i, h;

    spu_lock();
    while (done < frames) {
        int n = frames - done > SLICE ? SLICE : frames - done;

        for (h = 0; h < HOOKS; h++) {
            if (S.hook[h] != NULL) {
                S.hook[h]((double)n / SOUND_RATE);
            }
        }
        memset(buf, 0, sizeof(buf));
        memset(wet, 0, sizeof(wet));
        render(buf, wet, n);
        reverb_mix(buf, wet, n);
        for (i = 0; i < n * 2; i++) {
            int v = out[done * 2 + i] + (int)(buf[i] * 32767.0f);

            out[done * 2 + i] = (int16_t)(v > 32767 ? 32767 : v < -32768 ? -32768 : v);
        }
        done += n;
    }
    spu_unlock();
}

void spu_start(void) {
    if (!S.started) {
        S.started = 1;
        sound_stream_slot(2, mix);
    }
}

void spu_on_tick(void (*fn)(double sec)) {
    int h;

    spu_start();
    for (h = 0; h < HOOKS; h++) {
        if (S.hook[h] == fn) {
            return;
        }
    }
    for (h = 0; h < HOOKS; h++) {
        if (S.hook[h] == NULL) {
            S.hook[h] = fn;
            return;
        }
    }
}
