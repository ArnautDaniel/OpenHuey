/* Lorenzo in his chair: see chair.h. Ported from src/game/lorenzo.c LorenzoModel_BodyFrames,
 * Chair_OnFloor, Chair_Frame, LorenzoModel_AdjustBone, and src/game/vecmath.c Mtx_FrameKeepZ,
 * Mtx_TurnTwo, Quat_FromAxisAngle / Quat_ToMatrix. Mat4's columns are the original's matrix rows
 * (m[4k..4k+2] axis k, m[12..14] the place), so mat4_mul(a, b) is sceVu0MulMatrix(d, a, b). */
#include "chair.h"
#include "actor.h"
#include "humanlegs.h"

#include <math.h>

#define CHAIR_FRONT_BONE 0xA
#define HEAD_LOW_BONE 0xB
#define HEAD_MID_BONE 0x12
#define HEAD_TOP_BONE 0x13

static Vec3 axis(const Mat4 *m, int k) { return vec3(m->m[k * 4], m->m[k * 4 + 1], m->m[k * 4 + 2]); }
static void set_axis(Mat4 *m, int k, Vec3 v) { m->m[k * 4] = v.x; m->m[k * 4 + 1] = v.y; m->m[k * 4 + 2] = v.z; }

/* (sceVu0Normalize: a zero vector stays) */
static Vec3 unit(Vec3 v) {
    float l = sqrtf(vec3_dot(v, v));

    return l > 0.0f ? vec3_scale(v, 1.0f / l) : v;
}

/* the rotation part alone (the place cleared) */
static Mat4 rotation_of(Mat4 m) {
    m.m[12] = m.m[13] = m.m[14] = 0.0f;
    return m;
}

/* Chair_Frame: the floor's frame from the line d (unit): X across it, Z along it, Y up from them */
static Mat4 chair_frame(Vec3 d) {
    Mat4 f = mat4_identity();
    Vec3 x = unit(vec3(d.z, 0.0f, -d.x));

    set_axis(&f, 0, x);
    set_axis(&f, 2, d);
    set_axis(&f, 1, vec3_cross(d, x));
    return f;
}

/* Quat_FromAxisAngle + Quat_ToMatrix, as the original lays it out */
static Mat4 axis_turn(Vec3 a, float angle) {
    float s = sinf(0.5f * angle), x = a.x * s, y = a.y * s, z = a.z * s, w = cosf(0.5f * angle);
    Mat4 m = mat4_identity();

    m.m[0] = 1.0f - 2.0f * (y * y + z * z);
    m.m[1] = 2.0f * (x * y - z * w);
    m.m[2] = 2.0f * (x * z + y * w);
    m.m[4] = 2.0f * (x * y + z * w);
    m.m[5] = 1.0f - 2.0f * (x * x + z * z);
    m.m[6] = 2.0f * (y * z - x * w);
    m.m[8] = 2.0f * (x * z - y * w);
    m.m[9] = 2.0f * (y * z + x * w);
    m.m[10] = 1.0f - 2.0f * (x * x + y * y);
    return m;
}

/* Mtx_TurnTwo: bone b turned `a` about axes' X, then `b` about axes' Y (about its own place) */
static void turn_two(Mat4 *bone, const Mat4 *axes, float a, float b) {
    Mat4 r = mat4_mul(axis_turn(axis(axes, 1), b), axis_turn(axis(axes, 0), a));
    Vec3 at = vec3(bone->m[12], bone->m[13], bone->m[14]);

    *bone = mat4_mul(r, rotation_of(*bone));
    bone->m[12] = at.x;
    bone->m[13] = at.y;
    bone->m[14] = at.z;
}

/* Mtx_FrameKeepZ: a frame from bone b keeping its Z (level, turned to `up`'s side) with Y = up */
static Mat4 frame_keep_z(const Mat4 *b, Vec3 up) {
    Mat4 f = mat4_identity();
    Vec3 z = axis(b, 2), x;

    z.y = 0.0f;
    if (vec3_dot(axis(b, 1), up) < 0.0f) {
        z = vec3_scale(z, -1.0f);
    }
    x = vec3_cross(up, z);
    z = vec3_cross(x, up);
    set_axis(&f, 0, unit(x));
    set_axis(&f, 1, unit(up));
    set_axis(&f, 2, unit(z));
    return f;
}

