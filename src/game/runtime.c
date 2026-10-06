/* Metrowerks C++ runtime helpers. */
#include "common.h"
#include "ptmf.h"

extern u8 D_0044C898[];
extern u8 D_0044C900[];
extern u8 D_0044CAC0[];
extern u8 D_0044DAB0[];
void *func_00100540(void);
void *func_00100650(void);
void *func_00102310(void);
void *func_00114B90(void);

void *func_00100540(void) {
    return D_0044C898;
}

void *func_00100650(void) {
    return D_0044C900;
}
/* __ptmf_cmpr: whether two member function pointers differ */
s32 func_00100B80(const PTMF *a, const PTMF *b) {
    return (a->this_delta ^ b->this_delta) | (a->vtbl_offset ^ b->vtbl_offset) |
           (a->u.vptr_offset ^ b->u.vptr_offset) ? 1 : 0;
}

void *func_00102310(void) {
    return D_0044CAC0;
}

void *func_00114B90(void) {
    return D_0044DAB0;
}
