/* Game progress: story flags, byte variables and per-room flags (include/progress.h). */
#include "common.h"
#include "input.h"
#include "game.h"
#include "progress.h"
#include "ptmf.h"
#include "sce/libvu0.h"
#include "task.h"

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

/* the door at exit `exit` of room `room` (rooms +0x10) has state bit 0 */
s32 Progress_CurRoomFlag(Progress *p, s32 room, u32 exit) {
    u32 d = VCALL(D_0044E568, 0x10, u32 (*)(VObject *, s32, u32))(D_0044E568, room, exit) & 0xFFFF;

    return (p->roomFlags[d] & 1) != 0;
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

extern VObject *gCharacters[];   /* (func_0016CD60 reads slot 0xFF when nobody is found) */

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
#ifdef HG_NATIVE
    if (c == NULL) {   /* (a character the PC build leaves out, e.g. HG_NOPARTNER) */
        VCALL(dir, 0xC, void (*)(VObject *, void *, f32, f32, f32))(dir, NULL, 0.0f, 0.0f, 0.0f);
        return;
    }
#endif
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
/* event condition: the pad button `which` (0 circle, 1 square, 2 L1, 3 triangle, 4 R1, 5 cross,
 * 6 start) is held (`how` bit 0) or was pressed this frame (bit 1) */
s32 func_00176DD0(Progress *p, u32 which, u32 how) {
    static const u16 buttons[7] = {PAD_CIRCLE, PAD_SQUARE, PAD_L1, PAD_TRIANGLE, PAD_R1, PAD_CROSS, PAD_START};
    u32 b;

    if ((which & 0xFF) >= 7) {
        return 0;
    }
    b = buttons[which & 0xFF];
    if ((how & 1) && (D_0047E374 & b)) {
        return 1;
    }
    if ((how & 2) && (D_0047E37C & b)) {
        return 1;
    }
    return 0;
}

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

/* can door d be passed from side `side` (its lock bits, state bits 4..7: bit 4 side 0, bit 5
 * side 1, bit 6 sides 2..5); side 0xFF always, others never */
static s32 door_passable(Progress *p, u32 d, u32 side) {
    u32 lock = (DOOR_STATE(p, d) >> 4) & 0xF;

    switch (side & 0xFF) {
    case 0xFF:
        return 1;
    case 5:
    case 4:
    case 3:
    case 2:
        return !(lock & 4);
    case 1:
        return !(lock & 2);
    case 0:
        return !(lock & 1);
    }
    return 0;
}

s32 func_00178200(Progress *p, u32 d, u32 side) {
    return door_passable(p, d, side);
}

/* can the door at exit `exit` of room `room` be passed from side `side` (func_00178200) */
s32 func_00178300(Progress *p, s32 room, u32 exit, u32 side) {
    u32 d = VCALL(D_0044E568, 0x10, u32 (*)(VObject *, s32, u32))(D_0044E568, room, exit);

    return door_passable(p, d, side);
}

/* is the door at exit `exit` of room `room` unlocked */
s32 func_001785B0(Progress *p, s32 room, u32 exit) {
    u32 d = VCALL(D_0044E568, 0x10, u32 (*)(VObject *, s32, u32))(D_0044E568, room, exit);

    return (DOOR_STATE(p, d) >> 3) & 1;
}

/* is door d unlocked */
s32 func_00178610(Progress *p, u32 d) {
    return (DOOR_STATE(p, d) >> 3) & 1;
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

extern VObject *D_0044E570;   /* the nav mesh */

/* whether character c counts for the room's occupancy tests */
static s32 occupant(u8 *c, s32 room) {
    return c != NULL && room == AT(c, 0x30, s32) && AT(c, 0x28, u8) == 1 && !AT(c, 0x29, u8);
}

/* which characters are where: per exit k (+0xFD0 + k * 6: +5 at the door, +0 at its front, +3
 * may pass, +4 in its way) and per nav door region d (+0x1000 + d * 4: +3 inside, +2 / +1 the
 * side) */
void func_00175DE0(Progress *p) {
    s32 room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    VObject *doors = D_0044E558;
    VObject *nav;
    u32 k, d;
    s32 i;

    for (k = 0; k < 8; k++) {
        u8 *e = (u8 *)p + 0xFD0 + k * 6;

        e[5] = 0;
        e[3] = 0;
        e[4] = 0;
        e[0] = 0;
        for (i = 0; i < 6; i++) {
            u8 *c = (u8 *)gCharacters[i];
            s32 r;

            if (!occupant(c, room)) {
                continue;
            }
            r = VCALL(doors, 0x2C, s32 (*)(VObject *, s32, f32 *))(doors, (u8)k, (f32 *)((u8 *)gCharacters[i] + 0x10));
            if ((u8)r == 1) {
                u8 bit = (u8)(r << i);

                e[5] |= bit;
                if ((u8)VCALL(doors, 0x6C, s32 (*)(VObject *, s32, s32, f32 *))(
                        doors, 0, (u8)k, (f32 *)((u8 *)gCharacters[i] + 0x10)) == 1) {
                    e[0] |= bit;
                }
                if (VCALL(doors, 0x10, s32 (*)(VObject *, s32, s32, s32))(doors, (u8)k, 1, i) == 1 ||
                    VCALL(doors, 0x10, s32 (*)(VObject *, s32, s32, s32))(doors, (u8)k, 0, i) == 1) {
                    e[3] |= bit;
                }
            }
            if (VCALL(doors, 0x18, s32 (*)(VObject *, s32, f32 *))(doors, (u8)k, (f32 *)((u8 *)gCharacters[i] + 0x10)) == 1) {
                e[4] |= (u8)(1 << i);
            }
        }
    }
    nav = D_0044E570;
    for (d = 0; d < 5; d++) {
        u8 *q = (u8 *)p + 0x1000 + d * 4;

        q[3] = 0;
        q[2] = 0;
        q[1] = 0;
        for (i = 0; i < 6; i++) {
            u8 *c = (u8 *)gCharacters[i];
            u8 bit;
            s32 r;

            if (!occupant(c, room)) {
                continue;
            }
            if ((u8)VCALL(nav, 0x50, s32 (*)(VObject *, u32, f32 *))(nav, d, (f32 *)((u8 *)gCharacters[i] + 0x10)) != 1) {
                continue;
            }
            bit = (u8)(1 << i);
            q[3] |= bit;
            r = VCALL(nav, 0x60, s32 (*)(VObject *, u32, s32, f32 *))(
                nav, d, AT(gCharacters[i], 0x34, s32), (f32 *)((u8 *)gCharacters[i] + 0x10));
            if (r < 0) {
                continue;
            }
            if (r == 0) {
                q[2] |= bit;
            } else {
                q[1] |= bit;
            }
        }
    }
}

extern void func_002A8410(void *o);
extern s32 func_00123960(void *c, s32 d, s32 side);

/* a character's (mask) state at exit e (+0xFD0 + k * 6): bit 0 e[1], 1 e[2], 2 e[0], 3 e[3],
 * 4 e[4], 5 e[5] */
static u8 exit_flags(const u8 *e, u8 mask) {
    u8 f = 0;

    if (e[1] & mask) f |= 1;
    if (e[2] & mask) f |= 2;
    if (e[0] & mask) f |= 4;
    if (e[3] & mask) f |= 8;
    if (e[4] & mask) f |= 0x10;
    if (e[5] & mask) f |= 0x20;
    return f;
}

static u32 door_state(Progress *p, VObject *rooms, u32 k) {
    u32 d = VCALL(rooms, 0x10, u32 (*)(VObject *, s32, u32))(rooms, VCALL(p, 0xC, s32 (*)(Progress *))(p), k) & 0xFFFF;

    return AT(p, 0x124 + d * 4, u32);
}

/* the characters' requests from where they stand (per character slot, +0x1134 + i * 0xE0: +0
 * the request: 2 through an exit (0x80000002 locked), 3 a door region, 0x80000004 a scene
 * thing), from the exits they are at and the door regions they are in */
void func_00175430(Progress *p) {
    s32 room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    VObject *rooms = D_0044E568;
    Progress *gp = gProgress;
    s32 i;

    for (i = 0; i < 6; i++) {
        u8 *blk = (u8 *)p + i * 0xE0 + 0x1134;
        u8 *c = (u8 *)gCharacters[i];
        u8 mask = (u8)(1 << i);
        u32 k;
        s32 j;

        for (j = 0; j < 7; j++) {
            func_002A8410(blk + j * 0x20);
        }
        if (c == NULL || room != AT(c, 0x30, s32) || AT(c, 0xE0, u8) == 1 ||
            (AT(p, 0x8, u32) & 0x40000) != 0) {
            continue;
        }
        for (k = 0; k < 8; k++) {
            if (exit_flags((u8 *)p + 0xFD0 + k * 6, mask) & 4) {
                break;
            }
        }
        if (k < 8 && !(door_state(p, rooms, k) & 1)) {
            u8 flags = exit_flags((u8 *)p + 0xFD0 + k * 6, mask);
            s32 ok = 1;

            if (i != 0 && (flags & 8)) {
                goto regions;
            }
            /* nobody else (but Hewie, for her) may be in the doorway */
            for (j = 0; j < 6; j++) {
                u8 *cj;
                u8 f;

                if ((i == 0 && j == 1) || j == i) {
                    continue;
                }
                cj = (u8 *)gCharacters[j];
                if (cj == NULL || AT(cj, 0x28, u8) != 1 || AT(cj, 0x29, u8)) {
                    continue;
                }
                f = exit_flags((u8 *)p + 0xFD0 + k * 6, (u8)(1 << j));
                if ((f & 8) || (f & 0x14) == 0x14) {
                    ok = 0;
                    break;
                }
            }
            if (ok == 1) {
                u32 d;
                s32 locked;

                blk[0x1D] = 1;
                d = VCALL(rooms, 0x10, u32 (*)(VObject *, s32, u32))(rooms, VCALL(p, 0xC, s32 (*)(Progress *))(p), k) & 0xFFFF;
                if ((u8)VCALL(rooms, 0x44, s32 (*)(VObject *, u32))(rooms, d) & 1) {
                    locked = 1;
                } else if ((AT(p, 0x124 + d * 4, u32) >> 3) & 1) {
                    locked = 0;
                } else {
                    locked = (AT(p, 0x124 + d * 4, u32) >> 1) & 1;
                }
                if (locked == 1) {
                    AT(blk, 0x0, u32) = (door_state(p, rooms, k) >> 3) & 1 ? 0x80000002 : 2;
                    AT(blk, 0x8, u32) = (u8)k;
                    AT(blk, 0x4, s32) = 0;
                    AT(blk, 0x18, s32) = room;
                    blk[0x1C] = flags;
                } else if ((door_state(p, rooms, k) >> 3) & 1) {
                    AT(blk, 0x0, u32) = 0x80000002;
                    AT(blk, 0x8, u32) = (u8)k;
                    AT(blk, 0x4, s32) = 1;
                    AT(blk, 0x18, s32) = room;
                    blk[0x1C] = flags;
                    continue;
                } else {
                    u8 r2 = (u8)VCALL(rooms, 0x4C, s32 (*)(VObject *, s32, u32))(
                        rooms, VCALL(p, 0xC, s32 (*)(Progress *))(p), k);
                    u32 d3 = VCALL(rooms, 0x10, u32 (*)(VObject *, s32, u32))(
                                 rooms, VCALL(p, 0xC, s32 (*)(Progress *))(p), k) & 0xFFFF;
                    s32 open;

                    if ((u8)VCALL(rooms, 0x44, s32 (*)(VObject *, u32))(rooms, d3) & 1) {
                        open = 0;
                    } else if ((AT(gp, 0x124 + d3 * 4, u32) >> 3) & 1) {
                        open = 1;
                    } else {
                        open = (AT(gp, 0x124 + d3 * 4, u32) >> 2) & 1;
                    }
                    AT(blk, 0x0, u32) = 2;
                    AT(blk, 0x8, u32) = (u8)k;
                    AT(blk, 0x4, s32) = 1;
                    AT(blk, 0x18, s32) = room;
                    blk[0x1C] = flags;
                    if (r2 == 1) {
                        AT(blk, 0x40, s32) = 9;
                        AT(blk, 0x48, u32) = (u8)k;
                        AT(blk, 0x44, s32) = open == 1 ? 1 : 0;
                        AT(blk, 0x58, s32) = room;
                    }
                }
            }
        }
    regions:
        if (i == 0 && AT(p, 0x7B8, u8) >= 4) {
            continue;
        }
        if (AT(blk, 0x0, u32) == 0) {
            u8 sel = i != 0 ? 0xFF : 0xFD;
            u32 d;

            for (d = 0; d < 5; d++) {
                u8 *q = (u8 *)p + 0x1000 + d * 4;
                u8 rf = 0;

                if (q[0] & mask) rf |= 1;
                if (q[1] & mask) rf |= 2;
                if (q[2] & mask) rf |= 4;
                if (q[3] & mask) rf |= 8;
                if ((u32)(q[2] & sel) == (u32)(1 << i)) {
                    if (i == 0 && !(u8)func_00123960(gCharacters[i], d, 0)) {
                        continue;
                    }
                    AT(blk, 0x0, u32) = 3;
                    AT(blk, 0x4, s32) = 0;
                    AT(blk, 0x8, u32) = d;
                } else if ((u32)(q[1] & sel) == (u32)(1 << i)) {
                    if (i == 0 && !(u8)func_00123960(gCharacters[i], d, 1)) {
                        continue;
                    }
                    AT(blk, 0x0, u32) = 3;
                    AT(blk, 0x4, s32) = 1;
                    AT(blk, 0x8, u32) = d;
                }
                AT(blk, 0x18, s32) = room;
                blk[0x1C] = rf;
                blk[0x1D] = 2;
            }
        }
        if (i == 0 && AT(blk, 0x0, u32) == 0) {
            c = (u8 *)gCharacters[0];
            if (c != NULL && AT(c, 0x28, u8) == 1 && !AT(c, 0xE0, u8) && AT(c, 0x1AD5D2, u8) == 1) {
                AT(blk, 0x0, u32) = 0x80000004;
            }
        }
    }
}

extern void func_002EC4F0(void *o, void *timer);
extern void func_002A84A0(void *timer);

/* each frame: the pursuer request (+0x778) is handled; the 4 requests (+0x778, 0x10 each) are
 * kept as last frame's (+0x10D4) and cleared */
void func_001776F0(Progress *p) {
    u8 *t = (u8 *)p;
    s32 i;

    if (AT(t, 0x778, u8) != 0) {
        func_002EC4F0(t + 0x764, t + 0x778);
    }
    for (i = 0; i < 4; i++, t += 0x10) {
        AT(t, 0x10D4, u8) = AT(t, 0x778, u8);
        AT(t, 0x10D8, s32) = AT(t, 0x77C, s32);
        AT(t, 0x10DC, s32) = AT(t, 0x780, s32);
        AT(t, 0x10E0, u16) = AT(t, 0x784, u16);
        func_002A84A0(t + 0x778);
    }
}


/* a character action (Character +0x14E8: current, +0x1508: saved); state 7 = held by
 * another character's relation */
typedef struct CharAction {
    s32 state, a, b, c, d;
    f32 e;
    s32 f;
    u8 g, h;
    u16 i;
} CharAction;

static void char_set_action(u8 *c, const CharAction *act) {
    AT(c, 0x14E8, s32) = act->state;
    AT(c, 0x14EC, s32) = act->a;
    AT(c, 0x14F0, s32) = act->b;
    AT(c, 0x14F4, s32) = act->c;
    AT(c, 0x14F8, s32) = act->d;
    AT(c, 0x14FC, f32) = act->e;
    AT(c, 0x1500, s32) = act->f;
    AT(c, 0x1504, u8) = act->g;
    AT(c, 0x1505, u8) = act->h;
    AT(c, 0x1506, u16) = act->i;
}

/* the pending relation changes of the 3 character slots (+0x10B0, 12 bytes each: kind, sub,
 * other slot, arg, s32, f32): kind 1 sub 1 starts a relation on the other's +0x1014 entry;
 * sub 2 (either kind) holds the other character (action state 7); kind 2 sub 1 releases the
 * own character, restoring its saved action. The entry is then cleared. */
void func_00173670(Progress *p) {
    u8 *b = (u8 *)p;
    CharAction act;
    s32 i;

    for (i = 0; i < 3; i++) {
        u8 *e = b + 0x10B0 + i * 0xC;
        u8 *c;

        if (e[0] == 2) {
            if (e[1] == 2) {
                goto hold;
            } else if (e[1] == 1) {
                c = (u8 *)gCharacters[i];
                act.state = AT(c, 0x1508, s32);
                act.a = AT(c, 0x150C, s32);
                act.b = AT(c, 0x1510, s32);
                act.c = AT(c, 0x1514, s32);
                act.d = AT(c, 0x1518, s32);
                act.e = AT(c, 0x151C, f32);
                act.f = AT(c, 0x1520, s32);
                act.g = AT(c, 0x1524, u8);
                act.h = AT(c, 0x1525, u8);
                act.i = AT(c, 0x1526, u16);
                if (act.state != 0 && AT(gCharacters[i], 0x14E8, s32) != 7) {
                    char_set_action((u8 *)gCharacters[i], &act);
                }
                func_002A84C0(e);
            }
        } else if (e[0] == 1) {
            if (e[1] == 2) {
            hold:
                c = (u8 *)gCharacters[e[2]];
                act.state = 7;
                if (AT(c, 0x14E8, s32) != 7) {
                    char_set_action(c, &act);
                }
                func_002A84C0(e);
            } else if (e[1] == 1) {
                u8 *r = b + e[2] * 0x10;

                AT(r, 0x1014, u8) = 1 << i;
                AT(r, 0x1015, u8) = 10;
                AT(r, 0x1016, u16) = AT(e, 0x4, s32);
                AT(r, 0x1018, u16) = e[3];
                AT(r, 0x101C, f32) = AT(e, 0x8, f32);
                func_002A84C0(e);
            }
        }
    }
}

extern s32 func_00125D80(void *c);   /* the character can't take part (u8) */

/* character slot k is free for slot i's kind-9 command: neither has a pending command nor is
 * another's target (+0x10B0 entries) */
static s32 rel_slot_free(u8 *b, s32 k) {
    s32 n;

    if (AT(b, 0x10B0 + k * 0xC, u8) != 0) {
        return 0;
    }
    for (n = 0; n < 3; n++) {
        if (AT(b, 0x10B2 + n * 0xC, u8) == k) {
            return 0;
        }
    }
    return 1;
}

/* resolve the relation requests (+0x1014, 16 bytes per slot: target mask, kind, u16, s16, f32,
 * accepted mask +0xC): each target character is claimed by one requester (kind 6 last; Hewie,
 * slot 1, not while +0xC bit 0x2000), then asked (vtable +0x68); accepting ones take action 4
 * or, kind 9, queue a command for both (+0x10B0); requests of kind 9/10 nobody took hold their
 * requester (action 7). The requests are cleared. */
void func_00173B60(Progress *p) {
    u8 *b = (u8 *)p;
    u8 owner[3];
    CharAction act;
    u8 taken;
    u16 relA = 0;   /* the original leaves these as whatever its caller had when no request */
    s16 relB = 0;   /* was accepted before a kind 9/10 one holds its requester */
    s32 i, j, k;

    if (AT(b, 0x8, u32) & 0x800000) {
        goto clear;
    }
    AT(b, 0x1020, u8) = 0;
    AT(b, 0x1030, u8) = 0;
    owner[0] = 0xFF;
    owner[1] = 0xFF;
    owner[2] = 0xFF;
    AT(b, 0x1040, u8) = 0;
    taken = 0;
    for (j = 2; j >= 0; j--) {
        u8 *r = b + 0x1014 + j * 0x10;
        u8 m;

        if (gCharacters[j] == NULL || AT(gCharacters[j], 0x28, u8) == 0 || (taken & (1 << j)) ||
            r[1] == 6 || r[0] == 0) {
            continue;
        }
        m = r[0];
        for (k = 0; k < 3; k++) {
            u8 *c = (u8 *)gCharacters[k];

            if (c == NULL || !(m & (1 << k)) || AT(gCharacters[k], 0x28, u8) == 0) {
                continue;
            }
            if (r[1] != 5) {
                if (AT(gCharacters[k], 0x2D, u8) == 1) {
                    continue;
                }
            } else if (AT(gCharacters[k], 0xE0, u8) == 1 && AT(gCharacters[k], 0x2D, u8) == 1) {
                continue;
            }
            if (k == 1 && (AT(b, 0xC, u32) & 0x2000)) {
                continue;
            }
            if (taken & (1 << k)) {
                continue;
            }
            owner[k] = j;
            taken |= 1 << k;
        }
    }
    for (j = 2; j >= 0; j--) {
        u8 *r = b + 0x1014 + j * 0x10;
        u8 m;

        if (gCharacters[j] == NULL || AT(gCharacters[j], 0x28, u8) == 0 || (taken & (1 << j)) ||
            r[1] != 6 || r[0] == 0) {
            continue;
        }
        m = r[0];
        for (k = 0; k < 3; k++) {
            if (gCharacters[k] == NULL || !(m & (1 << k)) || AT(gCharacters[k], 0x28, u8) == 0 ||
                AT(gCharacters[k], 0x2D, u8) == 1) {
                continue;
            }
            if (k == 1 && (AT(b, 0xC, u32) & 0x2000)) {
                continue;
            }
            if (taken & (1 << k)) {
                continue;
            }
            owner[k] = j;
            taken |= 1 << k;
        }
    }
    for (i = 0; i < 3; i++) {
        u8 *r;
        u8 kind;
        f32 f;

        if (!(taken & (1 << i))) {
            continue;
        }
        r = b + 0x1014 + owner[i] * 0x10;
        kind = r[1];
        relB = AT(r, 0x4, s16);
        if ((u8)func_00125D80(gCharacters[i]) ||
            (u8)VCALL(gCharacters[i], 0x68, s32 (*)(VObject *, s32, s32, s32))(
                gCharacters[i], kind, owner[i], relB) != 1) {
            owner[i] = 0xFF;
            continue;
        }
        r = b + 0x1014 + owner[i] * 0x10;
        r[0xC] |= 1 << i;
        f = AT(r, 0x8, f32);
        relA = AT(r, 0x2, u16);
        if (kind == 9) {
            u8 *q = b + 0x10B0 + i * 0xC;

            if (!rel_slot_free(b, owner[i]) || !rel_slot_free(b, i)) {
                owner[i] = 0xFF;
                continue;
            }
            q[0] = 1;
            q[2] = owner[i];
            q[1] = 0;
            q[3] = relB;
            AT(q, 0x4, s32) = relA;
            AT(q, 0x8, f32) = f;
        } else {
            u8 *c = (u8 *)gCharacters[i];

            act.state = 4;
            act.a = kind;
            act.b = owner[i];
            act.c = relA;
            act.d = relB;
            act.e = f;
            if (AT(c, 0x14E8, s32) != 7) {
                char_set_action(c, &act);
            }
            func_002A84C0(b + 0x10B0 + i * 0xC);
        }
    }
    for (j = 0; j < 3; j++) {
        u8 kind = AT(b, 0x1015 + j * 0x10, u8);

        if (kind == 9 || kind == 10) {
            for (k = 0; k < 3; k++) {
                if (owner[k] == j) {
                    break;
                }
            }
            if (k == 3) {
                u8 *c = (u8 *)gCharacters[j];

                act.state = 7;
                act.a = kind;
                act.b = j;
                act.c = relA;
                act.d = relB;
                act.e = 0.0f;
                if (AT(c, 0x14E8, s32) != 7) {
                    char_set_action(c, &act);
                }
            }
        }
    }
clear:
    for (j = 0; j < 3; j++) {
        AT(b, 0x1014 + j * 0x10, u8) = 0;
        AT(b, 0x1015 + j * 0x10, u8) = 0;
        AT(b, 0x1016 + j * 0x10, u16) = 0;
        AT(b, 0x1018 + j * 0x10, u16) = 0;
    }
}

extern void func_002A84E0(u8 *r);

/* the characters' own requests (+0x1050, 32 bytes per slot: kind, u16, s16, f32): an active,
 * free character takes action 4 for it (no partner: 0xFF). The requests are cleared. */
void func_001739A0(Progress *p) {
    u8 *b = (u8 *)p;
    CharAction act;
    u32 i;

    if (!(AT(b, 0x8, u32) & 0x800000)) {
        for (i = 0; i < 3; i++) {
            u8 *r = b + 0x1050 + i * 0x20;
            u8 *c;

            if (gCharacters[i] == NULL || AT(gCharacters[i], 0x28, u8) != 1 ||
                AT(gCharacters[i], 0x2D, u8) != 0 || r[0] == 0 ||
                (u8)func_00125D80(gCharacters[i])) {
                continue;
            }
            c = (u8 *)gCharacters[i];
            act.state = 4;
            act.b = 0xFF;
            act.e = AT(r, 0x8, f32);
            act.a = r[0];
            act.c = AT(r, 0x2, u16);
            act.d = AT(r, 0x4, s16);
            if (AT(c, 0x14E8, s32) != 7) {
                char_set_action(c, &act);
            }
        }
    }
    for (i = 0; i < 3; i++) {
        func_002A84E0(b + 0x1050 + i * 0x20);
    }
}

extern s32 func_0019A2B0(u8 *c);   /* the player can be controlled (u8) */
extern void func_0019A420(u8 *c, s32, s32);
extern u32 D_0047E37C;
extern VObject *D_0044E4D0;
extern VObject *D_0044E988;        /* the sub screen (items) */

static void act_copy(u8 *dst, const u8 *src) {
    AT(dst, 0x0, s32) = AT(src, 0x0, s32);
    AT(dst, 0x4, s32) = AT(src, 0x4, s32);
    AT(dst, 0x8, s32) = AT(src, 0x8, s32);
    AT(dst, 0xC, s32) = AT(src, 0xC, s32);
    AT(dst, 0x10, s32) = AT(src, 0x10, s32);
    AT(dst, 0x14, f32) = AT(src, 0x14, f32);
    AT(dst, 0x18, s32) = AT(src, 0x18, s32);
    AT(dst, 0x1C, u8) = AT(src, 0x1C, u8);
    AT(dst, 0x1D, u8) = AT(src, 0x1D, u8);
    AT(dst, 0x1E, u16) = AT(src, 0x1E, u16);
}

/* give the player the action in Progress slot `off` (unless held, state 7); it is also kept
 * at +0x6FB000 from the slot */
static void player_take_action(u8 *b, s32 off) {
    if (AT(gCharPlayer, 0x14E8, s32) != 7) {
        act_copy(gCharPlayer + 0x14E8, b + off);
    }
    act_copy(b + off + 0x6FB000, b + off);
}

/* the player's buttons, each frame while Fiona is controlled and free: 0x2000 the prepared
 * action +0x1134 (0x80000000..2: becomes 2; 5: the event check first; 0x80000005: an event
 * call; 0x80000003/4, 0: none), 0x8000 action 8, 0x1000 the selected item (action 0xE),
 * 0x800 action 0xB */
void func_00174920(Progress *p) {
    u8 *b = (u8 *)p;
    u8 *pl;
    s32 noAct;

    if (gCharPlayer == NULL || AT(gCharPlayer, 0x28, u8) == 0 || AT(gCharPlayer, 0xF8, s32) != 0 ||
        AT(gCharPlayer, 0xFC, s32) == 5) {
        return;
    }
    pl = gCharPlayer;
    if (!(u8)func_0019A2B0(pl)) {
        return;
    }
    if ((u8)func_00125D80(gCharPlayer) == 1 || AT(b, 0x4, s32) != 0) {
        return;
    }
    noAct = AT(pl, 0x1AD580, s32) == 0xD;
    if (AT(gCharPlayer, 0xE0, u8) == 0) {
        act_copy(b + 0x1674, b + 0x1134);
    }
    if ((D_0047E37C & 0x2000) && !noAct) {
        s32 taken = 0;

        switch (AT(b, 0x1134, s32)) {
        case (s32)0x80000002:
        case (s32)0x80000001:
        case (s32)0x80000000:
            AT(b, 0x1134, s32) = 2;
            player_take_action(b, 0x1134);
            taken = 1;
            break;
        case 5:
            VCALL(D_0044E4D0, 0x18, void (*)(VObject *, s32, s32))(D_0044E4D0, 0, AT(b, 0x113C, u8));
            player_take_action(b, 0x1134);
            taken = 1;
            break;
        case (s32)0x80000005: {
            VObject *ev = D_0044E4D0;

            if (ev != NULL) {
                func_0019A420(gCharPlayer, 2, 0x1E);
                VCALL(ev, 0x3C, void (*)(VObject *, s32))(ev, AT(b, 0x1152, u16));
            }
            break;
        }
        case (s32)0x80000004:
        case (s32)0x80000003:
        case 0:
            break;
        default:
            player_take_action(b, 0x1134);
            taken = 1;
            break;
        }
        if (taken) {
            act_copy(b + 0x1694, b + 0x1134);
            act_copy(b + 0x1674, b + 0x1694);
        }
    }
    if (AT(b, 0x7B8, u8) >= 4 || (AT(b, 0x8, u32) & 0x100000)) {
        return;
    }
    if ((D_0047E37C & 0x8000) && AT(gCharPlayer, 0xF8, s32) == 0) {
        AT(b, 0x1154, s32) = 8;
        AT(b, 0x1158, s32) = 0x1A;
        AT(b, 0x115C, s32) = 0;
        player_take_action(b, 0x1154);
    }
    if ((D_0047E37C & 0x1000) && !noAct) {
        VObject *sub = D_0044E988;
        s32 item = VCALL(sub, 0x14, s32 (*)(VObject *))(sub);
        s32 n = VCALL(sub, 0x18, s32 (*)(VObject *))(sub);

        if (AT(gCharPlayer, 0xF8, s32) == 0 && item != 0) {
            AT(b, 0x1194, s32) = 0xE;
            AT(b, 0x1198, s32) = item;
            AT(b, 0x119C, s32) = n;
            player_take_action(b, 0x1194);
        }
    }
    if ((D_0047E37C & 0x800) && AT(gCharPlayer, 0xF8, s32) == 0) {
        AT(b, 0x11B4, s32) = 0xB;
        AT(b, 0x11B8, s32) = 0x22;
        AT(b, 0x11BC, s32) = 0;
        player_take_action(b, 0x11B4);
    }
}


/* every character: vtable +0x24 (the frame's thinking) */
void func_00176440(Progress *p) {
    u32 i;

    for (i = 0; i < 6; i++) {
        if (gCharacters[i] != NULL) {
            VCALL(gCharacters[i], 0x24, void (*)(VObject *))(gCharacters[i]);
        }
    }
}



/* the characters' frame: one in a special state (+0xE0) runs its handler (+0x44, or +0x48
 * while +0xE2); otherwise, unless the world is paused (+0x8 bit 0x800000), the first 3 slots
 * move (+0x30) - only in the current room while +0x8 bit 0x1000000 or the director asks */
void func_001762B0(Progress *p) {
    VObject *dir = D_0044E4F8;
    u32 i;

#ifdef HG_NATIVE
    {
        extern s32 hg_debug_nochars(void);   /* native/platform/debug.c: HG_NOCHARS */

        if (hg_debug_nochars()) {
            return;
        }
    }
#endif

    for (i = 0; i < 6; i++) {
        VObject *c = gCharacters[i];

        if (c == NULL || AT(gCharacters[i], 0x28, u8) == 0) {
            continue;
        }
        if (AT(gCharacters[i], 0xE0, u8) != 0) {
            if (AT(gCharacters[i], 0xE2, u8) == 0) {
                VCALL(gCharacters[i], 0x44, void (*)(VObject *))(gCharacters[i]);
            } else {
                VCALL(gCharacters[i], 0x48, void (*)(VObject *))(gCharacters[i]);
            }
            continue;
        }
        if (AT(p, 0x8, u32) & 0x800000) {
            continue;
        }
        if ((AT(p, 0x8, u32) & 0x1000000) &&
            AT(gCharacters[i], 0x30, s32) != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
            continue;
        }
        if (VCALL(dir, 0x38, s32 (*)(VObject *))(dir) &&
            AT(gCharacters[i], 0x30, s32) != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
            continue;
        }
        if (i < 3) {
            VCALL(gCharacters[i], 0x30, void (*)(VObject *))(gCharacters[i]);
        }
    }
}


/* character slot k has a pending relation command (+0x10B0) */
s32 func_00177870(Progress *p, u32 k) {
    return AT(p, 0x10B0 + (k & 0xFF) * 0xC, u8) != 0;
}


extern VObject *D_0044E4E8;   /* the texture cache */

/* the characters, drawn each frame (texture cache +0x18 and +0x6FC218 (+0x20) reset first):
 * each active and visible one draws (+0x2C); while the world is stopped (+0x8 bit 0x800000)
 * only with +0xC bit 0x10, and only those in the current room while +0x8 bit 0x1000000 -
 * except characters in a special state (+0xE0) */
void func_00176160(Progress *p) {
    u32 i;

#ifdef HG_NATIVE
    {
        extern s32 hg_debug_nochars(void);   /* native/platform/debug.c: HG_NOCHARS */

        if (hg_debug_nochars()) {
            return;
        }
    }
#endif
    VCALL(D_0044E4E8, 0x18, void (*)(VObject *))(D_0044E4E8);
    VCALL((u8 *)p + 0x6FC218, 0x20, void (*)(void *))((u8 *)p + 0x6FC218);
    for (i = 0; i < 6; i++) {
        if (gCharacters[i] == NULL) {
            continue;
        }
        if (AT(gCharacters[i], 0xE0, u8) == 0) {
            u32 f = AT(p, 0x8, u32);

            if ((f & 0x800000) && !(AT(p, 0xC, u32) & 0x10)) {
                continue;
            }
            if ((f & 0x1000000) && AT(gCharacters[i], 0x30, s32) != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
                continue;
            }
        }
        if (AT(gCharacters[i], 0x28, u8) != 0 && AT(gCharacters[i], 0x29, u8) == 0) {
            VCALL(gCharacters[i], 0x2C, void (*)(VObject *))(gCharacters[i]);
        }
    }
}


/* set condition bit `n` (0..6) of +0x14 */
void func_00177630(Progress *p, s32 n) {
    n &= 0xFF;
    if (n < 7) {
        AT(p, 0x14, u32) |= 1u << n;
    }
}


/* condition bit `n` (0..6) of +0x14 */
s32 func_00177670(Progress *p, s32 n) {
    n &= 0xFF;
    if (n < 7) {
        return (AT(p, 0x14, u32) & (1u << n)) != 0;
    }
    return 0;
}


/* +0x64 the stalker's alert (+0x16C8 of the active stalker; 0xFF: no stalker in play) */
s32 func_001770A0(Progress *p) {
    if (D_0044F808 == NULL || AT(D_0044F808, 0x28, u8) == 0) {
        return 0xFF;
    }
    return AT(D_0044F808, 0x16C8, u8);
}


/* is `id` one of the three linked entries (+0x10B0, 0xC each: +0 on, +2 the partner's id):
 * entry `id` is on, or some entry names it */
s32 func_00177770(Progress *p, s32 id) {
    s32 i;

    id &= 0xFF;
    if (AT(p, 0x10B0 + id * 0xC, u8) != 0) {
        return 1;
    }
    for (i = 0; i < 3; i++) {
        if (AT(p, 0x10B2 + i * 0xC, u8) == id) {
            return 1;
        }
    }
    return 0;
}


extern s32 func_0016D6D0(Progress *p, u32 id, u32 slot);   /* load event character id into slot */
extern void func_0016D180(Progress *p, s32 slot);

/* load event character `id` into `slot` and, when it came, set it up (func_0016D180); 1 if
 * loaded */
s32 func_0016D670(Progress *p, u32 id, u32 slot) {
    u32 ok = (u8)func_0016D6D0(p, id, slot);

    if (ok) {
        func_0016D180(p, (u8)slot);
    }
    return ok;
}


/* lock (`on`) or unlock door `door` for kind `kind` (0: bit 1, 1: bit 2, 2..5: bit 4 of the
 * door's lock bits, +0x124 + door x 4 bits 4..7), then let the door object know (+0x80) when
 * it's in this room (rooms +0x3C) */
void func_001780C0(Progress *p, s32 door, s32 kind, s32 on) {
    u32 bit, *w;
    u32 r;

    switch ((u8)kind) {
    case 0:
        bit = 1;
        break;
    case 1:
        bit = 2;
        break;
    case 2: case 3: case 4: case 5:
        bit = 4;
        break;
    default:
        bit = 0;
        break;
    }
    w = &AT(p, 0x124 + (door & 0xFFFF) * 4, u32);
    if (on) {
        *w = (*w & ~0xF0u) | ((((*w >> 4) & 0xF) | bit) & 0xF) << 4;
    } else {
        *w = (*w & ~0xF0u) | ((((*w >> 4) & 0xF) & ~bit) & 0xF) << 4;
    }
    r = (u8)VCALL(D_0044E568, 0x3C, s32 (*)(VObject *, s32, s32))(D_0044E568, door,
                                                                 VCALL(p, 0xC, s32 (*)(Progress *))(p));
    if (r != 0xFF) {
        VCALL(D_0044E558, 0x80, void (*)(VObject *, u32))(D_0044E558, r);
    }
}


/* is door `door` open: the rooms say so (+0x44 bit 0), or its state bits (+0x124 + door x 4)
 * have bit 1 without bit 3 */
s32 func_001788F0(Progress *p, u32 door) {
    u32 w;

    if ((u8)VCALL(D_0044E568, 0x44, s32 (*)(VObject *, u32))(D_0044E568, door) & 1) {
        return 1;
    }
    w = AT(p, 0x124 + (door & 0xFFFF) * 4, u32);
    if (w & 8) {
        return 0;
    }
    return (w & 2) != 0;
}


/* is the door at exit `exit` of room `room` open (as func_001788F0) */
s32 func_00178980(Progress *p, s32 room, s32 exit) {
    VObject *rooms = D_0044E568;
    u32 door = (u16)VCALL(rooms, 0x10, s32 (*)(VObject *, s32, s32))(rooms, room, exit);
    u32 w;

    if ((u8)VCALL(rooms, 0x44, s32 (*)(VObject *, u32))(rooms, door) & 1) {
        return 1;
    }
    w = AT(p, 0x124 + door * 4, u32);
    if (w & 8) {
        return 0;
    }
    return (w & 2) != 0;
}




/* ---- the characters by slot (gCharacters, 6) ---- */

extern u8 *gCharPartner, *gCharPursuer;
extern void func_0029F2C0(void *c);   /* a character's quick unload (keeps its model) */

#define SLOT_CHAR(slot) ((slot) < 6 ? gCharacters[slot] : NULL)

/* character `slot` active (+0x28) */
u8 func_00177160(Progress *p, u32 slot) {
    VObject *c = SLOT_CHAR(slot);

    return c != NULL ? AT(c, 0x28, u8) : 0;
}

/* character `slot` still loading */
s32 func_00177260(Progress *p, u32 slot) {
    VObject *c = SLOT_CHAR(slot);

    return c != NULL ? func_00124D40(c) : 0;
}

/* character `slot`: vtable +0x14 / +0x1C / +0xC */
void func_001772B0(Progress *p, u32 slot) {
    VObject *c = SLOT_CHAR(slot);

    if (c != NULL) {
        VCALL(c, 0x14, void (*)(VObject *))(c);
    }
}

void func_00177300(Progress *p, u32 slot) {
    VObject *c = SLOT_CHAR(slot);

    if (c != NULL) {
        VCALL(c, 0x1C, void (*)(VObject *))(c);
    }
}

void func_00177350(Progress *p, u32 slot) {
    VObject *c = SLOT_CHAR(slot);

    if (c != NULL) {
        VCALL(c, 0xC, void (*)(VObject *))(c);
    }
}

/* remove character `slot`: shut down (vtable +0x18, or the quick way when `quick`), +0x20,
 * +0x10, forget it as player / partner / pursuer; its model back to the scene heap; a loaded
 * character (2..5) also gives back its data buffers (+0x166C) and is destroyed. 1 when there
 * was one */
s32 func_001773A0(Progress *p, u32 slot, u8 quick) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);
    VObject *c = SLOT_CHAR(slot);

    if (c == NULL) {
        return 0;
    }
    if (quick) {
        func_0029F2C0(c);
    } else {
        VCALL(gCharacters[slot], 0x18, void (*)(VObject *))(gCharacters[slot]);
    }
    VCALL(gCharacters[slot], 0x20, void (*)(VObject *))(gCharacters[slot]);
    VCALL(gCharacters[slot], 0x10, void (*)(VObject *))(gCharacters[slot]);
    switch (slot) {
    case 0:
        gCharPlayer = NULL;
        break;
    case 1:
        gCharPartner = NULL;
        break;
    case 2:
        gCharPursuer = NULL;
        break;
    }
    c = gCharacters[slot];
    if (slot >= 2) {
        if (AT(c, 0x1668, u8) != 0) {
            VCALL(heap, 0x14, void (*)(VObject *, void *))(heap, AT(c, 0x166C, void *));
            AT(c, 0x1668, u8) = 0;
        }
        VCALL(heap, 0x14, void (*)(VObject *, void *))(heap, AT(c, 0xF0, void *));
        AT(c, 0xF0, void *) = NULL;
        VCALL(c, 0x8, void (*)(VObject *, s32))(c, 1);
        VCALL(heap, 0x14, void (*)(VObject *, void *))(heap, c);
    } else {
        VCALL(heap, 0x14, void (*)(VObject *, void *))(heap, AT(c, 0xF0, void *));
        AT(c, 0xF0, void *) = NULL;
    }
    gCharacters[slot] = NULL;
    return 1;
}

