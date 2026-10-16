#include "particles.h"

#include <math.h>
#include <string.h>

#include "sprites.h"
#include "../render/post.h"
#include "engine.h"
#include "things.h"
#include "camdirector.h"

#define PI 3.14159265f
#define MAX_PARTS 256   /* (most have 16; the blood spray 32, the rising smoke 64, the dust shaft 256) */

enum { FLICKER, SPARKS, FLAME, SWARM, SPLASH, BURST, SHARDS, MOTE, BUTTERFLIES, SPIRIT, VANISH, WANDER, SPRAY, SPLAT, PUFFS, STONES, DEBRIS, SMOKE, DUST, SPIRAL, DRIPS, RING, DROPLETS, ONEDRIP, MOTES, EMBERS, FLECKS, CHIMNEY, WALLSHADOW, FIRE, STEAM, FLOORGLOW, TV, BIGFIRE, SHAFT, GLINT, HAZE, CAUSTIC, GLOWSPOT, WISPCOL, SPARKSPRAY, HEAT, EMBERS2, LEAVES, LOOPSPRITE, OBJGLOW, RAIN, ASH, MARKER, WINDOWFLASH, SMOKETRAIL, MAGBURST, DROPFLASH, BONESMOKE, SPARK2, SINKRING, WELLING };

typedef struct Part {
    Vec3 at, v;
    Vec3 pull;             /* (specks) what turns them back to the centre; (splash) x, z: the drag */
    float w, h, turn;
    int rgba[4];           /* as the original keeps them: ints, alpha 0..0x80 */
    int alpha0;            /* (specks) their own alpha, dimmed near the camera */
    int frame;
    float rise, angle, radius;   /* (sparks; the spiral smoke's way round; a drip's ripple) */
    /* (shards) a box: its corners, scale, turn and spin about x y z */
    float corner[8][3], scale[3], tturn[3], spin[3];
    int alive;
    /* (falling stones) the floor under it, frames before it shows, landed, bouncing, its alpha */
    float floor;
    int delay, landed, bounce, alpha;
} Part;

typedef struct System {
    int used, what, batch;
    int batch2, batch3;    /* (spirits: its colour and trail, its body; a vanishing: its sparks) -1 none */
    int batch4;            /* (the magnesias' bursts: their rings) -1 none */
    int owner;             /* -1: an actor's; else the system that made it (its own: ends with it) */
    int ends_with_owner;
    int engines;           /* the engine's (made by another): goes with the room */
    Vec3 at;
    int kind, command, timer, rest, slow, ended;
    int mark, top;         /* (bursts, kind 1) the red that marks a brightening mote; the top alpha */
    float floor;
    uint32_t colour;       /* (shards) $RRGGBBAA */
    int n;
    Part p[MAX_PARTS];
    /* (butterflies) Butterflies_*: the centre, the point it follows, the target; tilt, heading,
     * phases, steps, circles; how many; the tilt a mote was last shed at; their colours */
    Vec3 c, wander, target;
    int started;
    float tilt, head, ph[3], step[3], circ[3], last;
    int count;
    uint32_t wing[8];
    /* (spirits) CreatureA's: its bob (an offset, its speed and turn, every `period` frames; now
     * and then a short burst), its glow, its trail, shown, vanishing (seen: sinking first), fade */
    float bob, bob_v, bob_a, glow, saved_v, saved_a;
    int bob_n, period, burst, burst_n, saved_n;
    Vec3 trail[3];
    int trail_set, frames, shown, vanishing, seen, fade, red;
} System;

static System sSys[PARTICLE_SYSTEMS];
static uint32_t sSeed = 0x2545F491u;
static Vec3 sCamera;   /* (this tick's: the stones fade near it) */
static void write(System *s);   /* (each part a camera-facing sprite) */

/* ---- small things ---- */

static float rnd(void) {   /* 0 <= r < 1 */
    sSeed = sSeed * 1103515245u + 12345u;
    return (float)((sSeed >> 8) & 0xFFFF) / 65536.0f;
}
static int irnd(int n) {   /* 0 .. n-1 */
    sSeed = sSeed * 1103515245u + 12345u;
    return (int)((sSeed >> 8) & 0x7FFF) % n;
}
static float half(void) { return rnd() - 0.5f; }
static float rnd_turn(void) { return 2.0f * PI * half(); }   /* -pi .. pi */
static int clampi(int v, int lo, int hi) { return v < lo ? lo : v > hi ? hi : v; }
static float wrap(float a) {
    while (a > PI) {
        a -= 2.0f * PI;
    }
    while (a < -PI) {
        a += 2.0f * PI;
    }
    return a;
}
static float deg(float d) { return d * PI / 180.0f; }
static void colour(Part *p, int r, int g, int b, int a) {
    p->rgba[0] = r;
    p->rgba[1] = g;
    p->rgba[2] = b;
    p->rgba[3] = a;
}
/* a colour a little varied: each of r g b up to 15 off (from 8 below), alpha up to 7 (from 4) */
static void varied(Part *p, uint32_t rgba) {
    colour(p, clampi((int)(rgba >> 24 & 0xFF) - 8 + irnd(16), 0, 255), clampi((int)(rgba >> 16 & 0xFF) - 8 + irnd(16), 0, 255),
           clampi((int)(rgba >> 8 & 0xFF) - 8 + irnd(16), 0, 255), clampi((int)(rgba & 0xFF) - 4 + irnd(8), 0, 0x80));
}
static void square(Part *p, float s) { p->w = p->h = s; }

static System *get(int p) { return p >= 0 && p < PARTICLE_SYSTEMS && sSys[p].used ? &sSys[p] : NULL; }

static int new_system(int what, Vec3 at) {
    int i;

    for (i = 0; i < PARTICLE_SYSTEMS; i++) {
        if (!sSys[i].used) {
            System *s = &sSys[i];

            memset(s, 0, sizeof(*s));
            s->batch = sprites_new();
            if (s->batch < 0) {
                return -1;
            }
            s->used = 1;
            s->what = what;
            s->owner = -1;
            s->batch2 = s->batch3 = s->batch4 = -1;
            s->at = at;
            return i;
        }
    }
    return -1;
}
static void end(System *s) {
    sprites_free(s->batch);
    if (s->batch2 >= 0) {
        sprites_free(s->batch2);
    }
    if (s->batch3 >= 0) {
        sprites_free(s->batch3);
    }
    if (s->batch4 >= 0) {
        sprites_free(s->batch4);
    }
    s->used = 0;
}

/* its sprite sheet: GAME_FIX (group 0x10) unless said */
static void look(System *s, int group, int id, int palette, int x, int y, int cw, int ch, int frames, int flags) {
    sprites_texture(s->batch, group, id, palette);
    sprites_cells(s->batch, x, y, cw, ch, 512, 256, frames);
    sprites_flags(s->batch, flags, 0x19);
}

/* ---- EvEffect7F: flickering ---- */

static int sprite_rest(int slow) {   /* frames: 0.5 .. 1.2 s, slow 2.5 .. 4 s */
    return (int)((slow ? 1.5f * rnd() + 2.5f : 0.7f * rnd() + 0.5f) * 30.0f);
}

int particles_flicker(Vec3 at, int slow) {
    int h = new_system(FLICKER, at);
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0, 0, 32, 32, 16, 0);
    s->slow = slow;
    s->n = 1;
    s->p[0].at = at;
    colour(&s->p[0], 0x80, 0x80, 0x80, 0x40);
    square(&s->p[0], 1.6f);
    s->p[0].frame = irnd(16);
    s->timer = 1;
    s->rest = sprite_rest(slow);
    return h;
}

static void flicker_step(System *s) {   /* EvEffect7F_Update */
    if (s->rest > 0) {
        s->rest--;
        return;
    }
    if (--s->timer > 0) {
        return;
    }
    s->timer = 1;
    if (++s->p[0].frame < 16) {
        return;
    }
    s->p[0].frame = 0;
    s->p[0].turn = rnd_turn();
    s->rest = sprite_rest(s->slow);
}

/* ---- Wisps: a flame's sparks - 4 orange sparks rising and swirling about a point. Kind's low
 * 12 bits 0: a lasting flame's (slow, coming back), else a burst ---- */

static int lasting(const System *s) { return (s->kind & 0xFFF) == 0; }

static void wisp(System *s, Part *p, int again) {   /* Wisps_Wisp */
    p->rise = lasting(s) ? 0.1f * rnd() + 0.2f : 0.25f * rnd() + 0.01f;
    p->angle = deg(360.0f * half());
    p->radius = 0.5f * rnd();
    colour(p, 0x80, 0x40, 0x10, irnd(64) + 0x40);
    if (!again && p->rise < 0.05f) {
        p->rise = 0.05f;
    }
    p->at = vec3(cosf(p->angle) * p->radius + s->at.x, s->at.y, sinf(p->angle) * p->radius + s->at.z);
    square(p, 0.1f * rnd() + 0.2f);
    p->turn = rnd_turn();
    p->frame = 0;
}

static int sparks_new(int owner, Vec3 at, int kind) {
    int h = new_system(SPARKS, at), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0x6C, 0x4C, 8, 8, 1, 0x40);
    s->owner = owner;
    s->engines = 1;
    s->ends_with_owner = (kind & 0x8000) != 0;
    s->kind = kind;
    s->n = 4;
    for (i = 0; i < 4; i++) {
        wisp(s, &s->p[i], !lasting(s));
    }
    return h;
}

static int sparks_step(System *s) {   /* Wisps_Update: 0 once all have gone (a burst's) */
    int i, going = 0;

    for (i = 0; i < 4; i++) {
        Part *p = &s->p[i];

        if (p->rgba[3] > 0) {
            p->rgba[3] -= irnd(4) + 4;
        }
        if (p->rgba[3] <= 0) {
            if (lasting(s)) {
                wisp(s, p, 1);
                going = 1;
            } else {
                p->rgba[3] = 0;
            }
            continue;
        }
        going = 1;
        p->radius += lasting(s) ? 0.02f : 0.04f;
        p->angle = wrap(p->angle + deg(30.0f * half()));
        p->rise = p->rise - 0.015f < 0.05f ? 0.05f : p->rise - 0.015f;
        p->at = vec3(cosf(p->angle) * p->radius + s->at.x, p->at.y + p->rise, sinf(p->angle) * p->radius + s->at.z);
    }
    return going;
}

/* ---- EvEffect86: a flame. Kind 0 a candle (32 frames: the first half loops; commanded, the
 * second half plays at half speed - command 2 shrinking it away); others a 16-frame fire;
 * commanded, a fire of kind 1 throws 2 / 4 sparks ---- */

int particles_flame(Vec3 at, int kind) {
    int h = new_system(FLAME, at);
    System *s;
    Part *p;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    p = &s->p[0];
    s->n = 1;
    s->timer = 1;
    p->at = at;
    colour(p, 0x80, 0x80, 0x80, 0x80);
    p->frame = irnd(16);
    sprites_offset(s->batch, 0.0f, -1.0f);
    if (kind != 0) {
        p->w = 1.0f;
        p->h = 2.0f;
        look(s, 0x10, 1, kind == 3 ? 5 : 4, 0, 0x80, 16, 32, 16, kind == 4 ? 0x85 : 0x81);
        s->kind = kind == 4 ? 1 : kind;
        if (s->kind == 1) {
            sparks_new(h, vec3(at.x, at.y + 2.0f, at.z), 0x8000);
        }
    } else {
        p->w = 0.25f;
        p->h = 1.0f;
        look(s, 0x10, 1, 1, 0, 0x20, 16, 32, 32, 0x81);
    }
    return h;
}

static void embers_gust(System *s, int ending);

void particles_command(int h, int v) {
    System *s = get(h);
    int i;

    if (s != NULL && s->what == MARKER) {   /* (Marker_SetParams: -1 done (state 2), -2 a frame's pause (3), else its size (4) in thousandths) */
        if (v == -1) {
            s->ended = 1;
        } else if (v == -2) {
            s->command = -2;
        } else if (v >= 0) {
            s->c.x = (float)v / 1000.0f;
        }
        return;
    }
    if (s != NULL && s->what == OBJGLOW) {   /* (ObjectGlow_SetParams: on, off, $FF gone at once) */
        s->command = v;
        return;
    }
    if (s != NULL && s->what == HEAT) {   /* (Room92Effect_SetParams: its height to v (0: 3), v >= 256: at once) */
        float to = (float)(v & 0xFF);

        s->c.y = to == 0.0f ? 3.0f : to;
        if (v >= 256 || to == 0.0f) {
            s->c.x = s->c.y;
        }
        return;
    }
    if (s != NULL && s->what == EMBERS2) {   /* (Embers_SetParams with none: off) */
        s->ended = v == 0 ? 1 : s->ended;
        return;
    }
    if (s != NULL && s->what == SPARKSPRAY) {   /* (SparkSpray_SetParams: none, off) */
        s->ended = v == 0 ? 1 : s->ended;
        return;
    }
    if (s != NULL && s->what == WISPCOL) {   /* (WispColumn_SetParams: none, off - the alpha runs down) */
        s->ended = v == 0 ? 1 : s->ended;
        return;
    }
    if (s != NULL && s->what == SHAFT) {   /* (LightShaft_SetParams: 1 the haze on, 2 off, -1 stopped) */
        if (v < 0) {
            s->ended = 1;
        } else if (v == 1 || v == 2) {
            s->slow = v == 1;
        }
        return;
    }
    if (s != NULL && s->what == BIGFIRE) {   /* (BigFire_SetParams: nonzero, out) */
        s->ended = v != 0 ? 1 : s->ended;
        return;
    }
    if (s != NULL && s->what == TV) {   /* (TvScreen_SetParams: on or off) */
        s->command = v;
        return;
    }
    if (s != NULL && s->what == FLOORGLOW) {   /* (FloorGlow_SetParams: its level) */
        if (v != s->kind) {
            s->kind = v;
            s->command = v == 0x80 ? 2 : 0;
        }
        return;
    }
    if (s != NULL && s->what == STEAM) {   /* (Room66Effect_SetParams with a byte < 0: dying down) */
        s->ended = v < 0 ? 1 : s->ended;
        return;
    }
    if (s != NULL && s->what == FIRE) {   /* (Fire_SetParams with a negative spot: put out) */
        s->ended = v < 0 ? 1 : s->ended;
        return;
    }
    if (s != NULL && s->what == EMBERS) {   /* (OrangeSparks_SetParams: a gust, or flung away as they end) */
        embers_gust(s, v);
        return;
    }
    if (s == NULL || s->what != FLAME) {
        return;
    }
    s->command = v;
    if (s->kind == 0) {   /* the candle: on to its second half */
        s->p[0].frame = 15;
    } else if (s->kind == 1) {
        for (i = 0; i < (v == 2 ? 4 : 2); i++) {
            sparks_new(h, vec3(s->at.x, s->at.y + 1.5f, s->at.z), 1);
        }
    }
}

static void flame_step(System *s) {   /* EvEffect86_Update */
    Part *p = &s->p[0];

    s->timer--;
    if (s->kind != 0) {
        if (s->timer == 0) {
            s->timer = 1;
            if (++p->frame >= 16) {
                p->frame = 0;
            }
        }
        return;
    }
    if (s->timer == 0) {
        s->timer = 1;
        p->frame++;
        if (s->command == 0) {
            if (p->frame >= 16) {
                p->frame = 0;
            }
        } else {
            s->timer = 2;
            if (p->frame >= 32) {
                p->frame = 0;
                s->timer = 1;
                s->command = 0;
            }
        }
    }
    p->h = s->command == 2 ? (float)(((p->frame - 12) >> 2) + s->timer) * 0.125f : 1.0f;
}

/* ---- SpeckSwarm: specks buzzing about a centre; now and then (1 in 10) a new heading, pulled
 * back to the centre; hidden with the camera within 10 of it, dim within 20 ---- */

static void speck_heading(System *s, Part *p) {   /* up to 0.05 x the spread each way, pulled back by an eighth */
    float k = (float)s->top * 0.1f;

    p->v = vec3(half() * k, half() * k, half() * k);
    p->pull = vec3_scale(p->v, -0.125f);
}

int particles_swarm(Vec3 at, int count, float spread, uint32_t rgba) {
    int h = new_system(SWARM, at), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, 2, 0xEE, 0x4E, 4, 4, 1, 0x20);
    s->top = (int)spread;   /* (the spread) */
    s->n = clampi(count, 0, 16);
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        varied(p, rgba);
        p->alpha0 = p->rgba[3];
        p->at = vec3(at.x + half() * spread, at.y + half() * spread, at.z + half() * spread);
        square(p, rnd() * spread * 0.01f + 0.05f);
        p->turn = rnd_turn();
        speck_heading(s, p);
    }
    return h;
}

/* one axis: the speed pulled, the speck moved, the pull turned back once past the centre */
static void speck_axis(float *pos, float *vel, float *pull, float centre) {
    float d;

    *vel += *pull;
    *pos += *vel;
    d = *pos - centre;
    if (*pull < 0.0f ? d < 0.0f : d > 0.0f) {
        *pull = -*pull;
    }
}

static void swarm_step(System *s, Vec3 camera) {   /* SpeckSwarm_Update */
    Vec3 d = vec3_sub(camera, s->at);
    float d2 = d.x * d.x + d.y * d.y + d.z * d.z;
    int shift = d2 < 100.0f ? 8 : d2 < 400.0f ? 1 : 0, i;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        if (rnd() < 0.1f) {
            speck_heading(s, p);
        }
        speck_axis(&p->at.x, &p->v.x, &p->pull.x, s->at.x);
        speck_axis(&p->at.y, &p->v.y, &p->pull.y, s->at.y);
        speck_axis(&p->at.z, &p->v.z, &p->pull.z, s->at.z);
        p->turn = rnd_turn();
        p->rgba[3] = p->alpha0 >> shift;
    }
}

/* ---- Splash: specks thrown up and away from a character, each bouncing on the ground and
 * fading a quarter a bounce until gone ---- */

static void splash_up(Part *p) {   /* off the ground again: a little sideways, up 1..2 */
    p->v.x += 0.1f * half();
    p->v.y = 1.0f + rnd();
    p->v.z += 0.1f * half();
    p->pull = vec3(-0.01f * p->v.x, 0.0f, -0.01f * p->v.z);
}

int particles_splash(Vec3 at, Vec3 from, int count, float spread, uint32_t rgba) {
    int h = new_system(SPLASH, at), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, 2, 0xEE, 0x4E, 4, 4, 1, 0x20);
    s->n = clampi(count, 0, 10);
    s->floor = at.y;
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];
        Vec3 away;
        float len;

        varied(p, rgba);
        p->at = vec3(at.x + spread * half(), at.y, at.z + spread * half());
        square(p, 0.2f + 0.01f * spread * rnd());
        p->turn = rnd_turn();
        away = vec3_sub(p->at, from);   /* thrown away from the character, up 1..3 */
        len = sqrtf(away.x * away.x + away.y * away.y + away.z * away.z);
        len = len < 1e-6f ? 1e-6f : len;
        p->v = vec3(away.x / len * rnd(), 1.0f + 2.0f * rnd(), away.z / len * rnd());
        p->pull = vec3(-0.01f * p->v.x, 0.0f, -0.01f * p->v.z);
    }
    return h;
}

static int splash_step(System *s) {   /* Splash_Update: ended a tick after the last speck went */
    int i, seen = 0;

    if (s->ended) {
        return 0;
    }
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        if (p->rgba[3] <= 0) {
            continue;
        }
        seen = 1;
        p->v.y += p->v.y > 0.0f ? 0.5f * (-0.5f - rnd()) : 0.01f * (rnd() - 1.0f);   /* falling hard while rising */
        p->v.x += p->pull.x + 0.1f * half();
        p->v.z += p->pull.z + 0.1f * half();
        p->at = vec3_add(p->at, p->v);
        if (p->at.y < s->floor) {   /* on the ground: fainter, and up again */
            p->rgba[3] -= 0x20;
            if (p->rgba[3] < 0) {
                p->rgba[3] = 0;
            } else {
                splash_up(p);
            }
        }
    }
    s->ended = !seen;
    return 1;
}

/* ---- SpriteBurst (dust) ---- */

int particles_burst(Vec3 at, int kind, int r, int g, int b, int top) {
    int h = new_system(BURST, at), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    s->kind = kind;
    s->mark = r;
    s->top = top;
    s->n = 16;
    if (kind == 3) {   /* a puff: its own cell, palette 3; dark ones blended */
        look(s, 0x10, 1, 3, 0xC0, 0x40, 32, 32, 1, r != 0 ? 0 : 0x20);
    } else {           /* a cell of row 64, at column 0 or 32 */
        look(s, 0x10, 1, -1, irnd(2) * 32, 0x40, 32, 32, 1, 0);
    }
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];
        float dx = half(), dy = half(), dz = half();

        colour(p, r, g, b, irnd(64) + 0x10);
        square(p, 2.0f);
        switch (kind) {
        case 1:   /* motes swirling from a ring, starting all but unseen */
            p->rgba[3] = 1;
            p->at = vec3(at.x + 2.0f * dx, at.y, at.z + 2.0f * dz);
            square(p, 2.0f + 2.0f * rnd());
            p->turn = rnd_turn();
            p->v = vec3(0.05f * dx, 0.01f, 0.05f * dz);
            break;
        case 2: {   /* spreading out */
            float speed = 0.01f * (float)top;

            p->at = vec3(at.x + 4.0f * dx, at.y + dy, at.z + 4.0f * dz);
            p->v = vec3(speed * dx, 0.01f, speed * dz);
            break;
        }
        case 3: {   /* rising */
            int light = r != 0;
            float up = rnd(), big = light ? 2.0f : 3.0f;

            colour(p, 0x10, 0x10, 0x10, light ? 0x80 : 0x10);
            p->at = vec3(at.x + 6.0f * dx, at.y + 10.0f * up * (light ? 1.0f : 2.0f), at.z + 6.0f * dz);
            square(p, big + big * rnd());
            p->v = vec3(0.05f * dx, 0.1f + 0.3f * up, 0.05f * dz);
            break;
        }
        default:   /* drifting up */
            p->at = vec3(at.x + 4.0f * dx, at.y + 4.0f * dy, at.z + 4.0f * dz);
            p->v = vec3(0.01f * dx, 0.01f + 0.1f * (0.5f + dy), 0.01f * dz);
            break;
        }
    }
    return h;
}

static void fade(Part *p, int extra) {
    p->rgba[3] = clampi(p->rgba[3] - (irnd(4) + extra), 0, 255);
}
static void grow(Part *p, float by) { square(p, p->w + by); }

static int burst_step(System *s) {   /* SpriteBurst_Update: 0 once none is left */
    int i, going = 0;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        switch (s->kind) {
        case 1:
            if (p->w <= 0.0f || p->rgba[3] <= 0) {
                break;
            }
            going = 1;
            square(p, p->w - 0.005f * rnd() < 0.0f ? 0.0f : p->w - 0.005f * rnd());
            p->turn = wrap(p->turn + deg(0.5f * rnd()));
            p->at = vec3_add(p->at, p->v);
            if (p->rgba[0] == s->mark) {   /* brightening to the top; there its red drops: fading */
                p->rgba[3] += irnd(2) + 1;
                if (p->rgba[3] >= s->top) {
                    p->rgba[0] -= 1;
                    p->rgba[3] = s->top;
                }
            } else {
                p->rgba[3] = clampi(p->rgba[3] - irnd(2), 0, 255);
            }
            break;
        case 2:
            if (p->rgba[3] <= 0) {
                break;
            }
            going = 1;
            grow(p, 0.05f);
            p->turn = wrap(p->turn + (p->v.x <= 0.0f ? -1.0f : 1.0f) * PI / 360.0f);
            p->v.x *= 0.95f;
            p->v.z *= 0.95f;
            p->at = vec3_add(p->at, p->v);
            fade(p, 1);
            break;
        case 3:
            if (!(p->at.y < 80.0f)) {
                break;
            }
            going = 1;
            grow(p, 0.04f);
            p->turn = wrap(p->turn + PI / 360.0f);
            p->at = vec3_add(p->at, p->v);
            fade(p, 0);
            break;
        default:
            if (p->rgba[3] <= 0) {
                break;
            }
            going = 1;
            grow(p, 0.04f);
            p->turn = wrap(p->turn + PI / 360.0f);
            p->at = vec3_add(p->at, p->v);
            fade(p, 0);
            break;
        }
    }
    return going;
}

/* ---- Effect6FF60: shards - small random boxes thrown up from a point that tumble, fall and
 * bounce on its height until they settle ---- */

static void shard(System *s, Part *p) {   /* Effect6FF60_Shard */
    float dot = 0.0f, m;
    int c, k;

    for (c = 0; c < 8; c++) {   /* the box's corners: x - on odd, y - on 2 3 6 7, z - on 4..7 */
        for (k = 0; k < 3; k++) {
            p->corner[c][k] = (c & (1 << k)) ? -rnd() : rnd();
        }
    }
    for (k = 0; k < 3; k++) {
        p->scale[k] = 0.9f * rnd() + 0.1f;
    }
    for (k = 0; k < 3; k++) {
        p->tturn[k] = 6.0f * half();
    }
    p->at = vec3(s->at.x + 4.0f * half(), s->at.y + 8.0f * rnd(), s->at.z + 4.0f * half());
    for (k = 0; k < 3; k++) {
        p->spin[k] = 22.5f * rnd();
    }
    p->alive = 1;
    if (s->kind == 2 || s->kind == 3) {   /* bigger, higher, turning slower */
        for (k = 0; k < 3; k++) {
            p->scale[k] = rnd() + 0.5f;
        }
        p->at.y = s->at.y + 15.0f * rnd();
        for (k = 0; k < 3; k++) {
            p->spin[k] = 10.0f * rnd();
        }
    }
    /* the box's lopsidedness against its size: how hard it is thrown up */
    for (k = 0; k < 3; k++) {
        float sum = 0.0f;

        for (c = 0; c < 8; c++) {
            sum += (c & (1 << k)) ? -p->corner[c][k] : p->corner[c][k];
        }
        dot += sum * p->scale[k];
    }
    m = 21.0f - dot;
    p->v.x = half() * (s->kind == 0 || s->kind == 3 ? 0.4f : 1.0f);
    switch (s->kind) {
    case 4:
        p->v.y = m * -0.02f;
        p->v.z = half();
        break;
    case 0:
    case 3:
        p->v.y = m * 0.05f + 0.1f;
        p->v.z = 0.4f * half();
        break;
    default:
        p->v.y = m * 0.01f + 0.1f;
        p->v.z = half();
        break;
    }
}

