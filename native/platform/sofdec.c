/* CRI Sofdec (the mwPly* movie player) on PC. The game's movies (.SFD) are MPEG program
 * streams: MPEG-2 video (stream 0xE0) and ADX sound (stream 0xC0). Here they are demuxed by
 * hand, the sound decoded as ADX into the mixer (sfd_render), and the video by FFmpeg's
 * libavcodec, loaded at run time (dlopen), its frames turned into the PS2's RGBA (alpha 0x80).
 * Without libavcodec no player is made and the game skips its movies.
 *
 *   HG_NOMOVIES=1   skip every movie        HG_SFDLOG=1   log each frame shown
 *
 * The player the game holds is a C++-style object: vtable +0x14 destroy, +0x18 start a file,
 * +0x20 status, +0x28 pause, +0x2C volume. The frame object it reads (MwsfdFrmObj) is the
 * movie's +0x18: +0x0 the image, +0x8 width, +0xC height, +0x20 the frame number. */
#include <SDL3/SDL.h>
#include <dlfcn.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snd.h"

/* mwPlyGetStat */
enum { STAT_STOP = 0, STAT_PREP, STAT_PLAYING, STAT_PLAYEND, STAT_ERROR };

/* ---- libavcodec, by hand: only what the decoder needs, so no headers are required ---- */

#define AV_CODEC_ID_MPEG2VIDEO 2
#define AV_PIX_FMT_YUV420P 0
#define AV_NOPTS_VALUE ((int64_t)UINT64_C(0x8000000000000000))
#define AV_PAD 64   /* AV_INPUT_BUFFER_PADDING_SIZE */

typedef struct AvFramePrefix {   /* the head of AVFrame, unchanged since FFmpeg 1.x */
    uint8_t *data[8];
    int linesize[8];
    uint8_t **extended_data;
    int width, height;
    int nb_samples;
    int format;
} AvFramePrefix;

static struct {
    int tried, ok;
    const void *(*find_decoder)(int id);
    void *(*alloc_context3)(const void *codec);
    int (*open2)(void *ctx, const void *codec, void **opts);
    void (*free_context)(void **ctx);
    int (*send_packet)(void *ctx, const void *pkt);
    int (*receive_frame)(void *ctx, void *frame);
    void *(*parser_init)(int id);
    int (*parser_parse2)(void *s, void *ctx, uint8_t **out, int *outSize, const uint8_t *buf, int size,
                         int64_t pts, int64_t dts, int64_t pos);
    void (*parser_close)(void *s);
    void *(*packet_alloc)(void);
    void (*packet_free)(void **pkt);
    int (*packet_from_data)(void *pkt, uint8_t *data, int size);
    void (*packet_unref)(void *pkt);
    void *(*frame_alloc)(void);
    void (*frame_free)(void **frame);
    void *(*av_malloc)(size_t size);
} av;

static int av_load(void) {
    static const char *const names[] = {"libavcodec.so",    "libavcodec.so.63", "libavcodec.so.62",
                                        "libavcodec.so.61", "libavcodec.so.60", "libavcodec.so.59",
                                        "libavcodec.so.58"};
    void *h = NULL;
    size_t i;

    if (av.tried) {
        return av.ok;
    }
    av.tried = 1;
    for (i = 0; i < sizeof(names) / sizeof(names[0]) && h == NULL; i++) {
        h = dlopen(names[i], RTLD_NOW | RTLD_GLOBAL);
    }
    if (h == NULL) {
        fprintf(stderr, "sofdec: no libavcodec, movies are skipped\n");
        return 0;
    }
#define SYM(field, name)                                                       \
    if ((*(void **)&av.field = dlsym(h, name)) == NULL) {                     \
        fprintf(stderr, "sofdec: libavcodec has no %s, movies are skipped\n", name); \
        return 0;                                                              \
    }
    SYM(find_decoder, "avcodec_find_decoder");
    SYM(alloc_context3, "avcodec_alloc_context3");
    SYM(open2, "avcodec_open2");
    SYM(free_context, "avcodec_free_context");
    SYM(send_packet, "avcodec_send_packet");
    SYM(receive_frame, "avcodec_receive_frame");
    SYM(parser_init, "av_parser_init");
    SYM(parser_parse2, "av_parser_parse2");
    SYM(parser_close, "av_parser_close");
    SYM(packet_alloc, "av_packet_alloc");
    SYM(packet_free, "av_packet_free");
    SYM(packet_from_data, "av_packet_from_data");
    SYM(packet_unref, "av_packet_unref");
    SYM(frame_alloc, "av_frame_alloc");
    SYM(frame_free, "av_frame_free");
    SYM(av_malloc, "av_malloc");
#undef SYM
    av.ok = 1;
    return 1;
}

