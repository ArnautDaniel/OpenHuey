/* Streamed music: see music.h.
 *
 * ADX: a big-endian header - 0x8000, the data's offset - 4 at +0x2, encoding +0x4 (3), block
 * size +0x5 (18), channels +0x7, rate +0x8, samples +0xC, the high-pass cutoff +0x10, version
 * +0x12 (3: loop info from +0x18, 4: from +0x24 - enabled, start sample, start byte, end
 * sample, end byte); then per 32 samples a block per channel: a scale and 16 bytes of 4-bit
 * deltas, predicted from the two samples before. */
#include "music.h"

#include "../core/files.h"
#include "sound.h"

#include <SDL3/SDL.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FRAME 32   /* samples a block */

static struct {
    SDL_Mutex *lock;
    uint8_t *data;
    size_t size;
    int playing, paused, loop;
    float volume;
    int ch, rate, block, coef1, coef2;
    size_t start, pos;          /* the data's start, the next block */
    size_t loop_start, loop_end;    /* bytes (loop_end 0: the end of the data) */
    int hist[2][2];
    int16_t pcm[2][FRAME];      /* the block decoded */
    int at;                     /* the next sample in it (FRAME: none left) */
    double frac;
    float last[2], cur[2];
} M;

static unsigned be16(const uint8_t *b) { return (unsigned)(b[0] << 8 | b[1]); }
static unsigned be32(const uint8_t *b) { return (unsigned)b[0] << 24 | (unsigned)b[1] << 16 | (unsigned)b[2] << 8 | b[3]; }

static void lock(void) {
    if (M.lock == NULL) {
        M.lock = SDL_CreateMutex();
    }
    SDL_LockMutex(M.lock);
}

static void unlock(void) { SDL_UnlockMutex(M.lock); }

/* the next block (per channel) decoded; 0 at the end */
static int decode(void) {
    size_t end = M.loop_end != 0 ? M.loop_end : M.size;
    int c, i;

    if (M.pos + (size_t)M.block * M.ch > end) {
        if (!M.loop) {
            return 0;
        }
        M.pos = M.loop_start != 0 ? M.loop_start : M.start;
        if (M.pos + (size_t)M.block * M.ch > M.size) {
            return 0;
        }
    }
    for (c = 0; c < M.ch; c++) {
        const uint8_t *b = M.data + M.pos + (size_t)c * M.block;
        int scale = (int)be16(b), h1 = M.hist[c][0], h2 = M.hist[c][1];

        if (scale & 0x8000) {   /* the end marker */
            if (!M.loop) {
                return 0;
            }
            M.pos = M.loop_start != 0 ? M.loop_start : M.start;
            return decode();
        }
        for (i = 0; i < FRAME; i++) {
            int v = (b[2 + i / 2] >> (i & 1 ? 0 : 4)) & 0xF;

            if (v & 8) {
                v -= 16;
            }
            v = v * scale + ((M.coef1 * h1 + M.coef2 * h2) >> 12);
            v = v > 32767 ? 32767 : v < -32768 ? -32768 : v;
            M.pcm[c][i] = (int16_t)v;
            h2 = h1;
            h1 = v;
        }
        M.hist[c][0] = h1;
        M.hist[c][1] = h2;
    }
    M.pos += (size_t)M.block * M.ch;
    M.at = 0;
    return 1;
}

static void mix(int16_t *out, int frames) {
    double step;
    int i;

    lock();
    if (!M.playing || M.paused || M.data == NULL) {
        unlock();
        return;
    }
    step = (double)M.rate / SOUND_RATE;
    for (i = 0; i < frames; i++) {
        int c;

        M.frac += step;
        while (M.frac >= 1.0) {
            if (M.at >= FRAME && !decode()) {
                M.playing = 0;
                break;
            }
            M.last[0] = M.cur[0];
            M.last[1] = M.cur[1];
            M.cur[0] = M.pcm[0][M.at];
            M.cur[1] = M.pcm[M.ch > 1][M.at];
            M.at++;
            M.frac -= 1.0;
        }
        if (!M.playing) {
            break;
        }
        for (c = 0; c < 2; c++) {
            int v = out[i * 2 + c] + (int)((M.last[c] + (M.cur[c] - M.last[c]) * (float)M.frac) * M.volume);

            out[i * 2 + c] = (int16_t)(v > 32767 ? 32767 : v < -32768 ? -32768 : v);
        }
    }
    unlock();
}

int music_play(const char *path, int loop, int paused) {
    char p[256];
    size_t size, i;
    uint8_t *data;
    unsigned off, cutoff, ver;
    double x, y, z;

    snprintf(p, sizeof(p), "%s", path);
    for (i = 0; p[i] != 0; i++) {   /* (the game writes DOS paths) */
        if (p[i] == '\\') {
            p[i] = '/';
        }
    }
    data = files_read(p, &size);
    if (data == NULL || size < 0x30 || be16(data) != 0x8000) {
        free(data);
        return 0;
    }
    lock();
    free(M.data);
    memset(&M.data, 0, sizeof(M) - offsetof(__typeof__(M), data));
    M.data = data;
    M.size = size;
    off = be16(data + 2) + 4;
    M.start = M.pos = off;
    M.block = data[5];
    M.ch = data[7] >= 2 ? 2 : 1;
    M.rate = (int)be32(data + 8);
    cutoff = be16(data + 0x10);
    ver = data[0x12];
    x = sqrt(2.0) - cos(2.0 * M_PI * cutoff / (M.rate > 0 ? M.rate : 48000));
    y = sqrt(2.0) - 1.0;
    z = (x - sqrt((x + y) * (x - y))) / y;
    M.coef1 = (int)floor(z * 8192.0);
    M.coef2 = (int)floor(z * z * -4096.0);
    if ((ver == 3 || ver == 4) && off >= (ver == 3 ? 0x2Cu : 0x38u)) {
        const uint8_t *l = data + (ver == 3 ? 0x18 : 0x24);

        if (be32(l) != 0) {
            M.loop_start = be32(l + 8);
            M.loop_end = be32(l + 16);
            if (M.loop_start < off || M.loop_start >= size || M.loop_end > size || M.loop_end <= M.loop_start) {
                M.loop_start = M.loop_end = 0;
            }
        }
    }
    M.loop = loop;
    M.paused = paused;
    M.volume = 1.0f;
    M.at = FRAME;
    M.playing = M.block > 2 && M.rate > 0;
    unlock();
    sound_stream_slot(1, mix);
    return 1;
}

void music_stop(void) {
    lock();
    M.playing = 0;
    free(M.data);
    M.data = NULL;
    unlock();
}

void music_pause(int on) {
    lock();
    M.paused = on != 0;
    unlock();
}

void music_volume(float v) {
    lock();
    M.volume = v < 0.0f ? 0.0f : v > 1.0f ? 1.0f : v;
    unlock();
}

int music_playing(void) {
    int on;

    lock();
    on = M.playing;
    unlock();
    return on;
}
