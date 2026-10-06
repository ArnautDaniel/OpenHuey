/* Camera (Game +0x14D9B00, global gCamera, vtable Camera_vtable; base vtable D_00469B40). The
 * view comes from an eye and a target point; matrices are built by the larger methods.
 *
 * (was camctl.c) The camera director (SceneGame +0xF6CBB0, second vtable at +0x64): drives the
 * camera (gCamera) through the room's camera setups.
 */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "progress.h"
#include "memcard.h"
#include "navmesh.h"
#include "hewie.h"
#include "vecmath.h"
#include "msl.h"
#include "camera.h"
#include "actor.h"
#include "doors.h"
#include "scene_game.h"
#include "sound.h"
#include "pursuer.h"
#include "charaction.h"
#include "input.h"
#include "gl2d.h"
#include "effects.h"
#include "fiona.h"
#include "model.h"
#include "renderer.h"
#include "heap.h"
#include "creature.h"
#include "effectmgr.h"
#include "daniella.h"
#include "loader.h"
#include "pad.h"
#include "system.h"
#include "cri/adx.h"
#include "libc.h"
#include "sce/eekernel.h"
#include "sce/intc.h"
#include "sce/iop.h"
#include "sce/libmc.h"
#include "sce/libpad2.h"
#include "sce/sif.h"
#include "scene.h"
#include "scene_boot.h"
#include "scene_title.h"
#include "text.h"
#include "ps2hw.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif

typedef struct Camera {
    /* 0x000 */ void **vtbl;
    /* 0x004 */ f32 unk4;
    /* 0x008 */ u32 pad8;
    /* 0x00C */ f32 nearZ;
    /* 0x010 */ f32 farZ;
    /* 0x014 */ f32 fov;          /* radians (60 degrees) */
    /* 0x018 */ f32 zMin;         /* Z buffer range */
    /* 0x01C */ f32 zMax;
    /* 0x020 */ f32 centerX;      /* screen centre, GS units */
    /* 0x024 */ f32 centerY;
    /* 0x028 */ f32 unk28;
    /* 0x02C */ f32 unk2C;
    /* 0x030 */ f32 roll;
    /* 0x034 */ u8 pad34[0xC];
    /* 0x040 */ f32 eye[4];
    /* 0x050 */ f32 target[4];
    /* 0x060 */ f32 unk60[4];
    /* 0x070 */ f32 unk70[4];
    /* 0x080 */ f32 unk80[4];
    /* 0x090 */ f32 viewScreen[4][4];
    /* 0x0D0 */ f32 view[4][4];   /* its columns are the camera axes */
    /* 0x110 */ f32 clip[4][4];     /* screen to clip space (-1..1) */
    /* 0x150 */ f32 unk150[4][4];
    /* 0x190 */ f32 unk190[4][4];
    /* 0x1D0 */ f32 unk1D0[4][4];
    /* 0x210 */ f32 unk210[4][4];
    /* 0x250 */ f32 unk250[4][4];
    /* 0x290 */ s32 *path;        /* 0x20-byte entries ending with -1 */
    /* 0x294 */ s32 unk294;
    /* 0x298 */ s32 unk298;
    /* 0x29C */ s32 unk29C;
    /* 0x2A0 */ u8 unk2A0[1];
} Camera;

_Static_assert(__builtin_offsetof(Camera, view) == 0xD0, "Camera.view");
_Static_assert(__builtin_offsetof(Camera, path) == 0x290, "Camera.path");

/* a camera preset (+0x88) */
typedef struct CameraSet {
    /* 0x00 */ f32 eye[4];
    /* 0x10 */ f32 target[4];
    /* 0x20 */ f32 fov;
    /* 0x24 */ s32 unk24;
} CameraSet;

extern void *Camera_vtable[];
extern void *D_00469B40[];

#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

s32 Camera_MtxTranslation(void *p, f32 a, f32 b);

void *CameraBase_dtor(u8 *o, s32 flags);

extern const PTMF D_003E5240;  /* { 0, -1, CamDirector_ModeNormal } */
extern const PTMF CamDirector_Remember_ptmf;  /* { 0, -1, CamDirector_Remember } */
extern void CamDirector_TakeData(u8 *d, s32 *data, s32 setup, f32 t);
extern void CamDirector_LoadSetup(u8 *d, s32 setup, f32 t);
extern f32 CamPath_Nearest(u8 *d, s32 mode, f32 t);
extern void CamPath_LookAt(u8 *d, f32 *out, f32 t);   /* the look-at point at t */
extern void CamPath_Eye(u8 *d, f32 *out, f32 t);   /* the eye at t */
extern void CamDirector_KeepInView(u8 *d, s32 n);
extern f32 CamPath_Move(u8 *d, f32 t, f32 u);
extern f32 D_0047E410, D_0047E418;   /* the right stick, x and y (-1..1) */
void EventCam_Frame(f32 *p);           /* the event camera's frame */
extern void *D_0046C660[], *CamDirector_vtable[], *D_0046C6F0[];
void CamDirector_Set50(u8 *o, f32 v);

extern void *DepthRange_vtable[];
void CamDirector_HoldEffect1C(u8 *o, s32 on);

void *Camera_ctor(VObject *o);

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

void CamDirector_NewRoom(u8 *d);
s32 CamDirector_Setup(u8 *d);
void CamDirector_ModeEvent(u8 *d);
void CamDirector_ModeDefault(u8 *d);
void CamDirector_Follow(u8 *f, u8 *target, f32 x, f32 y, f32 z);
void CamDirector_SetSetup(u8 *f, s32 a, s32 b);
s32 CamDirector_FreeMode(u8 *d);
void CamDirector_RoomStart(u8 *d, s32 target);
void CamDirector_Restart(u8 *d);
s32 CamDirector_SetupChanged(u8 *d);
void CamDirector_TakeData(u8 *d, s32 *data, s32 setup, f32 t);
void CamDirector_LoadSetup(u8 *d, s32 setup, f32 t);
f32 CamPath_Nearest(u8 *d, s32 mode, f32 t);
void CamPath_LookAt(u8 *d, f32 *out, f32 t);
void CamPath_Eye(u8 *d, f32 *out, f32 t);
void CamDirector_Ease(u8 *d);
f32 CamPath_Move(u8 *d, f32 to, f32 from);
void CamDirector_Track(u8 *d);
void CamDirector_Update(u8 *d);
void CamDirector_ModeNormal(u8 *d);
void CamDirector_KeepInView(u8 *o, s32 rate);
void *CamDirector_dtor(u8 *d, s32 flags);
void CamDirector_NextSetup(u8 *d);
void EventCam_Frame(f32 *p);
void EventCam_Set(u8 *d, f32 a, f32 b, f32 c, f32 e);
f32 CamDirector_PathLength(u8 *d);
void CamDirector_PathSeek(u8 *d, f32 u);
void CamDirector_Remember(u8 *d);

