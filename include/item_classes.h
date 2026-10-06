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
extern ItemObj *func_00264120(ItemObj *self);
extern ItemObj *func_00264150(ItemObj *self);
extern ItemObj *func_00264180(ItemObj *self);
extern ItemObj *func_002641B0(ItemObj *self);
extern ItemObj *func_002641E0(ItemObj *self);
extern ItemObj *func_00264210(ItemObj *self);
extern ItemObj *func_00264240(ItemObj *self);
extern ItemObj *func_00264270(ItemObj *self);
extern ItemObj *func_002642A0(ItemObj *self);
extern ItemObj *func_002642D0(ItemObj *self);
extern ItemObj *func_00264300(ItemObj *self);
extern ItemObj *func_00264330(ItemObj *self);
extern ItemObj *func_00264360(ItemObj *self);
extern ItemObj *func_00264390(ItemObj *self);
extern ItemObj *func_002643C0(ItemObj *self);
extern ItemObj *func_002643F0(ItemObj *self);
extern ItemObj *func_00264420(ItemObj *self);
extern ItemObj *func_00264450(ItemObj *self);
extern ItemObj *func_00264480(ItemObj *self);
extern ItemObj *func_002644B0(ItemObj *self);
extern ItemObj *func_002644E0(ItemObj *self);
extern ItemObj *func_00264510(ItemObj *self);
extern ItemObj *func_00264540(ItemObj *self);
extern ItemObj *func_00264570(ItemObj *self);
extern ItemObj *func_002645A0(ItemObj *self);
extern B2_Obj *func_002645D0(B2_Obj *self);
extern B2_Obj *func_00264600(B2_Obj *self);
extern B2_Obj *func_00264630(B2_Obj *self);
extern B2_Obj *func_00264660(B2_Obj *self);
extern B2_Obj *func_00264690(B2_Obj *self);
extern B2_Obj *func_002646C0(B2_Obj *self);
extern B2_Obj *func_002646F0(B2_Obj *self);
extern B2_Obj *func_00264720(B2_Obj *self);
extern B2_Obj *func_00264750(B2_Obj *self);
extern B2_Obj *func_00264780(B2_Obj *self);
extern B2_Obj *func_002647B0(B2_Obj *self);
extern B2_Obj *func_002647E0(B2_Obj *self);
extern B2_Obj *func_00264810(B2_Obj *self);
extern B2_Obj *func_00264840(B2_Obj *self);
extern B2_Obj *func_00264870(B2_Obj *self);
extern B2_Obj *func_002648A0(B2_Obj *self);
extern B2_Obj *func_002648D0(B2_Obj *self);
extern ItemObj *func_00263E20(ItemObj *self);
extern ItemObj *func_00263E50(ItemObj *self);
extern ItemObj *func_00263E80(ItemObj *self);
extern ItemObj *func_00263EB0(ItemObj *self);
extern ItemObj *func_00263EE0(ItemObj *self);
extern ItemObj *func_00263F10(ItemObj *self);
extern ItemObj *func_00263F40(ItemObj *self);
extern ItemObj *func_00263F70(ItemObj *self);
extern ItemObj *func_00263FA0(ItemObj *self);
extern ItemObj *func_00263FD0(ItemObj *self);
extern ItemObj *func_00264000(ItemObj *self);
extern ItemObj *func_00264030(ItemObj *self);
extern ItemObj *func_00264090(ItemObj *self);
extern ItemObj *func_00263AF0(ItemObj *self);
extern ItemObj *func_00263B20(ItemObj *self);
extern ItemObj *func_00263B50(ItemObj *self);
extern ItemObj *func_00263B80(ItemObj *self);
extern ItemObj *func_00263BB0(ItemObj *self);
extern ItemObj *func_00263BE0(ItemObj *self);
extern ItemObj *func_002639D0(ItemObj *self);
extern ItemObj *func_00263A00(ItemObj *self);
extern ItemObj *func_00263A30(ItemObj *self);
extern ItemObj *func_00263A60(ItemObj *self);
extern ItemObj *func_002638B0(ItemObj *self);
extern ItemObj *func_002638E0(ItemObj *self);
extern ItemObj *func_00263910(ItemObj *self);
extern ItemObj *func_00263940(ItemObj *self);
extern ItemObj *func_00263790(ItemObj *self);
extern ItemObj *func_002637C0(ItemObj *self);
extern ItemObj *func_002637F0(ItemObj *self);
extern ItemObj *func_00263820(ItemObj *self);
extern ItemObj *func_00263580(ItemObj *self);
extern ItemObj *func_002635B0(ItemObj *self);
extern ItemObj *func_002635E0(ItemObj *self);
extern ItemObj *func_00263610(ItemObj *self);
extern ItemObj *func_00263640(ItemObj *self);
extern ItemObj *func_00263670(ItemObj *self);
extern ItemObj *func_002636A0(ItemObj *self);
extern ItemObj *func_002636D0(ItemObj *self);
extern ItemObj *func_00263700(ItemObj *self);
extern ItemObj *func_002632B0(ItemObj *self);
extern ItemObj *func_002632E0(ItemObj *self);
extern ItemObj *func_00263310(ItemObj *self);
extern ItemObj *func_00263340(ItemObj *self);
extern ItemObj *func_00263370(ItemObj *self);
extern ItemObj *func_002633A0(ItemObj *self);
extern ItemObj *func_002633D0(ItemObj *self);
extern ItemObj *func_00263400(ItemObj *self);
extern ItemObj *func_00263430(ItemObj *self);
extern ItemObj *func_00263460(ItemObj *self);
extern ItemObj *func_00263490(ItemObj *self);
extern ItemObj *func_002634C0(ItemObj *self);
extern ItemObj *func_002634F0(ItemObj *self);
extern ItemObj *func_00263C70(ItemObj *self);
extern ItemObj *func_00263CA0(ItemObj *self);
extern ItemObj *func_00263CD0(ItemObj *self);
extern ItemObj *func_00263D00(ItemObj *self);
extern ItemObj *func_00263D30(ItemObj *self);
extern ItemObj *func_00263D60(ItemObj *self);
extern ItemObj *func_00263D90(ItemObj *self);
extern void func_0025FEF0(void *p);   /* operator delete (pool entries) */

#endif /* ITEM_CLASSES_H */
