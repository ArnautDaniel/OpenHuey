/* The item manager (gSubScreen): Fiona's inventory and equipment.
 *
 * (was item_classes.c) The item classes (pool entries, vtable PoolEntry_vtable): their
 * destructors. Each item has its own class (+0x28 loads its picture, +0x3C uses it) on one of a
 * few middle classes (PlainItem_vtable the plain item, ItemClassCB50_vtable ..., see the
 * vtables); the destructors only step the vtable back down the chain and free the entry.
 *
 * (was synth.c) The item synthesizer (the sub screen's modes 3 and 4, "SUBSCR\SYNSLOT"): a
 * 0x15C-byte page object at +0xA8C80 of the sub screen (subscreen.c sub_new_slots), base class
 * SynthBase_vtable with the pot (mode 4, SynthPot_vtable) and the slot machine (mode 3,
 * SlotMachine_vtable). Fiona picks one of six materials, the machine rolls ten rows of symbols;
 * matching pairs among 22 cells (or the wild symbol 5) count up four kinds of symbol, which pick
 * the item made. The object: +0x4 its state (PTMF), +0x10 the row being rolled (-1 before the
 * first) or the list's cursor, +0x14 its message task, +0x118 the six materials' counts (then the
 * found item's places), +0x11E the material used, +0x11F the roll's flags (1 rolling, 2 all rows
 * done, 4 a row stopping), +0x120 the ten rows' symbols (-1 not rolled), +0x12A the reel
 * (D_00460430: 35 symbols each), +0x12B its position, +0x12C the step between symbols, +0x12D /
 * +0x12E the roll's timer and speed, +0x12F the 22 cells' symbols (-1 no match), +0x145 the four
 * symbols' counts, +0x14A the result (-1 none), +0x14B how many, +0x14C the item (big endian),
 * +0x150 the prompt's blink, +0x151 the found item's frame, +0x152 its sound played, +0x158
 * "leave".
 */
#include "common.h"
#include "game.h"
#include "globals.h"
#include "ptmf.h"
#include "progress.h"
#include "actor.h"
#include "items.h"
#include "subscreen.h"
#include "item.h"
#include "pursuer.h"
#include "hewie.h"
#include "model.h"
#include "renderer.h"
#include "sound.h"
#include "vecmath.h"
#include "msl.h"
#include "sce/libvu0.h"
#include "navmesh.h"
#include "memcard.h"
#include "effects.h"
#include "loader.h"
#include "scene_game.h"
#include "heap.h"
#include "effectmgr.h"
#include <stdint.h>
#include "scene_boot.h"
#include "input.h"
#include "text.h"
#include "movie.h"
#include "gl2d.h"
#include "scene_title.h"
#include "music.h"
#include "libc.h"
#include "gs.h"
#include "fiona.h"
#include "daniella.h"
#include "pad.h"
#include "scene.h"
#include "system.h"
#include "sce/iop.h"
#include "director.h"
#include "doors.h"
#include "room.h"
#include "placed.h"
#include "room_map.h"
#include "lights.h"
#include "draw_leaves.h"
#include "sce/eekernel.h"
#include "sce/intc.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif

extern void *PoolEntry_vtable[], *ItemClassF430_vtable[];
extern void *ItemAC_vtable[];

ItemObj *ItemAC_ctor(ItemObj *self);
ItemObj *Item3F_ctor(ItemObj *self, s32 id);

void Item3F_Set(u8 *p, const u8 *src);

s32 PoolEntry_PartnerCheck(void *o);
extern void *Item12_vtable[], *Item13_vtable[], *Item3E_vtable[], *PlainItem_vtable[], *ItemClassC920_vtable[], *Item70_vtable[], *Item71_vtable[], *Item72_vtable[], *Item73_vtable[], *Item74_vtable[], *Item75_vtable[], *ItemClassCB50_vtable[], *Item80_vtable[], *Item81_vtable[], *Item82_vtable[], *Item83_vtable[], *ItemClassCCE0_vtable[], *Item86_vtable[], *Item87_vtable[], *Item88_vtable[], *Item89_vtable[], *ItemClassCE70_vtable[], *Item8A_vtable[], *Item8B_vtable[], *Item8C_vtable[], *Item8D_vtable[], *ItemClassD000_vtable[], *Item90_vtable[], *Item91_vtable[], *Item92_vtable[], *Item93_vtable[], *Item94_vtable[], *Item95_vtable[], *Item97_vtable[], *Item98_vtable[], *Item9B_vtable[], *ItemClassD320_vtable[], *ItemA4_vtable[], *ItemA5_vtable[], *ItemA6_vtable[], *ItemA7_vtable[], *ItemA8_vtable[], *ItemA9_vtable[], *ItemAA_vtable[], *ItemAB_vtable[], *ItemClassD640_vtable[], *Item40_vtable[], *Item41_vtable[], *Item00_vtable[], *Item01_vtable[], *Item02_vtable[], *Item03_vtable[], *Item04_vtable[], *Item05_vtable[], *ItemA0_vtable[], *ItemA1_vtable[], *Item06_vtable[], *Item07_vtable[], *Item08_vtable[], *Item09_vtable[], *Item0A_vtable[], *Item0B_vtable[], *Item0C_vtable[], *Item0D_vtable[], *Item0E_vtable[], *Item0F_vtable[], *Item10_vtable[], *Item11_vtable[], *Item14_vtable[], *Item42_vtable[], *Item43_vtable[], *Item44_vtable[], *Item45_vtable[], *Item46_vtable[], *Item47_vtable[], *Item48_vtable[], *Item49_vtable[], *Item4A_vtable[], *Item4B_vtable[], *Item4C_vtable[], *Item60_vtable[], *Item61_vtable[], *Item62_vtable[], *Item63_vtable[], *Item64_vtable[], *Item65_vtable[], *Item66_vtable[], *Item15_vtable[], *Item16_vtable[], *Item17_vtable[], *ItemA2_vtable[], *ItemClass6B00_vtable[], *ItemA3_vtable[], *Item18_vtable[], *Item19_vtable[], *Item1A_vtable[], *Item1B_vtable[], *Item1C_vtable[], *Item1D_vtable[], *Item1E_vtable[], *Item1F_vtable[], *Item20_vtable[], *Item21_vtable[], *Item22_vtable[], *Item23_vtable[], *Item24_vtable[], *Item25_vtable[], *Item26_vtable[], *Item27_vtable[], *Item28_vtable[], *Item29_vtable[];
extern char str_ITEM01_ITEM_022_TEX[]; /* file name */
extern char str_ITEM01_ITEM_021_TEX[]; /* file name */
extern char str_ITEM01_ITEM_020_TEX[]; /* file name */
extern char str_ITEM01_ITEM_01F_TEX[]; /* file name */
extern char str_ITEM01_ITEM_01E_TEX[]; /* file name */
extern char str_ITEM01_ITEM_01D_TEX[]; /* file name */
extern char str_ITEM04_ITEM_061_TEX[]; /* file name */
extern char str_ITEM04_ITEM_060_TEX[]; /* file name */
extern char str_ITEM03_ITEM_05F_TEX[]; /* file name */
extern char str_ITEM03_ITEM_05E_TEX[]; /* file name */
extern char str_ITEM03_ITEM_05D_TEX[]; /* file name */
extern char str_ITEM03_ITEM_05C_TEX[]; /* file name */
extern char str_ITEM03_ITEM_05B_TEX[]; /* file name */
extern char str_ITEM03_ITEM_05A_TEX[]; /* file name */
extern char str_ITEM03_ITEM_059_TEX[]; /* file name */
extern char str_ITEM03_ITEM_058_TEX[]; /* file name */
extern char str_ITEM03_ITEM_057_TEX[]; /* file name */
extern char str_ITEM03_ITEM_056_TEX[]; /* file name */
extern u8 D_0045D4A0[], str_ITEM00_ITEM_001_TEX[], str_ITEM00_ITEM_002_TEX[], str_ITEM00_ITEM_003_TEX[], str_ITEM00_ITEM_004_TEX[], str_ITEM00_ITEM_005_TEX[];
extern char str_ITEM01_ITEM_02F_TEX[];
extern char str_ITEM01_ITEM_030_TEX[];
extern u8 str_ITEM02_ITEM_03F_TEX[];
extern u8 str_ITEM02_ITEM_040_TEX[];
extern u8 str_ITEM02_ITEM_041_TEX[];
extern u8 str_ITEM02_ITEM_042_TEX[];
extern u8 str_ITEM02_ITEM_043_TEX[];
extern u8 str_ITEM02_ITEM_039_TEX[];
extern u8 str_ITEM02_ITEM_031_TEX[];
extern u8 str_ITEM02_ITEM_03A_TEX[];
extern u8 str_ITEM02_ITEM_046_TEX[];
extern u8 str_ITEM02_ITEM_03B_TEX[];
extern u8 str_ITEM01_ITEM_02C_TEX[];
extern u8 str_ITEM00_ITEM_00B_TEX[];
extern u8 str_ITEM00_ITEM_00C_TEX[];
extern u8 str_ITEM00_ITEM_00D_TEX[];
extern u8 str_ITEM00_ITEM_00E_TEX[];
extern u8 str_ITEM00_ITEM_00F_TEX[];
extern u8 str_ITEM00_ITEM_010_TEX[];
extern u8 str_ITEM00_ITEM_011_TEX[];
extern u8 str_ITEM00_ITEM_012_TEX[];
extern u8 str_ITEM00_ITEM_013_TEX[];
extern u8 D_00461020[];
extern u8 str_ITEM00_ITEM_015_TEX[];
extern u8 str_ITEM00_ITEM_016_TEX[];
extern u8 str_ITEM00_ITEM_017_TEX[];
extern u8 str_ITEM01_ITEM_018_TEX[];
extern u8 str_ITEM01_ITEM_019_TEX[];
extern u8 str_ITEM01_ITEM_01A_TEX[];
extern u8 str_ITEM01_ITEM_01B_TEX[];
extern u8 str_ITEM01_ITEM_01C_TEX[];
extern u8 str_ITEM02_ITEM_03C_TEX[];
extern u8 str_ITEM02_ITEM_03E_TEX[];
extern u8 str_ITEM02_ITEM_03D_TEX[];
extern u8 str_ITEM02_ITEM_044_TEX[];
extern u8 str_ITEM02_ITEM_045_TEX[];
extern u8 str_ITEM02_ITEM_033_TEX[];
extern u8 str_ITEM02_ITEM_034_TEX[];
extern u8 str_ITEM02_ITEM_035_TEX[];
extern u8 str_ITEM02_ITEM_036_TEX[];
extern u8 str_ITEM02_ITEM_037_TEX[];
extern u8 str_ITEM02_ITEM_038_TEX[];
extern u8 str_ITEM02_ITEM_047_TEX[];
extern u8 str_ITEM03_ITEM_048_TEX[];
extern u8 str_ITEM03_ITEM_049_TEX[];
extern u8 str_ITEM03_ITEM_04B_TEX[];
extern u8 str_ITEM03_ITEM_04C_TEX[];
extern u8 str_ITEM03_ITEM_04D_TEX[];
extern u8 str_ITEM03_ITEM_04E_TEX[];
extern u8 str_ITEM03_ITEM_04F_TEX[];
extern u8 str_ITEM03_ITEM_050_TEX[];
extern u8 str_ITEM03_ITEM_051_TEX[];
extern char str_ITEM03_ITEM_052_TEX[];
extern const char D_0045A7C0[], D_0045A8A0[], D_0045A8C0[], D_0045A8E0[], D_0045A900[], D_0045A920[], str_ITEM03_ITEM_055_TEX[], str_ITEM03_ITEM_054_TEX[], str_ITEM03_ITEM_053_TEX[], str_ITEM03_ITEM_04A_TEX[], str_ITEM01_ITEM_02A_TEX[], str_ITEM01_ITEM_029_TEX[], str_ITEM01_ITEM_026_TEX[], str_ITEM01_ITEM_028_TEX[], str_ITEM01_ITEM_025_TEX[], str_ITEM01_ITEM_02B_TEX[], str_ITEM01_ITEM_027_TEX[], str_ITEM01_ITEM_024_TEX[], str_ITEM01_ITEM_023_TEX[], str_ITEM00_ITEM_00A_TEX[], str_ITEM00_ITEM_009_TEX[], str_ITEM00_ITEM_006_TEX[], str_ITEM00_ITEM_007_TEX[], str_ITEM04_ITEM_062_TEX[];
typedef s32 (*LoaderLoadFn)(void *loader, const char *name, void *dest, s32 flags, s32 arg);

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

/* gFileLoader->vfunc_0xC(name, arg, 0x4000000, 0) */
#define LOAD(name, arg) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, void *, s32, s32, s32))(gFileLoader, name, arg, 0x4000000, 0)

/* gFileLoader->vfunc_0xC(name, dest, 0x4000000, 0): start loading a file */
#define LOAD_002D1360(name, dest) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, const char *, void *, s32, s32))(gFileLoader, name, dest, 0x4000000, 0)

/* gFileLoader vtable +0xC: start loading file `name` into `dest` (flags 0x4000000) */
#define FILE_LOAD_ASYNC(name, dest) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, void *, void *, u32, s32))(gFileLoader, name, dest, 0x4000000, 0)

#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

void *SynthBase_new(u32 size, void *mem);

extern const u8 D_00460A00[22];
extern const char str_xN_2[];                         /* its count's format */
extern const PTMF SlotMachine_StateChoose_ptmf2, SlotMachine_StateNothing_ptmf2;
void ItemFound_StateShow(u8 *o);

void *PoolEntry_ctor(void *e);
void *PoolEntry_dtor(void *e, s32 flags);

u32 Items_FirstFree(u8 *o, u8 row);
void *Items_GiveWithData(u8 *items, u32 id, u64 *data);

extern void *SynthBase_vtable[];
extern u8 gLanguage;
extern const char D_00460408[];                     /* the count's format */
extern const char str_ITEM_SYNTHESIZER_POT[];                     /* "ITEM SYNTHESIZER(POT)" */
extern const PTMF SynthPot_StateDebug_ptmf, SlotMachine_StateChoose_ptmf, SlotMachine_StateNothing_ptmf, SlotMachine_StateRoll_ptmf, ItemFound_StateShow_ptmf;
void SynthBase_Noop(void *o);
extern void *SynthPot_vtable[], *SlotMachine_vtable[];
extern const u8 D_00460430[][35];      /* the reels */
extern const u8 D_004605C0[][11];      /* per material: the reels' chances (%) */
extern const u8 kSlotReelsPicked[11];        /* the reels picked by them (the last: none picked) */
extern const u8 D_00460620[][6];       /* per material: the first row's symbols' chances (%) */
extern const u8 D_00460650[22][2];     /* the cells: the two rows they compare */
extern const s32 D_00460680[][9];      /* per result: 8 items (-1 none), its amounts' row */
extern const s8 D_00460A18[][4];       /* the amounts by how many symbols matched */
typedef void (*RectFn)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, u32, s32, s32, s32, s32);

#define SY_TASK(o) ((Task *)((o) + 0x14))

#define SY_STEP(o) AT(o, 0x10, s32)

#define SY_STATE(o) ((PTMF *)((o) + 0x4))

/* a sprite from the synthesizer's texture (4 of group 0x19), layer 0x30 */
static inline void sy_rect(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, u32 rgba, s32 tex, s32 group, s32 clut) {
    VCALL(gRenderer, 0x7C, RectFn)(gRenderer, x, y, w, h, u, v, w, h, rgba, tex, group, 0x30, clut);
}

/* 128 x `f` clamped to 0x80, as the alpha byte of a grey colour */
static inline u32 sy_alpha(f32 f, u32 add) {
    u32 a = (u32)(128.0f * f) + add;

    if (a >= 0x81) {
        a = 0x80;
    }
    return ((a << 24) & 0xFF000000) | 0x808080;
}

/* a subclass's +0x8: destroy as the base (its task too), freed when `flags` > 0 */
static inline void *sy_dtor(u8 *o, void **vtbl, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = vtbl;
        if (o != NULL) {
            AT(o, 0x0, void **) = SynthBase_vtable;
            VCALL(o, 0x14, void (*)(u8 *))(o);
            Task_dtor(SY_TASK(o), -1);
        }
        if ((s16)flags > 0) {
            SynthBase_Noop(o);
        }
    }
    return o;
}

/* the reel's symbol at position `pos` */
#define SY_REEL(o, pos) D_00460430[(s8)(o)[0x12A]][(s8)(pos)]

/* RNG +0x18 / +0x1C: a random 0..1 */
#define SY_RAND(k) VCALL(gRandom, k, f32 (*)(VObject *))(gRandom)

/* the reel steps on (every 5 frames, round its 35 symbols) */
static inline void sy_reel_step(u8 *o) {
    o[0x12C] = 0;
    o[0x12B]++;
    if ((s8)o[0x12B] >= 0x23) {
        o[0x12B] = 0;
    }
}

/* all rows rolled: the cells matched (two rows alike, or one the wild 5), the symbols counted,
 * the result's item picked at random among those still to get (the medallions 0x80..0x8F only
 * once; none left: item 0x75) with its amount by how many matched; then the found item's state
 * (ItemFound_StateShow_ptmf) */
static void sy_finish(u8 *o) {
    s32 c, res, total, size, n;

    VCALL(gSound, 0x18, void (*)(VObject *, s32, s32))(gSound, 1, 6);   /* the roll's sound stopped */
    o[0x145] = 0;
    o[0x146] = 0;
    o[0x147] = 0;
    o[0x148] = 0;
    o[0x149] = 0;
    o[0x14A] = 0;
    for (c = 0; c < 22; c++) {
        s8 r1 = o[0x120 + D_00460650[c][0]], r2 = o[0x120 + D_00460650[c][1]];

        o[0x12F + c] = -1;
        if (r1 == r2 || r1 == 5 || r2 == 5) {
            s8 v = r1;

            if (v == 5) {
                v = r2;
            }
            if (v == 5) {
                v = 0;
            }
            o[0x12F + c] = v;
            o[0x145 + v]++;
        }
    }
    o[0x14A] = res = SlotMachine_Result(o);
    if ((s8)res >= 0) {
        total = (s8)o[0x145] + (s8)o[0x146] + (s8)o[0x147] + (s8)o[0x148];
        size = -1;
        if (total >= 8) {
            size = 0;
        } else if (total >= 6) {
            size = 1;
        } else if (total >= 4) {
            size = 2;
        } else if (total > 0) {
            size = 3;
        }
        n = 0;
        if (size >= 0) {
            n = D_00460A18[D_00460680[(s8)o[0x14A]][8]][size];
        }
        if (n == 0) {
            o[0x14A] = -1;
        } else {
            u8 *items = (u8 *)gSubScreen + 8;
            s32 list[8], k = 0, j, id;

            for (j = 0; j < 8; j++) {
                id = D_00460680[(s8)o[0x14A]][j];
                if (id == -1) {
                    continue;
                }
                if ((u32)id >= 0x80 && (u32)id < 0x90 && (u8)Items_Count(items, id)) {
                    continue;
                }
                list[k++] = id;
            }
            if (k == 0) {
                id = 0x75;
                n = 1;
            } else {
                f32 r = SY_RAND(0x1C);

                id = list[(u8)((u32)(10.0f * (f32)(u32)k * r) / 10)];
            }
            o[0x14C] = (u32)id >> 24;
            o[0x14D] = (u32)id >> 16;
            o[0x14E] = (u32)id >> 8;
            o[0x14F] = id;
            o[0x14B] = n;
        }
    }
    o[0x151] = 0;
    o[0x152] = 0;
    ptmf_set(SY_STATE(o), &ItemFound_StateShow_ptmf);
}

