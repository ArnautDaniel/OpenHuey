/* The rooms' event handler classes (event system +0x120, one 4-byte object per room; vtable
 * +0xC.. +0x20 the scripts for each phase, +0x24 a character action script, +0x28 a script
 * callback, +0x30 a character's entering script). */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "progress.h"
#include "sce/libvu0.h"

extern PTMF D_01990BD0[];   /* the room callbacks (set up by the static initialisers) */

/* +0x28 callback `n` (from the event command 0x22) for character c, with the command's
 * string `cmd`: bit 1 wait, bit 0 go on */
s32 func_002B11A0(VObject *room, u8 n, u8 *c, const u8 *cmd) {
    return ptmf_scall_r2(room, &D_01990BD0[n], (s32)c, (s32)cmd);
}

#include "effectmgr.h"

extern void *D_00474000[];
extern void *D_00469D00[], *D_0046FC30[];

static void prop_init(void **obj) {
    obj[0] = D_00474000;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

/* room 0x2A callback: spawn the D_00474000 object at (220, 0, -100) */
s32 func_002B12D0(VObject *room) {
    u8 *mgr = D_0044E578;
    f32 pos[4] __attribute__((aligned(16)));
    s32 slot = Effect_New(mgr, 0x900, prop_init);

    pos[0] = 220.0f;
    pos[2] = -100.0f;
    pos[1] = 0.0f;
    pos[3] = 1.0f;
    func_002D6090(mgr, slot, pos);
    return 1;
}

extern void *D_00478BC0[];

static void obj_478BC0_init(void **obj) {
    obj[0] = D_00478BC0;
}

/* room 0x2A callback: spawn a D_00478BC0 object with the command's string */
s32 func_002B11D0(VObject *room, u8 *c, const u8 *cmd) {
    u8 *mgr = D_0044E578;
    s32 slot = Effect_New(mgr, 0x14, obj_478BC0_init);

    func_002D6090(mgr, slot, (void *)(cmd + 3));
    return 1;
}

extern VObject *D_00456DF8;          /* the room's named props */
extern const char *D_004070C0[3];    /* "sara_l", "sara_r", "tenbin" */

static u8 *room_prop(const char *name) {
    return VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);
}

static void prop_place(u8 *o, f32 rz, f32 x, f32 y, f32 z) {
    AT(o, 0x10, f32) = 0.0f;
    AT(o, 0x14, f32) = 0.0f;
    AT(o, 0x18, f32) = rz;
    AT(o, 0x20, f32) = x;
    AT(o, 0x24, f32) = y;
    AT(o, 0x28, f32) = z;
}

/* room 0x40 callback: the balance scale, level (cmd[3] 0) or tipped 25 degrees (else): its
 * beam ("tenbin") and the two pans hanging from its ends */
s32 func_002B2450(VObject *room, u8 *c, const u8 *cmd) {
    u8 *o;

    if ((o = room_prop(D_004070C0[0])) != NULL) {
        if (cmd[3] == 0) {
            prop_place(o, 0.0f, 0x1.8cd35a0000000p+2f /* 6.2004 */, 0x1.3ccccc0000000p+4f /* 19.8 */, 0x1.11999a0000000p+4f /* 17.1 */);
        } else {
            prop_place(o, 0.0f, 0x1.966cf40000000p+2f /* 6.3504 */, 0x1.49999a0000000p+4f /* 20.6 */, 0x1.11999a0000000p+4f /* 17.1 */);
        }
    }
    if ((o = room_prop(D_004070C0[1])) != NULL) {
        if (cmd[3] == 0) {
            prop_place(o, 0.0f, 0x1.8645a20000000p+1f /* 3.049 */, 0x1.3ccccc0000000p+4f /* 19.8 */, 0x1.11999a0000000p+4f /* 17.1 */);
        } else {
            prop_place(o, 0.0f, 0x1.cc08320000000p+1f /* 3.594 */, 19.25f, 0x1.11999a0000000p+4f /* 17.1 */);
        }
    }
    if ((o = room_prop(D_004070C0[2])) != NULL) {
        prop_place(o, cmd[3] == 0 ? 0.0f : 0x1.becde60000000p-2f /* 0.43633232 */, 0x1.27a29c0000000p+2f /* 4.6193 */, 0x1.49999a0000000p+4f /* 20.6 */, 0x1.114fe00000000p+4f /* 17.082 */);
    }
    return 1;
}

