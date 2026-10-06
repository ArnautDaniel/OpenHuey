/* The doors of the room (gDoors, vtable Doors_vtable; set up in scene_game_members.c): up to 8
 * of 0x210 bytes from +0x10, defined by PAC section 7 (+0x4: 8 offsets, 0 = none). Offsets here
 * are from the manager (`DOOR(d, i)`), i.e. 0x10 past the door's own:
 *   +0x18 its nav triangle, +0x20 / +0x30 points (+0x30 where it stands), +0x44 its turn now,
 *   +0x54 its turn at rest, +0x60 the animation playing, +0x64 its frame, +0x68 frame count,
 *   +0x6C the frames, +0x70 state (0 idle, 1 animating, 3 opened), +0x74 its opening angle,
 *   +0x78 shut side, +0x7C who opened it (0xFF none), +0x81 / +0x82 flags, +0x120 bits.
 * The animation tables are per buffer: +0x50C0[buf] the table, +0x50C8[buf] its count. */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "actor.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "navmesh.h"
#include "memcard.h"
#include "doors.h"
#include "hewie.h"
#include "scene_game.h"
#include "sound.h"
#include "vecmath.h"
#include "msl.h"
#include "ptmf.h"
#include "music.h"
#include "director.h"
#include "fiona.h"
#include "heap.h"
#include "room.h"
#include "text.h"
#include "model.h"
#include "movie.h"
#include "placed.h"
#include "pursuer.h"
#include "room_map.h"
#include "lights.h"
#include "system.h"
#include "draw_leaves.h"
#include "libc.h"
#include "sce/eekernel.h"
#include "sce/intc.h"
#include "input.h"

extern void *Doors_vtable[], *D_0046C5D0[], *D_0046C780[], *D_0046D800[], *Helper469D00_vtable[];
extern const f32 D_003E51A0[][8];   /* door kinds' areas: 4 (x, z) corners */

#define DOOR(d, i) ((u8 *)(d) + ((i) & 0xFF) * 0x210)
#define PI_F 0x1.921fb6p+1f
#define TWO_PI_F 0x1.921fb6p+2f

void *PlacedMeshHolder_ctor(u8 *p);
extern void Door_Reset(u8 *door);
extern void Door_Swing(u8 *door);
extern void Door_Draw(u8 *door);
extern void *PlacedMeshHolder_ctor(u8 *p);
/* the doorway (12 x 22, at depth z) through the door's matrix m, as the lights take it (+0x38):
 * its corners (top far, top near, bottom far, bottom near along x by `x0` -> `x1`), its facing
 * (z sign, normalised) and its top middle */
static void door_portal(sceVu0FMATRIX m, f32 x0, f32 x1, f32 z) {
    f32 q[6][4] __attribute__((aligned(16)));
    s32 i;

    q[0][0] = x0; q[0][1] = 22.0f; q[0][2] = z; q[0][3] = 1.0f;
    q[1][0] = x1; q[1][1] = 22.0f; q[1][2] = z; q[1][3] = 1.0f;
    q[2][0] = x0; q[2][1] = 0.0f; q[2][2] = z; q[2][3] = 1.0f;
    q[3][0] = x1; q[3][1] = 0.0f; q[3][2] = z; q[3][3] = 1.0f;
    for (i = 0; i < 4; i++) {
        sceVu0ApplyMatrix(q[i], m, q[i]);
    }
    q[4][0] = 0.0f; q[4][1] = 0.0f; q[4][2] = z == 0.0f ? 1.0f : -1.0f; q[4][3] = 0.0f;
    sceVu0ApplyMatrix(q[4], m, q[4]);
    sceVu0Normalize(q[4], q[4]);
    q[5][0] = 6.0f; q[5][1] = 22.0f; q[5][2] = z; q[5][3] = 1.0f;
    sceVu0ApplyMatrix(q[5], m, q[5]);
    VCALL(gLights, 0x38, void (*)(VObject *, f32 *))(gLights, q[0]);
}

/* the door's angle +0x34: its rest angle +0x44 turned by +0x64 degrees, kept above -pi */
static void door_turn(u8 *door) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kMinusPi = {0xC0490FDB}, kTwoPi = {0x40C90FDB};
    f32 a = AT(door, 0x64, f32);

    AT(door, 0x64, f32) = a;
    AT(door, 0x34, f32) = AT(door, 0x44, f32) + kPi.f * a / 180.0f;
    if (AT(door, 0x34, f32) < kMinusPi.f) {
        AT(door, 0x34, f32) = AT(door, 0x34, f32) + kTwoPi.f;
    }
}

/* the door's swing ended open (`open`) or shut: it stops (+0x60) and the progress learns of it
 * when the door (+0x4) leads somewhere from the current room */
static void door_settle(u8 *door, s32 open) {
    Progress *p;
    s32 room;

    AT(door, 0x60, s32) = 0;
    p = gProgress;
    room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    if ((VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, u32))(gRooms, room, AT(door, 0x4, u8)) & 0xFFFF) ==
        0xFFFF) {
        return;
    }
    p = gProgress;
    room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    if (open) {
        DoorHold_Open(p, room, AT(door, 0x4, u8), 0xFF);
    } else {
        DoorHold_Shut(p, room, AT(door, 0x4, u8), 0xFF);
    }
}

void Doors_Reset(u8 *p);
void Doors_ReleaseRequests(u8 *p);
void *Doors_DataBuffer(u8 *d, s32 i);
s32 Doors_SelectBuffer(u8 *d, s32 i);
void Doors_TakeRoom(u8 *d, u8 *sec);
void Doors_TakeSection8(VObject *d, u8 *sec);
void Doors_Refresh(VObject *doors, s32 exit);
s32 Doors_HasExit(VObject *doors, u8 exit);
void Doors_SetBits(VObject *doors, s32 set, u8 a, s32 b);
s32 Doors_AtDoor(VObject *doors, u32 k, const f32 *pos);
s32 Doors_Side(VObject *doors, u32 k, const f32 *pos);
void Doors_Update(void *d);
void Doors_Draw(void *d);
void Door_Draw(u8 *door);
void Door_Swing(u8 *door);
void Door_Reset(u8 *door);
void Doors_RoomIn(VObject *o);
s32 Doors_Passage(VObject *o, s32 mode, u32 door, s32 side, u32 flags);
s32 Doors_ShutSide(VObject *o, u32 door, s32 side, u32 flags);
s32 Doors_OpenSide(VObject *o, u32 door, s32 side, u32 flags);

