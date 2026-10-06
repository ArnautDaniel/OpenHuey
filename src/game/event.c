/* The event script system (SceneGame +0xF6AFB0, gEvents): runs the rooms' event scripts.
 * +0x120 holds one handler object per room (0x110 rooms); its vtable gives the room's scripts
 * for each phase (+0xC entering, +0x10.. +0x20 other phases). */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "navmesh.h"
#include "actor.h"
#include "ptmf.h"
#include "item.h"
#include "event.h"
#include "event_cmd.h"
#include "overlay.h"
#include "scene_game.h"
#include "script.h"
#include "msl.h"

extern void *Room101_vtable[], *Room100_vtable[], *Room82_vtable[], *Room67_vtable[], *Room65_vtable[], *Room64_vtable[];
extern void *Room5F_vtable[], *Room5E_vtable[], *Room5B_vtable[], *Room4D_vtable[], *Room44_vtable[], *Room42_vtable[];
extern void *Room41_vtable[], *Room16_vtable[], *Room07_vtable[], *Room05_vtable[], *Room01_vtable[], *Room00_vtable[];
extern void *Room02_vtable[], *Room03_vtable[], *Room04_vtable[], *Room06_vtable[], *Room08_vtable[], *Room09_vtable[];
extern void *Room0A_vtable[], *Room0B_vtable[], *Room0C_vtable[], *Room0D_vtable[], *Room0E_vtable[], *Room0F_vtable[];
extern void *Room10_vtable[], *Room11_vtable[], *Room12_vtable[], *Room13_vtable[], *Room14_vtable[], *Room15_vtable[];
extern void *Room18_vtable[], *Room19_vtable[], *Room1A_vtable[], *Room1B_vtable[], *Room1C_vtable[], *Room1D_vtable[];
extern void *Room1E_vtable[], *Room1F_vtable[], *Room20_vtable[], *Room21_vtable[], *Room22_vtable[], *Room23_vtable[];
extern void *Room24_vtable[], *Room25_vtable[], *Room26_vtable[], *Room28_vtable[], *Room29_vtable[], *Room2A_vtable[];
extern void *Room2B_vtable[], *Room2D_vtable[], *Room40_vtable[], *Room43_vtable[], *Room45_vtable[], *Room49_vtable[];
extern void *Room4A_vtable[], *Room4B_vtable[], *Room4C_vtable[], *Room4E_vtable[], *Room4F_vtable[], *Room50_vtable[];
extern void *Room51_vtable[], *Room52_vtable[], *Room56_vtable[], *Room57_vtable[], *Room58_vtable[], *Room59_vtable[];
extern void *Room5A_vtable[], *Room5C_vtable[], *Room5D_vtable[], *Room2F_vtable[], *Room30_vtable[], *Room17_vtable[];
extern void *Room46_vtable[], *Room102_vtable[], *Room103_vtable[], *Room104_vtable[], *Room105_vtable[], *Room106_vtable[];
extern void *Room107_vtable[], *Room108_vtable[], *Room109_vtable[], *Room10A_vtable[], *Room10B_vtable[], *Room27_vtable[];
extern void *Room2C_vtable[], *Room2E_vtable[], *Room66_vtable[], *Room55_vtable[], *Room63_vtable[], *Room62_vtable[];
extern void *Room68_vtable[], *Room47_vtable[], *Room48_vtable[], *Room53_vtable[], *Room54_vtable[], *Room60_vtable[];
extern void *Room61_vtable[], *Room31_vtable[], *Room32_vtable[], *Room86_vtable[], *RoomC0_vtable[], *RoomC7_vtable[];
extern void *Room81_vtable[], *Room83_vtable[], *Room84_vtable[], *Room85_vtable[], *Room87_vtable[], *Room88_vtable[];
extern void *Room8A_vtable[], *Room8C_vtable[], *Room8D_vtable[], *Room8E_vtable[], *Room8F_vtable[], *Room91_vtable[];
extern void *Room92_vtable[], *Room97_vtable[], *Room69_vtable[], *Room6A_vtable[], *Room6B_vtable[], *Room6C_vtable[];
extern void *Room6D_vtable[], *Room6E_vtable[], *Room6F_vtable[], *Room70_vtable[], *Room80_vtable[], *RoomC1_vtable[];
extern void *RoomC2_vtable[], *RoomC3_vtable[], *RoomC4_vtable[], *RoomC5_vtable[], *RoomC6_vtable[], *RoomC8_vtable[];
extern void *Room34_vtable[], *Room33_vtable[], *Room98_vtable[], *Room35_vtable[], *Room99_vtable[], *Room9A_vtable[];
extern void *Room9B_vtable[], *Room36_vtable[], *Room93_vtable[], *Room94_vtable[], *Room95_vtable[], *Room96_vtable[];
extern void *Room37_vtable[], *RoomD0_vtable[], *RoomD1_vtable[], *RoomD2_vtable[], *RoomD3_vtable[], *RoomD4_vtable[];
extern void *RoomD5_vtable[], *RoomD6_vtable[], *RoomD7_vtable[], *RoomD8_vtable[], *RoomD9_vtable[], *RoomE0_vtable[];
extern void *RoomE1_vtable[], *RoomE2_vtable[], *RoomE3_vtable[], *RoomE4_vtable[], *RoomE5_vtable[], *RoomE6_vtable[];
extern void *RoomE7_vtable[], *RoomE8_vtable[], *RoomE9_vtable[];

extern void *RoomHandler_new(u32 size, void *place);   /* placement new */

