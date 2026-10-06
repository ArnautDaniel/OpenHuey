/* The renderer (system +0x460, vtable 0x46AC50, global gRenderer). It builds PS2 DMA / GIF
 * packets in double-buffered arenas inside itself; the PC build interprets those packets where
 * the PS2 would send them (libdma / libgraph), so this code stays as the game had it.
 *
 * (was vram.c) VRAM / texture manager (system +0x30CF40, vtable 0x46B050, global gVram): hands
 * out areas of the PS2's 4 MB of video memory (the renderer's layers, textures, CLUTs).
 *
 * (was texcache.c) Texture cache (Game +0x14E8C90, vtable 0x46B1D0, global gTexCache): up to 64
 * registered textures (groups of a .TEX file each) share the renderer's 10 texture layers
 * (+0x304).
 *
 * (was overlay.c) Overlay: a full-screen colour rectangle drawn in a renderer layer (the boot
 * scene dims the screen behind its dialogs with one). vtable Overlay_vtable: +0x8 dtor, +0xC
 * draw. +0x08 u64 stamp (TexCache_Tex0, refreshed when +0x24 changes) +0x10 s32 +0x24 when the
 * stamp was taken +0x70 .. +0xE0 camera vectors: +0x70 eye (camera +0x24), +0x80 / +0x90 camera
 * axes (+0xA4 / +0xA0), +0xA0 their cross product, +0xB0 .. +0xD0 scratch, +0xE0 a point in front
 * of the camera (camera +0x20 + 1.4 cross + 1.05 axis). Computed but the rectangle itself is
 * drawn in screen space. +0xF8 u32 colour (RGBA, alpha in the top byte)
 */
#include "common.h"
#include "gl2d.h"
#include "game.h"
#include "globals.h"
#include "actor.h"
#include "renderer.h"
#include "draw_leaves.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif
#include "libc.h"
#include "msl.h"
#include "sce/eekernel.h"
#include "sce/libvu0.h"
#include "navmesh.h"
#include "memcard.h"
#include "pursuer.h"
#include "charaction.h"
#include "input.h"
#include "effects.h"
#include "fiona.h"
#include "hewie.h"
#include "model.h"
#include "scene_game.h"
#include "heap.h"
#include "sound.h"
#include "progress.h"
#include "creature.h"
#include "vecmath.h"
#include "effectmgr.h"
#include "daniella.h"
#include "loader.h"
#include "pad.h"
#include "scene.h"
#include "scene_boot.h"
#include "scene_title.h"
#include "system.h"
#include "text.h"
#include "sce/iop.h"
#include "music.h"
#include "director.h"
#include "doors.h"
#include "room.h"
#include "movie.h"
#include "placed.h"
#include "room_map.h"
#include "lights.h"
#include "sce/intc.h"
#include "cri/adx.h"
#include "sce/libmc.h"
#include "sce/libpad2.h"
#include "sce/sif.h"
#include "ps2hw.h"
#include "gs.h"
#include "ptmf.h"

extern void *Bloom_vtable[];
extern void *Helper469D00_vtable[];
void *Bloom_dtor(u8 *o, s32 flags);

extern void *TexCache_vtable[];
extern void *D_0046B1F0[];
void *TexCache_dtor(u8 *o, s32 flags);

/* the rectangle's corners: x, y (0 / 1: 512 units from the 0x700 origin), kick */
extern s32 D_00414310[4][3];
void Overlay_DrawRect(void *ov);
#define V(p, off) ((f32 *)((u8 *)(p) + (off)))

void Overlay_DrawRect(void *ov);
s32 Overlay_Draw(void *ov);
s32 Bloom_Draw(u8 *o);

extern void *Renderer_vtable[];
extern void *D_0046ACF0[];
extern void *D_0046AF20[];
extern void *Vram_vtable[];

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

s32 TexCache_Layer(TexCache *c, s32 sel, s32 group);
u8 *TexCache_Entry(TexCache *c, s32 sel, s32 group);
void TexCache_AddGroup(TexCache *c, u32 *tex, s32 group);
void TexCache_RemoveGroup(TexCache *c, s32 group);
void TexCache_ForgetLayers(TexCache *c);

extern u8 D_003C5D90[];
extern u8 D_003C6348[];
extern u8 D_003C6450[];
extern u8 D_003D5B80[];
extern u8 str_ADXENC_DLL_Ver_1_09_Nov_12_2004[];
extern u8 D_004555C8[];
extern u8 str_ROFS_Ver_1_76_Build_Sep_3_2004_17_20_10[];
extern u8 D_00455C38[];
extern u8 str_ROCI_Ver_1_15_Build_Sep_3_2004_17_20_13[];
extern u8 str_RSU_Ver_1_10_Build_Sep_3_2004_17_20_13[];
extern u8 D_0047E878[];
extern s32 Vram_FitUpper(u8 *v, u16 *psm, s32 w, s32 h);       /* ... in 24-bit pages' upper bits */
extern s32 Vram_PagesAt(u8 *v, u32 addr, u16 *psm, s32 w, s32 h);   /* ... at a given address */
extern s32 Vram_AllocClut(u8 *v, s32 clutpsm);                  /* allocate a CLUT slot (s16, -1) */
extern u32 Vram_PageCount(u8 *v, s32 psm, u32 w, u32 h);        /* size in pages */
extern u8 D_003B3040[][2];   /* where CLUT n goes in a CLUT block, 16-colour CLUTs (x, y) */
extern u8 D_003B3050[][2];   /* ... 256-colour (psm 2) */
/* allocation entry (0x12 bytes, 64 at +0x98) */
typedef struct VramEntry {
    /* 0x00 */ u8 used;
    /* 0x01 */ u8 locked;    /* kept by +0x20 */
    /* 0x02 */ s16 w;
    /* 0x04 */ s16 h;
    /* 0x06 */ s16 psm;      /* 0xFF: CLUT only */
    /* 0x08 */ s16 page;     /* first page (texture pages, or upper-bit pages for 8/4-bit H formats) */
    /* 0x0A */ s16 npages;
    /* 0x0C */ s16 cpsm;     /* CLUT format, 0xFF none */
    /* 0x0E */ s16 cregion;  /* CLUT region (page from 0xFC000) */
    /* 0x10 */ s16 cslot;    /* slot in the region */
} VramEntry;

/* CLUT region (8 at +4): CLUT format (0xFF: free), which slots are taken, how many, of how many
 * (16 for 16-bit CLUTs, 8 for 32-bit) */
typedef struct VramClutRegion {
    u16 psm;
    u16 mask;
    u16 count;
    u16 capacity;
} VramClutRegion;

#define VRAM_CLUT_REGION(v, i) ((VramClutRegion *)((v) + 4) + (i))

#define VRAM_PAGEMAP(v) ((u32 *)((v) + 0x44))     /* 1 bit per page */

#define VRAM_PAGEMAP_HI(v) ((u32 *)((v) + 0x54))  /* 2 bits per page: 8 / 4-bit formats in the

#define VRAM_HI_PAGES 0x110   /* pages below 0x88000 (the 24-bit frame buffers' upper bits) */

#define VRAM_PAGES 0x78       /* texture pages from 0xC0000 */

#define HI_FREE(v, p, m) (!(VRAM_PAGEMAP_HI(v)[(u32)(p) * 2 >> 5] & ((m) << ((u32)(p) * 2 & 0x1F))))

#define HI_TAKE(v, p, m) (VRAM_PAGEMAP_HI(v)[(u32)(p) * 2 >> 5] |= (m) << ((u32)(p) * 2 & 0x1F))

#define PSM_NONE 0xFF
#define PSMT8H 0x1B
#define PSMT4HL 0x24
#define PSMT4HH 0x2C

#define VRAM_HI_PAGES 0x110   /* pages below 0x88000 (the 24-bit frame buffers' upper bits) */

#define VRAM_ENTRY(v, id) ((VramEntry *)((v) + 0x98) + (id))

/* a .TEX file's texture entry */
typedef struct TexHeader_vram {
    u8 psm, cpsm, pad[2];
    u16 w, h, imageQwc, clutQwc;
    s32 data;
} TexHeader_vram;

/* log2 of a texture size, rounded up (at most 10) */
static inline u64 Vram_Log2(u32 n) {
    u32 p = 1;
    u64 l = 0;

    while (p < n) {
        l++;
        p *= 2;
        if (l >= 11) {
            break;
        }
    }
    return l;
}

/* GS TEX0 for entry `id` (format psm, w x h, CLUT format cpsm): TCC on; `high` the bits from 55 */
static inline u64 Vram_Tex0(u8 *v, s32 id, u32 psm, u32 w, u32 h, u32 cpsm, u64 high) {
    VramEntry *e = VRAM_ENTRY(v, id);
    u32 slots = (u16)e->cpsm == 2 ? 16 : 8;
    u64 th = Vram_Log2(h), tw;
    u32 tbp;

    if ((u16)e->psm != PSMT8H && (u16)e->psm != PSMT4HL && (u16)e->psm != PSMT4HH) {
        tbp = (e->page << 11) + 0xC0000;
    } else {
        tbp = e->page << 11;
    }
    tw = Vram_Log2(w);
    return high | (u64)(cpsm & 0xFFFF) << 51 |
           (u64)(s64)(s32)(((e->cregion << 11) + 0xFC000 + (u32)((u16)e->cslot << 11) / slots) >> 6) << 37 |
           th << 30 | (u64)(psm & 0xFFFF) << 20 | (u64)(s64)(s32)((w + 63) >> 6) << 14 |
           (u64)(s64)(s32)(tbp >> 6) | tw << 26 | (u64)4 << 32;
}

/* +0x50 / +0x4C load .TEX file `name` (into a buffer of whole sectors, 64-byte aligned) and
 * allocate its first texture through +0x5C, upper bits or not; the entry, -1 if none */
static inline s32 vram_load_tex(VObject *v, const char *name, s32 upper) {
    VObject *ld = gFileLoader;
    s32 size = VCALL(ld, 0x30, s32 (*)(VObject *, const char *))(ld, name);
    u8 *buf = func_00100550((size + 0x7FF) / 0x800 * 0x800 + 0x3F);
#ifdef HG_NATIVE
    u8 *tex = (u8 *)((u32)(buf + 0x3F) / 64 * 64);   /* (PC heaps can be above 2 GB) */
#else
    u8 *tex = (u8 *)((s32)(buf + 0x3F) / 64 * 64);
#endif
    s32 id;

    VCALL(ld, 0x34, void (*)(VObject *, const char *, void *))(ld, name, tex);
    if (tex == NULL || AT(tex, 0x0, u32) == 0) {
        id = -1;
    } else {
        id = VCALL(v, 0x5C, s32 (*)(VObject *, TexHeader_vram *, s32))(v, (TexHeader_vram *)(tex + 0x10), upper);
    }
    func_00100470(buf);
    return id;
}

