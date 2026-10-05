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

/* The PC vblank clock: one tick = one PS2 field (59.94 Hz): present the frame and pump input
 * (video.c), wait for the tick's time, then run the game's vblank handlers (start + end). */
#include <time.h>

extern void hg_frame(void);   /* video.c */

static void hg_vblank_tick(void) {
    static struct timespec next;
    struct timespec now;

    hg_frame();
    clock_gettime(CLOCK_MONOTONIC, &now);
    if (next.tv_sec == 0 || now.tv_sec > next.tv_sec + 1) {
        next = now;
    }
    next.tv_nsec += 16683333;
    if (next.tv_nsec >= 1000000000) {
        next.tv_nsec -= 1000000000;
        next.tv_sec++;
    }
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
    hg_vblank();
}

/* VSYNC_WAIT (include/ps2hw.h): clear the flag, tick until a vblank handler sets it */
void hg_wait_flag(volatile unsigned char *flag) {
    *flag = 0;
    while (!*flag) {
        hg_vblank_tick();
    }
}

/* The game's "wait for vsync" (system +0x1C, PS2 0x001BED00): vblank start, then end. */
void func_001BED00(void) {
    hg_wait_flag(&D_0047B204);
    hg_wait_flag(&D_0047B208);
}

/* DisableIntc(cause) */
int func_0026CC80(int cause) {
    if (cause >= 0 && cause < 16) {
        sEnabled[cause] = 0;
    }
    return 1;
}

/* RemoveIntcHandler(cause, id) */
int RemoveIntcHandler(int cause, int id) {
    (void)id;
    if (cause >= 0 && cause < 16) {
        sHandlers[cause] = 0;
    }
    return 0;
}