/* the rooms' handler classes (the other rooms keep the base handler) */
static const struct {
    s16 room;
    void **vtbl;
} sRooms[] = {
    {0x00, Room00_vtable}, {0x01, Room01_vtable}, {0x02, Room02_vtable}, {0x03, Room03_vtable},
    {0x04, Room04_vtable}, {0x05, Room05_vtable}, {0x06, Room06_vtable}, {0x07, Room07_vtable},
    {0x08, Room08_vtable}, {0x09, Room09_vtable}, {0x0A, Room0A_vtable}, {0x0B, Room0B_vtable},
    {0x0C, Room0C_vtable}, {0x0D, Room0D_vtable}, {0x0E, Room0E_vtable}, {0x0F, Room0F_vtable},
    {0x10, Room10_vtable}, {0x11, Room11_vtable}, {0x12, Room12_vtable}, {0x13, Room13_vtable},
    {0x14, Room14_vtable}, {0x15, Room15_vtable}, {0x16, Room16_vtable}, {0x17, Room17_vtable},
    {0x18, Room18_vtable}, {0x19, Room19_vtable}, {0x1A, Room1A_vtable}, {0x1B, Room1B_vtable},
    {0x1C, Room1C_vtable}, {0x1D, Room1D_vtable}, {0x1E, Room1E_vtable}, {0x1F, Room1F_vtable},
    {0x20, Room20_vtable}, {0x21, Room21_vtable}, {0x22, Room22_vtable}, {0x23, Room23_vtable},
    {0x24, Room24_vtable}, {0x25, Room25_vtable}, {0x26, Room26_vtable}, {0x27, Room27_vtable},
    {0x28, Room28_vtable}, {0x29, Room29_vtable}, {0x2A, Room2A_vtable}, {0x2B, Room2B_vtable},
    {0x2C, Room2C_vtable}, {0x2D, Room2D_vtable}, {0x2E, Room2E_vtable}, {0x2F, Room2F_vtable},
    {0x30, Room30_vtable}, {0x31, Room31_vtable}, {0x32, Room32_vtable}, {0x33, Room33_vtable},
    {0x34, Room34_vtable}, {0x35, Room35_vtable}, {0x36, Room36_vtable}, {0x37, Room37_vtable},
    {0x40, Room40_vtable}, {0x41, Room41_vtable}, {0x42, Room42_vtable}, {0x43, Room43_vtable},
    {0x44, Room44_vtable}, {0x45, Room45_vtable}, {0x46, Room46_vtable}, {0x47, Room47_vtable},
    {0x48, Room48_vtable}, {0x49, Room49_vtable}, {0x4A, Room4A_vtable}, {0x4B, Room4B_vtable},
    {0x4C, Room4C_vtable}, {0x4D, Room4D_vtable}, {0x4E, Room4E_vtable}, {0x4F, Room4F_vtable},
    {0x50, Room50_vtable}, {0x51, Room51_vtable}, {0x52, Room52_vtable}, {0x53, Room53_vtable},
    {0x54, Room54_vtable}, {0x55, Room55_vtable}, {0x56, Room56_vtable}, {0x57, Room57_vtable},
    {0x58, Room58_vtable}, {0x59, Room59_vtable}, {0x5A, Room5A_vtable}, {0x5B, Room5B_vtable},
    {0x5C, Room5C_vtable}, {0x5D, Room5D_vtable}, {0x5E, Room5E_vtable}, {0x5F, Room5F_vtable},
    {0x60, Room60_vtable}, {0x61, Room61_vtable}, {0x62, Room62_vtable}, {0x63, Room63_vtable},
    {0x64, Room64_vtable}, {0x65, Room65_vtable}, {0x66, Room66_vtable}, {0x67, Room67_vtable},
    {0x68, Room68_vtable}, {0x69, Room69_vtable}, {0x6A, Room6A_vtable}, {0x6B, Room6B_vtable},
    {0x6C, Room6C_vtable}, {0x6D, Room6D_vtable}, {0x6E, Room6E_vtable}, {0x6F, Room6F_vtable},
    {0x70, Room70_vtable}, {0x80, Room80_vtable}, {0x81, Room81_vtable}, {0x82, Room82_vtable},
    {0x83, Room83_vtable}, {0x84, Room84_vtable}, {0x85, Room85_vtable}, {0x86, Room86_vtable},
    {0x87, Room87_vtable}, {0x88, Room88_vtable}, {0x8A, Room8A_vtable}, {0x8C, Room8C_vtable},
    {0x8D, Room8D_vtable}, {0x8E, Room8E_vtable}, {0x8F, Room8F_vtable}, {0x91, Room91_vtable},
    {0x92, Room92_vtable}, {0x93, Room93_vtable}, {0x94, Room94_vtable}, {0x95, Room95_vtable},
    {0x96, Room96_vtable}, {0x97, Room97_vtable}, {0x98, Room98_vtable}, {0x99, Room99_vtable},
    {0x9A, Room9A_vtable}, {0x9B, Room9B_vtable}, {0xC0, RoomC0_vtable}, {0xC1, RoomC1_vtable},
    {0xC2, RoomC2_vtable}, {0xC3, RoomC3_vtable}, {0xC4, RoomC4_vtable}, {0xC5, RoomC5_vtable},
    {0xC6, RoomC6_vtable}, {0xC7, RoomC7_vtable}, {0xC8, RoomC8_vtable}, {0xD0, RoomD0_vtable},
    {0xD1, RoomD1_vtable}, {0xD2, RoomD2_vtable}, {0xD3, RoomD3_vtable}, {0xD4, RoomD4_vtable},
    {0xD5, RoomD5_vtable}, {0xD6, RoomD6_vtable}, {0xD7, RoomD7_vtable}, {0xD8, RoomD8_vtable},
    {0xD9, RoomD9_vtable}, {0xE0, RoomE0_vtable}, {0xE1, RoomE1_vtable}, {0xE2, RoomE2_vtable},
    {0xE3, RoomE3_vtable}, {0xE4, RoomE4_vtable}, {0xE5, RoomE5_vtable}, {0xE6, RoomE6_vtable},
    {0xE7, RoomE7_vtable}, {0xE8, RoomE8_vtable}, {0xE9, RoomE9_vtable}, {0x100, Room100_vtable},
    {0x101, Room101_vtable}, {0x102, Room102_vtable}, {0x103, Room103_vtable}, {0x104, Room104_vtable},
    {0x105, Room105_vtable}, {0x106, Room106_vtable}, {0x107, Room107_vtable}, {0x108, Room108_vtable},
    {0x109, Room109_vtable}, {0x10A, Room10A_vtable}, {0x10B, Room10B_vtable},
};

