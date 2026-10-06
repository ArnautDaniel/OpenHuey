/* Room 0x0A: its event handler class (vtable D_0046DD80, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046DD80[];
extern const char *D_003F43A0;

extern u8 D_003F3C90[];
extern u8 D_003F3CF0[];
extern u8 D_003F3DA0[];
extern u8 D_003F3E90[];
extern u8 D_003F3EF0[];
extern void *D_003F4350[];
extern void *D_003F4398[];
extern u8 D_003F43B0[];
extern PTMF D_01990830[];

void *func_002AAB00(void *o, s32 flags) { return room_dtor(o, flags, D_0046DD80, D_0046DB80); }

void *func_002AAB60(void) {
    return D_003F3C90;
}

void *func_002AAB70(void) {
    return D_003F3CF0;
}

void *func_002AAB80(void) {
    return D_003F3DA0;
}

void *func_002AAB90(void) {
    return D_003F3E90;
}

void *func_002AABA0(void) {
    return D_003F3EF0;
}

void *func_002AABB0(void *self, s32 i) {
    return D_003F4350[i];
}

void *func_002AABD0(void) {
    return D_003F43B0;
}

void *func_002AABE0(void *self, s32 i) {
    return D_003F4398[i];
}

/* (self->*D_01990830[i])(a, b) */
s32 func_002AAC00(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990830[i & 0xFF], a, b);
}

/* room 0x0A (D_003F4388): an effect on its object at -2.88 */
s32 func_002AAC30(void) {
    obj_effect(room_obj(D_003F43A0), 0xC0384E89);
    return 1;
}

/* the dial D_003F43A0 on progress var 3 */
s32 func_002AAD60(void *self, void *a1, u8 *cmd) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F43A0);

    if (o == NULL) {
        return 1;
    }
    return dial_step(cmd[3], o, 3, 0, 0);
}
