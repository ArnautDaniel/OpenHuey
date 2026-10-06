/* The rooms (global gRooms): the per-room state of the house (exits, doors, ...), kept
 * across rooms and saved with the game. */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "navmesh.h"
#include "globals.h"
#include "actor.h"
#include "hewie.h"
#include "room_map.h"
#include "scene_game_members.h"
#include "snd_place.h"
#include "stalker_math.h"
#include "stalker_progress.h"
#include "msl.h"

/* +0xC set the rooms' state: the 13 saved words `saved` (NULL: none), then rebuild (+0x90) */
/* 0x0021C760 */
void Rooms_SetState(VObject *rooms, const s32 *saved) {
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
    /* 0x9 */ u8 pad9[3];
    /* 0xC */ u32 flags;   /* 0x10 / 0x8: side 1 / 2 ..., 0x40 / 0x80: ... */
} DoorDef;

extern DoorDef kDoorDefs[];

#define ROOM_EXIT_DOOR(r, room, exit) AT(r, 0x38 + (room) * 16 + (exit) * 2, u16)   /* 0x110 x 8 */

/* +0x90 rebuild which door each room's exits lead through (0xFFFF none), from the door table,
 * leaving out the doors +0x60 says are closed off */
/* 0x0021B2B0 */
void Rooms_Rebuild(VObject *r) {
    s32 i, k;
    u16 d;

    for (i = 0; i < 0x110; i++) {
        for (k = 0; k < 8; k++) {
            ROOM_EXIT_DOOR(r, i, k) = 0xFFFF;
        }
    }
    if (kDoorDefs[0].room == 0xFFFF) {
        return;
    }
    d = 0;
    do {
        if (!VCALL(r, 0x60, s32 (*)(VObject *, s32))(r, d)) {
            for (k = 0; k < 2; k++) {
                u8 *e = (u8 *)&kDoorDefs[d] + k * 6;
                u16 *slot = &ROOM_EXIT_DOOR(r, AT(e, 0, u16), e[2]);

                if (*slot == 0xFFFF) {
                    *slot = d;
                }
            }
        }
        d++;
    } while (kDoorDefs[d].room != 0xFFFF);
}

/* +0x60 is door `d` closed off (bit d of the 13 saved words at +4; 400 doors)? */
/* 0x0021B160 */
s32 Rooms_DoorClosedOff(VObject *r, u16 d) {
    if (d >= 400) {
        return 0;
    }
    return (AT(r, 0x4 + (d >> 5) * 4, u32) & (1u << (d & 0x1F))) != 0;
}

extern char str_OBSTACLE_MTN[];   /* "OBSTACLE.MTN" */

/* start loading the obstacles' motions (OBSTACLE.MTN) into +0x380 */
/* 0x0021AF90 */
void Obstacles_LoadMotions(VObject *rooms) {
    VCALL(gFileLoader, 0xC, void (*)(VObject *, const void *, void *, u32, s32))(
        gFileLoader, str_OBSTACLE_MTN, (u8 *)rooms + 0x380, 0x10000000, 0);
}

extern void Obstacle_Reset(u8 *o);

/* reset the 5 obstacles (+0x10, 0xB0 each) */
/* 0x0021B040 */
void Obstacles_Reset(u8 *rooms) {
    s32 i;

    for (i = 0; i < 5; i++) {
        Obstacle_Reset(rooms + 0x10 + i * 0xB0);
    }
}

/* an obstacle released (nothing to do) */
/* 0x0017FA50 */
void Obstacle_Release(u8 *o) {
}

/* the 5 obstacles released */
/* 0x0021ABC0 */
void Obstacles_Release(u8 *list) {
    s32 i;

    for (i = 0; i < 5; i++) {
        Obstacle_Release(list + 0x10 + i * 0xB0);
    }
}

