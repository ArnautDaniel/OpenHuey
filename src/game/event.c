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

extern void *D_0046B4B0[], *D_0046B4F0[], *D_0046B530[], *D_0046B570[], *D_0046B5B0[], *D_0046B5F0[];
extern void *D_0046B630[], *D_0046B670[], *D_0046B6B0[], *D_0046B6F0[], *D_0046B730[], *D_0046B770[];
extern void *D_0046B7B0[], *D_0046B7F0[], *D_0046B830[], *D_0046B870[], *D_0046B8B0[], *D_0046DBC0[];
extern void *D_0046DC00[], *D_0046DC40[], *D_0046DC80[], *D_0046DCC0[], *D_0046DD00[], *D_0046DD40[];
extern void *D_0046DD80[], *D_0046DDC0[], *D_0046DE00[], *D_0046DE40[], *D_0046DE80[], *D_0046DEC0[];
extern void *D_0046DF00[], *D_0046DF40[], *D_0046DF80[], *D_0046DFC0[], *D_0046E000[], *D_0046E040[];
extern void *D_0046E080[], *D_0046E0C0[], *D_0046E100[], *D_0046E140[], *D_0046E180[], *D_0046E1C0[];
extern void *D_0046E200[], *D_0046E240[], *D_0046E280[], *D_0046E2C0[], *D_0046E300[], *D_0046E340[];
extern void *D_0046E380[], *D_0046E3C0[], *D_0046E400[], *D_0046E440[], *D_0046E480[], *D_0046E4C0[];
extern void *D_0046E500[], *D_0046E540[], *D_0046E580[], *D_0046E5C0[], *D_0046E600[], *D_0046E640[];
extern void *D_0046E680[], *D_0046E6C0[], *D_0046E700[], *D_0046E740[], *D_0046E780[], *D_0046E7C0[];
extern void *D_0046E800[], *D_0046E840[], *D_0046E880[], *D_0046E8C0[], *D_0046E900[], *D_0046E940[];
extern void *D_0046E980[], *D_0046E9C0[], *D_0046EA00[], *D_0046EDC0[], *D_0046EE00[], *D_0046F3F0[];
extern void *D_0046FC40[], *D_0046FC80[], *D_0046FCC0[], *D_0046FD00[], *D_0046FD40[], *D_0046FD80[];
extern void *D_0046FDC0[], *D_0046FE00[], *D_0046FE40[], *D_0046FE80[], *D_0046FEC0[], *D_00470DC0[];
extern void *D_00470EB0[], *D_00470EF0[], *D_00470F50[], *D_00471020[], *D_00471210[], *D_00471250[];
extern void *D_00471E60[], *D_00471EA0[], *D_00471EE0[], *D_00471F20[], *D_00471F60[], *D_00471FA0[];
extern void *D_00471FE0[], *D_00473460[], *D_00473C90[], *D_00474520[], *D_00474F40[], *D_004760E0[];
extern void *D_004771C0[], *D_00477200[], *D_00477240[], *D_00477280[], *D_004772C0[], *D_00477300[];
extern void *D_00477340[], *D_00477380[], *D_004773C0[], *D_00477400[], *D_00477440[], *D_00477480[];
extern void *D_004774C0[], *D_00477500[], *D_00477540[], *D_00477580[], *D_00477610[], *D_00477650[];
extern void *D_00477690[], *D_004776D0[], *D_00477710[], *D_00477750[], *D_00478570[], *D_004785B0[];
extern void *D_004785F0[], *D_00478630[], *D_00478670[], *D_004786B0[], *D_004786F0[], *D_00478730[];
extern void *D_00478AA0[], *D_00478B80[], *D_00478C00[], *D_00478C40[], *D_00479340[], *D_00479380[];
extern void *D_004793C0[], *D_00479620[], *D_004798D0[], *D_00479910[], *D_00479950[], *D_00479990[];
extern void *D_00479FB0[], *D_0047A070[], *D_0047A0B0[], *D_0047A0F0[], *D_0047A130[], *D_0047A170[];
extern void *D_0047A1B0[], *D_0047A1F0[], *D_0047A230[], *D_0047A270[], *D_0047A2B0[], *D_0047A450[];
extern void *D_0047A490[], *D_0047A4D0[], *D_0047A510[], *D_0047A550[], *D_0047A590[], *D_0047A5D0[];
extern void *D_0047A610[], *D_0047A650[], *D_0047A690[];

