/* Leaf functions, batch 4 (0x002CC840..): getters, constant returns, constructors. */
#include "common.h"
#include "ptmf.h"
#include "globals.h"
#include "progress.h"

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern u8 D_00412910[], D_00412914[], D_00412918[];
extern u8 D_00412950[], D_004129A0[], D_00412A30[], D_00412B60[], D_00412B90[];
extern void *D_00412E60[];
extern u8 D_00412E90[];
extern void *D_0047ABFC[];
extern u8 D_00412EA0[], D_00412F00[], D_00412FB0[], D_00413160[], D_004131A0[];
extern void *D_00413490[];
extern u8 D_004134C0[];
extern void *D_0047AC00[];
extern u8 D_0045D4A0[], D_0045D4C0[], D_0045D4E0[], D_0045D500[], D_0045D520[], D_0045D540[];
#include "item.h"
extern u8 D_00413550[];
extern u8 D_00413530[], D_004134F0[], D_00413510[], D_004134D0[];
extern u8 D_0046C790[], D_0046BA68[], D_0046ED30[], D_0046DB80[], D_00469D00[], D_0046F350[];
extern u8 D_0046BAA0[], D_0046BA80[], D_0046DB40[], D_0046D770[], D_0046C780[], D_0046D800[];
extern u8 D_0046C3E0[], D_0046A9D0[], D_0046D780[];
extern void *D_00456E00;

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

void *func_002CC9B0(void) { return D_00412950; }
void *func_002CC9C0(void) { return D_004129A0; }
void *func_002CC9D0(void) { return D_00412A30; }
void *func_002CC9E0(void) { return D_00412B60; }
void *func_002CC9F0(void) { return D_00412B90; }
void *func_002CCA00(void *self, s32 i) { return D_00412E60[i]; }
void *func_002CCA20(void) { return D_00412E90; }
void *func_002CCA30(void *self, s32 i) { return D_0047ABFC[i]; }
void *func_002CCAF0(void) { return D_00412EA0; }
void *func_002CCB00(void) { return D_00412F00; }
void *func_002CCB10(void) { return D_00412FB0; }
void *func_002CCB20(void) { return D_00413160; }
void *func_002CCB30(void) { return D_004131A0; }
void *func_002CCB40(void *self, s32 i) { return D_00413490[i]; }
void *func_002CCB60(void) { return D_004134C0; }
void *func_002CCB70(void *self, s32 i) { return D_0047AC00[i]; }

/* gFileLoader->vfunc_0xC(name, arg, 0x4000000, 0) */
#define LOAD(name, arg) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, void *, s32, s32, s32))(gFileLoader, name, arg, 0x4000000, 0)

s32 func_002CCC40(void *self, s32 a) { return LOAD(D_0045D4A0, a); }
s32 func_002CCDB0(void *self, s32 a) { return LOAD(D_0045D4C0, a); }

/* use: unless Progress +0x1C bit 0x80, at spot 9 of room 0x1C: event 0, flag 0x18 */
s32 func_002CCDE0(void) {
    Progress *p = gProgress;
    VObject *ev_mgr;

    if (AT(p, 0x1C, u32) & 0x80) {
        return 0;
    }
    if (!item_room_spot(p, 0x1C, 9)) {
        return 0;
    }
    ev_mgr = gEvents;
    item_event(ev_mgr, 0, 0, gCharPlayer);
    Progress_SetFlag(p, 0x18);
    return 4;
}

extern void func_00261090(void *list, s32 a, s32 b);

/* +0x38: a counter (+0x10) that runs 9000 frames; then it goes (the items' +8 list, func_00261090
   (2, 1)) and Progress +0x84 bit 31 is set */
s32 func_002CCED0(void *o) {
    if (AT(o, 0x10, u32) < 9001) {
        if (AT(o, 0x4, s32) != -1 && (VCALL(o, 0x18, s32 (*)(void *))(o) & 0xFF) == 2 && AT(o, 0x10, u32) != (u32)-1) {
            AT(o, 0x10, u32) += 1;
        }
        return 0;
    }
    func_00261090((u8 *)gSubScreen + 8, 2, 1);
    AT(gProgress, 0x84, u32) |= 0x80000000;
    return 2;
}

s32 func_002CD000(void *self, s32 a) { return LOAD(D_0045D4E0, a); }
/* use: on the altar */
s32 func_002CD030(void *o) {
    return item_offer(gProgress, o);
}

s32 func_002CD1A0(void *self, s32 a) { return LOAD(D_0045D500, a); }
/* use: at spot 5 of room 0x22: flag 0x18, event 5 */
s32 func_002CD1D0(void) {
    Progress *p = gProgress;

    if (!item_room_spot(p, 0x22, 5)) {
        return 0;
    }
    Progress_SetFlag(p, 0x18);
    item_event(gEvents, 0, 5, gCharPlayer);
    return 4;
}

s32 func_002CD310(void *self, s32 a) { return LOAD(D_0045D520, a); }
/* use: at spot 0xC of room 4: event 4, flag 0x18 */
s32 func_002CD340(void) {
    Progress *p = gProgress;

    if (!item_room_spot(p, 4, 0xC)) {
        return 0;
    }
    item_event(gEvents, 0, 4, gCharPlayer);
    Progress_SetFlag(p, 0x18);
    return 4;
}

s32 func_002CD490(void *self, s32 a) { return LOAD(D_0045D540, a); }

s32 func_002CD4C0(void) {
    F(gProgress, 0x84, u32) |= 0x400000;
    return 2;
}

void *func_002CD5F0(void) { return D_00413550; }

void func_002CD600(void *self, s32 id, u32 *out) {
    u32 z;

    switch (id) {
    case 1: z = 0x41266666; break;
    case 3: z = 0xC0EE17C2; break;
    case 0: z = 0xC0C6C49C; break;
    case 2: z = 0x40C93B64; break;
    default: return;
    }
    out[0] = 0;
    out[1] = 0;
    out[2] = z;
}

void func_002CD6A0(void *self, s32 id, u32 *out) {
    u32 x, z;

    switch (id) {
    case 10: case 11: x = 0x3F717C1C; z = 0x413403B0; break;
    case 12: case 13: x = 0x400B2B02; z = 0x4170AB9F; break;
    case 14: x = 0xBFCE4C30; z = 0xC08AF007; break;
    case 15: x = 0x3C95182B; z = 0xBF801A37; break;
    default: return;
    }
    out[0] = x;
    out[1] = 0;
    out[2] = z;
}

void func_002CD750(u8 *p) { p[0x16EE] = 1; }

u32 func_002CEF90(void) { return 0x2C020068; }
f32 func_002CEFC0(void) { return 0x1.3333340000000p-1f /* 0.6 */; }
f32 func_002CEFE0(void) { return 0x1.6666660000000p+0f /* 1.4 */; }
f32 func_002CF000(void) { return 24.0f; }
f32 func_002CF010(void) { return 16.0f; }
f32 func_002CF020(void) { return 20.0f; }
f32 func_002CF030(void) { return 10.0f; }
f32 func_002CF040(void) { return 20.0f; }
f32 func_002CF050(void) { return 0x1.eb851e0000000p-4f /* 0.12 */; }
f32 func_002CF070(void) { return 60.0f; }
f32 func_002CF080(void) { union { u32 u; f32 f; } c = { 0x3E0EFA35 }; return c.f; }
f32 func_002CF0A0(void) { union { u32 u; f32 f; } c = { 0x3D567750 }; return c.f; }

