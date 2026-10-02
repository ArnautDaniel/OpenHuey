/* Functions reached on the way into the game whose class isn't known yet. Each moves to its
 * subsystem's file once that is identified. */
#include "common.h"
#include "game.h"
#include "gs.h"
#include "texcache.h"

/* clear bit `bit` of the mask at +4 (-1: none) */
void func_001AAB80(u8 *p, s32 bit) {
    if (bit != -1) {
        AT(p, 0x4, u32) &= ~(1u << bit);
    }
}

s32 func_0017FD40(void *p) {
    return 1;
}

#include "ptmf.h"

extern const PTMF D_0041A150;

/* (SceneGame +0x73EB40) its state at +0x50 back to D_0041A150 */
void func_002F39B0(u8 *o) {
    AT(o, 0x50, PTMF) = D_0041A150;
}

extern VObject *gFileLoader;
static const char sAvoidTex[] = "SYSTEM\\AVOID.TEX";

/* (SceneGame +0x1053480, global D_00456DE8) for a new room: load SYSTEM\AVOID.TEX, reset */
void func_0031E150(u8 *o) {
    VCALL(gFileLoader, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(gFileLoader, sAvoidTex, o + 0x40, 0x10000000, 0);
    AT(o, 0x11040, u8) = 0xFF;
    AT(o, 0x11044, s32) = 0;
}


extern VObject *D_0044E9A0;   /* the VRAM manager */

/* the avoid prompt, each gameplay frame: unless hidden (+0x11040 0xFF), its frame (+0x11040, a
 * 160 x 32 row of AVOID.TEX, texture 0 of group 0x2D) as a 240 x 48 sprite near the bottom
 * right, in layer 0x30 */
void func_0031DE10(u8 *o) {
    VObject *tc;
    u8 *tex;
    s32 slot;
    u64 *p;

    if (AT(o, 0x11040, u8) == 0xFF) {
        return;
    }
    tc = D_0044E4E8;
    slot = TexCache_Resident(0, 0x2D, 0x30, &tex);
    if (slot == -1) {
        return;
    }
    p = VCALL(D_0044E4F0, 0x10, u64 *(*)(VObject *, s32, s32))(D_0044E4F0, 0xB, 0x30);
    if (p == NULL) {
        return;
    }
    VCALL(tc, 0x18, void (*)(VObject *))(tc);
    p[0] = 0x1000000A;              /* DMA cnt 10 */
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x5000000A;   /* VIF DIRECT 10 */
    p[2] = 4 | 0x8000 | (1ULL << 60);   /* GIF tag: 4 A+D, EOP */
    p[3] = 0xE;
    p[4] = (0x80ULL << 32) | 0x44;  /* ALPHA_1: (Cs - Cd) * As + Cd */
    p[5] = 0x42;
    p[6] = 0x60;                    /* TEX1_1: bilinear */
    p[7] = 0x14;
    p[8] = (0x80ULL << 32) | 0x8080; /* TEXA */
    p[9] = 0x3B;
    p[10] = 0x116;                  /* PRIM: sprite, textured, UV */
    p[11] = 0;
    p[12] = 0x8001 | (0x84ULL << 56);   /* reglist: TEX0 CLAMP RGBAQ UV XYZ2 UV XYZ2 NOP */
    p[13] = GIF_REGS_TEX_SPRITE;
    p[14] = VCALL(D_0044E9A0, 0x28, u64 (*)(VObject *, s32, s32, s32, s32, s32))(
        D_0044E9A0, slot, tex[0], AT(tex, 4, u16), AT(tex, 6, u16), tex[1]);
    p[15] = gs_clamp_region(0, AT(o, 0x11040, u8) * 32, 0xA0, 32);
    p[16] = 0x80808080;
    p[17] = gs_uv(0, AT(o, 0x11040, u8) * 32);
    p[18] = gs_xyz2(0x700 + 0x88, 0x720 + 0xC8);
    p[19] = gs_uv(0xA0, (AT(o, 0x11040, u8) + 1) * 32);
    p[20] = gs_xyz2(0x700 + 0x178, 0x720 + 0xF8);
    p[21] = 0;
}
