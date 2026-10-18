/* Writing FLAC: see flacenc.h (the format: RFC 9639). */
#include "flacenc.h"

#include <stdlib.h>
#include <string.h>

#define BLOCK 4096
#define MAX_ORDER 4
#define MAX_PART 8    /* partitions: up to 2^8 a subframe */
#define MAX_RICE 14   /* (15: the escape) */

/* ---- bits, most significant first ---- */

typedef struct Bits {
    uint8_t *b;
    size_t n, cap;   /* bytes done */
    uint64_t acc;    /* bits waiting */
    int nacc;
    int failed;
} Bits;

static void grow(Bits *w, size_t more) {
    if (w->n + more > w->cap) {
        size_t cap = w->cap ? w->cap * 2 : 1 << 20;
        uint8_t *b;

        while (cap < w->n + more) {
            cap *= 2;
        }
        if ((b = realloc(w->b, cap)) == NULL) {
            w->failed = 1;
            w->n = 0;
            return;
        }
        w->b = b;
        w->cap = cap;
    }
}

static void put(Bits *w, uint32_t v, int n) {   /* n <= 32 */
    if (n == 0) {
        return;
    }
    w->acc = (w->acc << n) | (v & (n == 32 ? 0xFFFFFFFFu : (1u << n) - 1));
    w->nacc += n;
    while (w->nacc >= 8) {
        grow(w, 1);
        if (w->failed) {
            return;
        }
        w->nacc -= 8;
        w->b[w->n++] = (uint8_t)(w->acc >> w->nacc);
    }
}

static void put_unary(Bits *w, uint32_t q) {   /* q zeros, then a one */
    while (q >= 32) {
        put(w, 0, 32);
        q -= 32;
    }
    put(w, 1, (int)q + 1);
}

static void align(Bits *w) {
    if (w->nacc > 0) {
        put(w, 0, 8 - w->nacc);
    }
}

static void put_le32(Bits *w, uint32_t v) {
    put(w, v & 0xFF, 8);
    put(w, (v >> 8) & 0xFF, 8);
    put(w, (v >> 16) & 0xFF, 8);
    put(w, v >> 24, 8);
}

/* ---- checks ---- */

static uint8_t crc8(const uint8_t *p, size_t n) {
    uint8_t c = 0;
    int k;

    while (n-- > 0) {
        c ^= *p++;
        for (k = 0; k < 8; k++) {
            c = (uint8_t)(c & 0x80 ? (c << 1) ^ 0x07 : c << 1);
        }
    }
    return c;
}

static uint16_t crc16(const uint8_t *p, size_t n) {
    uint16_t c = 0;
    int k;

    while (n-- > 0) {
        c ^= (uint16_t)(*p++ << 8);
        for (k = 0; k < 8; k++) {
            c = (uint16_t)(c & 0x8000 ? (c << 1) ^ 0x8005 : c << 1);
        }
    }
    return c;
}

/* ---- a subframe ---- */

typedef struct Plan {   /* how a channel's block is best written */
    int constant;
    int order, part;    /* the predictor, partitions 2^part */
    int rice[1 << MAX_PART];
    uint64_t bits;
} Plan;

static uint32_t zigzag(int32_t r) { return r >= 0 ? (uint32_t)r << 1 : ((uint32_t)(-(r + 1)) << 1) | 1; }

static int32_t residual(const int32_t *x, int i, int order) {
    switch (order) {
    case 0: return x[i];
    case 1: return x[i] - x[i - 1];
    case 2: return x[i] - 2 * x[i - 1] + x[i - 2];
    case 3: return x[i] - 3 * x[i - 1] + 3 * x[i - 2] - x[i - 3];
    default: return x[i] - 4 * x[i - 1] + 6 * x[i - 2] - 4 * x[i - 3] + x[i - 4];
    }
}

/* the best Rice parameter for n values summing to sum (zigzagged), and its cost in bits */
static uint64_t rice_cost(uint64_t sum, uint32_t n, int *k_out) {
    uint64_t best = UINT64_MAX;
    int k;

    for (k = 0; k <= MAX_RICE; k++) {
        uint64_t c = (uint64_t)n * (uint32_t)(k + 1) + (sum >> k);   /* (the quotients' sum, near enough to choose) */

        if (c < best) {
            best = c;
            *k_out = k;
        }
    }
    return best;
}

