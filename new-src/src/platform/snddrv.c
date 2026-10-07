/* The sound effects driver: see snddrv.h. */
#include "snddrv.h"

#include "../data/soundbank.h"
#include "spu.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NBANK 8
#define NV SPU_VOICES

typedef struct Bank {
    SoundBank b;
    char name[64];
    uint8_t *curve;     /* each sound's 3-D curve (its first entry's byte 0xA: SndTable_Curves) */
    int nsounds;
} Bank;

/* the driver's voice records (IRX D_0000D670) */
typedef struct VoiceRec {
    int pri;            /* the sound's priority (0: free) */
    int busy;
    int placed;         /* its volumes from the 3-D placement */
    int bank, entry;    /* the sound playing (-1: none) */
    int pan;            /* pan 0..127, or the placement's left << 16 | right */
    int vol;            /* its volume (14 bits) */
    int chvol;          /* the voice's own volume (0..255) */
    int last_l, last_r; /* the volumes set */
} VoiceRec;

static struct {
    Bank bank[NBANK];
    VoiceRec v[NV];
    int master;         /* 0..255 (D_0000DF84): sound x master */
    float sound_vol, master_vol;
    int stereo;         /* D_0000DF80; the EE's output mode */
    float placed_scale; /* the driver's +0x1D4 */
    float progress_scale;   /* progress +0x1118 */
    int inited;
    int32_t curves[0x600 / 4];
    float pan_r[0x401], pan_l[0x401];
    uint8_t positioned[8][2];
    int have_tables;
} D;

static uint32_t rd32(const uint8_t *p) { return p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24; }
static uint32_t rd16(const uint8_t *p) { return p[0] | p[1] << 8; }

static void init(void) {
    int v;

    if (D.inited) {
        return;
    }
    D.inited = 1;
    D.sound_vol = D.master_vol = 1.0f;
    D.master = 0xFF;
    D.stereo = 1;
    D.placed_scale = 1.0f;
    D.progress_scale = 1.0f;
    for (v = 0; v < NV; v++) {
        D.v[v].chvol = 0xFF;
        D.v[v].bank = D.v[v].entry = -1;
        D.v[v].pan = 0x40;
    }
    spu_start();
}

void snddrv_tables(const uint8_t *curves, const uint8_t *pan_r, const uint8_t *pan_l, const uint8_t *positioned) {
    int i;

    spu_lock();
    for (i = 0; i < 0x600 / 4; i++) {
        D.curves[i] = (int32_t)rd32(curves + i * 4);
    }
    for (i = 0; i <= 0x400; i++) {
        uint32_t r = rd32(pan_r + i * 4), l = rd32(pan_l + i * 4);

        memcpy(&D.pan_r[i], &r, 4);
        memcpy(&D.pan_l[i], &l, 4);
    }
    memcpy(D.positioned, positioned, 16);
    D.have_tables = 1;
    spu_unlock();
}

/* ---- the sound table ---- */

/* an entry unpacked (IRX 0x39FC, type 1) */
typedef struct Entry {
    unsigned flags;     /* 1 even priority wins, 2 fixed pan, 4 continues the sound before, 0x10 any
                           free voice, 0x20 empty */
    int prog, split, voice, pri, pan, vol, note, layers, curve;
} Entry;

static Entry entry(const SoundBank *b, int i) {
    Entry o;
    const uint8_t *e;

    memset(&o, 0, sizeof(o));
    if (i < 0 || (size_t)(i + 1) * 16 > b->sdt_size) {
        o.flags = 0x20;
        return o;
    }
    e = b->sdt + i * 16;
    if (e[0] == 1) {
        o.flags |= (e[2] >> 7) & 1;
        o.pri = e[2] & 0x7F;
        o.flags |= (e[3] >> 5) & 4;
        o.flags |= (e[3] >> 3) & 8;
        o.voice = e[3] & 0x3F;
        o.flags |= (e[4] >> 6) & 2;
        o.pan = e[4] & 0x7F;
        o.vol = (int8_t)e[5];
        o.note = e[6];
        o.layers = e[7] & 7;
        o.flags |= (e[7] * 2) & 0x10;
        o.prog = e[8] & 0x7F;
        o.split = e[9] & 0x7F;
        o.curve = e[0xA];
    } else {
        o.flags |= 0x20;
        if (e[0] == 0) {
            o.flags |= (e[3] >> 5) & 4;
        }
    }
    return o;
}

