/* Metrowerks C++ runtime helpers. */
#include "common.h"
#include "ptmf.h"
#include "runtime.h"

extern u8 str_exception[];
extern u8 str_bad_alloc[];
extern u8 str_bad_exception[];
extern u8 D_0044DAB0[];
void *Exception_What(void);
void *BadAlloc_What(void);
void *BadException_What(void);
void *Libc_Data44DAB0(void);

/* 0x00100540 */
void *Exception_What(void) {
    return str_exception;
}

/* 0x00100650 */
void *BadAlloc_What(void) {
    return str_bad_alloc;
}
/* __ptmf_cmpr: whether two member function pointers differ */
/* 0x00100B80 */
s32 __ptmf_cmpr(const PTMF *a, const PTMF *b) {
    return (a->this_delta ^ b->this_delta) | (a->vtbl_offset ^ b->vtbl_offset) |
           (a->u.vptr_offset ^ b->u.vptr_offset) ? 1 : 0;
}

/* 0x00102310 */
void *BadException_What(void) {
    return str_bad_exception;
}

/* 0x00114B90 */
void *Libc_Data44DAB0(void) {
    return D_0044DAB0;
}
