/* Metrowerks C++ runtime helpers. */
#include "common.h"
#include "ptmf.h"

/* __ptmf_cmpr: whether two member function pointers differ */
s32 func_00100B80(const PTMF *a, const PTMF *b) {
    return (a->this_delta ^ b->this_delta) | (a->vtbl_offset ^ b->vtbl_offset) |
           (a->u.vptr_offset ^ b->u.vptr_offset) ? 1 : 0;
}