/* reset an obstacle */
/* 0x0017FC80 */
void Obstacle_Reset(u8 *o) {
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
/* 0x0021BEF0 */
s32 Rooms_DoorExit(VObject *r, u32 d, u32 room) {
    DoorDef *def;

    if ((d & 0xFFFF) >= 400) {
        return 0xFF;
    }
    if (VCALL(r, 0x60, s32 (*)(VObject *, u32))(r, d)) {
        return 0xFF;
    }
    def = &kDoorDefs[d & 0xFFFF];
    if (room == def->room) {
        return def->exit;
    }
    if (room == def->room2) {
        return def->exit2;
    }
    return 0xFF;
}

/* +0x64 close off door d */
/* 0x0021B1C0 */
void Rooms_CloseOff(VObject *r, u16 d) {
    if (d >= 400) {
        return;
    }
    AT(r, 0x4 + (d >> 5) * 4, u32) |= 1u << (d % 32);
}

/* +0x68 open door d again */
/* 0x0021B210 */
void Rooms_Reopen(VObject *r, u16 d) {
    if (d >= 400) {
        return;
    }
    AT(r, 0x4 + (d >> 5) * 4, u32) &= ~(1u << (d % 32));
}

extern u8 D_003D8BC6[];   /* the room table (0x40 bytes per room, 8 entries of 8 bytes), at +6 */

/* +0x48 the camera area of entry `k` of room `room` (0xFFFF: none) */
/* 0x0021BE00 */
u32 Rooms_CameraArea(VObject *r, u32 room, u32 k) {
    if (room >= 0x110) {
        return 0xFFFF;
    }
    return AT(D_003D8BC6, room * 0x40 + (k & 0xFF) * 8, u16);
}

/* the door state flag of exit `exit` of room `room` as seen from its side (side 1: flag 0x10,
 * side 2: 0x8); `checked`: only for doors with the side's check flag (0x40 / 0x80). -1: none */
/* 0x0021BC60 */
s32 Rooms_ExitSideFlag(VObject *r, u32 room, u32 exit, s32 checked) {
    DoorDef *def;
    u32 d;
    s32 s;

    exit &= 0xFF;
    d = ROOM_EXIT_DOOR(r, room, exit);
    if (d == 0xFFFF) {
        return -1;
    }
    def = &kDoorDefs[d];
    for (s = 0; s < 2; s++) {
        const u8 *side = (const u8 *)def + s * 6;

        if (AT(side, 0, u16) != room || side[2] != exit) {
            continue;
        }
        if (s == 0) {
            if (!checked || (def->flags & 0x40)) {
                return (def->flags & 0x10) ? 1 : 0;
            }
        } else if (!checked || (def->flags & 0x80)) {
            return (def->flags & 0x8) ? 1 : 0;
        }
    }
    return -1;
}

/* whether exit `exit` of room `room` leads to another room (+0x40 flag 0x20: no; +0x18 the
 * room it leads to) */
/* 0x0021B820 */
s32 Rooms_ExitLeads(VObject *r, s32 room, s32 exit) {
    s32 to;

    if ((u8)VCALL(r, 0x40, s32 (*)(VObject *, s32, s32))(r, room, exit) & 0x20) {
        return 0;
    }
    to = VCALL(r, 0x18, s32 (*)(VObject *, s32, s32))(r, room, exit);
    return to != room && to != -1;
}

/* +0x40 the door flags of exit `exit` of room `room` (+0x10 the door, then +0x44) */
/* 0x0021BEB0 */
s32 Rooms_ExitDoorFlags(VObject *r, s32 room, s32 exit) {
    u32 d = VCALL(r, 0x10, u32 (*)(VObject *, s32, s32))(r, room, exit);

    return VCALL(r, 0x44, s32 (*)(VObject *, u32))(r, d);
}

/* ---- the door table (kDoorDefs: two sides {room, exit, tri at +4} 6 bytes apart, flags +0xC)
 * and the room table (D_003D8BC0: per room 8 exits {3 triangles, camera area}, 0x40 bytes) ---- */

extern u8 D_003D8BC0[];
extern f32 D_003DE8C0[];      /* the rooms' centres (x, y, z) */
extern void *D_0046C480[], *Rooms_vtable[];

#define DOOR_SIDE(def, s) ((const u8 *)(def) + (s) * 6)
#define ROOM_EXIT(room, exit, off) AT(D_003D8BC0, (room) * 0x40 + ((exit) & 0xFF) * 8 + (off), s16)

static s32 door_closed(VObject *r, u32 d) {
    return VCALL(r, 0x60, s32 (*)(VObject *, u32))(r, d);
}

/* +0x10 the door at exit `exit` of room `room` (0xFFFF: none) */
/* 0x0021C740 */
u32 Rooms_ExitDoor(VObject *r, s32 room, u32 exit) {
    return ROOM_EXIT_DOOR(r, room, exit & 0xFF);
}

/* +0x14 the exit on the other side of that door (0xFF: none) */
/* 0x0021C6A0 */
s32 Rooms_OtherExit(VObject *r, u32 room, u32 exit) {
    u32 d;
    s32 s;

    exit &= 0xFF;
    d = ROOM_EXIT_DOOR(r, room, exit);
    if (d == 0xFFFF) {
        return 0xFF;
    }
    for (s = 0; s < 2; s++) {
        const u8 *side = DOOR_SIDE(&kDoorDefs[d], s);

        if (AT(side, 0, u16) == room && side[2] == exit) {
            return DOOR_SIDE(&kDoorDefs[d], s ^ 1)[2];
        }
    }
    return 0xFF;
}

/* +0x18 the room on the other side (-1: none) */
/* 0x0021C610 */
s32 Rooms_OtherRoom(VObject *r, u32 room, u32 exit) {
    u32 d;
    s32 s;

    exit &= 0xFF;
    d = ROOM_EXIT_DOOR(r, room, exit);
    if (d == 0xFFFF) {
        return -1;
    }
    for (s = 0; s < 2; s++) {
        const u8 *side = DOOR_SIDE(&kDoorDefs[d], s);

        if (AT(side, 0, u16) == room && side[2] == exit) {
            return AT(DOOR_SIDE(&kDoorDefs[d], s ^ 1), 0, u16);
        }
    }
    return -1;
}

/* +0x1C the room door d leads to from `room` (-1: none or closed off) */
/* 0x0021C550 */
s32 Rooms_DoorLeadsTo(VObject *r, u32 d, u32 room) {
    s32 s;

    if ((d & 0xFFFF) >= 400 || door_closed(r, d)) {
        return -1;
    }
    for (s = 0; s < 2; s++) {
        if (AT(DOOR_SIDE(&kDoorDefs[d & 0xFFFF], s), 0, u16) == room) {
            return AT(DOOR_SIDE(&kDoorDefs[d & 0xFFFF], s ^ 1), 0, u16);
        }
    }
    return -1;
}

/* an exit's triangle `which` (doors +0x50.. +0x58 for the room's real door, else the room
 * table's) */
static s32 exit_tri(u32 exit, s32 which, f32 *pos) {
    VObject *doors = gDoors;
    f32 tmp[4] __attribute__((aligned(16)));
    s32 t;

    if ((u8)VCALL(doors, 0x40, s32 (*)(VObject *, u32))(doors, exit) == 1) {
        t = VCALL(doors, 0x50 + which * 4, s32 (*)(VObject *, u32, f32 *))(doors, exit, pos != NULL ? pos : tmp);
        if (t != -1) {
            ROOMLOG("exit point %d (%d): a real door -> tri %d (%.1f %.1f %.1f)", exit, which, t,
                    (pos != NULL ? pos : tmp)[0], (pos != NULL ? pos : tmp)[1], (pos != NULL ? pos : tmp)[2]);
            return t;
        }
    }
    t = ROOM_EXIT(VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress), exit, which * 2);
    if (pos != NULL) {
        VCALL((VObject *)gNavMesh, 0xC, void (*)(VObject *, s32, f32 *))((VObject *)gNavMesh, t, pos);
    }
    ROOMLOG("exit point %d (%d) of room %d: room table -> tri %d (%.1f %.1f %.1f)", exit, which,
            VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress), t, pos ? pos[0] : 0.0, pos ? pos[1] : 0.0,
            pos ? pos[2] : 0.0);
    return t;
}

/* +0x20 / +0x24 / +0x28 exit triangles (outside, inside, through) */
/* 0x0021C490 */
s32 Rooms_ExitTriOut(VObject *r, u32 exit) {
    return exit_tri(exit, 0, NULL);
}

/* 0x0021C3D0 */
s32 Rooms_ExitTriIn(VObject *r, u32 exit) {
    return exit_tri(exit, 1, NULL);
}

/* 0x0021C310 */
s32 Rooms_ExitTriThrough(VObject *r, u32 exit) {
    return exit_tri(exit, 2, NULL);
}

/* +0x2C / +0x30 / +0x34 the same with the point (the triangle's centre for table ones) */
/* 0x0021C230 */
s32 Rooms_ExitPointOut(VObject *r, u32 exit, f32 *pos) {
    return exit_tri(exit, 0, pos);
}

/* 0x0021C150 */
s32 Rooms_ExitPointIn(VObject *r, u32 exit, f32 *pos) {
    return exit_tri(exit, 1, pos);
}

/* 0x0021C070 */
s32 Rooms_ExitPointThrough(VObject *r, u32 exit, f32 *pos) {
    return exit_tri(exit, 2, pos);
}

/* +0x38 door d's triangle on `room`'s side (-1: none) */
/* 0x0021BFB0 */
s32 Rooms_DoorTri(VObject *r, u32 d, u32 room) {
    s32 s;

    if ((d & 0xFFFF) >= 400 || door_closed(r, d)) {
        return -1;
    }
    for (s = 0; s < 2; s++) {
        if (AT(DOOR_SIDE(&kDoorDefs[d & 0xFFFF], s), 0, u16) == room) {
            return AT(DOOR_SIDE(&kDoorDefs[d & 0xFFFF], s), 4, s16);
        }
    }
    return -1;
}

/* +0x44 door d's flags (0xFF: none) */
/* 0x0021BE40 */
s32 Rooms_DoorFlags(VObject *r, u32 d) {
    if ((d & 0xFFFF) >= 400 || door_closed(r, d)) {
        return 0xFF;
    }
    return (u8)kDoorDefs[d & 0xFFFF].flags;
}

