/* Camera (Game +0x14D9B00, global D_0044E4B8, vtable D_00469A60; base vtable D_00469B40). The
 * view comes from an eye and a target point; matrices are built by the larger methods. */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "sce/libvu0.h"

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

extern void *D_00469A60[];
extern void *D_00469B40[];
extern Camera *D_0044E4B8;
extern void func_00100490(void *p);   /* operator delete */
extern f32 func_0031C5C0(f32 x, f32 z);   /* heading of (x, z) */

/* +0x8 */
Camera *func_001218A0(Camera *c, s32 flags) {
    if (c != NULL) {
        c->vtbl = D_00469A60;
        if (c != NULL) {
            c->vtbl = D_00469B40;
            if (c != NULL) {
                D_0044E4B8 = NULL;
            }
        }
        if ((s16)flags > 0) {
            func_00100490(c);
        }
    }
    return c;
}

/* +0xC defaults: 60 degree view from the origin towards +z, no path */
void func_00122A30(Camera *c) {
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

/* +0x10 reset */
void func_00122A20(Camera *c) {
    VCALL(c, 0xC, void (*)(Camera *))(c);
}

/* +0x1C / +0x20 eye */
void func_001225E0(Camera *c, f32 x, f32 y, f32 z) {
    c->eye[0] = x;
    c->eye[1] = y;
    c->eye[2] = z;
}

void func_001225D0(Camera *c, f32 *out) { sceVu0CopyVector(out, c->eye); }

/* +0x24 */
void func_001225C0(Camera *c, f32 *out) { sceVu0CopyVector(out, c->unk60); }

/* +0x28 / +0x2C target */
void func_001225B0(Camera *c, f32 x, f32 y, f32 z) {
    c->target[0] = x;
    c->target[1] = y;
    c->target[2] = z;
}

void func_001225A0(Camera *c, f32 *out) { sceVu0CopyVector(out, c->target); }

/* +0x30 move the eye, the target following (same view direction) */
void func_00122550(Camera *c, f32 x, f32 y, f32 z) {
    f32 dx = x - c->eye[0], dy = y - c->eye[1], dz = z - c->eye[2];

    c->eye[0] = x;
    c->eye[1] = y;
    c->eye[2] = z;
    c->target[0] += dx;
    c->target[1] += dy;
    c->target[2] += dz;
}

/* +0x34 move the target, the eye following */
void func_00122500(Camera *c, f32 x, f32 y, f32 z) {
    f32 dx = x - c->target[0], dy = y - c->target[1], dz = z - c->target[2];

    c->target[0] = x;
    c->target[1] = y;
    c->target[2] = z;
    c->eye[0] += dx;
    c->eye[1] += dy;
    c->eye[2] += dz;
}

/* +0x44 .. +0x58 matrices */
void func_00122090(Camera *c, f32 (*out)[4]) { sceVu0CopyMatrix(out, c->unk150); }
void func_00122080(Camera *c, f32 (*out)[4]) { sceVu0CopyMatrix(out, c->unk190); }
void func_00122050(Camera *c, f32 (*out)[4]) { sceVu0CopyMatrix(out, c->unk1D0); }
void func_00122070(Camera *c, f32 (*m)[4]) { sceVu0CopyMatrix(c->unk150, m); }
void func_00122060(Camera *c, f32 (*m)[4]) { sceVu0CopyMatrix(c->unk190, m); }
void func_00122040(Camera *c, f32 (*out)[4]) { sceVu0CopyMatrix(out, c->unk210); }

/* +0x5C / +0x64 field of view */
void func_00122030(Camera *c, f32 fov) { c->fov = fov; }
f32 func_00122010(Camera *c) { return c->fov; }

/* +0x60 view matrix */
void func_00122020(Camera *c, f32 (*out)[4]) { sceVu0CopyMatrix(out, c->view); }

/* +0x68 heading of the view direction */
f32 func_00122000(Camera *c) { return func_0031C5C0(c->view[0][2], c->view[2][2]); }

/* +0x6C */
void func_00121FF0(Camera *c, f32 v) { c->unk4 = v; }

/* +0x70 / +0x74 roll */
void func_00121910(Camera *c, f32 v) { c->roll = v; }
f32 func_00121920(Camera *c) { return c->roll; }

/* +0x78 follow path `path` (NULL: none) from its start */
void func_00121FD0(Camera *c, s32 *path) {
    c->path = path;
    c->unk294 = -1;
    c->unk298 = -1;
}

/* +0x7C */
void func_00121FC0(Camera *c) { c->path = NULL; }

/* +0x80 number of path entries */
s32 func_00121F80(Camera *c) {
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
void func_00121D00(Camera *c, CameraSet *s) {
    VCALL(c, 0x1C, void (*)(Camera *, f32, f32, f32))(c, s->eye[0], s->eye[1], s->eye[2]);
    VCALL(c, 0x28, void (*)(Camera *, f32, f32, f32))(c, s->target[0], s->target[1], s->target[2]);
    VCALL(c, 0x5C, void (*)(Camera *, f32))(c, s->fov);
    c->unk29C = s->unk24;
    VCALL(c, 0x70, void (*)(Camera *, f32))(c, 0.0f);
}

/* +0x8C the preset +0x84 makes from `arg`, applied */
void func_00121CB0(Camera *c, s32 arg) {
    CameraSet s;

    VCALL(c, 0x84, void (*)(Camera *, s32, CameraSet *))(c, arg, &s);
    VCALL(c, 0x88, void (*)(Camera *, CameraSet *))(c, &s);
}

/* +0x90 .. +0x9C */
s32 func_00121930(Camera *c) { return c->unk294; }
s32 func_00121940(Camera *c) { return c->unk298; }
s32 *func_00121950(Camera *c) { return c->path; }
u8 *func_00121CA0(Camera *c) { return c->unk2A0; }

/* +0xA0 */
void func_00121960(Camera *c, f32 *out) { sceVu0CopyVector(out, c->unk70); }

/* +0xA4 the view matrix's second column (an axis), w 0 */
void func_00121970(Camera *c, f32 *out) {
    out[0] = c->view[0][1];
    out[1] = c->view[1][1];
    out[2] = c->view[2][1];
    out[3] = 0.0f;
}

/* +0xA8 .. +0xB4 depth ranges */
void func_00121990(Camera *c, f32 v) { c->zMax = v; }
void func_001219A0(Camera *c, f32 v) { c->zMin = v; }
void func_001219B0(Camera *c, f32 v) { c->nearZ = v; }
void func_001219C0(Camera *c, f32 v) { c->farZ = v; }

/* +0xB8 the Z buffer value of view depth `d` (zMin at nearZ, zMax at farZ, 1/d in between) */
s32 func_00121C50(Camera *c, f32 d) {
    f32 n = c->nearZ, f = c->farZ;

    return (s32)(n * f * (c->zMax - c->zMin) / (f - n) / d - (c->zMax * n - c->zMin * f) / (f - n));
}

/* +0xC0 / +0xC4 */
void func_00121B30(Camera *c, f32 a, f32 b) {
    c->unk28 = a;
    c->unk2C = b;
}

void func_001219D0(Camera *c, f32 x, f32 y) {
    c->centerX = x;
    c->centerY = y;
}

/* +0xC8 */
void func_00121B20(Camera *c, f32 (*out)[4]) { sceVu0CopyMatrix(out, c->viewScreen); }

/* +0xCC / +0xD0 */
f32 func_001219E0(Camera *c) { return c->nearZ; }
f32 func_001219F0(Camera *c) { return c->farZ; }

/* +0x14 update the matrices: world to view (+0x18), then the view-to-screen matrices (the
 * screen's and a half-size one for offscreen work) and the clip matrices from them */
void func_00122810(Camera *c) {
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

extern VObject *D_0044E550;               /* random numbers: +0x18 -> 0..1 */
extern f32 func_0031C338(f32 x);         /* tanf */
extern void func_0025C6F0(f32 *q, const f32 *axis, f32 angle);
extern void func_0025C770(const f32 *q, f32 (*m)[4]);

#define RAND01() VCALL(D_0044E550, 0x18, f32 (*)(VObject *))(D_0044E550)

/* +0x18 the world-to-view matrix `out`: the direction to the target (+0x70), the screen
 * distance for the field of view (+0x8 = 1.4 / tan(fov / 2)), the eye shaken by +0x4 at random
 * (+0x60), and the up vector rolled by +0x30 about the direction (+0x80) */
void func_001225F0(Camera *c, f32 (*out)[4]) {
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
    func_0025C6F0(q, c->unk70, c->roll);
    func_0025C770(q, rot);
    sceVu0ApplyMatrix(c->unk80, rot, s);
    sceVu0CameraMatrix(out, c->unk60, c->unk70, c->unk80);
}
