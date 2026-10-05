/* The stalkers' models (the character model classes their loaders build, model.c), and the
 * trivial methods of the model base class (vtable D_0046F9E0) that natively were still stubs.
 * The models' secondary motion (springs for hanging parts) works as Fiona's (model.c). */
#include "common.h"
#include "game.h"
#include "model.h"
#include "sce/libvu0.h"

extern f32 *func_0017CE80(u8 *skel, s32 bone);   /* a bone's matrix */

/* ---- the model base class's trivial methods ---- */

/* +0x1C / +0x20 / +0x24 / +0x2C / +0x30 / +0x14 / +0x3C / +0x4C / +0x50 / +0x54 / +0x60:
   nothing */
void func_001F4700(u8 *m) {
}

void func_001F46F0(u8 *m) {
}

void func_001F46E0(u8 *m) {
}

void func_00194FB0(u8 *m) {
}

void func_00195ED0(u8 *m) {
}

void func_001F6130(u8 *m) {
}

void func_0019C200(u8 *m) {
}

void func_001F1E20(u8 *m) {
}

void func_002DD030(u8 *m) {
}

void func_002DD020(u8 *m) {
}

void func_002DD010(u8 *m) {
}

void func_002DCF00(u8 *m) {
}

/* +0x74 .. +0xB0 (and the human models' +0x84 .. +0x90): none */
s32 func_0016D040(u8 *m) {
    return 0;
}

s32 func_0016D170(u8 *m) {
    return 0;
}

s32 func_001800A0(u8 *m) {
    return 0;
}

s32 func_001800B0(u8 *m) {
    return 0;
}

s32 func_001800C0(u8 *m) {
    return 0;
}

s32 func_001800D0(u8 *m) {
    return 0;
}

s32 func_0018CC90(u8 *m) {
    return 0;
}

s32 func_00195C60(u8 *m) {
    return 0;
}

s32 func_001F1E90(u8 *m) {
    return 0;
}

s32 func_001F1EA0(u8 *m) {
    return 0;
}

s32 func_001F1EB0(u8 *m) {
    return 0;
}

s32 func_001F1EC0(u8 *m) {
    return 0;
}

s32 func_001F7F90(u8 *m) {
    return 0;
}

s32 func_0020CAD0(u8 *m) {
    return 0;
}

s32 func_002DC6F0(u8 *m) {
    return 0;
}

s32 func_002DC700(u8 *m) {
    return 0;
}

/* +0x74 / +0x78 / +0x7C / +0x80: the part roles set up at load (+0x8AC, +0x8BC, +0x8B8, +0x8B0) */
s32 func_001F1E30(u8 *m) {
    return AT(m, 0x8AC, s32);
}

s32 func_001F1E40(u8 *m) {
    return AT(m, 0x8BC, s32);
}

s32 func_001F1E50(u8 *m) {
    return AT(m, 0x8B8, s32);
}

s32 func_001F1E60(u8 *m) {
    return AT(m, 0x8B0, s32);
}

/* +0x58: the scale +0x8C0 */
f32 func_00211180(u8 *m) {
    return AT(m, 0x8C0, f32);
}

/* +0x34: +0x878 = `v`, then +0x30 */
void func_001F1E70(u8 *m, s32 v) {
    AT(m, 0x878, s32) = v;
    VCALL(m, 0x30, void (*)(u8 *, s32))(m, v);
}

/* +0x28: the model matrix +0x7D0 = `mtx` */
void func_001F6E00(u8 *m, f32 (*mtx)[4]) {
    sceVu0CopyMatrix((f32 (*)[4])(m + 0x7D0), mtx);
}

/* +0xB4: no secondary-motion table */
void func_00210DF0(u8 *m) {
    AT(m, 0x874, void *) = NULL;
}

extern void func_002E3040(f32 (*mtx)[4], const f32 *pos, f32 heading);

/* +0x40: the model matrix from the actor's position and heading */
void func_002DCDD0(u8 *m, u8 *actor) {
    func_002E3040((f32 (*)[4])(m + 0x7D0), (f32 *)(actor + 0x10), AT(actor, 0x54, f32));
}

extern void func_001F7AC0(u8 *m);

/* +0x10 of the base: func_001F7AC0, no secondary-motion table */
void func_002DE070(u8 *m) {
    func_001F7AC0(m);
    AT(m, 0x874, void *) = NULL;
}

/* +0x10 of the human base */
void func_002118C0(u8 *m) {
    func_002DE070(m);
}

/* ---- Debilitas's model (vtable D_0046C0A0, 0xBA0 bytes: kinds 2 / 6 / 7 / 27): four hanging
   points at +0x9A0 (0x50 each) on a spring system +0xAE0 with two collision spheres +0xB20 ---- */

extern void *D_0046C0A0[];
extern void func_001002C0(void *array, void *(*dtor)(void *, s32), u32 size, u32 n);   /* __destroy_arr */
extern void *func_0016FBB0(void *e, s32 flags);

/* +0x8: destructor */
void *func_002108F0(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = D_0046C0A0;
        func_001002C0(m + 0x9A0, func_0016FBB0, 0x50, 4);
        HumanModel_Destroy(m, flags);
    }
    return m;
}

/* +0x10 */
void func_00210CD0(u8 *m) {
    func_002118C0(m);
}

extern u8 D_003D89A0[];

/* +0xB4: his secondary-motion table */
void func_00210A20(u8 *m) {
    AT(m, 0x874, u8 *) = D_003D89A0;
}

/* +0x84 .. +0x90: his mesh parts */
s32 func_00210A30(u8 *m) {
    return 3;
}

s32 func_00210A40(u8 *m) {
    return 7;
}

s32 func_00210A50(u8 *m) {
    return 0x12;
}

s32 func_00210A60(u8 *m) {
    return 0x1C;
}

extern void func_002EE960(u8 *set);
extern void func_002EE690(u8 *col, s32 bone, f32 x, f32 y, f32 z, f32 r);
extern void func_002EE8A0(u8 *s);
extern void func_002EE900(u8 *s);
extern void func_002EE840(u8 *s);
extern void func_002118D0(u8 *m);

/* his four hanging points (+0x9A0, 0x50 each, bones 10..13, the last two hanging from the first)
   on the spring system +0xAE0, and its two collision spheres on bone 2 */