void Vram_Init(u8 *v);
void *AdxEnc_Version(void);
void *Adx_Data47E878(void);
void *Adx_Data3C5D90(void);
void *Adx_Data3C6348(void);
void *Adx_Data3C6450(void);
void *Adx_Data4555C8(void);
void *Rofs_Version(void);
void *Adx_Data455C38(void);
void *Roci_Version(void);
void *Adx_Data3D5B80(void);
void *Rsu_Version(void);
s32 Vram_AllocAt(u8 *v, s32 addr, s32 psm_, s32 w, s32 h, s32 clutpsm_);
s32 Vram_Alloc(VObject *v, s32 psm, s32 w, s32 h, s32 clutpsm);
void Vram_Reset(VObject *v);
void Vram_KeepResident(u8 *v, s32 id);
void Vram_Free(u8 *v, s32 id);
void Vram_FreeAll(VObject *v);
s32 Vram_AllocClut(u8 *v, s32 clutpsm_);
u32 Vram_PageCount(u8 *v, s32 psm, u32 w, u32 h);
s32 Vram_FitPages(u8 *v, s32 psm, s32 w, s32 h);
s32 Vram_FitUpperHalf(u8 *v, s32 n, s32 m);
s32 Vram_FitUpperBoth(u8 *v, s32 n);
s32 Vram_FitUpper(u8 *v, u16 *psm, s32 w, s32 h);
s32 Vram_PagesAt(u8 *v, u32 addr, u16 *psm, s32 w, s32 h);
u32 Vram_ClutAddr(u8 *v, s32 id);
u32 Vram_TexAddr(u8 *v, s32 id);
u16 Vram_Height(u8 *v, s32 id);
u16 Vram_Width(u8 *v, s32 id);
s32 Vram_AllocTexture(VObject *v, TexHeader_vram *t, s32 upper);
s32 Vram_TexUpper(VObject *v, u8 *tex, u32 n);
s32 Vram_Tex(VObject *v, u8 *tex, u32 n);
s32 Vram_LoadTexUpper(VObject *v, const char *name);
s32 Vram_LoadTex(VObject *v, const char *name);
u64 Vram_Tex0Load(u8 *v, s32 id, s32 psm, u32 w, u32 h, s32 cpsm);
u64 Vram_Tex0Indexed(u8 *v, s32 id, u32 w, u32 h, s32 cpsm);
u64 Vram_Tex0Csa(u8 *v, s32 id, u32 csa, s32 psm, u32 w, u32 h, s32 cpsm);
u64 Vram_Tex2(u8 *v, s32 id, u32 csa, s32 psm, s32 cpsm);
u64 Vram_EntryTex0(u8 *v, s32 id);
u64 Vram_EntryTex0Indexed(u8 *v, s32 id);
u64 Vram_EntryTex0Csa(u8 *v, s32 id, u32 csa);
u64 Vram_EntryTex2(u8 *v, s32 id, u32 csa);
void Vram_Upload(u8 *v, s32 slot, const void *pix, const void *clut);

/* destructor (vtable Renderer_vtable) */
/* 0x001AAE10 */
void *Renderer_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Renderer_vtable;
        AT(o, 0x0, void **) = D_0046ACF0;
        gRenderer = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}
/* +0x1C */
/* 0x001BBB20 */
void Renderer_Set304DE0(u8 *r) {
    AT(r, 0x304DE0, u8) = 1;
}

/* +0x18 allocate `n` quadwords from the current buffer of arena 2 (+0x2806D0 + buffer * 0x80000);
 * NULL when full */
/* 0x001BBB40 */
void *Renderer_AllocArena2(u8 *r, s32 n) {
    u8 *cur = AT(r, 0x304BAC, u8 *);
    u8 *next = cur + n * 16;

    if ((u8 *)r + 0x2806D0 + (AT(r, 0x304BB5, u8) << 19) < next) {
        return NULL;
    }
    AT(r, 0x304BAC, u8 *) = next;
    return cur;
}

/* +0x14 allocate `n` quadwords from the current buffer of arena 1 (+0x1006C0 + buffer * 0x100000),
 * keeping one quadword spare; NULL when full */
/* 0x001BBBB0 */
void *Renderer_AllocArena1(u8 *r, s32 n) {
    u8 *cur = AT(r, 0x304BA8, u8 *);
    u8 *next = cur + n * 16;

    if (!(next + 0x10 < (u8 *)r + 0x1006C0 + (AT(r, 0x304BB4, u8) << 20))) {
        return NULL;
    }
    AT(r, 0x304BA8, u8 *) = next;
    return cur;
}

/* +0x3C id of render layer `i` (0..10), -1 if out of range */
/* 0x001BB950 */
s32 Renderer_LayerId(u8 *r, u32 i) {
    if (i < 11) {
        return AT(r, 0x304BB8 + i * 4, s32);
    }
    return -1;
}

extern void Renderer_ClearVram(u8 *r);
extern void Renderer_GsEnvironments(u8 *r);
extern void Renderer_Flip(u8 *r);
extern void Renderer_DefaultGsState(u8 *r);

#define RNG_REAL1() VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom)

/* Set up the graphics for video mode `mode` (2: 448 lines), then a table of 16 random
 * (x, y, ..., angle) entries (renderer +0x304C0C). */
/* 0x001B83D0 */
void Renderer_SetupVideo(u8 *r, s32 mode) {
    f32 *e;

    func_0010BFB0();
    func_0010BE10(0, 1, (u8)mode, 0);
    Renderer_ClearVram(r);
    AT(r, 0x304BFE, s16) = (u8)mode == 2 ? 448 : 512;
    AT(r, 0x304C09, u8) = mode;
    Renderer_GsEnvironments(r);
    Renderer_Flip(r);
    for (e = (f32 *)(r + 0x304C0C); e <= (f32 *)(r + 0x304D38); e += 5) {
        e[0] = -16.0f + 80.0f * (RNG_REAL1() - 0.5f);
        e[1] = -150.0f * RNG_REAL1();
        e[2] = 32.0f + 32.0f * RNG_REAL1();
        e[3] = 32.0f + 32.0f * RNG_REAL1();
        e[4] = 0x1.921fb6p+1f /* pi */ * (360.0f * (RNG_REAL1() - 0.5f)) / 180.0f;
    }
}

/* Allocate the renderer's VRAM (allocator +0x18: address, pixel format 0x13 = 8-bit indexed,
 * width, height): one 0xFF area (+0x304BE4) and 11 layers (+0x304BB8: 10 of 256x256 below
 * 0xFC000, one of 512x512 at 0xC0000), each kept resident (+0x24). */
/* 0x001B8250 */
void Renderer_AllocVram(u8 *r) {
    VObject *v;
    u32 i;

    if (AT(r, 0x304BE4, s32) < 0) {
        v = gVram;
        AT(r, 0x304BE4, s32) = VCALL(v, 0x14, s32 (*)(VObject *, s32, s32, s32, s32))(v, 0xFF, 0, 0, 0);
        VCALL(v, 0x24, void (*)(VObject *, s32))(v, AT(r, 0x304BE4, s32));
    }
    v = gVram;
    for (i = 0; i < 11; i++) {
        s32 *id = &AT(r, 0x304BB8 + i * 4, s32);

        if (*id < 0) {
            if (i < 10) {
                *id = VCALL(v, 0x18, s32 (*)(VObject *, s32, s32, s32, s32, s32))(v, 0xFC000 - ((i + 2) << 14), 0x13, 0x100, 0x100, 0);
            } else {
                *id = VCALL(v, 0x18, s32 (*)(VObject *, s32, s32, s32, s32, s32))(v, 0xC0000, 0x13, 0x200, 0x200, 0);
            }
            VCALL(v, 0x24, void (*)(VObject *, s32))(v, *id);
        }
    }
}


extern u32 *_fbss;   /* GIF DMA channel registers (sceDmaGetChan(2)) */

#ifdef HG_NATIVE
/* Clear all of VRAM (the original: 16 uploads of 256 KB of zeros): PC has no VRAM. */
/* 0x001B7ED0 */
void Renderer_ClearVram(u8 *r) {
    (void)r;
}
#endif

/* set the 9-bit base field (FBP / ZBP) of a GS register to VRAM byte address `addr` */
#define GS_SET_BASE(reg, addr) ((reg) = ((reg) & ~0x1FF) | (((addr) / 2048) & 0x1FF))

#ifdef HG_NATIVE
/* The GS display and drawing environments (two setups, from the VRAM layout): nothing on PC,
 * where the GL renderer owns the frame. */
/* 0x001B79F0 */
void Renderer_GsEnvironments(u8 *r) {
    (void)r;
}
#endif

#ifdef HG_NATIVE
/* The default GS state at each frame's start (blending, alpha / Z test, the frame and Z
 * buffers): the GL renderer sets its own per draw. */
/* 0x001B71E0 */
void Renderer_DefaultGsState(u8 *r) {
    (void)r;
}
#endif

/* the draw buffer (0 / 1) and its DMA chain: 53 layer slots, then an end tag */
#define REND_BUF(r) AT(r, 0x304BB4, u8)
#define REND_CHAIN(r, b) ((r) + (b) * 0x360 + 0x10)

/* Flip to the other draw buffer: rebuild its chain of 53 layer slots (each a DMA "next" tag to
 * the following slot; layers append their packets after their slot), reset its packet arena;
 * the second arena flips too when requested (+0x304DE0); then the frame-start state. The DMA
 * address field is 28 bits: on PC the build is 32-bit and its data lies below 0x10000000. */
/* 0x001B7370 */
void Renderer_Flip(u8 *r) {
    s32 i;

#ifdef HG_NATIVE
    {
        extern void glr_end_frame(void);   /* native/platform/glr.c */

        glr_end_frame();
    }
#endif
    REND_BUF(r) ^= 1;
    for (i = 0; i < 53; i++) {
        u64 *tag = (u64 *)(REND_CHAIN(r, REND_BUF(r)) + i * 16);

        tag[0] = DMA_TAG(DMA_NEXT, 0, (u32)(REND_CHAIN(r, REND_BUF(r)) + (i + 1) * 16) & 0x0FFFFFFF);
        tag[1] = 0;
        AT(r, 0x304A00 + REND_BUF(r) * 0xD4 + i * 4, u8 *) = REND_CHAIN(r, REND_BUF(r)) + i * 16;
    }
    AT(REND_CHAIN(r, REND_BUF(r)), 53 * 16, u64) = DMA_TAG(DMA_END, 0, 0);
    AT(REND_CHAIN(r, REND_BUF(r)), 53 * 16 + 8, u64) = 0;
    AT(r, 0x304BA8, u8 *) = r + (REND_BUF(r) << 20) + 0x6D0;
    if (AT(r, 0x304DE0, u8)) {
        AT(r, 0x304BB5, u8) ^= 1;
        AT(r, 0x304BAC, u8 *) = r + (AT(r, 0x304BB5, u8) << 19) + 0x2006D0;
        func_00115D20(r + 0x300A00, 0, 0x4000);
        AT(r, 0x304DE0, u8) = 0;
    }
    Renderer_DefaultGsState(r);
    AT(r, 0x304BB6, u8) = 0;
    AT(r, 0x304C05, u8) = 0;
    for (i = 0; i < 16; i++) {
        AT(r, 0x304D5C + i * 8, s32) = -1;
        AT(r, 0x304D60 + i * 8, s32) = 0;
    }
}

