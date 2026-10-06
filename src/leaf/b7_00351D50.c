/* Batch 7 leaf functions, 0x00351D50.. : resource-table getters, small state objects. */
#include "common.h"
#include "ptmf.h"
#include "globals.h"

#define B7_W(p, off)  (*(s32 *)((u8 *)(p) + (off)))

extern u8 D_004434D0[], D_00443510[], D_00443560[], D_004435E0[], D_00443680[];
extern void *D_00443708[];
extern u8 D_00443728[], D_00443740[], D_004437C0[];
extern void *D_0047AFBC[];
extern u8 D_00443860[], D_00443870[], D_00443890[], D_00443910[], D_00443AB0[], D_00443B00[];
extern void *D_00443DD0[];
extern u8 D_00443E10[];
extern void *D_00443E00[];
extern void *D_0047AFC8[];
extern u8 D_00443E60[];

void *func_00352B60(void) { return D_00443560; }
void *func_00352B70(void) { return D_004435E0; }
void *func_00352B90(void) { return D_00443680; }
void *func_00352BA0(void *self, s32 i) { return D_00443708[i]; }
void *func_00352BC0(void) { return D_00443728; }
void *func_00352D00(void) { return D_00443740; }
void *func_00352D10(void) { return D_004437C0; }
void *func_00352D30(void *self, s32 i) { return D_0047AFBC[i]; }
void *func_00352D50(void) { return D_00443860; }
void *func_00352E80(void) { return D_00443870; }
void *func_00352E90(void) { return D_00443890; }
void *func_00352EA0(void) { return D_00443910; }
void *func_00352EB0(void) { return D_00443AB0; }
void *func_00352EC0(void) { return D_00443B00; }
void *func_00352ED0(void *self, s32 i) { return D_00443DD0[i]; }
void *func_00352EF0(void) { return D_00443E10; }
void *func_00352F00(void *self, s32 i) { return D_00443E00[i]; }

void func_00358C80(u8 *p, u8 *src) {
    if (src != NULL) {
        B7_W(p, 0x4) = *src;
    }
}

s32 func_00359200(u8 *p) { return B7_W(p, 0x4) >= 0; }
void func_00359210(u8 *p) { B7_W(p, 0x4) = -1; }

void *func_0035AF20(void) { return D_00443E60; }
void *func_0035AF30(void *self, s32 i) { return D_0047AFC8[i]; }
