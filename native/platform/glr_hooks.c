/* Game draw leaves replaced on PC: functions that only build PS2 packets (VU1 / GS) get their
 * OpenGL version here instead of a decompiled one (the PS2 build keeps the original asm). Each
 * is named after the function it replaces. */
#include <stdint.h>
#include <string.h>

#include "glr.h"

/* func_002BB3E0: the fog drawer (vtable +0xC of D_0046EB60; colours +0x8 / +0xC, view depths
 * +0x10 .. +0x14). The original paints the Z buffer through a palette ramp from c0 to c1
 * between the two depths; on PC the mesh shader fogs by view depth. */
int func_002BB3E0(uint8_t *drawer) {
    float n, f;

    memcpy(&n, drawer + 0x10, 4);
    memcpy(&f, drawer + 0x14, 4);
    glr_fog(*(uint32_t *)(drawer + 0x8), *(uint32_t *)(drawer + 0xC), n, f);
    return 0;
}

/* func_002685F0: the two-colour screen effect drawer (vtable +0xC of D_0046D790; colour +0x8,
 * which +0xC, argument +0x10): GS frame-buffer copies in strips over the whole screen. Will be
 * a full-screen pass; returns 0 = nothing linked into the layer. */
int func_002685F0(uint8_t *drawer) {
    (void)drawer;
    glr_todo("two-colour screen effect (func_002685F0)");
    return 0;
}

/* func_0034E9E0: a rippling surface grid (texture +0x20, position +0x10, size +0x24, phase
 * +0x28, turn +0x2C): a sine-displaced grid of textured quads. GL TODO; 0 = nothing linked. */
int func_0034E9E0(uint8_t *d) {
    (void)d;
    glr_todo("ripple grid (func_0034E9E0)");
    return 0;
}

/* func_0021C840: the screen overlay's frame-buffer pass (overlay, strength, offset): copies the
 * frame through strips shifted by `offset` and tinted by `strength` (a heat-haze / blur).
 * Will be a full-screen shader pass. */
void func_0021C840(void *ov, int strength, int offset) {
    (void)ov;
    (void)strength;
    (void)offset;
    glr_todo("frame-buffer distortion (func_0021C840)");
}