/* ---- the player ---- */

#define AUD_RING (SND_RATE * 8)   /* stereo frames of sound decoded ahead */
#define ADX_BLOCK 18
#define ADX_FRAME 32

typedef struct Ply {
    void **vtbl;
    int stat;
    int paused;
    int vol;          /* 0.1 dB */
    FILE *f;
    int eof;          /* the file is read to its end */
    int drained;      /* the decoder gave its last frame */
    /* reading */
    uint8_t buf[0x10000];
    int bufPos, bufLen;
    /* video */
    void *ctx, *parser, *pkt, *frm;
    double fps;
    int w, h;
    uint8_t *rgb;     /* the frame shown, w x h RGBA */
    int shownNo;      /* its number, -1 none */
    int ready;        /* a decoded frame waits in frm */
    int readyNo;      /* its number */
    int decoded;      /* frames decoded so far */
    int fresh;        /* a frame not yet handed to the game */
    /* time */
    double clock;     /* seconds played */
    uint64_t lastTick;
    /* sound: the ADX stream as it arrives */
    uint8_t *adx;
    int adxLen, adxCap, adxPos;
    int adxCh, adxRate, adxHead, coef1, coef2;
    int hist[2][2];
    float ring[AUD_RING * 2];
    unsigned rdPos, wrPos;   /* frames, wrapping in the ring */
    double frac;
    float last[2], cur[2];
} Ply;

static Ply *sPlaying;   /* the one movie whose sound is mixed */

/* the current CRI folder (func_001E7430, adx.c) */
extern const void *adx_current_folder(void);

static int rd_fill(Ply *p) {
    if (p->bufPos < p->bufLen) {
        return 1;
    }
    p->bufLen = (int)fread(p->buf, 1, sizeof(p->buf), p->f);
    p->bufPos = 0;
    return p->bufLen > 0;
}

static int rd_byte(Ply *p) {
    if (!rd_fill(p)) {
        return -1;
    }
    return p->buf[p->bufPos++];
}

static int rd_bytes(Ply *p, uint8_t *dst, int n) {
    int got = 0;

    while (got < n && rd_fill(p)) {
        int k = p->bufLen - p->bufPos;

        if (k > n - got) {
            k = n - got;
        }
        if (dst != NULL) {
            memcpy(dst + got, p->buf + p->bufPos, (size_t)k);
        }
        p->bufPos += k;
        got += k;
    }
    return got;
}

/* ---- sound ---- */

static unsigned be16(const uint8_t *b) { return (unsigned)(b[0] << 8 | b[1]); }
static unsigned be32(const uint8_t *b) { return (unsigned)b[0] << 24 | (unsigned)b[1] << 16 | (unsigned)b[2] << 8 | b[3]; }

/* the ADX header, once enough of the stream is in */
static void adx_head(Ply *p) {
    const uint8_t *h = p->adx;
    unsigned off, cutoff;
    double x, y, z;

    if (p->adxLen < 0x14) {
        return;
    }
    if (be16(h) != 0x8000 || h[4] != 3 || h[5] != ADX_BLOCK || h[6] != 4 || h[7] < 1 || h[7] > 2) {
        fprintf(stderr, "sofdec: unsupported sound, the movie plays silent\n");
        p->adxHead = -1;
        return;
    }
    off = be16(h + 2) + 4;
    if ((unsigned)p->adxLen < off) {
        return;
    }
    p->adxCh = h[7];
    p->adxRate = (int)be32(h + 8);
    cutoff = be16(h + 0x10);
    x = sqrt(2.0) - cos(2.0 * M_PI * cutoff / p->adxRate);
    y = sqrt(2.0) - 1.0;
    z = (x - sqrt((x + y) * (x - y))) / y;
    p->coef1 = (int)floor(z * 8192.0);
    p->coef2 = (int)floor(z * z * -4096.0);
    p->adxPos = (int)off;
    p->adxHead = 1;
}