/* remove all the characters (the slow way) */
/* (possibly dead code: nothing in the game references it) */
void func_001766D0(Progress *p) {
    u32 i;

    for (i = 0; i < 6; i++) {
        func_001773A0(p, i, 0);
    }
}

/* character `a` near character `b` (both active and not hidden (+0x29 = 1), a != b) at `pos`:
 * pos's height within b's (+0x14 .. +0xCC higher, widened by `margin`) and its distance across
 * within b's radius (+0xC8) + margin */
s32 func_00177DB0(Progress *p, u32 a, const f32 *pos, u32 b, f32 margin) {
    f32 d[4] __attribute__((aligned(16)));
    VObject *cb;
    f32 y;

    a &= 0xFF;
    b &= 0xFF;
    if (a == b || gCharacters[a] == NULL || AT(gCharacters[a], 0x28, u8) == 0 || AT(gCharacters[a], 0x29, u8) == 1
        || gCharacters[b] == NULL || AT(gCharacters[b], 0x28, u8) == 0 || AT(gCharacters[b], 0x29, u8) == 1) {
        return 0;
    }
    cb = gCharacters[b];
    y = pos[1];
    if (y <= AT(cb, 0x14, f32) - margin || !(y < margin + (AT(cb, 0x14, f32) + AT(cb, 0xCC, f32)))) {
        return 0;
    }
    sceVu0SubVector(d, (f32 *)((u8 *)cb + 0x10), (f32 *)pos);
    return __builtin_sqrtf(__builtin_fabsf(d[2] * d[2] + d[0] * d[0])) < margin + AT(cb, 0xC8, f32);
}