/* +0x8 */
/* 0x001218A0 */
Camera *Camera_dtor(Camera *c, s32 flags) {
    if (c != NULL) {
        c->vtbl = Camera_vtable;
        if (c != NULL) {
            c->vtbl = D_00469B40;
            if (c != NULL) {
                gCamera = NULL;
            }
        }
        if ((s16)flags > 0) {
            func_00100490(c);
        }
    }
    return c;
}

/* +0xC defaults: 60 degree view from the origin towards +z, no path */
/* 0x00122A30 */
void Camera_Defaults(Camera *c) {
    c->nearZ = 1.0f;
    c->farZ = 2000.0f;
    c->fov = 0x1.0c1524p+0f;   /* 60 degrees */
    c->centerX = 2048.0f;
    c->centerY = 2048.0f;
    c->eye[0] = 0.0f;
    c->eye[1] = 0.0f;
    c->eye[2] = 0.0f;
    c->eye[3] = 1.0f;
    c->target[0] = 0.0f;
    c->target[1] = 0.0f;
    c->target[2] = 50.0f;
    c->target[3] = 1.0f;
    c->unk70[0] = 0.0f;
    c->unk70[1] = 0.0f;
    c->unk70[2] = 1.0f;
    c->unk70[3] = 0.0f;
    c->unk80[0] = 0.0f;
    c->unk80[1] = -1.0f;
    c->unk80[2] = 0.0f;
    c->unk80[3] = 0.0f;
    c->roll = 0.0f;
    c->unk4 = 0.0f;
    c->zMin = 0.0f;
    c->zMax = 65536.0f;
    VCALL(c, 0x78, void (*)(Camera *, s32 *))(c, NULL);
}

/* destructor (vtable D_00469B40) */
/* 0x00122AD0 */
void *CameraBase_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00469B40;
        gCamera = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* 0x0020E260 */
void *Camera_ctor(VObject *o) {
    gCamera = o;
    o->vtbl = Camera_vtable;
    return o;
}

/* move along the camera path from time `from` (below 1: the current time +0x8) towards `to`,
 * by the distance the look-at point (components 3..5) covers between them, measured in steps
 * of +0x0 and scaled by +0x50 / +0x24; clamped to `to` and the path's range +0x14..+0x18 */