/* +0x4C whether exit `exit` of `room` has its side's flag (side 1: 2, side 2: 4) */
/* 0x0021BD50 */
s32 Rooms_ExitHasSideFlag(VObject *r, u32 room, u32 exit) {
    u32 d;
    s32 s;

    exit &= 0xFF;
    d = ROOM_EXIT_DOOR(r, room, exit);
    if (d == 0xFFFF) {
        return 0;
    }
    for (s = 0; s < 2; s++) {
        const u8 *side = DOOR_SIDE(&kDoorDefs[d], s);

        if (AT(side, 0, u16) == room && side[2] == exit &&
            (kDoorDefs[d].flags & (s == 0 ? 2 : 4))) {
            return 1;
        }
    }
    return 0;
}

/* a side's state flag (side 0: f0 with check c0, side 1: f1 / c1); -2: go on looking */
static s32 side_flag(u32 flags, s32 s, s32 checked, u32 f0, u32 c0, u32 f1, u32 c1) {
    u32 f = s == 0 ? f0 : f1;
    u32 c = s == 0 ? c0 : c1;

    if (!checked || (flags & c)) {
        return (flags & f) ? 1 : 0;
    }
    return -2;
}

/* +0x54 door d seen from `room`'s side (0x10 / 0x8, checks 0x40 / 0x80); -1: none */
/* 0x0021BB20 */
s32 Rooms_DoorFromSide(VObject *r, u32 d, u32 room, s32 checked) {
    s32 s, v;

    if ((d & 0xFFFF) >= 400 || door_closed(r, d)) {
        return -1;
    }
    for (s = 0; s < 2; s++) {
        if (AT(DOOR_SIDE(&kDoorDefs[d & 0xFFFF], s), 0, u16) == room &&
            (v = side_flag(kDoorDefs[d & 0xFFFF].flags, s, checked, 0x10, 0x40, 0x8, 0x80)) != -2) {
            return v;
        }
    }
    return -1;
}

/* +0x58 exit `exit` of `room` seen from the other side (0x8 / 0x10, checks 0x80 / 0x40) */
/* 0x0021BA30 */
s32 Rooms_ExitFromOther(VObject *r, u32 room, u32 exit, s32 checked) {
    u32 d;
    s32 s, v;

    exit &= 0xFF;
    d = ROOM_EXIT_DOOR(r, room, exit);
    if (d == 0xFFFF) {
        return -1;
    }
    for (s = 0; s < 2; s++) {
        const u8 *side = DOOR_SIDE(&kDoorDefs[d], s);

        if (AT(side, 0, u16) == room && side[2] == exit &&
            (v = side_flag(kDoorDefs[d].flags, s, checked, 0x8, 0x80, 0x10, 0x40)) != -2) {
            return v;
        }
    }
    return -1;
}

/* +0x5C door d from the other side of `room`'s */
/* 0x0021B8F0 */
s32 Rooms_DoorFromOther(VObject *r, u32 d, u32 room, s32 checked) {
    s32 s, v;

    if ((d & 0xFFFF) >= 400 || door_closed(r, d)) {
        return -1;
    }
    for (s = 0; s < 2; s++) {
        if (AT(DOOR_SIDE(&kDoorDefs[d & 0xFFFF], s), 0, u16) == room &&
            (v = side_flag(kDoorDefs[d & 0xFFFF].flags, s, checked, 0x8, 0x80, 0x10, 0x40)) != -2) {
            return v;
        }
    }
    return -1;
}

/* +0x6C the room of door d's side s */
/* 0x0021B270 */
u32 Rooms_DoorSideRoom(VObject *r, u32 d, u32 s) {
    return AT(DOOR_SIDE(&kDoorDefs[d & 0xFFFF], s & 0xFF), 0, u16);
}

/* +0x70 whether the exit leads back into the same room */
/* 0x0021B8B0 */
s32 Rooms_ExitLoops(VObject *r, s32 room, s32 exit) {
    return VCALL(r, 0x18, s32 (*)(VObject *, s32, s32))(r, room, exit) == room;
}

/* +0x78 whether the exit's door is not locked (flag 1) */
/* 0x0021B7E0 */
s32 Rooms_ExitNotLocked(VObject *r, s32 room, s32 exit) {
    return !((u8)VCALL(r, 0x40, s32 (*)(VObject *, s32, s32))(r, room, exit) & 1);
}

/* +0x7C the saved state */
/* 0x0021B2A0 */
u8 *Rooms_SavedState(VObject *r) {
    return (u8 *)r + 4;
}

/* +0x80 room's centre (0: no room) */
/* 0x0021B750 */
s32 Rooms_Centre(VObject *r, u32 room, f32 *out) {
    out[2] = 0.0f;
    out[1] = 0.0f;
    out[0] = 0.0f;
    out[3] = 1.0f;
    if (room == (u32)-1 || room >= 0x110) {
        return 0;
    }
    out[0] = D_003DE8C0[room * 3 + 0];
    out[1] = D_003DE8C0[room * 3 + 1];
    out[2] = D_003DE8C0[room * 3 + 2];
    out[3] = 1.0f;
    return 1;
}

/* +0x84 the distance between two rooms' centres */
/* 0x0021B6A0 */
f32 Rooms_Distance(VObject *r, s32 a, s32 b) {
    f32 p[4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));

    if (a == b || a == -1 || b == -1) {
        return 0.0f;
    }
    VCALL(r, 0x80, s32 (*)(VObject *, s32, f32 *))(r, a, p);
    VCALL(r, 0x80, s32 (*)(VObject *, s32, f32 *))(r, b, q);
    sceVu0SubVector(p, q, p);
    p[3] = 1.0f;
    return __builtin_sqrtf(sceVu0InnerProduct(p, p));
}

/* +0x88 how many different rooms `room`'s open exits lead to (other than `except`) */
/* 0x0021B540 */
s32 Rooms_NeighbourCount(VObject *r, s32 room, s32 except) {
    Progress *p;
    s32 to[8];
    u8 n = 0;
    u8 k, j;

    if (room == -1) {
        return 0;
    }
    p = gProgress;
    for (k = 0; k < 8; k++) {
        s32 t;
        s32 seen = 0;

        to[k] = -1;
        if (DoorHold_Usable(p, room, k)) {
            continue;
        }
        t = VCALL(r, 0x18, s32 (*)(VObject *, s32, s32))(r, room, k);
        if (t == -1 || t == except || t == room) {
            continue;
        }
        for (j = 0; j < k; j++) {
            if (t == to[j]) {
                seen = 1;
                break;
            }
        }
        if (seen) {
            continue;
        }
        to[k] = t;
        n++;
    }
    return n;
}

