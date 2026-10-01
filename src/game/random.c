/* Random number generator (Mersenne Twister; vtable around 0x46AB60, global D_0044E550). */
#include "common.h"
#include "game.h"

/* MW runtime soft-float doubles, as raw IEEE bit patterns in 64-bit registers. */
extern u64 func_00100230(u32 x);             /* (double)x */
extern u64 func_0011F208(u64 a, u64 b);      /* a * b */
extern u64 func_0011F148(u64 a, u64 b);      /* a + b */

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
