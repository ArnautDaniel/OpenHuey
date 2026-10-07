/* The game's sound banks: see soundbank.h. */
#include "soundbank.h"

#include "../core/files.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t rd32(const uint8_t *p) { return p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24; }
static uint32_t rd16(const uint8_t *p) { return p[0] | p[1] << 8; }

int soundbank_load(SoundBank *b, const char *name) {
    char path[256];

    memset(b, 0, sizeof(*b));
    snprintf(path, sizeof(path), "%s.HD", name);
    b->hd = files_read(path, &b->hd_size);
    snprintf(path, sizeof(path), "%s.SDT", name);
    b->sdt = files_read(path, &b->sdt_size);
    snprintf(path, sizeof(path), "%s.BD", name);
    b->bd = files_read(path, &b->bd_size);
    if (b->hd == NULL || b->sdt == NULL || b->bd == NULL) {
        soundbank_free(b);
        return 0;
    }
    return 1;
}

void soundbank_free(SoundBank *b) {
    free(b->hd);
    free(b->sdt);
    free(b->bd);
    memset(b, 0, sizeof(*b));
}

/* ---- the sound table (an entry unpacked as the driver's parse_entry, type 1) ---- */

typedef struct Entry {
    int empty, more;    /* nothing to play; it goes on the sound before it */
    int prog, split, layers, note;
    int vol;            /* signed: spread over the program, split and sample volumes */
} Entry;

static Entry entry(const SoundBank *b, int i) {
    Entry e = {1, 0, 0, 0, 0, 0, 0};
    const uint8_t *p;

    if ((size_t)(i + 1) * 16 > b->sdt_size) {
        return e;
    }
    p = b->sdt + i * 16;
    e.more = (p[3] >> 5) & 4 ? 1 : 0;
    if (p[0] != 1) {
        return e;
    }
    e.empty = 0;
    e.vol = (int8_t)p[5];
    e.note = p[6];
    e.layers = p[7] & 7;
    e.prog = p[8] & 0x7F;
    e.split = p[9] & 0x7F;
    return e;
}

/* the first entry of sound `id` (the driver's find_sound) */
static int find_sound(const SoundBank *b, int id) {
    int n = 0, i = 0;

    while (n < id) {
        if ((size_t)(i + 1) * 16 > b->sdt_size) {
            return -1;
        }
        if (!entry(b, i).more) {
            n++;
        }
        i++;
    }
    return i;
}

/* ---- the header: a program's split's layers (the driver's read_tone) ---- */

typedef struct Layer {
    int note, fine, vol;
    uint32_t start;     /* in the .BD */
    int rate;
} Layer;

