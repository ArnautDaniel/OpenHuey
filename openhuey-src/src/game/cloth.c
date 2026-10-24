/* A model's hanging parts: see cloth.h. Ported from src/game/model.c (SpringSet_Begin / Step /
 * Finish; SwayPoint_Step / Pose, HairPoint2_Frame / AdjustBone, HangPoint_Frame,
 * BoneHangPoint_Frame, SprungPoint_Frame / AdjustBone, SpringPartBase_AdjustBone, HairPoint_Step /
 * Pose, HangingPart_Step, the sphere's and the capsule's push) and the models' setups
 * (src/game/fiona.c FionaModel_Setup and its sets, src/game/model.c EventHumanModel_Setup and
 * its sets, src/game/daniella.c DaniellaModel_Springs and its sets).
 *
 * The bones are worked on in the room, as the original's skeleton is: Mat4.m[4k..4k+2] is axis
 * k, m[12..14] the place (the original's rows). */
#include "cloth.h"

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
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

    return n->anchored || i == s->first ? place_of(bone(n->bone)) : c->nodes[n->from >= 0 ? n->from : i - 1].p;
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

/* a capsule's push (Capsule_Push): off its segment when the point is alongside, else off the
 * nearer end (that one not made unit, as the original) */
static Vec3 capsule_push(const ClothSphere *cp, Vec3 pt, float k) {
    Vec3 ax = unit(vec3_sub(cp->at2, cp->at)), d;
    float t0 = vec3_dot(ax, cp->at), t1 = vec3_dot(ax, cp->at2), tp = vec3_dot(ax, pt);
    float lo = fminf(t0, t1), hi = fmaxf(t0, t1), l2, l1;

    if (tp >= lo && tp <= hi) {
        d = vec3_sub(pt, cp->at);
        d = vec3_sub(d, vec3_scale(ax, vec3_dot(ax, d)));
        l2 = vec3_dot(d, d);
        if (l2 < cp->r * cp->r && l2 > 0.0f) {
            return vec3_scale(unit(d), (1.0f - sqrtf(l2) / cp->r) * k);
        }
    }
    d = vec3_sub(pt, cp->at);
    l2 = vec3_dot(d, d);
    l1 = vec3_dot(vec3_sub(pt, cp->at2), vec3_sub(pt, cp->at2));
    if (l1 < l2) {
        l2 = l1;
        d = vec3_sub(pt, cp->at2);
    }
    if (l2 < cp->r * cp->r && l2 > 0.0f) {
        return vec3_scale(d, (1.0f - sqrtf(l2) / cp->r) * k);
    }
    return vec3(0.0f, 0.0f, 0.0f);
}
static Vec3 collider_push(const ClothSphere *sp, Vec3 pt, float k) {
    return sp->capsule ? capsule_push(sp, pt, k) : sphere_push(sp, pt, k);
}

static void hang_step(Cloth *c, const ClothSet *s, int i) {
    ClothNode *n = &c->nodes[i];
    Vec3 prev = n->p, at;
    int k;

    n->v = vec3_sub(n->v, s->force);
    for (k = s->sphere; k < s->sphere + s->nspheres; k++) {
        n->v = vec3_add(n->v, collider_push(&c->spheres[k], n->p, 1.0f));
    }
    n->v = vec3_scale(n->v, s->damp);
    n->p = vec3_add(n->p, n->v);
    at = anchor_of(c, s, i);
    n->p = vec3_add(at, vec3_scale(unit(vec3_sub(n->p, at)), n->len));
    n->v = vec3_sub(n->p, prev);
}

/* a hair point (HairPoint_Step): moved with its anchor by its stiffness, falling, off the
 * colliders (three times their depth), kept about 0.438 from its partner, damped, at its length */
