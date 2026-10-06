/* Random number generator (Mersenne Twister; vtable around 0x46AB60, global gRandom). */
#include "common.h"
#include "game.h"
#include "globals.h"
#include "random.h"
#include "msl.h"

/* MW runtime soft-float doubles, as raw IEEE bit patterns in 64-bit registers. */

#define DBL_2POW26 0x4190000000000000ULL     /* 67108864.0 */
#define DBL_2POWM53 0x3CA0000000000000ULL    /* 1.0 / 9007199254740992.0 */

/* genrand_res53: uniform double in [0, 1) with 53-bit resolution, from two 32-bit outputs
 * (vtable +0x10). Returned as a double in v0. */
u64 func_001A43D0(VObject *rng) {
    u32 a = VCALL(rng, 0x10, u32 (*)(VObject *))(rng) >> 5;
    u32 b = VCALL(rng, 0x10, u32 (*)(VObject *))(rng) >> 6;
    u64 x = func_0011F208(DBL_2POW26, func_00100230(a));

    return func_0011F208(func_0011F148(x, func_00100230(b)), DBL_2POWM53);
}

extern void *D_0046AB50[];

/* constructor: register, seed (vtable +0xC) */
VObject *func_001A48C0(VObject *rng, s32 seed) {
    rng->vtbl = D_0046AB50;
    gRandom = rng;
    VCALL(rng, 0xC, void (*)(VObject *, s32))(rng, seed);
    return rng;
}

/* MT19937 state: mt[624] at +4, next output at +0x9C4, outputs left at +0x9C8. */
#define MT_N 624
#define MT_M 397
#define MT(r) ((u32 *)((u8 *)(r) + 4))
#define MT_NEXT(r) (*(u32 **)((u8 *)(r) + 0x9C4))
#define MT_LEFT(r) (*(s32 *)((u8 *)(r) + 0x9C8))
#define MT_TWIST(u, v) ((((u) & 0x80000000u) | ((v) & 0x7FFFFFFFu)) >> 1 ^ ((v) & 1 ? 0x9908B0DFu : 0))

/* +0xC init_genrand(seed) */
void func_001A4630(VObject *r, u32 seed) {
    u32 *mt = MT(r);
    u32 j;

    mt[0] = seed;
    for (j = 1; j < MT_N; j++) {
        mt[j] = 1812433253u * (mt[j - 1] ^ (mt[j - 1] >> 30)) + j;
    }
    MT_LEFT(r) = 1;
}

/* refill the state once all outputs are used */
void func_001A46D0(VObject *r) {
    u32 *p;
    s32 j;

    if (--MT_LEFT(r) != 0) {
        return;
    }
    p = MT(r);
    MT_LEFT(r) = MT_N;
    MT_NEXT(r) = p;
    for (j = MT_N - MT_M; j != 0; j--, p++) {
        *p = p[MT_M] ^ MT_TWIST(p[0], p[1]);
    }
    for (j = MT_M - 1; j != 0; j--, p++) {
        *p = p[MT_M - MT_N] ^ MT_TWIST(p[0], p[1]);
    }
    *p = p[MT_M - MT_N] ^ MT_TWIST(p[0], MT(r)[0]);
}

/* +0x10 genrand_int32 */
u32 func_001A45A0(VObject *r) {
    u32 y;

    func_001A46D0(r);
    y = *MT_NEXT(r)++;
    y ^= y >> 11;
    y ^= (y << 7) & 0x9D2C5680u;
    y ^= (y << 15) & 0xEFC60000u;
    return y ^ (y >> 18);
}

#define MT_INT32(r) VCALL(r, 0x10, u32 (*)(VObject *))(r)
#define DBL_2POW32 0x41F0000000000000ULL     /* 4294967296.0 */
#define DBL_2POW32M1 0x41EFFFFFFFE00000ULL   /* 4294967295.0 */
#define DBL_HALF 0x3FE0000000000000ULL

/* +0x14 genrand_int31 */
u32 func_001A4570(VObject *r) {
    return MT_INT32(r) >> 1;
}

/* +0x18 genrand_real1: [0, 1] */
f32 func_001A4510(VObject *r) {
    return func_0011F878(func_0011F458(func_00100230(MT_INT32(r)), DBL_2POW32M1));
}

/* +0x1C genrand_real2: [0, 1) (RNG01() in the game code) */
f32 func_001A44C0(VObject *r) {
    return func_0011F878(func_0011F458(func_00100230(MT_INT32(r)), DBL_2POW32));
}

/* +0x20 genrand_real3: (0, 1) */
f32 func_001A4460(VObject *r) {
    return func_0011F878(func_0011F458(func_0011F148(DBL_HALF, func_00100230(MT_INT32(r))), DBL_2POW32));
}

extern void *D_0046AB80[];

/* +0x8 destructor */
VObject *func_001A4850(VObject *r, s32 flags) {
    if (r != NULL) {
        r->vtbl = D_0046AB50;
        r->vtbl = D_0046AB80;
        gRandom = NULL;
        if ((s16)flags > 0) {
            func_00100490(r);
        }
    }
    return r;
}

extern void *D_0046AB80[];

/* +0x8 destructor (the global goes) */
void *func_001A4910(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AB80;
        if (o != NULL) {
            gRandom = NULL;
        }
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}