int particles_shards(Vec3 at, uint32_t gs, int kind, int tex, float u, float v, float du, float dv, float floor) {
    int h = new_system(SHARDS, at), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    sprites_texture(s->batch, 0, tex, -1);   /* (the room's bank) */
    sprites_uv(s->batch, u, v, u + du, v + dv);
    sprites_flags(s->batch, SPRITE_OPAQUE, 0x19);
    s->colour = (gs & 0xFF) << 24 | (gs >> 8 & 0xFF) << 16 | (gs >> 16 & 0xFF) << 8 | (gs >> 24 & 0xFF);
    s->kind = kind;
    s->n = 16;
    for (i = 0; i < s->n; i++) {
        shard(s, &s->p[i]);
    }
    s->at.y += floor;   /* (thrown, the ground they bounce on raised) */
    return h;
}

static int shards_step(System *s) {   /* Effect6FF60_Update: 0 once all have settled */
    int i, k, any = 0;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        if (!p->alive) {
            continue;
        }
        any = 1;
        for (k = 0; k < 3; k++) {
            p->tturn[k] += deg(p->spin[k]);
            if (p->tturn[k] > PI) {
                p->tturn[k] -= 2.0f * PI;
            }
        }
        p->at = vec3_add(p->at, p->v);
        p->v.y -= 0.1f;
        if (p->at.y < s->at.y) {
            if (p->v.y < -0.5f) {   /* a bounce */
                p->at.y = s->at.y;
                p->v = vec3(0.5f * p->v.x, -0.3f * p->v.y, 0.5f * p->v.z);
            } else {
                p->alive = 0;
            }
        }
    }
    return any;
}

/* a point turned about x, then y, then z (sceVu0RotMatrixX/Y/Z) */
static Vec3 turn_xyz(Vec3 w, float tx, float ty, float tz) {
    float c = cosf(tx), s = sinf(tx), y, x;

    y = w.y * c - w.z * s;
    w.z = w.z * c + w.y * s;
    w.y = y;
    c = cosf(ty);
    s = sinf(ty);
    x = w.x * c + w.z * s;
    w.z = w.z * c - w.x * s;
    w.x = x;
    c = cosf(tz);
    s = sinf(tz);
    x = w.x * c - w.y * s;
    w.y = w.x * s + w.y * c;
    w.x = x;
    return w;
}

static void quad(Sprite *r, uint32_t rgba, const Vec3 c[4]) {
    int k;

    memset(r, 0, sizeof(*r));
    r->rgba[0] = (uint8_t)(rgba >> 24);
    r->rgba[1] = (uint8_t)(rgba >> 16);
    r->rgba[2] = (uint8_t)(rgba >> 8);
    r->rgba[3] = (uint8_t)rgba;
    r->own = 1;
    for (k = 0; k < 4; k++) {
        r->c[k][0] = c[k].x;
        r->c[k][1] = c[k].y;
        r->c[k][2] = c[k].z;
    }
}

static void shards_write(System *s) {   /* each box's six faces */
    static const uint8_t faces[6][4] = {{0, 1, 2, 3}, {1, 5, 3, 7}, {5, 4, 7, 6}, {4, 0, 6, 2}, {0, 1, 4, 5}, {6, 7, 2, 3}};
    Sprite *r = sprites_records(s->batch, -1);
    int i, c, f, k, n = 0;

    for (i = 0; i < s->n; i++) {
        const Part *p = &s->p[i];
        Vec3 world[8], q[4];

        if (!p->alive) {
            continue;
        }
        for (c = 0; c < 8; c++) {
            Vec3 w = vec3(p->corner[c][0] * p->scale[0], p->corner[c][1] * p->scale[1], p->corner[c][2] * p->scale[2]);

            world[c] = vec3_add(turn_xyz(w, p->tturn[0], p->tturn[1], p->tturn[2]), p->at);
        }
        for (f = 0; f < 6; f++) {
            for (k = 0; k < 4; k++) {
                q[k] = world[faces[f][k]];
            }
            quad(&r[n++], s->colour, q);
        }
    }
    sprites_records(s->batch, n);
}

/* ---- Effect79B00: falling stones - 32 pieces dropped from a height onto random floor
 * triangles, each after its own wait (some already partway down at the start); they tumble, fall
 * (the bigger the faster), bounce once off floors that let them and fade out on the ground, then
 * start over. Unseen within 10 of the camera, fading in over the next 10 ---- */

static void stone(System *s, Part *p, int first) {   /* Effect79B00_Piece */
    const NavMesh *n = &gEngine.room.nav;
    int tri = n->ntris > 0 ? (int)((float)n->ntris * rnd()) : -1, c, k;
    Vec3 at = tri >= 0 && tri < n->ntris ? navmesh_center(n, tri) : vec3(0, 0, 0);

    p->landed = 0;
    p->alpha = 0x80;
    p->v = vec3(0, 0, 0);
    p->floor = at.y;
    p->at = vec3(at.x, s->floor, at.z);
    p->delay = (int)(30.0f * rnd());
    if (first && rnd() < 0.6f) {   /* (already falling, from partway down) */
        p->delay = 0;
        p->at.y -= (s->floor - p->floor) * rnd();
        p->v.y = -1.0f - 2.0f * rnd();
    }
    p->bounce = !(tri >= 0 && tri < n->ntris && (n->tris[tri].flags & 0x10000000));
    for (c = 0; c < 8; c++) {   /* (a lopsided box: 0.6 or -0.4 by the corner's signs, less 0.2 x random) */
        for (k = 0; k < 3; k++) {
            p->corner[c][k] = ((c & (1 << k)) ? -0.4f : 0.6f) - 0.2f * rnd();
        }
    }
    for (k = 0; k < 3; k++) {
        p->scale[k] = 0.1f + 0.4f * rnd();
        p->tturn[k] = rnd_turn();
        p->spin[k] = 10.0f * rnd();
    }
    p->alive = 1;
}

int particles_stones(float height, uint32_t gs, int tex, float u, float v, float du, float dv) {
    int h = new_system(STONES, vec3(0, height, 0)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    sprites_texture(s->batch, 0, tex, -1);   /* (the room's bank) */
    sprites_uv(s->batch, u, v, u + du, v + dv);
    sprites_flags(s->batch, 0, 0x19);
    s->colour = (gs & 0xFF) << 24 | (gs >> 8 & 0xFF) << 16 | (gs >> 16 & 0xFF) << 8;
    s->floor = height;
    s->n = 32;
    for (i = 0; i < s->n; i++) {
        stone(s, &s->p[i], 1);
    }
    return h;
}

static void stones_step(System *s) {   /* Effect79B00_Update */
    int i, k;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        if (p->delay > 0) {
            p->delay--;
            continue;
        }
        for (k = 0; k < 3; k++) {
            p->tturn[k] = wrap(p->tturn[k] + deg(p->spin[k]));
        }
        p->at = vec3_add(p->at, p->v);
        p->v.y -= 0.1f + 8.0f * (p->scale[2] * 0.05f * p->scale[0] * p->scale[1]);
        if (p->landed) {
            if (p->alpha < 8) {
                stone(s, p, 0);
            } else {
                p->alpha -= 8;
            }
            continue;
        }
        if (p->at.y < p->floor) {
            p->landed = 1;
            if (p->bounce) {   /* thrown back up at a fifth of its fall, up to 54 degrees off upright */
                float up = 0.2f * fabsf(p->v.y), tilt = 0.3f * PI - 0.6f * PI * rnd(), head = rnd_turn();

                p->at.y = p->floor;
                p->v = vec3(sinf(head) * sinf(tilt) * up, cosf(tilt) * up, cosf(head) * sinf(tilt) * up);
            }
        }
    }
}

static void stones_write(System *s) {
    static const uint8_t faces[6][4] = {{0, 1, 2, 3}, {1, 5, 3, 7}, {5, 4, 7, 6}, {4, 0, 6, 2}, {0, 1, 4, 5}, {6, 7, 2, 3}};
    Sprite *r = sprites_records(s->batch, -1);
    int i, c, f, k, n = 0;

    for (i = 0; i < s->n; i++) {
        const Part *p = &s->p[i];
        Vec3 world[8], q[4];
        float d = vec3_len(vec3_sub(p->at, sCamera)), near = d < 10.0f ? 0.0f : fminf(0.1f * (d - 10.0f), 1.0f);
        int a = (int)((float)p->alpha * near);

        if (p->delay > 0 || a == 0) {
            continue;
        }
        for (c = 0; c < 8; c++) {
            Vec3 w = vec3(p->corner[c][0] * p->scale[0], p->corner[c][1] * p->scale[1], p->corner[c][2] * p->scale[2]);

            world[c] = vec3_add(turn_xyz(w, p->tturn[0], p->tturn[1], p->tturn[2]), p->at);
        }
        for (f = 0; f < 6; f++) {
            for (k = 0; k < 4; k++) {
                q[k] = world[faces[f][k]];
            }
            quad(&r[n++], s->colour | (uint32_t)a, q);
        }
    }
    sprites_records(s->batch, n);
}

/* ---- Debris: 16 pieces thrown from a 4 x 4 grid up near the water tower's top, turned by the
 * spot's heading; they tumble and fall (the bigger, the faster) until the last one is below 500.
 * Kinds 0..2 three spots, 3..5 the same with pieces a tenth the size ---- */

static void debris_piece(System *s, Part *p, int i, float head, int small) {   /* Debris_Piece */
    float x, y, z, c = cosf(head), sn = sinf(head), vy, vz;
    int k, q;

    for (q = 0; q < 8; q++) {   /* (0.7 or -0.3 by the corner's signs, less 0.4 x random) */
        for (k = 0; k < 3; k++) {
            p->corner[q][k] = ((q & (1 << k)) ? -0.3f : 0.7f) - 0.4f * rnd();
        }
    }
    for (k = 0; k < 3; k++) {
        p->scale[k] = (0.5f + 3.0f * rnd()) * (small ? 0.1f : 1.0f);
        p->tturn[k] = rnd_turn();
    }
    x = -5.0f + 3.33f * (float)(i % 4) + 2.0f - 4.0f * rnd();
    y = 1.0f + 2.0f * (float)(i / 4) + 1.0f - 2.0f * rnd();
    z = 1.0f + rnd();
    p->at = vec3(s->at.x + x * c + z * sn, s->at.y + y, s->at.z + z * c - x * sn);   /* (turned about y) */
    for (k = 0; k < 3; k++) {
        p->spin[k] = 5.0f * rnd();
    }
    vy = 0.2f + 0.2f * rnd();
    vz = 0.2f + 0.4f * rnd();
    p->v = vec3(vz * sn, vy, vz * c);
    p->alive = 1;
}

int particles_debris(int kind) {
    static const float spot[3][4] = {{1.5f, 620.0f, 77.0f, 0.0f}, {-56.5f, 620.0f, 52.0f, -0.663f}, {-5.0f, 620.0f, -77.0f, PI}};
    int k = kind % 3, h, i;
    System *s;

    if (kind < 0 || kind > 5) {
        return -1;
    }
    h = new_system(DEBRIS, vec3(spot[k][0], spot[k][1], spot[k][2]));
    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    sprites_texture(s->batch, 0, 4, -1);   /* (the room's bank) */
    sprites_uv(s->batch, 0.0f, 0.32f, 0.68f, 1.0f);
    sprites_flags(s->batch, SPRITE_OPAQUE, 0x19);
    s->colour = 0x14141680u;   /* (GS $80161414) */
    s->n = 16;
    for (i = 0; i < s->n; i++) {
        debris_piece(s, &s->p[i], i, spot[k][3], kind >= 3);
    }
    return h;
}

static int debris_step(System *s) {   /* Debris_Update: 0 once the last piece is below 500 */
    int i, k;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        for (k = 0; k < 3; k++) {
            p->tturn[k] = wrap(p->tturn[k] + deg(p->spin[k]));
        }
        p->at = vec3_add(p->at, p->v);
        p->v.y += -0.01f * p->scale[0] * p->scale[1] * p->scale[2] - 0.09f;
    }
    return s->n == 0 || s->p[s->n - 1].at.y >= 500.0f;
}

/* ---- RisingSmoke: 64 puffs rising from the kiln of castle 2F (56.5, 13, -1.4), dim purple,
 * growing and turning as they go, fading out; anew above 25 or faded ---- */

static void smoke_puff(Part *p, int again) {   /* RisingSmoke_Particle */
    int k = 0;

    colour(p, 0x50, 0x40, 0x50, 0x50);
    p->at.x = 56.5f + 1.1f * half();
    if (!again) {   /* (at the start: partway up already, part faded) */
        k = (int)(12.0f * rnd());
        p->rgba[3] = clampi(p->rgba[3] - 10 * k, 0, 0x50);
    }
    p->at.y = 13.0f + (float)k;
    p->at.z = -1.4f + 1.1f * half();
    square(p, 0.4f);
    p->turn = 0.0f;
    p->frame = 0;
    p->v = vec3(0.03f * half(), 0.07f + 0.1f * rnd(), 0.03f * half());
}

int particles_rising_smoke(void) {
    int h = new_system(SMOKE, vec3(56.5f, 13.0f, -1.4f)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0, 0x40, 0x20, 0x20, 1, 0);
    s->n = 64;
    for (i = 0; i < s->n; i++) {
        smoke_puff(&s->p[i], 0);
    }
    return h;
}

static void smoke_step(System *s) {   /* RisingSmoke_Update */
    int i;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        p->w += 0.05f;
        p->h += 0.05f;
        p->turn = wrap(p->turn + deg(3.0f));
        p->at = vec3_add(p->at, p->v);
        if (p->at.y >= 25.0f) {
            smoke_puff(p, 1);
        }
        if (p->rgba[3] > 0) {
            p->rgba[3]--;
        } else {
            smoke_puff(p, 1);
        }
    }
}

/* ---- DustShaft: 256 motes of dust drifting down a shaft of light in the old mansion (around
 * x 38, z -11, from up to 50 high - narrower the higher), growing and turning; every other frame
 * fading in to $21 and out again; anew below -3 or faded out ---- */

static void dust_mote(Part *p, int again) {   /* DustShaft_Mote */
    float h = rnd(), x = rnd();

    colour(p, 0x80, 0x80, 0x80, 0);
    p->at = vec3(38.0f + 16.0f * (1.0f - h) * (x - 0.5f), 50.0f * h, -11.0f + 4.0f * half());
    square(p, 2.0f);
    p->turn = 0.0f;
    p->frame = 0;
    p->v = vec3(0.05f * half(), -0.01f - 0.1f * rnd(), 0.05f * half());
    p->landed = 0;   /* (fading out) */
    if (!again) {
        p->rgba[3] = irnd(32);
        p->landed = irnd(2);
    }
}

int particles_dust_shaft(void) {
    int h = new_system(DUST, vec3(38.0f, 25.0f, -11.0f)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0x20, 0x40, 0x20, 0x20, 1, SPRITE_ADD);
    s->n = 256;
    for (i = 0; i < s->n; i++) {
        dust_mote(&s->p[i], 0);
    }
    return h;
}

static void dust_step(System *s) {   /* DustShaft_Update */
    int i;

    s->timer ^= 1;
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        p->w += 0.02f;
        p->h += 0.02f;
        p->turn = wrap(p->turn + deg(0.5f));
        p->at = vec3_add(p->at, p->v);
        if (p->at.y < -3.0f) {
            dust_mote(p, 1);
        } else if (p->at.y < 6.0f) {
            p->v.y *= 0.9f;
        }
        if (s->timer) {
            continue;
        }
        if (!p->landed) {
            if (++p->rgba[3] >= 0x21) {
                p->landed = 1;
            }
        } else if (p->rgba[3] > 0) {
            p->rgba[3]--;
        } else {
            dust_mote(p, 1);
        }
    }
}

/* ---- SpiralSmoke: 128 orange puffs spiralling up from the old mansion's fire (38.5, 0, -10),
 * growing, turning, spiralling out (by up to 10 degrees a frame, 0.05 wider) and rising, their
 * colour dimming a step a frame and their alpha every other frame, anew once faded; with a
 * flickering pinkish glow 32 across at (38, 15, -11) ---- */

static void spiral_puff(Part *p, int again) {   /* SpiralSmoke_Puff */
    float up = 0.0f;

    colour(p, 0xC0, 0x60, 0x40, irnd(8) + 0x24);
    square(p, 2.0f + rnd());
    if (!again) {   /* (part way up already, dimmer, bigger) */
        int k;

        up = rnd();
        for (k = 0; k < 4; k++) {
            p->rgba[k] = (int)((float)p->rgba[k] * (1.0f - up));
        }
        square(p, p->w + 2.5f * up);
    }
    p->at = vec3(38.5f, 50.0f * up, -10.0f);
    p->turn = deg(rnd());
    p->frame = 0;
    p->rise = 0.5f + rnd();                 /* its rise a frame */
    p->angle = 2.0f * PI * half();          /* round the spiral */
    p->radius = up + rnd();
}

int particles_spiral_smoke(void) {
    int h = new_system(SPIRAL, vec3(38.5f, 0.0f, -10.0f)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, 2, 0xC0, 0x40, 0x20, 0x20, 1, SPRITE_ADD);
    s->batch2 = sprites_new();
    if (s->batch2 >= 0) {
        sprites_texture(s->batch2, 0x10, 1, -1);
        sprites_cells(s->batch2, 0xA0, 0x40, 32, 32, 512, 256, 1);
        sprites_flags(s->batch2, SPRITE_ADD, 0x19);
    }
    s->n = 128;
    for (i = 0; i < s->n; i++) {
        spiral_puff(&s->p[i], 0);
    }
    s->top = 0x38;
    return h;
}

static void spiral_step(System *s) {   /* SpiralSmoke_Update */
    int i, k;

    s->timer ^= 1;
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        square(p, p->w + 0.05f);
        p->turn += 0.5f * deg(rnd());
        p->angle = wrap(p->angle + deg(10.0f * rnd()));
        p->radius += 0.05f;
        p->at = vec3(38.3f + p->radius * sinf(p->angle), p->at.y + p->rise, -10.0f + p->radius * cosf(p->angle));
        if (s->timer == 0 && --p->rgba[3] < 0) {
            spiral_puff(p, 1);
        }
        for (k = 0; k < 3; k++) {
            p->rgba[k] = p->rgba[k] > 0 ? p->rgba[k] - 1 : 0;
        }
    }
    s->top = s->top == 0x38 ? 0x40 : 0x38;   /* (the glow flickers) */
}

static void spiral_glow(System *s) {
    Sprite *r;

    if (s->batch2 < 0) {
        return;
    }
    r = sprites_records(s->batch2, 1);
    memset(r, 0, sizeof(*r));
    r->x = 38.0f;
    r->y = 15.0f;
    r->z = -11.0f;
    r->w = r->h = 32.0f;
    r->rgba[0] = 0x80;
    r->rgba[1] = 0x70;
    r->rgba[2] = 0x70;
    r->rgba[3] = (uint8_t)s->top;
}

/* ---- Drips: water dripping at five spots of the old mansion's flooded room ($52) - each turns
 * on (90 frames: its drip sound, the next of four) and off (90..345) at random; while on, a new
 * ripple each frame (a flat blue-grey quad spreading, turning, bobbing up and falling back; its
 * life 32 down 2 a frame, its alpha) ---- */

#define DRIP_SPOTS 5
#define DRIP_RIPPLES 16
static const float kDripSpots[DRIP_SPOTS][3] = {{30, 8, -44}, {46, 1, 30}, {0, 1, 40}, {-20, 1, 49}, {-50, 8, -46}};

static void ripple_reset(Part *p, int k) {   /* (life 0: unused) */
    p->at = vec3(kDripSpots[k][0], kDripSpots[k][1], kDripSpots[k][2]);
    p->alive = 0;
    p->radius = 0.1f;
    p->angle = 2.0f * PI * half();
    p->rise = 0.5f + 0.1f * rnd();
}

static void ripples_step(System *s, int k) {   /* Drips_Ripples */
    int j;

    for (j = 0; j < DRIP_RIPPLES; j++) {
        Part *p = &s->p[k * DRIP_RIPPLES + j];

        if (p->alive == 0) {
            continue;
        }
        p->alive -= 2;
        if (p->alive <= 0) {
            ripple_reset(p, k);
            continue;
        }
        p->radius += 1.0f;
        p->angle = wrap(p->angle + deg(rnd()));
        p->rise -= 0.05f;
        p->at.y += p->rise;
        if (p->at.y < kDripSpots[k][1] - 4.0f) {
            ripple_reset(p, k);
        }
    }
}

int particles_drips(void) {
    int h = new_system(DRIPS, vec3(0, 0, 0)), k, j, n;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0xE0, 0x60, 0x20, 0x20, 1, SPRITE_ADD | SPRITE_CORNERS);
    s->n = DRIP_SPOTS * DRIP_RIPPLES;
    s->mark = 0;   /* (the next of the four drip sounds) */
    for (k = 0; k < DRIP_SPOTS; k++) {
        Part *spot = &s->p[k * DRIP_RIPPLES];

        for (j = 0; j < DRIP_RIPPLES; j++) {
            ripple_reset(&s->p[k * DRIP_RIPPLES + j], k);
        }
        spot->landed = irnd(2);   /* (the spot on) */
        spot->bounce = -1;        /* (its sound) */
        if (spot->landed) {
            spot->delay = 0x5A;
            for (n = 0; n < 64; n++) {
                ripples_step(s, k);
            }
        } else {
            spot->delay = irnd(256) + 0x5A;
        }
    }
    return h;
}

static void drips_step(System *s) {   /* Drips_Update */
    int k, j;

    for (k = 0; k < DRIP_SPOTS; k++) {
        Part *spot = &s->p[k * DRIP_RIPPLES];
        Vec3 at = vec3(kDripSpots[k][0], kDripSpots[k][1], kDripSpots[k][2]);

        if (--spot->delay == 0) {
            spot->landed ^= 1;
            if (spot->landed) {
                spot->delay = 0x5A;
                spot->bounce = s->mark;
                engine_sound_at((uint32_t)(spot->bounce + 7) | 0x40000000u, 6, at);
                s->mark = (s->mark + 1) & 3;
            } else {
                spot->delay = irnd(256) + 0x5A;
            }
        }
        if (spot->landed) {
            if (spot->bounce != -1) {   /* (kept going) */
                engine_sound_at((uint32_t)(spot->bounce + 7) | 0xC0000000u, 6, at);
            }
            for (j = 0; j < DRIP_RIPPLES; j++) {
                if (s->p[k * DRIP_RIPPLES + j].alive == 0) {
                    s->p[k * DRIP_RIPPLES + j].alive = 0x20;
                    break;
                }
            }
        }
        ripples_step(s, k);
    }
}

static void drips_write(System *s) {   /* each live ripple a flat quad, its alpha its life */
    Sprite *r = sprites_records(s->batch, -1);
    int i, n = 0, k;

    for (i = 0; i < s->n; i++) {
        const Part *p = &s->p[i];
        float c = cosf(p->angle) * p->radius, sn = sinf(p->angle) * p->radius;
        static const float kCorner[4][2] = {{1, -1}, {1, 1}, {-1, -1}, {-1, 1}};
        Vec3 q[4];

        if (p->alive <= 0) {
            continue;
        }
        for (k = 0; k < 4; k++) {   /* (turned about y: x' = x c + z s, z' = z c - x s) */
            float x = kCorner[k][0], z = kCorner[k][1];

            q[k] = vec3(p->at.x + x * c + z * sn, p->at.y, p->at.z + z * c - x * sn);
        }
        quad(&r[n++], 0x20203000u | (uint32_t)p->alive, q);
    }
    sprites_records(s->batch, n);
}

/* ---- SplashRing: a ring spreading on water - flat, 16 sides, 0.2 wide, from radius 0.2 by
 * `grow` a frame; fading in by 16 a frame to its alpha, then out by 4; gone once faded ---- */

static int ring_new(int owner, Vec3 at, uint32_t rgba, float grow) {
    int h = new_system(RING, at);
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    sprites_texture(s->batch, -1, 0, -1);   /* (untextured) */
    sprites_flags(s->batch, SPRITE_ADD | SPRITE_CORNERS, 0x19);
    s->owner = owner;
    s->engines = 1;
    s->n = 0;
    s->colour = rgba & 0xFFFFFF00u;
    s->top = (int)(rgba & 0xFF);   /* (the alpha it fades in to) */
    s->mark = 0;                   /* (its alpha now) */
    s->command = 1;                /* (fading in) */
    s->floor = 0.2f;               /* (its radius) */
    s->c.x = grow;
    return h;
}

static int ring_step(System *s) {   /* SplashRing_Update */
    if (s->command) {
        s->mark += 0x10;
        if (s->mark > s->top) {
            s->mark = s->top;
            s->command = 0;
        }
    } else {
        if (s->mark < 4) {
            return 0;
        }
        s->mark -= 4;
    }
    s->floor += s->c.x;
    return 1;
}

