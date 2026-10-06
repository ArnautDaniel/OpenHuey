/* Leaf functions func_002645D0..func_00265C60 (batch 2). */
#include "common.h"
#include "ptmf.h"
#include "globals.h"

typedef s32 (*LoaderLoadFn)(void *loader, const char *name, void *dest, s32 flags, s32 arg);

typedef struct B2_Obj {
    void *vtbl;
    s32 id;
    u8 flag;
    u64 data; /* +0x10 */
} B2_Obj;

extern char D_0045A7E0[]; /* file name */
extern char D_0045A800[]; /* file name */
extern char D_0045A820[]; /* file name */
extern char D_0045A840[]; /* file name */
extern char D_0045A860[]; /* file name */
extern char D_0045A880[]; /* file name */
extern char D_0045A9C0[]; /* file name */
extern char D_0045A9E0[]; /* file name */
extern char D_0045AA00[]; /* file name */
extern char D_0045AA20[]; /* file name */
extern char D_0045AB60[]; /* file name */
extern char D_0045AB80[]; /* file name */
extern char D_0045ABA0[]; /* file name */
extern char D_0045ABC0[]; /* file name */
extern char D_0045ABE0[]; /* file name */
extern char D_0045AC00[]; /* file name */
extern char D_0045AC20[]; /* file name */
extern char D_0045AC40[]; /* file name */
extern char D_0045AC60[]; /* file name */
extern char D_0045AC80[]; /* file name */
extern u8 D_0046C790[]; /* base class vtable */
extern u8 D_0046EE40[];
extern u8 D_0046EE90[];
extern u8 D_0046EEE0[];
extern u8 D_0046EF30[];
extern u8 D_0046EF80[];
extern u8 D_0046EFD0[];
extern u8 D_004703F0[];
extern u8 D_00470FB0[];
extern u8 D_00471080[];
extern u8 D_004710D0[];
extern u8 D_00471120[];
extern u8 D_00471170[];
extern u8 D_004711C0[];
extern u8 D_00472F80[];
extern u8 D_00472FD0[];
extern u8 D_00473020[];
extern u8 D_00473070[];

B2_Obj *func_002645D0(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x10;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00473070;
    return self;
}

B2_Obj *func_00264600(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xF;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00473020;
    return self;
}

B2_Obj *func_00264630(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xE;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00472FD0;
    return self;
}

B2_Obj *func_00264660(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xD;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00472F80;
    return self;
}

B2_Obj *func_00264690(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xC;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_004711C0;
    return self;
}

B2_Obj *func_002646C0(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xB;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00471170;
    return self;
}

B2_Obj *func_002646F0(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xA;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00471120;
    return self;
}

B2_Obj *func_00264720(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x9;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_004710D0;
    return self;
}

B2_Obj *func_00264750(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x8;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00471080;
    return self;
}

B2_Obj *func_00264780(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x7;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00470FB0;
    return self;
}

B2_Obj *func_002647B0(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x6;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_004703F0;
    return self;
}

B2_Obj *func_002647E0(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x5;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046EFD0;
    return self;
}

B2_Obj *func_00264810(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x4;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046EF80;
    return self;
}

B2_Obj *func_00264840(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x3;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046EF30;
    return self;
}

B2_Obj *func_00264870(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x2;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046EEE0;
    return self;
}

B2_Obj *func_002648A0(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x1;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046EE90;
    return self;
}

B2_Obj *func_002648D0(B2_Obj *self) {
    self->vtbl = D_0046C790;
    self->id = 0;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046EE40;
    return self;
}

s32 func_00264C10(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045AC80, dest, 0x4000000, 0);
}

s32 func_00264E90(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045AC60, dest, 0x4000000, 0);
}

s32 func_00264FE0(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045A880, dest, 0x4000000, 0);
}

s32 func_002650D0(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045A860, dest, 0x4000000, 0);
}

s32 func_00265180(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045A840, dest, 0x4000000, 0);
}

s32 func_00265230(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045A820, dest, 0x4000000, 0);
}

s32 func_002652E0(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045A800, dest, 0x4000000, 0);
}

s32 func_00265390(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045A7E0, dest, 0x4000000, 0);
}

s32 func_00265440(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045AC40, dest, 0x4000000, 0);
}

s32 func_00265520(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045AC20, dest, 0x4000000, 0);
}

s32 func_002655D0(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045AC00, dest, 0x4000000, 0);
}

s32 func_00265680(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045ABE0, dest, 0x4000000, 0);
}

s32 func_00265730(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045ABC0, dest, 0x4000000, 0);
}

s32 func_00265810(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045ABA0, dest, 0x4000000, 0);
}

s32 func_002658C0(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045AB80, dest, 0x4000000, 0);
}

s32 func_00265970(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045AB60, dest, 0x4000000, 0);
}

s32 func_00265A20(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045AA20, dest, 0x4000000, 0);
}

s32 func_00265B00(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045AA00, dest, 0x4000000, 0);
}

s32 func_00265BB0(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045A9E0, dest, 0x4000000, 0);
}

s32 func_00265C60(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, D_0045A9C0, dest, 0x4000000, 0);
}