extern f32 *func_0017CE80(void *skel, s32 bone);   /* a bone's matrix */

/* vtable +0x30 at bone `bone` of character `slot`'s model */
void func_00177F00(Progress *p, u32 slot, s32 bone, s32 arg, f32 f) {
    f32 at[4] __attribute__((aligned(16)));
    u8 *model = AT(gCharacters[slot & 0xFF], 0xF0, u8 *);

    sceVu0CopyVector(at, func_0017CE80(AT(model, 0x810, void *), bone) + 12);
    VCALL(p, 0x30, void (*)(Progress *, u32, f32 *, s32, f32))(p, slot, at, arg, f);
}

/* the three noise slots (+0x1050, 0x20 each) picked by the bits of `which`: a free one gets
   `pos`, `kind`, two values and `f` */
void func_00177FA0(Progress *p, const f32 *pos, u32 which, u8 kind, s16 a, s16 b, f32 f) {
    u8 *e = (u8 *)p;
    u32 i;

    for (i = 0; i < 3; i++, e += 0x20) {
        if ((which & 0xFF & (1 << i)) && AT(e, 0x1050, u8) == 0) {
            sceVu0CopyVector((f32 *)(e + 0x1060), (f32 *)pos);
            AT(e, 0x1050, u8) = kind;
            AT(e, 0x1052, s16) = a;
            AT(e, 0x1054, s16) = b;
            AT(e, 0x1058, f32) = f;
        }
    }
}

