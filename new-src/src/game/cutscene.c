/* The cutscene director: see cutscene.h. Ported from src/game/director.c (Cutscene_*). The
 * original streams the shots into two buffers while it plays; here they are all read at the
 * start (a scene's shots are a few hundred KB), so it never waits for one (status 4). */
#include "cutscene.h"

#include "../core/files.h"
#include "engine.h"
#include "progress.h"
#include "room.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SHOTS 64
#define SLOTS 32

enum { STATE_IDLE, STATE_SCRIPT, STATE_FIRST_SHOT, STATE_PLAYING };

typedef struct Slot {
    int in;             /* in the cutscene (+0) */
    int animated;       /* driven by the shot (+1) */
    int actor;          /* new-src's actor */
    int was_visible;    /* shown before (+4's 0x80) */
} Slot;

static struct {
    char name[32];
    uint8_t *script, *marks, *cues;
    size_t script_size, marks_size, cues_size;
    RoomLook look;          /* the fog and depth range as they were at the start (+0x238, +0x268) */
    uint8_t *shots[MAX_SHOTS];
    size_t shot_size[MAX_SHOTS];
    int state, status;
    int start_when_ready;   /* (+0x205) */
    int letterbox_off;      /* (+0x4) */
    int frame, last;        /* (+0xC, +0x10) */
    int shot;               /* the shot started (the playing buffer's), -1 none */
    const uint8_t *camera;  /* the shot's camera part (+0x20) */
    const uint8_t *doors[8];    /* the shot's doors' keys (+0x24) */
    const uint8_t *groups[8];   /* the shot's object groups' keys (+0x44) */
    Slot slots[SLOTS];
    int counts[16];         /* each signal's count so far (+0x206) */
    int cast;               /* Cutscene_Cast ran: the actors are taken */
} C;

/* actor i -> the character's script id (Cutscene_MapId) */
static const int8_t kMap[26] = {
    -1, 0, 1, 2, 3, 4, 9, 10, 11, 12, 8, 13, 18, 14, 15, 16, 17, 19, 23, 24, 25, 29, 30, 31, 32, 38,
};

static int rd16(const uint8_t *p) { return p[0] | p[1] << 8; }
static uint32_t rd32(const uint8_t *p) { return p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24; }

static void free_all(void) {
    int i;

    free(C.script);
    free(C.marks);
    free(C.cues);
    for (i = 0; i < MAX_SHOTS; i++) {
        free(C.shots[i]);
    }
    C.script = C.marks = C.cues = NULL;
    memset(C.shots, 0, sizeof(C.shots));
}

/* ---- the script: +0x0 the length, +0x2 the records; record r at +0x24 + r x 12: +0 the actors,
 * +4 the doors, +5 the object groups, +8 / +0xA its first and last frame ---- */

int cutscene_length(void) {
    return C.script != NULL ? rd16(C.script) : -1;
}

static int records(void) {
    return C.script != NULL ? rd16(C.script + 2) : 0;
}

static const uint8_t *record(int r) {
    if (r < 0 || 0x24 + (size_t)(r + 1) * 12 > C.script_size) {
        return NULL;
    }
    return C.script + 0x24 + r * 12;
}

static uint32_t script_actors(void) {
    uint32_t m = 0;
    int r;

    for (r = 0; r < records(); r++) {
        m |= rd32(record(r));
    }
    return m;
}

int cutscene_shot_at(int t) {
    int n = cutscene_length(), i;

    if (C.script == NULL || n <= 0) {
        return -1;
    }
    while (t < 0) {
        t += n;
    }
    while (t > n - 1) {
        t -= n;
    }
    for (i = 0; i < records(); i++) {
        const uint8_t *r = record(i);

        if (r != NULL && t >= rd16(r + 8) && t <= rd16(r + 0xA)) {
            return i;
        }
    }
    return -1;
}

/* how far frame t is into its shot (Cutscene_IntoShot) */
static int into_shot(int t) {
    int i = cutscene_shot_at(t);

    return i < 0 ? -1 : t - rd16(record(i) + 8);
}

/* frame f's signal bits (MARK.BIN: a u16 a frame) */
static int signals(int f) {
    if (f < 0 || f >= cutscene_length() || (size_t)(f + 1) * 2 > C.marks_size) {
        return 0;
    }
    return rd16(C.marks + f * 2);
}

