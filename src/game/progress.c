/* Game progress: story flags, byte variables and per-room flags (include/progress.h). */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "ptmf.h"

extern VObject *D_0044E568; /* the rooms (+0x10: the current room) */

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
    u32 room = VCALL(D_0044E568, 0x10, u32 (*)(VObject *))(D_0044E568) & 0xFFFF; /* returns a u16 */

    return (p->roomFlags[room] & 1) != 0;
}

/* how the game scene starts: the room (bits 0x40000000: the extra mode) */
void func_001779B0(Progress *p, s32 entry) {
    AT(p, 0x6FC214, s32) = entry;
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

/* every character: vtable +0x14 (load) */
void func_00176550(Progress *p) {
    u32 i;

    for (i = 0; i < 6; i++) {
        if (i < 6 && gCharacters[i] != NULL) {
            VCALL(gCharacters[i], 0x14, void (*)(VObject *))(gCharacters[i]);
        }
    }
}

/* the resident load buffer of character `k`: only the player's (+0x16C0) */
void *func_001776B0(Progress *p, u8 k) {
    if (k == 1) {
        return NULL;
    }
    if (k == 0) {
        return (u8 *)p + 0x16C0;
    }
    return NULL;
}

extern s32 func_00124D40(void *c);   /* a character still loading */

/* is any character still loading? */
s32 func_001764C0(Progress *p) {
    u32 i;

    for (i = 0; i < 6; i++) {
        u8 busy = (i < 6 && gCharacters[i] != NULL) ? (u8)func_00124D40(gCharacters[i]) : 0;

        if (busy == 1) {
            return 1;
        }
    }
    return 0;
}

/* activate character `i` (vtable +0x5C); 0 if there is none */
s32 func_001771A0(Progress *p, u32 i) {
    if (i < 6 && gCharacters[i] != NULL) {
        VCALL(gCharacters[i], 0x5C, void (*)(VObject *))(gCharacters[i]);
        return 1;
    }
    return 0;
}

/* every character: vtable +0x1C (enter the room) */
void func_001765D0(Progress *p) {
    u32 i;

    for (i = 0; i < 6; i++) {
        if (i < 6 && gCharacters[i] != NULL) {
            VCALL(gCharacters[i], 0x1C, void (*)(VObject *))(gCharacters[i]);
        }
    }
}

extern VObject *D_0044E4F8;   /* the camera director's interface */

extern u8 *gCharPlayer;

/* make the camera director follow character `idx`, 10 above its origin, if it is in the
 * current room (vt+0xC); 0xFF: follow nothing; otherwise follow the player (index 0).
 * Returns who it follows (0xFF: nobody). */
u8 func_00179170(Progress *p, u8 idx) {
    VObject *dir = D_0044E4F8;
    s32 ok = 1;
    u8 *c;

    if (dir == NULL) {
        return 0xFF;
    }
    if (idx == 0xFF) {
        VCALL(dir, 0xC, void (*)(VObject *, void *, f32, f32, f32))(dir, NULL, 0.0f, 0.0f, 0.0f);
        return 0xFF;
    }
    if (idx >= 6 || gCharacters[idx] == NULL) {
        ok = 0;
    } else if (idx != 0) {
        c = (u8 *)gCharacters[idx];
        if (AT(c, 0x28, u8) == 0 || AT(c, 0xE8, s32) == -1 ||
            AT(c, 0x30, s32) != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
            ok = 0;
        }
    }
    if (ok) {
        return idx;
    }
    VCALL(dir, 0xC, void (*)(VObject *, void *, f32, f32, f32))(dir, gCharPlayer, 0.0f, 10.0f, 0.0f);
    return 0;
}

/* point the camera director at character `idx` (+0x1130: the one it settled on) */
void func_001792C0(Progress *p, u8 idx) {
    VObject *dir = D_0044E4F8;
    u8 *c;

    if (dir == NULL) {
        return;
    }
    AT(p, 0x1130, u8) = func_00179170(p, idx);
    if (AT(p, 0x1130, u8) == 0xFF) {
        VCALL(dir, 0xC, void (*)(VObject *, void *, f32, f32, f32))(dir, NULL, 0.0f, 0.0f, 0.0f);
        return;
    }
    dir = D_0044E4F8;
    c = (u8 *)gCharacters[AT(p, 0x1130, u8)];
    VCALL(dir, 0xC, void (*)(VObject *, void *, f32, f32, f32))(dir, c, 0.0f, 10.0f, 0.0f);
    VCALL(dir, 0x28, void (*)(VObject *, s32, s32))(dir, AT(c, 0xE8, s32), AT(c, 0xEC, s32));
}

extern char D_0044F860[], D_0044F880[], D_0044F8A0[];   /* ST_%03X\ST1_%03X.HD / .SDT / .BD */

/* the room's sound bank (ST_xxx\ST1_xxx.HD / .SDT / .BD) into sound bank 6 */
void func_0016D480(Progress *p, s32 room) {
    char name[0x100];
    VObject *snd = D_0044E560;
    u8 *prog;

    VCALL(snd, 0x10, void (*)(VObject *, s32, s32))(snd, 0, 0x1B0C00);
    VCALL(snd, 0x84, void (*)(VObject *, s32))(snd, 6);
    VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 6);
    func_0026EDD0(name, sizeof(name), D_0044F860, room & ~7, room);
    prog = (u8 *)gProgress;
    VCALL(snd, 0x80, void (*)(VObject *, const char *, s32, s32, void *))(snd, name, 6, 0, prog + 0x1CA6C0);
    func_0026EDD0(name, sizeof(name), D_0044F880, room & ~7, room);
    VCALL(snd, 0x80, void (*)(VObject *, const char *, s32, s32, void *))(snd, name, 6, 2, prog + 0x1CAEC0);
    func_0026EDD0(name, sizeof(name), D_0044F8A0, room & ~7, room);
    VCALL(snd, 0x80, void (*)(VObject *, const char *, s32, s32, void *))(snd, name, 6, 3, prog + 0x1CCEC0);
}

