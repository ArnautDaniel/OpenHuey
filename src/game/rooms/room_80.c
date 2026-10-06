/* Room 0x80: its event handler class (vtable Room80_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "memcard.h"
#include "scene_game_members.h"

extern void *RoomBase_vtable[];
extern void *Room80_vtable[];
extern void *BackdropModel2_vtable[];
extern u32 Room80_EnterScript_data[];
extern u32 Room80_CharEnterScript_data[];
extern u32 Room80_Phase1Script_data[];
extern u32 Room80_Phase2Script_data[];
extern u32 Room80_ActionScripts[];
extern u32 Room80_ObjectNames[];

extern void *EffectBase_vtable[];

extern PTMF Room80_CmdTable[];

static void effect_7a430_init(void **obj) {
    obj[0] = BackdropModel2_vtable;
}

/* 0x0034A140 */
void *Room80_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room80_vtable, RoomBase_vtable); }

/* 0x0034A1A0 */
void *Room80_EnterScript(void) {
    return Room80_EnterScript_data;
}

/* 0x0034A1B0 */
void *Room80_CharEnterScript(void) {
    return Room80_CharEnterScript_data;
}

/* 0x0034A1C0 */
void *Room80_Phase1Script(void) {
    return Room80_Phase1Script_data;
}

/* 0x0034A1D0 */
void *Room80_Phase2Script(void) {
    return Room80_Phase2Script_data;
}

/* 0x0034A1E0 */
u32 Room80_ActionScript(void *self, s32 i) {
    return Room80_ActionScripts[i];
}

/* 0x0034A200 */
u32 Room80_ObjectName(void *self, s32 i) {
    return Room80_ObjectNames[i];
}

/* (self->*Room80_CmdTable[i])(a, b) */
/* 0x0034A220 */
s32 Room80_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room80_CmdTable[i & 0xFF], a, b);
}

/* (as Room2A_Cmd03) the 0x14-byte effect BackdropModel2_vtable started with byte 3 as a word */
/* 0x0034A250 */
s32 Room80_Cmd00(void *self, void *a1, u8 *cmd) {
    u8 *mgr = gEffects;
    s32 w = cmd[3];

    EffectMgr_Start(mgr, Effect_New(mgr, 0x14, effect_7a430_init), &w);
    return 1;
}

/* destructor (vtable BackdropModel2_vtable) */
/* 0x003784F0 */
void *BackdropModel2_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = BackdropModel2_vtable;
        AT(o, 0x0, void **) = EffectBase_vtable;
        if ((s16)flags > 0) {
            EffectMgr_free(o);
        }
    }
    return o;
}

/* (+0x18) set: +0x10 = the first word */
/* 0x00378550 */
void BackdropModel2_SetParams(u8 *o, s32 *prm) {
    if (prm != NULL) {
        AT(o, 0x10, s32) = prm[0];
    }
}