int cutscene_signal_count(int bit) {
    int n = 0, f;

    for (f = C.last + 1; f <= C.frame; f++) {
        if (signals(f) & (1 << bit)) {
            n++;
        }
    }
    return n;
}

int cutscene_near_end(void) {
    int f = cutscene_length() - 0x11;

    return C.last < f && C.frame >= f;
}

/* a part of shot s: its offset at `off`, 0 none */
static const uint8_t *shot_part(int s, int off, size_t *size) {
    uint32_t o;

    if (s < 0 || s >= MAX_SHOTS || C.shots[s] == NULL || (size_t)off + 4 > C.shot_size[s]) {
        return NULL;
    }
    o = rd32(C.shots[s] + off);
    if (o == 0 || o >= C.shot_size[s]) {
        return NULL;
    }
    if (size != NULL) {
        *size = C.shot_size[s] - o;
    }
    return C.shots[s] + o;
}

/* ---- the actors ---- */

/* the actor of the character with script id `kind`, or of one of its variants (Cutscene_KindSlot) */
static int kind_actor(int kind) {
    static const int8_t kAlt[][4] = {
        {2, 6, 7, 0x1B}, {3, 0x22, 0x23, 0x24}, {10, 0x27, -1, -1}, {23, 0x17, 0x25, -1},
    };
    int tries[4] = {kind, -1, -1, -1}, i, j, k;

    for (i = 0; i < 4; i++) {
        if (kAlt[i][0] == kind) {
            for (j = 1; j < 4; j++) {
                tries[j] = kAlt[i][j];
            }
        }
    }
    for (k = 0; k < 4; k++) {
        for (i = 0; tries[k] >= 0 && i < CHARACTERS; i++) {
            const ScriptChar *c = &gEvents.chars[i];

            if (c->present && c->id == tries[k] && c->actor >= 0 && c->actor < MAX_ACTORS &&
                gEngine.actors[c->actor].used) {
                return c->actor;
            }
        }
    }
    return -1;
}

static Actor *slot_actor(const Slot *s) {
    return s->in && s->actor >= 0 ? &gEngine.actors[s->actor] : NULL;
}

/* each actor in the script gets its character (Cutscene_Cast) */
static void cast(void) {
    uint32_t actors = script_actors();
    int i;

    for (i = 1; i < SLOTS; i++) {
        Slot *s = &C.slots[i];

        memset(s, 0, sizeof(*s));
        s->actor = -1;
        if (!(actors & (1u << i)) || i >= 26) {
            continue;
        }
        s->actor = kind_actor(kMap[i]);
        if (s->actor < 0) {
            continue;
        }
        s->in = 1;
        s->was_visible = gEngine.actors[s->actor].visible;
    }
    C.cast = 1;
}

/* the actors given back as they were (Cutscene_ReleaseActors) */
static void release(void) {
    int i;

    for (i = 1; i < SLOTS; i++) {
        Actor *a = slot_actor(&C.slots[i]);

        if (a != NULL) {
            a->drive = NULL;
            a->visible = C.slots[i].was_visible;
        }
        memset(&C.slots[i], 0, sizeof(C.slots[i]));
        C.slots[i].actor = -1;
    }
    C.cast = 0;
}

/* ---- the room's object groups: the script's header +0x4 + k x 4 is group k's list (a count,
 * then 0x10-byte entries named at +0x4); a shot's group keys (+0xA0 + k x 4: a count, then
 * 0x1C-byte entries named at +0xC with their keys' offset at +0x8 - 0x18 bytes a frame: the
 * turn, then the place) ---- */

static int script_groups(void) {
    int m = 0, r;

    for (r = 0; r < records(); r++) {
        m |= record(r)[5];
    }
    return m;
}

/* group k's object i (NULL none) */
static Placed *group_object(int k, int i) {
    uint32_t o = (uint32_t)rd32(C.script + 4 + k * 4);
    char name[13];

    if (o == 0 || o + 4 > C.script_size || i >= (int)rd32(C.script + o) || o + 4 + (size_t)(i + 1) * 0x10 > C.script_size) {
        return NULL;
    }
    memcpy(name, C.script + o + 4 + i * 0x10 + 4, 12);
    name[12] = 0;
    return placed_named(&gEngine.room.placed, name);
}

