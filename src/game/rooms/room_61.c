/* Room 0x61: its event handler class (vtable D_00471FE0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "effects.h"
#include "hewie.h"
#include "progress.h"
#include "scene_game_members.h"
#include "skeleton.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif
#include "msl.h"
#include "sce/libvu0.h"

f32 func_00310C90(u8 *st, f32 *at, f32 yaw);

#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00471FE0[];
extern void *D_0047A370[], *D_0046F580[];
extern u8 D_01991210[], D_01991250[], D_01991290[];   /* the three paths */
extern s16 D_019912D0[], D_019913D0[], D_01991490[];   /* their points */
extern void *D_0047A3B0[];

#define SHAFT_REC(o, i) ((o) + AT(o, 0x6EC, s32) * 0x300 + (i) * 0x30 + 0x10)

#ifdef HG_NATIVE

/* the beam's corners: the top edge (0, 60, 82 .. 57), the floor (10, 30, 100 .. 40), middles
 * at z 70 */
static const f32 kShaft[6][4] __attribute__((aligned(16))) = {
    {0.0f, 60.0f, 82.0f, 1.0f}, {0.0f, 60.0f, 57.0f, 1.0f}, {10.0f, 30.0f, 100.0f, 1.0f},
    {10.0f, 30.0f, 40.0f, 1.0f}, {0.0f, 60.0f, 70.0f, 1.0f}, {10.0f, 30.0f, 70.0f, 1.0f},
};

#endif

extern u8 D_004291C0[];
extern u8 D_00429210[];
extern u8 D_004292A0[];
extern u8 D_004293E0[];
extern u8 D_00429408[];
extern u8 D_00429418[];
extern void *D_00429540[];
extern u8 D_00429580[];

extern PTMF D_01991550[];

/* room 0x61: byte 3 0 the light shaft (D_0047A370) started with its motes from (30, 0, 70),
 * its slot in event var 3; 1 its haze on, 2 off */