extern f32 D_00412900;
extern f32 D_00412904;
extern f32 D_00412908;
void Cutscene_Set38(u8 *self, u32 a);
s32 Cutscene_Get28(u8 *self);
void Cutscene_PushC(u8 *self, u32 a);

void Cutscene_SetNoEnd(u8 *self);
s32 Cutscene_EntryDone(u8 *self);
s32 Cutscene_Get206(u8 *self, s32 i);
s32 Cutscene_Get216(u8 *self, s32 i);
void Cutscene_Mark82(u8 *self, s32 i);
void Cutscene_Mark83(u8 *self, s32 i);
void Cutscene_SetScript(u8 *self, u32 a, u32 b);

extern u8 D_00412910[], D_00412914[], D_00412918[];
#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

s32 Cutscene_Call8(void *self);
void Cutscene_Destroy(u8 *p);

/* install the room handlers (+0x120: 0x110 4-byte handler objects; each gets its room's
 * vtable) */
/* 0x00209850 */
void Events_InstallRooms(u8 *ev) {
    s32 i;

    for (i = 0; i < (s32)(sizeof(sRooms) / sizeof(sRooms[0])); i++) {
        void ***h = RoomHandler_new(4, ev + 0x120 + sRooms[i].room * 4);

        if (h != NULL) {
            *h = sRooms[i].vtbl;
        }
    }
    AT(ev, 0x704, s32) = 0;
    AT(ev, 0x11F2, u8) = 0;
}

/* placement new */
/* 0x002A8970 */
void *RoomHandler_new(u32 size, void *place) {
    return place;
}

/* 0x002C94E0 */
void Cutscene_Set38(u8 *self, u32 a) {
    *(u32 *)(self + 0x14) = a;
    *(f32 *)(self + 0x2A0) = D_00412900;
    *(f32 *)(self + 0x2A4) = D_00412904;
    *(f32 *)(self + 0x2A8) = D_00412908;
    self[0x204] = 0;
}

/* 0x002C95D0 */
s32 Cutscene_Get28(u8 *self) {
    u16 *p = *(u16 **)(self + 0x18);

    if (p == NULL) {
        return -1;
    }
    return *p;
}

/* 0x002C95F0 */
void Cutscene_PushC(u8 *self, u32 a) {
    *(u32 *)(self + 0x10) = *(u32 *)(self + 0xC);
    *(u32 *)(self + 0xC) = a;
}

/* 0x002C9620 */
void Cutscene_SetNoEnd(u8 *self) {
    self[0x205] = 1;
}

/* Entries of 12 bytes at +0x6C, current index at +0x64. */
/* 0x002C9630 */
s32 Cutscene_EntryDone(u8 *self) {
    s32 i = *(s32 *)(self + 0x64);

    return *(s32 *)(self + 0x6C + i * 12) == 3;
}

/* Returns an s8. */
/* 0x002C9660 */
s32 Cutscene_Get206(u8 *self, s32 i) {
    return (s8)(((s8 *)self)[0x206 + i] - 1);
}

/* Returns an s16. */
/* 0x002C9680 */
s32 Cutscene_Get216(u8 *self, s32 i) {
    return *(s16 *)(self + 0x216 + i * 2);
}

/* 0x002C96F0 */
void Cutscene_Mark82(u8 *self, s32 i) {
    self[0x82 + i * 12] = 1;
}

/* 0x002C9710 */
void Cutscene_Mark83(u8 *self, s32 i) {
    self[0x83 + i * 12] = 1;
}

/* 0x002CC830 */
void Cutscene_SetScript(u8 *self, u32 a, u32 b) {
    *(u32 *)(self + 0x18) = a;
    *(u32 *)(self + 0x1C) = b;
}

/* tail call to virtual slot 0x8 */
/* 0x002CC840 */
s32 Cutscene_Call8(void *self) {
    return VCALL(self, 0x8, s32 (*)(void *))(self);
}

/* 0x002CC850 */
void Cutscene_Destroy(u8 *p) {
    s32 i;

    F(p, 0x18, u32) = 0;
    F(p, 0x20, u32) = 0;
    F(p, 0x1C, u32) = 0;
    for (i = 0; i < 32; i++) {
        u8 *e = p + 0x80 + i * 12;
        e[0] = 0;
        e[1] = 0;
        e[4] = 0;
        F(e, 8, u32) = 0;
    }
    for (i = 0x44; i <= 0x60; i += 4) {
        F(p, i, u32) = 0;
    }
    F(p, 0x2A0, f32) = *(f32 *)D_00412910;
    F(p, 0x2A4, f32) = *(f32 *)D_00412914;
    F(p, 0x2A8, f32) = *(f32 *)D_00412918;
    p[0x204] = 0;
    p[0x4] = 0;
}

/* (gEvents) +0xC the room's event script (PAC section 2) */
/* 0x00209840 */
void Events_SetScript(u8 *ev, void *script) {
    AT(ev, 0x10, void *) = script;
}

#include "progress.h"
#include "task.h"

extern u8 D_003D6230[], D_003D6240[];   /* built-in scripts run after phases 2 and 1 */

#define EV_ROOM(ev) ((VObject *)((ev) + 0x120 + AT(ev, 0x560, s32) * 4))