/* decode the whole frames that have arrived into the ring (game thread, under the sound lock) */
static void adx_pump(Ply *p) {
    int n;

    if (p->adxHead == 0) {
        adx_head(p);
    }
    if (p->adxHead != 1) {
        return;
    }
    n = ADX_BLOCK * p->adxCh;
    snd_lock();
    while (p->adxLen - p->adxPos >= n && p->wrPos - p->rdPos < AUD_RING) {
        short pcm[2][ADX_FRAME];
        int c, i;

        for (c = 0; c < p->adxCh; c++) {
            const uint8_t *b = p->adx + p->adxPos + c * ADX_BLOCK;
            int scale = (int)be16(b);
            int h1 = p->hist[c][0], h2 = p->hist[c][1];

            if (scale & 0x8000) {   /* the end marker */
                p->adxPos = p->adxLen;
                snd_unlock();
                return;
            }
            for (i = 0; i < ADX_FRAME; i++) {
                int v = (b[2 + i / 2] >> (i & 1 ? 0 : 4)) & 0xF;

                if (v & 8) {
                    v -= 16;
                }
                v = v * scale + ((p->coef1 * h1 + p->coef2 * h2) >> 12);
                v = v > 32767 ? 32767 : v < -32768 ? -32768 : v;
                pcm[c][i] = (short)v;
                h2 = h1;
                h1 = v;
            }
            p->hist[c][0] = h1;
            p->hist[c][1] = h2;
        }
        for (i = 0; i < ADX_FRAME && p->wrPos - p->rdPos < AUD_RING; i++) {
            float *o = &p->ring[(p->wrPos % AUD_RING) * 2];

            o[0] = pcm[0][i] / 32768.0f;
            o[1] = pcm[p->adxCh > 1][i] / 32768.0f;
            p->wrPos++;
        }
        p->adxPos += n;
    }
    snd_unlock();
    if (p->adxPos > 0x40000) {   /* drop what's decoded */
        memmove(p->adx, p->adx + p->adxPos, (size_t)(p->adxLen - p->adxPos));
        p->adxLen -= p->adxPos;
        p->adxPos = 0;
    }
}

static void adx_append(Ply *p, const uint8_t *b, int n) {
    if (p->adxHead < 0) {
        return;
    }
    if (p->adxLen + n > p->adxCap) {
        p->adxCap = (p->adxLen + n) * 2;
        p->adx = realloc(p->adx, (size_t)p->adxCap);
    }
    memcpy(p->adx + p->adxLen, b, (size_t)n);
    p->adxLen += n;
    adx_pump(p);
}

/* the mixer (snd_mix.c, the lock held) */
void sfd_render(float *out, int frames) {
    Ply *p = sPlaying;
    double step;
    float g;
    int i;

    if (p == NULL || p->paused || p->stat != STAT_PLAYING || p->adxHead != 1) {
        return;
    }
    g = snd_db10(p->vol);
    step = (double)p->adxRate / SND_RATE;
    for (i = 0; i < frames; i++) {
        p->frac += step;
        while (p->frac >= 1.0) {
            const float *s;

            if (p->rdPos == p->wrPos) {   /* not decoded yet: hold */
                p->frac = 1.0;
                break;
            }
            s = &p->ring[(p->rdPos % AUD_RING) * 2];
            p->last[0] = p->cur[0];
            p->last[1] = p->cur[1];
            p->cur[0] = s[0];
            p->cur[1] = s[1];
            p->rdPos++;
            p->frac -= 1.0;
        }
        out[2 * i] += (p->last[0] + (p->cur[0] - p->last[0]) * (float)fmin(p->frac, 1.0)) * g;
        out[2 * i + 1] += (p->last[1] + (p->cur[1] - p->last[1]) * (float)fmin(p->frac, 1.0)) * g;
    }
}

/* ---- video ---- */

static const double kFrameRates[16] = {0, 24000.0 / 1001, 24, 25, 30000.0 / 1001, 30, 50, 60000.0 / 1001, 60};