#ifdef HG_NATIVE
/* a symbol's palette (symbols 0..5; others 1) */
static s32 sy_clut(u8 v) {
    static const u8 kClut[6] = {6, 3, 4, 5, 0, 7};

    return v < 6 ? kClut[v] : 1;
}
#endif

void *SynthBase_Destroy(u8 *o);
void SynthBase_Noop(void *o);
void SynthBase_Update(u8 *o);
void SynthBase_Reset(u8 *self);
void *SynthPot_dtor(u8 *o, s32 flags);
void SynthPot_StateDebug(u8 *self);
void SynthPot_Noop14(void);
void *SlotMachine_dtor(u8 *o, s32 flags);
void SynthBase_Noop14(void);
void SynthPot_Start(u8 *o);
void SynthPot_Update(u8 *o);
void SlotMachine_Start(u8 *o);
void SlotMachine_Update(u8 *o);
void SlotMachine_StateNothing(u8 *o);
s8 SlotMachine_MaterialList(u8 *o);
void SlotMachine_StateChoose(u8 *o);
void SlotMachine_Noop14(void);
s32 SlotMachine_Result(u8 *o);
void SlotMachine_StateRoll(u8 *o);

/* gCharPlayer +0x1AD5F4: f32 clamped to 0..100; +0x1AD5F8: s32 clamped to 0..1800 */
static inline void b5_adjust_meters(f32 df, s32 di) {
    u8 *g = (u8 *)gCharPlayer;
    f32 f = F32(g, 0x1AD5F4) + df;

    F32(g, 0x1AD5F4) = f;
    if (f < 0.0f) {
        F32(g, 0x1AD5F4) = 0.0f;
    } else if (!(f <= 100.0f)) {
        F32(g, 0x1AD5F4) = 100.0f;
    }
    S32(g, 0x1AD5F8) += di;
    if (S32(g, 0x1AD5F8) < 0) {
        S32(g, 0x1AD5F8) = 0;
    } else if (S32(g, 0x1AD5F8) > 1800) {
        S32(g, 0x1AD5F8) = 1800;
    }
}

/* gProgress +0x7E0 += d; +0x7D0 += d - (+0x7D4) unless that is negative */
static inline void b5_add_7E0(f32 d) {
    u8 *p = (u8 *)gProgress;
    f32 r;

    F32(p, 0x7E0) += d;
    r = d - F32(p, 0x7D4);
    if (!(r < 0.0f)) {
        F32(p, 0x7D0) += r;
    }
}

#define B5_HANDY(o) (VCALL(o, 0x40, s32 (*)(void *))(o) & 0xFF)

/* a middle class: its vtable, then the pool entry's */
static inline void *item_dtor2(void *o, s32 flags, void **own) {
    if (o != NULL) {
        AT(o, 0x0, void **) = own;
        if (o != NULL) {
            AT(o, 0x0, void **) = PoolEntry_vtable;
        }
        if ((s16)flags > 0) {
            SubPool_delete(o);
        }
    }
    return o;
}

/* an item class: its vtable, its middle class's, the pool entry's */
static inline void *item_dtor3(void *o, s32 flags, void **own, void **mid) {
    if (o != NULL) {
        AT(o, 0x0, void **) = own;
        if (o != NULL) {
            AT(o, 0x0, void **) = mid;
            if (o != NULL) {
                AT(o, 0x0, void **) = PoolEntry_vtable;
            }
        }
        if ((s16)flags > 0) {
            SubPool_delete(o);
        }
    }
    return o;
}

#define ITEM_LOAD(name, dst) \
    VCALL(gFileLoader, 0xC, s32 (*)(VObject *, const char *, s32, s32, s32))(gFileLoader, name, dst, 0x4000000, 0)

#define ITEM_HELD(o) (VCALL(o, 0x18, s32 (*)(void *))(o) & 0xFF)

/* used at event spot `spot` of room `room` while Fiona stands in it: event `ev` */
static s32 use_at_spot(s32 room, s32 spot, s32 ev) {
    VObject *ev_mgr;

    if (VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) != room) {
        return 0;
    }
    ev_mgr = gEvents;
    if (!item_at_spot(ev_mgr, gCharPlayer, spot)) {
        return 0;
    }
    item_event(ev_mgr, 0, ev, gCharPlayer);
    return 4;
}

/* used at open door `door` of room `room`: event `ev`, flag 0x18 */
static s32 use_at_door(s32 room, u32 door, s32 ev) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) != room || !item_door_open(p, door)) {
        return 0;
    }
    item_event(gEvents, 0, ev, gCharPlayer);
    Progress_SetFlag(p, 0x18);
    return 4;
}

void *PlainItem_dtor(void *o, s32 flags);
void *ItemClassC920_dtor(void *o, s32 flags);
void *ItemClassCB50_dtor(void *o, s32 flags);
void *ItemClassCCE0_dtor(void *o, s32 flags);
void *ItemClassCE70_dtor(void *o, s32 flags);
void *ItemClassD000_dtor(void *o, s32 flags);
void *ItemClassD320_dtor(void *o, s32 flags);
void *ItemClassD640_dtor(void *o, s32 flags);
void *ItemClassF430_dtor(void *o, s32 flags);
void *ItemClass6B00_dtor(void *o, s32 flags);
void *Item12_dtor(void *o, s32 flags);
void *Item13_dtor(void *o, s32 flags);
void *Item3E_dtor(void *o, s32 flags);
void *Item70_dtor(void *o, s32 flags);
s32 Item70_LoadPicture(void *self, void *dest);
void *Item71_dtor(void *o, s32 flags);
s32 Item71_LoadPicture(void *self, void *dest);
void *Item72_dtor(void *o, s32 flags);
s32 Item72_LoadPicture(void *self, void *dest);
void *Item73_dtor(void *o, s32 flags);
s32 Item73_LoadPicture(void *self, void *dest);
void *Item74_dtor(void *o, s32 flags);
s32 Item74_LoadPicture(void *self, void *dest);
void *Item75_dtor(void *o, s32 flags);
s32 Item75_LoadPicture(void *self, void *dest);
void *Item80_dtor(void *o, s32 flags);
s32 Item80_LoadPicture(void *self, void *dest);
void *Item81_dtor(void *o, s32 flags);
s32 Item81_LoadPicture(void *self, void *dest);
void *Item82_dtor(void *o, s32 flags);
s32 Item82_LoadPicture(void *self, void *dest);
void *Item83_dtor(void *o, s32 flags);
s32 Item83_LoadPicture(void *self, void *dest);
void *Item86_dtor(void *o, s32 flags);
s32 Item86_LoadPicture(void *self, void *dest);
void *Item87_dtor(void *o, s32 flags);
s32 Item87_LoadPicture(void *self, void *dest);
void *Item88_dtor(void *o, s32 flags);
s32 Item88_LoadPicture(void *self, void *dest);
void *Item89_dtor(void *o, s32 flags);
s32 Item89_LoadPicture(void *self, void *dest);
void *Item8A_dtor(void *o, s32 flags);
s32 Item8A_LoadPicture(void *self, void *dest);
void *Item8B_dtor(void *o, s32 flags);
s32 Item8B_LoadPicture(void *self, void *dest);
void *Item8C_dtor(void *o, s32 flags);
s32 Item8C_LoadPicture(void *self, void *dest);
void *Item8D_dtor(void *o, s32 flags);
s32 Item8D_LoadPicture(void *self, void *dest);
void *Item90_dtor(void *o, s32 flags);
void *Item91_dtor(void *o, s32 flags);
void *Item92_dtor(void *o, s32 flags);
void *Item93_dtor(void *o, s32 flags);
void *Item94_dtor(void *o, s32 flags);
void *Item95_dtor(void *o, s32 flags);
void *Item97_dtor(void *o, s32 flags);
void *Item98_dtor(void *o, s32 flags);
void *Item9B_dtor(void *o, s32 flags);
void *ItemA4_dtor(void *o, s32 flags);
void *ItemA5_dtor(void *o, s32 flags);
void *ItemA6_dtor(void *o, s32 flags);
void *ItemA7_dtor(void *o, s32 flags);
void *ItemA8_dtor(void *o, s32 flags);
void *ItemA9_dtor(void *o, s32 flags);
void *ItemAA_dtor(void *o, s32 flags);
void *ItemAB_dtor(void *o, s32 flags);
void *ItemAC_dtor(void *o, s32 flags);
void *Item40_dtor(void *o, s32 flags);
void *Item41_dtor(void *o, s32 flags);
void *Item00_dtor(void *o, s32 flags);
s32 Item00_LoadPicture(void *self, s32 a);
void *Item01_dtor(void *o, s32 flags);
void *Item02_dtor(void *o, s32 flags);
s32 Item02_LoadPicture(void *self, s32 a);
s32 Item02_Use(void *o);
void *Item03_dtor(void *o, s32 flags);
s32 Item03_LoadPicture(void *self, s32 a);
s32 Item03_Use(void);
void *Item04_dtor(void *o, s32 flags);
s32 Item04_LoadPicture(void *self, s32 a);
s32 Item04_Use(void);
void *Item05_dtor(void *o, s32 flags);
s32 Item05_LoadPicture(void *self, s32 a);
s32 Item05_Use(void);
void *ItemA0_dtor(void *o, s32 flags);
s32 ItemA0_LoadPicture(void *self, void *dest);
void *ItemA1_dtor(void *o, s32 flags);
s32 ItemA1_LoadPicture(void *self, void *dest);
void *Item06_dtor(void *o, s32 flags);
void *Item07_dtor(void *o, s32 flags);
void *Item08_dtor(void *o, s32 flags);
s32 Item08_LoadPicture(void *self, void *dest);
void *Item09_dtor(void *o, s32 flags);
s32 Item09_LoadPicture(void *self, void *dest);
void *Item0A_dtor(void *o, s32 flags);
s32 Item0A_LoadPicture(void *self, void *dest);
void *Item0B_dtor(void *o, s32 flags);
s32 Item0B_LoadPicture(void *self, void *dest);
void *Item0C_dtor(void *o, s32 flags);
s32 Item0C_LoadPicture(void *self, void *dest);
void *Item0D_dtor(void *o, s32 flags);
s32 Item0D_LoadPicture(void *self, void *dest);
void *Item0E_dtor(void *o, s32 flags);
s32 Item0E_LoadPicture(void *self, void *dest);
void *Item0F_dtor(void *o, s32 flags);
s32 Item0F_LoadPicture(void *self, void *dest);
void *Item10_dtor(void *o, s32 flags);
s32 Item10_LoadPicture(void *self, void *dest);
void *Item11_dtor(void *o, s32 flags);
s32 Item11_LoadPicture(void *self, void *dest);
void *Item14_dtor(void *o, s32 flags);
s32 Item14_LoadPicture(void *self, void *dest);
void *Item42_dtor(void *o, s32 flags);
u32 Item42_Actions(void);
void *Item43_dtor(void *o, s32 flags);
u32 Item43_Actions(void);
void *Item44_dtor(void *o, s32 flags);
u32 Item44_Actions(void);
void *Item45_dtor(void *o, s32 flags);
u32 Item45_Actions(void);
void *Item46_dtor(void *o, s32 flags);
u32 Item46_Actions(void);
void *Item47_dtor(void *o, s32 flags);
u32 Item47_Actions(void);
void *Item48_dtor(void *o, s32 flags);
u32 Item48_Actions(void);
void *Item49_dtor(void *o, s32 flags);
u32 Item49_Actions(void);
void *Item4A_dtor(void *o, s32 flags);
u32 Item4A_Actions(void);
void *Item4B_dtor(void *o, s32 flags);
u32 Item4B_Actions(void);
void *Item4C_dtor(void *o, s32 flags);
u32 Item4C_Actions(void);
void *Item60_dtor(void *o, s32 flags);
void *Item61_dtor(void *o, s32 flags);
void *Item62_dtor(void *o, s32 flags);
void *Item63_dtor(void *o, s32 flags);
u32 Item63_Actions(void);
void *Item64_dtor(void *o, s32 flags);
u32 Item64_Actions(void);
void *Item65_dtor(void *o, s32 flags);
u32 Item65_Actions(void);
void *Item66_dtor(void *o, s32 flags);
u32 Item66_Actions(void);
void *Item15_dtor(void *o, s32 flags);
s32 Item15_LoadPicture(void *self, void *dest);
void *Item16_dtor(void *o, s32 flags);
s32 Item16_LoadPicture(void *self, void *dest);
void *Item17_dtor(void *o, s32 flags);
s32 Item17_LoadPicture(void *self, void *dest);
void *ItemA2_dtor(void *o, s32 flags);
s32 ItemA2_LoadPicture(void *self, void *dest);
void *ItemA3_dtor(void *o, s32 flags);
s32 ItemA3_LoadPicture(void *self, void *dest);
void *Item18_dtor(void *o, s32 flags);
void *Item19_dtor(void *o, s32 flags);
void *Item1A_dtor(void *o, s32 flags);
void *Item1B_dtor(void *o, s32 flags);
void *Item1C_dtor(void *o, s32 flags);
void *Item1D_dtor(void *o, s32 flags);
void *Item1E_dtor(void *o, s32 flags);
s32 Item1E_LoadPicture(void *self, void *dest);
s32 Item1E_Use(void);
void *Item1F_dtor(void *o, s32 flags);
s32 Item1F_LoadPicture(void *self, void *dest);
s32 Item1F_Use(void);
void *Item20_dtor(void *o, s32 flags);
s32 Item20_LoadPicture(void *self, void *dest);
s32 Item20_Use(void);
void *Item21_dtor(void *o, s32 flags);
s32 Item21_LoadPicture(void *self, void *dest);
void *Item22_dtor(void *o, s32 flags);
s32 Item22_LoadPicture(void *self, void *dest);
void *Item23_dtor(void *o, s32 flags);
s32 Item23_LoadPicture(void *self, void *dest);
void *Item24_dtor(void *o, s32 flags);
s32 Item24_LoadPicture(void *self, void *dest);
void *Item25_dtor(void *o, s32 flags);
s32 Item25_LoadPicture(void *self, void *dest);
void *Item26_dtor(void *o, s32 flags);
s32 Item26_LoadPicture(void *self, void *dest);
void *Item27_dtor(void *o, s32 flags);
s32 Item27_LoadPicture(void *self, void *dest);
s32 Item27_Use(void);
void *Item28_dtor(void *o, s32 flags);
s32 Item28_LoadPicture(void *self, void *dest);
void *Item29_dtor(void *o, s32 flags);
s32 PoolEntry_Id(void *o);
s32 PoolEntry_Kind(void *o);
s32 PoolEntry_Actions(void *o);
s32 PoolEntry_IsCounted(void *o);
s32 PoolEntry_SortKey(void *o);
s32 PoolEntry_Get20(void *o);
void *PoolEntry_Data(void *o);
s32 PoolEntry_Use(void *o);
s32 PlainItem_Kind(void *o);
s32 PlainItem_Actions(void *o);
s32 PlainItem_IsCounted(void *o);
s32 Item3E_Use(void *o);
s32 ItemClassC920_Kind(void *o);
s32 ItemClassC920_Actions(void *o);
s32 ItemClassC920_IsCounted(void *o);
s32 Item70_SortKey(void *o);
s32 ItemClassCB50_Kind(void *o);
s32 ItemClassCB50_IsCounted(void *o);
s32 ItemClassCB50_Actions(void *o);
s32 ItemClassCB50_Use(void *o);
s32 Item71_SortKey(void *o);
s32 Item72_SortKey(void *o);
s32 Item73_SortKey(void *o);
s32 Item74_SortKey(void *o);
s32 Item75_SortKey(void *o);
s32 Item80_SortKey(void *o);
s32 ItemClassCCE0_Kind(void *o);
s32 ItemClassCCE0_Actions(void *o);
s32 ItemClassCCE0_IsCounted(void *o);
s32 Item81_SortKey(void *o);
s32 Item82_SortKey(void *o);
s32 Item83_SortKey(void *o);
s32 Item86_SortKey(void *o);
s32 ItemClassCE70_Kind(void *o);
s32 ItemClassCE70_Actions(void *o);
s32 ItemClassCE70_IsCounted(void *o);
s32 Item87_SortKey(void *o);
s32 Item88_SortKey(void *o);
s32 Item89_SortKey(void *o);
s32 Item8A_SortKey(void *o);
s32 ItemClassD000_Kind(void *o);
s32 ItemClassD000_Actions(void *o);
s32 ItemClassD000_IsCounted(void *o);
s32 Item8B_SortKey(void *o);
s32 Item8C_SortKey(void *o);
s32 Item8D_SortKey(void *o);
s32 Item90_SortKey(void *o);
s32 ItemClassD320_Kind(void *o);
s32 ItemClassD320_Actions(void *o);
s32 ItemClassD320_IsCounted(void *o);
s32 Item91_SortKey(void *o);
s32 Item92_SortKey(void *o);
s32 Item93_SortKey(void *o);
s32 Item94_SortKey(void *o);
s32 Item95_SortKey(void *o);
s32 Item97_SortKey(void *o);
s32 Item98_SortKey(void *o);
s32 Item9B_SortKey(void *o);
s32 ItemClassD640_Kind(void *o);
s32 ItemClassD640_Actions(void *o);
s32 ItemClassD640_IsCounted(void *o);
s32 ItemA5_Use(void *o);
s32 ItemA6_Use(void *o);
s32 ItemA7_Use(void *o);
s32 ItemA8_Use(void *o);
s32 ItemA9_Use(void *o);
s32 ItemAA_Use(void *o);
s32 ItemAB_Use(void *o);
s32 ItemAC_Use(void *o);
s32 Item40_SortKey(void *o);
s32 Item41_SortKey(void *o);
s32 Item01_IsCounted(void *o);
s32 Item01_LoadPicture(void *self, s32 a);
s32 Item01_Use(void);
s32 Item01_CountUp(void *o);
s32 ItemClassF430_Kind(void *o);
s32 ItemClassF430_Actions(void *o);
s32 ItemClassF430_IsCounted(void *o);
void *ItemClassF430_Get20(void *o);
s32 ItemA0_Use(void *o);
s32 Item42_SortKey(void *o);
s32 Item42_LoadPicture(void *self, void *dest);
s32 Item42_Use(void);
s32 Item43_SortKey(void *o);
s32 Item43_LoadPicture(void *self, void *dest);
s32 Item43_Use(void);
s32 Item44_SortKey(void *o);
s32 Item44_LoadPicture(void *self, void *dest);
s32 Item44_Use(void);
s32 Item45_SortKey(void *o);
s32 Item45_LoadPicture(void *self, void *dest);
s32 Item45_Use(void);
s32 Item46_SortKey(void *o);
s32 Item46_LoadPicture(void *self, void *dest);
s32 Item46_Use(void);
s32 Item47_SortKey(void *o);
s32 Item47_LoadPicture(void *self, void *dest);
s32 Item47_Use(void);
s32 Item48_SortKey(void *o);
s32 Item48_LoadPicture(void *self, void *dest);
s32 Item48_Use(void);
s32 Item49_SortKey(void *o);
s32 Item49_LoadPicture(void *self, void *dest);
s32 Item49_Use(void);
s32 Item4A_SortKey(void *o);
s32 Item4A_LoadPicture(void *self, void *dest);
s32 Item4A_Use(void);
s32 Item4B_SortKey(void *o);
s32 Item4B_LoadPicture(void *self, void *dest);
s32 Item4B_Use(void);
s32 Item4C_SortKey(void *o);
s32 Item4C_LoadPicture(void *self, void *dest);
s32 Item4C_Use(void);
s32 Item60_SortKey(void *o);
s32 ItemClass6B00_Kind(void *o);
s32 ItemClass6B00_Actions(void *o);
s32 ItemClass6B00_IsCounted(void *o);
s32 Item60_LoadPicture(void *self, void *dest);
s32 Item60_Use(void *o);
s32 Item61_SortKey(void *o);
s32 Item61_LoadPicture(void *self, void *dest);
s32 Item61_Use(void *o);
s32 Item62_SortKey(void *o);
s32 Item62_LoadPicture(void *self, void *dest);
s32 Item62_Use(void *o);
s32 Item63_SortKey(void *o);
s32 Item63_LoadPicture(void *self, void *dest);
s32 Item63_Use(void *o);
s32 Item64_SortKey(void *o);
s32 Item64_LoadPicture(void *self, void *dest);
s32 Item64_Use(void *o);
s32 Item65_SortKey(void *o);
s32 Item65_LoadPicture(void *self, void *dest);
s32 Item65_Use(void *o);
s32 Item66_SortKey(void *o);
s32 Item66_LoadPicture(void *self, void *dest);
s32 Item66_Use(void *o);
s32 ItemA2_Use(void *o);
s32 ItemClass6B00_PartnerCheck(void *o);
s32 ItemA3_Use(void *o);
s32 Item18_Actions(void *o);
s32 Item18_LoadPicture(void *self, void *dest);
s32 Item19_Actions(void *o);
s32 Item19_LoadPicture(void *self, void *dest);
s32 Item1A_Actions(void *o);
s32 Item1A_LoadPicture(void *self, void *dest);
s32 Item1B_Actions(void *o);
s32 Item1B_LoadPicture(void *self, void *dest);
s32 Item1C_Actions(void *o);
s32 Item1C_LoadPicture(void *self, void *dest);
s32 Item1D_Actions(void *o);
s32 Item1D_LoadPicture(void *self, void *dest);
s32 Item24_Use(void *o);
s32 Item29_Use(void *o);
s32 PoolEntry_LoadPicture(void *o, s32 dst);
s32 Item90_LoadPicture(void *o, s32 dst);
s32 Item91_LoadPicture(void *o, s32 dst);
s32 Item92_LoadPicture(void *o, s32 dst);
s32 Item93_LoadPicture(void *o, s32 dst);
s32 Item94_LoadPicture(void *o, s32 dst);
s32 Item95_LoadPicture(void *o, s32 dst);
s32 Item97_LoadPicture(void *o, s32 dst);
s32 Item98_LoadPicture(void *o, s32 dst);
s32 Item9B_LoadPicture(void *o, s32 dst);
s32 ItemA4_LoadPicture(void *o, s32 dst);
s32 ItemA5_LoadPicture(void *o, s32 dst);
s32 ItemA6_LoadPicture(void *o, s32 dst);
s32 ItemA7_LoadPicture(void *o, s32 dst);
s32 ItemA8_LoadPicture(void *o, s32 dst);
s32 ItemA9_LoadPicture(void *o, s32 dst);
s32 ItemAA_LoadPicture(void *o, s32 dst);
s32 ItemAB_LoadPicture(void *o, s32 dst);
s32 ItemAC_LoadPicture(void *o, s32 dst);
s32 Item40_LoadPicture(void *o, s32 dst);
s32 Item41_LoadPicture(void *o, s32 dst);
s32 Item06_LoadPicture(void *o, s32 dst);
s32 Item07_LoadPicture(void *o, s32 dst);
s32 Item29_LoadPicture(void *o, s32 dst);
s32 PoolEntry_AddStack(void *o, s32 n);
s32 PoolEntry_TakeOne(void *o);
s32 PoolEntry_Count(void *o);
s32 PoolEntry_CountUp(void *o);
s32 PoolEntry_PartnerCheck(void *o);
s32 Item00_Use(void *o);
s32 Item18_Use(void *o);
s32 Item19_Use(void *o);
s32 Item1A_Use(void *o);
s32 Item1B_Use(void *o);
s32 Item1C_Use(void *o);
s32 Item23_Use(void *o);
s32 Item40_Use(void *o);
s32 Item41_Use(void *o);
s32 Item0F_Use(void *o);
s32 Item11_Use(void *o);
s32 Item14_Use(void *o);

