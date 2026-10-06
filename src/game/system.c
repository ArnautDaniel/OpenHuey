/* The game's system object (Game +0x69AC0, vtable 0x46ADF0, ticked every frame) and the
 * objects it embeds: file loader, ... Constructors so far. */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "globals.h"
#include "memcard.h"
#include "actor.h"
#include "navmesh.h"
#include "progress.h"
#include "daniella.h"
#include "loader.h"
#include "pad.h"
#include "renderer.h"
#include "rumble.h"
#include "scene_game_members.h"
#include "snd_driver.h"
#include "system.h"
#include "cri/adx.h"
#include "libc.h"
#include "msl.h"
#include "sce/eekernel.h"
#include "sce/intc.h"
#include "sce/iop.h"
#include "sce/libmc.h"
#include "sce/libpad2.h"
#include "sce/sif.h"

extern void *D_0046BEE0[];

#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

s32 Loader_CurrentDone(u8 *p);
s32 Loader_Slot28(u8 *p, s32 kind);
s32 Loader_Slot24(u8 *p);

extern void *Renderer_vtable[];
extern void *D_0046ACF0[];
extern void *D_0046AD88[];
extern void *D_0046ADD0[];
extern void *D_0046AE10[];
extern void *D_0046AF20[];
extern void *Vram_vtable[];
void *Obj46ACF0_dtor(u8 *o, s32 flags);
void *Obj46ADD0_dtor(u8 *o, s32 flags);
void *Obj46AF20_dtor(u8 *o, s32 flags);

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

void Fades_Start(void *self, u8 *p);

/* Game's base class constructor. */
/* 0x0020E7F0 */
void *GameBase_ctor(Game *game) {
    gGamePtr = (u8 *)game;
    game->vtbl = D_0046BEE0;
    game->nextMode = 0;
    game->softResetEnabled = 0;
    game->modeParam = 0;
    return game;
}

/* Starts a 16.16 fade of channel i from `from` to `to` over `frames` (12 bits) frames. */
/* The original null-checks the address of each member (inlined constructors). */
/* 0x002D4680 */
void Fades_Start(void *self, u8 *p) {
    u32 a = (u32)p;
    s32 i;

    p[0] = 0;
    if (a + 0x4 != 0) F(p, 0x4, u16) = 0;
    if (a + 0x8 != 0) F(p, 0x8, u16) = 0;
    if (a + 0xC != 0) F(p, 0xC, u16) = 0;
    if (a + 0x14 != 0) F(p, 0x14, u16) = 0;
    if (a + 0x18 != 0) F(p, 0x18, u16) = 0;
    if (a + 0x1C != 0) F(p, 0x1C, u16) = 0;
    if (a + 0x20 != 0) F(p, 0x20, u16) = 0;
    if (a + 0x24 != 0) {
        for (i = 0; i < 16; i++) {
            p[0x24 + i] = 0;
        }
    }
    F(p, 0x50, u32) = 0;
    F(p, 0x40, u32) = 0;
    F(p, 0x54, u32) = 0;
    F(p, 0x44, u32) = 0;
    F(p, 0x58, u32) = 0;
    F(p, 0x48, u32) = 0;
    F(p, 0x5C, u32) = 0;
    F(p, 0x4C, u32) = 0;
}

/* Element constructors for two arrays in the +0x395D40 object. */
void *func_0020E7D0(void *e) {
    AT(e, 0x8, s32) = 0;
    AT(e, 0x4, s32) = 0;
    AT(e, 0x0, s32) = 0;
    AT(e, 0xC, u8) = 0;
    AT(e, 0xD, u8) = 0;
    return e;
}

void *func_0020E7B0(void *e) {
    AT(e, 0x4, s32) = 0;
    AT(e, 0x0, s32) = 0;
    AT(e, 0xC, s32) = 0;
    AT(e, 0x8, s32) = 0;
    AT(e, 0x14, s32) = 0;
    AT(e, 0x10, u8) = 0;
    return e;
}

/* Global pointers to the parts (set by the constructor). */

extern void *System_vtable[], *D_0046ADD0[], *Pads_vtable[], *D_0046ADC4[], *D_0046AD88[], *MemCard_vtable[], *D_0046AEB4[];
extern void *Renderer_vtable[], *D_0046AF00[], *D_0046AF0C[], *MovieLib_vtable[], *Vram_vtable[], *Loader_vtable[];
extern void *SndDriver_vtable[], *D_0046BF2C[];
extern u8 gInput[], kButtonMap[16], kAnalogMap[16];
extern const PTMF sGameStateNull;

