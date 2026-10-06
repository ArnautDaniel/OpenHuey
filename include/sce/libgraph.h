#ifndef LIBGRAPH_H
#define LIBGRAPH_H

/* libgraph.c: what other files call. */
#include "common.h"
#include "common.h"

/* One drawing context's registers (sceGsDrawEnv1 / 2), as A+D pairs. */
typedef struct GsDrawEnv {
    u64 frame, frameAddr;
    u64 zbuf, zbufAddr;
    u64 xyoffset, xyoffsetAddr;
    u64 scissor, scissorAddr;
    u64 prmodecont, prmodecontAddr;
    u64 colclamp, colclampAddr;
    u64 dthe, dtheAddr;
    u64 test, testAddr;
} GsDrawEnv;

/* libgraph.c */
extern s32 sceGsSetDefDrawEnv(GsDrawEnv *d, s32 psm, s32 w, s32 h, s32 ztest, s32 zpsm);   /* sceGsSetDefDrawEnv */
extern s32 sceGsSetDefDrawEnv2(GsDrawEnv *d, s32 psm, s32 w, s32 h, s32 ztest, s32 zpsm);   /* ... context 2 */
extern s32 sceGsSetDefClear(u64 *c, s32 ztest, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b, s32 a, u32 z);
extern void sceGsSwapDBuff(u8 *db, s32 field);   /* sceGsSwapDBuff */

#endif /* LIBGRAPH_H */
