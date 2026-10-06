/* The item classes (pool entries, vtable PoolEntry_vtable): their destructors. Each item has its own
 * class (+0x28 loads its picture, +0x3C uses it) on one of a few middle classes (PlainItem_vtable
 * the plain item, ItemClassCB50_vtable ..., see the vtables); the destructors only step the vtable back
 * down the chain and free the entry. */
#include "common.h"
#include "game.h"
#include "item.h"
#include "globals.h"
#include "progress.h"
#include "actor.h"
#include "ptmf.h"
#include "pursuer.h"
#include "hewie.h"
#include "item_classes.h"
#include "items.h"
#include "model.h"
#include "overlay.h"
#include "pursuer_ai.h"
#include "snd_place.h"
#include "stalker_math.h"
#include "stalker_progress.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif
#include "msl.h"
#include "sce/libvu0.h"

s32 PoolEntry_PartnerCheck(void *o);

extern void *PoolEntry_vtable[];            /* a pool entry */
extern void *Item12_vtable[], *Item13_vtable[], *Item3E_vtable[], *PlainItem_vtable[], *ItemClassC920_vtable[], *Item70_vtable[], *Item71_vtable[], *Item72_vtable[], *Item73_vtable[], *Item74_vtable[], *Item75_vtable[], *ItemClassCB50_vtable[], *Item80_vtable[], *Item81_vtable[], *Item82_vtable[], *Item83_vtable[], *ItemClassCCE0_vtable[], *Item86_vtable[], *Item87_vtable[], *Item88_vtable[], *Item89_vtable[], *ItemClassCE70_vtable[], *Item8A_vtable[], *Item8B_vtable[], *Item8C_vtable[], *Item8D_vtable[], *ItemClassD000_vtable[], *Item90_vtable[], *Item91_vtable[], *Item92_vtable[], *Item93_vtable[], *Item94_vtable[], *Item95_vtable[], *Item97_vtable[], *Item98_vtable[], *Item9B_vtable[], *ItemClassD320_vtable[], *ItemA4_vtable[], *ItemA5_vtable[], *ItemA6_vtable[], *ItemA7_vtable[], *ItemA8_vtable[], *ItemA9_vtable[], *ItemAA_vtable[], *ItemAB_vtable[], *ItemAC_vtable[], *ItemClassD640_vtable[], *Item40_vtable[], *Item41_vtable[], *Item00_vtable[], *Item01_vtable[], *Item02_vtable[], *Item03_vtable[], *Item04_vtable[], *Item05_vtable[], *ItemClassF430_vtable[], *ItemA0_vtable[], *ItemA1_vtable[], *Item06_vtable[], *Item07_vtable[], *Item08_vtable[], *Item09_vtable[], *Item0A_vtable[], *Item0B_vtable[], *Item0C_vtable[], *Item0D_vtable[], *Item0E_vtable[], *Item0F_vtable[], *Item10_vtable[], *Item11_vtable[], *Item14_vtable[], *Item42_vtable[], *Item43_vtable[], *Item44_vtable[], *Item45_vtable[], *Item46_vtable[], *Item47_vtable[], *Item48_vtable[], *Item49_vtable[], *Item4A_vtable[], *Item4B_vtable[], *Item4C_vtable[], *Item60_vtable[], *Item61_vtable[], *Item62_vtable[], *Item63_vtable[], *Item64_vtable[], *Item65_vtable[], *Item66_vtable[], *Item15_vtable[], *Item16_vtable[], *Item17_vtable[], *ItemA2_vtable[], *ItemClass6B00_vtable[], *ItemA3_vtable[], *Item18_vtable[], *Item19_vtable[], *Item1A_vtable[], *Item1B_vtable[], *Item1C_vtable[], *Item1D_vtable[], *Item1E_vtable[], *Item1F_vtable[], *Item20_vtable[], *Item21_vtable[], *Item22_vtable[], *Item23_vtable[], *Item24_vtable[], *Item25_vtable[], *Item26_vtable[], *Item27_vtable[], *Item28_vtable[], *Item29_vtable[];

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
extern char str_ITEM01_ITEM_02E_TEX[]; /* file name */
extern char str_ITEM01_ITEM_02D_TEX[]; /* file name */
typedef s32 (*LoaderLoadFn)(void *loader, const char *name, void *dest, s32 flags, s32 arg);

s32 func_00264C10(void *self, void *dest);
s32 func_00264E90(void *self, void *dest);
s32 Item70_LoadPicture(void *self, void *dest);
s32 Item71_LoadPicture(void *self, void *dest);
s32 Item72_LoadPicture(void *self, void *dest);
s32 Item73_LoadPicture(void *self, void *dest);
s32 Item74_LoadPicture(void *self, void *dest);
s32 Item75_LoadPicture(void *self, void *dest);
s32 Item80_LoadPicture(void *self, void *dest);
s32 Item81_LoadPicture(void *self, void *dest);
s32 Item82_LoadPicture(void *self, void *dest);
s32 Item83_LoadPicture(void *self, void *dest);
s32 Item86_LoadPicture(void *self, void *dest);
s32 Item87_LoadPicture(void *self, void *dest);
s32 Item88_LoadPicture(void *self, void *dest);
s32 Item89_LoadPicture(void *self, void *dest);
s32 Item8A_LoadPicture(void *self, void *dest);
s32 Item8B_LoadPicture(void *self, void *dest);
s32 Item8C_LoadPicture(void *self, void *dest);
s32 Item8D_LoadPicture(void *self, void *dest);

extern u8 D_0045D4A0[], str_ITEM00_ITEM_001_TEX[], str_ITEM00_ITEM_002_TEX[], str_ITEM00_ITEM_003_TEX[], str_ITEM00_ITEM_004_TEX[], str_ITEM00_ITEM_005_TEX[];
extern u8 D_00413550[];
#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

/* gFileLoader->vfunc_0xC(name, arg, 0x4000000, 0) */
#define LOAD(name, arg) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, void *, s32, s32, s32))(gFileLoader, name, arg, 0x4000000, 0)

s32 Item00_LoadPicture(void *self, s32 a);
s32 Item01_LoadPicture(void *self, s32 a);
s32 Item01_Use(void);
s32 Item01_CountUp(void *o);
s32 Item02_LoadPicture(void *self, s32 a);
s32 Item02_Use(void *o);
s32 Item03_LoadPicture(void *self, s32 a);
s32 Item03_Use(void);
s32 Item04_LoadPicture(void *self, s32 a);
s32 Item04_Use(void);
s32 Item05_LoadPicture(void *self, s32 a);
s32 Item05_Use(void);
void *Debilitas3_MotionFiles(void);
void Debilitas3_DoorOffset(void *self, s32 id, u32 *out);
void Debilitas3_ActionOffsets(void *self, s32 id, u32 *out);
void Debilitas3_ExitDone(u8 *p);

extern void *Pursuer_vtable[], *NPC_vtable[], *Character_vtable[], *Actor_vtable[];
extern const char *const pstr_O_DNL_DNL_202_TEX;
extern void *Debilitas3_vtable[];
extern void *Kind18_vtable[];
extern const PTMF Pursuer_StateRunThenNext_ptmf12;
extern void *Kind26_vtable[];
extern const PTMF Pursuer_StateRunThenNext_ptmf18;
extern void *Kind28_vtable[];
extern const PTMF Pursuer_StateRunThenNext_ptmf19;
extern void *Kind30_vtable[];
extern const PTMF Pursuer_StateRunThenNext_ptmf21;
extern void *Kind31_vtable[];
extern const PTMF Pursuer_StateRunThenNext_ptmf22;
extern void *Kind38_vtable[];
extern const PTMF Pursuer_StateRunThenNext_ptmf25;
void Debilitas3_FollowPathExit(void);
void Debilitas3_Arrived(void);
void Debilitas3_WalkToExit(void);
void Debilitas3_OnToNextExit(void);
s32 Debilitas3_PickDestination(void);

extern char str_ITEM00_ITEM_008_TEX[];
extern char str_ITEM01_ITEM_02F_TEX[];
extern char str_ITEM01_ITEM_030_TEX[];
/* gFileLoader->vfunc_0xC(name, dest, 0x4000000, 0): start loading a file */
#define LOAD_002D1360(name, dest) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, const char *, void *, s32, s32))(gFileLoader, name, dest, 0x4000000, 0)

s32 func_002D3A20(void *self, void *dest);
s32 ItemA0_LoadPicture(void *self, void *dest);
s32 ItemA1_LoadPicture(void *self, void *dest);

extern u8 str_ITEM02_ITEM_03F_TEX[];
extern u8 str_ITEM02_ITEM_040_TEX[];
extern u8 str_ITEM02_ITEM_041_TEX[];
extern u8 str_ITEM02_ITEM_042_TEX[];
extern u8 str_ITEM02_ITEM_043_TEX[];
/* gFileLoader vtable +0xC: start loading file `name` into `dest` (flags 0x4000000) */
#define FILE_LOAD_ASYNC(name, dest) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, void *, void *, u32, s32))(gFileLoader, name, dest, 0x4000000, 0)

s32 Item08_LoadPicture(void *self, void *dest);
s32 Item09_LoadPicture(void *self, void *dest);
s32 Item0A_LoadPicture(void *self, void *dest);
s32 Item0B_LoadPicture(void *self, void *dest);
s32 Item0C_LoadPicture(void *self, void *dest);

extern u8 pstr_O_FIW_FIW_200_PCK[];
extern u8 D_00429DB0[];
extern u8 str_ITEM02_ITEM_039_TEX[];
extern u8 str_ITEM02_ITEM_031_TEX[];
extern u8 str_ITEM02_ITEM_03A_TEX[];
extern u8 str_ITEM02_ITEM_046_TEX[];
extern u8 str_ITEM02_ITEM_03B_TEX[];
s32 Item0D_LoadPicture(void *self, void *dest);
s32 Item0E_LoadPicture(void *self, void *dest);
s32 Item0F_LoadPicture(void *self, void *dest);
s32 Item10_LoadPicture(void *self, void *dest);
s32 Item11_LoadPicture(void *self, void *dest);
void *Kind18_ModelFiles(void);
void *Kind18_MotionFiles(void);

extern u8 D_0042C990[];
extern u8 D_0042C9D0[];
extern u8 D_0042CA20[];
extern u8 D_0042CB60[];
extern u8 D_0042CC00[];
extern u8 D_0042D0C0[];
extern void *D_0042E310[];
extern void *pstr_EV0023[];
extern u8 D_0042E410[];
extern u8 D_0042E4C0[];
extern u8 str_ITEM01_ITEM_02C_TEX[];
extern u8 D_01991600[];
/* Field access by byte offset into objects whose layout is not yet known. */
#define S16(p, off) (*(s16 *)((u8 *)(p) + (off)))

#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

#define S64(p, off) (*(s64 *)((u8 *)(p) + (off)))

#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

#define PTR(p, off) (*(void * *)((u8 *)(p) + (off)))

#define CHAR_POS(c) ((f32 *)((u8 *)(c) + 0x10))

#define CHAR_ON(c) (*((u8 *)(c) + 0x28))

#define CHAR_ROOM(c) S32(c, 0x30)

