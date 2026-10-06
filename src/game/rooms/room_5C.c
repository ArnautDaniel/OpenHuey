/* Room 0x5C: its event handler class (vtable Room5C_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "actor.h"
#include "scene_game_members.h"
#include "sce/libvu0.h"

extern void *RoomBase_vtable[];
extern void *Room5C_vtable[];
extern void *CreatureVanish_vtable[];
extern u8 Room5C_EnterScript_data[];
extern u8 Room5C_CharEnterScript_data[];
extern u8 Room5C_Phase1Script_data[];
extern u8 Room5C_Phase2Script_data[];
extern u8 Room5C_Phase3Script_data[];
extern u32 Room5C_ActionScripts[];
extern u8 Room5C_Table38_data[];
extern u32 Room5C_ObjectNames[];

extern PTMF Room5C_CmdTable[];
extern PTMF Room5C_CondTable[];

static void effect_472370_init(void **obj) {
    obj[0] = CreatureVanish_vtable;
    obj[0x370 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x374 / 4] = -1;
    obj[0x370 / 4] = QuadDrawer_vtable;
}

/* 0x002B58B0 */
void *Room5C_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room5C_vtable, RoomBase_vtable); }

/* 0x002B5910 */
void *Room5C_EnterScript(void) {
    return Room5C_EnterScript_data;
}

/* 0x002B5920 */
void *Room5C_CharEnterScript(void) {
    return Room5C_CharEnterScript_data;
}

/* 0x002B5930 */
void *Room5C_Phase1Script(void) {
    return Room5C_Phase1Script_data;
}

/* 0x002B5940 */
void *Room5C_Phase2Script(void) {
    return Room5C_Phase2Script_data;
}

/* 0x002B5950 */
void *Room5C_Phase3Script(void) {
    return Room5C_Phase3Script_data;
}

/* 0x002B5960 */
u32 Room5C_ActionScript(void *self, s32 i) {
    return Room5C_ActionScripts[i];
}

/* 0x002B5980 */
void *Room5C_Table38(void) {
    return Room5C_Table38_data;
}

/* 0x002B5990 */
u32 Room5C_ObjectName(void *self, s32 i) {
    return Room5C_ObjectNames[i];
}

/* (self->*Room5C_CondTable[i])(a, b) */
/* 0x002B59B0 */
s32 Room5C_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room5C_CondTable[i & 0xFF], a, b);
}

/* room 0x5C (Room5C_Cond00_ptmf): the first creature within 3 of (59.1, 1.43) is put away with an
 * effect (CreatureVanish_vtable) above it - blue (+0x1571 below 0x12) or red - and its action 0x8B */
/* 0x002B59E0 */
s32 Room5C_Cond00(void) {
    s32 i;

    for (i = 0; i < 7; i++) {
        Character *c = AT(gCreatures, i * 4, Character *);
        f32 dz, dx;

        if (c == NULL) {
            continue;
        }
        dz = c->a.pos[2] - 0x1.6e147a0000000p+0f /* 1.43 */;
        dx = c->a.pos[0] - 0x1.d8cccc0000000p+5f /* 59.1 */;
        if (dz * dz + dx * dx < 9.0f) {
            struct {
                f32 pos[4];
                u8 col[4];
            } prm __attribute__((aligned(16)));
            s32 slot;

            c->a.active = 0;
            {
                /* (Effect_New, with the heap's +0x10 handed the loop index in a2 as the
                   original leaves it there - unused) */
                u8 *mgr = gEffects;
                void *mem = VCALL(EFFECT_HEAP(mgr), 0x10, void *(*)(VObject *, u32, s32))(EFFECT_HEAP(mgr), 0x4A0, i);
                s32 j;

                slot = -1;
                if (mem != NULL) {
                    for (j = 0; j < EFFECT_NUM_SLOTS; j++) {
                        if (EFFECT_SLOTS(mgr)[j] == NULL) {
                            void **obj = EffectMgr_new(0x4A0, mem);

                            if (obj != NULL) {
                                effect_472370_init(obj);
                            }
                            EFFECT_SLOTS(mgr)[j] = obj;
                            VCALL(EFFECT_SLOTS(mgr)[j], 0xC, void (*)(void **))(EFFECT_SLOTS(mgr)[j]);
                            slot = j;
                            break;
                        }
                    }
                }
            }
            sceVu0CopyVector(prm.pos, c->a.pos);
            prm.pos[1] = 12.0f + c->a.pos[1] + AT(c, 0x1554, f32);
            if (AT(c, 0x1571, u8) < 0x12) {
                prm.col[2] = 0x80;
                prm.col[0] = 0x30;
                prm.col[3] = 0x60;
                prm.col[1] = 0x30;
            } else {
                prm.col[0] = 0x80;
                prm.col[1] = 0x30;
                prm.col[3] = 0x60;
                prm.col[2] = 0x30;
            }
            EffectMgr_Start(gEffects, slot, &prm);
            Actor_PlaySound(&c->a, 0x8B, 5, 0, 0, NULL);
            return 1;
        }
    }
    return 0;
}

/* (self->*Room5C_CmdTable[i])(a, b) */
/* 0x002B5C00 */
s32 Room5C_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room5C_CmdTable[i & 0xFF], a, b);
}

/* room 0x5C (Room5C_Cmd00_ptmf): the dial (+0x7C, 0..1) from script variable 0 by byte 3: 0 0x2B..0x38
 * (/ 13, +0x74 0 / +0x78 1), 1 0xC..0x20 (/ 20), 2 7..0x12 (/ 11) (+0x74 1 / +0x78 0) */
/* 0x002B5C30 */
s32 Room5C_Cmd00(void *self, void *a1, u8 *cmd) {
    u8 *o = room_obj(D_0047ABEC);
    u32 v;

    if (o == NULL) {
        return 1;
    }
    v = VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0);
    switch (cmd[3]) {
    case 0:
        v = v < 0x2B ? 0x2B : v;
        v = v < 0x39 ? v : 0x38;
        AT(o, 0x74, s32) = 0;
        AT(o, 0x78, s32) = 1;
        AT(o, 0x7C, f32) = (f32)(v - 0x2B) / 13.0f;
        break;
    case 1:
        v = v < 0xC ? 0xC : v;
        v = v < 0x21 ? v : 0x20;
        AT(o, 0x74, s32) = 1;
        AT(o, 0x78, s32) = 0;
        AT(o, 0x7C, f32) = (f32)(v - 0xC) / 20.0f;
        break;
    case 2:
        v = v < 7 ? 7 : v;
        v = v < 0x13 ? v : 0x12;
        AT(o, 0x74, s32) = 1;
        AT(o, 0x78, s32) = 0;
        AT(o, 0x7C, f32) = (f32)(v - 7) / 11.0f;
        break;
    }
    if (!(AT(o, 0x7C, f32) <= 1.0f)) {
        AT(o, 0x7C, f32) = 1.0f;
    }
    if (AT(o, 0x7C, f32) < 0.0f) {
        AT(o, 0x7C, f32) = 0.0f;
    }
    return 1;
}
