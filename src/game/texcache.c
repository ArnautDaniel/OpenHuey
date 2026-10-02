/* Texture cache (Game +0x14E8C90, vtable 0x46B1D0, global D_0044E4E8): up to 64 registered
 * textures (groups of a .TEX file each) share the renderer's 10 texture layers (+0x304). */
#include "common.h"
#include "game.h"
#include "ptmf.h"

#define AT(p, off, type) (*(type *)((u8 *)(p) + (off)))

typedef struct TexEntry {
    /* 0x0 */ u8 *tex;     /* the texture's .TEX entry */
    /* 0x4 */ s16 age;     /* uses left before its layer may be taken (-1: free to take) */
    /* 0x6 */ s16 layer;   /* renderer layer (0..9), -1 none */
    /* 0x8 */ s8 group;
    /* 0x9 */ u8 index;
    /* 0xA */ u8 padA[2];
} TexEntry;

typedef struct TexCache {
    /* 0x000 */ void **vtbl;
    /* 0x004 */ TexEntry e[64];
    /* 0x304 */ s32 layers[10];
} TexCache;

_Static_assert(__builtin_offsetof(TexCache, layers) == 0x304, "TexCache.layers");

/* +0x8 the renderer layer to draw texture `sel` of group `group` with; bit 31: the layer was just
 * (re)assigned and needs the texture uploaded. -1 if there is no such texture. */
s32 func_001F41A0(TexCache *c, s32 sel, s32 group) {
    u32 idx = sel + group, i;
    TexEntry *e;
    s32 r = 0;

    if (idx >= 64) {
        return -1;
    }
    e = &c->e[idx];
    if (e->tex == NULL) {
        return -1;
    }
    if (e->layer < 0) {
        u32 freeLayer[10];
        s16 l;

        for (i = 0; i < 10; i++) {
            freeLayer[i] = 0xFF;
        }
        for (i = 0; i < 64; i++) {
            if (c->e[i].layer >= 0) {
                freeLayer[c->e[i].layer] = 0;
            }
        }
        for (l = 0; (u32)l < 10; l++) {
            if (freeLayer[l] != 0) {
                e->layer = l;
                r = 0x80000000;
                break;
            }
        }
        if (r == 0) {
            s32 best = -1;
            u32 bestIdx = (u32)-1;

            for (i = 0; i < 64; i++) {
                TexEntry *o = &c->e[i];

                if (i == idx || o->layer < 0) {
                    continue;
                }
                if (o->age >= 0) {
                    if (best < 0 || o->age < best) {
                        best = o->age;
                        bestIdx = i;
                    }
                    continue;
                }
                r = 0x80000000;   /* an expired entry: take its layer */
                e->layer = o->layer;
                o->layer = -1;
                break;
            }
            if (r == 0) {   /* the least recently used */
                r = 0x80000000;
                e->layer = c->e[bestIdx].layer;
                c->e[bestIdx].layer = -1;
                c->e[bestIdx].age = -1;
            }
        }
    }
    for (i = 0; i < 64; i++) {
        c->e[i].age = c->e[i].age > 0 ? c->e[i].age - 1 : -1;
    }
    e->age = 64;
    return c->layers[e->layer] | r;
}

/* +0xC the .TEX entry of texture `sel` of group `group` */
u8 *func_001F4160(TexCache *c, s32 sel, s32 group) {
    u32 idx = sel + group;

    if (idx >= 64) {
        return NULL;
    }
    return c->e[idx].tex;
}

/* +0x10 register the textures of .TEX file `tex` as group `group` (entries group.. group+n-1) */
void func_001F4420(TexCache *c, u32 *tex, s32 group) {
    TexEntry *e;
    u8 k;

    if (tex == NULL) {
        return;
    }
    e = &c->e[(s8)group];
    for (k = 0; k < *tex; k++, e++) {
        if ((u32)(k + (s8)group) < 64) {
            e->tex = (u8 *)tex + 0x10 + k * 0x10;
            e->age = -1;
            e->layer = -1;
            e->group = group;
            e->index = k;
        }
    }
}

/* +0x14 unregister group `group` */
void func_001F43C0(TexCache *c, s32 group) {
    u32 i;

    for (i = 0; i < 64; i++) {
        if (c->e[i].group == group) {
            c->e[i].tex = NULL;
            c->e[i].age = -1;
            c->e[i].layer = -1;
            c->e[i].index = 0xFF;
            c->e[i].group = -1;
        }
    }
}

/* +0x18 forget all layer assignments */
void func_001F44A0(TexCache *c) {
    u32 i;

    for (i = 0; i < 64; i++) {
        c->e[i].age = -1;
        c->e[i].layer = -1;
    }
}

extern TexCache *D_0044E4E8;
extern void *D_0044E9A0;   /* VRAM manager */
extern void *D_0044E4F0;   /* renderer */

/* TEX0 to draw cached texture `sel` (group 0) with, uploading it first when its VRAM slot was
 * just (re)assigned. 0 if there's no such texture. */
u64 func_002B71D0(s32 sel) {
    TexCache *c = D_0044E4E8;
    s32 slot;
    u8 *tex;
    u64 tex0;

    slot = VCALL(c, 0x8, s32 (*)(TexCache *, s32, s32))(c, sel, 0);
    if (slot == -1) {
        return 0;
    }
    tex = VCALL(c, 0xC, u8 *(*)(TexCache *, s32, s32))(c, sel, 0);
    tex0 = VCALL(D_0044E9A0, 0x28, u64 (*)(void *, s32, s32, u32, u32, s32))(
        D_0044E9A0, slot & 0x7FFFFFFF, tex[0], AT(tex, 4, u16), AT(tex, 6, u16), tex[1]);
    if (slot & 0x80000000) {
        VCALL(D_0044E4F0, 0x44, s32 (*)(void *, s32, u8 *, s32))(D_0044E4F0, slot, tex, -1);
    }
    return tex0;
}