static void video_feed(Ply *p, const uint8_t *b, int n) {
    while (n > 0 || (n == 0 && b == NULL)) {
        uint8_t *out = NULL;
        int outSize = 0;
        int used = av.parser_parse2(p->parser, p->ctx, &out, &outSize, b, n, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);

        if (used < 0) {
            return;
        }
        if (b != NULL) {
            b += used;
            n -= used;
        }
        if (outSize > 0) {
            uint8_t *data = av.av_malloc((size_t)outSize + AV_PAD);

            if (p->fps == 0.0) {   /* the sequence header's frame rate */
                int i;

                for (i = 0; i + 7 < outSize; i++) {
                    if (out[i] == 0 && out[i + 1] == 0 && out[i + 2] == 1 && out[i + 3] == 0xB3) {
                        p->fps = kFrameRates[out[i + 7] & 0xF];
                        break;
                    }
                }
            }
            if (data != NULL) {
                memcpy(data, out, (size_t)outSize);
                memset(data + outSize, 0, AV_PAD);
                if (av.packet_from_data(p->pkt, data, outSize) == 0) {
                    av.send_packet(p->ctx, p->pkt);
                    av.packet_unref(p->pkt);
                }
            }
        }
        if (b == NULL) {
            return;
        }
    }
}

/* one PES packet's payload, or the stream's end; 0 at the end of the file */
static int demux_one(Ply *p) {
    for (;;) {
        int c = rd_byte(p), id, len, hdr = 0;
        uint8_t pes[0x10000];

        if (c < 0) {
            return 0;
        }
        if (c != 0) {
            continue;
        }
        if ((c = rd_byte(p)) != 0) {
            continue;
        }
        while ((c = rd_byte(p)) == 0) {
        }
        if (c != 1) {
            continue;
        }
        if ((id = rd_byte(p)) < 0) {
            return 0;
        }
        if (id == 0xB9) {   /* program end */
            return 0;
        }
        if (id == 0xBA) {   /* pack header */
            int b = rd_byte(p);

            if ((b & 0xC0) == 0x40) {   /* MPEG-2 */
                rd_bytes(p, NULL, 8);
                rd_bytes(p, NULL, rd_byte(p) & 7);
            } else {
                rd_bytes(p, NULL, 7);
            }
            continue;
        }
        if (id < 0xBB) {
            continue;
        }
        len = rd_byte(p) << 8;
        len |= rd_byte(p);
        if (len <= 0 || rd_bytes(p, pes, len) != len) {
            return 0;
        }
        if (id != 0xC0 && id != 0xE0) {
            continue;
        }
        /* the PES header */
        if ((pes[0] & 0xC0) == 0x80) {   /* MPEG-2 */
            hdr = 3 + pes[2];
        } else {
            while (hdr < len && pes[hdr] == 0xFF) {
                hdr++;
            }
            if (hdr < len && (pes[hdr] & 0xC0) == 0x40) {
                hdr += 2;
            }
            if (hdr < len && (pes[hdr] & 0xF0) == 0x20) {
                hdr += 5;
            } else if (hdr < len && (pes[hdr] & 0xF0) == 0x30) {
                hdr += 10;
            } else {
                hdr++;
            }
        }
        if (hdr >= len) {
            continue;
        }
        if (id == 0xE0) {
            video_feed(p, pes + hdr, len - hdr);
        } else {
            adx_append(p, pes + hdr, len - hdr);
        }
        return 1;
    }
}

/* the next decoded frame into frm (ready); 0 when there are no more */
static int video_next(Ply *p) {
    while (!p->ready) {
        int r = av.receive_frame(p->ctx, p->frm);

        if (r == 0) {
            const AvFramePrefix *f = p->frm;

            if (f->format != AV_PIX_FMT_YUV420P) {
                continue;
            }
            p->ready = 1;
            p->readyNo = p->decoded++;
            return 1;
        }
        if (p->drained) {
            return 0;
        }
        if (p->eof) {
            video_feed(p, NULL, 0);
            av.send_packet(p->ctx, NULL);   /* flush */
            p->drained = 1;
            continue;
        }
        if (!demux_one(p)) {
            p->eof = 1;
        }
    }
    return 1;
}

