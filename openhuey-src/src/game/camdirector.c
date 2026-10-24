/* The camera director: see camdirector.h. Ported from src/game/camera.c (CamDirector_*,
 * CamPath_*, Camera_Preset / _TurnView) and src/game/vecmath.c (Spline_*), keeping their
 * arithmetic. */
#include "camdirector.h"

#include "engine.h"
#include "progress.h"
#include "../render/post.h"

#include <math.h>
#include <string.h>

CamDirector gCamDir;

#define PI_F 3.14159265358979f

static float wrap(float a) {   /* Angle_Wrap */
    while (!(a <= PI_F)) {
        a -= 2.0f * PI_F;
    }
    while (a < -PI_F) {
        a += 2.0f * PI_F;
    }
    return a;
}

/* ---- the spline (Spline_*) ---- */

static void spline_init(Spline *s, int dims, int n, const float *keys) {
    memset(s, 0, sizeof(*s));
    s->n = n;
    s->keys = keys;
    s->tmin = (int)keys[0];
    s->tmax = (int)keys[n * 8 - 8];
    s->t = (float)s->tmin;
    s->dims = dims;
}

static void spline_seek_to(const Spline *s, float *t, int *seg, float u) {
    const float *p;
    int k;

    if (s->keys == NULL || u < (float)s->tmin || !(u <= (float)s->tmax)) {
        return;
    }
    *t = u;
    k = *seg;
    p = s->keys + k * 8;
    for (;;) {
        float p0 = p[0];

        if (k + 1 < s->n) {
            if (!(u < p0) && u < p[8]) {
                return;
            }
        } else if (!(u < p0)) {   /* (the last key: at or past it - the original wants it exact,
                                   * and else walks off the keys) */
            return;
        }
        if (u < p0 && k == 0) {   /* (before the first: stay on it) */
            return;
        }
        if (u < p0) {
            (*seg)--;
            p -= 8;
        }
        if (!(u < p[8])) {
            (*seg)++;
            p += 8;
        }
        k = *seg;
    }
}

static void spline_seek(Spline *s, float u) {
    spline_seek_to(s, &s->t, &s->seg, u);
}

/* component `comp` at time u (the current time if u is outside the keys) */
static float spline_eval(const Spline *s, int comp, float u) {
    float t = s->t, x, y, a, a3, b2, b1;
    int seg = s->seg;
    const float *p;

    if (s->keys == NULL) {
        return 0.0f;
    }
    spline_seek_to(s, &t, &seg, u);
    if (comp < 0 || comp >= s->dims) {
        return 0.0f;
    }
    p = s->keys + (seg + comp * s->n) * 8;
    if (t == p[0]) {
        return p[1];
    }
    if (t <= p[0]) {
        return 0.0f;
    }
    x = (t - p[0]) / (p[8] - p[0]);
    y = 1.0f - x;
    a = 3.0f * y;
    a3 = x * (x * x);
    b2 = x * (a * x);
    b1 = x * (a * y);
    return ((p[1] + p[5]) * b1 + p[1] * (y * (y * y))) + (p[9] + p[11]) * b2 + p[9] * a3;
}

/* ---- the camera the director drives (the original's gCamera) ---- */

static const float *sSets;   /* the room's camera sets (section 5), 8 floats each */
static int sNsets;
static const Vec3 kUp = {0.0f, -1.0f, 0.0f};   /* the game's world has y down (Camera +0x80) */

/* Camera_Preset + Camera_ApplyPreset: set n's eye, point and view angle (n out of range: a
 * default view) */
static void take_set(CamDirector *d, int n) {
    if (n < 0 || n >= sNsets || sSets == NULL) {
        d->eye = vec3(0.0f, 30.0f, 150.0f);
        d->look = vec3(0.0f, 0.0f, 0.0f);
        d->set_fov = PI_F / 3.0f;
    } else {
        float f[8];

        memcpy(f, sSets + n * 8, sizeof(f));
        d->shown = n;
        d->eye = vec3(f[0], f[1], f[2]);
        d->look = vec3(f[4], f[5], f[6]);
        d->set_fov = f[3] == 0.0f ? PI_F / 3.0f : 2.0f * PI_F * f[3] / 360.0f;
    }
}

/* Camera_TurnView: the point looked at goes round the eye by yaw about y, then by pitch about
 * the axis square to the up and the view (the original's matrices: rows apply as below) */
