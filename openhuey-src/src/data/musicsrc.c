/* A music track's samples, ADX or FLAC: see musicsrc.h. */
#include "musicsrc.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "../third_party/dr_flac.h"

typedef struct Adx {
    int block, coef1, coef2;
    size_t start, pos;              /* the data's start, the next block */
    size_t loop_start, loop_end;    /* bytes (loop_end 0: the end of the data) */
    int hist[2][2];
} Adx;

typedef struct Flac {
    drflac *f;
    long pos, loop_start, loop_end, stop;   /* samples */
} Flac;

struct MusicSrc {
    uint8_t *data;
    size_t size;
    int ch, rate, loop;
    Adx adx;
    Flac flac;   /* (f NULL: an ADX) */
};

static unsigned be16(const uint8_t *b) { return (unsigned)(b[0] << 8 | b[1]); }
static unsigned be32(const uint8_t *b) { return (unsigned)b[0] << 24 | (unsigned)b[1] << 16 | (unsigned)b[2] << 8 | b[3]; }

/* ---- ADX ---- */

static int adx_open(MusicSrc *m) {
    const uint8_t *data = m->data;
    unsigned off, cutoff, ver;
    double x, y, z;
    Adx *a = &m->adx;

    if (m->size < 0x30 || be16(data) != 0x8000) {
        return 0;
    }
    off = be16(data + 2) + 4;
    a->start = a->pos = off;
    a->block = data[5];
    m->ch = data[7] >= 2 ? 2 : 1;
    m->rate = (int)be32(data + 8);
    cutoff = be16(data + 0x10);
    ver = data[0x12];
    x = sqrt(2.0) - cos(2.0 * M_PI * cutoff / (m->rate > 0 ? m->rate : 48000));
    y = sqrt(2.0) - 1.0;
    z = (x - sqrt((x + y) * (x - y))) / y;
    a->coef1 = (int)floor(z * 8192.0);
    a->coef2 = (int)floor(z * z * -4096.0);
    if ((ver == 3 || ver == 4) && off >= (ver == 3 ? 0x2Cu : 0x38u)) {
        const uint8_t *l = data + (ver == 3 ? 0x18 : 0x24);

        if (be32(l) != 0) {
            a->loop_start = be32(l + 8);
            a->loop_end = be32(l + 16);
            if (a->loop_start < off || a->loop_start >= m->size || a->loop_end > m->size || a->loop_end <= a->loop_start) {
                a->loop_start = a->loop_end = 0;
            }
        }
    }
    return a->block > 2 && m->rate > 0;
}

static int adx_next(MusicSrc *m, int16_t pcm[2][MUSICSRC_FRAME], int depth) {
    Adx *a = &m->adx;
    size_t end = a->loop_end != 0 ? a->loop_end : m->size, step = (size_t)a->block * m->ch;
    int c, i;

    if (a->pos + step > end) {
        if (!m->loop) {
            return 0;
        }
        a->pos = a->loop_start != 0 ? a->loop_start : a->start;
        if (a->pos + step > m->size) {
            return 0;
        }
    }
    for (c = 0; c < m->ch; c++) {
        const uint8_t *b = m->data + a->pos + (size_t)c * a->block;
        int scale = (int)be16(b), h1 = a->hist[c][0], h2 = a->hist[c][1];

        if (scale & 0x8000) {   /* the end marker */
            if (!m->loop || depth > 0) {
                return 0;
            }
            a->pos = a->loop_start != 0 ? a->loop_start : a->start;
            return adx_next(m, pcm, depth + 1);
        }
        for (i = 0; i < MUSICSRC_FRAME; i++) {
            int v = (b[2 + i / 2] >> (i & 1 ? 0 : 4)) & 0xF;

            if (v & 8) {
                v -= 16;
            }
            v = v * scale + ((a->coef1 * h1 + a->coef2 * h2) >> 12);
            v = v > 32767 ? 32767 : v < -32768 ? -32768 : v;
            pcm[c][i] = (int16_t)v;
            h2 = h1;
            h1 = v;
        }
        a->hist[c][0] = h1;
        a->hist[c][1] = h2;
    }
    a->pos += step;
    return MUSICSRC_FRAME;
}

/* `count` samples of the source into pcm (interleaved): how many there were */
static long pull(MusicSrc *m, int16_t *pcm, long count) {
    int16_t block[2][MUSICSRC_FRAME];
    long n = 0;
    int c, i, got;

    while (n < count && (got = adx_next(m, block, 0)) > 0) {
        for (i = 0; i < got && n < count; i++, n++) {
            for (c = 0; c < m->ch; c++) {
                pcm[n * m->ch + c] = block[c][i];
            }
        }
    }
    return n;
}

