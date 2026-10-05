/* The stalkers' models (the character model classes their loaders build, model.c), and the
 * trivial methods of the model base class (vtable D_0046F9E0) that natively were still stubs.
 * The models' secondary motion (springs for hanging parts) works as Fiona's (model.c). */
#include "common.h"
#include "game.h"
#include "model.h"
#include "sce/libvu0.h"

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