/* the ready frame into rgb (BT.601, TV range; alpha 0x80) */
static void video_show(Ply *p) {
    const AvFramePrefix *f = p->frm;
    int x, y;

    if (f->width != p->w || f->height != p->h || p->rgb == NULL) {
        p->w = f->width;
        p->h = f->height;
        free(p->rgb);
        p->rgb = malloc((size_t)p->w * p->h * 4);
    }
    for (y = 0; y < p->h; y++) {
        const uint8_t *py = f->data[0] + y * f->linesize[0];
        const uint8_t *pu = f->data[1] + (y / 2) * f->linesize[1];
        const uint8_t *pv = f->data[2] + (y / 2) * f->linesize[2];
        uint8_t *o = p->rgb + (size_t)y * p->w * 4;

        for (x = 0; x < p->w; x++) {
            int c = (py[x] - 16) * 298, d = pu[x / 2] - 128, e = pv[x / 2] - 128;
            int r = (c + 409 * e + 128) >> 8, g = (c - 100 * d - 208 * e + 128) >> 8, b = (c + 516 * d + 128) >> 8;

            o[0] = (uint8_t)(r < 0 ? 0 : r > 255 ? 255 : r);
            o[1] = (uint8_t)(g < 0 ? 0 : g > 255 ? 255 : g);
            o[2] = (uint8_t)(b < 0 ? 0 : b > 255 ? 255 : b);
            o[3] = 0x80;
            o += 4;
        }
    }
    p->shownNo = p->readyNo;
    p->ready = 0;
    p->fresh = 1;
    if (getenv("HG_SFDLOG") != NULL) {
        fprintf(stderr, "sofdec: frame %d (%d x %d) at %.2f s\n", p->shownNo, p->w, p->h, p->clock);
    }
}

static void ply_close(Ply *p) {
    snd_lock();
    if (sPlaying == p) {
        sPlaying = NULL;
    }
    snd_unlock();
    if (p->f != NULL) {
        fclose(p->f);
        p->f = NULL;
    }
    if (p->parser != NULL) {
        av.parser_close(p->parser);
        p->parser = NULL;
    }
    if (p->ctx != NULL) {
        av.free_context(&p->ctx);
    }
    if (p->pkt != NULL) {
        av.packet_free(&p->pkt);
    }
    if (p->frm != NULL) {
        av.frame_free(&p->frm);
    }
    free(p->adx);
    p->adx = NULL;
    p->adxLen = p->adxCap = p->adxPos = 0;
    free(p->rgb);
    p->rgb = NULL;
}

/* advance the clock and the frames to it */
static void ply_update(Ply *p) {
    uint64_t now = SDL_GetTicksNS();

    if (p->stat != STAT_PLAYING) {
        return;
    }
    if (!p->paused && p->shownNo >= 0) {
        p->clock += (double)(now - p->lastTick) / 1e9;
    }
    p->lastTick = now;
    for (;;) {
        int due = p->shownNo < 0 ? 0 : (int)(p->clock * (p->fps > 0.0 ? p->fps : 30.0));

        if (p->shownNo >= 0 && p->shownNo >= due) {
            break;
        }
        if (!video_next(p)) {
            /* the end: once the sound is out too */
            if (p->adxHead != 1 || p->rdPos == p->wrPos) {
                p->stat = STAT_PLAYEND;
            }
            break;
        }
        if (p->readyNo >= due || p->shownNo < 0) {
            video_show(p);
            if (p->shownNo >= due) {
                break;
            }
        } else {
            p->ready = 0;   /* late: skipped */
        }
    }
    adx_pump(p);
}

/* +0x14 destroy */
static void ply_destroy(Ply *p) {
    ply_close(p);
    free(p);
}

/* +0x18 start file `name` in the current folder */
static void ply_start(Ply *p, char *name) {
    const char *folder = crifs_folder(adx_current_folder());

    ply_close(p);
    memset(&p->bufPos, 0, sizeof(*p) - offsetof(Ply, bufPos));
    p->eof = p->drained = 0;
    p->paused = 0;
    p->shownNo = -1;
    p->f = crifs_open(folder, name);
    if (p->f == NULL) {
        fprintf(stderr, "sofdec: can't open %s/%s\n", folder, name);
        p->stat = STAT_PLAYEND;   /* (not an error: the game would retry for ever) */
        return;
    }
    p->ctx = av.alloc_context3(av.find_decoder(AV_CODEC_ID_MPEG2VIDEO));
    p->parser = av.parser_init(AV_CODEC_ID_MPEG2VIDEO);
    p->pkt = av.packet_alloc();
    p->frm = av.frame_alloc();
    if (p->ctx == NULL || p->parser == NULL || p->pkt == NULL || p->frm == NULL
        || av.open2(p->ctx, av.find_decoder(AV_CODEC_ID_MPEG2VIDEO), NULL) < 0) {
        fprintf(stderr, "sofdec: can't set up the decoder for %s\n", name);
        ply_close(p);
        p->stat = STAT_PLAYEND;
        return;
    }
    fprintf(stderr, "sofdec: play %s/%s\n", folder, name);
    p->lastTick = SDL_GetTicksNS();
    p->stat = STAT_PLAYING;
    snd_lock();
    sPlaying = p;
    snd_unlock();
}