static void plan(const int32_t *x, int n, int bps, Plan *best) {
    static uint64_t prefix[BLOCK + 1];
    Plan p;
    int order, part, i;

    memset(best, 0, sizeof(*best));
    for (i = 1; i < n && x[i] == x[0]; i++) {
    }
    if (i == n) {
        best->constant = 1;
        best->bits = 8 + (uint64_t)bps;
        return;
    }
    best->bits = UINT64_MAX;
    for (order = 0; order <= MAX_ORDER && order < n; order++) {
        prefix[0] = 0;
        for (i = 0; i < n; i++) {   /* (the warm-up samples count as 0) */
            prefix[i + 1] = prefix[i] + (i < order ? 0 : zigzag(residual(x, i, order)));
        }
        for (part = 0; part <= MAX_PART; part++) {
            int parts = 1 << part, len = n >> part;
            uint64_t bits = 8 + (uint64_t)order * bps + 6;

            if ((n & (parts - 1)) != 0 || len <= order) {
                break;
            }
            memset(&p, 0, sizeof(p));
            for (i = 0; i < parts; i++) {
                int from = i * len, count = len - (i == 0 ? order : 0);

                bits += 4 + rice_cost(prefix[from + len] - prefix[from], (uint32_t)count, &p.rice[i]);
            }
            if (bits < best->bits) {
                p.order = order;
                p.part = part;
                p.bits = bits;
                *best = p;
            }
        }
    }
    if (best->bits > 8 + (uint64_t)n * bps) {   /* (noise: verbatim would be smaller) */
        best->order = -1;
        best->bits = 8 + (uint64_t)n * bps;
    }
}

static void write_subframe(Bits *w, const int32_t *x, int n, int bps, const Plan *p) {
    int i, j;

    if (p->constant) {
        put(w, 0x00, 8);
        put(w, (uint32_t)x[0], bps);
        return;
    }
    if (p->order < 0) {
        put(w, 0x02, 8);   /* (verbatim: 000001, no wasted bits) */
        for (i = 0; i < n; i++) {
            put(w, (uint32_t)x[i], bps);
        }
        return;
    }
    put(w, (uint32_t)(0x08 | p->order) << 1, 8);   /* fixed: 001ooo */
    for (i = 0; i < p->order; i++) {
        put(w, (uint32_t)x[i], bps);
    }
    put(w, 0, 2);   /* Rice, 4-bit parameters */
    put(w, (uint32_t)p->part, 4);
    for (j = 0; j < 1 << p->part; j++) {
        int len = n >> p->part, from = j * len + (j == 0 ? p->order : 0), to = (j + 1) * len, k = p->rice[j];

        put(w, (uint32_t)k, 4);
        for (i = from; i < to; i++) {
            uint32_t u = zigzag(residual(x, i, p->order));

            put_unary(w, u >> k);
            put(w, u, k);
        }
    }
}

/* ---- a frame ---- */

static void write_frame(Bits *w, const int16_t *pcm, int n, int ch, uint32_t number) {
    static int32_t chans[4][BLOCK];   /* left, right, mid, side */
    Plan plans[4];
    size_t start = w->n;
    int i, assign, a, b;
    uint64_t cost[4];

    for (i = 0; i < n; i++) {
        int32_t l = pcm[i * ch], r = ch > 1 ? pcm[i * ch + 1] : 0;

        chans[0][i] = l;
        chans[1][i] = r;
        chans[2][i] = (l + r) >> 1;
        chans[3][i] = l - r;
    }
    plan(chans[0], n, 16, &plans[0]);
    if (ch > 1) {
        plan(chans[1], n, 16, &plans[1]);
        plan(chans[2], n, 16, &plans[2]);
        plan(chans[3], n, 17, &plans[3]);
    }
    /* the channel assignment: 0001 left/right, 1000 left/side, 1001 side/right, 1010 mid/side */
    assign = ch - 1;
    a = 0;
    b = 1;
    if (ch > 1) {
        cost[0] = plans[0].bits + plans[1].bits;
        cost[1] = plans[0].bits + plans[3].bits;
        cost[2] = plans[3].bits + plans[1].bits;
        cost[3] = plans[2].bits + plans[3].bits;
        for (i = 1, a = 0; i < 4; i++) {
            a = cost[i] < cost[a] ? i : a;
        }
        assign = a == 0 ? 1 : 7 + a;
        b = a == 0 ? 1 : a == 1 ? 3 : a == 2 ? 1 : 3;
        a = a == 0 ? 0 : a == 1 ? 0 : a == 2 ? 3 : 2;
    }
    put(w, 0xFFF8, 16);                  /* sync, fixed block sizes */
    put(w, n == BLOCK ? 12 : 7, 4);      /* 4096, or (n - 1) in 16 bits below */
    put(w, 0, 4);                        /* the rate: the stream's */
    put(w, (uint32_t)assign, 4);
    put(w, 4, 3);                        /* 16 bits a sample */
    put(w, 0, 1);
    if (number < 0x80) {                 /* the frame's number, coded as UTF-8 is */
        put(w, number, 8);
    } else if (number < 0x800) {
        put(w, 0xC0 | number >> 6, 8);
        put(w, 0x80 | (number & 0x3F), 8);
    } else if (number < 0x10000) {
        put(w, 0xE0 | number >> 12, 8);
        put(w, 0x80 | ((number >> 6) & 0x3F), 8);
        put(w, 0x80 | (number & 0x3F), 8);
    } else {
        put(w, 0xF0 | number >> 18, 8);
        put(w, 0x80 | ((number >> 12) & 0x3F), 8);
        put(w, 0x80 | ((number >> 6) & 0x3F), 8);
        put(w, 0x80 | (number & 0x3F), 8);
    }
    if (n != BLOCK) {
        put(w, (uint32_t)(n - 1), 16);
    }
    if (w->failed) {
        return;
    }
    put(w, crc8(w->b + start, w->n - start), 8);
    write_subframe(w, chans[a], n, a == 3 ? 17 : 16, &plans[a]);
    if (ch > 1) {
        write_subframe(w, chans[b], n, b == 3 ? 17 : 16, &plans[b]);
    }
    align(w);
    if (!w->failed) {
        uint16_t c = crc16(w->b + start, w->n - start);

        put(w, c, 16);
    }
}