/* +0x8C the heading through exit `exit` (into the room); 0: none / back into this room */
/* 0x0021B3E0 */
f32 Rooms_ExitHeading(VObject *r, u32 exit) {
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    s32 ta = VCALL(r, 0x34, s32 (*)(VObject *, u32, f32 *))(r, exit, a);
    s32 room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    VObject *doors;
    f32 h;

    if (VCALL(r, 0x70, s32 (*)(VObject *, s32, u32))(r, room, exit)) {
        return 0.0f;
    }
    if (VCALL(gDoors, 0x40, s32 (*)(VObject *, u32))(gDoors, exit)) {
        doors = gDoors;
        h = VCALL(doors, 0x3C, f32 (*)(VObject *, u32))(doors, exit);
        if (VCALL(doors, 0x18, s32 (*)(VObject *, u32, f32 *))(doors, exit, a) == 1) {
            static const union { u32 u; f32 f; } kPi = {0x40490FDB};

            h = Angle_Wrap(kPi.f + h);
        }
        return h;
    }
    if (ta == VCALL(r, 0x30, s32 (*)(VObject *, u32, f32 *))(r, exit, b)) {
        return 0.0f;
    }
    sceVu0SubVector(d, b, a);
    return Vec_Heading(d);
}

/* +0x8 destructor */
/* 0x0021B0F0 */
VObject *Rooms_dtor(VObject *r, s32 flags) {
    if (r == NULL) {
        return r;
    }
    r->vtbl = Rooms_vtable;
    r->vtbl = D_0046C480;
    gRooms = NULL;
    if ((s16)flags > 0) {
        func_00100490(r);
    }
    return r;
}

/* the base's destructor (+0xA8) */
/* 0x0021C7E0 */
VObject *RoomsBase_dtor(VObject *r, s32 flags) {
    if (r == NULL) {
        return r;
    }
    r->vtbl = D_0046C480;
    gRooms = NULL;
    if ((s16)flags > 0) {
        func_00100490(r);
    }
    return r;
}

extern void Obstacle_MoveFrame(u8 *o);

/* the obstacles, each frame: each of the 5 (0xB0 apart) that is active moves */
/* 0x0021AFD0 */
void Obstacles_Update(void *list) {
    u8 *o = (u8 *)list + 0x10;
    s32 i;

    for (i = 0; i < 5; i++, o += 0xB0) {
        if (AT(o, 0x0, u8) == 1) {
            Obstacle_MoveFrame(o);
        }
    }
}

/* the obstacles' models follow them, each frame: each active one's position (+0x40) moved by
 * its offset (+0x4 x, +0x8 z) goes to its model (+0x50, at +0x20) */
/* 0x0021AC10 */
void Obstacles_ModelsFollow(void *list) {
    u8 *o = (u8 *)list + 0x10;
    s32 i;

    for (i = 0; i < 5; i++, o += 0xB0) {
        f32 p[4] __attribute__((aligned(16)));

        if (AT(o, 0x0, u8) != 1) {
            continue;
        }
        sceVu0CopyVector(p, (f32 *)(o + 0x40));
        p[0] = p[0] + AT(o, 0x4, f32);
        p[2] = p[2] + AT(o, 0x8, f32);
        sceVu0CopyVector((f32 *)(AT(o, 0x50, u8 *) + 0x20), p);
    }
}

extern s32 D_0047B24C;   /* frames left of the move below */

/* a room 0x2A handler step (the script's command 0x22, arguments `arg`): arg[3] 0 starts it
 * (90 frames); then each frame event character 0xF rises 0.5 and moves 2 along z, the script
 * waiting (2) until the frames are up (1) */
/* 0x002B1400 */
s32 Room2A_HandlerStep(void *room, s32 n, const u8 *arg) {
    u8 *c;
    u8 i;

    if (arg[3] == 0) {
        D_0047B24C = 0x5A;
        return 1;
    }
    if (--D_0047B24C == 0) {
        return 1;
    }
    i = (u8)Progress_SlotOfId(gProgress, 0xF);
#ifdef HG_NATIVE
    if (i >= 6 || gCharacters[i] == NULL) {
        return 2;   /* (not loaded on the PC build yet) */
    }
#endif
    c = (u8 *)gCharacters[i];
    AT(c, 0x14, f32) = AT(c, 0x14, f32) + 0.5f;
    AT(c, 0x18, f32) = AT(c, 0x18, f32) + 2.0f;
    return 2;
}

/* ---- an obstacle (0xB0 bytes, in the obstacles' list below): it covers pairs of nav
 * triangles (a "square"), +0x64 parts (up to 3, from +0x68, 0x18 each: the square's two
 * triangles, then steps along x / z from the reference square and its size in squares x, z);
 * +0x40 its centre (w +0x4C), +0x50 the model, +0x54 / +0x58 the reference square ---- */

void Obstacle_MarkRing(u8 *o, const f32 *dir);
s32 Obstacle_StepSquare(u8 *o, u32 *a, u32 *b, const f32 *dir);

/* a part: `dx` / `dz` squares from the reference square, `w` x `d` squares in size; -1 when
 * full (3) */
/* 0x0017E060 */
s32 Obstacle_AddPart(u8 *o, s32 dx, s32 dz, s32 w, s32 d) {
    u8 *part;

    if (AT(o, 0x64, s32) >= 3) {
        return -1;
    }
    part = o + 0x68 + AT(o, 0x64, s32) * 0x18;
    AT(part, 0x8, s32) = dx;
    AT(part, 0xC, s32) = dz;
    AT(part, 0x10, s32) = w;
    AT(part, 0x14, s32) = d;
    AT(o, 0x64, s32)++;
    return 0;
}

/* each part's square: stepped from the reference square |dx| times along +-x, |dz| along +-z */
/* 0x0017E0B0 */
void Obstacle_PartSquares(u8 *o) {
    s32 i, k, n;
    u8 *part = o + 0x68;

    for (i = 0; i < AT(o, 0x64, s32); i++, part += 0x18) {
        f32 sx[4] __attribute__((aligned(16)));
        f32 sz[4] __attribute__((aligned(16)));
        u32 a, b;

        sx[0] = AT(part, 0x8, s32) >= 0 ? 1.0f : -1.0f;
        sx[1] = 0.0f;
        sx[2] = 0.0f;
        sz[0] = 0.0f;
        sz[1] = 0.0f;
        sz[2] = AT(part, 0xC, s32) >= 0 ? 1.0f : -1.0f;
        a = AT(o, 0x54, u32);
        b = AT(o, 0x58, u32);
        n = AT(part, 0x8, s32);
        if (n <= 0) {
            n = -n;
        }
        for (k = 0; k < n; k++) {
            Obstacle_StepSquare(o, &a, &b, sx);
        }
        n = AT(part, 0xC, s32);
        if (n <= 0) {
            n = -n;
        }
        for (k = 0; k < n; k++) {
            Obstacle_StepSquare(o, &a, &b, sz);
        }
        AT(part, 0x0, u32) = a;
        AT(part, 0x4, u32) = b;
    }
}

/* the centre of triangle pair (a, b) */
static void square_centre(NavMesh *nm, u32 a, u32 b, f32 *out) {
    static const union { u32 u; f32 f; } kSixth = {0x3E2AAAAD};   /* ~1/6 */
    NavTri *t = NavMesh_Tri(nm, a);

    sceVu0CopyVector(out, t->v[0]);
    sceVu0AddVector(out, out, t->v[1]);
    sceVu0AddVector(out, out, t->v[2]);
    t = NavMesh_Tri(nm, b);
    sceVu0AddVector(out, out, t->v[0]);
    sceVu0AddVector(out, out, t->v[1]);
    sceVu0AddVector(out, out, t->v[2]);
    func_0010E640(out, out, kSixth.f);
}

