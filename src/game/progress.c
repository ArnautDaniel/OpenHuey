/* Game progress: story flags, byte variables and per-room flags (include/progress.h).
 *
 * (was stalker_progress.c) The game progress's bookkeeping the pursuers use (0x1777D0..0x178F80):
 * the 3 character slots' pending commands to one another (+0x10B0, 12 bytes each: kind, sub,
 * other slot, arg, s32, f32), their relation requests (+0x1014, 16 bytes each), the per-room slot
 * bits (+0x1000), the pursuer groups (+0xFD0) and the doors they hold (+0x124 door states: bit 0
 * held, bit 1 open, bit 2, bit 3 unlocked, bits 4..7 the locked sides). Kept apart from
 * progress.c.
 */
#include "common.h"
#include "input.h"
#include "game.h"
#include "progress.h"
#include "ptmf.h"
#include "sce/libvu0.h"
#include "text.h"
#include "globals.h"
#include "navmesh.h"
#include "actor.h"
#include "char_load.h"
#include "creature.h"
#include "fiona.h"
#include "hewie.h"
#include "movie.h"
#include "pursuer.h"
#include "scene_game.h"
#include "heap.h"
#include "libc.h"
#include "msl.h"
#include "memcard.h"
#include "item.h"
#include "renderer.h"
#include "gl2d.h"
#include "daniella.h"
#include "debilitas2.h"
#include "lorenzo.h"
#include "model.h"
#include "system.h"
#include "vecmath.h"
#include "effectmgr.h"
#include "music.h"
#include "camera.h"
#include "doors.h"
#include "event.h"
#include "gameover.h"
#include "items.h"
#include "loader.h"
#include "pause.h"
#include "placed.h"
#include "room_map.h"
#include "scene.h"
#include "scene_title.h"
#include "draw_leaves.h"
#include "sce/eekernel.h"
#include "effects.h"
#include "sound.h"
#include "pad.h"
#include "scene_boot.h"
#include "sce/iop.h"
#include "cri/adx.h"
#include "subscreen.h"
#include "director.h"
#include "room.h"
#include "lights.h"
#include "sce/intc.h"
#include "charaction.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif

void Progress_Noop78(void);
void Progress_Noop74(void);

void *Progress_dtorGlobal(void *o, s32 flags);
void *Progress_ctor(u8 *p);

extern void Panic_Reset(u8 *p);
extern void PlacedThings_ctorPool(u8 *p);
extern void Creatures_ctorPools(u8 *p);
#define CMD(p, k) ((u8 *)(p) + 0x10B0 + (k) * 0xC)

#define DOOR(p, d) AT(p, 0x124 + ((d) & 0xFFFF) * 4, u32)

/* slot `k` has a command or is another's target */
static inline s32 cmd_busy(Progress *p, u8 k) {
    s32 i;

    if (CMD(p, k)[0] != 0) {
        return 1;
    }
    for (i = 0; i < 3; i++) {
        if (CMD(p, i)[2] == k) {
            return 1;
        }
    }
    return 0;
}

/* the fields of pursuer group `g` (+0xFD0, 6 slot masks) that have slot bit `bit`, as flags:
   bit 0 field 1, bit 1 field 2, bit 2 field 0, bits 3..5 fields 3..5 */
static inline u8 group_fields(const u8 *g, u8 bit) {
    u8 has = 0;

    has |= (g[1] & bit) != 0;
    has |= (g[2] & bit) ? 2 : 0;
    has |= (g[0] & bit) ? 4 : 0;
    has |= (g[3] & bit) ? 8 : 0;
    has |= (g[4] & bit) ? 0x10 : 0;
    has |= (g[5] & bit) ? 0x20 : 0;
    return has;
}

/* the door at exit `exit` of room `room` */
static inline u16 door_at(s32 room, s32 exit) {
    return VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, s32))(gRooms, room, exit);
}

/* is door `d` always open (the rooms' flag bit 0) */
static inline s32 door_fixed(u16 d) {
    return (u8)VCALL(gRooms, 0x44, s32 (*)(VObject *, u32))(gRooms, d) & 1;
}

/* a held door opened or shut by slot `slot` (it is let go): heard (level 0xF in the slot's
   noise, slots past 2 in the 4th) when it's not in the current room */
static inline void door_heard(Progress *p, s32 room, u16 d, u32 slot) {
    u8 k;

    if (room == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return;
    }
    switch (slot & 0xFF) {
    case 2:
        k = 2;
        break;
    case 1:
        k = 1;
        break;
    case 0:
        k = 0;
        break;
    default:
        k = 3;
        break;
    }
    Noise_Make((u8 *)p + 0x778 + k * 0x10, 0xF, room, -1, d);
}

static void copy_words(u8 *d, const u8 *s, u32 off, u32 n) {
    u32 i;

    for (i = 0; i < n; i++) {
        AT(d, off + i * 4, u32) = AT(s, off + i * 4, u32);
    }
}

u8 *Progress_Sub75C(u8 *d, const u8 *s);
u8 *Progress_Sub7B0(u8 *d, const u8 *s);
s32 Progress_Noop54(void);

/* an empty method returning 0 */
/* 0x0011FF20 */
s32 Progress_Noop54(void) {
    return 0;
}
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
    u32 d = VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, u32))(gRooms, room, exit) & 0xFFFF;

    return (p->roomFlags[d] & 1) != 0;
}

/* how the game scene starts: the room (bits 0x40000000: the extra mode) */
/* 0x001779B0 */
void Progress_SetStartEntry(Progress *p, s32 entry) {
    AT(p, 0x6FC214, s32) = entry;
}

/* slot `slot` leaves room `room`'s slot bits (which clears every bit up to it) */
/* 0x001779C0 */
void RoomSlots_Leave(Progress *p, u32 room, u32 slot) {
    AT(p, 0x1000 + (room & 0xFF) * 4, u8) &= (u8)(-2 << (slot & 0xFF));
}

/* slot `slot` is in room `room` */
/* 0x001779F0 */
void RoomSlots_Enter(Progress *p, u32 room, u32 slot) {
    AT(p, 0x1000 + (room & 0xFF) * 4, u8) |= (u8)(1 << (slot & 0xFF));
}

/* which of room `room`'s 4 slot-bit bytes (+0x1000) have slot `slot` (bit n: byte n) */
/* 0x00177A20 */
s32 RoomSlots_Bytes(Progress *p, s32 room, s32 slot) {
    u8 *r = (u8 *)p + 0x1000 + (room & 0xFF) * 4;
    u8 bit = 1 << (slot & 0xFF);
    u8 has = 0;

    has |= (r[0] & bit) != 0;
    has |= (r[1] & bit) ? 2 : 0;
    has |= (r[2] & bit) ? 4 : 0;
    has |= (r[3] & bit) ? 8 : 0;
    return has;
}

/* the first of the 8 pursuer groups with slot `slot` in one of the fields `kind` asks for; 0xFF
   if none */
/* 0x00177AB0 */
s32 PursuerGroup_Find(void *p, s32 kind, u8 slot) {
    u8 bit = 1 << slot;
    u8 i;

    for (i = 0; i < 8; i++) {
        if ((u8)kind & group_fields((u8 *)p + 0xFD0 + i * 6, bit)) {
            return i;
        }
    }
    return 0xFF;
}

/* the fields of group `i` that have slot `slot` */
/* 0x00177BF0 */
u32 PursuerGroup_Fields(Progress *p, u32 i, u32 slot) {
    return group_fields((u8 *)p + 0xFD0 + (i & 0xFF) * 6, 1 << (slot & 0xFF));
}

static const char sBankHd[] = "D_%01X000.HD";
static const char sBankSdt[] = "D_%01X000.SDT";
static const char sBankBd[] = "D_%01X000.BD";

/* the sound bank of set `set` (0 or 1: D_n000.HD / .SDT / .BD) into sound bank 4 */
/* 0x0016D350 */
void Progress_LoadSoundSet(Progress *p, s32 set) {
    char name[0x100];
    VObject *snd;
    u8 *prog;

    if (set != 0 && set != 1) {
        return;
    }
    snd = gSound;
    VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 4);
    msl_snprintf(name, sizeof(name), sBankHd, set);
    prog = (u8 *)gProgress;
    VCALL(snd, 0x80, void (*)(VObject *, const char *, s32, s32, void *))(snd, name, 4, 0, prog + 0x1CA6C0);
    msl_snprintf(name, sizeof(name), sBankSdt, set);
    VCALL(snd, 0x80, void (*)(VObject *, const char *, s32, s32, void *))(snd, name, 4, 2, prog + 0x1CAEC0);
    msl_snprintf(name, sizeof(name), sBankBd, set);
    VCALL(snd, 0x80, void (*)(VObject *, const char *, s32, s32, void *))(snd, name, 4, 3, prog + 0x1CCEC0);
}

