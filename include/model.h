/* Shared bits of the character model classes (model.c, stalker_models.c). */
#ifndef MODEL_H
#define MODEL_H

#include "common.h"

extern void *D_00469D00[], *D_0046ADA0[], *D_0046B210[], *D_0046F9E0[], *D_0046C160[],
    *D_0046B0D0[], *D_0046B1C0[];
extern void func_002DC6D0(void *p);   /* model delete */

/* the human model base's destruction: back down its vtables, then (flags > 0) delete */
static inline void *HumanModel_Destroy(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = D_0046C160;
        AT(m, 0x988, void **) = D_0046B0D0;
        AT(m, 0x928, void **) = D_0046B0D0;
        AT(m, 0x0, void **) = D_0046F9E0;
        AT(m, 0x0, void **) = D_0046B210;
        AT(m, 0x1D0, void **) = D_0046B1C0;
        AT(m, 0x1D0, void **) = D_00469D00;
        AT(m, 0x10, void **) = D_0046ADA0;
        AT(m, 0x10, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_002DC6D0(m);
        }
    }
    return m;
}

#endif
