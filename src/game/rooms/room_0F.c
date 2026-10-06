/* Room 0x0F: its event handler class (vtable D_0046DEC0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"

extern void *D_0046DB80[];
extern void *D_0046DEC0[];
extern const char *D_003F6F64;
extern u8 D_003F5EC0[];
extern u8 D_003F5FC0[];
extern u8 D_003F6120[];
extern u8 D_003F6420[];
extern void *D_003F6E70[];
extern void *D_003F6F40[];
extern u8 D_003F6F68[];
extern PTMF D_01990890[];

/* an object's +0x14 back to 0 (the PS2 writes through junk when it isn't there) */
static inline void obj_unturn(const char *name) {
    u8 *o = room_obj(name);

#ifdef HG_NATIVE
    if (o == NULL) {
        return;
    }
#endif
    AT(o, 0x14, s32) = 0;
}

void *func_002AB9F0(void *o, s32 flags) { return room_dtor(o, flags, D_0046DEC0, D_0046DB80); }

void *func_002ABA50(void) {
    return D_003F5EC0;
}

void *func_002ABA60(void) {
    return D_003F5FC0;
}

void *func_002ABA70(void) {
    return D_003F6120;
}

void *func_002ABA80(void) {
    return D_003F6420;
}

void *func_002ABA90(void *self, s32 i) {
    return D_003F6E70[i];
}

void *func_002ABAB0(void) {
    return D_003F6F68;
}

void *func_002ABAC0(void *self, s32 i) {
    return D_003F6F40[i];
}

/* (self->*D_01990890[i])(a, b) */
s32 func_002ABAE0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990890[i & 0xFF], a, b);
}

/* room 0x0F (D_003F6F28): the player's model +0xD0 (0, 1.5, -2 / -1.5 by byte 3) and +0xCC */
s32 func_002ABB10(void *self, void *a1, u8 *cmd) {
    VObject *m = gCharPlayer->motion;

    if (cmd[3] == 0) {
        VCALL(m, 0xD0, void (*)(VObject *, f32, f32, f32))(m, 0.0f, 1.5f, -2.0f);
        VCALL(m, 0xCC, void (*)(VObject *, s32))(m, 1);
    } else {
        VCALL(m, 0xD0, void (*)(VObject *, f32, f32, f32))(m, 0.0f, 1.5f, -1.5f);
        VCALL(m, 0xCC, void (*)(VObject *, s32))(m, 0);
    }
    return 1;
}

/* room 0x0F (D_003F6F18): the player's model +0xBC (1, 0.25) or (0, 0) by byte 3 */
s32 func_002ABBC0(void *self, void *a1, u8 *cmd) {
    VObject *m = gCharPlayer->motion;

    if (cmd[3] == 0) {
        VCALL(m, 0xBC, void (*)(VObject *, s32, f32))(m, 1, 0.25f);
    } else {
        VCALL(m, 0xBC, void (*)(VObject *, s32, f32))(m, 0, 0.0f);
    }
    return 1;
}

/* room 0x0F (D_003F6F08): an effect (D_00471060, 0x840 bytes) with its box */
s32 func_002ABC20(void) {
    s32 slot = Effect_New(gEffects, 0x840, effect_471060_init);
    f32 prm[9];

    prm[1] = 140.0f;
    prm[2] = -9.5f;
    prm[0] = 0.0f;
    prm[3] = 90.0f;
    prm[6] = 0.0f;
    prm[4] = 18.0f;
    prm[8] = 0.0f;
    prm[5] = 110.0f;
    prm[7] = -90.0f;
    func_002D6090(gEffects, slot, prm);
    return 1;
}

/* room 0x0F (D_003F6EF8): three objects' +0x14 back to 0 */
s32 func_002ABD50(void) {
    obj_unturn(D_003F6F48);
    obj_unturn(D_003F6F4C);
    obj_unturn(D_003F6F50);
    return 1;
}

s32 func_002ABDD0(void *self, void *a1, u8 *cmd) {
    return var_fade(D_003F6F64, 0, cmd);
}