void func_00210A70(u8 *m) {
    s32 i;

    func_002EE960(m + 0xAE0);
    for (i = 0; i < 4; i++) {
        u8 *node = m + 0x9A0 + i * 0x50;

        if (AT(m, 0xB10, u8 *) != NULL && AT(m, 0xB14, u8 *) != NULL) {
            AT(AT(m, 0xB14, u8 *), 0x28, u8 *) = node;
            AT(node, 0x28, u8 *) = NULL;
            AT(node, 0x2C, u8 *) = AT(m, 0xB14, u8 *);
            AT(m, 0xB14, u8 *) = node;
        } else {
            AT(m, 0xB14, u8 *) = node;
            AT(m, 0xB10, u8 *) = node;
            AT(node, 0x2C, u8 *) = NULL;
            AT(node, 0x28, u8 *) = NULL;
        }
    }
    for (i = 0; i < 2; i++) {
        u8 *col = m + 0xB20 + i * 0x40;

        AT(col, 0x2C, u8 *) = NULL;
        if (AT(m, 0xAF8, u8 *) == NULL) {
            AT(m, 0xAF8, u8 *) = col;
        } else {
            u8 *c = AT(m, 0xAF8, u8 *);

            while (AT(c, 0x2C, u8 *) != NULL) {
                c = AT(c, 0x2C, u8 *);
            }
            AT(c, 0x2C, u8 *) = col;
        }
    }
    AT(m, 0xAE0, f32) = 0.0f;
    AT(m, 0xAE4, u32) = 0x3ECCCCCD;   /* 0.4f (ee-gcc rounds the literal) */
    AT(m, 0xAE8, f32) = 0.0f;
    AT(m, 0xAF0, u32) = 0x3F7D70A4;   /* 0.99f */
    AT(m, 0xAF4, u8 *) = m;
    AT(m, 0xB00, u8) = 0;
    AT(m, 0xAFC, s32) = 0;
    AT(m, 0x9E0, f32) = 1.25f;
    AT(m, 0x9C4, s32) = 10;
    AT(m, 0x9C0, u8) = 1;
    AT(m, 0xA30, f32) = 1.25f;
    AT(m, 0xA14, s32) = 11;
    AT(m, 0xA10, u8) = 0;
    AT(m, 0xA80, f32) = 1.25f;
    AT(m, 0xA64, s32) = 12;
    AT(m, 0xA60, u8) = 0;
    AT(m, 0xAD0, f32) = 1.25f;
    AT(m, 0xAB4, s32) = 13;
    AT(m, 0xAB0, u8) = 0;
    AT(m, 0xA6C, u8 *) = m + 0x9A0;
    AT(m, 0xABC, u8 *) = m + 0x9A0;
    func_002EE690(m + 0xB20, 2, 0.0f, 0.0f, 0.0f, 0x1.ccccccp+0f);   /* 1.8 */
    func_002EE690(m + 0xB60, 2, 1.0f, 0.0f, 0.0f, 0x1.ccccccp+0f);
    AT(m, 0x850, u8) = 1;
}

/* +0xC: once loaded: the base setup, the part roles, the springs, per-part draw settings */
void func_00210CE0(u8 *m) {
    func_002118D0(m);
    AT(m, 0x890, s32) = 2;
    AT(m, 0x894, s32) = 3;
    AT(m, 0x898, s32) = 4;
    AT(m, 0x89C, s32) = 5;
    AT(m, 0x8B8, s32) = 0x14;
    AT(m, 0x8A0, s32) = 6;
    AT(m, 0x8A4, s32) = 7;
    AT(m, 0x8A8, s32) = 8;
    AT(m, 0x8AC, s32) = 9;
    AT(m, 0x8BC, s32) = 0x1E;
    AT(m, 0x8B0, s32) = 0x16;
    AT(m, 0x8B4, s32) = 0xF;
    AT(m, 0x860, f32) = 0.0f;
    AT(m, 0x864, f32) = 16.0f;
    AT(m, 0x868, f32) = 0.0f;
    AT(m, 0x854, s32) = 0;
    AT(m, 0x858, s32) = 0;
    func_00210A70(m);
    {
        static const u8 sLoose[] = {0x9C, 0x9E, 0xA0, 0xA2, 0xC2, 0xC4, 0xC6};
        static const u8 sStiff[] = {0xB4, 0xBC, 0xBE, 0xC0};
        u32 i;

        for (i = 0; i < sizeof(sLoose); i++) {
            AT(m, sLoose[i], u8) = 4;
            AT(m, sLoose[i] + 1, u8) = 0x40;
        }
        for (i = 0; i < sizeof(sStiff); i++) {
            AT(m, sStiff[i], u8) = 4;
            AT(m, sStiff[i] + 1, u8) = 0x80;
        }
    }
}

/* +0x3C: the springs a frame: one step, or 30 to settle after a reset (+0x850) */
void func_00210C50(u8 *m) {
    s32 n = AT(m, 0x850, u8) != 0 ? 30 : 1;
    s32 i;

    func_002EE8A0(m + 0xAE0);
    for (i = 0; i < n; i++) {
        func_002EE900(m + 0xAE0);
    }
    func_002EE840(m + 0xAE0);
    AT(m, 0x850, u8) = 0;
}

/* ---- more of the model base ---- */

extern void *D_004562A8;   /* the skeleton pool */
extern void *D_004562B0;   /* the chain pool (motion buffers) */
extern void func_0017CED0(void *pool, u8 *skel);   /* free a skeleton */
extern void func_00179BC0(void *pool, void *p);     /* free into D_004562B0 */

/* release the model's skeleton (+0x810) and the skeletons and buffers of its two motion slots
   (+0x564) and three blend channels (+0x6B0), and clear the slots' key lists */
void func_001F7AC0(u8 *m) {
    void *skels = D_004562A8;
    void *bufs;
    s32 i, j, k;

    func_0017CED0(skels, AT(m, 0x810, u8 *));
    AT(m, 0x810, s32) = 0;
    bufs = D_004562B0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            u8 *s = m + i * 0xA0 + j * 4;

            func_0017CED0(skels, AT(s, 0x58C, u8 *));
            func_00179BC0(bufs, AT(s, 0x584, void *));
            func_00179BC0(bufs, AT(s, 0x594, void *));
            AT(s, 0x58C, s32) = 0;
            AT(s, 0x584, s32) = 0;
            AT(s, 0x594, s32) = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            u8 *s = m + i * 0x60 + j * 0x1C;

            func_00179BC0(bufs, AT(s, 0x6DC, void *));
            AT(s, 0x6DC, s32) = 0;
            func_0017CED0(skels, AT(s, 0x6E0, u8 *));
            AT(s, 0x6E0, s32) = 0;
        }
    }
    for (i = 0; i < 2; i++) {
        for (k = 0; k < 24; k++) {
            AT(m, 0x59C + i * 0xA0 + k * 4, s32) = 0;
        }
    }
}

/* a model matrix: the heading (wrapped to -pi..pi) about Y, at `pos` */
void func_002E3040(f32 (*mtx)[4], const f32 *pos, f32 heading) {
    if (!(heading <= 0x1.921fb6p+1f)) {
        do {
            heading -= 0x1.921fb6p+2f;
        } while (!(heading <= 0x1.921fb6p+1f));
    }
    if (heading < -0x1.921fb6p+1f) {
        do {
            heading += 0x1.921fb6p+2f;
        } while (heading < -0x1.921fb6p+1f);
    }
    sceVu0UnitMatrix(mtx);
    sceVu0RotMatrixY(mtx, mtx, heading);
    sceVu0TransMatrix(mtx, mtx, pos);
}

/* the parts' base (vtable D_004703B0; 0x40 / 0x50 / 0x70 parts, the vtable at +0x30): +0x8 reset,
   +0xC / +0x10 nothing */