/* the first entry of sound `id` (-1: past the table) */
static int find_sound(const SoundBank *b, unsigned id) {
    unsigned n = 0;
    int i = 0;

    while (n < id) {
        if ((size_t)(i + 1) * 16 > b->sdt_size) {
            return -1;
        }
        if (!(entry(b, i).flags & 4)) {
            n++;
        }
        i++;
    }
    return (size_t)(i + 1) * 16 <= b->sdt_size ? i : -1;
}

/* SndTable_Curves: each sound's curve */
static void table_curves(Bank *k) {
    int n = 0, i;

    free(k->curve);
    k->curve = malloc(k->b.sdt_size / 16 + 1);
    for (i = 0; k->curve != NULL && (size_t)(i + 1) * 16 <= k->b.sdt_size; i++) {
        if (!(entry(&k->b, i).flags & 4)) {
            k->curve[n++] = k->b.sdt[i * 16 + 0xA];
        }
    }
    k->nsounds = n;
}

/* ---- the header: a program's split's sample layers (IRX 0x3CF4) ---- */

typedef struct Layer {
    int note, fine, pan, vol, mix;
    unsigned adsr1, adsr2;
    uint32_t ssa;
    int rate;
} Layer;

typedef struct Tone {
    int prog_vol, prog_pan, prog_trans, prog_fine;
    int split_vol, split_pan, split_trans, split_fine;
    int n;
    Layer l[16];
} Tone;

static int in_hd(const SoundBank *b, uint32_t off, uint32_t n) { return off < b->hd_size && n <= b->hd_size - off; }

static int read_tone(const SoundBank *b, int prog, int split, Tone *t) {
    const uint8_t *h = b->hd, *pc, *p, *sp, *ss, *vc;
    uint32_t prog_off, sset_off, smpl_off, vagi_off, o;
    int i;

    memset(t, 0, sizeof(*t));
    if (!in_hd(b, 0, 0x40)) {
        return 0;
    }
    prog_off = rd32(h + 0x24);
    sset_off = rd32(h + 0x28);
    smpl_off = rd32(h + 0x2C);
    vagi_off = rd32(h + 0x30);
    if (!in_hd(b, prog_off, 0x10) || !in_hd(b, vagi_off, 0x10)) {
        return 0;
    }
    pc = h + prog_off;
    if (prog > (int)rd32(pc + 0xC) || !in_hd(b, prog_off + 0x10 + prog * 4, 4)) {
        return 0;
    }
    o = rd32(pc + 0x10 + prog * 4);
    if (o == 0xFFFFFFFF || !in_hd(b, prog_off + o, 0x24)) {
        return 0;
    }
    p = pc + o;
    t->prog_vol = p[6];
    t->prog_pan = p[7];
    t->prog_trans = (int8_t)p[8];
    t->prog_fine = (int8_t)p[9];
    if (split >= p[4] || !in_hd(b, (uint32_t)(p - h) + rd32(p) + split * p[5], 0x14)) {
        return 0;
    }
    sp = p + rd32(p) + split * p[5];
    t->split_vol = sp[0x10];
    t->split_pan = sp[0x11];
    t->split_trans = (int8_t)sp[0x12];
    t->split_fine = (int8_t)sp[0x13];
    if (!in_hd(b, sset_off + 0x10 + rd16(sp) * 4, 4)) {
        return 0;
    }
    o = sset_off + rd32(h + sset_off + 0x10 + rd16(sp) * 4);
    if (!in_hd(b, o, 4)) {
        return 0;
    }
    ss = h + o;
    t->n = ss[3] > 16 ? 16 : ss[3];
    vc = h + vagi_off;
    for (i = 0; i < t->n; i++) {
        const uint8_t *sm, *v;
        uint32_t so;

        if (!in_hd(b, (uint32_t)(ss - h) + 4 + i * 2, 2) || !in_hd(b, smpl_off + 0x10 + rd16(ss + 4 + i * 2) * 4, 4)) {
            t->n = i;
            break;
        }
        so = smpl_off + rd32(h + smpl_off + 0x10 + rd16(ss + 4 + i * 2) * 4);
        if (!in_hd(b, so, 0x2A)) {
            t->n = i;
            break;
        }
        sm = h + so;
        t->l[i].note = sm[0xB];
        t->l[i].fine = (int8_t)sm[0xC];
        t->l[i].pan = sm[0xD];
        t->l[i].vol = sm[0x10];
        t->l[i].adsr1 = rd16(sm + 0x12);
        t->l[i].adsr2 = rd16(sm + 0x14);
        t->l[i].mix = sm[0x29];   /* (spuAttr: dry / wet left / right; IRX 0xBB20) */
        o = vagi_off + 0x10 + (rd32(vc + 0xC) + 1) * 4 + rd16(sm) * 8;
        if (!in_hd(b, o, 6)) {
            t->n = i;
            break;
        }
        v = h + o;
        t->l[i].ssa = rd32(v);
        t->l[i].rate = (int)rd16(v + 4);
    }
    return 1;
}

