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

void *func_002FEEB0(void *o, s32 flags) { return room_dtor(o, flags, D_00470EB0, D_0046DB80); }

void *func_002FEF10(void) {
    return D_0047ACB4;
}

void *func_002FEF20(void) {
    return D_0041D140;
}

void *func_002FEF30(void) {
    return D_0041D1C0;
}

u32 func_002FEF40(void *o, s32 i) { return ((u32 *)D_0047ACB8)[i]; }   /* D_00470EB0 +0x24 */

u32 func_002FEF60(void *o, s32 i) { return ((u32 *)D_0047ACC0)[i]; }   /* D_00470EB0 +0x34 */