static void turn_view(CamDirector *d, float pitch, float yaw) {
    Vec3 v;

    if (yaw != 0.0f) {
        float c = cosf(wrap(yaw)), s = sinf(wrap(yaw));

        v = vec3_sub(d->look, d->eye);
        d->look = vec3(c * v.x + s * v.z + d->eye.x, v.y + d->eye.y, -s * v.x + c * v.z + d->eye.z);
    }
    if (pitch != 0.0f) {
        Vec3 ax, w;
        float n, h = 0.5f * pitch, sn = sinf(h), qx, qy, qz, qw, m[3][3];
        int k;

        v = vec3_sub(d->look, d->eye);
        ax = vec3(kUp.y * v.z - kUp.z * v.y, kUp.z * v.x - kUp.x * v.z, kUp.x * v.y - kUp.y * v.x);
        n = sqrtf(ax.x * ax.x + ax.y * ax.y + ax.z * ax.z);
        if (n != 0.0f) {
            ax = vec3(ax.x / n, ax.y / n, ax.z / n);
        }
        qx = ax.x * sn;
        qy = ax.y * sn;
        qz = ax.z * sn;
        qw = cosf(h);
        m[0][0] = 1.0f + -2.0f * (qy * qy + qz * qz);
        m[0][1] = 2.0f * (qx * qy - qz * qw);
        m[0][2] = 2.0f * (qx * qz + qy * qw);
        m[1][0] = 2.0f * (qx * qy + qz * qw);
        m[1][1] = 1.0f + -2.0f * (qx * qx + qz * qz);
        m[1][2] = 2.0f * (qy * qz - qx * qw);
        m[2][0] = 2.0f * (qx * qz - qy * qw);
        m[2][1] = 2.0f * (qy * qz + qx * qw);
        m[2][2] = 1.0f + -2.0f * (qx * qx + qy * qy);
        for (k = 0; k < 3; k++) {   /* (sceVu0ApplyMatrix: out[k] = sum m[i][k] v[i]) */
            (&w.x)[k] = m[0][k] * v.x + m[1][k] * v.y + m[2][k] * v.z;
        }
        d->look = vec3_add(w, d->eye);
    }
}

int camera_out_of_view(const Camera *c, Vec3 p) {
    Mat4 v = camera_view(c);
    float x = v.m[0] * p.x + v.m[4] * p.y + v.m[8] * p.z + v.m[12];
    float y = v.m[1] * p.x + v.m[5] * p.y + v.m[9] * p.z + v.m[13];
    float z = -(v.m[2] * p.x + v.m[6] * p.y + v.m[10] * p.z + v.m[14]);
    float tv = tanf(c->fov * 0.5f), th = tv * 4.0f / 3.0f;

    return z < 1.0f || z > 2000.0f || fabsf(x) > z * th || fabsf(y) > z * tv;
}

/* ---- the path (CamPath_*) ---- */

/* where the one followed is now (its body, as the original reads its position through a pointer) */
static int target_at(const CamDirector *d, Vec3 *out) {
    const HBody *b = d->target < 0 ? NULL : hactor_body(d->target);

    if (b == NULL || !b->has) {
        return 0;
    }
    *out = vec3(b->pos[0], b->pos[1], b->pos[2]);
    return 1;
}

static int target_point(const CamDirector *d, Vec3 *out) {
    if (!target_at(d, out)) {
        return 0;
    }
    *out = vec3_add(*out, d->offset);
    return 1;
}

/* the look-at track's time nearest the target: by horizontal distance (mode 0), height (1) or
 * distance (2); -1 with no path or target */