static int group_count(int k) {
    uint32_t o = (uint32_t)rd32(C.script + 4 + k * 4);

    return o == 0 || o + 4 > C.script_size ? 0 : (int)rd32(C.script + o);
}

/* the groups the script animates put back as they were defined (Cutscene_RestoreGroups) */
static void restore_groups(void) {
    int groups = script_groups(), k, i;

    for (k = 0; k < 8; k++) {
        for (i = 0; (groups & (1 << k)) && i < group_count(k); i++) {
            Placed *p = group_object(k, i);

            if (p != NULL) {
                p->rot = p->def_rot;
                p->pos = p->def_pos;
            }
        }
    }
}

/* this frame's keys of the shot's groups and doors (Cutscene_GroupKeys, Cutscene_Update) */
static void group_door_keys(int t) {
    const uint8_t *r = record(cutscene_shot_at(C.frame));
    int k;

    if (r == NULL || t < 0) {
        return;
    }
    for (k = 0; k < 8; k++) {
        const uint8_t *tr = C.groups[k];
        uint32_t i;

        if (!(r[5] & (1 << k)) || tr == NULL) {
            continue;
        }
        for (i = 0; i < (uint32_t)rd32(tr); i++) {
            const uint8_t *e = tr + 4 + i * 0x1C, *key = tr + rd32(e + 8) + t * 0x18;
            char name[17];
            Placed *p;
            float f[6];
            int j;

            memcpy(name, e + 0xC, 16);
            name[16] = 0;
            p = placed_named(&gEngine.room.placed, name);
            if (p == NULL) {
                continue;
            }
            for (j = 0; j < 6; j++) {
                uint32_t u = (uint32_t)rd32(key + j * 4);

                memcpy(&f[j], &u, 4);
            }
            p->rot = vec3(f[0], f[1], f[2]);   /* (Angle_Wrap: the matrix doesn't mind) */
            p->pos = vec3(f[3], f[4], f[5]);
        }
    }
    for (k = 0; k < 8; k++) {   /* (Doors_TurnTo: the door's turn, radians) */
        if ((r[4] & (1 << k)) && C.doors[k] != NULL) {
            uint32_t u = (uint32_t)rd32(C.doors[k] + 0x20 + t * 12 + 4);
            float a;

            memcpy(&a, &u, 4);
            room_door_angle(&gEngine.room, k, a);
        }
    }
}

/* shot `rec` starts: the camera's keys; each actor in it driven by its keys (shown, animation
 * 0x8000), those out of it back to their own motion; the doors' keys; the object groups in it
 * shown with their keys, the others the script animates hidden (Cutscene_StartShot) */
static void start_shot(int rec) {
    const uint8_t *r = record(rec);
    uint32_t actors = r != NULL ? rd32(r) : 0;
    int i;

    C.shot = rec;
    for (i = 0; i < SLOTS; i++) {
        Slot *s = &C.slots[i];
        Actor *a = slot_actor(s);

        if (i == 0) {
            if (actors & 1) {
                C.camera = shot_part(rec, 0, NULL);
            }
            continue;
        }
        if (a == NULL) {
            continue;
        }
        if ((actors & (1u << i)) && i <= 25) {
            size_t size = 0;

            a->drive = shot_part(rec, i * 4, &size);
            a->drive_size = size;
            a->drive_frame = 0.0f;
            a->visible = 1;
            s->animated = a->drive != NULL;
        } else {
            a->drive = NULL;
            s->animated = 0;
        }
    }
    for (i = 0; i < 8; i++) {
        int k, hide;

        if (r != NULL && (r[4] & (1 << i))) {
            C.doors[i] = shot_part(rec, 0x80 + i * 4, NULL);
        }
        if (r != NULL && (r[5] & (1 << i))) {
            C.groups[i] = shot_part(rec, 0xA0 + i * 4, NULL);
            hide = 0;
        } else if (script_groups() & (1 << i)) {
            hide = 1;
        } else {
            continue;
        }
        for (k = 0; k < group_count(i); k++) {
            Placed *p = group_object(i, k);

            if (p != NULL) {
                p->shown = !hide;
            }
        }
    }
}

/* ---- the states ---- */

