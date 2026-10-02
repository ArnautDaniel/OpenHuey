/* The event script system (SceneGame +0xF6AFB0, D_0044E4D0): runs the rooms' event scripts.
 * +0x120 holds one handler object per room (0x110 rooms); its vtable gives the room's scripts
 * for each phase (+0xC entering, +0x10.. +0x20 other phases). */
#include "common.h"
#include "game.h"

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

/* (D_0044E4D0) +0xC the room's event script (PAC section 2) */
void func_00209840(u8 *ev, void *script) {
    AT(ev, 0x10, void *) = script;
}

#include "progress.h"
#include "task.h"

extern VObject *D_0044E560;   /* the sound driver */
extern VObject *D_0044E988;   /* the item manager */
extern VObject *D_0044E970;
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
        VCALL(D_0044E560, 0x7C, void (*)(VObject *, s32, s32))(D_0044E560, 1, 0);
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
        VCALL(D_0044E988, 0x28, void (*)(VObject *))(D_0044E988);
        VCALL(D_0044E970, 0x8, void (*)(VObject *, s32, s32, s32, f32))(D_0044E970, 0xFF, 0, 0, 1.0f);
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