static void run_script(u8 *ev, u8 *script) {
    Script_Start(ev, script);
    AT(ev, 0x700, u8) = 0;
    while (*AT(ev, 0x4, u8 *) != 0xFF) {
        if (*AT(ev, 0x4, u8 *) >= 0xF0) {
            Script_RunControl(ev);
        } else {
            EventCmd_Run((VObject *)ev);
        }
    }
}

/* run the current room's script for `phase` (0: entering the room: note it as visited and
 * reset the event state), then the built-in script of phases 1 and 2. Progress flag 0x26
 * suppresses all room scripts but phase 3's. */
/* 0x00209390 */
void Events_RunPhase(u8 *ev, u8 phase) {
    struct {
        s32 a, b, c, d;
        u8 e, f, g, h;
        s16 i;
    } ctx;
    u8 *script = NULL;
    u8 *builtin;
    s32 i;

    ctx.c = 0;
    ctx.d = 0;
    ctx.a = 0;
    ctx.h = 0xFF;
    ctx.b = 0;
    ctx.e = 0;
    ctx.f = 0;
    ctx.g = 0;
    ctx.i = 0;
    AT(ev, 0x6FC, void *) = &ctx;
    switch (phase) {
    case 0: {
        Progress *p = gProgress;
        VObject *o;

        AT(ev, 0x560, s32) = VCALL(p, 0xC, s32 (*)(Progress *))(p);
        AT(p, 0xDC + (AT(ev, 0x560, s32) >> 5) * 4, u32) |= 1 << (AT(ev, 0x560, s32) & 0x1F);
        VCALL(gSound, 0x7C, void (*)(VObject *, s32, s32))(gSound, 1, 0);
        for (i = 0; i < 32; i++) {
            AT(ev, 0x810 + i * 4, s32) = 0;
        }
        AT(ev, 0x890, s32) = 0;
        for (i = 0; i < 8; i++) {
            AT(ev, 0x894 + i * 0x14, s32) = -1;
        }
        Task_Close((Task *)(ev + 0x708));
        for (i = 0; i < 17; i++) {
            AT(ev, 0x564 + i * 0x18, s32) = 0;
        }
        VCALL(gSubScreen, 0x28, void (*)(VObject *))(gSubScreen);
        VCALL(gMusic, 0x8, void (*)(VObject *, s32, s32, s32, f32))(gMusic, 0xFF, 0, 0, 1.0f);
        AT(ev, 0x80C, s32) = 0;
        o = EV_ROOM(ev);
        script = VCALL(o, 0xC, u8 *(*)(VObject *))(o);
        break;
    }
    case 1: {
        VObject *o;

        AT(ev, 0x704, s32)++;
        for (i = 0; i < 32; i++) {
            AT(ev, 0xBF4 + i * 0x30, u8) = 0;
        }
        o = EV_ROOM(ev);
        script = VCALL(o, 0x10, u8 *(*)(VObject *))(o);
        break;
    }
    case 2:
        script = VCALL(EV_ROOM(ev), 0x14, u8 *(*)(VObject *))(EV_ROOM(ev));
        break;
    case 3:
        script = VCALL(EV_ROOM(ev), 0x18, u8 *(*)(VObject *))(EV_ROOM(ev));
        break;
    case 4:
        script = VCALL(EV_ROOM(ev), 0x1C, u8 *(*)(VObject *))(EV_ROOM(ev));
        break;
    case 5:
        Progress_ClearFlag(gProgress, 0x22);
        script = VCALL(EV_ROOM(ev), 0x20, u8 *(*)(VObject *))(EV_ROOM(ev));
        break;
    }
    if (Progress_TestFlag(gProgress, 0x26) && phase != 3) {
        script = NULL;
    }
    if (script != NULL) {
        run_script(ev, script);
    }
    if (phase == 2) {
        builtin = D_003D6230;
    } else if (phase == 1) {
        builtin = D_003D6240;
    } else {
        builtin = NULL;
    }
    if (builtin != NULL) {
        run_script(ev, builtin);
    }
}

extern u8 *D_003D6760[];   /* the shared action scripts (actions 0x80..) */

/* +0xE0 start character slot `slot`'s action `act`: a shared script (0x80..) or the room's
 * (its handler +0x24), through +0xE4 */
/* 0x001FBD70 */
void Events_StartAction(VObject *ev, s32 slot, s32 act) {
    u8 *script;

    act &= 0xFF;
    if (act & 0x80) {
        script = D_003D6760[act];
    } else {
        VObject *room = (VObject *)((u8 *)ev + 0x120 + AT(ev, 0x560, s32) * 4);

        script = VCALL(room, 0x24, u8 *(*)(VObject *, s32))(room, act);
    }
    VCALL(ev, 0xE4, void (*)(VObject *, s32, u8 *))(ev, slot, script);
}

/* +0xE8 the middle of the event area room entry `k` leads to (the room table +0x48 of the
 * current room; its line +0x10 -> +0x30 halved, the height +0x14), w 1, into `out`. 0: no
 * areas, or the entry has none */
/* 0x001FBC00 */
s32 Events_AreaMiddle(u8 *ev, s32 k, f32 *out) {
    u32 *tbl = AT(ev, 0x10, u32 *);
    u32 area;
    u8 *e;

    if (tbl == NULL) {
        return 0;
    }
    area = VCALL(gRooms, 0x48, u32 (*)(VObject *, s32, s32))(
               gRooms, VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress), k) & 0xFFFF;
    if (area == 0xFFFF) {
        return 0;
    }
    e = (u8 *)(tbl + tbl[area]);
    out[0] = (AT(e, 0x10, f32) + AT(e, 0x30, f32)) / 2.0f;
    out[1] = AT(e, 0x14, f32);
    out[2] = (AT(e, 0x18, f32) + AT(e, 0x38, f32)) / 2.0f;
    out[3] = 1.0f;
    return 1;
}

