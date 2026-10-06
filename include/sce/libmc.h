#ifndef SCE_LIBMC_H
#define SCE_LIBMC_H

/* libmc (memory card): native/platform/memcard.c on PC (host files). */
#include "common.h"

extern s32 sceMcEnd(void);   /* sceMcEnd */
extern void func_00225770(MemCard *mc);
extern void func_00225860(MemCard *mc);
extern void func_00225950(MemCard *mc);   /* write */
extern void func_00225C40(MemCard *mc);   /* read */
extern void func_00225F30(MemCard *mc);
extern void func_00226220(MemCard *mc);   /* check */
extern void func_00226570(void *obj);

#endif /* SCE_LIBMC_H */
