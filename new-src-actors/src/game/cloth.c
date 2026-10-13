/* A model's hanging parts: see cloth.h. Ported from src/game/model.c (SpringSet_Begin / Step /
 * Finish; SwayPoint_Step / Pose, HairPoint2_Frame / AdjustBone, HangPoint_Frame,
 * BoneHangPoint_Frame, SprungPoint_Frame / AdjustBone, SpringPartBase_AdjustBone, the sphere's
 * push) and the models' setups (src/game/fiona.c FionaModel_Setup and its sets,
 * src/game/model.c EventHumanModel_Setup and its sets).
 *
 * The bones are worked on in the room, as the original's skeleton is: Mat4.m[4k..4k+2] is axis
 * k, m[12..14] the place (the original's rows). */
#include "cloth.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159265f

/* ---- the bones in the room, this pose ---- */
static Mat4 sRoom[128];
static int sChanged[128];
static int sBones;

static Vec3 axis(const Mat4 *m, int k) { return vec3(m->m[4 * k], m->m[4 * k + 1], m->m[4 * k + 2]); }
static Vec3 place_of(const Mat4 *m) { return vec3(m->m[12], m->m[13], m->m[14]); }
static void set_axis(Mat4 *m, int k, Vec3 v) { m->m[4 * k] = v.x; m->m[4 * k + 1] = v.y; m->m[4 * k + 2] = v.z; }
static Vec3 turned(const Mat4 *m, Vec3 v) {   /* (sceVu0ApplyMatrix with w 0) */
    return vec3_add(vec3_add(vec3_scale(axis(m, 0), v.x), vec3_scale(axis(m, 1), v.y)), vec3_scale(axis(m, 2), v.z));
}
static Mat4 *bone(int b) {
    static Mat4 none;

    return b >= 0 && b < sBones ? &sRoom[b] : &none;
}
static Vec3 unit(Vec3 v) { return vec3_len(v) > 0.0f ? vec3_norm(v) : v; }
static float angle_to(Vec3 a, Vec3 b) {   /* (msl_acosf of the dot, kept within its range) */
    float d = vec3_dot(a, b);

    return acosf(d > 1.0f ? 1.0f : d < -1.0f ? -1.0f : d);
}
/* turned back onto the cone of `cone` about `x` (as the original: the sines' blend, unit d) */
static Vec3 within_cone(Vec3 d, Vec3 x, float cone) {
    float ang = angle_to(d, x), s;

    if (ang <= cone) {
        return d;
    }
    s = sinf(ang);
    if (s <= 0.0f) {
        return d;
    }
    return vec3_scale(vec3_add(vec3_scale(x, sinf(ang - cone)), vec3_scale(d, sinf(cone))), 1.0f / s);
}

static Vec3 anchor_of(const Cloth *c, const ClothSet *s, int i) {
    const ClothNode *n = &c->nodes[i];

    return n->anchored || i == s->first ? place_of(bone(n->bone)) : c->nodes[i - 1].p;
}

/* ---- a step of each kind ---- */
static void sway_step(Cloth *c, const ClothSet *s, int i, int again) {
    ClothNode *n = &c->nodes[i];
    Mat4 *from = bone(n->along);
    Vec3 x = axis(from, 0), at = anchor_of(c, s, i), prev = n->p, d, out;
    float k1, k2;

    n->v = vec3_scale(vec3_add(n->v, vec3_scale(x, s->force.y)), s->damp);
    n->p = vec3_add(n->p, n->v);
    d = within_cone(unit(vec3_sub(n->p, at)), x, n->cone);
    d = vec3_scale(d, n->len);
    n->p = vec3_add(at, d);
    n->v = vec3_sub(n->p, prev);
    out = turned(from, n->side);
    if (!again) {   /* SwayPoint: pushed back out when behind the outward side */
        float dot = vec3_dot(d, out);

        if (dot < 0.0f) {
            prev = n->p;
            n->p = vec3_add(n->p, vec3_scale(out, -dot));
            n->p = vec3_add(at, vec3_scale(unit(vec3_sub(n->p, at)), n->len));
            n->v = vec3_add(n->v, vec3_sub(n->p, prev));
        }
        return;
    }
    k1 = vec3_dot(d, x);   /* HairPoint2: along the axis and to the outward side only */
    k2 = vec3_dot(d, out);
    prev = n->p;
    if (k2 < 0.0f) {
        k2 = 0.0f;
    }
    d = vec3_scale(unit(vec3_add(vec3_scale(x, k1), vec3_scale(out, k2))), n->len);
    n->p = vec3_add(at, d);
    n->v = vec3_add(n->v, vec3_sub(n->p, prev));
}

