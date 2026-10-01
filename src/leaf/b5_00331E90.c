#include "common.h"
extern u8 *gCharPlayer; /* global manager object */
#include "ptmf.h"
#include "progress.h"

/* Field access by byte offset into objects whose layout is not yet known. */
#define S16(p, off) (*(s16 *)((u8 *)(p) + (off)))
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))
#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
#define PTR(p, off) (*(void * *)((u8 *)(p) + (off)))
/* gFileLoader vtable +0xC: start loading file `name` into `dest` (flags 0x4000000) */
#define FILE_LOAD_ASYNC(name, dest) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, void *, void *, u32, s32))(gFileLoader, name, dest, 0x4000000, 0)

extern u8 D_0042F270[];
extern u8 D_0042F380[];
extern u8 D_00460FE0[];
extern u8 D_00461000[];
extern u8 D_00461020[];
extern u8 D_00461040[];
extern u8 D_00461060[];
extern u8 D_00461080[];
extern u8 D_004610A0[];
extern u8 D_004610C0[];
extern u8 D_004610E0[];
extern u8 D_00461100[];
extern u8 D_00461120[];
extern u8 D_00461140[];
extern u8 D_00461160[];
extern u8 D_00461180[];
extern u8 D_004611A0[];
extern u8 D_004611C0[];
extern u8 D_004611E0[];
extern u8 D_00461200[];
extern u8 D_00461240[];
extern u8 D_00461260[];
extern u8 D_00461280[];
extern u8 D_004612A0[];
extern u8 D_004612C0[];
extern u8 D_004612E0[];
extern u8 D_00461300[];
extern u8 D_00461320[];
extern u8 D_00461340[];
extern u8 D_00461380[];
extern u8 D_004613A0[];
extern u8 D_004613C0[];
extern void *gFileLoader;

/* gCharPlayer +0x1AD5F4: f32 clamped to 0..100; +0x1AD5F8: s32 clamped to 0..1800 */
static inline void b5_adjust_meters(f32 df, s32 di) {
    u8 *g = gCharPlayer;
    f32 f = F32(g, 0x1AD5F4) + df;

    F32(g, 0x1AD5F4) = f;
    if (f < 0.0f) {
        F32(g, 0x1AD5F4) = 0.0f;
    } else if (!(f <= 100.0f)) {
        F32(g, 0x1AD5F4) = 100.0f;
    }
    S32(g, 0x1AD5F8) += di;
    if (S32(g, 0x1AD5F8) < 0) {
        S32(g, 0x1AD5F8) = 0;
    } else if (S32(g, 0x1AD5F8) > 1800) {
        S32(g, 0x1AD5F8) = 1800;
    }
}

u32 func_00331E90(void) {
    return 0x80000005;
}

s32 func_00331EB0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00460FE0, dest);
}

u32 func_00331FE0(void) {
    return 0x80000005;
}

s32 func_00332000(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00461000, dest);
}

u32 func_00332190(void) {
    return 0x80000005;
}

s32 func_003321B0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00461020, dest);
}

s32 func_003321E0(void) {
    u8 *p;

    b5_adjust_meters(-100.0f, -1800);
    p = (u8 *)gProgress;
    S32(p, 0x9FC) = 0;
    S32(p, 0xA00) = 1800;
    F32(p, 0xA04) = 3.0f;
    S32(p, 0xA08) = 1800;
    return 1;
}

u32 func_00332350(void) {
    return 0x80000005;
}

s32 func_00332370(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00461040, dest);
}

s32 func_003323A0(void) {
    S32((u8 *)gProgress, 0xA0C) = 1800;
    return 1;
}

s32 func_00332470(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00461060, dest);
}

s32 func_003326A0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00461080, dest);
}

s32 func_003328B0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_004610A0, dest);
}

u32 func_00332A80(void) {
    return 0x80000005;
}

s32 func_00332AA0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_004610C0, dest);
}

u32 func_00332CC0(void) {
    return 0x80000005;
}

s32 func_00332CE0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_004610E0, dest);
}

u32 func_00332EE0(void) {
    return 0x80000005;
}

s32 func_00332F00(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00461100, dest);
}

u32 func_003330D0(void) {
    return 0x80000005;
}

s32 func_003330F0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00461120, dest);
}

void *func_00336F00(void) {
    return D_00461240;
}

void *func_00336F10(void) {
    return D_00461260;
}

u32 func_00336F20(void) {
    return 0x41000;
}

void func_00336F30(u8 *self, s32 on) {
    s32 i;

    for (i = 0; i < 3; i++) {
        self[0x167C + i * 0x50] = on != 0;
    }
}

void func_00336F70(u8 *self, f32 x, f32 y, f32 z) {
    F32(self, 0x1720) = x;
    F32(self, 0x1724) = y;
    F32(self, 0x1728) = z;
}

void func_00336FD0(u8 *self, f32 x, f32 y, f32 z) {
    F32(self, 0xE90) = x;
    F32(self, 0xE94) = y;
    F32(self, 0xE98) = z;
}

void *func_003376D0(void *self, u32 i) {
    switch (i) {
    case 0: return D_00461140;
    case 1: return D_00461160;
    case 2: return D_00461180;
    case 3: return D_004611A0;
    case 4: return D_004611C0;
    case 5: return D_004611E0;
    case 6: return D_00461200;
    }
    return NULL;
}

void func_00337E00(u8 *self) {
    S16(self, 0x840) = 16;
    PTR(self, 0x844) = D_0042F270;
}

void *func_00337F70(void) {
    return D_00461380;
}

void *func_00337F80(void) {
    return D_004613A0;
}

u32 func_00337F90(void) {
    return 0x41000;
}

void func_00337FA0(u8 *self, s32 on) {
    s32 i;

    for (i = 0; i < 3; i++) {
        self[0x153C + i * 0x50] = on != 0;
    }
}

void func_00338030(u8 *self, f32 x, f32 y, f32 z) {
    F32(self, 0xD50) = x;
    F32(self, 0xD54) = y;
    F32(self, 0xD58) = z;
}

void *func_00338560(void *self, u32 i) {
    switch (i) {
    case 0: return D_00461280;
    case 1: return D_004612A0;
    case 2: return D_004612C0;
    case 3: return D_004612E0;
    case 4: return D_00461300;
    case 5: return D_00461320;
    case 6: return D_00461340;
    }
    return NULL;
}

void func_00338C00(u8 *self) {
    S16(self, 0x840) = 16;
    PTR(self, 0x844) = D_0042F380;
}

s32 func_00338F70(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_004613C0, dest);
}
