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