static Vec3 sphere_push(const ClothSphere *sp, Vec3 pt, float k) {
    Vec3 d = vec3_sub(pt, sp->at);
    float dd = vec3_dot(d, d);

    if (dd < sp->r * sp->r && dd > 0.0f) {
        return vec3_scale(d, (1.0f - sqrtf(dd) / sp->r) * k);
    }
    return vec3(0.0f, 0.0f, 0.0f);
}

static void hang_step(Cloth *c, const ClothSet *s, int i) {
    ClothNode *n = &c->nodes[i];
    Vec3 prev = n->p, at;
    int k;

    n->v = vec3_sub(n->v, s->force);
    for (k = s->sphere; k < s->sphere + s->nspheres; k++) {
        n->v = vec3_add(n->v, sphere_push(&c->spheres[k], n->p, 1.0f));
    }
    n->v = vec3_scale(n->v, s->damp);
    n->p = vec3_add(n->p, n->v);
    at = anchor_of(c, s, i);
    n->p = vec3_add(at, vec3_scale(unit(vec3_sub(n->p, at)), n->len));
    n->v = vec3_sub(n->p, prev);
}

static void bonehang_step(Cloth *c, const ClothSet *s, int i) {
    ClothNode *n = &c->nodes[i];
    const Mat4 *m = bone(n->bone);
    Vec3 prev = n->p, at = place_of(m), x = axis(m, 0), y = axis(m, 1), d;
    float k, cone = PI / 6.0f;

    n->v = vec3_scale(vec3_sub(n->v, s->force), s->damp);
    n->p = vec3_add(n->p, n->v);
    d = vec3_sub(n->p, at);
    k = vec3_dot(d, y);
    if (k < 0.0f) {   /* in front of the bone's Y */
        d = vec3_sub(d, vec3_scale(y, k));
    }
    d = vec3_scale(unit(d), n->len);
    {   /* within 30 degrees of its X (tested on the length-scaled offset, as the original) */
        float ang = angle_to(d, x);

        if (ang > cone) {
            d = vec3_scale(vec3_add(vec3_scale(x, sinf(ang - cone)), vec3_scale(d, sinf(cone))), 1.0f / sinf(ang));
        }
    }
    n->p = vec3_add(at, d);
    n->v = vec3_sub(n->p, prev);
}

static void sprung_step(Cloth *c, const ClothSet *s, int i) {
    ClothNode *n = &c->nodes[i];
    const Mat4 *m = bone(n->bone);
    Vec3 o = place_of(m), x = axis(m, 0), y = axis(m, 1), z = axis(m, 2), prev = n->p, d;
    float px, py;

    n->v = vec3_scale(vec3_add(n->v, vec3_scale(vec3_sub(o, n->p), n->len)), s->damp);
    n->p = vec3_add(n->p, n->v);
    d = vec3_sub(n->p, o);
    d = vec3_sub(d, vec3_scale(z, vec3_dot(d, z)));
    px = fmaxf(-0.2f, fminf(0.2f, vec3_dot(d, x)));
    py = fmaxf(-0.2f, fminf(0.3f, vec3_dot(d, y)));
    n->p = vec3_add(o, vec3_add(vec3_scale(x, px), vec3_scale(y, py)));
    n->v = vec3_sub(n->p, prev);
}

/* ---- the bones aimed at the parts (in list order: a part's bone sits where the one before
 * it points) ---- */
/* X from the anchor to the part, Z across it and `up`, Y square to both, at the anchor
 * (SwayPoint_Pose / HairPoint2_AdjustBone: up the outward side; SpringPartBase_AdjustBone: the
 * anchor bone's Y) */
static void aim(Mat4 *m, Vec3 x, Vec3 up, Vec3 at) {
    Vec3 z = vec3_cross(x, up), y = vec3_cross(z, x);

    set_axis(m, 0, unit(x));
    set_axis(m, 1, unit(y));
    set_axis(m, 2, unit(z));
    m->m[12] = at.x;
    m->m[13] = at.y;
    m->m[14] = at.z;
}

