#include "common.h"
#include "ptmf.h"

/* Field access by byte offset into objects whose layout is not yet known. */
/* gFileLoader vtable +0xC: start loading file `name` into `dest` (flags 0x4000000) */
#define FILE_LOAD_ASYNC(name, dest) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, void *, void *, u32, s32))(gFileLoader, name, dest, 0x4000000, 0)

extern u8 D_0042F470[];
extern u8 D_0042F4B0[];
extern u8 D_0042F500[];
extern u8 D_0042F590[];
extern u8 D_0042F5D0[];
extern u8 D_0042F7B0[];
extern u8 D_0042F860[];
extern u8 D_0042F9C0[];
extern void *D_00430700[];
extern void *D_004307F0[];
extern u8 D_00430820[];
extern u8 D_00430860[];
extern u8 D_004308B0[];
extern u8 D_004308F0[];
extern u8 D_00430940[];
extern u8 D_00430980[];
extern u8 D_00430A10[];
extern u8 D_00430A50[];
extern u8 D_00430AB0[];
extern u8 D_00430C40[];
extern u8 D_00430D80[];
extern u8 D_00430F30[];
extern u8 D_004613E0[];
extern u8 D_00461400[];
extern u8 D_00461580[];
extern u8 D_00461660[];
extern u8 D_004616C0[];
extern u8 D_004616E0[];
extern u8 D_00461700[];
extern u8 D_00461720[];
extern u8 D_00461740[];
extern void *gFileLoader;

s32 func_00339320(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_004613E0, dest);
}

s32 func_003396D0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00461400, dest);
}

void *func_00339B20(void) {
    return D_0042F470;
}

void *func_00339B30(void) {
    return D_0042F4B0;
}

void *func_00339D40(void) {
    return D_0042F500;
}

void *func_00339D50(void) {
    return D_0042F590;
}

void *func_00339D60(void) {
    return D_0042F5D0;
}

void *func_00339D70(void) {
    return D_0042F7B0;
}

void *func_00339D80(void) {
    return D_0042F860;
}

void *func_00339D90(void) {
    return D_0042F9C0;
}

void *func_00339DA0(void *self, s32 i) {
    return D_00430700[i];
}

void *func_00339DD0(void *self, s32 i) {
    return D_004307F0[i];
}

void *func_0033ACB0(void) {
    return D_00430820;
}

void *func_0033ACC0(void) {
    return D_00430860;
}

s32 func_0033AEE0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00461580, dest);
}

void *func_0033B030(void) {
    return D_004308B0;
}

void *func_0033B040(void) {
    return D_004308F0;
}

void *func_0033B300(void) {
    return D_00430940;
}

void *func_0033B310(void) {
    return D_00430980;
}

s32 func_0033BDC0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00461660, dest);
}

void *func_0033D7D0(void) {
    return D_00430A10;
}

void *func_0033D7E0(void) {
    return D_00430A50;
}

s32 func_0033E8D0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_004616C0, dest);
}

s32 func_0033EA40(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_004616E0, dest);
}

s32 func_0033EBB0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00461700, dest);
}

s32 func_0033ED20(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00461720, dest);
}

s32 func_0033EE90(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00461740, dest);
}

void *func_0033EFE0(void) {
    return D_00430AB0;
}

void *func_0033EFF0(void) {
    return D_00430C40;
}

void *func_0033F000(void) {
    return D_00430D80;
}

void *func_0033F010(void) {
    return D_00430F30;
}
