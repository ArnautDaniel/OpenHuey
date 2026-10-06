/* Room 0x25: its event handler class (vtable D_0046E3C0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E3C0[];
extern void *D_00470A50[];   /* the rising motes */
extern u8 D_00402330[];
extern u8 D_00402420[];
extern u8 D_004024F0[];
extern u8 D_00402700[];
extern u8 D_004027E0[];
extern u8 D_00402880[];
extern u32 D_00402CD0[];
extern u8 D_00402D30[];
extern u32 D_0047AB20[];

extern PTMF D_01990B38[];
extern PTMF D_01990B48[];

static void motes_init(void **obj) {
    obj[0] = D_00470A50;
    obj[0x3010 / 4] = D_00469D00;
    ((s32 *)obj)[0x3014 / 4] = -1;
    obj[0x3010 / 4] = D_0046FC30;
}

void *func_002B00F0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E3C0, D_0046DB80); }

void *func_002B0150(void) {
    return D_00402330;
}

void *func_002B0160(void) {
    return D_00402420;
}

void *func_002B0170(void) {
    return D_004024F0;
}

void *func_002B0180(void) {
    return D_00402700;
}

void *func_002B0190(void) {
    return D_004027E0;
}

void *func_002B01A0(void) {
    return D_00402880;
}

u32 func_002B01B0(void *self, s32 i) {
    return D_00402CD0[i];
}

void *func_002B01D0(void) {
    return D_00402D30;
}

u32 func_002B01E0(void *self, s32 i) {
    return D_0047AB20[i];
}

/* (self->*D_01990B48[i])(a, b) */
s32 func_002B0200(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B48[i & 0xFF], a, b);
}

s32 func_002B0230(void) {
    u8 *obj = (u8 *)gCharPlayer;

    if (obj == NULL || ((u8 *)gCharPlayer)[0x28] != 1 || *(s32 *)((u8 *)gCharPlayer + 0xF8) != 4 ||
        *(s32 *)((u8 *)gCharPlayer + 0x100) != 0xFF) {
        return 0;
    }
    return 1;
}

/* (self->*D_01990B38[i])(a, b) */
s32 func_002B02A0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B38[i & 0xFF], a, b);
}

/* the rising motes started */
s32 func_002B02D0(void) {
    Effect_New(gEffects, 0x3860, motes_init);
    return 1;
}
