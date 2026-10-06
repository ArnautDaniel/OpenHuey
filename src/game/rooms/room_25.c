/* Room 0x25: its event handler class (vtable Room25_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room25_vtable[];
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
    obj[0x3010 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x3014 / 4] = -1;
    obj[0x3010 / 4] = QuadDrawer_vtable;
}

/* 0x002B00F0 */
void *Room25_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room25_vtable, RoomBase_vtable); }

/* 0x002B0150 */
void *Room25_EnterScript(void) {
    return D_00402330;
}

/* 0x002B0160 */
void *Room25_CharEnterScript(void) {
    return D_00402420;
}

/* 0x002B0170 */
void *Room25_Phase1Script(void) {
    return D_004024F0;
}

/* 0x002B0180 */
void *Room25_Phase2Script(void) {
    return D_00402700;
}

/* 0x002B0190 */
void *Room25_Phase3Script(void) {
    return D_004027E0;
}

/* 0x002B01A0 */
void *Room25_Phase5Script(void) {
    return D_00402880;
}

/* 0x002B01B0 */
u32 Room25_ActionScript(void *self, s32 i) {
    return D_00402CD0[i];
}

/* 0x002B01D0 */
void *Room25_Table38(void) {
    return D_00402D30;
}

/* 0x002B01E0 */
u32 Room25_ObjectName(void *self, s32 i) {
    return D_0047AB20[i];
}

/* (self->*D_01990B48[i])(a, b) */
/* 0x002B0200 */
s32 Room25_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B48[i & 0xFF], a, b);
}

/* 0x002B0230 */
s32 Room25_Cond00(void) {
    u8 *obj = (u8 *)gCharPlayer;

    if (obj == NULL || ((u8 *)gCharPlayer)[0x28] != 1 || *(s32 *)((u8 *)gCharPlayer + 0xF8) != 4 ||
        *(s32 *)((u8 *)gCharPlayer + 0x100) != 0xFF) {
        return 0;
    }
    return 1;
}

/* (self->*D_01990B38[i])(a, b) */
/* 0x002B02A0 */
s32 Room25_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B38[i & 0xFF], a, b);
}

/* the rising motes started */
/* 0x002B02D0 */
s32 Room25_Cmd00(void) {
    Effect_New(gEffects, 0x3860, motes_init);
    return 1;
}
