#ifndef MEMCARD_H
#define MEMCARD_H

#include "common.h"
#include "ptmf.h"

/* Memory card manager (system +0x390, global D_0044FF00); see src/game/memcard.c. */
typedef struct MemCard {
    /* 0x00 */ void **vtbl;
    /* 0x04 */ s32 status;
    /* 0x08 */ s32 error;
    /* 0x0C */ void **subVtbl;
    /* 0x10 */ s32 step;      /* the state's own progress */
    /* 0x14 */ PTMF state;
    /* 0x20 */ s32 port;
    /* 0x24 */ s32 mcCmd;
    /* 0x28 */ s32 mcResult;
    /* 0x2C */ s32 cardType;
    /* 0x30 */ s32 freeKb;
    /* 0x34 */ s32 formatted;
    /* 0x38 */ s32 arg38;
    /* 0x3C */ void *buf;
    /* 0x40 */ s32 offset;
    /* 0x44 */ s32 size;
    /* 0x48 */ s32 lastResult;
    /* 0x4C */ s32 repeats;
    /* 0x50 */ char path[0x80];
} MemCard;

/* vtable */
#define MEMCARD_CHECK(mc, port) VCALL(mc, 0xC, void (*)(MemCard *, s32))(mc, port)
#define MEMCARD_READ(mc, port, buf, off, size) \
    VCALL(mc, 0x18, void (*)(MemCard *, s32, void *, s32, s32))(mc, port, buf, off, size)

/* status after a check */
enum {
    MC_NO_DATA = 0,     /* formatted, room for the game data */
    MC_HAS_DATA = 1,
    MC_NO_ROOM = 2,
    MC_UNFORMATTED = 3,
    MC_NO_CARD = 4,
};

#endif /* MEMCARD_H */
