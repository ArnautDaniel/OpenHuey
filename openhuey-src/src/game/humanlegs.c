/* A human's legs on the floor: see humanlegs.h. Ported from src/game/model.c
 * HumanModel_BodyFrames, Motion_FootPos, Motion_OntoFloor, HumanModel_PlantFeet, foot_point,
 * Model_SlopeKeep, Model_FootDown, Motion_FootDownPrev, Model_LegIK and the two-bone solver
 * (IK2_Setup, its +0x8 solve, IK2_PlaceMiddle, IK2_AimBones), and src/game/navmesh.c
 * NavTri_ExitEdgeHit, NavMesh_OntoPlane, NavMesh_TriNormal.
 *
 * The original works on the skeleton in the room; the pose hook gives it in model space, so the
 * legs' bones are taken into the room (place) for the solve and back (unplace). Bone matrices
 * are the original's rows: Mat4.m[4k..4k+2] is axis k, m[12..14] the place. */
#include "humanlegs.h"
#include "actor.h"

#include <math.h>
#include <string.h>

/* ---- small vector helpers (3 floats) ---- */
static void v_sub(float *o, const float *a, const float *b) { o[0] = a[0] - b[0]; o[1] = a[1] - b[1]; o[2] = a[2] - b[2]; }
static void v_add(float *o, const float *a, const float *b) { o[0] = a[0] + b[0]; o[1] = a[1] + b[1]; o[2] = a[2] + b[2]; }
static float v_dot(const float *a, const float *b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
static void v_cross(float *o, const float *a, const float *b) {
    float x = a[1] * b[2] - a[2] * b[1], y = a[2] * b[0] - a[0] * b[2], z = a[0] * b[1] - a[1] * b[0];

    o[0] = x; o[1] = y; o[2] = z;
}
static void v_norm(float *o, const float *a) {   /* (sceVu0Normalize; a zero vector stays) */
    float l = sqrtf(v_dot(a, a));

    if (l > 0.0f) {
        o[0] = a[0] / l; o[1] = a[1] / l; o[2] = a[2] / l;
    } else if (o != a) {
        o[0] = a[0]; o[1] = a[1]; o[2] = a[2];
    }
}
static void v_copy(float *o, const float *a) { o[0] = a[0]; o[1] = a[1]; o[2] = a[2]; }
static void v_from(float *o, Vec3 v) { o[0] = v.x; o[1] = v.y; o[2] = v.z; }
static float *row(Mat4 *m, int k) { return &m->m[k * 4]; }

/* ---- the floor ---- */

/* NavMesh_OntoPlane: p's height on triangle t's plane, from its x and z */
static void onto_plane(const NavTri *t, float *p) {
    float e2x = t->v[2].x - t->v[0].x, e2y = t->v[2].y - t->v[0].y, e1y = t->v[1].y - t->v[0].y;
    float e1x = t->v[1].x - t->v[0].x, e1z = t->v[1].z - t->v[0].z, e2z = t->v[2].z - t->v[0].z;
    float nx = e1y * e2z - e2y * e1z, nz = e1x * e2y - e2x * e1y, ny = e1z * e2x - e2z * e1x;

    p[1] = t->v[0].y - ((p[2] - t->v[0].z) * nz + (p[0] - t->v[0].x) * nx) / ny;
}

/* NavTri_ExitEdgeHit: the edge of t the step from -> to leaves it by (0..2), where it crosses
 * (`hit`, its height on t's plane); 3 `to` inside, 4 outside crossing no edge */
static int exit_edge_hit(const NavTri *t, float *hit, const float *from, const float *to) {
    float x2 = to[0], z2 = to[2];
    unsigned inside = 0, tested = 0;
    int e;

    for (e = 0; e < 3; e++) {
        Vec3 a = t->v[e], b = t->v[e < 2 ? e + 1 : 0];
        float ax = a.x, az = a.z, ez = b.z - az, ex = b.x - ax;
        float x1, z1, p, q, dx, dz, den, s, u;

        tested |= 1u << e;
        if (!((x2 - ax) * ez - (z2 - az) * ex < 0.0f)) {
            inside |= 1u << e;
            continue;
        }
        x1 = from[0];
        z1 = from[2];
        q = (z1 - az) * ex;
        p = (x1 - ax) * ez;
        if (p - q < 0.0f) {
            continue;
        }
        dx = x2 - x1;
        dz = z2 - z1;
        den = dx * ez - dz * ex;
        if (den == 0.0f) {
            continue;
        }
        s = (q - p) / den;
        if (s < 0.0f || !(s <= 1.0f)) {
            continue;
        }
        u = fabsf(ex) <= fabsf(ez) ? (z1 + s * dz - az) / ez : (x1 + s * dx - ax) / ex;
        if (u < 0.0f || !(u <= 1.0f)) {
            continue;
        }
        hit[0] = from[0] + (to[0] - from[0]) * s;
        hit[1] = from[1] + (to[1] - from[1]) * s;
        hit[2] = from[2] + (to[2] - from[2]) * s;
        onto_plane(t, hit);
        return e;
    }
    return inside == tested ? 3 : 4;
}

/* NavMesh_TriNormal: its unit normal as wound ((0, 1, 0) for no such triangle) */
static void tri_normal(const NavMesh *nav, int i, float *n) {
    const NavTri *t;
    float a[3], b[3];

    if (nav == NULL || i < 0 || i >= nav->ntris) {
        n[0] = 0.0f; n[1] = 1.0f; n[2] = 0.0f;
        return;
    }
    t = &nav->tris[i];
    a[0] = t->v[1].x - t->v[0].x; a[1] = t->v[1].y - t->v[0].y; a[2] = t->v[1].z - t->v[0].z;
    b[0] = t->v[2].x - t->v[0].x; b[1] = t->v[2].y - t->v[0].y; b[2] = t->v[2].z - t->v[0].z;
    v_cross(n, a, b);
    v_norm(n, n);
}

/* Motion_OntoFloor: p put on the floor, walking the mesh from the character's triangle (at
 * `from`) toward it: on a triangle, its height there; at a wall (no triangle beyond, or one
 * marked $80 whose flags the character's mask blocks) its height carried on along the line
 * from the character to where the wall was met */
void humanlegs_onto_floor(const NavMesh *nav, int start, uint32_t mask, Vec3 at, float *p) {
    float from[3], hit[3] = {0.0f, 0.0f, 0.0f}, d[3], len;
    int tri = start, n;

    if (nav == NULL || tri < 0 || tri >= nav->ntris) {
        return;
    }
    v_from(from, at);
    for (n = 0; n < 4 * nav->ntris + 16; n++) {   /* (the original walks without a bound) */
        const NavTri *t = &nav->tris[tri];
        int e = exit_edge_hit(t, hit, from, p);

        if (e == 3) {
            onto_plane(t, p);
            return;
        }
        if (e == 4) {
            onto_plane(&nav->tris[start], p);
            return;
        }
        tri = t->next[e];
        if (tri < 0 || tri >= nav->ntris) {
            break;
        }
        if (tri < nav->nmarks && (nav->marks[tri] & 0x80) && (nav->tris[tri].flags & mask)) {
            break;
        }
    }
    v_sub(d, p, from);
    len = sqrtf(v_dot(d, d));
    v_sub(d, hit, from);
    v_norm(d, d);
    p[1] = from[1] + d[1] * len;
}

static void onto_floor(const HumanLegs *h, float *p) {
    humanlegs_onto_floor(h->nav, h->tri, h->mask, h->at, p);
}

float humanlegs_floor_at(const NavMesh *nav, int tri, Vec3 at) {   /* (the nav mesh's +0x14) */
    float p[3];

    v_from(p, at);
    if (nav != NULL && tri >= 0 && tri < nav->ntris) {
        onto_plane(&nav->tris[tri], p);
    }
    return p[1];
}

/* ---- the motion's feet ---- */

static float fade_weight(const Actor *a) {   /* (+0x550: the motion fading out's share) */
    float x = a->fade_len > 0.0f && a->fade > 0.0f ? a->fade / a->fade_len : 0.0f;

    return x * x * (3.0f - 2.0f * x);
}

static float wrapped(float t, float len) {
    if (len <= 0.0f) {
        return t;
    }
    while (t < 0.0f) {
        t += len;
    }
    while (!(t < len)) {
        t -= len;
    }
    return t;
}

/* Track_Blend of a foot's place track: the motion's and its variant's, by the variant's share
 * (1 - w); either alone if the other hasn't it */
static void foot_track(const Actor *a, int motion, float frame, int var, float vframe, float w, int code, float *out) {
    float p[3] = {0.0f, 0.0f, 0.0f}, q[3] = {0.0f, 0.0f, 0.0f};
    int has_p = motion >= 0 && model_track(&a->model, motion, code, frame, p);
    int has_q = var >= 0 && model_track(&a->model, var, code, vframe, q);
    int i;

    for (i = 0; i < 3; i++) {
        out[i] = has_p && has_q ? q[i] * (1.0f - w) + p[i] * w : has_p ? p[i] : q[i];
    }
}

/* Motion_FootPos (dt 0): a foot's place in model space - the motion's, crossed with the one
 * fading out - and whether the motion's contact track has it down; 0 (at the origin) without
 * one */
static int foot_pos(const Actor *a, int left, float *out) {
    float c[3], q[3];
    int code = left ? -3 : -2, i;

    out[0] = out[1] = out[2] = 0.0f;
    if (a->motion < 0 || !model_track_raw(&a->model, a->motion, -4, a->frame, c)) {
        return 0;
    }
    foot_track(a, a->motion, a->frame, a->variant, a->vframe, a->vweight, code, out);
    if (a->prev_motion >= 0) {
        float w = fade_weight(a);
        float at = a->prev_held ? (float)(model_motion_frames(&a->model, a->prev_motion) - 1) : a->prev_frame;

        foot_track(a, a->prev_motion, at, a->prev_variant, a->prev_vframe, a->prev_vweight, code, q);
        for (i = 0; i < 3; i++) {
            out[i] = q[i] * w + out[i] * (1.0f - w);
        }
    }
    return c[left] > 0.0f;
}

/* Model_FootDown: foot down `ofs` frames from now in the motion (its time wrapped into it) */
static int foot_down(const Actor *a, int foot, float ofs) {
    float c[3] = {0.0f, 0.0f, 0.0f};

    if (a->motion < 0) {
        return 0;
    }
    model_track_raw(&a->model, a->motion, -4, wrapped(a->frame + ofs, (float)model_motion_frames(&a->model, a->motion)), c);
    return !(c[foot] <= 0.0f);
}

/* Motion_FootDownPrev: the same in the motion before */
static int foot_down_prev(const Actor *a, int foot, float ofs) {
    float c[3] = {0.0f, 0.0f, 0.0f};

    if (a->prev_motion < 0) {
        return 0;
    }
    model_track_raw(&a->model, a->prev_motion, -4,
                    wrapped(a->prev_frame + ofs, (float)model_motion_frames(&a->model, a->prev_motion)), c);
    return !(c[foot] <= 0.0f);
}

/* the actor's place: model -> room, its origin at height y */
static Mat4 place_at(const Actor *a, float y) {
    Mat4 p = mat4_identity();
    float c = cosf(a->yaw), s = sinf(a->yaw);

    p.m[0] = c * a->scale;  p.m[2] = -s * a->scale;
    p.m[5] = a->scale;
    p.m[8] = s * a->scale;  p.m[10] = c * a->scale;
    p.m[12] = a->pos.x;
    p.m[13] = y;
    p.m[14] = a->pos.z;
    return p;
}

static void to_room(const Mat4 *place, const float *in, float *out) {
    Vec3 r = mat4_point(place, vec3(in[0], in[1], in[2]));

    v_from(out, r);
}

void humanlegs_set(HumanLegs *h, int right, int left) {
    int k;

    memset(h, 0, sizeof(*h));
    h->on = right >= 0 && left >= 0;
    for (k = 0; k < 4; k++) {
        h->bones[0][k] = right + k;
        h->bones[1][k] = left + k;
    }
    h->tri = -1;
    h->jump = 1;   /* (HumanModel_Loaded: FeetUp) */
    h->keep = 1.0f;
}

/* HumanModel_BodyFrames (and Model_BodyFrames for tri -1) */
void humanlegs_fit(Actor *a, const NavMesh *nav, int tri, uint32_t mask, int plant, float back, float front) {
    HumanLegs *h = &a->hlegs;
    Mat4 place = place_at(a, a->pos.y);
    float l[3], r[3];

    if (!h->on || a->drive != NULL) {   /* (a scene's: Fiona_FollowAnim / Pursuer_PlaceModel - no +0x40 or +0x4C) */
        return;
    }
    h->asked = 1;
    h->nav = nav;
    h->tri = tri;
    h->mask = mask;
    h->at = a->pos;
    h->height = a->pos.y;
    if (tri < 0) {   /* (off the mesh: the model at its place, the height before kept) */
        h->plant = -1;
        return;
    }
    h->plant = plant;
    if (back == 0.0f && front == 0.0f) {
        h->last = h->height;
        return;
    }
    if (a->mflags & 4) {   /* (a slope motion: the floor back and ahead along its step) */
        float n[3], turn, k, pl[3] = {0.0f, 0.0f, 0.0f}, pr[3] = {0.0f, 0.0f, 0.0f};
        Vec3 step;

        tri_normal(nav, tri, n);
        actor_root_delta(a, &turn, &step);
        k = step.z * (n[1] * n[1]);
        pl[2] = back * k;
        pr[2] = front * k;
        to_room(&place, pl, l);
        to_room(&place, pr, r);
        onto_floor(h, l);
        onto_floor(h, r);
    } else {
        float fl[3], fr[3];
        int dl = foot_pos(a, 1, fl), dr = foot_pos(a, 0, fr), use_l = 1, use_r = 1;

        to_room(&place, fl, l);
        to_room(&place, fr, r);
        if (a->mflags & 0x100) {   /* (only the feet down) */
            if (dl == 1 && a->prev_motion >= 0) {
                dl = foot_down_prev(a, 1, 0.0f);
            }
            use_l = dl != 0;
            if (dr == 1 && a->prev_motion >= 0) {
                dr = foot_down_prev(a, 0, 0.0f);
            }
            use_r = dr != 0;
        }
        if (use_l) {
            onto_floor(h, l);
        } else {
            l[1] = a->pos.y;
        }
        if (use_r) {
            onto_floor(h, r);
        } else {
            r[1] = a->pos.y;
        }
    }
    h->height = l[1] < r[1] ? l[1] : r[1];
    if (!h->jump) {
        h->height = 0.25f * h->height + 0.75f * h->last;
    } else {
        h->jump = 0;
    }
    h->last = h->height;
}

/* Model_SlopeKeep: on a slope motion, 1 - (1 - ny^2) x |its facing along the floor's fall| */
static float slope_keep(const Actor *a, const HumanLegs *h) {
    float n[3], v[3], k = 1.0f, d;

    if (!(a->mflags & 4)) {
        return 1.0f;
    }
    tri_normal(h->nav, h->tri, n);
    if (n[1] < 1.0f) {
        k = n[1] * n[1];
        n[1] = 0.0f;
        v_norm(n, n);
        v[0] = sinf(a->yaw); v[1] = 0.0f; v[2] = cosf(a->yaw);
        d = fabsf(v_dot(v, n));
        k = 1.0f - (1.0f - k) * d;
    }
    return k;
}

/* HumanModel_PlantFeet before the pose: the slope's share eased, which feet go onto the floor */
void humanlegs_tick(Actor *a) {
    HumanLegs *h = &a->hlegs;
    int dl, dr;

    h->fitted = h->asked;
    if (!h->asked) {
        h->plant = -1;
        return;
    }
    h->asked = 0;
    if (h->plant == 0) {
        h->keep = 1.0f;
    }
    if (h->plant != 1) {
        return;
    }
    h->keep = 0.75f * h->keep + 0.25f * slope_keep(a, h);
    dl = foot_down(a, 1, -1.0f);
    dr = foot_down(a, 0, -1.0f);
    h->use[0] = h->use[1] = 1;
    if (!(a->mflags & 4) && (a->mflags & 0x100)) {
        if (dl == 1 && a->prev_motion >= 0) {
            dl = foot_down_prev(a, 1, 0.0f);
        }
        h->use[1] = dl != 0;
        if (dr == 1 && a->prev_motion >= 0) {
            dr = foot_down_prev(a, 0, 0.0f);
        }
        h->use[0] = dr != 0;
    }
}

/* ---- the two-bone solver ---- */

/* IK2_PlaceMiddle: the middle joint of a chain (lengths l1, l2) from root toward end, bent to
 * the side of pole x its direction (by `bend`); 1 when end is out of reach */
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
    ll = l1 * l1;
    side[0] *= bend; side[1] *= bend; side[2] *= bend;
    a = (ll - l2 * l2 + dist * dist) / (2.0f * dist);
    h = sqrtf(fabsf(ll - a * a));
    mid[0] = root[0] + a * d[0] + h * side[0];
    mid[1] = root[1] + a * d[1] + h * side[1];
    mid[2] = root[2] + a * d[2] + h * side[2];
    return out;
}

