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

/* func_002C6650: the depth of field (vtable +0xC of D_0046EC80; +0x8 .. +0x14 the view depths
 * a, from, to, b; layer 0x21). The original copies its Z buffer down to half size and marks
 * the frame's alpha with depth-tested sprites at eight depths (0x60 .. 0 over a .. from, 0x20
 * .. 0x80 over to .. b, 0x80 elsewhere), blurs the halved screen with eight 50/50 shifted
 * copies, and blends it back by that alpha. glr does it from each pixel's view depth; 0 =
 * nothing linked. */
int func_002C6650(uint8_t *d) {
    float v[4];

    memcpy(v, d + 0x8, sizeof(v));
    glr_dof(v[0], v[1], v[2], v[3]);
    return 0;
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

/* ---- the renderer's special layers (func_001B5EC0): each one's setup, run on the layer's first
 * draw of a frame, fills the layer before it (and after it) with GS state. On PC glr does what
 * they set up for the layers it knows; 1 = the layer can be drawn ---- */

/* layer 6 (shadows): half-size drawing against the scene's depth halved, blurred and taken off
 * the screen at its end - glr's layer 6 */
int func_001B4F30(uint8_t *r) {
    (void)r;
    return 1;
}

/* layer 0x17 (the reflection): drawn into the reflection buffer over the screen mirrored - glr */
int func_001B0D40(uint8_t *r) {
    (void)r;
    return 1;
}

/* layer 0x0D: as layer 6 (half size against the depth halved, blurred, taken off the screen
 * at its end) - glr */
int func_001B4330(uint8_t *r) {
    (void)r;
    return 1;
}

/* layer 0x14: drawn over the frame's alpha, then a half-size pass on it. GL TODO */
int func_001AF3B0(uint8_t *r) {
    (void)r;
    glr_todo("layer 0x14 pass (func_001AF3B0)");
    return 1;
}

/* layer 0x11: a light / flare pass. GL TODO */
int func_001B2160(uint8_t *r) {
    (void)r;
    glr_todo("layer 0x11 pass (func_001B2160)");
    return 1;
}

/* layer 0x1C: what it draws, softened (brought down to 256 x 256 and blended back by its
 * alpha at its end) - glr */
int func_001AB960(uint8_t *r) {
    (void)r;
    glr_soft_layer();
    return 1;
}

/* layers 0x0F / 0x1A / 0x23: fading layers - what they draw fades back to the screen behind
 * it, or turns towards the colour, by the renderer's tint (+0x304D54); glr does the start and
 * the end (func_001B1370 / func_001AAE80). Layer 0x23 also runs the two-colour effect on what
 * it draws (GL TODO). */
static uint32_t renderer_tint(const uint8_t *r) {
    uint32_t t;

    memcpy(&t, r + 0x304D54, 4);
    return t;
}

int func_001B18E0(uint8_t *r) {
    glr_tint_layer(0x0F, renderer_tint(r));
    return 1;
}

int func_001AB3F0(uint8_t *r) {
    glr_tint_layer(0x1A, renderer_tint(r));
    return 1;
}

int func_001AC0D0(uint8_t *r) {
    glr_tint_layer(0x23, renderer_tint(r));
    glr_todo("layer 0x23 two-colour pass on its draws (func_001AC0D0)");
    return 1;
}

int func_001B1370(uint8_t *r) {
    (void)r;
    return 1;
}

int func_001AAE80(uint8_t *r) {
    (void)r;
    return 1;
}

/* renderer +0x40 (func_001BB780): send an RGBA image (w x h) to VRAM at word address `addr` in
 * renderer layer `layer` (in 16-pixel columns). glr keeps it for showing (glr_vram_draw);
 * 1: done */
int func_001BB780(uint8_t *r, const void *src, uint32_t addr, int w, int h, int layer) {
    (void)r;
    glr_vram_upload(addr, src, w, h);
    if (addr == 0x88000) {   /* into the frame itself (a movie's frame): shown in its layer */
        glr_vram_blit(layer);
    }
    return 1;
}
