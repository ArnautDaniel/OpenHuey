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