extern void RenderState_Defaults(u8 *r);

/* System object constructor. */
/* 0x0020E340 */
void *System_ctor(u8 *s) {
    u8 *p;
    s32 i;

    AT(s, 0x0, void **) = System_vtable;
    gSystem = (VObject *)s;
    AT(s, 0x20, void **) = D_0046AD88;
    gPad = (VObject *)(s + 0x40);
    AT(s, 0x40, void **) = D_0046ADD0;
    Fades_Start(s + 0x40, gInput);
    for (i = 0; i < 16; i++) {
        kButtonMap[i] = i;
    }
    for (i = 0; i < 16; i++) {
        kAnalogMap[i] = i;
    }
    AT(s, 0x40, void **) = Pads_vtable;
    AT(s, 0x58, void **) = D_0046ADC4;
    Rumble_ctor((Rumble *)(s + 0x300));

    gMemCard = (MemCard *)(s + 0x390);
    AT(s, 0x390, void **) = MemCard_vtable;
    AT(s, 0x39C, void **) = D_0046AEB4;
    gRenderer = (VObject *)(s + 0x460);
    AT(s, 0x3A4, PTMF) = sGameStateNull;
    AT(s, 0x460, void **) = Renderer_vtable;
    RenderState_Defaults(s + 0x460);

    p = s + 0x305280;
    gAdx = p;
    AT(p, 0x0, void **) = D_0046AF00;
    AT(p, 0x4, s32) = 0;
    AT(p, 0x8, s32) = 0;
    AT(p, 0xC, s32) = 0;
    AT(p, 0x10, u8) = 0;
    AT(p, 0x124, void **) = D_0046AF0C;
    AT(p, 0x7C48, u8) = 0;
    gMovieLib = p + 0x7C44;
    AT(p, 0x7C44, void **) = MovieLib_vtable;
    func_00115D20(p + 0x7C4C, 0, 0x20);
    func_00115D20(p + 0x7C6C, 0, 0x30);

    gVram = (VObject *)(s + 0x30CF40);
    AT(s, 0x30CF40, void **) = Vram_vtable;
    gFileLoader = (VObject *)(s + 0x319900);
    AT(s, 0x319900, void **) = Loader_vtable;

    p = s + 0x395D40;
    gSound = (VObject *)(p + 4);
    AT(p, 0x0, void **) = SndDriver_vtable;
    AT(p, 0x4, void **) = D_0046BF2C;
    func_00100340(p + 0x84, func_0020E7D0, (void (*)(void *, s32))IopBuffers_dtor, 0x10, 8);
    func_00100340(p + 0x108, func_0020E7B0, func_001BECA0, 0x18, 8);
    AT(p, 0x80, s32) = 0;
    AT(p, 0x104, s8) = -1;
    AT(p, 0x7EC, s32) = 0x100;
    AT(p, 0x7F0, s32) = 0x100;
    AT(p, 0x7F4, s32) = 0x80;
    AT(p, 0x7F8, s32) = 0x80;
    AT(p, 0x7DC, u8 *) = p + 0x1DC;
    AT(p, 0x7E0, u8 *) = p + 0x3DC;
    AT(p, 0x7E4, u8 *) = p + 0x5DC;
    AT(p, 0x7E8, u8 *) = p + 0x6DC;
    return s;
}

extern u32 _fbss;                 /* first word of .bss: libgraph table entry 2 */