static float path_nearest(const CamDirector *d, int mode, float t) {
    const Spline *s = &d->path;
    float best_hd = -1.0f, best_dy = -1.0f, best_d = -1.0f, best = -1.0f, u, end;
    int n, k, kb = 0, lo, hi;
    Vec3 p;

    u = !(t < 1.0f) ? t : s->t;
    if (d->data == NULL || s->keys == NULL || !target_point(d, &p)) {
        return -1.0f;
    }
    n = s->n;
    for (k = 0; k < n; k++) {
        float tk = (float)(int32_t)s->keys[k * 8];
        float dx = spline_eval(s, 3, tk) - p.x, dy = spline_eval(s, 4, tk) - p.y;
        float dz = spline_eval(s, 5, tk) - p.z, hd = sqrtf(dz * dz + dx * dx), dd;
        int take;

        if (dy <= 0.0f) {
            dy = -dy;
        }
        dd = sqrtf(dy * dy + hd * hd);
        take = mode == 0 ? best_hd < 0.0f || !(best_hd <= hd)
             : mode == 1 ? best_dy < 0.0f || !(best_dy <= dy) : best_d < 0.0f || !(best_d <= dd);
        if (take) {
            best_hd = hd;
            kb = k;
            best_dy = dy;
            best_d = dd;
            best = tk;
        }
    }
    lo = kb >= 2 ? kb - 1 : 0;
    hi = kb < n - 1 ? kb + 1 : n - 1;
    end = (float)(int32_t)s->keys[kb * 8] + (float)((int32_t)s->keys[hi * 8] - (int32_t)s->keys[kb * 8]) / 2.0f;
    u = (float)(int32_t)s->keys[lo * 8] + (float)((int32_t)s->keys[kb * 8] - (int32_t)s->keys[lo * 8]) / 2.0f;
    while (u < end) {
        float dx = spline_eval(s, 3, u) - p.x, dy = spline_eval(s, 4, u) - p.y;
        float dz = spline_eval(s, 5, u) - p.z, hd = sqrtf(dz * dz + dx * dx), dd;
        int take;

        if (dy <= 0.0f) {
            dy = -dy;
        }
        dd = sqrtf(dy * dy + hd * hd);
        take = mode == 0 ? best_hd < 0.0f || !(best_hd <= hd)
             : mode == 1 ? best_dy < 0.0f || !(best_dy <= dy) : best_d < 0.0f || !(best_d <= dd);
        if (take) {
            best_hd = hd;
            best_dy = dy;
            best_d = dd;
            best = u;
        }
        u = u + d->step_look;
    }
    return best;
}

/* how far along the look-at track to go toward time `to` this frame (from `from`; below 1: the
 * current time): move_rate units at most, within the keys */
static float path_move(const CamDirector *d, float to, float from) {
    const Spline *s = &d->path;
    float pts[2][3], dir, t, end, len = 0.0f, res;
    int cur = 1;

    if (from < 1.0f) {
        from = s->t;
    }
    if (s->keys == NULL || !(d->step_look > 0.0f)) {   /* (no path loaded: nowhere to move - and no endless loop) */
        return from;
    }
    res = from;
    if (!(from <= to)) {
        dir = -1.0f;
        end = from;
        t = to;
    } else {
        dir = 1.0f;
        t = from;
        end = to;
    }
    pts[0][0] = spline_eval(s, 3, t);
    pts[0][1] = spline_eval(s, 4, t);
    pts[0][2] = spline_eval(s, 5, t);
    t += d->step_look;
    while (t < end) {
        float *p = pts[cur & 1], dx, dy, dz;

        p[0] = spline_eval(s, 3, t);
        p[1] = spline_eval(s, 4, t);
        p[2] = spline_eval(s, 5, t);
        cur ^= 1;
        dy = pts[0][1] - pts[1][1];
        dx = pts[0][0] - pts[1][0];
        dz = pts[0][2] - pts[1][2];
        t += d->step_look;
        len += sqrtf(dy * dy + dx * dx + dz * dz);
    }
    if (dir != 0.0f) {
        res = res + d->move_rate * (dir * (len / d->len_look));
        if (dir <= 0.0f) {
            if (res < to) {
                res = to;
            }
        } else if (!(res <= to)) {
            res = to;
        }
    }
    if (res <= (float)s->tmin) {
        res = (float)s->tmin;
    }
    if (!(res < (float)s->tmax)) {
        res = (float)s->tmax;
    }
    return res;
}

static Vec3 path_eye(const CamDirector *d) {
    return vec3(spline_eval(&d->path, 0, 0.0f), spline_eval(&d->path, 1, 0.0f), spline_eval(&d->path, 2, 0.0f));
}

static Vec3 path_look(const CamDirector *d) {
    return vec3(spline_eval(&d->path, 3, 0.0f), spline_eval(&d->path, 4, 0.0f), spline_eval(&d->path, 5, 0.0f));
}