/* the door being used (the rooms' +0x10), unless the rooms say otherwise (+0x44 bit 0): when
   not unlocked (bit 3) but bit 0 set, set bit 2 for it and clear bit 0; 1 if it did */
s32 func_00178660(Progress *p) {
    VObject *rooms = D_0044E568;
    u32 d = VCALL(rooms, 0x10, u32 (*)(VObject *))(rooms) & 0xFFFF;
    u32 *s;

    if ((u8)VCALL(rooms, 0x44, s32 (*)(VObject *, u32))(rooms, d) & 1) {
        return 0;
    }
    s = &DOOR_STATE(p, d);
    if ((*s >> 3) & 1 || !(*s & 1)) {
        return 0;
    }
    *s = (*s & ~4) | 4;
    *s &= ~1;
    return 1;
}

extern s32 func_00183190(void *c, s32 on);

/* Fiona: flag +0x1AD710 on, +0x1AD714 cleared, then func_00183190(1) (u8 result) */
s32 func_00176D80(Progress *p) {
    u8 *f = (u8 *)gCharPlayer;

    if (f == NULL) {
        return 0;
    }
    AT(f, 0x1AD710, u8) = 1;
    AT(f, 0x1AD714, s32) = 0;
    return func_00183190(f, 1) != 0;
}

