/* Game progress: story flags, byte variables and per-room flags (include/progress.h). */
#include "common.h"
#include "game.h"
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

/* how the game scene starts: the room (bits 0x40000000: the extra mode) */
void func_001779B0(Progress *p, s32 entry) {
    AT(p, 0x6FC214, s32) = entry;
}

/* clear the 17 words at +0x2718 */
void func_00179EA0(Progress *p) {
    s32 i;

    for (i = 0; i < 17; i++) {
        AT(p, 0x2718 + i * 4, s32) = 0;
    }
}

/* clear the 21 words at +0xC700 */
void func_0017D220(Progress *p) {
    s32 i;

    for (i = 0; i < 21; i++) {
        AT(p, 0xC700 + i * 4, s32) = 0;
    }
}

extern VObject *D_0044E560;   /* the sound driver */
extern s32 func_0026EDD0(char *buf, s32 size, const char *fmt, ...);   /* snprintf */
static const char sBankHd[] = "D_%01X000.HD";
static const char sBankSdt[] = "D_%01X000.SDT";
static const char sBankBd[] = "D_%01X000.BD";

/* the sound bank of set `set` (0 or 1: D_n000.HD / .SDT / .BD) into sound bank 4 */
void func_0016D350(Progress *p, s32 set) {
    char name[0x100];
    VObject *snd;
    u8 *prog;

    if (set != 0 && set != 1) {
        return;
    }
    snd = D_0044E560;
    VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 4);
    func_0026EDD0(name, sizeof(name), sBankHd, set);
    prog = (u8 *)gProgress;
    VCALL(snd, 0x80, void (*)(VObject *, const char *, s32, s32, void *))(snd, name, 4, 0, prog + 0x1CA6C0);
    func_0026EDD0(name, sizeof(name), sBankSdt, set);
    VCALL(snd, 0x80, void (*)(VObject *, const char *, s32, s32, void *))(snd, name, 4, 2, prog + 0x1CAEC0);
    func_0026EDD0(name, sizeof(name), sBankBd, set);
    VCALL(snd, 0x80, void (*)(VObject *, const char *, s32, s32, void *))(snd, name, 4, 3, prog + 0x1CCEC0);
}

extern VObject *gCharacters[6];

/* every character: vtable +0xC (start) */
void func_00176650(Progress *p) {
    u32 i;

    for (i = 0; i < 6; i++) {
        if (i < 6 && gCharacters[i] != NULL) {
            VCALL(gCharacters[i], 0xC, void (*)(VObject *))(gCharacters[i]);
        }
    }
}

extern void func_002A84C0(u8 *e);
extern void func_002A8500(u8 *e);

/* character slot `k` (0..2) starts afresh: its relations to the others (+0x10B0, 3 x 12 bytes;
 * its own marked 2) and the others' (+0x1014, 3 x 16 bytes) that involve it are reset */
void func_00177CC0(Progress *p, u8 k) {
    u8 *b = (u8 *)p;
    s32 i;
    u32 mask;

    for (i = 0; i < 3; i++) {
        if (i == k) {
            AT(b, 0x10B1 + k * 0xC, u8) = 2;
        } else if (AT(b, 0x10B2 + i * 0xC, u8) == k) {
            func_002A84C0(b + 0x10B0 + i * 0xC);
        }
    }
    mask = 1u << k;
    for (i = 0; i < 3; i++) {
        if (i == k || (AT(b, 0x1014 + i * 0x10, u8) & mask)) {
            func_002A8500(b + 0x1014 + i * 0x10);
        }
    }
}
