/* Sound out: see sound.h. The mixing happens in SDL's audio thread (the stream's callback). */
#include "sound.h"

#include <SDL3/SDL.h>
#include <stdlib.h>
#include <string.h>

#define VOICES 32

typedef struct Voice {
    int16_t *pcm;
    int n, at;
    float left, right;
} Voice;

static SDL_AudioStream *sStream;
static Voice sVoices[VOICES];
static SDL_Mutex *sLock;
#define STREAMS 4
static void (*volatile sStreamMix[STREAMS])(int16_t *out, int frames);   /* [0] the movie's */

static void SDLCALL feed(void *ud, SDL_AudioStream *s, int additional, int total) {
    int16_t buf[1024 * 2];
    (void)ud;
    (void)total;

    while (additional > 0) {
        int frames = additional / 4 > 1024 ? 1024 : additional / 4, i, v;

        if (frames <= 0) {
            break;
        }
        memset(buf, 0, sizeof(buf));
        SDL_LockMutex(sLock);
        for (v = 0; v < VOICES; v++) {
            Voice *vo = &sVoices[v];

            for (i = 0; vo->pcm != NULL && i < frames && vo->at < vo->n; i++, vo->at++) {
                int l = buf[i * 2] + (int)(vo->pcm[vo->at] * vo->left);
                int r = buf[i * 2 + 1] + (int)(vo->pcm[vo->at] * vo->right);

                buf[i * 2] = (int16_t)(l > 32767 ? 32767 : l < -32768 ? -32768 : l);
                buf[i * 2 + 1] = (int16_t)(r > 32767 ? 32767 : r < -32768 ? -32768 : r);
            }
            if (vo->pcm != NULL && vo->at >= vo->n) {
                free(vo->pcm);
                vo->pcm = NULL;
            }
        }
        SDL_UnlockMutex(sLock);
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

void sound_close(void) {
    int v;

    if (sStream != NULL) {
        SDL_DestroyAudioStream(sStream);
        sStream = NULL;
    }
    for (v = 0; v < VOICES; v++) {
        free(sVoices[v].pcm);
        sVoices[v].pcm = NULL;
    }
}

void sound_play(const int16_t *pcm, int n, float volume, float pan) {
    int v, oldest = 0;

    if (sStream == NULL || pcm == NULL || n <= 0) {
        return;
    }
    SDL_LockMutex(sLock);
    for (v = 0; v < VOICES; v++) {   /* a free voice, else the one furthest along */
        if (sVoices[v].pcm == NULL) {
            break;
        }
        if (sVoices[v].at > sVoices[oldest].at) {
            oldest = v;
        }
    }
    if (v == VOICES) {
        v = oldest;
        free(sVoices[v].pcm);
    }
    sVoices[v].pcm = malloc((size_t)n * sizeof(int16_t));
    if (sVoices[v].pcm != NULL) {
        memcpy(sVoices[v].pcm, pcm, (size_t)n * sizeof(int16_t));
        sVoices[v].n = n;
        sVoices[v].at = 0;
        sVoices[v].left = volume * (pan > 0.0f ? 1.0f - pan : 1.0f);
        sVoices[v].right = volume * (pan < 0.0f ? 1.0f + pan : 1.0f);
    }
    SDL_UnlockMutex(sLock);
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