#define SEEK_EVERY 12   /* frames between seek points (about a second): a loop's jump is quick */

uint8_t *flacenc(const int16_t *pcm, long frames, int channels, int rate, const char *const *tags, int ntags,
                 size_t *size) {
    Bits w = {0}, fr = {0};
    size_t vlen = 0, *seek;
    long at, nseek = (frames + (long)BLOCK * SEEK_EVERY - 1) / ((long)BLOCK * SEEK_EVERY), k;
    uint32_t number = 0;
    int i, j;

    /* the frames first (where every SEEK_EVERY-th starts: the seek table) */
    if ((seek = malloc(sizeof(size_t) * (size_t)(nseek + 1))) == NULL) {
        return NULL;
    }
    for (at = 0, k = 0; at < frames && !fr.failed; at += BLOCK) {
        if (number % SEEK_EVERY == 0) {
            seek[k++] = fr.n;
        }
        write_frame(&fr, pcm + at * channels, (int)(frames - at < BLOCK ? frames - at : BLOCK), channels, number++);
    }
    nseek = k;
    put(&w, 0x664C6143, 32);   /* "fLaC" */
    /* STREAMINFO */
    put(&w, 0, 1);
    put(&w, 0, 7);
    put(&w, 34, 24);
    put(&w, BLOCK, 16);
    put(&w, BLOCK, 16);
    put(&w, 0, 24);                       /* (frame sizes: not known) */
    put(&w, 0, 24);
    put(&w, (uint32_t)rate, 20);
    put(&w, (uint32_t)channels - 1, 3);
    put(&w, 15, 5);                       /* 16 bits */
    put(&w, (uint32_t)((uint64_t)frames >> 32) & 0xF, 4);
    put(&w, (uint32_t)frames, 32);
    for (i = 0; i < 4; i++) {
        put(&w, 0, 32);                   /* (no MD5) */
    }
    /* SEEKTABLE: each point's first sample, its frame's offset from the first frame, its samples */
    put(&w, 0, 1);
    put(&w, 3, 7);
    put(&w, (uint32_t)(18 * nseek), 24);
    for (k = 0; k < nseek; k++) {
        uint64_t sample = (uint64_t)k * SEEK_EVERY * BLOCK;

        put(&w, (uint32_t)(sample >> 32), 32);
        put(&w, (uint32_t)sample, 32);
        put(&w, (uint32_t)((uint64_t)seek[k] >> 32), 32);
        put(&w, (uint32_t)seek[k], 32);
        put(&w, (uint32_t)(frames - (long)sample < BLOCK ? frames - (long)sample : BLOCK), 16);
    }
    free(seek);
    /* VORBIS_COMMENT, the last block */
    for (i = 0; i < ntags; i++) {
        vlen += 4 + strlen(tags[i]);
    }
    put(&w, 1, 1);
    put(&w, 4, 7);
    put(&w, (uint32_t)(4 + 3 + 4 + vlen), 24);
    put_le32(&w, 3);
    put(&w, 'h', 8);
    put(&w, 'g', 8);
    put(&w, 'a', 8);
    put_le32(&w, (uint32_t)ntags);
    for (i = 0; i < ntags; i++) {
        put_le32(&w, (uint32_t)strlen(tags[i]));
        for (j = 0; tags[i][j] != 0; j++) {
            put(&w, (uint8_t)tags[i][j], 8);
        }
    }
    grow(&w, fr.n);
    if (w.failed || fr.failed) {
        free(w.b);
        free(fr.b);
        return NULL;
    }
    memcpy(w.b + w.n, fr.b, fr.n);
    *size = w.n + fr.n;
    free(fr.b);
    return w.b;
}