/* ---- the voices ---- */

/* the records of the voices gone quiet are cleared (the driver's main loop) */
static void voices_sweep(void) {
    int v;

    for (v = 0; v < NV; v++) {
        VoiceRec *r = &D.v[v];

        if (!spu_voice_busy(v)) {
            r->busy = 0;
            r->pri = 0;
            r->bank = -1;
            r->entry = -1;
            r->vol = 0;
            r->pan = r->placed == 1 ? 0 : 0x40;
        } else {
            r->busy = 1;
        }
    }
}

/* a voice's volumes from its volume (IRX 0x4244) */
static void voice_lr(int v, int vol14, int *l, int *r) {
    VoiceRec *rec = &D.v[v];
    int x = (int)((unsigned)((((unsigned)(((vol14 << 8) / 255) * D.master) >> 8) << 8) / 255) * rec->chvol) >> 8;
    int pan;

    if (rec->placed) {
        *l = (int16_t)(rec->pan >> 16) * (int16_t)x / 8192;
        *r = (int16_t)rec->pan * (int16_t)x / 8192;
        return;
    }
    pan = D.stereo ? rec->pan : 0x40;
    if (pan >= 0 && pan < 0x40) {
        *l = x;
        *r = x * (pan * 2) >> 7;
    } else if (pan > 0x40 && pan < 0x80) {
        if (pan == 0x7F) {
            pan = 0x80;
        }
        *r = x;
        *l = x * (0x100 - pan * 2) >> 7;
    } else {
        *l = *r = x;
    }
}

/* sceSdNote2Pitch: 0x1000 at the sample's own note; fine in 128ths of a semitone */
static unsigned note_pitch(int base, int note, int fine) {
    double p = 4096.0 * pow(2.0, ((note - base) + fine / 128.0) / 12.0);

    return p > 0xFFFF ? 0xFFFF : (unsigned)p;
}

/* the spread of an entry's volume offset over the program, split and sample volumes */
static int tone_volume(int p, int s, int q, int ev) {
    if (ev > 0) {
        if (p + ev <= 0x80) {
            p += ev;
        } else {
            int rest = ev - (0x80 - p);

            p = 0x80;
            if (s + rest <= 0x80) {
                s += rest;
            } else {
                q += rest - (0x80 - s);
                s = 0x80;
                q = q > 0x80 ? 0x80 : q;
            }
        }
    } else if (ev < 0) {
        if (p + ev >= 0) {
            p += ev;
        } else {
            int rest = ev + p;

            p = 0;
            if (s + rest >= 0) {
                s += rest;
            } else {
                q += rest + s;
                s = 0;
                q = q < 0 ? 0 : q;
            }
        }
    }
    return q * (p * s);
}

