/* The event script system (SceneGame +0xF6AFB0, D_0044E4D0): runs the rooms' event scripts.
 * +0x120 holds one handler object per room (0x110 rooms); its vtable gives the room's scripts
 * for each phase (+0xC entering, +0x10.. +0x20 other phases). */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"

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

extern void *gCharacters[6];

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

extern VObject *D_0044E570;   /* the nav mesh */

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
    if (VCALL(D_0044E570, 0x10, s32 (*)(VObject *, s32, f32 *))(D_0044E570, tri, at) == 3) {
        VCALL(D_0044E570, 0x14, void (*)(VObject *, s32, f32 *))(D_0044E570, tri, at);
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
    if (VCALL(D_0044E570, 0x10, s32 (*)(VObject *, s32, f32 *))(D_0044E570, AT(c, 0x34, s32), at) == 3) {
        VCALL(D_0044E570, 0x14, void (*)(VObject *, s32, f32 *))(D_0044E570, AT(c, 0x34, s32), at);
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


/* set event bit `n` (+0x890) */
void func_001FB190(u8 *ev, s32 n) {
    AT(ev, 0x890, u32) |= 1u << (n & 0xFF);
}
