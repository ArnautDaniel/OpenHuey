/* CRI ADX streams (ADXT) on PC: the game's music stream (src/game/bgm.c) plays .ADX files
 * through this. Standard 4-bit ADX (type 3, header versions 3 and 4 with their loop points),
 * decoded straight from the file on the mixer thread and resampled to the output rate. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snd.h"

/* ADXT_GetStat */
enum { ADXT_STAT_STOP = 0, ADXT_STAT_DECINFO, ADXT_STAT_PREP, ADXT_STAT_PLAYING, ADXT_STAT_DECEND,
       ADXT_STAT_PLAYEND, ADXT_STAT_ERROR };

#define ADX_MAXCH 2
#define BLOCK 18   /* bytes per channel per frame: scale + 32 nibbles */
#define FRAME 32   /* samples per frame */

typedef struct Adxt {
    /* settings */
    int maxch;
    int vol;            /* 0.1 dB */
    int pan[ADX_MAXCH]; /* -15..15, -128 auto */
    int loop;
    int paused;
    /* the stream */
    FILE *f;
    int stat;
    int ch, rate;
    unsigned total;             /* samples */
    long data;                  /* first frame's offset */
    int looping;                /* the file loops (and the loop flag is on) */
    unsigned loopStart, loopEnd; /* samples */
    int coef1, coef2;
    int hist[ADX_MAXCH][2];
    short pcm[ADX_MAXCH][FRAME];
    unsigned frame;             /* the decoded frame's first sample */
    unsigned pos;               /* the next sample */
    double frac;                /* resampling position between `pos - 1` and `pos` */
    float last[ADX_MAXCH], cur[ADX_MAXCH];
    struct Adxt *next;
} Adxt;

static Adxt *sAll;
static int sMono;                 /* ADXT_SetOutputMono */
static const void *sFolder;       /* CriFs_SetDir: the folder for the next start */

static unsigned be16(const unsigned char *p) { return (p[0] << 8) | p[1]; }
static unsigned be32(const unsigned char *p) { return ((unsigned)p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3]; }

/* read the header; 0 if it isn't a playable ADX */
static int adx_open(Adxt *a) {
    unsigned char h[0x40];
    unsigned off, ver, cutoff;
    double x, y, z;

    memset(h, 0, sizeof(h));
    if (fread(h, 1, sizeof(h), a->f) < 0x20 || be16(h) != 0x8000) {
        return 0;
    }
    off = be16(h + 2);
    if (h[4] != 3 || h[5] != BLOCK || h[6] != 4) {
        fprintf(stderr, "adx: unsupported encoding %d/%d/%d\n", h[4], h[5], h[6]);
        return 0;
    }
    a->ch = h[7];
    if (a->ch < 1 || a->ch > ADX_MAXCH) {
        return 0;
    }
    a->rate = (int)be32(h + 8);
    a->total = be32(h + 0xC);
    cutoff = be16(h + 0x10);
    ver = h[0x12];
    a->data = (long)off + 4;
    a->looping = 0;
    if (ver == 3 && off >= 0x2C - 4 && be32(h + 0x18) != 0) {
        a->looping = 1;
        a->loopStart = be32(h + 0x1C);
        a->loopEnd = be32(h + 0x24);
    } else if (ver == 4 && off >= 0x38 - 4 && be32(h + 0x24) != 0) {
        a->looping = 1;
        a->loopStart = be32(h + 0x28);
        a->loopEnd = be32(h + 0x30);
    }
    if (a->looping && (a->loopEnd <= a->loopStart || a->loopEnd > a->total)) {
        a->looping = 0;
    }
    /* the prediction filter from the high-pass cutoff */
    x = sqrt(2.0) - cos(2.0 * M_PI * cutoff / a->rate);
    y = sqrt(2.0) - 1.0;
    z = (x - sqrt((x + y) * (x - y))) / y;
    a->coef1 = (int)floor(z * 8192.0);
    a->coef2 = (int)floor(z * z * -4096.0);
    return 1;
}

/* decode the frame holding sample `s` (frames are decoded in order; a jump back restarts the
   filter history, as a loop does on the PS2 too - the loop start is frame-aligned) */