extern void *gCharacters[6];
extern s32 func_001770D0(Progress *p, s32 id);   /* character id -> gCharacters index */
extern void func_0032D3E0(void *c, s32 how, f32 x, f32 z);
extern s32 func_0032D2C0(void *c);

/* room 0x2B callback: character 0x1A (cmd[3] 0) sent off towards (-290, 42), or (else) asked
 * whether it has arrived - 1 go on, 2 wait */
s32 func_002B1700(VObject *room, u8 *c, const u8 *cmd) {
    u8 i = func_001770D0(gProgress, 0x1A);
    void *ch = gCharacters[i];

#ifdef HG_NATIVE
    if (i >= 6 || ch == NULL) {   /* (event characters aren't loaded on PC yet) */
        return 1;
    }
#endif
    if (cmd[3] == 0) {
        func_0032D3E0(ch, 2, -290.0f, 42.0f);
        return 1;
    }
    return func_0032D2C0(ch) == 0 ? 2 : 1;
}

extern VObject *D_0044E988;   /* the item manager (+0xC: an item is held) */
extern VObject *D_0044E560;   /* the sound driver */

/* room 0x50 callback: unless progress flag 0xAF, with flag 0x649 and item 0x238 held, sound
 * 0xC (+0x14, 5) */
s32 func_002B47C0(VObject *room) {
    Progress *p = gProgress;

    if (!(AT(p, 0x30, u32) & 0x8000) && (AT(p, 0xE4, u32) & 0x20000) &&
        VCALL(D_0044E988, 0xC, s32 (*)(VObject *, s32))(D_0044E988, 0x238)) {
        VCALL(D_0044E560, 0x14, void (*)(VObject *, s32, s32))(D_0044E560, 0xC, 5);
    }
    return 1;
}

extern const char *D_003FF110[];   /* names of room 0x20's swinging props */
extern f32 func_0031C248(f32 x);   /* sinf */
extern void func_002FF650(VObject *snd, s32 id, s32 arg2, const f32 *pos, s32 arg4, s32 arg5);

/* room 0x20 callback: the pendulum named cmd[3] - cmd[4] 0 stopped (phase +0x30 0), 1 swung on
 * a step (phase +2 degrees; a tick at (-85, 30, 90) each turn): turned (+0x14) by 15 degrees
 * x sin(phase) */
s32 func_002AED60(VObject *room, u8 *c, const u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB};
    u8 *o = room_prop(D_003FF110[cmd[3]]);
    f32 a;

    switch (cmd[4]) {
    case 0:
        AT(o, 0x30, f32) = 0.0f;
        return 1;
    case 1:
        AT(o, 0x30, f32) = AT(o, 0x30, f32) + 2.0f;
        if (!(AT(o, 0x30, f32) < 360.0f)) {
            f32 pos[4] __attribute__((aligned(16)));

            AT(o, 0x30, f32) = AT(o, 0x30, f32) - 360.0f;
            pos[0] = -85.0f;
            pos[1] = 30.0f;
            pos[2] = 90.0f;
            func_002FF650(D_0044E560, 0x40000001, 6, pos, 0, 0);
        }
        a = kPi.f * (15.0f * func_0031C248(kPi.f * AT(o, 0x30, f32) / 180.0f)) / 180.0f;
        AT(o, 0x14, f32) = a;
        if (!(a <= kPi.f)) {
            AT(o, 0x14, f32) = a - kTwoPi.f;
        }
        return 1;
    }
    return 1;
}

