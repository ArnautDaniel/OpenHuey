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

/* 0x002AC670 */
void *Room14_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046E000, D_0046DB80); }

/* 0x002AC6D0 */
void *Room14_EnterScript(void) {
    return D_003F99E0;
}

/* 0x002AC6E0 */
void *Room14_CharEnterScript(void) {
    return D_003F9A30;
}

/* 0x002AC6F0 */
void *Room14_Phase1Script(void) {
    return D_003F9AC0;
}

/* 0x002AC700 */
void *Room14_Phase2Script(void) {
    return D_003F9B60;
}

/* 0x002AC710 */
void *Room14_ActionScript(void *self, s32 i) {
    return D_003FA030[i];
}

/* 0x002AC730 */
void *Room14_Table38(void) {
    return D_003FA080;
}

/* 0x002AC740 */
void *Room14_ObjectName(void *self, s32 i) {
    return D_003FA070[i];
}

/* (self->*D_01990910[i])(a, b) */
/* 0x002AC760 */
s32 Room14_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990910[i & 0xFF], a, b);
}

/* 0x002AC790 */
s32 Room14_Cmd00(void *self, void *a1, u8 *cmd) {
    return var0_obj_anim(D_003FA078, cmd);
}