/* +0xE4 give character slot `slot` the script `script` (its context at +0x564 + (slot + 1) *
 * 0x18: the character, the pc, its id at +0x13); NULL: nothing */
/* 0x001FBCF0 */
void Events_GiveScript(VObject *ev, u8 slot, u8 *script) {
    u8 *c;
    u8 *ctx;

    if (script == NULL) {
        return;
    }
    c = (u8 *)gCharacters[slot];
    ctx = (u8 *)ev + 0x564 + (u8)(slot + 1) * 0x18;
    AT(ctx, 0x0, u8 *) = c;
    AT(ctx, 0x4, u8 *) = NULL;
    AT(ctx, 0x10, u8) = 0;
    AT(ctx, 0x8, s32) = 0;
    AT(ctx, 0x11, u8) = 0;
    AT(ctx, 0xC, s32) = 0;
    AT(ctx, 0x12, u8) = 0;
    AT(ctx, 0x13, u8) = 0xFF;
    AT(ctx, 0x14, s16) = 0;
    AT(ctx, 0x13, u8) = AT(c, 0x153C, u8);
    AT(ctx, 0x4, u8 *) = script;
}

extern u8 D_003D6C10[];   /* the script run instead while progress flag 0x26 is set */

/* run the room's script for character `c` entering it (the room handler's +0x30; nothing if it
 * has none or c isn't in this room), in its own context, then resume the current script */
/* 0x00209060 */
void Events_CharEnter(VObject *ev, u8 *c) {
    Progress *p;
    u8 *pc;
    u8 depth;
    void *saved;
    VObject *room;
    struct {
        u8 *c;
        s32 b, cc, d;
        u8 e, f, g, h;
        s16 i;
    } ctx;

    if (AT(ev, 0x560, s32) != AT(c, 0x30, s32)) {
        return;
    }
    ROOMLOG("entry script: char %p (id %d) entering room %d at (%.1f %.1f %.1f) tri %d door %d",
            (void *)c, AT(c, 0x153C, u8), AT(ev, 0x560, s32), AT(c, 0x10, f32), AT(c, 0x14, f32),
            AT(c, 0x18, f32), AT(c, 0x34, s32), AT(c, 0x14D4, u8));
    p = gProgress;
    room = (VObject *)((u8 *)ev + 0x120 + AT(ev, 0x560, s32) * 4);
    if (!Progress_TestFlag(p, 0x26) && VCALL(room, 0x30, u8 *(*)(VObject *))(room) == NULL) {
        return;
    }
    pc = AT(ev, 0x4, u8 *);
    depth = AT(ev, 0x8, u8);
    saved = AT(ev, 0x6FC, void *);
    if (Progress_TestFlag(p, 0x26)) {
        AT(ev, 0x4, u8 *) = D_003D6C10;
    } else {
        room = (VObject *)((u8 *)ev + 0x120 + AT(ev, 0x560, s32) * 4);
        AT(ev, 0x4, u8 *) = VCALL(room, 0x30, u8 *(*)(VObject *))(room);
    }
    AT(ev, 0x8, u8) = 0;
    ctx.c = c;
    ctx.b = 0;
    ctx.e = 0;
    ctx.cc = 0;
    ctx.d = 0;
    ctx.f = 0;
    ctx.g = 0;
    ctx.h = 0xFF;
    ctx.i = 0;
    ctx.h = AT(c, 0x153C, u8);
    AT(ev, 0x6FC, void *) = &ctx;
    AT(ev, 0x700, u8) = 0;
    while (*AT(ev, 0x4, u8 *) != 0xFF) {
        if (*AT(ev, 0x4, u8 *) >= 0xF0) {
            Script_RunControl((u8 *)ev);
        } else {
            Event_RunScript(ev);
        }
    }
    AT(ev, 0x4, u8 *) = pc;
    AT(ev, 0x8, u8) = depth;
    AT(ev, 0x6FC, void *) = saved;
    AT(ev, 0x701, u8) = 0;
}

/* +0xD8 whether `pos` (on nav triangle `tri`) is inside area `area` of the room's event data
 * (+0x10: area offsets; a type 1 area is 4 corners (x, z at +0x10.., 0x10 apart) and a
 * height range +0x24..+0x8) */
/* 0x001FC210 */
s32 Events_InArea(VObject *ev, const f32 *pos, s32 area, s32 tri) {
    f32 at[4] __attribute__((aligned(16)));
    u8 *data = AT(ev, 0x10, u8 *);
    u8 *a;
    s32 inside = 0;
    s32 k;
    f32 y;

    if (data == NULL) {
        return 0;
    }
    a = data + AT(data, area * 4, u32) * 4;
    if (AT(a, 0, s32) != 1) {
        return 0;
    }
    for (k = 0; k < 4; k++) {
        const f32 *p0 = (f32 *)(a + 0x10 + k * 0x10);
        const f32 *p1 = (f32 *)(a + 0x10 + ((k + 1) & 3) * 0x10);

        if ((pos[0] - p0[0]) * (p1[2] - p0[2]) - (pos[2] - p0[2]) * (p1[0] - p0[0]) <= 0.0f) {
            inside++;
        }
    }
    sceVu0CopyVector(at, (f32 *)pos);
    if (VCALL(gNavMesh, 0x10, s32 (*)(VObject *, s32, f32 *))((VObject *)gNavMesh, tri, at) == 3) {
        VCALL(gNavMesh, 0x14, void (*)(VObject *, s32, f32 *))((VObject *)gNavMesh, tri, at);
    }
    if (inside != 4) {
        return 0;
    }
    y = 1.0f + at[1];
    if (y < AT(a, 0x24, f32) || !(y <= AT(a, 0x8, f32))) {
        return 0;
    }
    return 1;
}