static void shaft_init(void **obj) {
    obj[0] = D_0047A370;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

#define PATH_PT(st, i) (AT(st, 0x28, s16 *) + (i) * 3)

static void path_point(f32 *out, u8 *st, s32 i) {
    out[0] = 0.0625f * (f32)PATH_PT(st, i)[0];
    out[1] = 0.0625f * (f32)PATH_PT(st, i)[1];
    out[2] = 0.0625f * (f32)PATH_PT(st, i)[2];
    out[3] = 1.0f;
}

/* character `kind` one frame along path `st`, turned to its heading */
static void swim_step(s32 kind, u8 *st) {
    u8 *c = (u8 *)gCharacters[func_001770D0(gProgress, kind) & 0xFF];
    f32 yaw;

#ifdef HG_NATIVE
    if (c == NULL) {   /* absent (the PS2 reads through junk) */
        return;
    }
#endif
    yaw = func_00310C90(st, (f32 *)(c + 0x10), AT(c, 0x54, f32));
    AT(c, 0x54, f32) = yaw;
    sceVu0UnitMatrix((f32 (*)[4])(c + 0x60));
    sceVu0RotMatrixY((f32 (*)[4])(c + 0x60), (f32 (*)[4])(c + 0x60), yaw);
}

static void glint_init(void **obj) {
    obj[0] = D_0047A3B0;
}

#ifdef HG_NATIVE
#endif

void *func_00310A30(void *o, s32 flags) { return room_dtor(o, flags, D_00471FE0, D_0046DB80); }

void *func_00310A90(void) {
    return D_004291C0;
}

void *func_00310AA0(void) {
    return D_00429210;
}

void *func_00310AB0(void) {
    return D_004292A0;
}

void *func_00310AC0(void) {
    return D_004293E0;
}

void *func_00310AD0(void *self, s32 i) {
    return D_00429540[i];
}

void *func_00310AF0(void) {
    return D_00429408;
}

void *func_00310B00(void) {
    return D_00429418;
}

void *func_00310B10(void) {
    return D_00429580;
}

/* point i's way on: 20 along the line from the point before to the one after */
void func_00310B20(u8 *st, s32 i, f32 *out) {
    f32 b[4] __attribute__((aligned(16)));
    s32 prev = i - 1, next;

    if (prev < 0) {
        prev = AT(st, 0x18, s32) - 1;
    }
    next = i + 1;
    if (AT(st, 0x18, s32) - 1 < next) {
        next = 0;
    }
    out[0] = 0.0625f * (f32)PATH_PT(st, next)[0];
    out[1] = 0.0625f * (f32)PATH_PT(st, next)[1];
    out[2] = 0.0625f * (f32)PATH_PT(st, next)[2];
    out[3] = 1.0f;
    b[0] = 0.0625f * (f32)PATH_PT(st, prev)[0];
    b[1] = 0.0625f * (f32)PATH_PT(st, prev)[1];
    b[2] = 0.0625f * (f32)PATH_PT(st, prev)[2];
    b[3] = 1.0f;
    sceVu0SubVector(out, out, b);
    sceVu0Normalize(out, out);
    sceVu0ScaleVector(out, out, 20.0f);
}

/* one frame along the path: `at` moved on the curve (kept in the box); returns the heading
 * `yaw` turned towards the way it moved, by at most 3.6 degrees */
f32 func_00310C90(u8 *st, f32 *at, f32 yaw) {
    static const union { u32 u; f32 f; } kTurn = {0x3D80ADFD}, kTurnN = {0xBD80ADFD};
    f32 c[4][4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 w[4];
    f32 t, u, k3, a;
    s32 next = AT(st, 0x24, s32) + 1, i;

    if (!(next < AT(st, 0x18, s32))) {
        next = 0;
    }
    t = (f32)AT(st, 0x20, s32) / (f32)AT(st, 0x1C, s32);
    u = 1.0f - t;
    k3 = 3.0f * u;
    w[0] = u * (u * u);
    w[1] = t * (k3 * u);
    w[2] = t * (k3 * t);
    w[3] = t * (t * t);
    sceVu0CopyVector((f32 *)(st + 0x30), at);
    path_point(c[0], st, AT(st, 0x24, s32));
    path_point(c[1], st, AT(st, 0x24, s32));
    func_00310B20(st, AT(st, 0x24, s32), d);
    sceVu0AddVector(c[1], c[1], d);
    path_point(c[2], st, next);
    func_00310B20(st, next, d);
    sceVu0SubVector(c[2], c[2], d);
    path_point(c[3], st, next);
    for (i = 0; i < 3; i++) {
        at[i] = 0.0f;
        at[i] = 0.0f + at[i] + w[0] * c[0][i];
        at[i] = 0.0f + at[i] + w[1] * c[1][i];
        at[i] = 0.0f + at[i] + w[2] * c[2][i];
        at[i] = 0.0f + at[i] + w[3] * c[3][i];
    }
    for (i = 0; i < 3; i++) {
        f32 v = at[i], hi = AT(st, 0xC + i * 4, f32), m = v <= hi ? v : hi;

        at[i] = m < AT(st, i * 4, f32) ? AT(st, i * 4, f32) : m;
    }
    AT(st, 0x20, s32) += 1;
    if (!(AT(st, 0x20, s32) < AT(st, 0x1C, s32))) {
        AT(st, 0x20, s32) = 0;
        AT(st, 0x24, s32) += 1;
        if (!(AT(st, 0x24, s32) < AT(st, 0x18, s32))) {
            AT(st, 0x24, s32) = 0;
        }
    }
    sceVu0SubVector(d, at, (f32 *)(st + 0x30));
    a = func_002E2D00(func_0031C5C0(d[0], d[2]) - yaw);
    if (a < kTurnN.f) {
        a = kTurnN.f;
    }
    if (!(a <= kTurn.f)) {
        a = kTurn.f;
    }
    return func_002E2D00(yaw + a);
}

/* set path `st` up: a point at random in each cell of an nx x ny x nz grid of 10-unit cells
 * from (ox, oy, oz) into `pts`, then shuffled (30 random swaps among the first 30 per point),
 * `frames` between two */
void func_00311140(u8 *st, s32 nx, s32 ny, s32 nz, s32 frames, s16 *pts, f32 ox, f32 oy, f32 oz) {
    VObject *rnd;
    s16 *p = pts;
    s32 x, y, z, n;

    rnd = gRandom;
    for (x = 0; x < nx; x++) {
        for (y = 0; y < ny; y++) {
            for (z = 0; z < nz; z++) {
                p[0] = (s16)(s32)(16.0f * (0.0f + ox + 10.0f * ((f32)x + VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd))));
                p[1] = (s16)(s32)(16.0f * (0.0f + oy + 10.0f * ((f32)y + VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd))));
                p[2] = (s16)(s32)(16.0f * (0.0f + oz + 10.0f * ((f32)z + VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd))));
                p += 3;
            }
        }
    }
    AT(st, 0x18, s32) = nz * (nx * ny);
    AT(st, 0x28, s16 *) = pts;
    if (AT(st, 0x18, s32) > 0) {
        rnd = gRandom;
        for (n = 0; n < AT(st, 0x18, s32); n++) {
            s32 i = (s32)(30.0f * VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd));
            s16 *a = pts + i * 3, *b, t;

            b = pts + (s32)(30.0f * VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd)) * 3;
            t = a[0], a[0] = b[0], b[0] = t;
            t = a[1], a[1] = b[1], b[1] = t;
            t = a[2], a[2] = b[2], b[2] = t;
        }
    }
    AT(st, 0x20, s32) = 0;
    AT(st, 0x24, s32) = 0;
    AT(st, 0x1C, s32) = frames;
    AT(st, 0x0, f32) = ox;
    AT(st, 0x4, f32) = oy;
    AT(st, 0x8, f32) = oz;
    AT(st, 0xC, f32) = 0.0f + ox + 10.0f * (f32)nx;
    AT(st, 0x10, f32) = 0.0f + oy + 10.0f * (f32)ny;
    AT(st, 0x14, f32) = 0.0f + oz + 10.0f * (f32)nz;
}