extern void *func_002A8970(u32 size, void *place);   /* placement new */

/* the rooms' handler classes (the other rooms keep the base handler) */
static const struct {
    s16 room;
    void **vtbl;
} sRooms[] = {
    {0x00, D_0046DBC0}, {0x01, D_0046B8B0}, {0x02, D_0046DC00}, {0x03, D_0046DC40},
    {0x04, D_0046DC80}, {0x05, D_0046B870}, {0x06, D_0046DCC0}, {0x07, D_0046B830},
    {0x08, D_0046DD00}, {0x09, D_0046DD40}, {0x0A, D_0046DD80}, {0x0B, D_0046DDC0},
    {0x0C, D_0046DE00}, {0x0D, D_0046DE40}, {0x0E, D_0046DE80}, {0x0F, D_0046DEC0},
    {0x10, D_0046DF00}, {0x11, D_0046DF40}, {0x12, D_0046DF80}, {0x13, D_0046DFC0},
    {0x14, D_0046E000}, {0x15, D_0046E040}, {0x16, D_0046B7F0}, {0x17, D_0046F3F0},
    {0x18, D_0046E080}, {0x19, D_0046E0C0}, {0x1A, D_0046E100}, {0x1B, D_0046E140},
    {0x1C, D_0046E180}, {0x1D, D_0046E1C0}, {0x1E, D_0046E200}, {0x1F, D_0046E240},
    {0x20, D_0046E280}, {0x21, D_0046E2C0}, {0x22, D_0046E300}, {0x23, D_0046E340},
    {0x24, D_0046E380}, {0x25, D_0046E3C0}, {0x26, D_0046E400}, {0x27, D_00470DC0},
    {0x28, D_0046E440}, {0x29, D_0046E480}, {0x2A, D_0046E4C0}, {0x2B, D_0046E500},
    {0x2C, D_00470EB0}, {0x2D, D_0046E540}, {0x2E, D_00470EF0}, {0x2F, D_0046EDC0},
    {0x30, D_0046EE00}, {0x31, D_00473460}, {0x32, D_00473C90}, {0x33, D_00478B80},
    {0x34, D_00478AA0}, {0x35, D_00478C40}, {0x36, D_00479620}, {0x37, D_00479FB0},
    {0x40, D_0046E580}, {0x41, D_0046B7B0}, {0x42, D_0046B770}, {0x43, D_0046E5C0},
    {0x44, D_0046B730}, {0x45, D_0046E600}, {0x46, D_0046FC40}, {0x47, D_00471EA0},
    {0x48, D_00471EE0}, {0x49, D_0046E640}, {0x4A, D_0046E680}, {0x4B, D_0046E6C0},
    {0x4C, D_0046E700}, {0x4D, D_0046B6F0}, {0x4E, D_0046E740}, {0x4F, D_0046E780},
    {0x50, D_0046E7C0}, {0x51, D_0046E800}, {0x52, D_0046E840}, {0x53, D_00471F20},
    {0x54, D_00471F60}, {0x55, D_00471020}, {0x56, D_0046E880}, {0x57, D_0046E8C0},
    {0x58, D_0046E900}, {0x59, D_0046E940}, {0x5A, D_0046E980}, {0x5B, D_0046B6B0},
    {0x5C, D_0046E9C0}, {0x5D, D_0046EA00}, {0x5E, D_0046B670}, {0x5F, D_0046B630},
    {0x60, D_00471FA0}, {0x61, D_00471FE0}, {0x62, D_00471250}, {0x63, D_00471210},
    {0x64, D_0046B5F0}, {0x65, D_0046B5B0}, {0x66, D_00470F50}, {0x67, D_0046B570},
    {0x68, D_00471E60}, {0x69, D_00477540}, {0x6A, D_00477580}, {0x6B, D_00477610},
    {0x6C, D_00477650}, {0x6D, D_00477690}, {0x6E, D_004776D0}, {0x6F, D_00477710},
    {0x70, D_00477750}, {0x80, D_00478570}, {0x81, D_004771C0}, {0x82, D_0046B530},
    {0x83, D_00477200}, {0x84, D_00477240}, {0x85, D_00477280}, {0x86, D_00474520},
    {0x87, D_004772C0}, {0x88, D_00477300}, {0x8A, D_00477340}, {0x8C, D_00477380},
    {0x8D, D_004773C0}, {0x8E, D_00477400}, {0x8F, D_00477440}, {0x91, D_00477480},
    {0x92, D_004774C0}, {0x93, D_004798D0}, {0x94, D_00479910}, {0x95, D_00479950},
    {0x96, D_00479990}, {0x97, D_00477500}, {0x98, D_00478C00}, {0x99, D_00479340},
    {0x9A, D_00479380}, {0x9B, D_004793C0}, {0xC0, D_00474F40}, {0xC1, D_004785B0},
    {0xC2, D_004785F0}, {0xC3, D_00478630}, {0xC4, D_00478670}, {0xC5, D_004786B0},
    {0xC6, D_004786F0}, {0xC7, D_004760E0}, {0xC8, D_00478730}, {0xD0, D_0047A070},
    {0xD1, D_0047A0B0}, {0xD2, D_0047A0F0}, {0xD3, D_0047A130}, {0xD4, D_0047A170},
    {0xD5, D_0047A1B0}, {0xD6, D_0047A1F0}, {0xD7, D_0047A230}, {0xD8, D_0047A270},
    {0xD9, D_0047A2B0}, {0xE0, D_0047A450}, {0xE1, D_0047A490}, {0xE2, D_0047A4D0},
    {0xE3, D_0047A510}, {0xE4, D_0047A550}, {0xE5, D_0047A590}, {0xE6, D_0047A5D0},
    {0xE7, D_0047A610}, {0xE8, D_0047A650}, {0xE9, D_0047A690}, {0x100, D_0046B4F0},
    {0x101, D_0046B4B0}, {0x102, D_0046FC80}, {0x103, D_0046FCC0}, {0x104, D_0046FD00},
    {0x105, D_0046FD40}, {0x106, D_0046FD80}, {0x107, D_0046FDC0}, {0x108, D_0046FE00},
    {0x109, D_0046FE40}, {0x10A, D_0046FE80}, {0x10B, D_0046FEC0},
};

