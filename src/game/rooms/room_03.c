/* Room 0x03: its event handler class (vtable D_0046DC40, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"

extern void *D_0046DB80[];
extern void *D_0046DC40[];
extern const char *D_003F0DC4;
extern void *D_0046FF20[];
extern u8 D_003F0450[];
extern u8 D_003F0540[];
extern u8 D_003F0580[];
extern u8 D_003F06A0[];
extern u8 D_003F0820[];
extern u8 D_003F0860[];
extern void *D_003F0D60[];
extern void *D_003F0DB0[];
extern u8 D_003F0DD0[];
extern PTMF D_01990780[];

static inline void dust_init(void **o) {
    o[0] = D_0046FF20;
    o[0x610 / 4] = D_00469D00;
    ((s32 *)o)[0x614 / 4] = -1;
    o[0x610 / 4] = D_0046FC30;
}

/* 0x002A9600 */
void *Room03_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046DC40, D_0046DB80); }

/* 0x002A9660 */
void *Room03_EnterScript(void) {
    return D_003F0450;
}

/* 0x002A9670 */
void *Room03_CharEnterScript(void) {
    return D_003F0540;
}

/* 0x002A9680 */
void *Room03_Phase1Script(void) {
    return D_003F0580;
}

/* 0x002A9690 */
void *Room03_Phase2Script(void) {
    return D_003F06A0;
}

/* 0x002A96A0 */
void *Room03_Phase3Script(void) {
    return D_003F0820;
}

/* 0x002A96B0 */
void *Room03_Phase5Script(void) {
    return D_003F0860;
}

/* 0x002A96C0 */
void *Room03_ActionScript(void *self, s32 i) {
    return D_003F0D60[i];
}

/* 0x002A96E0 */
void *Room03_Table38(void) {
    return D_003F0DD0;
}

/* 0x002A96F0 */
void *Room03_ObjectName(void *self, s32 i) {
    return D_003F0DB0[i];
}

/* (self->*D_01990780[i])(a, b) */
/* 0x002A9710 */
s32 Room03_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990780[i & 0xFF], a, b);
}

/* a lever (D_003F0DC4, tilt +0x18 between -10 and 0 degrees): byte 3 0 back 2 degrees, 1 pulled
 * (-10) with a puff of grey dust at it */
/* 0x002A9740 */
s32 Room03_Cmd01(void *self, void *a1, u8 *cmd) {
    /* (volatile: a compile-time fold of the pulled case would round as IEEE, not as the EE) */
    static const volatile union { u32 u; f32 f; } kPi = {0x40490FDB};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F0DC4);
    s32 dust = 0;
    f32 deg;

    if (o == NULL) {
        return 1;
    }
    deg = 180.0f * AT(o, 0x18, f32) / kPi.f;
    switch (cmd[3]) {
    case 0:
        deg += 2.0f;
        break;
    case 1:
        dust = 1;
        deg = -10.0f;
        break;
    }
    if (!(deg <= 0.0f)) {
        deg = 0.0f;
    }
    if (deg < -10.0f) {
        deg = -10.0f;
    }
    AT(o, 0x18, f32) = kPi.f * deg / 180.0f;
    if (dust != 0) {
        u8 *mgr = gEffects;
        s32 slot = Effect_New(mgr, 0x720, dust_init);
        struct {
            f32 pos[4];
            s32 kind, r, g, b, size;
        } dp __attribute__((aligned(16)));

        dp.pos[0] = AT(o, 0x20, f32);
        dp.pos[1] = AT(o, 0x24, f32);
        dp.pos[2] = AT(o, 0x28, f32);
        dp.pos[3] = 1.0f;
        dp.b = 0x80;
        dp.g = 0x80;
        dp.r = 0x80;
        dp.kind = 0;
        EffectMgr_Start(mgr, slot, &dp);
    }
    return 1;
}

/* room 0x03 (D_003F0D90): three objects turned (-60, -60 degrees about x; -90 about z) */
/* 0x002A99A0 */
s32 Room03_Cmd00(void) {
    obj_angle(D_003F0DBC, 0x10, 0xBF860A92);
    obj_angle(D_003F0DC0, 0x10, 0xBF860A92);
    obj_angle(D_003F0DC4, 0x18, 0xBFC90FDB);
    return 1;
}