#define CUR_ROOM() VCALL(gProgress, 0xC, s32 (*)(void *))(gProgress)

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
extern u8 D_0042F470[];
extern u8 D_0042F4B0[];
extern u8 D_004308B0[];
extern u8 D_004308F0[];
extern u8 D_00430940[];
extern u8 D_00430980[];
extern u8 str_ITEM02_ITEM_03E_TEX[];
extern u8 str_ITEM02_ITEM_03D_TEX[];
extern u8 str_ITEM02_ITEM_044_TEX[];
extern u8 str_ITEM02_ITEM_045_TEX[];
extern u8 str_ITEM02_ITEM_033_TEX[];
extern u8 str_ITEM02_ITEM_034_TEX[];
extern u8 str_ITEM02_ITEM_035_TEX[];
extern u8 str_ITEM02_ITEM_036_TEX[];
extern u8 str_ITEM02_ITEM_037_TEX[];
s32 Item16_LoadPicture(void *self, void *dest);
s32 Item17_LoadPicture(void *self, void *dest);
void *func_00339B20(void);
void *func_00339B30(void);
s32 ItemA2_LoadPicture(void *self, void *dest);
void *Kind30_ModelFiles(void);
void *Kind30_MotionFiles(void);
void *Kind31_ModelFiles(void);
void *Kind31_MotionFiles(void);
s32 ItemClass6B00_PartnerCheck(void *o);
s32 ItemA3_LoadPicture(void *self, void *dest);
s32 Item18_LoadPicture(void *self, void *dest);
s32 Item19_LoadPicture(void *self, void *dest);
s32 Item1A_LoadPicture(void *self, void *dest);
s32 Item1B_LoadPicture(void *self, void *dest);
s32 Item1C_LoadPicture(void *self, void *dest);

extern u8 str_ITEM02_ITEM_038_TEX[];
s32 Item1D_LoadPicture(void *self, void *dest);

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
s32 Item1E_LoadPicture(void *self, void *dest);
s32 Item1E_Use(void);
s32 Item1F_LoadPicture(void *self, void *dest);
s32 Item1F_Use(void);
s32 Item20_LoadPicture(void *self, void *dest);
s32 Item20_Use(void);
s32 Item21_LoadPicture(void *self, void *dest);
s32 Item22_LoadPicture(void *self, void *dest);
s32 Item23_LoadPicture(void *self, void *dest);
s32 Item24_LoadPicture(void *self, void *dest);
s32 Item25_LoadPicture(void *self, void *dest);
s32 Item26_LoadPicture(void *self, void *dest);
s32 Item27_LoadPicture(void *self, void *dest);
s32 Item27_Use(void);

extern char str_ITEM03_ITEM_052_TEX[];
extern u8 D_004434D0[], D_00443510[];
s32 Item28_LoadPicture(void *self, void *dest);
void *func_00352030(void);
void *func_00352040(void);

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

u32 Item49_Actions(void);
s32 Item49_LoadPicture(void *self, void *dest);
u32 Item4A_Actions(void);
s32 Item4A_LoadPicture(void *self, void *dest);
u32 Item4B_Actions(void);
s32 Item4B_LoadPicture(void *self, void *dest);
s32 Item4B_Use(void);
u32 Item4C_Actions(void);
s32 Item4C_LoadPicture(void *self, void *dest);
s32 Item4C_Use(void);
s32 Item60_LoadPicture(void *self, void *dest);
s32 Item61_LoadPicture(void *self, void *dest);
s32 Item62_LoadPicture(void *self, void *dest);
u32 Item63_Actions(void);
s32 Item63_LoadPicture(void *self, void *dest);
u32 Item64_Actions(void);
s32 Item64_LoadPicture(void *self, void *dest);
u32 Item65_Actions(void);
s32 Item65_LoadPicture(void *self, void *dest);
u32 Item66_Actions(void);
s32 Item66_LoadPicture(void *self, void *dest);
s32 Item15_LoadPicture(void *self, void *dest);

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

u32 Item42_Actions(void);
s32 Item42_LoadPicture(void *self, void *dest);
s32 Item42_Use(void);
u32 Item43_Actions(void);
s32 Item43_LoadPicture(void *self, void *dest);
s32 Item43_Use(void);
u32 Item44_Actions(void);
s32 Item44_LoadPicture(void *self, void *dest);
s32 Item44_Use(void);
u32 Item45_Actions(void);
s32 Item45_LoadPicture(void *self, void *dest);
s32 Item45_Use(void);
u32 Item46_Actions(void);
s32 Item46_LoadPicture(void *self, void *dest);
s32 Item46_Use(void);
u32 Item47_Actions(void);
s32 Item47_LoadPicture(void *self, void *dest);
s32 Item47_Use(void);
u32 Item48_Actions(void);
s32 Item48_LoadPicture(void *self, void *dest);
s32 Item48_Use(void);
s32 Item49_Use(void);
s32 Item4A_Use(void);
s32 Item60_Use(void *o);
s32 Item61_Use(void *o);
s32 Item62_Use(void *o);
s32 Item63_Use(void *o);
s32 Item64_Use(void *o);
s32 Item65_Use(void *o);
s32 Item66_Use(void *o);

static f32 animal_rnd(void) {
    return VCALL(gRandom, 0x20, f32 (*)(void *))(gRandom);
}

s32 Item14_LoadPicture(void *self, void *dest);
void *Kind26_ModelFiles(void);
void *Kind26_MotionFiles(void);
f32 Kind26_NearestDistSq(u8 *self, f32 *out);

/* destructor: own vtable -> Pursuer 0x46D810 -> NPC 0x46C220 -> Character; the model freed for
 * slots 3..5 */
static inline __attribute__((always_inline)) Character *creature_dtor(Character *c, s32 flags, void **vt) {
    if (c != NULL) {
        c->a.vtbl = vt;
        c->a.vtbl = Pursuer_vtable;
        VCALL(c, 0x10, void (*)(Character *))(c);
        if ((u32)c->a.slot >= 3 && (u32)c->a.slot < 6) {
            void **m = c->motion;

            if (m != NULL) {
                VCALL(m, 0x8, void (*)(void *, s32))(m, 1);
                c->motion = NULL;
            }
        }
        c->a.vtbl = NPC_vtable;
        VCALL(c, 0x10, void (*)(Character *))(c);
        c->a.vtbl = Character_vtable;
        c->a.vtbl = Actor_vtable;
        if ((s16)flags > 0) {
            Actor_Destroy(&c->a);
        }
    }
    return c;
}

/* in play: Actor_TeleportRandom(-1) */
static inline __attribute__((always_inline)) void creature_inplay(Pursuer *p) {
    if (Npc_InPlayedRoom(p) != 0) {
        Actor_TeleportRandom(&p->c.a, -1);
    }
}

/* the action 5 taken (+0x14E8): in play +0x8C, the state st, +0x114 1; the action cleared */
static inline __attribute__((always_inline)) void creature_act5(Pursuer *p, const PTMF *st) {
    if (PU(p, 0x14E8, s32) != 5) {
        return;
    }
    if ((u8)Npc_InPlayedRoom(p) != 0) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set(&PU(p, 0x174C, PTMF), st);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    PU(p, 0x14E8, s32) = 0;
    PU(p, 0x14EC, s32) = 0;
}

/* its slot's progress entry (Progress_HasRelationCmd) 1: SlotCmd_Cancel; -1 */
static inline __attribute__((always_inline)) s32 creature_slot_done(Pursuer *p) {
    Progress *g = gProgress;

    if ((u8)Progress_HasRelationCmd(g, *(u8 *)&p->c.a.slot) == 1) {
        SlotCmd_Cancel(g, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

Character *Debilitas3_dtor(Character *c, s32 flags);
Character *Kind18_dtor(Character *c, s32 flags);
void Kind18_ShowUp(Pursuer *p);
void Kind18_EventState(Pursuer *p);
s32 Kind18_GrabOrder(Pursuer *p);
Character *Kind26_dtor(Character *c, s32 flags);
void func_0032DA60(Pursuer *p);
void func_0032DAB0(Pursuer *p);
s32 func_0032DB80(Pursuer *p);
Character *func_00339A10(Character *c, s32 flags);
void func_00339B50(Pursuer *p);
void func_00339BA0(Pursuer *p);
s32 func_00339C70(Pursuer *p);
Character *Kind30_dtor(Character *c, s32 flags);
void Kind30_ShowUp(Pursuer *p);
void Kind30_EventState(Pursuer *p);
s32 Kind30_GrabOrder(Pursuer *p);
Character *Kind31_dtor(Character *c, s32 flags);
void Kind31_ShowUp(Pursuer *p);
void Kind31_EventState(Pursuer *p);
s32 Kind31_GrabOrder(Pursuer *p);
Character *func_00351F20(Character *c, s32 flags);
void func_00352060(Pursuer *p);
void func_003520B0(Pursuer *p);
s32 func_00352180(Pursuer *p);

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

/* 0x00264090 */
ItemObj *Item3E_ctor(ItemObj *self) {
    self->vtbl = PoolEntry_vtable;
    self->id = 0x3E;
    self->flag = 0;
    self->data = 0;
    self->vtbl = Item3E_vtable;
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
/* 0x002D2750 */
void *ItemClassF430_dtor(void *o, s32 flags) { return item_dtor2(o, flags, ItemClassF430_vtable); }
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

/* the items */
/* 0x002649F0 */
void *Item12_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item12_vtable, PlainItem_vtable); }
/* 0x00264C70 */
void *Item13_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item13_vtable, PlainItem_vtable); }
/* 0x00264EC0 */
void *Item3E_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item3E_vtable, PlainItem_vtable); }
/* 0x00264F70 */
void *Item70_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item70_vtable, ItemClassCB50_vtable); }

/* 0x00264FE0 */
s32 Item70_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_01D_TEX, dest, 0x4000000, 0);
}
/* 0x00265060 */
void *Item71_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item71_vtable, ItemClassCB50_vtable); }

/* 0x002650D0 */
s32 Item71_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_01E_TEX, dest, 0x4000000, 0);
}
/* 0x00265110 */
void *Item72_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item72_vtable, ItemClassCB50_vtable); }

/* 0x00265180 */
s32 Item72_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_01F_TEX, dest, 0x4000000, 0);
}
/* 0x002651C0 */
void *Item73_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item73_vtable, ItemClassCB50_vtable); }

/* 0x00265230 */
s32 Item73_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_020_TEX, dest, 0x4000000, 0);
}
/* 0x00265270 */
void *Item74_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item74_vtable, ItemClassCB50_vtable); }

/* 0x002652E0 */
s32 Item74_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_021_TEX, dest, 0x4000000, 0);
}
/* 0x00265320 */
void *Item75_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item75_vtable, ItemClassCB50_vtable); }

/* 0x00265390 */
s32 Item75_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_022_TEX, dest, 0x4000000, 0);
}
/* 0x002653D0 */
void *Item80_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item80_vtable, ItemClassCCE0_vtable); }

/* 0x00265440 */
s32 Item80_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_056_TEX, dest, 0x4000000, 0);
}
/* 0x002654B0 */
void *Item81_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item81_vtable, ItemClassCCE0_vtable); }

/* 0x00265520 */
s32 Item81_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_057_TEX, dest, 0x4000000, 0);
}
/* 0x00265560 */
void *Item82_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item82_vtable, ItemClassCCE0_vtable); }

/* 0x002655D0 */
s32 Item82_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_058_TEX, dest, 0x4000000, 0);
}
/* 0x00265610 */
void *Item83_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item83_vtable, ItemClassCCE0_vtable); }

