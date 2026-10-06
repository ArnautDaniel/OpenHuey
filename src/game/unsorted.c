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


extern VObject *gFileLoader;
static const char sAvoidTex[] = "SYSTEM\\AVOID.TEX";

/* (SceneGame +0x1053480, global D_00456DE8) for a new room: load SYSTEM\AVOID.TEX, reset */
void func_0031E150(u8 *o) {
    VCALL(gFileLoader, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(gFileLoader, sAvoidTex, o + 0x40, 0x10000000, 0);
    AT(o, 0x11040, u8) = 0xFF;
    AT(o, 0x11044, s32) = 0;
}


extern VObject *gVram;   /* the VRAM manager */

#ifdef HG_NATIVE
#include "gl2d.h"

/* AVOID.TEX's parts: u, v, w, h, palette, blending (0 normal, 1 subtracted / 2 mixed / 3 added
 * by the fixed alpha) */
extern u16 D_00429E00[][6];

/* +0xC draw part `part` of AVOID.TEX (texture 0 of group 0x2D) at x, y, its own size, with
 * fixed alpha `alpha`, layer 0x33 */
void func_0031D9C0(u8 *o, s32 x, s32 y, s32 part, s32 alpha) {
    static const u8 kAlpha[4] = {0x44, 0x62, 0x64, 0x68};
    u16 *e = D_00429E00[(u8)part];
    u8 *tex;
    s16 sx = x, sy = y;

    if (TexCache_Resident(0, 0x2D, 0x33, &tex) == -1) {
        return;
    }
    VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
    gl2d_sprite(0x33, sx, sy, sx + e[2], sy + e[3], tex, e[0], e[1], e[0] + e[2], e[1] + e[3], 0x80808080, e[4],
                gl2d_blend(((u64)(u8)alpha << 32) | kAlpha[e[5] & 3]));
}

/* the avoid prompt, each gameplay frame: unless hidden (+0x11040 0xFF), its frame (+0x11040, a
 * 160 x 32 row of AVOID.TEX, texture 0 of group 0x2D) as a 240 x 48 sprite near the bottom
 * right, opaque, in layer 0x30 */
void func_0031DE10(u8 *o) {
    VObject *tc;
    u8 *tex;
    s32 f = AT(o, 0x11040, u8);

    if (f == 0xFF) {
        return;
    }
    tc = gTexCache;
    if (TexCache_Resident(0, 0x2D, 0x30, &tex) == -1) {
        return;
    }
    VCALL(tc, 0x18, void (*)(VObject *))(tc);
    gl2d_sprite(0x30, 0x88, 0xC8, 0x178, 0xF8, tex, 0, f * 32, 0xA0, (f + 1) * 32, 0x80808080, 0, 0);
}
#endif
