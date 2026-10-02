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

#endif