/* 0x00265680 */
s32 Item83_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_059_TEX, dest, 0x4000000, 0);
}
/* 0x002656C0 */
void *Item86_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item86_vtable, ItemClassCE70_vtable); }

/* 0x00265730 */
s32 Item86_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_05A_TEX, dest, 0x4000000, 0);
}
/* 0x002657A0 */
void *Item87_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item87_vtable, ItemClassCE70_vtable); }

/* 0x00265810 */
s32 Item87_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_05B_TEX, dest, 0x4000000, 0);
}
/* 0x00265850 */
void *Item88_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item88_vtable, ItemClassCE70_vtable); }

/* 0x002658C0 */
s32 Item88_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_05C_TEX, dest, 0x4000000, 0);
}
/* 0x00265900 */
void *Item89_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item89_vtable, ItemClassCE70_vtable); }

/* 0x00265970 */
s32 Item89_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_05D_TEX, dest, 0x4000000, 0);
}
/* 0x002659B0 */
void *Item8A_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item8A_vtable, ItemClassD000_vtable); }

/* 0x00265A20 */
s32 Item8A_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_05E_TEX, dest, 0x4000000, 0);
}
/* 0x00265A90 */
void *Item8B_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item8B_vtable, ItemClassD000_vtable); }

/* 0x00265B00 */
s32 Item8B_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM03_ITEM_05F_TEX, dest, 0x4000000, 0);
}
/* 0x00265B40 */
void *Item8C_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item8C_vtable, ItemClassD000_vtable); }

/* 0x00265BB0 */
s32 Item8C_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM04_ITEM_060_TEX, dest, 0x4000000, 0);
}
/* 0x00265BF0 */
void *Item8D_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item8D_vtable, ItemClassD000_vtable); }

/* 0x00265C60 */
s32 Item8D_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM04_ITEM_061_TEX, dest, 0x4000000, 0);
}
/* 0x00265CA0 */
void *Item90_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item90_vtable, ItemClassD320_vtable); }
/* 0x00265D80 */
void *Item91_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item91_vtable, ItemClassD320_vtable); }
/* 0x00265E30 */
void *Item92_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item92_vtable, ItemClassD320_vtable); }
/* 0x00265EE0 */
void *Item93_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item93_vtable, ItemClassD320_vtable); }
/* 0x00265F90 */
void *Item94_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item94_vtable, ItemClassD320_vtable); }
/* 0x00266040 */
void *Item95_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item95_vtable, ItemClassD320_vtable); }
/* 0x002660F0 */
void *Item97_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item97_vtable, ItemClassD320_vtable); }
/* 0x002661A0 */
void *Item98_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item98_vtable, ItemClassD320_vtable); }
/* 0x00266250 */
void *Item9B_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item9B_vtable, ItemClassD320_vtable); }
/* 0x00266300 */
void *ItemA4_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA4_vtable, ItemClassD640_vtable); }
/* 0x00266450 */
void *ItemA5_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA5_vtable, ItemClassD640_vtable); }
/* 0x00266500 */
void *ItemA6_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA6_vtable, ItemClassD640_vtable); }
/* 0x002665B0 */
void *ItemA7_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA7_vtable, ItemClassD640_vtable); }
/* 0x00266660 */
void *ItemA8_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA8_vtable, ItemClassD640_vtable); }
/* 0x00266710 */
void *ItemA9_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA9_vtable, ItemClassD640_vtable); }
/* 0x002667C0 */
void *ItemAA_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemAA_vtable, ItemClassD640_vtable); }
/* 0x00266870 */
void *ItemAB_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemAB_vtable, ItemClassD640_vtable); }
/* 0x00266920 */
void *ItemAC_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemAC_vtable, ItemClassD640_vtable); }
/* 0x002669D0 */
void *Item40_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item40_vtable, ItemClassC920_vtable); }
/* 0x00266B60 */
void *Item41_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item41_vtable, ItemClassC920_vtable); }
/* 0x002CCBD0 */
void *Item00_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item00_vtable, PlainItem_vtable); }

/* 0x002CCC40 */
s32 Item00_LoadPicture(void *self, s32 a) { return LOAD(D_0045D4A0, a); }
/* 0x002CCD30 */
void *Item01_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item01_vtable, PlainItem_vtable); }
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

/* 0x002CD4E0 */
Character *Debilitas3_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Debilitas3_vtable); }

/* 0x002CD5F0 */
void *Debilitas3_MotionFiles(void) { return D_00413550; }

/* 0x002CD600 */
void Debilitas3_DoorOffset(void *self, s32 id, u32 *out) {
    u32 z;

    switch (id) {
    case 1: z = 0x41266666; break;
    case 3: z = 0xC0EE17C2; break;
    case 0: z = 0xC0C6C49C; break;
    case 2: z = 0x40C93B64; break;
    default: return;
    }
    out[0] = 0;
    out[1] = 0;
    out[2] = z;
}

/* 0x002CD6A0 */
void Debilitas3_ActionOffsets(void *self, s32 id, u32 *out) {
    u32 x, z;

    switch (id) {
    case 10: case 11: x = 0x3F717C1C; z = 0x413403B0; break;
    case 12: case 13: x = 0x400B2B02; z = 0x4170AB9F; break;
    case 14: x = 0xBFCE4C30; z = 0xC08AF007; break;
    case 15: x = 0x3C95182B; z = 0xBF801A37; break;
    default: return;
    }
    out[0] = x;
    out[1] = 0;
    out[2] = z;
}

/* 0x002CD750 */
void Debilitas3_ExitDone(u8 *p) { p[0x16EE] = 1; }

/* 0x002CD760 */
void Debilitas3_FollowPathExit(void) {
}

/* 0x002CD770 */
void Debilitas3_Arrived(void) {
}

/* 0x002CD780 */
void Debilitas3_WalkToExit(void) {
}

/* 0x002CD790 */
void Debilitas3_OnToNextExit(void) {
}

/* 0x002CD7A0 */
s32 Debilitas3_PickDestination(void) {
    return 0;
}
/* 0x002D7910 */
void *ItemA0_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA0_vtable, ItemClassD640_vtable); }

/* 0x002D7980 */
s32 ItemA0_LoadPicture(void *self, void *dest) { return LOAD_002D1360(str_ITEM01_ITEM_02F_TEX, dest); }
/* 0x002D79C0 */
void *ItemA1_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA1_vtable, ItemClassD640_vtable); }

/* 0x002D7A30 */
s32 ItemA1_LoadPicture(void *self, void *dest) { return LOAD_002D1360(str_ITEM01_ITEM_030_TEX, dest); }
/* 0x002EEB60 */
void *Item06_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item06_vtable, PlainItem_vtable); }
/* 0x00303C30 */
void *Item07_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item07_vtable, PlainItem_vtable); }
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
/* 0x0032CC90 */
void *Item14_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item14_vtable, PlainItem_vtable); }

/* 0x0032CD00 */
s32 Item14_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM01_ITEM_02C_TEX, dest);
}
/* 0x00331450 */
void *Item42_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item42_vtable, ItemClassC920_vtable); }

/* 0x003314C0 */
u32 Item42_Actions(void) {
    return 0x80000005;
}
/* 0x003315F0 */
void *Item43_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item43_vtable, ItemClassC920_vtable); }

/* 0x00331660 */
u32 Item43_Actions(void) {
    return 0x80000005;
}
/* 0x00331790 */
void *Item44_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item44_vtable, ItemClassC920_vtable); }

/* 0x00331800 */
u32 Item44_Actions(void) {
    return 0x80000005;
}
/* 0x003318B0 */
void *Item45_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item45_vtable, ItemClassC920_vtable); }

/* 0x00331920 */
u32 Item45_Actions(void) {
    return 0x80000005;
}
/* 0x003319D0 */
void *Item46_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item46_vtable, ItemClassC920_vtable); }

/* 0x00331A40 */
u32 Item46_Actions(void) {
    return 0x80000005;
}
/* 0x00331B60 */
void *Item47_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item47_vtable, ItemClassC920_vtable); }

/* 0x00331BD0 */
u32 Item47_Actions(void) {
    return 0x80000005;
}
/* 0x00331CE0 */
void *Item48_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item48_vtable, ItemClassC920_vtable); }

/* 0x00331D50 */
u32 Item48_Actions(void) {
    return 0x80000005;
}
/* 0x00331E20 */
void *Item49_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item49_vtable, ItemClassC920_vtable); }

/* 0x00331E90 */
u32 Item49_Actions(void) {
    return 0x80000005;
}
/* 0x00331F70 */
void *Item4A_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item4A_vtable, ItemClassC920_vtable); }

/* 0x00331FE0 */
u32 Item4A_Actions(void) {
    return 0x80000005;
}
/* 0x00332120 */
void *Item4B_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item4B_vtable, ItemClassC920_vtable); }

/* 0x00332190 */
u32 Item4B_Actions(void) {
    return 0x80000005;
}
/* 0x003322E0 */
void *Item4C_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item4C_vtable, ItemClassC920_vtable); }

/* 0x00332350 */
u32 Item4C_Actions(void) {
    return 0x80000005;
}
/* 0x003323C0 */
void *Item60_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item60_vtable, ItemClass6B00_vtable); }
/* 0x00332620 */
void *Item61_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item61_vtable, ItemClass6B00_vtable); }
/* 0x00332830 */
void *Item62_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item62_vtable, ItemClass6B00_vtable); }
/* 0x00332A10 */
void *Item63_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item63_vtable, ItemClass6B00_vtable); }

/* 0x00332A80 */
u32 Item63_Actions(void) {
    return 0x80000005;
}
/* 0x00332C50 */
void *Item64_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item64_vtable, ItemClass6B00_vtable); }

/* 0x00332CC0 */
u32 Item64_Actions(void) {
    return 0x80000005;
}
/* 0x00332E70 */
void *Item65_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item65_vtable, ItemClass6B00_vtable); }

/* 0x00332EE0 */
u32 Item65_Actions(void) {
    return 0x80000005;
}
/* 0x00333060 */
void *Item66_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item66_vtable, ItemClass6B00_vtable); }

/* 0x003330D0 */
u32 Item66_Actions(void) {
    return 0x80000005;
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
/* 0x0033BD50 */
void *ItemA3_dtor(void *o, s32 flags) { return item_dtor3(o, flags, ItemA3_vtable, ItemClassD640_vtable); }

/* 0x0033BDC0 */
s32 ItemA3_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_045_TEX, dest);
}
/* 0x0033E850 */
void *Item18_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item18_vtable, PlainItem_vtable); }
/* 0x0033E9C0 */
void *Item19_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item19_vtable, PlainItem_vtable); }
/* 0x0033EB30 */
void *Item1A_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item1A_vtable, PlainItem_vtable); }
/* 0x0033ECA0 */
void *Item1B_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item1B_vtable, PlainItem_vtable); }
/* 0x0033EE10 */
void *Item1C_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item1C_vtable, PlainItem_vtable); }
/* 0x00344520 */
void *Item1D_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item1D_vtable, PlainItem_vtable); }
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
/* 0x003517D0 */
void *Item24_dtor(void *o, s32 flags) { return item_dtor3(o, flags, Item24_vtable, PlainItem_vtable); }

/* 0x00351840 */
s32 Item24_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, str_ITEM03_ITEM_04E_TEX, dest, 0x4000000, 0);
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

