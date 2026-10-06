#ifndef RENDERER_H
#define RENDERER_H

/* renderer.c: what other files call. */
#include "common.h"
#include "game.h"
#include "globals.h"

typedef struct VObject VObject;

/* renderer.c */
extern void Renderer_SetupVideo(u8 *r, s32 mode);
extern void Renderer_AllocVram(u8 *r);
extern const u8 *gl2d_image(u32 block);
extern const u8 *gl2d_image_rgba(u32 block, const u8 *rgba, s32 w, s32 h);   /* PC: a 32-bit image kept at `block` */
extern void Renderer_WaitChain(u8 *r);
extern void Renderer_SendFinal(u8 *r);
extern void Renderer_NextClear(u8 *r);
extern void Renderer_EndFrame(u8 *r);
extern s32 Renderer_FillScreen(VObject *r, u32 rgba);

/* ---- (was texcache.h) ---- */

/* The texture cache (global gTexCache): textures by id within groups (a group is a VRAM area
 * a screen owns, e.g. 0x18 / 0x19 the sub screen's), uploaded through the renderer. */

/* texcache.c */
extern u64 TexCache_Tex0(s32 sel);   /* TEX0 of a texture */

/* renderer.c */
extern void Renderer_Call5C(void);
extern void *Renderer_dtor(u8 *o, s32 flags);
extern void *Vram_dtor(u8 *o, s32 flags);

/* The VRAM slot of texture `id` of `group` and its header (*tex), uploading it into renderer
 * layer `layer` first if it isn't resident (slot bit 31); -1 if it isn't cached or the upload
 * has no room. The original inlines this before every textured sprite. */
static inline s32 TexCache_Resident(s32 id, s32 group, s32 layer, u8 **tex) {
    VObject *tc = gTexCache;
    s32 slot = VCALL(tc, 0x8, s32 (*)(VObject *, s32, s32))(tc, id, group);

    if (slot == -1) {
        return -1;
    }
    *tex = VCALL(tc, 0xC, u8 *(*)(VObject *, s32, s32))(tc, id, group);
    if (slot & 0x80000000) {
        slot &= 0x7FFFFFFF;
        if (!(u8)VCALL(gRenderer, 0x44, s32 (*)(VObject *, s32, void *, s32))(gRenderer, slot, *tex, layer)) {
            return -1;
        }
    }
    return slot;
}

/* ---- (was overlay.h) ---- */

/* overlay.c: what other files call. */

/* overlay.c */
extern void Overlay_SetColor(void *ov, u32 rgba);   /* an overlay's colour */
extern void Bloom_Start(u8 *o, u32 rgba, s32 layer, s32 sub);

#endif /* RENDERER_H */
