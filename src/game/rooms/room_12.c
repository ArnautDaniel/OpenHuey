/* Room 0x12: its event handler class (vtable D_0046DF80, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046DF80[];
extern void *D_00470E20[];
extern u8 D_003F8200[];
extern u8 D_003F8350[];
extern u8 D_003F83F0[];
extern u8 D_003F8750[];
extern u8 D_003F8900[];
extern u8 D_003F89D0[];
extern void *D_003F90F0[];
extern void *D_003F9170[];
extern u8 D_003F9190[];
extern PTMF D_019908E0[];
extern PTMF D_019908F0[];

static void smoke_puffs_init(void **obj) {
    obj[0] = D_00470E20;
    obj[0x1810 / 4] = D_00469D00;
    ((s32 *)obj)[0x1814 / 4] = -1;
    obj[0x1810 / 4] = D_0046FC30;
}

void *func_002AC190(void *o, s32 flags) { return room_dtor(o, flags, D_0046DF80, D_0046DB80); }

void *func_002AC1F0(void) {
    return D_003F8200;
}

void *func_002AC200(void) {
    return D_003F8350;
}

void *func_002AC210(void) {
    return D_003F83F0;
}

void *func_002AC220(void) {
    return D_003F8750;
}

void *func_002AC230(void) {
    return D_003F8900;
}

void *func_002AC240(void) {
    return D_003F89D0;
}

void *func_002AC250(void *self, s32 i) {
    return D_003F90F0[i];
}

void *func_002AC270(void) {
    return D_003F9190;
}

void *func_002AC280(void *self, s32 i) {
    return D_003F9170[i];
}

/* (self->*D_019908F0[i])(a, b) */
s32 func_002AC2A0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019908F0[i & 0xFF], a, b);
}

/* room 0x12 (D_003F9160): the pursuer is about, in a mode other than 0 and 3 */
s32 func_002AC2D0(void) {
    Character *s = gCharPursuer;

    return s != NULL && s->a.active != 0 && AT(s, 0xE8, s32) != 3 && AT(s, 0xE8, s32) != 0;
}

/* (self->*D_019908E0[i])(a, b) */
s32 func_002AC340(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019908E0[i & 0xFF], a, b);
}

/* room 0x12 (D_003F9150): byte 3 0: the smoke puffs (D_00470E20), their slot in script
 * variable 0; else that slot started */
s32 func_002AC370(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(gEffects, 0x1C60, smoke_puffs_init);

        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 0, slot);
    } else {
        func_002D6090(gEffects,
                      VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0), NULL);
    }
    return 1;
}