extern s32 Renderer_3DBegin(u8 *r);   /* layer 38 setup (u8) */

#define REND_LAYER_TAIL(r, l) AT(r, 0x304A00 + REND_BUF(r) * 0xD4 + (l) * 4, u64 *)

/* +0x10 add a packet of `n` quadwords to layer `layer` (0..52) of this frame: allocated from
 * arena 1, linked after the layer's last packet, followed by a tag back to the next layer.
 * The first packet of layer 6 / 38 runs that layer's setup first. NULL if it can't be added. */
/* 0x001BBC20 */
u64 *Renderer_AddPacket(u8 *r, s32 n, s32 layer) {
    u64 *p, *back;

    if (n <= 0 || layer >= 53) {
        return NULL;
    }
    if (layer == 6) {
        if (REND_LAYER_TAIL(r, 6) == (u64 *)(REND_CHAIN(r, REND_BUF(r)) + 6 * 16) && !(u8)func_001B4F30(r)) {
            return NULL;
        }
    } else if (layer == 38) {
        if (REND_LAYER_TAIL(r, 38) == (u64 *)(REND_CHAIN(r, REND_BUF(r)) + 38 * 16) && !(u8)Renderer_3DBegin(r)) {
            return NULL;
        }
    }
    p = VCALL(r, 0x14, u64 *(*)(u8 *, s32))(r, n);
    if (p == NULL) {
        return NULL;
    }
    REND_LAYER_TAIL(r, layer)[0] = DMA_TAG(DMA_NEXT, 0, (u32)p & 0x0FFFFFFF);
    REND_LAYER_TAIL(r, layer)[1] = 0;
    back = AT(r, 0x304BA8, u64 *);
    back[0] = DMA_TAG(DMA_NEXT, 0, (u32)(REND_CHAIN(r, REND_BUF(r)) + (layer + 1) * 16) & 0x0FFFFFFF);
    AT(r, 0x304BA8, u64 *)[1] = 0;
    REND_LAYER_TAIL(r, layer) = AT(r, 0x304BA8, u64 *);
    AT(r, 0x304BA8, u8 *) += 16;
    return p;
}

/* Make layer `layer`'s state packet: for these layers, the first draw of a frame runs a setup
 * function that fills the layer before (layer - 1). If the setup fails, the arena is rolled
 * back and the layers before and after are skipped (their slots chain straight on); 0 is
 * returned. Layers 15 and 26 run their own checks. Other layers need nothing. */
static s32 layer_begin(u8 *r, s32 layer, s32 (*setup)(u8 *r), u8 *saved) {
    u8 *chain;

    chain = REND_CHAIN(r, REND_BUF(r));
    if (REND_LAYER_TAIL(r, layer - 1) != (u64 *)(chain + (layer - 1) * 16) || (u8)setup(r)) {
        return 1;
    }
    AT(r, 0x304BA8, u8 *) = saved;
    AT(REND_CHAIN(r, REND_BUF(r)), (layer - 1) * 16, u64) =
        DMA_TAG(DMA_NEXT, 0, (u32)(REND_CHAIN(r, REND_BUF(r)) + layer * 16) & 0x0FFFFFFF);
    AT(REND_CHAIN(r, REND_BUF(r)), (layer - 1) * 16 + 8, u64) = 0;
    REND_LAYER_TAIL(r, layer - 1) = (u64 *)(REND_CHAIN(r, REND_BUF(r)) + (layer - 1) * 16);
    AT(REND_CHAIN(r, REND_BUF(r)), (layer + 1) * 16, u64) =
        DMA_TAG(DMA_NEXT, 0, (u32)(REND_CHAIN(r, REND_BUF(r)) + (layer + 2) * 16) & 0x0FFFFFFF);
    AT(REND_CHAIN(r, REND_BUF(r)), (layer + 1) * 16 + 8, u64) = 0;
    REND_LAYER_TAIL(r, layer + 1) = (u64 *)(REND_CHAIN(r, REND_BUF(r)) + (layer + 1) * 16);
    return 0;
}

/* 0x001B5EC0 */
s32 Renderer_LayerBegin(u8 *r, s32 layer) {
    u8 *saved = AT(r, 0x304BA8, u8 *);

    switch (layer) {
    case 0x1A: return (u8)func_001AB3F0(r) ? 1 : 0;
    case 0x1C: return layer_begin(r, layer, func_001AB960, saved);
    case 0x23: return layer_begin(r, layer, func_001AC0D0, saved);
    case 0x14: return layer_begin(r, layer, func_001AF3B0, saved);
    case 0x17: return layer_begin(r, layer, func_001B0D40, saved);
    case 0x0F: return (u8)func_001B18E0(r) ? 1 : 0;
    case 0x26: return layer_begin(r, layer, Renderer_3DBegin, saved);
    case 0x11: return layer_begin(r, layer, func_001B2160, saved);
    case 0x0D: return layer_begin(r, layer, func_001B4330, saved);
    case 0x06: return layer_begin(r, layer, func_001B4F30, saved);
    }
    return 1;
}

extern void func_001B6CD0(u8 *r, u64 *packet, void *arg);

/* +0xC draw `obj` into layer `layer` (0..52): its +0xC method writes its packets at the arena
 * cursor, which are then linked into the layer (layer 10 with `arg` goes through
 * func_001B6CD0 instead). 0 if the layer can't be drawn or the object drew nothing; layers 15
 * and 26 finish with their own step. */
/* 0x001BBE60 */
s32 Renderer_Draw(u8 *r, void *obj, s32 layer, void *arg) {
    u64 *start;

    if (obj == NULL || layer >= 53 || !(u8)Renderer_LayerBegin(r, layer)) {
        return 0;
    }
    start = AT(r, 0x304BA8, u64 *);
#ifdef HG_NATIVE
    {
        extern void glr_layer(s32 layer);   /* native/platform/glr.c: what glr is sent is in `layer` */
        s32 ok;

        glr_layer(layer);
        ok = (u8)VCALL(obj, 0xC, s32 (*)(void *))(obj);
        glr_layer(-1);
        if (!ok) {
            AT(r, 0x304BA8, u64 *) = start;
            return 0;
        }
    }
#else
    if (!(u8)VCALL(obj, 0xC, s32 (*)(void *))(obj)) {
        AT(r, 0x304BA8, u64 *) = start;
        return 0;
    }
#endif
    if (layer == 10 && arg != NULL) {
        func_001B6CD0(r, start, arg);
    } else {
        u64 *back;

        REND_LAYER_TAIL(r, layer)[0] = DMA_TAG(DMA_NEXT, 0, (u32)start & 0x0FFFFFFF);
        REND_LAYER_TAIL(r, layer)[1] = 0;
        back = AT(r, 0x304BA8, u64 *);
        back[0] = DMA_TAG(DMA_NEXT, 0, (u32)(REND_CHAIN(r, REND_BUF(r)) + (layer + 1) * 16) & 0x0FFFFFFF);
        AT(r, 0x304BA8, u64 *)[1] = 0;
        REND_LAYER_TAIL(r, layer) = AT(r, 0x304BA8, u64 *);
        AT(r, 0x304BA8, u8 *) += 16;
    }
    if (layer == 0x1A) {
        return (u8)func_001AAE80(r) != 0;
    }
    if (layer == 0x0F) {
        return (u8)func_001B1370(r) != 0;
    }
    return 1;
}

/* destructor (vtable Vram_vtable) */
/* 0x001BF880 */
void *Vram_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Vram_vtable;
        AT(o, 0x0, void **) = D_0046AF20;
        gVram = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* Pages at VRAM byte address `addr`: below 0x88000 upper-bit pages (4-bit: lower half if free,
 * else upper; 8-bit: both), 0xC0000..0xFC000 texture pages. -1 if taken or elsewhere. */
/* 0x001BF8F0 */
s32 Vram_PagesAt(u8 *v, u32 addr, u16 *psm, s32 w, s32 h) {
    s32 n, start, okLo, okHi;
    u32 p, end;

    if (addr < 0x88000) {
        switch (*psm) {
        case PSMT4HL: case PSMT4HH: case 0x14:
            *psm = PSMT4HH;
            n = Vram_PageCount(v, *psm, w, h);
            start = (s16)(addr >> 11);
            end = start + n;
            okLo = 1;
            for (p = start; p < end; p++) {
                if (!HI_FREE(v, p, 1) || p >= VRAM_HI_PAGES) {
                    okLo = 0;
                    break;
                }
            }
            okHi = 1;
            for (p = start; p < end; p++) {
                if (!HI_FREE(v, p, 2) || p >= VRAM_HI_PAGES) {
                    okHi = 0;
                    break;
                }
            }
            if (okLo) {
                *psm = PSMT4HL;
                if (start != -1) {
                    for (p = start; p < (u32)(start + n); p++) {
                        HI_TAKE(v, p, 1);
                    }
                }
                return start;
            }
            if (okHi) {
                *psm = PSMT4HH;
                if (start != -1) {
                    for (p = start; p < (u32)(start + n); p++) {
                        HI_TAKE(v, p, 2);
                    }
                }
                return start;
            }
            return -1;
        case 0x13: case PSMT8H:
            *psm = PSMT8H;
            n = Vram_PageCount(v, *psm, w, h);
            start = (s16)(addr >> 11);
            end = start + n;
            for (p = start; p < end; p++) {
                if (!HI_FREE(v, p, 3) || p >= VRAM_HI_PAGES) {
                    return -1;
                }
            }
            if (start != -1) {
                for (p = start; p < (u32)(start + n); p++) {
                    HI_TAKE(v, p, 3);
                }
            }
            return start;
        }
        return -1;
    }
    if (addr >= 0xC0000 && addr < 0xFC000) {
        n = Vram_PageCount(v, *psm, w, h);
        start = (s16)((addr - 0xC0000) >> 11);
        end = start + n;
        for (p = start; p < end; p++) {
            if ((VRAM_PAGEMAP(v)[p >> 5] & (1 << (p & 0x1F))) || p >= VRAM_PAGES) {
                return -1;
            }
        }
        if (start != -1) {
            for (p = start; p < (u32)(start + n); p++) {
                VRAM_PAGEMAP(v)[p >> 5] |= 1 << (p & 0x1F);
            }
        }
        return start;
    }
    return -1;
}

/* +0x6C CLUT address of entry `id` (64-word blocks): CLUT pages from 0xFC000, 16 slots per page
 * for 16-bit CLUTs, 8 for 32-bit */