/* every character: vtable +0xC (start) */
/* 0x00176650 */
void Progress_StartChars(Progress *p) {
    u32 i;

    for (i = 0; i < 6; i++) {
        if (i < 6 && gCharacters[i] != NULL) {
            VCALL(gCharacters[i], 0xC, void (*)(VObject *))((VObject *)gCharacters[i]);
        }
    }
}

/* character slot `k` (0..2) starts afresh: its relations to the others (+0x10B0, 3 x 12 bytes;
 * its own marked 2) and the others' (+0x1014, 3 x 16 bytes) that involve it are reset */
/* 0x00177CC0 */
void Progress_SlotAfresh(Progress *p, u8 k) {
    u8 *b = (u8 *)p;
    s32 i;
    u32 mask;

    for (i = 0; i < 3; i++) {
        if (i == k) {
            AT(b, 0x10B1 + k * 0xC, u8) = 2;
        } else if (AT(b, 0x10B2 + i * 0xC, u8) == k) {
            SlotCmds_Reset(b + 0x10B0 + i * 0xC);
        }
    }
    mask = 1u << k;
    for (i = 0; i < 3; i++) {
        if (i == k || (AT(b, 0x1014 + i * 0x10, u8) & mask)) {
            Relations_Reset(b + 0x1014 + i * 0x10);
        }
    }
}

/* every character: vtable +0x14 (load) */
/* 0x00176550 */
void Progress_LoadChars(Progress *p) {
    u32 i;

    for (i = 0; i < 6; i++) {
        if (i < 6 && gCharacters[i] != NULL) {
            VCALL(gCharacters[i], 0x14, void (*)(VObject *))((VObject *)gCharacters[i]);
        }
    }
}

/* the resident load buffer of character `k`: only the player's (+0x16C0) */
/* 0x001776B0 */
void *Progress_CharLoadBuffer(Progress *p, u8 k) {
    if (k == 1) {
        return NULL;
    }
    if (k == 0) {
        return (u8 *)p + 0x16C0;
    }
    return NULL;
}

/* is any character still loading? */
/* 0x001764C0 */
s32 Progress_AnyLoading(Progress *p) {
    u32 i;

    for (i = 0; i < 6; i++) {
        u8 busy = (i < 6 && gCharacters[i] != NULL) ? (u8)Actor_DataLoaded((Actor *)gCharacters[i]) : 0;

        if (busy == 1) {
            return 1;
        }
    }
    return 0;
}

/* activate character `i` (vtable +0x5C); 0 if there is none */
/* 0x001771A0 */
s32 Progress_ActivateChar(Progress *p, u32 i) {
    if (i < 6 && gCharacters[i] != NULL) {
        VCALL(gCharacters[i], 0x5C, void (*)(VObject *))((VObject *)gCharacters[i]);
        return 1;
    }
    return 0;
}

/* every character: vtable +0x1C (enter the room) */
/* 0x001765D0 */
void Progress_CharsEnterRoom(Progress *p) {
    u32 i;

    for (i = 0; i < 6; i++) {
        if (i < 6 && gCharacters[i] != NULL) {
            VCALL(gCharacters[i], 0x1C, void (*)(VObject *))((VObject *)gCharacters[i]);
        }
    }
}

/* make the camera director follow character `idx`, 10 above its origin, if it is in the
 * current room (vt+0xC); 0xFF: follow nothing; otherwise follow the player (index 0).
 * Returns who it follows (0xFF: nobody). */
/* 0x00179170 */
s32 Progress_CameraFollow(Progress *p, u8 idx) {   /* (a u8) */
    VObject *dir = gCamDirector;
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
/* 0x001792C0 */
void Progress_CameraOn(Progress *p, u8 idx) {
    VObject *dir = gCamDirector;
    u8 *c;

    if (dir == NULL) {
        return;
    }
    AT(p, 0x1130, u8) = Progress_CameraFollow(p, idx);
    if (AT(p, 0x1130, u8) == 0xFF) {
        VCALL(dir, 0xC, void (*)(VObject *, void *, f32, f32, f32))(dir, NULL, 0.0f, 0.0f, 0.0f);
        return;
    }
    dir = gCamDirector;
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

extern char str_ST_N_ST1_N_HD[], str_ST_N_ST1_N_SDT[], str_ST_N_ST1_N_BD[];   /* ST_%03X\ST1_%03X.HD / .SDT / .BD */

/* the room's sound bank (ST_xxx\ST1_xxx.HD / .SDT / .BD) into sound bank 6 */
/* 0x0016D480 */
void Progress_LoadRoomSounds(Progress *p, s32 room) {
    char name[0x100];
    VObject *snd = gSound;
    u8 *prog;

    VCALL(snd, 0x10, void (*)(VObject *, s32, s32))(snd, 0, 0x1B0C00);
    VCALL(snd, 0x84, void (*)(VObject *, s32))(snd, 6);
    VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 6);
    msl_snprintf(name, sizeof(name), str_ST_N_ST1_N_HD, room & ~7, room);
    prog = (u8 *)gProgress;
    VCALL(snd, 0x80, void (*)(VObject *, const char *, s32, s32, void *))(snd, name, 6, 0, prog + 0x1CA6C0);
    msl_snprintf(name, sizeof(name), str_ST_N_ST1_N_SDT, room & ~7, room);
    VCALL(snd, 0x80, void (*)(VObject *, const char *, s32, s32, void *))(snd, name, 6, 2, prog + 0x1CAEC0);
    msl_snprintf(name, sizeof(name), str_ST_N_ST1_N_BD, room & ~7, room);
    VCALL(snd, 0x80, void (*)(VObject *, const char *, s32, s32, void *))(snd, name, 6, 3, prog + 0x1CCEC0);
}

/* the gCharacters index of the character with script id `id` (0xFE: the stalker, slot 2;
 * 0xFF / not found: 0xFF) */
/* event condition: the pad button `which` (0 circle, 1 square, 2 L1, 3 triangle, 4 R1, 5 cross,
 * 6 start) is held (`how` bit 0) or was pressed this frame (bit 1) */
/* 0x00176DD0 */
s32 Progress_PadCondition(Progress *p, u32 which, u32 how) {
    static const u16 buttons[7] = {PAD_CIRCLE, PAD_SQUARE, PAD_L1, PAD_TRIANGLE, PAD_R1, PAD_CROSS, PAD_START};
    u32 b;

    if ((which & 0xFF) >= 7) {
        return 0;
    }
    b = buttons[which & 0xFF];
    if ((how & 1) && (gPadHeld & b)) {
        return 1;
    }
    if ((how & 2) && (gPadPressed & b)) {
        return 1;
    }
    return 0;
}

/* 0x001770D0 */
s32 Progress_SlotOfId(Progress *p, s32 id) {
    s32 i;

    id &= 0xFF;
    if (id == 0xFF) {
        return 0xFF;
    }
    if (id == 0xFE) {
        return gCharSlot2 != NULL ? 2 : 0xFF;
    }
    for (i = 0; i < 6; i++) {
        if (gCharacters[i] != NULL && AT(gCharacters[i], 0x153C, u8) == id) {
            return (u8)i;
        }
    }
    return 0xFF;
}

/* the game mode byte (+0x10; 2: the alternative idle set) */
/* 0x00177620 */
s32 Progress_GameMode(Progress *p) {
    return AT(p, 0x10, u8);
}

/* the camera setup (a, b) of character slot `slot` (0xFF: the camera director's own; other
 * characters only in the current room, else none) */
/* 0x001793A0 */
void Progress_CameraSetup(Progress *p, u8 slot, s32 a, s32 b) {
    VObject *dir = gCamDirector;
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
/* 0x00177200 */
s32 Progress_CharDone(Progress *p, u32 slot) {
    if (slot < 6 && gCharacters[slot] != NULL) {
        VCALL(gCharacters[slot], 0x58, void (*)(VObject *, u32))((VObject *)gCharacters[slot], slot);
        return 1;
    }
    return 0;
}

/* ---- the doors' states (+0x124, a word per door: bit 1, bit 2, bit 3 = unlocked) ---- */

#define DOOR_STATE(p, d) AT(p, 0x124 + ((d) & 0xFFFF) * 4, u32)

/* refresh door `d` if it is in the current room */
static void door_refresh(Progress *p, u32 d) {
    s32 room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    u8 k = (u8)VCALL(gRooms, 0x3C, s32 (*)(VObject *, u32, s32))(gRooms, d, room);

    if (k != 0xFF) {
        VCALL(gDoors, 0x80, void (*)(VObject *, s32))(gDoors, k);
    }
}

/* unlock door d */
/* 0x00178450 */
s32 Progress_UnlockDoor(Progress *p, u32 d) {
    DOOR_STATE(p, d) = (DOOR_STATE(p, d) & ~8) | 8;
    door_refresh(p, d);
    return 1;
}

/* lock door d */
/* 0x00178500 */
s32 Progress_LockDoor(Progress *p, u32 d) {
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

/* 0x00178200 */
s32 Progress_DoorPassable(Progress *p, u32 d, u32 side) {
    return door_passable(p, d, side);
}

/* can the door at exit `exit` of room `room` be passed from side `side` (Progress_DoorPassable) */
/* 0x00178300 */
s32 Progress_ExitPassable(Progress *p, s32 room, u32 exit, u32 side) {
    u32 d = VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, u32))(gRooms, room, exit);

    return door_passable(p, d, side);
}

/* is the door at exit `exit` of room `room` unlocked */
/* 0x001785B0 */
s32 Progress_ExitUnlocked(Progress *p, s32 room, u32 exit) {
    u32 d = VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, u32))(gRooms, room, exit);

    return (DOOR_STATE(p, d) >> 3) & 1;
}