/* path `setup` of the room's data: its spline, its tracks' lengths and step sizes, at time t */
static void load_setup(CamDirector *d, int setup, float t) {
    const int32_t *data = d->data;
    const float *keys;
    float a[2][3], b[2][3], u, end;
    int n, i, cur;

    if (data == NULL || setup < 0 || setup >= d->npaths) {
        return;
    }
    d->loaded = setup;
    n = d->npaths;
    keys = (const float *)((const uint8_t *)data + n * 8 + ((n & 1) == 0 ? 8 : 0) + 8);
    for (i = 0; i < setup; i++) {
        keys = (const float *)((const uint8_t *)keys + data[2 + i * 2] * data[3 + i * 2] * 32);
    }
    spline_init(&d->path, data[2 + setup * 2], data[3 + setup * 2], keys);
    d->len_eye = 0.0f;
    d->len_look = 0.0f;
    end = (float)d->path.tmax;
    u = keys[0];
    for (i = 0; i < 3; i++) {
        b[0][i] = spline_eval(&d->path, 3 + i, u);
        a[0][i] = spline_eval(&d->path, i, u);
    }
    u = u + 1.0f;
    cur = 0;
    while (u < end) {
        float dx, dy, dz, ex, ey, ez;

        cur ^= 1;
        for (i = 0; i < 3; i++) {
            b[cur][i] = spline_eval(&d->path, 3 + i, u);
            a[cur][i] = spline_eval(&d->path, i, u);
        }
        u = u + 1.0f;
        dy = b[0][1] - b[1][1];
        dx = b[0][0] - b[1][0];
        dz = b[0][2] - b[1][2];
        ey = a[0][1] - a[1][1];
        ex = a[0][0] - a[1][0];
        ez = a[0][2] - a[1][2];
        d->len_look = d->len_look + sqrtf(dy * dy + dx * dx + dz * dz);
        d->len_eye = d->len_eye + sqrtf(ey * ey + ex * ex + ez * ez);
    }
    d->step_look = 0.195f / (d->len_look / 100.0f);
    d->step_eye = 0.195f / (d->len_eye / 100.0f);
    spline_seek(&d->path, t);
}

/* ---- the director ---- */

/* keep the target in view: beyond 5 degrees up / down or 10 across, turn `rate` percent of the
 * rest of the way (CamDirector_KeepInView) */
static void keep_in_view(CamDirector *d, int rate) {
    const float k5 = 0.0872665f, k10 = 0.1745329f;
    Vec3 t, v;
    float ht, hd, a, b;

    if (!target_point(d, &t)) {
        t = d->point;
    }
    v = vec3_sub(d->look, d->eye);
    t = vec3_sub(t, d->eye);
    ht = sqrtf(fabsf(t.z * t.z + t.x * t.x));
    hd = sqrtf(fabsf(v.z * v.z + v.x * v.x));
    a = atan2f(t.y, ht);
    b = -wrap(a - atan2f(v.y, hd));
    if (!(fabsf(b) <= k5)) {
        b = !(b <= 0.0f) ? b - k5 : b + k5;
        turn_view(d, b * (float)rate / 100.0f, 0.0f);
    }
    a = atan2f(t.x, t.z);
    b = wrap(a - atan2f(v.x, v.z));
    if (fabsf(b) <= k10) {
        return;
    }
    b = !(b <= 0.0f) ? b - k10 : b + k10;
    turn_view(d, 0.0f, b * (float)rate / 100.0f);
}

/* (CamDirector_NewRoom: the camera reset - its shake off, no cut to see - nothing set, nobody
 * followed, no event's shot nor cutscene; its rate, the characters' setups and whom it falls
 * back on kept) */
void camdir_glow_cut(void) {
    if (!(gProgress.state[0x28 >> 5] & (1u << (0x28 & 31)))) {
        post_glow_clear();
    }
}

void camdir_new_room(CamDirector *d) {
    float rate = d->move_rate;
    int player = d->player, shown = d->shown, followed = d->followed;
    int setups[HACTOR_MAX][2];

    memcpy(setups, d->setups, sizeof(setups));
    memset(d, 0, sizeof(*d));
    memcpy(d->setups, setups, sizeof(setups));
    d->player = player;
    d->shown = shown;
    d->followed = followed;
    d->move_rate = rate == 0.0f ? 6.0f : rate;
    d->set = 0;
    d->path_no = d->last_path = d->last_set = -1;
    d->target = d->last_target = -1;
    d->shown_now = d->shown_before = -1;
}