/* +0x3C (PoolEntry_vtable, PlainItem_vtable, ItemClassC920_vtable, ...) */
/* 0x0025FEE0 */
s32 PoolEntry_Use(void *o) {
    return 0;
}

/* operator delete for pool entries: nothing (the pool is dropped at once) */
/* 0x0025FEF0 */
void SubPool_delete(void *p) {
}

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

/* +0x1C (Item71_vtable) */
/* 0x00265100 */
s32 Item71_SortKey(void *o) {
    return 0xA20;
}

/* +0x1C (Item72_vtable) */
/* 0x002651B0 */
s32 Item72_SortKey(void *o) {
    return 0xA30;
}

/* +0x1C (Item73_vtable) */
/* 0x00265260 */
s32 Item73_SortKey(void *o) {
    return 0xA40;
}

/* +0x1C (Item74_vtable) */
/* 0x00265310 */
s32 Item74_SortKey(void *o) {
    return 0xA50;
}

/* +0x1C (Item75_vtable) */
/* 0x002653C0 */
s32 Item75_SortKey(void *o) {
    return 0xA60;
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

/* +0x1C (Item81_vtable) */
/* 0x00265550 */
s32 Item81_SortKey(void *o) {
    return 0x320;
}

/* +0x1C (Item82_vtable) */
/* 0x00265600 */
s32 Item82_SortKey(void *o) {
    return 0x330;
}

/* +0x1C (Item83_vtable) */
/* 0x002656B0 */
s32 Item83_SortKey(void *o) {
    return 0x340;
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

/* +0x1C (Item87_vtable) */
/* 0x00265840 */
s32 Item87_SortKey(void *o) {
    return 0x420;
}

/* +0x1C (Item88_vtable) */
/* 0x002658F0 */
s32 Item88_SortKey(void *o) {
    return 0x430;
}

/* +0x1C (Item89_vtable) */
/* 0x002659A0 */
s32 Item89_SortKey(void *o) {
    return 0x440;
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

/* +0x1C (Item8B_vtable) */
/* 0x00265B30 */
s32 Item8B_SortKey(void *o) {
    return 0x520;
}

/* +0x1C (Item8C_vtable) */
/* 0x00265BE0 */
s32 Item8C_SortKey(void *o) {
    return 0x530;
}

/* +0x1C (Item8D_vtable) */
/* 0x00265C90 */
s32 Item8D_SortKey(void *o) {
    return 0x540;
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

/* +0x1C (Item91_vtable) */
/* 0x00265E20 */
s32 Item91_SortKey(void *o) {
    return 0x610;
}

/* +0x1C (Item92_vtable) */
/* 0x00265ED0 */
s32 Item92_SortKey(void *o) {
    return 0x710;
}

/* +0x1C (Item93_vtable) */
/* 0x00265F80 */
s32 Item93_SortKey(void *o) {
    return 0x830;
}

/* +0x1C (Item94_vtable) */
/* 0x00266030 */
s32 Item94_SortKey(void *o) {
    return 0x620;
}

/* +0x1C (Item95_vtable) */
/* 0x002660E0 */
s32 Item95_SortKey(void *o) {
    return 0x720;
}

/* +0x1C (Item97_vtable) */
/* 0x00266190 */
s32 Item97_SortKey(void *o) {
    return 0x630;
}

/* +0x1C (Item98_vtable) */
/* 0x00266240 */
s32 Item98_SortKey(void *o) {
    return 0x730;
}

/* +0x1C (Item9B_vtable) */
/* 0x002662F0 */
s32 Item9B_SortKey(void *o) {
    return 0x810;
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

/* +0x3C (ItemA5_vtable) */
/* 0x002664C0 */
s32 ItemA5_Use(void *o) {
    return 0;
}

/* +0x3C (ItemA6_vtable) */
/* 0x00266570 */
s32 ItemA6_Use(void *o) {
    return 0;
}

/* +0x3C (ItemA7_vtable) */
/* 0x00266620 */
s32 ItemA7_Use(void *o) {
    return 0;
}

/* +0x3C (ItemA8_vtable) */
/* 0x002666D0 */
s32 ItemA8_Use(void *o) {
    return 0;
}

/* +0x3C (ItemA9_vtable) */
/* 0x00266780 */
s32 ItemA9_Use(void *o) {
    return 0;
}

/* +0x3C (ItemAA_vtable) */
/* 0x00266830 */
s32 ItemAA_Use(void *o) {
    return 0;
}

/* +0x3C (ItemAB_vtable) */
/* 0x002668E0 */
s32 ItemAB_Use(void *o) {
    return 0;
}

/* +0x3C (ItemAC_vtable) */
/* 0x00266990 */
s32 ItemAC_Use(void *o) {
    return 0;
}

/* +0x1C (Item40_vtable) */
/* 0x00266A40 */
s32 Item40_SortKey(void *o) {
    return 0x110;
}

/* +0x1C (Item41_vtable) */
/* 0x00266BD0 */
s32 Item41_SortKey(void *o) {
    return 0x120;
}

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

/* +0x3C (ItemA0_vtable) */
/* 0x002D79B0 */
s32 ItemA0_Use(void *o) {
    return 0;
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

/* +0x3C (ItemA2_vtable) */
/* 0x0033AF10 */
s32 ItemA2_Use(void *o) {
    return 0;
}

/* 0x0033AF20 */
Character *Kind30_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind30_vtable); }

/* 0x0033B030 */
void *Kind30_ModelFiles(void) {
    return D_004308B0;
}

/* 0x0033B040 */
void *Kind30_MotionFiles(void) {
    return D_004308F0;
}

/* 0x0033B060 */
void Kind30_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0033B0B0 */
void Kind30_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf21); }

/* 0x0033B180 */
s32 Kind30_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* 0x0033B1F0 */
Character *Kind31_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind31_vtable); }

/* 0x0033B300 */
void *Kind31_ModelFiles(void) {
    return D_00430940;
}

/* 0x0033B310 */
void *Kind31_MotionFiles(void) {
    return D_00430980;
}

/* 0x0033B330 */
void Kind31_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0033B380 */
void Kind31_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf22); }

/* 0x0033B450 */
s32 Kind31_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

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

/* +0x3C (ItemA3_vtable) */
/* 0x0033BDF0 */
s32 ItemA3_Use(void *o) {
    return 0;
}

/* +0x14 (Item18_vtable) */
/* 0x0033E8C0 */
s32 Item18_Actions(void *o) {
    return 0x5;
}

/* 0x0033E8D0 */
s32 Item18_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_033_TEX, dest);
}

/* +0x14 (Item19_vtable) */
/* 0x0033EA30 */
s32 Item19_Actions(void *o) {
    return 0x5;
}

/* 0x0033EA40 */
s32 Item19_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_034_TEX, dest);
}

/* +0x14 (Item1A_vtable) */
/* 0x0033EBA0 */
s32 Item1A_Actions(void *o) {
    return 0x5;
}

/* 0x0033EBB0 */
s32 Item1A_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_035_TEX, dest);
}

/* +0x14 (Item1B_vtable) */
/* 0x0033ED10 */
s32 Item1B_Actions(void *o) {
    return 0x5;
}

/* 0x0033ED20 */
s32 Item1B_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_036_TEX, dest);
}

/* +0x14 (Item1C_vtable) */
/* 0x0033EE80 */
s32 Item1C_Actions(void *o) {
    return 0x5;
}

/* 0x0033EE90 */
s32 Item1C_LoadPicture(void *self, void *dest) {
    return FILE_LOAD_ASYNC(str_ITEM02_ITEM_037_TEX, dest);
}

/* +0x14 (Item1D_vtable) */
/* 0x00344590 */
s32 Item1D_Actions(void *o) {
    return 0x5;
}

/* 0x003445A0 */
s32 Item1D_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, str_ITEM02_ITEM_038_TEX, dest, 0x4000000, 0);
}

/* +0x3C (Item24_vtable) */
/* 0x00351870 */
s32 Item24_Use(void *o) {
    return 0;
}

/* +0x3C (Item29_vtable) */
/* 0x0035BBC0 */
s32 Item29_Use(void *o) {
    return 0;
}

/* ---- +0x28: start loading the item's picture into `dst` ---- */

#define ITEM_LOAD(name, dst) \
    VCALL(gFileLoader, 0xC, s32 (*)(VObject *, const char *, s32, s32, s32))(gFileLoader, name, dst, 0x4000000, 0)

extern const char D_0045A7C0[], D_0045A8A0[], D_0045A8C0[], D_0045A8E0[], D_0045A900[], D_0045A920[], str_ITEM03_ITEM_055_TEX[], str_ITEM03_ITEM_054_TEX[], str_ITEM03_ITEM_053_TEX[], str_ITEM03_ITEM_04A_TEX[], str_ITEM01_ITEM_02A_TEX[], str_ITEM01_ITEM_029_TEX[], str_ITEM01_ITEM_026_TEX[], str_ITEM01_ITEM_028_TEX[], str_ITEM01_ITEM_025_TEX[], str_ITEM01_ITEM_02B_TEX[], str_ITEM01_ITEM_027_TEX[], str_ITEM01_ITEM_024_TEX[], str_ITEM01_ITEM_023_TEX[], str_ITEM00_ITEM_00A_TEX[], str_ITEM00_ITEM_009_TEX[], str_ITEM00_ITEM_006_TEX[], str_ITEM00_ITEM_007_TEX[], str_ITEM04_ITEM_062_TEX[];

/* "ITEM00\ITEM_FFF.TEX" */
/* 0x0025FF50 */
s32 PoolEntry_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A7C0, dst);
}

/* "ITEM01\ITEM_023.TEX" */
/* 0x00265D10 */
s32 Item90_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_023_TEX, dst);
}

/* "ITEM01\ITEM_024.TEX" */
/* 0x00265DF0 */
s32 Item91_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_024_TEX, dst);
}

/* "ITEM01\ITEM_027.TEX" */
/* 0x00265EA0 */
s32 Item92_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_027_TEX, dst);
}

/* "ITEM01\ITEM_02B.TEX" */
/* 0x00265F50 */
s32 Item93_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_02B_TEX, dst);
}

/* "ITEM01\ITEM_025.TEX" */
/* 0x00266000 */
s32 Item94_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_025_TEX, dst);
}

/* "ITEM01\ITEM_028.TEX" */
/* 0x002660B0 */
s32 Item95_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_028_TEX, dst);
}

/* "ITEM01\ITEM_026.TEX" */
/* 0x00266160 */
s32 Item97_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_026_TEX, dst);
}

/* "ITEM01\ITEM_029.TEX" */
/* 0x00266210 */
s32 Item98_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_029_TEX, dst);
}

/* "ITEM01\ITEM_02A.TEX" */
/* 0x002662C0 */
s32 Item9B_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM01_ITEM_02A_TEX, dst);
}

/* "ITEM03\ITEM_04A.TEX" */
/* 0x002663F0 */
s32 ItemA4_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM03_ITEM_04A_TEX, dst);
}

/* "ITEM03\ITEM_053.TEX" */
/* 0x002664D0 */
s32 ItemA5_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM03_ITEM_053_TEX, dst);
}

/* "ITEM03\ITEM_054.TEX" */
/* 0x00266580 */
s32 ItemA6_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM03_ITEM_054_TEX, dst);
}

/* "ITEM03\ITEM_055.TEX" */
/* 0x00266630 */
s32 ItemA7_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM03_ITEM_055_TEX, dst);
}

