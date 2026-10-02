/* Game draw leaves replaced on PC: functions that only build PS2 packets (VU1 / GS) get their
 * OpenGL version here instead of a decompiled one (the PS2 build keeps the original asm). Each
 * is named after the function it replaces. */
#include <stdint.h>

#include "glr.h"

/* func_002BB3E0: the fog drawer (vtable +0xC of D_0046EB60; colours +0x8 / +0xC, range +0x10 ..
 * +0x14). Fog will be a shader parameter. */
int func_002BB3E0(uint8_t *drawer) {
    (void)drawer;
    glr_todo("fog (func_002BB3E0)");
    return 1;
}

/* func_002685F0: the two-colour screen effect drawer (vtable +0xC of D_0046D790; colour +0x8,
 * which +0xC, argument +0x10): GS frame-buffer copies in strips over the whole screen. Will be
 * a full-screen pass; returns 0 = nothing linked into the layer. */
int func_002685F0(uint8_t *drawer) {
    (void)drawer;
    glr_todo("two-colour screen effect (func_002685F0)");
    return 0;
}

/* func_002E4760 / func_002E3500: the quad (sprite) drawer's packets (instances +0x10, corners
 * +0x14, layer +0x20, atlas cells +0x24..+0x30, flags +0x32: 2 own corners, 1/4 billboard
 * variants; texture id/group +0x34/+0x35, CLUT +0x36): camera-facing textured quads. Will be
 * GL quads once textures are in; 0 = nothing linked. */
int func_002E4760(uint8_t *d) {
    (void)d;
    glr_todo("sprite quads (func_002E4760)");
    return 0;
}

int func_002E3500(uint8_t *d) {
    (void)d;
    glr_todo("sprite quads, flagged (func_002E3500)");
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
