#ifndef MSL_H
#define MSL_H

/* The Metrowerks MSL runtime the game links (C++ new / delete and arrays, math, 64-bit
 * helpers): native/platform/msl.c on PC. */
#include "common.h"

extern void *__nw__FUiPv(u32 size, void *p);   /* placement new */
extern u64 sf_floatunsidf(u32 x);   /* (double)x */
extern void __destroy_arr(void *array, void *(*dtor)(void *, s32), u32 size, u32 n);   /* __destroy_arr */
extern void __construct_array(void *array, void *ctor, void *dtor, u32 size, u32 n);   /* __construct_array */
extern void __sys_free(void *p);   /* free */
extern void __dl__FPv(void *p);   /* operator delete */
extern void *__sys_alloc(u32 size);   /* malloc */
extern void *__nw__FUi(u32 size);   /* operator new */
extern void __register_global_object(void *obj, void (*dtor)(void *, s32), void *link);   /* __register_global_object */
extern void Iostream_InitCtor(void *obj);   /* construct an iostream init object */
extern void func_001032F0(void *obj);   /* construct another one */
extern u64 sf_extendsfdf2(f32 x);   /* (double)x */
extern u64 sf_adddf3(u64 a, u64 b);   /* a + b */
extern u64 sf_muldf3(u64 a, u64 b);   /* a * b */
extern u64 sf_divdf3(u64 a, u64 b);   /* a / b */
extern f32 sf_truncdfsf2(u64 a);   /* (float)a */
extern f32 msl_atanf(f32 x);   /* atanf */
extern f32 msl_cosf(f32 x);   /* cosf */
extern f32 msl_sinf(f32 x);   /* sinf */
extern f32 msl_tanf(f32 x);   /* tanf */
extern f32 msl_acosf(f32 x);   /* acosf */
extern f32 msl_asinf(f32 x);   /* asinf */
extern f32 msl_atan2f(f32 x, f32 z);   /* float math library: heading of (x, z), atan2-like */
extern f32 msl_logf(f32 x);   /* logf */
extern f32 msl_log10f(f32 x);   /* log10f */

#endif /* MSL_H */