/* (self->*D_01991550[i])(a, b) */
s32 func_00311490(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991550[i & 0xFF], a, b);
}

s32 func_003114C0(void *self, void *a1, u8 *cmd) {
    s32 arg[5] __attribute__((aligned(16)));
    s32 slot;

    switch (cmd[3]) {
    case 0:
        slot = Effect_New(gEffects, 0x700, shaft_init);
        AT(&arg[0], 0, f32) = 30.0f;
        AT(&arg[1], 0, f32) = 0.0f;
        AT(&arg[2], 0, f32) = 70.0f;
        AT(&arg[3], 0, f32) = 1.0f;
        arg[4] = 0;
        func_002D6090(gEffects, slot, arg);
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 3, slot);
        break;
    case 1:
        arg[4] = 1;
        func_002D6090(gEffects, VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 3), arg);
        break;
    case 2:
        arg[4] = 2;
        func_002D6090(gEffects, VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 3), arg);
        break;
    default:
        return 1;
    }
    return 1;
}

/* room 0x61 (byte 3): 0 / 2 / 4 set up the paths of characters 0x14 (2 x 3 x 7 cells from (25,
 * 0, 40), 480 frames a stretch; with the glint D_0047A3B0) / 0x15 (2 x 3 x 5 from (30, 0, 50),
 * 240) / 0x16 (the same box, 280); 1 / 3 / 5 move them a frame (returning 2: again next frame) */
s32 func_003116B0(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        func_00311140(D_01991210, 2, 3, 7, 0x1E0, D_019912D0, 25.0f, 0.0f, 40.0f);
        Effect_New(gEffects, 4, glint_init);
        return 1;
    case 1:
        swim_step(0x14, D_01991210);
        return 2;
    case 2:
        func_00311140(D_01991250, 2, 3, 5, 0xF0, D_019913D0, 30.0f, 0.0f, 50.0f);
        return 1;
    case 3:
        swim_step(0x15, D_01991250);
        return 2;
    case 4:
        func_00311140(D_01991290, 2, 3, 5, 0x118, D_01991490, 30.0f, 0.0f, 50.0f);
        return 1;
    case 5:
        swim_step(0x16, D_01991290);
        return 2;
    }
    return 1;
}