/* command 0x26 (IRX 0x7AA0): w flags | id, arg vol << 8 | pitch << 16 by the flags, place the
 * placement's left << 16 | right. Flags: 0x80000000 key the voices (else a playing sound's
 * settings change), 0x20000000 the extended ones apply: 0x08000000 placed, 0x400000 pitch
 * offset, 0x200000 pitch set, 0x100000 volume scaled, 0x80000 volume offset, 0x40000 volume
 * set, 0x20000 pan offset, 0x10000 pan set */
static void play_sound(int bank, uint32_t w, uint32_t arg, uint32_t place) {
    unsigned id = w & 0xFFFF;
    int ext = (w & 0x20000000) != 0;
    const SoundBank *b;
    int first, nent, j;

    if (bank < 0 || bank >= NBANK || D.bank[bank].b.sdt == NULL) {
        return;
    }
    b = &D.bank[bank].b;
    first = find_sound(b, id);
    if (first < 0) {
        return;
    }
    for (nent = 1; entry(b, first + nent).flags & 4; nent++) {
    }
    for (j = 0; j < nent; j++) {
        Entry e = entry(b, first + j);
        Tone tone;
        int v0, layers, l;
        uint64_t keys = 0;

        if (e.flags & 0x20) {
            continue;
        }
        /* the voice */
        if (!(e.flags & 0x10)) {
            v0 = e.voice >= NV ? NV - 1 : e.voice;
        } else {
            for (v0 = NV - 1; v0 >= 0 && spu_voice_busy(v0); v0--) {
            }
            if (v0 < 0) {
                continue;
            }
        }
        if (e.pri < D.v[v0].pri || (e.pri == D.v[v0].pri && !(e.flags & 1))) {
            continue;
        }
        if (!read_tone(b, e.prog, e.split, &tone)) {
            continue;
        }
        layers = e.layers + 1 < tone.n ? e.layers + 1 : tone.n;
        for (l = 0; l < layers && v0 + l < NV; l++) {
            keys |= 1ull << (v0 + l);
        }
        if (w & 0x80000000) {   /* the old sounds off first */
            spu_key_off(keys);
        }
        for (l = 0; l < layers && v0 + l < NV; l++) {
            Layer *ly = &tone.l[l];
            int v = v0 + l, pan, note, fine, vol, L, R;

            spu_voice_sample(v, b->bd, b->bd_size, ly->ssa);
            /* pan */
            if (e.flags & 2) {
                pan = e.pan;
            } else {
                pan = (int8_t)(ly->pan - 0x40) + (int8_t)(tone.prog_pan - 0x40) + (int8_t)(tone.split_pan - 0x40) + 0x40;
                pan = pan < 0 ? 0 : pan >= 0x80 ? 0x7F : pan;
                if (ext && (w & 0x10000)) {
                    pan = arg & 0x7F;
                } else if (ext && (w & 0x20000)) {
                    pan += (int8_t)arg;
                    pan = pan >= 0x80 ? 0x7F : pan < 0 ? 0 : pan;
                }
            }
            /* pitch */
            if (ext && (w & 0x200000)) {
                note = (arg >> 16) & 0x7F;
            } else {
                note = tone.prog_trans + ly->note + tone.split_trans + e.note;
                if (ext && (w & 0x400000)) {
                    note += (int8_t)(arg >> 16);
                    note = note >= 0x80 ? 0x7F : note < 0 ? 0 : note;
                }
            }
            fine = ly->fine + tone.prog_fine + tone.split_fine;
            spu_voice_pitch(v, note_pitch(ly->note, note, fine) * (unsigned)ly->rate / 48000u);
            spu_voice_adsr(v, ly->adsr1, ly->adsr2);
            spu_voice_mix(v, ly->mix);
            /* volume */
            vol = tone_volume(tone.prog_vol, tone.split_vol, ly->vol, e.vol);
            if (ext && (w & 0x40000)) {
                uint64_t x = (arg >> 8) & 0x7F;

                vol = (int)(x * x * x * 0xC0C0C0C1u >> 32 >> 7);
            } else if (ext && (w & 0x80000)) {
                vol = vol / 170 + ((int8_t)(arg >> 8) << 5);
                vol = (unsigned)vol >= 0x4000 ? 0x3FFF : vol;
            } else if (ext && (w & 0x100000)) {
                unsigned x = (unsigned)(vol / 170) << 8;

                vol = (int)((x + (x >> 7) * (int8_t)(arg >> 8)) >> 8);
                vol = (unsigned)vol >= 0x4000 ? 0x3FFF : vol;
            } else {
                vol = (int)((uint64_t)(unsigned)vol * 0xC0C0C0C1u >> 32 >> 7);
            }
            D.v[v].vol = vol;
            if (ext && (w & 0x08000000)) {
                D.v[v].placed = 1;
                D.v[v].pan = (int)place;
            } else {
                D.v[v].placed = 0;
                D.v[v].pan = pan;
            }
            voice_lr(v, vol, &L, &R);
            if (ext && (w & 0x08000000) && !(w & 0x80000000) && ((L ^ D.v[v].last_l) & 0x80000000)) {
                L = (L + D.v[v].last_l) / 2;   /* (a side crossing over: halfway this time) */
                R = (R + D.v[v].last_r) / 2;
            }
            spu_voice_volume(v, L, R);
            D.v[v].last_l = L;
            D.v[v].last_r = R;
            D.v[v].pri = e.pri;
        }
        if (w & 0x80000000) {
            for (l = 0; l < layers && v0 + l < NV; l++) {
                D.v[v0 + l].bank = bank;
                D.v[v0 + l].entry = first;
            }
            spu_key_on(keys);
        }
    }
}