void func_002EE6D0(u8 *e) {
    AT(e, 0x8, s32) = 0;
    AT(e, 0x4, s32) = 0;
    AT(e, 0x0, s32) = 0;
    AT(e, 0x18, s32) = 0;
    AT(e, 0x14, s32) = 0;
    AT(e, 0x10, s32) = 0;
    AT(e, 0x24, s32) = -1;
    AT(e, 0x20, u8) = 1;
    AT(e, 0x2C, s32) = 0;
    AT(e, 0x28, s32) = 0;
}

void func_002EE6C0(u8 *e) {
}

void func_002EE830(u8 *e) {
}

/* +0x68: the drawing state cleared (+0x38.., the 16 words at +0x58), then +0x30 */
void func_002DCA70(u8 *m) {
    s32 i;

    AT(m, 0x38, s32) = 0;
    AT(m, 0x3C, s32) = 0;
    AT(m, 0x40, s32) = 0;
    AT(m, 0x48, s32) = 0;
    AT(m, 0x4C, s32) = 0;
    AT(m, 0x50, s32) = 0;
    for (i = 0; i < 16; i++) {
        AT(m, 0x58 + i * 4, s32) = 0;
    }
    AT(m, 0x870, s32) = 0;
    AT(m, 0x30, u8) = 0;
    VCALL(m, 0x30, void (*)(u8 *))(m);
}

/* ---- spring sets (see model.c's spring systems): a set's links (+0x30 first, +0x34 last; a
   link's +0x28 next, +0x2C previous) and its colliders (+0x18 first, a collider's +0x2C next) ---- */

static inline void Set_AddLink(u8 *set, u8 *node) {
    if (AT(set, 0x30, u8 *) != NULL && AT(set, 0x34, u8 *) != NULL) {
        AT(AT(set, 0x34, u8 *), 0x28, u8 *) = node;
        AT(node, 0x28, u8 *) = NULL;
        AT(node, 0x2C, u8 *) = AT(set, 0x34, u8 *);
        AT(set, 0x34, u8 *) = node;
    } else {
        AT(set, 0x34, u8 *) = node;
        AT(set, 0x30, u8 *) = node;
        AT(node, 0x2C, u8 *) = NULL;
        AT(node, 0x28, u8 *) = NULL;
    }
}

static inline void Set_AddCollider(u8 *set, u8 *col) {
    AT(col, 0x2C, u8 *) = NULL;
    if (AT(set, 0x18, u8 *) == NULL) {
        AT(set, 0x18, u8 *) = col;
    } else {
        u8 *c = AT(set, 0x18, u8 *);

        while (AT(c, 0x2C, u8 *) != NULL) {
            c = AT(c, 0x2C, u8 *);
        }
        AT(c, 0x2C, u8 *) = col;
    }
}

/* a set's settings: force (x, y, z), damping, its model */
static inline void Set_Init(u8 *set, u8 *m, f32 fx, f32 fy, f32 fz, f32 damp) {
    AT(set, 0x0, f32) = fx;
    AT(set, 0x4, f32) = fy;
    AT(set, 0x8, f32) = fz;
    AT(set, 0x10, f32) = damp;
    AT(set, 0x14, u8 *) = m;
    AT(set, 0x20, u8) = 0;
    AT(set, 0x1C, s32) = 0;
}

/* a capsule collider between bones `b1` and `b2` (ends p1, p2 in their bones' space, radius r) */
void func_002EE530(u8 *cap, s32 b1, s32 b2, f32 x1, f32 y1, f32 z1, f32 r, f32 x2, f32 y2, f32 z2) {
    AT(cap, 0x10, f32) = x1;
    AT(cap, 0x14, f32) = y1;
    AT(cap, 0x18, f32) = z1;
    AT(cap, 0x1C, f32) = 1.0f;
    AT(cap, 0x20, f32) = r;
    AT(cap, 0x24, f32) = 1.0f / r;
    AT(cap, 0x28, s32) = b1;
    AT(cap, 0x50, f32) = x2;
    AT(cap, 0x54, f32) = y2;
    AT(cap, 0x58, f32) = z2;
    AT(cap, 0x5C, f32) = 1.0f;
    AT(cap, 0x60, s32) = b2;
}

/* ---- Daniella's model (vtable D_004702D0, 0x1580 bytes: kinds 3 / 34..36). Her hair: two
   strands of five (+0xDE0, 0x70 each) on the set +0x9A0, kept off her back by five capsules
   (+0x1240) whose shape follows her pose (+0x1578); six hanging parts (+0xAA0) on the set
   +0x9E0; one part (+0x1470) on +0xA20; two (+0x14D0) on +0xA60 ---- */

extern void *D_004702D0[];
extern void *func_001702F0(void *e, s32 flags);
extern void *func_00170EB0(void *e, s32 flags);
extern void *func_00170F30(void *e, s32 flags);
extern void *D_00470440[], *D_004703B0[];

/* +0x8: destructor */
void *func_002ECFD0(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = D_004702D0;
        func_001002C0(m + 0x14D0, func_001702F0, 0x50, 2);
        AT(m, 0x14A0, void **) = D_00470440;
        AT(m, 0x14A0, void **) = D_004703B0;
        func_001002C0(m + 0xDE0, func_00170EB0, 0x70, 0xA);
        func_001002C0(m + 0xAA0, func_00170F30, 0x50, 6);
        HumanModel_Destroy(m, flags);
    }
    return m;
}

/* +0x2C / +0x30: two of her parts' draw flags (+0xBA / +0xD6) and model flag 0x20000 on; off
   again by +0x878 (1 or 2 in +0x880) after the base +0x2C */
void func_002ED160(u8 *m) {
    AT(m, 0xBA, u8) |= 2;
    AT(m, 0xD6, u8) |= 2;
    AT(m, 0x4B0, u32) |= 0x20000;
}

void func_002ED190(u8 *m) {
    VCALL(m, 0x2C, void (*)(u8 *))(m);
    if (AT(m, 0x878, s32) == 0) {
        AT(m, 0xBA, u8) &= 0xFD;
        AT(m, 0x880, s32) = 1;
    } else {
        AT(m, 0xD6, u8) &= 0xFD;
        AT(m, 0x880, s32) = 2;
    }
    AT(m, 0x4B0, u32) &= ~0x20000;
}

extern u8 D_00419E60[];

/* +0xB4: her secondary-motion table */
void func_002ED210(u8 *m) {
    AT(m, 0x874, u8 *) = D_00419E60;
}

/* +0x84 .. +0x90: her mesh parts */
s32 func_002ED220(u8 *m) {
    return 3;
}

s32 func_002ED230(u8 *m) {
    return 7;
}

s32 func_002ED240(u8 *m) {
    return 0x1A;
}

s32 func_002ED250(u8 *m) {
    return 0x2A;
}

/* +0x10 */
void func_002EE110(u8 *m) {
    func_001F7AC0(m);
}