extern void func_0026BC00(void *o);
extern void func_002E2920(void *o);

/* the parts at +0x6FC218 (func_0026BC00), +0x6FC340 (vtable +0x24) and +0x706440 */
/* (possibly dead code: nothing in the game references it) */
void func_00176720(Progress *p) {
    VObject *o = (VObject *)((u8 *)p + 0x6FC340);

    func_0026BC00((u8 *)p + 0x6FC218);
    VCALL(o, 0x24, void (*)(VObject *))(o);
    func_002E2920((u8 *)p + 0x706440);
}

/* ---- movies ---- */

extern void *D_0044E960;   /* the scene table: scenes[] at +4, the scene heap at +0x10D9040 */
extern void *D_0044E958;   /* the movie playing */
extern void *__nw__FUiPv(u32 size, void *p);
extern void *func_002B70D0(void *movie);
extern void func_002B6D10(void *movie, const char *path, s32 mode, s32 keep);
extern void *D_0046EAB0[], *D_0046EAE0[], *D_0046EB10[], *D_0046EC30[], *D_0046EC90[], *D_00474F80[],
    *D_00470E80[];

/* play movie `path` as scene 0, as movie class `kind` (1..6; others the plain one): 1 if it
   started */
s32 func_001768B0(Progress *p, const char *path, u32 kind) {
    static void **const sClass[7] = { D_00470E80, D_0046EAB0, D_0046EAE0, D_0046EB10, D_0046EC30, D_0046EC90,
                                      D_00474F80 };
    u8 *table = D_0044E960;
    VObject *heap = (VObject *)(table + 0x10D9040);
    u32 size = (kind & 0xFF) == 4 ? 0x1E8 : 0x1E0;
    void *mem = VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, size);
    Scene *movie;

    kind &= 0xFF;
    if (mem == NULL) {
        return 0;
    }
    movie = __nw__FUiPv(size, mem);
    if (movie != NULL) {
        func_002B70D0(movie);
        movie->vtbl = sClass[kind <= 6 ? kind : 0];
    }
    AT(table, 0x4, Scene *) = movie;
    AT(table, 0x4, Scene *)->slot = 0;
    movie = AT(table, 0x4, Scene *);
    if (movie == NULL) {
        return 0;
    }
    movie->request = SCENE_REQ_RUN;
    movie->status = 0;
    movie->waitFrames = 0;
    func_002B6D10(D_0044E958, path, 0, 1);
    return 1;
}