/* +0x14 whether character c is inside area `area` by at least its radius (+0xC8; none: the
 * plain position test +0xD8) */
/* 0x001FC030 */
s32 Events_CharInArea(VObject *ev, u8 *c, s32 area) {
    f32 at[4] __attribute__((aligned(16)));
    f32 r = AT(c, 0xC8, f32);
    const f32 *pos = (f32 *)(c + 0x10);
    u8 *data;
    u8 *a;
    s32 inside = 0;
    s32 k;
    f32 y;

    if (r <= 0.0f) {
        return VCALL(ev, 0xD8, s32 (*)(VObject *, f32 *, s32, s32))(ev, (f32 *)(c + 0x10), area, AT(c, 0x34, s32));
    }
    data = AT(ev, 0x10, u8 *);
#ifdef HG_NATIVE
    if (data == NULL) {   /* a room without areas (the PS2 reads low memory: zeros) */
        return 0;
    }
#endif
    a = data + AT(data, area * 4, u32) * 4;
    if (AT(a, 0, s32) != 1) {
        return 0;
    }
    for (k = 0; k < 4; k++) {
        const f32 *p0 = (f32 *)(a + 0x10 + k * 0x10);
        const f32 *p1 = (f32 *)(a + 0x10 + ((k + 1) & 3) * 0x10);
        f32 ex = p1[0] - p0[0];
        f32 ez = p1[2] - p0[2];
        f32 cr = (pos[0] - p0[0]) * ez - (pos[2] - p0[2]) * ex;

        if (cr <= 0.0f) {
            f32 len = __builtin_sqrtf(ex * ex + ez * ez);

            if (cr <= 0.0f) {
                cr = -cr;
            }
            if (!(cr < r * len)) {
                inside++;
            }
        }
    }
    sceVu0CopyVector(at, (f32 *)(c + 0x10));
    if (VCALL(gNavMesh, 0x10, s32 (*)(VObject *, s32, f32 *))((VObject *)gNavMesh, AT(c, 0x34, s32), at) == 3) {
        VCALL(gNavMesh, 0x14, void (*)(VObject *, s32, f32 *))((VObject *)gNavMesh, AT(c, 0x34, s32), at);
    }
    if (inside != 4) {
        return 0;
    }
    y = 1.0f + at[1];
    if (y < AT(a, 0x24, f32) || !(y <= AT(a, 0x8, f32))) {
        return 0;
    }
    return 1;
}

/* each frame: the characters' running scripts (17 slots at +0x564, each the context a script
 * runs with: the character (-1: none needed), pc, depth +0x10, frame count +0x14), until one
 * waits (+0x700). A character that is no longer in a special state (+0xE0 clear, action not 5)
 * loses its script, and the message it owns (+0x80C) is closed. Then the message window. */
/* 0x00209210 */
void Events_RunCharScripts(VObject *ev) {
    u8 *e = (u8 *)ev;
    s32 i;

    AT(e, 0x11F3, u8) = 0;
    for (i = 0; i < 17; i++) {
        u8 *s = e + 0x564 + i * 0x18;
        u8 *c = AT(s, 0x0, u8 *);

        if (c == NULL) {
            continue;
        }
        if (c != (u8 *)-1 && AT(c, 0xE0, u8) == 0 && AT(c, 0x14E8, s32) != 5) {
            if (AT(e, 0x718, u8) != 0 && AT(e, 0x80C, u8 *) == c) {
                Task_Close((Task *)(e + 0x708));
                AT(e, 0x80C, u8 *) = NULL;
            }
            AT(s, 0x0, u8 *) = NULL;
            continue;
        }
        AT(s, 0x14, u16)++;
        AT(e, 0x4, u8 *) = AT(s, 0x4, u8 *);
        AT(e, 0x8, u8) = AT(s, 0x10, u8);
        AT(e, 0x6FC, void *) = s;
        AT(e, 0x700, u8) = 0;
        while (*AT(e, 0x4, u8 *) != 0xFF) {
            if (*AT(e, 0x4, u8 *) >= 0xF0) {
                Script_RunControl(e);
            } else {
                Event_RunScript(ev);
            }
            if (AT(e, 0x700, u8) != 0) {
                break;
            }
        }
        if (AT(e, 0x6FC, void *) == NULL) {
            AT(s, 0x0, u8 *) = NULL;
            continue;
        }
        AT(s, 0x4, u8 *) = AT(e, 0x4, u8 *);
        AT(s, 0x10, u8) = AT(e, 0x8, u8);
    }
    if (AT(e, 0x718, u8) != 0) {
        Task_Update((Task *)(e + 0x708));
    }
}

/* room handler default: no script */
/* 0x00209800 */
u8 *RoomBase_Phase3Script(VObject *room) {
    return NULL;
}

/* vtable +0x50 (second base): a scene is playing (+0x11F3, cleared each frame by the
 * character script runner) */
/* 0x001FBA10 */
s32 Events_ScenePlaying(u8 *ev) {
    return AT(ev, 0x11F3, u8);
}

/* +0x40 (second base): close the message window if it shows message `id` (0xFFFF: any) */
/* 0x001FB1F0 */
void Events_CloseMessage(u8 *ev, u32 id) {
    id &= 0xFFFF;
    if (id == 0xFFFF || id == AT(ev, 0x71A, u16)) {
        Task_Close((Task *)(ev + 0x708));
    }
}

/* +0x34 script variable n (+0x810) */
/* 0x00209020 */
s32 Events_GetVar(u8 *ev, s32 n) {
    return AT(ev, 0x810 + (n & 0xFF) * 4, s32);
}

/* +0x30 set script variable n (+0x810) */
/* 0x00209040 */
void Events_SetVar(u8 *ev, s32 n, s32 v) {
    AT(ev, 0x810 + (n & 0xFF) * 4, s32) = v;
}

