/* Room 0x18: its event handler class (vtable Room18_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"

extern void *RoomBase_vtable[];
extern void *Room18_vtable[];
extern u8 D_0047AA40[];

extern u8 D_003FAA20[];
extern u8 D_003FAAA0[];
extern u8 D_003FAB30[];
extern u8 D_003FACB0[];
extern u8 D_003FAD40[];
extern void *D_003FB320[];
extern u8 D_003FB390[];
extern PTMF D_01990940[];

/* 0x002AD020 */
void *Room18_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room18_vtable, RoomBase_vtable); }

/* 0x002AD080 */
void *Room18_EnterScript(void) {
    return D_003FAA20;
}

/* 0x002AD090 */
void *Room18_CharEnterScript(void) {
    return D_003FAAA0;
}

/* 0x002AD0A0 */
void *Room18_Phase1Script(void) {
    return D_003FAB30;
}

/* 0x002AD0B0 */
void *Room18_Phase2Script(void) {
    return D_003FACB0;
}

/* 0x002AD0C0 */
void *Room18_Phase3Script(void) {
    return D_003FAD40;
}

/* 0x002AD0D0 */
void *Room18_Phase5Script(void *o) { return D_0047AA40; }   /* Room18_vtable +0x20 */

/* 0x002AD0E0 */
void *Room18_ActionScript(void *self, s32 i) {
    return D_003FB320[i];
}

/* 0x002AD100 */
void *Room18_Table38(void) {
    return D_003FB390;
}

/* 0x002AD110 */
void *Room18_ObjectName(void *self, s32 i) {
    return (void *)D_003FB370[i];
}

/* (self->*D_01990940[i])(a, b) */
/* 0x002AD130 */
s32 Room18_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990940[i & 0xFF], a, b);
}

/* room 0x18 (Room18_Cmd00_ptmf): object byte 3's PlacedObject_ToDef */
/* 0x002AD160 */
s32 Room18_Cmd00(void *self, void *a1, u8 *cmd) {
    u8 *o = room_obj(D_003FB370[cmd[3]]);

    if (o != NULL) {
        PlacedObject_ToDef(o);
    }
    return 1;
}
