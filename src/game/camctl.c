/* The camera director (SceneGame +0xF6CBB0, second vtable at +0x64): drives the camera
 * (gCamera) through the room's camera setups. */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"

extern VObject *gCamera;   /* the camera */

/* reset for a new room: reset the camera, no setup selected, default light direction */
void func_00225550(u8 *d) {
    VObject *cam = gCamera;

    VCALL(cam, 0xC, void (*)(VObject *))(cam);
    VCALL(cam, 0x6C, void (*)(VObject *, f32))(cam, 0.0f);
    AT(d, 0x6C, s32) = 0;
    AT(d, 0x70, s32) = -1;
    AT(d, 0x7C, s32) = -1;
    AT(d, 0x78, s32) = -1;
    AT(d, 0xB4, s32) = 0;
    AT(d, 0xB0, s32) = 0;
    AT(d, 0x74, u8) = 0;
    AT(d, 0x68, u8) = 0;
    AT(d, 0xD0, f32) = 0.0f;
    AT(d, 0xD4, u32) = 0x3E4CCCCD;   /* 0.2f */
    AT(d, 0xD8, f32) = 1.0f;
    AT(d, 0xDC, f32) = 1.0f;
    sceVu0Normalize((f32 *)(d + 0xD0), (f32 *)(d + 0xD0));
    AT(d, 0xE0, f32) = 30.0f;
    AT(d, 0xE4, f32) = 0.0f;
    AT(d, 0x150, u8) = 0;
    AT(d, 0x69, u8) = 0;
    AT(d, 0x164, u8) = 0;
    ((void (*)(u8 *))AT(AT(d, 0x64, u8 *), 0x78, void *))(d);
}

extern VObject *gRenderer;   /* the renderer */
extern const PTMF D_003E5240;  /* { 0, -1, func_00224770 } */
extern const PTMF D_003E5250;  /* { 0, -1, func_00224740 } */

/* vt+0x24 the current camera setup (+0x6C); -1 in the free mode (+0xF4) */
s32 func_00224300(u8 *d) {
    if (AT(d, 0xF4, u8) == 1) {
        return -1;
    }
    return AT(d, 0x6C, s32);
}

/* two modes of the director (update state +0xE8, flag +0xF4); both clear the renderer's
 * work area (+0x5C) */
void func_00224330(u8 *d) {
    AT(d, 0xE8, PTMF) = D_003E5250;
    AT(d, 0xF4, u8) = 1;
    AT(d, 0x7C, s32) = -1;
    AT(d, 0x78, s32) = -1;
    VCALL(gRenderer, 0x5C, s32 (*)(VObject *))(gRenderer);
}

/* vt+0x78 (the default mode) */
void func_00224380(u8 *d) {
    AT(d, 0xE8, PTMF) = D_003E5240;
    AT(d, 0xF4, u8) = 0;
    AT(d, 0x7C, s32) = -1;
    AT(d, 0x78, s32) = -1;
    VCALL(gRenderer, 0x5C, s32 (*)(VObject *))(gRenderer);
}

/* (the director's interface, gCamDirector) +0xC follow `target` (its position, +0x10) with an
 * offset (x, y, z) */
void func_002246F0(u8 *f, u8 *target, f32 x, f32 y, f32 z) {
    AT(f, 0xB0, u8 *) = target;
    AT(f, 0xC0, f32) = x;
    AT(f, 0xC4, f32) = y;
    AT(f, 0xC8, f32) = z;
    AT(f, 0xCC, f32) = 1.0f;
    AT(f, 0x38, u8 *) = AT(f, 0xB0, u8 *) + 0x10;
    AT(f, 0x40, f32) = AT(f, 0xC0, f32);
    AT(f, 0x44, f32) = AT(f, 0xC4, f32);
    AT(f, 0x48, f32) = AT(f, 0xC8, f32);
    AT(f, 0x4C, f32) = 1.0f;
}

/* (interface) +0x28 the camera setup to use (+0x6C, +0x70) */
void func_00223E30(u8 *f, s32 a, s32 b) {
    AT(f, 0x6C, s32) = a;
    AT(f, 0x70, s32) = b;
}

/* +0x6C the mode flag +0xF4 */
s32 func_00223E20(u8 *d) {
    return AT(d, 0xF4, u8);
}

extern f32 func_0031C058(f32 x);
extern void func_0021A290(u8 *d, s32 *data, s32 setup, f32 t);
extern void func_00219E10(u8 *d, s32 setup, f32 t);
extern f32 func_002197A0(u8 *d, s32 mode, f32 t);
extern void func_0025F6A0(void *o, f32 v);
extern void func_00219D70(u8 *d, f32 *out, f32 t);   /* the look-at point at t */
extern void func_00219CD0(u8 *d, f32 *out, f32 t);   /* the eye at t */
extern void func_002243D0(u8 *d, s32 n);