/* door i is defined (section 7 has its entry) */
static s32 door_present(VObject *d, u32 i) {
    u8 *tbl = AT(d, 0x4, u8 *);

    if (tbl == NULL) {
        return 0;
    }
    i &= 0xFF;
    return (s32)i >= 0 && i < 8 ? AT(tbl, i * 4, s32) : 0;
}

/* the triangle a link of nav triangle t leads to (the original's NULL read past the end) */
static u32 nav_link(VObject *nm, u32 t, s32 edge) {
    u8 *tri = t < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL ? AT(nm, 0x4, u8 *) + t * 0x50 : NULL;

    return AT(tri + edge * 4, 0x30, u32);
}

/* walk the nav mesh from triangle *tri (its point `from`, moved to each triangle's centre) to
 * `to`: whether it gets there (*tri the triangle holding it) or leaves the mesh */
static s32 door_walk(VObject *nm, u32 *tri, f32 *from, f32 *to) {
    for (;;) {
        s32 r = VCALL(nm, 0x20, s32 (*)(VObject *, u32, f32 *, f32 *))(nm, *tri, from, to);

        if (r == 3) {
            return 1;
        }
        if (r == 4) {
            return 0;
        }
        *tri = nav_link(nm, *tri, r);
        if (*tri == (u32)-1) {
            return 0;
        }
        VCALL(nm, 0xC, void (*)(VObject *, u32, f32 *))(nm, *tri, from);
    }
}

/* a door's own destructor (its two draw objects) */
/* 0x00221920 */
void *Door_dtor(u8 *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x190, void **) = D_0046D800;
        AT(e, 0x190, void **) = Helper469D00_vtable;
        AT(e, 0x80, void **) = D_0046C780;
        AT(e, 0x88, s32) = 0;
        AT(e, 0x80, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            __dl__FPv(e);
        }
    }
    return e;
}

/* +0x8 destructor */
/* 0x00221890 */
void *Doors_dtor(u8 *d, s32 flags) {
    if (d != NULL) {
        AT(d, 0x0, void **) = Doors_vtable;
        __destroy_arr(d + 0x10, (void *(*)(void *, s32))Door_dtor, 0x210, 8);
        AT(d, 0x0, void **) = D_0046C5D0;
        gDoors = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(d);
        }
    }
    return d;
}

/* the base's destructor */
/* 0x00223D80 */
void *DoorsBase_dtor(u8 *d, s32 flags) {
    if (d != NULL) {
        AT(d, 0x0, void **) = D_0046C5D0;
        gDoors = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(d);
        }
    }
    return d;
}

/* the doors: 8 of 0x210 bytes (vtable Doors_vtable, global gDoors) */
/* 0x002D1200 */
void *Doors_ctor(u8 *p) {
    AT(p, 0x0, void **) = Doors_vtable;
    gDoors = (VObject *)p;
    __construct_array(p + 0x10, PlacedMeshHolder_ctor, Door_dtor, 0x210, 8);
    AT(p, 0x4, s32) = 0;
    return p;
}

/* +0xC start door i's animation `anim` from buffer `buf` (opened by character `who`) */
/* 0x002237E0 */
s32 Doors_StartAnim(VObject *d, u32 i, s32 anim, u32 who, s32 buf) {
    u8 *e, *tbl, *rec;

    if (!door_present(d, i) || anim < 0 || anim >= AT(d, 0x50C8 + buf * 4, s32) || who >= 6) {
        return -1;
    }
    e = DOOR(d, i);
    AT(e, 0x70, s32) = 1;
    AT(e, 0x7C, u32) = who;
    AT(e, 0x60, s32) = anim;
    tbl = AT(d, 0x50C0 + buf * 4, u8 *);
    rec = tbl + AT(tbl, anim * 4 + 4, s32);
    AT(e, 0x68, s32) = AT(rec, 0xC, s32);
    AT(e, 0x6C, u8 *) = rec + 0x10;
    AT(e, 0x64, s32) = 0;
    AT(e, 0x78, s32) = AT(e, 0x74, f32) < -45.0f ? 1 : 0;
    AT(e, 0x81, u8) = 0;
    return 0;
}

/* +0x10 whether character `who` stands on side `side` (0 / 1) of door i (its triangle lists in
 * the definition, +0x28: n, then n triangles, per side); -1 for no such door / side */
/* 0x00223520 */
s32 Doors_OnSide(VObject *d, u32 i, s32 side, u32 who) {
    u8 *tbl;
    s32 *list, n, k;

    if (who >= 6 || gCharacters[who] == NULL || !door_present(d, i) || side < 0 || side >= 2) {
        return -1;
    }
    tbl = AT(d, 0x4, u8 *);
    list = &AT(tbl + AT(tbl, (i & 0xFF) * 4, s32), 0x28, s32);
    n = *list++;
    if (side != 0) {
        list += n;
        n = *list++;
    }
    for (k = 0; k < n; k++) {
        if (*list++ == (s32)gCharacters[who]->a.navTri) {
            return 1;
        }
    }
    return 0;
}

/* a door's side `side` (0 / 1) passage: its walk-mesh triangles (the door's section entry +0x28:
 * a count and the triangles for side 0, then side 1's) get `flags` set (mode 0, shut) or
 * cleared (mode 1, open); -1 for no such door or side */
/* 0x00223630 */
s32 Doors_Passage(VObject *o, s32 mode, u32 door, s32 side, u32 flags) {
    u8 *tbl = AT(o, 0x4, u8 *);
    s32 off = 0, n, k;
    u32 *tri;
    NavMesh *nm;

    if (tbl != NULL && (u8)door < 8) {
        off = AT(tbl, (u8)door * 4, s32);
    }
    if (off == 0 || side < 0 || side >= 2) {
        return -1;
    }
    n = AT(tbl + off, 0x28, s32);
    tri = (u32 *)(tbl + off + 0x2C);
    if (side != 0) {
        tri += n;
        n = *tri++;
    }
    nm = gNavMesh;
    if (mode == 1) {
        for (k = 0; k < n; k++) {
            NavTri *t = NavMesh_Tri(nm, *tri++);

#ifdef HG_NATIVE
            if (t == NULL) {
                continue;
            }
#endif
            t->flags &= ~flags;
        }
    } else if (mode == 0) {
        for (k = 0; k < n; k++) {
            NavTri *t = NavMesh_Tri(nm, *tri++);

#ifdef HG_NATIVE
            if (t == NULL) {
                continue;
            }
#endif
            t->flags |= flags;
        }
    }
    return 0;
}

/* +0x14 where door i's animation `anim` (buffer `buf`) puts its user: the record's point (x,
 * z) turned with the door, on the nav mesh from the door's triangle (failing that, without
 * its x), w 1, into `out`, its turn (the record's + the door's) into rot[1]; the triangle, or
 * -1 */