void *PoolEntry_dtor(void *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x0, void **) = PoolEntry_vtable;
        if ((s16)flags > 0) {
            SubPool_delete(e);
        }
    }
    return e;
}
/* ---- the small methods (the base pool entry's and the items' own) ----
 * +0xC its id (+0x4), +0x10 / +0x14 / +0x20 class constants, +0x18 how it is held (1 a stack
 * counted at +0x10, 2 a counter), +0x1C its name / picture cell, +0x24 its data (+0x10) */

/* +0xC (PoolEntry_vtable, Item12_vtable, Item13_vtable, ...) */
/* 0x0025FC90 */
s32 PoolEntry_Id(void *o) {
    return AT(o, 0x4, s32);
}

/* +0x10 (PoolEntry_vtable) */
/* 0x0025FCA0 */
s32 PoolEntry_Kind(void *o) {
    return -1;
}

/* +0x14 (PoolEntry_vtable) */
/* 0x0025FCB0 */
s32 PoolEntry_Actions(void *o) {
    return 0;
}

/* +0x18 (PoolEntry_vtable) */
/* 0x0025FCC0 */
s32 PoolEntry_IsCounted(void *o) {
    return 0;
}

/* +0x1C (PoolEntry_vtable, Item12_vtable, Item13_vtable, ...) */
/* 0x0025FCD0 */
s32 PoolEntry_SortKey(void *o) {
    return 0;
}

/* +0x20 (PoolEntry_vtable, Item12_vtable, Item13_vtable, ...) */
/* 0x0025FCE0 */
s32 PoolEntry_Get20(void *o) {
    return 0;
}

/* +0x24 (PoolEntry_vtable, Item12_vtable, Item13_vtable, ...) */
/* 0x0025FCF0 */
void *PoolEntry_Data(void *o) {
    return (u8 *)o + 0x10;
}

/* +0x2C add `n` to a stack (+0x10, at most 98 more than the first): 1 if it took them */
/* 0x0025FD00 */
s32 PoolEntry_AddStack(void *o, s32 n) {
    if (AT(o, 0x4, s32) != -1 && ITEM_HELD(o) == 1 && AT(o, 0x10, u32) < 98) {
        AT(o, 0x10, u32) += n & 0xFF;
        if (AT(o, 0x10, u32) >= 99) {
            AT(o, 0x10, u32) = 98;
        }
        return 1;
    }
    return 0;
}

/* +0x30 take one off a stack: 1 if there was more than one */
/* 0x0025FDA0 */
s32 PoolEntry_TakeOne(void *o) {
    if (AT(o, 0x4, s32) != -1 && ITEM_HELD(o) == 1 && AT(o, 0x10, s32) != 0) {
        AT(o, 0x10, s32) -= 1;
        return 1;
    }
    return 0;
}

/* +0x34 how many there are (0: none) */
/* 0x0025FE10 */
s32 PoolEntry_Count(void *o) {
    if (AT(o, 0x4, s32) == -1) {
        return 0;
    }
    return ITEM_HELD(o) == 1 ? AT(o, 0x10, s32) + 1 : 1;
}

/* +0x38 a counter (held 2) counts one more, unless it is -1 */
/* 0x0025FE70 */
s32 PoolEntry_CountUp(void *o) {
    if (AT(o, 0x4, s32) != -1 && ITEM_HELD(o) == 2 && AT(o, 0x10, s32) != -1) {
        AT(o, 0x10, s32) += 1;
    }
    return 0;
}

/* +0x3C (PoolEntry_vtable, PlainItem_vtable, ItemClassC920_vtable, ...) */
/* 0x0025FEE0 */
s32 PoolEntry_Use(void *o) {
    return 0;
}

/* +0x40 the partner's Hewie_CanTakeCommand (0 without one) */
/* 0x0025FF10 */
s32 PoolEntry_PartnerCheck(void *o) {
    if (gCharPartner != NULL) {
        return Hewie_CanTakeCommand((Hewie *)gCharPartner);
    }
    return 0;
}

/* "ITEM00\ITEM_FFF.TEX" */
/* 0x0025FF50 */
s32 PoolEntry_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A7C0, dst);
}
/* +0x10: the item in equipment slot `slot` (+0x15E0[slot], its +0xC; -1: empty slot) */
/* 0x00260690 */
s32 Items_Equipped(u8 *items, u8 slot) {
    VObject *e = AT(items, 0x15E0 + slot * 4, VObject *);

    if (e == NULL) {
        return -1;
    }
    return VCALL(e, 0xC, s32 (*)(VObject *))(e);
}

extern void Items_Remove(u8 *o, VObject *it);   /* remove an entry */

/* entry `i` of list `l`: one tick (+0x30) while it is set (+0x18) and has 2 or more left
   (+0x34), otherwise it is removed */
static void item_use(u8 *o, u32 l, u32 i) {
    VObject *it = AT(o, 0x12E0 + (l & 0xFF) * 0x100 + (i & 0xFF) * 4, VObject *);

    if (it == NULL) {
        return;
    }
    if ((u8)VCALL(it, 0x18, s32 (*)(VObject *))(it) == 1 && !(VCALL(it, 0x34, u32 (*)(VObject *))(it) < 2)) {
        VCALL(it, 0x30, void (*)(VObject *))(it);
    } else {
        Items_Remove(o, it);
    }
}

/* each frame: the 3 lists of up to 64 entries (+0x12E0, 0x100 apart, ending at the first
 * empty slot): each one in use (+0x38 bits 0..1) is used (item_use) */
/* 0x00260EC0 */
void Items_Update(u8 *o) {
    u32 l, i;

    for (l = 0; l < 3; l++) {
        for (i = 0; i < 64; i++) {
            VObject *it = AT(o, 0x12E0 + l * 0x100 + i * 4, VObject *);

            if (it == NULL) {
                break;
            }
            if (!((u8)VCALL(it, 0x38, s32 (*)(VObject *))(it) & 3)) {
                continue;
            }
            item_use(o, l, i);
        }
    }
}

/* an item `id` (one, Items_Give) with its 8 bytes at +0x10 */
/* 0x00261000 */
void *Items_GiveWithData(u8 *items, u32 id, u64 *data) {
    u8 *it = Items_Give(items, id, 1);

    if (it != NULL) {
        AT(it, 0x10, u64) = *data;
    }
    return it;
}

/* use item `id` (+0xC) wherever it is in the lists: 1 if it was there */
/* 0x00260BB0 */
s32 Items_UseId(u8 *o, s32 id) {
    u32 l, i;

    for (l = 0; l < 3; l++) {
        for (i = 0; i < 64; i++) {
            VObject *it = AT(o, 0x12E0 + l * 0x100 + i * 4, VObject *);

            if (it == NULL) {
                break;
            }
            if (id == VCALL(it, 0xC, s32 (*)(VObject *))(it)) {
                item_use(o, l, i);
                return 1;
            }
        }
    }
    return 0;
}

/* how many of item `id` (+0x34) the lists hold: 0 if none */
/* 0x00260CF0 */
u32 Items_Count(u8 *o, s32 id) {
    u32 l, i;

    for (l = 0; l < 3; l++) {
        for (i = 0; i < 64; i++) {
            VObject *it = AT(o, 0x12E0 + l * 0x100 + i * 4, VObject *);

            if (it == NULL) {
                break;
            }
            if (id == VCALL(it, 0xC, s32 (*)(VObject *))(it)) {
                it = AT(o, 0x12E0 + l * 0x100 + i * 4, VObject *);
                return (u8)VCALL(it, 0x34, s32 (*)(VObject *))(it);
            }
        }
    }
    return 0;
}

/* ---- giving an item (Items_Give): the inventory's three groups (ids under 0x40, under 0xA0,
 * the rest) of 64 slots each (+0x12E0 + group * 0x100); objects 0x18 bytes from its pool
 * (+0x1208) ---- */

typedef void *(*ItemCtor)(void *self);

static ItemCtor const kItemCtors[0xAD] = {
    [0x0] = (void * (*)(void *))Item00_ctor,
    [0x1] = (void * (*)(void *))Item01_ctor,
    [0x2] = (void * (*)(void *))Item02_ctor,
    [0x3] = (void * (*)(void *))Item03_ctor,
    [0x4] = (void * (*)(void *))Item04_ctor,
    [0x5] = (void * (*)(void *))Item05_ctor,
    [0x6] = (void * (*)(void *))Item06_ctor,
    [0x7] = (void * (*)(void *))Item07_ctor,
    [0x8] = (void * (*)(void *))Item08_ctor,
    [0x9] = (void * (*)(void *))Item09_ctor,
    [0xA] = (void * (*)(void *))Item0A_ctor,
    [0xB] = (void * (*)(void *))Item0B_ctor,
    [0xC] = (void * (*)(void *))Item0C_ctor,
    [0xD] = (void * (*)(void *))Item0D_ctor,
    [0xE] = (void * (*)(void *))Item0E_ctor,
    [0xF] = (void * (*)(void *))Item0F_ctor,
    [0x10] = (void * (*)(void *))Item10_ctor,
    [0x11] = (void * (*)(void *))Item11_ctor,
    [0x12] = (void * (*)(void *))Item12_ctor,
    [0x13] = (void * (*)(void *))Item13_ctor,
    [0x14] = (void * (*)(void *))Item14_ctor,
    [0x15] = (void * (*)(void *))Item15_ctor,
    [0x16] = (void * (*)(void *))Item16_ctor,
    [0x17] = (void * (*)(void *))Item17_ctor,
    [0x18] = (void * (*)(void *))Item18_ctor,
    [0x19] = (void * (*)(void *))Item19_ctor,
    [0x1A] = (void * (*)(void *))Item1A_ctor,
    [0x1B] = (void * (*)(void *))Item1B_ctor,
    [0x1C] = (void * (*)(void *))Item1C_ctor,
    [0x1D] = (void * (*)(void *))Item1D_ctor,
    [0x1E] = (void * (*)(void *))Item1E_ctor,
    [0x1F] = (void * (*)(void *))Item1F_ctor,
    [0x20] = (void * (*)(void *))Item20_ctor,
    [0x21] = (void * (*)(void *))Item21_ctor,
    [0x22] = (void * (*)(void *))Item22_ctor,
    [0x23] = (void * (*)(void *))Item23_ctor,
    [0x24] = (void * (*)(void *))Item24_ctor,
    [0x25] = (void * (*)(void *))Item25_ctor,
    [0x26] = (void * (*)(void *))Item26_ctor,
    [0x27] = (void * (*)(void *))Item27_ctor,
    [0x28] = (void * (*)(void *))Item28_ctor,
    [0x29] = (void * (*)(void *))Item29_ctor,
    [0x3E] = (void * (*)(void *))Item3E_ctor,
    [0x40] = (void * (*)(void *))Item40_ctor,
    [0x41] = (void * (*)(void *))Item41_ctor,
    [0x42] = (void * (*)(void *))Item42_ctor,
    [0x43] = (void * (*)(void *))Item43_ctor,
    [0x44] = (void * (*)(void *))Item44_ctor,
    [0x45] = (void * (*)(void *))Item45_ctor,
    [0x46] = (void * (*)(void *))Item46_ctor,
    [0x47] = (void * (*)(void *))Item47_ctor,
    [0x48] = (void * (*)(void *))Item48_ctor,
    [0x49] = (void * (*)(void *))Item49_ctor,
    [0x4A] = (void * (*)(void *))Item4A_ctor,
    [0x4B] = (void * (*)(void *))Item4B_ctor,
    [0x4C] = (void * (*)(void *))Item4C_ctor,
    [0x60] = (void * (*)(void *))Item60_ctor,
    [0x61] = (void * (*)(void *))Item61_ctor,
    [0x62] = (void * (*)(void *))Item62_ctor,
    [0x63] = (void * (*)(void *))Item63_ctor,
    [0x64] = (void * (*)(void *))Item64_ctor,
    [0x65] = (void * (*)(void *))Item65_ctor,
    [0x66] = (void * (*)(void *))Item66_ctor,
    [0x70] = (void * (*)(void *))Item70_ctor,
    [0x71] = (void * (*)(void *))Item71_ctor,
    [0x72] = (void * (*)(void *))Item72_ctor,
    [0x73] = (void * (*)(void *))Item73_ctor,
    [0x74] = (void * (*)(void *))Item74_ctor,
    [0x75] = (void * (*)(void *))Item75_ctor,
    [0x80] = (void * (*)(void *))Item80_ctor,
    [0x81] = (void * (*)(void *))Item81_ctor,
    [0x82] = (void * (*)(void *))Item82_ctor,
    [0x83] = (void * (*)(void *))Item83_ctor,
    [0x86] = (void * (*)(void *))Item86_ctor,
    [0x87] = (void * (*)(void *))Item87_ctor,
    [0x88] = (void * (*)(void *))Item88_ctor,
    [0x89] = (void * (*)(void *))Item89_ctor,
    [0x8A] = (void * (*)(void *))Item8A_ctor,
    [0x8B] = (void * (*)(void *))Item8B_ctor,
    [0x8C] = (void * (*)(void *))Item8C_ctor,
    [0x8D] = (void * (*)(void *))Item8D_ctor,
    [0x90] = (void * (*)(void *))Item90_ctor,
    [0x91] = (void * (*)(void *))Item91_ctor,
    [0x92] = (void * (*)(void *))Item92_ctor,
    [0x93] = (void * (*)(void *))Item93_ctor,
    [0x94] = (void * (*)(void *))Item94_ctor,
    [0x95] = (void * (*)(void *))Item95_ctor,
    [0x97] = (void * (*)(void *))Item97_ctor,
    [0x98] = (void * (*)(void *))Item98_ctor,
    [0x9B] = (void * (*)(void *))Item9B_ctor,
    [0xA0] = (void * (*)(void *))ItemA0_ctor,
    [0xA1] = (void * (*)(void *))ItemA1_ctor,
    [0xA2] = (void * (*)(void *))ItemA2_ctor,
    [0xA3] = (void * (*)(void *))ItemA3_ctor,
    [0xA4] = (void * (*)(void *))ItemA4_ctor,
    [0xA5] = (void * (*)(void *))ItemA5_ctor,
    [0xA6] = (void * (*)(void *))ItemA6_ctor,
    [0xA7] = (void * (*)(void *))ItemA7_ctor,
    [0xA8] = (void * (*)(void *))ItemA8_ctor,
    [0xA9] = (void * (*)(void *))ItemA9_ctor,
    [0xAA] = (void * (*)(void *))ItemAA_ctor,
    [0xAB] = (void * (*)(void *))ItemAB_ctor,
    [0xAC] = (void *(*)(void *))ItemAC_ctor,
};

