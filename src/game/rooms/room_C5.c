/* Room 0xC5: its event handler class (vtable RoomC5_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "memcard.h"
#include "scene_game_members.h"
#include "snd_place.h"

extern void *RoomBase_vtable[];
extern void *RoomC5_vtable[];
extern u8 RoomC5_Phase5Script_data[], RoomC5_Table38_data[];
extern void *ObjectGlow_vtable[];
extern const char *RoomC5_ObjectNames[];   /* room objects 0..9 */
extern u32 RoomC5_EnterScript_data[];
extern u32 RoomC5_CharEnterScript_data[];
extern u32 RoomC5_Phase1Script_data[];
extern u32 RoomC5_Phase2Script_data[];
extern u32 RoomC5_ActionScripts[];

extern void *EffectBase_vtable[];

extern PTMF RoomC5_CmdTable[];

static void effect_79870_init(void **obj) {
    obj[0] = ObjectGlow_vtable;
}

/* 0x0034B0E0 */
void *RoomC5_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomC5_vtable, RoomBase_vtable); }

/* 0x0034B140 */
void *RoomC5_EnterScript(void) {
    return RoomC5_EnterScript_data;
}

/* 0x0034B150 */
void *RoomC5_CharEnterScript(void) {
    return RoomC5_CharEnterScript_data;
}

/* 0x0034B160 */
void *RoomC5_Phase1Script(void) {
    return RoomC5_Phase1Script_data;
}

/* 0x0034B170 */
void *RoomC5_Phase2Script(void) {
    return RoomC5_Phase2Script_data;
}

/* 0x0034B180 */
void *RoomC5_Phase5Script(void *o) { return RoomC5_Phase5Script_data; }   /* RoomC5_vtable +0x20 */

/* 0x0034B190 */
u32 RoomC5_ActionScript(void *self, s32 i) {
    return RoomC5_ActionScripts[i];
}

/* 0x0034B1B0 */
void *RoomC5_Table38(void *o) { return RoomC5_Table38_data; }   /* RoomC5_vtable +0x38 */

/* 0x0034B1C0 */
u32 RoomC5_ObjectName(void *self, s32 i) {
    return (u32)RoomC5_ObjectNames[i];
}

/* (self->*RoomC5_CmdTable[i])(a, b) */
/* 0x0034B1E0 */
s32 RoomC5_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomC5_CmdTable[i & 0xFF], a, b);
}

/* the 0x10-byte effect ObjectGlow_vtable on room object k + 1 (byte 4 = k, 1..8; script variable 11 - k
 * keeps its slot): made on first use when byte 3 is set, then sent (on byte 3, index 8 - k, the
 * variable, the object), with sound 1 at the object when on and the camera director's +0x38 is
 * clear */
/* 0x0034B210 */
s32 RoomC5_Cmd00(void *self, void *a1, u8 *cmd) {
    u32 k = cmd[4];
    u8 var = 11 - k, idx = 8 - k;   /* (k 0 / past 8: unset on the PS2) */
    u32 name = k + 1;
    u8 on = cmd[3];
    VObject *ev = gEvents;
    s32 slot = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, var);
    u8 *o;
    struct {
        u8 on, idx, var, pad;
        u8 *obj;
    } msg;

    if (slot == -1) {
        if (on == 0) {
            return 1;
        }
        slot = Effect_New(gEffects, 0x10, effect_79870_init);
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, var, slot);
    }
    o = obj_named(RoomC5_ObjectNames[name]);
    msg.idx = idx;
    msg.var = var;
    msg.on = on;
    msg.pad = 0;
    msg.obj = o;
    EffectMgr_Start(gEffects, slot, &msg);
    if (VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) == 0 && on != 0) {
        Sound_PlayBankAt(gSound, 1, 6, (f32 *)(o + 0x20), 0, 0);
    }
    return 1;
}

/* destructor (vtable ObjectGlow_vtable) */
/* 0x0035BBD0 */
void *ObjectGlow_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = ObjectGlow_vtable;
        AT(o, 0x0, void **) = EffectBase_vtable;
        if ((s16)flags > 0) {
            EffectMgr_free(o);
        }
    }
    return o;
}

/* 0x0035C9E0 */
void ObjectGlow_Start(u8 *o) {
    AT(o, 0x5, u8) = 0xFF;
}