/* Renderer state (system +0x460) defaults: 640x512 display, draw buffers, colour 0x80808080. */
/* 0x001B80C0 */
void RenderState_Defaults(u8 *r) {
    u32 i;

    AT(r, 0x304BE8, s32) = 0;
    AT(r, 0x304BEC, s32) = 0x88000;
    AT(r, 0x304BF0, s32) = 0x50000;
    AT(r, 0x304BF4, u32) = 0x80808080;
    AT(r, 0x304BF8, s32) = 0;
    AT(r, 0x304BFC, s16) = 640;
    AT(r, 0x304BFE, s16) = 512;
    AT(r, 0x304C00, s16) = 512;
    AT(r, 0x304C02, s16) = 448;
    AT(r, 0x304C04, u8) = 0;
    AT(r, 0x304C05, u8) = 0;
    AT(r, 0x304C06, u8) = 0x20;
    AT(r, 0x304C07, u8) = 0;
    AT(r, 0x304C08, u8) = 0;
    AT(r, 0x304C09, u8) = 2;
    AT(r, 0x304BB0, u32) = func_0010D3B8(1);
    _fbss = func_0010D3B8(2);
    AT(r, 0x304BB4, u8) = 0;
    AT(r, 0x304BB5, u8) = 0;
    ((void (*)(u8 *))(*(void ***)r)[0x1C / 4])(r);
    for (i = 0; i < 11; i++) {
        AT(r, 0x304BB8 + i * 4, s32) = -1;
    }
    AT(r, 0x304BE4, s32) = -1;
    AT(r, 0x304D58, s32) = 0;
    AT(r, 0x304DDC, s32) = 0;
}

/* destructor (vtable D_0046ACF0) */
/* 0x001BC090 */
void *Obj46ACF0_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046ACF0;
        gRenderer = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AD88) */
/* 0x001BC320 */
void *Obj46AD88_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AD88;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* Heap (vtable 0x46A1C0) setup: memory, size, block table, block count; then its init (+0xC). */
/* 0x00169260 */
void Heap_Setup(VObject *h, void *base, u32 size, void *blocks, s32 count) {
    AT(h, 0x4, void *) = base;
    AT(h, 0x8, u32) = size;
    AT(h, 0xC, void *) = blocks;
    AT(h, 0x10, s32) = count;
    VCALL(h, 0xC, void (*)(VObject *, void *, u32, void *, s32))(h, base, size, blocks, count);
}

/* Entries of 0x128 bytes; +0x12804 current index, +0x12805 end index. */
/* 0x0016B420 */
s32 Loader_CurrentDone(u8 *p) {
    s32 v = FLD(p + p[0x12804] * 0x128, 0x8, s32);

    return v == 6 || v == 7;
}

/* 0x0016B470 */
s32 Loader_Slot28(u8 *p, s32 kind) {
    u8 end = p[0x12805];
    u8 i = p[0x12804];

    if (i == end) {
        return 3;
    }
    for (; i != end; i++) {
        if (FLD(p + i * 0x128, 0x18, s32) == kind) {
            return 2;
        }
    }
    return 3;
}

/* 0x0016B510 */
s32 Loader_Slot24(u8 *p) {
    if (p[0x12804] == p[0x12805]) {
        return 3;
    }
    return 2;
}

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

extern void *D_0046BF08[], *D_004699E0[], *Heap_vtable[];

/* Game +0x400A00: the scene table (4 scene pointers) and the scene heap after it (Game.sceneHeap,
 * 0x10D9000 bytes from +0x40). */
/* 0x0020E280 */
void *SceneTable_ctor(u8 *t) {
    VObject *heap = (VObject *)(t + 0x10D9040);
    s32 i;

    gSceneTable = t;
    AT(t, 0x0, void **) = D_0046BF08;
    heap->vtbl = D_004699E0;
    AT(heap, 0x4, s32) = 0;
    AT(heap, 0x8, s32) = 0;
    heap->vtbl = Heap_vtable;
    AT(heap, 0xC, s32) = 0;
    AT(heap, 0x10, s32) = 0;
    for (i = 0; i < 4; i++) {
        AT(t, 0x4 + i * 4, void *) = NULL;
    }
    Heap_Setup(heap, t + 0x40, 0x10D9000, t + 0x10D9054, 10);
    return t;
}

extern void *Camera_vtable[];

/* 0x0020E260 */
void *Camera_ctor(VObject *o) {
    gCamera = o;
    o->vtbl = Camera_vtable;
    return o;
}

/* Object pool element constructors. */
/* 0x0020E240 */
void *SmallPool_ElemA(void *e) {
    AT(e, 0x0, s32) = 0;
    AT(e, 0x4, s32) = 0;
    AT(e, 0x8, s32) = 0;
    return e;
}

/* 0x0020E210 */
void *SmallPool_ElemB(void *e) {
    AT(e, 0xC, s32) = 0;
    AT(e, 0x4, s32) = 0;
    AT(e, 0x8, s32) = 0;
    AT(e, 0x10, s32) = 0;
    return e;
}

