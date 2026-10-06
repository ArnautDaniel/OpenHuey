/* Room 0x43: its event handler class (vtable Room43_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"
#include "scene_game.h"

extern void *RoomBase_vtable[];
extern void *Room43_vtable[];
extern u8 Room43_Phase5Script_data[];
extern void *BigFire_vtable[];
extern u8 Room43_EnterScript_data[];
extern u8 Room43_CharEnterScript_data[];
extern u8 Room43_Phase1Script_data[];
extern u8 Room43_Phase2Script_data[];
extern u32 Room43_ActionScripts[];
extern u8 Room43_Table38_data[];

extern PTMF Room43_CmdTable[];

static void effect_6cf0_init(void **obj) {
    obj[0] = BigFire_vtable;
    obj[0x6040 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x6044 / 4] = -1;
    obj[0x6040 / 4] = QuadDrawer_vtable;
    obj[0x6078 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x607C / 4] = -1;
    obj[0x6078 / 4] = QuadDrawer_vtable;
    obj[0x60B0 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x60B4 / 4] = -1;
    obj[0x60B0 / 4] = QuadDrawer_vtable;
}

/* 0x002B25E0 */
void *Room43_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room43_vtable, RoomBase_vtable); }

/* 0x002B2640 */
void *Room43_EnterScript(void) {
    return Room43_EnterScript_data;
}

/* 0x002B2650 */
void *Room43_CharEnterScript(void) {
    return Room43_CharEnterScript_data;
}

/* 0x002B2660 */
void *Room43_Phase1Script(void) {
    return Room43_Phase1Script_data;
}

/* 0x002B2670 */
void *Room43_Phase2Script(void) {
    return Room43_Phase2Script_data;
}

/* 0x002B2680 */
void *Room43_Phase5Script(void *o) { return Room43_Phase5Script_data; }   /* Room43_vtable +0x20 */

/* 0x002B2690 */
u32 Room43_ActionScript(void *self, s32 i) {
    return Room43_ActionScripts[i];
}

/* 0x002B26B0 */
void *Room43_Table38(void) {
    return Room43_Table38_data;
}

/* (self->*Room43_CmdTable[i])(a, b) */
/* 0x002B26C0 */
s32 Room43_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room43_CmdTable[i & 0xFF], a, b);
}

/* the effect BigFire_vtable (three quad drawers) started with parameter 0 */
/* 0x002B26F0 */
s32 Room43_Cmd00(void) {
    u8 *mgr = gEffects;
    s32 slot = Effect_New(mgr, 0x6CF0, effect_6cf0_init);
    s32 arg = 0;

    EffectMgr_Start(mgr, slot, &arg);
    return 1;
}
