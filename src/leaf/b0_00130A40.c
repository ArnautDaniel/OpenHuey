/* Leaf functions (batch 0), 0x00130A40 - 0x00171090. */
#include "common.h"
#include "ptmf.h"

#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern f32 D_003B1B48;
extern f32 D_003B1B4C;
extern f32 D_003B1B50;
extern void *gCharacters[6]; /* registered room objects, by kind */
extern void *gCharPlayer;
extern void *gCharPartner;
extern void *gCharPursuer;
extern void *D_00469C20[];
extern void *D_00469C60[];
extern void *D_00469D00[];
extern void *D_0046ADA0[];
extern void *D_0046B0D0[];
extern void *D_0046B1C0[];
extern void *D_0046D810[];
extern void *D_004702B0[];
extern void *D_00470390[];
extern void *D_004703A0[];
extern void *D_004703D0[];
extern void *D_00470440[];
extern void *D_00470460[];
extern void *D_00470700[];
extern void *D_00472350[];
extern void *D_00472C10[];
extern void *D_004737F0[];
extern void *D_00473810[];

void func_00130A40(u8 *p, u32 id, u32 b, s32 flag, f32 f) {
    p[5] = (u8)id;
    if ((u8)id != 0xFF) {
        FLD(p, 0x10, f32) = f;
        if (flag != 0) {
            p[4] = 0xFF;
        }
    }
    p[0x18] = (u8)b;
}

/* Memory block list: +0x4 base, +0x8 size, +0xC block table (16 bytes each), +0x10 count. */
typedef struct B0_Block {
    s32 used;
    s32 index;
    u32 addr;
    u32 size;
} B0_Block;

typedef struct B0_Heap {
    void **vtbl;
    u32 base;
    u32 size;
    B0_Block *blocks;
    u32 count;
} B0_Heap;

/* Store the parameters, then tail-call virtual +0xC (Reset) with them. */
/* LZSS decompression: +0x18 source (4-byte header), +0x1C destination.
 * Flag bit 1 = literal byte, 0 = 16-bit back-reference (len = low 4 bits + 2,
 * distance = high 12 bits); a zero reference ends the stream. */
void func_001695D0(u8 *p) {
    u8 *src = FLD(p, 0x18, u8 *) + 4;
    u8 *dst = FLD(p, 0x1C, u8 *);
    u32 bits = 1;
    u32 flags = 0;

    for (;;) {
        bits--;
        flags >>= 1;
        if (bits == 0) {
            flags = *src++;
            bits = 8;
        }
        if (flags & 1) {
            *dst++ = *src++;
        } else {
            u32 w = src[0] | (src[1] << 8);
            u32 dist, len;

            src += 2;
            if (w == 0) {
                return;
            }
            len = (w & 0xF) + 2;
            dist = w >> 4;
            do {
                *dst = *(dst - dist);
                dst++;
            } while (--len != 0);
        }
    }
}

/* Entries of 0x128 bytes; +0x12804 current index, +0x12805 end index. */
s32 func_0016B420(u8 *p) {
    s32 v = FLD(p + p[0x12804] * 0x128, 0x8, s32);

    return v == 6 || v == 7;
}

s32 func_0016B470(u8 *p, s32 kind) {
    u8 end = p[0x12805];
    u8 i = p[0x12804];

    if (i == end) {
        return 3;
    }
    for (; i != end; i++) {
        if (FLD(p + i * 0x128, 0x18, s32) == kind) {
            return 2;
        }
    }
    return 3;
}

s32 func_0016B510(u8 *p) {
    if (p[0x12804] == p[0x12805]) {
        return 3;
    }
    return 2;
}

/* Tail call of the sub-object's virtual +0x14 with the value at +0x73EDC0. */
void func_0016CD30(u8 *p, s32 a1, s32 a2, s32 a3) {
    void *obj = p + 0x6FBF00;

    VCALL(obj, 0x14, void (*)(void *, s32, s32, s32))(obj, FLD(p, 0x73EDC0, s32), a2, a3);
}

s32 Characters_Register(void *self, u32 kind, void *obj) {
    if (kind < 6 && gCharacters[kind] == NULL) {
        gCharacters[kind] = obj;
        FLD(gCharacters[kind], 0x20, u32) = kind;
        switch (kind) {
        case 0:
            gCharPlayer = obj;
            break;
        case 1:
            gCharPartner = obj;
            break;
        case 2:
            gCharPursuer = obj;
            break;
        }
        return 1;
    }
    return 0;
}

void *func_0016F740(u8 *p) {
    FLD(p, 0x0, void **) = D_00469D00;
    FLD(p, 0x4, s32) = -1;
    FLD(p, 0x0, void **) = D_0046B1C0;
    FLD(p, 0x2E0, s32) = 0;
    return p;
}

void *func_0016FAE0(u8 *p) {
    FLD(p, 0x30, void **) = D_00470390;
    return p;
}

void *func_0016FB60(u8 *p) {
    FLD(p, 0x30, void **) = D_00473810;
    return p;
}

void *func_0016FB80(u8 *p) {
    FLD(p, 0x34, s32) = 0;
    FLD(p, 0x30, s32) = 0;
    return p;
}

void *func_0016FB90(u8 *p) {
    FLD(p, 0x30, void **) = D_004703A0;
    return p;
}

void *func_0016FC10(u8 *p) {
    FLD(p, 0x30, void **) = D_004702B0;
    return p;
}

void *func_00170460(u8 *p) {
    FLD(p, 0x30, void **) = D_00470700;
    return p;
}

void *func_00170650(u8 *p) {
    FLD(p, 0x30, void **) = D_00470440;
    return p;
}

void *func_00170670(u8 *p) {
    FLD(p, 0x30, void **) = D_004703D0;
    return p;
}

void *func_001706F0(u8 *p) {
    FLD(p, 0x58, void **) = D_0046B0D0;
    return p;
}

void *func_00170A30(u8 *p) {
    FLD(p, 0x30, void **) = D_00472350;
    return p;
}

void *func_00170D10(u8 *p) {
    FLD(p, 0x30, void **) = D_004737F0;
    return p;
}

void *func_00170F10(u8 *p) {
    FLD(p, 0x30, void **) = D_00470460;
    return p;
}

void *func_00170F90(u8 *p) {
    FLD(p, 0x30, void **) = D_00472C10;
    return p;
}

/* Room object constructor: base 0x469C20 -> 0x469C60 -> 0x46D810; id at +0x153C. */
void *func_00171090(u8 *p, u32 id, s32 arg) {
    FLD(p, 0x0, void **) = D_00469C20;
    FLD(p, 0x20, s32) = arg;
    FLD(p, 0x24, s32) = 0x2000000;
    FLD(p, 0x0, void **) = D_00469C60;
    FLD(p, 0x1380, s32) = 0;
    p[0x153C] = (u8)id;
    FLD(p, 0x0, void **) = D_0046D810;
    return p;
}