extern VObject *D_0044E4D0;   /* the event system (+0x30 / +0x34 its object slots) */
extern u8 *D_0044E4C0;        /* the room effects */
extern void *D_00472F60[], *D_004795A0[];
extern void *func_002672F0(u32 size, void *place);   /* placement new */
extern s32 func_00266C70(u8 *fx, s32 n, void *arg);
extern void func_002670F0(void *fx, s32 k);           /* remove room effect k */
extern void func_002D6170(u8 *mgr, s32 slot);         /* end a spawned effect */

static void obj_4795A0_init(void **obj) {
    obj[0] = D_004795A0;
}

/* room 0x60 callback: cmd[3] 0 a new D_00472F60 room effect (+0x14A4) and a D_004795A0 object
 * (kept as the event's object 1), then (also for cmd[3] 2..) room effect 0x1B: a 20 x 20 floor
 * quad at y -0.2 whose strength follows the event's value 0 (0, 30, 60, 90, 128), handed to
 * that object too when not 0; cmd[3] 1 both removed */
s32 func_003106E0(VObject *room, u8 *c, const u8 *cmd) {
    struct {
        f32 q[4][4];
        s32 a;
        s32 level_f;
        f32 one;
        s32 level;
    } arg __attribute__((aligned(16)));
    VObject *ev;
    s32 v;

    if (cmd[3] == 1) {
        func_002670F0(D_0044E4C0, 0x1B);
        func_002D6170(D_0044E578,
                      VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 1));
        return 1;
    }
    if (cmd[3] == 0) {
        u8 *fx = D_0044E4C0;
        void **slot = (void **)(fx + 0x14A4);
        void *mem;
        s32 obj;

        if (*slot != NULL) {
            VCALL(fx + 0x1400, 0x14, void (*)(void *, void *))(fx + 0x1400, *slot);
            *slot = NULL;
        }
        mem = VCALL(fx + 0x1400, 0x10, void *(*)(void *, s32))(fx + 0x1400, 0xA0);
        if (mem != NULL) {
            void **e = func_002672F0(0xA0, mem);

            if (e != NULL) {
                e[0] = D_00472F60;
            }
            *slot = e;
            VCALL(*slot, 0xC, void (*)(void *))(*slot);
        }
        obj = Effect_New(D_0044E578, 0x10, obj_4795A0_init);
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 1, obj);
    }
    arg.q[0][0] = 10.0f;  arg.q[0][1] = -0x1.99999a0000000p-3f /* 0.2 */; arg.q[0][2] = -10.0f; arg.q[0][3] = 1.0f;
    arg.q[1][0] = -10.0f; arg.q[1][1] = -0x1.99999a0000000p-3f /* 0.2 */; arg.q[1][2] = -10.0f; arg.q[1][3] = 1.0f;
    arg.q[2][0] = 10.0f;  arg.q[2][1] = -0x1.99999a0000000p-3f /* 0.2 */; arg.q[2][2] = 10.0f;  arg.q[2][3] = 1.0f;
    arg.q[3][0] = -10.0f; arg.q[3][1] = -0x1.99999a0000000p-3f /* 0.2 */; arg.q[3][2] = 10.0f;  arg.q[3][3] = 1.0f;
    arg.a = 0;
    arg.level = 0;
    ev = D_0044E4D0;
    switch (VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0)) {
    case 0:
        arg.level = 0;
        break;
    case 1:
        arg.level = 30;
        break;
    case 2:
        arg.level = 60;
        break;
    case 3:
        arg.level = 90;
        break;
    case 4:
        arg.level = 0x80;
        break;
    }
    arg.one = 1.0f;
    arg.level_f = arg.level;   /* (the word copied as is) */
    func_00266C70(D_0044E4C0, 0x1B, &arg);
    if (arg.level != 0) {
        v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 1);
        func_002D6090(D_0044E578, v, &arg.level);
    }
    return 1;
}

