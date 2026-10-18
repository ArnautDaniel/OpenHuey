/* Streamed music: see music.h. The track's samples come from data/musicsrc.c (the library's
 * FLAC, or the disc's ADX), resampled here into the mixer. */
#include "music.h"

#include "../core/files.h"
#include "../data/musicsrc.h"
#include "sound.h"

#include <SDL3/SDL.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

static struct {
    SDL_Mutex *lock;
    MusicSrc *src;
    int playing, paused;
    float volume;
    int ch, rate;
    int16_t pcm[2][MUSICSRC_FRAME];   /* the samples to play next */
    int at, n;                        /* the next of them, how many */
    double frac;
    float last[2], cur[2];
} M;

static void lock(void) {
    if (M.lock == NULL) {
        M.lock = SDL_CreateMutex();
    }
    SDL_LockMutex(M.lock);
}

static void unlock(void) { SDL_UnlockMutex(M.lock); }

static void mix(int16_t *out, int frames) {
    double step;
    int i;

    lock();
    if (!M.playing || M.paused || M.src == NULL) {
        unlock();
        return;
    }
    step = (double)M.rate / SOUND_RATE;
    for (i = 0; i < frames; i++) {
        int c;

        M.frac += step;
        while (M.frac >= 1.0) {
            if (M.at >= M.n) {
                M.n = musicsrc_next(M.src, M.pcm);
                M.at = 0;
                if (M.n <= 0) {
                    M.playing = 0;
                    break;
                }
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

/* the library's FLAC at the ADX's path ("ADX00/AD_01.ADX" -> "ADX00/AD_01.flac"), else the ADX */
int music_play(const char *path, int loop, int paused) {
    char p[256], *dot;
    size_t size, i;
    uint8_t *data;
    MusicSrc *src;

    snprintf(p, sizeof(p), "%s", path);
    for (i = 0; p[i] != 0; i++) {   /* (the game writes DOS paths) */
        if (p[i] == '\\') {
            p[i] = '/';
        }
    }
    dot = strrchr(p, '.');
    if (dot != NULL && strcasecmp(dot, ".ADX") == 0) {
        snprintf(dot, sizeof(p) - (size_t)(dot - p), ".flac");
        data = files_read(p, &size);
        snprintf(dot, sizeof(p) - (size_t)(dot - p), ".ADX");
    } else {
        data = NULL;
    }
    if (data == NULL) {
        data = files_read(p, &size);
    }
    if ((src = musicsrc_open(data, size, loop)) == NULL) {
        return 0;
    }
    lock();
    musicsrc_free(M.src);
    M.src = src;
    M.ch = musicsrc_channels(src);
    M.rate = musicsrc_rate(src);
    M.paused = paused;
    M.volume = 1.0f;
    M.at = M.n = 0;
    M.frac = 0;
    M.last[0] = M.last[1] = M.cur[0] = M.cur[1] = 0;
    M.playing = 1;
    unlock();
    sound_stream_slot(1, mix);
    return 1;
}

void music_stop(void) {
    lock();
    M.playing = 0;
    musicsrc_free(M.src);
    M.src = NULL;
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