/* ---- the 3-D placement (SndLib_Place) ---- */

typedef struct Place {
    float vol;          /* 0..1 */
    float scale;        /* the distance scale (the driver's +0x1D8: 1000) */
    float at[3], ahead[3];
    int curve;          /* the sound's curve (+8 with the alternate set) */
    int combine;        /* 0 both sides their average, 1 both positive, else as they come */
} Place;

static uint32_t place(const Place *p) {
    int at, facing, rel, k, i, v = 0;
    float dist;
    const int32_t *c;
    int l, r;

    /* (the view x mirrored as Sound_SetPosition does; the listener at the eye) */
    at = (int)(4096.0f / 6.28318530718f * atan2f(-p->at[0], p->at[2])) & 0xFFF;
    dist = sqrtf(p->at[0] * p->at[0] + p->at[1] * p->at[1] + p->at[2] * p->at[2]);
    facing = (int)(4096.0f / 6.28318530718f * atan2f(-p->ahead[0], p->ahead[2])) & 0xFFF;
    rel = (((facing - at + 0x1000) & 0xFFF) / 2 + 0x200) & 0x7FF;
    k = p->curve;
    if (rel > 0x400) {
        k += 4;
    }
    k &= 0xF;
    c = D.curves + k * 20;
    for (i = 1; i < 16 && k * 20 + i * 2 + 1 < 0x600 / 4; i++) {
        float d;

        if (c[i * 2 + 1] == 0) {
            v = 0;
            break;
        }
        d = p->scale * (float)c[i * 2 + 1] / 100.0f;
        if (dist < d) {
            float d0 = p->scale * (float)c[i * 2 - 1] / 100.0f;

            v = (int)((float)c[i * 2 - 2] + (float)(c[i * 2] - c[i * 2 - 2]) * (dist - d0) / (d - d0));
            break;
        }
    }
    v = (int16_t)((v << 13) / 100);
    if (rel > 0x400) {
        l = (int16_t)(int)(p->vol * ((float)v * -D.pan_l[0x800 - rel]));
        r = (int16_t)(int)(p->vol * ((float)v * D.pan_r[0x800 - rel]));
    } else {
        l = (int16_t)(int)(p->vol * ((float)v * D.pan_l[rel]));
        r = (int16_t)(int)(p->vol * ((float)v * D.pan_r[rel]));
    }
    switch (p->combine) {
    case 0:
        l = r = (abs(l) + abs(r)) / 2;
        break;
    case 1:
        l = abs(l);
        r = abs(r);
        break;
    }
    return (uint32_t)(l & 0xFFFF) << 16 | (uint32_t)(r & 0xFFFF);
}

