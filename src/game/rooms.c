/* The rooms (global D_0044E568): the per-room state of the house (exits, doors, ...), kept
 * across rooms and saved with the game. */
#include "common.h"
#include "game.h"

/* +0xC set the rooms' state: the 13 saved words `saved` (NULL: none), then rebuild (+0x90) */
void func_0021C760(VObject *rooms, const s32 *saved) {
    s32 i;

    if (saved != NULL) {
        for (i = 0; i < 13; i++) {
            AT(rooms, 0x4 + i * 4, s32) = saved[i];
        }
    } else {
        for (i = 0; i < 13; i++) {
            AT(rooms, 0x4 + i * 4, s32) = 0;
        }
    }
    VCALL(rooms, 0x90, void (*)(VObject *))(rooms);
}

/* the doors: per door two ends (room, exit); 16-byte entries, room 0xFFFF ends the table */
typedef struct DoorDef {
    /* 0x0 */ u16 room;
    /* 0x2 */ u8 exit;
    /* 0x3 */ u8 pad3[3];
    /* 0x6 */ u16 room2;
    /* 0x8 */ u8 exit2;
    /* 0x9 */ u8 pad9[7];
} DoorDef;

extern DoorDef D_003DCFC0[];

#define ROOM_EXIT_DOOR(r, room, exit) AT(r, 0x38 + (room) * 16 + (exit) * 2, u16)   /* 0x110 x 8 */

/* +0x90 rebuild which door each room's exits lead through (0xFFFF none), from the door table,
 * leaving out the doors +0x60 says are closed off */
void func_0021B2B0(VObject *r) {
    s32 i, k;
    u16 d;

    for (i = 0; i < 0x110; i++) {
        for (k = 0; k < 8; k++) {
            ROOM_EXIT_DOOR(r, i, k) = 0xFFFF;
        }
    }
    if (D_003DCFC0[0].room == 0xFFFF) {
        return;
    }
    d = 0;
    do {
        if (!VCALL(r, 0x60, s32 (*)(VObject *, s32))(r, d)) {
            for (k = 0; k < 2; k++) {
                u8 *e = (u8 *)&D_003DCFC0[d] + k * 6;
                u16 *slot = &ROOM_EXIT_DOOR(r, AT(e, 0, u16), e[2]);

                if (*slot == 0xFFFF) {
                    *slot = d;
                }
            }
        }
        d++;
    } while (D_003DCFC0[d].room != 0xFFFF);
}

/* +0x60 is door `d` closed off (bit d of the 13 saved words at +4; 400 doors)? */
s32 func_0021B160(VObject *r, u16 d) {
    if (d >= 400) {
        return 0;
    }
    return (AT(r, 0x4 + (d >> 5) * 4, u32) & (1u << (d & 0x1F))) != 0;
}

extern VObject *gFileLoader;
extern char D_004572A0[];   /* "OBSTACLE.MTN" */

/* start loading the obstacles' motions (OBSTACLE.MTN) into +0x380 */
void func_0021AF90(VObject *rooms) {
    VCALL(gFileLoader, 0xC, void (*)(VObject *, const void *, void *, u32, s32))(
        gFileLoader, D_004572A0, (u8 *)rooms + 0x380, 0x10000000, 0);
}

extern void func_0017FC80(u8 *o);

/* reset the 5 obstacles (+0x10, 0xB0 each) */
void func_0021B040(u8 *rooms) {
    s32 i;

    for (i = 0; i < 5; i++) {
        func_0017FC80(rooms + 0x10 + i * 0xB0);
    }
}

/* reset an obstacle */
void func_0017FC80(u8 *o) {
    AT(o, 0x0, u8) = 0;
    AT(o, 0x1, u8) = 0;
    AT(o, 0x54, s32) = -1;
    AT(o, 0x58, s32) = -1;
    AT(o, 0x5C, s32) = -1;
    AT(o, 0x60, s32) = -1;
    AT(o, 0x48, f32) = 0.0f;
    AT(o, 0x44, f32) = 0.0f;
    AT(o, 0x40, f32) = 0.0f;
    AT(o, 0x4C, f32) = 1.0f;
    AT(o, 0x64, s32) = 0;
    AT(o, 0x14, s32) = 0;
    AT(o, 0x10, s32) = 0;
    AT(o, 0x50, s32) = 0;
}

/* +0x3C the exit of room `room` that door `d` is (0xFF: not in that room, or closed off) */
s32 func_0021BEF0(VObject *r, u32 d, u32 room) {
    DoorDef *def;

    if ((d & 0xFFFF) >= 400) {
        return 0xFF;
    }
    if (VCALL(r, 0x60, s32 (*)(VObject *, u32))(r, d)) {
        return 0xFF;
    }
    def = &D_003DCFC0[d & 0xFFFF];
    if (room == def->room) {
        return def->exit;
    }
    if (room == def->room2) {
        return def->exit2;
    }
    return 0xFF;
}

/* +0x64 close off door d */
void func_0021B1C0(VObject *r, u16 d) {
    if (d >= 400) {
        return;
    }
    AT(r, 0x4 + (d >> 5) * 4, u32) |= 1u << (d % 32);
}

/* +0x68 open door d again */
void func_0021B210(VObject *r, u16 d) {
    if (d >= 400) {
        return;
    }
    AT(r, 0x4 + (d >> 5) * 4, u32) &= ~(1u << (d % 32));
}

extern u8 D_003D8BC6[];   /* the room table (0x40 bytes per room, 8 entries of 8 bytes), at +6 */

/* +0x48 the camera area of entry `k` of room `room` (0xFFFF: none) */
u32 func_0021BE00(VObject *r, u32 room, u32 k) {
    if (room >= 0x110) {
        return 0xFFFF;
    }
    return AT(D_003D8BC6, room * 0x40 + (k & 0xFF) * 8, u16);
}