/* 0x002231A0 */
s32 Doors_AnimUserSpot(VObject *d, u32 i, s32 anim, f32 *out, f32 *rot, s32 buf) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));
    f32 to[4] __attribute__((aligned(16)));
    f32 from[4] __attribute__((aligned(16)));
    VObject *nm;
    u8 *e, *tbl;
    f32 *rec;
    u32 tri;
    s32 pass;

    if (!door_present(d, i) || anim < 0 || anim >= AT(d, 0x50C8 + buf * 4, s32)) {
        return -1;
    }
    e = DOOR(d, i);
    AT(e, 0x60, s32) = anim;
    tbl = AT(d, 0x50C0 + buf * 4, u8 *);
    rec = (f32 *)(tbl + AT(tbl, anim * 4 + 4, s32));
    nm = NULL;
    for (pass = 0; pass < 2; pass++) {
        p[0] = pass == 0 ? rec[0] : 0.0f;
        p[1] = 0.0f;
        p[2] = rec[1];
        Mtx_AtHeading(m, (f32 *)(e + 0x30), AT(e, 0x54, f32));
        Mtx_ApplyPoint(to, m, p);
        tri = AT(e, 0x18, u32);
        sceVu0CopyVector(from, (f32 *)(e + 0x30));
        if (nm == NULL) {
            nm = (VObject *)gNavMesh;
        }
        if (door_walk(nm, &tri, from, to)) {
            sceVu0CopyVector(out, to);
            out[3] = 1.0f;
            rot[1] = rec[2] + AT(e, 0x54, f32);
            rot[1] = Angle_Wrap(rot[1]);
            rot[0] = 0.0f;
            rot[2] = 0.0f;
            return tri;
        }
    }
    return -1;
}

/* +0x28 door i, as it is opened from the near side (its angle past -40: open) - animating: on to
 * the next half (+0x78: 1 past -80), else whether it is shut; -1 for no door */
/* 0x00222E40 */
s32 Doors_OpenFromNear(VObject *d, u32 i) {
    u8 *e;

    if (!door_present(d, i)) {
        return -1;
    }
    e = DOOR(d, i);
    AT(e, 0x7C, s32) = 0xFF;
    if (AT(e, 0x70, s32) != 1) {
        return !(AT(e, 0x74, f32) < -40.0f);
    }
    AT(e, 0x70, s32) = 2;
    if (AT(e, 0x78, s32) == 0) {
        if (AT(e, 0x74, f32) < -40.0f) {
            return 0;
        }
        AT(e, 0x78, s32) = 1;
        AT(e, 0x81, u8) = 0;
        return 1;
    }
    if (!(AT(e, 0x74, f32) < -80.0f)) {
        return 1;
    }
    AT(e, 0x78, s32) = 0;
    return 0;
}

/* the doors +0x24: PAC section 8 (one offset per present door, in door order; entries
 * placed 0x10 apart): reset each present door and point it at its entry, then +0x7C */
/* 0x00222F60 */
void Doors_TakeSection8(VObject *d, u8 *sec) {
    u8 i;
    s32 n = 0;

    if (sec == NULL) {
        return;
    }
    for (i = 0; i < 8; i++) {
        u8 *tbl = AT(d, 0x4, u8 *);
        u8 *e;

        if (tbl == NULL || AT(tbl, i * 4, s32) == 0) {
            continue;
        }
        e = (u8 *)d + i * 0x210;
        RoomMesh_Reset(e + 0x90);
        e += 0x10;
        Door_Reset(e);
        /* (the original then skips doors whose byte +0x70 equals -1, which a byte never is) */
        AT(e, 0x0, u8 *) = sec + AT(sec, n * 4, u32) + n * 0x10;
        n++;
    }
    VCALL(d, 0x7C, void (*)(VObject *))(d);
}

/* the doors +0x18: which side of door k `pos` is on (1: in front, along its facing +0x44; 0:
 * behind; -1: no door) */
/* 0x002230A0 */
s32 Doors_Side(VObject *doors, u32 k, const f32 *pos) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 diff[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    u8 *tbl = AT(doors, 0x4, u8 *);
    u8 *e;

    k &= 0xFF;
    if (tbl == NULL || k >= 8 || AT(tbl, k * 4, s32) == 0) {
        return -1;
    }
    e = (u8 *)doors + k * 0x210 + 0x10;
    dir[0] = 0.0f;
    dir[1] = 0.0f;
    dir[2] = 1.0f;   /* (w is left unset, as in the original) */
    Mtx_TurnY(m, AT(e, 0x44, f32));
    Mtx_ApplyVector(dir, m, dir);
    sceVu0SubVector(diff, (f32 *)pos, (f32 *)(e + 0x20));
    return !(sceVu0InnerProduct(dir, diff) < 0.0f);
}

/* +0x30 door i is idle (or there is none) */
/* 0x00221B80 */
s32 Doors_IsIdle(VObject *d, u32 i) {
    if (!door_present(d, i)) {
        return 1;
    }
    return AT(DOOR(d, i), 0x70, s32) == 0;
}

/* +0x34 where door i stands (+0x30) */
/* 0x00221C00 */
s32 Doors_GetPos(VObject *d, u32 i, f32 *out) {
    if (!door_present(d, i)) {
        return -1;
    }
    sceVu0CopyVector(out, (f32 *)(DOOR(d, i) + 0x30));
    return 0;
}

/* +0x38 door i's point +0x20 */
/* 0x00221C90 */
s32 Doors_GetPoint20(VObject *d, u32 i, f32 *out) {
    if (!door_present(d, i)) {
        return -1;
    }
    sceVu0CopyVector(out, (f32 *)(DOOR(d, i) + 0x20));
    return 0;
}

/* +0x3C door i's turn at rest */
/* 0x00221D20 */
f32 Doors_GetRestTurn(VObject *d, u32 i) {
    return door_present(d, i) ? AT(DOOR(d, i), 0x54, f32) : 0.0f;
}

/* the doors +0x40: whether the room has a door at exit `exit` (section 7's entry) */
/* 0x00221DA0 */
s32 Doors_HasExit(VObject *doors, u8 exit) {
    u8 *tbl = AT(doors, 0x4, u8 *);

    if (tbl == NULL || exit >= 8) {
        return 0;
    }
    return AT(tbl, exit * 4, s32) != 0;
}

/* +0x44 door i's nav triangle (-1: none) */
/* 0x00221DF0 */
s32 Doors_GetNavTri(VObject *d, u32 i) {
    return door_present(d, i) ? AT(DOOR(d, i), 0x18, s32) : -1;
}