/* ---- the game's side ---- */

int snddrv_bank(int k, const char *name) {
    Bank *b;
    SoundBank nb;

    if (k < 0 || k >= NBANK) {
        return 0;
    }
    b = &D.bank[k];
    if (name == NULL) {
        name = "";
    }
    if (strcmp(b->name, name) == 0 && (name[0] == 0 || b->b.hd != NULL)) {
        return name[0] == 0 || b->b.hd != NULL;
    }
    memset(&nb, 0, sizeof(nb));
    if (name[0] != 0 && !soundbank_load(&nb, name)) {
        fprintf(stderr, "sound: bank %d: can't load %s\n", k, name);
    }
    spu_lock();
    init();
    if (b->b.bd != NULL) {
        int v;

        spu_forget(b->b.bd);
        for (v = 0; v < NV; v++) {
            if (D.v[v].bank == k) {
                D.v[v].bank = D.v[v].entry = -1;
            }
        }
    }
    soundbank_free(&b->b);
    b->b = nb;
    snprintf(b->name, sizeof(b->name), "%s", name);
    table_curves(b);
    spu_unlock();
    return name[0] == 0 || b->b.hd != NULL;
}

const char *snddrv_bank_name(int k) { return k >= 0 && k < NBANK ? D.bank[k].name : ""; }

int snddrv_bank_loaded(int k) { return k >= 0 && k < NBANK && D.bank[k].b.sdt != NULL; }

/* LIBSD.IRX (its 2004 build in the game's MODULES): the effect presets at 0x4E94, 0x44 bytes
 * a mode (32 registers in the PS1's order, then 2 spare), their work areas' sizes at 0x4E68 */
int snddrv_reverb_setup(const uint8_t *libsd, size_t n) {
    uint16_t regs[10][32];
    uint32_t size[10];
    int m, i;

    if (libsd == NULL || n < 0x4E94 + 10 * 0x44 || rd16(libsd + 0x4E94 + 0x44) != 0x007D ||
        rd16(libsd + 0x4E94 + 0x44 + 2) != 0x005B) {
        return 0;
    }
    for (m = 0; m < 10; m++) {
        for (i = 0; i < 32; i++) {
            regs[m][i] = (uint16_t)rd16(libsd + 0x4E94 + m * 0x44 + i * 2);
        }
        size[m] = rd32(libsd + 0x4E68 + m * 4);
    }
    spu_lock();
    init();
    spu_reverb_presets(regs, size);
    spu_reverb_mode(0, 3);
    spu_reverb_mode(1, 3);
    spu_reverb_volume(0, 0, 0);
    spu_reverb_volume(1, 0, 0);
    spu_unlock();
    return 1;
}

void snddrv_reverb_volume(int k, int v) {
    if (k >= 0 && k < 2) {
        spu_lock();
        spu_reverb_volume(k, v & 0x3FFF, v & 0x3FFF);
        spu_unlock();
    }
}

/* SndDriver_Play */
void snddrv_play(uint32_t id, int k) {
    if (k < 0 || k >= NBANK || D.bank[k].b.sdt == NULL) {
        return;
    }
    spu_lock();
    init();
    voices_sweep();
    play_sound(k, (id & 0xFFFFFF) | 0x04000000 | (id & 0x80000000 ? 0 : 0x80000000), 0x7840, 0);
    spu_unlock();
}

/* SndDriver_PlayPlaced, the block as Sound_SetPosition fills it; SndLib_Call places it with the
 * sound's curve */