int musicsrc_adx_pcm(const uint8_t *data, size_t size, int16_t **pcm, long *frames, int *channels, int *rate,
                     long *loop_start, long *loop_end, long *stop) {
    MusicSrc m;
    int16_t *again = NULL, *third = NULL;
    long cap, n, ls, body, k, i;

    memset(&m, 0, sizeof(m));
    m.data = (uint8_t *)data;
    m.size = size;
    if (!adx_open(&m)) {
        return 0;
    }
    cap = (long)(size / ((size_t)m.adx.block * m.ch) + 1) * MUSICSRC_FRAME;
    if ((*pcm = malloc((size_t)cap * 2 * m.ch * sizeof(int16_t))) == NULL) {
        return 0;
    }
    n = pull(&m, *pcm, cap);   /* once through (m.loop 0) */
    ls = m.adx.loop_end != 0 ? (long)((m.adx.loop_start - m.adx.start) / ((size_t)m.adx.block * m.ch)) * MUSICSRC_FRAME : 0;
    ls = ls < n ? ls : 0;
    body = n - ls;
    /* the second pass and the third, played on as the player does (the prediction carried over) */
    memset(&m.adx.hist, 0, sizeof(m.adx.hist));
    m.adx.pos = m.adx.start;
    m.loop = 1;
    again = malloc((size_t)body * m.ch * sizeof(int16_t));
    third = malloc((size_t)body * m.ch * sizeof(int16_t));
    if (again == NULL || third == NULL || body <= 0 || pull(&m, *pcm + (size_t)n * m.ch, n) != n ||
        pull(&m, again, body) != body || pull(&m, third, body) != body || memcmp(again, third, (size_t)body * m.ch * 2) != 0) {
        free(again);
        free(third);
        free(*pcm);
        return 0;
    }
    for (k = body; k > 0 && memcmp(again + (k - 1) * m.ch, *pcm + (ls + k - 1) * m.ch, (size_t)m.ch * 2) == 0; k--) {
    }
    for (i = 0; i < k * m.ch; i++) {   /* (the second pass's start, after the first) */
        (*pcm)[n * m.ch + i] = again[i];
    }
    free(again);
    free(third);
    *frames = n + k;
    *channels = m.ch;
    *rate = m.rate;
    *loop_start = ls + k;
    *loop_end = n + k;
    *stop = n;
    return 1;
}

/* ---- FLAC ---- */

static void flac_tags(void *user, drflac_metadata *md) {   /* LOOPSTART=n, LOOPEND=n */
    Flac *fl = user;
    drflac_vorbis_comment_iterator it;
    drflac_uint32 len;
    const char *s;

    if (md->type != DRFLAC_METADATA_BLOCK_TYPE_VORBIS_COMMENT) {
        return;
    }
    drflac_init_vorbis_comment_iterator(&it, md->data.vorbis_comment.commentCount, md->data.vorbis_comment.pComments);
    while ((s = drflac_next_vorbis_comment(&it, &len)) != NULL) {
        char tag[64];

        snprintf(tag, sizeof(tag), "%.*s", (int)(len < 63 ? len : 63), s);
        if (strncasecmp(tag, "LOOPSTART=", 10) == 0) {
            fl->loop_start = atol(tag + 10);
        } else if (strncasecmp(tag, "LOOPEND=", 8) == 0) {
            fl->loop_end = atol(tag + 8);
        } else if (strncasecmp(tag, "STOPAT=", 7) == 0) {
            fl->stop = atol(tag + 7);
        }
    }
}

static int flac_open(MusicSrc *m) {
    Flac *fl = &m->flac;
    long total;

    if (m->size < 4 || memcmp(m->data, "fLaC", 4) != 0) {
        return 0;
    }
    fl->loop_end = fl->stop = -1;
    fl->f = drflac_open_memory_with_metadata(m->data, m->size, flac_tags, fl, NULL);
    if (fl->f == NULL || fl->f->channels < 1 || fl->f->channels > 2) {
        return 0;
    }
    total = (long)fl->f->totalPCMFrameCount;
    if (fl->loop_end < 0 || fl->loop_end > total) {
        fl->loop_end = total;
    }
    if (fl->loop_start < 0 || fl->loop_start >= fl->loop_end) {
        fl->loop_start = 0;
    }
    if (fl->stop < 0 || fl->stop > total) {
        fl->stop = fl->loop_end;
    }
    if (!m->loop) {   /* (not looped: it stops there) */
        fl->loop_end = fl->stop;
    }
    m->ch = fl->f->channels;
    m->rate = (int)fl->f->sampleRate;
    return m->rate > 0;
}

static int flac_next(MusicSrc *m, int16_t pcm[2][MUSICSRC_FRAME]) {
    Flac *fl = &m->flac;
    int16_t buf[MUSICSRC_FRAME * 2];
    long n;
    int i, c;

    if (fl->pos >= fl->loop_end) {
        if (!m->loop || !drflac_seek_to_pcm_frame(fl->f, (drflac_uint64)fl->loop_start)) {
            return 0;
        }
        fl->pos = fl->loop_start;
    }
    n = fl->loop_end - fl->pos < MUSICSRC_FRAME ? fl->loop_end - fl->pos : MUSICSRC_FRAME;
    n = (long)drflac_read_pcm_frames_s16(fl->f, (drflac_uint64)n, buf);
    if (n <= 0) {   /* (shorter than it said: its end here) */
        fl->loop_end = fl->pos;
        return fl->pos > fl->loop_start ? flac_next(m, pcm) : 0;
    }
    for (i = 0; i < n; i++) {
        for (c = 0; c < m->ch; c++) {
            pcm[c][i] = buf[i * m->ch + c];
        }
    }
    fl->pos += n;
    return (int)n;
}

/* ---- either ---- */

MusicSrc *musicsrc_open(uint8_t *data, size_t size, int loop) {
    MusicSrc *m = data != NULL ? calloc(1, sizeof(*m)) : NULL;

    if (m == NULL) {
        free(data);
        return NULL;
    }
    m->data = data;
    m->size = size;
    m->loop = loop;
    if (!flac_open(m) && (m->flac.f != NULL || !adx_open(m))) {
        musicsrc_free(m);
        return NULL;
    }
    return m;
}

void musicsrc_free(MusicSrc *m) {
    if (m != NULL) {
        drflac_close(m->flac.f);
        free(m->data);
        free(m);
    }
}

int musicsrc_rate(const MusicSrc *m) { return m->rate; }
int musicsrc_channels(const MusicSrc *m) { return m->ch; }

int musicsrc_next(MusicSrc *m, int16_t pcm[2][MUSICSRC_FRAME]) {
    return m->flac.f != NULL ? flac_next(m, pcm) : adx_next(m, pcm, 0);
}