/* give n of item id: added to one already held if that kind stacks (+0x18), else made in the
 * group's first free slot with its count set (+0x2C, n - 1). The item, or NULL if it wouldn't
 * take them or the group is full */
/* 0x00261090 */
void *Items_Give(u8 *items, u32 id, s32 n) {
    u8 *grp = items + (id < 0x40 ? 0 : id < 0xA0 ? 1 : 2) * 0x100;
    void *o;
    u32 i;

    for (i = 0; i < 0x40; i++) {
        o = AT(grp, 0x12E0 + i * 4, void *);
        if (o == NULL) {
            break;
        }
        if (id == VCALL(o, 0xC, u32 (*)(void *))(o) && (VCALL(o, 0x18, u32 (*)(void *))(o) & 0xFF) == 1) {
            return VCALL(o, 0x2C, s32 (*)(void *, s32))(o, n) != 0 ? o : NULL;
        }
    }
    if (i == 0x40) {
        return NULL;
    }
    o = NULL;
    if (id < 0xAD && (kItemCtors[id] != NULL || id == 0x3F)) {
        void *pool = items + 0x1208;

        o = VCALL(pool, 0x10, void *(*)(void *, u32))(pool, 0x18);
        if (o != NULL) {
            void *p = SubPool_new(0x18, o);

            if (p != NULL) {
                if (id == 0x3F) {
                    Item3F_ctor(p, (s32)o);   /* (sic: no id passed - a1 still the object) */
                } else {
                    kItemCtors[id](p);
                }
            }
        }
    }
    AT(grp, 0x12E0 + i * 4, void *) = o;
#ifdef HG_NATIVE
    if (o == NULL) {   /* an id with no class (the PS2 calls through NULL) */
        return NULL;
    }
#endif
    VCALL(o, 0x2C, s32 (*)(void *, s32))(o, (u8)((n & 0xFF) - 1));
    return o;
}

/* 0x00263220 */
ItemObj *ItemAC_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xAC;
    self->flag = 0;
    self->data = 0;
    self->vtbl = ItemAC_vtable;
    return self;
}

/* 0x00263250 */
void *ItemClassD640_dtor(void *o, s32 flags) { return item_dtor2(o, flags, ItemClassD640_vtable); }

/* 0x002632B0 */
ItemObj *ItemAB_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xAB;
    self->flag = 0;
    self->data = 0;
    self->vtbl = ItemAB_vtable;
    return self;
}

/* 0x002632E0 */
ItemObj *ItemAA_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xAA;
    self->flag = 0;
    self->data = 0;
    self->vtbl = ItemAA_vtable;
    return self;
}

/* 0x00263310 */
ItemObj *ItemA9_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xA9;
    self->flag = 0;
    self->data = 0;
    self->vtbl = ItemA9_vtable;
    return self;
}

/* 0x00263340 */
ItemObj *ItemA8_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xA8;
    self->flag = 0;
    self->data = 0;
    self->vtbl = ItemA8_vtable;
    return self;
}

/* 0x00263370 */
ItemObj *ItemA7_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xA7;
    self->flag = 0;
    self->data = 0;
    self->vtbl = ItemA7_vtable;
    return self;
}

/* 0x002633A0 */
ItemObj *ItemA6_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xA6;
    self->flag = 0;
    self->data = 0;
    self->vtbl = ItemA6_vtable;
    return self;
}

/* 0x002633D0 */
ItemObj *ItemA5_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xA5;
    self->flag = 0;
    self->data = 0;
    self->vtbl = ItemA5_vtable;
    return self;
}

/* 0x00263400 */
ItemObj *ItemA4_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xA4;
    self->flag = 0;
    self->data = 0;
    self->vtbl = ItemA4_vtable;
    return self;
}

/* 0x00263430 */
ItemObj *ItemA3_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xA3;
    self->flag = 0;
    self->data = 0;
    self->vtbl = ItemA3_vtable;
    return self;
}

/* 0x00263460 */
ItemObj *ItemA2_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xA2;
    self->flag = 0;
    self->data = 0;
    self->vtbl = ItemA2_vtable;
    return self;
}

/* 0x00263490 */
ItemObj *ItemA1_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xA1;
    self->flag = 0;
    self->data = 0;
    self->vtbl = ItemA1_vtable;
    return self;
}

/* 0x002634C0 */
ItemObj *ItemA0_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xA0;
    self->flag = 0;
    self->data = 0;
    self->vtbl = ItemA0_vtable;
    return self;
}

/* 0x002634F0 */
ItemObj *Item9B_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x9B;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item9B_vtable;
    return self;
}

/* 0x00263520 */
void *ItemClassD320_dtor(void *o, s32 flags) { return item_dtor2(o, flags, ItemClassD320_vtable); }

/* 0x00263580 */
ItemObj *Item98_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x98;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item98_vtable;
    return self;
}

/* 0x002635B0 */
ItemObj *Item97_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x97;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item97_vtable;
    return self;
}

/* 0x002635E0 */
ItemObj *Item95_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x95;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item95_vtable;
    return self;
}

/* 0x00263610 */
ItemObj *Item94_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x94;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item94_vtable;
    return self;
}

/* 0x00263640 */
ItemObj *Item93_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x93;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item93_vtable;
    return self;
}

/* 0x00263670 */
ItemObj *Item92_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x92;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item92_vtable;
    return self;
}

/* 0x002636A0 */
ItemObj *Item91_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x91;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item91_vtable;
    return self;
}

/* 0x002636D0 */
ItemObj *Item90_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x90;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item90_vtable;
    return self;
}

/* 0x00263700 */
ItemObj *Item8D_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x8D;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item8D_vtable;
    return self;
}

/* 0x00263730 */
void *ItemClassD000_dtor(void *o, s32 flags) { return item_dtor2(o, flags, ItemClassD000_vtable); }

/* 0x00263790 */
ItemObj *Item8C_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x8C;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item8C_vtable;
    return self;
}

/* 0x002637C0 */
ItemObj *Item8B_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x8B;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item8B_vtable;
    return self;
}

/* 0x002637F0 */
ItemObj *Item8A_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x8A;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item8A_vtable;
    return self;
}

/* 0x00263820 */
ItemObj *Item89_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x89;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item89_vtable;
    return self;
}

/* 0x00263850 */
void *ItemClassCE70_dtor(void *o, s32 flags) { return item_dtor2(o, flags, ItemClassCE70_vtable); }

/* 0x002638B0 */
ItemObj *Item88_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x88;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item88_vtable;
    return self;
}

/* 0x002638E0 */
ItemObj *Item87_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x87;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item87_vtable;
    return self;
}

/* 0x00263910 */
ItemObj *Item86_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x86;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item86_vtable;
    return self;
}

/* 0x00263940 */
ItemObj *Item83_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x83;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item83_vtable;
    return self;
}

/* 0x00263970 */
void *ItemClassCCE0_dtor(void *o, s32 flags) { return item_dtor2(o, flags, ItemClassCCE0_vtable); }

/* 0x002639D0 */
ItemObj *Item82_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x82;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item82_vtable;
    return self;
}

/* 0x00263A00 */
ItemObj *Item81_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x81;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item81_vtable;
    return self;
}

/* 0x00263A30 */
ItemObj *Item80_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x80;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item80_vtable;
    return self;
}

/* 0x00263A60 */
ItemObj *Item75_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x75;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item75_vtable;
    return self;
}

/* 0x00263A90 */
void *ItemClassCB50_dtor(void *o, s32 flags) { return item_dtor2(o, flags, ItemClassCB50_vtable); }

/* 0x00263AF0 */
ItemObj *Item74_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x74;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item74_vtable;
    return self;
}

/* 0x00263B20 */
ItemObj *Item73_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x73;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item73_vtable;
    return self;
}

/* 0x00263B50 */
ItemObj *Item72_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x72;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item72_vtable;
    return self;
}

/* 0x00263B80 */
ItemObj *Item71_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x71;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item71_vtable;
    return self;
}

/* 0x00263BB0 */
ItemObj *Item70_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x70;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item70_vtable;
    return self;
}

/* 0x00263BE0 */
ItemObj *Item66_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x66;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item66_vtable;
    return self;
}

/* 0x00263C10 */
void *ItemClass6B00_dtor(void *o, s32 flags) { return item_dtor2(o, flags, ItemClass6B00_vtable); }

/* 0x00263C70 */
ItemObj *Item65_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x65;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item65_vtable;
    return self;
}

/* 0x00263CA0 */
ItemObj *Item64_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x64;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item64_vtable;
    return self;
}

/* 0x00263CD0 */
ItemObj *Item63_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x63;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item63_vtable;
    return self;
}

/* 0x00263D00 */
ItemObj *Item62_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x62;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item62_vtable;
    return self;
}

/* 0x00263D30 */
ItemObj *Item61_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x61;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item61_vtable;
    return self;
}

/* 0x00263D60 */
ItemObj *Item60_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x60;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item60_vtable;
    return self;
}

/* 0x00263D90 */
ItemObj *Item4C_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x4C;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item4C_vtable;
    return self;
}

/* 0x00263DC0 */
void *ItemClassC920_dtor(void *o, s32 flags) { return item_dtor2(o, flags, ItemClassC920_vtable); }

/* 0x00263E20 */
ItemObj *Item4B_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x4B;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item4B_vtable;
    return self;
}

/* 0x00263E50 */
ItemObj *Item4A_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x4A;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item4A_vtable;
    return self;
}

/* 0x00263E80 */
ItemObj *Item49_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x49;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item49_vtable;
    return self;
}

/* 0x00263EB0 */
ItemObj *Item48_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x48;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item48_vtable;
    return self;
}

/* 0x00263EE0 */
ItemObj *Item47_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x47;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item47_vtable;
    return self;
}

/* 0x00263F10 */
ItemObj *Item46_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x46;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item46_vtable;
    return self;
}

/* 0x00263F40 */
ItemObj *Item45_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x45;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item45_vtable;
    return self;
}

/* 0x00263F70 */
ItemObj *Item44_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x44;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item44_vtable;
    return self;
}

/* 0x00263FA0 */
ItemObj *Item43_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x43;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item43_vtable;
    return self;
}

/* 0x00263FD0 */
ItemObj *Item42_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x42;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item42_vtable;
    return self;
}

/* 0x00264000 */
ItemObj *Item41_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x41;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item41_vtable;
    return self;
}

/* 0x00264030 */
ItemObj *Item40_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x40;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item40_vtable;
    return self;
}

/* item 0x3F's class takes its id as an argument; Items_Give calls it without one, so the id
 * is whatever a1 held: the object's own address (see there) */
/* 0x00264060 */
ItemObj *Item3F_ctor(ItemObj *self, s32 id) {
    self->vtbl = PoolEntry_vtable;
    self->id = id;
    self->flag = 0;
    self->data = 0;
    self->vtbl = ItemClassF430_vtable;
    return self;
}

/* 0x00264090 */
ItemObj *Item3E_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x3E;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item3E_vtable;
    return self;
}

/* the middle classes */

/* 0x002640C0 */
void *PlainItem_dtor(void *o, s32 flags) { return item_dtor2(o, flags, PlainItem_vtable); }

/* 0x00264120 */
ItemObj *Item29_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x29;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item29_vtable;
    return self;
}

/* 0x00264150 */
ItemObj *Item28_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x28;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item28_vtable;
    return self;
}

/* 0x00264180 */
ItemObj *Item27_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x27;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item27_vtable;
    return self;
}

/* 0x002641B0 */
ItemObj *Item26_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x26;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item26_vtable;
    return self;
}

/* 0x002641E0 */
ItemObj *Item25_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x25;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item25_vtable;
    return self;
}

/* 0x00264210 */
ItemObj *Item24_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x24;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item24_vtable;
    return self;
}

/* 0x00264240 */
ItemObj *Item23_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x23;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item23_vtable;
    return self;
}

/* 0x00264270 */
ItemObj *Item22_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x22;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item22_vtable;
    return self;
}

/* 0x002642A0 */
ItemObj *Item21_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x21;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item21_vtable;
    return self;
}

/* 0x002642D0 */
ItemObj *Item20_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x20;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item20_vtable;
    return self;
}

/* 0x00264300 */
ItemObj *Item1F_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x1F;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item1F_vtable;
    return self;
}

/* 0x00264330 */
ItemObj *Item1E_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x1E;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item1E_vtable;
    return self;
}

/* 0x00264360 */
ItemObj *Item1D_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x1D;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item1D_vtable;
    return self;
}

/* 0x00264390 */
ItemObj *Item1C_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x1C;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item1C_vtable;
    return self;
}

/* 0x002643C0 */
ItemObj *Item1B_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x1B;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item1B_vtable;
    return self;
}

/* 0x002643F0 */
ItemObj *Item1A_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x1A;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item1A_vtable;
    return self;
}

/* 0x00264420 */
ItemObj *Item19_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x19;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item19_vtable;
    return self;
}

/* 0x00264450 */
ItemObj *Item18_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x18;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item18_vtable;
    return self;
}

/* 0x00264480 */
ItemObj *Item17_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x17;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item17_vtable;
    return self;
}

/* 0x002644B0 */
ItemObj *Item16_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x16;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item16_vtable;
    return self;
}

/* 0x002644E0 */
ItemObj *Item15_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x15;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item15_vtable;
    return self;
}

/* 0x00264510 */
ItemObj *Item14_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x14;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item14_vtable;
    return self;
}

/* 0x00264540 */
ItemObj *Item13_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x13;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item13_vtable;
    return self;
}

/* 0x00264570 */
ItemObj *Item12_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x12;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item12_vtable;
    return self;
}

/* 0x002645A0 */
ItemObj *Item11_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x11;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item11_vtable;
    return self;
}

/* 0x002645D0 */
B2_Obj *Item10_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x10;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item10_vtable;
    return self;
}

/* 0x00264600 */
B2_Obj *Item0F_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xF;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item0F_vtable;
    return self;
}

/* 0x00264630 */
B2_Obj *Item0E_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xE;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item0E_vtable;
    return self;
}

/* 0x00264660 */
B2_Obj *Item0D_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xD;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item0D_vtable;
    return self;
}

/* 0x00264690 */
B2_Obj *Item0C_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xC;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item0C_vtable;
    return self;
}

/* 0x002646C0 */
B2_Obj *Item0B_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xB;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item0B_vtable;
    return self;
}

/* 0x002646F0 */
B2_Obj *Item0A_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0xA;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item0A_vtable;
    return self;
}

/* 0x00264720 */
B2_Obj *Item09_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x9;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item09_vtable;
    return self;
}

/* 0x00264750 */
B2_Obj *Item08_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x8;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item08_vtable;
    return self;
}

/* 0x00264780 */
B2_Obj *Item07_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x7;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item07_vtable;
    return self;
}

/* 0x002647B0 */
B2_Obj *Item06_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x6;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item06_vtable;
    return self;
}

/* 0x002647E0 */
B2_Obj *Item05_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x5;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item05_vtable;
    return self;
}

/* 0x00264810 */
B2_Obj *Item04_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x4;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item04_vtable;
    return self;
}

/* 0x00264840 */
B2_Obj *Item03_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x3;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item03_vtable;
    return self;
}

/* 0x00264870 */
B2_Obj *Item02_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x2;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item02_vtable;
    return self;
}

/* 0x002648A0 */
B2_Obj *Item01_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x1;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item01_vtable;
    return self;
}

/* 0x002648D0 */
B2_Obj *Item00_ctor(B2_Obj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item00_vtable;
    return self;
}

/* the items */
/* 0x002649F0 */
void *Item12_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item12_vtable, PlainItem_vtable); }

/* +0x10 (Item12_vtable, Item13_vtable, Item3E_vtable, ...) */
/* 0x00264C40 */
s32 PlainItem_Kind(void *o) {
    return 0;
}

/* +0x14 (Item12_vtable, Item13_vtable, Item3E_vtable, ...) */
/* 0x00264C50 */
s32 PlainItem_Actions(void *o) {
    return 0x1;
}

/* +0x18 (Item12_vtable, Item13_vtable, Item3E_vtable, ...) */
/* 0x00264C60 */
s32 PlainItem_IsCounted(void *o) {
    return 0;
}

/* 0x00264C70 */
void *Item13_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item13_vtable, PlainItem_vtable); }

/* 0x00264EC0 */
void *Item3E_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item3E_vtable, PlainItem_vtable); }

/* +0x3C (Item3E_vtable) */
/* 0x00264F30 */
s32 Item3E_Use(void *o) {
    return 0;
}

/* +0x10 (ItemClassC920_vtable, Item40_vtable, Item41_vtable, ...) */
/* 0x00264F40 */
s32 ItemClassC920_Kind(void *o) {
    return 0x2;
}

/* +0x14 (ItemClassC920_vtable, Item40_vtable, Item41_vtable) */
/* 0x00264F50 */
s32 ItemClassC920_Actions(void *o) {
    return 0x5;
}

/* +0x18 (ItemClassC920_vtable, Item40_vtable, Item41_vtable, ...) */
/* 0x00264F60 */
s32 ItemClassC920_IsCounted(void *o) {
    return 0x1;
}

/* 0x00264F70 */
void *Item70_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item70_vtable, ItemClassCB50_vtable); }

/* 0x00264FE0 */
s32 Item70_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_01D_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item70_vtable) */
/* 0x00265010 */
s32 Item70_SortKey(void *o) {
    return 0xA10;
}

/* +0x10 (Item70_vtable, Item71_vtable, Item72_vtable, ...) */
/* 0x00265020 */
s32 ItemClassCB50_Kind(void *o) {
    return 0x8;
}

/* +0x18 (Item70_vtable, Item71_vtable, Item72_vtable, ...) */
/* 0x00265030 */
s32 ItemClassCB50_IsCounted(void *o) {
    return 0x1;
}

/* +0x14 (Item70_vtable, Item71_vtable, Item72_vtable, ...) */
/* 0x00265040 */
s32 ItemClassCB50_Actions(void *o) {
    return 0x5;
}

/* +0x3C (Item70_vtable, Item71_vtable, Item72_vtable, ...) */
/* 0x00265050 */
s32 ItemClassCB50_Use(void *o) {
    return 0;
}

/* 0x00265060 */
void *Item71_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item71_vtable, ItemClassCB50_vtable); }

/* 0x002650D0 */
s32 Item71_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_01E_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item71_vtable) */
/* 0x00265100 */
s32 Item71_SortKey(void *o) {
    return 0xA20;
}

/* 0x00265110 */
void *Item72_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item72_vtable, ItemClassCB50_vtable); }

/* 0x00265180 */
s32 Item72_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_01F_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item72_vtable) */
/* 0x002651B0 */
s32 Item72_SortKey(void *o) {
    return 0xA30;
}

/* 0x002651C0 */
void *Item73_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item73_vtable, ItemClassCB50_vtable); }