static void ring_write(System *s) {   /* SplashRing_Draw: a quad a side, outer and inner edge */
    Sprite *r = sprites_records(s->batch, -1);
    float r0 = s->floor, r1 = s->floor - 0.2f;
    int i;

    for (i = 0; i < 16; i++) {
        float a = deg(22.5f * (float)i), b = deg(22.5f * (float)(i + 1));
        Vec3 q[4] = {vec3(s->at.x + r0 * sinf(a), s->at.y, s->at.z - r0 * cosf(a)), vec3(s->at.x + r0 * sinf(b), s->at.y, s->at.z - r0 * cosf(b)),
                     vec3(s->at.x + r1 * sinf(a), s->at.y, s->at.z - r1 * cosf(a)), vec3(s->at.x + r1 * sinf(b), s->at.y, s->at.z - r1 * cosf(b))};

        quad(&r[i], s->colour | (uint32_t)s->mark, q);
    }
    sprites_records(s->batch, 16);
}

/* ---- DropletSpray: up to 16 droplets fanned round a point, rising and falling under gravity,
 * thrown out at their speed, fading by one a frame; it ends once one has faded or falls faster
 * than 10 ---- */

typedef struct Spray {
    Vec3 at;
    uint32_t rgba;
    int n;
    float size, size_rnd, dist, dist_rnd, rise, rise_rnd, speed, speed_rnd, lift, gravity;
} Spray;

static int droplets_new(int owner, const Spray *sp) {
    int h = new_system(DROPLETS, sp->at), i;
    float share;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0x6C, 0x4C, 8, 8, 1, SPRITE_ADD | SPRITE_GLOW);
    s->owner = owner;
    s->engines = 1;
    s->n = sp->n < 0 ? 0 : sp->n > 16 ? 16 : sp->n;
    s->floor = sp->gravity;
    share = 360.0f / (float)(s->n > 0 ? s->n : 1);
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];
        float a = wrap(deg(0.3f * share - 2.0f * 0.3f * share * rnd() + (float)i * share)), d = sp->dist + sp->dist_rnd * rnd();

        colour(p, (int)(sp->rgba >> 24), (int)(sp->rgba >> 16 & 0xFF), (int)(sp->rgba >> 8 & 0xFF), (int)(sp->rgba & 0xFF));
        p->at = vec3(sp->at.x + d * sinf(a), sp->at.y + sp->lift, sp->at.z + d * cosf(a));
        square(p, sp->size + sp->size_rnd * rnd());
        p->turn = 2.0f * PI * half();
        p->frame = 0;
        p->rise = sp->rise + sp->rise_rnd * rnd();
        p->radius = sp->speed + sp->speed_rnd * rnd();
        p->angle = a;
    }
    return h;
}

static int droplets_step(System *s) {   /* DropletSpray_Update */
    int i, going = 1;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        p->rise -= s->floor;
        p->at = vec3(p->at.x + p->radius * sinf(p->angle), p->at.y + p->rise, p->at.z + p->radius * cosf(p->angle));
        if (--p->rgba[3] == 0 || p->rise < -10.0f) {
            going = 0;
        }
    }
    return going;
}

/* ---- DropletFlash (Riccardo_Debris: the debris where his shot strikes): 32 droplets
 * in a colour flung out from a point - the bigger the slower, three times as fast up or down -
 * each slowed by its own pull and out once it has stopped rising or falling; they flicker; a
 * flash at the point on the first frame ---- */
int particles_drop_flash(Vec3 at, int r, int g, int b) {
    int h = new_system(DROPFLASH, at), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0x6C, 0x4C, 8, 8, 1, SPRITE_ADD);
    s->engines = 1;
    s->batch2 = sprites_new();
    if (s->batch2 >= 0) {   /* (the flash) */
        sprites_texture(s->batch2, 0x10, 1, -1);
        sprites_cells(s->batch2, 0x40, 0x40, 32, 32, 512, 256, 1);
        sprites_flags(s->batch2, 0, 0x19);
    }
    s->n = 32;
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];
        float speed, k;

        colour(p, clampi(r, 0, 0x7F), clampi(g, 0, 0x7F), clampi(b, 0, 0x7F), irnd(0x20) + 0x60);
        p->at = at;
        square(p, 0.2f + 0.4f * rnd());
        p->turn = 0.0f;
        p->frame = 0;
        speed = 8.0f - 10.0f * p->w;
        p->v = vec3(speed * half(), 3.0f * speed * half(), speed * half());
        k = 0.2f + 0.3f * rnd();
        p->pull = vec3_scale(p->v, -k);
    }
    s->timer = 0;
    return h;
}

static int drop_flash_step(System *s) {   /* DropletFlash_Update */
    int i, left = 0;

    s->timer++;
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        if (p->rgba[3] <= 0) {
            continue;
        }
        left = 1;
        p->rgba[3] = irnd(0x80) + 1;
        p->v = vec3_add(p->v, p->pull);
        if (p->pull.y > 0.0f ? p->v.y > -0.01f : p->v.y < 0.01f) {   /* (stopped rising or falling) */
            p->rgba[3] = 0;
        }
        p->at = vec3_add(p->at, p->v);
    }
    return left;
}

static void drop_flash_write(System *s) {
    Sprite *r;

    write(s);
    if (s->batch2 < 0) {
        return;
    }
    if (s->timer > 1) {   /* (the flash: the first frame only) */
        sprites_records(s->batch2, 0);
        return;
    }
    r = sprites_records(s->batch2, 1);
    memset(r, 0, sizeof(*r));
    r->x = s->at.x;
    r->y = s->at.y;
    r->z = s->at.z;
    r->w = r->h = 2.0f;
    r->rgba[0] = 0x80;
    r->rgba[1] = 0x50;
    r->rgba[2] = 0x40;
    r->rgba[3] = 0x80;
}

/* ---- BoneSmoke (StrikeMark kind 2 -> BoneSmoke: the smoke off Riccardo's gun as it fires):
 * sixteen wisps from a point, one more showing each of the first 16 frames; each 0.4..0.6 wide
 * and 1.2 tall, drifting (up to 0.015 a frame) and rising 0.02, growing up to 0.04 a frame,
 * fading in by 3..6 to 0x10 then out by 0..1 every other frame; done once none shows ---- */
static void bone_smoke_wisp(Part *p, Vec3 at) {   /* BoneSmoke_Wisp */
    colour(p, 0x80, 0x80, 0x80, 0);
    p->at = at;
    p->w = 0.4f + 0.2f * rnd();
    p->h = 1.2f;
    p->turn = 0.0f;
    p->frame = 0;
    p->v = vec3(0.03f * half(), 0.0f, 0.03f * half());
}
int particles_bone_smoke(Vec3 at) {
    int h = new_system(BONESMOKE, at), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, irnd(2) * 32, 0x40, 32, 32, 1, SPRITE_ADD);
    s->engines = 1;
    s->n = 16;
    for (i = 0; i < s->n; i++) {
        bone_smoke_wisp(&s->p[i], at);
    }
    s->timer = 0;
    return h;
}

static int bone_smoke_step(System *s) {   /* BoneSmoke_Update */
    int i, showing = 0;

    s->slow ^= 1;
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        if (p->rgba[3] <= 0) {
            continue;
        }
        showing = 1;
        p->w += 0.04f * rnd();
        p->h += 0.04f * rnd();
        p->at = vec3(p->at.x + p->v.x, p->at.y + 0.02f, p->at.z + p->v.z);
        if (p->rgba[0] == 0x80) {   /* (fading in; at 0x10 marked, its red one less) */
            p->rgba[3] += irnd(4) + 3;
            if (p->rgba[3] >= 0x10) {
                p->rgba[0]--;
                p->rgba[3] = 0x10;
            }
        } else if (s->slow == 0) {
            p->rgba[3] = clampi(p->rgba[3] - irnd(2), 0, 0x80);
        }
    }
    if (s->timer < 16) {   /* (one more shown, at the point) */
        showing = 1;
        bone_smoke_wisp(&s->p[s->timer], s->at);
        s->p[s->timer].rgba[3] = 1;
    }
    s->timer++;
    return showing;
}

/* ---- LorenzoSpark (Lorenzo2_BlowSparks: at his blows' key, seven of them about his hand): two
 * white sparks at a point, turned at random, rising 0.1 x `rise` and drifting (0.1 / 0.05 at
 * most); each frame growing (growth - 1) x size, turning up to 3 degrees the way it drifts,
 * fading 128 / life and dimming by 1 - out once black ---- */
int particles_spark2(Vec3 at, float rise, float size, float growth, int life) {
    int h = new_system(SPARK2, at), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, 9, 0x180, 0x80, 32, 32, 1, SPRITE_ADD);
    s->engines = 1;
    s->n = 2;
    s->floor = (growth - 1.0f) * size;              /* (its growth a frame) */
    s->rest = life != 0 ? 0x80 / life : 1;          /* (its fade a frame) */
    if (s->rest == 0) {
        s->rest = 1;
    }
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];
        float drift = i == 0 ? 0.2f : 0.1f;

        colour(p, 0x80, 0x80, 0x80, 0x80);
        p->at = at;
        square(p, size);
        p->turn = deg(360.0f * half());
        p->frame = 0;
        p->v = vec3(drift * half(), 0.1f * rise, drift * half());
    }
    return h;
}

static int spark2_step(System *s) {   /* LorenzoSpark_Update */
    int i, k, left = 0;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        if (p->rgba[3] <= 0) {
            continue;
        }
        left = 1;
        square(p, p->w + s->floor);
        p->turn += (p->v.x <= 0.0f ? -1.0f : 1.0f) * deg(3.0f * rnd());
        p->at = vec3_add(p->at, p->v);
        p->rgba[3] = clampi(p->rgba[3] - s->rest, 0, 0x80);
        for (k = 0; k < 3; k++) {
            p->rgba[k] = clampi(p->rgba[k] - 1, 0, 0x80);
        }
        if (p->rgba[0] + p->rgba[1] + p->rgba[2] == 0) {
            p->rgba[3] = 0;
        }
    }
    return left;
}

/* ---- LightRing (where Lorenzo's other class sinks away or comes up): a ring of light spreading
 * from radius 3 - 4 a frame, slowing (by 2 to 0.2, then by 0.01 to 0.1), fading by 7, its width
 * 7 / 4.5 / 3 x its alpha's share (here one alpha across it, half its own: the original's runs
 * from that at its inner edge to clear) - and 16 sparks about the point, 10..20 up, spiralling
 * and moving 0.1..0.35 a frame down (`rising`: up, and no ring), flickering, fading by 1..4;
 * done once no spark is left ---- */
int particles_sink_ring(Vec3 at, int rising) {
    int h = new_system(SINKRING, at), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0x80, 0, 32, 32, 4, SPRITE_ADD);
    s->engines = 1;
    s->batch2 = sprites_new();
    if (s->batch2 >= 0) {   /* (the ring: 16 x 16 texels at (200, 70)) */
        sprites_texture(s->batch2, 0x10, 1, -1);
        sprites_cells(s->batch2, 200, 70, 16, 16, 512, 256, 1);
        sprites_flags(s->batch2, SPRITE_ADD | SPRITE_CORNERS, 0x19);
    }
    s->c = rising ? at : vec3(at.x, at.y + 5.0f, at.z);   /* (the ring's centre) */
    s->floor = 3.0f;                                     /* (its radius) */
    s->tilt = rising ? -1.0f : 4.0f;                      /* (its speed) */
    s->mark = 0x80;                                      /* (its alpha) */
    s->head = 0.0f;                                      /* (its width) */
    s->n = 16;
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        colour(p, 0x80, 0x80, 0x80, irnd(0x80));
        p->at = vec3(at.x + 2.0f * half(), at.y + 20.0f - 10.0f * rnd(), at.z + 2.0f * half());
        square(p, 0.2f + 0.4f * rnd());
        p->turn = deg(360.0f * half());
        p->frame = irnd(4);
        p->angle = deg(360.0f * half());                  /* (its spin) */
        p->rise = (rising ? -1.0f : 1.0f) * (0.1f + 0.25f * rnd());
    }
    return h;
}

static int sink_ring_step(System *s) {   /* LightRing_Update */
    int i, left = 0;

    if (s->tilt <= 0.0f) {
        s->mark = 0;
    } else {
        s->head = (s->mark == 0x80 ? 7.0f : s->mark >= 0x79 ? 4.5f : 3.0f) * ((float)s->mark / 128.0f);
        s->floor += s->tilt;
        if (s->tilt <= 0.2f) {
            s->tilt = fmaxf(0.1f, s->tilt - 0.01f);
        } else {
            s->tilt = fmaxf(0.2f, s->tilt - 2.0f);
        }
        s->mark -= 7;
    }
    if (s->mark < 0) {
        s->mark = 0;
    }
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        if (p->rgba[3] <= 0) {
            continue;
        }
        left = 1;
        square(p, 0.2f + 0.4f * rnd());
        p->turn = deg(360.0f * half());
        p->angle = wrap(p->angle + deg(90.0f * rnd()));
        p->at = vec3(p->at.x + 0.05f * sinf(p->angle), p->at.y - p->rise, p->at.z + 0.05f * cosf(p->angle));
        p->rgba[3] = clampi(p->rgba[3] - (irnd(4) + 1), 0, 0x80);
        p->frame = irnd(4);
    }
    return left;
}

static void sink_ring_write(System *s) {
    Sprite *r;
    float r0 = s->floor, r1 = s->floor - s->head;
    int i;

    write(s);
    if (s->batch2 < 0) {
        return;
    }
    if (s->mark == 0) {
        sprites_records(s->batch2, 0);
        return;
    }
    r = sprites_records(s->batch2, 16);
    for (i = 0; i < 16; i++) {
        float a = deg(22.5f * (float)i), b = deg(22.5f * (float)(i + 1));
        Vec3 q[4] = {vec3(s->c.x + r0 * sinf(a), s->c.y, s->c.z - r0 * cosf(a)), vec3(s->c.x + r0 * sinf(b), s->c.y, s->c.z - r0 * cosf(b)),
                     vec3(s->c.x + r1 * sinf(a), s->c.y, s->c.z - r1 * cosf(a)), vec3(s->c.x + r1 * sinf(b), s->c.y, s->c.z - r1 * cosf(b))};

        quad(&r[i], 0x80808000u | (uint32_t)(s->mark / 2), q);
    }
}

/* ---- OneDrip: one drop at a time falling from 30 high in the Chaos Forest (-226..-234,
 * -100..-108), after a wait of 1..4 s; where it lands, a ring and a droplet ---- */

static void onedrip_drop(System *s) {   /* OneDrip_Drop */
    Part *p = &s->p[0];

    colour(p, 0x20, 0x20, 0x20, 0x70);
    p->at = vec3(-226.0f - 8.0f * rnd(), 30.0f, -100.0f - 8.0f * rnd());
    square(p, 0.5f);
    p->turn = 2.0f * PI * half();
    p->frame = 0;
    p->rise = 1.5f + rnd();               /* (its fall a frame) */
    p->delay = (int)(90.0f * rnd()) + 30;
}

int particles_one_drip(void) {
    int h = new_system(ONEDRIP, vec3(-230.0f, 30.0f, -104.0f));
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0x6C, 0x4C, 8, 8, 1, SPRITE_ADD | SPRITE_GLOW);
    s->n = 1;
    onedrip_drop(s);
    return h;
}

static void onedrip_step(int self, System *s) {   /* OneDrip_Update */
    Part *p = &s->p[0];

    if (p->delay > 0) {
        p->delay--;
        return;
    }
    p->at.y -= p->rise;
    if (p->at.y < 0.0f) {
        Vec3 at = vec3(p->at.x, 0.1f, p->at.z);
        Spray sp = {at, 0x20202030u, 1, 0.4f, 0.2f, 0, 0, 0.3f, 0.2f, 0, 0, 0.5f, 0.1f};

        ring_new(self, at, 0x40404030u, 0.15f);
        droplets_new(self, &sp);
        onedrip_drop(s);
    }
}

/* ---- a footstep in water (Character_WaterStep): a ring spreading on the surface (0.1 above
 * it) and 16 droplets thrown up round it - `big` (running, a fall) the larger. The engine's:
 * they go by themselves ---- */

void particles_water_step(Vec3 at, int big) {
    Vec3 ring = vec3(at.x, at.y + 0.1f, at.z);
    float s = big ? 0.25f : 0.2f, r = big ? 0.15f : 0.1f;
    Spray sp = {at, 0x20202040u, 16, s, s, 0.1f, 0.2f, s, r, s, r, 0.1f, 0.1f};

    ring_new(-1, ring, 0x30303030u, big ? 0.2f : 0.15f);
    droplets_new(-1, &sp);
}

/* ---- RisingMotes: 128 motes rising from around (0, 70, 35) in castle 1F's hall, growing and
 * turning; renewed past 130 high or, every other frame, once faded ---- */

static void motes_mote(Part *p, int again) {   /* RisingMotes_Mote */
    int up = 0;

    colour(p, 0x80, 0x80, 0x80, 0x38);
    p->at.x = 4.0f * half();
    square(p, 1.0f);
    if (!again) {   /* (somewhere up the column already: bigger, fainter) */
        up = (int)(60.0f * rnd());
        p->rgba[3] = clampi(p->rgba[3] - (up >> 1), 0, 0x38);
        square(p, 1.0f + 0.04f * (float)up);
    }
    p->at.y = 70.0f + (float)up;
    p->at.z = 35.0f + 4.0f * half();
    p->turn = 0.0f;
    p->frame = 0;
    p->v = vec3(0.03f * half(), 0.07f + 0.4f * rnd(), 0.03f * half());
}

int particles_rising_motes(void) {
    int h = new_system(MOTES, vec3(0, 70, 35)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0, 0x40, 0x20, 0x20, 1, 0);
    s->n = 128;
    for (i = 0; i < s->n; i++) {
        motes_mote(&s->p[i], 0);
    }
    return h;
}

static void motes_step(System *s) {   /* RisingMotes_Update */
    int i;

    s->timer ^= 1;
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        square(p, p->w + 0.04f);
        p->turn = wrap(p->turn + deg(3.0f));
        p->at = vec3_add(p->at, p->v);
        if (p->at.y >= 130.0f) {
            motes_mote(p, 1);
        }
        if (s->timer == 0) {
            if (p->rgba[3] > 0) {
                p->rgba[3]--;
            } else {
                motes_mote(p, 1);
            }
        }
    }
}

/* ---- OrangeSparks: 32 embers drifting up from castle 1F's fireplace (276.5, 6, -145.9), fading;
 * renewed once faded, falling back or 50 high - unless they are ending (a gust that flings them
 * away, `particles_command` 1: they just go out). Command 0: a gust that blows them back ---- */

static void ember(Part *p, int again) {   /* OrangeSparks_Spark */
    int up = 0;

    p->v = vec3(0.1f * half(), 0.05f + 0.5f * rnd(), 0.1f * half());
    colour(p, 0x80, 0x58, 0x10, 0x80);
    p->at.x = 276.5f + 4.0f * half();
    if (!again) {   /* (somewhere up already: fainter, slower) */
        up = (int)(10.0f * rnd());
        p->rgba[3] = clampi(p->rgba[3] - (up << 4), 0, 0x80);
        p->v.y -= 0.01f * (float)up;
        if (p->v.y < 0.0f) {
            p->v.y = 0.1f;
        }
    }
    p->at.y = 6.0f + (float)up;
    p->at.z = -145.9f + 4.0f * half();
    square(p, 1.0f);
    p->turn = 0.0f;
    p->frame = 0;
}

int particles_embers(int ending) {
    int h = new_system(EMBERS, vec3(276.5f, 6.0f, -145.9f)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0x60, 0x40, 0x20, 0x20, 1, SPRITE_ADD);
    s->n = 32;
    for (i = 0; i < s->n; i++) {
        ember(&s->p[i], 0);
    }
    if (ending >= 0) {
        embers_gust(s, ending);
    }
    return h;
}

static void embers_gust(System *s, int ending) {
    int i;

    s->command = ending;
    for (i = 0; i < s->n; i++) {
        Vec3 *v = &s->p[i].v;

        if (!ending) {   /* (blown back: x -0.2, z -0.3 at least, and up) */
            v->x = fmaxf(v->x - 0.2f, -0.2f);
            v->y = fmaxf(v->y + 0.01f, 0.1f);
            v->z = fmaxf(v->z - 0.3f, -0.3f);
        } else {
            v->x *= 2.0f;
            v->y += 0.01f;
            v->z *= 3.0f;
        }
    }
}

static int ember_spent(System *s, Part *p) {   /* out when ending, else renewed */
    if (s->command) {
        p->rgba[3] = 0;
        return 0;
    }
    ember(p, 1);
    return 1;
}

static int embers_step(System *s) {   /* OrangeSparks_Update: 0 once all are out */
    int i, done = 1;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        p->v.x += 0.01f;
        p->v.y -= 0.01f;
        if (p->v.y < 0.0f && ember_spent(s, p)) {
            done = 0;
        }
        p->v.z += 0.01f;
        p->at = vec3_add(p->at, p->v);
        if (p->at.y >= 50.0f && ember_spent(s, p)) {
            done = 0;
        }
        if (p->rgba[3] > 0) {
            done = 0;
            p->rgba[3] -= 4;
        } else if (ember_spent(s, p)) {
            done = 0;
        }
    }
    return !done;
}

/* ---- DriftingFlecks: 64 flat flecks drifting across an area (its half width and depth, placed and
 * turned), each turned the way it drifts, its drift nudged now and then; once out of the area a
 * fleck fades; it ends when all have ---- */

int particles_flecks(Vec3 at, float half_w, float depth, Vec3 turn) {
    int h = new_system(FLECKS, at), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    sprites_texture(s->batch, 0x10, 0, 2);
    sprites_cells(s->batch, 0x80, 0, 0x40, 0x40, 512, 256, 1);
    sprites_flags(s->batch, SPRITE_CORNERS, 0x19);
    s->c = vec3(half_w, depth, 0);
    s->wander = vec3(deg(turn.x), deg(turn.y), deg(turn.z));
    s->n = 64;
    for (i = 0; i < s->n; i++) {   /* DriftingFlecks_Start */
        Part *p = &s->p[i];

        p->at = vec3(4.0f * half(), 0, 0);
        p->v = vec3(0.05f * p->at.x * rnd(), 0, 0.1f + 0.2f * rnd());
        colour(p, 0x80, 0x80, 0x80, 0x80);
        p->w = 0.4f + 0.4f * rnd();
        p->h = 0.4f + 0.4f * rnd();
        p->delay = 8;
    }
    return h;
}

static int flecks_step(System *s) {   /* DriftingFlecks_Update: 0 once all are out and faded */
    int i, any = 0;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        if (p->delay != 0) {
            p->delay--;
        } else {
            if (irnd(4) == 0) {
                p->delay = (irnd(256) + 4) & 0xF;
            }
            p->v.x += 0.1f * half();
            p->v.z += 0.1f * half();
        }
        p->at = vec3_add(p->at, p->v);
        if (p->at.x < s->c.x && p->at.x > -s->c.x && p->at.z < s->c.y && p->at.z >= 0.0f) {
            any = 1;
        } else if (p->rgba[3] != 0) {
            p->rgba[3] -= 0x30;
            if (p->rgba[3] > 0) {
                any = 1;
            } else {
                p->rgba[3] = 0;
            }
        }
    }
    return any;
}

static void flecks_write(System *s) {   /* DriftingFlecks_Draw */
    static const float kCorner[4][2] = {{1, -1}, {1, 1}, {-1, -1}, {-1, 1}};
    Sprite *r = sprites_records(s->batch, -1);
    int i, k, n = 0;

    for (i = 0; i < s->n; i++) {
        const Part *p = &s->p[i];
        float a = 0.0f;
        Vec3 q[4];

        if (p->rgba[3] <= 0) {
            continue;
        }
        if (p->v.z != 0.0f) {   /* (turned the way it drifts) */
            a = atanf(p->v.x / p->v.z) - PI / 2.0f + (p->v.z > 0.0f ? 0.0f : PI);
        }
        for (k = 0; k < 4; k++) {
            float x = kCorner[k][0] * p->w, z = kCorner[k][1] * p->h, c = cosf(a), sn = sinf(a);
            Vec3 w = vec3(x * c + z * sn + p->at.x, 0.0f, z * c - x * sn + p->at.z);

            q[k] = vec3_add(turn_xyz(w, s->wander.x, s->wander.y, s->wander.z), s->at);
        }
        quad(&r[n++], 0x80808000u | (uint32_t)p->rgba[3], q);
    }
    sprites_records(s->batch, n);
}

/* ---- SmokePuffs: 64 puffs of smoke rising from around (15.5, 11, 22.5) in castle B1, growing and
 * turning; from 20 high fading (every other frame), renewed once gone or 22 high ---- */

static void chimney_puff(Part *p, int again) {   /* SmokePuffs_Puff */
    int up = 0;

    colour(p, 0x80, 0x80, 0x80, 0x20);
    p->at.x = 15.5f + 1.1f * half();
    square(p, 0.3f);
    if (!again) {   /* (somewhere up already: bigger, fainter) */
        up = (int)(12.0f * rnd());
        p->rgba[3] = clampi(p->rgba[3] - up, 0, 0x20);
        square(p, 0.3f * (float)up);
    }
    p->at.y = 11.0f + (float)up;
    p->at.z = 22.5f + 1.1f * half();
    p->turn = 0.0f;
    p->frame = 0;
    p->v = vec3(0.03f * half(), 0.04f + 0.05f * rnd(), 0.03f * half());
}

int particles_smoke_puffs(void) {
    int h = new_system(CHIMNEY, vec3(15.5f, 11.0f, 22.5f)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0, 0x40, 0x20, 0x20, 1, 0);
    s->n = 64;
    for (i = 0; i < s->n; i++) {
        chimney_puff(&s->p[i], 0);
    }
    return h;
}

static void chimney_step(System *s) {   /* SmokePuffs_Update */
    int i;

    s->timer ^= 1;
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        square(p, p->w + 0.02f);
        p->turn = wrap(p->turn + deg(1.0f));
        p->at = vec3_add(p->at, p->v);
        if (p->at.y >= 22.0f) {
            chimney_puff(p, 1);
        }
        if (p->rgba[3] > 0) {
            if (p->at.y >= 20.0f && s->timer != 0) {
                p->rgba[3] = p->rgba[3] > 4 ? p->rgba[3] - 4 : 0;
            }
        } else {
            chimney_puff(p, 1);
        }
    }
}

