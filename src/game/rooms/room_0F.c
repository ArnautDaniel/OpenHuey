/* Room 0x0F: its event handler class (vtable Room0F_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"

extern void *RoomBase_vtable[];
extern void *Room0F_vtable[];
extern const char *pstr_fumi_yuka;
extern u8 Room0F_EnterScript_data[];
extern u8 Room0F_CharEnterScript_data[];
extern u8 Room0F_Phase1Script_data[];
extern u8 Room0F_Phase2Script_data[];
extern void *Room0F_ActionScripts[];
extern void *Room0F_ObjectNames[];
extern u8 Room0F_Table38_data[];
extern PTMF Room0F_CmdTable[];

/* an object's +0x14 back to 0 (the PS2 writes through junk when it isn't there) */
static inline void obj_unturn(const char *name) {
    u8 *o = room_obj(name);

#ifdef HG_NATIVE
    if (o == NULL) {
        return;
    }
#endif
    AT(o, 0x14, s32) = 0;
}

/* 0x002AB9F0 */
void *Room0F_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room0F_vtable, RoomBase_vtable); }

/* 0x002ABA50 */
void *Room0F_EnterScript(void) {
    return Room0F_EnterScript_data;
}

/* 0x002ABA60 */
void *Room0F_CharEnterScript(void) {
    return Room0F_CharEnterScript_data;
}

/* 0x002ABA70 */
void *Room0F_Phase1Script(void) {
    return Room0F_Phase1Script_data;
}

/* 0x002ABA80 */
void *Room0F_Phase2Script(void) {
    return Room0F_Phase2Script_data;
}

/* 0x002ABA90 */
void *Room0F_ActionScript(void *self, s32 i) {
    return Room0F_ActionScripts[i];
}

/* 0x002ABAB0 */
void *Room0F_Table38(void) {
    return Room0F_Table38_data;
}

/* 0x002ABAC0 */
void *Room0F_ObjectName(void *self, s32 i) {
    return Room0F_ObjectNames[i];
}

/* (self->*Room0F_CmdTable[i])(a, b) */
/* 0x002ABAE0 */
s32 Room0F_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room0F_CmdTable[i & 0xFF], a, b);
}

/* room 0x0F (Room0F_Cmd04_ptmf): the player's model +0xD0 (0, 1.5, -2 / -1.5 by byte 3) and +0xCC */
/* 0x002ABB10 */
s32 Room0F_Cmd04(void *self, void *a1, u8 *cmd) {
    VObject *m = gCharPlayer->motion;

    if (cmd[3] == 0) {
        VCALL(m, 0xD0, void (*)(VObject *, f32, f32, f32))(m, 0.0f, 1.5f, -2.0f);
        VCALL(m, 0xCC, void (*)(VObject *, s32))(m, 1);
    } else {
        VCALL(m, 0xD0, void (*)(VObject *, f32, f32, f32))(m, 0.0f, 1.5f, -1.5f);
        VCALL(m, 0xCC, void (*)(VObject *, s32))(m, 0);
    }
    return 1;
}

/* room 0x0F (Room0F_Cmd03_ptmf): the player's model +0xBC (1, 0.25) or (0, 0) by byte 3 */
/* 0x002ABBC0 */
s32 Room0F_Cmd03(void *self, void *a1, u8 *cmd) {
    VObject *m = gCharPlayer->motion;

    if (cmd[3] == 0) {
        VCALL(m, 0xBC, void (*)(VObject *, s32, f32))(m, 1, 0.25f);
    } else {
        VCALL(m, 0xBC, void (*)(VObject *, s32, f32))(m, 0, 0.0f);
    }
    return 1;
}

/* room 0x0F (Room0F_Cmd02_ptmf): an effect (DriftingFlecks_vtable, 0x840 bytes) with its box */
/* 0x002ABC20 */
s32 Room0F_Cmd02(void) {
    s32 slot = Effect_New(gEffects, 0x840, effect_471060_init);
    f32 prm[9];

    prm[1] = 140.0f;
    prm[2] = -9.5f;
    prm[0] = 0.0f;
    prm[3] = 90.0f;
    prm[6] = 0.0f;
    prm[4] = 18.0f;
    prm[8] = 0.0f;
    prm[5] = 110.0f;
    prm[7] = -90.0f;
    EffectMgr_Start(gEffects, slot, prm);
    return 1;
}

/* room 0x0F (Room0F_Cmd01_ptmf): three objects' +0x14 back to 0 */
/* 0x002ABD50 */
s32 Room0F_Cmd01(void) {
    obj_unturn(pstr_hook);
    obj_unturn(pstr_a_Puppet);
    obj_unturn(pstr_Puppetdoor);
    return 1;
}

/* 0x002ABDD0 */
s32 Room0F_Cmd00(void *self, void *a1, u8 *cmd) {
    return var_fade(pstr_fumi_yuka, 0, cmd);
}