/* clear event bit n (+0x890) */
/* 0x001FB170 */
void Events_ClearBit(u8 *ev, s32 n) {
    AT(ev, 0x890, u32) &= ~(1u << (n & 0x1F));   /* (sllv: the low 5 bits) */
}

/* set event bit `n` (+0x890) */
/* 0x001FB190 */
void Events_SetBit(u8 *ev, s32 n) {
    AT(ev, 0x890, u32) |= 1u << (n & 0xFF);
}

/* event bit `n` (+0x890) set */
/* 0x001FB1B0 */
s32 Events_TestBit(u8 *ev, s32 n) {
    return (AT(ev, 0x890, u32) & (1u << (n & 0xFF))) != 0;
}

/* draw the event's screen fade (+0x20) in renderer layer `layer` */
/* 0x001FBA20 */
void Events_DrawFade(u8 *ev, s32 layer) {
#ifdef HG_NATIVE
    extern void glr_overlay(u32 rgba);

    (void)layer;
    glr_overlay(AT(ev, 0x20 + 0xF8, u32));
#else
    VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, void *))(gRenderer, ev + 0x20, layer, NULL);
#endif
}

/* set the event's screen fade (+0x20) colour (a second entry point inside Events_DrawFade's block) */
/* 0x001FBA50 */
void Events_SetFadeColour(u8 *ev, u32 r, u32 g, u32 b, u32 a) {
    Overlay_SetColor(ev + 0x20, (u32)(u8)r << 24 | (u32)(u8)g << 16 | (u32)(u8)b << 8 | (u8)a);
}

/* the screen fade: colour `rgba`, drawn in renderer layer `layer` */
/* 0x001FBA80 */
void Events_Fade(u8 *ev, u32 rgba, s32 layer) {
    Overlay_SetColor(ev + 0x20, rgba);
#ifdef HG_NATIVE
    {
        extern void glr_overlay(u32 rgba);

        (void)layer;
        glr_overlay(AT(ev, 0x20 + 0xF8, u32));
    }
#else
    VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, void *))(gRenderer, ev + 0x20, layer, NULL);
#endif
}

/* the cutscene director's cue moved on this frame (+0xBE4 against the one before, +0xBE8) */
/* 0x001FB1D0 */
s32 Events_CueMoved(u8 *ev) {
    return AT(ev, 0xBE8, s32) != AT(ev, 0xBE4, s32);
}

/* open message `id` in the message window */
/* 0x001FB230 */
void Events_OpenMessage(u8 *ev, s32 id) {
    Task_Open((Task *)(ev + 0x708), id);
}

/* +0x703 */
/* 0x001FB240 */
u8 Events_Get703(u8 *ev) {
    return AT(ev, 0x703, u8);
}

/* text on screen for one frame (not while progress flag 8): the message window shows it and
 * closes */
/* 0x001FB450 */
void Events_ShowText(u8 *ev, s32 x, s32 y, s32 color, u8 *text, s32 alpha, s32 layer, s32 glyphW, s32 glyphH) {
    if ((Progress_TestFlag((Progress *)gProgress, 8) & 0xFF) != 0) {
        return;
    }
    Task_ShowText((Task *)(ev + 0x708), x, y, color, text, alpha, layer, glyphW, glyphH);
    Task_Close((Task *)(ev + 0x708));
}

extern u8 D_003D6B20[][3];   /* per progress +0xBC: three ids (0x100 + n) */

/* id 0x100..0x105's place (0..2) in the row progress +0xBC picks, while progress +0xBB; -1 */
/* 0x001FB560 */
s32 Events_ThingPlace(u8 *ev, u32 id) {
    s32 i;

    if (id < 0x100 || id >= 0x106 || AT(gProgress, 0xBB, u8) == 0) {
        return -1;
    }
    for (i = 0; i < 3; i++) {
        if (id == D_003D6B20[AT(gProgress, 0xBC, u8)][i] + 0x100u) {
            return i;
        }
    }
    return -1;
}

/* point `i` of the 32 at +0xBF4 (0x30 each: +0 on, +1 flags, +0xC position, w 1, +0x1C / +0x20
 * two values) set */
/* 0x001FB800 */
void Events_SetPoint(u8 *ev, u8 i, const f32 *pos, f32 a, f32 b) {
    u8 *r = ev + i * 0x30;

    r[0xBF4] = 1;
    r[0xBF5] = 0;
    func_0010E5F0((f32 *)(r + 0xC00), pos);
    AT(r, 0xC0C, f32) = 1.0f;
    AT(r, 0xC10, f32) = a;
    AT(r, 0xC14, f32) = b;
}

/* the nearest point (of those on with flag bit 0) to `pos` into `out`; 0 if none */
/* 0x001FB880 */
s32 Events_NearestPoint(u8 *ev, const f32 *pos, f32 *out) {
    f32 p[4] __attribute__((aligned(16)));
    f32 best = 0.0f, d, dx, dy, dz;
    s32 i, k = -1;

    for (i = 0; i < 32; i++) {
        u8 *r = ev + i * 0x30;

        if (r[0xBF4]) {
            func_0010E5F0(p, (f32 *)(r + 0xC00));
            p[3] = 1.0f;
        }
        if (r[0xBF4] && ((r[0xBF4] ? r[0xBF5] : 0xFF) & 1)) {
            dy = pos[1] - p[1];
            dx = pos[0] - p[0];
            dz = pos[2] - p[2];
            d = dy * dy + dx * dx + dz * dz;
            if (k == -1 || !(best <= d)) {
                best = d;
                k = i;
            }
        }
    }
    if (k == -1) {
        return 0;
    }
    if (ev[k * 0x30 + 0xBF4]) {
        func_0010E5F0(out, (f32 *)(ev + k * 0x30 + 0xC00));
        out[3] = 1.0f;
    }
    return 1;
}

