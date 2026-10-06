/* Room 0x1F: its event handler class (vtable Room1F_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "snd_place.h"
#include "msl.h"

extern void *RoomBase_vtable[];
extern void *Room1F_vtable[];
extern const char *D_0047AAB8[];

extern u8 D_003FE300[];
extern u8 D_003FE350[];
extern u8 D_003FE3D0[];
extern u8 D_003FE420[];
extern u32 D_0047AAB0[];
extern u8 D_003FE450[];

extern PTMF D_019909D8[];

/* 0x002AE090 */
void *Room1F_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room1F_vtable, RoomBase_vtable); }

/* 0x002AE0F0 */
void *Room1F_EnterScript(void) {
    return D_003FE300;
}

/* 0x002AE100 */
void *Room1F_CharEnterScript(void) {
    return D_003FE350;
}

/* 0x002AE110 */
void *Room1F_Phase1Script(void) {
    return D_003FE3D0;
}

/* 0x002AE120 */
void *Room1F_Phase2Script(void) {
    return D_003FE420;
}

/* 0x002AE130 */
u32 Room1F_ActionScript(void *self, s32 i) {
    return D_0047AAB0[i];
}

/* 0x002AE150 */
void *Room1F_Table38(void) {
    return D_003FE450;
}

/* 0x002AE160 */
u32 Room1F_ObjectName(void *self, s32 i) {
    return (u32)D_0047AAB8[i];
}

/* (self->*D_019909D8[i])(a, b) */
/* 0x002AE180 */
s32 Room1F_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019909D8[i & 0xFF], a, b);
}

/* two wheels (D_0047AAB8) rocking 4 degrees (+0x18) through their phase +0x30, 6 degrees a step
 * (byte 3 1; 0 reset), the first one's creak (-366, 30, -25) at each turn */
/* 0x002AE1B0 */
s32 Room1F_Cmd00(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    VObject *objs = D_00456DF8, *snd = gSound;
    f32 at[4] __attribute__((aligned(16)));
    s32 i;

    at[0] = -366.0f;
    at[1] = 30.0f;
    at[2] = -25.0f;
    for (i = 0; i < 2; i++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_0047AAB8[i]);

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