/* move the square (*a, *b) one square along `dir`: from its centre 5 units along dir through
 * the mesh until two new triangles are crossed; -1 (unchanged) at an edge of the mesh */
/* 0x0017F660 */
s32 Obstacle_StepSquare(u8 *o, u32 *a, u32 *b, const f32 *dir) {
    NavMesh *nm = gNavMesh;
    u32 ta = *a, tb = *b, cur;
    f32 from[4] __attribute__((aligned(16)));
    f32 to[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    s32 n = 0;

#ifdef HG_NATIVE
    if (NavMesh_Tri(nm, ta) == NULL || NavMesh_Tri(nm, tb) == NULL) {
        return -1;
    }
#endif
    square_centre(nm, ta, tb, from);
    from[3] = 1.0f;
    sceVu0Normalize(d, dir);
    func_0010E640(d, d, 5.0f);
    sceVu0AddVector(to, from, d);
    if (VCALL(nm, 0x10, s32 (*)(NavMesh *, u32, f32 *))(nm, *a, from) == 3) {
        cur = *a;
    } else {
        cur = *b;
    }
    for (;;) {
        s32 e = VCALL(nm, 0x20, s32 (*)(NavMesh *, u32, f32 *, f32 *))(nm, cur, from, to);

        if (e == 3 || e == 4) {
            return -1;
        }
#ifdef HG_NATIVE
        if (NavMesh_Tri(nm, cur) == NULL) {
            return -1;
        }
#endif
        cur = NavMesh_Tri(nm, cur)->adj[e];
        if (cur == NAV_NONE) {
            return -1;
        }
        if (cur != ta && cur != tb) {
            tb = ta;
            ta = cur;
            if (++n == 2) {
                *a = ta;
                *b = tb;
                return 0;
            }
        }
    }
}

enum { SQ_BLOCK, SQ_UNBLOCK, SQ_TEST, SQ_FLAG, SQ_CHARS, SQ_UNFLAG, SQ_FIND };

struct sq_ctx {
    u32 any, all;   /* SQ_TEST: the flags of any / all triangles */
    s32 n;          /* SQ_CHARS: the characters' triangles */
    u32 tri[5];     /* SQ_FIND: tri[0] the one looked for (on the sides, not the corners) */
};

/* one square (a, b) of a walk; nonzero to stop */
static inline __attribute__((always_inline)) s32 sq_visit(s32 mode, struct sq_ctx *c, u32 a, u32 b, s32 corner) {
    s32 k;

    switch (mode) {
    case SQ_BLOCK:
        NavMesh_Tri(gNavMesh, a)->flags |= 0x20400000;
        NavMesh_Tri(gNavMesh, b)->flags |= 0x20400000;
        break;
    case SQ_UNBLOCK:
        NavMesh_Tri(gNavMesh, a)->flags &= 0xDFBFFFFF;
        NavMesh_Tri(gNavMesh, b)->flags &= 0xDFBFFFFF;
        break;
    case SQ_TEST: {
        u32 fa = NavMesh_Tri(gNavMesh, a)->flags;
        u32 fb = NavMesh_Tri(gNavMesh, b)->flags;

        c->any = c->any | fa | fb;
        c->all = c->all & fa & fb;
        break;
    }
    case SQ_FLAG:
        NavMesh_Tri(gNavMesh, a)->flags |= corner ? 0x20000000 : 0x20800000;
        NavMesh_Tri(gNavMesh, b)->flags |= corner ? 0x20000000 : 0x20800000;
        break;
    case SQ_UNFLAG:
        NavMesh_Tri(gNavMesh, a)->flags &= 0xDF7FFFFF;
        NavMesh_Tri(gNavMesh, b)->flags &= 0xDF7FFFFF;
        break;
    case SQ_FIND:
        if (!corner && (a == c->tri[0] || b == c->tri[0])) {
            return 1;
        }
        break;
    case SQ_CHARS:
        for (k = 0; k < c->n; k++) {
            if (a == c->tri[k] || b == c->tri[k]) {
                return 1;
            }
        }
        break;
    }
    return 0;
}

/* each square of a part (its square first stepped twice along `dir`, if any), row by row */
static inline __attribute__((always_inline)) void part_area(u8 *o, const u8 *part, const f32 *dir, s32 mode,
                                                            struct sq_ctx *c) {
    f32 sx[4] __attribute__((aligned(16)));
    f32 sz[4] __attribute__((aligned(16)));
    u32 a, b;
    s32 x, z;

    sx[1] = 0.0f;
    sx[0] = 1.0f;
    sz[2] = 1.0f;
    sz[0] = 0.0f;
    sx[2] = 0.0f;
    sz[1] = 0.0f;
    a = AT(part, 0x0, u32);
    b = AT(part, 0x4, u32);
    if (dir != NULL) {
        Obstacle_StepSquare(o, &a, &b, dir);
        Obstacle_StepSquare(o, &a, &b, dir);
    }
    for (z = 0; z < AT(part, 0x14, s32); z++) {
        for (x = 0; x < AT(part, 0x10, s32); x++) {
            sq_visit(mode, c, a, b, 0);
            Obstacle_StepSquare(o, &a, &b, sx);
        }
        Obstacle_StepSquare(o, &a, &b, sz);
    }
}

/* the ring of squares around a part from its square (a, b): each side, then a corner, round
 * from the -x -z corner; 1 when stopped */
static inline __attribute__((always_inline)) s32 part_ring(u8 *o, const u8 *part, u32 a, u32 b, s32 mode,
                                                           struct sq_ctx *c) {
    f32 sx[4] __attribute__((aligned(16)));
    f32 sz[4] __attribute__((aligned(16)));
    s32 side, k;

    sx[1] = 0.0f;
    sx[0] = -1.0f;
    sz[2] = -1.0f;
    sz[0] = 0.0f;
    sx[2] = 0.0f;
    sz[1] = 0.0f;
    Obstacle_StepSquare(o, &a, &b, sx);
    Obstacle_StepSquare(o, &a, &b, sz);
    for (side = 0; side < 4; side++) {
        f32 *d = (side & 1) ? sz : sx;
        s32 n = AT(part, (side & 1) ? 0x14 : 0x10, s32);

        d[(side & 1) ? 2 : 0] = side < 2 ? 1.0f : -1.0f;
        for (k = 0; k < n; k++) {
            Obstacle_StepSquare(o, &a, &b, d);
            if (sq_visit(mode, c, a, b, 0)) {
                return 1;
            }
        }
        if (mode == SQ_FIND && side == 3) {   /* (the search leaves out the last corner) */
            break;
        }
        Obstacle_StepSquare(o, &a, &b, d);
        if (sq_visit(mode, c, a, b, 1)) {
            return 1;
        }
    }
    return 0;
}

/* triangle `tri` is on the ring around one of the parts (moved two squares along `dir`, if
 * any): 0; else -1 */
/* 0x0017DD60 */
s32 Obstacle_OnRing(u8 *o, u32 tri, const f32 *dir) {
    struct sq_ctx c;
    u8 *part = o + 0x68;
    s32 i;

    c.tri[0] = tri;
    for (i = 0; i < AT(o, 0x64, s32); i++, part += 0x18) {
        u32 a = AT(part, 0x0, u32), b = AT(part, 0x4, u32);

        if (dir != NULL) {
            Obstacle_StepSquare(o, &a, &b, dir);
            Obstacle_StepSquare(o, &a, &b, dir);
        }
        if (part_ring(o, part, a, b, SQ_FIND, &c)) {
            return 0;
        }
    }
    return -1;
}

/* block the triangles under each part (flags |= 0x20400000), moved two squares along `dir` */
/* 0x0017F430 */
void Obstacle_Block(u8 *o, const f32 *dir) {
    s32 i;

    for (i = 0; i < AT(o, 0x64, s32); i++) {
        part_area(o, o + 0x68 + i * 0x18, dir, SQ_BLOCK, NULL);
    }
}

/* ... and unblock them */
/* 0x0017F1F0 */
void Obstacle_Unblock(u8 *o, const f32 *dir) {
    s32 i;

    for (i = 0; i < AT(o, 0x64, s32); i++) {
        part_area(o, o + 0x68 + i * 0x18, dir, SQ_UNBLOCK, NULL);
    }
}

/* mark the ring of squares around each part (moved two squares along `dir`, if any): its
 * sides 0x20800000, its corners 0x20000000 */
/* 0x0017EA30 */
void Obstacle_MarkRing(u8 *o, const f32 *dir) {
    u8 *part = o + 0x68;
    s32 i;

    for (i = 0; i < AT(o, 0x64, s32); i++, part += 0x18) {
        u32 a = AT(part, 0x0, u32), b = AT(part, 0x4, u32);

        if (dir != NULL) {
            Obstacle_StepSquare(o, &a, &b, dir);
            Obstacle_StepSquare(o, &a, &b, dir);
        }
        part_ring(o, part, a, b, SQ_FLAG, NULL);
    }
}

/* ... and unmark them (sides and corners alike) */
/* 0x0017E260 */
void Obstacle_UnmarkRing(u8 *o, const f32 *dir) {
    u8 *part = o + 0x68;
    s32 i;

    for (i = 0; i < AT(o, 0x64, s32); i++, part += 0x18) {
        u32 a = AT(part, 0x0, u32), b = AT(part, 0x4, u32);

        if (dir != NULL) {
            Obstacle_StepSquare(o, &a, &b, dir);
            Obstacle_StepSquare(o, &a, &b, dir);
        }
        part_ring(o, part, a, b, SQ_UNFLAG, NULL);
    }
}

/* the area the parts would cover two squares along `dir` is free: none of its triangles has
 * a flag of `forbid`, and all have one of `need` (0; else -1) */
/* 0x0017D580 */
s32 Obstacle_AreaFree(u8 *o, u32 forbid, u32 need, const f32 *dir) {
    struct sq_ctx c;
    s32 i;

    c.any = 0;
    c.all = need;
    for (i = 0; i < AT(o, 0x64, s32); i++) {
        part_area(o, o + 0x68 + i * 0x18, dir, SQ_TEST, &c);
    }
    if (!(c.any & forbid) && (c.all & need)) {
        return 0;
    }
    return -1;
}

/* no other character (slots 1..5, in the scene and not +0x29) stands on the ring around the
 * parts moved one or two squares along `dir`: -1; else 0 */
/* 0x0017D7F0 */
s32 Obstacle_RingClear(u8 *o, const f32 *dir) {
    struct sq_ctx c;
    u8 *part = o + 0x68;
    s32 i, pass;

    c.n = 0;
    for (i = 1; i < 6; i++) {
        u8 *ch = (u8 *)gCharacters[i];

        if (ch != NULL && AT(ch, 0x28, u8) == 1 && AT(ch, 0x29, u8) == 0) {
            c.tri[c.n++] = AT(ch, 0x34, u32);
        }
    }
    for (i = 0; i < AT(o, 0x64, s32); i++, part += 0x18) {
        for (pass = 0; pass < 2; pass++) {
            u32 a = AT(part, 0x0, u32), b = AT(part, 0x4, u32);

            Obstacle_StepSquare(o, &a, &b, dir);
            if (pass == 0) {
                Obstacle_StepSquare(o, &a, &b, dir);
            }
            if (part_ring(o, part, a, b, SQ_CHARS, &c)) {
                return 0;
            }
        }
    }
    return -1;
}

/* can it be pushed along `dir`: nobody in the way, and (with its own blocks lifted) the place
 * is walkable floor (0x100) without 0x440080 */
/* 0x0017D4E0 */
s32 Obstacle_CanPush(u8 *o, const f32 *dir) {
    s32 ok = 0;

    if (Obstacle_RingClear(o, dir) == 0) {
        return -1;
    }
    Obstacle_Unblock(o, NULL);
    if (Obstacle_AreaFree(o, 0x440080, 0x100, dir) == 0) {
        ok = 1;
    }
    Obstacle_Block(o, NULL);
    if ((u8)ok == 1) {
        return 0;
    }
    return -1;
}

/* the target square: the reference square two squares along `dir` (+0x5C / +0x60) */
/* 0x0017D290 */
void Obstacle_SetTarget(u8 *o, const f32 *dir) {
    u32 a = AT(o, 0x54, u32), b = AT(o, 0x58, u32);

    Obstacle_StepSquare(o, &a, &b, dir);
    Obstacle_StepSquare(o, &a, &b, dir);
    AT(o, 0x5C, u32) = a;
    AT(o, 0x60, u32) = b;
}

/* triangle `t` is the obstacle's square (the target one on the last step of a move) */
/* 0x0017D300 */
s32 Obstacle_IsSquare(u8 *o, u32 t) {
    if (AT(o, 0x10, s32) == AT(o, 0x14, s32) - 1) {
        return AT(o, 0x5C, u32) == t || AT(o, 0x60, u32) == t;
    }
    return AT(o, 0x54, u32) == t || AT(o, 0x58, u32) == t;
}

/* a move done: the target square becomes the reference, the parts and centre follow */
static inline __attribute__((always_inline)) void obstacle_moved(u8 *o) {
    if (AT(o, 0x5C, u32) == NAV_NONE) {
        return;
    }
    AT(o, 0x54, u32) = AT(o, 0x5C, u32);
    AT(o, 0x58, u32) = AT(o, 0x60, u32);
    AT(o, 0x5C, u32) = NAV_NONE;
    AT(o, 0x60, u32) = NAV_NONE;
    Obstacle_PartSquares(o);
    square_centre(gNavMesh, AT(o, 0x54, u32), AT(o, 0x58, u32), (f32 *)(o + 0x40));
    AT(o, 0x4C, f32) = 1.0f;
    AT(o, 0x10, s32) = AT(o, 0x14, s32);
}

/* 0x0017D370 */
void Obstacle_Moved(u8 *o) {
    obstacle_moved(o);
}

/* a frame of a move: at step 6 its scraping sound and a noise (0x1F) at its square; each step
 * moves the centre by the direction (+0x20) times the move's step (+0x30 table); at the last
 * step the move is done */
/* 0x0017FA60 */
void Obstacle_MoveFrame(u8 *o) {
    if (AT(o, 0x10, s32) == AT(o, 0x14, s32)) {
        return;
    }
    if (AT(o, 0x10, s32) == 6) {
        Progress *p;

        Sound_PlayBankAt(gSound, 0, 6, (f32 *)(o + 0x40), 0, 0);
        p = gProgress;
        Noise_Make((u8 *)p + 0x778, 0x1F, VCALL(p, 0xC, s32 (*)(Progress *))(p), AT(o, 0x54, s32), 0xFFFF);
    }
    AT(o, 0x10, s32)++;
    if (AT(o, 0x10, s32) == AT(o, 0x14, s32)) {
        obstacle_moved(o);
    } else {
        f32 v[4] __attribute__((aligned(16)));

        func_0010E640(v, (f32 *)(o + 0x20), AT(o, 0x30, f32 *)[AT(o, 0x10, s32) - 1]);
        sceVu0AddVector((f32 *)(o + 0x40), (f32 *)(o + 0x40), v);
    }
}

/* placed: the centre of the reference square, then the parts' squares, blocked (no move) and
 * settled (Obstacle_MarkRing) */
/* 0x0017F8F0 */
void Obstacle_Placed(u8 *o) {
    NavMesh *nm = gNavMesh;

    square_centre(nm, AT(o, 0x54, u32), AT(o, 0x58, u32), (f32 *)(o + 0x40));
    AT(o, 0x4C, f32) = 1.0f;
    Obstacle_PartSquares(o);
    Obstacle_Block(o, NULL);
    Obstacle_MarkRing(o, NULL);
}

/* ---- the obstacles (gObstacles, room manager +0x9380, vtable Obstacles_vtable): 5 things in the
 * room that can be pushed about (+0x10, 0xB0 each; the room object "oshi0n" is each one's
 * model, +0x50), their moves (OBSTACLE.MTN at +0x380: count, then offsets), the 5 saved
 * places (+0x580, 8 each) and spots (+0x5B0, 0x10 each) ---- */

extern void *Obstacles_vtable[], *D_0046C380[];
extern VObject *gRoomObjects;   /* the room's objects: +0x18 (name) the object */
extern u8 *D_0047A938[];      /* obstacle kinds: offset (x, z), n parts, then n x 0x10 */
extern const char D_0047A940[], D_0047A948[], D_0047A950[];   /* "oshi00" */
extern void Obstacle_UnmarkRing(u8 *o, const f32 *dir);

#define OBST(l, i) ((u8 *)(l) + 0x10 + (i) * 0xB0)

/* the room object "oshi0n" */
static u8 *obstacle_model(const char *base, u32 n) {
    char name[7];
    s32 k;

    for (k = 0; k < 7; k++) {
        name[k] = base[k];
    }
    name[5] += n;
    return VCALL(gRoomObjects, 0x18, u8 *(*)(VObject *, const char *))(gRoomObjects, name);
}

/* an obstacle's destructor */
/* 0x0021A370 */
void *Obstacle_dtor(void *o, s32 flags) {
    if (o != NULL && (s16)flags > 0) {
        func_00100490(o);
    }
    return o;
}

/* +0x8 destructor */
/* 0x0021A2E0 */
void *Obstacles_dtor(u8 *l, s32 flags) {
    if (l != NULL) {
        AT(l, 0x0, void **) = Obstacles_vtable;
        func_001002C0(l + 0x10, Obstacle_dtor, 0xB0, 5);
        AT(l, 0x0, void **) = D_0046C380;
        gObstacles = NULL;
        if ((s16)flags > 0) {
            func_00100490(l);
        }
    }
    return l;
}

/* the base's destructor */
/* 0x0021B090 */
void *ObstaclesBase_dtor(void *l, s32 flags) {
    if (l != NULL) {
        AT(l, 0x0, void **) = D_0046C380;
        gObstacles = NULL;
        if ((s16)flags > 0) {
            func_00100490(l);
        }
    }
    return l;
}

/* place obstacle i of `kind` with model "oshi0n": its offset and parts from the kind */
static void obstacle_make(u8 *l, s32 i, u32 n, s32 kind, const char *base, s32 a, s32 b, s32 saved) {
    u8 *def = D_0047A938[kind];
    u8 *o, *m, *part;
    s32 k;

    if (!(AT(def, 0x8, s32) > 0 && AT(def, 0x8, s32) < 4)) {
        return;
    }
    o = OBST(l, i);
    m = obstacle_model(base, n & 0xFF);
    if (m == NULL) {
        return;
    }
    AT(o, 0x0, u8) = 1;
    AT(o, 0x50, u8 *) = m;
    AT(o, 0x4, f32) = AT(def, 0x0, f32);
    AT(o, 0x8, f32) = AT(def, 0x4, f32);
    if (saved) {
        AT(o, 0x54, s32) = AT(l, 0x580 + i * 8, s32);
        AT(o, 0x58, s32) = AT(l, 0x584 + i * 8, s32);
    } else {
        AT(o, 0x54, s32) = a;
        AT(o, 0x58, s32) = b;
    }
    part = def + 0xC;
    for (k = 0; k < AT(def, 0x8, s32); k++, part += 0x10) {
        Obstacle_AddPart(o, AT(part, 0x0, s32), AT(part, 0x4, s32), AT(part, 0x8, s32), AT(part, 0xC, s32));
    }
    Obstacle_Placed(o);
}

/* +0x10 obstacle i (model "oshi0n", `kind`) placed at (a, b) */
/* 0x0021AE30 */
void Obstacles_PlaceAt(u8 *l, s32 i, u32 n, s32 kind, s32 a, s32 b) {
    if (i < 5) {
        obstacle_make(l, i, n, kind, D_0047A940, a, b, 0);
    }
}

/* +0x14 the same, at its saved place */
/* 0x0021ACD0 */
void Obstacles_PlaceSaved(u8 *l, s32 i, u32 n, s32 kind) {
    if (i < 5) {
        obstacle_make(l, i, n, kind, D_0047A948, 0, 0, 1);
    }
}

/* +0x18 / +0x1C obstacle i pushed (Obstacle_Block + Obstacle_MarkRing) / pulled (Obstacle_Unblock +
 * Obstacle_UnmarkRing) by `a`; -1 for none */
/* 0x0021AB30 */
s32 Obstacles_Push(u8 *l, s32 i, const f32 *a) {
    u8 *o;

    if (i < 0 || i >= 5 || AT(o = OBST(l, i), 0x0, u8) == 0) {
        return -1;
    }
    Obstacle_Block(o, a);
    Obstacle_MarkRing(o, a);
    return 0;
}

/* 0x0021AAA0 */
s32 Obstacles_Pull(u8 *l, s32 i, const f32 *a) {
    u8 *o;

    if (i < 0 || i >= 5 || AT(o = OBST(l, i), 0x0, u8) == 0) {
        return -1;
    }
    Obstacle_Unblock(o, a);
    Obstacle_UnmarkRing(o, a);
    return 0;
}

/* +0x20 the other active obstacles settle (Obstacle_MarkRing(0)); obstacle i pushed by `a` */
/* 0x0021A9D0 */
void Obstacles_Settle(u8 *l, s32 i, const f32 *a) {
    s32 k;

    for (k = 0; k < 5; k++) {
        if (AT(OBST(l, k), 0x0, u8) == 1 && k != i) {
            Obstacle_MarkRing(OBST(l, k), NULL);
        }
    }
    if (AT(OBST(l, i), 0x0, u8) == 1) {
        Obstacle_Block(OBST(l, i), a);
        Obstacle_MarkRing(OBST(l, i), a);
    }
}

/* +0x24 which active obstacles have triangle `a` on the ring round them (bit per obstacle) */
/* 0x0021A930 */
u32 Obstacles_RingMask(u8 *l, s32 a) {
    u32 mask = 0;
    s32 k;

    for (k = 0; k < 5; k++) {
        if (AT(OBST(l, k), 0x0, u8) == 1 && Obstacle_OnRing(OBST(l, k), a, NULL) == 0) {
            mask |= 1 << k;
        }
    }
    return mask;
}

/* +0x28 obstacle i starts move `mv` of OBSTACLE.MTN from `at` */
/* 0x0021A850 */
s32 Obstacles_StartMove(u8 *l, s32 i, s32 mv, const f32 *at) {
    u8 *o, *rec;

    if (i < 0 || i >= 5 || AT(o = OBST(l, i), 0x0, u8) == 0 || mv < 0 || (u32)mv >= AT(l, 0x380, u32)) {
        return -1;
    }
    AT(o, 0xC, s32) = mv;
    sceVu0CopyVector((f32 *)(o + 0x20), at);
    rec = l + AT(l, 0x384 + mv * 4, u8);
    AT(o, 0x10, s32) = 0;
    AT(o, 0x14, s32) = AT(rec, 0x380, s32);
    AT(o, 0x30, u8 *) = rec + 0x384;
    return 0;
}

/* +0x2C obstacle i: Obstacle_Moved */
/* 0x0021A7E0 */
s32 Obstacles_Moved(u8 *l, s32 i) {
    u8 *o;

    if (i < 0 || i >= 5 || AT(o = OBST(l, i), 0x0, u8) == 0) {
        return -1;
    }
    Obstacle_Moved(o);
    return 0;
}

/* +0x30 obstacle i (not stopped, +0x1): Obstacle_CanPush(a); -1 otherwise */
/* 0x0021A760 */
s32 Obstacles_CanPush(u8 *l, s32 i, const f32 *a) {
    u8 *o;

    if (i < 0 || i >= 5 || AT(o = OBST(l, i), 0x0, u8) == 0 || AT(o, 0x1, u8) == 1) {
        return -1;
    }
    return Obstacle_CanPush(o, a);
}

/* +0x34 obstacle i: Obstacle_IsSquare(a) (0 for none) */
/* 0x0021A6F0 */
s32 Obstacles_IsSquare(u8 *l, s32 i, u32 a) {
    u8 *o;

    if (i < 0 || i >= 5 || AT(o = OBST(l, i), 0x0, u8) == 0) {
        return 0;
    }
    return Obstacle_IsSquare(o, a);
}

/* +0x38 obstacle i stopped */
/* 0x0021A690 */
void Obstacles_Stop(u8 *l, s32 i) {
    if (i >= 0 && i < 5 && AT(OBST(l, i), 0x0, u8) != 0) {
        AT(OBST(l, i), 0x1, u8) = 1;
    }
}

/* +0x3C obstacle i: Obstacle_SetTarget(a) */
/* 0x0021A630 */
void Obstacles_SetTarget(u8 *l, s32 i, const f32 *a) {
    if (i >= 0 && i < 5 && AT(OBST(l, i), 0x0, u8) != 0) {
        Obstacle_SetTarget(OBST(l, i), a);
    }
}

/* +0x40 obstacle i's saved place set / +0x44 taken from it now (+0x54 / +0x58) */
/* 0x0021A600 */
void Obstacles_SetSaved(u8 *l, s32 i, s32 a, s32 b) {
    if (i >= 0 && i < 5) {
        AT(l, 0x580 + i * 8, s32) = a;
        AT(l, 0x584 + i * 8, s32) = b;
    }
}

/* 0x0021A5B0 */
void Obstacles_TakeSaved(u8 *l, s32 i) {
    if (i >= 0 && i < 5) {
        AT(l, 0x580 + i * 8, s32) = AT(OBST(l, i), 0x54, s32);
        AT(l, 0x584 + i * 8, s32) = AT(OBST(l, i), 0x58, s32);
    }
}

/* +0x48 obstacle i's spot (+0x5B0) kept: where it stands (+0x40, moved by its offset) */
/* 0x0021A510 */
void Obstacles_KeepSpot(u8 *l, s32 i) {
    if (i >= 0 && i < 5) {
        f32 *s = &AT(l, 0x5B0 + i * 0x10, f32);
        u8 *o = OBST(l, i);

        sceVu0CopyVector(s, (f32 *)(o + 0x40));
        s[0] = s[0] + AT(o, 0x4, f32);
        s[2] = s[2] + AT(o, 0x8, f32);
    }
}

/* +0x4C the model of obstacle i ("oshi0n") back at its kept spot */
/* 0x0021A440 */
void Obstacles_ModelBack(u8 *l, s32 i, u32 n) {
    u8 *m;

    if (i < 0 || i >= 5) {
        return;
    }
    m = obstacle_model(D_0047A950, n & 0xFF);
    if (m != NULL) {
        AT(OBST(l, i), 0x50, u8 *) = m;
        sceVu0CopyVector((f32 *)(m + 0x20), &AT(l, 0x5B0 + i * 0x10, f32));
    }
}

/* +0x50 where obstacle i stands (+0x40, moved by its offset) */
/* 0x0021A3C0 */
void Obstacles_Pos(u8 *l, s32 i, f32 *out) {
    if (i >= 0 && i < 5) {
        u8 *o = OBST(l, i);

        sceVu0CopyVector(out, (f32 *)(o + 0x40));
        out[0] = out[0] + AT(o, 0x4, f32);
        out[2] = out[2] + AT(o, 0x8, f32);
    }
}

extern void *Fiona_vtable[], *Character_vtable[], *Actor_vtable[];

/* Fiona's class destructor (Fiona -> 0x469C60 -> Actor) */
/* 0x0017FCD0 */
void *Fiona_dtor(void **o, s32 flags) {
    if (o != NULL) {
        o[0] = Fiona_vtable;
        o[0] = Character_vtable;
        o[0] = Actor_vtable;
        if ((s16)flags > 0) {
            Actor_Destroy((Actor *)o);
        }
    }
    return o;
}