/* the doors (gDoors) +0x48: door `i`'s data buffer (0x2000 bytes each, from +0x10C0) */
/* 0x00221E70 */
void *Doors_DataBuffer(u8 *d, s32 i) {
    return d + (i << 13) + 0x10C0;
}

/* the doors +0x4C: select door buffer `i` (+0x50C0[i]; its first word to +0x50C8[i]); whether
 * it holds anything */
/* 0x00221E80 */
s32 Doors_SelectBuffer(u8 *d, s32 i) {
    AT(d, 0x50C0 + i * 4, u8 *) = d + (i << 13) + 0x10C0;
    AT(d, 0x50C8 + i * 4, s32) = AT(AT(d, 0x50C0 + i * 4, u8 *), 0, s32);
    return AT(d, 0x50C8 + i * 4, s32) != 0;
}

/* +0x64 door i's opening angle */
/* 0x00221EB0 */
f32 Doors_GetOpenAngle(VObject *d, u32 i) {
    return door_present(d, i) ? AT(DOOR(d, i), 0x74, f32) : 0.0f;
}

/* +0x70 door i is opened */
/* 0x00221F30 */
s32 Doors_IsOpened(VObject *d, u32 i) {
    return door_present(d, i) && AT(DOOR(d, i), 0x70, s32) == 3;
}

/* +0x88 */
/* 0x00221FB0 */
void Doors_SetFlag82(VObject *d, u32 i, u8 v) {
    AT(DOOR(d, i), 0x82, u8) = v;
}

/* the doors +0x84: set or clear bit (a + 1 + b) in each present door's bits (+0x120) */
/* 0x00221FD0 */
void Doors_SetBits(VObject *doors, s32 set, u8 a, s32 b) {
    u32 bit = (u32)(a + 1) + b;
    u32 mask = 1 << (bit & 0x1F);
    u8 *base = (u8 *)doors + (bit >> 5) * 4;
    u8 i;

    for (i = 0; i < 8; i++) {
        u8 *tbl = AT(doors, 0x4, u8 *);

        if (tbl == NULL || AT(tbl, i * 4, s32) == 0) {
            continue;
        }
        if (set) {
            AT(base + i * 0x210, 0x120, u32) |= mask;
        } else {
            AT(base + i * 0x210, 0x120, u32) &= ~mask;
        }
    }
}

/* the doors +0x80: refresh exit `exit`'s door from its state in the progress: locked (+0x40
 * says it can be) -> flags 0x5000000, else 0x1000000 / 0x4000000 per side closed */
/* 0x002220C0 */
void Doors_Refresh(VObject *doors, s32 exit) {
    Progress *p = gProgress;
    s32 room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    u32 d;
    u32 flags = 0;

    if ((u8)VCALL(gDoors, 0x40, s32 (*)(VObject *, s32))(gDoors, exit) != 1) {
        return;
    }
    d = VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, s32))(gRooms, room, exit) & 0xFFFF;
    if (d == 0xFFFF) {
        return;
    }
    VCALL(doors, 0x20, void (*)(VObject *, s32, s32, u32))(doors, exit, 0, 0x5000000);
    if ((u8)Progress_DoorUnlocked(p, d) == 1) {
        flags = 0x5000000;
    } else {
        if (!(u8)Progress_DoorPassable(p, d, 1)) {
            flags = 0x1000000;
        }
        if (!(u8)Progress_DoorPassable(p, d, 2)) {
            flags |= 0x4000000;
        }
    }
    if (flags != 0) {
        VCALL(doors, 0x1C, void (*)(VObject *, s32, s32, u32))(doors, exit, 0, flags);
    }
}

/* +0x7C the room's doors (0x210 each from +0x10) as the room comes in: each present one
 * (doors +0x40) that leads somewhere (rooms +0x10) stands open (-90 degrees, +0x64, its angle
 * +0x34 a quarter turn from +0x44) or shut, its two sides' passage set to match (+0x20 /
 * +0x1C); then each door's +0x80 */
/* 0x00222230 */
void Doors_RoomIn(VObject *o) {
    static const union { u32 u; f32 f; } kHalfPi = {0x3FC90FDB}, kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB};
    Progress *p = gProgress;
    s32 room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    VObject *doors = gDoors;
    VObject *rooms = gRooms;
    u32 i;

    for (i = 0; i < 8; i++) {
        u8 *e = (u8 *)o + i * 0x210 + 0x10;

        if ((u8)VCALL(doors, 0x40, s32 (*)(VObject *, u32))(doors, i & 0xFF) == 1 &&
            (u16)VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, room, i & 0xFF) != 0xFFFF) {
            if ((u8)Progress_ExitOpen(p, room, i & 0xFF) == 1) {
                AT(e, 0x64, f32) = -90.0f;
                AT(e, 0x34, f32) = AT(e, 0x44, f32) + -kHalfPi.f;
                if (AT(e, 0x34, f32) < -kPi.f) {
                    AT(e, 0x34, f32) = AT(e, 0x34, f32) + kTwoPi.f;
                }
                VCALL(o, 0x20, void (*)(VObject *, u32, s32, s32))(o, i & 0xFF, 0, 0x60000);
                VCALL(o, 0x1C, void (*)(VObject *, u32, s32, s32))(o, i & 0xFF, 1, 0x60000);
            } else {
                AT(e, 0x64, f32) = 0.0f;
                AT(e, 0x34, f32) = AT(e, 0x44, f32) + 0.0f;
                if (AT(e, 0x34, f32) < -kPi.f) {
                    AT(e, 0x34, f32) = AT(e, 0x34, f32) + kTwoPi.f;
                }
                VCALL(o, 0x20, void (*)(VObject *, u32, s32, s32))(o, i & 0xFF, 1, 0x60000);
                VCALL(o, 0x1C, void (*)(VObject *, u32, s32, s32))(o, i & 0xFF, 0, 0x60000);
            }
        }
        VCALL(o, 0x80, void (*)(VObject *, u32))(o, i & 0xFF);
    }
}

static void door_turn_wrap(u8 *e) {
    while (AT(e, 0x44, f32) < -PI_F) {
        AT(e, 0x44, f32) = AT(e, 0x44, f32) + TWO_PI_F;
    }
    while (!(AT(e, 0x44, f32) <= PI_F)) {
        AT(e, 0x44, f32) = AT(e, 0x44, f32) - TWO_PI_F;
    }
}

/* +0x74 door i turned `a` from rest */
/* 0x002219D0 */
void Doors_TurnBy(VObject *d, u32 i, f32 a) {
    u8 *e = DOOR(d, i);

    AT(e, 0x44, f32) = AT(e, 0x54, f32) + a;
    door_turn_wrap(e);
}

