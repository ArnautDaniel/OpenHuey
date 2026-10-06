/* Leaf functions, batch 4 (func_002D1360..func_002DCA50). */
#include "common.h"
#include "ptmf.h"
#include "globals.h"
#include "progress.h"
#include "actor.h"

extern u8 D_00414390[];
extern u8 D_004143B0[];
extern u8 D_00414430[];
extern void *D_0047AC18[];
extern void *D_0047AC20[];

void *func_002D2560(void) { return D_00414390; }

void *func_002D2570(void) { return D_004143B0; }

void *func_002D2580(void) { return D_00414430; }

void *func_002D25A0(void *self, s32 i) { return D_0047AC18[i]; }

void *func_002D25C0(void *self, s32 i) { return D_0047AC20[i]; }
