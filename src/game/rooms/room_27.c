/* Room 0x27: its event handler class (vtable D_00470DC0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00470DC0[];
extern u8 D_0047ACB0[];
extern u32 func_00260CF0(void *list, s32 item);   /* how many */
extern void func_00261090(void *list, s32 item, s32 n);   /* given */

extern u8 D_0041C390[];
extern u8 D_0041C3E0[];
extern u8 D_0041C420[];
extern void *D_0041CA40[];
extern PTMF D_01990FD8[];

void *func_002FCB40(void *o, s32 flags) { return room_dtor(o, flags, D_00470DC0, D_0046DB80); }

void *func_002FCBA0(void) {
    return D_0041C390;
}

void *func_002FCBB0(void *o) { return D_0047ACB0; }   /* D_00470DC0 +0x30 */

void *func_002FCBC0(void) {
    return D_0041C3E0;
}

void *func_002FCBD0(void) {
    return D_0041C420;
}

void *func_002FCBE0(void *self, s32 i) {
    return D_0041CA40[i];
}

/* (self->*D_01990FD8[i])(a, b) */
s32 func_002FCC00(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990FD8[i & 0xFF], a, b);
}

/* the items 0x91 / 0x92, by byte 3: 0 which of them Fiona lacks (one each) kept in event var 0
 * (0x91 low byte, 0x92 the next), and event +0x5C(3) when any; 1 they are given back, event
 * +0x5C(0) / (1) */
s32 func_002FCC30(void *self, void *a1, u8 *cmd) {
    VObject *ev;
    u32 got;

    switch (cmd[3]) {
    case 0: {
        u8 *items = (u8 *)gSubScreen + 8;
        s32 a = 1 - (func_00260CF0(items, 0x91) & 0xFF);
        s32 b;

        if (a < 0) {
            a = 0;
        }
        b = 1 - (func_00260CF0(items, 0x92) & 0xFF);
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
            func_00261090((u8 *)gSubScreen + 8, 0x91, got & 0xFF);
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 0);
        }
        if (got & 0xFF00) {
            func_00261090((u8 *)gSubScreen + 8, 0x92, (got >> 8) & 0xFF);
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 1);
        }
        break;
    }
    return 1;
}