/* her five back capsules (bone 2 to bone 6) by pose: 0 standing, 1 flat, 2 bent (crawling) */
void func_002ED260(u8 *m, s32 pose) {
    static const f32 sCaps[3][5][7] = {
        {
            {0.0f, 1.0f, 1.0f, 0x1.99999ap+0f, 0.0f, 1.0f, -1.0f},
            {1.0f, 1.0f, 1.0f, 0x1.b33334p+0f, 1.0f, 1.0f, -1.0f},
            {2.0f, 1.0f, 1.0f, 0x1.ccccccp+0f, 2.0f, 1.0f, -1.0f},
            {3.0f, 1.0f, 1.0f, 0x1.e66666p+0f, 3.0f, 1.0f, -1.0f},
            {4.0f, 1.0f, 1.0f, 2.0f, 4.0f, 1.0f, -1.0f},
        },
        {
            {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f},
            {2.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f},
            {3.0f, 0.0f, 0.0f, 1.0f, 3.0f, 0.0f, 0.0f},
            {4.0f, 0.0f, 0.0f, 1.0f, 4.0f, 0.0f, 0.0f},
        },
        {
            {0.0f, 1.0f, -1.0f, 2.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, -1.0f, 2.0f, 1.0f, 1.0f, 1.0f},
            {2.0f, 1.0f, -1.0f, 2.0f, 2.0f, 1.0f, 1.0f},
            {3.0f, 1.0f, -1.0f, 2.0f, 3.0f, 1.0f, 1.0f},
            {4.0f, 1.0f, -1.0f, 2.0f, 4.0f, 1.0f, 1.0f},
        },
    };
    const f32 (*c)[7] = sCaps[pose == 1 ? 1 : pose == 2 ? 2 : 0];
    s32 k;

    AT(m, 0x1578, s8) = pose;
    for (k = 0; k < 5; k++) {
        func_002EE530(m + 0x1240 + k * 0x70, 2, 6, c[k][0], c[k][1], c[k][2], c[k][3], c[k][4], c[k][5], c[k][6]);
    }
}

/* the two parts on the set +0xA60 (bones 0x2F, 0x30) */
void func_002ED600(u8 *m) {
    s32 i;

    func_002EE960(m + 0xA60);
    for (i = 0; i < 2; i++) {
        Set_AddLink(m + 0xA60, m + 0x14D0 + i * 0x50);
    }
    Set_Init(m + 0xA60, m, 0.0f, 0x1.99999ap-4f /* 0.1 */, 0.0f, 0x1.fae148p-1f /* 0.99 */);
    AT(m, 0x1510, f32) = 1.0f;
    AT(m, 0x14F4, s32) = 0x2F;
    AT(m, 0x14F0, u8) = 1;
    AT(m, 0x1560, f32) = 1.0f;
    AT(m, 0x1544, s32) = 0x30;
    AT(m, 0x1540, u8) = 1;
}

/* a set's hanging parts put back under their anchors (a bone when +0x20, else the point at
   +0x2C), `len` along `dir`, at rest */
static inline void Parts_Rest(u8 *p, s32 n, s32 size, const f32 *dir, u8 *owner) {
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    s32 i;

    for (i = 0; i < n; i++, p += size) {
        AT(p, 0x18, f32) = 0.0f;
        AT(p, 0x14, f32) = 0.0f;
        AT(p, 0x10, f32) = 0.0f;
        if (AT(p, 0x20, u8) != 0) {
            sceVu0CopyVector(at, func_0017CE80(AT(owner, 0x810, u8 *), AT(p, 0x24, s32)) + 12);
        } else {
            sceVu0CopyVector(at, AT(p, 0x2C, f32 *));
        }
        sceVu0ScaleVector(d, (f32 *)dir, AT(p, 0x40, f32));
        sceVu0AddVector((f32 *)p, at, d);
        sceVu0CopyVector((f32 *)(p + 0x50), at);
    }
}

/* her hair at rest: each point its length (+0x40) along bone 0's Z axis from its anchor */
void func_002ED6E0(u8 *m) {
    f32 down[4] __attribute__((aligned(16)));

    sceVu0CopyVector(down, func_0017CE80(AT(AT(m, 0x9B4, u8 *), 0x810, u8 *), 0) + 8);
    Parts_Rest(m + 0xDE0, 10, 0x70, down, AT(m, 0x9B4, u8 *));
}

/* her hair: the two strands (bones 0xB..0xF and 0x10..0x14), each point tied to the one beside
   it in the other strand, stiffer at the root; the five capsules */
void func_002ED7C0(u8 *m) {
    static const f32 sStiff[5] = {
        0x1.99999ap-1f, 0x1.333334p-1f, 0x1.99999ap-2f, 0x1.99999ap-3f, 0.0f,   /* 0.8 .. 0 */
    };
    s32 i;

    func_002EE960(m + 0x9A0);
    for (i = 0; i < 10; i++) {
        Set_AddLink(m + 0x9A0, m + 0xDE0 + i * 0x70);
    }
    for (i = 0; i < 5; i++) {
        Set_AddCollider(m + 0x9A0, m + 0x1240 + i * 0x70);
    }
    Set_Init(m + 0x9A0, m, 0.0f, 0x1.99999ap-1f /* 0.8 */, 0.0f, 0.5f);
    for (i = 0; i < 10; i++) {
        u8 *n = m + 0xDE0 + i * 0x70;

        AT(n, 0x20, u8) = i % 5 == 0;
        AT(n, 0x24, s32) = 0xB + i;
        AT(n, 0x40, f32) = 0x1.333334p+0f;   /* 1.2 */
        AT(n, 0x44, u8 *) = m + 0xDE0 + (i < 5 ? i + 5 : i - 5) * 0x70;
        AT(n, 0x48, f32) = i < 5 ? -1.0f : 1.0f;
        AT(n, 0x60, f32) = sStiff[i % 5];
    }
    func_002ED260(m, 0);
}

/* the one part on the set +0xA20 (bone 0x17) */
void func_002EDA90(u8 *m) {
    func_002EE960(m + 0xA20);
    Set_AddLink(m + 0xA20, m + 0x1470);
    Set_Init(m + 0xA20, m, 0.0f, 0.0f, 0.0f, 0.75f);
    AT(m, 0x14B0, f32) = 0x1.cccccc0p-1f;   /* 0.9 */
    AT(m, 0x1494, s32) = 0x17;
    AT(m, 0x1490, u8) = 1;
    AT(m, 0x14C8, f32) = 1.0f;
    AT(m, 0x14C4, f32) = 1.0f;
    AT(m, 0x14C0, f32) = 1.0f;
}

/* the six hanging parts (bones 0x22..0x27, two strands of three) on the set +0x9E0, with two
   capsules and two spheres */