extern const char *D_003F99B8[];   /* room 0x13's prop */

/* room 0x13 callback: its prop turned a quarter (-90 degrees about y) for cmd[3], else back */
s32 func_002AC600(VObject *room, u8 *c, const u8 *cmd) {
    u8 *o = room_prop(D_003F99B8[0]);

    if (o != NULL) {
        AT(o, 0x14, f32) = cmd[3] != 0 ? -0x1.921fb60000000p+0f /* 1.5707964 */ : 0.0f;
    }
    return 1;
}

extern void *D_0046EA40[];

/* room effect +0x143C made anew (a D_0046EA40) */
static void room_fx_143C_new(void) {
    u8 *fx = D_0044E4C0;
    void **slot = (void **)(fx + 0x143C);
    void *mem;

    if (*slot != NULL) {
        VCALL(fx + 0x1400, 0x14, void (*)(void *, void *))(fx + 0x1400, *slot);
        *slot = NULL;
    }
    mem = VCALL(fx + 0x1400, 0x10, void *(*)(void *, s32))(fx + 0x1400, 0xA0);
    if (mem != NULL) {
        void **e = func_002672F0(0xA0, mem);

        if (e != NULL) {
            e[0] = D_0046EA40;
        }
        *slot = e;
        VCALL(*slot, 0xC, void (*)(void *))(*slot);
    }
}

/* room 0x21 callback: room_fx_143C_new */
s32 func_002AF730(VObject *room) {
    room_fx_143C_new();
    return 1;
}

/* room 0x31 callback: the same */
s32 func_0031E510(VObject *room) {
    room_fx_143C_new();
    return 1;
}

/* room 0x32 callback: the same */
s32 func_00321590(VObject *room) {
    room_fx_143C_new();
    return 1;
}

extern void *D_0046F5A0[], *D_00476BF0[];

static void obj_46F5A0_init(void **obj) {
    obj[0] = D_0046F5A0;
    obj[0x1810 / 4] = D_00469D00;
    ((s32 *)obj)[0x1814 / 4] = -1;
    obj[0x1810 / 4] = D_0046FC30;
}

static void obj_476BF0_init(void **obj) {
    obj[0] = D_00476BF0;
}

/* room 0x24 callback: a D_0046F5A0 object (0x1C60 bytes, its quad drawer at +0x1810) */
s32 func_002AFE90(VObject *room) {
    Effect_New(D_0044E578, 0x1C60, obj_46F5A0_init);
    return 1;
}

/* room 0x52 callback: a D_00476BF0 object (0x6E0 bytes) */
s32 func_002B4BE0(VObject *room) {
    Effect_New(D_0044E578, 0x6E0, obj_476BF0_init);
    return 1;
}

/* room 0x51 callback: unless progress flag 0xAF, with flag 0x648 and item 0x238 held, sound
 * 0xC (+0x14, 5) */
s32 func_002B4970(VObject *room) {
    Progress *p = gProgress;

    if (!(AT(p, 0x30, u32) & 0x8000) && (AT(p, 0xE4, u32) & 0x10000) &&
        VCALL(D_0044E988, 0xC, s32 (*)(VObject *, s32))(D_0044E988, 0x238)) {
        VCALL(D_0044E560, 0x14, void (*)(VObject *, s32, s32))(D_0044E560, 0xC, 5);
    }
    return 1;
}

extern const char *D_003F0404[];   /* room 0x02's lift */

/* room 0x02 callback: the lift (its height +0x24) - cmd[3] 0 set at the top (37.978, with
 * progress flag 0x12) or bottom (11), 1 raised a step (0.25; at the top the event's +0x5C 2),
 * 2 raised a step, others lowered one */