int cutscene_start(const char *name) {
    char path[96];
    int i;

    if (C.cast) {
        release();
    }
    free_all();
    memset(&C, 0, sizeof(C));
    snprintf(C.name, sizeof(C.name), "%s", name);
    C.last = -2;
    C.frame = -1;
    C.shot = -1;
    snprintf(path, sizeof(path), "%s/%s.DH", name, name);
    C.script = files_read(path, &C.script_size);
    snprintf(path, sizeof(path), "%s/MARK.BIN", name);
    C.marks = files_read(path, &C.marks_size);
    snprintf(path, sizeof(path), "%s/PARAMS.BIN", name);
    C.cues = files_read(path, &C.cues_size);
    if (C.script == NULL || C.script_size < 0x24) {
        fprintf(stderr, "cutscene: no scene %s\n", name);
        free_all();
        C.status = 5;
        return 0;
    }
    for (i = 0; i < records() && i < MAX_SHOTS; i++) {
        snprintf(path, sizeof(path), "%s/CUT%03X.DP", name, i);
        C.shots[i] = files_read(path, &C.shot_size[i]);
    }
    for (i = 0; i < SLOTS; i++) {
        C.slots[i].actor = -1;
    }
    C.state = STATE_SCRIPT;
    C.status = 1;
    return 1;
}

void cutscene_run_state(void) {
    switch (C.state) {
    case STATE_SCRIPT:   /* (Cutscene_StateScript: the files are in) */
        C.state = STATE_FIRST_SHOT;
        C.status = 1;
        break;
    case STATE_FIRST_SHOT:   /* (Cutscene_StateFirstShot) */
        C.status = 2;
        if (!C.start_when_ready) {
            break;
        }
        C.state = STATE_PLAYING;
        cast();
        C.look = gRoomLook;
        C.status = 3;
        break;
    case STATE_PLAYING: {   /* (Cutscene_StatePlaying) */
        int now = cutscene_shot_at(C.frame);

        C.status = 3;
        if (C.frame >= cutscene_length() || C.frame < 0) {
            C.status = 5;
            break;
        }
        if (now >= 0 && now != C.shot) {
            start_shot(now);
        }
        break;
    }
    default:
        break;
    }
}

void cutscene_start_when_ready(void) {
    C.start_when_ready = 1;
}

void cutscene_set_frame(int frame) {
    C.last = C.frame;
    C.frame = frame;
}

int cutscene_frame(void) {
    return C.frame;
}

int cutscene_status(void) {
    return C.status;
}

int cutscene_playing_shot(void) {
    return C.state == STATE_PLAYING && C.shot >= 0 && C.shot == cutscene_shot_at(C.frame);
}

/* this frame's lights and effects from the cues (PARAMS.BIN: a count, then 0x30-byte cues from
 * +0x10 for shot +0x2: type +0x1 - 0 the lights (the room's scaled by +0x18, the ambient raised
 * by +0x14, a light from the camera's side +0x10 turned +0x1C / +0x20, a second +0x24 turned
 * +0x28 / +0x2C if it has a colour), 1 / 2 the depth range / fog fed +0x10); the fog otherwise
 * as at the start (Cutscene_Cues) */
static float f32_at(const uint8_t *p) {
    uint32_t u = rd32(p);
    float f;

    memcpy(&f, &u, 4);
    return f;
}

static void cues(void) {
    int shot = cutscene_shot_at(C.frame), n, i;

    memset(&gEngine.extra, 0, sizeof(gEngine.extra));
    room_look_set(0x1C, NULL, 0);
    gRoomLook.has_fog = C.look.has_fog;
    memcpy(gRoomLook.fog_near_color, C.look.fog_near_color, sizeof(gRoomLook.fog_near_color));
    memcpy(gRoomLook.fog_far_color, C.look.fog_far_color, sizeof(gRoomLook.fog_far_color));
    gRoomLook.fog_near = C.look.fog_near;
    gRoomLook.fog_far = C.look.fog_far;
    if (C.cues == NULL || C.cues_size < 0x10) {
        return;
    }
    n = (int)rd32(C.cues);
    for (i = 0; i < n && 0x10 + (size_t)(i + 1) * 0x30 <= C.cues_size; i++) {
        const uint8_t *c = C.cues + 0x10 + i * 0x30;

        if (rd16(c + 2) != shot) {
            continue;
        }
        switch (c[1]) {
        case 0:
            gEngine.extra.on = 1;
            gEngine.extra.ambient = vec3(c[0x14], c[0x15], c[0x16]);
            gEngine.extra.scale = f32_at(c + 0x18);
            gEngine.extra.cam[0].on = 1;
            gEngine.extra.cam[0].color = vec3(c[0x10], c[0x11], c[0x12]);
            gEngine.extra.cam[0].up = f32_at(c + 0x1C);
            gEngine.extra.cam[0].about = f32_at(c + 0x20);
            if (c[0x24] != 0 || c[0x25] != 0 || c[0x26] != 0) {
                gEngine.extra.cam[1].on = 1;
                gEngine.extra.cam[1].color = vec3(c[0x24], c[0x25], c[0x26]);
                gEngine.extra.cam[1].up = f32_at(c + 0x28);
                gEngine.extra.cam[1].about = f32_at(c + 0x2C);
            }
            break;
        case 1:
            room_look_set(0x1C, c + 0x10, 0x10);
            break;
        case 2:
            room_look_set(0x1D, c + 0x10, 0x10);
            break;
        }
    }
}