/* 0x001BFDB0 */
u32 Vram_ClutAddr(u8 *v, s32 id) {
    VramEntry *e = VRAM_ENTRY(v, id);

    return ((e->cregion << 11) + 0xFC000 + (u32)((u16)e->cslot << 11) / ((u16)e->cpsm == 2 ? 16 : 8)) >> 6;
}

/* +0x68 texture address of entry `id` (64-word blocks) */
/* 0x001BFE10 */
u32 Vram_TexAddr(u8 *v, s32 id) {
    VramEntry *e = VRAM_ENTRY(v, id);
    u16 psm = e->psm;

    if (psm != PSMT8H && psm != PSMT4HL && psm != PSMT4HH) {
        return (u32)((e->page << 11) + 0xC0000) >> 6;
    }
    return (u32)(e->page << 11) >> 6;
}

/* +0x64 / +0x60 height / width of entry `id` */
/* 0x001BFE80 */
u16 Vram_Height(u8 *v, s32 id) { return VRAM_ENTRY(v, id)->h; }

/* 0x001BFEA0 */
u16 Vram_Width(u8 *v, s32 id) { return VRAM_ENTRY(v, id)->w; }

/* +0x5C allocate room for texture `t` and fill it (+0x48); `upper`: 8 / 4-bit textures go to the
 * frame buffers' upper bits. The entry, -1 if there's no room. */
/* 0x001BFEC0 */
s32 Vram_AllocTexture(VObject *v, TexHeader_vram *t, s32 upper) {
    u16 psm;
    s32 id;
    u8 *image, *clut;

    if (t == NULL) {
        return -1;
    }
    psm = t->psm;
    if (upper != 0 && (u32)(psm - 0x13) < 2) {
        psm = psm == 0x14 ? PSMT4HL : PSMT8H;
    }
    id = VCALL(v, 0x14, s32 (*)(VObject *, s32, s32, s32, s32))(v, (u8)psm, t->w, t->h, t->cpsm);
    if (id != -1) {
        image = NULL;
        clut = NULL;
        if (t->imageQwc != 0) {
            image = (u8 *)t + t->data;
        }
        if (t->clutQwc != 0) {
            clut = t->imageQwc != 0 ? image + t->imageQwc * 16 : (u8 *)t + t->data;
        }
        VCALL(v, 0x48, void (*)(VObject *, s32, u8 *, u8 *))(v, id, image, clut);
    }
    return id;
}

/* +0x58 / +0x54 texture `n` of a loaded .TEX file (a count, then 0x10-byte entries) through
 * +0x5C, upper bits or not; -1 if there's none */
/* 0x001BFFC0 */
s32 Vram_TexUpper(VObject *v, u8 *tex, u32 n) {
    if (tex == NULL || n >= AT(tex, 0x0, u32)) {
        return -1;
    }
    return VCALL(v, 0x5C, s32 (*)(VObject *, TexHeader_vram *, s32))(v, (TexHeader_vram *)(tex + 0x10 + n * 0x10), 1);
}

/* 0x001C0020 */
s32 Vram_Tex(VObject *v, u8 *tex, u32 n) {
    if (tex == NULL || n >= AT(tex, 0x0, u32)) {
        return -1;
    }
    return VCALL(v, 0x5C, s32 (*)(VObject *, TexHeader_vram *, s32))(v, (TexHeader_vram *)(tex + 0x10 + n * 0x10), 0);
}

/* 0x001C0080 */
s32 Vram_LoadTexUpper(VObject *v, const char *name) {
    return vram_load_tex(v, name, 1);
}

/* 0x001C0190 */
s32 Vram_LoadTex(VObject *v, const char *name) {
    return vram_load_tex(v, name, 0);
}

/* upload slot `slot`'s texture `pix` and its CLUT `clut` (either NULL: not sent) at once
 * (slots of 0x12 bytes at +0x98: used, w, h, psm, page, CLUT psm (0xFF none), CLUT page, index) */
/* 0x001C02A0 */
void Vram_Upload(u8 *v, s32 slot, const void *pix, const void *clut) {
    u8 li[0x60] __attribute__((aligned(16)));
    u8 *e;
    u16 cpsm;

    if (slot < 0) {
        return;
    }
    e = v + 0x98 + slot * 0x12;
    if (!e[0]) {
        return;
    }
    if (pix != NULL) {
        u16 psm = AT(e, 0x6, u16);
        u32 tbp;

        if (psm == 0x1B || psm == 0x24 || psm == 0x2C) {
            tbp = (u32)(AT(e, 0x8, s16) << 11) >> 6;   /* (in the upper bits of a 24-bit page) */
        } else {
            tbp = (u32)((AT(e, 0x8, s16) << 11) + 0xC0000) >> 6;
        }
        sceGsSetDefLoadImage(li, (s16)tbp, (s16)((AT(e, 0x2, u16) + 63) / 64), (s16)psm, 0, 0,
                             (s16)AT(e, 0x2, u16), AT(e, 0x4, s16));
        FlushCache(0);
        sceGsExecLoadImage(li, pix);
        sceGsSyncPath(0, 0);
    }
    cpsm = AT(e, 0xC, u16);
    if (cpsm == 0xFF || clut == NULL) {
        return;
    }
    {
        u8 *at = cpsm == 2 ? D_003B3050[AT(e, 0x10, u16)] : D_003B3040[AT(e, 0x10, u16)];
        u32 cbp = (u32)((AT(e, 0xE, s16) << 11) + 0xFC000) >> 6;

        sceGsSetDefLoadImage(li, (s16)cbp, 1, (s16)cpsm, (s16)at[0], (s16)at[1], 0x10, 0x10);
        FlushCache(0);
        sceGsExecLoadImage(li, clut);
        sceGsSyncPath(0, 0);
    }
}

/* Size of a w x h texture of format psm in pages (a page is 8 KB: 64x32 at 32 bits, 64x64 at 16,
 * 128x64 at 8, 128x128 at 4); 0 for other formats. */
/* 0x001C04B0 */
u32 Vram_PageCount(u8 *v, s32 psm, u32 w, u32 h) {
    u32 pw, ph;

    switch ((u16)psm) {
    case 0x00: case 0x01: case PSMT8H: case PSMT4HH: case PSMT4HL:
        pw = 64;
        ph = 32;
        break;
    case 0x02:
        pw = 64;
        ph = 64;
        break;
    case 0x13:
        pw = 128;
        ph = 64;
        break;
    case 0x14:
        pw = 128;
        ph = 128;
        break;
    default:
        return 0;
    }
    return (w + pw - 1) / pw * ((h + ph - 1) / ph);
}

/* First fit of n upper-bit pages with bits `m` free (not marked). */
/* 0x001C0570 */
s32 Vram_FitUpperHalf(u8 *v, s32 n, s32 m) {
    s32 start = -1, left = 0;
    u32 p;

    if (n == 0) {
        return -1;
    }
    for (p = 0; p < VRAM_HI_PAGES; p++) {
        if (!HI_FREE(v, p, m)) {
            start = -1;
            continue;
        }
        if (start < 0) {
            left = (u16)n;
            start = p;
        }
        if (--left == 0) {
            return (s16)start;
        }
    }
    return -1;
}

/* First fit of n upper-bit pages with both halves free (8-bit); marks them. */
/* 0x001C0610 */
s32 Vram_FitUpperBoth(u8 *v, s32 n) {
    s32 start = -1, left = 0;
    u32 p, end;

    if (n == 0) {
        return -1;
    }
    for (p = 0; p < VRAM_HI_PAGES; p++) {
        if (!HI_FREE(v, p, 2) || !HI_FREE(v, p, 1)) {
            start = -1;
            continue;
        }
        if (start < 0) {
            left = (u16)n;
            start = p;
        }
        if (--left == 0) {
            end = start + (u16)n;
            for (p = start; p < end; p++) {
                HI_TAKE(v, p, 3);
            }
            return (s16)start;
        }
    }
    return -1;
}

/* Upper-bit pages for a 4 / 8-bit texture: 8-bit takes both halves (PSMT8H); 4-bit takes the
 * lower half (PSMT4HL) or the upper (PSMT4HH), whichever fits first. *psm is set to the choice. */
/* 0x001C08B0 */
s32 Vram_FitUpper(u8 *v, u16 *psm, s32 w, s32 h) {
    u32 n = Vram_PageCount(v, *psm, w, h) & 0xFFFF;
    s32 lo, hi, p;

    if (n == 0) {
        return -1;
    }
    switch (*psm) {
    case PSMT8H:
        return Vram_FitUpperBoth(v, n);
    case PSMT4HL:
    case PSMT4HH:
        lo = (s16)Vram_FitUpperHalf(v, n, 1);
        hi = (s16)Vram_FitUpperHalf(v, n, 2);
        if (lo != -1 && (hi == -1 || hi >= lo)) {
            *psm = PSMT4HL;
            for (p = lo; (u32)p < lo + n; p++) {
                HI_TAKE(v, p, 1);
            }
            return lo;
        }
        if (hi != -1) {
            *psm = PSMT4HH;
            for (p = hi; (u32)p < hi + n; p++) {
                HI_TAKE(v, p, 2);
            }
            return hi;
        }
        return -1;
    }
    return -1;
}

/* First fit of n texture pages; marks them. */
/* 0x001C0AD0 */
s32 Vram_FitPages(u8 *v, s32 psm, s32 w, s32 h) {
    s32 n = Vram_PageCount(v, psm, w, h);
    s32 start = -1, left = 0;
    u32 p, end;

    if (n == 0) {
        return -1;
    }
    for (p = 0; p < VRAM_PAGES; p++) {
        if (VRAM_PAGEMAP(v)[p >> 5] & (1 << (p & 0x1F))) {
            start = -1;
            continue;
        }
        if (start < 0) {
            left = n;
            start = p;
        }
        if (--left == 0) {
            end = start + n;
            for (p = (u16)start; p < end; p++) {
                VRAM_PAGEMAP(v)[p >> 5] |= 1 << (p & 0x1F);
            }
            return (s16)start;
        }
    }
    return -1;
}

/* Allocate a CLUT slot of format clutpsm (0 / 2): region * 16 + slot, or -1. */
/* 0x001C0D40 */
s32 Vram_AllocClut(u8 *v, s32 clutpsm_) {
    u16 clutpsm = clutpsm_;
    VramClutRegion *rg;
    u32 i, j;

    if (clutpsm != 2 && clutpsm != 0) {
        return -1;
    }
    for (i = 0, rg = VRAM_CLUT_REGION(v, 0); i < 8; i++, rg++) {
        if (rg->psm != 0xFF && rg->count < rg->capacity && clutpsm == rg->psm) {
            for (j = 0; j < rg->capacity; j++) {
                if (!(rg->mask & (1 << j))) {
                    rg->mask |= 1 << j;
                    rg->count++;
                    return (s16)(i * 16 + j);
                }
            }
        }
    }
    for (i = 0, rg = VRAM_CLUT_REGION(v, 0); i < 8; i++, rg++) {
        if (rg->psm == 0xFF) {
            rg->psm = clutpsm_;
            rg->mask = 1;
            rg->count = 1;
            rg->capacity = clutpsm == 2 ? 16 : 8;
            return (s16)(i * 16);
        }
    }
    return -1;
}

