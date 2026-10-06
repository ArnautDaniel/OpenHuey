#include "common.h"
#include "globals.h"
#include "ptmf.h"
#include "progress.h"
#include "actor.h"

extern u8 D_00429E40[];
extern u8 D_00429E60[];
extern u8 D_00429F10[];
extern void *D_0042A0A0[];
extern void *D_0042A0E0[];
extern u8 D_0042A0F0[];
extern u8 D_0042A130[];
extern u8 D_0042B0B0[];
extern u8 D_0042B160[];

void *func_0031E220(void) {
    return D_00429E40;
}

void *func_0031E230(void) {
    return D_00429E60;
}

void *func_0031E240(void) {
    return D_00429F10;
}

void *func_0031E250(void *self, s32 i) {
    return D_0042A0A0[i];
}

void *func_0031E270(void *self, s32 i) {
    return D_0042A0E0[i];
}

void *func_0031E6D0(void) {
    return D_0042A0F0;
}

void *func_0031E6E0(void) {
    return D_0042A130;
}

void *func_00320EB0(void) {
    return D_0042B0B0;
}

void *func_00320EC0(void) {
    return D_0042B160;
}
