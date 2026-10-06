/* Room 0x56: its event handler class (vtable D_0046E880, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E880[];
extern VObject *D_00456E00;

extern u8 D_0040ED10[];
extern u8 D_0040ED80[];
extern u8 D_0040EDC0[];
extern u8 D_0040EF30[];
extern u8 D_0040EFB0[];
extern u32 D_0040F430[];
extern u8 D_0040F478[];

extern PTMF D_01990DC8[];

void *func_002B4CD0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E880, D_0046DB80); }

void *func_002B4D30(void) {
    return D_0040ED10;
}

void *func_002B4D40(void) {
    return D_0040ED80;
}

void *func_002B4D50(void) {
    return D_0040EDC0;
}

void *func_002B4D60(void) {
    return D_0040EF30;
}

void *func_002B4D70(void) {
    return D_0040EFB0;
}

u32 func_002B4D80(void *self, s32 i) {
    return D_0040F430[i];
}

void *func_002B4DA0(void) {
    return D_0040F478;
}

/* (self->*D_01990DC8[i])(a, b) */
s32 func_002B4DB0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990DC8[i & 0xFF], a, b);
}

/* room 0x56 (D_0040F468): creatures 7..9 in the current room on a live triangle D_00456E00
 * says yes to: +0x10, then the creature list's +0x28 */
s32 func_002B4DE0(void) {
    u8 *list = gCreatures;
    Progress *g = gProgress;
    VObject *chk = D_00456E00;
    s32 k;

    for (k = 7; k < 10; k++) {
        VObject *c = AT(list, 0x1C + (k - 7) * 4, VObject *);

        if (c == NULL || AT(c, 0x30, s32) != VCALL((VObject *)g, 0xC, s32 (*)(VObject *))((VObject *)g)
            || AT(c, 0x34, s32) == -1
            || (u8)VCALL(chk, 0x14, s32 (*)(VObject *, s32, s32))(chk, AT(c, 0x34, s32), 0) != 1) {
            continue;
        }
        VCALL(c, 0x10, void (*)(VObject *))(c);
        VCALL_AT(list, 0x28, 0x28, void (*)(u8 *, s32))(list, k & 0xFF);
    }
    return 1;
}