/* is door d unlocked */
/* 0x00178610 */
s32 Progress_DoorUnlocked(Progress *p, u32 d) {
    return (DOOR_STATE(p, d) >> 3) & 1;
}

/* 0x00178630 */
void Progress_DoorSetBit2(Progress *p, u32 d) {
    DOOR_STATE(p, d) = (DOOR_STATE(p, d) & ~4) | 4;
}

/* 0x00178A60 */
s32 Progress_DoorSetBit1(Progress *p, u32 d) {
    DOOR_STATE(p, d) = (DOOR_STATE(p, d) & ~2) | 2;
    return 1;
}

/* shut the held door at exit `exit` of room `room` (slot `slot`): 1 if done */
/* 0x00178A90 */
s32 DoorHold_Shut(Progress *p, s32 room, s32 exit, u32 slot) {
    u16 d = door_at(room, exit);
    u32 w;

    if (door_fixed(d)) {
        return 0;
    }
    w = DOOR(p, d);
    if (w & 8) {
        return 0;
    }
    if (!(w & 1)) {
        return 0;
    }
    DOOR(p, d) = w & ~2;
    DOOR(p, d) &= ~1;
    door_heard(p, room, d, slot);
    return 1;
}

/* open it (not when bit 2) */
/* 0x00178C10 */
s32 DoorHold_Open(Progress *p, s32 room, s32 exit, u32 slot) {
    u16 d = door_at(room, exit);
    u32 w;

    if (door_fixed(d)) {
        return 0;
    }
    w = DOOR(p, d);
    if (w & 8) {
        return 0;
    }
    if (w & 4) {
        return 0;
    }
    if (!(w & 1)) {
        return 0;
    }
    DOOR(p, d) = (w & ~2) | 2;
    DOOR(p, d) &= ~1;
    door_heard(p, room, d, slot);
    return 1;
}

/* take hold of the door at exit `exit` from side `side` (0xFF any; else its lock sides as
   Progress_DoorPassable): 0 if now held by the caller, 1 if it can't be (fixed, locked that side or
   already held) */
/* 0x00178DB0 */
u32 DoorHold_Take(Progress *p, s32 room, s32 exit, u32 side) {
    u16 d = door_at(room, exit);
    u32 lock;
    u8 ok;

    if (door_fixed(d)) {
        return 1;
    }
    if ((side & 0xFF) != 0xFF) {
        lock = (DOOR(p, d) >> 4) & 0xF;
        switch (side & 0xFF) {
        case 5:
        case 4:
        case 3:
        case 2:
            ok = !(lock & 4);
            break;
        case 1:
            ok = !(lock & 2);
            break;
        case 0:
            ok = !(lock & 1);
            break;
        default:
            ok = 0;
            break;
        }
        if (!ok) {
            return 1;
        }
    }
    if (DOOR(p, d) & 1) {
        return 1;
    }
    DOOR(p, d) |= 1;
    return 0;
}

/* 0x00178A30 */
s32 Progress_DoorClearBit1(Progress *p, u32 d) {
    DOOR_STATE(p, d) = DOOR_STATE(p, d) & ~2;
    return 1;
}

/* whether character c counts for the room's occupancy tests */
static s32 occupant(u8 *c, s32 room) {
    return c != NULL && room == AT(c, 0x30, s32) && AT(c, 0x28, u8) == 1 && !AT(c, 0x29, u8);
}

/* which characters are where: per exit k (+0xFD0 + k * 6: +5 at the door, +0 at its front, +3
 * may pass, +4 in its way) and per nav door region d (+0x1000 + d * 4: +3 inside, +2 / +1 the
 * side) */
/* 0x00175DE0 */
void Progress_WhoIsWhere(Progress *p) {
    s32 room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    VObject *doors = gDoors;
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
    nav = (VObject *)gNavMesh;
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
/* 0x00175430 */
void Progress_CharRequests(Progress *p) {
    s32 room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    VObject *rooms = gRooms;
    Progress *gp = gProgress;
    s32 i;

    for (i = 0; i < 6; i++) {
        u8 *blk = (u8 *)p + i * 0xE0 + 0x1134;
        u8 *c = (u8 *)gCharacters[i];
        u8 mask = (u8)(1 << i);
        u32 k;
        s32 j;

        for (j = 0; j < 7; j++) {
            Record20_Clear(blk + j * 0x20);
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
                    if (i == 0 && !(u8)Actor_FacingDoor((Actor *)gCharacters[i], d, 0)) {
                        continue;
                    }
                    AT(blk, 0x0, u32) = 3;
                    AT(blk, 0x4, s32) = 0;
                    AT(blk, 0x8, u32) = d;
                } else if ((u32)(q[1] & sel) == (u32)(1 << i)) {
                    if (i == 0 && !(u8)Actor_FacingDoor((Actor *)gCharacters[i], d, 1)) {
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

/* each frame: the pursuer request (+0x778) is handled; the 4 requests (+0x778, 0x10 each) are
 * kept as last frame's (+0x10D4) and cleared */
/* 0x001776F0 */
void Progress_PursuerRequest(Progress *p) {
    u8 *t = (u8 *)p;
    s32 i;

    if (AT(t, 0x778, u8) != 0) {
        Summoner_Noise(t + 0x764, t + 0x778);
    }
    for (i = 0; i < 4; i++, t += 0x10) {
        AT(t, 0x10D4, u8) = AT(t, 0x778, u8);
        AT(t, 0x10D8, s32) = AT(t, 0x77C, s32);
        AT(t, 0x10DC, s32) = AT(t, 0x780, s32);
        AT(t, 0x10E0, u16) = AT(t, 0x784, u16);
        CharRequest_Clear(t + 0x778);
    }
}


/* (kept out of line, as the original) */
static void char_set_action_ool(u8 *c, const CharAction *act) {
    char_set_action(c, act);
}

/* the pending relation changes of the 3 character slots (+0x10B0, 12 bytes each: kind, sub,
 * other slot, arg, s32, f32): kind 1 sub 1 starts a relation on the other's +0x1014 entry;
 * sub 2 (either kind) holds the other character (action state 7); kind 2 sub 1 releases the
 * own character, restoring its saved action. The entry is then cleared. */
/* 0x00173670 */
void Progress_RelationChanges(Progress *p) {
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
                    char_set_action_ool((u8 *)gCharacters[i], &act);
                }
                SlotCmds_Reset(e);
            }
        } else if (e[0] == 1) {
            if (e[1] == 2) {
            hold:
                c = (u8 *)gCharacters[e[2]];
                act.state = 7;
                if (AT(c, 0x14E8, s32) != 7) {
                    char_set_action_ool(c, &act);
                }
                SlotCmds_Reset(e);
            } else if (e[1] == 1) {
                u8 *r = b + e[2] * 0x10;

                AT(r, 0x1014, u8) = 1 << i;
                AT(r, 0x1015, u8) = 10;
                AT(r, 0x1016, u16) = AT(e, 0x4, s32);
                AT(r, 0x1018, u16) = e[3];
                AT(r, 0x101C, f32) = AT(e, 0x8, f32);
                SlotCmds_Reset(e);
            }
        }
    }
}

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
/* 0x00173B60 */
void Progress_ResolveRelations(Progress *p) {
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
        if ((u8)Character_Held(gCharacters[i]) ||
            (u8)VCALL(gCharacters[i], 0x68, s32 (*)(VObject *, s32, s32, s32))(
                (VObject *)gCharacters[i], kind, owner[i], relB) != 1) {
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
                char_set_action_ool(c, &act);
            }
            SlotCmds_Reset(b + 0x10B0 + i * 0xC);
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
                    char_set_action_ool(c, &act);
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

/* the characters' own requests (+0x1050, 32 bytes per slot: kind, u16, s16, f32): an active,
 * free character takes action 4 for it (no partner: 0xFF). The requests are cleared. */
/* 0x001739A0 */
void Progress_OwnRequests(Progress *p) {
    u8 *b = (u8 *)p;
    CharAction act;
    u32 i;

    if (!(AT(b, 0x8, u32) & 0x800000)) {
        for (i = 0; i < 3; i++) {
            u8 *r = b + 0x1050 + i * 0x20;
            u8 *c;

            if (gCharacters[i] == NULL || AT(gCharacters[i], 0x28, u8) != 1 ||
                AT(gCharacters[i], 0x2D, u8) != 0 || r[0] == 0 ||
                (u8)Character_Held(gCharacters[i])) {
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
                char_set_action_ool(c, &act);
            }
        }
    }
    for (i = 0; i < 3; i++) {
        OwnRequest_Clear(b + 0x1050 + i * 0x20);
    }
}

extern u32 gPadPressed;

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
        act_copy((u8 *)gCharPlayer + 0x14E8, b + off);
    }
    act_copy(b + off + 0x6FB000, b + off);
}