/* ---- WallShadow: a soft shadow on the old mansion's wall at x 127 (z -114 .. -130, y 1 .. 26) -
 * eight black quads stepping 0.1 off the wall, alpha $80 down to $10 ---- */

int particles_wall_shadow(void) {
    int h = new_system(WALLSHADOW, vec3(127.0f, 13.5f, -122.0f));

    if (h < 0) {
        return -1;
    }
    sprites_texture(sSys[h].batch, -1, 0, -1);   /* (untextured) */
    sprites_flags(sSys[h].batch, SPRITE_CORNERS, 0x1E);
    return h;
}

static void wall_shadow_write(System *s) {
    Sprite *r = sprites_records(s->batch, 8);
    int k;

    for (k = 0; k < 8; k++) {
        float x = 127.0f - 0.1f * (float)k;
        Vec3 q[4] = {vec3(x, 26, -114), vec3(x, 26, -130), vec3(x, 1, -114), vec3(x, 1, -130)};

        quad(&r[k], (uint32_t)((8 - k) << 4), q);
    }
}

/* ---- Fire: a fire of 64 flames in the old mansion's B1 ($66) at one of its spots, small or big:
 * each flame rising, growing, turning, fading in (to $60 small, $40 big) and out again; one
 * spent flame a frame lit again until it is put out (`particles_command` -1); it ends once none
 * shows ---- */

static const float kFireSmall[4][2] = {{-170.0f, 19.7f}, {15.5f, -40.0f}, {-19.5f, -4.5f}, {-59.5f, -110.0f}};
static const float kFireBig[4][2] = {{75.0f, 108.0f}, {-108.0f, 80.0f}, {-80.0f, -78.0f}, {-140.0f, -148.0f}};

static void flame_part(System *s, Part *p, int hidden) {   /* Fire_Flame */
    float a = half(), b = half(), c = half();

    p->landed = 0;   /* (fading in) */
    if (!s->kind) {   /* (small: within 7.5 across, 3..10 big, rising 0.02 .. 0.07) */
        colour(p, 0x46, 0x2B, 0x1E, hidden ? 0 : 0x10);
        p->at = vec3(s->at.x + 15.0f * a, 2.0f * b, s->at.z + 15.0f * c);
        square(p, 3.0f + 7.0f * rnd());
        p->v = vec3(0.05f * a, 0.02f + 0.05f * (0.5f + b), 0.05f * c);
    } else {   /* (big: 12 up, within 5 across, 3..6 big, rising 0.02 .. 0.12) */
        colour(p, 0x46, 0x30, 0x1A, hidden ? 0 : 0x10);
        p->at = vec3(s->at.x + 10.0f * a, 12.0f + 20.0f * b, s->at.z + 10.0f * c);
        square(p, 3.0f + 3.0f * rnd());
        p->v = vec3(0.05f * a, 0.02f + 0.1f * (0.5f + b), 0.05f * c);
    }
    p->turn = 0.0f;
    p->frame = 0;
}

int particles_fire(int spot, int big) {
    const float (*spots)[2] = big ? kFireBig : kFireSmall;
    int k = spot & 3, h = new_system(FIRE, vec3(spots[k][0], 0.0f, spots[k][1])), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0x20, 0x40, 0x20, 0x20, 1, 0);
    s->kind = big != 0;
    s->n = 64;
    for (i = 0; i < s->n; i++) {
        flame_part(s, &s->p[i], i >= (big ? 32 : 16));
    }
    return h;
}

static int fire_step(System *s) {   /* Fire_Update: 0 once none shows */
    int i, done = 1, top = s->kind ? 0x40 : 0x60;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        if (p->rgba[3] == 0) {
            continue;
        }
        done = 0;
        square(p, p->w + 0.04f);
        p->turn = wrap(p->turn + deg(0.5f));
        p->at = vec3_add(p->at, p->v);
        if (!p->landed) {
            p->rgba[3] += irnd(8);
            if (p->rgba[3] > top) {
                p->rgba[3] = top;
                p->landed = 1;
            }
        } else {
            p->rgba[3] = clampi(p->rgba[3] - irnd(s->kind ? 4 : 8), 0, top);
        }
    }
    if (!s->ended) {   /* (unless put out: a spent flame lit again) */
        for (i = 0; i < s->n; i++) {
            if (s->p[i].rgba[3] == 0) {
                flame_part(s, &s->p[i], 0);
                done = 0;
                break;
            }
        }
    }
    return !done;
}

/* ---- Room66Effect: white puffs rising from one of the old mansion B1's four spots with a pinkish
 * glow 12 up (as many as show, flickering); dying down (`particles_command` -1) each puff weakens
 * as it starts again and the glow fades, until none shows ---- */

static const float kSteamSpots[4][2] = {{-190.0f, 22.0f}, {30.0f, -42.0f}, {-20.0f, 10.0f}, {-40.0f, -110.0f}};

static void steam_puff(System *s, Part *p, int again) {   /* Room66Effect_Puff */
    float up = 0.0f, k = p->radius;   /* (its strength) */

    if (s->ended) {   /* (dying down) */
        p->radius = fmaxf(p->radius - 0.2f, 0.0f);
        k = p->radius;
        s->floor -= 0.003f;
        if (s->floor < 0.0f) {
            s->floor = 0.0f;
            p->rgba[3] = 0;
            return;
        }
    }
    colour(p, 0x80, 0x80, 0x80, (int)(k * (float)(irnd(64) - 0x20)) + 0x40);
    square(p, 0.5f + 7.5f * rnd());
    if (!again) {   /* (part way up already) */
        up = rnd();
        p->rgba[3] = (int)((float)p->rgba[3] * (1.0f - up));
        square(p, p->w + 1.5f * up);
    }
    p->at = vec3(s->at.x + k * (s->kind == 2 ? 6.0f : 1.0f) * half(), 30.0f * up - 4.0f,
                 s->at.z + k * (s->kind != 2 ? 17.0f : 1.0f) * half());
    p->turn = deg(rnd());
    p->frame = 0;
    p->rise = 0.65f + 0.3f * k * half();
    p->angle = 2.0f * PI * half();
}

int particles_steam(int spot) {
    int k = spot & 3, h = new_system(STEAM, vec3(kSteamSpots[k][0], 12.0f, kSteamSpots[k][1])), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, 6, 0x180, 0x80, 0x20, 0x20, 1, SPRITE_ADD);
    s->batch2 = sprites_new();
    if (s->batch2 >= 0) {
        sprites_texture(s->batch2, 0x10, 1, -1);
        sprites_cells(s->batch2, 0xA0, 0x40, 32, 32, 512, 256, 1);
        sprites_flags(s->batch2, SPRITE_ADD, 0x19);
    }
    s->kind = k;
    s->floor = 1.0f;   /* (the glow's strength) */
    s->n = 64;
    for (i = 0; i < s->n; i++) {
        s->p[i].radius = 1.0f;
        steam_puff(s, &s->p[i], 0);
    }
    return h;
}

static int steam_step(System *s) {   /* Room66Effect_Update: 0 once dying down and none shows */
    int i, n = 0;

    s->timer ^= 1;
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        square(p, p->w + 0.01f + 0.2f * rnd());
        p->turn += 0.5f * deg(rnd());
        p->angle = wrap(p->angle + deg(10.0f * rnd()));
        p->at = vec3(p->at.x + 0.1f * sinf(p->angle), p->at.y + p->rise, p->at.z + 0.1f * cosf(p->angle));
        if (p->at.y > 50.0f) {
            steam_puff(s, p, 1);
        }
        if (s->timer == 0) {
            float w = 1.0f - p->radius;

            p->rgba[3] -= (int)(20.0f * w * w) + irnd(4);
            if (p->rgba[3] <= 0) {
                steam_puff(s, p, 1);
            }
        }
    }
    for (i = 0; i < s->n; i++) {
        n += s->p[i].rgba[3] != 0;
    }
    if (s->ended && n == 0) {
        return 0;
    }
    s->top = (int)((float)(n + irnd(8)) * s->floor);   /* (the glow) */
    return 1;
}

static void steam_glow(System *s) {
    Sprite *r;

    if (s->batch2 < 0) {
        return;
    }
    r = sprites_records(s->batch2, 1);
    memset(r, 0, sizeof(*r));
    r->x = s->at.x;
    r->y = 12.0f;
    r->z = s->at.z;
    r->w = r->h = 20.0f;
    r->rgba[0] = 0x80;
    r->rgba[1] = 0x70;
    r->rgba[2] = 0x70;
    r->rgba[3] = (uint8_t)clampi(s->top, 0, 255);
}

/* ---- FloorGlow: the old mansion 3F's floor light's glow at the room's middle - a pool on the
 * floor (radius 20, y 0.3) and a cone from 16 below up to a ring of 20 at y 20.2, white at its
 * centre fading to nothing at the rims, added. Its strength follows a level (`particles_command`):
 * up to it and back down to 30, or for level $80 slowly up to $38 ---- */

int particles_floor_glow(void) {
    int h = new_system(FLOORGLOW, vec3(0, 0, 0));

    if (h < 0) {
        return -1;
    }
    sprites_texture(sSys[h].batch, -1, 0, -1);   /* (untextured) */
    sprites_flags(sSys[h].batch, SPRITE_ADD | SPRITE_CORNERS, 0x19);
    sSys[h].command = 0xFF;   /* (no level yet) */
    return h;
}

static int floor_glow_step(System *s) {   /* FloorGlow_Update: the strength (top) by its state */
    switch (s->command) {
    case 0:
        s->top += s->kind >> 4;
        if (s->top > s->kind) {
            s->top = s->kind;
            s->command = 1;
        }
        break;
    case 1:
        s->top -= s->kind >> 4;
        if (s->top < 0x1E) {
            s->top = 0x1E;
            s->command = 0xFF;
        }
        break;
    case 2:
        s->top += s->kind >> 5;
        if (s->top >= 0x38) {
            s->top = 0x38;
            s->command = 0xFF;
        }
        break;
    }
    return 1;
}

static void glow_tri(Sprite *r, Vec3 a, Vec3 b, Vec3 c, uint32_t ca, uint32_t cr) {   /* (a degenerate quad) */
    Vec3 q[4] = {a, b, c, c};
    int k;

    quad(r, ca, q);
    r->shaded = 1;
    for (k = 0; k < 4; k++) {
        uint32_t w = k == 0 ? ca : cr;

        r->crgba[k][0] = (uint8_t)(w >> 24);
        r->crgba[k][1] = (uint8_t)(w >> 16);
        r->crgba[k][2] = (uint8_t)(w >> 8);
        r->crgba[k][3] = (uint8_t)w;
    }
}

static void floor_glow_write(System *s) {   /* FloorGlow_Draw */
    Sprite *r = sprites_records(s->batch, -1);
    uint32_t centre = 0xFFFFFF00u | (uint32_t)clampi(s->top, 0, 255);
    float d = 20.0f * sinf(PI / 4.0f);
    Vec3 pool[8] = {vec3(20, 0.3f, 0), vec3(d, 0.3f, d), vec3(0, 0.3f, 20), vec3(-d, 0.3f, d),
                    vec3(-20, 0.3f, 0), vec3(-d, 0.3f, -d), vec3(0, 0.3f, -20), vec3(d, 0.3f, -d)};
    int i, n = 0;

    if (s->kind == 0 || s->top == 0) {
        sprites_records(s->batch, 0);
        return;
    }
    for (i = 0; i < 8; i++) {
        glow_tri(&r[n++], vec3(0, 0.3f, 0), pool[i], pool[(i + 1) % 8], centre, 0x80808000u);
    }
    for (i = 0; i < 16; i++) {
        float a = deg(22.5f * (float)i), b = deg(22.5f * (float)(i + 1));

        glow_tri(&r[n++], vec3(0, -16.0f, 0), vec3(20.0f * sinf(a), 20.2f, 20.0f * cosf(a)),
                 vec3(20.0f * sinf(b), 20.2f, 20.0f * cosf(b)), centre, 0);
    }
    sprites_records(s->batch, n);
}

/* ---- TvScreenA / B: a TV in castle 2F showing the screen - last frame's picture, grey, on its
 * screen's quad at half strength while it is on (`particles_command` nonzero) ---- */

static const float kTvScreen[2][6] = {   /* x0 x1 top bottom z0 z1 */
    {-2.6139f, -0.1217f, 4.3915f, 2.0626f, 5.7884f, 7.5254f}, {20.8022f, 22.96f, 12.0452f, 9.8035f, 7.7294f, 9.804f}};

int particles_tv(int which) {
    int h = new_system(TV, vec3(0, 0, 0));

    if (h < 0) {
        return -1;
    }
    sSys[h].kind = which & 1;
    sprites_texture(sSys[h].batch, -1, 0, -1);
    sprites_uv(sSys[h].batch, 0.0f, 1.0f, 1.0f, 0.0f);   /* (the picture is upside down in GL) */
    sprites_flags(sSys[h].batch, SPRITE_CORNERS | SPRITE_OPAQUE, 0x19);
    return h;
}

static void tv_write(System *s) {   /* tv_draw */
    const float *t = kTvScreen[s->kind];
    unsigned int tex = post_tv_texture();
    Vec3 q[4] = {vec3(t[0], t[2], t[4]), vec3(t[1], t[2], t[5]), vec3(t[0], t[3], t[4]), vec3(t[1], t[3], t[5])};

    if (!s->command) {
        sprites_records(s->batch, 0);
        return;
    }
    post_tv_want();
    if (tex == 0) {
        sprites_records(s->batch, 0);
        return;
    }
    sprites_raw_texture(s->batch, tex);
    quad(sprites_records(s->batch, 1), 0x40404080u, q);
}

/* ---- BigFire: the old mansion 1F's fire - 128 puffs of smoke rising from (-145.6, 35, 7.3) and
 * fading, 32 orange tongues of flame wandering out of (-135, 9.5, 11) and rising ever slower, and
 * a flickering glow at (-128, 5, 14); put out (`particles_command` nonzero) the smoke and glow go
 * and it ends once no flame shows ---- */

#define BIGFIRE_SMOKE 128
#define BIGFIRE_FLAMES 32

static void big_fire_smoke(Part *p, int again) {   /* BigFire_Smoke */
    int age = 0;

    colour(p, 0x80, 0x80, 0x80, irnd(32) + 0x20);
    square(p, 1.5f);
    if (!again) {   /* (part way up already) */
        age = (int)(65.0f * rnd());
        p->rgba[3] = clampi(p->rgba[3] - (age >> 1), 0, 0x40);
        square(p, 1.5f + 0.04f * (float)age);
    }
    p->at = vec3(-145.6f + 4.0f * half(), 35.0f + (float)age, 7.3f + 4.0f * half());
    p->turn = 2.0f * PI * half();
    p->frame = 0;
    p->v = vec3(0.03f * half(), 0.07f + 0.1f * rnd(), 0.03f * half());
}

static void big_fire_flame(Part *p) {   /* BigFire_Flame (its first time's age is always 0) */
    p->radius = 0.7f + 0.1f * rnd();   /* (its drift out) */
    p->rise = 0.1f + 0.2f * rnd();
    p->angle = 2.0f * PI * half();     /* (its heading) */
    colour(p, irnd(32) + 0x80, 0x40, 0x10, irnd(64) + 0x40);
    p->at = vec3(-135.0f + 4.0f * half() + 0.1f * cosf(p->angle), 9.5f + 2.0f * half(), 11.0f + 4.0f * half() + 0.1f * sinf(p->angle));
    square(p, 0.3f + 0.2f * rnd());
    p->turn = 2.0f * PI * half();
    p->frame = 0;
}

int particles_big_fire(void) {
    int h = new_system(BIGFIRE, vec3(-135.0f, 9.5f, 11.0f)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0, 0x40, 0x20, 0x20, 1, 0);   /* (the smoke) */
    s->batch2 = sprites_new();   /* (the flames) */
    if (s->batch2 >= 0) {
        sprites_texture(s->batch2, 0x10, 1, -1);
        sprites_cells(s->batch2, 0x6C, 0x4C, 8, 8, 512, 256, 1);
        sprites_flags(s->batch2, SPRITE_ADD, 0x19);
    }
    s->batch3 = sprites_new();   /* (the glow) */
    if (s->batch3 >= 0) {
        sprites_texture(s->batch3, 0x10, 1, -1);
        sprites_cells(s->batch3, 0xA0, 0x40, 32, 32, 512, 256, 1);
        sprites_flags(s->batch3, SPRITE_ADD, 0x19);
    }
    s->n = BIGFIRE_SMOKE + BIGFIRE_FLAMES;
    for (i = 0; i < BIGFIRE_SMOKE; i++) {
        big_fire_smoke(&s->p[i], 0);
    }
    for (i = 0; i < BIGFIRE_FLAMES; i++) {
        big_fire_flame(&s->p[BIGFIRE_SMOKE + i]);
    }
    s->top = 0x30;
    return h;
}

static int big_fire_step(System *s) {   /* BigFire_Update: 0 once out and no flame shows */
    int i, alive = s->ended != 0;

    s->timer ^= 1;
    if (!s->ended) {
        for (i = 0; i < BIGFIRE_SMOKE; i++) {
            Part *p = &s->p[i];

            square(p, p->w + 0.04f);
            p->turn = wrap(p->turn + deg(3.0f));
            p->at = vec3_add(p->at, p->v);
            if (p->at.y > 100.0f) {
                big_fire_smoke(p, 1);
            }
            if (s->timer == 0) {
                p->rgba[3] -= irnd(4);
            }
            if (p->rgba[3] <= 0) {
                big_fire_smoke(p, 1);
            }
        }
        s->top = s->top == 0x30 ? 0x38 : 0x30;
    }
    for (i = 0; i < BIGFIRE_FLAMES; i++) {
        Part *p = &s->p[BIGFIRE_SMOKE + i];

        if (p->rgba[3] > 0) {
            p->rgba[3] -= irnd(4) + 2;
            p->rgba[0] = p->rgba[0] > 0 ? p->rgba[0] - irnd(2) : 0;
        }
        if (p->rgba[3] <= 0) {
            if (s->ended) {
                p->rgba[3] = 0;
            } else {
                big_fire_flame(p);
                alive = 0;
            }
            continue;
        }
        alive = 0;
        if (!s->ended) {
            p->rise -= 0.015f;
            p->angle += deg(30.0f * half());
        } else {
            p->rise -= 0.005f;
            p->angle += deg(10.0f * half());
        }
        p->angle = wrap(p->angle);
        p->radius = fmaxf(p->radius - 0.1f, 0.01f);
        p->rise = fmaxf(p->rise, 0.05f);
        p->at.x += p->radius + fabsf(0.1f * cosf(p->angle));
        p->at.y += p->rise;
        p->at.z += 0.1f * sinf(p->angle);
    }
    return !alive;
}

static void sprite_rec(Sprite *r, const Part *p) {
    int k;

    memset(r, 0, sizeof(*r));
    r->x = p->at.x;
    r->y = p->at.y;
    r->z = p->at.z;
    r->w = p->w;
    r->h = p->h;
    r->rot = p->turn;
    r->frame = p->frame;
    for (k = 0; k < 4; k++) {
        r->rgba[k] = (uint8_t)clampi(p->rgba[k], 0, 255);
    }
}

static void big_fire_write(System *s) {
    Sprite *r = sprites_records(s->batch, -1);
    int i;

    for (i = 0; i < BIGFIRE_SMOKE && !s->ended; i++) {
        sprite_rec(&r[i], &s->p[i]);
    }
    sprites_records(s->batch, s->ended ? 0 : BIGFIRE_SMOKE);
    if (s->batch2 >= 0) {
        r = sprites_records(s->batch2, -1);
        for (i = 0; i < BIGFIRE_FLAMES; i++) {
            sprite_rec(&r[i], &s->p[BIGFIRE_SMOKE + i]);
        }
        sprites_records(s->batch2, BIGFIRE_FLAMES);
    }
    if (s->batch3 >= 0) {
        Part glow = {0};

        glow.at = vec3(-128.0f, 5.0f, 14.0f);
        square(&glow, 5.0f);
        colour(&glow, 0x70, 0x40, 0x40, s->top);
        r = sprites_records(s->batch3, -1);
        sprite_rec(&r[0], &glow);
        sprites_records(s->batch3, s->ended ? 0 : 1);
    }
}

/* ---- LightShaft: the old mansion B1's beam of light - a texture (the room's 8, rows 224..256)
 * scrolling along a strip from its top edge (0, 60, 82 .. 57) to the floor (10, 30, 100 .. 40),
 * its middle brighter, drawn twice (the second mirrored a quarter further); 16 motes of dust
 * rising through it from (30, 0, 70), wobbling, fading; the haze it can lay over the screen
 * (`particles_command` 1 on, 2 off: post_haze) ---- */

static void shaft_mote(System *s, Part *p) {   /* LightShaft_Mote */
    if (s->ended) {
        return;
    }
    p->rise = 0.1f + 0.2f * rnd();
    p->angle = 2.0f * PI * half();
    colour(p, 0x80, 0x80, 0x80, irnd(64) + 0x40);
    p->at = vec3(30.0f, 0.0f, 70.0f + 100.0f * half());
    square(p, 0.75f * p->rise);
    p->turn = 2.0f * PI * half();
    p->frame = 0;
}

int particles_light_shaft(void) {
    int h = new_system(SHAFT, vec3(30.0f, 0.0f, 70.0f)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, 5, 0x1A0, 0x40, 0x20, 0x20, 1, SPRITE_ADD);   /* (the motes) */
    s->batch2 = sprites_new();   /* (the beam) */
    if (s->batch2 >= 0) {
        sprites_texture(s->batch2, 0, 8, -1);
        sprites_flags(s->batch2, SPRITE_CORNERS, 0x19);
    }
    s->timer = (int)(4096.0f * rnd());   /* (the scroll, 1/16 texels) */
    s->tilt = 2.0f * PI * half();   /* (the haze's phases) */
    s->head = 2.0f * PI * half();
    s->n = 16;
    for (i = 0; i < s->n; i++) {
        shaft_mote(s, &s->p[i]);
    }
    return h;
}

static int shaft_step(System *s) {   /* LightShaft_Update */
    int i;

    if (s->ended) {
        return 0;
    }
    s->rest ^= 1;   /* (every other frame) */
    s->timer += 3;   /* (3.2, cut to whole sixteenths each frame) */
    if (s->slow) {   /* (the haze: its phases on 3..5 and 1..3 degrees a frame) */
        s->tilt = wrap(s->tilt + deg(3.0f + 2.0f * rnd()));
        s->head = wrap(s->head + deg(1.0f + 2.0f * rnd()));
        post_haze(s->tilt, 2.0f * sinf(s->head), 2.0f, 0x48, 0x48);
    }
    if (s->timer > 4096) {
        s->timer -= 4096;
    }
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        p->turn += 0.5f * deg(rnd());
        p->angle = wrap(p->angle + deg(90.0f * rnd()));
        p->at = vec3(p->at.x + 0.1f * sinf(p->angle), p->at.y + p->rise, p->at.z + 0.1f * cosf(p->angle));
        if (s->rest == 0) {
            p->rgba[3] -= irnd(2);
            if (p->rgba[3] < 0) {
                shaft_mote(s, p);
            }
        }
    }
    return 1;
}

static void shaft_write(System *s) {   /* LightShaft_Draw: the beam twice, then the motes */
    static const Vec3 kTop[3] = {{0, 60, 82}, {0, 60, 70}, {0, 60, 57}}, kFloor[3] = {{10, 30, 100}, {10, 30, 70}, {10, 30, 40}};
    int tw, th, pass, q, k;

    write(s);
    if (s->batch2 < 0) {
        return;
    }
    if (!sprites_texture_size(0, 8, &tw, &th) || tw <= 0 || th <= 0) {
        sprites_records(s->batch2, 0);
        return;
    }
    {
        Sprite *r = sprites_records(s->batch2, -1);

        for (pass = 0; pass < 2; pass++) {
            int sc = s->timer;

            if (pass == 1 && (sc += 0x400) > 0x1000) {
                sc -= 0x1000;
            }
            for (q = 0; q < 2; q++) {   /* (front to middle, middle to back) */
                Sprite *x = &r[pass * 2 + q];
                Vec3 c[4] = {kTop[q], kTop[q + 1], kFloor[q], kFloor[q + 1]};

                quad(x, 0x80808080u, c);
                x->shaded = x->mapped = 1;
                for (k = 0; k < 4; k++) {
                    int col = q + (k & 1);

                    if (pass == 1) {
                        col = 2 - col;
                    }
                    x->cst[k][0] = (float)(sc + 8 + col * 0x800) / 16.0f / (float)tw;
                    x->cst[k][1] = (k & 2 ? 256.5f : 224.5f) / (float)th;
                    x->crgba[k][0] = x->crgba[k][1] = x->crgba[k][2] = 0x80;
                    x->crgba[k][3] = q + (k & 1) == 1 ? 0x80 : 0x40;
                }
            }
        }
        sprites_records(s->batch2, 4);
    }
}

/* ---- Room49Effect: the old mansion 1F's haze - 64 motes rising from (-60, 0, -60 + 0.4 x the
 * frames so far), wobbling, fading; one frame in 8 each one gone comes back one time in 16; the
 * screen wavering at $60 ---- */

static void haze_mote(System *s, Part *p) {   /* LightShaft_Mote2 */
    p->rise = 0.1f + 0.2f * rnd();
    p->angle = 2.0f * PI * half();
    colour(p, 0x80, 0x80, 0x80, irnd(64) + 0x40);
    p->at = vec3(-60.0f, 0.0f, -60.0f + 0.4f * (float)s->frames);
    square(p, 0.75f * p->rise);
    p->turn = 2.0f * PI * half();
    p->frame = 0;
}