/* 0x00265230 */
s32 Item73_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_020_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item73_vtable) */
/* 0x00265260 */
s32 Item73_SortKey(void *o) {
    return 0xA40;
}

/* 0x00265270 */
void *Item74_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item74_vtable, ItemClassCB50_vtable); }

/* 0x002652E0 */
s32 Item74_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_021_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item74_vtable) */
/* 0x00265310 */
s32 Item74_SortKey(void *o) {
    return 0xA50;
}

/* 0x00265320 */
void *Item75_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item75_vtable, ItemClassCB50_vtable); }

/* 0x00265390 */
s32 Item75_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_022_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item75_vtable) */
/* 0x002653C0 */
s32 Item75_SortKey(void *o) {
    return 0xA60;
}

/* 0x002653D0 */
void *Item80_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item80_vtable, ItemClassCCE0_vtable); }

/* 0x00265440 */
s32 Item80_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_056_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item80_vtable) */
/* 0x00265470 */
s32 Item80_SortKey(void *o) {
    return 0x310;
}

/* +0x10 (Item80_vtable, Item81_vtable, Item82_vtable, ...) */
/* 0x00265480 */
s32 ItemClassCCE0_Kind(void *o) {
    return 0x4;
}

/* +0x14 (Item80_vtable, Item81_vtable, Item82_vtable, ...) */
/* 0x00265490 */
s32 ItemClassCCE0_Actions(void *o) {
    return 0x6;
}

/* +0x18 (Item80_vtable, Item81_vtable, Item82_vtable, ...) */
/* 0x002654A0 */
s32 ItemClassCCE0_IsCounted(void *o) {
    return 0;
}

/* 0x002654B0 */
void *Item81_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item81_vtable, ItemClassCCE0_vtable); }

/* 0x00265520 */
s32 Item81_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_057_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item81_vtable) */
/* 0x00265550 */
s32 Item81_SortKey(void *o) {
    return 0x320;
}

/* 0x00265560 */
void *Item82_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item82_vtable, ItemClassCCE0_vtable); }

/* 0x002655D0 */
s32 Item82_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_058_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item82_vtable) */
/* 0x00265600 */
s32 Item82_SortKey(void *o) {
    return 0x330;
}

/* 0x00265610 */
void *Item83_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item83_vtable, ItemClassCCE0_vtable); }

/* 0x00265680 */
s32 Item83_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_059_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item83_vtable) */
/* 0x002656B0 */
s32 Item83_SortKey(void *o) {
    return 0x340;
}

/* 0x002656C0 */
void *Item86_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item86_vtable, ItemClassCE70_vtable); }

/* 0x00265730 */
s32 Item86_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_05A_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item86_vtable) */
/* 0x00265760 */
s32 Item86_SortKey(void *o) {
    return 0x410;
}

/* +0x10 (Item86_vtable, Item87_vtable, Item88_vtable, ...) */
/* 0x00265770 */
s32 ItemClassCE70_Kind(void *o) {
    return 0x5;
}

/* +0x14 (Item86_vtable, Item87_vtable, Item88_vtable, ...) */
/* 0x00265780 */
s32 ItemClassCE70_Actions(void *o) {
    return 0x6;
}

/* +0x18 (Item86_vtable, Item87_vtable, Item88_vtable, ...) */
/* 0x00265790 */
s32 ItemClassCE70_IsCounted(void *o) {
    return 0;
}

/* 0x002657A0 */
void *Item87_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item87_vtable, ItemClassCE70_vtable); }

/* 0x00265810 */
s32 Item87_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_05B_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item87_vtable) */
/* 0x00265840 */
s32 Item87_SortKey(void *o) {
    return 0x420;
}

/* 0x00265850 */
void *Item88_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item88_vtable, ItemClassCE70_vtable); }

/* 0x002658C0 */
s32 Item88_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_05C_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item88_vtable) */
/* 0x002658F0 */
s32 Item88_SortKey(void *o) {
    return 0x430;
}

/* 0x00265900 */
void *Item89_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item89_vtable, ItemClassCE70_vtable); }

/* 0x00265970 */
s32 Item89_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_05D_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item89_vtable) */
/* 0x002659A0 */
s32 Item89_SortKey(void *o) {
    return 0x440;
}

/* 0x002659B0 */
void *Item8A_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item8A_vtable, ItemClassD000_vtable); }

/* 0x00265A20 */
s32 Item8A_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_05E_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item8A_vtable) */
/* 0x00265A50 */
s32 Item8A_SortKey(void *o) {
    return 0x510;
}

/* +0x10 (Item8A_vtable, Item8B_vtable, Item8C_vtable, ...) */
/* 0x00265A60 */
s32 ItemClassD000_Kind(void *o) {
    return 0x9;
}

/* +0x14 (Item8A_vtable, Item8B_vtable, Item8C_vtable, ...) */
/* 0x00265A70 */
s32 ItemClassD000_Actions(void *o) {
    return 0x6;
}

/* +0x18 (Item8A_vtable, Item8B_vtable, Item8C_vtable, ...) */
/* 0x00265A80 */
s32 ItemClassD000_IsCounted(void *o) {
    return 0;
}

/* 0x00265A90 */
void *Item8B_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item8B_vtable, ItemClassD000_vtable); }

/* 0x00265B00 */
s32 Item8B_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_05F_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item8B_vtable) */
/* 0x00265B30 */
s32 Item8B_SortKey(void *o) {
    return 0x520;
}

/* 0x00265B40 */
void *Item8C_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item8C_vtable, ItemClassD000_vtable); }

/* 0x00265BB0 */
s32 Item8C_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM04_ITEM_060_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item8C_vtable) */
/* 0x00265BE0 */
s32 Item8C_SortKey(void *o) {
    return 0x530;
}

/* 0x00265BF0 */
void *Item8D_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item8D_vtable, ItemClassD000_vtable); }

/* 0x00265C60 */
s32 Item8D_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM04_ITEM_061_TEX, dest, 0x4000000, 0);
}

/* +0x1C (Item8D_vtable) */
/* 0x00265C90 */
s32 Item8D_SortKey(void *o) {
    return 0x540;
}

/* 0x00265CA0 */
void *Item90_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item90_vtable, ItemClassD320_vtable); }

/* "ITEM01\ITEM_023.TEX" */
/* 0x00265D10 */
s32 Item90_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_023_TEX, dst);
}

/* +0x1C (Item90_vtable) */
/* 0x00265D40 */
s32 Item90_SortKey(void *o) {
    return 0x820;
}

/* +0x10 (Item90_vtable, Item91_vtable, Item92_vtable, ...) */
/* 0x00265D50 */
s32 ItemClassD320_Kind(void *o) {
    return 0x6;
}

/* +0x14 (Item90_vtable, Item91_vtable, Item92_vtable, ...) */
/* 0x00265D60 */
s32 ItemClassD320_Actions(void *o) {
    return 0x6;
}

/* +0x18 (Item90_vtable, Item91_vtable, Item92_vtable, ...) */
/* 0x00265D70 */
s32 ItemClassD320_IsCounted(void *o) {
    return 0x1;
}

/* 0x00265D80 */
void *Item91_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item91_vtable, ItemClassD320_vtable); }

/* "ITEM01\ITEM_024.TEX" */
/* 0x00265DF0 */
s32 Item91_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_024_TEX, dst);
}

/* +0x1C (Item91_vtable) */
/* 0x00265E20 */
s32 Item91_SortKey(void *o) {
    return 0x610;
}

/* 0x00265E30 */
void *Item92_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item92_vtable, ItemClassD320_vtable); }

/* "ITEM01\ITEM_027.TEX" */
/* 0x00265EA0 */
s32 Item92_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_027_TEX, dst);
}

/* +0x1C (Item92_vtable) */
/* 0x00265ED0 */
s32 Item92_SortKey(void *o) {
    return 0x710;
}

/* 0x00265EE0 */
void *Item93_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item93_vtable, ItemClassD320_vtable); }

/* "ITEM01\ITEM_02B.TEX" */
/* 0x00265F50 */
s32 Item93_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_02B_TEX, dst);
}

/* +0x1C (Item93_vtable) */
/* 0x00265F80 */
s32 Item93_SortKey(void *o) {
    return 0x830;
}

/* 0x00265F90 */
void *Item94_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item94_vtable, ItemClassD320_vtable); }

/* "ITEM01\ITEM_025.TEX" */
/* 0x00266000 */
s32 Item94_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_025_TEX, dst);
}

/* +0x1C (Item94_vtable) */
/* 0x00266030 */
s32 Item94_SortKey(void *o) {
    return 0x620;
}

/* 0x00266040 */
void *Item95_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item95_vtable, ItemClassD320_vtable); }

/* "ITEM01\ITEM_028.TEX" */
/* 0x002660B0 */
s32 Item95_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_028_TEX, dst);
}

/* +0x1C (Item95_vtable) */
/* 0x002660E0 */
s32 Item95_SortKey(void *o) {
    return 0x720;
}

/* 0x002660F0 */
void *Item97_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item97_vtable, ItemClassD320_vtable); }

/* "ITEM01\ITEM_026.TEX" */
/* 0x00266160 */
s32 Item97_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_026_TEX, dst);
}

/* +0x1C (Item97_vtable) */
/* 0x00266190 */
s32 Item97_SortKey(void *o) {
    return 0x630;
}

/* 0x002661A0 */
void *Item98_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item98_vtable, ItemClassD320_vtable); }

/* "ITEM01\ITEM_029.TEX" */
/* 0x00266210 */
s32 Item98_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_029_TEX, dst);
}

/* +0x1C (Item98_vtable) */
/* 0x00266240 */
s32 Item98_SortKey(void *o) {
    return 0x730;
}

/* 0x00266250 */
void *Item9B_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item9B_vtable, ItemClassD320_vtable); }

/* "ITEM01\ITEM_02A.TEX" */
/* 0x002662C0 */
s32 Item9B_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_02A_TEX, dst);
}

/* +0x1C (Item9B_vtable) */
/* 0x002662F0 */
s32 Item9B_SortKey(void *o) {
    return 0x810;
}

/* 0x00266300 */
void *ItemA4_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA4_vtable, ItemClassD640_vtable); }

/* "ITEM03\ITEM_04A.TEX" */
/* 0x002663F0 */
s32 ItemA4_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM03_ITEM_04A_TEX, dst);
}

/* +0x10 (ItemA4_vtable, ItemA5_vtable, ItemA6_vtable, ...) */
/* 0x00266420 */
s32 ItemClassD640_Kind(void *o) {
    return 0x7;
}

/* +0x14 (ItemA4_vtable, ItemA5_vtable, ItemA6_vtable, ...) */
/* 0x00266430 */
s32 ItemClassD640_Actions(void *o) {
    return 0x1;
}

/* +0x18 (ItemA4_vtable, ItemA5_vtable, ItemA6_vtable, ...) */
/* 0x00266440 */
s32 ItemClassD640_IsCounted(void *o) {
    return 0;
}

/* 0x00266450 */
void *ItemA5_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA5_vtable, ItemClassD640_vtable); }

/* +0x3C (ItemA5_vtable) */
/* 0x002664C0 */
s32 ItemA5_Use(void *o) {
    return 0;
}

/* "ITEM03\ITEM_053.TEX" */
/* 0x002664D0 */
s32 ItemA5_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM03_ITEM_053_TEX, dst);
}

/* 0x00266500 */
void *ItemA6_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA6_vtable, ItemClassD640_vtable); }

/* +0x3C (ItemA6_vtable) */
/* 0x00266570 */
s32 ItemA6_Use(void *o) {
    return 0;
}

/* "ITEM03\ITEM_054.TEX" */
/* 0x00266580 */
s32 ItemA6_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM03_ITEM_054_TEX, dst);
}

/* 0x002665B0 */
void *ItemA7_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA7_vtable, ItemClassD640_vtable); }

/* +0x3C (ItemA7_vtable) */
/* 0x00266620 */
s32 ItemA7_Use(void *o) {
    return 0;
}

/* "ITEM03\ITEM_055.TEX" */
/* 0x00266630 */
s32 ItemA7_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM03_ITEM_055_TEX, dst);
}

/* 0x00266660 */
void *ItemA8_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA8_vtable, ItemClassD640_vtable); }

/* +0x3C (ItemA8_vtable) */
/* 0x002666D0 */
s32 ItemA8_Use(void *o) {
    return 0;
}

/* "ITEM00\ITEM_FFF.TEX" */
/* 0x002666E0 */
s32 ItemA8_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A920, dst);
}

/* 0x00266710 */
void *ItemA9_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA9_vtable, ItemClassD640_vtable); }

/* +0x3C (ItemA9_vtable) */
/* 0x00266780 */
s32 ItemA9_Use(void *o) {
    return 0;
}

/* "ITEM00\ITEM_FFF.TEX" */
/* 0x00266790 */
s32 ItemA9_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A900, dst);
}

/* 0x002667C0 */
void *ItemAA_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemAA_vtable, ItemClassD640_vtable); }

/* +0x3C (ItemAA_vtable) */
/* 0x00266830 */
s32 ItemAA_Use(void *o) {
    return 0;
}

/* "ITEM00\ITEM_FFF.TEX" */
/* 0x00266840 */
s32 ItemAA_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A8E0, dst);
}

/* 0x00266870 */
void *ItemAB_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemAB_vtable, ItemClassD640_vtable); }

/* +0x3C (ItemAB_vtable) */
/* 0x002668E0 */
s32 ItemAB_Use(void *o) {
    return 0;
}

/* "ITEM00\ITEM_FFF.TEX" */
/* 0x002668F0 */
s32 ItemAB_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A8C0, dst);
}

/* 0x00266920 */
void *ItemAC_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemAC_vtable, ItemClassD640_vtable); }

/* +0x3C (ItemAC_vtable) */
/* 0x00266990 */
s32 ItemAC_Use(void *o) {
    return 0;
}

/* "ITEM00\ITEM_FFF.TEX" */
/* 0x002669A0 */
s32 ItemAC_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A8A0, dst);
}

/* 0x002669D0 */
void *Item40_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item40_vtable, ItemClassC920_vtable); }

/* +0x1C (Item40_vtable) */
/* 0x00266A40 */
s32 Item40_SortKey(void *o) {
    return 0x110;
}

/* "ITEM00\ITEM_00A.TEX" */
/* 0x00266A50 */
s32 Item40_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM00_ITEM_00A_TEX, dst);
}

/* use: composure -100, stamina -1800 */
/* 0x00266A80 */
s32 Item40_Use(void *o) {
    item_meters(-100.0f, -1800);
    return 1;
}

/* 0x00266B60 */
void *Item41_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item41_vtable, ItemClassC920_vtable); }

/* +0x1C (Item41_vtable) */
/* 0x00266BD0 */
s32 Item41_SortKey(void *o) {
    return 0x120;
}

/* "ITEM00\ITEM_009.TEX" */
/* 0x00266BE0 */
s32 Item41_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM00_ITEM_009_TEX, dst);
}

/* use: Progress +0x9E0 -0.2666 for 300 frames (+0x9E4) */
/* 0x00266C10 */
s32 Item41_Use(void *o) {
    AT(gProgress, 0x9E0, u32) = 0xBE888889;   /* -0.26666668 */
    AT(gProgress, 0x9E4, s32) = 300;
    return 1;
}

/* 0x002CCBD0 */
void *Item00_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item00_vtable, PlainItem_vtable); }

/* 0x002CCC40 */
s32 Item00_LoadPicture(void *self, s32 a) { return LOAD(D_0045D4A0, a); }

/* Item00_vtable: at spot 0xA of room 0xF, event 0x1 */
/* 0x002CCC70 */
s32 Item00_Use(void *o) {
    return use_at_spot(0xF, 0xA, 0x1);
}

/* 0x002CCD30 */
void *Item01_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item01_vtable, PlainItem_vtable); }

/* +0x18 (Item01_vtable) */
/* 0x002CCDA0 */
s32 Item01_IsCounted(void *o) {
    return 0x2;
}

/* 0x002CCDB0 */
s32 Item01_LoadPicture(void *self, s32 a) { return LOAD(str_ITEM00_ITEM_001_TEX, a); }

/* use: unless Progress +0x1C bit 0x80, at spot 9 of room 0x1C: event 0, flag 0x18 */
/* 0x002CCDE0 */
s32 Item01_Use(void) {
    Progress *p = gProgress;
    VObject *ev_mgr;

    if (AT(p, 0x1C, u32) & 0x80) {
        return 0;
    }
    if (!item_room_spot(p, 0x1C, 9)) {
        return 0;
    }
    ev_mgr = gEvents;
    item_event(ev_mgr, 0, 0, gCharPlayer);
    Progress_SetFlag(p, 0x18);
    return 4;
}

/* +0x38: a counter (+0x10) that runs 9000 frames; then it goes (the items' +8 list, Items_Give
   (2, 1)) and Progress +0x84 bit 31 is set */
/* 0x002CCED0 */
s32 Item01_CountUp(void *o) {
    if (AT(o, 0x10, u32) < 9001) {
        if (AT(o, 0x4, s32) != -1 && (VCALL(o, 0x18, s32 (*)(void *))(o) & 0xFF) == 2 && AT(o, 0x10, u32) != (u32)-1) {
            AT(o, 0x10, u32) += 1;
        }
        return 0;
    }
    Items_Give((u8 *)gSubScreen + 8, 2, 1);
    AT(gProgress, 0x84, u32) |= 0x80000000;
    return 2;
}

/* 0x002CCF90 */
void *Item02_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item02_vtable, PlainItem_vtable); }

/* 0x002CD000 */
s32 Item02_LoadPicture(void *self, s32 a) { return LOAD(str_ITEM00_ITEM_002_TEX, a); }

/* use: on the altar */
/* 0x002CD030 */
s32 Item02_Use(void *o) {
    return item_offer(gProgress, o);
}

/* 0x002CD130 */
void *Item03_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item03_vtable, PlainItem_vtable); }

/* 0x002CD1A0 */
s32 Item03_LoadPicture(void *self, s32 a) { return LOAD(str_ITEM00_ITEM_003_TEX, a); }

/* use: at spot 5 of room 0x22: flag 0x18, event 5 */
/* 0x002CD1D0 */
s32 Item03_Use(void) {
    Progress *p = gProgress;

    if (!item_room_spot(p, 0x22, 5)) {
        return 0;
    }
    Progress_SetFlag(p, 0x18);
    item_event(gEvents, 0, 5, gCharPlayer);
    return 4;
}

/* 0x002CD2A0 */
void *Item04_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item04_vtable, PlainItem_vtable); }

/* 0x002CD310 */
s32 Item04_LoadPicture(void *self, s32 a) { return LOAD(str_ITEM00_ITEM_004_TEX, a); }

/* use: at spot 0xC of room 4: event 4, flag 0x18 */
/* 0x002CD340 */
s32 Item04_Use(void) {
    Progress *p = gProgress;

    if (!item_room_spot(p, 4, 0xC)) {
        return 0;
    }
    item_event(gEvents, 0, 4, gCharPlayer);
    Progress_SetFlag(p, 0x18);
    return 4;
}

/* 0x002CD420 */
void *Item05_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item05_vtable, PlainItem_vtable); }