static int adx_decode(Adxt *a, unsigned s) {
    unsigned char buf[BLOCK * ADX_MAXCH];
    unsigned fr = s / FRAME;
    int c, i;

    if (fseek(a->f, a->data + (long)fr * BLOCK * a->ch, SEEK_SET) != 0
        || fread(buf, 1, (size_t)BLOCK * a->ch, a->f) != (size_t)BLOCK * a->ch) {
        return 0;
    }
    for (c = 0; c < a->ch; c++) {
        const unsigned char *b = buf + c * BLOCK;
        int scale = (int)be16(b);
        int h1 = a->hist[c][0], h2 = a->hist[c][1];

        if (scale & 0x8000) {   /* the end marker */
            return 0;
        }
        for (i = 0; i < FRAME; i++) {
            int n = (b[2 + i / 2] >> (i & 1 ? 0 : 4)) & 0xF;
            int v;

            if (n & 8) {
                n -= 16;
            }
            v = n * scale + ((a->coef1 * h1 + a->coef2 * h2) >> 12);
            if (v > 32767) {
                v = 32767;
            } else if (v < -32768) {
                v = -32768;
            }
            a->pcm[c][i] = (short)v;
            h2 = h1;
            h1 = v;
        }
        a->hist[c][0] = h1;
        a->hist[c][1] = h2;
    }
    a->frame = fr * FRAME;
    return 1;
}

/* the next source sample into cur[]; 0 at the end */
static int adx_next(Adxt *a) {
    int c;

    if (a->looping && a->loop && a->pos >= a->loopEnd) {
        a->pos = a->loopStart;
        a->frame = (unsigned)-1;
    }
    if (a->pos >= a->total) {
        return 0;
    }
    if (a->frame == (unsigned)-1 || a->pos < a->frame || a->pos >= a->frame + FRAME) {
        if (a->pos != a->frame + FRAME) {
            memset(a->hist, 0, sizeof(a->hist));
        }
        if (!adx_decode(a, a->pos)) {
            return 0;
        }
    }
    for (c = 0; c < a->ch; c++) {
        a->last[c] = a->cur[c];
        a->cur[c] = a->pcm[c][a->pos - a->frame] / 32768.0f;
    }
    a->pos++;
    return 1;
}

static void adx_close(Adxt *a) {
    if (a->f != NULL) {
        fclose(a->f);
        a->f = NULL;
    }
}

void adx_render(float *out, int frames) {
    Adxt *a;
    int i;

    static int mute = -1;

    if (mute < 0) {
        mute = getenv("HG_NOMUSIC") != NULL;
    }
    for (a = sAll; a != NULL && !mute; a = a->next) {
        double step;
        float g, gl, gr;

        if (a->stat != ADXT_STAT_PLAYING || a->paused || a->f == NULL) {
            continue;
        }
        g = snd_db10(a->vol);
        gl = gr = g;
        if (a->ch == 1 && a->pan[0] != -128) {   /* a mono stream panned: -15 left .. 15 right */
            float p = (a->pan[0] + 15) / 30.0f;

            gl = g * sqrtf(1.0f - p);
            gr = g * sqrtf(p);
        }
        step = (double)a->rate / SND_RATE;
        for (i = 0; i < frames; i++) {
            float l, r;

            a->frac += step;
            while (a->frac >= 1.0) {
                if (!adx_next(a)) {
                    a->stat = ADXT_STAT_PLAYEND;
                    adx_close(a);
                    break;
                }
                a->frac -= 1.0;
            }
            if (a->stat != ADXT_STAT_PLAYING) {
                break;
            }
            l = a->last[0] + (a->cur[0] - a->last[0]) * (float)a->frac;
            r = a->ch > 1 ? a->last[1] + (a->cur[1] - a->last[1]) * (float)a->frac : l;
            if (sMono) {
                l = r = 0.5f * (l + r);
            }
            out[2 * i] += l * gl;
            out[2 * i + 1] += r * gr;
        }
    }
}

/* ---- the ADXT API the game calls ---- */

void *ADXT_Create(int maxch, void *work, int size) {
    Adxt *a;

    (void)work;
    (void)size;
    snd_init();
    a = calloc(1, sizeof(*a));
    a->maxch = maxch;
    a->pan[0] = a->pan[1] = -128;
    a->stat = ADXT_STAT_STOP;
    snd_lock();
    a->next = sAll;
    sAll = a;
    snd_unlock();
    return a;
}

