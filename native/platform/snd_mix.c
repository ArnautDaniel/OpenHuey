/* The PC sound output: one SDL3 audio stream (48 kHz stereo float) whose callback mixes the
 * sound sources - the CRI ADX streams (adx.c) and the SPU voices of the sound driver
 * (snddrv.c). Game-side calls change the sources under snd_lock(), which holds off the mixer. */
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snd.h"

static SDL_AudioStream *sStream;
static SDL_Mutex *sLock;
static int sTried;

#define CHUNK 512

static void SDLCALL mix_cb(void *ud, SDL_AudioStream *s, int additional, int total) {
    float buf[CHUNK * 2];

    (void)ud;
    (void)total;
    while (additional > 0) {
        int frames = additional / (int)(2 * sizeof(float));
        int i;

        if (frames > CHUNK) {
            frames = CHUNK;
        }
        if (frames <= 0) {
            frames = 1;
        }
        memset(buf, 0, sizeof(float) * 2 * frames);
        SDL_LockMutex(sLock);
        adx_render(buf, frames);
        spu_render(buf, frames);
        SDL_UnlockMutex(sLock);
        for (i = 0; i < frames * 2; i++) {
            if (buf[i] > 1.0f) {
                buf[i] = 1.0f;
            } else if (buf[i] < -1.0f) {
                buf[i] = -1.0f;
            }
        }
        SDL_PutAudioStreamData(s, buf, (int)(sizeof(float) * 2 * frames));
        additional -= (int)(sizeof(float) * 2 * frames);
    }
}

/* open the output once (later calls do nothing); 0 if there is no sound (it all stays silent) */
int snd_init(void) {
    SDL_AudioSpec spec;

    if (sTried) {
        return sStream != NULL;
    }
    sTried = 1;
    sLock = SDL_CreateMutex();
    if (getenv("HG_NOSOUND") != NULL) {
        return 0;
    }
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        fprintf(stderr, "snd: no audio: %s\n", SDL_GetError());
        return 0;
    }
    spec.format = SDL_AUDIO_F32;
    spec.channels = 2;
    spec.freq = SND_RATE;
    sStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, mix_cb, NULL);
    if (sStream == NULL) {
        fprintf(stderr, "snd: can't open the audio device: %s\n", SDL_GetError());
        return 0;
    }
    SDL_ResumeAudioStreamDevice(sStream);
    return 1;
}

void snd_lock(void) {
    if (sLock == NULL) {
        sLock = SDL_CreateMutex();
    }
    SDL_LockMutex(sLock);
}

void snd_unlock(void) {
    SDL_UnlockMutex(sLock);
}

/* 0.1 dB (0 full, -999 and below silent) as a gain */
float snd_db10(int db10) {
    if (db10 <= -999) {
        return 0.0f;
    }
    if (db10 > 0) {
        db10 = 0;
    }
    return SDL_powf(10.0f, (float)db10 / 200.0f);
}
