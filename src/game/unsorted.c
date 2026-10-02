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