/* +0x78 door i turned to `a` */
/* 0x00221A90 */
void Doors_TurnTo(VObject *d, u32 i, f32 a) {
    u8 *e = DOOR(d, i);

    AT(e, 0x44, f32) = a;
    door_turn_wrap(e);
}

/* +0x1C shut door `door`'s side `side` (passage flags set) */
/* 0x00221B40 */
s32 Doors_ShutSide(VObject *o, u32 door, s32 side, u32 flags) {
    return Doors_Passage(o, 0, door, side, flags);
}

/* +0x20 open it (cleared) */
/* 0x00221B60 */
s32 Doors_OpenSide(VObject *o, u32 door, s32 side, u32 flags) {
    return Doors_Passage(o, 1, door, side, flags);
}

/* where `off` (in the door's frame, turned about to the far side when the events say so) from
 * door i lands on the nav mesh (`out`, on its plane): the triangle, or -1 */
/* 0x00222A60 */
s32 Doors_NavSpot(VObject *d, u32 i, const f32 *off, f32 *out) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 to[4] __attribute__((aligned(16)));
    f32 from[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    VObject *nm;
    u8 *e;
    f32 a;
    u32 tri;

    if (!door_present(d, i)) {
        return -1;
    }
    e = DOOR(d, i);
    a = AT(e, 0x54, f32);
    if (!(VCALL(gEvents, 0x20, u32 (*)(VObject *, u32, f32 *))(gEvents, i, side) & 0xFF)) {
        return -1;
    }
    if (VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, i, side) == 0) {
        a = Angle_Wrap(PI_F + a);
    }
    Mtx_AtHeading(m, (f32 *)(e + 0x30), a);
    Mtx_ApplyPoint(to, m, off);
    tri = AT(e, 0x18, u32);
    sceVu0CopyVector(from, (f32 *)(e + 0x30));
    nm = (VObject *)gNavMesh;
    if (!door_walk(nm, &tri, from, to)) {
        return -1;
    }
    sceVu0CopyVector(out, to);
    VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, tri, out);
    return tri;
}

/* +0x58 / +0x54 / +0x50 the spot 12 behind / 12 ahead / 4 behind door i */
/* 0x00222CA0 */
s32 Doors_SpotBehind(VObject *d, u32 i, f32 *out) {
    f32 off[4] __attribute__((aligned(16)));

    off[2] = -12.0f;
    off[0] = 0.0f;
    off[1] = 0.0f;
    return Doors_NavSpot(d, i, off, out);
}

/* 0x00222CD0 */
s32 Doors_SpotAhead(VObject *d, u32 i, f32 *out) {
    f32 off[4] __attribute__((aligned(16)));

    off[2] = 12.0f;
    off[0] = 0.0f;
    off[1] = 0.0f;
    return Doors_NavSpot(d, i, off, out);
}

/* 0x00222D00 */
s32 Doors_SpotJustBehind(VObject *d, u32 i, f32 *out) {
    f32 off[4] __attribute__((aligned(16)));

    off[2] = -4.0f;
    off[0] = 0.0f;
    off[1] = 0.0f;
    return Doors_NavSpot(d, i, off, out);
}

/* the doors +0x2C: whether `pos` is at door k (within 5 vertically and 20 across of its point
 * +0x20) */
/* 0x00222D30 */
s32 Doors_AtDoor(VObject *doors, u32 k, const f32 *pos) {
    f32 v[4] __attribute__((aligned(16)));
    u8 *tbl = AT(doors, 0x4, u8 *);
    f32 dy;

    k &= 0xFF;
    if (tbl == NULL || k >= 8 || AT(tbl, k * 4, s32) == 0) {
        return 0;
    }
    sceVu0SubVector(v, (f32 *)((u8 *)doors + k * 0x210 + 0x30), (f32 *)pos);
    dy = v[1];
    if (dy <= 0.0f) {
        dy = -dy;
    }
    if (!(dy <= 5.0f)) {
        return 0;
    }
    return __builtin_sqrtf(v[2] * v[2] + v[0] * v[0]) <= 20.0f;
}

/* +0x5C each door of the room the rooms know (id under 400) has its state put into the
 * progress (+0x60) */
/* 0x00222960 */
void Doors_SaveAll(VObject *d) {
    s32 room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    VObject *doors = gDoors, *rooms = gRooms;
    u32 i;

    for (i = 0; i < 8; i++) {
        if ((VCALL(doors, 0x40, u32 (*)(VObject *, u32))(doors, i & 0xFF) & 0xFF) != 1) {
            continue;
        }
        if ((VCALL(rooms, 0x10, u32 (*)(VObject *, s32, u32))(rooms, room, i & 0xFF) & 0xFFFF) < 400) {
            VCALL(d, 0x60, void (*)(VObject *, s32, u32))(d, room, i & 0xFF);
        }
    }
}

/* +0x60 door i of `room` into the progress (when the current room's doors count): open or
 * shut (+0x78), with its opener (+0x7C; 0xFF while animating) */
/* 0x00222850 */
void Doors_SaveDoor(VObject *d, s32 room, u32 i) {
    Progress *p = gProgress;
    u8 *e;
    s32 who;

    if ((Progress_CurRoomFlag(p, room, i) & 0xFF) != 1) {
        return;
    }
    e = DOOR(d, i);
    who = AT(e, 0x70, s32) != 2 ? AT(e, 0x7C, u8) : 0xFF;
    if (AT(e, 0x78, s32) == 0) {
        DoorHold_Open(p, room, i, who);
    } else {
        DoorHold_Shut(p, room, i, who);
    }
}

/* +0x68 door i (not animating) set opened: 0 from the near side (with a creak), 1 the far;
 * its passage flags 0x60000 moved to the other side; -1 otherwise */
/* 0x002226E0 */
s32 Doors_SetOpened(VObject *d, u32 i, s32 how) {
    u8 *e;

    if (!door_present(d, i)) {
        return -1;
    }
    e = DOOR(d, i);
    AT(e, 0x7C, s32) = 0;
    if (AT(e, 0x70, s32) == 1) {
        return -1;
    }
    if (how == 0) {
        AT(e, 0x81, u8) = 1;
        Door_PlaySound(e + 0x10, 0x91, 3);
        AT(e, 0x70, s32) = 3;
        AT(e, 0x78, s32) = 0;
        VCALL(d, 0x20, void (*)(VObject *, u32, s32, u32))(d, i, 0, 0x60000);
        VCALL(d, 0x1C, void (*)(VObject *, u32, s32, u32))(d, i, 1, 0x60000);
        return 0;
    }
    if (how == 1) {
        AT(e, 0x70, s32) = 3;
        AT(e, 0x78, s32) = 1;
        VCALL(d, 0x20, void (*)(VObject *, u32, s32, u32))(d, i, 1, 0x60000);
        VCALL(d, 0x1C, void (*)(VObject *, u32, s32, u32))(d, i, 0, 0x60000);
        return 0;
    }
    return -1;
}

