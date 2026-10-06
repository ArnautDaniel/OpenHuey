/* Room 0xC5: its event handler class (vtable D_004786B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "memcard.h"
#include "scene_game_members.h"
#include "snd_place.h"

extern void *D_0046DB80[];
extern void *D_004786B0[];
extern u8 D_0047AF20[], D_0047AF28[];
extern void *D_00479870[];
extern const char *D_00441140[];   /* room objects 0..9 */
extern u32 D_004401B0[];
extern u32 D_00440210[];
extern u32 D_00440250[];
extern u32 D_00440E00[];
extern u32 D_00441110[];

extern void *D_0046F580[];

extern PTMF D_019919F8[];

static void effect_79870_init(void **obj) {
    obj[0] = D_00479870;
}

void *func_0034B0E0(void *o, s32 flags) { return room_dtor(o, flags, D_004786B0, D_0046DB80); }

void *func_0034B140(void) {
    return D_004401B0;
}

void *func_0034B150(void) {
    return D_00440210;
}

void *func_0034B160(void) {
    return D_00440250;
}

void *func_0034B170(void) {
    return D_00440E00;
}

void *func_0034B180(void *o) { return D_0047AF20; }   /* D_004786B0 +0x20 */

u32 func_0034B190(void *self, s32 i) {
    return D_00441110[i];
}

void *func_0034B1B0(void *o) { return D_0047AF28; }   /* D_004786B0 +0x38 */

u32 func_0034B1C0(void *self, s32 i) {
    return (u32)D_00441140[i];
}

/* (self->*D_019919F8[i])(a, b) */
s32 func_0034B1E0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019919F8[i & 0xFF], a, b);
}

/* the 0x10-byte effect D_00479870 on room object k + 1 (byte 4 = k, 1..8; script variable 11 - k
 * keeps its slot): made on first use when byte 3 is set, then sent (on byte 3, index 8 - k, the
 * variable, the object), with sound 1 at the object when on and the camera director's +0x38 is
 * clear */
s32 func_0034B210(void *self, void *a1, u8 *cmd) {
    u32 k = cmd[4];
    u8 var = 11 - k, idx = 8 - k;   /* (k 0 / past 8: unset on the PS2) */
    u32 name = k + 1;
    u8 on = cmd[3];
    VObject *ev = gEvents;
    s32 slot = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, var);
    u8 *o;
    struct {
        u8 on, idx, var, pad;
        u8 *obj;
    } msg;

    if (slot == -1) {
        if (on == 0) {
            return 1;
        }
        slot = Effect_New(gEffects, 0x10, effect_79870_init);
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, var, slot);
    }
    o = obj_named(D_00441140[name]);
    msg.idx = idx;
    msg.var = var;
    msg.on = on;
    msg.pad = 0;
    msg.obj = o;
    func_002D6090(gEffects, slot, &msg);
    if (VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) == 0 && on != 0) {
        func_002FF650(gSound, 1, 6, (f32 *)(o + 0x20), 0, 0);
    }
    return 1;
}

/* destructor (vtable D_00479870) */
void *func_0035BBD0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479870;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

void func_0035C9E0(u8 *o) {
    AT(o, 0x5, u8) = 0xFF;
}