/* +0x20 status */
static int ply_stat(Ply *p) {
    ply_update(p);
    return p->stat;
}

/* +0x28 pause */
static void ply_pause(Ply *p, int on) {
    ply_update(p);
    p->paused = on != 0;
}

/* +0x2C volume (0.1 dB) */
static void ply_volume(Ply *p, int db10) {
    p->vol = db10;
}

static void ply_nop(Ply *p) { (void)p; }

static void *sVtbl[12] = {
    NULL, NULL, (void *)ply_nop, (void *)ply_nop, (void *)ply_nop, (void *)ply_destroy,
    (void *)ply_start, (void *)ply_nop, (void *)ply_stat, (void *)ply_nop, (void *)ply_pause, (void *)ply_volume,
};

/* ---- the mwPly API the game calls ---- */

/* mwPlyInitSfdFx */
void mwPlyInitSfdFx(void *prm) { (void)prm; }

/* mwPlyFinishSfdFx (PS2 0x0023AE40) */
void func_0023AE40(void) {}

/* mwPlyCalcWorkCprmSfd: work buffer size for the creation parameters (unused here) */
int mwPlyCalcWorkCprmSfd(void *cprm) { (void)cprm; return 0x40; }

/* mwPlyCreateSofdec (PS2 0x00238BF0): NULL without libavcodec (the game then skips the movie) */
void *func_00238BF0(void *cprm) {
    Ply *p;

    (void)cprm;
    if (getenv("HG_NOMOVIES") != NULL || !av_load()) {
        return NULL;
    }
    p = calloc(1, sizeof(*p));
    if (p != NULL) {
        p->vtbl = sVtbl;
        p->shownNo = -1;
    }
    return p;
}

/* (PS2 0x0023E878) a player setting (event command 0x6E / the movie scene) */
void func_0023E878(void *ply, unsigned a, unsigned b, int c) { (void)ply; (void)a; (void)b; (void)c; }

/* (PS2 0x0023CA88) a player mode */
void func_0023CA88(void *ply, int mode) { (void)ply; (void)mode; }

/* mwPlyGetTime (PS2 0x0023C480): the frames played */
int func_0023C480(void *ply) {
    Ply *p = ply;

    return p != NULL && p->shownNo >= 0 ? p->shownNo : 0;
}

/* mwPlyIsPause (PS2 0x0023B4B0) */
int func_0023B4B0(void *ply) {
    Ply *p = ply;

    return p != NULL && p->paused;
}

/* mwPlyGetCurFrm (PS2 0x00239828): the frame due, if it is new (its image NULL otherwise) */
void func_00239828(void *ply, void *frmObj) {
    Ply *p = ply;
    uint8_t *o = frmObj;

    memset(o, 0, 0x24);
    if (p == NULL) {
        return;
    }
    ply_update(p);
    if (p->fresh && p->rgb != NULL) {
        *(uint8_t **)o = p->rgb;
        *(int *)(o + 0x8) = p->w;
        *(int *)(o + 0xC) = p->h;
        *(int *)(o + 0x20) = p->shownNo;
        p->fresh = 0;
    }
}

/* mwPlyRelCurFrm (PS2 0x0023A180) */
void func_0023A180(void *ply) { (void)ply; }

/* (PS2 0x002410B0) the current frame copied out to `dst` (w x h RGBA) */
void func_002410B0(void *ply, void *frmObj, void *dst) {
    Ply *p = ply;

    (void)frmObj;
    if (p != NULL && p->rgb != NULL && dst != NULL) {
        memcpy(dst, p->rgb, (size_t)p->w * p->h * 4);
    }
}