/* +0x6C `pos` is within door i's area of `kind` (D_003E51A0: a quad in the door's frame, within
 * 5 of its height) */
/* 0x00222480 */
s32 Doors_InArea(VObject *d, s32 kind, u32 i, const f32 *pos) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 c[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    const f32 *q;
    u8 *e;
    f32 dy;
    s32 k;

    if (!door_present(d, i)) {
        return 0;
    }
    e = DOOR(d, i);
    dy = AT(e, 0x34, f32) - pos[1];
    if (dy <= 0.0f) {
        dy = -dy;
    }
    if (!(dy <= 5.0f)) {
        return 0;
    }
    Mtx_AtHeading(m, (f32 *)(e + 0x30), AT(e, 0x54, f32));
    q = D_003E51A0[kind];
    v[1] = 0.0f;
    for (k = 0; k < 4; k++) {
        v[0] = q[k * 2];
        v[2] = q[k * 2 + 1];
        Mtx_ApplyPoint(c[k], m, v);
    }
    for (k = 0; k < 4; k++) {
        f32 *a = c[k], *b = c[(k + 1) & 3];

        if ((b[0] - a[0]) * (pos[2] - a[2]) - (b[2] - a[2]) * (pos[0] - a[0]) < 0.0f) {
            return 0;
        }
    }
    return 1;
}

/* the doors released (each present one's request) and the definition dropped */
/* 0x002238F0 */
void Doors_Release(VObject *d) {
    u32 i;

    if (AT(d, 0x4, u8 *) == NULL) {
        return;
    }
    for (i = 0; i < 8; i = (i + 1) & 0xFF) {
        if (door_present(d, i)) {
            Door_ReleaseRequest(DOOR(d, i) + 0x10);
        }
    }
    AT(d, 0x4, u8 *) = NULL;
}

/* the doors, drawn each frame: each of the 8 with a definition (+0x4 table) */
/* 0x002239C0 */
void Doors_Draw(void *d) {
    u8 *doors = d;
    u32 k;

    if (AT(doors, 0x4, void **) == NULL) {
        return;
    }
    for (k = 0; k < 8; k++) {
        void **tbl = AT(doors, 0x4, void **);

        if (tbl != NULL && tbl[k] != NULL) {
            Door_Draw(doors + 0x10 + k * 0x210);
        }
    }
}

/* the doors, each frame: each of the 8 with a definition (+0x4 table) updates */
/* 0x00223A90 */
void Doors_Update(void *d) {
    u8 *doors = d;
    u32 k;

    if (AT(doors, 0x4, void **) == NULL) {
        return;
    }
    for (k = 0; k < 8; k++) {
        void **tbl = AT(doors, 0x4, void **);

        if (tbl != NULL && tbl[k] != NULL) {
            Door_Swing(doors + 0x10 + k * 0x210);
        }
    }
}

/* the doors: take the room's doors (PAC section 7: 8 offsets, 0 = no door). Each door
 * (+0x10, 0x210 each): +0x4 index, +0x8 its id (-1: none, else +0x70 set), +0x10 / +0x20
 * two points, +0x30 / +0x40 a third (w 0), +0x72 present */
/* 0x00223B60 */
void Doors_TakeRoom(u8 *d, u8 *sec) {
    u8 i;

    AT(d, 0x4, u8 *) = sec;
    if (sec == NULL) {
        return;
    }
    for (i = 0; i < 8; i++) {
        u8 *e = d + 0x10 + i * 0x210;
        u8 *src;

        AT(e, 0x70, u8) = 0;
        if (AT(sec, i * 4, u32) == 0) {
            continue;
        }
        src = sec + AT(sec, i * 4, u32);
        AT(e, 0x4, u8) = i;
        AT(e, 0x8, s32) = AT(src, 0, s32);
        if (AT(e, 0x8, s32) != -1) {
            AT(e, 0x70, u8) = 1;
        }
        vu0_CopyXYZ((f32 *)(e + 0x10), (f32 *)(src + 0x4));
        AT(e, 0x1C, f32) = 1.0f;
        vu0_CopyXYZ((f32 *)(e + 0x20), (f32 *)(src + 0x10));
        AT(e, 0x2C, f32) = 1.0f;
        vu0_CopyXYZ((f32 *)(e + 0x40), (f32 *)(src + 0x1C));
        AT(e, 0x4C, f32) = 0.0f;
        sceVu0CopyVector((f32 *)(e + 0x30), (f32 *)(e + 0x40));
        AT(e, 0x72, u8) = 1;
    }
}

/* release the requests of the 8 doors that have one */
/* 0x00223C90 */
void Doors_ReleaseRequests(u8 *p) {
    u8 i;

    if (AT(p, 0x4, void *) == NULL) {
        return;
    }
    for (i = 0; i < 8; i++) {
        u8 *tbl = AT(p, 0x4, u8 *);
        s32 used = tbl != NULL && i < 8 ? AT(tbl, i * 4, s32) : 0;

        if (used) {
            Door_ReleaseRequest(p + 0x10 + i * 0x210);
        }
    }
    AT(p, 0x4, void *) = NULL;
}

/* 0x00223D60 */
void Doors_Reset(u8 *p) {
    AT(p, 0x4, s32) = 0;
    AT(p, 0x50C0, s32) = 0;
    AT(p, 0x50C8, s32) = 0;
    AT(p, 0x50C4, s32) = 0;
    AT(p, 0x50CC, s32) = 0;
}

/* ---- a door sound and the noise it makes ---- */

/* door `e` (its own +0x0) plays sound `id` and is heard as a noise by its opener's slot (+0x6C:
 * 0..2, else 3): `how` 1 / 2 quiet (15), 3 / 4 loud (95), else silent */
/* 0x00220D10 */
void Door_PlaySound(u8 *e, s32 id, s32 how) {
    Progress *p;
    s32 slot, loud = 0, room;
    u32 door;

    Sound_PlayBankAt(gSound, id & 0xFFFF, 5, (f32 *)(e + 0x10), 0, 0);
    switch (AT(e, 0x6C, s32)) {
    case 2: slot = 2; break;
    case 1: slot = 1; break;
    case 0: slot = 0; break;
    default: slot = 3; break;
    }
    switch (how & 0xFF) {
    case 1:
    case 2:
        loud = 0xF;
        break;
    case 3:
    case 4:
        loud = 0x5F;
        break;
    }
    p = gProgress;
    door = VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, u32))(gRooms, VCALL(p, 0xC, s32 (*)(Progress *))(p), AT(e, 0x4, u8)) & 0xFFFF;
    room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    Noise_Make((u8 *)p + 0x778 + (slot & 0xFF) * 0x10, loud, room, -1, door);
}

