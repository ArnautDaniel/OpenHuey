/* Game draw leaves replaced on PC: functions that only build PS2 packets (VU1 / GS) get their
 * OpenGL version here instead of a decompiled one (the PS2 build keeps the original asm). Each
 * is named after the function it replaces. */
#include <stdint.h>
#include <string.h>

#include "glr.h"

/* func_002BB3E0: the fog drawer (vtable +0xC of D_0046EB60; colours +0x8 / +0xC, view depths
 * +0x10 .. +0x14). The original paints the Z buffer through a palette ramp from c0 to c1
 * between the two depths, blended over the frame in the drawer's layer; on PC glr's fog pass
 * does it from each pixel's view depth, in the same place among the layers. */
int func_002BB3E0(uint8_t *drawer) {
    float n, f;

    memcpy(&n, drawer + 0x10, 4);
    memcpy(&f, drawer + 0x14, 4);
    glr_fog(*(uint32_t *)(drawer + 0x8), *(uint32_t *)(drawer + 0xC), n, f);
    return 0;
}

/* func_002685F0: the two-colour screen effect drawer (vtable +0xC of D_0046D790; colour +0x8,
 * which +0xC, argument +0x10): the screen halved, brightened (+0x10 0: blurred), stretched
 * back and tinted by the colour at alpha / 2: the first colour adds it, the second pushes the
 * screen away from it (GS ALPHA (Cd - Cs) * FIX + Cd). The original's GS frame-buffer copies
 * become glr passes; returns 0 = nothing linked into the layer. */
int func_002685F0(uint8_t *drawer) {
    int32_t which, arg;

    memcpy(&which, drawer + 0xC, 4);
    memcpy(&arg, drawer + 0x10, 4);
    glr_screen2(*(uint32_t *)(drawer + 0x8), which != 0, arg == 0);
    return 0;
}

/* func_002C6650: the depth-band drawer (vtable +0xC of D_0046EC80; +0x8 .. +0x14 its values,
 * +0xC / +0x10 the band's near / far depth): a frame-buffer pass over the band (2085
 * instructions of GS packets). GL TODO; 0 = nothing linked. */
int func_002C6650(uint8_t *d) {
    (void)d;
    glr_todo("depth band effect (func_002C6650)");
    return 0;
}

/* func_00317D40: room effect D_00472F60's draw (+0x14; a floor quad +0x50 of strength +0x14,
 * mode +0x10): 1901 instructions of GS packets. GL TODO. */
void func_00317D40(uint8_t *e) {
    (void)e;
    glr_todo("floor quad effect (func_00317D40)");
}

/* func_003582D0: D_004795A0's draw (+0x14; level +0x4, strength +0x8): a glow of world
 * geometry (535 instructions of GS packets). GL TODO. */
void func_003582D0(uint8_t *e) {
    (void)e;
    glr_todo("glow effect (func_003582D0)");
}

/* func_0021C840: the vignette, every gameplay frame (strength 50, offset 0). The original
 * draws four gouraud triangles - white at alpha `strength` in each screen corner, clear at the
 * middles of its edges (moved by `offset`) - into a work buffer, copies the screen's colours
 * in under that alpha, and takes colour x alpha back off the screen (ALPHA (0 - Cs) * As + Cd)
 * in layer 0x2A. (Its 16-bit channel shuffle in between only touches alpha bits 6-7, which
 * stay 0 below strength 64.) glr does it as one pass. */
void func_0021C840(void *ov, int strength, int offset) {
    (void)ov;
    glr_vignette(strength, offset);
}