extern f32 D_00412900;
extern f32 D_00412904;
extern f32 D_00412908;
void func_002C94E0(u8 *self, u32 a);
s32 func_002C95D0(u8 *self);
void func_002C95F0(u8 *self, u32 a);

void func_002C9620(u8 *self);
s32 func_002C9630(u8 *self);
s32 func_002C9660(u8 *self, s32 i);
s32 func_002C9680(u8 *self, s32 i);
void func_002C96F0(u8 *self, s32 i);
void func_002C9710(u8 *self, s32 i);
void func_002CC830(u8 *self, u32 a, u32 b);

extern u8 D_00412910[], D_00412914[], D_00412918[];
#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

s32 func_002CC840(void *self);
void func_002CC850(u8 *p);

/* install the room handlers (+0x120: 0x110 4-byte handler objects; each gets its room's
 * vtable) */
void func_00209850(u8 *ev) {
    s32 i;

    for (i = 0; i < (s32)(sizeof(sRooms) / sizeof(sRooms[0])); i++) {
        void ***h = func_002A8970(4, ev + 0x120 + sRooms[i].room * 4);

        if (h != NULL) {
            *h = sRooms[i].vtbl;
        }
    }
    AT(ev, 0x704, s32) = 0;
    AT(ev, 0x11F2, u8) = 0;
}

