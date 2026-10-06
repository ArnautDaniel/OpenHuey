/* Room 0x52: its event handler class (vtable Room52_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room52_vtable[];
extern void *Drips_vtable[];
extern u8 Room52_EnterScript_data[];
extern u8 Room52_CharEnterScript_data[];
extern u8 Room52_Phase1Script_data[];
extern u8 Room52_Phase2Script_data[];
extern u32 Room52_ActionScripts[];
extern u8 Room52_Table38_data[];
extern u32 Room52_ObjectNames[];

extern PTMF Room52_CmdTable[];
extern PTMF Room52_CondTable[];

static inline void effect476bf0_init(void **o) {
    o[0] = Drips_vtable;
}

/* 0x002B4A70 */
void *Room52_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room52_vtable, RoomBase_vtable); }

/* 0x002B4AD0 */
void *Room52_EnterScript(void) {
    return Room52_EnterScript_data;
}

/* 0x002B4AE0 */
void *Room52_CharEnterScript(void) {
    return Room52_CharEnterScript_data;
}

/* 0x002B4AF0 */
void *Room52_Phase1Script(void) {
    return Room52_Phase1Script_data;
}

/* 0x002B4B00 */
void *Room52_Phase2Script(void) {
    return Room52_Phase2Script_data;
}

/* 0x002B4B10 */
u32 Room52_ActionScript(void *self, s32 i) {
    return Room52_ActionScripts[i];
}

/* 0x002B4B30 */
void *Room52_Table38(void) {
    return Room52_Table38_data;
}

/* (self->*Room52_CondTable[i])(a, b) */
/* 0x002B4B40 */
s32 Room52_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room52_CondTable[i & 0xFF], a, b);
}

/* 1 unless the object at +0x18 exists and its byte +0x28 is 1. */
/* 0x002B4B70 */
s32 Room52_Cond00(void) {
    u8 *p = *(u8 **)(gCreatures + 0x18);

    return !(p != NULL && p[0x28] == 1);
}

/* (self->*Room52_CmdTable[i])(a, b) */
/* 0x002B4BB0 */
s32 Room52_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room52_CmdTable[i & 0xFF], a, b);
}

/* 0x002B4BE0 */
s32 Room52_Cmd00(void) {   /* a scene effect (Drips_vtable, 0x6E0 bytes) */
    Effect_New(gEffects, 0x6E0, effect476bf0_init);
    return 1;
}

/* 0x002B4CB0 */
u32 Room52_ObjectName(void *self, s32 i) {
    return Room52_ObjectNames[i];
}
