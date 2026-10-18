/* A sound bank's sample: see vag.h. */
#include "vag.h"

#include <stdlib.h>
#include <string.h>

static const int kFilter[5][2] = {{0, 0}, {60, 0}, {115, -52}, {98, -55}, {122, -60}};

static void decode_block(VagCursor *c) {
    const uint8_t *b;
    int shift, f, i;

    if (c->mem == NULL) {   /* (the samples themselves: the block at nax) */
        for (i = 0; i < VAG_BLOCK; i++) {
            c->buf[i] = (long)c->nax + i < c->frames ? c->pcm[c->nax + (size_t)i] : 0;
        }
        return;
    }
    if (c->nax + 16 > c->size) {
        c->ended = 1;
        return;
    }
    b = c->mem + (c->nax & ~(size_t)0xF);
    shift = b[0] & 0xF;
    f = (b[0] >> 4) & 7;
    shift = shift > 12 ? 9 : shift;
    f = f > 4 ? 0 : f;
    if (b[1] & 4) {   /* the loop's start */
        c->lsa = c->nax;
    }
    for (i = 0; i < VAG_BLOCK; i++) {
        int n = (b[2 + i / 2] >> ((i & 1) * 4)) & 0xF;
        int s = (int16_t)(n << 12) >> shift;

        s += (c->s1 * kFilter[f][0] + c->s2 * kFilter[f][1]) / 64;
        s = s > 32767 ? 32767 : s < -32768 ? -32768 : s;
        c->buf[i] = (int16_t)s;
        c->s2 = c->s1;
        c->s1 = s;
    }
}

void vag_start(VagCursor *c) {
    c->nax = c->ssa;
    c->lsa = c->ssa;
    c->s1 = c->s2 = 0;
    c->ended = c->mem == NULL && c->pcm == NULL;
    if (!c->ended) {
        decode_block(c);
    }
}

void vag_next(VagCursor *c) {
    if (c->ended) {
        return;
    }
    if (c->mem == NULL) {
        if ((long)c->nax + VAG_BLOCK >= c->frames) {   /* the last block played */
            if (c->loop_start < 0) {
                c->ended = 1;
                return;
            }
            c->nax = (size_t)c->loop_start;
        } else {
            c->nax += VAG_BLOCK;
        }
    } else {
        const uint8_t *b = c->mem + (c->nax & ~(size_t)0xF);

        if (b[1] & 1) {   /* the end: back to the loop, or silence */
            if (!(b[1] & 2)) {
                c->ended = 1;
                return;
            }
            c->nax = c->lsa;
        } else {
            c->nax += 16;
        }
    }
    decode_block(c);
}

/* ---- the conversion ---- */

/* the blocks played from here: `blocks` of them into out (whole blocks), how many there were
 * before it ended */
static long play(VagCursor *c, int16_t *out, long blocks) {
    long k;

    for (k = 0; k < blocks && !c->ended; k++) {
        memcpy(out + k * VAG_BLOCK, c->buf, sizeof(c->buf));
        vag_next(c);
    }
    return k;
}

int vag_pcm(const uint8_t *bd, size_t size, size_t ssa, int16_t **pcm, long *frames, long *loop_start) {
    VagCursor c;
    long most = (long)(size / 16) + 1, first, ls, body, k, i;
    int16_t *out, *again, *third;
    size_t end_at;

    memset(&c, 0, sizeof(c));
    c.mem = bd;
    c.size = size;
    c.ssa = ssa & ~(size_t)0xF;
    vag_start(&c);
    if (c.ended || (out = malloc(sizeof(int16_t) * VAG_BLOCK * (size_t)most * 2)) == NULL) {
        return 0;
    }
    /* once through: up to its end block (the jump, if it loops, comes after) */
    for (first = 0; first < most && !c.ended; first++) {
        const uint8_t *b = bd + (c.nax & ~(size_t)0xF);

        memcpy(out + first * VAG_BLOCK, c.buf, sizeof(c.buf));
        if (b[1] & 1) {
            first++;
            break;
        }
        vag_next(&c);
    }
    end_at = c.nax;
    vag_next(&c);
    if (c.ended) {   /* (it stops at its end) */
        *pcm = out;
        *frames = first * VAG_BLOCK;
        *loop_start = -1;
        return 1;
    }
    ls = (long)(c.lsa - c.ssa) / 16 * VAG_BLOCK;   /* the loop in the first pass */
    body = first * VAG_BLOCK - ls;
    if (c.lsa < c.ssa || c.lsa > end_at || body <= 0) {
        free(out);
        return 0;
    }
    again = malloc(sizeof(int16_t) * (size_t)body);
    third = malloc(sizeof(int16_t) * (size_t)body);
    if (again == NULL || third == NULL || play(&c, again, body / VAG_BLOCK) != body / VAG_BLOCK ||
        play(&c, third, body / VAG_BLOCK) != body / VAG_BLOCK || memcmp(again, third, sizeof(int16_t) * (size_t)body) != 0) {
        free(again);
        free(third);
        free(out);
        return 0;
    }
    for (k = body; k > 0 && again[k - 1] == out[ls + k - 1]; k--) {
    }
    k = (k + VAG_BLOCK - 1) / VAG_BLOCK * VAG_BLOCK;   /* (whole blocks: the loop starts on one) */
    for (i = 0; i < k; i++) {
        out[first * VAG_BLOCK + i] = again[i];
    }
    free(again);
    free(third);
    *pcm = out;
    *frames = first * VAG_BLOCK + k;
    *loop_start = ls + k;
    return 1;
}

