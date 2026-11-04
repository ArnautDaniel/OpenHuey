/* The SPU's voices (platform/spu.c) checked: a sample set on a voice that is playing waits for
 * its next key on, as the SSA register does - a looped sound kept going (the rooms' fires,
 * their hum: the sound driver's "settings only" updates each frame name its sample again) plays
 * on as it was, and doesn't start over.
 *
 *   spu-check */
#include <stdio.h>
#include <string.h>

#include "../src/platform/sound.h"
#include "../src/platform/spu.h"

static void (*sMix)(int16_t *out, int frames);

void sound_stream_slot(int slot, void (*mix)(int16_t *out, int frames)) {   /* (the output: kept to call here) */
    (void)slot;
    sMix = mix;
}

#define LEN 280     /* ten blocks, looped from the start */
#define HALF 400    /* frames before the update; as many after (more than once round the loop) */

static int16_t sRamp[LEN];

static void voice(int v) {
    spu_voice_pcm(v, sRamp, sRamp, LEN, 0);
    spu_voice_pitch(v, 0x1000);   /* (48 kHz: a sample a frame) */
    spu_voice_adsr(v, 0x000F, 0); /* (at full at once, and held) */
    spu_voice_volume(v, 0x3FFF, 0x3FFF);
    spu_voice_mix(v, 3);
}

/* voice v keyed and played 2 * HALF frames into out; `again`: its sample set anew halfway */
static void play(int v, int again, int16_t *out) {
    voice(v);
    spu_key_on(1ull << v);
    memset(out, 0, sizeof(int16_t) * 2 * 2 * HALF);
    sMix(out, HALF);
    if (again) {
        voice(v);   /* (no key on: settings only) */
    }
    sMix(out + 2 * HALF, HALF);
    spu_forget(sRamp);
}

int main(void) {
    static int16_t plain[2 * 2 * HALF], updated[2 * 2 * HALF];
    int i;

    for (i = 0; i < LEN; i++) {
        sRamp[i] = (int16_t)(i * 100 - 14000);
    }
    spu_start();
    if (sMix == NULL) {
        fprintf(stderr, "FAIL: no output\n");
        return 1;
    }
    play(0, 0, plain);
    play(0, 1, updated);
    for (i = 0; i < 2 * 2 * HALF; i++) {
        if (plain[i] != updated[i]) {
            fprintf(stderr, "FAIL: frame %d after the update: %d, playing on it was %d (started over)\n", i / 2 - HALF,
                    updated[i], plain[i]);
            return 1;
        }
    }
    printf("spu: a looped voice plays on through a settings-only update\n");
    return 0;
}
