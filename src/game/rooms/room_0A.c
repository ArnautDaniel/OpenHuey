/* Room 0x0A: its event handler class (vtable Room0A_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room0A_vtable[];
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

/* 0x002AAB00 */
void *Room0A_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room0A_vtable, RoomBase_vtable); }

/* 0x002AAB60 */
void *Room0A_EnterScript(void) {
    return D_003F3C90;
}

/* 0x002AAB70 */
void *Room0A_CharEnterScript(void) {
    return D_003F3CF0;
}

/* 0x002AAB80 */
void *Room0A_Phase1Script(void) {
    return D_003F3DA0;
}

/* 0x002AAB90 */
void *Room0A_Phase2Script(void) {
    return D_003F3E90;
}

/* 0x002AABA0 */
void *Room0A_Phase3Script(void) {
    return D_003F3EF0;
}

/* 0x002AABB0 */
void *Room0A_ActionScript(void *self, s32 i) {
    return D_003F4350[i];
}

/* 0x002AABD0 */
void *Room0A_Table38(void) {
    return D_003F43B0;
}

/* 0x002AABE0 */
void *Room0A_ObjectName(void *self, s32 i) {
    return D_003F4398[i];
}

/* (self->*D_01990830[i])(a, b) */
/* 0x002AAC00 */
s32 Room0A_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990830[i & 0xFF], a, b);
}

/* room 0x0A (D_003F4388): an effect on its object at -2.88 */
/* 0x002AAC30 */
s32 Room0A_Cmd01(void) {
    obj_effect(room_obj(D_003F43A0), 0xC0384E89);
    return 1;
}

/* the dial D_003F43A0 on progress var 3 */
/* 0x002AAD60 */
s32 Room0A_Cmd00(void *self, void *a1, u8 *cmd) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F43A0);

    if (o == NULL) {
        return 1;
    }
    return dial_step(cmd[3], o, 3, 0, 0);
}
