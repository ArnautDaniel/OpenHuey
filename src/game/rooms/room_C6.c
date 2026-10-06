/* Room 0xC6: its event handler class (vtable RoomC6_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomC6_vtable[];
extern u8 RoomC6_Phase5Script_data[];

extern u32 RoomC6_EnterScript_data[];
extern u32 RoomC6_CharEnterScript_data[];
extern u32 RoomC6_Phase1Script_data[];
extern u32 RoomC6_Phase2Script_data[];
extern u32 RoomC6_ActionScripts[];
extern u32 RoomC6_Table38_data[];
extern u32 RoomC6_ObjectNames[];

/* 0x0034B4C0 */
void *RoomC6_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomC6_vtable, RoomBase_vtable); }

/* 0x0034B520 */
void *RoomC6_EnterScript(void) {
    return RoomC6_EnterScript_data;
}

/* 0x0034B530 */
void *RoomC6_CharEnterScript(void) {
    return RoomC6_CharEnterScript_data;
}

/* 0x0034B540 */
void *RoomC6_Phase1Script(void) {
    return RoomC6_Phase1Script_data;
}

/* 0x0034B550 */
void *RoomC6_Phase2Script(void) {
    return RoomC6_Phase2Script_data;
}

/* 0x0034B560 */
void *RoomC6_Phase5Script(void *o) { return RoomC6_Phase5Script_data; }   /* RoomC6_vtable +0x20 */

/* 0x0034B570 */
u32 RoomC6_ActionScript(void *self, s32 i) {
    return RoomC6_ActionScripts[i];
}

/* 0x0034B590 */
void *RoomC6_Table38(void) {
    return RoomC6_Table38_data;
}

/* 0x0034B5A0 */
u32 RoomC6_ObjectName(void *self, s32 i) {
    return RoomC6_ObjectNames[i];
}
