/* Room 0x109: its event handler class (vtable D_0046FE40, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046FE40[];
extern s32 func_001770D0(Progress *p, s32 kind);   /* the slot of character kind (0xFF) */
extern s32 D_0047B278;
extern s32 func_0032D150(Character *c);
extern void func_0032D270(Character *c, s32 a, f32 x, f32 y);

extern u8 D_004193D0[];
extern u8 D_00419400[];
extern u8 D_00419530[];
extern u8 D_00419638[];
extern void *D_004196E0[];
extern u8 D_00419710[];

extern PTMF D_01990F30[];

void *func_002E7460(void *o, s32 flags) { return room_dtor(o, flags, D_0046FE40, D_0046DB80); }

void *func_002E74C0(void) { return D_004193D0; }

void *func_002E74D0(void) { return D_00419400; }

void *func_002E74E0(void) { return D_00419530; }

void *func_002E74F0(void) { return D_00419638; }

void *func_002E7500(void *self, s32 i) { return D_004196E0[i]; }

void *func_002E7520(void) { return D_00419710; }

/* (self->*D_01990F30[i])(a, b) */
s32 func_002E7530(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F30[i & 0xFF], a, b);
}

s32 func_002E7560(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B278, cmd, 1.0f); }

/* character kind 0x1A: byte 3 0 starts func_0032D270(2, -6, 257); else waits (2) until
 * func_0032D150 says done */
s32 func_002E7600(void *self, void *a1, u8 *cmd) {
    Character *c = gCharacters[func_001770D0(gProgress, 0x1A) & 0xFF];

    if (cmd[3] == 0) {
        func_0032D270(c, 2, -6.0f, 257.0f);
        return 1;
    }
    return func_0032D150(c) == 0 ? 2 : 1;
}