/* draw a shown door (+0x70) that has a model (+0x0): its draw object (+0x80) placed from the
 * door's position / rotation; when it is on screen and open, its effect (+0x190, when +0x72)
 * and, open past 78.75 degrees, the doorway on both sides for the lights */
/* 0x00220E80 */
void Door_Draw(u8 *door) {
    u8 *obj = door + 0x80;
    sceVu0FMATRIX m;

    if (AT(door, 0x70, u8) != 1 || AT(door, 0x0, void *) == NULL) {
        return;
    }
    sceVu0CopyVector((f32 *)(obj + 0x70), (f32 *)(door + 0x10));
    sceVu0CopyVector((f32 *)(obj + 0x80), (f32 *)(door + 0x30));
    AT(obj, 0x64, s32) = -1;
    AT(obj, 0x68, s32) = 0;
    AT(obj, 0x8, void *) = AT(door, 0x0, void *);
    AT(obj, 0x18, s32) = 0;
    VCALL(gRenderer, 0xC, void (*)(VObject *, u8 *, s32, s32))(gRenderer, obj, 1, 0);
    if (!PlacedMesh_LastShown(obj) || !(AT(door, 0x64, f32) < 0.0f)) {
        return;
    }
    if (AT(door, 0x72, u8) == 1) {
        DoorShadow_Queue(door + 0x190, AT(door, 0x8, u32), (f32 *)(door + 0x10), (f32 *)(door + 0x30));
    }
    if (!(AT(door, 0x64, f32) <= -78.75f)) {
        return;
    }
    sceVu0UnitMatrix(m);
    sceVu0RotMatrix(m, m, (f32 *)(door + 0x30));
    sceVu0TransMatrix(m, m, (f32 *)(door + 0x10));
    door_portal(m, 12.0f, 0.0f, 0.0f);
    door_portal(m, 0.0f, 12.0f, -1.0f);
}

/* release a door's (?) pending request: only while it is active (+0x70 == 1) */
/* 0x002212D0 */
void Door_ReleaseRequest(u8 *p) {
    if (AT(p, 0x70, u8) == 1 && AT(p, 0x0, s32) != 0) {
        AT(p, 0x0, s32) = 0;
    }
}

/* a door's swing, each frame (+0x60: 1 along a list of angles +0x5C (+0x54 of +0x58 done),
 * 2 by 5 degrees a frame, 3 slammed by 15; +0x68 0 opening to -90 degrees (+0x64), else
 * closing to 0), while it is shown (+0x70); the creak / latch sounds once (+0x71) */
/* 0x00221300 */
void Door_Swing(u8 *door) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kMinusPi = {0xC0490FDB}, kTwoPi = {0x40C90FDB};
    f32 step;

    switch (AT(door, 0x60, s32)) {
    case 1: {
        f32 *key;

        if (AT(door, 0x70, u8) != 1 || (key = AT(door, 0x5C, f32 *)) == NULL) {
            return;
        }
        AT(door, 0x64, f32) = *key;
        AT(door, 0x34, f32) = AT(door, 0x44, f32) + kPi.f * *key / 180.0f;
        if (AT(door, 0x34, f32) < kMinusPi.f) {
            AT(door, 0x34, f32) = AT(door, 0x34, f32) + kTwoPi.f;
        }
        AT(door, 0x54, s32) += 1;
        if (AT(door, 0x54, s32) == AT(door, 0x58, s32)) {
            AT(door, 0x5C, s32) = 0;
            AT(door, 0x60, s32) = 0;
        } else {
            AT(door, 0x5C, s32) += 4;
        }
        if (AT(door, 0x71, u8) != 0) {
            return;
        }
        if (AT(door, 0x68, s32) == 1) {
            if (!(AT(door, 0x64, f32) <= -6.0f)) {
                AT(door, 0x71, u8) = 1;
                Door_PlaySound(door, 0x28, 2);
            }
        } else if (AT(door, 0x64, f32) < 0.0f) {
            AT(door, 0x71, u8) = 1;
            Door_PlaySound(door, 0x27, 1);
        }
        return;
    }
    case 2:
        if (AT(door, 0x70, u8) != 1) {
            return;
        }
        if (AT(door, 0x68, s32) == 0) {
            AT(door, 0x64, f32) = AT(door, 0x64, f32) - 5.0f;
            if (AT(door, 0x64, f32) < -90.0f) {
                AT(door, 0x64, f32) = -90.0f;
                door_settle(door, 1);
            }
        } else {
            AT(door, 0x64, f32) = AT(door, 0x64, f32) + 5.0f;
            if (AT(door, 0x71, u8) == 0 && !(AT(door, 0x64, f32) <= -6.0f)) {
                AT(door, 0x71, u8) = 1;
                Door_PlaySound(door, 0x28, 0);
            }
            if (!(AT(door, 0x64, f32) <= 0.0f)) {
                AT(door, 0x64, f32) = 0.0f;
                door_settle(door, 0);
            }
        }
        door_turn(door);
        return;
    case 3:
        if (AT(door, 0x70, u8) != 1) {
            return;
        }
        step = 15.0f;
        if (AT(door, 0x68, s32) == 0) {
            AT(door, 0x64, f32) = AT(door, 0x64, f32) - step;
            if (AT(door, 0x64, f32) < -90.0f) {
                AT(door, 0x64, f32) = -90.0f;
                door_settle(door, 1);
            }
        } else {
            AT(door, 0x64, f32) = AT(door, 0x64, f32) + step;
            if (!(AT(door, 0x64, f32) <= 0.0f)) {
                AT(door, 0x71, u8) = 1;
                Door_PlaySound(door, 0x28, 4);
                AT(door, 0x64, f32) = 0.0f;
                door_settle(door, 0);
            }
        }
        door_turn(door);
        return;
    }
}

/* reset a door: not opening (+0x5C), no frame (+0x60) */
/* 0x00221880 */
void Door_Reset(u8 *door) {
    AT(door, 0x5C, s32) = 0;
    AT(door, 0x60, s32) = 0;
}

