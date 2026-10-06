/* Room 0x14: its event handler class (vtable D_0046E000, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E000[];
extern const char *D_003FA078;

extern u8 D_003F99E0[];
extern u8 D_003F9A30[];
extern u8 D_003F9AC0[];
extern u8 D_003F9B60[];
extern void *D_003FA030[];
extern void *D_003FA070[];
extern u8 D_003FA080[];
extern PTMF D_01990910[];

void *func_002AC670(void *o, s32 flags) { return room_dtor(o, flags, D_0046E000, D_0046DB80); }

void *func_002AC6D0(void) {
    return D_003F99E0;
}

void *func_002AC6E0(void) {
    return D_003F9A30;
}

void *func_002AC6F0(void) {
    return D_003F9AC0;
}

void *func_002AC700(void) {
    return D_003F9B60;
}

void *func_002AC710(void *self, s32 i) {
    return D_003FA030[i];
}

void *func_002AC730(void) {
    return D_003FA080;
}

void *func_002AC740(void *self, s32 i) {
    return D_003FA070[i];
}

/* (self->*D_01990910[i])(a, b) */
s32 func_002AC760(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990910[i & 0xFF], a, b);
}

s32 func_002AC790(void *self, void *a1, u8 *cmd) {
    return var0_obj_anim(D_003FA078, cmd);
}
