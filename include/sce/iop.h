#ifndef SCE_IOP_H
#define SCE_IOP_H

/* IOP modules and memory: native/platform/iop.c on PC. */
#include "common.h"

extern s32 Iop_LoadModule(void *iop, const char *module, s32, s32, s32);   /* IOP: load a module */
extern void Iop_Reset(void *iop);   /* IOP: reset, set up module loading */
extern s32 Iop_LoadEmbeddedModule(s32 *result);   /* load the embedded IOP module, *result = its status */

#endif /* SCE_IOP_H */
