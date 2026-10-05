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

/* Static PTMF constants in the original's rodata are separate 16-byte aligned objects;
 * this describes a run of consecutive ones. */
typedef struct PTMF16 {
    PTMF p;
    u32 pad;
} PTMF16;

static inline s32 ptmf_test(const PTMF *p) {
    return p->this_delta != 0 || p->vtbl_offset != 0 || p->u.func != NULL;
}

/* dst = src, unless src is null (the original's `PTMF s = ...; if (__ptmf_test(&s)) dst = s;`,
 * inlined at every state change) */
static inline void ptmf_set(PTMF *dst, const PTMF *src) {
    PTMF s = *src;

    if (ptmf_test(&s)) {
        *dst = s;
    }
}

/* dst = the non-virtual member function fn */
static inline void ptmf_set_fn(PTMF *dst, void *fn) {
    PTMF s = {0, -1, {fn}};

    if (ptmf_test(&s)) {
        *dst = s;
    }
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

/* (self->*p)() for a member function returning an int/bool */
static inline s32 ptmf_scall_r(void *self, const PTMF *p) {
    char *obj = (char *)self + p->this_delta;
    s32 (*fn)(void *);

    if (p->vtbl_offset < 0) {
        fn = (s32 (*)(void *))p->u.func;
    } else {
        char *vtbl = *(char **)(obj + p->u.vptr_offset);
        fn = *(s32 (**)(void *))(vtbl + p->vtbl_offset);
    }
    return fn(obj);
}

/* (self->*p)(a) */
static inline void ptmf_scall_1(void *self, const PTMF *p, s32 a) {
    char *obj = (char *)self + p->this_delta;
    void (*fn)(void *, s32);

    if (p->vtbl_offset < 0) {
        fn = (void (*)(void *, s32))p->u.func;
    } else {
        char *vtbl = *(char **)(obj + p->u.vptr_offset);
        fn = *(void (**)(void *, s32))(vtbl + p->vtbl_offset);
    }
    fn(obj, a);
}

/* (self->*p)(a, b) returning an int */
static inline s32 ptmf_scall_r2(void *self, const PTMF *p, s32 a, s32 b) {
    char *obj = (char *)self + p->this_delta;
    s32 (*fn)(void *, s32, s32);

    if (p->vtbl_offset < 0) {
        fn = (s32 (*)(void *, s32, s32))p->u.func;
    } else {
        char *vtbl = *(char **)(obj + p->u.vptr_offset);
        fn = *(s32 (**)(void *, s32, s32))(vtbl + p->vtbl_offset);
    }
    return fn(obj, a, b);
}

/* (self->*p)(x, y) with float arguments, returning an int */
static inline s32 ptmf_scall_rff(void *self, const PTMF *p, f32 x, f32 y) {
    char *obj = (char *)self + p->this_delta;
    s32 (*fn)(void *, f32, f32);

    if (p->vtbl_offset < 0) {
        fn = (s32 (*)(void *, f32, f32))p->u.func;
    } else {
        char *vtbl = *(char **)(obj + p->u.vptr_offset);
        fn = *(s32 (**)(void *, f32, f32))(vtbl + p->vtbl_offset);
    }
    return fn(obj, x, y);
}

/* Call a virtual function by its byte offset in the (Metrowerks-layout) vtable at obj+0:
 * VCALL(obj, 0x14, void (*)(void *, s32))(obj, 1);
 * Used while the surrounding code is still asm; becomes a real C++ virtual call later. */
#define VCALL(obj, offset, type) ((type)(*(void ***)(obj))[(offset) / 4])
/* the same through a vtable kept at `vtbl` in the object (a second base class's, or a member's) */
#define VCALL_AT(obj, vtbl, offset, type) ((type)(*(void ***)((u8 *)(obj) + (vtbl)))[(offset) / 4])

#endif /* PTMF_H */