int particles_haze(void) {
    int h = new_system(HAZE, vec3(-60.0f, 0.0f, -60.0f)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, 5, 0x1A0, 0x40, 0x20, 0x20, 1, SPRITE_ADD);
    s->n = 64;
    for (i = 0; i < s->n; i++) {   /* (all hidden at first) */
        haze_mote(s, &s->p[i]);
        s->p[i].rgba[3] = 0;
    }
    s->tilt = 2.0f * PI * half();
    s->head = 2.0f * PI * half();
    return h;
}

static void haze_step(System *s) {   /* Room49Effect_Update */
    int i;

    s->rest ^= 1;
    s->frames++;
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        p->turn += 0.5f * deg(rnd());
        p->angle = wrap(p->angle + deg(90.0f * rnd()));
        p->at = vec3(p->at.x + 0.1f * sinf(p->angle), p->at.y + p->rise, p->at.z + 0.1f * cosf(p->angle));
        if (s->rest == 0 && (p->rgba[3] -= irnd(2)) < 0) {
            p->rgba[3] = 0;
        }
    }
    if (irnd(8) == 0) {
        for (i = 0; i < s->n; i++) {
            if (s->p[i].rgba[3] == 0 && irnd(16) == 0) {
                haze_mote(s, &s->p[i]);
            }
        }
    }
    s->tilt = wrap(s->tilt + deg(3.0f + 2.0f * rnd()));
    s->head = wrap(s->head + deg(1.0f + 2.0f * rnd()));
    post_haze(s->tilt, 2.0f * sinf(s->head), 2.0f, 0x60, 0x60);
}

/* ---- a caustic (the original's Caustic_Draw, queued by Effect78BC0, Effect7A3D0, Room4EEffect,
 * BackdropModel, BackdropModel2, TurningModel): one of the room's textures laid flat as two 8 x 8
 * grids of `size` at a point, turned about y, their texture coordinates wobbling by 0.05 with a
 * phase (a quarter turn a cell; the second grid -0.4 of a half turn a cell), added at 1/8 in a
 * colour. The phase and the two grids' turns move on by `step` / `turn` a frame (`turn` 0: the
 * grids stay unturned). Its glow (the screen blurred where it left the frame's alpha): not drawn ---- */

int particles_caustic(Vec3 at, int tex, float size, uint32_t rgba, float step, float turn) {
    int h = new_system(CAUSTIC, at);
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    sprites_texture(s->batch, 0, tex, -1);   /* (the room's bank) */
    sprites_flags(s->batch, SPRITE_ADD | SPRITE_CORNERS, 0x19);
    s->floor = size;
    s->colour = (rgba & 0xFFFFFF00u) | 0x10u;   /* (added at 1/8) */
    s->c = vec3(step, turn, 0);
    s->tilt = 2.0f * PI * half();   /* (the phase) */
    s->head = turn != 0.0f ? 2.0f * PI * half() : 0.0f;
    s->last = turn != 0.0f ? 2.0f * PI * half() : 0.0f;
    return h;
}

static void caustic_step(System *s) {   /* (spin_step) */
    s->tilt = wrap(s->tilt + s->c.x);
    s->head = wrap(s->head + s->c.y);
    s->last = wrap(s->last - s->c.y);
}

static void caustic_write(System *s) {
    Sprite *r = sprites_records(s->batch, -1);
    float half_size = 0.5f * s->floor, cell = 0.125f * s->floor, ph = s->tilt;
    int g, i, j, k, n = 0;

    for (g = 0; g < 2; g++) {
        float a = g == 0 ? s->head : s->last, c = cosf(a), sn = sinf(a);

        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                Sprite *x = &r[n++];
                Vec3 q[4];

                for (k = 0; k < 4; k++) {   /* (corner k: row i + (k & 1), column j + (k >> 1)) */
                    int rw = i + (k & 1), cl = j + (k >> 1);
                    float lx = (float)rw * cell - half_size, lz = (float)cl * cell - half_size;

                    q[k] = vec3(s->at.x + lx * c + lz * sn, s->at.y, s->at.z + lz * c - lx * sn);
                }
                quad(x, s->colour, q);
                x->mapped = 1;
                for (k = 0; k < 4; k++) {
                    int rw = i + (k & 1), cl = j + (k >> 1);

                    if (g == 0) {
                        x->cst[k][0] = 0.1f + 0.1f * (float)rw + 0.05f * cosf(wrap(ph + 0.5f * PI * (float)rw));
                        x->cst[k][1] = 0.1f * (float)cl + 0.05f * sinf(wrap(ph + 0.5f * PI * (float)cl));
                    } else {
                        x->cst[k][0] = 0.1f * (float)cl + 0.05f * cosf(wrap(ph - 0.4f * PI * (float)rw));
                        x->cst[k][1] = 0.1f + 0.1f * (float)rw + 0.05f * sinf(wrap(ph - 0.4f * PI * (float)cl));
                    }
                }
            }
        }
    }
    sprites_records(s->batch, n);
}

/* ---- Effect737D0: a glow standing still - one sprite (cell (0xA0, 0x40)) added at a point, its size
 * and colour as given ---- */

int particles_glow(Vec3 at, uint32_t rgba, float size) {
    int h = new_system(GLOWSPOT, at);
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0xA0, 0x40, 0x20, 0x20, 1, SPRITE_ADD);
    s->n = 1;
    s->p[0].at = at;
    square(&s->p[0], size);
    colour(&s->p[0], (int)(rgba >> 24), (int)(rgba >> 16 & 0xFF), (int)(rgba >> 8 & 0xFF), (int)(rgba & 0xFF));
    return h;
}

/* ---- WispColumn: a column of 64 wisps 20 across rising 5 a frame up the House of Truth's
 * middle from -20 to 60 and over again, turning; turned off (`particles_command` 0) their alpha
 * runs down from $10 and it ends. (The original also drops it at once in a cutscene unless the
 * stalker is kind $C: not here) ---- */

static void column_wisp(System *s, Part *p, int i, int stagger) {   /* WispColumn_Wisp */
    colour(p, 0x80, 0x80, 0x80, s->top);
    p->at = vec3(0.0f, -20.0f - (stagger ? 5.0f * (float)(i >> 2) : 0.0f), 0.0f);
    square(p, 20.0f);
    p->turn = deg(180.0f * rnd());
    p->frame = 0;
}

int particles_wisp_column(void) {
    int h = new_system(WISPCOL, vec3(0, 20, 0)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, 6, 0x180, 0x80, 0x20, 0x20, 1, SPRITE_GLOW);
    s->top = 0x10;
    s->n = 64;
    for (i = 0; i < s->n; i++) {
        column_wisp(s, &s->p[i], i, 1);
    }
    return h;
}

static int wisp_column_step(System *s) {   /* WispColumn_Update */
    int i;

    if (s->ended && --s->top <= 0) {
        return 0;
    }
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        p->rgba[3] = s->top;
        p->turn = wrap(p->turn + deg(10.0f));
        p->at.y += 5.0f;
        if (p->at.y >= 60.0f) {
            column_wisp(s, p, i, 0);
        }
    }
    return 1;
}

/* ---- SparkSpray: 64 sparks sprayed across the House of Truth from one side (0 the left, x -75;
 * else the right, x 75; 14 up) and down, each started in turn, growing, turning, fading and
 * sprayed again while on; turned off (`particles_command` 0) it ends once all have faded. (The
 * original drops it at once in a cutscene: not here) ---- */

static void spray_spark(System *s, Part *p, int i, int delayed) {   /* SparkSpray_Spark */
    float d = 2.0f + 6.0f * rnd(), a = -PI + 2.0f * PI * rnd(), sign = s->kind == 0 ? 1.0f : -1.0f;

    colour(p, 0x80, 0x80, 0x80, 0x40);
    p->at = vec3(-75.0f * sign, 14.0f + d * cosf(a), d * sinf(a));
    p->v = vec3(sign * (4.0f + rnd()), -0.5f, -0.1f + 0.2f * rnd());
    p->pull = vec3(-0.15f * sign, 0.1f, 0.0f);
    square(p, 5.0f);
    p->turn = deg(180.0f * rnd());
    p->frame = 0;
    p->delay = delayed ? i + 1 : 0;
    if (delayed) {
        p->rgba[3] = 0;
    }
}

int particles_spark_spray(int side) {
    int h = new_system(SPARKSPRAY, vec3(side ? 75.0f : -75.0f, 14.0f, 0.0f)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0, 0x40, 0x20, 0x20, 1, SPRITE_ADD);
    s->kind = side != 0;
    s->n = 64;
    for (i = 0; i < s->n; i++) {
        spray_spark(s, &s->p[i], i, 1);
    }
    return h;
}

static int spark_spray_step(System *s) {   /* SparkSpray_Update */
    int i, done = 1;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        if (p->delay != 0) {
            if (--p->delay == 0) {
                spray_spark(s, p, i, 0);
            }
        } else if (p->rgba[3] != 0) {
            p->rgba[3]--;
            done = 0;
            if (!s->ended && p->rgba[3] == 0) {
                spray_spark(s, p, i, 0);
            }
        }
        square(p, p->w + 0.5f);
        p->turn = wrap(p->turn + deg(10.0f));
        p->v = vec3_add(p->v, p->pull);
        p->at = vec3_add(p->at, p->v);
    }
    return !(s->ended && done);
}

/* ---- Room92Effect: a heat haze over the House of Truth - the screen wavering in columns, the wave's
 * height easing 0.1 a frame toward its mark (2 at first; `particles_command`), the phase on 50..70
 * degrees a frame, strongest in the middle of the screen ---- */

int particles_heat_haze(void) {
    int h = new_system(HEAT, vec3(0, 0, 0));

    if (h < 0) {
        return -1;
    }
    sSys[h].tilt = 2.0f * PI * half();
    sSys[h].c = vec3(2.0f, 2.0f, 0.0f);   /* (the height, its mark) */
    return h;
}

static int heat_step(System *s) {   /* Room92Effect_Update */
    if (s->ended) {
        return 0;
    }
    s->tilt = wrap(s->tilt + deg(50.0f + 20.0f * rnd()));
    if (s->c.x < s->c.y) {
        s->c.x = fminf(s->c.x + 0.1f, s->c.y);
    } else if (s->c.x > s->c.y) {
        s->c.x = fmaxf(s->c.x - 0.1f, s->c.y);
    }
    post_haze(s->tilt, 0.0f, s->c.x, 0x30, 0x10);
    return 1;
}

/* ---- Embers: the House of Truth's pit - 16 embers rising from within 10 of the middle at -4,
 * and (with flames) 24 flames 10..13 across rising from within 5 at -16, shrinking; they sway,
 * fade every other frame and start again. Off (`particles_command` 0): gone ---- */

#define EMBERS2_FLAMES 24

static void pit_ember(Part *p, int fresh) {   /* Embers_Ember */
    float y = 0.0f;

    colour(p, 0x80, 0x80, 0x80, 0x80);
    if (!fresh) {
        y = 5.0f * rnd();
        p->rgba[3] = (int)(0.2f * (128.0f * (5.0f - y)));
    }
    p->at = vec3(20.0f * half(), y - 4.0f, 20.0f * half());
    square(p, 0.2f + 0.2f * rnd());
    p->turn = 2.0f * PI * half();
    p->frame = 0;
    p->rise = 0.2f + 0.4f * rnd();
    p->angle = 2.0f * PI * half();
}

static void pit_flame(Part *p, int fresh) {   /* Embers_Flame */
    float y = 0.0f;

    colour(p, 0x80, 0x80, 0x80, 0x40);
    square(p, 10.0f + 3.0f * rnd());
    if (!fresh) {
        y = 25.0f * rnd();
        p->rgba[3] = (int)(0.04f * (64.0f * (25.0f - y)));
        square(p, p->w - 0.5f * y);
    }
    p->at = vec3(10.0f * half(), y - 16.0f, 10.0f * half());
    p->turn = 2.0f * PI * half();
    p->frame = 0;
    p->rise = 0.6f + 0.05f * rnd();
    p->angle = 2.0f * PI * half();
}

int particles_pit_embers(int flames) {
    int h = new_system(EMBERS2, vec3(0, -4, 0)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, 6, 0x6C, 0x4C, 8, 8, 1, SPRITE_ADD);   /* (the embers) */
    s->kind = flames != 0;
    if (s->kind) {
        s->batch2 = sprites_new();
        if (s->batch2 >= 0) {
            sprites_texture(s->batch2, 0x10, 1, 6);
            sprites_cells(s->batch2, 0x180, 0x80, 32, 32, 512, 256, 1);
            sprites_flags(s->batch2, SPRITE_ADD, 0x19);
        }
    }
    s->n = EMBERS2_FLAMES + 16;
    for (i = 0; i < EMBERS2_FLAMES; i++) {
        pit_flame(&s->p[i], 0);
    }
    for (i = 0; i < 16; i++) {
        pit_ember(&s->p[EMBERS2_FLAMES + i], 0);
    }
    return h;
}

static int embers2_step(System *s) {   /* Embers_Update */
    int i;

    if (s->ended) {
        return 0;
    }
    s->timer ^= 1;
    for (i = 0; i < EMBERS2_FLAMES && s->kind; i++) {
        Part *p = &s->p[i];

        square(p, p->w - (0.2f + 0.1f * rnd()));
        if (p->w < 0.0f) {
            square(p, 0.0f);
            p->rgba[3] = 0;
        }
        p->turn += deg(rnd());
        p->angle = wrap(p->angle + deg(10.0f * rnd()));
        p->at = vec3(p->at.x + 0.1f * sinf(p->angle), p->at.y + p->rise, p->at.z + 0.1f * cosf(p->angle));
        if (s->timer == 0 && (p->rgba[3] -= irnd(2) + 2) < 0) {
            pit_flame(p, 1);
        }
    }
    for (i = EMBERS2_FLAMES; i < s->n; i++) {
        Part *p = &s->p[i];

        p->turn += 0.5f * deg(rnd());
        p->angle = wrap(p->angle + deg(10.0f * rnd()));
        p->at = vec3(p->at.x + 0.2f * sinf(p->angle), p->at.y + p->rise, p->at.z + 0.2f * cosf(p->angle));
        if (s->timer == 0 && (p->rgba[3] -= irnd(4) + 7) < 0) {
            pit_ember(p, 1);
        }
    }
    return 1;
}

static void embers2_write(System *s) {
    Sprite *r = sprites_records(s->batch, -1);
    int i;

    for (i = 0; i < 16; i++) {
        sprite_rec(&r[i], &s->p[EMBERS2_FLAMES + i]);
    }
    sprites_records(s->batch, 16);
    if (s->kind && s->batch2 >= 0) {
        r = sprites_records(s->batch2, -1);
        for (i = 0; i < EMBERS2_FLAMES; i++) {
            sprite_rec(&r[i], &s->p[i]);
        }
        sprites_records(s->batch2, EMBERS2_FLAMES);
    }
}

/* ---- Room2AWisps: 16 leaves blowing across the front garden from by (211.28, 2.125, -220.57) -
 * flat, tumbling about all three axes, a 16-frame strip; drifting on +x (0.2..0.25) and back on
 * z under a wind that shifts now and then; blown in again past x 300 ---- */

static void leaf(System *s, Part *p) {   /* Room2AWisps_Particle */
    int k;

    colour(p, 0x80, 0x80, 0x80, 0x80);
    p->at = vec3(s->at.x - 10.0f + 10.0f * half(), s->at.y + rnd(), s->at.z + 20.0f + 150.0f * rnd());
    square(p, 1.0f);
    p->frame = irnd(16);
    p->v = vec3(0.2f + 0.05f * rnd(), 0.01f * rnd(), -0.05f * rnd());
    for (k = 0; k < 3; k++) {
        p->tturn[k] = PI * rnd();
        p->spin[k] = 10.0f + 22.5f * half();
    }
}

int particles_leaves(void) {
    int h = new_system(LEAVES, vec3(211.28f, 2.125f, -220.567f)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    sprites_texture(s->batch, 0x10, 1, 0xA);
    sprites_cells(s->batch, 0, 0xE0, 32, 32, 512, 256, 16);
    sprites_flags(s->batch, SPRITE_CORNERS, 0x19);
    s->n = 16;
    for (i = 0; i < s->n; i++) {
        leaf(s, &s->p[i]);
        s->p[i].at.x = s->at.x - 20.0f + 100.0f * rnd();   /* (spread to one side at first) */
    }
    s->c = vec3(0.0f, 0.0f, -0.1f);   /* (the wind) */
    s->timer = irnd(32) + 0x5A;
    s->mark = 0;
    return h;
}

static void leaves_step(System *s) {   /* Room2AWisps_Update */
    int i, k;

    if (--s->timer == 0) {   /* (the wind turns now and then) */
        s->timer = irnd(64);
        s->mark = (s->mark + 1) & 3;
    }
    if (s->mark == 1) {
        s->c.x = fmaxf(s->c.x - 0.005f, -0.05f);
        s->c.y = 0.0f;
    } else if (s->mark == 3) {
        s->c.x = fminf(s->c.x + 0.005f, 0.05f);
        s->c.y = -0.03f;
    }
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        p->at = vec3_add(p->at, vec3_add(p->v, s->c));
        if (p->at.x >= 300.0f) {
            leaf(s, p);
            continue;
        }
        for (k = 0; k < 3; k++) {
            p->tturn[k] = wrap(p->tturn[k] + deg(p->spin[k]));
        }
        if (++p->frame >= 16) {
            p->frame = 0;
        }
    }
}

static void leaves_write(System *s) {   /* Room2AWisps_Draw: a unit quad flat, turned */
    static const float kCorner[4][2] = {{0.5f, -0.5f}, {0.5f, 0.5f}, {-0.5f, -0.5f}, {-0.5f, 0.5f}};
    Sprite *r = sprites_records(s->batch, -1);
    int i, k;

    for (i = 0; i < s->n; i++) {
        const Part *p = &s->p[i];
        Vec3 q[4];

        for (k = 0; k < 4; k++) {
            q[k] = vec3_add(turn_xyz(vec3(kCorner[k][0], 0.0f, kCorner[k][1]), p->tturn[0], p->tturn[1], p->tturn[2]), p->at);
        }
        quad(&r[i], 0x80808080u, q);
        r[i].frame = p->frame;
    }
    sprites_records(s->batch, s->n);
}

/* ---- FallingDrops: 16 drops falling 1.5..2.5 a frame anywhere over an 80 x 80 square from up to
 * 150; on the floor a spray of droplets, on the water (no floor under them) at -5.9 a ring and a
 * droplet; the first drop's landing also sounds (2..4 by turns, bank 6) ---- */

static void rain_drop(Part *p) {   /* FallingDrops_Drop */
    float h;

    colour(p, 0x20, 0x20, 0x20, 0x70);
    p->at = vec3(80.0f * half(), 150.0f * rnd(), 80.0f * half());
    square(p, 0.5f);
    p->turn = 2.0f * PI * half();
    p->frame = 0;
    p->rise = 1.5f + rnd();
    p->delay = navmesh_find(&gEngine.room.nav, vec3(p->at.x, 0.0f, p->at.z), 1000.0f, &h);   /* (its floor's triangle) */
}

int particles_rain(void) {
    int h = new_system(RAIN, vec3(0, 75, 0)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0x6C, 0x4C, 8, 8, 1, SPRITE_ADD | SPRITE_GLOW);
    s->n = 16;
    for (i = 0; i < s->n; i++) {
        rain_drop(&s->p[i]);
    }
    return h;
}

static void rain_step(int self, System *s) {   /* FallingDrops_Update */
    int i;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];
        int landed = 0;

        p->at.y -= p->rise;
        if (p->delay < 0) {   /* (over the water) */
            if (p->at.y < -6.0f) {
                Vec3 at = vec3(p->at.x, -5.9f, p->at.z);
                Spray sp = {at, 0x20202030u, 1, 0.5f, 0.2f, 0, 0, 0.3f, 0.2f, 0, 0, 0.5f, 0.1f};

                ring_new(self, at, 0x40404020u, 0.15f + 0.1f * rnd());
                droplets_new(self, &sp);
                landed = 1;
            }
        } else {
            float floor = navmesh_height(&gEngine.room.nav, p->delay, p->at.x, p->at.z);

            if (p->at.y < floor) {
                Spray sp = {vec3(p->at.x, floor, p->at.z), 0x20202010u, 16, 0.4f, 0.4f, 0.1f, 0.2f, 0.1f, 0.2f, 0.1f, 0.2f, 0.5f, 0.1f};

                droplets_new(self, &sp);
                landed = 1;
            }
        }
        if (landed) {
            if (i == 0) {
                engine_sound_at((uint32_t)(s->mark + 2), 6, p->at);
                s->mark = s->mark >= 2 ? 0 : s->mark + 1;
            }
            rain_drop(p);
        }
    }
}

/* ---- AshFlakes: 128 flakes of ash about (-80, -20, -10) in the Chaos Forest - flat 2 x 2 quads
 * tumbling about all three axes (a 16-frame strip), carried up on a wind (0.2, 2, 0) and rising
 * ever faster; hidden at first, three a frame come out, spread deeper the longer it runs ---- */

static void ash_flake(System *s, Part *p, int spread) {   /* AshFlakes_Flake */
    int k;

    colour(p, 0x80, 0x80, 0x80, spread != 0 ? 0x80 : 0);
    p->at = vec3(s->at.x + 20.0f * half(), s->at.y - 10.0f * rnd(), s->at.z + 2.0f * (float)spread * half());
    square(p, 1.0f);
    p->frame = irnd(16);
    p->v = vec3(0.1f * half(), 0.1f * rnd(), 0.1f * half());
    for (k = 0; k < 3; k++) {
        p->tturn[k] = PI * rnd();
        p->spin[k] = 10.0f + 22.5f * half();
    }
}

int particles_ash(void) {
    int h = new_system(ASH, vec3(-80.0f, -20.0f, -10.0f)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    sprites_texture(s->batch, 0x10, 1, 0xB);
    sprites_cells(s->batch, 0, 0xE0, 32, 32, 512, 256, 16);
    sprites_flags(s->batch, SPRITE_CORNERS, 0x19);
    s->n = 128;
    for (i = 0; i < s->n; i++) {
        ash_flake(s, &s->p[i], 0);
    }
    return h;
}

static void ash_step(System *s) {   /* AshFlakes_Update */
    int i, k, n = 3;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        if (p->rgba[3] == 0) {
            continue;
        }
        p->v.y += 0.01f * rnd();
        p->at = vec3_add(p->at, vec3_add(p->v, vec3(0.2f, 2.0f, 0.0f)));
        for (k = 0; k < 3; k++) {
            p->tturn[k] = wrap(p->tturn[k] + deg(p->spin[k]));
        }
        if (++p->frame >= 16) {
            p->frame = 0;
        }
    }
    s->frames++;
    for (i = 0; i < s->n && n > 0; i++) {
        if (s->p[i].rgba[3] == 0) {
            ash_flake(s, &s->p[i], s->frames);
            n--;
        }
    }
}

static void ash_write(System *s) {   /* AshFlakes_Draw: a 2 x 2 quad flat, turned */
    static const float kCorner[4][2] = {{1, -1}, {1, 1}, {-1, -1}, {-1, 1}};
    Sprite *r = sprites_records(s->batch, -1);
    int i, k, n = 0;

    for (i = 0; i < s->n; i++) {
        const Part *p = &s->p[i];
        Vec3 q[4];

        if (p->rgba[3] == 0) {
            continue;
        }
        for (k = 0; k < 4; k++) {
            q[k] = vec3_add(turn_xyz(vec3(kCorner[k][0], 0.0f, kCorner[k][1]), p->tturn[0], p->tturn[1], p->tturn[2]), p->at);
        }
        quad(&r[n], 0x80808080u, q);
        r[n++].frame = p->frame;
    }
    sprites_records(s->batch, n);
}

/* ---- LoopingSprite: one grey sprite 1.6 across, half seen, looping its 4 frames (cell (0x40, 0)),
 * each loop at a new turn; moved by a room's command (`particles_place`) ---- */

int particles_loop_sprite(Vec3 at) {
    int h = new_system(LOOPSPRITE, at);
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0x40, 0, 0x20, 0x20, 4, 0);
    s->n = 1;
    s->p[0].at = at;
    colour(&s->p[0], 0x80, 0x80, 0x80, 0x40);
    square(&s->p[0], 1.6f);
    s->p[0].turn = 2.0f * PI * half();
    return h;
}

static void loop_sprite_step(System *s) {   /* LoopingSprite_Update */
    Part *p = &s->p[0];

    if (++p->frame >= 4) {
        p->frame = 0;
        p->turn = 2.0f * PI * half();
    }
}

/* ---- ObjectGlow: a disc of light on the floor under one of water-tower-7f-1's planets (its radius
 * by the planet, a white middle fading to nothing), fading in to $40 by 2 a frame while on and
 * out while off - at nothing its sound 2 and it is gone; with 8 faint blue strips 40 high turning
 * round it ($FF: gone at once). The first planet's bloom: not drawn ---- */

static const float kGlowRadius[8] = {4.82f, 4.0f, 3.99f, 4.92f, 4.41f, 3.94f, 3.47f, 9.64f};

int particles_object_glow(Vec3 at, int idx) {
    int h = new_system(OBJGLOW, vec3(at.x, -1.0f, at.z));
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    sprites_texture(s->batch, -1, 0, -1);
    sprites_flags(s->batch, SPRITE_ADD | SPRITE_CORNERS, 0x19);
    s->batch2 = sprites_new();   /* (the disc: into the bloom's mask) */
    if (s->batch2 >= 0) {
        sprites_texture(s->batch2, -1, 0, -1);
        sprites_flags(s->batch2, SPRITE_MASK | SPRITE_CORNERS, 0x26);
    }
    s->kind = idx;
    s->command = 1;
    s->top = 0;        /* (its alpha) */
    s->floor = 0.0f;   /* (the strips' phase, degrees) */
    return h;
}