/* ik_aim: a bone aimed from -> to - X along it, Z from the pole made square to it, unit - at from */
static void aim(Mat4 *m, const float *from, const float *to, const float *pole) {
    float *x = row(m, 0), *y = row(m, 1), *z = row(m, 2);

    *m = mat4_identity();
    v_sub(x, to, from);
    v_copy(z, pole);
    v_cross(y, z, x);
    v_cross(z, x, y);
    v_norm(x, x); v_norm(y, y); v_norm(z, z);
    m->m[12] = from[0]; m->m[13] = from[1]; m->m[14] = from[2];
}

/* one leg (Model_LegIK's half): bones in the room; `floor` the foot's place on the floor */
static void leg(Mat4 *hip, Mat4 *knee, Mat4 *ankle, Mat4 *foot, float l1, float l2, const float *floor, float height) {
    float root[3], mid[3], end[3], pole[3], to_foot[3], was[3], d[3];

    v_copy(root, &hip->m[12]);
    v_copy(mid, &knee->m[12]);
    v_sub(to_foot, &foot->m[12], &ankle->m[12]);
    v_copy(was, &ankle->m[12]);
    end[0] = floor[0] - to_foot[0];   /* (leg_target: the ankle over the floor as it was over the model) */
    end[1] = ankle->m[13] + floor[1] - height;
    end[2] = floor[2] - to_foot[2];
    v_copy(pole, row(hip, 2));   /* (IK2's +0x8 solve: the hip's Z the bend's side) */
    if (place_middle(root, mid, end, pole, l1, l2, -1.0f) != 0) {
        v_sub(d, end, root);
        v_norm(d, d);
        end[0] = root[0] + d[0] * (l1 + l2);
        end[1] = root[1] + d[1] * (l1 + l2);
        end[2] = root[2] + d[2] * (l1 + l2);
    }
    aim(hip, root, mid, pole);
    aim(knee, mid, end, pole);
    v_sub(d, end, was);   /* (the ankle and the foot moved as far as the solved ankle went) */
    v_add(&ankle->m[12], &ankle->m[12], d);
    v_add(&foot->m[12], &foot->m[12], d);
}