/* ---- vtable methods that do nothing ---- */

/* the Progress base (Progress_vtable +0x14 / +0x18) */
void func_00179150(Progress *p) {
}

void func_00179140(Progress *p) {
}

/* the Progress base (+0xC / +0x10): 0 */
s32 func_001794B0(Progress *p) {
    return 0;
}

s32 func_00179160(Progress *p) {
    return 0;
}

void func_00176880(Progress *p) {
}

void func_00176890(Progress *p) {
}

void func_001768A0(Progress *p) {
}

void func_00177600(Progress *p) {
}

void func_00177610(Progress *p) {
}

void func_001780B0(Progress *p) {
}

void func_00178430(Progress *p) {
}

void func_00178440(Progress *p) {
}

s32 func_00177990(Progress *p) {
    return 0;
}

s32 func_001779A0(Progress *p) {
    return -1;
}

s32 func_001780A0(Progress *p) {
    return 0;
}

/* ---- a character's cutscene motion buffer: Fiona's (+0x1AD540) as big as her model says
 * (+0xAC whether, +0xB0 how big), the others' (+0x1688) their data size +0x1C (vtable +0xFC);
 * Hewie none ---- */

void func_0016D050(Progress *p, s32 slot) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);
    u8 *c = (u8 *)gCharacters[slot];

    if (slot == 0) {
        VObject *m = AT(c, 0xF0, VObject *);

        if (VCALL(m, 0xAC, s32 (*)(VObject *))(m) == 0) {
            AT(c, 0x1AD540, void *) = NULL;
        } else {
            AT(c, 0x1AD540, void *) = VCALL(heap, 0x10, void *(*)(VObject *, u32))(
                heap, VCALL(AT(c, 0xF0, VObject *), 0xB0, u32 (*)(VObject *))(AT(c, 0xF0, VObject *)));
        }
    } else if (slot != 1) {
        u32 n = AT(VCALL((VObject *)c, 0xFC, u8 *(*)(VObject *))((VObject *)c), 0x1C, u32);

        AT(c, 0x1688, void *) = n == 0 ? NULL : VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, n);
    }
}