static int object_glow_step(System *s) {   /* ObjectGlow_Update */
    if (s->command == 0xFF) {
        return 0;
    }
    if (s->command) {
        s->top = s->top < 0x40 ? s->top + 2 : 0x40;
    } else if ((s->top -= 2) <= 0) {
        engine_sound_at(2, 6, vec3(s->at.x, s->at.y + 1.0f, s->at.z));
        return 0;
    }
    s->floor += 2.0f;
    if (s->floor >= 45.0f) {
        s->floor -= 45.0f;
    }
    return 1;
}

static void object_glow_write(System *s) {   /* ObjectGlow_Draw */
    static const uint32_t kStrip[4] = {0x00202010u, 0x00004000u, 0x00002000u, 0x00001000u};
    Sprite *r = sprites_records(s->batch, -1), *m = s->batch2 >= 0 ? sprites_records(s->batch2, -1) : NULL;
    float rad = s->kind >= 0 && s->kind < 8 ? kGlowRadius[s->kind] : 0.0f, r2 = 5.0f + rad;
    Vec3 c = s->at;
    int i, k, n = 0;

    if (s->kind == 0) {   /* (the first: the screen brightened through the mask too) */
        post_bloom(0x40808080u, 0);
    }
    for (i = 0; m != NULL && i < 16; i++) {   /* (the disc: a fan, marking the bloom's mask) */
        float a = deg(22.5f * (float)i), b = deg(22.5f * (float)(i + 1));

        glow_tri(&m[i], c, vec3(c.x + rad * sinf(a), c.y, c.z - rad * cosf(a)), vec3(c.x + rad * sinf(b), c.y, c.z - rad * cosf(b)),
                 0x80808000u | (uint32_t)s->top, 0x80808000u);
    }
    if (m != NULL) {
        sprites_records(s->batch2, 16);
    }
    for (i = 0; i < 8; i++) {   /* (the strips: the middle and a point round it, up 40) */
        float a = deg(45.0f * (float)i + s->floor);
        Vec3 o = vec3(c.x + r2 * sinf(a), c.y, c.z - r2 * cosf(a));
        Vec3 q[4] = {c, o, vec3(c.x, c.y + 40.0f, c.z), vec3(o.x, o.y + 40.0f, o.z)};
        Sprite *x = &r[n++];

        quad(x, 0, q);
        x->shaded = 1;
        for (k = 0; k < 4; k++) {
            x->crgba[k][0] = (uint8_t)(kStrip[k] >> 24);
            x->crgba[k][1] = (uint8_t)(kStrip[k] >> 16);
            x->crgba[k][2] = (uint8_t)(kStrip[k] >> 8);
            x->crgba[k][3] = (uint8_t)kStrip[k];
        }
    }
    sprites_records(s->batch, n);
}

/* ---- Glint: a glint on a character - a white sprite 1 across at its bone 6 (the old mansion
 * pool's fish $14) ---- */

int particles_glint(int actor) {
    int h = new_system(GLINT, vec3(0, 0, 0));

    if (h < 0) {
        return -1;
    }
    look(&sSys[h], 0x10, 1, -1, 0x20, 0x40, 0x20, 0x20, 1, 0);
    sprites_flags(sSys[h].batch, 0, 0x26);
    sSys[h].kind = actor;
    return h;
}

static void glint_write(System *s) {
    const Actor *a = s->kind >= 0 && s->kind < MAX_ACTORS ? &gEngine.actors[s->kind] : NULL;
    Part p = {0};

    if (a == NULL || !a->used || !a->visible) {
        sprites_records(s->batch, 0);
        return;
    }
    p.at = a->bones[6];
    square(&p, 1.0f);
    colour(&p, 0x80, 0x80, 0x80, 0x80);
    sprite_rec(sprites_records(s->batch, 1), &p);
}

/* ---- Marker: the glow and sparks on a stalker (Marker_* and Spark_*; the house of truth's
 * Lorenzo). Its glow pulses orange (0.4 x (1 + sin a) x its size, a turning by up to 10 degrees
 * a frame; from size 1.5 a fixed orange): the shine of the characters in layer $14
 * (post_shine), and for the first kind a screen glow round the model's root (the original counts
 * spheres where they show: here a glow sprite). Each frame
 * two or three sparks start at the model's bones and drift off: each a glow and a flare quad,
 * growing, turning, fading 2 + up to 7 / size and darkening 1 / size a frame. The first kind:
 * a spark at any of 27 bones; the second: a few chosen bones, now and then one on a second
 * model (Fiona). `particles_command`: -1 done, -2 no sparks this frame, else its size in
 * thousandths ---- */

static const Actor *marker_model(int id) {
    const Actor *a = id >= 0 && id < MAX_ACTORS ? &gEngine.actors[id] : NULL;

    return a != NULL && a->used && a->visible ? a : NULL;
}

static Part *marker_free_part(System *s) {
    int i;

    for (i = 0; i < s->n; i++) {
        if (s->p[i].rgba[3] <= 0) {
            return &s->p[i];
        }
    }
    return s->n < MAX_PARTS ? &s->p[s->n++] : NULL;
}

/* one quad of a spark: white at `alpha`, `w` across, growing up to `grow` a frame, drifting `v`,
 * faded by `fade`, turning the way `way` points */
static void marker_quad(System *s, Vec3 at, int alpha, float w, float grow, Vec3 v, float fade, float way, int flicker) {
    Part *p = marker_free_part(s);
    float k = rnd();

    if (p == NULL) {
        return;
    }
    memset(p, 0, sizeof(*p));
    colour(p, 0x80, 0x80, 0x80, alpha);
    p->at = at;
    p->v = v;
    square(p, w);
    p->turn = deg(360.0f * half());
    p->radius = grow;
    p->rise = fade;
    p->angle = way;
    if (flicker) {   /* (Spark_ToBone: dimmed, thrown and grown by a random amount) */
        colour(p, (int)(p->rgba[0] * k), (int)(p->rgba[1] * k), (int)(p->rgba[2] * k), (int)(p->rgba[3] * k));
        p->at = vec3_add(p->at, vec3_scale(v, 20.0f * k));
        square(p, p->w + k);
    }
}

/* a spark at bone `bone` of model `id` (Spark_SetParams): small ones (size 0.49 and under) a
 * faint glow and a tiny rising flare */
static void marker_spark(System *s, int id, int bone, float scale, int flicker) {
    const Actor *a = marker_model(id);
    float size = s->c.x, fade = scale * size;
    Vec3 at, gv, fv;

    if (a == NULL) {
        return;
    }
    at = a->bones[clampi(bone, 0, MODEL_MAX_BONES - 1)];
    if (fade < 0.1f) {
        fade = 0.1f;
    }
    if (size <= 0.49f) {
        gv = vec3(0.1f * half(), 0.1f, 0.1f * half());
        fv = vec3(0.1f * half(), 0.4f * (0.5f - fade), 0.1f * half());
        marker_quad(s, at, 0x40, 1.0f + rnd(), 0.1f, gv, 1.0f, gv.x, flicker);
        marker_quad(s, at, 0x80, 0.2f + 0.2f * rnd(), 0.2f, fv, 1.0f, gv.x, flicker);
        return;
    }
    gv = vec3(0.2f * half(), 0.25f, 0.2f * half());
    fv = vec3(0.1f * half(), 0.1f, 0.1f * half());
    marker_quad(s, at, size < 1.0f ? (int)(0x80 * size) : 0x80, 0.5f + 0.5f * rnd(), 0.1f, gv, fade, gv.x, flicker);
    marker_quad(s, at, size < 1.0f ? (int)(0x80 * size) : 0x80, 1.0f + rnd(), 0.2f, fv, fade, gv.x, flicker);
}

/* a frame's sparks (Marker_Sparks) */
static void marker_sparks(System *s, int flicker) {
    int k;

    if (!s->slow) {   /* (the first kind: one anywhere; another on a few bones, half again as big) */
        marker_spark(s, s->kind, irnd(27), 1.0f, flicker);
        if (irnd(2) == 0) {
            k = irnd(4);
            marker_spark(s, s->kind, k < 2 ? k + 6 : k, 1.5f, flicker);
        } else {
            k = irnd(4) + 3;
            k = k == 6 ? 5 : k * k;
            marker_spark(s, s->kind, k, k < 11 ? 1.5f : 1.0f, flicker);
        }
        return;
    }
    k = irnd(4) + 3;   /* (the second: bones 9, 20, 25 or 5; 6, 7, 2 or 3) */
    marker_spark(s, s->kind, k == 6 ? 5 : k == 4 ? 20 : k * k, 1.0f, flicker);
    k = irnd(4);
    marker_spark(s, s->kind, k < 2 ? k + 6 : k, 1.0f, flicker);
    if (irnd(2)) {   /* (now and then one on the other model; else one anywhere but 13..16) */
        marker_spark(s, s->top, irnd(4) + 2, 1.0f, flicker);
        return;
    }
    k = irnd(27);
    if (k < 13 || k >= 17) {
        marker_spark(s, s->kind, k, 1.0f, flicker);
    }
}

int particles_marker(int model, int other, float size, int second) {
    int h = new_system(MARKER, vec3(0, 0, 0)), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    if (second) {   /* (the other cells: 64 x 64 at (320, 160), palette 7) */
        look(s, 0x10, 1, 7, 0x140, 0xA0, 0x40, 0x40, 1, SPRITE_ADD);
    } else {
        look(s, 0x10, 1, 6, 0x180, 0x80, 0x20, 0x20, 1, SPRITE_ADD);
    }
    s->batch2 = sprites_new();   /* (the glow) */
    if (s->batch2 >= 0) {
        sprites_texture(s->batch2, 0x10, 1, -1);
        sprites_cells(s->batch2, 0xA0, 0x40, 32, 32, 512, 256, 1);
        sprites_flags(s->batch2, SPRITE_ADD, 0x19);
    }
    s->kind = model;
    s->top = other;
    s->slow = second;
    s->c.x = size;
    for (i = 0; i < 20; i++) {   /* (twenty rounds to start, flickering) */
        marker_sparks(s, 1);
    }
    return h;
}

/* a spark's quad's step (Spark_Update): out once black */
static void marker_quad_step(Part *p) {
    int dim = (int)(1.0f / p->rise), k;

    square(p, p->w + p->radius * rnd());
    p->turn += (p->angle > 0.0f ? 1.0f : -1.0f) * deg(3.0f * rnd());
    p->at = vec3_add(p->at, p->v);
    p->rgba[3] = clampi(p->rgba[3] - ((int)((float)irnd(8) / p->rise) + 2), 0, 255);
    for (k = 0; k < 3; k++) {
        p->rgba[k] = clampi(p->rgba[k] - dim, 0, 255);
    }
    if (p->rgba[0] + p->rgba[1] + p->rgba[2] == 0) {
        p->rgba[3] = 0;
    }
}

static int marker_step(System *s) {   /* Marker_Update */
    int i;

    for (i = 0; i < s->n; i++) {
        if (s->p[i].rgba[3] > 0) {
            marker_quad_step(&s->p[i]);
        }
    }
    if (s->ended) {   /* (done: its sparks go with it) */
        return 0;
    }
    s->tilt = wrap(s->tilt + deg(10.0f * rnd()));
    if (s->command != -2 && s->c.x > 0.0f) {
        marker_sparks(s, 0);
    }
    s->command = 0;
    return 1;
}

static void marker_write(System *s) {
    Sprite *r = sprites_records(s->batch, s->n);
    const Actor *a = marker_model(s->kind);
    float v = 0.4f * (1.0f + sinf(s->tilt));
    int i, c;

    for (i = 0; i < s->n; i++) {
        sprite_rec(&r[i], &s->p[i]);
    }
    if (s->batch2 < 0) {
        return;
    }
    if (a == NULL) {
        sprites_records(s->batch2, 0);
        return;
    }
    if (s->slow) {   /* (the second kind: no screen glow, the shine only) */
        sprites_records(s->batch2, 0);
    }
    if (s->c.x < 1.0f) {
        v *= s->c.x;
    }
    c = (int)(128.0f * v);
    /* (renderer +0x78: layer $14's shine - the stalker's own - in the pulsing orange) */
    post_shine(s->c.x > 1.5f ? 0x80400080u : (uint32_t)c << 24 | (uint32_t)(c >> 1) << 16 | (uint32_t)c);
    if (s->slow) {
        return;
    }
    r = sprites_records(s->batch2, 1);
    memset(r, 0, sizeof(*r));
    r->x = a->bones[0].x;
    r->y = a->bones[0].y;
    r->z = a->bones[0].z;
    r->w = r->h = s->slow ? 12.0f : 40.0f;
    r->rgba[0] = (uint8_t)(s->c.x > 1.5f ? 0x80 : c);
    r->rgba[1] = (uint8_t)(s->c.x > 1.5f ? 0x40 : c >> 1);
    r->rgba[2] = 0;
    r->rgba[3] = (uint8_t)(s->c.x > 1.5f ? 0x80 : c);
}

/* ---- WindowFlash: a dial's lens catching the light (castle 1F's dials; the rooms' obj_effect) -
 * a ring 0.1 out from the object (half its outline kept, mirrored for points 12..23), turned
 * with it, five frames of fans through more and then less of it, marking the bloom's mask, the
 * screen brightened through it (white) ---- */

static const float kWindowOutline[12][3] = {
    {0.000f, 3.239f, 3.450f}, {-0.564f, 3.608f, 3.450f}, {-0.368f, 3.454f, 3.450f}, {0.167f, 4.053f, 2.921f},
    {0.480f, 3.946f, 2.960f}, {0.736f, 3.744f, 3.033f}, {0.902f, 3.473f, 3.132f}, {0.960f, 3.164f, 3.244f},
    {0.902f, 2.855f, 3.356f}, {0.736f, 2.584f, 3.455f}, {0.480f, 2.383f, 3.529f}, {0.167f, 2.166f, 3.607f},
};
static const int8_t kWindowFans[5][21] = {   /* (D_00444A70: a count, then the points) */
    {6, 1, 16, 17, 18, 19, 20},
    {16, 0, 3, 4, 5, 6, 2, 12, 13, 14, 15, 16, 17, 18, 19, 20, 3},
    {20, 0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 3},
    {16, 0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 1, 3},
    {6, 2, 7, 8, 9, 10, 11},
};

int particles_window_flash(Vec3 at, float turn) {
    int h = new_system(WINDOWFLASH, at);

    if (h < 0) {
        return -1;
    }
    sprites_texture(sSys[h].batch, -1, 0, -1);   /* (untextured) */
    sprites_flags(sSys[h].batch, SPRITE_MASK | SPRITE_CORNERS, 0x26);
    sSys[h].floor = turn;
    sSys[h].timer = -1;
    return h;
}

static int window_flash_step(System *s) {   /* WindowFlash_Update: frames 0..4, then gone */
    return ++s->timer < 5;
}

static Vec3 window_point(const System *s, int id) {
    Vec3 v = id < 12 ? vec3(kWindowOutline[id][0], kWindowOutline[id][1], 0.1f + kWindowOutline[id][2])
                     : vec3(-kWindowOutline[23 - id][0], kWindowOutline[23 - id][1], 0.1f + kWindowOutline[23 - id][2]);

    return vec3_add(turn_xyz(v, 0.0f, s->floor, 0.0f), s->at);
}

static void window_flash_write(System *s) {   /* WindowFlash_Draw: the fan */
    const int8_t *fan = kWindowFans[clampi(s->timer, 0, 4)];
    Sprite *r = sprites_records(s->batch, fan[0] - 2);
    int k;

    post_bloom(0x80808080u, 0);   /* (the screen brightened through it) */
    for (k = 1; k + 1 < fan[0]; k++) {
        glow_tri(&r[k - 1], window_point(s, fan[1]), window_point(s, fan[k + 1]), window_point(s, fan[k + 2]), 0xFFFFFF80u, 0xFFFFFF80u);
    }
}

/* ---- SmokeTrail: smoke rising from a thing (the house of truth's traps) - 32 puffs, 3 across, from
 * 2 above it, drifting up to 0.2 a frame each way (0.2..0.6 up) and turning a degree, fading
 * (alpha 1, colour 2 a frame) and narrowing 0.05; a faded puff starts again at the thing while it
 * is there. Once it is gone the rest fade out ---- */

static void trail_puff(System *s, int i, int stagger) {   /* SmokeTrail_Puff */
    Part *p = &s->p[i];
    const Vec3 *at = things_at(s->kind);

    colour(p, 0x80, 0x80, 0x80, stagger ? 0x20 - i : 0x20);
    p->at = at != NULL ? vec3(at->x, at->y + 2.0f, at->z) : s->at;
    square(p, 3.0f);
    p->turn = deg(180.0f * rnd());
    p->v = vec3(-0.2f + 0.4f * rnd(), 0.2f + 0.4f * rnd(), -0.2f + 0.4f * rnd());
}

int particles_smoke_trail(int thing) {
    int h, i;

    if (!things_used(thing)) {
        return -1;
    }
    h = new_system(SMOKETRAIL, *things_at(thing));
    if (h < 0) {
        return -1;
    }
    look(&sSys[h], 0x10, 1, 6, 0x180, 0x80, 0x20, 0x20, 1, 0);
    sSys[h].kind = thing;
    sSys[h].n = 32;
    for (i = 0; i < 32; i++) {
        trail_puff(&sSys[h], i, 1);
    }
    return h;
}

static int smoke_trail_step(System *s) {   /* SmokeTrail_Update */
    int i, done = 1;

    if (!s->ended && (!things_used(s->kind) || things_spent(s->kind))) {
        s->ended = 1;
    }
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        if (p->rgba[3] > 0) {
            colour(p, p->rgba[0] - 2, p->rgba[1] - 2, p->rgba[2] - 2, p->rgba[3] - 1);
            done = 0;
            if (!s->ended && p->rgba[3] == 0) {
                trail_puff(s, i, 0);
            }
        }
        p->w -= 0.05f;   /* (only its width, as the original) */
        p->turn = wrap(p->turn + deg(1.0f));
        p->at = vec3_add(p->at, p->v);
    }
    return !(s->ended && done);
}

/* ---- the magnesias' bursts (BurstA: the refined magnesia's; BurstB: the prima's, three across
 * the view - at it and 5 either side). Each point: a flash standing on the ground (playing its
 * 10 frames, fading), a ring flat on the floor spreading out and fading fast, pieces thrown out
 * (their push growing for 4 frames, then falling back, dimming and fading) and puffs rising and
 * sinking back (gone under the floor). Added, in the burst's colour; gone once the pieces,
 * flashes and rings are ---- */

#define BURST_POINTS 3
enum { BURST_PIECES_A = 16, BURST_PUFFS_A = 12, BURST_PIECES_B = 32, BURST_PUFFS_B = 32 };

typedef struct BurstLayout {   /* where each part's records are in the system's parts */
    int points, pieces, puffs;
} BurstLayout;

static BurstLayout burst_layout(const System *s) {
    BurstLayout l = {1, BURST_PIECES_A, BURST_PUFFS_A};

    if (s->kind) {   /* (the prima's: three of everything) */
        l.points = BURST_POINTS;
        l.pieces = BURST_PIECES_B;
        l.puffs = BURST_PUFFS_B;
    }
    return l;
}
/* the parts, in order: pieces, puffs, flashes, rings */
static Part *burst_piece(System *s, int i) { return &s->p[i]; }
static Part *burst_puff(System *s, int i) { return &s->p[burst_layout(s).pieces + i]; }
static Part *burst_flash(System *s, int i) { BurstLayout l = burst_layout(s); return &s->p[l.pieces + l.puffs + i]; }
static Part *burst_ring(System *s, int i) { BurstLayout l = burst_layout(s); return &s->p[l.pieces + l.puffs + l.points + i]; }

static void burst_colour(const System *s, Part *p, int blue, int alpha) {
    if (s->kind) {
        colour(p, 0x7F, 0x41, blue, alpha);
    } else {
        colour(p, 0x48, 0x30, 0x10, alpha);
    }
}

int particles_mag_burst(Vec3 at, int prima, Vec3 floor_up) {
    int h = new_system(MAGBURST, at), i;
    Vec3 right = vec3(cosf(gEngine.camera.yaw), 0.0f, sinf(gEngine.camera.yaw));
    static const float kAcross[BURST_POINTS] = {0.0f, 5.0f, -5.0f};
    Vec3 pts[BURST_POINTS];
    BurstLayout l;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    s->kind = prima != 0;
    l = burst_layout(s);
    look(s, 0x10, 1, -1, 0x20, 0x40, 0x20, 0x20, 1, SPRITE_ADD);   /* (the pieces) */
    s->batch2 = sprites_new();   /* (the puffs) */
    s->batch3 = sprites_new();   /* (the flashes) */
    s->batch4 = sprites_new();   /* (the rings) */
    if (s->batch2 < 0 || s->batch3 < 0 || s->batch4 < 0) {
        end(s);
        return -1;
    }
    sprites_texture(s->batch2, 0x10, 1, -1);
    sprites_cells(s->batch2, 0xE, 0x6E, 4, 4, 512, 256, 1);
    sprites_flags(s->batch2, SPRITE_ADD, 0x19);
    sprites_texture(s->batch3, 0x10, 1, -1);
    sprites_cells(s->batch3, 0, 0xA0, 0x20, 0x40, 512, 256, 10);
    sprites_flags(s->batch3, SPRITE_ADD | SPRITE_UPRIGHT, 0x19);
    sprites_offset(s->batch3, 0.0f, -1.0f);   /* (standing on the ground) */
    sprites_texture(s->batch4, 0x10, 1, -1);
    sprites_cells(s->batch4, 0xC0, 0x60, 0x20, 0x20, 512, 256, 1);
    sprites_flags(s->batch4, SPRITE_ADD | SPRITE_CORNERS, 0x19);
    s->floor = at.y;
    s->c = vec3_norm(floor_up);
    s->n = l.pieces + l.puffs + 2 * l.points;
    for (i = 0; i < l.points; i++) {
        Part *f = burst_flash(s, i), *r = burst_ring(s, i);

        pts[i] = vec3_add(at, vec3_scale(right, kAcross[i]));
        burst_colour(s, f, 0x1E, 0x80);
        f->at = pts[i];
        f->w = s->kind && i == 0 ? 12.0f : 9.0f;
        f->h = s->kind && i == 0 ? 17.0f : 14.0f;
        burst_colour(s, r, 0x1E, 0x80);
        r->radius = s->kind ? 1.0f : 2.0f;
        r->at = vec3_add(pts[i], vec3_scale(s->c, 0.1f));   /* (just off the floor) */
    }
    s->tilt = s->kind ? 1.0f : 2.0f;   /* (the rings' size: one for all of them, as the original keeps) */
    for (i = 0; i < l.pieces; i++) {   /* (thrown out: their push, its fall back, a sideways drift) */
        Part *p = burst_piece(s, i);
        Vec3 c = pts[i % l.points];
        float back = 0.1f + 0.05f * rnd();

        burst_colour(s, p, 0x14, 0x40 + irnd(0x40));
        p->at = vec3(c.x + 4.0f * half(), c.y + 1.0f, c.z + 4.0f * half());
        square(p, 4.0f + 4.0f * rnd());
        p->turn = rnd_turn();
        p->v = vec3(2.0f * half(), 0.1f + 0.1f * rnd(), 2.0f * half());
        p->pull = vec3(-p->v.x * back, -0.5f * p->v.y * back, -p->v.z * back);
        p->rise = p->v.x / 8.0f;    /* (the drift: x) */
        p->angle = p->v.z / 8.0f;   /* (z) */
    }
    for (i = 0; i < l.puffs; i++) {   /* (rising, slowed by their rate, sinking back) */
        Part *p = burst_puff(s, i);
        Vec3 c = pts[i % l.points];

        burst_colour(s, p, 0x1E, i % 2 == 0 ? 0x40 : 0x80);
        p->at = vec3(c.x + 2.0f * half(), c.y + 1.0f, c.z + 2.0f * half());
        square(p, s->kind ? 0.1f + 0.65f * rnd() : 0.1f + 0.5f * rnd());
        p->v = vec3(2.0f * half(), 3.5f * (0.1f + rnd()) - 0.5f, 2.0f * half());
        p->radius = 0.02f + 0.1f * p->v.y;
    }
    return h;
}

static int mag_burst_step(System *s) {   /* burst_update */
    BurstLayout l = burst_layout(s);
    int i, k, going = 0;

    for (i = 0; i < l.pieces; i++) {
        Part *p = burst_piece(s, i);

        if (p->rgba[3] <= 0) {
            continue;
        }
        going = 1;
        p->turn = wrap(p->turn + deg(2.0f * rnd()));
        p->at.x += p->v.x + p->rise;
        p->at.z += p->v.z + p->angle;
        if (s->timer < 4) {   /* (the first frames: the drift grows by half) */
            p->rise += p->rise / 2.0f;
            p->angle += p->angle / 2.0f;
            continue;
        }
        for (k = 0; k < 3; k++) {
            p->rgba[k] = p->rgba[k] > 0 ? p->rgba[k] - 1 : 0;
        }
        p->rgba[3] = clampi(p->rgba[3] - (irnd(8) + 1), 0, 255);
        if (s->timer < 8) {
            p->v = vec3_add(p->v, p->pull);
        }
        p->at.y += (i % 2 == 0 ? 4.0f : 8.0f) * p->v.y;
        p->rise = p->angle = 0.0f;
    }
    s->timer++;
    for (i = 0; i < l.points; i++) {
        Part *f = burst_flash(s, i), *r = burst_ring(s, i);

        if (f->rgba[3] > 0) {
            going = 1;
            f->rgba[3]--;
            if (++f->frame >= 10) {
                f->rgba[3] = 0;
                f->frame = 0;
            }
        }
        if (r->rgba[3] > 0) {   /* (the shared size grows for each ring still showing: each a step bigger) */
            going = 1;
            r->rgba[3] = clampi(r->rgba[3] - 13, 0, 255);
            s->tilt += i == 0 ? 1.0f : 0.5f;
            r->radius = s->tilt;
        }
    }
    for (i = 0; i < l.puffs; i++) {
        Part *p = burst_puff(s, i);

        p->at = vec3_add(p->at, p->v);
        p->v.y -= p->radius;
        if (p->at.y < s->floor) {
            p->rgba[3] = 0;
            continue;
        }
        p->rgba[3] = clampi(p->rgba[3] - (irnd(8) + 1), 0, 255);
    }
    return going;
}

