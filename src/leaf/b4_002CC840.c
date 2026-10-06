/* Leaf functions, batch 4 (0x002CC840..): getters, constant returns, constructors. */
#include "common.h"
#include "ptmf.h"
#include "globals.h"
#include "progress.h"
#include "actor.h"

extern u8 D_00412950[], D_004129A0[], D_00412A30[], D_00412B60[], D_00412B90[];
extern void *D_00412E60[];
extern u8 D_00412E90[];
extern void *D_0047ABFC[];
extern u8 D_00412EA0[], D_00412F00[], D_00412FB0[], D_00413160[], D_004131A0[];
extern void *D_00413490[];
extern u8 D_004134C0[];
extern void *D_0047AC00[];
#include "item.h"

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
