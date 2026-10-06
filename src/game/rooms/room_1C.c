/* Room 0x1C: its event handler class (vtable D_0046E180, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"
#include "snd_place.h"

extern void *D_0046DB80[];
extern void *D_0046E180[];
extern void *D_0046EC60[];

extern u32 D_003FD310[];
extern u8 D_003FD360[];
extern u32 D_0047AA78[];

extern u8 D_003FCB30[];
extern u8 D_003FCBC0[];
extern u8 D_003FCC40[];
extern u8 D_003FCD70[];
extern PTMF D_01990990[];

void *func_002AD9F0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E180, D_0046DB80); }

void *func_002ADA50(void) {
    return D_003FCB30;
}

void *func_002ADA60(void) {
    return D_003FCBC0;
}

void *func_002ADA70(void) {
    return D_003FCC40;
}

void *func_002ADA80(void) {
    return D_003FCD70;
}

u32 func_002ADA90(void *self, s32 i) {
    return D_003FD310[i];
}

void *func_002ADAB0(void) {
    return D_003FD360;
}

u32 func_002ADAC0(void *self, s32 i) {
    return D_0047AA78[i];
}

/* (self->*D_01990990[i])(a, b) */
s32 func_002ADAE0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990990[i & 0xFF], a, b);
}

/* room 0x1C (D_003FD350): a sound (0xC0000000, bank 6) at the room's effect 1 */
s32 func_002ADB10(void) {
    u8 *e = func_00266C40(gRoomEffects, 1);
    f32 at[4] __attribute__((aligned(16)));

    at[0] = AT(e, 0x20, f32);
    at[1] = AT(e, 0x24, f32);
    at[2] = AT(e, 0x28, f32);
    func_002FF650(gSound, 0xC0000000, 6, at, 0, 0);
    return 1;
}

/* room 0x1C (D_003FD338): room effect 0x1C (a depth range) with the cutscene from frame 0x14A:
 * near 1 .. 1 + 1.4 t (at most 67.6), far 48.6 + 4 t (at most 230) */
s32 func_002ADB70(void) {
    f32 t = (f32)(VCALL(gCutscene, 0x34, s32 (*)(VObject *))(gCutscene) - 0x14A);
    f32 r[4];
    f32 v;

    room_effect_slot_new(gRoomEffects, 0x1C, D_0046EC60);
    r[0] = 1.0f;
    v = 1.0f + 0x1.6666660000000p+0f /* 1.4 */ * t;
    r[1] = v <= 0x1.0e66660000000p+6f /* 67.6 */ ? v : 0x1.0e66660000000p+6f /* 67.6 */;
    r[2] = v <= 0x1.0e66660000000p+6f /* 67.6 */ ? v : 0x1.0e66660000000p+6f /* 67.6 */;
    v = 0x1.84cccc0000000p+5f /* 48.6 */ + 4.0f * t;
    r[3] = v <= 230.0f ? v : 230.0f;
    func_00266C70(gRoomEffects, 0x1C, r);
    return 1;
}