static void mag_burst_write(System *s) {
    BurstLayout l = burst_layout(s);
    Vec3 right = vec3(cosf(gEngine.camera.yaw), 0.0f, sinf(gEngine.camera.yaw));
    Vec3 ahead = vec3(sinf(gEngine.camera.yaw), 0.0f, -cosf(gEngine.camera.yaw));
    Vec3 up = s->c, across, along;
    Sprite *r;
    int i, k;

    /* (the floor's own axes: level, the camera's) */
    across = fabsf(up.y) >= 0.999f ? right : vec3_norm(vec3_cross(up, vec3(0.0f, -1.0f, 0.0f)));
    along = fabsf(up.y) >= 0.999f ? ahead : vec3_norm(vec3_cross(up, across));
    r = sprites_records(s->batch, l.pieces);
    for (i = 0; i < l.pieces; i++) {
        sprite_rec(&r[i], burst_piece(s, i));
    }
    r = sprites_records(s->batch2, l.puffs);
    for (i = 0; i < l.puffs; i++) {
        sprite_rec(&r[i], burst_puff(s, i));
    }
    r = sprites_records(s->batch3, l.points);
    for (i = 0; i < l.points; i++) {
        sprite_rec(&r[i], burst_flash(s, i));
    }
    r = sprites_records(s->batch4, l.points);
    for (i = 0; i < l.points; i++) {   /* (flat, 1.5 x its size across the view, its size along it) */
        const Part *p = burst_ring(s, i);
        Vec3 q[4];

        for (k = 0; k < 4; k++) {
            float x = (k & 1 ? 1.5f : -1.5f) * p->radius, z = (k & 2 ? 1.0f : -1.0f) * p->radius;

            q[k] = vec3_add(p->at, vec3_add(vec3_scale(across, x), vec3_scale(along, z)));
        }
        quad(&r[i], (uint32_t)clampi(p->rgba[0], 0, 255) << 24 | (uint32_t)clampi(p->rgba[1], 0, 255) << 16 |
                        (uint32_t)clampi(p->rgba[2], 0, 255) << 8 | (uint32_t)clampi(p->rgba[3], 0, 255), q);
        r[i].frame = 0;
    }
}

/* ---- SinkingSprite: a butterfly's mote of dust - slowly growing and sinking, fading 0..3 a
 * frame; gone the frame after it has faded out ---- */

static int mote_new(int owner, uint32_t rgba, Vec3 at, Vec3 v) {
    int h = new_system(MOTE, at);
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 1, -1, 0x40, 0x40, 32, 32, 1, 0x20);
    s->owner = owner;
    s->engines = 1;
    s->n = 1;
    s->p[0].at = at;
    s->p[0].v = v;
    colour(&s->p[0], (int)(rgba >> 24), (int)(rgba >> 16 & 0xFF), (int)(rgba >> 8 & 0xFF), (int)(rgba & 0xFF));
    square(&s->p[0], 0.2f * rnd() + 0.2f);
    return h;
}

static int mote_step(System *s) {   /* SinkingSprite_Update */
    Part *p = &s->p[0];

    if (s->ended) {
        return 0;
    }
    s->ended = 1;
    if (p->rgba[3] > 0) {
        s->ended = 0;
        p->rgba[3] = clampi(p->rgba[3] - irnd(4), 0, 255);
        p->v.y -= 0.025f;
        if (p->v.y < -0.05f) {
            p->v.y = 0.02f * half() - 0.05f;
        }
        p->at = vec3_add(p->at, p->v);
        grow(p, 0.001f);
    }
    return 1;
}

/* ---- Butterflies: circling a centre that makes for a target, while a point they follow
 * wanders about it; bobbing, banking, flapping; now and then one sheds a mote. Two wings each
 * (a 40 x 64 cell of GAME_FIX's first sheet). (Their size factor scales a homogeneous point on
 * the PS2 - no change - so all are one size.) ---- */

int particles_butterflies(Vec3 to, int count, const uint8_t *colours) {
    int h = new_system(BUTTERFLIES, to), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    look(s, 0x10, 0, 0, 0, 0, 40, 64, 1, 2);
    sprites_flags(s->batch, 2, 1);
    for (i = 0; i < 3; i++) {   /* Butterflies_Start */
        s->ph[i] = rnd() * 2.0f * PI - PI;
        s->step[i] = wrap(rnd() * PI + PI / 2.0f);
        s->circ[i] = rnd() * 2.0f + 2.0f;
    }
    s->count = 8;
    for (i = 0; i < 8; i++) {   /* the table's GS words: R in the low byte */
        uint32_t gs = colours != NULL ? (uint32_t)colours[i * 4] | (uint32_t)colours[i * 4 + 1] << 8 |
                                            (uint32_t)colours[i * 4 + 2] << 16 | (uint32_t)colours[i * 4 + 3] << 24
                                      : 0;

        s->wing[i] = gs == 0 ? 0x80808080u : (gs & 0xFF) << 24 | (gs >> 8 & 0xFF) << 16 | (gs >> 16 & 0xFF) << 8 | gs >> 24;
    }
    particles_butterflies_to(h, to, count);
    return h;
}

void particles_butterflies_to(int h, Vec3 to, int count) {   /* Butterflies_SetParams */
    System *s = get(h);

    if (s == NULL || s->what != BUTTERFLIES) {
        return;
    }
    if (count != 0) {
        s->count = count;
    }
    s->target = to;
    if (!s->started) {
        s->started = 1;
        s->c = s->wander = to;
    }
}

static void butterflies_step(System *s) {   /* Butterflies_Update */
    Vec3 d = vec3_sub(s->target, s->c);
    float d2 = d.x * d.x + d.y * d.y + d.z * d.z, tilt, side, speed;

    if (d2 > 1.0f) {   /* the centre makes for the target, 0.4 a frame */
        s->c = vec3_add(s->c, vec3_scale(d, 0.4f / sqrtf(d2)));
    } else if (d2 > 0.0f) {
        s->c = s->target;
    }
    tilt = deg(rnd() * 10.0f - 5.0f);   /* banking, kept within 10 degrees */
    if (fabsf(tilt + s->tilt) > 0.17453f) {
        tilt = -tilt;
    }
    s->tilt += tilt;
    /* the point they follow turns toward the centre and wanders on */
    side = wrap(atan2f(s->c.x - s->wander.x, s->c.z - s->wander.z) - s->head) > 0.0f ? 1.0f : -1.0f;
    s->head = wrap(s->head + side * deg(rnd() * 20.0f + 15.0f));
    speed = 0.4f * rnd() + 0.4f;
    s->wander.x += speed * sinf(s->head);
    s->wander.z += speed * cosf(s->head);
    s->ph[0] = wrap(s->ph[0] + rnd() * 20.0f + 1.0472f);
    s->ph[1] = wrap(s->ph[1] + deg(rnd() * 9.0f + 2.0f));
    s->ph[2] = wrap(s->ph[2] + deg(rnd() * 20.0f + 40.0f));
}

/* a wing (side -1 left, 1 right): (0 0 1) (side*x y 1) (0 0 -0.4) (side*x y -0.4), turned, moved */
static void wing(Sprite *r, uint32_t rgba, float side, float wx, float wy, float tx, float ty, Vec3 at) {
    Vec3 q[4] = {vec3(0, 0, 1), vec3(side * wx, wy, 1), vec3(0, 0, -0.4f), vec3(side * wx, wy, -0.4f)};
    int k;

    for (k = 0; k < 4; k++) {
        q[k] = vec3_add(turn_xyz(q[k], tx, ty, 0.0f), at);
    }
    quad(r, rgba, q);
}

/* turned about y (sceVu0RotMatrixY: (0, 0, 1) to (sin a, 0, cos a)) */
static void turn_y(float *x, float *z, float a) {
    float c = cosf(a), s = sinf(a), nx = *x * c + *z * s;

    *z = -*x * s + *z * c;
    *x = nx;
}

static void butterflies_write(int h, System *s) {   /* Butterflies_Draw (and the motes it sheds) */
    Sprite *r = sprites_records(s->batch, -1);
    int i, n = 0;

    for (i = 0; i < s->count && n + 2 <= SPRITES_PER_BATCH; i++) {
        float sgn = (i & 1) ? 1.0f : -1.0f, ii = (float)(i * (i + 1));
        float a = wrap(ii * (s->step[0] + 0.92502f)) * sgn;
        float b = wrap(ii * (s->step[1] + 0.82030f)) * sgn;
        float c = wrap(ii * (s->step[2] + 1.16937f)) * sgn;
        float flap = sinf(wrap(a + s->ph[0] + s->step[0])) * 1.0472f;
        float bob = sinf(wrap(b + s->ph[1] + s->step[0])) * 0.8f + sinf(wrap(c + s->ph[2] + s->step[0])) * 0.4f;
        float turn = (float)(i + 1) * sgn * 0.87266f;
        float cx = s->circ[0], cz = s->circ[2], ox = s->wander.x - s->c.x, oz = s->wander.z - s->c.z;
        Vec3 at;

        turn_y(&cx, &cz, wrap(a));          /* on its circle about the centre */
        turn_y(&ox, &oz, wrap(turn));       /* and the wandering point, turned its own way */
        at = vec3(cx + s->c.x + ox, s->circ[1] + s->c.y + bob, cz + s->c.z + oz);
        wing(&r[n++], s->wing[i & 7], -1.0f, cosf(flap) * 0.7f, sinf(flap), s->tilt, wrap(turn + s->head), at);
        wing(&r[n++], s->wing[i & 7], 1.0f, cosf(flap) * 0.7f, sinf(flap), s->tilt, wrap(turn + s->head), at);
        if (flap < -1.0f && rnd() < 0.1f && s->count >= 2 && s->last != s->tilt && !gCamDir.scene) {   /* a mote shed
                                                                                          * (not while a cutscene has the camera) */
            uint32_t rgba = (uint32_t)(irnd(32) + 0x80) << 24 | (uint32_t)irnd(32) << 16 | (uint32_t)(irnd(32) + 0x80) << 8 |
                            (uint32_t)(irnd(16) + 0x70);

            s->last = s->tilt;
            mote_new(h, rgba, vec3(at.x, at.y + 0.5f * rnd(), at.z),
                     vec3((at.x - s->c.x) * 0.005f, 0.05f * rnd() + 0.05f, (at.z - s->c.z) * 0.005f));
        }
    }
    sprites_records(s->batch, n);
}

/* ---- a creature's vanishing (CreatureVanish): a glow (15 frames) and 8 sparks flung out that
 * then zig-zag, slow, shrink and fade ---- */

static void sprite_at(Sprite *r, Vec3 at, float size, const int rgba[4], int frame) {
    int k;

    memset(r, 0, sizeof(*r));
    r->x = at.x;
    r->y = at.y;
    r->z = at.z;
    r->w = r->h = size;
    r->frame = frame;
    for (k = 0; k < 4; k++) {
        r->rgba[k] = (uint8_t)clampi(rgba[k], 0, 255);
    }
}

static int vanish_new(Vec3 at, const int rgba[4]) {   /* CreatureVanish_Start, _SetParams */
    int h = new_system(VANISH, at), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    s->engines = 1;
    s->batch2 = sprites_new();
    look(s, 0x10, 1, -1, 0, 0x60, 32, 32, 15, SPRITE_ADD);   /* the glow */
    if (s->batch2 >= 0) {                                    /* the sparks */
        sprites_texture(s->batch2, 0x10, 1, -1);
        sprites_cells(s->batch2, 0x60, 0x40, 32, 32, 512, 256, 1);
        sprites_flags(s->batch2, SPRITE_ADD, 0x19);
    }
    colour(&s->p[0], rgba[0], rgba[1], rgba[2], rgba[3]);
    s->p[0].at = at;
    square(&s->p[0], 5.0f);
    s->n = 9;   /* the glow, then 8 sparks */
    for (i = 1; i <= 8; i++) {
        Part *p = &s->p[i];
        int dark = irnd(32);
        float sign = irnd(2) ? -1.0f : 1.0f, sign2;

        colour(p, rgba[0] - dark, rgba[1] - dark, rgba[2] - dark, rgba[3]);
        p->at = at;
        square(p, 4.0f);
        p->v.x = sign * rnd() * 0.25f * (float)(irnd(4) + 1) / 2.0f;   /* flung out, upward only */
        p->v.y = rnd() * 0.25f * (float)(irnd(4) + 1) / 2.0f;
        sign2 = irnd(2) ? -1.0f : 1.0f;
        p->v.z = sign2 * rnd() * 0.25f * (float)(irnd(4) + 1) / 2.0f;
        p->rise = 0.0125f * sign2 * (float)(irnd(4) + 1);             /* its zig-zag */
        p->radius = (0.005f + 0.025f * rnd()) / 2.0f;                  /* its drag (doubling) */
        p->angle = 0.005f + 0.02f * (float)irnd(8);                    /* its shrink */
        p->alpha0 = irnd(4) + 1;                                       /* its fade */
    }
    return h;
}

static int vanish_step(System *s) {   /* CreatureVanish_Update: 0 once all is done */
    int i, glow_done, shrunk = 0, faded = 0;

    if (++s->p[0].frame >= 15) {
        s->p[0].frame = 15;
        s->p[0].rgba[3] = 0;
    }
    glow_done = s->p[0].frame >= 15;
    for (i = 1; i <= 8; i++) {
        Part *p = &s->p[i];

        if (s->frames < 9) {   /* flung out, then zig-zagging (turning every 7..10 frames) */
            p->at.x += p->v.x;
            p->at.z += p->v.z;
        } else {
            p->at.x += p->rise;
            p->at.z += p->rise;
            if (s->frames % (irnd(4) + 7) == 0) {
                p->rise = -p->rise;
            }
        }
        if (s->frames >= 5) {   /* slowing */
            if (p->v.y > p->radius) {
                p->v.y = p->radius;
                p->radius += p->radius;
            }
            if (s->frames % 6 == 0) {
                p->radius += p->radius / 12.0f;
            }
        }
        p->at.y += p->v.y - p->radius;
        if (s->frames < 5) {
            p->radius += p->radius;
        }
        if (s->frames % 2 == 0) {
            square(p, p->w - p->angle);
        }
        if (p->w < 0.0f) {
            square(p, 0.0f);
            shrunk = 1;
        }
        p->rgba[3] = (int)((float)p->rgba[3] - (float)p->alpha0);
        if (p->rgba[3] < 0) {
            p->rgba[3] = 0;
            faded = 1;
        }
    }
    if (glow_done && shrunk && faded) {
        return 0;
    }
    s->frames++;
    return 1;
}

static void vanish_write(System *s) {
    Sprite *g = sprites_records(s->batch, 1);
    int i;

    sprite_at(&g[0], s->p[0].at, s->p[0].w, s->p[0].rgba, s->p[0].frame);
    if (s->batch2 >= 0) {
        Sprite *r = sprites_records(s->batch2, 8);

        for (i = 0; i < 8; i++) {
            sprite_at(&r[i], s->p[i + 1].at, s->p[i + 1].w, s->p[i + 1].rgba, 0);
        }
    }
}

/* ---- a spirit (CreatureA's look: _Bob, _Draw, _StateVanish) ---- */

int particles_vanish(Vec3 at, int r, int g, int b, int a) {   /* (CreatureVanish: a creature put away) */
    const int rgba[4] = {r, g, b, a};

    return vanish_new(at, rgba);
}

int particles_spirit(Vec3 at, int red, float bob) {
    int h = new_system(SPIRIT, at);
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    s->batch2 = sprites_new();
    s->batch3 = sprites_new();
    if (s->batch2 < 0 || s->batch3 < 0) {   /* (all three, or none) */
        end(s);
        return -1;
    }
    look(s, 0x10, 1, -1, 0x40, 0x40, 32, 32, 1, SPRITE_ADD | SPRITE_GLOW);   /* its core */
    {   /* its colour, its trail */
        sprites_texture(s->batch2, 0x10, 1, -1);
        sprites_cells(s->batch2, 0xA0, 0x40, 32, 32, 512, 256, 1);
        sprites_flags(s->batch2, SPRITE_ADD | SPRITE_GLOW, 0x19);
    }
    {   /* its body */
        sprites_texture(s->batch3, 0x10, 1, 3);
        sprites_cells(s->batch3, 0x60, 0x40, 32, 32, 512, 256, 1);
        sprites_flags(s->batch3, 0, 0x19);
    }
    s->red = red;
    s->bob = bob;
    s->bob_v = 0.5f;
    s->bob_a = 0.05f;
    s->period = 20;
    s->fade = 0x80;
    return h;
}

void particles_spirit_at(int h, Vec3 at, int shown) {
    System *s = get(h);

    if (s != NULL && s->what == SPIRIT) {
        s->at = at;
        s->shown = shown;
    }
}

void particles_spirit_vanish(int h, int seen) {
    System *s = get(h);
    int rgba[4];

    if (s == NULL || s->what != SPIRIT || s->vanishing) {
        return;
    }
    s->vanishing = 1;
    s->seen = seen;
    if (seen) {   /* a burst of light 12 above it, its motion stopped */
        rgba[0] = s->red ? 0x80 : 0x30;
        rgba[1] = 0x30;
        rgba[2] = s->red ? 0x30 : 0x80;
        rgba[3] = 0x60;
        vanish_new(vec3(s->at.x, s->at.y + 12.0f + s->bob, s->at.z), rgba);
        s->bob_v = s->bob_a = 0.0f;
    }
}

static void spirit_bob(System *s) {   /* CreatureA_Bob */
    if (!s->burst) {
        if (irnd(32) == 0) {   /* now and then a short burst: quick turns at 0.08 */
            s->burst = 1;
            s->saved_v = s->bob_v;
            s->saved_a = s->bob_a;
            s->saved_n = s->bob_n;
            s->burst_n = 0;
            s->bob_n = 0;
            s->bob_a = s->bob_a < 0.0f ? -0.08f : 0.08f;
            s->period = 5;
        }
    } else if (s->burst_n++ == s->period * 2) {
        s->burst = 0;
        s->bob_v = s->saved_v;
        s->bob_a = s->saved_a;
        s->bob_n = s->saved_n;
        s->burst_n = 0;
        s->period = 20;
    }
    if (s->bob_n % s->period == 0) {
        s->bob_a = -s->bob_a;
        s->bob_n = 0;
    }
    s->bob_n++;
    s->bob = s->bob > 5.0f ? 5.0f : s->bob <= -10.0f ? -10.0f : s->bob;
    s->glow += s->bob_a;
    s->bob += s->bob_v;
    s->bob_v += s->bob_a;
}

static int spirit_step(System *s) {
    Vec3 up = vec3(s->at.x, s->at.y + 12.0f + s->bob, s->at.z);
    int k;

    if (!(s->vanishing && s->seen)) {
        spirit_bob(s);
    }
    if (!s->trail_set) {   /* (its trail starts at it) */
        for (k = 0; k < 3; k++) {
            s->trail[k] = up;
        }
        s->trail_set = 1;
    }
    s->trail[(s->frames + s->bob_n) % 3] = up;
    s->frames++;
    if (!s->vanishing) {
        return 1;
    }
    if (s->seen && s->bob > -12.0f) {   /* sinking to 12 below, faster and faster */
        s->bob_a -= 0.1f;
        s->bob += s->bob_a;
        return 1;
    }
    if (s->seen) {
        s->bob = -12.0f;
    }
    s->fade -= 8;
    return s->fade > 0;
}

static void spirit_write(System *s) {   /* CreatureA_Draw: core, colour, three trail points, body */
    Vec3 up = vec3(s->at.x, s->at.y + 12.0f + s->bob, s->at.z);
    int white[4] = {0x80, 0x80, 0x80, 0x60}, body[4] = {0x80, 0x80, 0x80, 0x80}, own[4], c[4];
    int i;
    Sprite *r;

    if (!s->shown) {
        sprites_records(s->batch, 0);
        sprites_records(s->batch2, 0);
        sprites_records(s->batch3, 0);
        return;
    }
    own[0] = s->red ? 0x80 : 0;
    own[1] = 0;
    own[2] = s->red ? 0 : 0x80;
    own[3] = 0x60;
    r = sprites_records(s->batch, 1);
    sprite_at(&r[0], up, 2.5f, white, 0);
    if (s->vanishing) {
        r[0].rgba[3] = 0;
    }
    r = sprites_records(s->batch2, 4);
    sprite_at(&r[0], up, 2.5f + s->glow, own, 0);
    for (i = 2; i <= 4; i++) {   /* the trail: fainter and smaller each */
        c[0] = s->red ? 0x80 - i * 5 : (i + 1) * 0x10;
        c[1] = (i + 1) * 0x10;
        c[2] = s->red ? (i + 1) * 0x10 : 0x80 - i * 5;
        c[3] = 0x60 - i * 5;
        sprite_at(&r[i - 1], s->trail[i - 2], 2.5f - 0.5f * (float)(i - 1), c, 0);
        r[i - 1].h = 2.0f - 0.4f * (float)(i - 1);
    }
    if (s->vanishing) {
        for (i = 0; i < 4; i++) {
            r[i].rgba[3] = 0;
        }
    }
    r = sprites_records(s->batch3, 1);
    if (s->vanishing) {   /* its body in its colour, added */
        c[0] = s->red ? 0x80 : 0x30;
        c[1] = 0x30;
        c[2] = s->red ? 0x30 : 0x80;
        c[3] = 0x60;
        sprites_texture(s->batch3, 0x10, 1, -1);
        sprites_flags(s->batch3, SPRITE_ADD | SPRITE_GLOW, 0x19);
        sprite_at(&r[0], up, 2.5f, c, 0);
    } else {
        sprite_at(&r[0], up, 2.5f, body, 0);
    }
}

/* ---- Effect71000 (the scripts' 0xA9): a flat 2 x 2 quad wandering about its place - waiting,
 * then walking toward a target within its range (x, z), turned the way it goes; there, 1 in 8 a
 * new target (somewhere in the range, or back home) a tenth of the way a step, else a nudge ----
 * (its place `at`, its turn `c` (radians), its range `target`; `p[0].at` the offset, `p[0].v`
 * the step, `p[0].pull` where it heads, `rest` frames to wait) */

static int wander_wait(int quick) { return irnd(quick ? 2 : 4) * 30 + 1; }

int particles_wander(Vec3 at, float rx, float rz, Vec3 turn) {
    int h = new_system(WANDER, at);
    System *s;
    Part *p;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    sprites_texture(s->batch, 0x10, 0, 1);
    sprites_cells(s->batch, 0x40, 0, 0x40, 0x40, 512, 256, 1);
    sprites_flags(s->batch, 0, 0x19);
    s->c = turn;
    s->target = vec3(rx - 1.0f, 0.0f, rz - 1.0f);
    p = &s->p[0];
    p->v = vec3(0.5f * half(), 0.0f, 0.5f * half());   /* (Effect71000_Start) */
    s->rest = wander_wait(1);
    return h;
}

/* one axis: on by the step, kept within the range (a side reached becomes the target), stopping
 * at the target */
static void wander_axis(float *cur, float *to, float step, float range) {
    *cur += step;
    if (*cur >= range) {
        *cur = *to = range;
    } else if (*cur <= -range) {
        *cur = *to = -range;
    } else if (step > 0.0f ? *to - *cur < 0.0f : step < 0.0f && *to - *cur > 0.0f) {
        *cur = *to;
    }
}

static void wander_step(System *s) {   /* Effect71000_Update */
    Part *p = &s->p[0];

    if (s->rest > 0) {
        s->rest--;
        return;
    }
    if (p->pull.x == p->at.x && p->pull.z == p->at.z) {   /* there */
        if (irnd(8) == 0) {
            if (p->pull.x == 0.0f && p->pull.z == 0.0f) {
                p->pull.x = 2.0f * s->target.x * half();
                p->pull.z = 2.0f * s->target.z * half();
            } else {
                p->pull.x = p->pull.z = 0.0f;   /* home */
            }
            p->v.x = 0.1f * (p->pull.x - p->at.x);
            p->v.z = 0.1f * (p->pull.z - p->at.z);
        } else {
            s->rest = wander_wait(1);
            p->v.x += irnd(2) ? 1.0f : -1.0f;   /* a nudge to its facing */
            p->v.z += irnd(2) ? 1.0f : -1.0f;
        }
        return;
    }
    if (p->pull.x != p->at.x) {
        wander_axis(&p->at.x, &p->pull.x, p->v.x, s->target.x);
    }
    if (p->pull.z != p->at.z) {
        wander_axis(&p->at.z, &p->pull.z, p->v.z, s->target.z);
    }
    if (p->pull.x == p->at.x && p->pull.z == p->at.z) {
        s->rest = wander_wait(0);
    }
}

static void wander_write(System *s) {   /* Effect71000_Draw: on the floor, turned the way it steps */
    static const float kCorner[4][2] = {{1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, -1.0f}, {-1.0f, 1.0f}};
    const Part *p = &s->p[0];
    float len = sqrtf(p->v.x * p->v.x + p->v.z * p->v.z), yaw = 0.0f;
    Vec3 q[4];
    int k;

    if (len > 0.0f && p->v.z != 0.0f) {
        yaw = atanf(p->v.x / p->v.z) - PI / 2.0f;
        if (p->v.z < 0.0f) {
            yaw += PI;
        }
    }
    for (k = 0; k < 4; k++) {
        Vec3 c = vec3(kCorner[k][0], 0.0f, kCorner[k][1]);

        turn_y(&c.x, &c.z, yaw);
        c = vec3_add(c, vec3(p->at.x, 0.0f, p->at.z));
        q[k] = vec3_add(turn_xyz(c, s->c.x, s->c.y, s->c.z), s->at);
    }
    quad(sprites_records(s->batch, 1), 0x80808080u, q);
}