void humanlegs_pose(HumanLegs *h, const Model *m, Mat4 *world, const Mat4 *place, const Mat4 *unplace) {
    float x[3], z[3], o[3], floor[2][3];
    int i, k;

    if (!h->on || !h->fitted || h->plant != 1) {
        return;
    }
    for (i = 0; i < 2; i++) {
        for (k = 0; k < 4; k++) {
            if (h->bones[i][k] < 0 || h->bones[i][k] >= m->nbones) {
                return;
            }
        }
    }
    v_copy(x, &place->m[0]);   /* (the model's matrix: +0x7D0 its X, +0x7F0 its Z, +0x800 its place) */
    v_copy(z, &place->m[8]);
    v_copy(o, &place->m[12]);
    /* foot_point, then onto the floor (else at the character's height): left +0x8AC, right +0x89C */
    for (i = 0; i < 2; i++) {
        float *p = floor[i];

        to_room(place, &world[h->bones[i][3]].m[12], p);
        if (h->keep < 1.0f) {   /* (its height above the model eased by the slope) */
            float a = v_dot(x, o), b = v_dot(z, o), c = v_dot(x, p), d = v_dot(z, p);

            for (k = 0; k < 3; k++) {
                p[k] = o[k] + z[k] * (h->keep * (d - b)) + x[k] * (c - a);
            }
        }
        if (h->use[i]) {
            onto_floor(h, p);
        } else {
            p[1] = h->at.y;
        }
    }
    /* Model_LegIK: in the room, each leg's hip -> knee -> ankle onto its floor point */
    for (i = 0; i < 2; i++) {
        Mat4 b[4];

        for (k = 0; k < 4; k++) {
            b[k] = mat4_mul(*place, world[h->bones[i][k]]);
        }
        leg(&b[0], &b[1], &b[2], &b[3], m->bones[h->bones[i][1]].rest_pos[0], m->bones[h->bones[i][2]].rest_pos[0],
            floor[i], h->height);
        for (k = 0; k < 4; k++) {
            world[h->bones[i][k]] = mat4_mul(*unplace, b[k]);
        }
    }
}