/* 0x002CD490 */
s32 Item05_LoadPicture(void *self, s32 a) { return LOAD(str_ITEM00_ITEM_005_TEX, a); }

/* 0x002CD4C0 */
s32 Item05_Use(void) {
    F(gProgress, 0x84, u32) |= 0x400000;
    return 2;
}

/* a pool entry: constructor / destructor */
void *PoolEntry_ctor(void *e) {
    AT(e, 0x0, void **) = PoolEntry_vtable;
    AT(e, 0x4, s32) = -1;
    AT(e, 0x8, u8) = 0;
    AT(e, 0x10, s64) = 0;
    return e;
}

/* 0x002D2750 */
void *ItemClassF430_dtor(void *o, s32 flags) { return item_dtor2(o, flags, ItemClassF430_vtable); }

/* +0x10 (ItemClassF430_vtable) */
/* 0x002D27B0 */
s32 ItemClassF430_Kind(void *o) {
    return 0x1;
}

/* +0x14 (ItemClassF430_vtable) */
/* 0x002D27C0 */
s32 ItemClassF430_Actions(void *o) {
    return 0x5;
}

/* +0x18 (ItemClassF430_vtable) */
/* 0x002D27D0 */
s32 ItemClassF430_IsCounted(void *o) {
    return 0;
}

/* +0x20 (ItemClassF430_vtable) */
/* 0x002D3A50 */
void *ItemClassF430_Get20(void *o) {
    return (u8 *)o + 0x10;
}

/* 0x002D3A60 */
void Item3F_Set(u8 *p, const u8 *src) {
    u32 i;

    for (i = 0; i < 8; i++) {
        p[0x10 + i] = src[i];
    }
}

/* 0x002D7910 */
void *ItemA0_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA0_vtable, ItemClassD640_vtable); }

/* 0x002D7980 */
s32 ItemA0_LoadPicture(void *self, void *dest) { return LOAD_002D1360(str_ITEM01_ITEM_02F_TEX, dest); }

/* +0x3C (ItemA0_vtable) */
/* 0x002D79B0 */
s32 ItemA0_Use(void *o) {
    return 0;
}

/* 0x002D79C0 */
void *ItemA1_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA1_vtable, ItemClassD640_vtable); }

/* 0x002D7A30 */
s32 ItemA1_LoadPicture(void *self, void *dest) { return LOAD_002D1360(str_ITEM01_ITEM_030_TEX, dest); }

/* 0x002EEB60 */
void *Item06_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item06_vtable, PlainItem_vtable); }

/* "ITEM00\ITEM_006.TEX" */
/* 0x002EEBD0 */
s32 Item06_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM00_ITEM_006_TEX, dst);
}

/* 0x00303C30 */
void *Item07_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item07_vtable, PlainItem_vtable); }

/* "ITEM00\ITEM_007.TEX" */
/* 0x00303CA0 */
s32 Item07_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM00_ITEM_007_TEX, dst);
}

/* 0x00306B20 */
void *Item08_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item08_vtable, PlainItem_vtable); }

/* 0x00306B90 */
s32 Item08_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_03F_TEX, dest);
}

/* 0x00307130 */
void *Item09_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item09_vtable, PlainItem_vtable); }

/* 0x003071A0 */
s32 Item09_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_040_TEX, dest);
}

/* 0x00307740 */
void *Item0A_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item0A_vtable, PlainItem_vtable); }

/* 0x003077B0 */
s32 Item0A_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_041_TEX, dest);
}

/* 0x00307D50 */
void *Item0B_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item0B_vtable, PlainItem_vtable); }

/* 0x00307DC0 */
s32 Item0B_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_042_TEX, dest);
}

/* 0x00308360 */
void *Item0C_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item0C_vtable, PlainItem_vtable); }

/* 0x003083D0 */
s32 Item0C_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_043_TEX, dest);
}

/* 0x0031CAA0 */
void *Item0D_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item0D_vtable, PlainItem_vtable); }

/* 0x0031CB10 */
s32 Item0D_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_039_TEX, dest);
}

/* 0x0031CD70 */
void *Item0E_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item0E_vtable, PlainItem_vtable); }

/* 0x0031CDE0 */
s32 Item0E_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_031_TEX, dest);
}

/* 0x0031CFB0 */
void *Item0F_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item0F_vtable, PlainItem_vtable); }

/* 0x0031D020 */
s32 Item0F_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_03A_TEX, dest);
}

/* Item0F_vtable: at door 0 of room 0x4F, event 1 */
/* 0x0031D050 */
s32 Item0F_Use(void *o) {
    return use_at_door(0x4F, 0, 1);
}

/* 0x0031D110 */
void *Item10_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item10_vtable, PlainItem_vtable); }

/* 0x0031D180 */
s32 Item10_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_046_TEX, dest);
}

/* 0x0031D590 */
void *Item11_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item11_vtable, PlainItem_vtable); }

/* 0x0031D600 */
s32 Item11_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_03B_TEX, dest);
}

/* Item11_vtable: at door 4 of room 0x55, event 2 */
/* 0x0031D630 */
s32 Item11_Use(void *o) {
    return use_at_door(0x55, 4, 2);
}

/* base +0x8: destroy (the task's window freed); the object returned, not freed */
/* 0x003224F0 */
void *SynthBase_Destroy(u8 *o) {
    if (o != NULL) {
        AT(o, 0x0, void **) = SynthBase_vtable;
        VCALL(o, 0x14, void (*)(u8 *))(o);
        if ((Task *)(o + 0x14) != NULL && SY_TASK(o)->child != NULL) {
            Task_dtor(SY_TASK(o)->child, 1);
            SY_TASK(o)->child = NULL;
        }
    }
    return o;
}

/* 0x00322560 */
void SynthBase_Noop(void *o) {
}

/* placement new: its memory */
/* 0x00322570 */
void *SynthBase_new(u32 size, void *mem) {
    return mem;
}

/* the material list: a frame as long as the materials there are, each with its count, the
 * cursor (+0x10) round them; message 2 while choosing. Confirm on one there is: its place
 * (0..5), cancel: "leave"; -1 for none chosen (or no materials) */
/* 0x00322580 */
s8 SlotMachine_MaterialList(u8 *o) {
    Task *t = SY_TASK(o);
    s32 n = 0, last, i, k, y, old;
    s32 sel = -1;
    s8 chosen = -1;

    for (i = 0; i < 6; i++) {
        if ((s8)o[0x118 + i] != 0) {
            n = (u8)(n + 1);
        }
    }
    if (n == 0) {
        return -1;
    }
    sy_rect(0, 0x40, 0xF8, 0x30, 0x108, 0x30, 0x80808080, 0x19, 4, 2);
    last = n - 1;
    for (i = 0; i < last; i++) {
        sy_rect(0, 0x70 + i * 0x20, 0xF8, 0x20, 0x108, 0x50, 0x80808080, 0x19, 4, 2);
    }
    sy_rect(0, 0x70 + i * 0x20, 0xF8, 0x20, 0x108, 0x70, 0x80808080, 0x19, 4, 2);
    for (k = 0, i = 0, y = 0x60; i < 6; i++) {
        if ((s8)o[0x118 + i] != 0) {
            u8 color = 0;

            if (k == SY_STEP(o)) {
                sel = i;
                color = 2;
            }
            Task_ShowText(t, 0x20, y, color, Task_MessageText(t, (u16)(i + 0x8170)), 0x80, 0x30, 0x10, 0x15);
            Task_Printf(t, 0xB1, y, color, D_00460408, (s8)o[0x118 + i]);
            y += 0x20;
            k++;
        }
    }
    old = SY_STEP(o);
    if (gMenuPressed & MENU_UP) {
        if (--SY_STEP(o) < 0) {
            SY_STEP(o) = last;
        }
    } else if (gMenuPressed & MENU_DOWN) {
        if (last < ++SY_STEP(o)) {
            SY_STEP(o) = 0;
        }
    }
    if (old != SY_STEP(o)) {
        Sound_PlaySE(SE_CURSOR);
    } else if (gMenuPressed & MENU_CANCEL) {
        o[0x158] = 1;
    } else if ((gMenuPressed & MENU_CONFIRM) && (s8)o[0x118 + sel] > 0) {
        chosen = sel;
        Sound_PlaySE(SE_DECIDE);
    }
    if (chosen == -1) {
        Task_Open(t, 2);
        Task_Run(t);
    } else {
        Task_Close(t);
    }
    return chosen;
}

/* the six materials' counts (items 0x70..0x75) into +0x118..; any at all */
/* 0x00322970 */
s32 Synth_CountMaterials(u8 *o) {
    u8 *items = (u8 *)gSubScreen + 8;
    s32 any = 0, i;

    for (i = 0; i < 6; i++) {
        s8 n = Items_Count(items, i + 0x70);

        o[0x118 + i] = n;
        any = any || n != 0;
    }
    return any;
}

/* 0x00322A00 */
void SynthBase_Noop14(void) {
}

/* base +0x10: each frame, its state */
/* 0x00322A10 */
void SynthBase_Update(u8 *o) {
    gLanguage = 1;
    if (ptmf_test(SY_STATE(o))) {
        ptmf_scall(o, SY_STATE(o));
    }
}

/* 0x00322A60 */
void SynthBase_Reset(u8 *self) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        ((volatile u8 *)self)[0x118 + i] = 0xFF;
    }
    self[0x158] = 0;
}

/* the pot +0x8 */
/* 0x00322AA0 */
void *SynthPot_dtor(u8 *o, s32 flags) {
    return sy_dtor(o, SynthPot_vtable, flags);
}

/* 0x00322B30 */
void SynthPot_StateDebug(u8 *self) {
    if ((gMenuPressed >> 5) & 1) {
        self[0x158] = 1;
    }
}

/* 0x00322B70 */
void SynthPot_Noop14(void) {
}

/* the pot +0x10: its title (a debug line), then as the base */
/* 0x00322B80 */
void SynthPot_Update(u8 *o) {
    Task_Printf(SY_TASK(o), 0x32, 0x32, 0x80, str_ITEM_SYNTHESIZER_POT);
    SynthBase_Update(o);
}

/* the pot +0xC: started - the counts taken, its state SynthPot_StateDebug_ptmf */
/* 0x00322BD0 */
void SynthPot_Start(u8 *o) {
    SY_STEP(o) = 0;
    Synth_CountMaterials(o);
    ptmf_set(SY_STATE(o), &SynthPot_StateDebug_ptmf);
}

/* the slot machine +0x8 */
/* 0x00322C40 */
void *SlotMachine_dtor(u8 *o, s32 flags) {
    return sy_dtor(o, SlotMachine_vtable, flags);
}

/* the found item's step: the panel over the rows fading in (3 frames) and out (12; then bit 0,
 * "up"); when `shown`, the item's panel after it, fading out from frame 9 (then bit 7, "done"
 * - and with nothing shown, done as soon as up); counted to 50 */
/* 0x00322CD0 */
u8 ItemFound_Step(u8 *o, s32 shown) {
    s32 t = (s8)o[0x151], d;
    u8 flags = 0;
    f32 f;

    if ((u32)t < 4) {
        f = (f32)(u32)t / 3.0f;
    } else {
        d = t - 3;
        if ((u32)d >= 13) {
            flags |= 1;
            d = 12;
        }
        f = (f32)(u32)(12 - d) / 12.0f;
    }
    sy_rect(0x170, 0xA0, 0x40, 0xB0, 0x90, 0x20, sy_alpha(f, 0), 4, 0x19, 0xA);
    if (!shown) {
        if (flags) {
            flags |= 0x80;
        }
    } else {
        t = (s8)o[0x151];
        if ((u32)t < 10) {
            f = t == 0 ? 0.0f : 1.0f;
        } else {
            d = t - 9;
            if ((u32)d >= 14) {
                flags |= 0x80;
                d = 13;
            }
            f = (f32)(u32)(13 - d) / 13.0f;
        }
        sy_rect(0x180, 0xB0, 0x38, 0x90, 0xD0, 0x20, sy_alpha(f, 0), 4, 0x19, 9);
    }
    if ((s8)o[0x151] < 0x32) {
        o[0x151]++;
    }
    return flags;
}

/* state: "nothing to put in" - message 3; cancel or confirm leaves */
/* 0x00323040 */
void SlotMachine_StateNothing(u8 *o) {
    Task_Open(SY_TASK(o), 3);
    Task_Run(SY_TASK(o));
    if (gMenuPressed & (MENU_CANCEL | MENU_CONFIRM)) {
        o[0x158] = 1;
    }
}

#ifdef HG_NATIVE
/* cell `k`'s symbol `v` (D_00460920: its texels u, v, w, h, place x, y and flips), with OpenGL */
/* 0x003230B0 */
void SlotMachine_DrawCell(u8 *o, u8 k, u8 v) {
    extern const u8 D_00460920[][10];
    const u8 *e = D_00460920[k];
    s32 w = e[2], h = e[3], x0, x1, y0, y1;
    u8 *tex;

    if (TexCache_Resident(4, 0x19, 0x30, &tex) == -1) {
        return;
    }
    if (!e[9]) {
        x0 = AT(e, 4, s16);
        x1 = x0 + w;
    } else {
        x1 = AT(e, 4, s16);
        x0 = x1 + w;
    }
    if (!e[8]) {
        y0 = AT(e, 6, s16);
        y1 = y0 + h;
    } else {
        y1 = AT(e, 6, s16);
        y0 = y1 + h;
    }
    gl2d_sprite(0x30, x0, y0, x1, y1, tex, e[0], e[1], e[0] + w, e[1] + h, 0x80808080, sy_clut(v), 0x40);
}
#endif

#ifdef HG_NATIVE
/* row `row`'s symbol `v` (32 x 32 at D_004608F0 + 16, the texture's column 0x60 + 64 x `a`: `a`
 * the frame of it sliding in), with OpenGL */
/* 0x00323510 */
void SlotMachine_DrawRow(u8 *o, u8 row, u8 v, s32 a) {
    extern const s16 D_004608F0[][2];
    u8 *tex;
    s32 x = D_004608F0[row][0] + 0x10, y = D_004608F0[row][1] + 0x10, u = (u8)a * 64 + 0x60;

    if (TexCache_Resident(4, 0x19, 0x30, &tex) == -1) {
        return;
    }
    gl2d_sprite(0x30, x, y, x + 0x20, y + 0x20, tex, u, 0, u + 0x20, 0x20, 0x80808080, sy_clut(v), 0x40);
}
#endif

/* the result for the four symbols' counts (+0x145..): the more symbols and kinds the better
 * (0 best); -1 for none */
/* 0x00323890 */
s32 SlotMachine_Result(u8 *o) {
    s32 a = (s8)o[0x145], b = (s8)o[0x146], c = (s8)o[0x147], d = (s8)o[0x148];
    s32 kinds = (a > 0) + (b > 0) + (c > 0) + (d > 0);
    s32 total = a + b + c + d;
    s32 kinds3 = (b > 0) + (c > 0) + (d > 0);

    if (kinds >= 4 && total >= 8) {
        return 0;
    }
    if (total >= 22) {
        return 0;
    }
    if (kinds >= 4 || total >= 18) {
        return 1;
    }
    if (kinds3 >= 3 || total >= 15) {
        return 2;
    }
    if (d >= 7) {
        return 0x10;
    }
    if (d >= 5) {
        return 3;
    }
    if (b >= 5) {
        return 4;
    }
    if (d >= 3) {
        return 5;
    }
    if (kinds >= 3) {
        return 6;
    }
    if (d >= 2) {
        return 7;
    }
    if (b >= 2) {
        return 8;
    }
    if (c >= 2) {
        return 9;
    }
    if (a >= 3) {
        return 10;
    }
    if (d > 0) {
        return 11;
    }
    if (b > 0) {
        return 12;
    }
    if (c > 0) {
        return 13;
    }
    if (a >= 2) {
        return 14;
    }
    return a > 0 ? 15 : -1;
}

/* state: the rolling. First the top row's symbol drawn by the material's chances
 * (D_00460620); then each row in turn: its reel picked (D_004605C0 / kSlotReelsPicked) at a random
 * place, spun (a symbol every 5 frames, speeding through 8 levels every 31) until confirm or
 * the top speed stops it, easing onto the next symbol; after the ninth, sy_finish. The rows
 * drawn (the rolling one with the next symbol coming in) and message 0x10 */
/* 0x00323A70 */
void SlotMachine_StateRoll(u8 *o) {
    s32 r;

    if (SY_STEP(o) < 0) {
        f32 acc = 0.0f, lim;
        s32 i, found = 0;

        SY_STEP(o) = 0;
        lim = 100.0f * SY_RAND(0x18);
        for (i = 0; i < 5; i++) {
            acc += (f32)D_00460620[(s8)o[0x11E]][i];
            if (lim <= acc) {
                o[0x120] = i;
                found = 1;
                break;
            }
        }
        if (!found) {
            o[0x120] = 5;
        }
        Sound_Play(gSound, 1, 6);
    }
    if (o[0x11F] & 4) {   /* a row stopping */
        s8 c = o[0x12C];

        if (c != 0) {
            if (c >= 3) {
                o[0x12C] = c + 1;
                if ((s8)o[0x12C] >= 5) {
                    sy_reel_step(o);
                }
            } else {
                o[0x12C] = c - 1;
            }
        }
        if ((s8)o[0x12C] == 0) {
            o[0x11F] &= ~4;
            o[0x121 + SY_STEP(o)] = SY_REEL(o, o[0x12B]);
            if (++SY_STEP(o) >= 9) {
                o[0x11F] |= 2;
            } else {
                o[0x11F] &= ~1;
            }
        }
    }
    if (!(o[0x11F] & 4)) {
        if (!(o[0x11F] & 1)) {   /* the next row's reel */
            f32 acc = 0.0f, lim;
            s32 i, found = 0;

            lim = 100.0f * SY_RAND(0x18);
            for (i = 0; i < 10; i++) {
                acc += (f32)D_004605C0[(s8)o[0x11E]][i];
                if (lim <= acc) {
                    o[0x12A] = kSlotReelsPicked[i];
                    found = 1;
                    break;
                }
            }
            if (!found) {
                o[0x12A] = kSlotReelsPicked[10];
            }
            o[0x12B] = (s32)(35.0f * SY_RAND(0x1C));
            o[0x12E] = 0;
            o[0x12D] = 0;
            o[0x12C] = 0;
            o[0x11F] |= 1;
        }
        if (o[0x11F] & 2) {
            sy_finish(o);
        } else {
            o[0x12C]++;
            if ((s8)o[0x12C] >= 5) {
                sy_reel_step(o);
            }
            o[0x12D]++;
            if ((u32)(s8)o[0x12D] >= 0x1F) {
                o[0x12D] = 0;
                o[0x12E]++;
                if ((s8)o[0x12E] >= 9) {
                    o[0x12E] = 8;
                }
            }
            if ((gMenuPressed & MENU_CONFIRM) || (s8)o[0x12E] == 8) {
                Sound_Play(gSound, 2, 6);
                o[0x11F] |= 4;
            }
        }
    }
    for (r = 0; r < 10; r++) {
        s8 v = o[0x120 + r];

        if (v >= 0) {
            SlotMachine_DrawRow(o, r, v, 0);
        } else if (r == SY_STEP(o) + 1) {
            s8 c;

            SlotMachine_DrawRow(o, r, SY_REEL(o, o[0x12B]), 0);
            c = o[0x12C];
            if (c != 0) {
                u8 next = (s8)o[0x12B] + 1;

                if (next >= 0x23) {
                    next = 0;
                }
                if (SY_REEL(o, next) != SY_REEL(o, o[0x12B])) {
                    SlotMachine_DrawRow(o, r, SY_REEL(o, next), (u8)c);
                }
            }
        }
    }
    Task_Open(SY_TASK(o), 0x10);
    Task_Run(SY_TASK(o));
}

