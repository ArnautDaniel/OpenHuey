/* Room 0x27: its event handler class (vtable Room27_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "items.h"

extern void *RoomBase_vtable[];
extern void *Room27_vtable[];
extern u8 D_0047ACB0[];

extern u8 D_0041C390[];
extern u8 D_0041C3E0[];
extern u8 D_0041C420[];
extern void *D_0041CA40[];
extern PTMF D_01990FD8[];

/* 0x002FCB40 */
void *Room27_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room27_vtable, RoomBase_vtable); }

/* 0x002FCBA0 */
void *Room27_EnterScript(void) {
    return D_0041C390;
}

/* 0x002FCBB0 */
void *Room27_CharEnterScript(void *o) { return D_0047ACB0; }   /* Room27_vtable +0x30 */

/* 0x002FCBC0 */
void *Room27_Phase1Script(void) {
    return D_0041C3E0;
}

/* 0x002FCBD0 */
void *Room27_Phase2Script(void) {
    return D_0041C420;
}

/* 0x002FCBE0 */
void *Room27_ActionScript(void *self, s32 i) {
    return D_0041CA40[i];
}

/* (self->*D_01990FD8[i])(a, b) */
/* 0x002FCC00 */
s32 Room27_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990FD8[i & 0xFF], a, b);
}

/* the items 0x91 / 0x92, by byte 3: 0 which of them Fiona lacks (one each) kept in event var 0
 * (0x91 low byte, 0x92 the next), and event +0x5C(3) when any; 1 they are given back, event
 * +0x5C(0) / (1) */
/* 0x002FCC30 */
s32 Room27_Cmd00(void *self, void *a1, u8 *cmd) {
    VObject *ev;
    u32 got;

    switch (cmd[3]) {
    case 0: {
        u8 *items = (u8 *)gSubScreen + 8;
        s32 a = 1 - (Items_Count(items, 0x91) & 0xFF);
        s32 b;

        if (a < 0) {
            a = 0;
        }
        b = 1 - (Items_Count(items, 0x92) & 0xFF);
        if (b < 0) {
            b = 0;
        }
        if ((a | b) != 0) {
            ev = gEvents;
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, (b << 8) | a);
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 3);
        }
        break;
    }
    case 1:
        ev = gEvents;
        got = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 0);
        if (got & 0xFF) {
            Items_Give((u8 *)gSubScreen + 8, 0x91, got & 0xFF);
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 0);
        }
        if (got & 0xFF00) {
            Items_Give((u8 *)gSubScreen + 8, 0x92, (got >> 8) & 0xFF);
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 1);
        }
        break;
    }
    return 1;
}
