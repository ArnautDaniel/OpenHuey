/* Leaf functions, batch 4 (0x002CC840..): getters, constant returns, constructors. */
#include "common.h"
#include "ptmf.h"
#include "globals.h"
#include "navmesh.h"

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
extern u8 *gProgress;
extern u8 D_00413550[];
extern u8 D_00413530[], D_004134F0[], D_00413510[], D_004134D0[];
extern u8 D_0046C790[], D_0046BA68[], D_0046ED30[], D_0046DB80[], D_00469D00[], D_0046F350[];
extern u8 D_0046BAA0[], D_0046BA80[], D_0046DB40[], D_0046D770[], D_0046C780[], D_0046D800[];
extern u8 D_0046C3E0[], D_0046A9D0[], D_0046D780[];
extern void *D_00456E00;

void func_002CF0C0(u8 *p, s32 v) { F(p, 0x1660, s32) = v != 0 ? v : 900; }
void func_002CF0E0(u8 *p) { F(p, 0x1660, s32) = 600; }

void *func_002CF140(void) {
    return (F(gProgress, 0x30, u32) & 0x8000) ? D_00413530 : D_004134F0;
}

void *func_002CF180(void) {
    return (F(gProgress, 0x30, u32) & 0x8000) ? D_00413510 : D_004134D0;
}

void func_002CF6F0(u8 *p) {
    F(p, 0x18, u32) = 0;
    F(p, 0x1C, u32) = 0;
    p[0x20] = 0;
}

void *func_002D0FE0(u8 *p) {
    F(p, 0x0, void *) = D_0046BA68;
    p[0x4] = 0;
    return p;
}

void *func_002D1000(u8 *p) {
    gCutscene = (VObject *)p;
    F(p, 0x0, void *) = D_0046ED30;
    return p;
}

void *func_002D1020(u8 *p) {
    F(p, 0x0, void *) = D_0046DB80;
    return p;
}

void *func_002D1040(u8 *p) {
    F(p, 0x0, void *) = D_00469D00;
    F(p, 0x4, s32) = -1;
    F(p, 0x0, void *) = D_0046F350;
    F(p, 0x24, u32) = 0;
    F(p, 0x10, s32) = -1;
    p[0x14] = 0;
    return p;
}

void *func_002D1080(u8 *p) {
    gEvents = (VObject *)p;
    F(p, 0x0, void *) = D_0046BAA0;
    return p;
}

void *func_002D10A0(u8 *p) {
    F(p, 0x0, void *) = D_0046BA80;
    F(p, 0x4, u32) = 0;
    p[0x8] = 0;
    return p;
}

void *func_002D1130(u8 *p) {
    D_00456E00 = p;
    F(p, 0x0, void *) = D_0046DB40;
    F(p, 0x4, u32) = 0;
    F(p, 0x8, u32) = 0;
    return p;
}

void *func_002D11C0(u8 *p) {
    F(p, 0x40, void *) = D_00469D00;
    F(p, 0x44, s32) = -1;
    F(p, 0x40, void *) = D_0046D770;
    F(p, 0xA0, u32) = 0;
    F(p, 0x98, u32) = 0;
    F(p, 0x9C, u32) = 0;
    return p;
}

void *func_002D1260(u8 *p) {
    F(p, 0x80, void *) = D_00469D00;
    F(p, 0x84, s32) = -1;
    F(p, 0x80, void *) = D_0046C780;
    F(p, 0x120, u32) = 0;
    F(p, 0x124, u32) = 0;
    F(p, 0xE4, s32) = -1;
    F(p, 0x128, u32) = 0;
    p[0xE0] = 0;
    F(p, 0xE8, u32) = 0;
    F(p, 0x190, void *) = D_00469D00;
    F(p, 0x194, s32) = -1;
    F(p, 0x190, void *) = D_0046D800;
    return p;
}

void *func_002D12C0(u8 *p) {
    gRooms = (VObject *)p;
    F(p, 0x0, void *) = D_0046C3E0;
    return p;
}

void *func_002D12E0(u8 *p) {
    s32 i;

    gNavMesh = (NavMesh *)p;
    F(p, 0x0, void *) = D_0046A9D0;
    for (i = 0x4; i <= 0x18; i += 4) {
        F(p, i, u32) = 0;
    }
    return p;
}

void *func_002D1320(u8 *p) {
    F(p, 0x0, void *) = D_00469D00;
    F(p, 0x4, s32) = -1;
    F(p, 0x0, void *) = D_0046D780;
    F(p, 0x20, u32) = 0;
    F(p, 0x24, u32) = 0;
    F(p, 0x18, s32) = -1;
    return p;
}
