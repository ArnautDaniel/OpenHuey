/* Metrowerks MSL C++ runtime (PS2 0x00100230..0x0010BDC0) as far as the game needs it on PC. */

/* iostream init objects (std::ios_base::Init and a sibling): the game doesn't use iostreams. */
void func_00102D80(void *obj) { (void)obj; }
void func_001032F0(void *obj) { (void)obj; }

/* __register_global_object(obj, dtor, link): global destructors to run at exit. The process
 * exit frees everything on PC, so nothing is kept. */
void func_00100AB0(void *obj, void (*dtor)(void), void *link) {
    (void)obj;
    (void)dtor;
    (void)link;
}

/* __construct_array(array, ctor, dtor, size, count): construct `count` elements. */
void func_00100340(void *array, void *(*ctor)(void *), void (*dtor)(void *, int), unsigned size,
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

u64bits func_00100230(unsigned x) { return b_of((double)x); }                       /* (double)u32 */
u64bits func_0011F148(u64bits a, u64bits b) { return b_of(d_of(a) + d_of(b)); }     /* a + b */
u64bits func_0011F208(u64bits a, u64bits b) { return b_of(d_of(a) * d_of(b)); }     /* a * b */
u64bits func_0011F458(u64bits a, u64bits b) { return b_of(d_of(a) / d_of(b)); }     /* a / b */
float func_0011F878(u64bits a) { return (float)d_of(a); }                          /* (float)a */

#include <stdlib.h>

/* operator new / delete and the plain allocator they sit on */
void *func_00100660(unsigned size) { return calloc(1, size ? size : 1); }
void func_00100490(void *p) { free(p); }
void *func_00100550(unsigned size) { return calloc(1, size ? size : 1); }
void func_00100470(void *p) { free(p); }

/* float math library (MSL, 0x319B00..0x31CAA0); identified by running the originals in the
 * difftest interpreter (tools/mathprobe.py) */
float func_0031C058(float x) { return cosf(x); }
float func_0031C248(float x) { return sinf(x); }
float func_0031C5C0(float y, float x) { return atan2f(y, x); }