/* ---- WAV ---- */

static void le32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
}
static uint32_t rd32(const uint8_t *p) { return p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24; }
static unsigned rd16(const uint8_t *p) { return (unsigned)(p[0] | p[1] << 8); }

uint8_t *vag_wav(const int16_t *pcm, long frames, int rate, long loop_start, size_t *size) {
    size_t data = (size_t)frames * 2, smpl = loop_start >= 0 ? 8 + 36 + 24 : 0, n = 12 + 8 + 16 + smpl + 8 + data;
    uint8_t *w = calloc(1, n), *p;
    long i;

    if (w == NULL) {
        return NULL;
    }
    memcpy(w, "RIFF", 4);
    le32(w + 4, (uint32_t)(n - 8));
    memcpy(w + 8, "WAVEfmt ", 8);
    le32(w + 16, 16);
    w[20] = 1;   /* PCM */
    w[22] = 1;   /* mono */
    le32(w + 24, (uint32_t)rate);
    le32(w + 28, (uint32_t)rate * 2);
    w[32] = 2;
    w[34] = 16;
    p = w + 36;
    if (smpl != 0) {   /* the loop: start and end (its last sample) */
        memcpy(p, "smpl", 4);
        le32(p + 4, 36 + 24);
        le32(p + 16, (uint32_t)(1000000000.0 / rate));   /* (the period, ns) */
        le32(p + 20, 60);                                /* (middle C) */
        le32(p + 36, 1);                                 /* one loop */
        le32(p + 52, (uint32_t)loop_start);
        le32(p + 56, (uint32_t)(frames - 1));
        p += 8 + 36 + 24;
    }
    memcpy(p, "data", 4);
    le32(p + 4, (uint32_t)data);
    for (i = 0; i < frames; i++) {
        p[8 + i * 2] = (uint8_t)pcm[i];
        p[9 + i * 2] = (uint8_t)((uint16_t)pcm[i] >> 8);
    }
    *size = n;
    return w;
}

int vag_wav_read(const uint8_t *d, size_t size, int16_t **pcm, long *frames, long *loop_start) {
    size_t at = 12;
    const uint8_t *data = NULL;
    size_t dlen = 0;
    int fmt_ok = 0;
    long i, loop_end = -1;

    if (size < 12 || memcmp(d, "RIFF", 4) != 0 || memcmp(d + 8, "WAVE", 4) != 0) {
        return 0;
    }
    *loop_start = -1;
    while (at + 8 <= size) {
        uint32_t len = rd32(d + at + 4);

        if (len > size - at - 8) {
            break;
        }
        if (memcmp(d + at, "fmt ", 4) == 0 && len >= 16) {
            fmt_ok = rd16(d + at + 8) == 1 && rd16(d + at + 10) == 1 && rd16(d + at + 22) == 16;
        } else if (memcmp(d + at, "smpl", 4) == 0 && len >= 36 + 24 && rd32(d + at + 8 + 28) > 0) {
            *loop_start = (long)rd32(d + at + 8 + 36 + 8);
            loop_end = (long)rd32(d + at + 8 + 36 + 12);
        } else if (memcmp(d + at, "data", 4) == 0) {
            data = d + at + 8;
            dlen = len;
        }
        at += 8 + len + (len & 1);
    }
    if (!fmt_ok || data == NULL) {
        return 0;
    }
    *frames = (long)(dlen / 2);
    if (*loop_start >= 0 && loop_end >= *loop_start && loop_end + 1 < *frames) {   /* (a loop ending early: played to there) */
        *frames = loop_end + 1;
    }
    if ((*pcm = malloc(sizeof(int16_t) * (size_t)(*frames + 1))) == NULL) {
        return 0;
    }
    for (i = 0; i < *frames; i++) {
        (*pcm)[i] = (int16_t)rd16(data + i * 2);
    }
    if (*loop_start >= *frames) {
        *loop_start = -1;
    }
    return 1;
}
