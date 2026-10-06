/* Room 0x18: its event handler class (vtable D_0046E080, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E080[];
extern u8 D_0047AA40[];
extern void func_0025F810(u8 *o);

extern u8 D_003FAA20[];
extern u8 D_003FAAA0[];
extern u8 D_003FAB30[];
extern u8 D_003FACB0[];
extern u8 D_003FAD40[];
extern void *D_003FB320[];
extern u8 D_003FB390[];
extern PTMF D_01990940[];

void *func_002AD020(void *o, s32 flags) { return room_dtor(o, flags, D_0046E080, D_0046DB80); }

void *func_002AD080(void) {
    return D_003FAA20;
}

void *func_002AD090(void) {
    return D_003FAAA0;
}

void *func_002AD0A0(void) {
    return D_003FAB30;
}

void *func_002AD0B0(void) {
    return D_003FACB0;
}

void *func_002AD0C0(void) {
    return D_003FAD40;
}

void *func_002AD0D0(void *o) { return D_0047AA40; }   /* D_0046E080 +0x20 */

void *func_002AD0E0(void *self, s32 i) {
    return D_003FB320[i];
}

void *func_002AD100(void) {
    return D_003FB390;
}

void *func_002AD110(void *self, s32 i) {
    return (void *)D_003FB370[i];
}

/* (self->*D_01990940[i])(a, b) */
s32 func_002AD130(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990940[i & 0xFF], a, b);
}

/* room 0x18 (D_003FB358): object byte 3's func_0025F810 */
s32 func_002AD160(void *self, void *a1, u8 *cmd) {
    u8 *o = room_obj(D_003FB370[cmd[3]]);

    if (o != NULL) {
        func_0025F810(o);
    }
    return 1;
}