/* the room's point `n` (the table +0x10, by the rooms' +0x48 index for the current room):
 * three vectors (+0x10 / +0x20 / +0x30); 0 if none */
/* 0x001FBB10 */
s32 Events_RoomPoint(u8 *ev, s32 n, f32 *a, f32 *b, f32 *c) {
    u32 *tab = AT(ev, 0x10, u32 *);
    u32 i;
    u8 *e;

    if (tab == NULL) {
        return 0;
    }
    i = VCALL(gRooms, 0x48, u32 (*)(VObject *, s32, s32))(
        gRooms, VCALL((VObject *)gProgress, 0xC, s32 (*)(VObject *))((VObject *)gProgress), n) & 0xFFFF;
    if (i == 0xFFFF) {
        return 0;
    }
    e = (u8 *)AT(ev, 0x10, u32 *) + AT(ev, 0x10, u32 *)[i] * 4;
    sceVu0CopyVector(a, (f32 *)(e + 0x10));
    sceVu0CopyVector(b, (f32 *)(e + 0x20));
    sceVu0CopyVector(c, (f32 *)(e + 0x30));
    return 1;
}

/* a room handler's +0x14 phase script (rooms 0x100 / 0x101): none */
/* 0x00209810 */
s32 RoomBase_Phase2Script(void) {
    return 0;
}

/* a room id as the scripts see it: with progress flag 0xAF, rooms 0x40 / 0x41 are 0x70 */
/* 0x001FB520 */
s32 Events_ScriptRoom(void *ev, s32 room) {
    if ((AT(gProgress, 0x30, u32) & 0x8000) && (u32)(room - 0x40) < 2) {
        return 0x70;
    }
    return room;
}

extern void *D_0046BA80[], *D_0046BAA0[];

/* destructor of class D_0046BA80 */
/* 0x0020C120 */
void *Obj46BA80_dtor(void **o, s32 flags) {
    if (o != NULL) {
        o[0] = D_0046BA80;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* the events' base destructor (D_0046BAA0): the global events pointer cleared */
/* 0x0020C170 */
void *EventsBase_dtor(void **o, s32 flags) {
    if (o != NULL) {
        o[0] = D_0046BAA0;
        gEvents = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

extern void *Events_vtable[], *D_0046B3B8[], *Cutscene_vtable[], *D_0046BB20[], *Overlay_vtable[], *Helper469D00_vtable[];

/* +0x8 the events' destructor: its 32 points, the cutscene director (+0x938), the message
 * window (+0x708), the rooms' handlers (+0x120), the fade (+0x20), then its bases */
/* 0x001FB250 */
u8 *Events_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = Events_vtable;
    AT(o, 0xC, void **) = D_0046B3B8;
    func_001002C0(o + 0xBF0, (void * (*)(void *, s32))Obj46BA68_dtor, 0x30, 0x20);
    AT(o, 0x938, void **) = Cutscene_vtable;
    AT(o, 0x938, void **) = D_0046BB20;
    gCutscene = NULL;
    if (AT(o, 0x784, void *) != NULL) {
        Task_dtor(AT(o, 0x784, void *), 1);
        AT(o, 0x784, void *) = NULL;
    }
    func_001002C0(o + 0x120, RoomBase_dtor, 4, 0x110);
    AT(o, 0x20, void **) = Overlay_vtable;
    AT(o, 0x20, void **) = Helper469D00_vtable;
    AT(o, 0xC, void **) = D_0046BAA0;
    gEvents = NULL;
    AT(o, 0x0, void **) = D_0046BA80;
    if ((s16)flags > 0) {
        func_00100490(o);
    }
    return o;
}

/* a new deal of the six rooms' (0x100..0x105) things while progress variable 0x1F is below 4:
 * progress +0xBB counted up, +0xBC a row of D_003D6B20 at random (of 79); the stalker in one
 * of those rooms is sent on (+0x64: room 0x10A from 0x109, else 0x109); then in rooms 0..5 the
 * twelve doors / objects 0x95 + 12 x room are shown (+0x68) where the row puts that room,
 * else hidden (+0x64), and the rooms' +0x90 */
/* 0x001FB5F0 */
void Events_DealThings(void) {
    Progress *g = gProgress;
    u8 *p;
    u8 *c;
    u8 k;
    s32 r, i, j, n;
    VObject *rooms;

    if ((u8)Progress_GetVar(g, 0x1F) >= 4) {
        return;
    }
    p = (u8 *)gProgress;
    AT(p, 0xBB, u8)++;
    AT(p, 0xBC, u8) = (u32)VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) % 0x4F;
    k = AT(p, 0xBC, u8);
    c = (u8 *)gCharSlot2;
    if (c != NULL && AT(c, 0x28, u8) != 0 && AT(c, 0x30, u32) >= 0x100 && AT(c, 0x30, u32) < 0x106) {
        r = VCALL((VObject *)g, 0xC, s32 (*)(VObject *))((VObject *)g);
        VCALL((VObject *)c, 0x64, void (*)(VObject *, s32, s32, s32))((VObject *)c, r == 0x109 ? 0x10A : 0x109, -1, 2);
    }
    rooms = gRooms;
    for (r = 0; r < 6; r++) {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 4; j++) {
                n = 0x95 + r * 0xC + i * 4 + j;
                if (r == D_003D6B20[k][i]) {
                    VCALL(rooms, 0x68, void (*)(VObject *, u32))(rooms, n & 0xFFFF);
                } else {
                    VCALL(rooms, 0x64, void (*)(VObject *, u32))(rooms, n & 0xFFFF);
                }
            }
        }
    }
    VCALL(rooms, 0x90, void (*)(VObject *))(rooms);
}