void ADXT_Destroy(void *h) {
    Adxt *a = h, **p;

    if (a == NULL) {
        return;
    }
    snd_lock();
    for (p = &sAll; *p != NULL; p = &(*p)->next) {
        if (*p == a) {
            *p = a->next;
            break;
        }
    }
    adx_close(a);
    snd_unlock();
    free(a);
}

/* ADXT_StartFname: play `name` from the current folder (CriFs_SetDir) */
void ADXT_StartFname(void *h, char *name) {
    Adxt *a = h;
    FILE *f;

    if (a == NULL) {
        return;
    }
    f = crifs_open(crifs_folder(sFolder), name);
    snd_lock();
    adx_close(a);
    a->f = f;
    a->stat = ADXT_STAT_ERROR;
    if (f == NULL) {
        fprintf(stderr, "adx: can't open %s/%s\n", crifs_folder(sFolder), name);
    } else if (adx_open(a)) {
        fprintf(stderr, "adx: play %s/%s (%d ch, %d Hz, %.1f s%s)\n", crifs_folder(sFolder), name, a->ch, a->rate,
                (double)a->total / a->rate, a->looping ? ", looping" : "");
        a->stat = ADXT_STAT_PLAYING;
        a->pos = 0;
        a->frame = (unsigned)-1;
        a->frac = 0.0;
        memset(a->hist, 0, sizeof(a->hist));
        memset(a->last, 0, sizeof(a->last));
        memset(a->cur, 0, sizeof(a->cur));
    } else {
        adx_close(a);
    }
    snd_unlock();
}

void ADXT_Stop(void *h) {
    Adxt *a = h;

    if (a != NULL) {
        snd_lock();
        adx_close(a);
        a->stat = ADXT_STAT_STOP;
        snd_unlock();
    }
}

int ADXT_GetStat(void *h) {
    Adxt *a = h;

    return a != NULL ? a->stat : ADXT_STAT_STOP;
}

/* ended (PS2 0x001D3E20) */
int ADXT_IsPlaying(void *h) {
    Adxt *a = h;

    return a == NULL ? -1 : a->stat == ADXT_STAT_PLAYEND;
}

int ADXT_IsReadyPlayStart(void *h) {
    Adxt *a = h;

    return a != NULL && a->stat == ADXT_STAT_PLAYING;
}

void ADXT_SetReloadSct(void *h, int n) { (void)h; (void)n; }
void ADXT_SetWaitPlayStart(void *h, int on) { (void)h; (void)on; }

void ADXT_SetOutVol(void *h, int vol) {
    Adxt *a = h;

    if (a != NULL) {
        a->vol = vol;
    }
}

void ADXT_SetOutPan(void *h, int ch, int pan) {
    Adxt *a = h;

    if (a != NULL && ch >= 0 && ch < ADX_MAXCH) {
        a->pan[ch] = pan;
    }
}

void ADXT_SetLpFlg(void *h, int on) {
    Adxt *a = h;

    if (a != NULL) {
        a->loop = on;
    }
}

void ADXT_Pause(void *h, int on) {
    Adxt *a = h;

    if (a != NULL) {
        a->paused = on;
    }
}

/* ADX: mono / stereo output (PS2 0x001D4750) */
void ADXT_SetOutputMono(int mono) {
    sMono = mono;
}

/* the CRI file system's current folder (a directory listing) for the next start (PS2
   0x001E7430; `a` is the device) */
void CriFs_SetDir(int a, const void *list) {
    (void)a;
    sFolder = list;
}

/* the folder set by CriFs_SetDir (the movie player opens its files there too, sofdec.c) */
const void *adx_current_folder(void) {
    return sFolder;
}

/* 0x0023C310 -> CRI 0x001CC710: run the middleware's server once by hand (the PC mixer thread
 * runs ADX on its own) */
void mwPly_ExecServer(void) {
}

/* 0x001EEA38: the libpad/dbc shutdown hook the system calls on exit; nothing to do on the PC */
int func_001EEA38(void) { return 0; }