/* +0x8 destructor (the quad drawer's inlined) */
u8 *func_00374B30(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_0047A370;
    AT(o, 0x610, void **) = D_0046FC30;
    AT(o, 0x610, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* mote i (re)started at the bottom: rising 0.1 .. 0.3 a frame, grey, alpha 0x40 .. 0x7F, spread
 * 50 either way along z, sized 3/4 of its rise, a random turn */
void func_00374BC0(u8 *o, s32 i) {
    VObject *rnd;
    u8 *r;

    if (AT(o, 0x6F0, u8) == 1) {
        return;
    }
    rnd = gRandom;
    AT(o, 0x660 + i * 4, f32) = 0x1.99999ap-4f /* 0.1 */ + 0x1.99999ap-3f /* 0.2 */ * shaft_rnd(rnd);
    AT(o, 0x6A0 + i * 4, f32) = 0x1.921fb6p+2f /* 2 pi */ * (shaft_rnd(rnd) - 0.5f);
    r = SHAFT_REC(o, i);
    AT(r, 0x0, s32) = 0x80;
    AT(r, 0x4, s32) = 0x80;
    AT(r, 0x8, s32) = 0x80;
    AT(r, 0xC, s32) = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 0x3F) + 0x40;
    AT(r, 0x10, f32) = AT(o, 0x650, f32);
    AT(r, 0x14, f32) = AT(o, 0x654, f32);
    AT(r, 0x18, f32) = AT(o, 0x658, f32) + 100.0f * (shaft_rnd(rnd) - 0.5f);
    AT(r, 0x1C, f32) = 1.0f;
    AT(r, 0x20, f32) = AT(r, 0x24, f32) = 0.75f * AT(o, 0x660 + i * 4, f32);
    AT(r, 0x28, f32) = 0x1.921fb6p+1f /* pi */ * (360.0f * (VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd) - 0.5f)) / 180.0f;
    AT(r, 0x2C, s32) = 0;
}

/* +0x18 start: NULL stops it; kind (+0x10) 0 the motes from the position (+0x0), 1 the haze
 * on, 2 off */
void func_00374DB0(u8 *o, u8 *arg) {
    s32 i;

    if (arg == NULL) {
        AT(o, 0x6F0, u8) = 1;
        return;
    }
    switch (AT(arg, 0x10, s32)) {
    case 0:
        sceVu0CopyVector((f32 *)(o + 0x650), (f32 *)arg);
        for (i = 0; i < 16; i++) {
            func_00374BC0(o, i);
        }
        break;
    case 1:
        AT(o, 0x6F1, u8) = 1;
        break;
    case 2:
        AT(o, 0x6F1, u8) = 0;
        break;
    }
}

#ifdef HG_NATIVE

/* +0x14 draw (PC; the PS2 sends GS packets): when all of the beam is in view, the strip
 * top-front, floor-front, top-middle, floor-middle, top-back, floor-back (alpha 0x40 at the
 * edges, 0x80 in the middle, texture group 8 id 0, rows 224 .. 256) twice, scrolled +0x6E8 and
 * (mirrored) a quarter further, in layer 0x19 without depth writes; then the motes. With the
 * haze on: in layer 0x2A the screen at half size is copied, blended half with itself drawn in
 * 33 columns 8 apart each moved down 16 sin(a) (2 + (1 + cos a) / 2) / 16 (a = +0x6E0 + 90
 * degrees a column) and all of it 2 sin(+0x6E4) right; that blended back over the screen at
 * 0x48 / 128 */
void func_00374E50(u8 *o) {
    static const s8 kOrder[6] = {0, 2, 4, 5, 1, 3};
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 xyzw[6][4] __attribute__((aligned(16)));
    f32 st[6][2];
    u8 rgba[6][4];
    u8 *tex = NULL;
    s32 in = 1, i, pass;

    if (AT(o, 0x6F0, u8) == 1) {
        return;
    }
    VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
    VCALL(gBootMessage, 0x20, void (*)(VObject *))(gBootMessage);
    if (TexCache_Resident(8, 0, 0x19, &tex) == -1) {
        in = 0;
    }
    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, clip);
    for (i = 0; i < 6; i++) {
        f32 v[4] __attribute__((aligned(16)));

        sceVu0ApplyMatrix(v, clip, kShaft[i]);
        if (!(v[0] <= v[3]) || v[0] < -v[3] || !(v[1] <= v[3]) || v[1] < -v[3] || !(v[2] <= v[3]) || v[2] < -v[3]) {
            in = 0;
        }
    }
    if (in && tex != NULL && AT(tex, 4, u16) != 0 && AT(tex, 6, u16) != 0) {
        f32 tw = (f32)AT(tex, 4, u16), th = (f32)AT(tex, 6, u16);

        glr_layer(0x19);
        for (pass = 0; pass < 2; pass++) {
            s32 s = AT(o, 0x6E8, s32);

            if (pass == 1 && (s += 0x400) >= 0x1001) {
                s -= 0x1000;
            }
            for (i = 0; i < 6; i++) {
                s32 col = i >> 1;   /* front, middle, back */

                sceVu0CopyVector(xyzw[i], kShaft[kOrder[i]]);
                AT(&xyzw[i][3], 0, u32) = 0;
                if (pass == 1) {
                    col = 2 - col;
                }
                st[i][0] = (f32)(s + 8 + (col == 0 ? 0 : col == 1 ? 0x800 : 0x1000)) / 16.0f / tw;
                st[i][1] = (i & 1 ? 256.5f : 224.5f) / th;
                rgba[i][0] = rgba[i][1] = rgba[i][2] = 0x80;
                rgba[i][3] = (i >> 1) == 1 ? 0x80 : 0x40;
            }
            glr_strip(&clip[0][0], 6, &xyzw[0][0], &st[0][0], &rgba[0][0], tex, 1ull << 34,
                      0x10 | 0x40 | 0x20000 /* GLR_PRIM_NOZW */);
        }
        glr_layer(-1);
    }
    AT(o, 0x620, u8 *) = o + AT(o, 0x6EC, s32) * 0x300 + 0x10;
    func_002E56C0(o + 0x610);
    if (AT(o, 0x6F1, u8) == 1) {
        VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
        VCALL(gBootMessage, 0x20, void (*)(VObject *))(gBootMessage);
        glr_haze(AT(o, 0x6E0, f32), 2.0f * func_0031C248(AT(o, 0x6E4, f32)));
    }
}

