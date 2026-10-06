/* Room 0x06: its event handler class (vtable D_0046DCC0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "sce/libvu0.h"

extern void *D_0046DB80[];
extern void *D_0046DCC0[];
extern const f32 D_003F2180[4][4];   /* room 0x06: where its four objects go */
extern const char D_0047A9A0[7];     /* room 0x06: the first object's name (its 6th letter counts on) */

extern u8 D_003F1B60[];
extern u8 D_003F1BD0[];
extern u8 D_003F1CD0[];
extern u8 D_003F1D58[];
extern u8 D_003F1D70[];
extern u8 D_003F1F00[];
extern void *D_003F2170[];
extern void *D_003F21D0[];
extern PTMF D_019907D0[];

void *func_002AA250(void *o, s32 flags) { return room_dtor(o, flags, D_0046DCC0, D_0046DB80); }

void *func_002AA2B0(void) {
    return D_003F1B60;
}

void *func_002AA2C0(void) {
    return D_003F1BD0;
}

void *func_002AA2D0(void) {
    return D_003F1CD0;
}

void *func_002AA2E0(void) {
    return D_003F1D58;
}

void *func_002AA2F0(void) {
    return D_003F1D70;
}

void *func_002AA300(void) {
    return D_003F1F00;
}

void *func_002AA310(void *self, s32 i) {
    return D_003F2170[i];
}

void *func_002AA330(void *self, s32 i) {
    return D_003F21D0[i];
}

/* (self->*D_019907D0[i])(a, b) */
s32 func_002AA350(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019907D0[i & 0xFF], a, b);
}

/* room 0x06 (D_003F21C0): its four objects (the name's 6th letter counting) to their places */
s32 func_002AA380(void) {
    char name[7];
    s32 i;

    for (i = 0; i < 7; i++) {
        name[i] = D_0047A9A0[i];
    }
    for (i = 0; i < 4; i++) {
        u8 *o = room_obj(name);

#ifdef HG_NATIVE
        if (o != NULL)   /* (the PS2 writes through junk) */
#endif
        sceVu0CopyVector((f32 *)(o + 0x20), (f32 *)D_003F2180[i]);
        name[5]++;
    }
    return 1;
}