/* 0x0020E190 */
void *BigPool_ElemA(void *e) {
    AT(e, 0x0, s32) = 0;
    AT(e, 0x4, s32) = 0;
    return e;
}

/* 0x0020E180 */
void *BigPool_ElemB(void *e) {
    AT(e, 0x44, s32) = 0;
    AT(e, 0x48, s32) = 0;
    return e;
}

extern void *gChainPool, *gSkelPool;   /* the two pools */

/* Game +0x14D9DD0: pool of 64 x 0xC and 462 x 0x14 entries. */
/* 0x0020E1A0 */
void *SmallPool_ctor(u8 *p) {
    gChainPool = p;
    func_00100340(p, SmallPool_ElemA, func_0020D970, 0xC, 0x40);
    func_00100340(p + 0x300, SmallPool_ElemB, func_0020D9C0, 0x14, 0x1CE);
    return p;
}

/* Game +0x14DC530: pool of 32 x 0xC and 632 x 0x50 entries. */
/* 0x0020E110 */
void *BigPool_ctor(u8 *p) {
    gSkelPool = p;
    func_00100340(p, BigPool_ElemA, func_0020D8D0, 0xC, 0x20);
    func_00100340(p + 0x180, BigPool_ElemB, func_0020D920, 0x50, 0x278);
    return p;
}

extern void *TexCache_vtable[];

/* reset: 64 empty slots; the ids of the renderer's 10 layers (renderer +0x3C) */
static inline void Slots_Reset(u8 *o) {
    s32 i;

    for (i = 0; i < 64; i++) {
        u8 *e = o + 4 + i * 0xC;

        AT(e, 0x0, s32) = 0;
        AT(e, 0x4, s16) = -1;
        AT(e, 0x6, s16) = -1;
        AT(e, 0x9, s8) = -1;
        AT(e, 0x8, s8) = -1;
    }
    for (i = 0; i < 10; i++) {
        AT(o, 0x304 + i * 4, s32) = -1;
    }
    for (i = 0; i < 10; i++) {
        AT(o, 0x304 + i * 4, s32) = VCALL(gRenderer, 0x3C, s32 (*)(VObject *, s32))(gRenderer, i);
    }
}

/* Game +0x14E8C90 (shut down by Slots_Reset2): constructor */
/* 0x001F4600 */
void *Slots_ctor(u8 *o) {
    AT(o, 0x0, void **) = TexCache_vtable;
    gTexCache = (VObject *)o;
    Slots_Reset(o);
    return o;
}

/* ... init (from Game_Init) */
/* 0x001F44D0 */
void Slots_Init(u8 *o) {
    Slots_Reset(o);
}

/* ---- system init (vtable +0xC, from Game_Init) ---- */

#include "ps2hw.h"

extern const char str_SIO2MAN_IRX[], str_SIO2D_IRX[], str_DBCMAN_IRX[], str_LIBSD_IRX[];   /* SIO2MAN, SIO2D, DBCMAN, LIBSD .IRX */
extern void func_001AACD0(void *obj);
extern s32 func_001BEDA0(s32 cause), func_001BED80(s32 cause);         /* vblank start / end handlers */
extern u8 gVblankStartSeen, gVblankEndSeen;   /* vblank start / end seen */
extern u32 gVblankCount;              /* vblank count */

/* 0x001BF080 */
void System_Init(u8 *s) {
    func_001BC220(s + 0x20);
    func_0010D3E0(1);
    AT(s, 0x4, s32) = func_001BC0F0(s + 0x20, str_SIO2MAN_IRX, 0, 0, 0);
    AT(s, 0x8, s32) = func_001BC0F0(s + 0x20, str_SIO2D_IRX, 0, 0, 0);
    AT(s, 0xC, s32) = func_001BC0F0(s + 0x20, str_DBCMAN_IRX, 0, 0, 0);
    func_001EE798();
    AT(s, 0x10, s32) = func_001BC0F0(s + 0x20, str_LIBSD_IRX, 0, 0, 0);
    func_001AACD0(s + 0x305280);
    SndDriver_Start(s + 0x395D40);
    Pads_Init(s + 0x40);
    func_00226570(s + 0x390);
    Renderer_SetupVideo(s + 0x460, 2);
    VCALL(s + 0x30CF40, 0xC, void (*)(void *))(s + 0x30CF40);
    Renderer_AllocVram(s + 0x460);
    /* timer 0 and 1: count on the horizontal blank */
    HW_WRITE32(0x10000010, 0x82);
    HW_WRITE32(0x10000000, 0);
    HW_WRITE32(0x10000810, 0x82);
    gVblankCount = 0;
    gVblankStartSeen = 0;
    AT(s, 0x14, s32) = func_0026BE80(2, func_001BEDA0, 0);
    func_0026CCE8(2);
    gVblankEndSeen = 0;
    AT(s, 0x18, s32) = func_0026BE80(3, func_001BED80, 0);
    func_0026CCE8(3);
    AT(s, 0x1C, s32) = gVblankCount;
    Loader_Init(s + 0x319900);
    Loader_RegisterAll(s + 0x319900);
}

