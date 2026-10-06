/* Room 0x2C: its event handler class (vtable D_00470EB0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00470EB0[];
extern u8 D_0047ACB8[], D_0047ACC0[];

extern u8 D_0041D140[];
extern u8 D_0041D1C0[];
extern u8 D_0047ACB4[];

/* 0x002FEEB0 */
void *Room2C_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00470EB0, D_0046DB80); }

/* 0x002FEF10 */
void *Room2C_EnterScript(void) {
    return D_0047ACB4;
}

/* 0x002FEF20 */
void *Room2C_CharEnterScript(void) {
    return D_0041D140;
}

/* 0x002FEF30 */
void *Room2C_Phase1Script(void) {
    return D_0041D1C0;
}

/* 0x002FEF40 */
u32 Room2C_ActionScript(void *o, s32 i) { return ((u32 *)D_0047ACB8)[i]; }   /* D_00470EB0 +0x24 */

/* 0x002FEF60 */
u32 Room2C_ObjectName(void *o, s32 i) { return ((u32 *)D_0047ACC0)[i]; }   /* D_00470EB0 +0x34 */