static void adjust(Cloth *c, const ClothSet *s, int i) {
    ClothNode *n = &c->nodes[i];
    Mat4 *m;
    Vec3 at;

    if (n->bone < 0 || n->bone >= sBones) {
        return;
    }
    m = bone(n->bone);
    at = anchor_of(c, s, i);
    switch (n->kind) {
    case CLOTH_SWAY:
    case CLOTH_CHAIN:
        aim(m, vec3_sub(n->p, at), turned(bone(n->along), n->side), at);
        break;
    case CLOTH_HANG:
    case CLOTH_BONEHANG:
        aim(m, vec3_sub(n->p, at),
            axis(n->anchored || i == s->first ? m : bone(c->nodes[i - 1].bone), 1), at);
        break;
    case CLOTH_SPRUNG:
        set_axis(m, 0, vec3_scale(axis(m, 0), n->scale.x));
        set_axis(m, 1, vec3_scale(axis(m, 1), n->scale.y));
        set_axis(m, 2, vec3_scale(axis(m, 2), n->scale.z));
        m->m[12] = n->p.x;
        m->m[13] = n->p.y;
        m->m[14] = n->p.z;
        break;
    }
    sChanged[n->bone] = 1;
}

/* ---- settling afresh: the hanging set under its anchors along its bone's Z, the others out
 * along their axes, at rest (the original's points start at the origin; its 30 steps bring
 * them in) ---- */
static void settle(Cloth *c) {
    int k, i;

    for (k = 0; k < c->nsets; k++) {
        const ClothSet *s = &c->sets[k];

        for (i = s->first; i < s->first + s->n; i++) {
            ClothNode *n = &c->nodes[i];
            Vec3 at = anchor_of(c, s, i), dir;

            if (k == c->hang_set) {
                dir = vec3_scale(axis(bone(c->hang_bone), 2), -1.0f);
            } else if (n->kind == CLOTH_SWAY || n->kind == CLOTH_CHAIN) {
                dir = axis(bone(n->along), 0);
            } else {
                dir = axis(bone(n->bone), 0);
            }
            n->p = n->kind == CLOTH_SPRUNG ? at : vec3_add(at, vec3_scale(dir, n->len));
            n->v = vec3(0.0f, 0.0f, 0.0f);
        }
    }
}

void cloth_tick(Cloth *c) {
    if (c != NULL && c->pending < 4) {
        c->pending++;
    }
}

void cloth_pose(Cloth *c, Mat4 *world, int nbones, const Mat4 *place, const Mat4 *unplace) {
    int steps, k, i, j;
    Vec3 root = place_of(place);

    sBones = nbones < 128 ? nbones : 128;
    for (i = 0; i < sBones; i++) {
        sRoom[i] = mat4_mul(*place, world[i]);
        sChanged[i] = 0;
    }
    if (vec3_len(vec3_sub(root, c->root)) > 30.0f) {   /* (put somewhere else: settle there) */
        c->reset = 1;
    }
    c->root = root;
    if (c->reset) {
        settle(c);
        steps = 30;
    } else {
        steps = c->pending;
    }
    c->reset = 0;
    c->pending = 0;
    for (k = 0; k < c->nspheres; k++) {   /* (SpringSet_Begin: the spheres on their bones) */
        c->spheres[k].at = mat4_point(bone(c->spheres[k].bone), c->spheres[k].off);
    }
    for (j = 0; j < steps; j++) {
        for (k = 0; k < c->nsets; k++) {
            const ClothSet *s = &c->sets[k];

            for (i = s->first; i < s->first + s->n; i++) {
                switch (c->nodes[i].kind) {
                case CLOTH_SWAY: sway_step(c, s, i, 0); break;
                case CLOTH_CHAIN: sway_step(c, s, i, 1); break;
                case CLOTH_HANG: hang_step(c, s, i); break;
                case CLOTH_BONEHANG: bonehang_step(c, s, i); break;
                case CLOTH_SPRUNG: sprung_step(c, s, i); break;
                }
            }
        }
    }
    for (k = 0; k < c->nsets; k++) {
        const ClothSet *s = &c->sets[k];

        for (i = s->first; i < s->first + s->n; i++) {
            adjust(c, s, i);
        }
    }
    for (i = 0; i < sBones; i++) {
        if (sChanged[i]) {
            world[i] = mat4_mul(*unplace, sRoom[i]);
        }
    }
}

/* ==== the models' parts ==== */
static ClothSet *add_set(Cloth *c, float force, float damp) {
    ClothSet *s = &c->sets[c->nsets++];

    *s = (ClothSet){vec3(0.0f, force, 0.0f), damp, c->nnodes, 0, c->nspheres, 0};
    return s;
}
static ClothNode *add_node(Cloth *c, ClothSet *s, ClothKind kind, int b, int anchored, float len) {
    ClothNode *n = &c->nodes[c->nnodes++];

    memset(n, 0, sizeof(*n));
    n->kind = kind;
    n->bone = b;
    n->anchored = anchored;
    n->len = len;
    n->scale = vec3(1.0f, 1.0f, 1.0f);
    s->n++;
    return n;
}
static void add_spheres(Cloth *c, ClothSet *s, const int *bones) {   /* five on bones, the sixth 1 up its bone */
    int i;

    for (i = 0; i < 6; i++) {
        c->spheres[c->nspheres++] = (ClothSphere){bones[i], vec3(0.0f, i == 5 ? 1.0f : 0.0f, 0.0f), 1.0f, vec3(0, 0, 0)};
    }
    s->nspheres = 6;
}