#endif

/* +0x10 update: the buffers swapped (the motes copied over), the texture scrolled 3.2 / 16
 * texels; each mote turns, wobbles 0.1 about its angle and rises; on every other frame it
 * fades by 0 or 1 and starts over once gone. The haze's phases on by 3 .. 5 and 1 .. 3 degrees */
s32 func_00376200(u8 *o) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    VObject *rnd;
    f32 t;
    s32 i, k;

    if (AT(o, 0x6F0, u8) == 1) {
        return 0;
    }
    AT(o, 0x6EC, s32) ^= 1;
    AT(o, 0x6E8, s32) = (s32)((f32)AT(o, 0x6E8, s32) + 0x1.99999ap+1f /* 3.2 */);
    t = (f32)AT(o, 0x6E8, s32);
    if (!(t <= 4096.0f)) {
        AT(o, 0x6E8, s32) = (s32)(t - 4096.0f);
    }
    rnd = gRandom;
    for (i = 0; i < 16; i++) {
        s32 cur = AT(o, 0x6EC, s32);
        u32 *dst = (u32 *)(o + cur * 0x300 + i * 0x30 + 0x10);
        u32 *src = (u32 *)(o + (cur ^ 1) * 0x300 + i * 0x30 + 0x10);
        u8 *r;
        f32 a;

        for (k = 0; k < 12; k++) {
            *dst++ = *src++;
        }
        r = SHAFT_REC(o, i);
        AT(r, 0x28, f32) = AT(r, 0x28, f32) + 0.5f * (kPi.f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd) / 180.0f);
        a = AT(o, 0x6A0 + i * 4, f32) + kPi.f * (90.0f * shaft_rnd(rnd)) / 180.0f;
        AT(o, 0x6A0 + i * 4, f32) = a;
        if (!(a <= kPi.f)) {
            AT(o, 0x6A0 + i * 4, f32) = a - k2Pi.f;
        }
        AT(r, 0x10, f32) = AT(r, 0x10, f32) + 0x1.99999ap-4f /* 0.1 */ * func_0031C248(AT(o, 0x6A0 + i * 4, f32));
        AT(r, 0x14, f32) = AT(r, 0x14, f32) + AT(o, 0x660 + i * 4, f32);
        AT(r, 0x18, f32) = AT(r, 0x18, f32) + 0x1.99999ap-4f /* 0.1 */ * func_0031C058(AT(o, 0x6A0 + i * 4, f32));
        if (AT(o, 0x6EC, s32) == 0) {
            AT(r, 0xC, s32) = AT(r, 0xC, s32) - (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 1);
            if (AT(r, 0xC, s32) < 0) {
                func_00374BC0(o, i);
            }
        }
    }
    if (AT(o, 0x6F1, u8) == 1) {
        rnd = gRandom;
        t = AT(o, 0x6E0, f32) + kPi.f * (3.0f + 2.0f * shaft_rnd(rnd)) / 180.0f;
        AT(o, 0x6E0, f32) = t;
        AT(o, 0x6E0, f32) = func_002E2D00(t);
        t = AT(o, 0x6E4, f32) + kPi.f * (1.0f + 2.0f * shaft_rnd(rnd)) / 180.0f;
        AT(o, 0x6E4, f32) = t;
        AT(o, 0x6E4, f32) = func_002E2D00(t);
    }
    return 1;
}