/* the room's camera at the start of play: the camera's range, its view angle (+0x84) and the
 * director's angle limits (+0x88, +0x80); with the room's camera data (PAC section 6): take it
 * and put the camera at the current setup's (+0x70) eye and look-at point; no setup: pick one */
void func_002252B0(u8 *d, s32 target) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, k02 = {0x3E4CCCCD};
    VObject *cam;
    f32 fovDeg, limDeg;

    AT(d, 0x7C, s32) = -1;
    AT(d, 0x78, s32) = -1;
    VCALL(gRenderer, 0x5C, s32 (*)(VObject *))(gRenderer);
    cam = gCamera;
    VCALL(cam, 0xB0, void (*)(VObject *, f32))(cam, 1.0f);
    VCALL(cam, 0xB4, void (*)(VObject *, f32))(cam, 2000.0f);
    VCALL(cam, 0x8C, void (*)(VObject *, s32))(cam, AT(d, 0x6C, s32));
    VCALL(cam, 0x2C, void (*)(VObject *, void *))(cam, d + 0xA0);
    AT(d, 0x84, f32) = VCALL(cam, 0x64, f32 (*)(VObject *))(cam);
    AT(d, 0x88, f32) = kPi.f * (k02.f * (180.0f * AT(d, 0x84, f32) / kPi.f)) / 180.0f;
    limDeg = 180.0f * AT(d, 0x88, f32) / kPi.f;
    fovDeg = 180.0f * AT(d, 0x84, f32) / kPi.f;
    AT(d, 0x80, f32) = kPi.f * (fovDeg - (1.0f + func_0031C058(0.0f)) * limDeg) / 180.0f;
    AT(d, 0x8C, u8) = 0;
    AT(d, 0x90, s32) = 0;
    if (target != 0) {
        func_0021A290(d, (s32 *)target, 0, 1.0f);
        AT(d, 0x38, u8 *) = AT(d, 0xB0, u8 *) + 0x10;
        AT(d, 0x40, f32) = AT(d, 0xC0, f32);
        AT(d, 0x44, f32) = AT(d, 0xC4, f32);
        AT(d, 0x48, f32) = AT(d, 0xC8, f32);
        AT(d, 0x4C, f32) = 1.0f;
        if (AT(d, 0x70, s32) != -1) {
            f32 v[4];

            func_00219E10(d, AT(d, 0x70, s32), 1.0f);
            func_0025F6A0(d + 8, func_002197A0(d, 2, 0.0f));
            func_00219D70(d, v, 0.0f);
            cam = gCamera;
            VCALL(cam, 0x28, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
            func_00219CD0(d, v, 0.0f);
            VCALL(cam, 0x1C, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
        }
    }
    if (AT(d, 0x70, s32) == -1) {
        func_002243D0(d, 100);
    }
}


/* the camera director restarted (renderer +0x5C; +0x8C 0x80, +0x90 60, the field of view back
 * to +0x84): with no setup (+0x70) the free camera on its target, else setup's position at
 * t 0 */
void func_002251C0(u8 *d) {
    VCALL(gRenderer, 0x5C, s32 (*)(VObject *))(gRenderer);
    AT(d, 0x90, s32) = 0x3C;
    AT(d, 0x8C, u8) = 0x80;
    AT(d, 0x80, f32) = AT(d, 0x84, f32);
    if (AT(d, 0x70, s32) == -1) {
        VObject *cam = gCamera;

        VCALL(cam, 0x8C, void (*)(VObject *, s32))(cam, AT(d, 0x6C, s32));
        VCALL(cam, 0x2C, void (*)(VObject *, void *))(cam, d + 0xA0);
        func_002243D0(d, 100);
        return;
    }
    VCALL(gCamera, 0x70, void (*)(VObject *, f32))(gCamera, 0.0f);
    func_0025F6A0(d + 8, func_002197A0(d, 2, 0.0f));
}

/* the setup changed since it was taken (+0x78 / +0x7C against +0x6C / +0x70) */
s32 func_00224650(u8 *d) {
    return !(AT(d, 0x78, s32) == AT(d, 0x6C, s32) && AT(d, 0x7C, s32) == AT(d, 0x70, s32));
}

/* take the room's camera data (count first; none or empty: no data), then setup `setup` at t */
void func_0021A290(u8 *d, s32 *data, s32 setup, f32 t) {
    AT(d, 0x2C, s32 *) = data;
    if (data == NULL) {
        return;
    }
    AT(d, 0x30, s32) = AT(d, 0x2C, s32 *)[0];
    if (AT(d, 0x30, s32) <= 0) {
        AT(d, 0x2C, s32) = 0;
    }
    func_00219E10(d, setup, t);
}

extern f32 func_0025F580(void *spline, s32 comp, f32 u);       /* evaluate component comp at u */
extern void func_0025F7A0(void *spline, s32 n, s32 dims, f32 *keys);

/* setup `setup` of the room's camera data: its spline (keys after the header and the earlier
 * setups' keys) into +0x8, the lengths of its two paths (components 0..2 +0x28, 3..5 +0x24,
 * sampled at whole steps up to +0x18) and the speeds along them (+0x4, +0x0), then go to t */
void func_00219E10(u8 *d, s32 setup, f32 t) {
    static const union { u32 u; f32 f; } kSpeed = {0x3E47AE14};   /* 0.195f */
    f32 a[2][4], b[2][4];
    s32 *data = AT(d, 0x2C, s32 *);
    s32 n, i, cur;
    f32 *keys;
    f32 u, end;

    if (data == NULL || setup < 0 || setup >= AT(d, 0x30, s32)) {
        return;
    }
    AT(d, 0x34, s32) = setup;
    n = AT(d, 0x30, s32);
    keys = (f32 *)((u8 *)data + n * 8 + ((n & 1) == 0 ? 8 : 0) + 8);
    for (i = 0; i < AT(d, 0x34, s32); i++) {
        keys = (f32 *)((u8 *)keys + data[2 + i * 2] * data[3 + i * 2] * 32);
    }
    func_0025F7A0(d + 8, data[2 + AT(d, 0x34, s32) * 2], data[3 + AT(d, 0x34, s32) * 2], keys);
    AT(d, 0x28, f32) = 0.0f;
    AT(d, 0x24, f32) = 0.0f;
    end = (f32)AT(d, 0x18, s32);
    u = keys[0];
    b[0][0] = func_0025F580(d + 8, 3, u);
    b[0][1] = func_0025F580(d + 8, 4, u);
    b[0][2] = func_0025F580(d + 8, 5, u);
    a[0][0] = func_0025F580(d + 8, 0, u);
    a[0][1] = func_0025F580(d + 8, 1, u);
    a[0][2] = func_0025F580(d + 8, 2, u);
    u = u + 1.0f;
    cur = 0;
    while (u < end) {
        f32 dx, dy, dz, ex, ey, ez;

        cur ^= 1;
        b[cur][0] = func_0025F580(d + 8, 3, u);
        b[cur][1] = func_0025F580(d + 8, 4, u);
        b[cur][2] = func_0025F580(d + 8, 5, u);
        a[cur][0] = func_0025F580(d + 8, 0, u);
        a[cur][1] = func_0025F580(d + 8, 1, u);
        a[cur][2] = func_0025F580(d + 8, 2, u);
        u = u + 1.0f;
        dy = b[0][1] - b[1][1];
        dx = b[0][0] - b[1][0];
        dz = b[0][2] - b[1][2];
        ey = a[0][1] - a[1][1];
        ex = a[0][0] - a[1][0];
        ez = a[0][2] - a[1][2];
        AT(d, 0x24, f32) = AT(d, 0x24, f32) + __builtin_sqrtf(dy * dy + dx * dx + dz * dz);
        AT(d, 0x28, f32) = AT(d, 0x28, f32) + __builtin_sqrtf(ey * ey + ex * ex + ez * ez);
    }
    AT(d, 0x0, f32) = kSpeed.f / (AT(d, 0x24, f32) / 100.0f);
    AT(d, 0x4, f32) = kSpeed.f / (AT(d, 0x28, f32) / 100.0f);
    func_0025F6A0(d + 8, t);
}


/* the point of the target to keep in view: its position (+0x38) + offset (+0x40) */

/* the time along the look-at path (components 3..5) nearest the target point, by horizontal
 * distance (mode 0), height difference (1) or distance (2): the nearest key, then steps of
 * +0x0 around it; -1: no path / target */
f32 func_002197A0(u8 *d, s32 mode, f32 t) {
    f32 p[4] __attribute__((aligned(16)));
    f32 bestHd = -1.0f, bestDy = -1.0f, bestD = -1.0f, best = -1.0f;
    f32 *keys;
    f32 u, end;
    s32 n, k, kb = 0, lo, hi;
    u8 m = (u8)mode;

    if (!(t < 1.0f)) {
        u = t;
    } else {
        u = AT(d, 0x8, f32);
    }
    if (AT(d, 0x2C, void *) == NULL || AT(d, 0x38, f32 *) == NULL) {
        return -1.0f;
    }
#ifdef HG_NATIVE
    if ((u32)AT(d, 0x38, f32 *) < 0x1000) {   /* following an absent character (the PS2 reads low memory) */
        return -1.0f;
    }
#endif
    sceVu0CopyVector(p, AT(d, 0x38, f32 *));
    sceVu0AddVector(p, p, (f32 *)(d + 0x40));
    func_0025F580(d + 8, 3, u);
    func_0025F580(d + 8, 4, u);
    func_0025F580(d + 8, 5, u);
    n = AT(d, 0x10, s32);
    for (k = 0; k < n; k++) {
        f32 tk = (f32)(s32)AT(d, 0xC, f32 *)[k * 8];
        f32 x = func_0025F580(d + 8, 3, tk);
        f32 y = func_0025F580(d + 8, 4, tk);
        f32 z = func_0025F580(d + 8, 5, tk);
        f32 dz = z - p[2], dx = x - p[0], dy = y - p[1];
        f32 hd = __builtin_sqrtf(dz * dz + dx * dx);
        f32 dd;
        s32 take;

        if (dy <= 0.0f) {
            dy = -dy;
        }
        dd = __builtin_sqrtf(dy * dy + hd * hd);
        if (m == 0) {
            take = bestHd < 0.0f || !(bestHd <= hd);
        } else if (m == 1) {
            take = bestDy < 0.0f || !(bestDy <= dy);
        } else {
            take = bestD < 0.0f || !(bestD <= dd);
        }
        if (take) {
            bestHd = hd;
            kb = k;
            bestDy = dy;
            bestD = dd;
            best = tk;
        }
    }
    keys = AT(d, 0xC, f32 *);
    lo = kb >= 2 ? kb - 1 : 0;
    hi = kb < n - 1 ? kb + 1 : n - 1;
    end = (f32)(s32)keys[kb * 8] + (f32)((s32)keys[hi * 8] - (s32)keys[kb * 8]) / 2.0f;
    u = (f32)(s32)keys[lo * 8] + (f32)((s32)keys[kb * 8] - (s32)keys[lo * 8]) / 2.0f;
    while (u < end) {
        f32 x = func_0025F580(d + 8, 3, u);
        f32 y = func_0025F580(d + 8, 4, u);
        f32 z = func_0025F580(d + 8, 5, u);
        f32 dz = z - p[2], dx = x - p[0], dy = y - p[1];
        f32 hd = __builtin_sqrtf(dz * dz + dx * dx);
        f32 dd;
        s32 take;

        if (dy <= 0.0f) {
            dy = -dy;
        }
        dd = __builtin_sqrtf(dy * dy + hd * hd);
        if (m == 0) {
            take = bestHd < 0.0f || !(bestHd <= hd);
        } else if (m == 1) {
            take = bestDy < 0.0f || !(bestDy <= dy);
        } else {
            take = bestD < 0.0f || !(bestD <= dd);
        }
        if (take) {
            bestHd = hd;
            bestDy = dy;
            bestD = dd;
            best = u;
        }
        u = u + AT(d, 0x0, f32);
    }
    return best;
}

/* the camera path's look-at point (components 3..5) at time t (below 1: the current time) */
void func_00219D70(u8 *d, f32 *out, f32 t) {
    f32 u = t < 1.0f ? AT(d, 0x8, f32) : t;

    out[0] = func_0025F580(d + 8, 3, u);
    out[1] = func_0025F580(d + 8, 4, u);
    out[2] = func_0025F580(d + 8, 5, u);
    out[3] = 1.0f;
}

/* the camera path's eye (components 0..2) at time t (below 1: the current time) */
void func_00219CD0(u8 *d, f32 *out, f32 t) {
    f32 u = t < 1.0f ? AT(d, 0x8, f32) : t;

    out[0] = func_0025F580(d + 8, 0, u);
    out[1] = func_0025F580(d + 8, 1, u);
    out[2] = func_0025F580(d + 8, 2, u);
    out[3] = 1.0f;
}

extern f32 func_00219530(u8 *d, f32 t, f32 u);

/* each frame (not while an event drives it, +0xF4 / +0x69): the view angle +0x80 eases in over
 * 60 frames (+0x90) and back out after a cut (+0x8C counts down; 0x80: settled); the camera
 * follows the current setup (+0x70) along its path (+0xB0: from the player's nearest point),
 * else a setup is chosen */
void func_00224EE0(u8 *d) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    f32 fovDeg, limDeg, deg;
    u8 cut = AT(d, 0x8C, u8);

    if (AT(d, 0xF4, u8) == 1 || AT(d, 0x69, u8) != 0) {
        return;
    }
    if (!(cut & 0x80)) {
        if (cut != 0) {
            AT(d, 0x8C, u8) = cut - 1;
            limDeg = 180.0f * AT(d, 0x88, f32) / kPi.f;
            fovDeg = 180.0f * AT(d, 0x84, f32) / kPi.f;
            deg = fovDeg - (1.0f + func_0031C058(0.0f)) * limDeg;
        } else if (AT(d, 0x90, u32) < 60) {
            AT(d, 0x90, u32)++;
            limDeg = 180.0f * AT(d, 0x88, f32) / kPi.f;
            fovDeg = 180.0f * AT(d, 0x84, f32) / kPi.f;
            deg = fovDeg - (1.0f + func_0031C058(kPi.f * (f32)AT(d, 0x90, u32) / 60.0f)) * limDeg;
        } else {
            AT(d, 0x8C, u8) = 0x80;
            deg = 180.0f * AT(d, 0x84, f32) / kPi.f;
        }
        AT(d, 0x80, f32) = kPi.f * deg / 180.0f;
    }
    if (AT(d, 0x70, s32) == -1) {
        func_002243D0(d, 10);
    } else {
        VObject *cam;
        f32 v[3];

        if (AT(d, 0xB0, s32) != 0) {
            func_0025F6A0(d + 8, func_00219530(d, func_002197A0(d, 2, 0.0f), 0.0f));
        }
        func_00219D70(d, v, 0.0f);
        cam = gCamera;
        VCALL(cam, 0x28, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
        func_00219CD0(d, v, 0.0f);
        VCALL(cam, 0x1C, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
    }
}


/* move along the camera path from time `from` (below 1: the current time +0x8) towards `to`,
 * by the distance the look-at point (components 3..5) covers between them, measured in steps
 * of +0x0 and scaled by +0x50 / +0x24; clamped to `to` and the path's range +0x14..+0x18 */
f32 func_00219530(u8 *d, f32 to, f32 from) {
    f32 pts[2][4];
    f32 dir, t, end, len = 0.0f, res;
    s32 cur = 1;

    if (from < 1.0f) {
        from = AT(d, 0x8, f32);
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
    pts[0][0] = func_0025F580(d + 8, 3, t);
    pts[0][1] = func_0025F580(d + 8, 4, t);
    pts[0][2] = func_0025F580(d + 8, 5, t);
    t += AT(d, 0x0, f32);
    while (t < end) {
        f32 *p = pts[cur & 1];
        f32 dx, dy, dz;

        p[0] = func_0025F580(d + 8, 3, t);
        p[1] = func_0025F580(d + 8, 4, t);
        p[2] = func_0025F580(d + 8, 5, t);
        cur ^= 1;
        dy = pts[0][1] - pts[1][1];
        dx = pts[0][0] - pts[1][0];
        dz = pts[0][2] - pts[1][2];
        t += AT(d, 0x0, f32);
        len += __builtin_sqrtf(dy * dy + dx * dx + dz * dz);
    }
    if (dir != 0.0f) {
        res = res + AT(d, 0x50, f32) * (dir * (len / AT(d, 0x24, f32)));
        if (dir <= 0.0f) {
            if (res < to) {
                res = to;
            }
        } else if (!(res <= to)) {
            res = to;
        }
    }
    if (res <= (f32)AT(d, 0x14, s32)) {
        res = (f32)AT(d, 0x14, s32);
    }
    if (!(res < (f32)AT(d, 0x18, s32))) {
        res = (f32)AT(d, 0x18, s32);
    }
    return res;
}


/* each frame: when the camera set (+0x6C, last applied +0x78) changes or the target (+0xB0,
 * last +0xB4) left the view (no setup: the camera's test +0xD4; else its path time differs
 * from the current one by more than 35 units), the camera takes the set and the view angle
 * resets; when the setup (+0x70, last +0x7C) changes or on such a jump, the camera path is
 * set up again and put at the target's nearest point; the renderer is told (+0x5C) */
void func_00224C60(u8 *d) {
    VObject *cam;
    s32 far = 0;
    s32 changed = 0;

    if (AT(d, 0xF4, u8) == 1) {
        return;
    }
    if (AT(d, 0xB0, u8 *) != NULL && AT(d, 0xB4, u8 *) != AT(d, 0xB0, u8 *)) {
        if (AT(d, 0x70, s32) == -1) {
            cam = gCamera;
            far = (u8)VCALL(cam, 0xD4, s32 (*)(VObject *, u8 *))(cam, AT(d, 0xB0, u8 *) + 0x10);
        } else {
            f32 span = (f32)AT(d, 0x18, s32);
            f32 cur = AT(d, 0x8, f32);
            f32 t = func_002197A0(d, 2, 0.0f);
            f32 diff = cur / span - t / span;

            if (diff <= 0.0f) {
                diff = -diff;
            }
            if (!(AT(d, 0x24, f32) * diff <= 35.0f)) {
                far = 1;
            }
        }
    }
    if (AT(d, 0x78, s32) != AT(d, 0x6C, s32) || far) {
        changed = 1;
        cam = gCamera;
        VCALL(cam, 0x8C, void (*)(VObject *, s32))(cam, AT(d, 0x6C, s32));
        VCALL(cam, 0x2C, void (*)(VObject *, void *))(cam, d + 0xA0);
        AT(d, 0x80, f32) = AT(d, 0x84, f32) = VCALL(cam, 0x64, f32 (*)(VObject *))(cam);
        AT(d, 0x8C, u8) = 0x80;
        AT(d, 0x90, s32) = 0;
        if (AT(d, 0x70, s32) == -1) {
            func_002243D0(d, 100);
        }
    }
    if (AT(d, 0x70, s32) != -1 && (AT(d, 0x7C, s32) != AT(d, 0x70, s32) || far)) {
        changed = 1;
        func_00219E10(d, AT(d, 0x70, s32), 1.0f);
        if (AT(d, 0xB0, u8 *) != NULL) {
            func_0025F6A0(d + 8, func_002197A0(d, 2, 0.0f));
            func_0025F6A0(d + 8, func_00219530(d, func_002197A0(d, 2, 0.0f), 0.0f));
        } else {
            func_0025F6A0(d + 8, 1.0f);
        }
    }
    if (changed) {
        VCALL(gRenderer, 0x5C, void (*)(VObject *))(gRenderer);
    }
}


/* each frame: the director's mode (+0xE8, a member function), then the camera's update */
void func_00224C20(u8 *d) {
    ptmf_scall(d, &AT(d, 0xE8, PTMF));
    VCALL(gCamera, 0x14, void (*)(VObject *))(gCamera);
}


extern f32 D_0047E410, D_0047E418;   /* the right stick, x and y (-1..1) */
void func_00223F70(f32 *p);           /* the event camera's frame */

/* the director's normal mode (+0xE8), each frame. The free camera (+0x68, e.g. debug):
 * orbits the target (+0xB0) with the right stick (yaw +0xE4 in degrees, distance +0xE0 >= 5).
 * An event camera (+0x69): started on the target the first frame, then run. Otherwise the
 * camera follows the current setup's path. The last set / setup / target are kept
 * (+0x78 / +0x7C / +0xB4) for the switch test; changing mode forces it. */
void func_00224770(u8 *d) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kFov = {0x3F860A92};
    VObject *cam;
    f32 v[4] __attribute__((aligned(16)));

    AT(d, 0x78, s32) = AT(d, 0x6C, s32);
    AT(d, 0x7C, s32) = AT(d, 0x70, s32);
    AT(d, 0xB4, u8 *) = AT(d, 0xB0, u8 *);
    if (AT(d, 0x68, u8) == 0) {
        if (AT(d, 0x74, u8) != AT(d, 0x68, u8)) {
            AT(d, 0x7C, s32) = -1;
            AT(d, 0x78, s32) = -1;
            func_00224C60(d);
        }
        if (AT(d, 0xB0, u8 *) == NULL) {
            AT(d, 0x69, u8) = 0;
        }
        if (AT(d, 0x69, u8) != 0) {
            if (AT(d, 0x150, u8) != AT(d, 0x69, u8)) {
                u8 *t = AT(d, 0xB0, u8 *);

                AT(d, 0x108, f32) = 180.0f * AT(t, 0x54, f32) / kPi.f;
                AT(d, 0x140, u8) = 0;
                AT(d, 0x10C, s32) = 0;
                sceVu0CopyVector((f32 *)(d + 0x120), (f32 *)(t + 0x10));
                sceVu0AddVector((f32 *)(d + 0x120), (f32 *)(d + 0xC0), (f32 *)(d + 0x120));
                sceVu0AddVector((f32 *)(d + 0x120), (f32 *)(d + 0x130), (f32 *)(d + 0x120));
                VCALL(gRenderer, 0x5C, void (*)(VObject *))(gRenderer);
            }
            func_00223F70((f32 *)(d + 0x100));
        } else {
            if (AT(d, 0x150, u8) != AT(d, 0x69, u8)) {
                AT(d, 0x7C, s32) = -1;
                AT(d, 0x78, s32) = -1;
                func_00224C60(d);
            }
            if (AT(d, 0x70, s32) != -1) {
                func_00219D70(d, v, 0.0f);
                cam = gCamera;
                VCALL(cam, 0x28, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
                func_00219CD0(d, v, 0.0f);
                VCALL(cam, 0x1C, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
            }
            VCALL(gCamera, 0x5C, void (*)(VObject *, f32))(gCamera, AT(d, 0x80, f32));
        }
    } else {
        sceVu0FMATRIX m;
        f32 w[4] __attribute__((aligned(16)));

        if (AT(d, 0xB0, u8 *) == NULL) {
            if (AT(d, 0x70, s32) != -1) {
                func_00219D70(d, v, 0.0f);
                VCALL(gCamera, 0x28, void (*)(VObject *, f32, f32, f32))(gCamera, v[0], v[1], v[2]);
            }
        } else {
            sceVu0CopyVector(v, (f32 *)(AT(d, 0xB0, u8 *) + 0x10));
            sceVu0AddVector(v, (f32 *)(d + 0xC0), v);
            if (AT(d, 0x74, u8) != AT(d, 0x68, u8)) {
                AT(d, 0xE0, f32) = 30.0f;
                AT(d, 0xE4, f32) = 180.0f + 180.0f * AT(AT(d, 0xB0, u8 *), 0x54, f32) / kPi.f;
                if (AT(d, 0xE4, f32) < -180.0f) {
                    AT(d, 0xE4, f32) = AT(d, 0xE4, f32) + 360.0f;
                }
                if (!(AT(d, 0xE4, f32) <= 180.0f)) {
                    AT(d, 0xE4, f32) = AT(d, 0xE4, f32) - 360.0f;
                }
            }
            VCALL(gCamera, 0x28, void (*)(VObject *, f32, f32, f32))(gCamera, v[0], v[1], v[2]);
        }
        AT(d, 0xE4, f32) = AT(d, 0xE4, f32) + 3.0f * D_0047E410;
        AT(d, 0xE0, f32) = AT(d, 0xE0, f32) + 3.0f * D_0047E418;
        if (AT(d, 0xE4, f32) < -180.0f) {
            AT(d, 0xE4, f32) = AT(d, 0xE4, f32) + 360.0f;
        }
        if (!(AT(d, 0xE4, f32) <= 180.0f)) {
            AT(d, 0xE4, f32) = AT(d, 0xE4, f32) - 360.0f;
        }
        if (AT(d, 0xE0, f32) < 5.0f) {
            AT(d, 0xE0, f32) = 5.0f;
        }
        sceVu0UnitMatrix(m);
        sceVu0RotMatrixY(m, m, kPi.f * AT(d, 0xE4, f32) / 180.0f);
        sceVu0ApplyMatrix(w, m, (f32 *)(d + 0xD0));
        sceVu0ScaleVector(w, w, AT(d, 0xE0, f32));
        sceVu0AddVector(w, v, w);
        cam = gCamera;
        VCALL(cam, 0x1C, void (*)(VObject *, f32, f32, f32))(cam, w[0], w[1], w[2]);
        VCALL(cam, 0x5C, void (*)(VObject *, f32))(cam, kFov.f);
    }
    AT(d, 0x74, u8) = AT(d, 0x68, u8);
    AT(d, 0x150, u8) = AT(d, 0x69, u8);
}


extern f32 func_0031C5C0(f32 x, f32 z);   /* heading of (x, z) */
extern f32 func_002E2D00(f32 angle);     /* wrapped into -pi..pi */
/* keep the target in view: the director's target (a character +0xB0's position plus +0xC0, else
 * the point +0xA0) against where the camera looks (+0x2C) from its eye (+0x20); beyond 5 degrees
 * up / down or 10 across, the camera turns (+0x3C) `rate` percent of the rest of the way */
void func_002243D0(u8 *o, s32 rate) {
    static const union { u32 u; f32 f; } k5deg = {0x3DB2B8C3}, k10deg = {0x3E32B8C3};
    VObject *cam;
    f32 t[4] __attribute__((aligned(16)));
    f32 eye[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 ht, hd, a, b;

    if (AT(o, 0xB0, u8 *) != NULL) {
        sceVu0AddVector(t, (f32 *)(AT(o, 0xB0, u8 *) + 0x10), (f32 *)(o + 0xC0));
    } else {
        sceVu0CopyVector(t, (f32 *)(o + 0xA0));
    }
    cam = gCamera;
    VCALL(cam, 0x20, void (*)(VObject *, f32 *))(cam, eye);
    VCALL(cam, 0x2C, void (*)(VObject *, f32 *))(cam, d);
    sceVu0SubVector(d, d, eye);
    sceVu0SubVector(t, t, eye);
    ht = __builtin_sqrtf(__builtin_fabsf(t[2] * t[2] + t[0] * t[0]));
    hd = __builtin_sqrtf(__builtin_fabsf(d[2] * d[2] + d[0] * d[0]));
    a = func_0031C5C0(t[1], ht);
    b = -func_002E2D00(a - func_0031C5C0(d[1], hd));
    if (!((b <= 0.0f ? -b : b) <= k5deg.f)) {
        b = !(b <= 0.0f) ? b - k5deg.f : b + k5deg.f;
        VCALL(cam, 0x3C, void (*)(VObject *, f32, f32))(cam, b * (f32)rate / 100.0f, 0.0f);
    }
    a = func_0031C5C0(t[0], t[2]);
    b = func_002E2D00(a - func_0031C5C0(d[0], d[2]));
    if ((b <= 0.0f ? -b : b) <= k10deg.f) {
        return;
    }
    b = !(b <= 0.0f) ? b - k10deg.f : b + k10deg.f;
    VCALL(cam, 0x3C, void (*)(VObject *, f32, f32))(cam, 0.0f, b * (f32)rate / 100.0f);
}

/* ---- the camera director's (D_0046C668 at +0x60) small methods (2026-10-05) ---- */

extern void func_00100490(void *p);   /* operator delete */
extern void *D_0046C660[], *D_0046C668[], *D_0046C6F0[];
extern void *gCamDirector;
extern VObject *gCamera;   /* the camera */
extern VObject *gCutscene;   /* the cutscene director */

/* destructor: its two vtables (+0x64, +0x60), the global gCamDirector cleared, its state reset
 * (+0x2C / +0x38 0, +0x50 6, the spline +0x8..+0x20 cleared) */
void *func_00223E40(u8 *d, s32 flags) {
    if (d != NULL) {
        AT(d, 0x64, void **) = D_0046C660;
        AT(d, 0x60, void **) = D_0046C668;
        AT(d, 0x60, void **) = D_0046C6F0;
        gCamDirector = NULL;
        AT(d, 0x2C, s32) = 0;
        AT(d, 0x38, s32) = 0;
        AT(d, 0x50, f32) = 6.0f;
        AT(d, 0x8, s32) = 0;
        AT(d, 0xC, s32) = 0;
        AT(d, 0x10, s32) = 0;
        AT(d, 0x14, s32) = 0;
        AT(d, 0x18, s32) = 0;
        AT(d, 0x1C, s32) = 0;
        AT(d, 0x20, s32) = 0;
        if ((s16)flags > 0) {
            func_00100490(d);
        }
    }
    return d;
}

/* on to the next of the camera's set-ups (+0x6C, wrapping at the camera's +0x80 count); no
 * path (+0x70 -1), +0xB0 0 */
void func_00223F00(u8 *d) {
    u32 n = VCALL(gCamera, 0x80, u32 (*)(VObject *))(gCamera);

    AT(d, 0x6C, u32) += 1;
    if (!(AT(d, 0x6C, u32) < n)) {
        AT(d, 0x6C, u32) = 0;
    }
    AT(d, 0x70, s32) = -1;
    AT(d, 0xB0, s32) = 0;
}

/* wrapped into -180..180 degrees */
static inline f32 camdeg_wrap(f32 a) {
    if (a < -180.0f) {
        a = a + 360.0f;
    }
    if (!(a <= 180.0f)) {
        a = a - 360.0f;
    }
    return a;
}

/* the event camera's frame (its block at the director's +0x100, set by func_00224190): the
 * camera on an orbit - p[0] the distance, pitch p[1] + p[3] and yaw p[2] + p[4] (in
 * degrees) from the point p + 0x20 looked at; a 60 degree view */
void func_00223F70(f32 *p) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 pitch = camdeg_wrap(p[1] + p[3]);
    f32 yaw = camdeg_wrap(p[2] + p[4]);
    VObject *cam;

    v[2] = -1.0f;
    v[3] = 1.0f;
    v[0] = 0.0f;
    v[1] = 0.0f;
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixX(m, m, 0x1.921fb6p+1f /* pi */ * pitch / 180.0f);
    sceVu0RotMatrixY(m, m, 0x1.921fb6p+1f /* pi */ * yaw / 180.0f);
    sceVu0ApplyMatrix(v, m, v);
    sceVu0ScaleVector(v, v, p[0]);
    cam = gCamera;
    VCALL(cam, 0x28, void (*)(VObject *, f32, f32, f32))(cam, p[8], p[9], p[10]);
    sceVu0AddVector(v, p + 8, v);
    VCALL(cam, 0x1C, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
    VCALL(cam, 0x5C, void (*)(VObject *, f32))(cam, 0x1.0c1524p+0f /* 60 degrees */);
}

/* set the event camera's block (+0x100): distance, pitch, the yaw +0x110, and the vector
 * +0x130 = (0, e, 0, 1) */
void func_00224190(u8 *d, f32 a, f32 b, f32 c, f32 e) {
    AT(d, 0x100, f32) = a;
    AT(d, 0x104, f32) = b;
    AT(d, 0x110, f32) = c;
    AT(d, 0x138, s32) = 0;
    AT(d, 0x130, s32) = 0;
    AT(d, 0x13C, f32) = 1.0f;
    AT(d, 0x134, f32) = e;
}

/* the path's length (+0x8), -1 with no path (+0x70) */
f32 func_00224680(u8 *d) {
    if (AT(d, 0x70, s32) == -1) {
        return -1.0f;
    }
    return AT(d, 0x8, f32);
}

/* the path (+0x8) to u, if there is one */
void func_002246B0(u8 *d, f32 u) {
    if (AT(d, 0x70, s32) != -1) {
        func_0025F6A0(d + 0x8, u);
    }
}

/* remember the set-up and path (+0x6C / +0x70 into +0x78 / +0x7C), then the cutscene
 * director's letterbox (+0x64) */
void func_00224740(u8 *d) {
    AT(d, 0x78, s32) = AT(d, 0x6C, s32);
    AT(d, 0x7C, s32) = AT(d, 0x70, s32);
    VCALL(gCutscene, 0x64, void (*)(VObject *))(gCutscene);
}