/* state: choosing the material; once chosen, one of it used, the rolling begins (SlotMachine_StateRoll_ptmf) */
/* 0x00324410 */
void SlotMachine_StateChoose(u8 *o) {
    s8 m = SlotMachine_MaterialList(o);

    if (m >= 0) {
        Items_UseId((u8 *)gSubScreen + 8, m + 0x70);
        o[0x11E] = m;
        o[0x11F] = 0;
        SY_STEP(o) = -1;
        ptmf_set(SY_STATE(o), &SlotMachine_StateRoll_ptmf);
    }
}

/* 0x003244C0 */
void SlotMachine_Noop14(void) {
}

/* the slot machine +0x10: as the base, then the start prompt blinking (alpha 0x40..0x80 over
 * 60 frames) */
/* 0x003244D0 */
void SlotMachine_Update(u8 *o) {
    s32 t;
    f32 f;

    SynthBase_Update(o);
    o[0x150]++;
    if ((u32)(s8)o[0x150] >= 0x3C) {
        o[0x150] = 0;
    }
    t = (s8)o[0x150];
    if ((u32)t < 0x1E) {
        f = (30.0f - (f32)t) / 30.0f;
    } else {
        f = ((f32)t - 30.0f) / 30.0f;
    }
    {
        u32 a = (u32)(64.0f * f) + 0x40;

        if (a >= 0x81) {
            a = 0x80;
        }
        sy_rect(0x1B8, 0x100, 0x40, 0x30, 0x1A0, 0, ((a << 24) & 0xFF000000) | 0x808080, 4, 0x19, 8);
    }
}

/* the slot machine +0xC: started - the counts taken; the material list (SlotMachine_StateChoose_ptmf) or, with
 * none, "nothing to use" (SlotMachine_StateNothing_ptmf) */
/* 0x00324650 */
void SlotMachine_Start(u8 *o) {
    SY_STEP(o) = 0;
    o[0x150] = 0;
    if (Synth_CountMaterials(o)) {
        ptmf_set(SY_STATE(o), &SlotMachine_StateChoose_ptmf);
    } else {
        ptmf_set(SY_STATE(o), &SlotMachine_StateNothing_ptmf);
    }
}

/* 0x0032CC90 */
void *Item14_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item14_vtable, PlainItem_vtable); }

/* 0x0032CD00 */
s32 Item14_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM01_ITEM_02C_TEX, dest);
}

/* Item14_vtable: at door 1 of room 0x25 unless Progress +0x20 bit 0x800000, event 4 */
/* 0x0032CD30 */
s32 Item14_Use(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) != 0x25 || (AT(p, 0x20, u32) & 0x800000) || !item_door_open(p, 1)) {
        return 0;
    }
    item_event(gEvents, 0, 4, gCharPlayer);
    Progress_SetFlag(p, 0x18);
    return 4;
}

/* 0x00331450 */
void *Item42_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item42_vtable, ItemClassC920_vtable); }

/* 0x003314C0 */
u32 Item42_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item42_vtable) */
/* 0x003314D0 */
s32 Item42_SortKey(void *o) {
    return 0x130;
}

/* 0x003314E0 */
s32 Item42_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM00_ITEM_00B_TEX, dest);
}

/* 0x00331510 */
s32 Item42_Use(void) {
    item_meters(-25.0f, -450);
    return 1;
}

/* 0x003315F0 */
void *Item43_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item43_vtable, ItemClassC920_vtable); }

/* 0x00331660 */
u32 Item43_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item43_vtable) */
/* 0x00331670 */
s32 Item43_SortKey(void *o) {
    return 0x140;
}

/* 0x00331680 */
s32 Item43_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM00_ITEM_00C_TEX, dest);
}

/* 0x003316B0 */
s32 Item43_Use(void) {
    item_meters(-100.0f, -1800);
    return 1;
}

/* 0x00331790 */
void *Item44_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item44_vtable, ItemClassC920_vtable); }

/* 0x00331800 */
u32 Item44_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item44_vtable) */
/* 0x00331810 */
s32 Item44_SortKey(void *o) {
    return 0x150;
}

/* 0x00331820 */
s32 Item44_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM00_ITEM_00D_TEX, dest);
}

/* 0x00331850 */
s32 Item44_Use(void) {
    b5_add_7E0(25.0f);
    return 1;
}

/* 0x003318B0 */
void *Item45_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item45_vtable, ItemClassC920_vtable); }

/* 0x00331920 */
u32 Item45_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item45_vtable) */
/* 0x00331930 */
s32 Item45_SortKey(void *o) {
    return 0x160;
}

/* 0x00331940 */
s32 Item45_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM00_ITEM_00E_TEX, dest);
}

/* 0x00331970 */
s32 Item45_Use(void) {
    b5_add_7E0(100.0f);
    return 1;
}

/* 0x003319D0 */
void *Item46_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item46_vtable, ItemClassC920_vtable); }

/* 0x00331A40 */
u32 Item46_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item46_vtable) */
/* 0x00331A50 */
s32 Item46_SortKey(void *o) {
    return 0x170;
}

/* 0x00331A60 */
s32 Item46_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM00_ITEM_00F_TEX, dest);
}

/* use: composure +25, she drinks (event 0x8D) */
/* 0x00331A90 */
s32 Item46_Use(void) {
    item_composure(25.0f);
    item_event(gEvents, 0, 0x8D, gCharPlayer);
    return 5;
}

/* 0x00331B60 */
void *Item47_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item47_vtable, ItemClassC920_vtable); }

/* 0x00331BD0 */
u32 Item47_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item47_vtable) */
/* 0x00331BE0 */
s32 Item47_SortKey(void *o) {
    return 0x180;
}

/* 0x00331BF0 */
s32 Item47_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM00_ITEM_010_TEX, dest);
}

/* use: composure +100 */
/* 0x00331C20 */
s32 Item47_Use(void) {
    item_composure(100.0f);
    item_event(gEvents, 0, 0x8D, gCharPlayer);
    return 5;
}

/* 0x00331CE0 */
void *Item48_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item48_vtable, ItemClassC920_vtable); }

/* 0x00331D50 */
u32 Item48_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item48_vtable) */
/* 0x00331D60 */
s32 Item48_SortKey(void *o) {
    return 0x190;
}

/* 0x00331D70 */
s32 Item48_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM00_ITEM_011_TEX, dest);
}

/* use: Progress +0x9E8 2.0 for 1800 frames (+0x9EC) */
/* 0x00331DA0 */
s32 Item48_Use(void) {
    F32(gProgress, 0x9E8) = 2.0f;
    S32(gProgress, 0x9EC) = 1800;
    item_event(gEvents, 0, 0x8D, gCharPlayer);
    return 5;
}

/* 0x00331E20 */
void *Item49_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item49_vtable, ItemClassC920_vtable); }

/* 0x00331E90 */
u32 Item49_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item49_vtable) */
/* 0x00331EA0 */
s32 Item49_SortKey(void *o) {
    return 0x1A0;
}

/* 0x00331EB0 */
s32 Item49_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM00_ITEM_012_TEX, dest);
}

/* use: unless it still runs, Progress +0x9F4 / +0x9F8 1800 frames */
/* 0x00331EE0 */
s32 Item49_Use(void) {
    if (S32(gProgress, 0x9F4) != 0) {
        return 0;
    }
    S32(gProgress, 0x9F8) = 1800;
    S32(gProgress, 0x9F4) = 1800;
    item_event(gEvents, 0, 0x8D, gCharPlayer);
    return 5;
}

/* 0x00331F70 */
void *Item4A_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item4A_vtable, ItemClassC920_vtable); }

/* 0x00331FE0 */
u32 Item4A_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item4A_vtable) */
/* 0x00331FF0 */
s32 Item4A_SortKey(void *o) {
    return 0x1B0;
}

/* 0x00332000 */
s32 Item4A_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM00_ITEM_013_TEX, dest);
}

/* use: composure -50, Progress +0x7D8 +50 */
/* 0x00332030 */
s32 Item4A_Use(void) {
    item_composure(-50.0f);
    F32(gProgress, 0x7D8) += 50.0f;
    item_event(gEvents, 0, 0x8D, gCharPlayer);
    return 5;
}

/* 0x00332120 */
void *Item4B_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item4B_vtable, ItemClassC920_vtable); }

/* 0x00332190 */
u32 Item4B_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item4B_vtable) */
/* 0x003321A0 */
s32 Item4B_SortKey(void *o) {
    return 0x1C0;
}

/* 0x003321B0 */
s32 Item4B_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00461020, dest);
}

/* 0x003321E0 */
s32 Item4B_Use(void) {
    u8 *p;

    b5_adjust_meters(-100.0f, -1800);
    p = (u8 *)gProgress;
    S32(p, 0x9FC) = 0;
    S32(p, 0xA00) = 1800;
    F32(p, 0xA04) = 3.0f;
    S32(p, 0xA08) = 1800;
    return 1;
}

/* 0x003322E0 */
void *Item4C_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item4C_vtable, ItemClassC920_vtable); }

/* 0x00332350 */
u32 Item4C_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item4C_vtable) */
/* 0x00332360 */
s32 Item4C_SortKey(void *o) {
    return 0x1E0;
}

/* 0x00332370 */
s32 Item4C_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM00_ITEM_015_TEX, dest);
}

/* 0x003323A0 */
s32 Item4C_Use(void) {
    S32((u8 *)gProgress, 0xA0C) = 1800;
    return 1;
}

/* 0x003323C0 */
void *Item60_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item60_vtable, ItemClass6B00_vtable); }

/* +0x1C (Item60_vtable) */
/* 0x00332430 */
s32 Item60_SortKey(void *o) {
    return 0x210;
}

/* +0x10 (Item60_vtable, Item61_vtable, Item62_vtable, ...) */
/* 0x00332440 */
s32 ItemClass6B00_Kind(void *o) {
    return 0x3;
}

/* +0x14 (Item60_vtable, Item61_vtable, Item62_vtable, ...) */
/* 0x00332450 */
s32 ItemClass6B00_Actions(void *o) {
    return 0x5;
}

/* +0x18 (Item60_vtable, Item61_vtable, Item62_vtable, ...) */
/* 0x00332460 */
s32 ItemClass6B00_IsCounted(void *o) {
    return 0x1;
}

/* 0x00332470 */
s32 Item60_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM00_ITEM_016_TEX, dest);
}

/* use: Hewie heals 20; already full, he trusts her a little less */
/* 0x003324A0 */
s32 Item60_Use(void *o) {
    if (!B5_HANDY(o)) {
        return 0;
    }
    if (item_hewie_heal(20)) {
        item_trust(-1);
    }
    return item_give_hewie(gEvents, 0x8F);
}

/* 0x00332620 */
void *Item61_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item61_vtable, ItemClass6B00_vtable); }

/* +0x1C (Item61_vtable) */
/* 0x00332690 */
s32 Item61_SortKey(void *o) {
    return 0x220;
}

/* 0x003326A0 */
s32 Item61_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM00_ITEM_017_TEX, dest);
}

/* use: Hewie heals 100; trust -1 */
/* 0x003326D0 */
s32 Item61_Use(void *o) {
    if (!B5_HANDY(o)) {
        return 0;
    }
    item_hewie_heal(100);
    item_trust(-1);
    return item_give_hewie(gEvents, 0x8F);
}

/* 0x00332830 */
void *Item62_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item62_vtable, ItemClass6B00_vtable); }

/* +0x1C (Item62_vtable) */
/* 0x003328A0 */
s32 Item62_SortKey(void *o) {
    return 0x230;
}

/* 0x003328B0 */
s32 Item62_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM01_ITEM_018_TEX, dest);
}

/* use: Hewie's +0x94 (50); trust +3 */
/* 0x003328E0 */
s32 Item62_Use(void *o) {
    if (!B5_HANDY(o)) {
        return 0;
    }
    VCALL(gCharPartner, 0x94, void (*)(u8 *, s32))((u8 *)gCharPartner, 50);
    item_trust(3);
    return item_give_hewie(gEvents, 0x90);
}

/* 0x00332A10 */
void *Item63_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item63_vtable, ItemClass6B00_vtable); }

/* 0x00332A80 */
u32 Item63_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item63_vtable) */
/* 0x00332A90 */
s32 Item63_SortKey(void *o) {
    return 0x240;
}

/* 0x00332AA0 */
s32 Item63_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM01_ITEM_019_TEX, dest);
}

/* use: as Item60_Use */
/* 0x00332AD0 */
s32 Item63_Use(void *o) {
    if (!B5_HANDY(o)) {
        return 0;
    }
    if (item_hewie_heal(20)) {
        item_trust(-1);
    }
    return item_give_hewie(gEvents, 0x8F);
}

/* 0x00332C50 */
void *Item64_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item64_vtable, ItemClass6B00_vtable); }

/* 0x00332CC0 */
u32 Item64_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item64_vtable) */
/* 0x00332CD0 */
s32 Item64_SortKey(void *o) {
    return 0x250;
}

/* 0x00332CE0 */
s32 Item64_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM01_ITEM_01A_TEX, dest);
}

/* use: as Item61_Use */
/* 0x00332D10 */
s32 Item64_Use(void *o) {
    if (!B5_HANDY(o)) {
        return 0;
    }
    item_hewie_heal(100);
    item_trust(-1);
    return item_give_hewie(gEvents, 0x8F);
}

/* 0x00332E70 */
void *Item65_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item65_vtable, ItemClass6B00_vtable); }

/* 0x00332EE0 */
u32 Item65_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item65_vtable) */
/* 0x00332EF0 */
s32 Item65_SortKey(void *o) {
    return 0x260;
}

/* 0x00332F00 */
s32 Item65_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM01_ITEM_01B_TEX, dest);
}

/* use: Hewie waits (Hewie_SetMode mode 3, 450 frames; Progress +0xA10 too); trust +20 */
/* 0x00332F30 */
s32 Item65_Use(void *o) {
    if (!B5_HANDY(o)) {
        return 0;
    }
    Hewie_SetMode((Hewie *)gCharPartner, 3, 450);
    S32(gProgress, 0xA10) = 450;
    item_trust(20);
    return item_give_hewie(gEvents, 0x91);
}

/* 0x00333060 */
void *Item66_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item66_vtable, ItemClass6B00_vtable); }

/* 0x003330D0 */
u32 Item66_Actions(void) {
    return 0x80000005;
}

/* +0x1C (Item66_vtable) */
/* 0x003330E0 */
s32 Item66_SortKey(void *o) {
    return 0x270;
}

/* 0x003330F0 */
s32 Item66_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM01_ITEM_01C_TEX, dest);
}

/* use: Hewie's health 1 (back on his feet); trust +3 */
/* 0x00333120 */
s32 Item66_Use(void *o) {
    if (!B5_HANDY(o)) {
        return 0;
    }
    S32(gCharPartner, 0x14C8) = 1;
    item_trust(3);
    return item_give_hewie(gEvents, 0x90);
}

/* 0x00338F00 */
void *Item15_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item15_vtable, PlainItem_vtable); }

/* 0x00338F70 */
s32 Item15_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_03C_TEX, dest);
}

/* 0x003392B0 */
void *Item16_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item16_vtable, PlainItem_vtable); }

/* 0x00339320 */
s32 Item16_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_03E_TEX, dest);
}

/* 0x00339660 */
void *Item17_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item17_vtable, PlainItem_vtable); }

/* 0x003396D0 */
s32 Item17_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_03D_TEX, dest);
}

/* 0x0033AE70 */
void *ItemA2_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA2_vtable, ItemClassD640_vtable); }

/* 0x0033AEE0 */
s32 ItemA2_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_044_TEX, dest);
}

/* +0x3C (ItemA2_vtable) */
/* 0x0033AF10 */
s32 ItemA2_Use(void *o) {
    return 0;
}

/* +0x40: Hewie is at hand - he can be reached (PoolEntry_PartnerCheck), within 20 and his triangle is
   the one Fiona finds his position on */
/* 0x0033B4C0 */
s32 ItemClass6B00_PartnerCheck(void *o) {
    u32 tri;

    if ((PoolEntry_PartnerCheck(o) & 0xFF) != 1) {
        return 0;
    }
    if (!(Actor_Distance((Actor *)gCharPlayer, (f32 *)((u8 *)gCharPartner + 0x10)) < 20.0f)) {
        return 0;
    }
    tri = *(u32 *)((u8 *)gCharPartner + 0x34);
    return Actor_TriTo((Actor *)gCharPlayer, (f32 *)((u8 *)gCharPartner + 0x10), 0x20008) == tri;
}

/* 0x0033BD50 */
void *ItemA3_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA3_vtable, ItemClassD640_vtable); }

/* 0x0033BDC0 */
s32 ItemA3_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_045_TEX, dest);
}

/* +0x3C (ItemA3_vtable) */
/* 0x0033BDF0 */
s32 ItemA3_Use(void *o) {
    return 0;
}

/* 0x0033E850 */
void *Item18_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item18_vtable, PlainItem_vtable); }

/* +0x14 (Item18_vtable) */
/* 0x0033E8C0 */
s32 Item18_Actions(void *o) {
    return 0x5;
}

/* 0x0033E8D0 */
s32 Item18_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_033_TEX, dest);
}

/* Item18_vtable: at spot 0x17 of room 0x48, event 0x0 */
/* 0x0033E900 */
s32 Item18_Use(void *o) {
    return use_at_spot(0x48, 0x17, 0x0);
}

/* 0x0033E9C0 */
void *Item19_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item19_vtable, PlainItem_vtable); }

/* +0x14 (Item19_vtable) */
/* 0x0033EA30 */
s32 Item19_Actions(void *o) {
    return 0x5;
}

/* 0x0033EA40 */
s32 Item19_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_034_TEX, dest);
}

/* Item19_vtable: at spot 0x17 of room 0x48, event 0x17 */
/* 0x0033EA70 */
s32 Item19_Use(void *o) {
    return use_at_spot(0x48, 0x17, 0x17);
}

/* 0x0033EB30 */
void *Item1A_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item1A_vtable, PlainItem_vtable); }

/* +0x14 (Item1A_vtable) */
/* 0x0033EBA0 */
s32 Item1A_Actions(void *o) {
    return 0x5;
}

/* 0x0033EBB0 */
s32 Item1A_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_035_TEX, dest);
}

/* Item1A_vtable: at spot 0x17 of room 0x48, event 0x18 */
/* 0x0033EBE0 */
s32 Item1A_Use(void *o) {
    return use_at_spot(0x48, 0x17, 0x18);
}

/* 0x0033ECA0 */
void *Item1B_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item1B_vtable, PlainItem_vtable); }