/* ---- the head: HumanModel_AdjustBone ---- */

/* sceVu0RotMatrixX / Y's rotations (Mat4 has the sce matrices' layout) */
static Mat4 rot_x(float t) {
    Mat4 r = mat4_identity();

    r.m[5] = cosf(t);   r.m[6] = sinf(t);
    r.m[9] = -sinf(t);  r.m[10] = cosf(t);
    return r;
}

static Mat4 rot_y(float t) {
    Mat4 r = mat4_identity();

    r.m[0] = cosf(t);  r.m[2] = -sinf(t);
    r.m[8] = sinf(t);  r.m[10] = cosf(t);
    return r;
}

void humanhead_adjust(int i, Mat4 *world, void *user) {
    const HumanHead *h = user;

    if (i == h->head) {   /* (b = b * RotY(yaw / 2) * RotX(-pitch): in its own frame) */
        world[i] = mat4_mul(world[i], mat4_mul(rot_y(0.5f * h->yaw), rot_x(-h->pitch)));
    }
    if (i == h->neck) {   /* (b = RotY(yaw / 2) about its place * b less its place) */
        Mat4 r = rot_y(0.5f * h->yaw), b = world[i];

        r.m[12] = b.m[12];  r.m[13] = b.m[13];  r.m[14] = b.m[14];
        b.m[12] = b.m[13] = b.m[14] = 0.0f;
        world[i] = mat4_mul(r, b);
    }
}
