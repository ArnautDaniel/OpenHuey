/* The game progress's bookkeeping the pursuers use (0x1777D0..0x178F80): the 3 character slots'
 * pending commands to one another (+0x10B0, 12 bytes each: kind, sub, other slot, arg, s32,
 * f32), their relation requests (+0x1014, 16 bytes each), the per-room slot bits (+0x1000),
 * the pursuer groups (+0xFD0) and the doors they hold (+0x124 door states: bit 0 held, bit 1
 * open, bit 2, bit 3 unlocked, bits 4..7 the locked sides). Kept apart from progress.c. */

#include "common.h"
#include "game.h"
#include "progress.h"
#include "ptmf.h"
#include "globals.h"
#include "scene_game_members.h"
#include "stalker_progress.h"

#define CMD(p, k) ((u8 *)(p) + 0x10B0 + (k) * 0xC)
#define DOOR(p, d) AT(p, 0x124 + ((d) & 0xFFFF) * 4, u32)

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

/* the door at exit `exit` of room `room` */
static inline u16 door_at(s32 room, s32 exit) {
    return VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, s32))(gRooms, room, exit);
}

/* is door `d` always open (the rooms' flag bit 0) */
static inline s32 door_fixed(u16 d) {
    return (u8)VCALL(gRooms, 0x44, s32 (*)(VObject *, u32))(gRooms, d) & 1;
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