/* 0x001C0E80 */
u64 Vram_EntryTex2(u8 *v, s32 id, u32 csa) {
    VramEntry *e = VRAM_ENTRY(v, id);

    return VCALL(v, 0x34, u64 (*)(u8 *, s32, u32, s32, s32))(v, id, csa, (u16)e->psm, (u16)e->cpsm);
}

/* 0x001C0EB0 */
u64 Vram_EntryTex0Csa(u8 *v, s32 id, u32 csa) {
    VramEntry *e = VRAM_ENTRY(v, id);

    return VCALL(v, 0x30, u64 (*)(u8 *, s32, u32, s32, u32, u32, s32))(v, id, csa, (u16)e->psm, (u16)e->w, (u16)e->h, (u16)e->cpsm);
}

/* 0x001C0EE0 */
u64 Vram_EntryTex0Indexed(u8 *v, s32 id) {
    VramEntry *e = VRAM_ENTRY(v, id);

    return VCALL(v, 0x2C, u64 (*)(u8 *, s32, u32, u32, s32))(v, id, (u16)e->w, (u16)e->h, (u16)e->cpsm);
}

/* +0x38 .. +0x44: the same, with the entry's own format and size */
/* 0x001C0F10 */
u64 Vram_EntryTex0(u8 *v, s32 id) {
    VramEntry *e = VRAM_ENTRY(v, id);

    return VCALL(v, 0x28, u64 (*)(u8 *, s32, s32, u32, u32, s32))(v, id, (u16)e->psm, (u16)e->w, (u16)e->h, (u16)e->cpsm);
}

/* +0x34 TEX2 for entry `id`: format psm, CLUT `csa` of the entry's CLUT region (format cpsm) */
/* 0x001C0F40 */
u64 Vram_Tex2(u8 *v, s32 id, u32 csa, s32 psm, s32 cpsm) {
    VramEntry *e = VRAM_ENTRY(v, id);
    u32 slots = (u16)e->cpsm == 2 ? 16 : 8;
    u32 cbp = ((e->cregion << 11) + 0xFC000 + (u32)((u16)e->cslot << 11) / slots) >> 6;

    return ((u64)csa << 56) | ((u64)(cpsm & 0xFFFF) << 51) | ((u64)cbp << 37) | ((u64)(psm & 0xFFFF) << 20);
}

/* +0x30 ... with CLUT offset `csa` and no CLUT load */
/* 0x001C0FE0 */
u64 Vram_Tex0Csa(u8 *v, s32 id, u32 csa, s32 psm, u32 w, u32 h, s32 cpsm) {
    return Vram_Tex0(v, id, psm, w, h, cpsm, (u64)csa << 56);
}

/* +0x2C ... as an 8-bit indexed texture */
/* 0x001C1160 */
u64 Vram_Tex0Indexed(u8 *v, s32 id, u32 w, u32 h, s32 cpsm) {
    return Vram_Tex0(v, id, 0x13, w, h, cpsm, (u64)0x20000000 << 32);
}

/* +0x28 TEX0 for entry `id` as a psm w x h texture, loading its CLUT (CLD 1) */
/* 0x001C12E0 */
u64 Vram_Tex0Load(u8 *v, s32 id, s32 psm, u32 w, u32 h, s32 cpsm) {
    return Vram_Tex0(v, id, psm, w, h, cpsm, (u64)0x20000000 << 32);
}

/* +0x24 keep entry `id` resident (not freed by +0x20) */
/* 0x001C1460 */
void Vram_KeepResident(u8 *v, s32 id) {
    VramEntry *e = (VramEntry *)(v + 0x98) + id;

    if (e->used) {
        e->locked = 1;
    }
}

/* +0x20 free every entry that isn't kept resident (+0x1C) */
/* 0x001C14A0 */
void Vram_FreeAll(VObject *v) {
    VramEntry *e = (VramEntry *)((u8 *)v + 0x98);
    u32 i;

    for (i = 0; i < 64; i++, e++) {
        if (e->used && !e->locked) {
            VCALL(v, 0x1C, void (*)(VObject *, u32))(v, i);
        }
    }
}

/* +0x1C free entry `id`: its pages (8 / 4-bit H formats: their upper bits, both halves or one;
 * else the page bits), then its CLUT slot (the region emptied with its last) */
/* 0x001C1520 */
void Vram_Free(u8 *v, s32 id) {
    VramEntry *e;
    u32 p, end;

    if (id < 0) {
        return;
    }
    e = (VramEntry *)(v + 0x98) + id;
    if (!e->used) {
        return;
    }
    e->used = 0;
    e->locked = 0;
    switch ((u16)e->psm) {
    case PSMT4HH:
    case PSMT4HL:
    case PSMT8H: {
        u32 bits = (u16)e->psm == PSMT4HH ? 2 : (u16)e->psm == PSMT4HL ? 1 : 3;

        if (e->page == -1) {
            break;
        }
        end = e->page + (u16)e->npages;
        for (p = e->page; p < end; p++) {
            VRAM_PAGEMAP_HI(v)[(p * 2) >> 5] &= ~(bits << ((p * 2) & 0x1F));
        }
        break;
    }
    default:
        if (e->page == -1) {
            break;
        }
        end = e->page + (u16)e->npages;
        for (p = e->page; p < end; p++) {
            VRAM_PAGEMAP(v)[p >> 5] &= ~(1u << (p & 0x1F));
        }
        break;
    }
    if ((u16)e->cpsm != PSM_NONE) {
        s16 c = (s16)(e->cregion * 16 | (u16)e->cslot);

        if (c != -1) {
            VramClutRegion *r = VRAM_CLUT_REGION(v, c >> 4);

            r->mask &= ~(1 << (c & 0xF));
            if (--r->count == 0) {
                r->psm = PSM_NONE;
                r->mask = 0;
                r->count = 0;
                r->capacity = 0;
            }
        }
    }
}

/* +0x18 allocate a w x h texture of format psm (0xFF: CLUT only) at `addr` (< 0: anywhere), with
 * a CLUT of format clutpsm (0xFF: none; 0 / 2 for indexed formats). Returns the entry, -1 if
 * it doesn't fit (whatever was taken is given back). */
/* 0x001C1E00 */
s32 Vram_AllocAt(u8 *v, s32 addr, s32 psm_, s32 w, s32 h, s32 clutpsm_) {
    u16 psm = psm_;
    u16 clutpsm = clutpsm_;
    VramEntry *e;
    s32 i, pos, clut;
    u32 p, end;

    switch (psm) {
    case 0x00: case 0x01: case 0x02:   /* 32 / 24 / 16-bit: no CLUT */
        if (clutpsm != PSM_NONE) {
            return -1;
        }
        break;
    case PSM_NONE: case PSMT8H: case PSMT4HH: case PSMT4HL: case 0x13: case 0x14:
        if (clutpsm != 2 && clutpsm != 0) {
            return -1;
        }
        break;
    default:
        return -1;
    }
    e = (VramEntry *)(v + 0x98);
    for (i = 0; i < 64; i++, e++) {
        if (e->used) {
            continue;
        }
        pos = -1;   /* (the PS2 code leaves this unset for CLUT-only allocations) */
        if (addr >= 0) {
            pos = (s16)Vram_PagesAt(v, addr, &psm, w, h);
        } else if (psm == PSMT4HL || psm == PSMT4HH || psm == PSMT8H) {
            pos = (s16)Vram_FitUpper(v, &psm, w, h);
        } else if (psm != PSM_NONE) {
            pos = (s16)Vram_FitPages(v, psm, w, h);
        }
        clut = (s16)Vram_AllocClut(v, clutpsm);
        if ((psm == PSM_NONE || pos >= 0) && (clutpsm == PSM_NONE || clut >= 0)) {
            e->used = 1;
            e->psm = psm;
            e->page = pos;
            e->w = w;
            e->h = h;
            e->npages = Vram_PageCount(v, psm, w, h);
            e->cpsm = clutpsm_;
            e->cregion = clut >> 4;
            e->cslot = clut & 0xF;
            return i;
        }
        /* give back what was taken */
        if (clut != -1) {
            VramClutRegion *rg = VRAM_CLUT_REGION(v, clut >> 4);

            rg->mask &= ~(1 << (clut & 0xF));
            if (--rg->count == 0) {
                rg->psm = 0xFF;
                rg->mask = 0;
                rg->count = 0;
                rg->capacity = 0;
            }
        }
        if (pos != -1) {
            end = pos + (u16)e->npages;
            switch (psm) {
            case PSMT8H:
                for (p = pos; p < end; p++) {
                    VRAM_PAGEMAP_HI(v)[p * 2 >> 5] &= ~(3 << (p * 2 & 0x1F));
                }
                break;
            case PSMT4HL:
                for (p = pos; p < end; p++) {
                    VRAM_PAGEMAP_HI(v)[p * 2 >> 5] &= ~(1 << (p * 2 & 0x1F));
                }
                break;
            case PSMT4HH:
                for (p = pos; p < end; p++) {
                    VRAM_PAGEMAP_HI(v)[p * 2 >> 5] &= ~(2 << (p * 2 & 0x1F));
                }
                break;
            default:
                for (p = pos; p < end; p++) {
                    VRAM_PAGEMAP(v)[p >> 5] &= ~(1 << (p & 0x1F));
                }
                break;
            }
        }
        return -1;
    }
    return -1;
}

/* +0x14 allocate anywhere */
/* 0x001C2930 */
s32 Vram_Alloc(VObject *v, s32 psm, s32 w, s32 h, s32 clutpsm) {
    return VCALL(v, 0x18, s32 (*)(VObject *, s32, s32, s32, s32, s32))(v, -1, psm, w, h, clutpsm);
}

/* +0x10 reset (init again) */
/* 0x001C2960 */
void Vram_Reset(VObject *v) {
    VCALL(v, 0xC, void (*)(VObject *))(v);
}

