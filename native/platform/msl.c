/* Metrowerks MSL C++ runtime (PS2 0x00100230..0x0010BDC0) as far as the game needs it on PC. */

/* iostream init objects (std::ios_base::Init and a sibling): the game doesn't use iostreams. */
void Iostream_InitCtor(void *obj) { (void)obj; }
void func_001032F0(void *obj) { (void)obj; }

/* __register_global_object(obj, dtor, link): global destructors to run at exit. The process
 * exit frees everything on PC, so nothing is kept. */
void __register_global_object(void *obj, void (*dtor)(void), void *link) {
    (void)obj;
    (void)dtor;
    (void)link;
}

/* __construct_array(array, ctor, dtor, size, count): construct `count` elements. */
void __construct_array(void *array, void *(*ctor)(void *), void (*dtor)(void *, int), unsigned size,
                   unsigned count) {
    unsigned i;

    (void)dtor;
    for (i = 0; i < count; i++) {
        ctor((char *)array + i * size);
    }
}

/* MW soft-float doubles: the game passes doubles as raw IEEE bit patterns in 64-bit registers. */
#include <math.h>
#include <string.h>

typedef unsigned long long u64bits;

static double d_of(u64bits b) { double d; memcpy(&d, &b, 8); return d; }
static u64bits b_of(double d) { u64bits b; memcpy(&b, &d, 8); return b; }

u64bits sf_floatunsidf(unsigned x) { return b_of((double)x); }                       /* (double)u32 */
u64bits sf_extendsfdf2(float x) { return b_of((double)x); }                          /* (double)f32 */
u64bits sf_adddf3(u64bits a, u64bits b) { return b_of(d_of(a) + d_of(b)); }     /* a + b */
u64bits sf_muldf3(u64bits a, u64bits b) { return b_of(d_of(a) * d_of(b)); }     /* a * b */
u64bits sf_divdf3(u64bits a, u64bits b) { return b_of(d_of(a) / d_of(b)); }     /* a / b */
float sf_truncdfsf2(u64bits a) { return (float)d_of(a); }                          /* (float)a */

#include <stdlib.h>

/* operator new / delete and the plain allocator they sit on */
void *__nw__FUi(unsigned size) { return calloc(1, size ? size : 1); }
void __dl__FPv(void *p) { free(p); }
void *__sys_alloc(unsigned size) { return calloc(1, size ? size : 1); }
void __sys_free(void *p) { free(p); }

/* float math library (MSL, 0x319B00..0x31CAA0); identified by running the originals in the
 * difftest interpreter (tools/mathprobe.py) */
float msl_cosf(float x) { return cosf(x); }
float msl_sinf(float x) { return sinf(x); }
float msl_atan2f(float y, float x) { return atan2f(y, x); }

/* placement new */
void *__nw__FUiPv(unsigned size, void *p) { (void)size; return p; }

/* the library answers a domain error with this (not NaN) */
#define MSL_HUGE 0x1.ff933cp+127f

float func_0031C140(float x) { return fabsf(x); }
float func_0031C160(float x) { return floorf(x); }
float msl_tanf(float x) { return tanf(x); }
float msl_acosf(float x) { return x > 1.0f || x < -1.0f ? MSL_HUGE : acosf(x); }
float msl_asinf(float x) { return x > 1.0f || x < -1.0f ? MSL_HUGE : asinf(x); }
float msl_logf(float x) { return x < 0.0f ? MSL_HUGE : x == 0.0f ? -0x1.fffffep+127f : logf(x); }
float msl_log10f(float x) { return x < 0.0f ? MSL_HUGE : x == 0.0f ? -0x1.fffffep+127f : log10f(x); }
float func_0031C980(float x) { return x < 0.0f ? MSL_HUGE : sqrtf(x); }
float msl_atanf(float x) { return atanf(x); }

/* __destroy_arr (PS2 0x001002C0): destroy n objects of `size` bytes, last first */
void __destroy_arr(void *block, void (*dtor)(void *, int), unsigned size, unsigned n) {
    char *p = (char *)block + size * n;

    while (n-- != 0) {
        p -= size;
        dtor(p, -1);
    }
}
