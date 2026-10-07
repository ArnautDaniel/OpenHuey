/* A dog's legs on the floor: see doglegs.h. Ported from src/game/hewie.c DogModel_PlantFeet /
 * DogModel_LegIK and src/game/model.c IK2_* / IK3_* (the two- and three-bone solvers).
 *
 * The bone matrices are worked on as the original's rows: Mat4.m[4k..4k+2] is axis k, m[12..14]
 * the place (new-src's column-major layout is the same memory). */
#include "doglegs.h"

#include <math.h>
#include <string.h>

static const int kFootBones[4] = {0xB, 0xF, 0x17, 0x1C};

/* ---- small vector helpers (3 floats) ---- */
static void v_sub(float *o, const float *a, const float *b) { o[0] = a[0] - b[0]; o[1] = a[1] - b[1]; o[2] = a[2] - b[2]; }
static void v_madd(float *o, const float *a, const float *b, float s) {
    o[0] = a[0] + b[0] * s; o[1] = a[1] + b[1] * s; o[2] = a[2] + b[2] * s;
}
static float v_dot(const float *a, const float *b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
static void v_cross(float *o, const float *a, const float *b) {
    float x = a[1] * b[2] - a[2] * b[1], y = a[2] * b[0] - a[0] * b[2], z = a[0] * b[1] - a[1] * b[0];

    o[0] = x; o[1] = y; o[2] = z;
}
static void v_norm(float *o, const float *a) {
    float l = sqrtf(v_dot(a, a));

    if (l > 0.0f) {
        o[0] = a[0] / l; o[1] = a[1] / l; o[2] = a[2] / l;
    } else if (o != a) {
        o[0] = a[0]; o[1] = a[1]; o[2] = a[2];
    }
}
static void v_copy(float *o, const float *a) { o[0] = a[0]; o[1] = a[1]; o[2] = a[2]; }
static float *row(Mat4 *m, int k) { return &m->m[k * 4]; }

/* IK2_PlaceMiddle: the middle joint of a two-bone chain from root toward end, bent to the side
 * of pole x direction; 1 when end is out of reach */
static int place_middle(const float *root, float *mid, const float *end, const float *pole, float l1, float l2,
                        float bend) {
    float d[3], side[3], dist, a, h, ll;
    int out = 0;

    v_sub(d, end, root);
    dist = sqrtf(fabsf(v_dot(d, d)));
    v_norm(d, d);
    if (!(dist <= l1 + l2)) {
        dist = l1 + l2;
        out = 1;
    }
    v_cross(side, pole, d);
    v_norm(side, side);
    side[0] *= bend; side[1] *= bend; side[2] *= bend;
    ll = l1 * l1;
    a = dist > 0.0f ? (ll - l2 * l2 + dist * dist) / (2.0f * dist) : 0.0f;
    h = sqrtf(fabsf(ll - a * a));
    mid[0] = root[0] + a * d[0] + h * side[0];
    mid[1] = root[1] + a * d[1] + h * side[1];
    mid[2] = root[2] + a * d[2] + h * side[2];
    return out;
}

/* ik_aim: X from -> to, Z from the pole made square to it, at `from` */
static void aim(Mat4 *m, const float *from, const float *to, const float *pole) {
    float *x = row(m, 0), *y = row(m, 1), *z = row(m, 2);

    v_sub(x, to, from);
    v_copy(z, pole);
    v_cross(y, z, x);
    v_cross(z, x, y);
    v_norm(x, x); v_norm(y, y); v_norm(z, z);
    x[3] = y[3] = z[3] = 0.0f;
    m->m[12] = from[0]; m->m[13] = from[1]; m->m[14] = from[2]; m->m[15] = 1.0f;
}

/* ik_turn: X from -> to, keeping its Z side */
static void turn(Mat4 *m, const float *from, const float *to) {
    float *x = row(m, 0), *y = row(m, 1), *z = row(m, 2);

    v_sub(x, to, from);
    v_cross(y, z, x);
    v_cross(z, x, y);
    v_norm(x, x); v_norm(y, y); v_norm(z, z);
    x[3] = y[3] = z[3] = 0.0f;
    m->m[12] = from[0]; m->m[13] = from[1]; m->m[14] = from[2]; m->m[15] = 1.0f;
}

static float round_fraction(const float *a, const float *b, const float *c) {
    float la = sqrtf(fabsf(v_dot(a, a))), lb = sqrtf(fabsf(v_dot(b, b))), lc = sqrtf(fabsf(v_dot(c, c)));
    float cab = la * lb > 0.0f ? v_dot(a, b) / (la * lb) : 1.0f, cac = la * lc > 0.0f ? v_dot(a, c) / (la * lc) : 1.0f;
    float ab = acosf(cab < -1.0f ? -1.0f : cab > 1.0f ? 1.0f : cab), ac = acosf(cac < -1.0f ? -1.0f : cac > 1.0f ? 1.0f : cac);

    return ab + ac > 0.0f ? ab / (ab + ac) : 0.5f;
}

static void slerp(float *o, const float *p, const float *q, float w, float t) {
    float s = sinf(w), sp, sq;

    if (s == 0.0f) {
        v_copy(o, p);
        return;
    }
    sp = sinf(w * (1.0f - t));
    sq = sinf(w * t);
    o[0] = (q[0] * sq + p[0] * sp) / s;
    o[1] = (q[1] * sq + p[1] * sp) / s;
    o[2] = (q[2] * sq + p[2] * sp) / s;
}

static float safe_acos(float x) { return acosf(x < -1.0f ? -1.0f : x > 1.0f ? 1.0f : x); }

/* IK_RoundVector: t of the way round from a to b, by way of mid when they lie on its either side */
static void round_vector(float *o, const float *a, const float *mid, const float *b, float t) {
    float ca[3], cb[3], w1, w2, sum;

    v_cross(ca, mid, a);
    v_cross(cb, mid, b);
    if (!(v_dot(ca, cb) < 0.0f)) {
        slerp(o, a, b, safe_acos(v_dot(a, b)), t);
        return;
    }
    w1 = safe_acos(v_dot(a, mid));
    w2 = safe_acos(v_dot(mid, b));
    sum = w1 + w2;
    if (sum <= 0.0f) {
        v_copy(o, a);
    } else if (t < w1 / sum) {
        slerp(o, a, mid, w1, t * sum / w1);
    } else {
        slerp(o, mid, b, w2, (t * sum - w1) / w2);
    }
}

/* IK3_PlaceKneeHock */
static void place_knee_hock(const float *root, float *knee, float *hock, float *foot, const float *pole, float l1,
                            float l2, float l3, float b1, float b2, float t, float reach) {
    float up[3], dk[3], dh[3], dir[3], dist;
    int r1, r2;

    v_sub(dk, foot, root);
    up[0] = -dk[0]; up[1] = -dk[1]; up[2] = -dk[2];
    r1 = place_middle(root, knee, foot, pole, l1, l2 + l3, b1);
    v_sub(dk, knee, foot);
    v_norm(dk, dk);
    r2 = place_middle(root, hock, foot, pole, l1 + l2, l3, b2);
    v_sub(dh, hock, foot);
    v_norm(dh, dh);
    dist = sqrtf(fabsf(v_dot(up, up)));
    v_norm(up, up);
    if (r1 == 0 && r2 == 0) {
        round_vector(dir, dk, up, dh, t);
    } else {
        v_copy(dir, dk);
    }
    if (!(dist <= reach)) {
        v_madd(foot, root, up, -reach);
    }
    v_madd(hock, foot, dir, l3);
    place_middle(root, knee, hock, pole, l1, l2, b1);
}

/* IK3_Loaded: a front leg (root, knee, hock, foot bones) onto `target`, keeping its pose */
static void leg3(Mat4 *w, const int *b, const float *lens, float reach, const float *target_in) {
    Mat4 *R = &w[b[0]], *K = &w[b[1]], *H = &w[b[2]], *F = &w[b[3]];
    float root[3], knee[3], hock[3], pole[3], v[3], flat[3], kneeAt[3], hockAt[3], target[3], foot_at[3];
    float off, along, t;

    v_copy(root, &R->m[12]);
    v_copy(knee, &K->m[12]);
    v_copy(hock, &H->m[12]);
    v_copy(pole, row(R, 2));
    v_copy(foot_at, &F->m[12]);
    v_copy(target, target_in);
    v_sub(v, foot_at, hock);
    along = v_dot(pole, v);
    off = sqrtf(fabsf(v_dot(v, v) - along * along));
    v_madd(flat, target, pole, -along);
    place_middle(root, kneeAt, foot_at, pole, lens[0], lens[1] + lens[2], 1.0f);
    place_middle(root, hockAt, foot_at, pole, lens[0] + lens[1], lens[2], -1.0f);
    v_sub(v, hock, foot_at);
    v_sub(kneeAt, kneeAt, foot_at);
    v_sub(hockAt, hockAt, foot_at);
    t = round_fraction(v, kneeAt, hockAt);
    place_knee_hock(root, knee, hock, flat, pole, lens[0], lens[1], off, 1.0f, -1.0f, t, reach);
    v_madd(target, flat, pole, along);
    aim(R, root, knee, pole);
    aim(K, knee, hock, pole);
    aim(H, hock, target, pole);
    F->m[12] = target[0]; F->m[13] = target[1]; F->m[14] = target[2];
}

/* IK2_Loaded: a hind leg (hip, knee, foot bones) onto `target`, keeping the knee's side */
static void leg2(Mat4 *w, const int *b, const float *lens, const float *target_in) {
    Mat4 *R = &w[b[0]], *M = &w[b[1]], *E = &w[b[2]];
    float root[3], mid[3], z[3], a[3], bb[3], pole[3], target[3], d[3];

    v_copy(root, &R->m[12]);
    v_copy(target, target_in);
    v_copy(z, row(R, 2));
    v_sub(a, target, root);
    v_cross(bb, z, a);
    v_cross(pole, a, bb);
    v_norm(pole, pole);
    v_sub(a, &M->m[12], root);
    v_sub(bb, &E->m[12], &M->m[12]);
    v_cross(z, a, bb);
    if (place_middle(root, mid, target, pole, lens[0], lens[1], v_dot(row(R, 2), z) <= 0.0f ? 1.0f : -1.0f) != 0) {
        v_sub(d, target, root);
        v_norm(d, d);
        v_madd(target, root, d, lens[0] + lens[1]);
    }
    turn(R, root, mid);
    turn(M, mid, target);
    E->m[12] = target[0]; E->m[13] = target[1]; E->m[14] = target[2];
}

static const int kFront[2][4] = {{8, 9, 10, 11}, {12, 13, 14, 15}};
static const int kHind[2][3] = {{0x15, 0x16, 0x17}, {0x1A, 0x1B, 0x1C}};

void doglegs_reset(DogLegs *d) {
    memset(d, 0, sizeof(*d));
}

void doglegs_pose(DogLegs *d, const Model *m, Mat4 *world, const Mat4 *place, const Mat4 *unplace) {
    int i;

    if (!d->on || m->nbones <= 0x1C) {
        return;
    }
    /* DogModel_PlantFeet: in the room, a foot down this frame and the last is held where it
     * landed; one lifting eases back to the animation over 4 frames */
    for (i = 0; i < 4; i++) {
        Vec3 at = mat4_point(place, vec3(world[kFootBones[i]].m[12], world[kFootBones[i]].m[13], world[kFootBones[i]].m[14]));
        float a3[3] = {at.x, at.y, at.z};

        if (!d->init) {
            d->was[i] = d->planted[i];
            d->planted[i] = 0;
            v_copy(d->foot[i], a3);
            continue;
        }
        d->was[i] = d->planted[i];
        if (d->left[i] == 0 && d->down[i] && d->down_was[i]) {
            d->planted[i] = 1;
            continue;
        }
        if (d->was[i]) {
            d->len[i] = 5;
            d->left[i] = 4;
            v_sub(d->off[i], d->foot[i], a3);
        }
        if (d->left[i] == 0) {
            v_copy(d->foot[i], a3);
        } else {
            float k = (float)d->left[i] / (float)d->len[i];

            k *= k;
            v_madd(d->foot[i], a3, d->off[i], k);
            d->left[i]--;
        }
        d->planted[i] = 0;
    }
    if (!d->init) {
        d->init = 1;
        return;
    }
    /* DogModel_LegIK: the legs' lengths from the bind pose, the feet on the targets (model
     * space) */
    for (i = 0; i < 2; i++) {
        float lens[3] = {m->bones[kFront[i][1]].rest_pos[0], m->bones[kFront[i][2]].rest_pos[0],
                         m->bones[kFront[i][3]].rest_pos[0]};
        float reach = fabsf(lens[0]) + fabsf(lens[1]) + fabsf(lens[2]);
        Vec3 t = mat4_point(unplace, vec3(d->foot[i][0], d->foot[i][1], d->foot[i][2]));
        float t3[3] = {t.x, t.y, t.z};

        lens[0] = fabsf(lens[0]); lens[1] = fabsf(lens[1]); lens[2] = fabsf(lens[2]);
        leg3(world, kFront[i], lens, reach, t3);
    }
    for (i = 0; i < 2; i++) {
        float lens[2] = {fabsf(m->bones[kHind[i][1]].rest_pos[0]), fabsf(m->bones[kHind[i][2]].rest_pos[0])};
        Vec3 t = mat4_point(unplace, vec3(d->foot[2 + i][0], d->foot[2 + i][1], d->foot[2 + i][2]));
        float t3[3] = {t.x, t.y, t.z};

        leg2(world, kHind[i], lens, t3);
    }
}
