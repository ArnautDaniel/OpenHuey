#ifndef TEXCACHE_H
#define TEXCACHE_H

/* The texture cache (global D_0044E4E8): textures by id within groups (a group is a VRAM area
 * a screen owns, e.g. 0x18 / 0x19 the sub screen's), uploaded through the renderer. */
#include "common.h"
#include "game.h"

extern VObject *D_0044E4E8;   /* the texture cache */
extern VObject *D_0044E4F0;   /* the renderer */

/* The VRAM slot of texture `id` of `group` and its header (*tex), uploading it into renderer
 * layer `layer` first if it isn't resident (slot bit 31); -1 if it isn't cached or the upload
 * has no room. The original inlines this before every textured sprite. */
static inline s32 TexCache_Resident(s32 id, s32 group, s32 layer, u8 **tex) {
    VObject *tc = D_0044E4E8;
    s32 slot = VCALL(tc, 0x8, s32 (*)(VObject *, s32, s32))(tc, id, group);

    if (slot == -1) {
        return -1;
    }
    *tex = VCALL(tc, 0xC, u8 *(*)(VObject *, s32, s32))(tc, id, group);
    if (slot & 0x80000000) {
        slot &= 0x7FFFFFFF;
        if (!(u8)VCALL(D_0044E4F0, 0x44, s32 (*)(VObject *, s32, void *, s32))(D_0044E4F0, slot, *tex, layer)) {
            return -1;
        }
    }
    return slot;
}

#endif /* TEXCACHE_H */