static void paired_step(Cloth *c, const ClothSet *s, int i) {
    ClothNode *n = &c->nodes[i];
    Vec3 at = anchor_of(c, s, i), prev = n->p, d;
    float len;
    int k;

    n->p = vec3_add(n->p, vec3_scale(vec3_sub(at, n->last_at), n->stiff));
    n->v = vec3_sub(n->v, s->force);
    for (k = s->sphere; k < s->sphere + s->nspheres; k++) {
        n->v = vec3_add(n->v, collider_push(&c->spheres[k], n->p, 3.0f));
    }
    d = vec3_sub(n->p, c->nodes[n->partner].p);
    len = vec3_len(d);
    n->v = vec3_add(n->v, vec3_scale(unit(d), 0.8f * -(len - 0.4379f)));
    n->v = vec3_scale(n->v, s->damp);
    n->p = vec3_add(n->p, n->v);
    n->p = vec3_add(at, vec3_scale(unit(vec3_sub(n->p, at)), n->len));
    n->v = vec3_sub(n->p, prev);
    n->last_at = at;
}

/* a coat part (Part60_Step): moved with its anchor (0.6), falling, off the colliders, held about
 * 1.9 from the node it is tied to (by its stiffness), damped, at its length */
static void tied_step(Cloth *c, const ClothSet *s, int i) {
    ClothNode *n = &c->nodes[i];
    Vec3 at = anchor_of(c, s, i), prev, d;
    int k;

    n->p = vec3_add(n->p, vec3_scale(vec3_sub(at, n->last_at), 0.6f));
    prev = n->p;
    n->v = vec3_sub(n->v, s->force);
    for (k = s->sphere; k < s->sphere + s->nspheres; k++) {
        n->v = vec3_add(n->v, collider_push(&c->spheres[k], n->p, 1.0f));
    }
    if (n->partner >= 0) {
        float len;

        d = vec3_sub(c->nodes[n->partner].p, n->p);
        len = vec3_len(d);
        if (len > 0.0f) {
            n->v = vec3_add(n->v, vec3_scale(d, n->stiff * (len - 1.9f) / len));
        }
    }
    n->v = vec3_scale(n->v, s->damp);
    n->p = vec3_add(n->p, n->v);
    n->p = vec3_add(at, vec3_scale(unit(vec3_sub(n->p, at)), n->len));
    n->v = vec3_sub(n->p, prev);
    n->last_at = at;
}

/* a part drawn toward where its bone points (Part50_Step): falling, toward its length out along
 * its bone's X (by its stiffness), damped, at its length */
static void reach_step(Cloth *c, const ClothSet *s, int i) {
    ClothNode *n = &c->nodes[i];
    Vec3 prev = n->p, at, t = mat4_point(bone(n->bone), vec3(n->len, 0.0f, 0.0f));

    n->v = vec3_sub(n->v, s->force);
    n->v = vec3_add(n->v, vec3_scale(vec3_sub(t, n->p), n->stiff));
    n->v = vec3_scale(n->v, s->damp);
    n->p = vec3_add(n->p, n->v);
    at = anchor_of(c, s, i);
    n->p = vec3_add(at, vec3_scale(unit(vec3_sub(n->p, at)), n->len));
    n->v = vec3_sub(n->p, prev);
}

/* a hanging part (HangingPart_Step): drawn along its pull bone's Z, falling, off the colliders,
 * damped, at its length */
static void bent_step(Cloth *c, const ClothSet *s, int i) {
    ClothNode *n = &c->nodes[i];
    Vec3 prev = n->p, at;
    int k;

    n->v = vec3_add(n->v, vec3_scale(axis(bone(n->pull), 2), n->stiff));
    n->v = vec3_sub(n->v, s->force);
    for (k = s->sphere; k < s->sphere + s->nspheres; k++) {
        n->v = vec3_add(n->v, collider_push(&c->spheres[k], n->p, 1.0f));
    }
    n->v = vec3_scale(n->v, s->damp);
    n->p = vec3_add(n->p, n->v);
    at = anchor_of(c, s, i);
    n->p = vec3_add(at, vec3_scale(unit(vec3_sub(n->p, at)), n->len));
    n->v = vec3_sub(n->p, prev);
}

/* a costume's hanging point (CostumeHangPoint_Frame / WindHangPoint_Frame): a rigid one on straight
 * from the one before; else falling (or blown by the set's wind: 0.2 a step toward it from its
 * root bone), off the spheres, at its length from what it hangs from, then within its swing
 * limit of its bone's X. (The limit is tested on the length-scaled offset and the offset scaled by
 * the length again after - both as in the original; the lengths are short) */
