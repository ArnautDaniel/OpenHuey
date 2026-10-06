/* Room 0xE4: its event handler class (vtable D_0047A550, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A550[];
extern u8 D_0047B0E8[], D_0047B0F0[];
extern const char *D_00449540;   /* a room object's name */
extern char D_0047B318[];
extern const char D_004639B8[];

extern u8 D_00449250[];
extern u8 D_004492C0[];
extern u8 D_00449380[];
extern u8 D_004494F0[];
extern u8 D_00449558[];
extern PTMF D_01991D90[];
extern PTMF D_01991DA8[];

void *func_00379820(void *o, s32 flags) { return room_dtor(o, flags, D_0047A550, D_0046DB80); }

void *func_00379880(void) {
    return D_00449250;
}

void *func_00379890(void) {
    return D_004492C0;
}

void *func_003798A0(void) {
    return D_00449380;
}

void *func_003798B0(void *o) { return D_0047B0E8; }   /* D_0047A550 +0x14 */

void *func_003798C0(void) {
    return D_004494F0;
}

u32 func_003798D0(void *o, s32 i) { return ((u32 *)D_0047B0F0)[i]; }   /* D_0047A550 +0x24 */

void *func_003798F0(void) {
    return D_00449558;
}

void *func_00379900(void *self, s32 i) {
    return ((void **)&D_00449540)[i];   /* its table of names (one is reached by name too) */
}

/* (self->*D_01991DA8[i])(a, b) */
s32 func_00379920(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991DA8[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_00379950(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991D90[i])(a, b) */
s32 func_00379970(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D90[i & 0xFF], a, b);
}

s32 func_003799A0(void) { return clock_draw(D_0047B318, D_004639B8); }

/* (as func_002ABDD0) the room object D_00449540 by event variable 2 */
s32 func_00379AF0(void *self, void *a1, u8 *cmd) {
    return var_fade(D_00449540, 2, cmd);
}
