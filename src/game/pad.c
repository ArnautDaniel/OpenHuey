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

/* port state (system +0x40 +0x40): socket, module, libpad2 state, read phase, data, profile,
 * actuator (rumble) values */
#define PAD_SOCKET(p) AT(p, 0x0, s32)
#define PAD_STATE(p) AT(p, 0x8, s32)
#define PAD_PHASE(p) AT(p, 0xC, s32)
#define PAD_ACTCOUNT(p) AT(p, 0x244, s32)

extern s32 func_001EFD40(s32 socket);                 /* libpad2: state (1 = ready) */
extern s32 func_001EFC70(s32 socket, void *profile);  /* libpad2: button profile */
extern s32 func_001EFB98(s32 socket, void *data);     /* libpad2: read */
extern s32 func_002D25D8(s32 socket, void *mask);     /* actuators (libdbc): count, mask */
extern void func_002D2658(s32 socket, s32 count, void *mask, s32 bytes, void *data);   /* set actuators (libdbc) */
extern void func_002D4780(u8 *pads);                  /* input state from the pad data */

/* +0xC set the motors of port `port` (only port 0): small on / off, large strength */
void func_001BE1F0(u8 *pads, s32 port, s32 small, s32 large) {
    s32 n = (u8)port;

    if (n <= 0) {
        AT(pads, 0x288 + n * 0x280, u8) = small != 0;
        AT(pads, 0x289 + n * 0x280, s8) = large;
    }
}

/* per-frame tick: read the pad (profile first), send changed motor values, update the input */
void func_001BE4B0(u8 *pads) {
    u8 *p = pads + 0x40;
    s32 bytes = 0, data = 0, changed = 1;
    u8 m;

    PAD_STATE(p) = func_001EFD40(PAD_SOCKET(p));
    switch (PAD_STATE(p)) {
    case 2:
        break;
    case 1:
        if (PAD_PHASE(p) == 0) {
            func_001EFC70(PAD_SOCKET(p), p + 0x110);
            PAD_PHASE(p) = 1;
        } else {
            func_001EFB98(PAD_SOCKET(p), p + 0x10);
            PAD_PHASE(p) = 2;
        }
        break;
    default:
        PAD_PHASE(p) = 0;
        break;
    }
    if (AT(p, 0x248, u8) == AT(p, 0x24A, u8) && AT(p, 0x249, u8) == AT(p, 0x24B, u8)) {
        changed = 0;
    }
    if (PAD_STATE(p) == 1 && PAD_PHASE(p) == 2) {
        PAD_ACTCOUNT(p) = func_002D25D8(PAD_SOCKET(p), p + 0x242);
        if (PAD_ACTCOUNT(p) > 0) {
            m = AT(p, 0x242, u8);
            if (m & 1) {
                bytes = 1;
                data = AT(p, 0x248, u8) & 1;
            }
            if (m & 2) {
                data = (data | ((AT(p, 0x249, u8) << bytes) & 0xFFFF)) & 0xFFFF;
                bytes += 8;
            }
            bytes = (bytes + 7) / 8;
        }
    } else {
        PAD_ACTCOUNT(p) = -1;
    }
    if (bytes != 0 && changed) {
        AT(p, 0x240, u8) = data;
        AT(p, 0x241, u8) = (u16)data >> 8;
        func_002D2658(PAD_SOCKET(p), PAD_ACTCOUNT(p), p + 0x242, bytes, p + 0x240);
        AT(p, 0x24A, u8) = AT(p, 0x248, u8);
        AT(p, 0x24B, u8) = AT(p, 0x249, u8);
    }
    func_002D4780(pads);
}
