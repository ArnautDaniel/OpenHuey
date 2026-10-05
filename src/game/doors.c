/* The doors of the room (D_0044E558, vtable D_0046C540; set up in scene_game_members.c): up to 8
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

extern VObject *D_0044E558;   /* the doors */
extern VObject *D_0044E568;   /* the rooms: +0x10 (room, exit) the door id */
extern VObject *D_0044E570;   /* the nav mesh */
extern VObject *D_0044E4D0;   /* the events */
extern Character *gCharacters[];
extern void *D_0046C540[], *D_0046C5D0[], *D_0046C780[], *D_0046D800[], *D_00469D00[];
extern const f32 D_003E51A0[][8];   /* door kinds' areas: 4 (x, z) corners */
extern void func_00100490(void *p);   /* operator delete */
extern void func_001002C0(void *array, void *(*dtor)(void *, s32), u32 size, u32 n);   /* __destroy_arr */
extern void func_002E3130(f32 (*m)[4], const f32 *pos, f32 angle);   /* turned by angle about y, at pos */
extern void func_002E2DD0(f32 *out, f32 (*m)[4], const f32 *v);
extern f32 func_002E2D00(f32 angle);   /* wrapped into -pi..pi */
extern void func_002212D0(u8 *door);
extern void func_00220D10(u8 *door, s32 sound, s32 arg);   /* a door sound */
extern s32 Progress_CurRoomFlag(Progress *p, s32 room, u32 exit);
extern void func_00178C10(Progress *p, s32 room, s32 door, s32 arg);   /* door is open */
extern void func_00178A90(Progress *p, s32 room, s32 door, s32 arg);   /* door is shut */

#define DOOR(d, i) ((u8 *)(d) + ((i) & 0xFF) * 0x210)
#define PI_F 0x1.921fb6p+1f
#define TWO_PI_F 0x1.921fb6p+2f

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
void *func_00221920(u8 *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x190, void **) = D_0046D800;
        AT(e, 0x190, void **) = D_00469D00;
        AT(e, 0x80, void **) = D_0046C780;
        AT(e, 0x88, s32) = 0;
        AT(e, 0x80, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(e);
        }
    }
    return e;
}

/* +0x8 destructor */
void *func_00221890(u8 *d, s32 flags) {
    if (d != NULL) {
        AT(d, 0x0, void **) = D_0046C540;
        func_001002C0(d + 0x10, (void *(*)(void *, s32))func_00221920, 0x210, 8);
        AT(d, 0x0, void **) = D_0046C5D0;
        D_0044E558 = NULL;
        if ((s16)flags > 0) {
            func_00100490(d);
        }
    }
    return d;
}

/* the base's destructor */
void *func_00223D80(u8 *d, s32 flags) {
    if (d != NULL) {
        AT(d, 0x0, void **) = D_0046C5D0;
        D_0044E558 = NULL;
        if ((s16)flags > 0) {
            func_00100490(d);
        }
    }
    return d;
}