/* destructor (vtable D_0046AE10) */
/* 0x001BF220 */
void *Obj46AE10_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AE10;
        gSystem = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AF20) */
/* 0x001BF7A0 */
void *Obj46AF20_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AF20;
        gVram = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
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

extern void func_001AACC0(void *snd);     /* ADX sound system tick */

/* +0x10 end of frame: finish the renderer's frame, wait for the vblank (at least two since the
 * last frame: the game runs at 30 fps), restart the timers, send the frame, then tick the parts. */
/* 0x001BEEF0 */
void System_EndFrame(u8 *s) {
    s32 n;

    Renderer_WaitChain(s + 0x460);
    VSYNC_WAIT(gVblankStartSeen);
    n = gVblankCount - AT(s, 0x1C, s32);
    if (n < 0) {
        n = -n;
    }
    if (n < 2) {
        VSYNC_WAIT(gVblankStartSeen);
    }
    AT(s, 0x1C, s32) = gVblankCount;
    HW_WRITE32(0x10000800, 0);   /* timer 1 count */
    Renderer_SendFinal(s + 0x460);
    Renderer_NextClear(s + 0x460);
    VSYNC_WAIT(gVblankEndSeen);
    HW_WRITE32(0x10000000, 0);   /* timer 0 count */
    Renderer_EndFrame(s + 0x460);
    SndDriver_Frame(s + 0x395D40);
    func_001AACC0(s + 0x305280);
    Loader_Tick(s + 0x319900);
    Rumble_Tick((Rumble *)(s + 0x300));
    Pads_Tick(s + 0x40);
    MemCard_Tick((MemCard *)(s + 0x390));
}

/* ---- the rest of the system object (2026-10-05) ---- */

/* the pads (+0x40): close the socket, end the library */
/* 0x001BE480 */
void Pads_Shutdown(u8 *pads) {
    func_001EFB40(AT(pads, 0x40, s32));
    func_001EF9D0();
}

/* destructor (vtable D_0046ADD0) */
/* 0x001BE730 */
void *Obj46ADD0_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046ADD0;
        gPad = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* +0x395D40 +0x84 element (8 x 0x10): two IOP buffers */
/* 0x001BEC10 */
void *IopBuffers_dtor(u8 *e, s32 flags) {
    if (e == NULL) {
        return e;
    }
    if (AT(e, 0x0, u32) != 0) {
        func_00274640(AT(e, 0x0, u32));
        AT(e, 0x0, u32) = 0;
    }
    if (AT(e, 0x4, u32) != 0) {
        func_00274640(AT(e, 0x4, u32));
        AT(e, 0x4, u32) = 0;
    }
    AT(e, 0x8, s32) = 0;
    AT(e, 0xC, u8) = 0;
    AT(e, 0xD, u8) = 0;
    if ((s16)flags > 0) {
        func_00100490(e);
    }
    return e;
}

/* destructor (vtable ?) */
void *func_001BECA0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x4, s32) = 0;
        AT(o, 0x0, s32) = 0;
        AT(o, 0xC, s32) = 0;
        AT(o, 0x8, s32) = 0;
        AT(o, 0x14, s32) = 0;
        AT(o, 0x10, u8) = 0;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

extern void *D_0046AE10[], *D_0046AF90[], *D_0046AF20[], *D_0046A220[], *D_0046ACF0[];
extern void *D_0046AEC0[], *D_0046AED0[], *D_0046AE60[], *Rumble_vtable[], *D_0046AE30[];

