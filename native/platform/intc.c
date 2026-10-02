/* PS2 interrupts: the game registers vblank handlers (INTC 2 = vblank start, 3 = vblank end).
 * The PC frame loop calls them through hg_vblank() once per displayed frame. */
#include <stdio.h>

typedef int (*IntcHandler)(int cause);

static IntcHandler sHandlers[16];
static int sEnabled[16];

/* AddIntcHandler(cause, handler, next): returns a handler id */
int func_0026BE80(int cause, IntcHandler handler, int next) {
    (void)next;
    if (cause >= 0 && cause < 16) {
        sHandlers[cause] = handler;
    }
    return cause + 1;
}

/* EnableIntc(cause) */
int func_0026CCE8(int cause) {
    if (cause >= 0 && cause < 16) {
        sEnabled[cause] = 1;
    }
    return 1;
}

/* The game's vblank handlers (PS2 0x001BEDA0 / 0x001BED80 end with `ei`, so they stay assembly in
 * the PS2 build). */
extern unsigned char D_0047B204, D_0047B208;
extern unsigned D_0047B20C;

int func_001BEDA0(int cause) {
    (void)cause;
    D_0047B204 = 1;
    D_0047B20C++;
    return 0;
}

int func_001BED80(int cause) {
    (void)cause;
    D_0047B208 = 1;
    return 0;
}

/* one displayed frame: vblank start then end */
void hg_vblank(void) {
    if (sEnabled[2] && sHandlers[2]) {
        sHandlers[2](2);
    }
    if (sEnabled[3] && sHandlers[3]) {
        sHandlers[3](3);
    }
}

/* writes to PS2 hardware registers (include/ps2hw.h) */
void hg_hw_write32(unsigned addr, unsigned value) {
    (void)addr;
    (void)value;
}
