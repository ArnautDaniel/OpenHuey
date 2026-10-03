/* The rooms' event handler classes (event system +0x120, one 4-byte object per room; vtable
 * +0xC.. +0x20 the scripts for each phase, +0x24 a character action script, +0x28 a script
 * callback, +0x30 a character's entering script). */
#include "common.h"
#include "game.h"
#include "ptmf.h"

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