/* ---- the route planner (gRoutePlanner, SceneGame +0xF6A940, vtable RoutePlanner_vtable): a breadth-first
 * search from room to room through the doors (gRooms's links), up to 128 steps:
 *   +0x4 from, +0x8 to, +0xC the door wanted at the end (-1: any), +0x10 the side the doors
 *   must open from, +0x14 rooms to avoid (bits, NULL none), +0x18 where the route goes (door
 *   ids, u16; NULL none), +0x1C / +0x1E the queue's head / tail, +0x20 the most steps (-1 no
 *   limit), +0x24 the door being looked at, +0x28 the walker's kind, +0x2C the steps (0xC
 *   each: door, room, door on the far side, depth, from), +0x62C the step looked at, +0x630
 *   rooms seen (bits) ---- */

extern void *RoutePlanner_vtable[], *D_0046C530[];

typedef struct RouteStep {
    u16 door, room, far, depth;
    struct RouteStep *from;
} RouteStep;

/* the doors out of `room` queued as steps (those not yet seen, not to avoid, unlocked and
 * openable from the right side; with +0x24 0 / 1, only through the door matching it); -1 when
 * the queue is full */
/* 0x002206F0 */
s32 RoutePlanner_QueueDoors(u8 *rp, s32 room) {
    VObject *rooms = gRooms;
    Progress *p = gProgress;
    u32 i;

    for (i = 0; i < 8; i++) {
        u32 d = VCALL(rooms, 0x10, u32 (*)(VObject *, s32, u32))(rooms, room, i & 0xFF) & 0xFFFF;
        u32 m, *seen;

        if (d == 0xFFFF) {
            continue;
        }
        if ((AT(rp, 0x24, s32) == 0 || AT(rp, 0x24, s32) == 1) &&
            AT(rp, 0x24, s32) != VCALL(rooms, 0x54, s32 (*)(VObject *, u32, s32, u32))(rooms, d, room, AT(rp, 0x28, u8))) {
            continue;
        }
        m = 1u << (d & 0x1F);
        seen = &AT(rp, 0x630 + (d >> 5) * 4, u32);
        if (*seen & m) {
            continue;
        }
        if (!(AT(rp, 0x14, u32 *) != NULL && (m & AT(rp, 0x14, u32 *)[d >> 5])) && !(Progress_DoorUnlocked(p, d) & 0xFF) &&
            (Progress_DoorPassable(p, d, AT(rp, 0x10, u8)) & 0xFF) == 1) {
            s16 n = AT(rp, 0x1E, s16)++;
            RouteStep *s = (RouteStep *)(rp + 0x2C) + n;

            if (!(AT(rp, 0x1E, s16) < 0x80)) {
                return -1;
            }
            s->door = d;
            s->room = VCALL(rooms, 0x1C, u16 (*)(VObject *, u32, s32))(rooms, d, room);
            s->far = VCALL(rooms, 0x5C, u16 (*)(VObject *, u32, s32, u32))(rooms, d, room, AT(rp, 0x28, u8));
            s->from = AT(rp, 0x62C, RouteStep *);
            s->depth = s->from != NULL ? s->from->depth + 1 : 0;
        }
        *seen |= m;
    }
    return 0;
}

/* search on: the steps in turn until one reaches the goal room (through the wanted door) - its
 * route written backwards from +0x18 (which ends before its start) - and the number of doors;
 * -1 when none is left or the limit is reached */
/* 0x00220930 */
s32 RoutePlanner_Search(u8 *rp) {
    for (;;) {
        RouteStep *s;
        s16 lim;

        if (AT(rp, 0x1C, s16) == AT(rp, 0x1E, s16)) {
            return -1;
        }
        s = (RouteStep *)(rp + 0x2C) + AT(rp, 0x1C, s16);
        AT(rp, 0x62C, RouteStep *) = s;
        AT(rp, 0x1C, s16)++;
        lim = AT(rp, 0x20, s16);
        if (lim >= 0 && !((s16)AT(rp, 0x62C, RouteStep *)->depth < lim)) {
            return -1;
        }
        s = AT(rp, 0x62C, RouteStep *);
        AT(rp, 0x24, s32) = s->far;
        if (s->room == AT(rp, 0x8, u32) && (AT(rp, 0xC, s32) == -1 || AT(rp, 0x24, s32) == AT(rp, 0xC, s32))) {
            RouteStep *t;
            s32 n = 0, k;

            for (t = s; t != NULL; t = t->from) {
                n++;
            }
            if (AT(rp, 0x18, u16 *) != NULL) {
                AT(rp, 0x18, u16 *) += n - 1;
                for (k = 0; k < n; k++) {
                    *AT(rp, 0x18, u16 *) = s->door;
                    AT(rp, 0x18, u16 *) -= 1;
                    s = s->from;
                }
            }
            return n;
        }
        if (RoutePlanner_QueueDoors(rp, s->room) == -1) {
            return -1;
        }
    }
}

/* +0xC a route from room `from` to room `to` (doors openable from `side`, avoiding `avoid`,
 * into `out`) for a walker of `kind`, starting from door `door`, ending at door `want` (-1:
 * any), in at most `max` doors: its length, 0 already there, -1 none */
/* 0x00220BC0 */
s32 RoutePlanner_FindRoute(u8 *rp, u32 from, u32 to, s32 side, u32 *avoid, u16 *out, s32 kind, s32 door, s32 want, s16 max) {
    s32 i;

    if (from >= 0x110 || to >= 0x110) {
        return -1;
    }
    AT(rp, 0x24, s32) = door;
    if (from == to && (want == -1 || AT(rp, 0x24, s32) == -1 || AT(rp, 0x24, s32) == want)) {
        return 0;
    }
    AT(rp, 0x28, u8) = kind;
    AT(rp, 0x4, u32) = from;
    AT(rp, 0x8, u32) = to;
    AT(rp, 0xC, s32) = want;
    AT(rp, 0x10, s32) = side;
    AT(rp, 0x20, s16) = max;
    AT(rp, 0x14, u32 *) = avoid;
    AT(rp, 0x62C, void *) = NULL;
    AT(rp, 0x18, u16 *) = out;
    AT(rp, 0x1E, s16) = 0;
    AT(rp, 0x1C, s16) = 0;
    for (i = 0; i < 13; i++) {
        AT(rp, 0x630 + i * 4, u32) = 0;
    }
    RoutePlanner_QueueDoors(rp, from);
    return RoutePlanner_Search(rp);
}

/* +0x8 destructor */
/* 0x00220680 */
void *RoutePlanner_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = RoutePlanner_vtable;
        AT(o, 0x0, void **) = D_0046C530;
        gRoutePlanner = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* the base's destructor */
/* 0x00220CB0 */
void *RoutePlannerBase_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046C530;
        gRoutePlanner = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}