void func_0016CF50(Progress *p, s32 slot) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);
    u8 *c = (u8 *)gCharacters[slot];

    if (slot == 0) {
        VObject *m = AT(c, 0xF0, VObject *);

        if (VCALL(m, 0xAC, s32 (*)(VObject *))(m) != 0) {
            VCALL(heap, 0x14, void (*)(VObject *, void *))(heap, AT(c, 0x1AD540, void *));
        }
    } else if (slot != 1 && AT(VCALL((VObject *)c, 0xFC, u8 *(*)(VObject *))((VObject *)c), 0x1C, u32) != 0) {
        VCALL(heap, 0x14, void (*)(VObject *, void *))(heap, AT(c, 0x1688, void *));
    }
}

extern void func_00176780(Progress *p);
extern void *func_0016CC40(void *o, s32 flags);
extern void func_00100490(void *p);
extern void *Progress_vtable[], *D_0046A9C0[], *D_0046A9B0[], *D_0046FC00[], *D_0046A980[];
extern void *D_0046F5C0[], *D_0046A950[], *D_0046D7D0[], *D_0046A0D0[], *D_0046A1C0[];
extern void *D_004699C0[], *D_004699E0[];
extern void *D_0044F258, *D_0044F260, *gBootMessage;

#define VT(o, off) AT(o, off, void **)