/* "ITEM00\ITEM_FFF.TEX" */
/* 0x002666E0 */
s32 ItemA8_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A920, dst);
}

/* "ITEM00\ITEM_FFF.TEX" */
/* 0x00266790 */
s32 ItemA9_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A900, dst);
}

/* "ITEM00\ITEM_FFF.TEX" */
/* 0x00266840 */
s32 ItemAA_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A8E0, dst);
}

/* "ITEM00\ITEM_FFF.TEX" */
/* 0x002668F0 */
s32 ItemAB_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A8C0, dst);
}

/* "ITEM00\ITEM_FFF.TEX" */
/* 0x002669A0 */
s32 ItemAC_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A8A0, dst);
}

/* "ITEM00\ITEM_00A.TEX" */
/* 0x00266A50 */
s32 Item40_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM00_ITEM_00A_TEX, dst);
}

/* "ITEM00\ITEM_009.TEX" */
/* 0x00266BE0 */
s32 Item41_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM00_ITEM_009_TEX, dst);
}

/* "ITEM00\ITEM_006.TEX" */
/* 0x002EEBD0 */
s32 Item06_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM00_ITEM_006_TEX, dst);
}

/* "ITEM00\ITEM_007.TEX" */
/* 0x00303CA0 */
s32 Item07_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM00_ITEM_007_TEX, dst);
}

/* "ITEM04\ITEM_062.TEX" */
/* 0x0035BB90 */
s32 Item29_LoadPicture(void *o, s32 dst) {
    return ITEM_LOAD(str_ITEM04_ITEM_062_TEX, dst);
}

/* ---- the base pool entry's (an item's) stack ---- */

#define ITEM_HELD(o) (VCALL(o, 0x18, s32 (*)(void *))(o) & 0xFF)

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

/* +0x40 the partner's Hewie_CanTakeCommand (0 without one) */
/* 0x0025FF10 */
s32 PoolEntry_PartnerCheck(void *o) {
    if (gCharPartner != NULL) {
        return Hewie_CanTakeCommand((Hewie *)gCharPartner);
    }
    return 0;
}

/* ---- +0x3C: using an item ----
 * Returns what the menu does next: 0 nothing happens, 1 used up, 2 a flag set, 4 an event
 * started (Fiona's state 5). */

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

/* Item00_vtable: at spot 0xA of room 0xF, event 0x1 */
/* 0x002CCC70 */
s32 Item00_Use(void *o) {
    return use_at_spot(0xF, 0xA, 0x1);
}

/* Item18_vtable: at spot 0x17 of room 0x48, event 0x0 */
/* 0x0033E900 */
s32 Item18_Use(void *o) {
    return use_at_spot(0x48, 0x17, 0x0);
}

/* Item19_vtable: at spot 0x17 of room 0x48, event 0x17 */
/* 0x0033EA70 */
s32 Item19_Use(void *o) {
    return use_at_spot(0x48, 0x17, 0x17);
}

/* Item1A_vtable: at spot 0x17 of room 0x48, event 0x18 */
/* 0x0033EBE0 */
s32 Item1A_Use(void *o) {
    return use_at_spot(0x48, 0x17, 0x18);
}

/* Item1B_vtable: at spot 0x17 of room 0x48, event 0x19 */
/* 0x0033ED50 */
s32 Item1B_Use(void *o) {
    return use_at_spot(0x48, 0x17, 0x19);
}

/* Item1C_vtable: at spot 0x17 of room 0x48, event 0x1A */
/* 0x0033EEC0 */
s32 Item1C_Use(void *o) {
    return use_at_spot(0x48, 0x17, 0x1A);
}

/* Item23_vtable: at spot 0xB of room 0xC7, event 0x4 */
/* 0x00351710 */
s32 Item23_Use(void *o) {
    return use_at_spot(0xC7, 0xB, 0x4);
}

/* use: composure -100, stamina -1800 */
/* 0x00266A80 */
s32 Item40_Use(void *o) {
    item_meters(-100.0f, -1800);
    return 1;
}

/* use: Progress +0x9E0 -0.2666 for 300 frames (+0x9E4) */
/* 0x00266C10 */
s32 Item41_Use(void *o) {
    AT(gProgress, 0x9E0, u32) = 0xBE888889;   /* -0.26666668 */
    AT(gProgress, 0x9E4, s32) = 300;
    return 1;
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

/* Item0F_vtable: at door 0 of room 0x4F, event 1 */
/* 0x0031D050 */
s32 Item0F_Use(void *o) {
    return use_at_door(0x4F, 0, 1);
}

/* Item11_vtable: at door 4 of room 0x55, event 2 */
/* 0x0031D630 */
s32 Item11_Use(void *o) {
    return use_at_door(0x55, 4, 2);
}

/* 0x0031D6F0 */
Character *Kind18_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind18_vtable); }

/* 0x0031D800 */
void *Kind18_ModelFiles(void) {
    return pstr_O_FIW_FIW_200_PCK;
}

/* 0x0031D810 */
void *Kind18_MotionFiles(void) {
    return D_00429DB0;
}

/* 0x0031D830 */
void Kind18_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0031D880 */
void Kind18_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf12); }

/* 0x0031D950 */
s32 Kind18_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

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

/* 0x0032CE00 */
Character *Kind26_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind26_vtable); }

/* 0x0032CF10 */
void *Kind26_ModelFiles(void) {
    return D_0042C990;
}

/* 0x0032CF20 */
void *Kind26_MotionFiles(void) {
    return D_0042C9D0;
}

/* the squared distance to the nearest of Fiona, Hewie and the pursuer (those two when active
 * and in the room), whose position goes into out */
/* 0x0032CF40 */
f32 Kind26_NearestDistSq(u8 *self, f32 *out) {
    f32 d[4] __attribute__((aligned(16)));
    f32 pf[4] __attribute__((aligned(16)));
    f32 ph[4] __attribute__((aligned(16)));
    f32 pp[4] __attribute__((aligned(16)));
    f32 df, dh, dp;

    sceVu0SubVector(d, CHAR_POS(self), CHAR_POS(gCharPlayer));
    df = sceVu0InnerProduct(d, d);
    sceVu0CopyVector(pf, CHAR_POS(gCharPlayer));
#ifdef HG_NATIVE
    if (gCharPartner != NULL && CHAR_ON(gCharPartner) && CHAR_ROOM(gCharPartner) == CUR_ROOM()) {   /* (no Hewie: HG_NOPARTNER) */
#else
    if (CHAR_ON(gCharPartner) && CHAR_ROOM(gCharPartner) == CUR_ROOM()) {
#endif
        sceVu0SubVector(d, CHAR_POS(self), CHAR_POS(gCharPartner));
        dh = sceVu0InnerProduct(d, d);
        sceVu0CopyVector(ph, CHAR_POS(gCharPartner));
    } else {
        dh = 1.0e8f;
    }
    if (gCharPursuer != NULL && CHAR_ON(gCharPursuer) && CHAR_ROOM(gCharPursuer) == CUR_ROOM()) {
        sceVu0SubVector(d, CHAR_POS(self), CHAR_POS(gCharPursuer));
        dp = sceVu0InnerProduct(d, d);
        sceVu0CopyVector(pp, CHAR_POS(gCharPursuer));
    } else {
        dp = 1.0e8f;
    }
    sceVu0CopyVector(out, pf);
    if (!(df <= dh)) {
        sceVu0CopyVector(out, ph);
        df = dh;
    }
    if (!(df <= dp)) {
        sceVu0CopyVector(out, pp);
        df = dp;
    }
    return df;
}

/* turn 0.3 of the way towards (x, z) */
static inline __attribute__((always_inline)) void animal_face(u8 *self, f32 x, f32 z) {
    static const union { u32 u; f32 f; } k03 = {0x3E99999A};
    f32 t[4] __attribute__((aligned(16)));
    f32 a, d;

    t[0] = x;
    t[1] = 0.0f;
    t[2] = z;
    t[3] = 1.0f;
    sceVu0SubVector(t, t, CHAR_POS(self));
    a = Angle_Wrap(Vec_Heading(t));
    d = Angle_Wrap(a - Angle_Wrap(F32(self, 0x54)));
    F32(self, 0x54) = Angle_Wrap(0.0f + F32(self, 0x54) + k03.f * d);
}

#define ANIMAL_CLIP_DONE(self) ((S32(PTR(PTR(self, 0xF0), 0x6A4), 0x18) & 0x20) != 0)

static inline void animal_go(u8 *self, s32 state) {
    S32(self, 0x1624) = state;
    S32(self, 0x1628) = 1;
}

/* a frame of its behaviour: 0 grazing (turned to whoever it watches, 1..2 s), 1 looking up, 4
 * turning home (5 frames), 5 going back to grazing; 2 alert (someone within 30: meant to face
 * away from them, but the original takes the point's y for its z), 3 running off (within 20) to +0x163C / +0x1640 - 1 once there (its sound
 * +0x1630 + 1 played) */
s32 func_0032D430(u8 *self) {
    static const union { u32 u; f32 f; } k08 = {0x3F4CCCCD};
    f32 who[4] __attribute__((aligned(16)));
    f32 d = Kind26_NearestDistSq(self, who);

    if (S32(self, 0x1624) != 3 && d < 400.0f) {
        animal_go(self, 3);
    }
    if (S32(self, 0x1624) != 3 && S32(self, 0x1624) != 2 && d < 900.0f) {
        animal_go(self, 2);
    }
    switch (S32(self, 0x1624)) {
    case 0:
        if (S32(self, 0x1628) != 0) {
            Motion_PlayWith(PTR(self, 0xF0), 0x9000, 1, -1, 5.0f);
            S32(self, 0x162C) = (s32)(60.0f * (1.0f + animal_rnd()));
            S32(self, 0x1628) = 0;
        }
        animal_face(self, F32(D_01991600, 0), F32(D_01991600, 8));
        S32(self, 0x162C) -= 1;
        if (S32(self, 0x162C) < 0) {
            if (d <= 2500.0f || !(animal_rnd() < k08.f)) {
                S32(self, 0x1624) = 1;
            } else {
                S32(self, 0x1624) = 4;
            }
            S32(self, 0x1628) = 1;
            S32(self, 0x162C) = 0;
        }
        break;
    case 1:
        if (S32(self, 0x1628) != 0) {
            Motion_PlayWith(PTR(self, 0xF0), 0x9001, 1, -1, 5.0f);
            S32(self, 0x1628) = 0;
        }
        if (ANIMAL_CLIP_DONE(self) && animal_rnd() < 0.5f) {
            animal_go(self, 0);
        }
        break;
    case 4:
        if (S32(self, 0x1628) != 0) {
            S32(self, 0x162C) = 5;
            S32(self, 0x1628) = 0;
        }
        animal_face(self, F32(self, 0x1634), F32(self, 0x1638));
        if (--S32(self, 0x162C) < 0) {
            animal_go(self, 5);
        }
        break;
    case 5:
        if (S32(self, 0x1628) != 0) {
            Motion_PlayWith(PTR(self, 0xF0), 0x9002, 1, -1, 5.0f);
            S32(self, 0x1628) = 0;
        }
        if (ANIMAL_CLIP_DONE(self)) {
            animal_go(self, 0);
        }
        break;
    case 2: {
        f32 away[4] __attribute__((aligned(16)));

        if (S32(self, 0x1628) != 0) {
            Motion_PlayWith(PTR(self, 0xF0), 0x9002, 1, -1, 5.0f);
            S32(self, 0x1628) = 0;
            Kind26_NearestDistSq(self, (f32 *)D_01991600);
        }
        sceVu0SubVector(away, CHAR_POS(self), (f32 *)D_01991600);
        sceVu0ScaleVector(away, away, 100.0f);
        sceVu0AddVector(away, away, CHAR_POS(self));
        animal_face(self, away[0], away[1]);   /* (sic: y for z - the game's own slip) */
        if (ANIMAL_CLIP_DONE(self)) {
            animal_go(self, 0);
        }
        break;
    }
    case 3: {
        f32 dz, dx;

        if (S32(self, 0x1628) != 0) {
            Motion_PlayWith(PTR(self, 0xF0), 0x9003, 1, -1, 5.0f);
            S32(self, 0x1628) = 0;
        }
        animal_face(self, F32(self, 0x163C), F32(self, 0x1640));
        dz = F32(self, 0x1640) - F32(self, 0x18);
        dx = F32(self, 0x163C) - F32(self, 0x10);
        if (dz * dz + dx * dx < 1.0f) {
            Actor_PlaySound((Actor *)self, S32(self, 0x1630) + 1, 6, 0, 0, NULL);
            return 1;
        }
        break;
    }
    }
    return 0;
}

/* a frame of it (func_0032D430) moved by its animation's root motion, a footstep sound
 * (+0x1630, unless -1) as either foot comes down; 1 once it has run off */
static inline __attribute__((always_inline)) s32 animal_step(u8 *self) {
    f32 d[4] __attribute__((aligned(16)));
    void *m;
    u8 done = func_0032D430(self) & 0xFF;
    u8 l, r;

    Motion_RootMovement(PTR(self, 0xF0), d, 0.0f);
    Character_RootTurn((Character *)self);
    sceVu0ApplyMatrix(d, (f32 (*)[4])(self + 0x60), d);
    sceVu0AddVector(CHAR_POS(self), CHAR_POS(self), d);
    m = PTR(self, 0xF0);
    l = VCALL(m, 0x64, s32 (*)(void *, s32, s32))(m, 1, 0) & 0xFF;
    m = PTR(self, 0xF0);
    r = VCALL(m, 0x64, s32 (*)(void *, s32, s32))(m, 0, 0) & 0xFF;
    if (((l == 1 && self[0x16A8] == 0) || (r == 1 && self[0x16A9] == 0)) && S32(self, 0x1630) != -1) {
        Actor_PlaySound((Actor *)self, S32(self, 0x1630), 6, 0, 0, NULL);
    }
    self[0x16A8] = l;
    self[0x16A9] = r;
    return done;
}

/* the two animals' copies (started by Kind26_MoveTo / Kind26_MoveToB) */
/* 0x0032D150 */
s32 Kind26_MoveDone(u8 *self) {
    return animal_step(self);
}

/* 0x0032D2C0 */
s32 Kind26_MoveDoneB(u8 *self) {
    return animal_step(self);
}

void *func_0032DC50(void) {
    return D_0042CA20;
}

void *func_0032DC60(void) {
    return D_0042CB60;
}

void *func_0032DC70(void) {
    return D_0042CC00;
}

void *func_0032DC80(void) {
    return D_0042D0C0;
}

void *func_0032DC90(void *self, s32 i) {
    return D_0042E310[i];
}

void *func_0032DCB0(void) {
    return D_0042E410;
}

void *func_0032DCC0(void *self, s32 i) {
    return pstr_EV0023[i];
}

void func_0032F4E0(u8 *self) {
    S32(self, 0xFC8) = 0;
    self[0xFCC] = 0;
    S32(self, 0xFC0) = 0;
    S32(self, 0xFC4) = 0;

    S64(self, 0xC18) = -1;
    S32(self, 0xC24) = 0;
    S32(self, 0xC28) = 0;
    S32(self, 0xC2C) = 0;
    S32(self, 0xC30) = 25;
    S16(self, 0xC34) = 0x10;
    S16(self, 0xC36) = 0x20;
    S16(self, 0xC38) = 0x40;
    S16(self, 0xC3A) = 0x20;
    S16(self, 0xC3C) = 0x20;
    S16(self, 0xC3E) = 0x200;
    S16(self, 0xC40) = 0x100;
    self[0xC42] = 0x40;
    self[0xC43] = 1;
    self[0xC44] = 1;
    self[0xC45] = 0x10;
    self[0xC46] = 0xFF;

    S64(self, 0xC50) = -1;
    S32(self, 0xC5C) = 0;
    S32(self, 0xC60) = 0;
    S32(self, 0xC64) = 0;
    S32(self, 0xC68) = 25;
    S16(self, 0xC6C) = 0x10;
    S16(self, 0xC6E) = 0xE;
    S16(self, 0xC70) = 0x6E;
    S16(self, 0xC72) = 4;
    S16(self, 0xC74) = 4;
    S16(self, 0xC76) = 0x200;
    S16(self, 0xC78) = 0x100;
    self[0xC7A] = 0x40;
    self[0xC7B] = 1;
    self[0xC7C] = 1;
    self[0xC7D] = 0x10;
    self[0xC7E] = 0xFF;
}

void *func_0032F6C0(void) {
    return D_0042E4C0;
}

void func_0032DA60(Pursuer *p) { creature_inplay(p); }

void func_0032DAB0(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf18); }

