/* Controller input (system +0x40, vtable 0x46ADB0, global D_0044FEB0), on top of libpad2
 * (replaced on PC by native/platform/pad.c). */
#include "common.h"
#include "game.h"

#define AT(p, off, type) (*(type *)((u8 *)(p) + (off)))

extern s32 func_001EF990(s32 mode);                 /* libpad2: init */
extern s32 func_001EFA38(s32 port, void *buffer);   /* libpad2: create a socket for port, DMA buffer */
extern s32 func_001BC0F0(void *iop, const char *module, s32, s32, s32);   /* IOP: load a module */
extern const char D_0044FEA0[];                     /* pad IOP module */

/* init: libpad2, its IOP module, a socket for port 0 */
void func_001BE6A0(u8 *pads) {
    u8 *p = pads + 0x40;

    func_001EF990(0);
    AT(p, 0x4, s32) = func_001BC0F0(pads + 0x18, D_0044FEA0, 0, 0, 0);
    AT(p, 0x0, s32) = func_001EFA38(0, p + 0x140);
    AT(p, 0x8, s32) = 0;
    AT(p, 0xC, s32) = 0;
    AT(p, 0x244, s32) = -1;
    AT(p, 0x24B, u8) = 0;
    AT(p, 0x24A, u8) = 0;
    AT(p, 0x249, u8) = 0;
    AT(p, 0x248, u8) = 0;
}