/* 0x00219530 */
f32 CamPath_Move(u8 *d, f32 to, f32 from) {
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
    pts[0][0] = Spline_Eval(d + 8, 3, t);
    pts[0][1] = Spline_Eval(d + 8, 4, t);
    pts[0][2] = Spline_Eval(d + 8, 5, t);
    t += AT(d, 0x0, f32);
    while (t < end) {
        f32 *p = pts[cur & 1];
        f32 dx, dy, dz;

        p[0] = Spline_Eval(d + 8, 3, t);
        p[1] = Spline_Eval(d + 8, 4, t);
        p[2] = Spline_Eval(d + 8, 5, t);
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

/* the point of the target to keep in view: its position (+0x38) + offset (+0x40) */

/* the time along the look-at path (components 3..5) nearest the target point, by horizontal
 * distance (mode 0), height difference (1) or distance (2): the nearest key, then steps of
 * +0x0 around it; -1: no path / target */
/* 0x002197A0 */
f32 CamPath_Nearest(u8 *d, s32 mode, f32 t) {
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
    Spline_Eval(d + 8, 3, u);
    Spline_Eval(d + 8, 4, u);
    Spline_Eval(d + 8, 5, u);
    n = AT(d, 0x10, s32);
    for (k = 0; k < n; k++) {
        f32 tk = (f32)(s32)AT(d, 0xC, f32 *)[k * 8];
        f32 x = Spline_Eval(d + 8, 3, tk);
        f32 y = Spline_Eval(d + 8, 4, tk);
        f32 z = Spline_Eval(d + 8, 5, tk);
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
        f32 x = Spline_Eval(d + 8, 3, u);
        f32 y = Spline_Eval(d + 8, 4, u);
        f32 z = Spline_Eval(d + 8, 5, u);
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

/* the camera path's eye (components 0..2) at time t (below 1: the current time) */
/* 0x00219CD0 */
void CamPath_Eye(u8 *d, f32 *out, f32 t) {
    f32 u = t < 1.0f ? AT(d, 0x8, f32) : t;

    out[0] = Spline_Eval(d + 8, 0, u);
    out[1] = Spline_Eval(d + 8, 1, u);
    out[2] = Spline_Eval(d + 8, 2, u);
    out[3] = 1.0f;
}

/* the camera path's look-at point (components 3..5) at time t (below 1: the current time) */
/* 0x00219D70 */
void CamPath_LookAt(u8 *d, f32 *out, f32 t) {
    f32 u = t < 1.0f ? AT(d, 0x8, f32) : t;

    out[0] = Spline_Eval(d + 8, 3, u);
    out[1] = Spline_Eval(d + 8, 4, u);
    out[2] = Spline_Eval(d + 8, 5, u);
    out[3] = 1.0f;
}

/* setup `setup` of the room's camera data: its spline (keys after the header and the earlier
 * setups' keys) into +0x8, the lengths of its two paths (components 0..2 +0x28, 3..5 +0x24,
 * sampled at whole steps up to +0x18) and the speeds along them (+0x4, +0x0), then go to t */
/* 0x00219E10 */
void CamDirector_LoadSetup(u8 *d, s32 setup, f32 t) {
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
    Spline_Init(d + 8, data[2 + AT(d, 0x34, s32) * 2], data[3 + AT(d, 0x34, s32) * 2], keys);
    AT(d, 0x28, f32) = 0.0f;
    AT(d, 0x24, f32) = 0.0f;
    end = (f32)AT(d, 0x18, s32);
    u = keys[0];
    b[0][0] = Spline_Eval(d + 8, 3, u);
    b[0][1] = Spline_Eval(d + 8, 4, u);
    b[0][2] = Spline_Eval(d + 8, 5, u);
    a[0][0] = Spline_Eval(d + 8, 0, u);
    a[0][1] = Spline_Eval(d + 8, 1, u);
    a[0][2] = Spline_Eval(d + 8, 2, u);
    u = u + 1.0f;
    cur = 0;
    while (u < end) {
        f32 dx, dy, dz, ex, ey, ez;

        cur ^= 1;
        b[cur][0] = Spline_Eval(d + 8, 3, u);
        b[cur][1] = Spline_Eval(d + 8, 4, u);
        b[cur][2] = Spline_Eval(d + 8, 5, u);
        a[cur][0] = Spline_Eval(d + 8, 0, u);
        a[cur][1] = Spline_Eval(d + 8, 1, u);
        a[cur][2] = Spline_Eval(d + 8, 2, u);
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
    Spline_Seek(d + 8, t);
}

/* take the room's camera data (count first; none or empty: no data), then setup `setup` at t */
/* 0x0021A290 */
void CamDirector_TakeData(u8 *d, s32 *data, s32 setup, f32 t) {
    AT(d, 0x2C, s32 *) = data;
    if (data == NULL) {
        return;
    }
    AT(d, 0x30, s32) = AT(d, 0x2C, s32 *)[0];
    if (AT(d, 0x30, s32) <= 0) {
        AT(d, 0x2C, s32) = 0;
    }
    CamDirector_LoadSetup(d, setup, t);
}

/* (CamDirector_vtable) +0x50 = v, 6 if below 0 */
/* 0x00223DE0 */
void CamDirector_Set50(u8 *o, f32 v) {
    if (v < 0.0f) {
        AT(o, 0x50, f32) = 6.0f;
        return;
    }
    AT(o, 0x50, f32) = v;
}

/* +0x6C the mode flag +0xF4 */
/* 0x00223E20 */
s32 CamDirector_FreeMode(u8 *d) {
    return AT(d, 0xF4, u8);
}

/* (interface) +0x28 the camera setup to use (+0x6C, +0x70) */
/* 0x00223E30 */
void CamDirector_SetSetup(u8 *f, s32 a, s32 b) {
    AT(f, 0x6C, s32) = a;
    AT(f, 0x70, s32) = b;
}

/* destructor: its two vtables (+0x64, +0x60), the global gCamDirector cleared, its state reset
 * (+0x2C / +0x38 0, +0x50 6, the spline +0x8..+0x20 cleared) */
/* 0x00223E40 */
void *CamDirector_dtor(u8 *d, s32 flags) {
    if (d != NULL) {
        AT(d, 0x64, void **) = D_0046C660;
        AT(d, 0x60, void **) = CamDirector_vtable;
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
/* 0x00223F00 */
void CamDirector_NextSetup(u8 *d) {
    u32 n = VCALL(gCamera, 0x80, u32 (*)(VObject *))(gCamera);

    AT(d, 0x6C, u32) += 1;
    if (!(AT(d, 0x6C, u32) < n)) {
        AT(d, 0x6C, u32) = 0;
    }
    AT(d, 0x70, s32) = -1;
    AT(d, 0xB0, s32) = 0;
}

/* the event camera's frame (its block at the director's +0x100, set by EventCam_Set): the
 * camera on an orbit - p[0] the distance, pitch p[1] + p[3] and yaw p[2] + p[4] (in
 * degrees) from the point p + 0x20 looked at; a 60 degree view */
/* 0x00223F70 */
void EventCam_Frame(f32 *p) {
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
/* 0x00224190 */
void EventCam_Set(u8 *d, f32 a, f32 b, f32 c, f32 e) {
    AT(d, 0x100, f32) = a;
    AT(d, 0x104, f32) = b;
    AT(d, 0x110, f32) = c;
    AT(d, 0x138, s32) = 0;
    AT(d, 0x130, s32) = 0;
    AT(d, 0x13C, f32) = 1.0f;
    AT(d, 0x134, f32) = e;
}

/* hold (`on`) or give back screen effect 0x1C (+0x69): held, its colour (+0x50..+0x5C) is
 * kept at +0x154 and the effect removed; given back, a new one (DepthRange_vtable, from the effects'
 * heap +0x1400, at +0x14A8) is started with that colour */
/* 0x002241C0 */
void CamDirector_HoldEffect1C(u8 *o, s32 on) {
    u8 *fx;
    u8 *e;
    void *mem;

    AT(o, 0x69, u8) = on;
    if (AT(o, 0x69, u8) == 0) {
        if (AT(o, 0x164, u8) == 0) {
            return;
        }
        AT(o, 0x164, u8) = 0;
        fx = (u8 *)gRoomEffects;
        if (AT(fx, 0x14A8, void *) != NULL) {
            VCALL(fx + 0x1400, 0x14, void (*)(void *, void *))(fx + 0x1400, AT(fx, 0x14A8, void *));
            AT(fx, 0x14A8, void *) = NULL;
        }
        mem = VCALL(fx + 0x1400, 0x10, void *(*)(void *, s32))(fx + 0x1400, 0xA0);
        if (mem != NULL) {
            e = RoomEffects_new(0xA0, mem);
            if (e != NULL) {
                AT(e, 0x0, void **) = DepthRange_vtable;
            }
            AT(fx, 0x14A8, u8 *) = e;
            VCALL(AT(fx, 0x14A8, u8 *), 0xC, void (*)(u8 *))(AT(fx, 0x14A8, u8 *));
        }
        RoomEffects_Send(fx, 0x1C, o + 0x154);
        return;
    }
    fx = (u8 *)gRoomEffects;
    e = RoomEffects_Get(fx, 0x1C);
    if (e != NULL) {
        AT(o, 0x164, u8) = 1;
        AT(o, 0x154, f32) = AT(e, 0x50, f32);
        AT(o, 0x158, f32) = AT(e, 0x54, f32);
        AT(o, 0x15C, f32) = AT(e, 0x58, f32);
        AT(o, 0x160, f32) = AT(e, 0x5C, f32);
        RoomEffects_Release(fx, 0x1C);
    }
}

/* vt+0x24 the current camera setup (+0x6C); -1 in the free mode (+0xF4) */
/* 0x00224300 */
s32 CamDirector_Setup(u8 *d) {
    if (AT(d, 0xF4, u8) == 1) {
        return -1;
    }
    return AT(d, 0x6C, s32);
}

/* two modes of the director (update state +0xE8, flag +0xF4); both clear the renderer's
 * work area (+0x5C) */
/* 0x00224330 */
void CamDirector_ModeEvent(u8 *d) {
    AT(d, 0xE8, PTMF) = CamDirector_Remember_ptmf;
    AT(d, 0xF4, u8) = 1;
    AT(d, 0x7C, s32) = -1;
    AT(d, 0x78, s32) = -1;
    VCALL(gRenderer, 0x5C, s32 (*)(VObject *))(gRenderer);
}

/* vt+0x78 (the default mode) */
/* 0x00224380 */
void CamDirector_ModeDefault(u8 *d) {
    AT(d, 0xE8, PTMF) = D_003E5240;
    AT(d, 0xF4, u8) = 0;
    AT(d, 0x7C, s32) = -1;
    AT(d, 0x78, s32) = -1;
    VCALL(gRenderer, 0x5C, s32 (*)(VObject *))(gRenderer);
}

/* keep the target in view: the director's target (a character +0xB0's position plus +0xC0, else
 * the point +0xA0) against where the camera looks (+0x2C) from its eye (+0x20); beyond 5 degrees
 * up / down or 10 across, the camera turns (+0x3C) `rate` percent of the rest of the way */
/* 0x002243D0 */
void CamDirector_KeepInView(u8 *o, s32 rate) {
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
    b = -Angle_Wrap(a - func_0031C5C0(d[1], hd));
    if (!((b <= 0.0f ? -b : b) <= k5deg.f)) {
        b = !(b <= 0.0f) ? b - k5deg.f : b + k5deg.f;
        VCALL(cam, 0x3C, void (*)(VObject *, f32, f32))(cam, b * (f32)rate / 100.0f, 0.0f);
    }
    a = func_0031C5C0(t[0], t[2]);
    b = Angle_Wrap(a - func_0031C5C0(d[0], d[2]));
    if ((b <= 0.0f ? -b : b) <= k10deg.f) {
        return;
    }
    b = !(b <= 0.0f) ? b - k10deg.f : b + k10deg.f;
    VCALL(cam, 0x3C, void (*)(VObject *, f32, f32))(cam, 0.0f, b * (f32)rate / 100.0f);
}

/* the setup changed since it was taken (+0x78 / +0x7C against +0x6C / +0x70) */
/* 0x00224650 */
s32 CamDirector_SetupChanged(u8 *d) {
    return !(AT(d, 0x78, s32) == AT(d, 0x6C, s32) && AT(d, 0x7C, s32) == AT(d, 0x70, s32));
}

/* the path's length (+0x8), -1 with no path (+0x70) */
/* 0x00224680 */
f32 CamDirector_PathLength(u8 *d) {
    if (AT(d, 0x70, s32) == -1) {
        return -1.0f;
    }
    return AT(d, 0x8, f32);
}

/* the path (+0x8) to u, if there is one */
/* 0x002246B0 */
void CamDirector_PathSeek(u8 *d, f32 u) {
    if (AT(d, 0x70, s32) != -1) {
        Spline_Seek(d + 0x8, u);
    }
}

/* (the director's interface, gCamDirector) +0xC follow `target` (its position, +0x10) with an
 * offset (x, y, z) */
/* 0x002246F0 */
void CamDirector_Follow(u8 *f, u8 *target, f32 x, f32 y, f32 z) {
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

/* remember the set-up and path (+0x6C / +0x70 into +0x78 / +0x7C), then the cutscene
 * director's letterbox (+0x64) */
/* 0x00224740 */
void CamDirector_Remember(u8 *d) {
    AT(d, 0x78, s32) = AT(d, 0x6C, s32);
    AT(d, 0x7C, s32) = AT(d, 0x70, s32);
    VCALL(gCutscene, 0x64, void (*)(VObject *))(gCutscene);
}

/* the director's normal mode (+0xE8), each frame. The free camera (+0x68, e.g. debug):
 * orbits the target (+0xB0) with the right stick (yaw +0xE4 in degrees, distance +0xE0 >= 5).
 * An event camera (+0x69): started on the target the first frame, then run. Otherwise the
 * camera follows the current setup's path. The last set / setup / target are kept
 * (+0x78 / +0x7C / +0xB4) for the switch test; changing mode forces it. */
/* 0x00224770 */
void CamDirector_ModeNormal(u8 *d) {
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
            CamDirector_Track(d);
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
            EventCam_Frame((f32 *)(d + 0x100));
        } else {
            if (AT(d, 0x150, u8) != AT(d, 0x69, u8)) {
                AT(d, 0x7C, s32) = -1;
                AT(d, 0x78, s32) = -1;
                CamDirector_Track(d);
            }
            if (AT(d, 0x70, s32) != -1) {
                CamPath_LookAt(d, v, 0.0f);
                cam = gCamera;
                VCALL(cam, 0x28, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
                CamPath_Eye(d, v, 0.0f);
                VCALL(cam, 0x1C, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
            }
            VCALL(gCamera, 0x5C, void (*)(VObject *, f32))(gCamera, AT(d, 0x80, f32));
        }
    } else {
        sceVu0FMATRIX m;
        f32 w[4] __attribute__((aligned(16)));

        if (AT(d, 0xB0, u8 *) == NULL) {
            if (AT(d, 0x70, s32) != -1) {
                CamPath_LookAt(d, v, 0.0f);
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

/* each frame: the director's mode (+0xE8, a member function), then the camera's update */
/* 0x00224C20 */
void CamDirector_Update(u8 *d) {
    ptmf_scall(d, &AT(d, 0xE8, PTMF));
    VCALL(gCamera, 0x14, void (*)(VObject *))(gCamera);
}

/* each frame: when the camera set (+0x6C, last applied +0x78) changes or the target (+0xB0,
 * last +0xB4) left the view (no setup: the camera's test +0xD4; else its path time differs
 * from the current one by more than 35 units), the camera takes the set and the view angle
 * resets; when the setup (+0x70, last +0x7C) changes or on such a jump, the camera path is
 * set up again and put at the target's nearest point; the renderer is told (+0x5C) */
/* 0x00224C60 */
void CamDirector_Track(u8 *d) {
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
            f32 t = CamPath_Nearest(d, 2, 0.0f);
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
            CamDirector_KeepInView(d, 100);
        }
    }
    if (AT(d, 0x70, s32) != -1 && (AT(d, 0x7C, s32) != AT(d, 0x70, s32) || far)) {
        changed = 1;
        CamDirector_LoadSetup(d, AT(d, 0x70, s32), 1.0f);
        if (AT(d, 0xB0, u8 *) != NULL) {
            Spline_Seek(d + 8, CamPath_Nearest(d, 2, 0.0f));
            Spline_Seek(d + 8, CamPath_Move(d, CamPath_Nearest(d, 2, 0.0f), 0.0f));
        } else {
            Spline_Seek(d + 8, 1.0f);
        }
    }
    if (changed) {
        VCALL(gRenderer, 0x5C, void (*)(VObject *))(gRenderer);
    }
}

/* each frame (not while an event drives it, +0xF4 / +0x69): the view angle +0x80 eases in over
 * 60 frames (+0x90) and back out after a cut (+0x8C counts down; 0x80: settled); the camera
 * follows the current setup (+0x70) along its path (+0xB0: from the player's nearest point),
 * else a setup is chosen */
/* 0x00224EE0 */
void CamDirector_Ease(u8 *d) {
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
        CamDirector_KeepInView(d, 10);
    } else {
        VObject *cam;
        f32 v[3];

        if (AT(d, 0xB0, s32) != 0) {
            Spline_Seek(d + 8, CamPath_Move(d, CamPath_Nearest(d, 2, 0.0f), 0.0f));
        }
        CamPath_LookAt(d, v, 0.0f);
        cam = gCamera;
        VCALL(cam, 0x28, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
        CamPath_Eye(d, v, 0.0f);
        VCALL(cam, 0x1C, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
    }
}

/* the camera director restarted (renderer +0x5C; +0x8C 0x80, +0x90 60, the field of view back
 * to +0x84): with no setup (+0x70) the free camera on its target, else setup's position at
 * t 0 */
/* 0x002251C0 */
void CamDirector_Restart(u8 *d) {
    VCALL(gRenderer, 0x5C, s32 (*)(VObject *))(gRenderer);
    AT(d, 0x90, s32) = 0x3C;
    AT(d, 0x8C, u8) = 0x80;
    AT(d, 0x80, f32) = AT(d, 0x84, f32);
    if (AT(d, 0x70, s32) == -1) {
        VObject *cam = gCamera;

        VCALL(cam, 0x8C, void (*)(VObject *, s32))(cam, AT(d, 0x6C, s32));
        VCALL(cam, 0x2C, void (*)(VObject *, void *))(cam, d + 0xA0);
        CamDirector_KeepInView(d, 100);
        return;
    }
    VCALL(gCamera, 0x70, void (*)(VObject *, f32))(gCamera, 0.0f);
    Spline_Seek(d + 8, CamPath_Nearest(d, 2, 0.0f));
}

/* the room's camera at the start of play: the camera's range, its view angle (+0x84) and the
 * director's angle limits (+0x88, +0x80); with the room's camera data (PAC section 6): take it
 * and put the camera at the current setup's (+0x70) eye and look-at point; no setup: pick one */
/* 0x002252B0 */
void CamDirector_RoomStart(u8 *d, s32 target) {
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
        CamDirector_TakeData(d, (s32 *)target, 0, 1.0f);
        AT(d, 0x38, u8 *) = AT(d, 0xB0, u8 *) + 0x10;
        AT(d, 0x40, f32) = AT(d, 0xC0, f32);
        AT(d, 0x44, f32) = AT(d, 0xC4, f32);
        AT(d, 0x48, f32) = AT(d, 0xC8, f32);
        AT(d, 0x4C, f32) = 1.0f;
        if (AT(d, 0x70, s32) != -1) {
            f32 v[4];

            CamDirector_LoadSetup(d, AT(d, 0x70, s32), 1.0f);
            Spline_Seek(d + 8, CamPath_Nearest(d, 2, 0.0f));
            CamPath_LookAt(d, v, 0.0f);
            cam = gCamera;
            VCALL(cam, 0x28, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
            CamPath_Eye(d, v, 0.0f);
            VCALL(cam, 0x1C, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
        }
    }
    if (AT(d, 0x70, s32) == -1) {
        CamDirector_KeepInView(d, 100);
    }
}

/* reset for a new room: reset the camera, no setup selected, default light direction */
/* 0x00225550 */
void CamDirector_NewRoom(u8 *d) {
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

/* +0x10 reset */
/* 0x00122A20 */
void Camera_Reset(Camera *c) {
    VCALL(c, 0xC, void (*)(Camera *))(c);
}

/* +0x1C / +0x20 eye */
/* 0x001225E0 */
void Camera_SetEye(Camera *c, f32 x, f32 y, f32 z) {
    c->eye[0] = x;
    c->eye[1] = y;
    c->eye[2] = z;
}

/* 0x001225D0 */
void Camera_GetEye(Camera *c, f32 *out) { sceVu0CopyVector(out, c->eye); }

/* +0x24 */
/* 0x001225C0 */
void Camera_Get60(Camera *c, f32 *out) { sceVu0CopyVector(out, c->unk60); }

/* +0x28 / +0x2C target */
/* 0x001225B0 */
void Camera_SetTarget(Camera *c, f32 x, f32 y, f32 z) {
    c->target[0] = x;
    c->target[1] = y;
    c->target[2] = z;
}

/* 0x001225A0 */
void Camera_GetTarget(Camera *c, f32 *out) { sceVu0CopyVector(out, c->target); }

/* +0x30 move the eye, the target following (same view direction) */
/* 0x00122550 */
void Camera_MoveEye(Camera *c, f32 x, f32 y, f32 z) {
    f32 dx = x - c->eye[0], dy = y - c->eye[1], dz = z - c->eye[2];

    c->eye[0] = x;
    c->eye[1] = y;
    c->eye[2] = z;
    c->target[0] += dx;
    c->target[1] += dy;
    c->target[2] += dz;
}

/* +0x34 move the target, the eye following */
/* 0x00122500 */
void Camera_MoveTarget(Camera *c, f32 x, f32 y, f32 z) {
    f32 dx = x - c->target[0], dy = y - c->target[1], dz = z - c->target[2];

    c->target[0] = x;
    c->target[1] = y;
    c->target[2] = z;
    c->eye[0] += dx;
    c->eye[1] += dy;
    c->eye[2] += dz;
}

/* +0x44 .. +0x58 matrices */
/* 0x00122090 */
void Camera_GetMtx150(Camera *c, f32 (*out)[4]) { sceVu0CopyMatrix(out, c->unk150); }
/* 0x00122080 */
void Camera_GetMtx190(Camera *c, f32 (*out)[4]) { sceVu0CopyMatrix(out, c->unk190); }
/* 0x00122050 */
void Camera_GetMtx1D0(Camera *c, f32 (*out)[4]) { sceVu0CopyMatrix(out, c->unk1D0); }
/* 0x00122070 */
void Camera_SetMtx150(Camera *c, f32 (*m)[4]) { sceVu0CopyMatrix(c->unk150, m); }
/* 0x00122060 */
void Camera_SetMtx190(Camera *c, f32 (*m)[4]) { sceVu0CopyMatrix(c->unk190, m); }
/* 0x00122040 */
void Camera_GetMtx210(Camera *c, f32 (*out)[4]) { sceVu0CopyMatrix(out, c->unk210); }

/* +0x5C / +0x64 field of view */
/* 0x00122030 */
void Camera_SetFov(Camera *c, f32 fov) { c->fov = fov; }
/* 0x00122010 */
f32 Camera_GetFov(Camera *c) { return c->fov; }

/* +0x60 view matrix */
/* 0x00122020 */
void Camera_GetView(Camera *c, f32 (*out)[4]) { sceVu0CopyMatrix(out, c->view); }

/* +0x68 heading of the view direction */
/* 0x00122000 */
f32 Camera_ViewHeading(Camera *c) { return func_0031C5C0(c->view[0][2], c->view[2][2]); }

/* +0x6C */
/* 0x00121FF0 */
void Camera_Set4(Camera *c, f32 v) { c->unk4 = v; }

/* +0x70 / +0x74 roll */
/* 0x00121910 */
void Camera_SetRoll(Camera *c, f32 v) { c->roll = v; }
/* 0x00121920 */
f32 Camera_GetRoll(Camera *c) { return c->roll; }

/* +0x78 follow path `path` (NULL: none) from its start */
/* 0x00121FD0 */
void Camera_FollowPath(Camera *c, s32 *path) {
    c->path = path;
    c->unk294 = -1;
    c->unk298 = -1;
}

/* +0x7C */
/* 0x00121FC0 */
void Camera_NoPath(Camera *c) { c->path = NULL; }

/* +0x80 number of path entries */
/* 0x00121F80 */
s32 Camera_PathCount(Camera *c) {
    s32 *e = c->path;
    s32 n = 0;

    if (e == NULL) {
        return 0;
    }
    while (*e != -1) {
        e += 8;
        n++;
    }
    return n;
}

/* +0x88 apply a preset */
/* 0x00121D00 */
void Camera_ApplyPreset(Camera *c, CameraSet *s) {
    VCALL(c, 0x1C, void (*)(Camera *, f32, f32, f32))(c, s->eye[0], s->eye[1], s->eye[2]);
    VCALL(c, 0x28, void (*)(Camera *, f32, f32, f32))(c, s->target[0], s->target[1], s->target[2]);
    VCALL(c, 0x5C, void (*)(Camera *, f32))(c, s->fov);
    c->unk29C = s->unk24;
    VCALL(c, 0x70, void (*)(Camera *, f32))(c, 0.0f);
}

/* +0x8C the preset +0x84 makes from `arg`, applied */
/* 0x00121CB0 */
void Camera_ApplyPresetArg(Camera *c, s32 arg) {
    CameraSet s;

    VCALL(c, 0x84, void (*)(Camera *, s32, CameraSet *))(c, arg, &s);
    VCALL(c, 0x88, void (*)(Camera *, CameraSet *))(c, &s);
}

/* +0x90 .. +0x9C */
/* 0x00121930 */
s32 Camera_Get294(Camera *c) { return c->unk294; }
/* 0x00121940 */
s32 Camera_Get298(Camera *c) { return c->unk298; }
/* 0x00121950 */
s32 *Camera_GetPath(Camera *c) { return c->path; }
/* 0x00121CA0 */
u8 *Camera_CurrentPreset(Camera *c) { return c->unk2A0; }

/* +0xA0 */
/* 0x00121960 */
void Camera_GetDir(Camera *c, f32 *out) { sceVu0CopyVector(out, c->unk70); }

/* +0xA4 the view matrix's second column (an axis), w 0 */
/* 0x00121970 */
void Camera_ViewAxis(Camera *c, f32 *out) {
    out[0] = c->view[0][1];
    out[1] = c->view[1][1];
    out[2] = c->view[2][1];
    out[3] = 0.0f;
}

/* +0xA8 .. +0xB4 depth ranges */
/* 0x00121990 */
void Camera_SetZMax(Camera *c, f32 v) { c->zMax = v; }
/* 0x001219A0 */
void Camera_SetZMin(Camera *c, f32 v) { c->zMin = v; }
/* 0x001219B0 */
void Camera_SetNear(Camera *c, f32 v) { c->nearZ = v; }
/* 0x001219C0 */
void Camera_SetFar(Camera *c, f32 v) { c->farZ = v; }

/* +0xB8 the Z buffer value of view depth `d` (zMin at nearZ, zMax at farZ, 1/d in between) */
/* 0x00121C50 */
s32 Camera_ZValue(Camera *c, f32 d) {
    f32 n = c->nearZ, f = c->farZ;

    return (s32)(n * f * (c->zMax - c->zMin) / (f - n) / d - (c->zMax * n - c->zMin * f) / (f - n));
}

/* +0xC0 / +0xC4 */
/* 0x00121B30 */
void Camera_Set28(Camera *c, f32 a, f32 b) {
    c->unk28 = a;
    c->unk2C = b;
}

/* Copy the translation column of a matrix (rows at +0xC4) into a vec4 (w = 0). */
/* 0x00121B40 */
s32 Camera_MtxTranslation(void *p, f32 a, f32 b) {
    f32 f10, fc, d, lo, hi;
    s32 n;

    if (a == b) {
        return 0;
    }
    f10 = FLD(p, 0x10, f32);
    fc = FLD(p, 0xC, f32);
    if (f10 == b) {
        n = 0;
    } else {
        n = (s32)(((a * (f10 - b)) / f10) / (b - a)) + 1;
    }
    d = b - a;
    lo = 65536.0f * ((f32)n - ((a * (f10 - b)) / f10) / d);
    hi = 65536.0f * ((f32)n + ((a * (b - fc)) / fc) / d);
    if (lo < 0.0f || !(lo <= 16777215.0f)) {
        return 0;
    }
    if (hi < 0.0f || !(hi <= 16777215.0f)) {
        return 0;
    }
    if (hi - lo < 65536.0f) {
        return 0;
    }
    FLD(p, 0x18, f32) = lo;
    FLD(p, 0x1C, f32) = hi;
    FLD(p, 0x28, f32) = a;
    FLD(p, 0x2C, f32) = b;
    return 1;
}

/* 0x001219D0 */
void Camera_SetCentre(Camera *c, f32 x, f32 y) {
    c->centerX = x;
    c->centerY = y;
}

/* +0xC8 */
/* 0x00121B20 */
void Camera_GetViewScreen(Camera *c, f32 (*out)[4]) { sceVu0CopyMatrix(out, c->viewScreen); }

/* +0xCC / +0xD0 */
/* 0x001219E0 */
f32 Camera_GetNear(Camera *c) { return c->nearZ; }
/* 0x001219F0 */
f32 Camera_GetFar(Camera *c) { return c->farZ; }

/* +0x14 update the matrices: world to view (+0x18), then the view-to-screen matrices (the
 * screen's and a half-size one for offscreen work) and the clip matrices from them */
/* 0x00122810 */
void Camera_Update(Camera *c) {
    f32 half[4][4] __attribute__((aligned(16)));
    f32 clip2[4][4] __attribute__((aligned(16)));

    VCALL(c, 0x18, void (*)(Camera *, f32 (*)[4]))(c, c->view);
    VCALL(c, 0xBC, void (*)(Camera *, f32, f32))(c, c->unk28, c->unk2C);
    sceVu0ViewScreenMatrix(c->viewScreen, AT(c, 0x8, f32), 0x1.6db6dcp+7f, 0x1.aaaaacp+7f, c->centerX, c->centerY,
                           c->zMin, c->zMax, c->nearZ, c->farZ);
    sceVu0ViewScreenMatrix(half, AT(c, 0x8, f32), 0x1.6db6dcp+6f, 0x1.aaaaacp+6f, c->centerX, c->centerY,
                           c->zMin, c->zMax, c->nearZ, c->farZ);
    sceVu0MulMatrix(c->unk150, c->viewScreen, c->view);
    sceVu0MulMatrix(c->unk1D0, half, c->view);

    /* screen (GS units around 2048) to -1..1 */
    sceVu0UnitMatrix(c->clip);
    AT(c->clip, 0x00, u32) = 0x3A001002;   /* 1 / 2047 */
    AT(c->clip, 0x14, u32) = 0x3A001002;
    c->clip[2][2] = 2.0f / (c->zMax - c->zMin);
    AT(c->clip, 0x30, u32) = 0xBF801002;   /* -2048 / 2047 */
    AT(c->clip, 0x34, u32) = 0xBF801002;
    c->clip[3][2] = -(c->zMax + c->zMin) / (c->zMax - c->zMin);
    sceVu0MulMatrix(c->unk190, c->clip, c->unk150);
    sceVu0MulMatrix(c->unk210, c->clip, c->unk1D0);

    sceVu0UnitMatrix(clip2);
    clip2[0][0] = 0x1.0p-8f;          /* 1 / 256 */
    clip2[1][1] = 0x1.24924ap-8f;     /* 1 / 224 */
    clip2[3][0] = -8.0f;
    clip2[3][1] = -0x1.24924ap+3f;
    clip2[2][2] = 2.0f / (c->zMax - c->zMin);
    clip2[3][2] = -(c->zMax + c->zMin) / (c->zMax - c->zMin);
    VCALL(c, 0x44, void (*)(Camera *, f32 (*)[4]))(c, c->unk250);
    sceVu0MulMatrix(c->unk250, clip2, c->unk250);
    c->unk298 = c->unk294;
    c->unk294 = c->unk29C;
}

#define RAND01() VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom)

/* +0x18 the world-to-view matrix `out`: the direction to the target (+0x70), the screen
 * distance for the field of view (+0x8 = 1.4 / tan(fov / 2)), the eye shaken by +0x4 at random
 * (+0x60), and the up vector rolled by +0x30 about the direction (+0x80) */
/* 0x001225F0 */
void Camera_ViewMatrix(Camera *c, f32 (*out)[4]) {
    f32 rot[4][4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));
    f32 s[4] __attribute__((aligned(16)));

    c->unk70[0] = c->target[0] - c->eye[0];
    c->unk70[1] = c->target[1] - c->eye[1];
    c->unk70[2] = c->target[2] - c->eye[2];
    sceVu0Normalize(c->unk70, c->unk70);
    AT(c, 0x8, f32) = 0x1.666666p+0f / func_0031C338(0.5f * c->fov);
    if (c->unk4 == 0.0f) {
        s[0] = 0.0f;
        s[2] = 0.0f;
        s[1] = 0.0f;
    } else {
        s[0] = RAND01();
        s[1] = RAND01();
        s[2] = RAND01();
        s[0] = 2.0f * (s[0] - 0.5f) * c->unk4;
        s[1] = 2.0f * (s[1] - 0.5f) * c->unk4;
        s[2] = 2.0f * (s[2] - 0.5f) * c->unk4;
    }
    c->unk60[0] = c->eye[0] + s[0];
    c->unk60[1] = c->eye[1] + s[1];
    c->unk60[2] = c->eye[2] + s[2];
    c->unk60[3] = 1.0f;
    /* up: (0, -1, 0) */
    s[1] = -1.0f;
    s[2] = 0.0f;
    s[0] = 0.0f;
    s[3] = 0.0f;
    q[3] = 0.0f;
    q[2] = 0.0f;
    q[1] = 0.0f;
    q[0] = 0.0f;
    Quat_FromAxisAngle(q, c->unk70, c->roll);
    /* the translation row is left unset (the original's stack garbage, times w = 0); cleared so
     * that garbage can't be a NaN / infinity on PC */
    rot[3][0] = 0.0f;
    rot[3][1] = 0.0f;
    rot[3][2] = 0.0f;
    Quat_ToMatrix(q, rot);
    sceVu0ApplyMatrix(c->unk80, rot, s);
    sceVu0CameraMatrix(out, c->unk60, c->unk70, c->unk80);
}

/* The room's camera presets (PAC section 5): 0x20-byte entries {eye x, y, z, view angle in
 * degrees (0: 60), target x, y, z, w}; the list ends with x = -1 as an int. */
static const union { u32 u; f32 f; } sFov60 = {0x3F860A92};   /* 60 degrees */

/* +0x84 preset `n` into `out` and note it as the current one (+0x2A0); `out` keeps the
 * defaults if there is no such preset */
/* 0x00121D90 */
void Camera_Preset(Camera *c, u32 n, CameraSet *out) {
    static const union { u32 u; f32 f; } k2Pi = {0x40C90FDB};
    s32 *e;
    u32 i;

    if (out == NULL) {
        return;
    }
    out->eye[0] = 0.0f;
    out->eye[1] = 30.0f;
    out->eye[2] = 150.0f;
    out->eye[3] = 1.0f;
    out->fov = sFov60.f;
    out->target[0] = 0.0f;
    out->target[1] = 0.0f;
    out->target[2] = 0.0f;
    out->target[3] = 1.0f;
    if (n >= VCALL(c, 0x80, u32 (*)(Camera *))(c)) {
        return;
    }
    e = c->path;
    if (e == NULL) {
        return;
    }
    for (i = 0; e[0] != -1; i++, e += 8) {
        f32 *f = (f32 *)e;

        if (i != n) {
            continue;
        }
        out->unk24 = n;
        out->eye[0] = f[0];
        out->eye[1] = f[1];
        out->eye[2] = f[2];
        if (f[3] == 0.0f) {
            out->fov = sFov60.f;
        } else {
            out->fov = k2Pi.f * f[3] / 360.0f;
        }
        out->target[0] = f[4];
        out->target[1] = f[5];
        out->target[2] = f[6];
        out->target[3] = f[7];
        AT(c, 0x2A0, f32) = out->eye[0];
        AT(c, 0x2A4, f32) = out->eye[1];
        AT(c, 0x2A8, f32) = out->eye[2];
        AT(c, 0x2AC, f32) = out->eye[3];
        AT(c, 0x2B0, f32) = out->target[0];
        AT(c, 0x2B4, f32) = out->target[1];
        AT(c, 0x2B8, f32) = out->target[2];
        AT(c, 0x2BC, f32) = out->target[3];
        AT(c, 0x2C0, f32) = out->fov;
        AT(c, 0x2C4, s32) = out->unk24;
        return;
    }
}

/* +0x3C turn the view: the point looked at (+0x50) goes round the eye (+0x40) by `yaw` about Y,
 * then by `pitch` about the axis square to the up (+0x80) and the view */
/* 0x001221E0 */
void Camera_TurnView(Camera *c, f32 pitch, f32 yaw) {
    f32 r[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));

    q[3] = 0.0f;
    q[2] = 0.0f;
    q[1] = 0.0f;
    q[0] = 0.0f;
    if (yaw != 0.0f) {
        sceVu0UnitMatrix(r);
        sceVu0RotMatrixY(r, r, Angle_Wrap(yaw));
        sceVu0SubVector(v, c->target, c->eye);
        v[3] = 1.0f;
        sceVu0ApplyMatrix(v, r, v);
        c->target[0] = v[0] + c->eye[0];
        c->target[1] = v[1] + c->eye[1];
        c->target[2] = v[2] + c->eye[2];
    }
    if (pitch == 0.0f) {
        return;
    }
    sceVu0SubVector(v, c->target, c->eye);
    sceVu0OuterProduct(v, c->unk80, v);
    sceVu0Normalize(v, v);
    Quat_FromAxisAngle(q, v, pitch);
#ifdef HG_NATIVE
    if (yaw == 0.0f) {
        r[3][0] = r[3][1] = r[3][2] = 0.0f;   /* (unset in the original then; no NaN on PC) */
        r[3][3] = 1.0f;
    }
#endif
    Quat_ToMatrix(q, r);
    sceVu0SubVector(v, c->target, c->eye);
    sceVu0ApplyMatrix(v, r, v);
    c->target[0] = v[0] + c->eye[0];
    c->target[1] = v[1] + c->eye[1];
    c->target[2] = v[2] + c->eye[2];
}

/* is point p (its w set to 1) outside the view volume: through the world-to-clip matrix
 * (+0x250), any of |x|, |y|, |z| beyond |w| */
/* 0x00121A00 */
s32 Camera_OutOfView(Camera *c, f32 *p) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w;

    p[3] = 1.0f;
    sceVu0ApplyMatrix(v, (void *)((u8 *)c + 0x250), p);
    if (v[3] <= 0.0f) {
        v[3] = -v[3];
    }
    w = v[3];
    if (!(v[0] <= w) || v[0] < -w) {
        return 1;
    }
    if (!(v[1] <= w) || v[1] < -w) {
        return 1;
    }
    if (!(v[2] <= w) || v[2] < -w) {
        return 1;
    }
    return 0;
}

/* move the camera (eye and target) by d in its own frame: x to the right, y up the view, z
 * along the view on the level; nothing for a zero d */
/* 0x001220A0 */
void Camera_MoveLocal(Camera *c, f32 *d) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));

    if (d[0] == 0.0f && d[1] == 0.0f && d[2] == 0.0f) {
        return;
    }
    sceVu0UnitMatrix(m);
    sceVu0SubVector(v, c->target, c->eye);
    v[1] = 0.0f;
    sceVu0Normalize(m[2], v);
    sceVu0OuterProduct(m[0], c->unk80, m[2]);
    sceVu0Normalize(m[0], m[0]);
    sceVu0OuterProduct(m[1], m[2], m[0]);
    sceVu0ApplyMatrix(v, m, d);
    c->eye[0] = c->eye[0] + v[0];
    c->eye[1] = c->eye[1] + v[1];
    c->eye[2] = c->eye[2] + v[2];
    c->target[0] = c->target[0] + v[0];
    c->target[1] = c->target[1] + v[1];
    c->target[2] = c->target[2] + v[2];
}

/* turn the view the other way round: the eye (+0x40) goes round the point looked at (+0x50) by
 * `yaw` about Y, then by `pitch` about the axis square to the up and the view */
/* 0x00122370 */
void Camera_Orbit(Camera *c, f32 pitch, f32 yaw) {
    f32 r[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));

    q[3] = 0.0f;
    q[2] = 0.0f;
    q[1] = 0.0f;
    q[0] = 0.0f;
    if (yaw != 0.0f) {
        sceVu0UnitMatrix(r);
        sceVu0RotMatrixY(r, r, Angle_Wrap(yaw));
        sceVu0SubVector(v, c->eye, c->target);
        v[3] = 1.0f;
        sceVu0ApplyMatrix(v, r, v);
        c->eye[0] = v[0] + c->target[0];
        c->eye[1] = v[1] + c->target[1];
        c->eye[2] = v[2] + c->target[2];
    }
    if (pitch == 0.0f) {
        return;
    }
    sceVu0SubVector(v, c->target, c->eye);
    sceVu0OuterProduct(v, c->unk80, v);
    sceVu0Normalize(v, v);
    Quat_FromAxisAngle(q, v, pitch);
#ifdef HG_NATIVE
    if (yaw == 0.0f) {
        r[3][0] = r[3][1] = r[3][2] = 0.0f;   /* (unset in the original then; no NaN on PC) */
        r[3][3] = 1.0f;
    }
#endif
    Quat_ToMatrix(q, r);
    sceVu0SubVector(v, c->eye, c->target);
    sceVu0ApplyMatrix(v, r, v);
    c->eye[0] = v[0] + c->target[0];
    c->eye[1] = v[1] + c->target[1];
    c->eye[2] = v[2] + c->target[2];
}