/* ---- the floor under a point: triangle t's plane at (x, z) ---- */

static const NavTri *nav_tri(int t) {
    const NavMesh *n = &gEngine.room.nav;

    return t >= 0 && t < n->ntris ? &n->tris[t] : NULL;
}
static float tri_floor(const NavTri *t, float x, float z, Vec3 *normal) {
    Vec3 a = vec3_sub(t->v[1], t->v[0]), b = vec3_sub(t->v[2], t->v[0]);
    Vec3 n = vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    float len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);

    if (len > 0.0f) {
        n = vec3(n.x / len, n.y / len, n.z / len);
    }
    if (n.y < 0.0f) {
        n = vec3(-n.x, -n.y, -n.z);
    }
    if (normal != NULL) {
        *normal = n;
    }
    if (fabsf(n.y) < 1e-4f) {
        return t->v[0].y;
    }
    return t->v[0].y - (n.x * (x - t->v[0].x) + n.z * (z - t->v[0].z)) / n.y;
}

/* ---- FloorSplat: a splat of blood (or a stain) laid on the floor where a drop fell, growing
 * and fading ---- */

static void splat_new(Vec3 at, int tri, int stain) {   /* FloorSplat_SetParams */
    const NavTri *t = nav_tri(tri);
    Vec3 n;
    int h;
    System *s;
    Part *p;

    if (t == NULL || (t->flags & 0x10000000)) {
        return;
    }
    h = new_system(SPLAT, at);
    if (h < 0) {
        return;
    }
    s = &sSys[h];
    s->engines = 1;
    s->kind = stain;
    s->n = 1;
    if (stain) {
        look(s, 0x10, 1, -1, 0x140, 0x80, 32, 32, 1, 0x42);
    } else {
        look(s, 0x10, 1, -1, 0x6C, 0x4C, 8, 8, 1, 2);
    }
    p = &s->p[0];
    if (stain) {
        colour(p, 0x40, 0x10, 8, 0x20);
    } else {
        colour(p, 0x20, 0, 0, irnd(32) + 0x20);
    }
    p->at = vec3(at.x, tri_floor(t, at.x, at.z, &n), at.z);
    p->at = vec3_add(p->at, vec3(0.5f * n.x, 0.5f * n.y, 0.5f * n.z));   /* (lifted half the normal) */
    p->pull = n;
    square(p, 2.0f);
}
static int splat_step(System *s) {   /* FloorSplat_Update */
    Part *p = &s->p[0];

    if (p->rgba[3] <= 0) {
        return 0;
    }
    if (s->kind) {
        if (irnd(4) == 1) {
            p->rgba[3] -= 1;
        }
    } else {
        p->rgba[3] -= irnd(4);
    }
    p->rgba[3] = clampi(p->rgba[3], 0, 255);
    square(p, p->w + 0.1f);
    return 1;
}
static void splat_write(System *s) {   /* in the floor's plane, round its normal */
    static const float kCorner[4][2] = {{-1.0f, -1.0f}, {1.0f, -1.0f}, {-1.0f, 1.0f}, {1.0f, 1.0f}};
    const Part *p = &s->p[0];
    Vec3 n = p->pull, u = vec3(1, 0, 0), w = vec3(0, 0, 1), q[4];
    int k;

    if (fabsf(n.y) < 0.999f) {   /* (tilted: a frame round the normal) */
        u = vec3(n.z, 0.0f, -n.x);
        {
            float l = sqrtf(u.x * u.x + u.z * u.z);

            u = vec3(u.x / l, 0.0f, u.z / l);
        }
        w = vec3(n.y * u.z - n.z * u.y, n.z * u.x - n.x * u.z, n.x * u.y - n.y * u.x);
    }
    for (k = 0; k < 4; k++) {
        float a = kCorner[k][0] * p->w, b = kCorner[k][1] * p->w;

        q[k] = vec3(p->at.x + a * u.x + b * w.x, p->at.y + a * u.y + b * w.y, p->at.z + a * u.z + b * w.z);
    }
    quad(sprites_records(s->batch, 1),
         (uint32_t)clampi(p->rgba[0], 0, 255) << 24 | (uint32_t)clampi(p->rgba[1], 0, 255) << 16 |
             (uint32_t)clampi(p->rgba[2], 0, 255) << 8 | (uint32_t)clampi(p->rgba[3], 0, 255),
         q);
}

/* ---- Cr19Bubbles (where the leaving homunculus sinks away): at the spot, 64 bubbles welling up
 * and 32 drops thrown up and out, then (frame 100) a stain laid on the floor. Each fades in to
 * past 0x20 and then out (`landed`: fading out). The bubbles: until frame 127 up to 4 a frame
 * (re)start, growing 0.05..0.1 a time to 0.7 for 90 frames, then shrinking (gone under 0.3),
 * within 3 x their size of the spot, rising 0.03..0.06 a frame and slowing by 0.0025, kept
 * above the spot by their size. The drops: one a frame until frame 150, from within 2 of the
 * spot (lower the later), 2.5..3 big, thrown out and up (faster the later), shrinking, turning.
 * Done once nothing shows ---- */
enum { WELL_BUBBLES = 64, WELL_DROPS = 32 };
#define WELL_DROP(s, i) (&(s)->p[WELL_BUBBLES + (i)])

static void well_bubble(System *s, Part *p) {   /* Cr19Bubbles_Bubble */
    float dx = 6.0f * p->w * half(), dz = 6.0f * p->w * half();

    colour(p, 0x60, 0x30, 0x28, 1);
    p->landed = 0;
    p->at = vec3(s->at.x + dx, s->at.y + 2.0f * p->w, s->at.z + dz);
    p->turn = deg(360.0f * half());
    p->v = vec3(0.03f * dx * rnd(), 0.03f + 0.03f * rnd(), 0.03f * dz * rnd());
}
static void well_drop(System *s, Part *p) {   /* Cr19Bubbles_Drop */
    float dx = 4.0f * half(), dz = 4.0f * half();

    colour(p, 0x40, 0x10, 8, 1);
    p->landed = 0;
    p->at = vec3(s->at.x + dx, 1.0f + s->at.y - 0.01f * (float)s->timer * rnd(), s->at.z + dz);
    square(p, 2.5f + 0.5f * rnd());
    p->turn = deg(360.0f * half());
    p->v = vec3(0.01f * -dx * rnd(), 0.01f + 0.0005f * (float)s->timer, 0.01f * -dz * rnd());
}
int particles_welling(Vec3 at, int tri) {   /* Cr19Bubbles_SetParams */
    int h, i;
    System *s;

    if (tri < 0 || (h = new_system(WELLING, at)) < 0) {
        return -1;
    }
    s = &sSys[h];
    s->engines = 1;
    s->kind = tri;
    s->timer = 0;
    look(s, 0x10, 1, 2, 0x1A0, 0x40, 32, 32, 1, 0);   /* (the bubbles) */
    s->batch2 = sprites_new();
    if (s->batch2 >= 0) {   /* (the drops) */
        sprites_texture(s->batch2, 0x10, 1, -1);
        sprites_cells(s->batch2, 0, 0x40, 32, 32, 512, 256, 1);
        sprites_flags(s->batch2, 0, 0x19);
    }
    s->n = WELL_BUBBLES + WELL_DROPS;
    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        colour(p, 0, 0, 0, 0);
        p->at = vec3(0, 0, 0);
        p->v = vec3(0, 0, 0);
        p->turn = 0.0f;
        p->frame = 0;
        square(p, i < WELL_BUBBLES ? 0.1f * (float)(irnd(4) + 1) : 0.5f);
    }
    return h;
}
static int well_fade(Part *p, int most) {   /* in past 0x20, then out by 1..most; 0: gone */
    if (!p->landed) {
        p->rgba[3] += irnd(most) + 1;
        if (p->rgba[3] > 0x20) {
            p->landed = 1;
        }
    } else {
        p->rgba[3] -= irnd(most) + 1;
    }
    if (p->rgba[3] <= 0) {
        p->rgba[3] = 0;
    }
    return p->rgba[3] > 0;
}
static int welling_step(System *s) {   /* Cr19Bubbles_Update */
    int i, n = 0, showing = 0;

    s->timer++;
    if (s->timer == 100) {
        splat_new(s->at, s->kind, 1);
    }
    for (i = 0; i < WELL_BUBBLES; i++) {
        Part *p = &s->p[i];

        if (p->rgba[3] == 0 || !well_fade(p, 4)) {
            continue;
        }
        showing = 1;
        p->v.y -= 0.0025f;
        p->at = vec3_add(p->at, p->v);
        p->at.y = fmaxf(p->at.y, s->at.y + p->w);
    }
    for (i = 0; i < WELL_BUBBLES && s->timer < 127 && n < 4; i++) {
        Part *p = &s->p[i];

        if (p->rgba[3] != 0) {
            continue;
        }
        if (s->timer < 90) {
            p->w = fminf(0.7f, p->w + 0.05f + 0.05f * rnd());
        } else {
            p->w -= 0.05f + 0.05f * rnd();
            if (p->w <= 0.3f) {
                p->w = p->h = 0.0f;
                continue;
            }
        }
        p->h = p->w;
        well_bubble(s, p);
        n++;
        showing = 1;
    }
    for (i = 0; i < WELL_DROPS; i++) {
        Part *p = WELL_DROP(s, i);

        if (p->rgba[3] == 0) {
            continue;
        }
        showing = 1;
        if (!well_fade(p, 2)) {
            continue;
        }
        p->w -= 0.01f * rnd();
        if (p->w < 0.0f) {
            p->rgba[3] = 0;
            p->w = 0.0f;
        }
        p->h = p->w;
        p->turn = wrap(p->turn + deg(rnd()));
        p->at = vec3_add(p->at, p->v);
    }
    for (i = 0; i < WELL_DROPS && s->timer < 150; i++) {
        if (WELL_DROP(s, i)->rgba[3] == 0) {
            well_drop(s, WELL_DROP(s, i));
            showing = 1;
            break;
        }
    }
    return showing;
}
static void welling_write(System *s) {   /* the bubbles in its batch, the drops in the second */
    Sprite *r;
    int i, k;

    s->n = WELL_BUBBLES;
    write(s);
    s->n = WELL_BUBBLES + WELL_DROPS;
    if (s->batch2 < 0) {
        return;
    }
    r = sprites_records(s->batch2, WELL_DROPS);
    for (i = 0; i < WELL_DROPS; i++) {
        const Part *p = WELL_DROP(s, i);

        memset(&r[i], 0, sizeof(r[i]));
        r[i].x = p->at.x;
        r[i].y = p->at.y;
        r[i].z = p->at.z;
        r[i].w = p->w;
        r[i].h = p->h;
        r[i].rot = p->turn;
        for (k = 0; k < 4; k++) {
            r[i].rgba[k] = (uint8_t)clampi(p->rgba[k], 0, 255);
        }
    }
}

/* ---- BloodSpray: where a blow lands, 32 dark red drops that shrink and fall to the floor of
 * their triangle (-1: none, no floor), half of them leaving a splat there ---- */

int particles_spray(Vec3 at, int tri, int heavy) {   /* BloodSpray_SetParams */
    int h = new_system(SPRAY, at), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    s->engines = 1;
    s->kind = tri;
    s->n = 32;
    look(s, 0x20, 1, -1, 0x60, 0x40, 32, 32, 16, 0);
    for (i = 0; i < 32; i++) {
        Part *p = &s->p[i];
        float sz;

        colour(p, 0x30, 0, 0, irnd(32) + (heavy ? 0x60 : 0x40));
        p->at = vec3(at.x + 0.5f * half(), at.y + 0.5f * rnd(), at.z + 0.5f * half());
        sz = (heavy ? 3.0f : 1.0f) + 2.0f * rnd();
        square(p, sz);
        if (heavy) {
            p->v = vec3(half(), 0.4f + 2.0f * rnd() / sz, half());
        } else {
            p->v = vec3(0.5f * half(), 0.2f + 2.0f * rnd() / sz, 0.5f * half());
        }
    }
    return h;
}
static int spray_step(System *s) {   /* BloodSpray_Update: 0 once all are gone */
    const NavTri *t = nav_tri(s->kind);
    int i, going = 0;

    for (i = 0; i < s->n; i++) {
        Part *p = &s->p[i];

        if (p->w <= 0.0f || p->rgba[3] <= 0) {
            continue;
        }
        going = 1;
        square(p, p->w - 0.02f < 0.0f ? 0.0f : p->w - 0.02f);
        p->v.y += p->v.y < 0.0f ? -0.05f : -0.1f;
        if (t != NULL && p->at.y + p->v.y < tri_floor(t, p->at.x, p->at.z, NULL)) {
            p->rgba[3] = 0;
            p->w = p->h = 0.0f;
        } else {
            p->at = vec3_add(p->at, p->v);
        }
        p->rgba[3] = clampi(p->rgba[3] - 4, 0, 255);
        if ((p->rgba[3] == 0 || p->w == 0.0f) && t != NULL && irnd(2) == 0) {
            splat_new(p->at, s->kind, 0);
        }
    }
    return going;
}

/* ---- ThingBurst: where a thing breaks or a character is shoved - 16 brown pieces thrown out
 * (pushed harder for 4 frames, then falling for 10, fading and darkening) and 16 puffs rising
 * from it (out under the floor) ---- */

int particles_puffs(Vec3 at) {   /* ThingBurst_SetParams */
    int h = new_system(PUFFS, at), i;
    System *s;

    if (h < 0) {
        return -1;
    }
    s = &sSys[h];
    s->engines = 1;
    s->n = 32;   /* (0..15 the pieces, 16..31 the puffs) */
    look(s, 0x10, 1, -1, 0x20, 0x40, 32, 32, 16, 0x40);
    s->batch2 = sprites_new();
    if (s->batch2 >= 0) {
        sprites_texture(s->batch2, 0x10, 1, -1);
        sprites_cells(s->batch2, 0xE, 0x6E, 4, 4, 512, 256, 16);
        sprites_flags(s->batch2, 0x40, 0x19);
    }
    at.y += 1.0f;
    for (i = 0; i < 16; i++) {
        Part *p = &s->p[i];
        float k = 0.1f + 0.05f * rnd();

        colour(p, 0x5F, 0x48, 0x1C, irnd(64) + 0x40);
        p->at = vec3(at.x + 4.0f * half(), at.y, at.z + 4.0f * half());
        square(p, 2.0f + 2.0f * rnd());
        p->turn = deg(360.0f * half());
        p->v = vec3(2.0f * half(), 0.1f + 0.1f * rnd(), 2.0f * half());
        p->pull = vec3(-p->v.x * k, -0.5f * (p->v.y * k), -p->v.z * k);   /* (its fall) */
        p->rise = p->v.x / 8.0f;    /* (its push, x and z) */
        p->angle = p->v.z / 8.0f;
    }
    for (i = 16; i < 32; i++) {
        Part *p = &s->p[i];

        colour(p, 0x5F, 0x48, 0x1C, i % 2 == 0 ? 0x40 : 0x80);
        p->at = vec3(at.x + 2.0f * half(), at.y, at.z + 2.0f * half());
        square(p, 0.1f + 0.5f * rnd());
        p->v = vec3(1.5f * half(), 2.5f * (0.1f + rnd()) - 0.5f, 1.5f * half());
        p->radius = 0.02f + 0.1f * p->v.y;   /* (how fast it slows) */
    }
    return h;
}
static int puffs_step(System *s) {   /* ThingBurst_Update (burst_update: 16 pieces, 16 puffs) */
    int i, going = 0;

    for (i = 0; i < 16; i++) {
        Part *p = &s->p[i];

        if (p->rgba[3] <= 0) {
            continue;
        }
        going = 1;
        square(p, p->w + 0.05f);
        p->turn = wrap(p->turn + deg(2.0f * rnd()));
        p->at.x += p->v.x + p->rise;
        p->at.z += p->v.z + p->angle;
        if (s->timer < 4) {
            p->rise += p->rise / 6.0f;
            p->angle += p->angle / 6.0f;
            continue;
        }
        for (int k = 0; k < 3; k++) {
            if (p->rgba[k] > 0) {
                p->rgba[k] -= 1;
            }
        }
        p->rgba[3] = clampi(p->rgba[3] - (irnd(8) + 1), 0, 255);
        if (s->timer < 10) {
            p->v = vec3_add(p->v, p->pull);
        }
        p->at.y += (i % 2 == 0 ? 2.0f : 4.0f) * p->v.y;
        p->rise = p->angle = 0.0f;
    }
    s->timer++;
    for (i = 16; i < 32; i++) {
        Part *p = &s->p[i];
        float h;

        if (p->rgba[3] <= 0) {
            continue;
        }
        p->at = vec3_add(p->at, p->v);
        p->v.y -= p->radius;
        if (navmesh_find(&gEngine.room.nav, p->at, 1000.0f, &h) >= 0 && p->at.y < h) {
            p->rgba[3] = 0;
            continue;
        }
        p->rgba[3] = clampi(p->rgba[3] - (irnd(8) + 1), 0, 255);
    }
    return going;
}
static void puffs_write(System *s) {
    Sprite *r = sprites_records(s->batch, 16), *q = s->batch2 >= 0 ? sprites_records(s->batch2, 16) : NULL;
    int i, k;

    for (i = 0; i < 32; i++) {
        const Part *p = &s->p[i];
        Sprite *o = i < 16 ? &r[i] : q != NULL ? &q[i - 16] : NULL;

        if (o == NULL) {
            continue;
        }
        memset(o, 0, sizeof(*o));
        o->x = p->at.x;
        o->y = p->at.y;
        o->z = p->at.z;
        o->w = p->w;
        o->h = p->h;
        o->rot = p->turn;
        for (k = 0; k < 4; k++) {
            o->rgba[k] = (uint8_t)clampi(p->rgba[k], 0, 255);
        }
    }
}

/* ---- the sprites written, a tick ---- */

static void write(System *s) {
    Sprite *r = sprites_records(s->batch, s->n);
    int i, k;

    for (i = 0; i < s->n; i++) {
        const Part *p = &s->p[i];

        memset(&r[i], 0, sizeof(r[i]));
        r[i].x = p->at.x;
        r[i].y = p->at.y;
        r[i].z = p->at.z;
        r[i].w = p->w;
        r[i].h = p->h;
        r[i].rot = p->turn;
        r[i].frame = p->frame;
        for (k = 0; k < 4; k++) {
            r[i].rgba[k] = (uint8_t)clampi(p->rgba[k], 0, 255);
        }
    }
    if (s->what == FLICKER && s->rest > 0) {   /* (resting: unseen) */
        sprites_records(s->batch, 0);
    }
}


void particles_tick(Vec3 camera) {
    int i;

    sCamera = camera;
    for (i = 0; i < PARTICLE_SYSTEMS; i++) {
        System *s = &sSys[i];
        int going = 1;

        if (!s->used || s->ended == 2) {
            continue;
        }
        if (s->ends_with_owner && get(s->owner) == NULL) {
            going = 0;
        } else {
            switch (s->what) {
            case FLICKER: flicker_step(s); break;
            case SPARKS: going = sparks_step(s); break;
            case FLAME: flame_step(s); break;
            case SWARM: swarm_step(s, camera); break;
            case SPLASH: going = splash_step(s); break;
            case BURST: going = burst_step(s); break;
            case SHARDS: going = shards_step(s); break;
            case MOTE: going = mote_step(s); break;
            case BUTTERFLIES: butterflies_step(s); break;
            case SPIRIT: going = spirit_step(s); break;
            case VANISH: going = vanish_step(s); break;
            case WANDER: wander_step(s); break;
            case SPRAY: going = spray_step(s); break;
            case SPLAT: going = splat_step(s); break;
            case PUFFS: going = puffs_step(s); break;
            case STONES: stones_step(s); break;
            case DEBRIS: going = debris_step(s); break;
            case SMOKE: smoke_step(s); break;
            case DUST: dust_step(s); break;
            case SPIRAL: spiral_step(s); break;
            case DRIPS: drips_step(s); break;
            case RING: going = ring_step(s); break;
            case DROPLETS: going = droplets_step(s); break;
            case ONEDRIP: onedrip_step(i, s); break;
            case MOTES: motes_step(s); break;
            case EMBERS: going = embers_step(s); break;
            case FLECKS: going = flecks_step(s); break;
            case CHIMNEY: chimney_step(s); break;
            case FIRE: going = fire_step(s); break;
            case STEAM: going = steam_step(s); break;
            case BIGFIRE: going = big_fire_step(s); break;
            case SHAFT: going = shaft_step(s); break;
            case HAZE: haze_step(s); break;
            case CAUSTIC: caustic_step(s); break;
            case WISPCOL: going = wisp_column_step(s); break;
            case SPARKSPRAY: going = spark_spray_step(s); break;
            case HEAT: going = heat_step(s); break;
            case LEAVES: leaves_step(s); break;
            case LOOPSPRITE: loop_sprite_step(s); break;
            case OBJGLOW: going = object_glow_step(s); break;
            case RAIN: rain_step(i, s); break;
            case ASH: ash_step(s); break;
            case MARKER: going = marker_step(s); break;
            case WINDOWFLASH: going = window_flash_step(s); break;
            case SMOKETRAIL: going = smoke_trail_step(s); break;
            case MAGBURST: going = mag_burst_step(s); break;
            case DROPFLASH: going = drop_flash_step(s); break;
            case BONESMOKE: going = bone_smoke_step(s); break;
            case SPARK2: going = spark2_step(s); break;
            case SINKRING: going = sink_ring_step(s); break;
            case WELLING: going = welling_step(s); break;
            case EMBERS2: going = embers2_step(s); break;
            case FLOORGLOW: going = floor_glow_step(s); break;
            }
        }
        if (!going) {
            if (s->engines) {   /* (the engine's own go; an actor's wait to be freed) */
                end(s);
                continue;
            }
            s->ended = 2;
            sprites_records(s->batch, 0);
            if (s->batch2 >= 0) {
                sprites_records(s->batch2, 0);
            }
            if (s->batch3 >= 0) {
                sprites_records(s->batch3, 0);
            }
            if (s->batch4 >= 0) {
                sprites_records(s->batch4, 0);
            }
            continue;
        }
        switch (s->what) {
        case SHARDS: shards_write(s); break;
        case STONES: stones_write(s); break;
        case DEBRIS: shards_write(s); break;
        case SPIRAL: write(s); spiral_glow(s); break;
        case STEAM: write(s); steam_glow(s); break;
        case DRIPS: drips_write(s); break;
        case RING: ring_write(s); break;
        case FLECKS: flecks_write(s); break;
        case WALLSHADOW: wall_shadow_write(s); break;
        case FLOORGLOW: floor_glow_write(s); break;
        case TV: tv_write(s); break;
        case BIGFIRE: big_fire_write(s); break;
        case SHAFT: shaft_write(s); break;
        case GLINT: glint_write(s); break;
        case CAUSTIC: caustic_write(s); break;
        case EMBERS2: embers2_write(s); break;
        case LEAVES: leaves_write(s); break;
        case ASH: ash_write(s); break;
        case MARKER: marker_write(s); break;
        case WINDOWFLASH: window_flash_write(s); break;
        case MAGBURST: mag_burst_write(s); break;
        case DROPFLASH: drop_flash_write(s); break;
        case SINKRING: sink_ring_write(s); break;
        case WELLING: welling_write(s); break;
        case OBJGLOW: object_glow_write(s); break;
        case BUTTERFLIES: butterflies_write(i, s); break;
        case SPIRIT: spirit_write(s); break;
        case VANISH: vanish_write(s); break;
        case WANDER: wander_write(s); break;
        case SPLAT: splat_write(s); break;
        case PUFFS: puffs_write(s); break;
        default: write(s); break;
        }
    }
}

void particles_place(int h, Vec3 at) {   /* (a room effect moved by a room's command: its parts with it) */
    System *s = get(h);
    Vec3 d;
    int i;

    if (s == NULL) {
        return;
    }
    d = vec3_sub(at, s->at);
    s->at = at;
    for (i = 0; i < s->n && i < MAX_PARTS; i++) {
        s->p[i].at = vec3_add(s->p[i].at, d);
    }
}

void particles_rest(int h, int frames) {   /* (a flickering sprite's rest before its next run) */
    System *s = get(h);

    if (s != NULL) {
        s->rest = frames;
    }
}

int particles_alive(int p) {
    System *s = get(p);

    return s != NULL && s->ended != 2;
}

int particles_children(int p) {
    int i, n = 0;

    for (i = 0; i < PARTICLE_SYSTEMS; i++) {
        if (sSys[i].used && sSys[i].engines && sSys[i].owner == p) {
            n++;
        }
    }
    return n;
}

int particles_info(int p, int *kind, int *frame, int *rest) {
    System *s = get(p);

    if (s == NULL) {
        return 0;
    }
    *kind = s->kind;
    *frame = s->p[0].frame;
    *rest = s->rest;
    return 1;
}

void particles_engines(int p) {
    System *s = get(p);

    if (s != NULL) {
        s->engines = 1;
    }
}

void particles_free(int p) {
    System *s = get(p);

    if (s != NULL) {
        end(s);
    }
}

void particles_new_room(void) {
    int i;

    for (i = 0; i < PARTICLE_SYSTEMS; i++) {
        if (sSys[i].used && sSys[i].engines) {
            end(&sSys[i]);
        }
    }
}

void particles_free_all(void) {
    int i;

    for (i = 0; i < PARTICLE_SYSTEMS; i++) {
        if (sSys[i].used) {
            end(&sSys[i]);
        }
    }
}