typedef struct Tone {
    int prog_vol, prog_trans, prog_fine, split_vol, split_trans, split_fine;
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
    if (!in_hd(b, prog_off, 0x10)) {
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
    t->prog_trans = (int8_t)p[8];
    t->prog_fine = (int8_t)p[9];
    if (split >= p[4] || !in_hd(b, (uint32_t)(p - h) + rd32(p) + split * p[5], 0x14)) {
        return 0;
    }
    sp = p + rd32(p) + split * p[5];
    t->split_vol = sp[0x10];
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

        if (!in_hd(b, smpl_off + 0x10 + rd16(ss + 4 + i * 2) * 4, 4)) {
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
        t->l[i].vol = sm[0x10];
        o = vagi_off + 0x10 + (rd32(vc + 0xC) + 1) * 4 + rd16(sm) * 8;
        if (!in_hd(b, o, 6)) {
            t->n = i;
            break;
        }
        v = h + o;
        t->l[i].start = rd32(v);
        t->l[i].rate = (int)rd16(v + 4);
    }
    return 1;
}

/* ---- PS2 ADPCM: 16-byte blocks of 28 samples (shift / filter, flags, 14 bytes of nibbles) ---- */

static int16_t *decode_vag(const SoundBank *b, uint32_t start, int *n) {
    static const int f0[5] = {0, 60, 115, 98, 122}, f1[5] = {0, 0, -52, -55, -60};
    int cap = 28 * 64, count = 0, s1 = 0, s2 = 0;
    int16_t *out = malloc((size_t)cap * sizeof(int16_t));
    uint32_t a;

    for (a = start; out != NULL && a + 16 <= b->bd_size; a += 16) {
        const uint8_t *blk = b->bd + a;
        int shift = blk[0] & 0xF, filter = (blk[0] >> 4) & 7, k;

        if (filter > 4) {
            filter = 0;
        }
        if (count + 28 > cap) {
            int16_t *more = realloc(out, (size_t)(cap *= 2) * sizeof(int16_t));

            if (more == NULL) {
                break;
            }
            out = more;
        }
        for (k = 0; k < 28; k++) {
            int nib = (blk[2 + k / 2] >> ((k & 1) * 4)) & 0xF, s;

            s = (int16_t)(nib << 12) >> shift;
            s += (s1 * f0[filter] + s2 * f1[filter] + 32) >> 6;
            s = s > 32767 ? 32767 : s < -32768 ? -32768 : s;
            s2 = s1;
            s1 = s;
            out[count++] = (int16_t)s;
        }
        if (blk[1] & 1) {   /* the end (a loop's end too: played once here) */
            break;
        }
        if (count > 48000 * 20) {
            break;
        }
    }
    *n = count;
    return out;
}

int16_t *soundbank_render(const SoundBank *b, int id, int rate, int *n) {
    int first = find_sound(b, id), i, total = 0;
    float *mix = NULL;
    int16_t *out;

    *n = 0;
    if (first < 0) {
        return NULL;
    }
    for (i = first;; i++) {
        Entry e = entry(b, i);
        Tone t;
        int l;

        if (!e.empty && read_tone(b, e.prog, e.split, &t)) {
            int layers = e.layers + 1 < t.n ? e.layers + 1 : t.n;

            for (l = 0; l < layers; l++) {
                Layer *ly = &t.l[l];
                int vn, k, len, p = t.prog_vol, s = t.split_vol, q = ly->vol, vol;
                int16_t *v = decode_vag(b, ly->start, &vn);
                double note = t.prog_trans + ly->note + t.split_trans + e.note;
                double fine = ly->fine + t.prog_fine + t.split_fine;
                double hz = ly->rate * pow(2.0, (note - ly->note + fine / 128.0) / 12.0), step = hz / rate;

                /* the volume: program x split x sample, the entry's offset spread over them */
                if (e.vol > 0) {
                    int rest = e.vol;

                    if (p + rest <= 0x80) {
                        p += rest;
                    } else {
                        rest -= 0x80 - p;
                        p = 0x80;
                        if (s + rest <= 0x80) {
                            s += rest;
                        } else {
                            q += rest - (0x80 - s);
                            s = 0x80;
                            q = q > 0x80 ? 0x80 : q;
                        }
                    }
                } else if (e.vol < 0) {
                    int rest = e.vol;

                    if (p + rest >= 0) {
                        p += rest;
                    } else {
                        rest += p;
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
                vol = q * (p * s) / 170;
                vol = vol > 0x3FFF ? 0x3FFF : vol;
                if (v == NULL || vn == 0 || step <= 0.0) {
                    free(v);
                    continue;
                }
                len = (int)(vn / step);
                if (len > total) {
                    float *more = realloc(mix, (size_t)len * sizeof(float));

                    if (more == NULL) {
                        free(v);
                        continue;
                    }
                    memset(more + total, 0, (size_t)(len - total) * sizeof(float));
                    mix = more;
                    total = len;
                }
                for (k = 0; k < len; k++) {   /* (linear between the decoded samples) */
                    double at = k * step;
                    int j = (int)at;
                    double fr = at - j, a = v[j], c = j + 1 < vn ? v[j + 1] : 0;

                    mix[k] += (float)((a + (c - a) * fr) * vol / 16383.0);
                }
                free(v);
            }
        }
        if (!entry(b, i + 1).more || i > first + 64) {   /* (the next entry says it belongs) */
            break;
        }
    }
    if (mix == NULL) {
        return NULL;
    }
    out = malloc((size_t)total * sizeof(int16_t));
    for (i = 0; out != NULL && i < total; i++) {
        float x = mix[i];

        out[i] = (int16_t)(x > 32767.0f ? 32767 : x < -32768.0f ? -32768 : x);
    }
    free(mix);
    *n = out != NULL ? total : 0;
    return out;
}
