/* Room 0x2B: its event handler class (vtable D_0046E500, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E500[];
extern s32 func_001770D0(Progress *p, s32 kind);   /* the slot of character kind (0xFF) */
extern s32 func_0032D2C0(Character *c);

extern u8 D_00405640[];
extern u8 D_004056D0[];
extern u8 D_00405730[];
extern u8 D_00405850[];
extern u32 D_00405A70[];
extern u8 D_00405AA0[];

#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

void func_0032D3E0(u8 *self, s32 a, f32 x, f32 y);

extern PTMF D_01990C00[];

void *func_002B1600(void *o, s32 flags) { return room_dtor(o, flags, D_0046E500, D_0046DB80); }

void *func_002B1660(void) {
    return D_00405640;
}

void *func_002B1670(void) {
    return D_004056D0;
}

void *func_002B1680(void) {
    return D_00405730;
}

void *func_002B1690(void) {
    return D_00405850;
}

u32 func_002B16A0(void *self, s32 i) {
    return D_00405A70[i];
}

void *func_002B16C0(void) {
    return D_00405AA0;
}

/* (self->*D_01990C00[i])(a, b) */
s32 func_002B16D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C00[i & 0xFF], a, b);
}

/* character kind 0x1A: byte 3 0 starts func_0032D3E0(2, -290, 42); else waits (2) until
 * func_0032D2C0 says done */
s32 func_002B1700(void *self, void *a1, u8 *cmd) {
    Character *c = gCharacters[func_001770D0(gProgress, 0x1A) & 0xFF];

#ifdef HG_NATIVE
    if ((func_001770D0(gProgress, 0x1A) & 0xFF) >= 6 || c == NULL) {   /* (the PS2 writes through junk) */
        return 1;
    }
#endif
    if (cmd[3] != 0) {
        return func_0032D2C0(c) == 0 ? 2 : 1;
    }
    func_0032D3E0((u8 *)c, 2, -290.0f, 42.0f);
    return 1;
}

void func_0032D3E0(u8 *self, s32 a, f32 x, f32 y) {
    S32(self, 0x1624) = 0;
    S32(self, 0x1628) = 1;
    S32(self, 0x162C) = 60;
    F32(self, 0x1634) = F32(self, 0x10);
    F32(self, 0x1638) = F32(self, 0x18);
    F32(self, 0x163C) = x;
    F32(self, 0x1640) = y;
    self[0x16A8] = 1;
    self[0x16A9] = 1;
    S32(self, 0x1630) = a;
}