void snddrv_play_placed(uint32_t id, int k, int vol, int pitch, const float at[3], const float ahead[3]) {
    Place p;
    uint32_t w, arg;

    if (k < 0 || k >= NBANK || D.bank[k].b.sdt == NULL || !D.have_tables) {
        return;
    }
    memset(&p, 0, sizeof(p));
    p.vol = (float)(int)fminf(127.0f * D.placed_scale * (id & 0x40000000 ? D.progress_scale : 1.0f), 127.0f) / 127.0f;
    p.scale = 1000.0f;
    memcpy(p.at, at, sizeof(p.at));
    memcpy(p.ahead, ahead, sizeof(p.ahead));
    p.curve = (int)(id & 0xFFFF) < D.bank[k].nsounds ? D.bank[k].curve[id & 0xFFFF] : 0;
    p.combine = D.stereo;
    w = (id & 0xFFFFFF) | 0x2C500000 | (id & 0x80000000 ? 0 : 0x80000000);
    arg = ((uint32_t)vol << 8 & 0xFF00) | ((uint32_t)pitch << 16 & 0xFF0000);
    spu_lock();
    init();
    voices_sweep();
    play_sound(k, w, arg, place(&p));
    spu_unlock();
}

/* command 0x28: its voices released */
void snddrv_stop(uint32_t id, int k) {
    const SoundBank *b;
    int first, v;
    uint64_t off = 0;

    if (k < 0 || k >= NBANK || (b = &D.bank[k].b)->sdt == NULL) {
        return;
    }
    spu_lock();
    init();
    voices_sweep();
    first = find_sound(b, id & 0xFFFF);
    for (v = 0; v < NV; v++) {
        if (D.v[v].bank == k && D.v[v].entry == first) {
            off |= 1ull << v;
        }
    }
    spu_key_off(off);
    spu_unlock();
}

void snddrv_stop_masks(uint32_t core0, uint32_t core1) {
    spu_lock();
    init();
    spu_key_off((uint64_t)(core0 & 0xFFFFFF) | (uint64_t)(core1 & 0xFFFFFF) << 24);
    spu_unlock();
}

void snddrv_stop_all(void) { snddrv_stop_masks(0xFFFFFF, 0xFFFFFF); }

/* command 0x35, 2 on the positioned sounds' voices: their own volume, applied */
void snddrv_placed_volume(int s) {
    int i;

    spu_lock();
    init();
    voices_sweep();
    for (i = 0; i < 8; i++) {
        int v = 24 + (D.positioned[i][1] & 0x1F), L, R;

        if (v >= NV) {
            continue;
        }
        D.v[v].chvol = s & 0xFF;
        voice_lr(v, D.v[v].vol, &L, &R);
        spu_voice_volume(v, L, R);
    }
    spu_unlock();
}

/* SndDriver_SendMaster (command 0x24): the voices playing re-volumed */
void snddrv_volume(float sound, float master) {
    int v, x;

    spu_lock();
    init();
    D.sound_vol = sound < 0.0f ? 0.0f : sound > 1.0f ? 1.0f : sound;
    D.master_vol = master < 0.0f ? 0.0f : master > 1.0f ? 1.0f : master;
    x = (int)(255.0f * D.sound_vol * D.master_vol);
    D.master = x < 0 ? 0 : x > 0xFF ? 0xFF : x;
    for (v = 0; v < NV; v++) {
        if (spu_voice_busy(v) && D.v[v].bank >= 0) {
            int L, R;

            voice_lr(v, D.v[v].vol, &L, &R);
            spu_voice_volume(v, L, R);
        }
    }
    spu_unlock();
}

void snddrv_stereo(int on) {
    spu_lock();
    init();
    D.stereo = on != 0;
    spu_unlock();
}

void snddrv_placed_scale(float s) {
    spu_lock();
    init();
    D.placed_scale = s < 0.0f ? 0.0f : s;
    spu_unlock();
}

void snddrv_progress_scale(float s) {
    spu_lock();
    init();
    D.progress_scale = s < 0.0f ? 0.0f : s;
    spu_unlock();
}

int snddrv_voice(int v, int *bank, int *entry, int *pri, int *l, int *r) {
    if (v < 0 || v >= NV) {
        return 0;
    }
    spu_lock();
    init();
    voices_sweep();
    *bank = D.v[v].bank;
    *entry = D.v[v].entry;
    *pri = D.v[v].pri;
    *l = D.v[v].last_l;
    *r = D.v[v].last_r;
    spu_unlock();
    return D.v[v].busy;
}