void chair_set(Chair *c, int on) {
    c->on = on;
    c->asked = c->fitted = 0;
    c->body = c->front = mat4_identity();
    c->pitch = c->yaw = 0.0f;
}

/* LorenzoModel_BodyFrames */
void chair_fit(Actor *a, const NavMesh *nav, int tri, uint32_t mask, float front, float back, float pitch,
               float yaw) {
    Chair *c = &a->chair;
    float cy = cosf(a->yaw), sy = sinf(a->yaw), floor, dy, p[3];
    Mat4 unturn;
    Vec3 ahead = vec3(sy, 0.0f, cy), d;

    if (!c->on || a->drive != NULL) {   /* (a scene's: LorenzoModel_SetMatrix - both frames its matrix) */
        return;
    }
    c->asked = 1;
    c->pitch = pitch;
    c->yaw = yaw;
    c->body = c->front = mat4_identity();
    if (nav == NULL || tri < 0 || (front == 0.0f && back == 0.0f)) {
        return;
    }
    floor = humanlegs_floor_at(nav, tri, a->pos);
    dy = fabsf(a->pos.y - floor);
    if (!(dy <= 1.0e-4f || a->pos.y <= floor)) {   /* (Chair_OnFloor: off it, as he stands) */
        return;
    }
    unturn = mat4_identity();   /* (his heading undone: the frames in model space) */
    unturn.m[0] = cy;   unturn.m[2] = sy;
    unturn.m[8] = -sy;  unturn.m[10] = cy;
    p[0] = a->pos.x + ahead.x * front;   /* the front: the floor `front` ahead, along him to it */
    p[1] = a->pos.y;
    p[2] = a->pos.z + ahead.z * front;
    humanlegs_onto_floor(nav, tri, mask, a->pos, p);
    d = unit(vec3_sub(vec3(p[0], p[1], p[2]), a->pos));
    c->front = mat4_mul(unturn, chair_frame(d));
    p[0] = a->pos.x + ahead.x * back;    /* the body: the floor `back` behind, along it to him */
    p[1] = a->pos.y;
    p[2] = a->pos.z + ahead.z * back;
    humanlegs_onto_floor(nav, tri, mask, a->pos, p);
    d = unit(vec3_sub(a->pos, vec3(p[0], p[1], p[2])));
    c->body = mat4_mul(unturn, chair_frame(d));
}

void chair_tick(Chair *c) {
    c->fitted = c->asked;
    c->asked = 0;
}

/* LorenzoModel_AdjustBone */
void chair_adjust(int i, Mat4 *world, void *user) {
    const Chair *c = user;

    if (!c->on || !c->fitted) {
        return;
    }
    switch (i) {
    case 0:   /* (the body on its tilt: the root, and so all of him) */
        world[0] = mat4_mul(c->body, world[0]);
        break;
    case CHAIR_FRONT_BONE: {   /* (the front on its own: the body's tilt taken back off, the front's on) */
        Vec3 at = vec3(world[i].m[12], world[i].m[13], world[i].m[14]);
        Mat4 back = rotation_of(c->body);
        int r, k;

        for (r = 0; r < 3; r++) {   /* (the body's tilt undone: its transpose) */
            for (k = 0; k < 3; k++) {
                back.m[r * 4 + k] = c->body.m[k * 4 + r];
            }
        }
        world[i] = mat4_mul(c->front, mat4_mul(back, rotation_of(world[i])));
        world[i].m[12] = at.x;
        world[i].m[13] = at.y;
        world[i].m[14] = at.z;
        break;
    }
    case HEAD_LOW_BONE:
        turn_two(&world[i], &c->front, 0.0f, 0.3f * -c->yaw);
        break;
    case HEAD_MID_BONE: {
        Mat4 f = frame_keep_z(&world[HEAD_LOW_BONE], axis(&c->front, 1));

        turn_two(&world[i], &f, 0.4f * c->pitch, 0.3f * -c->yaw);
        break;
    }
    case HEAD_TOP_BONE: {
        Mat4 f = frame_keep_z(&world[HEAD_MID_BONE], axis(&c->front, 1));

        turn_two(&world[i], &f, 0.2f * c->pitch, 0.4f * -c->yaw);
        break;
    }
    }
}
