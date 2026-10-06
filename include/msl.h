#ifndef MSL_H
#define MSL_H

/* The Metrowerks MSL runtime the game links (C++ new / delete and arrays, math, 64-bit
 * helpers): native/platform/msl.c on PC. */
#include "common.h"

extern void *__nw__FUiPv(u32 size, void *p);   /* placement new */
extern u64 func_00100230(u32 x);   /* (double)x */
extern void func_001002C0(void *array, void *(*dtor)(void *, s32), u32 size, u32 n);   /* __destroy_arr */
extern void func_00100340(void *array, void *ctor, void *dtor, u32 size, u32 n);   /* __construct_array */
extern void func_00100470(void *p);   /* free */
extern void func_00100490(void *p);   /* operator delete */
extern void *func_00100550(u32 size);   /* malloc */
extern void *func_00100660(u32 size);   /* operator new */
extern void func_00100AB0(void *obj, void (*dtor)(void *, s32), void *link);   /* __register_global_object */
extern void func_00102D80(void *obj);   /* construct an iostream init object */
extern void func_001032F0(void *obj);   /* construct another one */
extern u64 func_0011ED78(f32 x);   /* (double)x */
extern u64 func_0011F148(u64 a, u64 b);   /* a + b */
extern u64 func_0011F208(u64 a, u64 b);   /* a * b */
extern u64 func_0011F458(u64 a, u64 b);   /* a / b */
extern f32 func_0011F878(u64 a);   /* (float)a */
extern f32 func_0031BDB0(f32 x);   /* atanf */
extern f32 func_0031C058(f32 x);   /* cosf */
extern f32 func_0031C248(f32 x);   /* sinf */
extern f32 func_0031C338(f32 x);   /* tanf */
extern f32 func_0031C3C0(f32 x);   /* acosf */
extern f32 func_0031C4C0(f32 x);   /* asinf */
extern f32 func_0031C5C0(f32 x, f32 z);   /* float math library: heading of (x, z), atan2-like */
extern f32 func_0031C6E8(f32 x);   /* logf */
extern f32 func_0031C830(f32 x);   /* log10f */

#endif /* MSL_H */