/* +0xC set up: a random scroll; the motes' drawer: layer 0x19, texture group 0x10 cell
 * (0x1A0, 0x40) 32 x 32 of 512 x 256, additive, 5 frames; the haze at random phases */
void func_003765B0(u8 *o) {
    VObject *rnd = gRandom;

    AT(o, 0x6EC, s32) = 0;
    AT(o, 0x6F0, u8) = 0;
    AT(o, 0x6F1, u8) = 0;
    AT(o, 0x6E8, s32) = (s32)(4096.0f * shaft_rnd(rnd));
    AT(o, 0x618, s64) = -1;
    AT(o, 0x624, s32) = 0;
    AT(o, 0x628, s32) = 0;
    AT(o, 0x62C, s32) = 0;
    AT(o, 0x630, s32) = 0x19;
    AT(o, 0x634, s16) = 0x10;
    AT(o, 0x636, s16) = 0x1A0;
    AT(o, 0x638, s16) = 0x40;
    AT(o, 0x63A, s16) = 0x20;
    AT(o, 0x63C, s16) = 0x20;
    AT(o, 0x63E, s16) = 0x200;
    AT(o, 0x640, s16) = 0x100;
    AT(o, 0x642, s8) = 0x40;
    AT(o, 0x643, s8) = 1;
    AT(o, 0x644, s8) = 1;
    AT(o, 0x645, s8) = 0x10;
    AT(o, 0x646, s8) = 5;
    AT(o, 0x6E0, f32) = 0x1.921fb6p+1f /* pi */ * (360.0f * (shaft_rnd(rnd) - 0.5f)) / 180.0f;
    AT(o, 0x6E4, f32) = 0x1.921fb6p+1f /* pi */ * (360.0f * (shaft_rnd(rnd) - 0.5f)) / 180.0f;
}

/* +0x8 destructor */
void **func_00377B10(void **o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    o[0] = D_0047A3B0;
    o[0] = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* +0x14 draw: unless the effects are paused, a white 1 x 1 sprite (group 0x10 cell (0x40,
 * 0x20) 32 x 32 of 512 x 256) at the bone, in layer 0x26 */
void func_00377B70(void) {
    struct {
        void **vtbl;
        s32 a;
        u64 tex;
        u8 *rec;
        s32 b, c, d;
        s32 layer;
        s16 s[7];
        s8 k[5];
    } q __attribute__((aligned(8)));
    s32 rec[12] __attribute__((aligned(16)));
    u8 *ch;

    if (func_002D6010(gEffects) != 0) {
        return;
    }
    ch = (u8 *)gCharacters[func_001770D0(gProgress, 0x14) & 0xFF];
#ifdef HG_NATIVE
    if (ch == NULL) {   /* absent (the PS2 reads through junk) */
        return;
    }
#endif
    rec[0] = rec[1] = rec[2] = rec[3] = 0x80;
    sceVu0CopyVector((f32 *)&rec[4], Skel_Bone(AT(AT(ch, 0xF0, u8 *), 0x810, u8 *), 6) + 12);
    AT(&rec[8], 0, f32) = 1.0f;
    AT(&rec[9], 0, f32) = 1.0f;
    q.a = -1;
    q.vtbl = D_0046FC30;
    q.k[4] = -1;
    q.tex = -1;
    q.c = 0;
    q.rec = (u8 *)rec;
    q.d = 0;
    q.layer = 0x26;
    q.b = 0;
    q.s[2] = 0x40;
    q.s[0] = 1;
    q.s[5] = 0x200;
    q.s[1] = 0x20;
    q.s[6] = 0x100;
    q.s[3] = 0x20;
    q.k[3] = 0x10;
    q.s[4] = 0x20;
    q.k[0] = 0;
    q.k[1] = 1;
    q.k[2] = 1;
    rec[10] = 0;
    rec[11] = 0;
    func_002E56C0((u8 *)&q);
    q.vtbl = D_00469D00;
}

/* +0x10 update */
s32 func_00377CA0(void) {
    return 1;
}

/* +0xC set up */
void func_00377CB0(void) {
}