void func_002EDB50(u8 *m) {
    static const f32 sStiff[3] = {0x1.99999ap-3f, 0x1.99999ap-4f, 0.0f};   /* 0.2, 0.1, 0 */
    s32 i;

    func_002EE960(m + 0x9E0);
    for (i = 0; i < 6; i++) {
        Set_AddLink(m + 0x9E0, m + 0xAA0 + i * 0x50);
    }
    for (i = 0; i < 2; i++) {
        Set_AddCollider(m + 0x9E0, m + 0xC80 + i * 0x70);
    }
    for (i = 0; i < 2; i++) {
        Set_AddCollider(m + 0x9E0, m + 0xD60 + i * 0x40);
    }
    Set_Init(m + 0x9E0, m, 0.0f, 0x1.333334p-2f /* 0.3 */, 0.0f, 0.5f);
    for (i = 0; i < 6; i++) {
        u8 *n = m + 0xAA0 + i * 0x50;

        AT(n, 0x20, u8) = i % 3 == 0;
        AT(n, 0x24, s32) = 0x22 + i;
        AT(n, 0x40, f32) = 0x1.99999ap-1f;   /* 0.8 */
        AT(n, 0x44, f32) = sStiff[i % 3];
    }
    func_002EE530(m + 0xC80, 0x19, 0x29, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    func_002EE530(m + 0xCF0, 0x17, 0x17, 1.5f, 0.0f, -0.5f, 1.0f, -1.5f, 0.0f, -0.5f);
    func_002EE690(m + 0xD60, 0x1F, 0.0f, 0.0f, 0.0f, 1.0f);
    func_002EE690(m + 0xDA0, 0x1F, 0.0f, 1.0f, 0.0f, 1.0f);
}

/* the six hanging parts at rest: each its length (+0x40) along bone 0x1F's Z axis bent by the
   set's force (+0x44 of it), from its anchor */
void func_002EDE30(u8 *m) {
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u8 *p = m + 0xAA0;
    s32 i;

    for (i = 0; i < 6; i++, p += 0x50) {
        AT(p, 0x18, f32) = 0.0f;
        AT(p, 0x14, f32) = 0.0f;
        AT(p, 0x10, f32) = 0.0f;
        if (AT(p, 0x20, u8) != 0) {
            sceVu0CopyVector(at, func_0017CE80(AT(AT(m, 0x9F4, u8 *), 0x810, u8 *), AT(p, 0x24, s32)) + 12);
        } else {
            sceVu0CopyVector(at, AT(p, 0x2C, f32 *));
        }
        sceVu0CopyVector(d, func_0017CE80(AT(AT(m, 0x9F4, u8 *), 0x810, u8 *), 0x1F) + 8);
        sceVu0ScaleVector(d, d, AT(p, 0x44, f32));
        sceVu0AddVector(d, d, (f32 *)(m + 0x9E0));
        sceVu0Normalize(d, d);
        sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
        sceVu0AddVector((f32 *)p, at, d);
    }
}

/* all her springs */
void func_002EDF30(u8 *m) {
    func_002EDB50(m);
    func_002EDA90(m);
    func_002ED7C0(m);
    func_002ED600(m);
    AT(m, 0x850, u8) = 1;
}

/* +0x3C: her springs a frame. While she crawls (0x1800..0x1803) the capsules bend and her hair
   falls forward (the force -0.4 along her facing); after a reset (+0x850) everything back at
   rest and settled (+0x1570 / +0x1574 steps) */
void func_002EDF80(u8 *m) {
    s32 n = 1, nHair = 1;
    s32 i;

    if ((u32)(AT(m, 0x55C, s32) - 0x1800) < 4) {
        if (AT(m, 0x1578, s8) != 2) {
            func_002ED260(m, 2);
            AT(m, 0x9A0, f32) = -0x1.99999ap-2f * AT(m, 0x7F0, f32);
            AT(m, 0x9A8, f32) = -0x1.99999ap-2f * AT(m, 0x7F8, f32);
        }
    } else if (AT(m, 0x1578, s8) == 2) {
        func_002ED260(m, 0);
        AT(m, 0x9A0, s32) = 0;
        AT(m, 0x9A8, s32) = 0;
    }
    if (AT(m, 0x850, u8) != 0) {
        func_002ED6E0(m);
        func_002EDE30(m);
        nHair = AT(m, 0x1574, s32);
        n = AT(m, 0x1570, s32);
    }
    func_002EE8A0(m + 0x9E0);
    func_002EE8A0(m + 0xA20);
    func_002EE8A0(m + 0x9A0);
    func_002EE8A0(m + 0xA60);
    for (i = 0; i < n; i++) {
        func_002EE900(m + 0x9E0);
        func_002EE900(m + 0xA20);
        func_002EE900(m + 0xA60);
    }
    for (i = 0; i < nHair; i++) {
        func_002EE900(m + 0x9A0);
    }
    func_002EE840(m + 0x9E0);
    func_002EE840(m + 0xA20);
    func_002EE840(m + 0x9A0);
    func_002EE840(m + 0xA60);
    AT(m, 0x850, u8) = 0;
}

/* +0xC: once loaded: the base setup, the part roles, per-part draw settings, her springs */
void func_002EE120(u8 *m) {
    func_002118D0(m);
    AT(m, 0x890, s32) = 2;
    AT(m, 0x894, s32) = 3;
    AT(m, 0x898, s32) = 4;
    AT(m, 0x89C, s32) = 5;
    AT(m, 0x8B8, s32) = 0x1C;
    AT(m, 0x8A0, s32) = 6;
    AT(m, 0x8A4, s32) = 7;
    AT(m, 0x8A8, s32) = 8;
    AT(m, 0x8AC, s32) = 9;
    AT(m, 0x8BC, s32) = 0x2C;
    AT(m, 0x8B0, s32) = 0x1F;
    AT(m, 0x8B4, s32) = 0x15;
    AT(m, 0x860, f32) = 0.0f;
    AT(m, 0x864, f32) = 16.0f;
    AT(m, 0x868, f32) = 0.0f;
    AT(m, 0x854, s32) = 0;
    AT(m, 0x858, s32) = 0;
    {
        static const u8 sParts[][2] = {
            {0x98, 0x40}, {0xCC, 0x40}, {0xDA, 0x40}, {0xDC, 0x40}, {0xDE, 0x40},
            {0xD2, 0xC0}, {0xBA, 0xFF}, {0xD6, 0xFF},
        };
        u32 i;

        for (i = 0; i < sizeof(sParts) / sizeof(sParts[0]); i++) {
            AT(m, sParts[i][0], u8) = 4;
            AT(m, sParts[i][0] + 1, u8) = sParts[i][1];
        }
    }
    AT(m, 0x1570, s32) = 50;
    AT(m, 0x1574, s32) = 50;
    func_002EDF30(m);
}

/* ---- the capsule collider (vtable D_00470390, +0x30 in a 0x70 part): ends +0 / +0x40 in the
   world, +0x10 / +0x50 in their bones' (+0x28 / +0x60) space, radius +0x20 (1 / it +0x24) ---- */

/* +0x8: the push on point `pt` (into `out`), `k` times its depth: off the segment when it's
   alongside, else off the nearer end (not normalized there, as the original) */
void func_002EE220(u8 *cap, f32 *out, const f32 *pt, f32 k) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 d1[4] __attribute__((aligned(16)));
    f32 t0, t1, lo, hi, tp, l2;

    sceVu0SubVector(d, (f32 *)(cap + 0x40), (f32 *)cap);
    sceVu0Normalize(axis, d);
    t0 = sceVu0InnerProduct(axis, (f32 *)cap);
    t1 = sceVu0InnerProduct(axis, (f32 *)(cap + 0x40));
    if (t0 <= t1) {
        lo = t0;
        hi = t1;
    } else {
        lo = t1;
        hi = t0;
    }
    tp = sceVu0InnerProduct(axis, (f32 *)pt);
    if (!(tp < lo) && tp <= hi) {
        f32 s;

        sceVu0SubVector(d, (f32 *)pt, (f32 *)cap);
        s = sceVu0InnerProduct(axis, d);
        d[0] = d[0] - s * axis[0];
        d[1] = d[1] - s * axis[1];
        d[2] = d[2] - s * axis[2];
        l2 = sceVu0InnerProduct(d, d);
        if (l2 < AT(cap, 0x20, f32) * AT(cap, 0x20, f32) && !(l2 <= 0.0f)) {
            sceVu0Normalize(d, d);
            sceVu0ScaleVector(out, d, (1.0f - __builtin_sqrtf(l2) * AT(cap, 0x24, f32)) * k);
            return;
        }
    }
    sceVu0SubVector(d, (f32 *)pt, (f32 *)cap);
    l2 = sceVu0InnerProduct(d, d);
    sceVu0SubVector(d1, (f32 *)pt, (f32 *)(cap + 0x40));
    {
        f32 l1 = sceVu0InnerProduct(d1, d1);

        if (!(l2 <= l1)) {
            l2 = l1;
            sceVu0CopyVector(d, d1);
        }
    }
    if (l2 < AT(cap, 0x20, f32) * AT(cap, 0x20, f32) && !(l2 <= 0.0f)) {
        sceVu0ScaleVector(out, d, (1.0f - __builtin_sqrtf(l2) * AT(cap, 0x24, f32)) * k);
        return;
    }
    AT(out, 0x0, s32) = 0;
    AT(out, 0x4, s32) = 0;
    AT(out, 0x8, s32) = 0;
    AT(out, 0xC, s32) = 0;
}

