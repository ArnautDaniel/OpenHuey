/* Functions reached on the way into the game whose class isn't known yet. Each moves to its
 * subsystem's file once that is identified. */
#include "common.h"
#include "game.h"

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
