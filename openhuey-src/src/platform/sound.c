/* Sound out: see sound.h. The mixing happens in SDL's audio thread (the stream's callback). */
#include "sound.h"

#include <SDL3/SDL.h>
#include <string.h>

static SDL_AudioStream *sStream;
static SDL_Mutex *sLock;
#define STREAMS 4
static void (*volatile sStreamMix[STREAMS])(int16_t *out, int frames);   /* [0] the movie's */

static void SDLCALL feed(void *ud, SDL_AudioStream *s, int additional, int total) {
    int16_t buf[1024 * 2];
    (void)ud;
    (void)total;

    while (additional > 0) {
        int frames = additional / 4 > 1024 ? 1024 : additional / 4, v;

        if (frames <= 0) {
            break;
        }
        memset(buf, 0, sizeof(buf));
        for (v = 0; v < STREAMS; v++) {
            if (sStreamMix[v] != NULL) {
                sStreamMix[v](buf, frames);
            }
        }
        SDL_PutAudioStreamData(s, buf, frames * 4);
        additional -= frames * 4;
    }
}

int sound_open(void) {
    SDL_AudioSpec spec = {SDL_AUDIO_S16, 2, SOUND_RATE};

    if (sStream != NULL) {
        return 1;
    }
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        return 0;
    }
    sLock = SDL_CreateMutex();
    sStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, feed, NULL);
    if (sStream == NULL) {
        return 0;
    }
    SDL_ResumeAudioStreamDevice(sStream);
    return 1;
}

static float sGain = 1.0f, sFade = 1.0f;   /* the options' volume; the game's fade over it */

static float clamp01(float v) {
    return v < 0.0f ? 0.0f : v > 1.0f ? 1.0f : v;
}

static void apply_gain(void) {
    if (sStream != NULL) {
        SDL_SetAudioStreamGain(sStream, sGain * sFade);
    }
}

void sound_gain(float g) {   /* (everything heard: the options' volume) */
    sGain = clamp01(g);
    apply_gain();
}

void sound_fade_all(float v) {
    sFade = clamp01(v);
    apply_gain();
}

float sound_fade_level(void) {
    return sFade;
}

void sound_close(void) {
    if (sStream != NULL) {
        SDL_DestroyAudioStream(sStream);
        sStream = NULL;
    }
}

void sound_stream(void (*mix)(int16_t *out, int frames)) {
    if (sLock != NULL) {
        SDL_LockMutex(sLock);
    }
    sStreamMix[0] = mix;
    if (sLock != NULL) {
        SDL_UnlockMutex(sLock);
    }
}

void sound_stream_slot(int slot, void (*mix)(int16_t *out, int frames)) {
    if (slot <= 0 || slot >= STREAMS) {
        return;
    }
    if (sLock != NULL) {
        SDL_LockMutex(sLock);
    }
    sStreamMix[slot] = mix;
    if (sLock != NULL) {
        SDL_UnlockMutex(sLock);
    }
}
