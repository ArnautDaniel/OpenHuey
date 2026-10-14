/* The pad's rumble (src/game/pad.c: Rumble_Tick, Rumble_SetA / SetB / MoveB, Rumble_StartLists). */
#include "rumble.h"

#include "../platform/platform.h"

#include <string.h>

#define SEQUENCE_STEPS 8

typedef struct Motor {     /* a motor's ask on a channel */
    int frames;            /* frames it lasts */
    float value, step;     /* its strength now, and its change a frame */
} Motor;

typedef struct Step {
    int small_on, from, to, frames;
} Step;

static struct {
    int enabled, allowed;
    Motor small[RUMBLE_CHANNELS], large[RUMBLE_CHANNELS];
    Step steps[SEQUENCE_STEPS];
    int nsteps, next;      /* the sequence: its steps, the next to play */
    int small_now, large_now;
} R = {.enabled = 1, .allowed = 1};

static void ask(Motor *m, float value, float step, int frames) {
    m->frames = frames;
    m->value = value;
    m->step = step;
}

static int valid(int channel) { return channel >= 0 && channel < RUMBLE_CHANNELS; }

void rumble_jolt(int channel, int strength, int frames) {
    if (valid(channel)) {
        ask(&R.large[channel], (float)strength, 0.0f, frames);
    }
}

void rumble_buzz(int channel, int on, int frames) {
    if (valid(channel)) {
        ask(&R.small[channel], on ? 1.0f : 0.0f, 0.0f, frames);
    }
}

void rumble_swell(int channel, int from, int to, int frames) {
    if (valid(channel)) {
        ask(&R.large[channel], (float)from, frames > 0 ? (float)(to - from) / (float)frames : 0.0f, frames);
    }
}

static void play_step(void) {
    const Step *s = &R.steps[R.next++];

    rumble_buzz(RUMBLE_SEQUENCE, s->small_on, s->frames);
    rumble_swell(RUMBLE_SEQUENCE, s->from, s->to, s->frames);
}

void rumble_sequence_clear(void) {
    R.nsteps = R.next = 0;
    ask(&R.small[RUMBLE_SEQUENCE], 0.0f, 0.0f, 0);
    ask(&R.large[RUMBLE_SEQUENCE], 0.0f, 0.0f, 0);
}

void rumble_sequence_add(int small_on, int from, int to, int frames) {
    if (R.nsteps < SEQUENCE_STEPS) {
        R.steps[R.nsteps++] = (Step){small_on, from, to, frames};
        if (R.nsteps == 1) {
            play_step();
        }
    }
}

void rumble_clear(void) {
    memset(R.small, 0, sizeof(R.small));
    memset(R.large, 0, sizeof(R.large));
    R.nsteps = R.next = 0;
}

void rumble_allow(int on) {
    R.allowed = on != 0;
    rumble_enable(R.enabled);
}

void rumble_enable(int on) {
    R.enabled = on != 0;
    if (!R.enabled || !R.allowed) {
        R.small_now = R.large_now = 0;
        platform_rumble(0, 0.0f);
    }
}

static float run(Motor *m) {   /* a frame of it: what it gives now (0 once over) */
    if (m->frames <= 0) {
        m->value = m->step = 0.0f;
        return 0.0f;
    }
    m->frames--;
    m->value += m->step;
    if (m->value < 0.0f) {
        m->value = 0.0f;
    }
    return m->value;
}

void rumble_tick(void) {
    float small = 0.0f, large = 0.0f;
    int c;

    if (!R.enabled || !R.allowed) {
        return;
    }
    if (R.large[RUMBLE_SEQUENCE].frames <= 0 && R.small[RUMBLE_SEQUENCE].frames <= 0 && R.next < R.nsteps) {
        play_step();   /* (the sequence's next step) */
    }
    for (c = 0; c < RUMBLE_CHANNELS; c++) {
        float s = run(&R.small[c]), l = run(&R.large[c]);

        small = s > small ? s : small;
        large = l > large ? l : large;
    }
    R.small_now = small > 0.0f;
    R.large_now = large > 255.0f ? 255 : (int)large;
    platform_rumble(R.small_now, (float)R.large_now / 255.0f);
}

void rumble_now(int *small_on, int *large) {
    *small_on = R.small_now;
    *large = R.large_now;
}
