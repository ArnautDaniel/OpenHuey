/* The effect manager (SceneGame +0xF6E200, D_0044E578): spawned scene objects (effects,
 * props) in 0x400 slots, allocated from its heap. */
#ifndef EFFECTMGR_H
#define EFFECTMGR_H

#include "common.h"
#include "game.h"

/* A heap at +0x10000 (vtable +0x10 alloc(size)) and 0x400
 * effect slots at +0x18034; func_002D6090 starts the effect in a slot with its parameters. */
extern u8 *D_0044E578;
extern void *func_002D63C0(u32 size, void *mem);            /* placement new */
extern s32 func_002D6090(u8 *mgr, s32 slot, void *params);

#define EFFECT_HEAP(mgr) ((VObject *)((mgr) + 0x10000))
#define EFFECT_SLOTS(mgr) ((void ***)((mgr) + 0x18034))
#define EFFECT_NUM_SLOTS 0x400

/* Construct an effect of `size` bytes in a free slot (`init` sets its vtables); -1 if the heap
 * or the slot table is full. */
static inline s32 Effect_New(u8 *mgr, u32 size, void (*init)(void **obj)) {
    void *mem = VCALL(EFFECT_HEAP(mgr), 0x10, void *(*)(VObject *, u32))(EFFECT_HEAP(mgr), size);
    s32 i;

    if (mem == NULL) {
        return -1;
    }
    for (i = 0; i < EFFECT_NUM_SLOTS; i++) {
        if (EFFECT_SLOTS(mgr)[i] == NULL) {
            void **obj = func_002D63C0(size, mem);

            if (obj != NULL) {
                init(obj);
            }
            EFFECT_SLOTS(mgr)[i] = obj;
            VCALL(EFFECT_SLOTS(mgr)[i], 0xC, void (*)(void **))(EFFECT_SLOTS(mgr)[i]);
            return i;
        }
    }
    return -1;
}

/* ---- the hit effect (0xE60 bytes, vtable 0x470F30, its part at +0xC10) the stalkers and Hewie
   leave where a blow lands ---- */
extern void *D_00470F30[], *D_00469D00[], *D_0046FC30[];

typedef struct {
    f32 pos[4];
    u32 kind;      /* Daniella 0xFE; Riccardo 1 on Hewie, else 0 */
    f32 big;       /* 1.0 or 0 */
} HitEffectParams;

static inline void HitEffect_Init(void **obj) {
    obj[0] = D_00470F30;
    obj[0xC10 / 4] = D_00469D00;
    ((s32 *)obj)[0xC14 / 4] = -1;
    obj[0xC10 / 4] = D_0046FC30;
}

static inline void HitEffect_Spawn(HitEffectParams *hp) {
    u8 *mgr = D_0044E578;

    func_002D6090(mgr, Effect_New(mgr, 0xE60, HitEffect_Init), hp);
}

#endif