s32 func_0032DB80(Pursuer *p) { return creature_slot_done(p); }

/* Item26_vtable: at spot 3 of room 0x92: flag 0x18, event 4 */
s32 func_00351B40(void *o) {
    Progress *p = gProgress;

    if (!item_room_spot(p, 0x92, 3)) {
        return 0;
    }
    Progress_SetFlag(p, 0x18);
    item_event(gEvents, 0, 4, gCharPlayer);
    return 4;
}

/* no use here: unless Progress +0x30 bit 0x8000, while item `id` is held, a sound (bank 0xC,
   5); else nothing */
static s32 use_sound_only(s32 need_24_4, s32 id) {
    Progress *p = gProgress;

    if ((AT(p, 0x30, u32) & 0x8000) || (need_24_4 && !(AT(p, 0x24, u32) & 4)) ||
        VCALL(gSubScreen, 0xC, s32 (*)(VObject *, s32))(gSubScreen, id) == 0) {
        return 0;
    }
    VCALL(gSound, 0x14, void (*)(VObject *, s32, s32))(gSound, 0xC, 5);
    return 8;
}

/* ItemA4_vtable */
s32 func_00266370(void *o) {
    return use_sound_only(0, 0x24B);
}

/* Item06_vtable: at door 0 of room 6: event 1, flag 0x18; else (with Progress +0x24 bit 4) the
   sound while item 0x232 is held */
s32 func_002EEC00(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 6 && item_door_open(p, 0)) {
        item_event(gEvents, 0, 1, gCharPlayer);
        Progress_SetFlag(p, 0x18);
        return 4;
    }
    return use_sound_only(1, 0x232);
}

/* at spot 0x11 of room 0xF once route 8 is open (Progress_DoorUnlocked): event 0x16; else on the altar */
static s32 use_route8_or_offer(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0xF && Progress_DoorUnlocked(p, 8) != 0 &&
        item_at_spot(gEvents, gCharPlayer, 0x11)) {
        item_event(gEvents, 0, 0x16, gCharPlayer);
        return 4;
    }
    return item_offer(p, o);
}

/* Item12_vtable */
s32 func_00264A60(void *o) {
    return use_route8_or_offer(o);
}

s32 func_00264C10(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_02D_TEX, dest, 0x4000000, 0);
}

/* Item13_vtable */
s32 func_00264CE0(void *o) {
    return use_route8_or_offer(o);
}

s32 func_00264E90(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_02E_TEX, dest, 0x4000000, 0);
}

/* Item07_vtable: at open door 1 of room 0x14: event 4, flag 0x18; else on the altar */
s32 func_00303CD0(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x14 && item_door_open(p, 1)) {
        item_event(gEvents, 0, 4, gCharPlayer);
        Progress_SetFlag(p, 0x18);
        return 4;
    }
    return item_offer(p, o);
}

/* at spot 0xB of room 0xC7: event 3, or 0x10 with Progress +0x2C bit 0x4000 */
static s32 use_room_c7(void) {
    Progress *p = gProgress;

    if (!item_room_spot(p, 0xC7, 0xB)) {
        return 0;
    }
    item_event(gEvents, 0, (AT(p, 0x2C, u32) & 0x4000) ? 0x10 : 3, gCharPlayer);
    return 4;
}

/* Item21_vtable */
s32 func_00351390(void *o) {
    return use_room_c7();
}

/* Item22_vtable */
s32 func_00351550(void *o) {
    return use_room_c7();
}

/* Item25_vtable: at room 0x82's open door 0 with route 0xE2: event 2; at room 0x8C's with route
   0xE6: event 8 */
s32 func_00351920(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x82) {
        return item_door_route(p, 0, 0xE2) ? item_event_flag(p, 2) : 0;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x8C && item_door_route(p, 0, 0xE6)) {
        return item_event_flag(p, 8);
    }
    return 0;
}

/* (as func_00351D80) at spot 3 of room 0x53: event 0; else on the altar */
s32 func_0031CE10(void *o) {
    Progress *p = gProgress;

    if (item_room_spot(p, 0x53, 3)) {
        return item_event_flag(p, 0);
    }
    return item_offer(p, o);
}

/* Item28_vtable: at spot 5 of room 0: event 0xF; else on the altar */
s32 func_00351D80(void *o) {
    Progress *p = gProgress;

    if (item_room_spot(p, 0, 5)) {
        return item_event_flag(p, 0xF);
    }
    return item_offer(p, o);
}

Character *func_00351F20(Character *c, s32 flags) { return creature_dtor(c, flags, Kind38_vtable); }

void *func_00352030(void) { return D_004434D0; }

void *func_00352040(void) { return D_00443510; }

void func_00352060(Pursuer *p) { creature_inplay(p); }

void func_003520B0(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf25); }

s32 func_00352180(Pursuer *p) { return creature_slot_done(p); }

/* Item0D_vtable: in room 0x4B, at open door 2 with route 0x51: event 1; door 3 with route 0x52:
   event 3; else on the altar */
s32 func_0031CB40(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x4B) {
        if (item_door_route(p, 2, 0x51)) {
            return item_event_flag(p, 1);
        }
        if (item_door_route(p, 3, 0x52)) {
            return item_event_flag(p, 3);
        }
    }
    return item_offer(p, o);
}

/* where one of the five medallions (Item08_vtable ..) is placed: room, spot, the events' +0x30
 * mode (-1: not called) and Fiona's event; room 0x40's only without Progress +0x2C bit 0x20 */
typedef struct ItemSlot {
    u8 room, spot;
    s8 mode;
    u8 ev;
} ItemSlot;

static s32 place_item(void *o, const ItemSlot *t, s32 n) {
    Progress *p = gProgress;
    s32 i;

    for (i = 0; i < n; i++, t++) {
        if (VCALL(p, 0xC, s32 (*)(Progress *))(p) != t->room) {
            continue;
        }
        if (t->room == 0x40 && (AT(p, 0x2C, u32) & 0x20)) {
            continue;
        }
        if (!item_at_spot(gEvents, gCharPlayer, t->spot)) {
            continue;
        }
        if (t->mode >= 0) {
            VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, t->mode, AT(o, 0x4, s32));
        }
        return item_event_flag(p, t->ev);
    }
    return 0;
}