/* +0xC start door i's animation `anim` from buffer `buf` (opened by character `who`) */
s32 func_002237E0(VObject *d, u32 i, s32 anim, u32 who, s32 buf) {
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
s32 func_00223520(VObject *d, u32 i, s32 side, u32 who) {
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

/* +0x14 where door i's animation `anim` (buffer `buf`) puts its user: the record's point (x,
 * z) turned with the door, on the nav mesh from the door's triangle (failing that, without
 * its x), w 1, into `out`, its turn (the record's + the door's) into rot[1]; the triangle, or
 * -1 */
s32 func_002231A0(VObject *d, u32 i, s32 anim, f32 *out, f32 *rot, s32 buf) {
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
        func_002E3130(m, (f32 *)(e + 0x30), AT(e, 0x54, f32));
        func_002E2DD0(to, m, p);
        tri = AT(e, 0x18, u32);
        sceVu0CopyVector(from, (f32 *)(e + 0x30));
        if (nm == NULL) {
            nm = D_0044E570;
        }
        if (door_walk(nm, &tri, from, to)) {
            sceVu0CopyVector(out, to);
            out[3] = 1.0f;
            rot[1] = rec[2] + AT(e, 0x54, f32);
            rot[1] = func_002E2D00(rot[1]);
            rot[0] = 0.0f;
            rot[2] = 0.0f;
            return tri;
        }
    }
    return -1;
}

/* +0x28 door i, as it is opened from the near side (its angle past -40: open) - animating: on to
 * the next half (+0x78: 1 past -80), else whether it is shut; -1 for no door */
s32 func_00222E40(VObject *d, u32 i) {
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

/* +0x30 door i is idle (or there is none) */
s32 func_00221B80(VObject *d, u32 i) {
    if (!door_present(d, i)) {
        return 1;
    }
    return AT(DOOR(d, i), 0x70, s32) == 0;
}

/* +0x34 where door i stands (+0x30) */
s32 func_00221C00(VObject *d, u32 i, f32 *out) {
    if (!door_present(d, i)) {
        return -1;
    }
    sceVu0CopyVector(out, (f32 *)(DOOR(d, i) + 0x30));
    return 0;
}

/* +0x38 door i's point +0x20 */
s32 func_00221C90(VObject *d, u32 i, f32 *out) {
    if (!door_present(d, i)) {
        return -1;
    }
    sceVu0CopyVector(out, (f32 *)(DOOR(d, i) + 0x20));
    return 0;
}

/* +0x3C door i's turn at rest */
f32 func_00221D20(VObject *d, u32 i) {
    return door_present(d, i) ? AT(DOOR(d, i), 0x54, f32) : 0.0f;
}

/* +0x44 door i's nav triangle (-1: none) */
s32 func_00221DF0(VObject *d, u32 i) {
    return door_present(d, i) ? AT(DOOR(d, i), 0x18, s32) : -1;
}

/* +0x64 door i's opening angle */
f32 func_00221EB0(VObject *d, u32 i) {
    return door_present(d, i) ? AT(DOOR(d, i), 0x74, f32) : 0.0f;
}

/* +0x70 door i is opened */
s32 func_00221F30(VObject *d, u32 i) {
    return door_present(d, i) && AT(DOOR(d, i), 0x70, s32) == 3;
}

/* +0x88 */
void func_00221FB0(VObject *d, u32 i, u8 v) {
    AT(DOOR(d, i), 0x82, u8) = v;
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
void func_002219D0(VObject *d, u32 i, f32 a) {
    u8 *e = DOOR(d, i);

    AT(e, 0x44, f32) = AT(e, 0x54, f32) + a;
    door_turn_wrap(e);
}

/* +0x78 door i turned to `a` */
void func_00221A90(VObject *d, u32 i, f32 a) {
    u8 *e = DOOR(d, i);

    AT(e, 0x44, f32) = a;
    door_turn_wrap(e);
}

/* where `off` (in the door's frame, turned about to the far side when the events say so) from
 * door i lands on the nav mesh (`out`, on its plane): the triangle, or -1 */
s32 func_00222A60(VObject *d, u32 i, const f32 *off, f32 *out) {
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
    if (!(VCALL(D_0044E4D0, 0x20, u32 (*)(VObject *, u32, f32 *))(D_0044E4D0, i, side) & 0xFF)) {
        return -1;
    }
    if (VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, i, side) == 0) {
        a = func_002E2D00(PI_F + a);
    }
    func_002E3130(m, (f32 *)(e + 0x30), a);
    func_002E2DD0(to, m, off);
    tri = AT(e, 0x18, u32);
    sceVu0CopyVector(from, (f32 *)(e + 0x30));
    nm = D_0044E570;
    if (!door_walk(nm, &tri, from, to)) {
        return -1;
    }
    sceVu0CopyVector(out, to);
    VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, tri, out);
    return tri;
}

/* +0x58 / +0x54 / +0x50 the spot 12 behind / 12 ahead / 4 behind door i */
s32 func_00222CA0(VObject *d, u32 i, f32 *out) {
    f32 off[4] __attribute__((aligned(16)));

    off[2] = -12.0f;
    off[0] = 0.0f;
    off[1] = 0.0f;
    return func_00222A60(d, i, off, out);
}

s32 func_00222CD0(VObject *d, u32 i, f32 *out) {
    f32 off[4] __attribute__((aligned(16)));

    off[2] = 12.0f;
    off[0] = 0.0f;
    off[1] = 0.0f;
    return func_00222A60(d, i, off, out);
}

s32 func_00222D00(VObject *d, u32 i, f32 *out) {
    f32 off[4] __attribute__((aligned(16)));

    off[2] = -4.0f;
    off[0] = 0.0f;
    off[1] = 0.0f;
    return func_00222A60(d, i, off, out);
}

/* +0x5C each door of the room the rooms know (id under 400) has its state put into the
 * progress (+0x60) */
void func_00222960(VObject *d) {
    s32 room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    VObject *doors = D_0044E558, *rooms = D_0044E568;
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
void func_00222850(VObject *d, s32 room, u32 i) {
    Progress *p = gProgress;
    u8 *e;
    s32 who;

    if ((Progress_CurRoomFlag(p, room, i) & 0xFF) != 1) {
        return;
    }
    e = DOOR(d, i);
    who = AT(e, 0x70, s32) != 2 ? AT(e, 0x7C, u8) : 0xFF;
    if (AT(e, 0x78, s32) == 0) {
        func_00178C10(p, room, i, who);
    } else {
        func_00178A90(p, room, i, who);
    }
}

/* +0x68 door i (not animating) set opened: 0 from the near side (with a creak), 1 the far;
 * its passage flags 0x60000 moved to the other side; -1 otherwise */
s32 func_002226E0(VObject *d, u32 i, s32 how) {
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
        func_00220D10(e + 0x10, 0x91, 3);
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
s32 func_00222480(VObject *d, s32 kind, u32 i, const f32 *pos) {
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
    func_002E3130(m, (f32 *)(e + 0x30), AT(e, 0x54, f32));
    q = D_003E51A0[kind];
    v[1] = 0.0f;
    for (k = 0; k < 4; k++) {
        v[0] = q[k * 2];
        v[2] = q[k * 2 + 1];
        func_002E2DD0(c[k], m, v);
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
void func_002238F0(VObject *d) {
    u32 i;

    if (AT(d, 0x4, u8 *) == NULL) {
        return;
    }
    for (i = 0; i < 8; i = (i + 1) & 0xFF) {
        if (door_present(d, i)) {
            func_002212D0(DOOR(d, i) + 0x10);
        }
    }
    AT(d, 0x4, u8 *) = NULL;
}
