#ifndef SCE_IOP_H
#define SCE_IOP_H

/* IOP modules and memory: native/platform/iop.c on PC. */
#include "common.h"

extern s32 func_001BC0F0(void *iop, const char *module, s32, s32, s32);   /* IOP: load a module */
extern void func_001BC220(void *iop);   /* IOP: reset, set up module loading */
extern s32 func_0037E1F0(s32 *result);   /* load the embedded IOP module, *result = its status */

#endif /* SCE_IOP_H */