s32 func_002A94C0(VObject *room, u8 *c, const u8 *cmd) {
    static const union { u32 u; f32 f; } kTop = {0x4217E979};
    u8 *o = room_prop(D_003F0404[0]);

    if (o == NULL) {
        return 1;
    }
    switch (cmd[3]) {
    case 0:
        AT(o, 0x24, f32) = (AT(gProgress, 0x1C, u32) & 0x40000) ? kTop.f : 11.0f;
        break;
    case 1:
        AT(o, 0x24, f32) = AT(o, 0x24, f32) + 0.25f;
        if (!(AT(o, 0x24, f32) < kTop.f)) {
            VCALL(D_0044E4D0, 0x5C, void (*)(VObject *, s32))(D_0044E4D0, 2);
            AT(o, 0x24, f32) = kTop.f;
        }
        break;
    case 2:
        AT(o, 0x24, f32) = AT(o, 0x24, f32) + 0.25f;
        break;
    default:
        AT(o, 0x24, f32) = AT(o, 0x24, f32) - 0.25f;
        break;
    }
    return 1;
}

extern f32 D_0047B280;   /* room 0x62's falling object's speed */
extern u8 *func_00266C40(void *fx, s32 k);   /* room effect slot k */

/* room 0x62 callback: room effect 0 dropped - cmd[3] 0 held (speed 0), 1 moved on 0.5 in z,
 * others falling (+0x74 0): forward 0.16 a step, speed down by 0.5, bouncing at height 0.7
 * (speed x -0.3, a sound) until slower than 0.2 (+0x74 1, go on: 1; else wait: 2) */
s32 func_00308C20(VObject *room, u8 *c, const u8 *cmd) {
    static const union { u32 u; f32 f; } kStep = {0x3E23D70A}, kBounce = {0xBE99999A}, kRest = {0x3E4CCCCD},
        kFloor = {0x3F333333};
    u8 *e = func_00266C40(D_0044E4C0, 0);
    f32 pos[4] __attribute__((aligned(16)));

    switch (cmd[3]) {
    case 0:
        D_0047B280 = 0.0f;
        return 1;
    case 1:
        AT(e, 0x28, f32) = AT(e, 0x28, f32) + 0.5f;
        return 1;
    }
    AT(e, 0x74, s32) = 0;
    AT(e, 0x28, f32) = AT(e, 0x28, f32) + kStep.f;
    D_0047B280 = D_0047B280 - 0.5f;
    AT(e, 0x24, f32) = AT(e, 0x24, f32) + D_0047B280;
    if (!(AT(e, 0x24, f32) < kFloor.f)) {
        return 2;
    }
    AT(e, 0x24, f32) = kFloor.f;
    D_0047B280 = D_0047B280 * kBounce.f;
    sceVu0CopyVector(pos, (f32 *)(e + 0x20));
    func_002FF650(D_0044E560, 0, 6, pos, 0, 0);
    if (!(D_0047B280 < kRest.f)) {
        return 2;
    }
    AT(e, 0x74, s32) = 1;
    return 1;
}

extern void *D_0047A370[];

static void obj_47A370_init(void **obj) {
    obj[0] = D_0047A370;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

/* room 0x61 callback: cmd[3] 0 a D_0047A370 object (0x700 bytes, its quad drawer at +0x610)
 * started at (30, 0, 70) and kept as the event's object 3; 1 / 2 that object told 1 / 2 */
s32 func_003114C0(VObject *room, u8 *c, const u8 *cmd) {
    struct {
        f32 pos[4];
        s32 how;
    } arg __attribute__((aligned(16)));
    s32 slot;

    switch (cmd[3]) {
    case 0:
        slot = Effect_New(D_0044E578, 0x700, obj_47A370_init);
        arg.pos[0] = 30.0f;
        arg.pos[2] = 70.0f;
        arg.pos[1] = 0.0f;
        arg.pos[3] = 1.0f;
        arg.how = 0;
        func_002D6090(D_0044E578, slot, &arg);
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 3, slot);
        break;
    case 1:
    case 2:
        arg.how = cmd[3];
        func_002D6090(D_0044E578, VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 3), &arg);
        break;
    }
    return 1;
}