/* placement new */
void *func_002A8970(u32 size, void *place) {
    return place;
}

void func_002C94E0(u8 *self, u32 a) {
    *(u32 *)(self + 0x14) = a;
    *(f32 *)(self + 0x2A0) = D_00412900;
    *(f32 *)(self + 0x2A4) = D_00412904;
    *(f32 *)(self + 0x2A8) = D_00412908;
    self[0x204] = 0;
}

s32 func_002C95D0(u8 *self) {
    u16 *p = *(u16 **)(self + 0x18);

    if (p == NULL) {
        return -1;
    }
    return *p;
}

void func_002C95F0(u8 *self, u32 a) {
    *(u32 *)(self + 0x10) = *(u32 *)(self + 0xC);
    *(u32 *)(self + 0xC) = a;
}

void func_002C9620(u8 *self) {
    self[0x205] = 1;
}

/* Entries of 12 bytes at +0x6C, current index at +0x64. */
s32 func_002C9630(u8 *self) {
    s32 i = *(s32 *)(self + 0x64);

    return *(s32 *)(self + 0x6C + i * 12) == 3;
}

/* Returns an s8. */
s32 func_002C9660(u8 *self, s32 i) {
    return (s8)(((s8 *)self)[0x206 + i] - 1);
}

/* Returns an s16. */
s32 func_002C9680(u8 *self, s32 i) {
    return *(s16 *)(self + 0x216 + i * 2);
}

void func_002C96F0(u8 *self, s32 i) {
    self[0x82 + i * 12] = 1;
}

void func_002C9710(u8 *self, s32 i) {
    self[0x83 + i * 12] = 1;
}

void func_002CC830(u8 *self, u32 a, u32 b) {
    *(u32 *)(self + 0x18) = a;
    *(u32 *)(self + 0x1C) = b;
}

/* tail call to virtual slot 0x8 */
s32 func_002CC840(void *self) {
    return VCALL(self, 0x8, s32 (*)(void *))(self);
}