extern u8 *D_0044F808;   /* the stalker in play */

/* the gCharacters index of the character with script id `id` (0xFE: the stalker, slot 2;
 * 0xFF / not found: 0xFF) */
s32 func_001770D0(Progress *p, s32 id) {
    s32 i;

    id &= 0xFF;
    if (id == 0xFF) {
        return 0xFF;
    }
    if (id == 0xFE) {
        return D_0044F808 != NULL ? 2 : 0xFF;
    }
    for (i = 0; i < 6; i++) {
        if (gCharacters[i] != NULL && AT(gCharacters[i], 0x153C, u8) == id) {
            return (u8)i;
        }
    }
    return 0xFF;
}

/* the game mode byte (+0x10; 2: the alternative idle set) */
s32 func_00177620(Progress *p) {
    return AT(p, 0x10, u8);
}

/* the camera setup (a, b) of character slot `slot` (0xFF: the camera director's own; other
 * characters only in the current room, else none) */
void func_001793A0(Progress *p, u8 slot, s32 a, s32 b) {
    VObject *dir = D_0044E4F8;
    u8 *c;

    if (slot == 0xFF) {
        if (dir != NULL) {
            VCALL(dir, 0x28, void (*)(VObject *, s32, s32))(dir, a, b);
        }
        AT(p, 0x1130, u8) = 0xFF;
        return;
    }
    if (slot >= 6 || gCharacters[slot] == NULL) {
        return;
    }
    c = (u8 *)gCharacters[slot];
    if (slot == 0 || AT(c, 0x30, s32) == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        AT(gCharacters[slot], 0xE8, s32) = a;
        AT(gCharacters[slot], 0xEC, s32) = b;
    } else {
        AT(gCharacters[slot], 0xEC, s32) = -1;
        AT(gCharacters[slot], 0xE8, s32) = -1;
    }
}

/* character slot `slot`'s +0x58 (1: done, 0: no such character) */
s32 func_00177200(Progress *p, u32 slot) {
    if (slot < 6 && gCharacters[slot] != NULL) {
        VCALL(gCharacters[slot], 0x58, void (*)(VObject *, u32))(gCharacters[slot], slot);
        return 1;
    }
    return 0;
}

/* ---- the doors' states (+0x124, a word per door: bit 1, bit 2, bit 3 = unlocked) ---- */

extern VObject *D_0044E558;   /* the doors */

#define DOOR_STATE(p, d) AT(p, 0x124 + ((d) & 0xFFFF) * 4, u32)

/* refresh door `d` if it is in the current room */
static void door_refresh(Progress *p, u32 d) {
    s32 room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    u8 k = (u8)VCALL(D_0044E568, 0x3C, s32 (*)(VObject *, u32, s32))(D_0044E568, d, room);

    if (k != 0xFF) {
        VCALL(D_0044E558, 0x80, void (*)(VObject *, s32))(D_0044E558, k);
    }
}

/* unlock door d */
s32 func_00178450(Progress *p, u32 d) {
    DOOR_STATE(p, d) = (DOOR_STATE(p, d) & ~8) | 8;
    door_refresh(p, d);
    return 1;
}

/* lock door d */
s32 func_00178500(Progress *p, u32 d) {
    DOOR_STATE(p, d) = DOOR_STATE(p, d) & ~8;
    door_refresh(p, d);
    return 1;
}

void func_00178630(Progress *p, u32 d) {
    DOOR_STATE(p, d) = (DOOR_STATE(p, d) & ~4) | 4;
}

s32 func_00178A60(Progress *p, u32 d) {
    DOOR_STATE(p, d) = (DOOR_STATE(p, d) & ~2) | 2;
    return 1;
}

s32 func_00178A30(Progress *p, u32 d) {
    DOOR_STATE(p, d) = DOOR_STATE(p, d) & ~2;
    return 1;
}