static void costume_step(Cloth *c, const ClothSet *s, int i) {
    ClothNode *n = &c->nodes[i];
    Vec3 prev = n->p, at, d, x;
    int k;

    if (n->rigid && i > s->first) {
        const ClothNode *q = &c->nodes[i - 1];

        n->p = vec3_add(q->p, vec3_scale(unit(vec3_sub(q->p, anchor_of(c, s, i - 1))), n->len));
        return;
    }
    if (n->kind == CLOTH_WIND && s->wind_bone >= 0) {
        int root = n->anchored || i == s->first ? n->bone
                   : c->nodes[i - 1].anchored || i - 1 == s->first ? c->nodes[i - 1].bone : c->nodes[i - 2].bone;
        Vec3 wind = mat4_point(bone(s->wind_bone), s->wind_at);

        n->v = vec3_add(n->v, vec3_scale(unit(vec3_sub(wind, place_of(bone(root)))), 0.2f));
    } else {
        n->v = vec3_sub(n->v, s->force);
    }
    for (k = s->sphere; k < s->sphere + s->nspheres; k++) {
        n->v = vec3_add(n->v, collider_push(&c->spheres[k], n->p, 1.0f));
    }
    n->v = vec3_scale(n->v, s->damp);
    n->p = vec3_add(n->p, n->v);
    at = anchor_of(c, s, i);
    d = vec3_scale(unit(vec3_sub(n->p, at)), n->len);
    n->p = vec3_add(at, d);
    if (n->kind == CLOTH_COSTUME) {
        n->v = vec3_sub(n->p, prev);
    }
    x = axis(bone(n->bone), 0);
    {
        float ang = angle_to(d, x), sa = sinf(ang);

        if (ang > n->cone && sa > 0.0f) {
            d = vec3_scale(vec3_add(vec3_scale(x, sinf(ang - n->cone)), vec3_scale(d, sinf(n->cone))), 1.0f / sa);
        }
    }
    n->p = vec3_add(at, vec3_scale(d, n->len));
    if (n->kind == CLOTH_WIND) {
        n->v = vec3_sub(n->p, prev);
    }
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
    case CLOTH_PAIRED:   /* (HairPoint_Pose: X to it, Y toward its partner) */
        aim(m, vec3_sub(n->p, at), vec3_scale(vec3_sub(c->nodes[n->partner].p, n->p), n->toward), at);
        break;
    case CLOTH_HANG:
    case CLOTH_BONEHANG:
    case CLOTH_COSTUME:
    case CLOTH_BENT:
    case CLOTH_TIED:
    case CLOTH_REACH:
        aim(m, vec3_sub(n->p, at),
            axis(n->anchored || i == s->first ? m : bone(c->nodes[n->from >= 0 ? n->from : i - 1].bone), 1), at);
        break;
    case CLOTH_WIND: {   /* (WindHangPoint_AdjustBone: X to it, keeping the anchor bone's Z) */
        Vec3 bx = unit(vec3_sub(n->p, at)), z = axis(n->anchored || i == s->first ? m : bone(c->nodes[i - 1].bone), 2);
        Vec3 by = unit(vec3_cross(z, bx));

        set_axis(m, 0, bx);
        set_axis(m, 1, by);
        set_axis(m, 2, unit(vec3_cross(bx, by)));
        m->m[12] = at.x;
        m->m[13] = at.y;
        m->m[14] = at.z;
        break;
    }
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

            if (n->kind == CLOTH_PAIRED) {   /* (DaniellaModel_HairRest: along bone 0's Z) */
                dir = axis(bone(0), 2);
                n->last_at = at;
            } else if (n->kind == CLOTH_TIED) {   /* (RiccardoModel_PartsRest: along bone 1's Z, or back) */
                dir = vec3_scale(axis(bone(n->rest_bone), 2), n->rest_way);
                n->last_at = at;
            } else if (n->kind == CLOTH_BENT) {   /* (DaniellaModel_HangingRest: its pull, the set's force) */
                dir = unit(vec3_add(vec3_scale(axis(bone(n->pull), 2), n->stiff), s->force));
            } else if (k == c->hang_set) {
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

void cloth_settle(Cloth *c) {
    if (c != NULL) {
        c->reset = 1;
    }
}

void cloth_tick(Cloth *c) {
    if (c != NULL && c->pending < 4) {
        c->pending++;
    }
}

void cloth_pose(Cloth *c, Mat4 *world, int nbones, const Mat4 *place, const Mat4 *unplace) {
    int steps, k, i, j;

    sBones = nbones < 128 ? nbones : 128;
    for (i = 0; i < sBones; i++) {
        sRoom[i] = mat4_mul(*place, world[i]);
        sChanged[i] = 0;
    }
    if (c->reset) {
        settle(c);
        steps = c->settle_steps;
    } else {
        steps = c->pending;
    }
    c->reset = 0;
    c->pending = 0;
    for (k = 0; k < c->nspheres; k++) {   /* (SpringSet_Begin: the colliders on their bones) */
        ClothSphere *sp = &c->spheres[k];

        sp->at = mat4_point(bone(sp->bone), sp->off);
        if (sp->capsule) {
            sp->at2 = mat4_point(bone(sp->bone2), sp->off2);
        }
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
                case CLOTH_COSTUME:
                case CLOTH_WIND: costume_step(c, s, i); break;
                case CLOTH_PAIRED: paired_step(c, s, i); break;
                case CLOTH_BENT: bent_step(c, s, i); break;
                case CLOTH_TIED: tied_step(c, s, i); break;
                case CLOTH_REACH: reach_step(c, s, i); break;
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

    *s = (ClothSet){vec3(0.0f, force, 0.0f), damp, c->nnodes, 0, c->nspheres, 0, -1, vec3(0, 0, 0)};
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
    n->from = -1;
    n->partner = -1;
    s->n++;
    return n;
}
static void add_spheres(Cloth *c, ClothSet *s, const int *bones) {   /* five on bones, the sixth 1 up its bone */
    int i;

    for (i = 0; i < 6; i++) {
        c->spheres[c->nspheres++] = (ClothSphere){.bone = bones[i], .off = vec3(0.0f, i == 5 ? 1.0f : 0.0f, 0.0f), .r = 1.0f};
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

/* a skirt: pairs from her hips (SwayPointA x 2 a group: on bone b and b + 1), each group out its own
 * side of hip bone 2 or 6 (alternately) */
static void skirt(Cloth *c, int groups, const int *bones, const float (*sides)[3]) {
    ClothSet *s = add_set(c, 0.2f, 0.9f);
    int i, j;

    for (i = 0; i < groups; i++) {
        for (j = 0; j < 2; j++) {
            ClothNode *n = add_node(c, s, CLOTH_SWAY, bones[i] + j, j == 0, 0.4092f);

            n->along = i & 1 ? 6 : 2;
            n->side = vec3(sides[i][0], sides[i][1], sides[i][2]);
            n->cone = j == 0 ? 0.34906587f : 0.69813174f;
        }
    }
}
static void bone_hanging(Cloth *c, int b) {   /* (a BoneHangPoint alone: gravity 0.1, damping 0.99) */
    add_node(c, add_set(c, 0.1f, 0.99f), CLOTH_BONEHANG, b, 1, 1.0f);
}
static void bone_sprung(Cloth *c, int b) {   /* (a SprungPoint alone: damping 0.75, stiffness 0.9) */
    add_node(c, add_set(c, 0.0f, 0.75f), CLOTH_SPRUNG, b, 1, 0.9f);
}

/* her clothes (EventHumanModel_Setup): her skirt's six pairs from her hips, a bone hanging, her
 * hair, her chest, another bone hanging */
static const int kSkirtBones[6] = {0xB, 0x11, 0xD, 0x13, 0xF, 0x15};
static const float kSkirtSides[6][3] = {{0, 1, 0}, {0, 1, 0}, {0, -1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
static void clothes(Cloth *c) {
    static const int kHairSpheres[6] = {0x1A, 0x2A, 0x21, 0x1B, 0x2B, 0x21};
    ClothSet *s;
    int i;

    skirt(c, 6, kSkirtBones, kSkirtSides);
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

/* ---- her other costumes (Costume2 / 3 / 6 / 7 Model_Setup) ---- */
/* a costume set's six spheres (costume_cols: bones $20 $21 $1B $3A $1A $39 moved by `shift`; the
 * first two 0.8 across) */
static void costume_spheres(Cloth *c, ClothSet *s, int shift) {
    static const int kBones[6] = {0x20, 0x21, 0x1B, 0x3A, 0x1A, 0x39};
    int i;

    for (i = 0; i < 6; i++) {
        c->spheres[c->nspheres++] = (ClothSphere){.bone = kBones[i] + shift, .r = i < 2 ? 0.8f : 1.0f};
    }
    s->nspheres = 6;
}
/* the costume's sixteen hanging points (costume_hang16: those on a bone swing within `limit`, the
 * others 60 degrees; the rigid ones go on straight) - the same lengths and pattern for both costumes */
static const float kHang16Length[16] = {0.547f, 0.4111f, 0.4111f, 0.6778f, 0.6778f, 0.5455f, 0.4294f, 0.4294f,
                                        0.547f, 0.4111f, 0.4111f, 0.6778f, 0.6778f, 0.5455f, 0.4294f, 0.4294f};
static const uint8_t kHang16Anchored[16] = {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0};
static const uint8_t kHang16Rigid[16] = {0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1};
static void costume_hang16(Cloth *c, int first_bone, int shift, float limit) {
    ClothSet *s = add_set(c, 0.1f, 0.8f);
    int i;

    for (i = 0; i < 16; i++) {
        ClothNode *n = add_node(c, s, CLOTH_COSTUME, first_bone + i, kHang16Anchored[i], kHang16Length[i]);

        n->rigid = kHang16Rigid[i];
        n->cone = kHang16Anchored[i] ? limit : PI / 3.0f;
    }
    costume_spheres(c, s, shift);
}
/* its three blown by the wind (costume_hang3: within 120 degrees; the wind a point 1.5 up and 1.5
 * back of `wind_bone`) */
static void costume_hang3(Cloth *c, int first_bone, int shift, int wind_bone) {
    static const float kLength[3] = {0.7814f, 0.8604f, 0.8604f};
    ClothSet *s = add_set(c, 0.0f, 0.8f);
    int i;

    s->wind_bone = wind_bone;
    s->wind_at = vec3(0.0f, 1.5f, -1.5f);
    for (i = 0; i < 3; i++) {
        add_node(c, s, CLOTH_WIND, first_bone + i, i == 0, kLength[i])->cone = 2.0f * PI / 3.0f;
    }
    costume_spheres(c, s, shift);
}
static void costume2(Cloth *c) {   /* (O_FIH) */
    skirt(c, 6, kSkirtBones, kSkirtSides);
    bone_hanging(c, 0x28);
    bone_sprung(c, 0x19);
    bone_hanging(c, 0x3F);
    costume_hang16(c, 41, 0, PI / 6.0f);
    costume_hang3(c, 37, 0, 24);
}
static void costume3(Cloth *c) {   /* (O_FIO) */
    static const int kBones[4] = {0xB, 0xD, 0xF, 0x11};
    static const float kSides[4][3] = {{0, 1, 0}, {0, 1, 0}, {0, -1, 0}, {0, -1, 0}};

    skirt(c, 4, kBones, kSides);
    bone_hanging(c, 0x24);
    bone_sprung(c, 0x15);
    bone_hanging(c, 0x3B);
    costume_hang16(c, 37, -4, 35.0f * PI / 180.0f);
    costume_hang3(c, 33, -4, 20);
}
static void costume6(Cloth *c) {   /* (O_FIB) a bone hanging, her hair, her chest */
    static const int kHairSpheres[6] = {0xD, 0x1D, 0x14, 0xE, 0x1E, 0x14};
    ClothSet *s;
    int i;

    bone_hanging(c, 0x1C);
    c->hang_set = c->nsets;
    c->hang_bone = 0x14;
    s = add_set(c, 0.1f, 0.8f);
    for (i = 0; i < 4; i++) {
        add_node(c, s, CLOTH_HANG, 0x18 + i, i == 0, 0.45f);
    }
    add_spheres(c, s, kHairSpheres);
    bone_sprung(c, 0xC);
}
static void costume7(Cloth *c) {   /* (O_FIC) five bones hanging, two sprung, one more hanging */
    ClothSet *s = add_set(c, 0.1f, 0.99f);
    int i;

    for (i = 0; i < 5; i++) {
        add_node(c, s, CLOTH_BONEHANG, 0x1A + i, 1, 1.0f);
    }
    bone_sprung(c, 0xC);
    bone_sprung(c, 0xD);
    bone_hanging(c, 0xE);
}

/* ---- Daniella (DaniellaModel_Springs) ---- */
static ClothSphere *add_capsule(Cloth *c, int b1, int b2, Vec3 p1, float r, Vec3 p2) {
    ClothSphere *cp = &c->spheres[c->nspheres++];

    *cp = (ClothSphere){.bone = b1, .off = p1, .r = r, .capsule = 1, .bone2 = b2, .off2 = p2};
    return cp;
}
/* her back's five capsules (bone 2 to bone 6) - standing, flat, bent as she crawls
 * (DaniellaModel_BackCapsules: x1 y1 z1 r x2 y2 z2 each) */
static const float kBack[3][5][7] = {
    {{0, 1, 1, 1.6f, 0, 1, -1}, {1, 1, 1, 1.7f, 1, 1, -1}, {2, 1, 1, 1.8f, 2, 1, -1}, {3, 1, 1, 1.9f, 3, 1, -1}, {4, 1, 1, 2, 4, 1, -1}},
    {{0, 0, 0, 1, 0, 0, 0}, {1, 0, 0, 1, 1, 0, 0}, {2, 0, 0, 1, 2, 0, 0}, {3, 0, 0, 1, 3, 0, 0}, {4, 0, 0, 1, 4, 0, 0}},
    {{0, 1, -1, 2, 0, 1, 1}, {1, 1, -1, 2, 1, 1, 1}, {2, 1, -1, 2, 2, 1, 1}, {3, 1, -1, 2, 3, 1, 1}, {4, 1, -1, 2, 4, 1, 1}},
};
static void back_capsules(Cloth *c, int pose) {
    int k;

    c->back_pose = pose;
    for (k = 0; k < 5; k++) {
        const float *b = kBack[pose == 1 || pose == 2 ? pose : 0][k];
        ClothSphere *cp = &c->spheres[c->back + k];

        cp->off = vec3(b[0], b[1], b[2]);
        cp->r = b[3];
        cp->off2 = vec3(b[4], b[5], b[6]);
    }
}
static void daniella(Cloth *c) {
    static const float kHangPull[3] = {0.2f, 0.1f, 0.0f};
    static const float kHairStiff[5] = {0.8f, 0.6f, 0.4f, 0.2f, 0.0f};
    ClothSet *s;
    int i, first;

    /* what hangs from her waist: two strands of three (bones $22..$27), drawn along bone $1F's Z;
     * off a capsule down her side ($19 to $29), one across her back ($17) and two spheres on $1F */
    s = add_set(c, 0.3f, 0.5f);
    for (i = 0; i < 6; i++) {
        ClothNode *n = add_node(c, s, CLOTH_BENT, 0x22 + i, i % 3 == 0, 0.8f);

        n->pull = 0x1F;
        n->stiff = kHangPull[i % 3];
    }
    add_capsule(c, 0x19, 0x29, vec3(0, 0, 0), 1.0f, vec3(0, 0, 0));
    add_capsule(c, 0x17, 0x17, vec3(1.5f, 0, -0.5f), 1.0f, vec3(-1.5f, 0, -0.5f));
    c->spheres[c->nspheres++] = (ClothSphere){.bone = 0x1F, .r = 1.0f};
    c->spheres[c->nspheres++] = (ClothSphere){.bone = 0x1F, .off = vec3(0, 1, 0), .r = 1.0f};
    s->nspheres = 4;
    /* a part sprung to bone $17 */
    bone_sprung(c, 0x17);
    /* her hair: two strands of five (bones $B..$F, $10..$14), each point kept near the one beside
     * it, stiffer at the root; off her back's capsules */
    c->crawl_set = c->nsets;
    s = add_set(c, 0.8f, 0.5f);
    first = c->nnodes;
    for (i = 0; i < 10; i++) {
        ClothNode *n = add_node(c, s, CLOTH_PAIRED, 0xB + i, i % 5 == 0, 1.2f);

        n->partner = first + (i < 5 ? i + 5 : i - 5);
        n->toward = i < 5 ? -1.0f : 1.0f;
        n->stiff = kHairStiff[i % 5];
    }
    c->back = c->nspheres;
    for (i = 0; i < 5; i++) {
        add_capsule(c, 2, 6, vec3(0, 0, 0), 1.0f, vec3(0, 0, 0));
    }
    back_capsules(c, 0);
    s->nspheres = 5;
    /* two bones hanging ($2F, $30) */
    s = add_set(c, 0.1f, 0.99f);
    add_node(c, s, CLOTH_BONEHANG, 0x2F, 1, 1.0f);
    add_node(c, s, CLOTH_BONEHANG, 0x30, 1, 1.0f);
    c->settle_steps = 50;
}

/* ---- the stalkers' (DebilitasModel_Springs, RiccardoModel_Springs, Kind23Model_Springs,
 * Lorenzo2Model_Points) ---- */
static ClothSphere *add_sphere(Cloth *c, int b, Vec3 off, float r) {
    ClothSphere *sp = &c->spheres[c->nspheres++];

    *sp = (ClothSphere){.bone = b, .off = off, .r = r};
    return sp;
}
/* Debilitas: four points (bones 10..13), the last three hanging from the first; off two spheres
 * on bone 2 */
static void debilitas(Cloth *c) {
    ClothSet *s = add_set(c, 0.4f, 0.99f);
    int first = c->nnodes, i;

    for (i = 0; i < 4; i++) {
        ClothNode *n = add_node(c, s, CLOTH_HANG, 10 + i, i == 0, 1.25f);

        if (i > 0) {
            n->from = first;
        }
    }
    add_sphere(c, 2, vec3(0, 0, 0), 1.8f);
    add_sphere(c, 2, vec3(1, 0, 0), 1.8f);
    s->nspheres = 2;
}
/* Riccardo's look: four points in pairs (bones $16..$19) off four spheres along bone 2; his coat,
 * twelve in pairs (bones $A..$15), each pair after the first two of its six tied to that one, off
 * five capsules down his back */
static void riccardo_look(Cloth *c) {
    ClothSet *s = add_set(c, 0.5f, 0.4f);
    int i, first;

    for (i = 0; i < 4; i++) {
        add_node(c, s, CLOTH_HANG, 0x16 + i, i % 2 == 0, 1.01f);
    }
    for (i = 0; i < 4; i++) {
        add_sphere(c, 2, vec3(-0.8f + 0.8f * (float)i, 0, 0), 1.0f);
    }
    s->nspheres = 4;
    s = add_set(c, 0.1f, 0.8f);
    first = c->nnodes;
    for (i = 0; i < 12; i++) {
        ClothNode *n = add_node(c, s, CLOTH_TIED, 0xA + i, i % 2 == 0, 1.1f);
        int j = i % 6;

        n->rest_bone = 1;
        n->rest_way = i < 6 ? 1.0f : -1.0f;
        if (j >= 2) {
            n->stiff = 0.5f;
            n->partner = first + i - j + j % 2;
        }
    }
    add_capsule(c, 2, 6, vec3(-0.5f, 0, -0.1f), 1.0f, vec3(-0.5f, 0, 0.1f));
    add_capsule(c, 2, 6, vec3(0.0f, 0, -0.1f), 1.0f, vec3(0.0f, 0, 0.1f));
    add_capsule(c, 2, 6, vec3(0.5f, 0, -0.1f), 1.0f, vec3(0.5f, 0, 0.1f));
    add_capsule(c, 2, 6, vec3(1.0f, 0, -0.1f), 1.0f, vec3(1.0f, 0, 0.1f));
    add_capsule(c, 2, 6, vec3(0.0f, -0.3f, -0.1f), 1.0f, vec3(0.0f, -0.3f, 0.1f));
    s->nspheres = 5;
}
/* Riccardo himself: his look, and four more in pairs (bones $26..$29) drawn toward where their
 * bones point */
static void riccardo(Cloth *c) {
    ClothSet *s;
    int i;

    riccardo_look(c);
    s = add_set(c, 0.1f, 0.8f);
    for (i = 0; i < 4; i++) {
        add_node(c, s, CLOTH_REACH, 0x26 + i, i % 2 == 0, 0.67f)->stiff = 0.4f;
    }
}
/* Lorenzo: six points (bones $17..$1C, two of three) off eight spheres */
static void lorenzo(Cloth *c) {
    static const struct { int bone; float y, z; } kSpheres[8] = {
        {0xC, 0, 0}, {0x1D, 0, 0}, {0xD, 0, 0}, {0x1E, 0, 0}, {0x13, 0, 0}, {0x13, 1, 0}, {0x13, 0, 0.4f}, {0x13, 1, 0.4f},
    };
    ClothSet *s = add_set(c, 0.1f, 0.8f);
    int i;

    for (i = 0; i < 6; i++) {
        add_node(c, s, CLOTH_HANG, 0x17 + i, i % 3 == 0, 0.5542f);
    }
    for (i = 0; i < 8; i++) {
        add_sphere(c, kSpheres[i].bone, vec3(0, kSpheres[i].y, kSpheres[i].z), 1.0f);
    }
    s->nspheres = 8;
}

/* Lorenzo's other class (O_LRY: Lorenzo2Model_SwayPoints): 24 swaying points, six groups of four
 * down from their first (bones 6, $16, $A, $1A, $E, $1E on), each pair swinging from bone 2 / 3
 * ($12 / $13 the odd groups), out to their group's side, freer down it (5 .. 20 degrees) */
static void lorenzo_other(Cloth *c) {
    static const int kFirst[6] = {6, 0x16, 0xA, 0x1A, 0xE, 0x1E};
    static const float kSide[6][3] = {{0, 1, 0}, {0, 1, 0}, {0, -1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
    ClothSet *s = add_set(c, 0.2f, 0.9f);
    int g, k;

    for (g = 0; g < 6; g++) {
        for (k = 0; k < 4; k++) {
            ClothNode *n = add_node(c, s, CLOTH_SWAY, kFirst[g] + k, k == 0, 2.125f);

            n->along = (g % 2 ? 0x12 : 2) + (k >= 2);
            n->side = vec3(kSide[g][0], kSide[g][1], kSide[g][2]);
            n->cone = 5.0f * (float)(k + 1) * PI / 180.0f;
        }
    }
}

void cloth_back(Cloth *c, int pose) {
    if (c != NULL && c->crawl_set >= 0) {
        back_capsules(c, pose);
    }
}

void cloth_watch(Cloth *c, int anim, float yaw) {
    ClothSet *hair;

    if (c == NULL || c->crawl_set < 0) {
        return;
    }
    hair = &c->sets[c->crawl_set];
    if ((unsigned)(anim - 0x1800) < 4) {
        if (c->back_pose != 2) {
            back_capsules(c, 2);
            hair->force.x = -0.4f * sinf(yaw);
            hair->force.z = -0.4f * cosf(yaw);
        }
    } else if (c->back_pose == 2) {
        back_capsules(c, 0);
        hair->force.x = 0.0f;
        hair->force.z = 0.0f;
    }
}

Cloth *cloth_for(const char *name) {
    Cloth *c;
    static const struct { const char *prefix; void (*make)(Cloth *); } kModels[] = {
        {"O_FIS/", slip}, {"O_FIN/", clothes}, {"O_FIH/", costume2}, {"O_FIO/", costume3}, {"O_FIB/", costume6},
        {"O_FIC/", costume7}, {"O_DNL/", daniella}, {"O_DB0/", debilitas}, {"O_RCG/", riccardo},
        {"O_RCT/", riccardo_look}, {"O_LRO/", lorenzo}, {"O_LRY/", lorenzo_other},
    };
    size_t k;

    for (k = 0; k < sizeof(kModels) / sizeof(kModels[0]); k++) {
        if (strncmp(name, kModels[k].prefix, strlen(kModels[k].prefix)) == 0) {
            break;
        }
    }
    if (k == sizeof(kModels) / sizeof(kModels[0])) {
        return NULL;
    }
    c = calloc(1, sizeof(*c));
    if (c == NULL) {
        return NULL;
    }
    c->hang_set = -1;
    c->crawl_set = -1;
    c->settle_steps = 30;
    kModels[k].make(c);
    c->reset = 1;
    return c;
}