s32 func_00306BC0(void *o) {
    static const ItemSlot t[7] = {
        {0x40, 0x11, 0, 1}, {0x42, 5, 1, 1}, {0x56, 4, 0, 0xA}, {0x57, 0xD, 1, 3},
        {0x69, 0xB, 1, 4},  {0x47, 8, -1, 1}, {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

s32 func_003071D0(void *o) {
    static const ItemSlot t[7] = {
        {0x40, 0x11, 0, 1}, {0x42, 5, 1, 1},   {0x47, 8, 0, 2}, {0x56, 4, 0, 0xA},
        {0x69, 0xB, 1, 4},  {0x57, 0xD, -1, 0}, {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

s32 func_003077E0(void *o) {
    static const ItemSlot t[7] = {
        {0x40, 0x11, 0, 1}, {0x42, 5, 1, 1},   {0x47, 8, 0, 2}, {0x56, 4, 0, 0xA},
        {0x57, 0xD, 1, 3},  {0x69, 0xB, -1, 0}, {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

s32 func_00307DF0(void *o) {
    static const ItemSlot t[7] = {
        {0x47, 8, 0, 2},    {0x56, 4, 0, 0xA}, {0x57, 0xD, 1, 3}, {0x69, 0xB, 1, 4},
        {0x40, 0x11, 0, 0}, {0x42, 5, -1, 0},  {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

s32 func_00308400(void *o) {
    static const ItemSlot t[7] = {
        {0x42, 5, 1, 1},   {0x47, 8, 0, 2},   {0x56, 4, 0, 0xA},    {0x57, 0xD, 1, 3},
        {0x69, 0xB, 1, 4}, {0x40, 0x11, 0, 0}, {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

/* the three statues' pedestals (room 0x54, spots 0xD / 0x11 / 0x14): a pedestal takes the
 * statue while neither of its two Progress +0x28 bits is set; else on the altar */
typedef struct ItemPedestal {
    u32 taken;   /* +0x28 bits that block it */
    u8 spot, ev;
} ItemPedestal;

static s32 place_statue(void *o, const ItemPedestal *t) {
    Progress *p = gProgress;
    s32 i;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x54) {
        for (i = 0; i < 3; i++, t++) {
            if (!(AT(p, 0x28, u32) & t->taken) && item_at_spot(gEvents, gCharPlayer, t->spot)) {
                return item_event_flag(p, t->ev);
            }
        }
    }
    return item_offer(p, o);
}

/* Item15_vtable */
s32 func_00338FA0(void *o) {
    static const ItemPedestal t[3] = {{0x180, 0xD, 0}, {0xC00, 0x11, 3}, {0x6000, 0x14, 6}};

    return place_statue(o, t);
}

/* Item16_vtable */
s32 func_00339350(void *o) {
    static const ItemPedestal t[3] = {{0xA00, 0x11, 4}, {0x140, 0xD, 1}, {0x5000, 0x14, 7}};

    return place_statue(o, t);
}

/* Item17_vtable */
s32 func_00339700(void *o) {
    static const ItemPedestal t[3] = {{0x3000, 0x14, 8}, {0xC0, 0xD, 2}, {0x600, 0x11, 5}};

    return place_statue(o, t);
}

Character *func_00339A10(Character *c, s32 flags) { return creature_dtor(c, flags, Kind28_vtable); }

void *func_00339B20(void) {
    return D_0042F470;
}

void *func_00339B30(void) {
    return D_0042F4B0;
}

void func_00339B50(Pursuer *p) { creature_inplay(p); }

void func_00339BA0(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf19); }

s32 func_00339C70(Pursuer *p) { return creature_slot_done(p); }

/* Item10_vtable: the medallions' slots without the altar */
s32 func_0031D1B0(void *o) {
    static const ItemSlot t[5] = {
        {0x42, 5, 1, 1}, {0x47, 8, 0, 2}, {0x57, 0xD, 1, 3}, {0x69, 0xB, 1, 4}, {0x56, 4, -1, 9},
    };

    return place_item(o, t, 5);
}

/* Item1D_vtable: in room 0x52 with Hewie at hand (+0x40), up and within 20, Fiona not busy
 * (+0xE8): event 0xB; else on the altar; else the sound while item 0x239 is held, or (with
 * Progress +0x30 bit 0x8000) while Hewie is in the room being played and item 0x27F is held */
s32 func_003445D0(void *o) {
    Progress *p = gProgress;
    VObject *ev_mgr = gEvents;
    s32 r;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x52) {
        u8 *h = (u8 *)gCharPartner;

        if (VCALL(o, 0x40, s32 (*)(void *))(o) != 0 && AT(h, 0xC4, s32) != 2 && AT(gCharPlayer, 0xE8, s32) == 0 &&
            Actor_Distance((Actor *)gCharPlayer, (f32 *)(h + 0x10)) < 20.0f) {
            item_event(ev_mgr, 0, 0xB, gCharPlayer);
            return 4;
        }
    }
    r = item_offer(p, o);
    if (r != 0) {
        return r;
    }
    if (!(AT(p, 0x30, u32) & 0x8000)) {
        return use_sound_only(0, 0x239);
    }
    {
        s32 room = AT(gCharPartner, 0x30, s32);

        if (room == VCALL(p, 0xC, s32 (*)(Progress *))(p) &&
            VCALL(gSubScreen, 0xC, s32 (*)(VObject *, s32))(gSubScreen, 0x27F) != 0) {
            VCALL(gSound, 0x14, void (*)(VObject *, s32, s32))(gSound, 0xC, 5);
            return 8;
        }
    }
    return 0;
}

/* free the file loader's 0x4000000 area if it is in state 2 */
void func_002600C0(void) {
    VObject *ld = gFileLoader;

    if (VCALL(ld, 0x28, s32 (*)(VObject *, s32))(ld, 0x4000000) == 2) {
        VCALL(ld, 0x14, void (*)(VObject *, s32))(ld, 0x4000000);
    }
}

/* an item's +0x18 set from a message { kind (0xFF: none), sub, byte 2, pad, word }: +0x4 0 when
 * unset (+0x5 0xFF), +0x7 the kind, +0x5 the sub (0xFF without a kind), +0x6, +0xC; +0x8 0 */
void func_0035BC30(u8 *o, const u8 *m) {
    if (m == NULL) {
        return;
    }
    if (o[5] == 0xFF) {
        o[4] = 0;
    }
    o[7] = m[0];
    if (o[7] == 0xFF) {
        o[5] = 0xFF;
    } else {
        o[5] = m[1];
    }
    o[6] = m[2];
    AT(o, 0xC, s32) = AT(m, 0x4, s32);
    AT(o, 0x8, s32) = 0;
}

extern void Sound_PlayBankAt(VObject *snd, u32 id, u32 bank, f32 *pos, s32 vol, s32 pitch);

/* the room-object glow (vtable ObjectGlow_vtable, made by RoomC5_Cmd00) +0x10 update: 0 while unset
 * (+0x5 0xFF). On (+0x7) it fades in (+0x4 up to 0x40 by 2), off it fades out; at 0 it is unset,
 * frees its script variable (+0x6, events +0x30) and plays sound 6 at the object (+0xC, +0x20)
 * unless the camera director's +0x38 is set - all 0. Its phase +0x8 runs 0..45 by 2 a frame */
s32 func_0035C8C0(u8 *o) {
    f32 t;

    if (o[5] == 0xFF) {
        return 0;
    }
    if (o[7] != 0) {
        if (o[4] != 0x40) {
            o[4] += 2;
        }
    } else {
        o[4] -= 2;
        if (o[4] == 0) {
            o[5] = 0xFF;
            VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, o[6], -1);
            if (VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) == 0) {
                Sound_PlayBankAt(gSound, 2, 6, (f32 *)(AT(o, 0xC, u8 *) + 0x20), 0, 0);
            }
            return 0;
        }
    }
    t = AT(o, 0x8, f32) + 2.0f;
    AT(o, 0x8, f32) = t;
    if (!(t < 45.0f)) {
        AT(o, 0x8, f32) = t - 45.0f;
    }
    return 1;
}

extern const char D_0045D818[], D_0045D820[], D_0045D828[], D_0045D838[], D_0045D840[], D_0045D848[],
    D_0045D850[], D_0045D858[], D_0045D860[], D_0045D870[], D_0045D878[], D_0045D880[];

/* the word item `it` carries (vtable +0x20) is `w` (the first 8 letters) */
static inline s32 item_named(VObject *it, const char *w) {
    const char *n = VCALL(it, 0x20, const char *(*)(VObject *))(it);
    const char *q = w;
    u32 i;

    for (i = 0; i < 8; i++, q++) {
        if (n[i] != *q) {
            return 0;
        }
        if (n[i] == 0) {
            break;
        }
    }
    return 1;
}

/* the elements' flag `bit` (Progress +0x1C bits) set */
static inline void item_progress_bit(Progress *p, s32 bit) {
    AT(p, 0x1C + (bit >> 5) * 4, u32) |= 1 << (bit & 0x1F);
}

/* +0x3C use (the word plate, vtable ItemClassF430_vtable): what Fiona does with the word it carries, room
 * by room (6: an event started) -
 *  0x23, spot 3: event 5
 *  0x24, spot 6, until progress +0x1C bit 27: "EMETH" (bit 26, event 0xA) / "METH" (bit 25,
 *        event 0xB) / "SALTATIO" (event 0xE), each with flag 0x18; anything else event 5
 *  0x26, spot 0xA, until +0x7C bit 12: "MAGNUS" event 0xB (flag 0x18), else 0xC
 *  0x0F, route 8 taken, spot 0x11: "REST" event 0x14 (flag 0x18), else 0x17
 *  0x49, spot 2: event 1
 *  0x66: spot 0x14 event 9; spot 0x16 "SALTATIO" event 0x11, else the word to the events (+0x30
 *        0 / 1) and event 0xA
 *  0x4F, until +0x24 bit 16: the elements "MERCURY" / "SULFUR" / "SALT" (flags 0x4D..0x4F, done:
 *        +0x24 bits 13..15). With two done and Hewie with her (or at spot 0xA): event 5, flag
 *        0x18; else spot 0xA event 0xA (4); spots 8 / 9 (until +0x24 bits 17 / 18) events 6 / 7.
 *        The word given: its flag and the events' +0x5C, else +0x60
 *  0x27 with +0x30 bit 14, spot 3: "ALCHYMIA" / "ADAMAS" / "POWDER" / "MORGAN" (not yet used:
 *        +0x88 bits 8..11) to the events (+0x30 1: 0x88 / 0x8C / 0x83 / 0x89) and event 8, else 9
 * elsewhere 0 */
s32 func_002D27E0(VObject *it) {
    Progress *p = gProgress;
    VObject *ev = gEvents;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x23 && item_at_spot(ev, gCharPlayer, 3)) {
        item_event(ev, 0, 5, gCharPlayer);
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x24 && !(AT(p, 0x1C, u32) & 0x8000000) &&
        item_at_spot(ev, gCharPlayer, 6)) {
        if (item_named(it, D_0045D818)) {
            AT(p, 0x1C, u32) |= 0x4000000;
            item_event(ev, 0, 0xA, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else if (item_named(it, D_0045D820)) {
            AT(p, 0x1C, u32) |= 0x2000000;
            item_event(ev, 0, 0xB, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else if (item_named(it, D_0045D828)) {
            item_event(ev, 0, 0xE, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else {
            item_event(ev, 0, 5, gCharPlayer);
        }
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x26 && !(AT(p, 0x7C, u32) & 0x1000) &&
        item_at_spot(ev, gCharPlayer, 0xA)) {
        if (item_named(it, D_0045D838)) {
            item_event(ev, 0, 0xB, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else {
            item_event(ev, 0, 0xC, gCharPlayer);
        }
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0xF && Progress_DoorUnlocked(p, 8) != 0 &&
        item_at_spot(ev, gCharPlayer, 0x11)) {
        if (item_named(it, D_0045D840)) {
            item_event(ev, 0, 0x14, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else {
            item_event(ev, 0, 0x17, gCharPlayer);
        }
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x49 && item_at_spot(ev, gCharPlayer, 2)) {
        item_event(ev, 0, 1, gCharPlayer);
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x66) {
        if (item_at_spot(ev, gCharPlayer, 0x14)) {
            item_event(ev, 0, 9, gCharPlayer);
            return 6;
        }
        if (item_at_spot(ev, gCharPlayer, 0x16)) {
            if (item_named(it, D_0045D828)) {
                item_event(ev, 0, 0x11, gCharPlayer);
            } else {
                const s32 *w = VCALL(it, 0x20, const s32 *(*)(VObject *))(it);

                VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, w[0]);
                VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, w[1]);
                item_event(ev, 0, 0xA, gCharPlayer);
            }
            return 6;
        }
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x4F && !(AT(p, 0x24, u32) & 0x10000)) {
        s32 given = 0, done = 0, bit = -1;

        if (AT(p, 0x24, u32) & 0x2000) {
            done = 1;
        } else if (item_named(it, D_0045D848)) {
            given = 1;
            bit = 0x4D;
        }
        if (AT(p, 0x24, u32) & 0x4000) {
            done++;
        } else if (item_named(it, D_0045D850)) {
            given = 1;
            bit = 0x4E;
        }
        if (AT(p, 0x24, u32) & 0x8000) {
            done++;
        } else if (item_named(it, D_0045D858)) {
            given = 1;
            bit = 0x4F;
        }
        if (done == 2) {
            u8 *h = (u8 *)gCharPartner;

            if (h != NULL && AT(h, 0x28, u8) == 1 && AT(h, 0x30, s32) == VCALL(p, 0xC, s32 (*)(Progress *))(p) &&
                AT(h, 0xC4, s32) != 2 &&
                (item_at_spot(ev, gCharPlayer, 0xA) || Actor_Distance((Actor *)gCharPlayer, (f32 *)(h + 0x10)) < 20.0f)) {
                item_event(ev, 0, 5, gCharPlayer);
                Progress_SetFlag(p, 0x18);
                if (!given) {
                    VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 0);
                } else {
                    item_progress_bit(p, bit);
                    VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 0);
                }
                return 6;
            }
            if (item_at_spot(ev, gCharPlayer, 0xA)) {
                item_event(ev, 0, 0xA, gCharPlayer);
                return 4;
            }
        } else if (item_at_spot(ev, gCharPlayer, 8) && !(AT(p, 0x24, u32) & 0x20000)) {
            item_event(ev, 0, 6, gCharPlayer);
            if (!given) {
                VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 0);
            } else {
                item_progress_bit(p, bit);
                Progress_SetFlag(p, 0x18);
                VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 0);
            }
            return 6;
        } else if (item_at_spot(ev, gCharPlayer, 9) && !(AT(p, 0x24, u32) & 0x40000)) {
            item_event(ev, 0, 7, gCharPlayer);
            if (!given) {
                VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 0);
            } else {
                item_progress_bit(p, bit);
                Progress_SetFlag(p, 0x18);
                VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 0);
            }
            return 6;
        }
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x27 && (AT(p, 0x30, u32) & 0x4000) &&
        item_at_spot(ev, gCharPlayer, 3)) {
        if (item_named(it, D_0045D860) && !(AT(p, 0x88, u32) & 0x100)) {
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, 0x88);
            item_event(ev, 0, 8, gCharPlayer);
        } else if (item_named(it, D_0045D870) && !(AT(p, 0x88, u32) & 0x200)) {
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, 0x8C);
            item_event(ev, 0, 8, gCharPlayer);
        } else if (item_named(it, D_0045D878) && !(AT(p, 0x88, u32) & 0x400)) {
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, 0x83);
            item_event(ev, 0, 8, gCharPlayer);
        } else if (item_named(it, D_0045D880) && !(AT(p, 0x88, u32) & 0x800)) {
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, 0x89);
            item_event(ev, 0, 8, gCharPlayer);
        } else {
            item_event(ev, 0, 9, gCharPlayer);
        }
        return 6;
    }
    return 0;
}

s32 func_002D3A20(void *self, void *dest) { return LOAD_002D1360(str_ITEM00_ITEM_008_TEX, dest); }

#ifdef HG_NATIVE
#include "gl2d.h"
#include "sce/libvu0.h"

extern void *Bloom_vtable[], *Helper469D00_vtable[];
extern f32 func_0031C058(f32 x);   /* cosf */
extern f32 func_0031C248(f32 x);   /* sinf */

/* is clip-space point `p` (camera +0x48) inside the view */
static s32 glow_in_view(f32 (*clip)[4], const f32 *pt) {
    f32 v[4] __attribute__((aligned(16)));

    sceVu0ApplyMatrix(v, clip, (f32 *)pt);
    return v[0] <= v[3] && !(v[0] < -v[3]) && v[1] <= v[3] && !(v[1] < -v[3]) && v[2] <= v[3] && !(v[2] < -v[3]);
}

/* the glow +0x14 draw (while set): around its room object (+0xC, at floor height -1)
 * - a disc of radius r (by its kind +0x5) fading out from the middle (alpha +0x4), added into
 *   layer 0x26 (the bloom's mask); only when all of it is in view (kind 0: the disc left out
 *   then, and the screen also brightened, Bloom_Start 0x40808080 in layer 0x28)
 * - eight faint cyan strips standing 40 high at radius r + 5, turning with its phase (+0x8),
 *   added in layer 2, when all in view */
void func_0035BCA0(u8 *o) {
    static const union { u32 u; f32 f; } kRadius[8] = {
        {0x409A3D71}, {0x40800000}, {0x407F5C29}, {0x409D70A4}, {0x408D1EB8}, {0x407C28F6}, {0x405E147B}, {0x411A3D71},
    };
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 pt[18][4] __attribute__((aligned(16)));
    u8 *obj = AT(o, 0xC, u8 *);
    f32 c[4], r, r2;
    s32 i, clipped = 0;

    if (o[5] == 0xFF) {
        return;
    }
    c[0] = AT(obj, 0x20, f32);
    c[1] = -1.0f;
    c[2] = AT(obj, 0x28, f32);
    c[3] = 1.0f;
    r = o[5] < 8 ? kRadius[o[5]].f : 0.0f;
    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, clip);

    /* the disc: the middle, then 16 points round it */
    sceVu0CopyVector(pt[0], c);
    for (i = 0; i < 16; i++) {
        f32 a = kPi.f * (22.5f * (f32)i) / 180.0f;

        pt[i + 1][0] = c[0] + r * func_0031C248(a);
        pt[i + 1][1] = c[1];
        pt[i + 1][2] = c[2] - r * func_0031C058(a);
        pt[i + 1][3] = 1.0f;
    }
    for (i = 0; i < 17; i++) {
        if (!glow_in_view(clip, pt[i])) {
            if (o[5] != 0) {
                return;
            }
            clipped = 1;
        }
    }
    if (o[5] == 0) {
        u8 drawer[0x20] __attribute__((aligned(16)));

        AT(drawer, 0x0, void **) = Bloom_vtable;
        AT(drawer, 0x4, s32) = -1;
        Bloom_Start(drawer, 0x40808080, 0x28, 0);
        AT(drawer, 0x0, void **) = Helper469D00_vtable;
    }
    if (!clipped) {
        f32 tri[3][4];
        f32 st[3][2] = {{0}};
        u8 col[3][4] = {{0x80, 0x80, 0x80, 0}, {0}, {0}};

        col[0][3] = o[4];
        glr_layer(0x26);
        for (i = 0; i < 16; i++) {   /* the fan, closing on its first rim point */
            sceVu0CopyVector(tri[0], pt[0]);
            sceVu0CopyVector(tri[1], pt[i + 1]);
            sceVu0CopyVector(tri[2], pt[(i + 1) % 16 + 1]);
            AT(&tri[0][3], 0, u32) = AT(&tri[1][3], 0, u32) = AT(&tri[2][3], 0, u32) = 0;
            glr_strip((const f32 *)clip, 3, &tri[0][0], &st[0][0], &col[0][0], NULL, 0,
                      0x40 | GLR_PRIM_ADD | GLR_PRIM_NOZW);
        }
        glr_layer(-1);
    }

    /* the strips: the middle at the bottom and 40 up, then 8 pairs round it */
    r2 = 5.0f + r;
    sceVu0CopyVector(pt[0], c);
    sceVu0CopyVector(pt[1], c);
    pt[1][1] += 40.0f;
    for (i = 0; i < 8; i++) {
        f32 a = kPi.f * (45.0f * (f32)i + AT(o, 0x8, f32)) / 180.0f;

        sceVu0CopyVector(pt[2 + i * 2], c);
        pt[2 + i * 2][0] = c[0] + r2 * func_0031C248(a);
        pt[2 + i * 2][2] = c[2] - r2 * func_0031C058(a);
        sceVu0CopyVector(pt[3 + i * 2], pt[2 + i * 2]);
        pt[3 + i * 2][1] += 40.0f;
    }
    for (i = 0; i < 18; i++) {
        if (!glow_in_view(clip, pt[i])) {
            return;
        }
    }
    glr_layer(2);
    for (i = 0; i < 8; i++) {
        static const u8 kCol[4][4] = {{0x00, 0x20, 0x20, 0x10}, {0, 0, 0x40, 0}, {0, 0, 0x20, 0}, {0, 0, 0x10, 0}};
        f32 q[4][4];
        f32 st[4][2] = {{0}};
        s32 k;

        sceVu0CopyVector(q[0], pt[0]);
        sceVu0CopyVector(q[1], pt[2 + i * 2]);
        sceVu0CopyVector(q[2], pt[1]);
        sceVu0CopyVector(q[3], pt[3 + i * 2]);
        for (k = 0; k < 4; k++) {
            AT(&q[k][3], 0, u32) = 0;
        }
        glr_strip((const f32 *)clip, 4, &q[0][0], &st[0][0], &kCol[0][0], NULL, 0, 0x40 | GLR_PRIM_ADD | GLR_PRIM_NOZW);
    }
    glr_layer(-1);
}
#endif

/* progress +0x84 bits 22..26 as bits 0..4 */
u8 func_00303F00(void) {
    u32 b = AT(gProgress, 0x84, u32);
    u8 v = 0;

    if (b & 0x400000) {
        v |= 1;
    }
    if (b & 0x800000) {
        v |= 2;
    }
    if (b & 0x1000000) {
        v |= 4;
    }
    if (b & 0x2000000) {
        v |= 8;
    }
    if (b & 0x4000000) {
        v |= 0x10;
    }
    return v;
}