void camdir_room_start(CamDirector *d, const float *sets, int nsets, const int32_t *paths) {
    float fov_deg, lim_deg;

    sSets = sets;   /* (the original's camera has the room's sets already) */
    sNsets = nsets;
    d->started = 1;
    camdir_glow_cut();
    gEngine.camera.znear = 1.0f;     /* (the camera's range: Camera +0xB0 / +0xB4 - the story's 0x0B */
    gEngine.camera.zfar = 2000.0f;   /* changes it after, e.g. water-tower-8f-1's far 20000) */

    d->last_set = d->last_path = -1;
    take_set(d, d->set);
    d->point = d->look;
    d->fov_dip = PI_F * (0.2f * (180.0f * d->set_fov / PI_F)) / 180.0f;
    lim_deg = 180.0f * d->fov_dip / PI_F;
    fov_deg = 180.0f * d->set_fov / PI_F;
    d->fov = PI_F * (fov_deg - (1.0f + cosf(0.0f)) * lim_deg) / 180.0f;
    d->cut = 0;
    d->ease = 0;
    d->data = paths;
    if (paths != NULL) {
        d->npaths = paths[0];
        if (d->npaths <= 0) {
            d->data = NULL;
        }
        load_setup(d, 0, 1.0f);
        if (d->path_no != -1) {
            load_setup(d, d->path_no, 1.0f);
            spline_seek(&d->path, path_nearest(d, 2, 0.0f));
            d->look = path_look(d);
            d->eye = path_eye(d);
        }
    }
    if (d->path_no == -1) {
        keep_in_view(d, 100);
    }
}

void camdir_set_setup(CamDirector *d, int set, int path) {
    d->set = set;
    d->path_no = path;
}

void camdir_follow(CamDirector *d, int target, Vec3 offset) {
    d->target = target;
    d->offset = offset;
}

void camdir_char_setup(CamDirector *d, int who, int set, int path) {
    if (who >= 0 && who < HACTOR_MAX) {
        d->setups[who][0] = set;
        d->setups[who][1] = path;
    }
}

void camdir_forget_setups(CamDirector *d) {
    int i;

    for (i = 0; i < HACTOR_MAX; i++) {
        d->setups[i][0] = d->setups[i][1] = -1;
    }
}

int camdir_follow_check(CamDirector *d, int who) {
    const HBody *b;

    if (who < 0) {
        camdir_follow(d, -1, vec3(0.0f, 0.0f, 0.0f));
        return -1;
    }
    b = who < HACTOR_MAX ? hactor_body(who) : NULL;
    if (b != NULL && (who == d->player || (b->has && d->setups[who][0] != -1 && b->room == gEngine.room.id))) {
        return who;
    }
    camdir_follow(d, d->player, vec3(0.0f, 10.0f, 0.0f));
    return d->player;
}

void camdir_camera_on(CamDirector *d, int who) {
    d->followed = camdir_follow_check(d, who);
    if (d->followed < 0) {
        return;
    }
    camdir_follow(d, d->followed, vec3(0.0f, 10.0f, 0.0f));
    camdir_set_setup(d, d->setups[d->followed][0], d->setups[d->followed][1]);
}

void camdir_take_followed(CamDirector *d) {
    d->followed = camdir_follow_check(d, d->followed);
    if (d->followed >= 0) {
        camdir_set_setup(d, d->setups[d->followed][0], d->setups[d->followed][1]);
    }
}

void camdir_scene(CamDirector *d, int on) {
    d->scene = on != 0;
    d->last_set = d->last_path = -1;
    camdir_glow_cut();   /* (CamDirector_ModeEvent / ModeDefault) */
}

void camdir_event(CamDirector *d, int on) {
    if (on && !d->event) {   /* (CamDirector_ModeNormal: the event's shot comes in) */
        camdir_glow_cut();
    }
    d->event = on != 0;
}

void camdir_retake(CamDirector *d) {
    d->last_set = d->last_path = -1;
    camdir_track(d);
}

void camdir_next_set(CamDirector *d) {
    d->set = d->set + 1 < sNsets && d->set + 1 >= 0 ? d->set + 1 : 0;
    d->path_no = -1;
    d->target = -1;
}

void camdir_rate(CamDirector *d, float rate) {
    d->move_rate = rate < 0.0f ? 6.0f : rate;
}

void camdir_shake(const CamDirector *d, Camera *c) {
    static uint32_t seed = 0x2545F491u;
    float s[3];
    int i;

    if (d->shake == 0.0f) {
        return;
    }
    for (i = 0; i < 3; i++) {
        seed = seed * 1664525u + 1013904223u;
        s[i] = 2.0f * ((float)(seed >> 8) / 16777216.0f - 0.5f) * d->shake;
    }
    c->pos = vec3_add(c->pos, vec3(s[0], s[1], s[2]));
}

