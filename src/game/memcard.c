/* Memory card manager (system +0x390, global gMemCard, vtable MemCard_vtable; base D_0046AE60).
 * Requests (one at a time) store their parameters and a state; the per-frame tick runs the
 * state until it sets the status (+0x4, -1 while busy). The states talk to the card through
 * libmc and are platform code (native/platform/memcard.c on PC).
 *
 * Status of a check (+0xC): 0 card with free space but no game data, 1 game data present,
 * 2 no game data and not enough space, 3 unformatted, 4 no card. +0x8: 5 if the card was
 * replaced since the last check. Status of a read (+0x18): 0 done, 6 / 9 / 10 failed. */
#include "common.h"
#include "memcard.h"
#include "ptmf.h"
#include "globals.h"
#include "navmesh.h"
#include "msl.h"
#include "sce/libmc.h"

extern void *MemCard_vtable[], *D_0046AEB4[], *D_0046AD88[], *D_0046AE60[];

/* the states (libmc) */

void *MemCardBase_dtor(u8 *o, s32 flags);

/* destructor (vtable D_0046AE60) */
/* 0x001BF2E0 */
void *MemCardBase_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AE60;
        gMemCard = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}
static inline void request(MemCard *mc, s32 port, void (*state)(MemCard *)) {
    PTMF s = {0, -1, {(void *)state}};

    mc->port = port;
    mc->status = -1;
    mc->step = 0;
    mc->state = s;
}

/* +0x8 */
/* 0x001BF550 */
MemCard *MemCard_dtor(MemCard *mc, s32 flags) {
    if (mc != NULL) {
        mc->vtbl = MemCard_vtable;
        mc->subVtbl = D_0046AEB4;
        if (&mc->subVtbl != NULL) {
            mc->subVtbl = D_0046AD88;
        }
        mc->vtbl = D_0046AE60;
        gMemCard = NULL;
        if ((s16)flags > 0) {
            func_00100490(mc);
        }
    }
    return mc;
}

/* +0xC check the card in `port` */
/* 0x001BF340 */
void MemCard_Check(MemCard *mc, s32 port) {
    mc->error = -1;
    request(mc, port, func_00226220);
}

/* +0x10 */
/* 0x001BF390 */
void MemCard_Request10(MemCard *mc, s32 port, s32 arg, void *buf, s32 size) {
    mc->arg38 = arg;
    mc->buf = buf;
    mc->size = size;
    request(mc, port, func_00225F30);
}

/* +0x14 write `size` bytes from `buf` at `offset` of the game data file */
/* 0x001BF3F0 */
void MemCard_Write(MemCard *mc, s32 port, void *buf, s32 offset, s32 size) {
    mc->buf = buf;
    mc->offset = offset;
    mc->size = size;
    request(mc, port, func_00225950);
}

/* +0x18 read `size` bytes at `offset` of the game data file into `buf` */
/* 0x001BF450 */
void MemCard_Read(MemCard *mc, s32 port, void *buf, s32 offset, s32 size) {
    mc->buf = buf;
    mc->offset = offset;
    mc->size = size;
    request(mc, port, func_00225C40);
}

/* +0x1C */
/* 0x001BF4B0 */
void MemCard_Request1C(MemCard *mc, s32 port) {
    request(mc, port, func_00225860);
}

/* +0x20 */
/* 0x001BF500 */
void MemCard_Request20(MemCard *mc, s32 port) {
    request(mc, port, func_00225770);
}

/* every frame: run the request's state */
/* 0x00226510 */
void MemCard_Tick(MemCard *mc) {
    if (ptmf_test(&mc->state)) {
        ptmf_scall(mc, &mc->state);
    }
}

/* shut the memory card library down (the mc argument is unused) */
/* 0x00226560 */
s32 MemCard_Shutdown(MemCard *mc) {
    return func_00110B60();
}
