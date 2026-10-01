#ifndef PTMF_H
#define PTMF_H

#include "common.h"

/*
 * Metrowerks C++ pointer-to-member-function (12 bytes), as used by the original
 * code for state machines (`(this->*state)()`):
 *   non-virtual: { this_delta, -1,          function       }
 *   virtual:     { this_delta, vtbl_offset, vptr_offset    }
 * A null PTMF is all zero.
 *
 * The original calls these through MSL helpers (__ptmf_test, __ptmf_scall, which
 * takes the PTMF in $t9); C code uses the inline equivalents below.
 */
typedef struct PTMF {
    s32 this_delta;
    s32 vtbl_offset; /* byte offset into the vtable, or -1 for a non-virtual function */
    union {
        void *func;      /* non-virtual target */
        s32 vptr_offset; /* virtual: where the vtable pointer sits in the object */
    } u;
} PTMF;

static inline s32 ptmf_test(const PTMF *p) {
    return p->this_delta != 0 || p->vtbl_offset != 0 || p->u.func != NULL;
}

/* (self->*p)() for a member function taking no arguments */
static inline void ptmf_scall(void *self, const PTMF *p) {
    char *obj = (char *)self + p->this_delta;
    void (*fn)(void *);

    if (p->vtbl_offset < 0) {
        fn = (void (*)(void *))p->u.func;
    } else {
        char *vtbl = *(char **)(obj + p->u.vptr_offset);
        fn = *(void (**)(void *))(vtbl + p->vtbl_offset);
    }
    fn(obj);
}

/* Call a virtual function by its byte offset in the (Metrowerks-layout) vtable at obj+0:
 * VCALL(obj, 0x14, void (*)(void *, s32))(obj, 1);
 * Used while the surrounding code is still asm; becomes a real C++ virtual call later. */
#define VCALL(obj, offset, type) ((type)(*(void ***)(obj))[(offset) / 4])

#endif /* PTMF_H */
