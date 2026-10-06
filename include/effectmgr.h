/* The effect manager (SceneGame +0xF6E200, gEffects): spawned scene objects (effects,
 * props) in 0x400 slots, allocated from its heap. */
#ifndef EFFECTMGR_H
#define EFFECTMGR_H

#include "common.h"
#include "game.h"

/* A heap at +0x10000 (vtable +0x10 alloc(size)) and 0x400
 * effect slots at +0x18034; func_002D6090 starts the effect in a slot with its parameters. */
extern u8 *gEffects;
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
    u8 *mgr = gEffects;

    func_002D6090(mgr, Effect_New(mgr, 0xE60, HitEffect_Init), hp);
}


/* ---- the drop splash (0xF70 bytes, vtable D_004727C0; creature.c): spawned by the dripping
   strand and by the kind-1 thing (placed.c) with { s32 colour 0..127 x3; f32 pos[3]; f32 size } ---- */
extern void *D_004727C0[];

static inline void DropSplash_Init(void **obj) {
    obj[0] = D_004727C0;
    obj[0xC10 / 4] = D_00469D00;
    ((s32 *)obj)[0xC14 / 4] = -1;
    obj[0xC10 / 4] = D_0046FC30;
}

/* ---- the burst (0xFD0 bytes, vtable D_00474FB0; two quad drawers) a kind-2 thing or a shoved
   character gives off, spawned with the position ---- */
extern void *D_00474FB0[];

static inline void ShoveBurst_Init(void **obj) {
    obj[0] = D_00474FB0;
    obj[0xC10 / 4] = D_00469D00;
    ((s32 *)obj)[0xC14 / 4] = -1;
    obj[0xC10 / 4] = D_0046FC30;
    obj[0xC48 / 4] = D_00469D00;
    ((s32 *)obj)[0xC4C / 4] = -1;
    obj[0xC48 / 4] = D_0046FC30;
}

/* a quad (sprite) drawer (vtable D_0046FC30) handed to the renderer for one frame by
   func_002E56C0; see gl_sprites in effects.c for its fields */
typedef struct QuadDrawer {
    /* 0x00 */ void **vtbl;
    /* 0x04 */ s32 a;
    /* 0x08 */ u64 tex;
    /* 0x10 */ void *rec;       /* the instances: 0x30 each */
    /* 0x14 */ s32 corners;
    /* 0x18 */ f32 cx, cy;      /* the corners' offset */
    /* 0x20 */ s32 layer;
    /* 0x24 */ s16 count;
    /* 0x26 */ s16 cellX, cellY;
    /* 0x2A */ s16 cellW, cellH;
    /* 0x2E */ s16 texW, texH;
    /* 0x32 */ s8 flags;        /* 0x40 additive, 0x80 also a glow pass */
    /* 0x33 */ s8 frames;
    /* 0x34 */ s8 texId, texGroup;
    /* 0x36 */ s8 palette;      /* -1: the first */
} QuadDrawer;

/* an instance of a quad drawer: colour (0x80 = 1.0), position, size, turn, frame */
typedef struct QuadRec {
    s32 rgba[4];
    f32 pos[4];
    f32 w, h;
    f32 turn;
    s32 frame;
} QuadRec;


#endif