/* her slip (FionaModel_Setup): the slip's eight strips of four from her hips, a bone hanging
 * from her head, her hair (four, off her head and shoulders), her chest */
static void slip(Cloth *c) {
    static const int kAlong[8] = {2, 2, 6, 6, 2, 2, 6, 6};
    static const float kSide[8][3] = {
        {0, 0.34f, 0.94f}, {0, 1, 0}, {0, 1, 0}, {0, 0.34f, -0.94f},
        {0, -0.34f, 0.94f}, {0, -1, 0}, {0, -1, 0}, {0, -0.34f, -0.94f},
    };
    static const int kHairSpheres[6] = {0x2E, 0x3E, 0x35, 0x2F, 0x3F, 0x35};
    ClothSet *s;
    int g, i;

    s = add_set(c, 0.4f, 0.6f);
    for (g = 0; g < 8; g++) {
        for (i = 0; i < 4; i++) {
            ClothNode *n = add_node(c, s, CLOTH_CHAIN, 0xB + g * 4 + i, i == 0, 1.0f);

            n->along = kAlong[g];
            n->side = vec3(kSide[g][0], kSide[g][1], kSide[g][2]);
            n->cone = 2.0f * PI * (float)(i + 1) / 18.0f;
        }
    }
    s = add_set(c, 0.1f, 0.99f);
    add_node(c, s, CLOTH_BONEHANG, 0x3D, 1, 1.0f);
    c->hang_set = c->nsets;
    c->hang_bone = 0x35;
    s = add_set(c, 0.1f, 0.8f);
    for (i = 0; i < 4; i++) {
        add_node(c, s, CLOTH_HANG, 0x39 + i, i == 0, 0.45f);
    }
    add_spheres(c, s, kHairSpheres);
    s = add_set(c, 0.0f, 0.75f);
    add_node(c, s, CLOTH_SPRUNG, 0x2D, 1, 0.9f);
}

/* her clothes (EventHumanModel_Setup): her skirt's six pairs from her hips, a bone hanging, her
 * hair, her chest, another bone hanging */
static void clothes(Cloth *c) {
    static const int kBones[6] = {0xB, 0x11, 0xD, 0x13, 0xF, 0x15};
    static const float kSide[6][3] = {{0, 1, 0}, {0, 1, 0}, {0, -1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
    static const int kHairSpheres[6] = {0x1A, 0x2A, 0x21, 0x1B, 0x2B, 0x21};
    ClothSet *s;
    int i, j;

    s = add_set(c, 0.2f, 0.9f);
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 2; j++) {
            ClothNode *n = add_node(c, s, CLOTH_SWAY, kBones[i] + j, j == 0, 0.4092f);

            n->along = i & 1 ? 6 : 2;
            n->side = vec3(kSide[i][0], kSide[i][1], kSide[i][2]);
            n->cone = j == 0 ? 0.34906587f : 0.69813174f;
        }
    }
    s = add_set(c, 0.1f, 0.99f);
    add_node(c, s, CLOTH_BONEHANG, 0x29, 1, 1.0f);
    c->hang_set = c->nsets;
    c->hang_bone = 0x21;
    s = add_set(c, 0.1f, 0.8f);
    for (i = 0; i < 4; i++) {
        add_node(c, s, CLOTH_HANG, 0x25 + i, i == 0, 0.45f);
    }
    add_spheres(c, s, kHairSpheres);
    s = add_set(c, 0.0f, 0.75f);
    add_node(c, s, CLOTH_SPRUNG, 0x19, 1, 0.9f);
    s = add_set(c, 0.1f, 0.99f);
    add_node(c, s, CLOTH_BONEHANG, 0x30, 1, 1.0f);
}

Cloth *cloth_for(const char *name) {
    Cloth *c;
    int is_slip = strncmp(name, "O_FIS/", 6) == 0, is_clothes = strncmp(name, "O_FIN/", 6) == 0;

    if (!is_slip && !is_clothes) {
        return NULL;
    }
    c = calloc(1, sizeof(*c));
    if (c == NULL) {
        return NULL;
    }
    c->hang_set = -1;
    if (is_slip) {
        slip(c);
    } else {
        clothes(c);
    }
    c->reset = 1;
    return c;
}
