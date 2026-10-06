/* Room 0x10: its event handler class (vtable D_0046DF00, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046DF00[];
extern u8 D_0047A9F0[];

extern u8 D_003F6F80[];
extern u8 D_003F7060[];
extern u8 D_003F7150[];
extern u8 D_003F72D0[];
extern u8 D_003F73B0[];
extern u8 D_003F74E0[];
extern void *D_003F78D0[];
extern u8 D_003F7910[];
extern PTMF D_019908D0[];

void *func_002ABF00(void *o, s32 flags) { return room_dtor(o, flags, D_0046DF00, D_0046DB80); }

void *func_002ABF60(void) {
    return D_003F6F80;
}

void *func_002ABF70(void) {
    return D_003F7060;
}

void *func_002ABF80(void) {
    return D_003F7150;
}

void *func_002ABF90(void) {
    return D_003F72D0;
}

void *func_002ABFA0(void) {
    return D_003F73B0;
}

void *func_002ABFB0(void) {
    return D_003F74E0;
}

void *func_002ABFC0(void *self, s32 i) {
    return D_003F78D0[i];
}

void *func_002ABFE0(void) {
    return D_003F7910;
}

u32 func_002ABFF0(void *o, s32 i) { return ((u32 *)D_0047A9F0)[i]; }   /* D_0046DF00 +0x34 */

/* (self->*D_019908D0[i])(a, b) */
s32 func_002AC010(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019908D0[i & 0xFF], a, b);
}

/* room 0x10 (D_003F78F8): the stalker is there, not about, in mode 2, 6 or 7 */
s32 func_002AC040(void) {
    u8 *c = (u8 *)gCharSlot2;
    u8 k;

    if (c == NULL || AT(c, 0x28, u8) != 0) {
        return 0;
    }
    k = AT(c, 0x153C, u8);
    return k == 2 || k == 6 || k == 7;
}