void camdir_track(CamDirector *d) {
    int far = 0, changed = 0;
    Vec3 p;

    if (d->scene) {
        return;
    }
    if (d->target >= 0 && d->last_target != d->target && target_at(d, &p)) {
        if (d->path_no == -1) {
            far = camera_out_of_view(&gEngine.camera, p);   /* (the camera as last drawn) */
        } else {
            float span = (float)d->path.tmax, cur = d->path.t, t = path_nearest(d, 2, 0.0f);
            float diff = cur / span - t / span;

            if (diff <= 0.0f) {
                diff = -diff;
            }
            if (!(d->len_look * diff <= 35.0f)) {
                far = 1;
            }
        }
    }
    if (d->last_set != d->set || far) {
        changed = 1;
        take_set(d, d->set);
        d->point = d->look;
        d->fov = d->set_fov;
        d->cut = 0x80;
        d->ease = 0;
        if (d->path_no == -1) {
            keep_in_view(d, 100);
        }
    }
    if (d->path_no != -1 && (d->last_path != d->path_no || far)) {
        changed = 1;
        load_setup(d, d->path_no, 1.0f);
        if (d->target >= 0) {
            spline_seek(&d->path, path_nearest(d, 2, 0.0f));
            spline_seek(&d->path, path_move(d, path_nearest(d, 2, 0.0f), 0.0f));
        } else {
            spline_seek(&d->path, 1.0f);
        }
    }
    if (changed) {
        camdir_glow_cut();
    }
}

void camdir_ease(CamDirector *d) {
    float fov_deg, lim_deg, deg;
    uint8_t cut = d->cut;

    d->changed = 0;
    if (d->scene || d->event) {
        return;
    }
    if (!(cut & 0x80)) {
        if (cut != 0) {
            d->cut = cut - 1;
            lim_deg = 180.0f * d->fov_dip / PI_F;
            fov_deg = 180.0f * d->set_fov / PI_F;
            deg = fov_deg - (1.0f + cosf(0.0f)) * lim_deg;
        } else if (d->ease < 60) {
            d->ease++;
            lim_deg = 180.0f * d->fov_dip / PI_F;
            fov_deg = 180.0f * d->set_fov / PI_F;
            deg = fov_deg - (1.0f + cosf(PI_F * (float)d->ease / 60.0f)) * lim_deg;
        } else {
            d->cut = 0x80;
            deg = 180.0f * d->set_fov / PI_F;
        }
        d->fov = PI_F * deg / 180.0f;
    }
    if (d->path_no == -1) {
        keep_in_view(d, 10);
    } else {
        if (d->target >= 0) {
            spline_seek(&d->path, path_move(d, path_nearest(d, 2, 0.0f), 0.0f));
        }
        d->look = path_look(d);
        d->eye = path_eye(d);
    }
}

void camdir_remember(CamDirector *d) {
    d->changed = camdir_setup_changed(d);
    d->last_set = d->set;
    d->last_path = d->path_no;
    d->last_target = d->target;
}

int camdir_cut(const CamDirector *d) {
    return d->shown_before != -1 && d->shown_now != d->shown_before;
}

void camdir_update(CamDirector *d) {
    camdir_remember(d);
    d->shown_before = d->shown_now;
    d->shown_now = d->shown;
    if (d->scene || d->event) {
        return;
    }
    if (d->path_no != -1) {
        d->look = path_look(d);
        d->eye = path_eye(d);
    }
}

void camdir_restart(CamDirector *d) {
    camdir_glow_cut();
    d->ease = 60;
    d->cut = 0x80;
    d->fov = d->set_fov;
    if (d->path_no == -1) {
        take_set(d, d->set);
        d->point = d->look;
        keep_in_view(d, 100);
        return;
    }
    spline_seek(&d->path, path_nearest(d, 2, 0.0f));
}

int camdir_setup_changed(const CamDirector *d) {
    return !(d->last_set == d->set && d->last_path == d->path_no);
}

void camdir_apply(const CamDirector *d, Camera *c) {
    Vec3 v = vec3_sub(d->look, d->eye);
    float h = sqrtf(v.x * v.x + v.z * v.z);

    c->pos = d->eye;
    c->roll = 0.0f;   /* (a set's, a path's: Camera_ApplyPreset / Restart) */
    c->yaw = atan2f(v.x, -v.z);
    c->pitch = atan2f(v.y * c->up, h);
    c->fov = 2.0f * atanf(0.75f * tanf(d->fov * 0.5f));
}
