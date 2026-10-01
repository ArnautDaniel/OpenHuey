/* Game progress: story flags, byte variables and per-room flags (include/progress.h). */
#include "common.h"
#include "progress.h"
#include "ptmf.h"

extern void *D_0044E568; /* room manager? vtable +0x10 returns the current room number */

s32 Progress_TestFlag(Progress *p, u32 id) {
    if (id >= PROGRESS_NUM_FLAGS) {
        return 0;
    }
    return (p->flags[id >> 5] & (1 << (id & 31))) != 0;
}

void Progress_SetFlag(Progress *p, u32 id) {
    if (id < PROGRESS_NUM_FLAGS) {
        p->flags[id >> 5] |= 1 << (id & 31);
    }
}

void Progress_ClearFlag(Progress *p, u32 id) {
    if (id < PROGRESS_NUM_FLAGS) {
        p->flags[id >> 5] &= ~(1 << (id & 31));
    }
}

/* Sub-word parameters/returns are passed as full registers and masked here: the original
 * callee does the masking and its callers may leave junk in the upper bits (README). */
u32 Progress_GetVar(Progress *p, u32 id) {
    return p->vars[(u8)id];
}

void Progress_SetVar(Progress *p, u32 id, u32 value) {
    p->vars[(u8)id] = (u8)value;
}

/* Counts up to 3. */
void Progress_IncVar(Progress *p, u32 id) {
    u8 *v = &p->vars[(u8)id];

    *v += 1;
    if (*v >= 4) {
        *v = 3;
    }
}

/* 1 if bit `id` of the second bit set is clear (0 for id -1). */
s32 Progress_IsBitClear(Progress *p, s32 id) {
    if (id == -1) {
        return 0;
    }
    return (p->bits[(u32)id >> 5] & (1 << (id & 31))) == 0;
}

s32 Progress_CurRoomFlag(Progress *p) {
    u32 room = VCALL(D_0044E568, 0x10, u32 (*)(void *))(D_0044E568) & 0xFFFF; /* returns a u16 */

    return (p->roomFlags[room] & 1) != 0;
}