/* ---- Daniella's hair point (vtable D_00470460, +0x30 in a 0x70 part): the position +0, its
   velocity +0x10, anchored (+0x20) to bone +0x24 or the point +0x2C, its length +0x40, the
   partner point in the other strand +0x44 (side +0x48), the anchor last frame +0x50, its
   stiffness +0x60 ---- */

extern void sceVu0OuterProduct(f32 *out, const f32 *a, const f32 *b);

/* +0x14: its bone's matrix from the point: X toward the point, Y toward the partner, at the
   anchor */
void func_002EEF90(u8 *pt, u8 *set) {
    f32 *mtx = func_0017CE80(AT(AT(set, 0x14, u8 *), 0x810, u8 *), AT(pt, 0x24, s32));
    f32 at[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));

    if (AT(pt, 0x20, u8) != 0) {
        sceVu0CopyVector(at, mtx + 12);
    } else {
        sceVu0CopyVector(at, AT(pt, 0x2C, f32 *));
    }
    sceVu0SubVector(side, AT(pt, 0x44, f32 *), (f32 *)pt);
    sceVu0ScaleVector(side, side, AT(pt, 0x48, f32));
    sceVu0Normalize(side, side);
    sceVu0SubVector(mtx, (f32 *)pt, at);
    sceVu0CopyVector(mtx + 4, side);
    sceVu0OuterProduct(mtx + 8, mtx, mtx + 4);
    sceVu0OuterProduct(mtx + 4, mtx + 8, mtx);
    sceVu0Normalize(mtx, mtx);
    sceVu0Normalize(mtx + 4, mtx + 4);
    sceVu0Normalize(mtx + 8, mtx + 8);
    sceVu0CopyVector(mtx + 12, at);
}

/* +0x10: a step: follow the anchor by the stiffness, the set's force, pushed off the colliders
   (3 times their depth), kept about 0.438 from its partner, damped, held at its length from
   the anchor */
void func_002EF0A0(u8 *pt, u8 *set) {
    f32 *mtx = func_0017CE80(AT(AT(set, 0x14, u8 *), 0x810, u8 *), AT(pt, 0x24, s32));
    f32 at[4] __attribute__((aligned(16)));
    f32 prev[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 *vel = (f32 *)(pt + 0x10);
    u8 *c;
    f32 len;

    if (AT(pt, 0x20, u8) != 0) {
        sceVu0CopyVector(at, mtx + 12);
    } else {
        sceVu0CopyVector(at, AT(pt, 0x2C, f32 *));
    }
    sceVu0CopyVector(prev, (f32 *)pt);
    sceVu0SubVector(d, at, (f32 *)(pt + 0x50));
    sceVu0ScaleVector(d, d, AT(pt, 0x60, f32));
    sceVu0AddVector((f32 *)pt, (f32 *)pt, d);
    sceVu0SubVector(vel, vel, (f32 *)set);
    for (c = AT(set, 0x18, u8 *); c != NULL; c = AT(c, 0x2C, u8 *)) {
        (*(void (**)(u8 *, f32 *, u8 *, f32))(AT(c, 0x30, u8 *) + 8))(c, d, pt, 3.0f);
        sceVu0AddVector(vel, vel, d);
    }
    sceVu0SubVector(d, (f32 *)pt, AT(pt, 0x44, f32 *));
    len = __builtin_sqrtf(sceVu0InnerProduct(d, d));
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, 0x1.99999ap-1f * -(len - 0x1.c068dcp-2f));   /* 0.8, 0.4379 */
    sceVu0AddVector(vel, vel, d);
    sceVu0ScaleVector(vel, vel, AT(set, 0x10, f32));
    sceVu0AddVector((f32 *)pt, (f32 *)pt, vel);
    sceVu0SubVector(d, (f32 *)pt, at);
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, AT(pt, 0x40, f32));
    sceVu0AddVector((f32 *)pt, at, d);
    sceVu0SubVector(vel, (f32 *)pt, prev);
    sceVu0CopyVector((f32 *)(pt + 0x50), at);
}

extern void sceVu0ApplyMatrix(f32 *out, f32 (*m)[4], const f32 *v);

/* the capsule's +0xC: its ends from their bones */
void func_002EE4B0(u8 *cap, u8 *m) {
    f32 mtx[4][4] __attribute__((aligned(16)));

    sceVu0CopyMatrix(mtx, (f32 (*)[4])func_0017CE80(AT(m, 0x810, u8 *), AT(cap, 0x28, s32)));
    sceVu0ApplyMatrix((f32 *)cap, mtx, (f32 *)(cap + 0x10));
    sceVu0CopyMatrix(mtx, (f32 (*)[4])func_0017CE80(AT(m, 0x810, u8 *), AT(cap, 0x60, s32)));
    sceVu0ApplyMatrix((f32 *)(cap + 0x40), mtx, (f32 *)(cap + 0x50));
}

/* ---- Riccardo's model (vtable D_00470480, 0x1490 bytes: kind 4): twelve 0x60 parts
   (+0xC20, pairs: the second and third pair of each six hang off the first) on the set
   +0x10A0 with five capsules (+0x10E0); four parts (+0x9A0) on +0xBE0 with four spheres
   (+0xAE0); four (+0x1310) on +0x1450 ---- */

extern void *D_00470480[];
extern void *func_00170CB0(void *e, s32 flags);
extern void *func_0016FB00(void *e, s32 flags);