/* +0x14 (Item1B_vtable) */
/* 0x0033ED10 */
s32 Item1B_Actions(void *o) {
    return 0x5;
}

/* 0x0033ED20 */
s32 Item1B_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_036_TEX, dest);
}

/* Item1B_vtable: at spot 0x17 of room 0x48, event 0x19 */
/* 0x0033ED50 */
s32 Item1B_Use(void *o) {
    return use_at_spot(0x48, 0x17, 0x19);
}

/* 0x0033EE10 */
void *Item1C_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item1C_vtable, PlainItem_vtable); }

/* +0x14 (Item1C_vtable) */
/* 0x0033EE80 */
s32 Item1C_Actions(void *o) {
    return 0x5;
}

/* 0x0033EE90 */
s32 Item1C_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_037_TEX, dest);
}

/* Item1C_vtable: at spot 0x17 of room 0x48, event 0x1A */
/* 0x0033EEC0 */
s32 Item1C_Use(void *o) {
    return use_at_spot(0x48, 0x17, 0x1A);
}

/* 0x00344520 */
void *Item1D_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item1D_vtable, PlainItem_vtable); }

/* +0x14 (Item1D_vtable) */
/* 0x00344590 */
s32 Item1D_Actions(void *o) {
    return 0x5;
}

/* 0x003445A0 */
s32 Item1D_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, str_ITEM02_ITEM_038_TEX, dest, 0x4000000, 0);
}

/* 0x003510B0 */
void *Item1E_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item1E_vtable, PlainItem_vtable); }

/* 0x00351120 */
s32 Item1E_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, str_ITEM02_ITEM_047_TEX, dest, 0x4000000, 0);
}

/* 0x00351150 */
s32 Item1E_Use(void) {
    *(u32 *)((u8 *)gProgress + 0x84) |= 0x800000;
    return 2;
}

/* 0x00351170 */
void *Item1F_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item1F_vtable, PlainItem_vtable); }

/* 0x003511E0 */
s32 Item1F_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, str_ITEM03_ITEM_048_TEX, dest, 0x4000000, 0);
}

/* 0x00351210 */
s32 Item1F_Use(void) {
    *(u32 *)((u8 *)gProgress + 0x84) |= 0x1000000;
    return 2;
}

/* 0x00351230 */
void *Item20_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item20_vtable, PlainItem_vtable); }

/* 0x003512A0 */
s32 Item20_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, str_ITEM03_ITEM_049_TEX, dest, 0x4000000, 0);
}

/* 0x003512D0 */
s32 Item20_Use(void) {
    *(u32 *)((u8 *)gProgress + 0x84) |= 0x2000000;
    return 2;
}

/* 0x003512F0 */
void *Item21_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item21_vtable, PlainItem_vtable); }

/* 0x00351360 */
s32 Item21_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, str_ITEM03_ITEM_04B_TEX, dest, 0x4000000, 0);
}

/* 0x003514B0 */
void *Item22_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item22_vtable, PlainItem_vtable); }

/* 0x00351520 */
s32 Item22_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, str_ITEM03_ITEM_04C_TEX, dest, 0x4000000, 0);
}

/* 0x00351670 */
void *Item23_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item23_vtable, PlainItem_vtable); }

/* 0x003516E0 */
s32 Item23_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, str_ITEM03_ITEM_04D_TEX, dest, 0x4000000, 0);
}

/* Item23_vtable: at spot 0xB of room 0xC7, event 0x4 */
/* 0x00351710 */
s32 Item23_Use(void *o) {
    return use_at_spot(0xC7, 0xB, 0x4);
}

/* 0x003517D0 */
void *Item24_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item24_vtable, PlainItem_vtable); }

/* 0x00351840 */
s32 Item24_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, str_ITEM03_ITEM_04E_TEX, dest, 0x4000000, 0);
}

/* +0x3C (Item24_vtable) */
/* 0x00351870 */
s32 Item24_Use(void *o) {
    return 0;
}

/* 0x00351880 */
void *Item25_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item25_vtable, PlainItem_vtable); }

/* 0x003518F0 */
s32 Item25_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, str_ITEM03_ITEM_04F_TEX, dest, 0x4000000, 0);
}

/* 0x00351AA0 */
void *Item26_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item26_vtable, PlainItem_vtable); }

/* 0x00351B10 */
s32 Item26_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, str_ITEM03_ITEM_050_TEX, dest, 0x4000000, 0);
}

/* 0x00351C20 */
void *Item27_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item27_vtable, PlainItem_vtable); }

/* 0x00351C90 */
s32 Item27_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, str_ITEM03_ITEM_051_TEX, dest, 0x4000000, 0);
}

/* 0x00351CC0 */
s32 Item27_Use(void) {
    *(u32 *)((u8 *)gProgress + 0x84) |= 0x4000000;
    return 2;
}

/* 0x00351CE0 */
void *Item28_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item28_vtable, PlainItem_vtable); }

/* Starts loading a file named str_ITEM03_ITEM_052_TEX into dest. */
/* 0x00351D50 */
s32 Item28_LoadPicture(void *self, void *dest) {
    void *loader = gFileLoader;

    return VCALL(loader, 0xC, s32 (*)(void *, const char *, void *, s32, s32))(loader, str_ITEM03_ITEM_052_TEX, dest,
                                                                              0x4000000, 0);
}

/* 0x0035BB20 */
void *Item29_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item29_vtable, PlainItem_vtable); }

/* "ITEM04\ITEM_062.TEX" */
/* 0x0035BB90 */
s32 Item29_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM04_ITEM_062_TEX, dst);
}

/* +0x3C (Item29_vtable) */
/* 0x0035BBC0 */
s32 Item29_Use(void *o) {
    return 0;
}

/* state: an item found (+0x14A its place, -1 none; +0x14B how many; +0x14C its id, big endian):
 * the screen's rows and cells refreshed; once up, "obtained" with the item's name (and count)
 * in a box, its sound played once (+0x152) - or message 5 for none; when done and confirmed,
 * the item added, the places cleared (but +0x150), the task closed and the next state
 * (Synth_CountMaterials: SlotMachine_StateChoose_ptmf2, else SlotMachine_StateNothing_ptmf2) */
/* 0x0037ED50 */
void ItemFound_StateShow(u8 *o) {
    Task *t = (Task *)(o + 0x14);
    u8 flags = ItemFound_Step(o, (s8)o[0x14A] >= 0);
    s32 id = -1, i;

    for (i = 0; i < 10; i++) {
        SlotMachine_DrawRow(o, i, o[0x120 + i], 0);
    }
    for (i = 0; i < 22; i++) {
        u8 k = D_00460A00[i];
        s8 c = o[0x12F + k];

        if (c != -1 && c != 4) {
            SlotMachine_DrawCell(o, k, c);
        }
    }
    if (flags & 1) {
        if ((s8)o[0x14A] < 0) {
            Task_Open(t, 5);
            Task_Run(t);
        } else {
            u16 w1, w2, cw;
            s32 x;

            if ((s8)o[0x152] == 0) {
                Sound_Play(gSound, 3, 6);
                o[0x152] = 1;
            }
            id = o[0x14C] << 24 | o[0x14D] << 16 | o[0x14E] << 8 | o[0x14F];
            w1 = Task_MessageWidth(t, (id + 0x8100) & 0xFFFF, 0x10);
            cw = (s8)o[0x14B] == 1 ? 0 : 0x1B;
            w2 = Task_MessageWidth(t, 4, 0x10);
            x = (u16)(0x100 - (s32)(w1 + cw + w2) / 2);
            Task_DrawBox(t, 0x100, 0x172, 0x100, 5, 0x70, 0x30);
            Task_ShowText(t, x, 0x164, 0, Task_MessageText(t, 4), 0x80, 0x30, 0x10, 0x15);
            Task_ShowText(t, x + w2, 0x164, 7, Task_MessageText(t, (id + 0x8100) & 0xFFFF), 0x80, 0x30, 0x10, 0x15);
            if (cw != 0) {
                Task_Printf(t, w1 + (x + w2), 0x164, 0, str_xN_2, (s8)o[0x14B]);
            }
        }
    }
    if ((flags & 0x80) && (gMenuPressed & MENU_CONFIRM)) {
        if (id != -1) {
            Items_Give((u8 *)gSubScreen + 8, id, o[0x14B]);
        }
        for (i = 0; i < 0x40; i++) {
            if (i != 0x38) {
                o[0x118 + i] = 0xFF;
            }
        }
        Task_Close(t);
        AT(o, 0x10, s32) = 0;
        if (Synth_CountMaterials(o)) {
            ptmf_set((PTMF *)(o + 0x4), &SlotMachine_StateChoose_ptmf2);
        } else {
            ptmf_set((PTMF *)(o + 0x4), &SlotMachine_StateNothing_ptmf2);
        }
    }
}

/* item `i` of list `l`'s +0x20 (0 for an empty place) */
/* 0x002604E0 */
s32 Items_Field20(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return 0;
    }
    return VCALL(it, 0x20, s32 (*)(VObject *))(it);
}

extern const s8 kItemKindSlot[];   /* by item kind (+0x10) */

/* item `i` of list `l`: kItemKindSlot of its kind (+0x10); -1 for an empty place */
/* 0x00260630 */
s32 Items_KindSlot(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return -1;
    }
    return kItemKindSlot[VCALL(it, 0x10, s32 (*)(VObject *))(it)];
}

/* item `id` (+0xC) in the lists: its +0x28 with `arg` */
/* 0x00260170 */
void Items_Notify(u8 *o, s32 id, s32 arg) {
    u32 l, i;

    for (l = 0; l < 3; l++) {
        for (i = 0; i < 64; i++) {
            VObject *it = AT(o, 0x12E0 + l * 0x100 + i * 4, VObject *);

            if (it == NULL) {
                break;
            }
            if (id == VCALL(it, 0xC, s32 (*)(VObject *))(it)) {
                it = AT(o, 0x12E0 + l * 0x100 + i * 4, VObject *);
                VCALL(it, 0x28, void (*)(VObject *, s32))(it, arg);
                return;
            }
        }
    }
}

/* a new item 0x3F (one) set from `src` (Item3F_Set); the item, NULL if none */
/* 0x00261040 */
void *Items_NewItem3F(u8 *items, const u8 *src) {
    u8 *it = Items_Give(items, 0x3F, 1);

    if (it != NULL) {
        Item3F_Set(it, src);
    }
    return it;
}

/* item `i` of list `l` equipped: into its kind's slot (kItemKindSlot, +0x15E0) if it has one */
/* 0x00260840 */
void Items_Equip(u8 *items, u8 l, u8 i) {
    VObject **e = &AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);
    s32 k = *e == NULL ? -1 : kItemKindSlot[VCALL(*e, 0x10, s32 (*)(VObject *))(*e)];

    if (k >= 0) {
        AT(items, 0x15E0 + k * 4, VObject *) = *e;
    }
}

/* item `i` of list `l` unequipped: its kind's slot (+0x15E0) cleared if it holds it */
/* 0x002607A0 */
void Items_Unequip(u8 *items, u8 l, u8 i) {
    VObject **e = &AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);
    s32 k = *e == NULL ? -1 : kItemKindSlot[VCALL(*e, 0x10, s32 (*)(VObject *))(*e)];

    if (k >= 0 && AT(items, 0x15E0 + k * 4, VObject *) == *e) {
        AT(items, 0x15E0 + k * 4, VObject *) = NULL;
    }
}

/* item `i` of list `l`'s equipment: -1 its kind has no slot (or no item), 0 the slot is empty,
 * 1 it is the one equipped, 2 another is */
/* 0x002606E0 */
s32 Items_EquipState(u8 *items, u8 l, u8 i) {
    VObject **e = &AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);
    s32 k = *e == NULL ? -1 : kItemKindSlot[VCALL(*e, 0x10, s32 (*)(VObject *))(*e)];
    VObject *eq;

    if (k < 0) {
        return -1;
    }
    eq = AT(items, 0x15E0 + k * 4, VObject *);
    if (eq == NULL) {
        return 0;
    }
    return eq == *e ? 1 : 2;
}

extern void *PoolEntry_vtable[];   /* a pool entry */

/* item `it` taken out: off any equipment slot, out of its list (the rest moved up), destroyed
 * back to a bare pool entry and returned to the pool (+0x1208 vtable +0x14) */
/* 0x002608D0 */
void Items_Remove(u8 *items, VObject *it) {
    u32 g, i;
    s32 found;

    if (it == NULL) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (AT(items, 0x15E0 + i * 4, VObject *) == it) {
            AT(items, 0x15E0 + i * 4, VObject *) = NULL;
        }
    }
    for (g = 0; g < 3; g++) {
        found = 0;
        for (i = 0; i < 0x40; i++) {
            if (AT(items, 0x12E0 + g * 0x100 + i * 4, VObject *) == it) {
                found = 1;
                break;
            }
        }
        if (found) {
            break;
        }
    }
    if (i < 0x40) {
        u8 *grp = items + g * 0x100;

        for (; i < 0x3F; i++) {
            VObject *next = AT(grp, 0x12E0 + (i + 1) * 4, VObject *);

            if (next == NULL) {
                break;
            }
            AT(grp, 0x12E0 + i * 4, VObject *) = next;
        }
        AT(grp, 0x12E0 + i * 4, VObject *) = NULL;
    }
    VCALL(it, 0x8, void (*)(VObject *, s32))(it, 1);
    {
        u8 *b = SubPool_new(0x18, it);

        if (b != NULL) {
            AT(b, 0x0, void **) = PoolEntry_vtable;
            AT(b, 0x4, s32) = -1;
            AT(b, 0x8, u8) = 0;
            AT(b, 0x10, u64) = 0;
        }
    }
    VCALL(items + 0x1208, 0x14, void (*)(void *, VObject *))(items + 0x1208, it);
}

extern void Items_Remove(u8 *items, VObject *it);   /* an item taken out of the lists */

/* one of item `it` used: a counted one (+0x18 1) with 2 or more (+0x34) loses one (+0x30), else
 * it goes (Items_Remove) */
static inline void item_use_one(u8 *items, VObject *it) {
    if ((u8)VCALL(it, 0x18, s32 (*)(VObject *))(it) == 1 && VCALL(it, 0x34, u32 (*)(VObject *))(it) >= 2) {
        VCALL(it, 0x30, void (*)(VObject *))(it);
    } else {
        Items_Remove(items, it);
    }
}

/* the item in equipment slot `slot` used once */
/* 0x00260A60 */
void Items_UseEquipped(u8 *items, u8 slot) {
    VObject *it = AT(items, 0x15E0 + slot * 4, VObject *);

    if (it != NULL) {
        item_use_one(items, it);
    }
}

/* (out of line, `l` / `i` kept in their registers as the original leaves them for difftest) */
static __attribute__((noinline)) void item_use_one_at(u8 *items, u32 l, u32 i, VObject *it) {
    (void)l;
    (void)i;
    item_use_one(items, it);
}

/* item `i` of list `l` used once */
/* 0x00260B00 */
void Items_UseOne(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it != NULL) {
        item_use_one_at(items, l, i, it);
    }
}

/* item `i` of list `l` used (vtable +0x3C, its result returned); when that gives bit 0 or 1,
 * one of it is spent */
/* 0x00260DD0 */
u32 Items_Use(u8 *items, u8 l, u8 i) {
    VObject **e = &AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);
    u32 r = 0;

    if (*e != NULL) {
        r = VCALL(*e, 0x3C, u32 (*)(VObject *))(*e) & 0xFF;
        if ((r & 3) && *e != NULL) {
            item_use_one(items, *e);
        }
    }
    return r;
}

/* list `l` sorted by vtable +0x1C (a bubble sort up to its first empty place) */
/* 0x0025FF80 */
void Items_Sort(u8 *items, u8 l) {
    u8 *grp = items + l * 0x100;
    u32 n, pass, j;

    for (n = 0; n < 0x40; n++) {
        if (AT(grp, 0x12E0 + n * 4, VObject *) == NULL) {
            break;
        }
    }
    if (n == 0) {
        return;
    }
    for (pass = 0; pass < n - 1; pass++) {
        u32 lim = n - 1 - pass;

        for (j = 0; j < lim; j++) {
            VObject **s = &AT(grp, 0x12E0 + j * 4, VObject *);
            u32 a = VCALL(s[0], 0x1C, u32 (*)(VObject *))(s[0]);
            u32 b = VCALL(s[1], 0x1C, u32 (*)(VObject *))(s[1]);

            if (b < a) {
                VObject *t = s[0];

                s[0] = s[1];
                s[1] = t;
            }
        }
    }
}

/* item `i` of list `l` started being used (vtable +0x28, with `arg`) */
/* 0x00260250 */
void Items_StartUse(u8 *items, u8 l, u8 i, void *arg) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it != NULL) {
        VCALL(it, 0x28, void (*)(VObject *, void *))(it, arg);
    }
}

/* item `i` of list `l`: its kind (vtable +0x10); -1 for an empty place */
/* 0x00260420 */
s32 Items_Kind(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return -1;
    }
    return VCALL(it, 0x10, s32 (*)(VObject *))(it);
}

/* item `i` of list `l`: what can be done with it (vtable +0x14: 1 use, 2 equip, 4 examine;
 * 0x80000000 its note can change); 0 for an empty place */
/* 0x002603C0 */
u32 Items_Actions(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return 0;
    }
    return VCALL(it, 0x14, u32 (*)(VObject *))(it);
}

/* item `i` of list `l`: its id (vtable +0xC); -1 for an empty place */
/* 0x00260480 */
s32 Items_Id(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return -1;
    }
    return VCALL(it, 0xC, s32 (*)(VObject *))(it);
}

/* item `i` of list `l`: how many (vtable +0x34; 0 for an empty place) */
/* 0x00260300 */
s32 Items_HowMany(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return 0;
    }
    return VCALL(it, 0x34, s32 (*)(VObject *))(it);
}

/* item `i` of list `l`: is it counted (vtable +0x18; 0 for an empty place) */
/* 0x00260360 */
u8 Items_IsCounted(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return 0;
    }
    return VCALL(it, 0x18, s32 (*)(VObject *))(it);
}

/* item `i` of list `l`: its 8 bytes of data (vtable +0x24; NULL for an empty place) */
/* 0x002602A0 */
u64 *Items_Data(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return NULL;
    }
    return VCALL(it, 0x24, u64 *(*)(VObject *))(it);
}

/* how many items 0x3F the lists hold */
/* 0x00260540 */
s32 Items_CountItem3F(u8 *items) {
    u32 l, i;
    s32 n = 0;

    for (l = 0; l < 3; l++) {
        for (i = 0; i < 64; i++) {
            VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

            if (it == NULL) {
                break;
            }
            if (VCALL(it, 0xC, s32 (*)(VObject *))(it) == 0x3F) {
                n++;
            }
        }
    }
    return n;
}

/* the first free (0) of the 64 words in row `row` (0x100 bytes from +0x12E0); 64 if none */
/* 0x002605F0 */
u32 Items_FirstFree(u8 *o, u8 row) {
    u32 i;

    for (i = 0; i < 0x40; i++) {
        if (AT(o, 0x12E0 + row * 0x100 + i * 4, s32) == 0) {
            break;
        }
    }
    return i;
}