void func_002CC850(u8 *p) {
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
void func_00209840(u8 *ev, void *script) {
    AT(ev, 0x10, void *) = script;
}

#include "progress.h"
#include "task.h"

extern u8 D_003D6230[], D_003D6240[];   /* built-in scripts run after phases 2 and 1 */
extern void func_00121890(u8 *ev, u8 *script);   /* start a script */
extern void func_00121730(u8 *ev);   /* a control op (0xF0..) */
extern void func_002029B0(u8 *ev);   /* a command */

#define EV_ROOM(ev) ((VObject *)((ev) + 0x120 + AT(ev, 0x560, s32) * 4))

static void run_script(u8 *ev, u8 *script) {
    func_00121890(ev, script);
    AT(ev, 0x700, u8) = 0;
    while (*AT(ev, 0x4, u8 *) != 0xFF) {
        if (*AT(ev, 0x4, u8 *) >= 0xF0) {
            func_00121730(ev);
        } else {
            func_002029B0(ev);
        }
    }
}

/* run the current room's script for `phase` (0: entering the room: note it as visited and
 * reset the event state), then the built-in script of phases 1 and 2. Progress flag 0x26
 * suppresses all room scripts but phase 3's. */
void func_00209390(u8 *ev, u8 phase) {
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
void func_001FBD70(VObject *ev, s32 slot, s32 act) {
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
s32 func_001FBC00(u8 *ev, s32 k, f32 *out) {
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
void func_001FBCF0(VObject *ev, u8 slot, u8 *script) {
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
extern void func_00201B90(VObject *ev);   /* a character script command */

/* run the room's script for character `c` entering it (the room handler's +0x30; nothing if it
 * has none or c isn't in this room), in its own context, then resume the current script */
void func_00209060(VObject *ev, u8 *c) {
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
            func_00121730((u8 *)ev);
        } else {
            func_00201B90(ev);
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
s32 func_001FC210(VObject *ev, const f32 *pos, s32 area, s32 tri) {
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
s32 func_001FC030(VObject *ev, u8 *c, s32 area) {
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
void func_00209210(VObject *ev) {
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
                func_00121730(e);
            } else {
                func_00201B90(ev);
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
u8 *func_00209800(VObject *room) {
    return NULL;
}

/* vtable +0x50 (second base): a scene is playing (+0x11F3, cleared each frame by the
 * character script runner) */
s32 func_001FBA10(u8 *ev) {
    return AT(ev, 0x11F3, u8);
}

/* +0x40 (second base): close the message window if it shows message `id` (0xFFFF: any) */
void func_001FB1F0(u8 *ev, u32 id) {
    id &= 0xFFFF;
    if (id == 0xFFFF || id == AT(ev, 0x71A, u16)) {
        Task_Close((Task *)(ev + 0x708));
    }
}

/* +0x34 script variable n (+0x810) */
s32 func_00209020(u8 *ev, s32 n) {
    return AT(ev, 0x810 + (n & 0xFF) * 4, s32);
}

/* +0x30 set script variable n (+0x810) */
void func_00209040(u8 *ev, s32 n, s32 v) {
    AT(ev, 0x810 + (n & 0xFF) * 4, s32) = v;
}

/* clear event bit n (+0x890) */
void func_001FB170(u8 *ev, s32 n) {
    AT(ev, 0x890, u32) &= ~(1u << (n & 0x1F));   /* (sllv: the low 5 bits) */
}

/* set event bit `n` (+0x890) */
void func_001FB190(u8 *ev, s32 n) {
    AT(ev, 0x890, u32) |= 1u << (n & 0xFF);
}

/* event bit `n` (+0x890) set */
s32 func_001FB1B0(u8 *ev, s32 n) {
    return (AT(ev, 0x890, u32) & (1u << (n & 0xFF))) != 0;
}

/* draw the event's screen fade (+0x20) in renderer layer `layer` */
void func_001FBA20(u8 *ev, s32 layer) {
#ifdef HG_NATIVE
    extern void glr_overlay(u32 rgba);

    (void)layer;
    glr_overlay(AT(ev, 0x20 + 0xF8, u32));
#else
    VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, void *))(gRenderer, ev + 0x20, layer, NULL);
#endif
}

extern void func_002CF390(void *ov, u32 rgba);

/* set the event's screen fade (+0x20) colour (a second entry point inside func_001FBA20's block) */
void func_001FBA50(u8 *ev, u32 r, u32 g, u32 b, u32 a) {
    func_002CF390(ev + 0x20, (u32)(u8)r << 24 | (u32)(u8)g << 16 | (u32)(u8)b << 8 | (u8)a);
}

/* the screen fade: colour `rgba`, drawn in renderer layer `layer` */
void func_001FBA80(u8 *ev, u32 rgba, s32 layer) {
    func_002CF390(ev + 0x20, rgba);
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
s32 func_001FB1D0(u8 *ev) {
    return AT(ev, 0xBE8, s32) != AT(ev, 0xBE4, s32);
}

/* open message `id` in the message window */
void func_001FB230(u8 *ev, s32 id) {
    Task_Open((Task *)(ev + 0x708), id);
}

/* +0x703 */
u8 func_001FB240(u8 *ev) {
    return AT(ev, 0x703, u8);
}

/* text on screen for one frame (not while progress flag 8): the message window shows it and
 * closes */
void func_001FB450(u8 *ev, s32 x, s32 y, s32 color, u8 *text, s32 alpha, s32 layer, s32 glyphW, s32 glyphH) {
    if ((Progress_TestFlag((Progress *)gProgress, 8) & 0xFF) != 0) {
        return;
    }
    Task_ShowText((Task *)(ev + 0x708), x, y, color, text, alpha, layer, glyphW, glyphH);
    Task_Close((Task *)(ev + 0x708));
}

extern u8 D_003D6B20[][3];   /* per progress +0xBC: three ids (0x100 + n) */

/* id 0x100..0x105's place (0..2) in the row progress +0xBC picks, while progress +0xBB; -1 */
s32 func_001FB560(u8 *ev, u32 id) {
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

extern void func_0010E5F0(f32 *dst, const f32 *src);   /* libvu0: copy x, y, z */

/* point `i` of the 32 at +0xBF4 (0x30 each: +0 on, +1 flags, +0xC position, w 1, +0x1C / +0x20
 * two values) set */
void func_001FB800(u8 *ev, u8 i, const f32 *pos, f32 a, f32 b) {
    u8 *r = ev + i * 0x30;

    r[0xBF4] = 1;
    r[0xBF5] = 0;
    func_0010E5F0((f32 *)(r + 0xC00), pos);
    AT(r, 0xC0C, f32) = 1.0f;
    AT(r, 0xC10, f32) = a;
    AT(r, 0xC14, f32) = b;
}

/* the nearest point (of those on with flag bit 0) to `pos` into `out`; 0 if none */
s32 func_001FB880(u8 *ev, const f32 *pos, f32 *out) {
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
s32 func_001FBB10(u8 *ev, s32 n, f32 *a, f32 *b, f32 *c) {
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
s32 func_00209810(void) {
    return 0;
}

/* a room id as the scripts see it: with progress flag 0xAF, rooms 0x40 / 0x41 are 0x70 */
s32 func_001FB520(void *ev, s32 room) {
    if ((AT(gProgress, 0x30, u32) & 0x8000) && (u32)(room - 0x40) < 2) {
        return 0x70;
    }
    return room;
}

extern void func_00100490(void *p);   /* operator delete */
extern void *D_0046BA80[], *D_0046BAA0[];

/* destructor of class D_0046BA80 */
void *func_0020C120(void **o, s32 flags) {
    if (o != NULL) {
        o[0] = D_0046BA80;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* the events' base destructor (D_0046BAA0): the global events pointer cleared */
void *func_0020C170(void **o, s32 flags) {
    if (o != NULL) {
        o[0] = D_0046BAA0;
        gEvents = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

extern void *D_0046B3A0[], *D_0046B3B8[], *D_0046ED30[], *D_0046BB20[], *D_0046F350[], *D_00469D00[];
extern void *func_001FB3B0(void *, s32);
extern void *func_001FB400(void *, s32);
extern void func_001002C0(void *array, void *(*dtor)(void *, s32), u32 size, u32 n);   /* __destroy_arr */

/* +0x8 the events' destructor: its 32 points, the cutscene director (+0x938), the message
 * window (+0x708), the rooms' handlers (+0x120), the fade (+0x20), then its bases */
u8 *func_001FB250(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_0046B3A0;
    AT(o, 0xC, void **) = D_0046B3B8;
    func_001002C0(o + 0xBF0, func_001FB400, 0x30, 0x20);
    AT(o, 0x938, void **) = D_0046ED30;
    AT(o, 0x938, void **) = D_0046BB20;
    gCutscene = NULL;
    if (AT(o, 0x784, void *) != NULL) {
        Task_dtor(AT(o, 0x784, void *), 1);
        AT(o, 0x784, void *) = NULL;
    }
    func_001002C0(o + 0x120, func_001FB3B0, 4, 0x110);
    AT(o, 0x20, void **) = D_0046F350;
    AT(o, 0x20, void **) = D_00469D00;
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
void func_001FB5F0(void) {
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