/* +0x8: destructor */
void *func_002F61E0(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = D_00470480;
        func_001002C0(m + 0x1310, func_00170CB0, 0x50, 4);
        func_001002C0(m + 0xC20, func_0016FB00, 0x60, 0xC);
        func_001002C0(m + 0x9A0, func_0016FBB0, 0x50, 4);
        HumanModel_Destroy(m, flags);
    }
    return m;
}

/* +0x2C / +0x30: part +0xBA's draw flag 2 on / off */
void func_002F6340(u8 *m) {
    AT(m, 0xBA, u8) |= 2;
}

void func_002F6350(u8 *m) {
    AT(m, 0xBA, u8) &= 0xFD;
}

extern u8 D_0041A2C0[];

/* +0xB4: his secondary-motion table */
void func_002F6360(u8 *m) {
    AT(m, 0x874, u8 *) = D_0041A2C0;
}

/* +0x98 / +0x9C, +0x84 .. +0x90: his mesh parts */
s32 func_002F6370(u8 *m) {
    return 0x31;
}

s32 func_002F6380(u8 *m) {
    return 0x30;
}

s32 func_002F6390(u8 *m) {
    return 3;
}

s32 func_002F63A0(u8 *m) {
    return 7;
}

s32 func_002F63B0(u8 *m) {
    return 0x1E;
}

s32 func_002F63C0(u8 *m) {
    return 0x2C;
}

/* the four parts on +0x1450 (bones 0x26..0x29, two of two) */
void func_002F63D0(u8 *m) {
    s32 i;

    func_002EE960(m + 0x1450);
    for (i = 0; i < 4; i++) {
        Set_AddLink(m + 0x1450, m + 0x1310 + i * 0x50);
    }
    Set_Init(m + 0x1450, m, 0.0f, 0x1.99999ap-4f /* 0.1 */, 0.0f, 0x1.99999ap-1f /* 0.8 */);
    for (i = 0; i < 4; i++) {
        u8 *n = m + 0x1310 + i * 0x50;

        AT(n, 0x40, f32) = 0x1.570a3ep-1f;   /* 0.67 */
        AT(n, 0x24, s32) = 0x26 + i;
        AT(n, 0x20, u8) = i % 2 == 0;
        AT(n, 0x44, f32) = 0x1.99999ap-2f;   /* 0.4 */
    }
}

/* the twelve 0x60 parts at rest: their length along bone 1's Z axis (the second six the other
   way) from their anchors */
void func_002F64F0(u8 *m) {
    f32 down[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u8 *p = m + 0xC20;
    s32 i;

    sceVu0CopyVector(down, func_0017CE80(AT(AT(m, 0x10B4, u8 *), 0x810, u8 *), 1) + 8);
    for (i = 0; i < 12; i++, p += 0x60) {
        AT(p, 0x18, f32) = 0.0f;
        AT(p, 0x14, f32) = 0.0f;
        AT(p, 0x10, f32) = 0.0f;
        if (AT(p, 0x20, u8) != 0) {
            sceVu0CopyVector(at, func_0017CE80(AT(AT(m, 0x10B4, u8 *), 0x810, u8 *), AT(p, 0x24, s32)) + 12);
        } else {
            sceVu0CopyVector(at, AT(p, 0x2C, f32 *));
        }
        if (i < 6) {
            sceVu0ScaleVector(d, down, AT(p, 0x40, f32));
        } else {
            sceVu0ScaleVector(d, down, -AT(p, 0x40, f32));
        }
        sceVu0AddVector((f32 *)p, at, d);
        sceVu0CopyVector((f32 *)(p + 0x50), at);
    }
}

/* the twelve 0x60 parts (bones 0xA..0x15) on +0x10A0 and its five capsules */
void func_002F6600(u8 *m) {
    s32 i;

    func_002EE960(m + 0x10A0);
    for (i = 0; i < 12; i++) {
        Set_AddLink(m + 0x10A0, m + 0xC20 + i * 0x60);
    }
    for (i = 0; i < 5; i++) {
        Set_AddCollider(m + 0x10A0, m + 0x10E0 + i * 0x70);
    }
    Set_Init(m + 0x10A0, m, 0.0f, 0x1.99999ap-4f /* 0.1 */, 0.0f, 0x1.99999ap-1f /* 0.8 */);
    for (i = 0; i < 12; i++) {
        u8 *n = m + 0xC20 + i * 0x60;
        s32 j = i % 6;

        AT(n, 0x20, u8) = i % 2 == 0;
        AT(n, 0x24, s32) = 0xA + i;
        AT(n, 0x40, f32) = 0x1.19999ap+0f;   /* 1.1 */
        if (j < 2) {
            AT(n, 0x44, f32) = 0.0f;
            AT(n, 0x48, u8 *) = NULL;
        } else {
            AT(n, 0x44, f32) = 0.5f;
            AT(n, 0x48, u8 *) = m + 0xC20 + (i - j + j % 2) * 0x60;
        }
    }
    func_002EE530(m + 0x10E0, 2, 6, -0.5f, 0.0f, -0x1.99999ap-4f, 1.0f, -0.5f, 0.0f, 0x1.99999ap-4f);
    func_002EE530(m + 0x1150, 2, 6, 0.0f, 0.0f, -0x1.99999ap-4f, 1.0f, 0.0f, 0.0f, 0x1.99999ap-4f);
    func_002EE530(m + 0x11C0, 2, 6, 0.5f, 0.0f, -0x1.99999ap-4f, 1.0f, 0.5f, 0.0f, 0x1.99999ap-4f);
    func_002EE530(m + 0x1230, 2, 6, 1.0f, 0.0f, -0x1.99999ap-4f, 1.0f, 1.0f, 0.0f, 0x1.99999ap-4f);
    func_002EE530(m + 0x12A0, 2, 6, 0.0f, -0x1.333334p-2f, -0x1.99999ap-4f, 1.0f, 0.0f, -0x1.333334p-2f, 0x1.99999ap-4f);
}

/* the four parts (bones 0x16..0x19, two of two) on +0xBE0 and its four spheres on bone 2 */
void func_002F69C0(u8 *m) {
    s32 i;

    func_002EE960(m + 0xBE0);
    for (i = 0; i < 4; i++) {
        Set_AddLink(m + 0xBE0, m + 0x9A0 + i * 0x50);
    }
    for (i = 0; i < 4; i++) {
        Set_AddCollider(m + 0xBE0, m + 0xAE0 + i * 0x40);
    }
    Set_Init(m + 0xBE0, m, 0.0f, 0.5f, 0.0f, 0x1.99999ap-2f /* 0.4 */);
    for (i = 0; i < 4; i++) {
        u8 *n = m + 0x9A0 + i * 0x50;

        AT(n, 0x40, f32) = 0x1.028f5cp+0f;   /* 1.01 */
        AT(n, 0x24, s32) = 0x16 + i;
        AT(n, 0x20, u8) = i % 2 == 0;
    }
    func_002EE690(m + 0xAE0, 2, -0x1.99999ap-1f, 0.0f, 0.0f, 1.0f);
    func_002EE690(m + 0xB20, 2, 0.0f, 0.0f, 0.0f, 1.0f);
    func_002EE690(m + 0xB60, 2, 0x1.99999ap-1f, 0.0f, 0.0f, 1.0f);
    func_002EE690(m + 0xBA0, 2, 0x1.99999ap+0f, 0.0f, 0.0f, 1.0f);
}

/* all his springs */
void func_002F6BD0(u8 *m) {
    AT(m, 0x850, u8) = 1;
    func_002F69C0(m);
    func_002F6600(m);
    func_002F63D0(m);
}

/* +0x3C: his springs a frame: one step, or after a reset (+0x850) at rest and 30 to settle */
void func_002F6C10(u8 *m) {
    s32 n = 1;
    s32 i;

    if (AT(m, 0x850, u8) != 0) {
        func_002F64F0(m);
        n = 30;
    }
    func_002EE8A0(m + 0xBE0);
    func_002EE8A0(m + 0x10A0);
    func_002EE8A0(m + 0x1450);
    for (i = 0; i < n; i++) {
        func_002EE900(m + 0xBE0);
        func_002EE900(m + 0x10A0);
        func_002EE900(m + 0x1450);
    }
    func_002EE840(m + 0xBE0);
    func_002EE840(m + 0x10A0);
    func_002EE840(m + 0x1450);
    AT(m, 0x850, u8) = 0;
}

/* +0x10 */
void func_002F6CD0(u8 *m) {
    func_001F7AC0(m);
}

/* +0xC: once loaded: the base setup, the part roles, his springs, per-part draw settings */
void func_002F6CE0(u8 *m) {
    func_002118D0(m);
    AT(m, 0x890, s32) = 2;
    AT(m, 0x894, s32) = 3;
    AT(m, 0x898, s32) = 4;
    AT(m, 0x89C, s32) = 5;
    AT(m, 0x8B8, s32) = 0x20;
    AT(m, 0x8A0, s32) = 6;
    AT(m, 0x8A4, s32) = 7;
    AT(m, 0x8A8, s32) = 8;
    AT(m, 0x8AC, s32) = 9;
    AT(m, 0x8BC, s32) = 0x2E;
    AT(m, 0x8B0, s32) = 0x23;
    AT(m, 0x8B4, s32) = 0x1A;
    AT(m, 0x880, s32) = 8;
    AT(m, 0x860, f32) = 0.0f;
    AT(m, 0x864, f32) = 16.0f;
    AT(m, 0x868, f32) = 0.0f;
    AT(m, 0x854, s32) = 0;
    AT(m, 0x858, s32) = 0;
    func_002F6BD0(m);
    AT(m, 0xC8, u8) = 4;
    AT(m, 0xC9, u8) = 0x40;
    AT(m, 0xCA, u8) = 4;
    AT(m, 0xCB, u8) = 0x40;
    AT(m, 0xCC, u8) = 4;
    AT(m, 0xCD, u8) = 0x40;
    AT(m, 0xBA, u8) = 4;
    AT(m, 0xBB, u8) = 0xC0;
}

/* ---- Riccardo's parts ---- */

/* a part's anchor: its bone's position when anchored (+0x20), else the point it hangs from */
static inline void Part_Anchor(u8 *p, u8 *set, f32 *at) {
    if (AT(p, 0x20, u8) != 0) {
        sceVu0CopyVector(at, func_0017CE80(AT(AT(set, 0x14, u8 *), 0x810, u8 *), AT(p, 0x24, s32)) + 12);
    } else {
        sceVu0CopyVector(at, AT(p, 0x2C, f32 *));
    }
}

/* a part kept at its length from its anchor; its velocity is how far it went */
static inline void Part_Hold(u8 *p, const f32 *at, const f32 *prev) {
    f32 d[4] __attribute__((aligned(16)));

    sceVu0SubVector(d, (f32 *)p, (f32 *)at);
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
    sceVu0AddVector((f32 *)p, (f32 *)at, d);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)p, (f32 *)prev);
}