void cutscene_update(void) {
    int t = into_shot(C.frame), i;

    for (i = 1; i < SLOTS; i++) {
        Actor *a = slot_actor(&C.slots[i]);

        if (a != NULL && C.slots[i].animated) {
            a->drive_frame = (float)(t < 0 ? 0 : t);
        }
    }
    group_door_keys(t);
    cues();
    for (i = 0; i < 16; i++) {
        C.counts[i] += cutscene_signal_count(i);
    }
}

int cutscene_signal_total(int bit) {
    return bit >= 0 && bit < 16 ? C.counts[bit] - 1 : -1;
}

void cutscene_end(void) {
    if (C.state == STATE_PLAYING) {   /* the fog and depth range as at the start, the lights' extra off */
        gRoomLook.has_fog = C.look.has_fog;
        memcpy(gRoomLook.fog_near_color, C.look.fog_near_color, sizeof(gRoomLook.fog_near_color));
        memcpy(gRoomLook.fog_far_color, C.look.fog_far_color, sizeof(gRoomLook.fog_far_color));
        gRoomLook.fog_near = C.look.fog_near;
        gRoomLook.fog_far = C.look.fog_far;
        gRoomLook.has_dof = C.look.has_dof;
        memcpy(gRoomLook.dof, C.look.dof, sizeof(gRoomLook.dof));
        memset(&gEngine.extra, 0, sizeof(gEngine.extra));
        restore_groups();
    }
    release();
    C.state = STATE_IDLE;
    C.camera = NULL;
}

/* the camera's keys: a count, the frames, the keys' offset; 32-byte keys (eye, target, the view
 * angle across a 4:3 picture, the roll) (Cutscene_CameraKey) */
int cutscene_camera(Camera *c) {
    const uint8_t *r = record(cutscene_shot_at(C.frame));
    const uint8_t *k;
    float f[8];
    int t = into_shot(C.frame), frames, j;
    Vec3 eye, look, v;
    float h;

    if (C.state != STATE_PLAYING || C.status == 5 || C.camera == NULL || r == NULL || !(rd32(r) & 1) || t < 0) {
        return 0;
    }
    frames = (int)rd32(C.camera + 4);
    if (t >= frames) {
        t = frames - 1;
    }
    k = C.camera + rd32(C.camera + 8) + t * 32;
    for (j = 0; j < 8; j++) {
        uint32_t u = rd32(k + j * 4);

        memcpy(&f[j], &u, 4);
    }
    eye = vec3(f[0], f[1], f[2]);
    look = vec3(f[3], f[4], f[5]);
    v = vec3_sub(look, eye);
    h = sqrtf(v.x * v.x + v.z * v.z);
    c->pos = eye;
    c->yaw = atan2f(v.x, -v.z);
    c->pitch = atan2f(v.y * c->up, h);
    c->fov = 2.0f * atanf(0.75f * tanf(f[6] * 0.5f));   /* (the roll, f[7]: not yet) */
    return 1;
}

int cutscene_active(void) {
    return C.state == STATE_PLAYING && C.status != 5;
}

int cutscene_letterbox(void) {
    return cutscene_active() && !C.letterbox_off;
}

void cutscene_set_letterbox_off(int off) {
    C.letterbox_off = off;
}