/* +0xC init: 8 free regions, 64 empty entries */
/* 0x001C2970 */
void Vram_Init(u8 *v) {
    VramEntry *e = (VramEntry *)(v + 0x98);
    s32 i;

    for (i = 0; i < 8; i++) {
        AT(v, 4 + i * 8, s16) = 0xFF;
        AT(v, 6 + i * 8, s16) = 0;
        AT(v, 8 + i * 8, s16) = 0;
        AT(v, 10 + i * 8, s16) = 0;
    }
    AT(v, 0x44, s32) = 0;
    AT(v, 0x48, s32) = 0;
    AT(v, 0x4C, s32) = 0;
    AT(v, 0x50, s32) = 0;
    for (i = 0; i < 64; i++, e++) {
        e->used = 0;
        e->locked = 0;
        e->w = 0;
        e->h = 0;
        e->psm = 0xFF;
        e->page = 0;
        e->npages = 0;
        e->cpsm = 0xFF;
        e->cregion = 0;
        e->cslot = 0;
    }
    AT(v, 0x518, s32) = 0;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x001CEC50 */
void *AdxEnc_Version(void) {
    return str_ADXENC_DLL_Ver_1_09_Nov_12_2004;
}

/* 0x001D6E40 */
void *Adx_Data47E878(void) {
    return D_0047E878;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x001DC580 */
void *Adx_Data3C5D90(void) {
    return D_003C5D90;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x001DE030 */
void *Adx_Data3C6348(void) {
    return D_003C6348;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x001DE0A8 */
void *Adx_Data3C6450(void) {
    return D_003C6450;
}

/* 0x001E61D0 */
void *Adx_Data4555C8(void) {
    return D_004555C8;
}

/* 0x001E8138 */
void *Rofs_Version(void) {
    return str_ROFS_Ver_1_76_Build_Sep_3_2004_17_20_10;
}

/* 0x001EB650 */
void *Adx_Data455C38(void) {
    return D_00455C38;
}

/* 0x001ED190 */
void *Roci_Version(void) {
    return str_ROCI_Ver_1_15_Build_Sep_3_2004_17_20_13;
}

/* 0x001ED1D0 */
void *Adx_Data3D5B80(void) {
    return D_003D5B80;
}

/* 0x001EDFF0 */
void *Rsu_Version(void) {
    return str_RSU_Ver_1_10_Build_Sep_3_2004_17_20_13;
}

/* +0xC the .TEX entry of texture `sel` of group `group` */
/* 0x001F4160 */
u8 *TexCache_Entry(TexCache *c, s32 sel, s32 group) {
    u32 idx = sel + group;

    if (idx >= 64) {
        return NULL;
    }
    return c->e[idx].tex;
}

/* +0x8 the renderer layer to draw texture `sel` of group `group` with; bit 31: the layer was just
 * (re)assigned and needs the texture uploaded. -1 if there is no such texture. */
/* 0x001F41A0 */
s32 TexCache_Layer(TexCache *c, s32 sel, s32 group) {
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

/* +0x14 unregister group `group` */
/* 0x001F43C0 */
void TexCache_RemoveGroup(TexCache *c, s32 group) {
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

/* +0x10 register the textures of .TEX file `tex` as group `group` (entries group.. group+n-1) */
/* 0x001F4420 */
void TexCache_AddGroup(TexCache *c, u32 *tex, s32 group) {
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

/* +0x18 forget all layer assignments */
/* 0x001F44A0 */
void TexCache_ForgetLayers(TexCache *c) {
    u32 i;

    for (i = 0; i < 64; i++) {
        c->e[i].age = -1;
        c->e[i].layer = -1;
    }
}

/* destructor (vtable TexCache_vtable) */
/* (possibly dead code: nothing in the game references it) */
/* 0x001F4590 */
void *TexCache_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = TexCache_vtable;
        AT(o, 0x0, void **) = D_0046B1F0;
        gTexCache = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* the renderer's +0x5C */
/* (possibly dead code: nothing in the game references it) */
/* 0x00267140 */
void Renderer_Call5C(void) {
    VCALL(gRenderer, 0x5C, void (*)(VObject *))(gRenderer);
}

/* destructor (vtable Bloom_vtable) */
/* 0x00269970 */
void *Bloom_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Bloom_vtable;
        AT(o, 0x0, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

#ifdef HG_NATIVE
/* the bloom's draw: the screen halved (256 x 224), brightened and blurred (8 taps at 1/2, then
 * 8 wider ones at 1/8 added back), an eighth of it added into the renderer's glow buffer, and
 * stretched over the screen tinted by +0x8 (0x80 = 1.0) at its alpha / 2 - added, or subtracted
 * when +0xC. On PC glr does the passes (the original's 0x209 qwords of GS sprites) */
/* 0x002699D0 */
s32 Bloom_Draw(u8 *o) {
    extern void glr_bloom(u32 rgba, s32 subtract);   /* native/platform/glr.c */

    glr_bloom(AT(o, 0x8, u32), AT(o, 0xC, s32) != 0);
    return 1;
}
#endif

/* the screen bloom (vtable Bloom_vtable; its draw Bloom_Draw): colour `rgba`, subtracted when
 * `sub`, drawn in renderer layer `layer`; the renderer's glow pass (+0x58) runs this frame too */
/* 0x0026B180 */
void Bloom_Start(u8 *o, u32 rgba, s32 layer, s32 sub) {
    VObject *r = (VObject *)gRenderer;

    AT(o, 0x8, u32) = rgba;
    AT(o, 0xC, s32) = sub;
    VCALL(r, 0xC, void (*)(VObject *, void *, s32, s32))(r, o, layer, 0);
    VCALL(r, 0x58, void (*)(VObject *))(r);
}

/* TEX0 to draw cached texture `sel` (group 0) with, uploading it first when its VRAM slot was
 * just (re)assigned. 0 if there's no such texture. */
/* 0x002B71D0 */
u64 TexCache_Tex0(s32 sel) {
    TexCache *c = (TexCache *)gTexCache;
    s32 slot;
    u8 *tex;
    u64 tex0;

    slot = VCALL(c, 0x8, s32 (*)(TexCache *, s32, s32))(c, sel, 0);
    if (slot == -1) {
        return 0;
    }
    tex = VCALL(c, 0xC, u8 *(*)(TexCache *, s32, s32))(c, sel, 0);
    tex0 = VCALL(gVram, 0x28, u64 (*)(void *, s32, s32, u32, u32, s32))(
        gVram, slot & 0x7FFFFFFF, tex[0], AT(tex, 4, u16), AT(tex, 6, u16), tex[1]);
    if (slot & 0x80000000) {
        VCALL(gRenderer, 0x44, s32 (*)(void *, s32, u8 *, s32))(gRenderer, slot, tex, -1);
    }
    return tex0;
}

/* set the colour (RGBA, alpha in the top byte) */
/* 0x002CF390 */
void Overlay_SetColor(void *ov, u32 rgba) {
    AT(ov, 0xF8, u32) = rgba;
}

#ifdef HG_NATIVE
/* the rectangle: a blended strip over the corners (0 / 1: 512 game pixels, from y -32) in the
 * overlay colour, in the layer being drawn; the camera vectors placed per corner as the
 * original does */
/* 0x002CF700 */
void Overlay_DrawRect(void *ov) {
    f32 rel[4], tmp[4], xy[8];
    u8 c[16];
    s32 i;

    for (i = 0; i < 4; i++) {
        s32 x = D_00414310[i][0], y = D_00414310[i][1];

        sceVu0ScaleVector(V(ov, 0xC0), V(ov, 0xA0), 0.0f + (f32)x);
        sceVu0ScaleVector(V(ov, 0xD0), V(ov, 0x80), 0.0f + (f32)y);
        sceVu0AddVector(V(ov, 0xB0), V(ov, 0xC0), V(ov, 0xD0));
        sceVu0SubVector(tmp, V(ov, 0xE0), V(ov, 0xB0));
        sceVu0SubVector(rel, V(ov, 0x70), tmp);
        xy[i * 2] = x << 9;
        xy[i * 2 + 1] = (y << 9) - 0x20;
    }
    gl2d_colors(c, AT(ov, 0xF8, u32), 4);
    glr_prim2d(-1, GLR_2D_STRIP, 4, xy, NULL, c, NULL, 0, 0x40);
}
#endif

/* +0xC draw: place the camera vectors (when +0x24 changed, refresh the stamp first), then the
 * packet. Always draws. */
/* 0x002CF8C0 */
s32 Overlay_Draw(void *ov) {
    void *cam;

    if (AT(ov, 0x24, s32) != AT(ov, 0x10, s32)) {
        AT(ov, 0x8, u64) = TexCache_Tex0(0);
    }
    cam = gCamera;
    VCALL(cam, 0x24, void (*)(void *, f32 *))(cam, V(ov, 0x70));
    VCALL(cam, 0xA4, void (*)(void *, f32 *))(cam, V(ov, 0x80));
    VCALL(cam, 0xA0, void (*)(void *, f32 *))(cam, V(ov, 0x90));
    sceVu0OuterProduct(V(ov, 0xA0), V(ov, 0x80), V(ov, 0x90));
    sceVu0ScaleVector(V(ov, 0xC0), V(ov, 0xA0), 0x1.666666p+0f);   /* 1.4 */
    sceVu0ScaleVector(V(ov, 0xD0), V(ov, 0x80), 0x1.0cccccp+0f);   /* 1.05 */
    sceVu0AddVector(V(ov, 0xB0), V(ov, 0xC0), V(ov, 0xD0));
    VCALL(cam, 0x20, void (*)(void *, f32 *))(cam, V(ov, 0xE0));
    sceVu0AddVector(V(ov, 0xE0), V(ov, 0xE0), V(ov, 0xB0));
    Overlay_DrawRect(ov);
    AT(ov, 0x10, s32) = AT(ov, 0x24, s32);
    return 1;
}

#ifdef HG_NATIVE
#include <stdlib.h>

/* Raw images sent to VRAM (+0x48 / +0x4C) that sprites then show: kept decoded to RGBA behind a
 * .TEX-style header (PSMCT32, w x h) so the GL renderer takes them like any texture; by their
 * VRAM block. The header's spare bytes count the uploads (the renderer re-reads it on change). */
typedef struct Gl2dImage {
    u32 block;
    const u8 *src;
    u32 sum;
    u8 *buf;
} Gl2dImage;

static Gl2dImage sImages[4];

static u32 image_sum(const u8 *p, s32 n) {
    u32 c = 2166136261u;
    s32 i;

    for (i = 0; i < n; i += 61) {
        c = (c ^ p[i]) * 16777619u;
    }
    return c;
}

/* the image at VRAM block `block` (NULL: none sent) */
const u8 *gl2d_image(u32 block) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (sImages[i].buf != NULL && sImages[i].block == block) {
            return sImages[i].buf;
        }
    }
    return NULL;
}

/* keep `img` (w x h indexed pixels, `bits` 4 or 8, its CLUT of 32-bit colours right after them
 * in the GS's upload order) as the image at `block` */
static void image_put(u32 block, const u8 *img, s32 w, s32 h, s32 bits) {
    s32 n = w * h, ncol = bits == 4 ? 16 : 256, i, slot = -1;
    const u8 *clut = img + (bits == 4 ? n >> 1 : n);
    u32 sum = image_sum(img, n * bits / 8 + ncol * 4), pal[256];
    Gl2dImage *e;
    u32 *px;

    for (i = 0; i < 4 && slot < 0; i++) {
        if (sImages[i].block == block && sImages[i].buf != NULL) {
            slot = i;
        }
    }
    for (i = 0; i < 4 && slot < 0; i++) {
        if (sImages[i].buf == NULL) {
            slot = i;
        }
    }
    if (slot < 0) {
        slot = 0;
    }
    e = &sImages[slot];
    if (e->buf != NULL && e->block == block && e->src == img && e->sum == sum &&
        AT(e->buf, 4, u16) == w && AT(e->buf, 6, u16) == h) {
        return;   /* (sent again unchanged, as the title does every frame) */
    }
    if (e->buf == NULL || AT(e->buf, 4, u16) * AT(e->buf, 6, u16) < n) {
        free(e->buf);
        e->buf = malloc(16 + (u32)n * 4);
        AT(e->buf, 2, u16) = 0;
    }
    for (i = 0; i < ncol; i++) {   /* 256 colours: the CLUT's CSM1 layout (8..15 / 16..23 swapped) */
        s32 k = bits == 4 ? i : (i % 8 + ((i / 16) % 2) * 8) + ((i / 32) * 2 + (i / 8) % 2) * 16;

        pal[i] = AT(clut, k * 4, u32);
    }
    e->block = block;
    e->src = img;
    e->sum = sum;
    e->buf[0] = 0;   /* PSMCT32 */
    e->buf[1] = 0;
    AT(e->buf, 2, u16) += 1;
    AT(e->buf, 4, u16) = w;
    AT(e->buf, 6, u16) = h;
    AT(e->buf, 8, u16) = (u16)((u32)n * 4 / 16);
    AT(e->buf, 10, u16) = 0;
    AT(e->buf, 12, s32) = 16;
    px = (u32 *)(e->buf + 16);
    for (i = 0; i < n; i++) {
        px[i] = bits == 4 ? pal[(img[i >> 1] >> ((i & 1) * 4)) & 0xF] : pal[img[i]];
    }
}

/* +0x4C a 4-bit image (w x h, its 16-colour CLUT right after the pixels) for VRAM block
 * 0x3400 */
/* 0x001BB010 */
s32 Renderer_PutImage4(u8 *r, u8 *img, s32 w, s32 h, s32 layer) {
    image_put(0x3400, img, w, h, 4);
    return 1;
}

/* +0x48 an 8-bit image (w x h, its 256-colour CLUT right after the pixels) for VRAM at byte
 * address `addr` */
/* 0x001BB230 */
s32 Renderer_PutImage8(u8 *r, u8 *img, s32 w, s32 h, s32 addr, s32 layer) {
    image_put((u32)addr >> 6, img, w, h, 8);
    return 1;
}
#endif

/* +0x28 video mode (2: NTSC 448 lines) */
/* 0x001BB9D0 */
u8 Renderer_VideoMode(u8 *r) { return AT(r, 0x304C09, u8); }

/* +0x2C display settings (+0x1F / +0x20: screen offset) */
/* 0x001BB9C0 */
u8 *Renderer_Display(u8 *r) { return r + 0x304BE8; }

/* +0x30 set the screen offset */
/* 0x001BB9A0 */
void Renderer_SetScreenOffset(u8 *r, s32 x, s32 y) {
    AT(r, 0x304C07, u8) = x;
    AT(r, 0x304C08, u8) = y;
}

/* +0x34 */
/* 0x001BB990 */
void Renderer_Set304BF8(u8 *r, s32 v) { AT(r, 0x304BF8, s32) = v; }

/* +0x38 the renderer's own VRAM entry */
/* 0x001BB980 */
s32 Renderer_VramEntry(u8 *r) { return AT(r, 0x304BE4, s32); }

extern u64 D_0047D300[];   /* the frame's final packet: draw buffer -> display buffer */

#define REND_VIF1_CHAN(r) AT(r, 0x304BB0, u32 *)

#ifdef HG_NATIVE
/* wait for the previous frame's chain - nothing on PC */
/* 0x001B87D0 */
void Renderer_WaitChain(u8 *r) {
    (void)r;
}
#endif

#ifdef HG_NATIVE
/* buffer 1's environment and the final copy packet sent - nothing on PC */
/* 0x001B8750 */
void Renderer_SendFinal(u8 *r) {
    (void)r;
}
#endif

#ifdef HG_NATIVE
/* after the GIF finished: the next frame's clear and buffer 0's environment - nothing on PC */
/* 0x001B86A0 */
void Renderer_NextClear(u8 *r) {
    (void)r;
}
#endif

extern void Renderer_FinalPacket(u8 *r);

#ifdef HG_NATIVE
/* end of frame: flip to the other draw buffer (the original also sends the layer chain over
 * VIF1 and sets up the final packet first) */
/* 0x001B85B0 */
void Renderer_EndFrame(u8 *r) {
    Renderer_Flip(r);
}
#endif

#ifdef HG_NATIVE
/* The frame's final packet (the draw buffer copied onto the display buffer, blended with the
 * last frame by +0x304C06 when +0x304C05 is set): glr_present shows the GL frame instead. */
/* 0x001B75E0 */
void Renderer_FinalPacket(u8 *r) {
    (void)r;
}
#endif

/* a texture's entry in a .TEX file */
typedef struct TexHeader {
    /* 0x0 */ u8 psm;
    /* 0x1 */ u8 cpsm;      /* CLUT format */
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ u16 w;
    /* 0x6 */ u16 h;
    /* 0x8 */ u16 imageQwc;
    /* 0xA */ u16 clutQwc;
    /* 0xC */ s32 data;     /* image then CLUT, from this entry */
} TexHeader;

#define VRAM_TEX_ADDR(v, id) VCALL(v, 0x68, u32 (*)(VObject *, s32))(v, id)    /* in 64-word blocks */
#define VRAM_CLUT_ADDR(v, id) VCALL(v, 0x6C, u32 (*)(VObject *, s32))(v, id)

#ifdef HG_NATIVE
/* +0x44 upload texture `t` to its VRAM entry `id`: nothing to do on PC (the GL renderer reads
 * .TEX entries where they are loaded) */
/* 0x001BB470 */
s32 Renderer_UploadTexture(u8 *r, s32 id, TexHeader *t, s32 layer) {
    return 1;
}
#endif


extern PTMF kPaletteGenerators[];     /* palette generators by mode: (this, index, arg) -> RGBA */

#ifdef HG_NATIVE
/* +0x94: build a 256-colour palette with generator `mode` (kPaletteGenerators) for VRAM slot `slot`'s
 * CLUT, in renderer layer `layer` - the palette of layer 0x11's effect, which glr doesn't draw
 * yet (func_001B2160) */
/* 0x001B8D30 */
s32 Renderer_BuildPalette(VObject *r, s32 slot, s32 mode, s32 layer, s32 arg) {
    (void)r;
    (void)slot;
    (void)mode;
    (void)layer;
    (void)arg;
    return 1;
}
#endif

/* palette generator 0: grey levels squeezed to 0x7E..0x81 around the middle (a nearly flat
 * ramp, the index clamped to 0x7E..0x80, plus one), in all four channels */
/* 0x001B8CE0 */
u32 Palette_FlatLow(VObject *r, u32 i) {
    u32 v;

    if (i > 0x80) {
        v = 0x81;
    } else if (i < 0x7E) {
        v = 0x7E;
    } else {
        v = i + 1;
    }
    return v | v << 8 | v << 16 | v << 24;
}

/* palette generator 1: the same, the index clamped to 0x7F..0x81, minus one */
/* 0x001B8C90 */
u32 Palette_FlatHigh(VObject *r, u32 i) {
    u32 v;

    if (i < 0x7F) {
        v = 0x7E;
    } else if (i >= 0x82) {
        v = 0x81;
    } else {
        v = i - 1;
    }
    return v | v << 8 | v << 16 | v << 24;
}

#ifdef HG_NATIVE
/* the 3D layers' start (layer 0x25; the original clears the frame's alpha and turns FBA on, put
 * back at 0x27): glr keeps the frame alpha marks itself (the bloom mask) */
/* 0x001B1E50 */
s32 Renderer_3DBegin(u8 *rp) {
    (void)rp;
    return 1;
}
#endif

/* +0x80 draw a box described by 13 words (+0x7C with them as arguments) */
/* 0x001B9810 */
void Renderer_DrawBox(VObject *r, const s32 *b) {
    VCALL(r, 0x7C, void (*)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32))(
        r, b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8], b[9], b[10], b[11], b[12]);
}

#define SX32(x) ((s64)(s32)(u32)(x))

#ifdef HG_NATIVE
/* the texture-cache entry of texture `tex` of group `group` for a 2D draw (NULL: not loaded;
 * the cache keeps it resident) */
static TexHeader *tex2d_entry(s32 tex, s32 group) {
    VObject *tc = gTexCache;

    if (VCALL(tc, 0x8, s32 (*)(VObject *, s32, s32))(tc, tex, group) == -1) {
        return NULL;
    }
    return VCALL(tc, 0xC, TexHeader *(*)(VObject *, s32, s32))(tc, tex, group);
}

/* a sprite colour: an alpha over 0x80 is opaque (drawn at 0x80, unblended), else blended */
static inline u32 sprite_prim(u32 *rgba) {
    if (*rgba >= 0x81000000) {
        *rgba = (*rgba & 0xFFFFFF) | 0x80000000;
        return 0;
    }
    return 0x40;
}

/* +0x7C a sprite: the w x h rectangle at x, y (screen pixels), coloured `rgba` (alpha over 0x80:
 * opaque), textured with the tw x th texels at u, v of texture `tex` of group `group` (-1:
 * untextured; `clut` -1: its own palette, else palette `clut` of its CLUT), in renderer layer
 * `layer`. 0 when the texture isn't loaded. */
/* 0x001B9880 */
s32 Renderer_Sprite(VObject *r, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 tw, s32 th, u32 rgba,
                  s32 tex, s32 group, s32 layer, s32 clut) {
    TexHeader *t = NULL;
    u32 prim = sprite_prim(&rgba);

    if (tex != -1 && (t = tex2d_entry(tex, group)) == NULL) {
        return 0;
    }
    gl2d_sprite(layer, x, y, x + w, y + h, (u8 *)t, u, v, u + tw, v + th, rgba, clut == -1 ? 0 : clut, prim);
    return 1;
}

/* +0x84 a quad with free corners (x0, y0) .. (x3, y3) (strip order: the texels' top left, top
 * right, bottom left, bottom right), otherwise as +0x7C */
/* 0x001B92F0 */
s32 Renderer_Quad(VObject *r, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 u, s32 v,
                  s32 tw, s32 th, u32 rgba, s32 tex, s32 group, s32 layer, s32 clut) {
    TexHeader *t = NULL;
    u32 prim = sprite_prim(&rgba);
    f32 xy[8] = {x0, y0, x1, y1, x2, y2, x3, y3}, st[8];
    u8 c[16];

    if (tex != -1 && (t = tex2d_entry(tex, group)) == NULL) {
        return 0;
    }
    if (t != NULL) {
        st[0] = (f32)u / t->w;        st[1] = (f32)v / t->h;
        st[2] = (f32)(u + tw) / t->w; st[3] = (f32)v / t->h;
        st[4] = (f32)u / t->w;        st[5] = (f32)(v + th) / t->h;
        st[6] = (f32)(u + tw) / t->w; st[7] = (f32)(v + th) / t->h;
    }
    gl2d_colors(c, rgba, 4);
    glr_prim2d(layer, GLR_2D_STRIP, 4, xy, st, c, t, clut == -1 ? 0 : clut, prim);
    return 1;
}
#endif

#ifdef HG_NATIVE
/* +0x5C clear the 128 x 112 glow work buffer to black (the original: a sprite in layer 0x29 at
 * page 0x1F0); not when progress flag 0x28 is set */
/* 0x001BA090 */
s32 Renderer_GlowClear(u8 *r) {
    extern void glr_glow_clear(void);   /* native/platform/glr.c */

    if ((u8)Progress_TestFlag(gProgress, 0x28) == 1) {
        return 1;
    }
    AT(r, 0x304BB6, u8) = 1;
    glr_glow_clear();
    return 1;
}
#endif

#ifdef HG_NATIVE
/* +0x58 the glow, once a frame (not after +0x5C's clear; layer 0x29): the 128 x 112 work
 * buffer (page 0x1F0, which glow sprites - GlSprites_Glow - also draw into) goes through the
 * one at page 0x180 at half colour and is blended back 50 / 50, so fades to 3/4, then is
 * stretched over the 512 x 448 frame and added at half strength. The buffer is not cleared
 * between frames, so moving glows leave trails. On PC glr does the passes (the original's
 * packet, 0x96 qwords from +0x10, isn't made: 0 when there was no room for it) */
/* 0x001BA260 */
s32 Renderer_Glow(u8 *r) {
    extern void glr_glow(void);   /* native/platform/glr.c */

    if (AT(r, 0x304BB6, u8) != 0) {
        return 1;
    }
    AT(r, 0x304BB6, u8) = 1;
    glr_glow();
    return 1;
}
#endif

/* +0x20 the packet for vertex data `key` this frame: open addressing over 2048 slots
 * (+0x300A00, {key, packet}); found: *built = 0 and its packet; new: the slot takes the arena 2
 * cursor (+0x304BAC) and *built = 1 (the caller writes the packet there) */
/* 0x001BBAA0 */
void *Renderer_PacketFor(u8 *r, void *key, u8 *built) {
    u32 i = ((u32)key >> 4) & 0x7FF;
    u8 *slot;

    for (;;) {
        slot = r + 0x300A00 + i * 8;
        if (AT(slot, 0x0, void *) == NULL) {
            AT(slot, 0x0, void *) = key;
            AT(slot, 0x4, void *) = AT(r, 0x304BAC, void *);
            *built = 1;
            return AT(slot, 0x4, void *);
        }
        if (AT(slot, 0x0, void *) == key) {
            *built = 0;
            return AT(slot, 0x4, void *);
        }
        i = (i + 1) & 0x7FF;
    }
}

/* +0x8C the floor effect's value (+0x304DDC) */
/* 0x001B9250 */
void Renderer_SetFloorEffect(u8 *r, s32 v) {
    AT(r, 0x304DDC, s32) = v;
#ifdef HG_NATIVE
    {
        extern void glr_refl_flip(s32 flip);   /* native/platform/glr.c: the reflection's mirror */

        glr_refl_flip(v);
    }
#endif
}

/* ---- small renderer methods (2026-10-05) ---- */

static inline u32 grey_rgba(u32 c, u32 a) {
    return c | c << 8 | c << 16 | a;
}

/* palette entry i of the layer-0x11 alpha table: 0x80 - clamp(((255-a)>>7) x ((255-a)&127) +
 * 256 - i - a, 0, 128), a = *alpha; the same in all four channels */
/* 0x001B87F0 */
u32 Palette_Alpha11(void *r, s32 i, s32 *alpha) {
    s32 t = 0xFF - *alpha;
    s32 v = (t >> 7) * (t & 0x7F) + 0x100 - (i + *alpha);

    if (v < 0) {
        v = 0;
    } else if (!(v < 0x81)) {
        v = 0x80;
    }
    v = 0x80 - v;
    return (u32)v << 24 | grey_rgba(v, 0);
}

/* palette entry i: black below *limit, else grey 0xC0 (alpha 0x80) */
/* 0x001B8860 */
u32 Palette_Threshold(void *r, u32 i, u32 *limit) {
    if (i < *limit) {
        return 0x80000000;
    }
    return 0x80C0C0C0;
}

/* palette generator 2: entry i of the ramp from colour c[1] (index 0) to c[0] (index 255), each
 * channel rounded, alpha halved */
static inline u32 ramp_ch(f32 t, f32 u, u32 a, u32 b) {
    return (u32)(0.5f + (u * (f32)b + t * (f32)a));
}

/* 0x001B8910 */
u32 Palette_Colour(void *r, u32 i, const u32 *c) {
    f32 t = (f32)i / 255.0f;
    f32 u = 1.0f - t;
    u32 g, rb;

    g = ramp_ch(t, u, (c[0] >> 8) & 0xFF, (c[1] >> 8) & 0xFF) << 8;
    rb = g | ramp_ch(t, u, c[0] & 0xFF, c[1] & 0xFF);
    rb |= ramp_ch(t, u, (c[0] >> 16) & 0xFF, (c[1] >> 16) & 0xFF) << 16;
    return (ramp_ch(t, u, c[0] >> 24, c[1] >> 24) >> 1) << 24 | rb;
}

/* palette entry i: the grey ((2i + 5)(i + 1)) & 0xFF */
/* 0x001B8890 */
u32 Palette_Grey(void *r, s32 i) {
    return grey_rgba(((i * 2 + 5) * (i + 1)) & 0xFF, 0x80000000);
}

/* palette entry: a random grey */
/* 0x001B88C0 */
u32 Palette_RandomGrey(void) {
    return grey_rgba(VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 0xFF, 0x80000000);
}

/* +0x64 (and others): the layer-0x11 / special colours */
/* 0x001B9D20 */
void Renderer_SetSpecialColour(u8 *r, u32 c) {
    AT(r, 0x304D58, u32) = c;
}

/* 0x001B9D30 */
u32 Renderer_GetColour54(u8 *r) {
    return AT(r, 0x304D54, u32);
}

/* 0x001B9D40 */
void Renderer_SetColour54(u8 *r, u32 c) {
    AT(r, 0x304D54, u32) = c;
}

/* 0x001BA000 */
u32 Renderer_GetTint11(u8 *r) {
    return AT(r, 0x304D4C, u32);
}

/* the layer-0x11 tint (+0x304D4C) and its model (+0x304D50; none given: the slot-2 character's,
 * once the game runs) */
/* 0x001BA010 */
void Renderer_SetTint11(u8 *r, u32 c, void *model) {
    AT(r, 0x304D4C, u32) = c;
    if (model == NULL && gProgress != NULL) {
        AT(r, 0x304D50, void *) = AT(gCharSlot2, 0xF0, void *);
        return;
    }
    AT(r, 0x304D50, void *) = model;
}

/* sprites additive (+0x304C05) with the given mode (+0x304C06) */
/* 0x001BA070 */
void Renderer_Additive(u8 *r, u8 mode) {
    AT(r, 0x304C05, u8) = 1;
    AT(r, 0x304C06, u8) = mode;
}

#ifdef HG_NATIVE
/* +0x90 the whole screen in colour `rgba` (blended by its alpha), layer 0x31; 1 if drawn */
/* 0x001B9000 */
s32 Renderer_FillScreen(VObject *r, u32 rgba) {
    gl2d_sprite(0x31, 0, 0, 512, 448, NULL, 0, 0, 0, 0, rgba, 0, 0x40);
    return 1;
}
#endif

/* the 17-word argument block handed on to +0x84 */
/* 0x001B9260 */
void Renderer_QuadArgs(VObject *r, s32 *a) {
    VCALL(r, 0x84, void (*)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                            s32, s32, s32))(r, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10],
                                            a[11], a[12], a[13], a[14], a[15], a[16]);
}

/* +0x6C: the layer-0x11 flares (16 x { x0, y0, x1, y1, phase } at +0x304C0C) drift - each
 * phase turns by up to 2 degrees at random, its sine and cosine (halved) nudge the corners,
 * kept to x0 -56..24, y0 -150..0, x1 / y1 32..64 */
/* 0x001B9D50 */
void Renderer_Flares(u8 *r) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB};
    VObject *rnd = gRandom;
    f32 *e = (f32 *)(r + 0x304C0C);
    u32 i;

    for (i = 0; i < 16; i++, e += 5) {
        f32 c, s;

        e[4] = e[4] + kPi.f * (2.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd)) / 180.0f;
        if (!(e[4] <= kPi.f)) {
            e[4] = e[4] - kTwoPi.f;
        }
        c = 0.5f * func_0031C058(e[4]);
        s = 0.5f * func_0031C248(e[4]);
        if (i < 8) {
            e[0] = e[0] + c;
        } else {
            e[0] = e[0] + s;
        }
        if (e[0] < -56.0f) {
            e[0] = -56.0f;
        } else if (!(e[0] <= 24.0f)) {
            e[0] = 24.0f;
        }
        if (i & 1) {
            e[1] = e[1] + s;
        } else {
            e[1] = e[1] + c;
        }
        if (e[1] < -150.0f) {
            e[1] = -150.0f;
        } else if (!(e[1] <= 0.0f)) {
            e[1] = 0.0f;
        }
        if (i & 1) {
            e[2] = e[2] - c;
        } else {
            e[2] = e[2] - s;
        }
        if (e[2] < 32.0f) {
            e[2] = 32.0f;
        } else if (!(e[2] <= 64.0f)) {
            e[2] = 64.0f;
        }
        if (i < 8) {
            e[3] = e[3] - s;
        } else {
            e[3] = e[3] - c;
        }
        if (e[3] < 32.0f) {
            e[3] = 32.0f;
        } else if (!(e[3] <= 64.0f)) {
            e[3] = 64.0f;
        }
    }
}

/* +0x14: the video mode (2 NTSC, 3 PAL; 0x50 progressive 480p): the GS reset for it, the
 * frame height (+0x304BFE: 448 / 512), the mode kept (+0x304C09), the display set up again */
/* 0x001BB9E0 */
void Renderer_SetVideoMode(u8 *r, u8 mode) {
    if (mode == 0x50) {
        func_0010BE10(0, 0, 0x50, 0);
        AT(r, 0x304BFE, s16) = 0x1C0;
    } else {
        func_0010BE10(0, 1, mode, 0);
        if (mode == 2) {
            AT(r, 0x304BFE, s16) = 0x1C0;
        } else {
            AT(r, 0x304BFE, s16) = 0x200;
        }
    }
    AT(r, 0x304C09, u8) = mode;
    Renderer_GsEnvironments(r);
}

_Static_assert(__builtin_offsetof(TexCache, layers) == 0x304, "TexCache.layers");