/* destructor (vtable +0x8): the members in reverse, each with its vtable chain and global cleared */
/* 0x001BE7A0 */
void *System_dtor(u8 *s, s32 flags) {
    u8 *p;

    if (s == NULL) {
        return s;
    }
    AT(s, 0x0, void **) = System_vtable;

    p = s + 0x395D40;   /* sound driver */
    AT(p, 0x0, void **) = SndDriver_vtable;
    AT(p, 0x4, void **) = D_0046BF2C;
    func_001002C0(p + 0x108, (void *(*)(void *, s32))func_001BECA0, 0x18, 8);
    func_001002C0(p + 0x84, (void *(*)(void *, s32))IopBuffers_dtor, 0x10, 8);
    AT(p, 0x4, void **) = D_0046AF90;
    gSound = NULL;
    AT(p, 0x0, void **) = D_0046AD88;

    AT(s, 0x319900, void **) = Loader_vtable;   /* file loader */
    AT(s, 0x319900, void **) = D_0046A220;
    gFileLoader = NULL;

    AT(s, 0x30CF40, void **) = Vram_vtable;
    AT(s, 0x30CF40, void **) = D_0046AF20;
    gVram = NULL;

    p = s + 0x305280;   /* ADX sound system */
    AT(p, 0x0, void **) = D_0046AF00;
    AT(p, 0x124, void **) = D_0046AF0C;
    AT(p, 0x7C44, void **) = MovieLib_vtable;
    AT(p, 0x7C44, void **) = D_0046AED0;
    AT(p, 0x7C48, u8) = 0;
    gMovieLib = NULL;
    AT(p, 0x124, void **) = D_0046AD88;
    AT(p, 0x0, void **) = D_0046AEC0;
    AT(p, 0x4, s32) = 0;
    AT(p, 0x8, s32) = 0;
    AT(p, 0xC, s32) = 0;
    gAdx = NULL;

    AT(s, 0x460, void **) = Renderer_vtable;   /* renderer */
    AT(s, 0x460, void **) = D_0046ACF0;
    gRenderer = NULL;

    AT(s, 0x390, void **) = MemCard_vtable;   /* memory card */
    AT(s, 0x39C, void **) = D_0046AEB4;
    AT(s, 0x39C, void **) = D_0046AD88;
    AT(s, 0x390, void **) = D_0046AE60;
    gMemCard = NULL;

    AT(s, 0x300, void **) = Rumble_vtable;   /* rumble */
    AT(s, 0x300, void **) = D_0046AE30;
    gRumble = NULL;

    AT(s, 0x40, void **) = Pads_vtable;    /* pads */
    AT(s, 0x58, void **) = D_0046ADC4;
    AT(s, 0x58, void **) = D_0046AD88;
    AT(s, 0x40, void **) = D_0046ADD0;
    gPad = NULL;

    AT(s, 0x20, void **) = D_0046AD88;
    AT(s, 0x0, void **) = D_0046AE10;
    gSystem = NULL;
    if ((s16)flags > 0) {
        func_00100490(s);
    }
    return s;
}

/* +0x18 shutdown: the vblank handlers off, then each part's shutdown, then reset the GS */
/* 0x001BEDD0 */
void System_Shutdown(u8 *s) {
    func_0026CC80(2);
    func_0026CC80(3);
    RemoveIntcHandler(3, AT(s, 0x18, s32));
    RemoveIntcHandler(2, AT(s, 0x14, s32));
    Pads_Shutdown(s + 0x40);
    MemCard_Shutdown((MemCard *)(s + 0x390));
    func_001EEA38();
    Loader_CloseAll(s + 0x319900);
    SndDriver_FreeIop(s + 0x395D40);
    MovieLib_Shutdown(s + 0x305280);
    func_0010D3E0(0);
}

/* +0x14 frame without the vblank wait: finish and send the renderer's frame, tick the parts */
/* 0x001BEE70 */
void System_FrameNoWait(u8 *s) {
    Renderer_WaitChain(s + 0x460);
    func_0023C310();
    Renderer_EndFrame(s + 0x460);
    SndDriver_Frame(s + 0x395D40);
    func_001AACC0(s + 0x305280);
    Loader_Tick(s + 0x319900);
    Rumble_Tick((Rumble *)(s + 0x300));
    Pads_Tick(s + 0x40);
}
