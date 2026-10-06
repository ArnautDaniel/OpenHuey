/* The renderer (system +0x460, vtable 0x46AC50, global gRenderer). It builds PS2 DMA / GIF
 * packets in double-buffered arenas inside itself; the PC build interprets those packets where
 * the PS2 would send them (libdma / libgraph), so this code stays as the game had it. */
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

#include "gs.h"

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

#include "ptmf.h"

extern PTMF D_0047E300[];     /* palette generators by mode: (this, index, arg) -> RGBA */

#ifdef HG_NATIVE
/* +0x94: build a 256-colour palette with generator `mode` (D_0047E300) for VRAM slot `slot`'s
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

#include "progress.h"

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
