#ifndef ITEM_CLASSES_H
#define ITEM_CLASSES_H

/* item_classes.c: what other files call. */
#include "common.h"
#include "common.h"

typedef struct ItemObj {
    void *vtbl;
    s32 id;
    u8 flag;
    u64 data; /* +0x10 */
} ItemObj;

typedef struct B2_Obj {
    void *vtbl;
    s32 id;
    u8 flag;
    u64 data; /* +0x10 */
} B2_Obj;

/* item_classes.c */
extern ItemObj *Item29_ctor(ItemObj *self);
extern ItemObj *Item28_ctor(ItemObj *self);
extern ItemObj *Item27_ctor(ItemObj *self);
extern ItemObj *Item26_ctor(ItemObj *self);
extern ItemObj *Item25_ctor(ItemObj *self);
extern ItemObj *Item24_ctor(ItemObj *self);
extern ItemObj *Item23_ctor(ItemObj *self);
extern ItemObj *Item22_ctor(ItemObj *self);
extern ItemObj *Item21_ctor(ItemObj *self);
extern ItemObj *Item20_ctor(ItemObj *self);
extern ItemObj *Item1F_ctor(ItemObj *self);
extern ItemObj *Item1E_ctor(ItemObj *self);
extern ItemObj *Item1D_ctor(ItemObj *self);
extern ItemObj *Item1C_ctor(ItemObj *self);
extern ItemObj *Item1B_ctor(ItemObj *self);
extern ItemObj *Item1A_ctor(ItemObj *self);
extern ItemObj *Item19_ctor(ItemObj *self);
extern ItemObj *Item18_ctor(ItemObj *self);
extern ItemObj *Item17_ctor(ItemObj *self);
extern ItemObj *Item16_ctor(ItemObj *self);
extern ItemObj *Item15_ctor(ItemObj *self);
extern ItemObj *Item14_ctor(ItemObj *self);
extern ItemObj *Item13_ctor(ItemObj *self);
extern ItemObj *Item12_ctor(ItemObj *self);
extern ItemObj *Item11_ctor(ItemObj *self);
extern B2_Obj *Item10_ctor(B2_Obj *self);
extern B2_Obj *Item0F_ctor(B2_Obj *self);
extern B2_Obj *Item0E_ctor(B2_Obj *self);
extern B2_Obj *Item0D_ctor(B2_Obj *self);
extern B2_Obj *Item0C_ctor(B2_Obj *self);
extern B2_Obj *Item0B_ctor(B2_Obj *self);
extern B2_Obj *Item0A_ctor(B2_Obj *self);
extern B2_Obj *Item09_ctor(B2_Obj *self);
extern B2_Obj *Item08_ctor(B2_Obj *self);
extern B2_Obj *Item07_ctor(B2_Obj *self);
extern B2_Obj *Item06_ctor(B2_Obj *self);
extern B2_Obj *Item05_ctor(B2_Obj *self);
extern B2_Obj *Item04_ctor(B2_Obj *self);
extern B2_Obj *Item03_ctor(B2_Obj *self);
extern B2_Obj *Item02_ctor(B2_Obj *self);
extern B2_Obj *Item01_ctor(B2_Obj *self);
extern B2_Obj *Item00_ctor(B2_Obj *self);
extern ItemObj *Item4B_ctor(ItemObj *self);
extern ItemObj *Item4A_ctor(ItemObj *self);
extern ItemObj *Item49_ctor(ItemObj *self);
extern ItemObj *Item48_ctor(ItemObj *self);
extern ItemObj *Item47_ctor(ItemObj *self);
extern ItemObj *Item46_ctor(ItemObj *self);
extern ItemObj *Item45_ctor(ItemObj *self);
extern ItemObj *Item44_ctor(ItemObj *self);
extern ItemObj *Item43_ctor(ItemObj *self);
extern ItemObj *Item42_ctor(ItemObj *self);
extern ItemObj *Item41_ctor(ItemObj *self);
extern ItemObj *Item40_ctor(ItemObj *self);
extern ItemObj *Item3E_ctor(ItemObj *self);
extern ItemObj *Item74_ctor(ItemObj *self);
extern ItemObj *Item73_ctor(ItemObj *self);
extern ItemObj *Item72_ctor(ItemObj *self);
extern ItemObj *Item71_ctor(ItemObj *self);
extern ItemObj *Item70_ctor(ItemObj *self);
extern ItemObj *Item66_ctor(ItemObj *self);
extern ItemObj *Item82_ctor(ItemObj *self);
extern ItemObj *Item81_ctor(ItemObj *self);
extern ItemObj *Item80_ctor(ItemObj *self);
extern ItemObj *Item75_ctor(ItemObj *self);
extern ItemObj *Item88_ctor(ItemObj *self);
extern ItemObj *Item87_ctor(ItemObj *self);
extern ItemObj *Item86_ctor(ItemObj *self);
extern ItemObj *Item83_ctor(ItemObj *self);
extern ItemObj *Item8C_ctor(ItemObj *self);
extern ItemObj *Item8B_ctor(ItemObj *self);
extern ItemObj *Item8A_ctor(ItemObj *self);
extern ItemObj *Item89_ctor(ItemObj *self);
extern ItemObj *Item98_ctor(ItemObj *self);
extern ItemObj *Item97_ctor(ItemObj *self);
extern ItemObj *Item95_ctor(ItemObj *self);
extern ItemObj *Item94_ctor(ItemObj *self);
extern ItemObj *Item93_ctor(ItemObj *self);
extern ItemObj *Item92_ctor(ItemObj *self);
extern ItemObj *Item91_ctor(ItemObj *self);
extern ItemObj *Item90_ctor(ItemObj *self);
extern ItemObj *Item8D_ctor(ItemObj *self);
extern ItemObj *ItemAB_ctor(ItemObj *self);
extern ItemObj *ItemAA_ctor(ItemObj *self);
extern ItemObj *ItemA9_ctor(ItemObj *self);
extern ItemObj *ItemA8_ctor(ItemObj *self);
extern ItemObj *ItemA7_ctor(ItemObj *self);
extern ItemObj *ItemA6_ctor(ItemObj *self);
extern ItemObj *ItemA5_ctor(ItemObj *self);
extern ItemObj *ItemA4_ctor(ItemObj *self);
extern ItemObj *ItemA3_ctor(ItemObj *self);
extern ItemObj *ItemA2_ctor(ItemObj *self);
extern ItemObj *ItemA1_ctor(ItemObj *self);
extern ItemObj *ItemA0_ctor(ItemObj *self);
extern ItemObj *Item9B_ctor(ItemObj *self);
extern ItemObj *Item65_ctor(ItemObj *self);
extern ItemObj *Item64_ctor(ItemObj *self);
extern ItemObj *Item63_ctor(ItemObj *self);
extern ItemObj *Item62_ctor(ItemObj *self);
extern ItemObj *Item61_ctor(ItemObj *self);
extern ItemObj *Item60_ctor(ItemObj *self);
extern ItemObj *Item4C_ctor(ItemObj *self);
extern void SubPool_delete(void *p);   /* operator delete (pool entries) */

#endif /* ITEM_CLASSES_H */