/* a list head (vtables D_004699E0 -> D_004699C0): destructor body */
static inline void ListHead_Destroy(u8 *o) {
    if (o != NULL) {
        VT(o, 0) = D_004699C0;
        if (o != NULL) {
            VT(o, 0) = D_004699E0;
        }
    }
}

/* Progress: destructor - the characters in slots 2..5 (their models unloaded by +0x6FBF00
 * +0x14, then deleted), then its parts in reverse order of construction */
void *func_0016C8A0(Progress *p, s32 flags) {
    u8 *b = (u8 *)p;
    s32 i;

    if (p == NULL) {
        return p;
    }
    VT(b, 0) = Progress_vtable;
    func_00176780(p);
    for (i = 2; i < 6; i++) {
        if (gCharacters[i] != NULL) {
            VObject *c;

            VCALL(b + 0x6FBF00, 0x14, void (*)(void *, VObject *))(b + 0x6FBF00, gCharacters[i]);
            c = gCharacters[i];
            if (c != NULL) {
                VCALL(c, 0x8, void (*)(VObject *, s32))(c, 1);
            }
            gCharacters[i] = NULL;
        }
    }
    if (b + 0x73EB60 != NULL) {
        VT(b, 0x73EB60) = D_0046A9C0;
        func_0016CC40(b + 0x73EC80, -1);
        Task_dtor((Task *)(b + 0x73EB6C), -1);
    }
    if (b + 0x73EB00 != NULL) {
        VT(b, 0x73EB4C) = D_0046A9B0;
    }
    if (b + 0x706440 != NULL) {   /* the creatures (D_0044F258) */
        VT(b, 0x706468) = D_0046FC00;
        ListHead_Destroy(b + 0x715A70);
        ListHead_Destroy(b + 0x714080);
        if (b + 0x706440 != NULL) {
            VT(b, 0x706468) = D_0046A980;
            if (b + 0x706440 != NULL) {
                D_0044F258 = NULL;
            }
        }
    }
    if (b + 0x6FC340 != NULL) {   /* D_0044F260 */
        VT(b, 0x6FC340) = D_0046F5C0;
        ListHead_Destroy(b + 0x706380);
        if (b + 0x6FC340 != NULL) {
            VT(b, 0x6FC340) = D_0046A950;
            if (b + 0x6FC340 != NULL) {
                D_0044F260 = NULL;
            }
        }
    }
    if (b + 0x6FC218 != NULL) {   /* the boot message */
        VT(b, 0x6FC218) = D_0046D7D0;
        if (b + 0x6FC218 != NULL) {
            VT(b, 0x6FC218) = D_0046A0D0;
            if (b + 0x6FC218 != NULL) {
                gBootMessage = NULL;
            }
        }
    }
    if (b + 0x6FBF00 != NULL) {
        VT(b, 0x6FBF00) = D_0046A1C0;
        if (b + 0x6FBF00 != NULL) {
            VT(b, 0x6FBF00) = D_004699E0;
        }
    }
    if (p != NULL) {
        gProgress = NULL;
    }
    if ((s16)flags > 0) {
        func_00100490(p);
    }
    return p;
}

/* the base class's defaults (vtable +0x40 .. +0x88; the game overrides them) */
s32 func_0016CCA0(Progress *p) { return 0; }   /* +0x40 */
void func_0016CCB0(Progress *p) {}            /* +0x4C */
s32 func_0016CD00(Progress *p) { return 0; }   /* +0x50 */
s32 func_0016CCF0(Progress *p) { return 4; }   /* +0x58 */
s32 func_0016CCE0(Progress *p) { return 0; }   /* +0x5C */
s32 func_0016CCD0(Progress *p) { return 0; }   /* +0x60 */
s32 func_0016CCC0(Progress *p) { return 1; }   /* +0x7C */
void func_0016CD20(Progress *p) {}            /* +0x84 */
s32 func_0016CD10(Progress *p) { return 0; }   /* +0x88 */

extern VObject *gFileLoader;

/* the object at +0x73EC80: destructor (its base, vtable D_00469D00) */
void *func_0016CC40(void *o, s32 flags) {
    extern void *D_0046F350[], *D_00469D00[];

    if (o != NULL) {
        VT(o, 0) = D_0046F350;
        if (o != NULL) {
            VT(o, 0) = D_00469D00;
        }
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* start loading the speech file `name` into a fresh 0x41000 buffer (+0x73EDC0, from the memory
   manager +0x6FBF00) */
void func_0016CEC0(Progress *p, const char *name) {
    u8 *b = (u8 *)p;

    AT(b, 0x73EDC0, void *) = VCALL(b + 0x6FBF00, 0x10, void *(*)(void *, u32))(b + 0x6FBF00, 0x41000);
    VCALL(gFileLoader, 0xC, s32 (*)(VObject *, const char *, void *, s32, u32))(gFileLoader, name,
                                                                                AT(b, 0x73EDC0, void *), 0x10000000, 0);
}

/* once it has loaded, character `who` (+0x153C; 0xFE the third slot, 0xFF none) speaks it:
   the boot message object +0x14 / +0x10 with the character's voice (+0x1528). 0: not loaded. */
s32 func_0016CD60(Progress *p, s32 who, s32 arg) {
    VObject *msg;
    VObject *c;
    u32 i, slot;

    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, s32))(gFileLoader, 0x10000000) != 3) {
        return 0;
    }
    who &= 0xFF;
    slot = 0xFF;
    if (who == 0xFE) {
        if (gCharacters[2] != NULL) {
            slot = 2;
        }
    } else if (who != 0xFF) {
        for (i = 0; i < 6; i++) {
            if (gCharacters[i] != NULL && who == AT(gCharacters[i], 0x153C, u8)) {
                slot = i;
                break;
            }
        }
    }
    msg = gBootMessage;
    c = gCharacters[slot];
    VCALL(msg, 0x14, void (*)(VObject *, u32))(msg, AT(c, 0x1528, u8));
    VCALL(msg, 0x10, void (*)(VObject *, u32, void *, s32))(msg, AT(c, 0x1528, u8), AT(p, 0x73EDC0, void *), arg);
    return 1;
}

/* unload character slot `i`'s model (+0xF0) through the memory manager (+0x6FBF00 +0x14) */
void func_0016D2F0(Progress *p, s32 i) {
    u8 *mm = (u8 *)p + 0x6FBF00;

    VCALL(mm, 0x14, void (*)(void *, void *))(mm, AT(gCharacters[i], 0xF0, void *));
    AT(gCharacters[i], 0xF0, void *) = NULL;
}