/* the player's buttons, each frame while Fiona is controlled and free: 0x2000 the prepared
 * action +0x1134 (0x80000000..2: becomes 2; 5: the event check first; 0x80000005: an event
 * call; 0x80000003/4, 0: none), 0x8000 action 8, 0x1000 the selected item (action 0xE),
 * 0x800 action 0xB */
/* 0x00174920 */
void Progress_PlayerButtons(Progress *p) {
    u8 *b = (u8 *)p;
    u8 *pl;
    s32 noAct;

    if (gCharPlayer == NULL || AT(gCharPlayer, 0x28, u8) == 0 || AT(gCharPlayer, 0xF8, s32) != 0 ||
        AT(gCharPlayer, 0xFC, s32) == 5) {
        return;
    }
    pl = (u8 *)gCharPlayer;
    if (!(u8)Fiona_IsIdle((Fiona *)pl)) {
        return;
    }
    if ((u8)Character_Held(gCharPlayer) == 1 || AT(b, 0x4, s32) != 0) {
        return;
    }
    noAct = AT(pl, 0x1AD580, s32) == 0xD;
    if (AT(gCharPlayer, 0xE0, u8) == 0) {
        act_copy(b + 0x1674, b + 0x1134);
    }
    if ((gPadPressed & 0x2000) && !noAct) {
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
            VCALL(gEvents, 0x18, void (*)(VObject *, s32, s32))(gEvents, 0, AT(b, 0x113C, u8));
            player_take_action(b, 0x1134);
            taken = 1;
            break;
        case (s32)0x80000005: {
            VObject *ev = gEvents;

            if (ev != NULL) {
                Fiona_SetTarget((Fiona *)((u8 *)gCharPlayer), 2, 0x1E);
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
    if ((gPadPressed & 0x8000) && AT(gCharPlayer, 0xF8, s32) == 0) {
        AT(b, 0x1154, s32) = 8;
        AT(b, 0x1158, s32) = 0x1A;
        AT(b, 0x115C, s32) = 0;
        player_take_action(b, 0x1154);
    }
    if ((gPadPressed & 0x1000) && !noAct) {
        VObject *sub = gSubScreen;
        s32 item = VCALL(sub, 0x14, s32 (*)(VObject *))(sub);
        s32 n = VCALL(sub, 0x18, s32 (*)(VObject *))(sub);

        if (AT(gCharPlayer, 0xF8, s32) == 0 && item != 0) {
            AT(b, 0x1194, s32) = 0xE;
            AT(b, 0x1198, s32) = item;
            AT(b, 0x119C, s32) = n;
            player_take_action(b, 0x1194);
        }
    }
    if ((gPadPressed & 0x800) && AT(gCharPlayer, 0xF8, s32) == 0) {
        AT(b, 0x11B4, s32) = 0xB;
        AT(b, 0x11B8, s32) = 0x22;
        AT(b, 0x11BC, s32) = 0;
        player_take_action(b, 0x11B4);
    }
}

/* every character: vtable +0x24 (the frame's thinking) */
/* 0x00176440 */
void Progress_CharsThink(Progress *p) {
    u32 i;

    for (i = 0; i < 6; i++) {
        if (gCharacters[i] != NULL) {
            VCALL(gCharacters[i], 0x24, void (*)(VObject *))((VObject *)gCharacters[i]);
        }
    }
}

/* the characters' frame: one in a special state (+0xE0) runs its handler (+0x44, or +0x48
 * while +0xE2); otherwise, unless the world is paused (+0x8 bit 0x800000), the first 3 slots
 * move (+0x30) - only in the current room while +0x8 bit 0x1000000 or the director asks */
/* 0x001762B0 */
void Progress_CharsFrame(Progress *p) {
    VObject *dir = gCamDirector;
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
        VObject *c = (VObject *)gCharacters[i];

        if (c == NULL || AT(gCharacters[i], 0x28, u8) == 0) {
            continue;
        }
        if (AT(gCharacters[i], 0xE0, u8) != 0) {
            if (AT(gCharacters[i], 0xE2, u8) == 0) {
                VCALL(gCharacters[i], 0x44, void (*)(VObject *))((VObject *)gCharacters[i]);
            } else {
                VCALL(gCharacters[i], 0x48, void (*)(VObject *))((VObject *)gCharacters[i]);
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
            VCALL(gCharacters[i], 0x30, void (*)(VObject *))((VObject *)gCharacters[i]);
        }
    }
}

/* character slot k has a pending relation command (+0x10B0) */
/* 0x00177870 */
s32 Progress_HasRelationCmd(Progress *p, u32 k) {
    return AT(p, 0x10B0 + (k & 0xFF) * 0xC, u8) != 0;
}

/* give slot `slot` command `kind` (arg, n, f) towards slot `other`, when neither is busy: 1 if
   given */
/* 0x00177890 */
s32 SlotCmd_Give(Progress *p, s32 kind, s32 arg, u8 other, u8 slot, s32 n, f32 f) {
    u8 *c;

    if (cmd_busy(p, other) || cmd_busy(p, slot)) {
        return 0;
    }
    c = CMD(p, slot);
    c[0] = kind;
    c[2] = other;
    c[1] = 0;
    c[3] = arg;
    AT(c, 0x4, s32) = n;
    AT(c, 0x8, f32) = f;
    return 1;
}

/* the characters, drawn each frame (texture cache +0x18 and +0x6FC218 (+0x20) reset first):
 * each active and visible one draws (+0x2C); while the world is stopped (+0x8 bit 0x800000)
 * only with +0xC bit 0x10, and only those in the current room while +0x8 bit 0x1000000 -
 * except characters in a special state (+0xE0) */
/* 0x00176160 */
void Progress_DrawChars(Progress *p) {
    u32 i;

#ifdef HG_NATIVE
    {
        extern s32 hg_debug_nochars(void);   /* native/platform/debug.c: HG_NOCHARS */

        if (hg_debug_nochars()) {
            return;
        }
    }
#endif
    VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
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
            VCALL(gCharacters[i], 0x2C, void (*)(VObject *))((VObject *)gCharacters[i]);
        }
    }
}

/* set condition bit `n` (0..6) of +0x14 */
/* 0x00177630 */
void Progress_SetCondBit(Progress *p, s32 n) {
    n &= 0xFF;
    if (n < 7) {
        AT(p, 0x14, u32) |= 1u << n;
    }
}

/* condition bit `n` (0..6) of +0x14 */
/* 0x00177670 */
s32 Progress_CondBit(Progress *p, s32 n) {
    n &= 0xFF;
    if (n < 7) {
        return (AT(p, 0x14, u32) & (1u << n)) != 0;
    }
    return 0;
}

/* +0x64 the stalker's alert (+0x16C8 of the active stalker; 0xFF: no stalker in play) */
/* 0x001770A0 */
s32 Progress_StalkerAlert(Progress *p) {
    if (gCharSlot2 == NULL || AT(gCharSlot2, 0x28, u8) == 0) {
        return 0xFF;
    }
    return AT(gCharSlot2, 0x16C8, u8);
}

/* is `id` one of the three linked entries (+0x10B0, 0xC each: +0 on, +2 the partner's id):
 * entry `id` is on, or some entry names it */
/* 0x00177770 */
s32 Progress_IsLinked(Progress *p, s32 id) {
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

/* slot `k`'s command is cancelled (sub 2) */
/* 0x001777D0 */
void SlotCmd_Cancel(Progress *p, u32 slot) {
    CMD(p, slot & 0xFF)[1] = 2;
}

/* slot `k`'s command is under way (sub 1) */
/* 0x001777F0 */
void SlotCmd_Start(Progress *p, u32 slot) {
    CMD(p, slot & 0xFF)[1] = 1;
}

/* its arg */
/* 0x00177810 */
u32 SlotCmd_Arg(Progress *p, u32 slot) {
    return CMD(p, slot & 0xFF)[3];
}

/* its kind (0: none) */
/* 0x00177830 */
u32 SlotCmd_Kind(Progress *p, u32 slot) {
    return CMD(p, slot & 0xFF)[0];
}

/* the slot it is for */
/* 0x00177850 */
u32 SlotCmd_Target(Progress *p, u32 slot) {
    return CMD(p, slot & 0xFF)[2];
}

/* load event character `id` into `slot` and, when it came, set it up (CharLoad_Buffers); 1 if
 * loaded */
/* 0x0016D670 */
s32 Progress_LoadEventChar(Progress *p, u32 id, u32 slot) {
    u32 ok = (u8)CharLoad_EventChar(p, id, slot);

    if (ok) {
        CharLoad_Buffers(p, (u8)slot);
    }
    return ok;
}

/* 0x00173650 */
void Progress_Noop78(void) {
}

/* (the base's: the game's progress is SceneGame's second base, whose +0x74 is
 * SceneGame_PlaceCreature) */
/* 0x00173660 */
void Progress_Noop74(void) {
}

/* lock (`on`) or unlock door `door` for kind `kind` (0: bit 1, 1: bit 2, 2..5: bit 4 of the
 * door's lock bits, +0x124 + door x 4 bits 4..7), then let the door object know (+0x80) when
 * it's in this room (rooms +0x3C) */
/* 0x001780C0 */
void Progress_LockDoorFor(Progress *p, s32 door, s32 kind, s32 on) {
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
    r = (u8)VCALL(gRooms, 0x3C, s32 (*)(VObject *, s32, s32))(gRooms, door,
                                                                 VCALL(p, 0xC, s32 (*)(Progress *))(p));
    if (r != 0xFF) {
        VCALL(gDoors, 0x80, void (*)(VObject *, u32))(gDoors, r);
    }
}

/* is door `door` open: the rooms say so (+0x44 bit 0), or its state bits (+0x124 + door x 4)
 * have bit 1 without bit 3 */
/* 0x001788F0 */
s32 Progress_DoorOpen(Progress *p, u32 door) {
    u32 w;

    if ((u8)VCALL(gRooms, 0x44, s32 (*)(VObject *, u32))(gRooms, door) & 1) {
        return 1;
    }
    w = AT(p, 0x124 + (door & 0xFFFF) * 4, u32);
    if (w & 8) {
        return 0;
    }
    return (w & 2) != 0;
}

/* is the door at exit `exit` of room `room` open (as Progress_DoorOpen) */
/* 0x00178980 */
s32 Progress_ExitOpen(Progress *p, s32 room, s32 exit) {
    VObject *rooms = gRooms;
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

#define SLOT_CHAR(slot) ((slot) < 6 ? (VObject *)gCharacters[slot] : NULL)

/* character `slot` active (+0x28) */
/* 0x00177160 */
u8 Progress_CharActive(Progress *p, u32 slot) {
    VObject *c = SLOT_CHAR(slot);

    return c != NULL ? AT(c, 0x28, u8) : 0;
}

/* character `slot` still loading */
/* 0x00177260 */
s32 Progress_CharLoading(Progress *p, u32 slot) {
    VObject *c = SLOT_CHAR(slot);

    return c != NULL ? Actor_DataLoaded((Actor *)c) : 0;
}

/* character `slot`: vtable +0x14 / +0x1C / +0xC */
/* 0x001772B0 */
void Progress_CharLoad(Progress *p, u32 slot) {
    VObject *c = SLOT_CHAR(slot);

    if (c != NULL) {
        VCALL(c, 0x14, void (*)(VObject *))(c);
    }
}

/* 0x00177300 */
void Progress_CharStart2(Progress *p, u32 slot) {
    VObject *c = SLOT_CHAR(slot);

    if (c != NULL) {
        VCALL(c, 0x1C, void (*)(VObject *))(c);
    }
}

/* 0x00177350 */
void Progress_CharStart3(Progress *p, u32 slot) {
    VObject *c = SLOT_CHAR(slot);

    if (c != NULL) {
        VCALL(c, 0xC, void (*)(VObject *))(c);
    }
}

/* remove character `slot`: shut down (vtable +0x18, or the quick way when `quick`), +0x20,
 * +0x10, forget it as player / partner / pursuer; its model back to the scene heap; a loaded
 * character (2..5) also gives back its data buffers (+0x166C) and is destroyed. 1 when there
 * was one */
/* 0x001773A0 */
s32 Progress_RemoveChar(Progress *p, u32 slot, u8 quick) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);
    VObject *c = SLOT_CHAR(slot);

    if (c == NULL) {
        return 0;
    }
    if (quick) {
        Pursuer_ReleaseModelFiles((Pursuer *)c);
    } else {
        VCALL(gCharacters[slot], 0x18, void (*)(VObject *))((VObject *)gCharacters[slot]);
    }
    VCALL(gCharacters[slot], 0x20, void (*)(VObject *))((VObject *)gCharacters[slot]);
    VCALL(gCharacters[slot], 0x10, void (*)(VObject *))((VObject *)gCharacters[slot]);
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
    c = (VObject *)gCharacters[slot];
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
/* 0x001766D0 */
void Progress_RemoveAll(Progress *p) {
    u32 i;

    for (i = 0; i < 6; i++) {
        Progress_RemoveChar(p, i, 0);
    }
}

/* character `a` near character `b` (both active and not hidden (+0x29 = 1), a != b) at `pos`:
 * pos's height within b's (+0x14 .. +0xCC higher, widened by `margin`) and its distance across
 * within b's radius (+0xC8) + margin */
/* 0x00177DB0 */
s32 Progress_CharNear(Progress *p, u32 a, const f32 *pos, u32 b, f32 margin) {
    f32 d[4] __attribute__((aligned(16)));
    VObject *cb;
    f32 y;

    a &= 0xFF;
    b &= 0xFF;
    if (a == b || gCharacters[a] == NULL || AT(gCharacters[a], 0x28, u8) == 0 || AT(gCharacters[a], 0x29, u8) == 1
        || gCharacters[b] == NULL || AT(gCharacters[b], 0x28, u8) == 0 || AT(gCharacters[b], 0x29, u8) == 1) {
        return 0;
    }
    cb = (VObject *)gCharacters[b];
    y = pos[1];
    if (y <= AT(cb, 0x14, f32) - margin || !(y < margin + (AT(cb, 0x14, f32) + AT(cb, 0xCC, f32)))) {
        return 0;
    }
    sceVu0SubVector(d, (f32 *)((u8 *)cb + 0x10), (f32 *)pos);
    return __builtin_sqrtf(__builtin_fabsf(d[2] * d[2] + d[0] * d[0])) < margin + AT(cb, 0xC8, f32);
}

/* vtable +0x30 at bone `bone` of character `slot`'s model */
/* 0x00177F00 */
void Progress_CharBoneCall(Progress *p, u32 slot, s32 bone, s32 arg, f32 f) {
    f32 at[4] __attribute__((aligned(16)));
    u8 *model = AT(gCharacters[slot & 0xFF], 0xF0, u8 *);

    sceVu0CopyVector(at, Skel_Bone(AT(model, 0x810, void *), bone) + 12);
    VCALL(p, 0x30, void (*)(Progress *, u32, f32 *, s32, f32))(p, slot, at, arg, f);
}

/* the three noise slots (+0x1050, 0x20 each) picked by the bits of `which`: a free one gets
   `pos`, `kind`, two values and `f` */
/* 0x00177FA0 */
void Progress_Noise(Progress *p, const f32 *pos, u32 which, u8 kind, s16 a, s16 b, f32 f) {
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

/* slot `slot`'s relation request: hit/kind, sub, u16, s16, f32 (on +0x1014) */
/* 0x00178070 */
void Relation_Request(Progress *p, u32 slot, s32 a2, s32 a3, s32 a4, s32 a5, f32 f) {
    u8 *r = (u8 *)p + 0x1014 + (slot & 0xFF) * 0x10;

    r[0] = a2;
    r[1] = a3;
    AT(r, 0x2, u16) = a4;
    AT(r, 0x4, s16) = a5;
    AT(r, 0x8, f32) = f;
}

/* the door being used (the rooms' +0x10), unless the rooms say otherwise (+0x44 bit 0): when
   not unlocked (bit 3) but bit 0 set, set bit 2 for it and clear bit 0; 1 if it did */
/* 0x00178660 */
s32 Progress_UseDoor(Progress *p) {
    VObject *rooms = gRooms;
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

/* let go of the held door at exit `exit` (clears bit 2 too): 1 if it was held */
/* 0x00178750 */
s32 DoorHold_Release(Progress *p, s32 room, s32 exit) {
    u16 d = door_at(room, exit);
    u32 w;

    if (door_fixed(d)) {
        return 0;
    }
    w = DOOR(p, d);
    if (w & 8) {
        return 0;
    }
    if (!(w & 1)) {
        return 0;
    }
    DOOR(p, d) = w & ~4;
    DOOR(p, d) &= ~1;
    return 1;
}

/* the door at exit `exit`: unlocked, or its bit 2 */
/* 0x00178840 */
s32 DoorHold_Usable(Progress *p, s32 room, s32 exit) {
    u16 d = door_at(room, exit);
    u32 w;

    if (door_fixed(d)) {
        return 0;
    }
    w = DOOR(p, d);
    if (w & 8) {
        return 1;
    }
    return (w & 4) != 0;
}

/* Fiona: flag +0x1AD710 on, +0x1AD714 cleared, then Fiona_Shakes(1) (u8 result) */
/* 0x00176D80 */
s32 Progress_FionaFlag(Progress *p) {
    u8 *f = (u8 *)gCharPlayer;

    if (f == NULL) {
        return 0;
    }
    AT(f, 0x1AD710, u8) = 1;
    AT(f, 0x1AD714, s32) = 0;
    return ((s32 (*)(void *, s32))Fiona_Shakes)(f, 1) != 0;
}

/* the parts at +0x6FC218 (Message_ClearAll), +0x6FC340 (vtable +0x24) and +0x706440 */
/* (possibly dead code: nothing in the game references it) */
/* 0x00176720 */
void Progress_ResetParts(Progress *p) {
    VObject *o = (VObject *)((u8 *)p + 0x6FC340);

    Message_ClearAll((VObject *)((u8 *)p + 0x6FC218));
    VCALL(o, 0x24, void (*)(VObject *))(o);
    Creatures_RemoveAll((u8 *)p + 0x706440);
}

/* Progress: reset (no characters registered; its tables, the message object, the pools) */
/* 0x00176780 */
void Progress_Reset(u8 *prog) {
    s32 i, j;

    gCharacters[0] = NULL;
    gCharPlayer = NULL;
    gCharacters[1] = NULL;
    gCharacters[2] = NULL;
    gCharPartner = NULL;
    gCharacters[3] = NULL;
    gCharacters[4] = NULL;
    gCharPursuer = NULL;
    gCharacters[5] = NULL;
    Progress_SubReset(prog + 8);
    RoomSlotBytes_Clear(prog + 0xFD0);
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 7; j++) {
            Record20_Clear(prog + 0x1134 + i * 0xE0 + j * 0x20);
        }
    }
    Message_Init(prog + 0x6FC218);
    PlacedThings_ctorPool(prog + 0x6FC340);
    Creatures_ctorPools(prog + 0x706440);
}

/* ---- movies ---- */

extern void *MovieBlended_vtable[], *MovieOpaque_vtable[], *MovieHalf_vtable[], *MovieCopied_vtable[], *MovieAdded_vtable[], *MovieSmall_vtable[],
    *MovieOwnBuf_vtable[];

/* play movie `path` as scene 0, as movie class `kind` (1..6; others the plain one): 1 if it
   started */
/* 0x001768B0 */
s32 Progress_PlayMovie(Progress *p, const char *path, u32 kind) {
    static void **const sClass[7] = { MovieOwnBuf_vtable, MovieBlended_vtable, MovieOpaque_vtable, MovieHalf_vtable, MovieCopied_vtable, MovieAdded_vtable,
                                      MovieSmall_vtable };
    u8 *table = gSceneTable;
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
        Movie_ctor((Movie *)movie);
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
    Movie_SetFile(gMovie, path, 0, 1);
    return 1;
}

/* ---- vtable methods that do nothing ---- */

/* the Progress base (Progress_vtable +0x14 / +0x18) */
/* 0x00179150 */
void ProgressBase_Slot14(Progress *p) {
}

/* 0x00179140 */
void ProgressBase_Slot18(Progress *p) {
}

/* the Progress base (+0xC / +0x10): 0 */
/* 0x001794B0 */
s32 ProgressBase_SlotC(Progress *p) {
    return 0;
}

/* the play time { hours, minutes, seconds, frames (30 a second) }, each frame; it stops at
 * 99:59:59 */
/* 0x002A7630 */
void PlayTime_Tick(u8 *t) {
    if (t[0] == 99 && t[1] == 59 && t[2] == 59) {
        return;
    }
    if (++t[3] < 30) {
        return;
    }
    t[3] = 0;
    if (++t[2] < 60) {
        return;
    }
    t[2] = 0;
    if (++t[1] < 60) {
        return;
    }
    t[1] = 0;
    t[0]++;
}

/* the whole state; the copy's 400 words at +0x11C lose their bit 0 */
/* 0x002A7C70 */
void Progress_CopyState(const u8 *s, u8 *d) {
    u32 i;

    copy_words(d, s, 0x0, 0x94 / 4);
    for (i = 0; i < 0x40; i++) {
        AT(d, 0x94 + i, u8) = AT(s, 0x94 + i, u8);
    }
    copy_words(d, s, 0xD4, (0x75C - 0xD4) / 4);
    Progress_Sub75C(d + 0x75C, s + 0x75C);
    copy_words(d, s, 0x770, 0x10);
    Progress_Sub7B0(d + 0x7B0, s + 0x7B0);
    copy_words(d, s, 0x7F8, (0xFAC - 0x7F8) / 4);
    for (i = 0; i < 8; i++) {
        AT(d, 0xFAC + i * 2, s16) = AT(s, 0xFAC + i * 2, s16);
    }
    for (i = 0; i < 4; i++) {
        AT(d, 0xFBC + i, u8) = AT(s, 0xFBC + i, u8);
    }
    for (i = 0; i < 400; i++) {
        AT(d, 0x11C + i * 4, u32) &= ~1u;
    }
}

/* a sub-record (+0x7B0) */
/* 0x002A7F70 */
u8 *Progress_Sub7B0(u8 *d, const u8 *s) {
    AT(d, 0x0, u8) = AT(s, 0x0, u8);
    AT(d, 0x1, u8) = AT(s, 0x1, u8);
    AT(d, 0x2, s16) = AT(s, 0x2, s16);
    AT(d, 0x4, u32) = AT(s, 0x4, u32);
    AT(d, 0x8, s16) = AT(s, 0x8, s16);
    copy_words(d, s, 0xC, 13);
    AT(d, 0x40, s32) = AT(s, 0x40, s32);
    AT(d, 0x44, u8) = AT(s, 0x44, u8);
    return d;
}

/* a sub-record (+0x75C): four words and three bytes */
/* 0x002A8020 */
u8 *Progress_Sub75C(u8 *d, const u8 *s) {
    AT(d, 0x0, s32) = AT(s, 0x0, s32);
    AT(d, 0x4, s32) = AT(s, 0x4, s32);
    AT(d, 0x8, s32) = AT(s, 0x8, s32);
    AT(d, 0xC, s32) = AT(s, 0xC, s32);
    AT(d, 0x10, u8) = AT(s, 0x10, u8);
    AT(d, 0x11, u8) = AT(s, 0x11, u8);
    AT(d, 0x12, u8) = AT(s, 0x12, u8);
    return d;
}

/* a progress sub-object's reset */
/* 0x002A8060 */
void Progress_SubReset(u8 *p) {
    s32 i;

    AT(p, 0x0, s32) = 0;
    AT(p, 0x4, s32) = 0;
    AT(p, 0xA, u8) = 0;
    AT(p, 0x9, u8) = 0;
    AT(p, 0x8, u8) = 0;
    AT(p, 0x10, s32) = 0;
    AT(p, 0xC, s32) = 0;
    for (i = 0; i < 9; i++) {
        AT(p, 0xF8 + i * 4, s32) = 0;
    }
    Summoner_Reset(p + 0x75C);
    for (i = 0; i < 0x20; i++) {
        AT(p, 0x14 + i * 4, s32) = 0;
    }
    for (i = 0; i < 0x40; i++) {
        AT(p, 0x94 + i, u8) = 0;
    }
    for (i = 0; i < 9; i++) {
        AT(p, 0xD4 + i * 4, s32) = 0;
    }
    for (i = 0; i < 400; i++) {
        AT(p, 0x11C + i * 4, u32) &= ~0xFFu;   /* (a chain of bit-field clears) */
    }
    for (i = 0; i < 4; i++) {
        AT(p, 0x770 + i * 0x10, u8) = 0;
        AT(p, 0x774 + i * 0x10, s32) = -1;
        AT(p, 0x778 + i * 0x10, s32) = -1;
        AT(p, 0x77C + i * 0x10, u16) = 0xFFFF;
    }
    Panic_Reset(p + 0x7B0);
    AT(p, 0x9D8, f32) = 0.0f;
    AT(p, 0x9DC, f32) = 0.0f;
    AT(p, 0x9E0, f32) = 1.0f;
    AT(p, 0x9E4, f32) = 0.0f;
    AT(p, 0x9E8, f32) = 1.0f;
    AT(p, 0x9EC, f32) = 0.0f;
    AT(p, 0x9F0, f32) = 0.0f;
    AT(p, 0x9F4, f32) = 0.0f;
    AT(p, 0x9F8, f32) = 0.0f;
    AT(p, 0x9FC, f32) = 1.0f;
    AT(p, 0xA00, f32) = 0.0f;
    AT(p, 0xA04, f32) = 0.0f;
    AT(p, 0xA08, f32) = 0.0f;
    for (i = 0; i < 60; i++) {
        AT(p, 0xA0C + i * 0x18, s32) = -1;
        AT(p, 0xA10 + i * 0x18, s32) = -1;
        AT(p, 0xA14 + i * 0x18, s32) = -1;
        AT(p, 0xA18 + i * 0x18, f32) = 0.0f;
        AT(p, 0xA1C + i * 0x18, f32) = 0.0f;
        AT(p, 0xA20 + i * 0x18, s32) = 0;
    }
    for (i = 0; i < 8; i++) {
        AT(p, 0xFAC + i * 2, u16) = 0;
    }
    for (i = 0; i < 4; i++) {
        AT(p, 0xFBC + i, u8) = 0;
    }
}

/* a noise for the pursuer to hear (the loudest this frame wins): loudness `loud` (u8), in room
 * `room`, at triangle `tri` - or at door `door` (0xFFFF: none) */
/* 0x002A8440 */
void Noise_Make(u8 *n, s32 loud, s32 room, s32 tri, s32 door) {
    if (loud == 0 || room == -1 || (u8)loud < AT(n, 0x0, u8)) {
        return;
    }
    AT(n, 0x0, u8) = loud;
    AT(n, 0x4, s32) = room;
    if ((u16)door != 0xFFFF) {
        AT(n, 0x8, s32) = -1;
        AT(n, 0xC, u16) = door;
    } else {
        AT(n, 0x8, s32) = tri;
        AT(n, 0xC, u16) = 0xFFFF;
    }
}

/* clear a character request: { u8 kind, s32 a = -1, s32 b = -1, u16 c = 0xFFFF } */
/* 0x002A84A0 */
void CharRequest_Clear(u8 *r) {
    AT(r, 0x0, u8) = 0;
    AT(r, 0x4, s32) = -1;
    AT(r, 0x8, s32) = -1;
    AT(r, 0xC, u16) = 0xFFFF;
}

/* (the 12-byte entries of Progress +0x10B0: reset) */
/* 0x002A84C0 */
void SlotCmds_Reset(u8 *e) {
    AT(e, 0x0, u8) = 0;
    AT(e, 0x1, u8) = 0;
    AT(e, 0x2, u8) = 0xFF;
    AT(e, 0x3, u8) = 0xFF;
    AT(e, 0x4, s32) = 0;
    AT(e, 0x8, s32) = 0;
}

/* clear a character's own request: { u8 kind, u16, s16, f32 } */
/* 0x002A84E0 */
void OwnRequest_Clear(u8 *r) {
    AT(r, 0x0, u8) = 0;
    AT(r, 0x2, u16) = 0;
    AT(r, 0x4, u16) = 0;
    AT(r, 0x8, s32) = 0;
}

/* (the 16-byte entries of Progress +0x1014: reset) */
/* 0x002A8500 */
void Relations_Reset(u8 *e) {
    AT(e, 0x0, u8) = 0;
    AT(e, 0x1, u8) = 0;
    AT(e, 0x2, u16) = 0;
    AT(e, 0x4, u16) = 0;
    AT(e, 0x8, s32) = 0;
    AT(e, 0xC, u8) = 0;
}

/* the progress object: gProgress cleared */
/* 0x002D0F90 */
void *Progress_dtorGlobal(void *o, s32 flags) {
    if (o != NULL) {
        gProgress = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* Progress constructor: registers the global instance. */
/* 0x002D15C0 */
void *Progress_ctor(u8 *p) {
    gProgress = (Progress *)p;
    return p;
}

/* a countdown (+0x4, frames) in seconds, at least 1 while it runs (gProgress +0x764) */
/* 0x002EC410 */
s32 Countdown_Seconds(u8 *t) {
    u32 n = AT(t, 0x4, u32);

    if (n / 30 == 0 && n != 0) {
        return 1;
    }
    return n / 30;
}

/* the threat meter (gProgress +0x7B8) raised by `amount`: a big one (10 or more) also holds it
   for 30 frames and counts half toward +0x14 */
/* 0x002EF9E0 */
void Threat_Raise(u8 *o, f32 amount) {
    if (amount < 0.0f) {
        return;
    }
    if (amount < 10.0f) {
        AT(o, 0x24, f32) += amount;
    } else {
        f32 h = 0.5f * amount;

        AT(o, 0x8, s16) = 30;
        AT(o, 0x24, f32) += h;
        AT(o, 0x14, f32) += h;
    }
}

/* 0x00179160 */
s32 ProgressBase_Slot10(Progress *p) {
    return 0;
}

/* 0x00176880 */
void ProgressBase_Slot70(Progress *p) {
}

/* 0x00176890 */
void ProgressBase_Slot6C(Progress *p) {
}

/* 0x001768A0 */
void ProgressBase_Slot68(Progress *p) {
}

/* 0x00177600 */
void ProgressBase_Slot48(Progress *p) {
}

/* 0x00177610 */
void ProgressBase_Slot44(Progress *p) {
}

/* 0x001780B0 */
void ProgressBase_Slot24(Progress *p) {
}

/* 0x00178430 */
void ProgressBase_Slot20(Progress *p) {
}

/* 0x00178440 */
void ProgressBase_Slot1C(Progress *p) {
}

/* 0x00177990 */
s32 ProgressBase_Slot3C(Progress *p) {
    return 0;
}

/* 0x001779A0 */
s32 ProgressBase_Slot38(Progress *p) {
    return -1;
}

/* 0x001780A0 */
s32 ProgressBase_Slot28(Progress *p) {
    return 0;
}

/* ---- a character's cutscene motion buffer: Fiona's (+0x1AD540) as big as her model says
 * (+0xAC whether, +0xB0 how big), the others' (+0x1688) their data size +0x1C (vtable +0xFC);
 * Hewie none ---- */

/* 0x0016D050 */
void Progress_CutsceneSlot(Progress *p, s32 slot) {
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

/* 0x0016CF50 */
void Progress_CutsceneSlotDone(Progress *p, s32 slot) {
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

extern void *Progress_vtable[], *Obj46A9C0_vtable[], *Obj46A9B0_vtable[], *Creatures_vtable[], *CreaturesBase_vtable[];
extern void *PlacedThings_vtable[], *PlacedThingsBase_vtable[], *Message_vtable[], *MessageBase_vtable[], *Heap_vtable[];
extern void *BlockPool_vtable[], *SceneTableBase_vtable[];

#define VT(o, off) AT(o, off, void **)

/* a list head (vtables SceneTableBase_vtable -> BlockPool_vtable): destructor body */
static inline void ListHead_Destroy(u8 *o) {
    if (o != NULL) {
        VT(o, 0) = BlockPool_vtable;
        if (o != NULL) {
            VT(o, 0) = SceneTableBase_vtable;
        }
    }
}

/* Progress: destructor - the characters in slots 2..5 (their models unloaded by +0x6FBF00
 * +0x14, then deleted), then its parts in reverse order of construction */
/* 0x0016C8A0 */
void *Progress_dtor(Progress *p, s32 flags) {
    u8 *b = (u8 *)p;
    s32 i;

    if (p == NULL) {
        return p;
    }
    VT(b, 0) = Progress_vtable;
    Progress_Reset((u8 *)p);
    for (i = 2; i < 6; i++) {
        if (gCharacters[i] != NULL) {
            VObject *c;

            VCALL(b + 0x6FBF00, 0x14, void (*)(void *, VObject *))(b + 0x6FBF00, (VObject *)gCharacters[i]);
            c = (VObject *)gCharacters[i];
            if (c != NULL) {
                VCALL(c, 0x8, void (*)(VObject *, s32))(c, 1);
            }
            gCharacters[i] = NULL;
        }
    }
    if (b + 0x73EB60 != NULL) {
        VT(b, 0x73EB60) = Obj46A9C0_vtable;
        Progress73EC80_dtor(b + 0x73EC80, -1);
        Task_dtor((Task *)(b + 0x73EB6C), -1);
    }
    if (b + 0x73EB00 != NULL) {
        VT(b, 0x73EB4C) = Obj46A9B0_vtable;
    }
    if (b + 0x706440 != NULL) {   /* the creatures (gCreatures) */
        VT(b, 0x706468) = Creatures_vtable;
        ListHead_Destroy(b + 0x715A70);
        ListHead_Destroy(b + 0x714080);
        if (b + 0x706440 != NULL) {
            VT(b, 0x706468) = CreaturesBase_vtable;
            if (b + 0x706440 != NULL) {
                gCreatures = NULL;
            }
        }
    }
    if (b + 0x6FC340 != NULL) {   /* gPlacedThings */
        VT(b, 0x6FC340) = PlacedThings_vtable;
        ListHead_Destroy(b + 0x706380);
        if (b + 0x6FC340 != NULL) {
            VT(b, 0x6FC340) = PlacedThingsBase_vtable;
            if (b + 0x6FC340 != NULL) {
                gPlacedThings = NULL;
            }
        }
    }
    if (b + 0x6FC218 != NULL) {   /* the boot message */
        VT(b, 0x6FC218) = Message_vtable;
        if (b + 0x6FC218 != NULL) {
            VT(b, 0x6FC218) = MessageBase_vtable;
            if (b + 0x6FC218 != NULL) {
                gBootMessage = NULL;
            }
        }
    }
    if (b + 0x6FBF00 != NULL) {
        VT(b, 0x6FBF00) = Heap_vtable;
        if (b + 0x6FBF00 != NULL) {
            VT(b, 0x6FBF00) = SceneTableBase_vtable;
        }
    }
    if (p != NULL) {
        gProgress = NULL;
    }
    if ((s16)flags > 0) {
        __dl__FPv(p);
    }
    return p;
}

/* the base class's defaults (vtable +0x40 .. +0x88; the game overrides them) */
/* 0x0016CCA0 */
s32 ProgressBase_Slot40(Progress *p) { return 0; }   /* +0x40 */
/* 0x0016CCB0 */
void ProgressBase_Slot4C(Progress *p) {}            /* +0x4C */
/* 0x0016CD00 */
s32 ProgressBase_Slot50(Progress *p) { return 0; }   /* +0x50 */
/* 0x0016CCF0 */
s32 ProgressBase_Slot58(Progress *p) { return 4; }   /* +0x58 */
/* 0x0016CCE0 */
s32 ProgressBase_Slot5C(Progress *p) { return 0; }   /* +0x5C */
/* 0x0016CCD0 */
s32 ProgressBase_Slot60(Progress *p) { return 0; }   /* +0x60 */
/* 0x0016CCC0 */
s32 ProgressBase_Slot7C(Progress *p) { return 1; }   /* +0x7C */
/* 0x0016CD20 */
void ProgressBase_Slot84(Progress *p) {}            /* +0x84 */
/* 0x0016CD10 */
s32 ProgressBase_Slot88(Progress *p) { return 0; }   /* +0x88 */

/* the object at +0x73EC80: destructor (its base, vtable Helper469D00_vtable) */
/* 0x0016CC40 */
void *Progress73EC80_dtor(void *o, s32 flags) {
    extern void *Overlay_vtable[], *Helper469D00_vtable[];

    if (o != NULL) {
        VT(o, 0) = Overlay_vtable;
        if (o != NULL) {
            VT(o, 0) = Helper469D00_vtable;
        }
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* start loading the speech file `name` into a fresh 0x41000 buffer (+0x73EDC0, from the memory
   manager +0x6FBF00) */
/* 0x0016CEC0 */
void Progress_LoadSpeech(Progress *p, const char *name) {
    u8 *b = (u8 *)p;

    AT(b, 0x73EDC0, void *) = VCALL(b + 0x6FBF00, 0x10, void *(*)(void *, u32))(b + 0x6FBF00, 0x41000);
    VCALL(gFileLoader, 0xC, s32 (*)(VObject *, const char *, void *, s32, u32))(gFileLoader, name,
                                                                                AT(b, 0x73EDC0, void *), 0x10000000, 0);
}

/* once it has loaded, character `who` (+0x153C; 0xFE the third slot, 0xFF none) speaks it:
   the boot message object +0x14 / +0x10 with the character's voice (+0x1528). 0: not loaded. */
/* 0x0016CD60 */
s32 Progress_Speak(Progress *p, s32 who, s32 arg) {
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
    c = (VObject *)gCharacters[slot];
    VCALL(msg, 0x14, void (*)(VObject *, u32))(msg, AT(c, 0x1528, u8));
    VCALL(msg, 0x10, void (*)(VObject *, u32, void *, s32))(msg, AT(c, 0x1528, u8), AT(p, 0x73EDC0, void *), arg);
    return 1;
}

/* unload character slot `i`'s model (+0xF0) through the memory manager (+0x6FBF00 +0x14) */
/* 0x0016D2F0 */
void Progress_UnloadModel(Progress *p, s32 i) {
    u8 *mm = (u8 *)p + 0x6FBF00;

    VCALL(mm, 0x14, void (*)(void *, void *))(mm, AT(gCharacters[i], 0xF0, void *));
    AT(gCharacters[i], 0xF0, void *) = NULL;
}

/* ---- Hewie's commands ---- */

/* the command in progress +`at` (state, a; b cleared) given to Hewie unless he is held (action
 * state 7), and kept in its slot of the queue (+0x6FAF20 on) */
static inline void hewie_order(u8 *p, u32 at, s32 state, s32 a) {
    CharAction *act = (CharAction *)(p + at);
    CharAction *q = (CharAction *)(p + 0x6FAF20 + at);

    act->state = state;
    act->a = a;
    act->b = 0;
    if (AT(gCharPartner, 0x14E8, s32) != 7) {
        char_set_action((u8 *)gCharPartner, act);
    }
    q->state = act->state;
    q->a = act->a;
    q->b = act->b;
    q->c = act->c;
    q->d = act->d;
    q->e = act->e;
    q->f = act->f;
    q->g = act->g;
    q->h = act->h;
    q->i = act->i;
}

/* the command buttons pressed this frame (while Hewie is about and listening, and nothing holds
 * them, +0x4): R1 action 0xB / 0, L1 0xB / 2, square 8, circle 0xB / 1 */
/* 0x00174270 */
void Progress_CommandButtons(Progress *p) {
    u8 *b = (u8 *)p;

    if (gCharPartner == NULL || AT(gCharPartner, 0x28, u8) == 0 || (u8)Hewie_FreeForCommand2((Hewie *)gCharPartner) == 0
        || AT(b, 0x4, s32) != 0) {
        return;
    }
    if (gPadPressed & PAD_R1) {
        hewie_order(b, 0x1294, 0xB, 0);
    }
    if (gPadPressed & PAD_L1) {
        hewie_order(b, 0x1254, 0xB, 2);
    }
    if (gPadPressed & PAD_SQUARE) {
        hewie_order(b, 0x1234, 8, 0);
    }
    if (gPadPressed & PAD_CIRCLE) {
        hewie_order(b, 0x1214, 0xB, 1);
    }
}
