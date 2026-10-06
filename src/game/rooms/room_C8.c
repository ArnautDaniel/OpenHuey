/* Room 0xC8: its event handler class (vtable RoomC8_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "memcard.h"
#include "scene_game_members.h"

extern void *RoomBase_vtable[];
extern void *RoomC8_vtable[];
extern u8 D_0047AF40[];
extern void *TurningModel_vtable[];
extern u32 D_00441520[];
extern u32 D_00441560[];
extern u32 D_00441620[];
extern u32 D_004416A0[];
extern u32 D_004417A8[];
extern u32 D_0047AF3C[];

extern void *EffectBase_vtable[];

extern PTMF D_01991A08[];

static void effect_47a410_init(void **obj) {
    obj[0] = TurningModel_vtable;
}

/* 0x0034B5C0 */
void *RoomC8_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomC8_vtable, RoomBase_vtable); }

/* 0x0034B620 */
void *RoomC8_EnterScript(void) {
    return D_00441520;
}

/* 0x0034B630 */
void *RoomC8_CharEnterScript(void) {
    return D_00441560;
}

/* 0x0034B640 */
void *RoomC8_Phase1Script(void) {
    return D_00441620;
}

/* 0x0034B650 */
void *RoomC8_Phase2Script(void) {
    return D_004416A0;
}

/* 0x0034B660 */
u32 RoomC8_ActionScript(void *self, s32 i) {
    return D_004417A8[i];
}

/* 0x0034B680 */
void *RoomC8_Table38(void *o) { return D_0047AF40; }   /* RoomC8_vtable +0x38 */

/* 0x0034B690 */
u32 RoomC8_ObjectName(void *self, s32 i) {
    return D_0047AF3C[i];
}

/* (self->*D_01991A08[i])(a, b) */
/* 0x0034B6B0 */
s32 RoomC8_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A08[i & 0xFF], a, b);
}

/* an effect TurningModel_vtable (8 bytes), not started */
/* 0x0034B6E0 */
s32 RoomC8_Cmd00(void) {
    Effect_New(gEffects, 0x8, effect_47a410_init);
    return 1;
}

/* destructor (vtable TurningModel_vtable) */
/* 0x00378310 */
void *TurningModel_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = TurningModel_vtable;
        AT(o, 0x0, void **) = EffectBase_vtable;
        if ((s16)flags > 0) {
            EffectMgr_free(o);
        }
    }
    return o;
}
