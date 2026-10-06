/* Room 0x108: its event handler class (vtable D_0046FE00, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"

extern void *D_0046DB80[];
extern void *D_0046FE00[];
extern const char *D_004193A8, *D_004193AC;

extern u8 D_00418DC0[];
extern u8 D_00418E40[];
extern u8 D_00418F40[];
extern u8 D_00419050[];
extern void *D_00419370[];
extern u8 D_004190F0[];
extern u8 D_004193B0[];

extern void *D_004193A0[];

extern PTMF D_01990F18[];

void *func_002E7220(void *o, s32 flags) { return room_dtor(o, flags, D_0046FE00, D_0046DB80); }

void *func_002E7280(void) { return D_00418DC0; }

void *func_002E7290(void) { return D_00418E40; }

void *func_002E72A0(void) { return D_00418F40; }

void *func_002E72B0(void) { return D_00419050; }

void *func_002E72C0(void *self, s32 i) { return D_00419370[i]; }

void *func_002E72E0(void) { return D_004190F0; }

void *func_002E72F0(void) { return D_004193B0; }

void *func_002E7300(void *self, s32 i) { return D_004193A0[i]; }

/* (self->*D_01990F18[i])(a, b) */
s32 func_002E7320(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F18[i & 0xFF], a, b);
}

/* byte 3: 0 / 1 a named progress call; 2 waits (2) for func_0016CD60(1, 0), then the partner's
 * message slot shows progress +0x73EDC0; else func_0016CD30 and the slot is closed */
s32 func_002E7350(void *self, void *a1, u8 *cmd) {
    Progress *p;

    switch (cmd[3]) {
    case 0:
        func_0016CEC0(gProgress, D_004193A8);
        break;
    case 1:
        func_0016CEC0(gProgress, D_004193AC);
        break;
    case 2:
        p = gProgress;
        if (func_0016CD60(p, 1, 0) == 0) {
            return 2;
        }
        VCALL(gBootMessage, 0x10, void (*)(VObject *, u32, s32, s32))(gBootMessage, gCharPartner->msgSlot,
                                                                       AT(p, 0x73EDC0, s32), 1);
        break;
    default:
        ((void (*)(Progress *))func_0016CD30)(gProgress);
        VCALL(gBootMessage, 0x14, void (*)(VObject *, u32))(gBootMessage, gCharPartner->msgSlot);
        break;
    }
    return 1;
}
