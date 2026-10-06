/* Room 0x1F: its event handler class (vtable Room1F_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "sound.h"
#include "msl.h"

extern void *RoomBase_vtable[];
extern void *Room1F_vtable[];
extern const char *Room1F_ObjectNames[];

extern u8 Room1F_EnterScript_data[];
extern u8 Room1F_CharEnterScript_data[];
extern u8 Room1F_Phase1Script_data[];
extern u8 Room1F_Phase2Script_data[];
extern u32 Room1F_ActionScripts[];
extern u8 Room1F_Table38_data[];

extern PTMF Room1F_CmdTable[];

/* 0x002AE090 */
void *Room1F_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room1F_vtable, RoomBase_vtable); }

/* 0x002AE0F0 */
void *Room1F_EnterScript(void) {
    return Room1F_EnterScript_data;
}

/* 0x002AE100 */
void *Room1F_CharEnterScript(void) {
    return Room1F_CharEnterScript_data;
}

/* 0x002AE110 */
void *Room1F_Phase1Script(void) {
    return Room1F_Phase1Script_data;
}

/* 0x002AE120 */
void *Room1F_Phase2Script(void) {
    return Room1F_Phase2Script_data;
}

/* 0x002AE130 */
u32 Room1F_ActionScript(void *self, s32 i) {
    return Room1F_ActionScripts[i];
}

/* 0x002AE150 */
void *Room1F_Table38(void) {
    return Room1F_Table38_data;
}

/* 0x002AE160 */
u32 Room1F_ObjectName(void *self, s32 i) {
    return (u32)Room1F_ObjectNames[i];
}

/* (self->*Room1F_CmdTable[i])(a, b) */
/* 0x002AE180 */
s32 Room1F_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room1F_CmdTable[i & 0xFF], a, b);
}

/* two wheels (Room1F_ObjectNames) rocking 4 degrees (+0x18) through their phase +0x30, 6 degrees a step
 * (byte 3 1; 0 reset), the first one's creak (-366, 30, -25) at each turn */
/* 0x002AE1B0 */
s32 Room1F_Cmd00(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    VObject *objs = gRoomObjects, *snd = gSound;
    f32 at[4] __attribute__((aligned(16)));
    s32 i;

    at[0] = -366.0f;
    at[1] = 30.0f;
    at[2] = -25.0f;
    for (i = 0; i < 2; i++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, Room1F_ObjectNames[i]);

        if (o == NULL) {
            continue;
        }
        switch (cmd[3]) {
        case 0:
            AT(o, 0x30, f32) = 0.0f;
            AT(o, 0x18, f32) = 0.0f;
            if (i == 0) {
                Sound_PlayBankAt(snd, 0, 6, at, 0, 0);
            }
            break;
        case 1:
            AT(o, 0x30, f32) = AT(o, 0x30, f32) + 6.0f;
            if (!(AT(o, 0x30, f32) < 360.0f)) {
                if (i == 0) {
                    Sound_PlayBankAt(snd, 0, 6, at, 0, 0);
                }
                AT(o, 0x30, f32) = AT(o, 0x30, f32) - 360.0f;
            }
            AT(o, 0x18, f32) = kPi.f * (4.0f * func_0031C248(kPi.f * AT(o, 0x30, f32) / 180.0f)) / 180.0f;
            break;
        }
    }
    return 1;
}