/* +0x10 of his 0x60 parts (vtable D_00473810): follow the anchor (0.6), the set's force, the
   colliders (strength 1), held about 1.9 from the part it's tied to (+0x48, by +0x44), damped,
   at its length */
void func_0031EB70(u8 *p, u8 *set) {
    f32 at[4] __attribute__((aligned(16)));
    f32 prev[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 *vel = (f32 *)(p + 0x10);
    u8 *c;

    Part_Anchor(p, set, at);
    sceVu0SubVector(d, at, (f32 *)(p + 0x50));
    sceVu0ScaleVector(d, d, 0x1.333334p-1f);   /* 0.6 */
    sceVu0AddVector((f32 *)p, (f32 *)p, d);
    sceVu0CopyVector(prev, (f32 *)p);
    sceVu0SubVector(vel, vel, (f32 *)set);
    for (c = AT(set, 0x18, u8 *); c != NULL; c = AT(c, 0x2C, u8 *)) {
        (*(void (**)(u8 *, f32 *, u8 *, f32))(AT(c, 0x30, u8 *) + 8))(c, d, p, 1.0f);
        sceVu0AddVector(vel, vel, d);
    }
    if (AT(p, 0x48, f32 *) != NULL) {
        f32 len;

        sceVu0SubVector(d, AT(p, 0x48, f32 *), (f32 *)p);
        len = __builtin_sqrtf(sceVu0InnerProduct(d, d));
        sceVu0ScaleVector(d, d, AT(p, 0x44, f32) * (len - 0x1.e66666p+0f /* 1.9 */) / len);
        sceVu0AddVector(vel, vel, d);
    }
    sceVu0ScaleVector(vel, vel, AT(set, 0x10, f32));
    sceVu0AddVector((f32 *)p, (f32 *)p, vel);
    Part_Hold(p, at, prev);
    sceVu0CopyVector((f32 *)(p + 0x50), at);
}

/* +0x10 of his 0x50 parts (vtable D_004737F0): drawn toward where its bone points (its length
   out along the bone's X axis, by +0x44), the set's force, damped, at its length */
void func_0031EA10(u8 *p, u8 *set) {
    f32 prev[4] __attribute__((aligned(16)));
    f32 t[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 *vel = (f32 *)(p + 0x10);
    f32 *mtx;

    sceVu0CopyVector(prev, (f32 *)p);
    sceVu0SubVector(vel, vel, (f32 *)set);
    mtx = func_0017CE80(AT(AT(set, 0x14, u8 *), 0x810, u8 *), AT(p, 0x24, s32));
    t[0] = AT(p, 0x40, f32);
    t[3] = 1.0f;
    t[1] = 0.0f;
    t[2] = 0.0f;
    sceVu0ApplyMatrix(t, (f32 (*)[4])mtx, t);
    sceVu0SubVector(t, t, (f32 *)p);
    sceVu0ScaleVector(t, t, AT(p, 0x44, f32));
    sceVu0AddVector(vel, vel, t);
    sceVu0ScaleVector(vel, vel, AT(set, 0x10, f32));
    sceVu0AddVector((f32 *)p, (f32 *)p, vel);
    Part_Anchor(p, set, at);
    Part_Hold(p, at, prev);
}
