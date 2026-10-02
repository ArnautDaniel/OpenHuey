/* The sub screen: the menu system of the title and of the in-game menus (vtable D_0047A790, at
 * SceneTitle +0x74A80; code 0x384C00..0x3A0000). Its textures are SUBSCR\SUBBASE.TEX and
 * SUBSCR\SUBBACK.TEX. */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "progress.h"
#include "task.h"

#define AT(p, off, type) (*(type *)((u8 *)(p) + (off)))

extern VObject *gFileLoader;
extern u8 *D_0044E978;          /* the system data; +0x30 the options */
extern VObject *D_0044E7A8;     /* the pad actuator (vibration) */
extern VObject *D_0044E560;     /* the sound driver */
extern VObject *D_00456DF0;
extern void *D_0044E958;        /* the movie playing */
extern void *D_0044E980;        /* the ADX sound system (music) */
extern VObject *D_0044E4F0;     /* the renderer */
extern void *D_0046C790[];      /* a pool entry */
extern void func_002B6340(void *movie);   /* apply the movie volume */
extern void func_002D1FD0(void *bgm);     /* apply the music volume */
extern void func_00120EC0(void *pool, u8 *base, u32 size, u32 n, u8 *used);

void func_00396900(void *s);
void func_00399920(void *s);
void func_00398850(void *s);

static const char sSubBase[] = "SUBSCR\\SUBBASE.TEX";
static const char sSubBack[] = "SUBSCR\\SUBBACK.TEX";

static inline void set_state(PTMF *dst, void (*fn)(void *)) {
    PTMF s = {0, -1, {(void *)fn}};

    if (ptmf_test(&s)) {
        *dst = s;
    }
}

/* placement new (pool entries) */
void *func_0025FF00(u32 size, void *p) {
    return p;
}

/* the entries' pool: the first 64 entries constructed again, the tables cleared, the block
 * pool (+0x1208) over all 192 */
void func_00264900(u8 *pool) {
    s32 i, j;

    for (i = 0; i < 0x40; i++) {
        u8 *e = func_0025FF00(0x18, pool + 8 + i * 0x18);

        if (e != NULL) {
            AT(e, 0x0, void **) = D_0046C790;
            AT(e, 0x4, s32) = -1;
            AT(e, 0x8, u8) = 0;
            AT(e, 0x10, s64) = 0;
        }
    }
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 0x40; i++) {
            AT(pool, 0x12E0 + j * 0x100 + i * 4, s32) = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        AT(pool, 0x15E0 + i * 4, s32) = 0;
    }
    func_00120EC0(pool + 0x1208, pool + 8, 0x18, 0xC0, pool + 0x1220);
}

/* start: load the textures, reset */
void func_00399A10(u8 *s) {
    VObject *ld = gFileLoader;
    s32 i;

    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sSubBase, s + 0x1740, 0x6000000, 0);
    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sSubBack, s + 0x11F40, 0x6000000, 0);
    func_00264900(s + 8);
    for (i = 0; i < 9; i++) {
        AT(s, 0x97740 + i * 4, s32) = 0;
    }
    for (i = 0; i < 0x80; i++) {
        AT(s, 0x15F8 + i * 2, u16) = 0;
    }
    AT(s, 0x4, u8) = 0;
    AT(s, 0xA8DE0, s32) = 0;
    AT(s, 0x16F8, u8) = 1;
    AT(s, 0xA8C50, u8) = 0;
    for (i = 0; i < 3; i++) {
        AT(s, 0xA8C51 + i, u8) = 0;
    }
    AT(s, 0xA8C55, u8) = 0;
    AT(s, 0xA8C56, u8) = 0;
    AT(s, 0xA8DE5, u8) = 0;
    AT(s, 0xA8C67, u8) = 0;
    AT(s, 0xA8C66, u8) = 0;
    set_state(&AT(s, 0x1714, PTMF), func_00396900);
    set_state(&AT(s, 0x16FC, PTMF), func_00399920);
}

/* apply the options: controller layout, vibration, sound output, volume, screen position */
void func_003921A0(VObject *s) {
    u8 *opt = D_0044E978 + 0x30;
    VObject *o;
    f32 vol;

    VCALL(s, 0x34, void (*)(VObject *, s32))(s, (s8)opt[6]);
    o = D_0044E7A8;
    VCALL(o, 0x10, void (*)(VObject *))(o);
    if ((s8)opt[4] == 1) {
        VCALL(o, 0x28, void (*)(VObject *, s32))(o, 1);
    } else {
        VCALL(o, 0x28, void (*)(VObject *, s32))(o, 0);
    }
    o = D_0044E560;
    VCALL(o, 0x68, void (*)(VObject *, s32))(o, (s8)opt[0]);
    vol = *(f32 *)(opt + 8);
    VCALL(o, 0xA8, void (*)(VObject *, f32))(o, vol);
    if (D_00456DF0 != NULL) {
        VCALL(D_00456DF0, 0x20, void (*)(VObject *, f32))(D_00456DF0, vol);
    }
    if (D_0044E958 != NULL) {
        f32 *v = &AT(D_0044E958, 0x1D0, f32);

        *v = vol;
        if (vol < 0.0f) {
            *v = 0.0f;
        }
        if (!(*v <= 1.0f)) {
            *v = 1.0f;
        }
        func_002B6340(D_0044E958);
    }
    if (D_0044E980 != NULL) {
        f32 *v = &AT(D_0044E980, 0x11C, f32);

        *v = vol;
        if (vol < 0.0f) {
            *v = 0.0f;
        }
        if (!(*v <= 1.0f)) {
            *v = 1.0f;
        }
        func_002D1FD0(D_0044E980);
    }
    VCALL(D_0044E4F0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4F0, (s8)opt[2], (s8)opt[3]);
}

extern u16 D_0044B5E0[][6][2];   /* per controller layout: 6 x (button, its name's message) */
extern u8 D_0047E3C0[];          /* button mapping (+0xA: the six configurable actions) */
extern u8 D_0047B150[6];         /* the name slot of each action */
extern void func_00380990(void *self, s32 slot, s32 id);

/* +0x34 apply controller layout `type`: map the six actions and name their buttons */
void func_003913B0(u8 *s, s32 type) {
    s32 i;

    for (i = 0; i < 6; i++) {
        D_0047E3C0[0xA + i] = D_0044B5E0[type][i][0];
        func_00380990(s + 0x97764, D_0047B150[i], D_0044B5E0[type][i][1]);
    }
}

extern VObject *D_0044E4E8;      /* the texture cache */
extern void *func_00322570(u32 size, void *p);   /* placement new */
extern void func_00305380(void *p);
extern void func_002BFB00(void *card, void *buf, void *buf2);
extern void func_0038F7D0(void *s);
extern void func_00388D30(void *s);
extern void *D_00474020[];       /* the 0x15C helper's base vtable */
extern void *D_00474040[];
extern void *D_00474060[];

void func_00393BD0(void *s);
void func_003912E0(void *s);
void func_003908B0(void *s);
void func_00390790(void *s);
void func_00391250(void *s);
void func_0038FBE0(void *s);
void func_0038E0C0(void *s);
void func_003894F0(void *s);
void func_0038D7E0(void *s);
void func_00388FF0(void *s);
void func_003888A0(void *s);
void func_00387F00(void *s);
void func_00386150(void *s);
void func_003984D0(void *s);
void func_00398100(void *s);

static const char sSubSave[] = "SUBSCR\\SUBSAVE.TEX";
static const char sPlate[] = "SUBSCR\\PLATE.TEX";
static const char sSynSlot[] = "SUBSCR\\SYN_SLOT.TEX";
static const char sSubMg[] = "SUBSCR\\SUBMG.TEX";
static const char sGalMovie[] = "SUBSCR\\GAL_MOVIE.TEX";
static const char sThumbnail[] = "SUBSCR\\THUMBNAIL.TXS";
static const char sGalModel[] = "SUBSCR\\GAL_MODEL.TEX";
static const char sGalMusic[] = "SUBSCR\\GAL_MUSIC.TEX";
static const char sGalArt[] = "SUBSCR\\GAL_ART.TEX";
static const char sArtLen[] = "SUBSCR\\ART00.LEN";
static const char sGalType[] = "SUBSCR\\GAL_TYPE.TEX";

#define SUB_KIND(s)     AT(s, 0xA8C66, u8)   /* what the screen shows (0x80..0x8F) */
#define SUB_STATE(s)    AT(s, 0x1708, PTMF)
#define SUB_DRAW(s)     AT(s, 0x16FC, PTMF)

static void sub_load(u8 *s, const char *name, void *dst) {
    VObject *ld = gFileLoader;

    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, name, dst, 0x6000000, 0);
}

static void sub_free_vram(void) {
    VCALL(D_0044E4E8, 0x14, void (*)(VObject *, s32))(D_0044E4E8, 0x19);
}

static void sub_se(void) {
    VCALL(D_0044E560, 0x14, void (*)(VObject *, s32, s32))(D_0044E560, 0x94, 5);
}

/* the options being edited: a copy of the system data's */
static void sub_copy_options(u8 *s) {
    u8 *sys = D_0044E978;
    s32 i;

    for (i = 0; i < 7; i++) {
        AT(s, 0xA8C44 + i, u8) = sys[0x30 + i];
    }
    AT(s, 0xA8C4C, f32) = AT(sys, 0x38, f32);
}

/* the save slot screen's helper object (0x15C bytes at +0xA8C80) */
static void sub_new_slots(u8 *s, void **vtbl) {
    u8 *p = func_00322570(0x15C, s + 0xA8C80);

    if (p != NULL) {
        AT(p, 0, void **) = D_00474020;
        Task_Construct((Task *)(p + 0x14));
        VCALL((VObject *)p, 0xC, void (*)(VObject *))((VObject *)p);
        AT(p, 0, void **) = vtbl;
        VCALL((VObject *)p, 0xC, void (*)(VObject *))((VObject *)p);
    }
}

/* open the sub screen in mode +4: 0 the in-game menu, 5 options, 6 load (title), the rest
 * save screens and the galleries */
void func_00398850(void *self) {
    u8 *s = self;
    u8 *sys;
    u8 n;

    AT(s, 0xA8DE5, u8) = 1;
    switch (AT(s, 0x4, u8)) {
    case 0:
        sub_copy_options(s);
        if (gProgress != NULL && AT(gProgress, 0x1FBEC1, u8) == 1) {
            SUB_KIND(s) = 0xA;
            set_state(&SUB_STATE(s), func_00393BD0);
        } else {
            func_00305380(s + 0x97980);
            SUB_KIND(s) = AT(s, 0xA8C67, u8);
            SUB_STATE(s) = AT(s, 0x1714, PTMF);
        }
        sub_se();
        break;
    case 1:
        sub_free_vram();
        sub_load(s, sSubSave, s + 0x11F40);
        func_002BFB00(s + 0xA8AC0, s + 0x42F40, s + 0x43740);
        AT(s, 0xA8AC4, s32) = 0;
        AT(s, 0xA8AC8, s32) = 0;
        SUB_KIND(s) = 0x85;
        set_state(&SUB_STATE(s), func_003912E0);
        break;
    case 2:
        sub_free_vram();
        sub_load(s, sPlate, s + 0x11F40);
        AT(s, 0xA8C57, u8) = 0;
        for (n = 0; n < 8; n++) {
            AT(s, 0xA8C58 + n, u8) = 0;
        }
        AT(s, 0xA8C60, u8) = 0;
        SUB_KIND(s) = 0x84;
        set_state(&SUB_STATE(s), func_003908B0);
        sub_se();
        break;
    case 3:
        sub_free_vram();
        sub_load(s, sSynSlot, s + 0x11F40);
        sub_new_slots(s, D_00474060);
        SUB_KIND(s) = 0x86;
        set_state(&SUB_STATE(s), func_00390790);
        sub_se();
        break;
    case 4:
        sub_free_vram();
        sub_load(s, sSynSlot, s + 0x11F40);
        sub_new_slots(s, D_00474040);
        SUB_KIND(s) = 0x87;
        set_state(&SUB_STATE(s), func_00390790);
        sub_se();
        break;
    case 5:
        sub_copy_options(s);
        sub_se();
        SUB_KIND(s) = 0x8A;
        set_state(&SUB_STATE(s), func_00393BD0);
        break;
    case 6:
        sub_free_vram();
        sub_load(s, sSubSave, s + 0x11F40);
        func_002BFB00(s + 0xA8AC0, NULL, NULL);
        AT(s, 0xA8AC4, s32) = 0;
        AT(s, 0xA8AC8, s32) = 0;
        SUB_KIND(s) = 0x85;
        set_state(&SUB_STATE(s), func_00391250);
        break;
    case 7:
        sub_free_vram();
        sub_load(s, sSubMg, s + 0x11F40);
        SUB_KIND(s) = 0x80;
        func_0038F7D0(s);
        set_state(&SUB_STATE(s), func_0038FBE0);
        break;
    case 8: {
        VObject *ld;
        Progress *p;

        sub_free_vram();
        ld = gFileLoader;
        VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sGalMovie, s + 0x11F40, 0x6000000, 0);
        p = gProgress;
        VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sThumbnail, (u8 *)p + 0x16C0, 0x6000000, 0);
        SUB_KIND(s) = 0x8B;
        AT(s, 0xA8C80, u8) = Progress_GetVar(p, 0x2B);
        set_state(&SUB_STATE(s), func_0038E0C0);
        break;
    }
    case 9:
        func_002BFB00(s + 0xA8AC0, s + 0x42F40, s + 0x43740);
        AT(s, 0xA8AC4, s32) = 0;
        AT(s, 0xA8AC8, s32) = 1;
        SUB_KIND(s) = 0x85;
        set_state(&SUB_STATE(s), func_003912E0);
        break;
    case 10:
        /* the extras menu: entries 0..2, then 3 and 4 when unlocked, then 5 */
        sub_free_vram();
        sub_load(s, sSubMg, s + 0x11F40);
        SUB_KIND(s) = 0x80;
        AT(s, 0xA8C80, u8) = 0;
        AT(s, 0xA8C81, u8) = 1;
        AT(s, 0xA8C82, u8) = 2;
        n = 3;
        sys = D_0044E978;
        if ((AT(sys, 0x2C, u32) & 0x40000) != 0) {
            AT(s, 0xA8C83, u8) = 3;
            n++;
        }
        if ((AT(sys, 0x2C, u32) & 0x80000) != 0) {
            AT(s, 0xA8C80 + n, u8) = 4;
            n++;
        }
        AT(s, 0xA8C80 + n, u8) = 5;
        AT(s, 0xA8C86, u8) = n + 1;
        AT(s, 0xA8C87, u8) = 0;
        set_state(&SUB_STATE(s), func_003894F0);
        break;
    case 11:
        sub_free_vram();
        sub_load(s, sGalModel, s + 0x11F40);
        SUB_KIND(s) = 0x8C;
        AT(s, 0xA8C80, u8) = 0;
        set_state(&SUB_STATE(s), func_0038D7E0);
        break;
    case 12:
        SUB_KIND(s) = 0x80;
        func_00388D30(s);
        set_state(&SUB_STATE(s), func_00388FF0);
        break;
    case 13:
        sub_free_vram();
        sub_load(s, sGalMusic, s + 0x11F40);
        SUB_KIND(s) = 0x8D;
        AT(s, 0xA8C80, u8) = 0;
        AT(s, 0xA8C82, u8) = 0xFF;
        AT(s, 0xA8C81, u8) = 0xFF;
        set_state(&SUB_STATE(s), func_003888A0);
        break;
    case 14: {
        VObject *ld;
        void *buf;

        sub_free_vram();
        ld = gFileLoader;
        VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sGalArt, s + 0x11F40, 0x6000000, 0);
        SUB_KIND(s) = 0x8E;
        AT(s, 0xA8C80, u8) = 0;
        AT(s, 0xA8C81, u8) = 0;
        AT(s, 0xA8C82, u8) = 0;
        AT(s, 0xA8C83, u8) = 1;
        buf = VCALL((VObject *)gProgress, 0x88, void *(*)(VObject *))((VObject *)gProgress);
        VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sArtLen, buf, 0x6000000, 0);
        AT(s, 0xA8C84, s16) = 0;
        AT(s, 0xA8C86, s16) = 0;
        AT(s, 0xA8C88, u8) = 0;
        AT(s, 0xA8C89, u8) = 1;
        AT(s, 0xA8C8A, u8) = 0;
        AT(s, 0xA8C8B, u8) = 0;
        AT(s, 0xA8C8C, u8) = 0;
        set_state(&SUB_STATE(s), func_00387F00);
        break;
    }
    case 15:
        sub_free_vram();
        sub_load(s, sGalType, s + 0x11F40);
        SUB_KIND(s) = 0x8F;
        AT(s, 0xA8C80, u8) = 0;
        set_state(&SUB_STATE(s), func_00386150);
        break;
    }
    AT(s, 0x16F8, u8) = 1;
    AT(s, 0xA8C61, u8) = 1;
    AT(s, 0xA8C62, s16) = 0;
    AT(s, 0xA8C64, s16) = 0x10;
    set_state(&SUB_DRAW(s), func_003984D0);
    if (AT(s, 0x4, u8) == 7 || AT(s, 0x4, u8) == 0xC || AT(s, 0x4, u8) == 0xA) {
        AT(s, 0xA8C64, s16) = 8;
        set_state(&SUB_DRAW(s), func_00398100);
    }
    if (gProgress != NULL) {
        Progress_ClearFlag(gProgress, 4);
    }
}

/* per frame: run the draw/step state (+0x16FC); true while the menu behind should be drawn */
s32 func_003999C0(u8 *s) {
    ptmf_scall(s, &AT(s, 0x16FC, PTMF));
    AT(s, 0xA8DE0, s32)++;
    return AT(s, 0x16F8, u8);
}

/* state: wait for the textures, upload them (VRAM slots 0x18, 0x19), set the screen up */
void func_00399920(void *self) {
    u8 *s = self;
    VObject *tc;

    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x6000000) == 2) {
        return;
    }
    tc = D_0044E4E8;
    VCALL(tc, 0x10, void (*)(VObject *, void *, s32))(tc, s + 0x1740, 0x18);
    VCALL(tc, 0x10, void (*)(VObject *, void *, s32))(tc, s + 0x11F40, 0x19);
    func_00398850(s);
}

extern s8 D_0044C120[][4];       /* per screen kind: up to 4 panels to draw while fading */
extern u8 D_0047B350;
extern void *D_0046F350[];       /* overlay vtable */
extern void *D_00469D00[];       /* its base */
extern void func_002CF390(void *ov, u32 rgba);
void func_00397F00(void *s);

#define SUB_FADE(s)     AT(s, 0xA8C62, s16)   /* 0 .. 0x80 */
#define SUB_FADESTEP(s) AT(s, 0xA8C64, s16)

extern VObject *D_0044E9A0;   /* the VRAM manager */

/* a fixed piece of the screen (SUBBACK.TEX, VRAM group 0x19): texture, u, v, w, h, x, y */
extern u16 D_0044C160[][7];
/* a movable part (SUBBASE.TEX, group 0x18): u, v, w, h, screen w, h, CLUT, texture, blending
 * (0 normal, 1..3 fixed alpha variants) */
extern u16 D_0044C280[][9];

/* the VRAM slot of a cached texture, uploading it into `layer` if it is not resident; -1 none */
static s32 sub_texture(u16 id, s32 group, s32 layer, u8 **tex) {
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

#define XYZ2(x, y) ((u64)(u32)((x) << 4) | ((u64)(u32)((y) << 4) << 16) | 0xFFFFFFFF00000000ULL)

/* draw panel `id` (D_0044C160) with fixed alpha `alpha` in renderer layer `layer` */
void func_003858E0(void *s, s32 id, s32 alpha, s32 layer) {
    u16 *e = D_0044C160[(u8)id];
    u8 *tex;
    s32 slot = sub_texture(e[0], 0x19, (u8)layer, &tex);
    u64 *p;

    if (slot == -1) {
        return;
    }
    p = VCALL(D_0044E4F0, 0x10, u64 *(*)(VObject *, s32, s32))(D_0044E4F0, 0xB, (u8)layer);
    if (p == NULL) {
        return;
    }
    p[0] = 0x1000000A;              /* DMA cnt 10 */
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x5000000A;   /* VIF DIRECT 10 */
    p[2] = 4 | (1ULL << 60);        /* GIF tag: 4 A+D, EOP */
    p[3] = 0xE;
    p[4] = ((u64)(u8)alpha << 32) | 0x64;   /* ALPHA_1: (Cs - Cd) * FIX + Cd */
    p[5] = 0x42;
    p[6] = 0x60;                    /* TEX1_1: bilinear */
    p[7] = 0x14;
    p[8] = (0x80ULL << 32) | 0x8080; /* TEXA */
    p[9] = 0x3B;
    p[10] = 0x156;                  /* PRIM: sprite, textured, blended, UV */
    p[11] = 0;
    p[12] = 0x8001 | (0x84ULL << 56);   /* reglist: TEX0 CLAMP RGBAQ UV XYZ2 UV XYZ2 NOP */
    p[13] = 0xFFFFFFFFF5353186ULL;   /* (lui sign-extends) */
    p[14] = VCALL(D_0044E9A0, 0x28, u64 (*)(VObject *, s32, s32, s32, s32, s32))(
        D_0044E9A0, slot, tex[0], AT(tex, 4, u16), AT(tex, 6, u16), tex[1]);
    p[15] = 0xA | ((u64)e[1] << 4) | ((u64)(s32)(e[1] + e[3]) << 14) | ((u64)e[2] << 24)
            | ((u64)(s32)(e[2] + e[4]) << 34);   /* CLAMP_1: region clamp */
    p[16] = 0x80808080 | (1ULL << 32);
    p[17] = (u64)(u32)(e[1] << 4) | ((u64)(u32)(e[2] << 4) << 16);
    p[18] = XYZ2(e[5] + 0x700, e[6] + 0x720);
    p[19] = (u64)(u32)((e[1] + e[3]) << 4) | ((u64)(u32)((e[2] + e[4]) << 4) << 16);
    p[20] = XYZ2(e[5] + 0x700 + e[3], e[6] + 0x720 + e[4]);
    p[21] = 0;
}

/* draw part `part` (D_0044C280) at x, y with alpha `alpha`, in layer 0x30 (0x33 with `top`) */
void func_003854C0(void *s, s32 x, s32 y, s32 part, s32 alpha, s32 top) {
    u16 *e = D_0044C280[(u8)part];
    s32 layer = top ? 0x33 : 0x30;
    u8 *tex;
    s32 slot = sub_texture(e[7], 0x18, layer, &tex);
    u64 *p;
    u32 sx, sy;

    if (slot == -1) {
        return;
    }
    p = VCALL(D_0044E4F0, 0x10, u64 *(*)(VObject *, s32, s32))(D_0044E4F0, 0xC, layer);
    if (p == NULL) {
        return;
    }
    p[0] = 0x1000000B;
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x5000000B;
    p[2] = 5 | (1ULL << 60);        /* GIF tag: 5 A+D, EOP */
    p[3] = 0xE;
    switch (e[8]) {
    case 0:
        p[4] = 0x44;                /* (Cs - Cd) * As + Cd */
        break;
    case 1:
        p[4] = ((u64)(u8)alpha << 32) | 0x62;
        break;
    case 2:
        p[4] = ((u64)(u8)alpha << 32) | 0x64;
        break;
    case 3:
        p[4] = ((u64)(u8)alpha << 32) | 0x68;
        break;
    }
    p[5] = 0x42;
    p[6] = VCALL(D_0044E9A0, 0x2C, u64 (*)(VObject *, s32, s32, s32, s32))(
        D_0044E9A0, slot, AT(tex, 4, u16), AT(tex, 6, u16), tex[1]);   /* TEX1_1 */
    p[7] = 0x6;
    p[8] = 0x60;
    p[9] = 0x14;
    p[10] = (0x80ULL << 32) | 0x8080;
    p[11] = 0x3B;
    p[12] = 0x156;
    p[13] = 0;
    p[14] = 0x8001 | (0x84ULL << 56);
    p[15] = 0xFFFFFFFFF5353186ULL;
    p[16] = VCALL(D_0044E9A0, 0x30, u64 (*)(VObject *, s32, s32, s32, s32, s32, s32))(
        D_0044E9A0, slot, e[6], tex[0], AT(tex, 4, u16), AT(tex, 6, u16), tex[1]);
    sx = (u16)x + 0x700;
    sy = (u16)y + 0x720;
    p[17] = 0xA | ((u64)e[0] << 4) | ((u64)(s32)(e[0] + e[2]) << 14) | ((u64)e[1] << 24)
            | ((u64)(s32)(e[1] + e[3]) << 34);
    p[18] = 0x80808080;
    p[19] = (u64)(u32)(e[0] << 4) | ((u64)(u32)(e[1] << 4) << 16);
    p[20] = XYZ2(sx, sy);
    p[21] = (u64)(u32)((e[0] + e[2]) << 4) | ((u64)(u32)((e[1] + e[3]) << 4) << 16);
    p[22] = XYZ2(sx + e[4], sy + e[5]);
    p[23] = 0;
}

/* the fade: a black overlay over the menu for screens 0x80.., else the screen's panels
 * (D_0044C120) drawn at the fade's alpha */
static void sub_draw_fade(u8 *s) {
    u8 kind;

    kind = SUB_KIND(s);
    if (kind & 0x80) {
        /* a full-screen black overlay */
        u8 ov[0x100] __attribute__((aligned(16)));

        AT(ov, 0x24, s32) = 0;
        AT(ov, 0x0, void **) = D_0046F350;
        AT(ov, 0x4, s32) = -1;
        AT(ov, 0x10, s32) = -1;
        AT(ov, 0x14, u8) = 0;
        func_002CF390(ov, (u32)SUB_FADE(s) << 24);
        VCALL(D_0044E4F0, 0xC, void (*)(VObject *, void *, s32, s32))(D_0044E4F0, ov, 0x31, 0);
        AT(ov, 0x0, void **) = D_00469D00;
    } else {
        u8 alpha = SUB_FADE(s);

        if (alpha != 0 && kind != 0xFF) {
            s8 *panel = D_0044C120[kind & 0x7F];
            s32 i;

            for (i = 0; i < 4; i++, panel++) {
                if (*panel < 0) {
                    break;
                }
                func_003858E0(s, (u8)*panel, alpha, 0x31);
            }
        }
    }
}

/* the sound and music volume follow the fade (`vol` = 1 - fade / 255) */
static void sub_set_volume(u8 *s, f32 vol) {
    u8 *bgm;

    VCALL(D_0044E560, 0x94, void (*)(VObject *, s32))(D_0044E560, (u8)(0xFF - SUB_FADE(s)));
    bgm = D_0044E980;
    AT(bgm, 0x114, f32) = vol;
    if (vol < 0.0f) {
        AT(bgm, 0x114, f32) = 0.0f;
    }
    if (!(AT(bgm, 0x114, f32) <= 1.0f)) {
        AT(bgm, 0x114, f32) = 1.0f;
    }
    func_002D1FD0(bgm);
    if (D_00456DF0 != NULL) {
        VCALL(D_00456DF0, 0x44, void (*)(VObject *, f32))(D_00456DF0, vol);
    }
}

/* draw state: fade the screen in (the music and sound down with it); at full the screen takes
 * over (virtual +0x28, its texture uploaded) and the fade turns round (func_00397F00) */
void func_003984D0(void *self) {
    u8 *s = self;
    f32 vol;

    SUB_FADE(s) += SUB_FADESTEP(s);
    if (SUB_FADE(s) >= 0x80) {
        AT(s, 0x16F8, u8) = 0;
        SUB_FADE(s) = 0x80;
        if (AT(s, 0x4, u8) == 0
            || VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x6000000) != 2) {
            VCALL((VObject *)s, 0x28, void (*)(VObject *))((VObject *)s);
            if (AT(s, 0x4, u8) != 0) {
                VCALL(D_0044E4E8, 0x10, void (*)(VObject *, void *, s32))(D_0044E4E8, s + 0x11F40, 0x19);
            }
            D_0047B350 = 2;
            SUB_FADESTEP(s) = -0x20;
            set_state(&SUB_DRAW(s), func_00397F00);
        }
    }

    vol = 100.0f * (f32)(0xFF - SUB_FADE(s)) / 255.0f / 100.0f;
    sub_set_volume(s, vol);

    sub_draw_fade(s);
    if (gProgress != NULL) {
        Progress_ClearFlag(gProgress, 4);
    }
}

void func_00397D60(void *s);

/* draw state: run the screen (+0x1708) while fading back from black; at 0 the fade is done
 * (func_00397D60) */
void func_00397F00(void *self) {
    u8 *s = self;

    SUB_FADE(s) += SUB_FADESTEP(s);
    if (SUB_FADE(s) < 0) {
        AT(s, 0xA8C61, u8) = 0;
        SUB_FADE(s) = 0;
        AT(s, 0xA8C68, u8) = 0;
        AT(s, 0xA8DE4, u8) = 0;
        set_state(&SUB_DRAW(s), func_00397D60);
    }
    ptmf_scall(s, &SUB_STATE(s));
    sub_draw_fade(s);
    if (gProgress != NULL) {
        Progress_ClearFlag(gProgress, 4);
    }
}

/* +0x28 the screen takes over */
void func_00384C30(u8 *s) {
    AT(s, 0xA8DDD, u8) = 0;
    AT(s, 0xA8DDE, u8) = 1;
}

/* the menu buttons (pressed this frame) */
extern u32 D_0047E36C;
#define MENU_UP      0x1
#define MENU_DOWN    0x4
#define MENU_CONFIRM 0x10
#define MENU_CANCEL  0x20
#define MENU_PREV    0x40    /* the in-game menu's page switches */
#define MENU_NEXT    0x80
#define MENU_DEFAULT 0x200   /* restore the defaults */

#define SUB_FADING(s)  AT(s, 0xA8C61, u8)
#define SUB_CLOSE(s)   AT(s, 0xA8DE4, u8)   /* leave the screen */
#define OPT_CURSOR(s)  AT(s, 0xA8C56, u8)

void func_00393880(void *s);
void func_00393480(void *s);
void func_003930C0(void *s);
void func_00392B50(void *s);
void func_00392670(void *s);
void func_00392360(void *s);
void func_00394260(void *s);
void func_00391450(u8 *s, s32 editing);

#define SE(id) VCALL(D_0044E560, 0x14, void (*)(VObject *, s32, s32))(D_0044E560, id, 5)

/* in-game, not in a special scene (gProgress +0x1FBEC1) */
static s32 sub_ingame_menu(u8 *s) {
    return gProgress != NULL && AT(gProgress, 0x1FBEC1, u8) == 0 && AT(s, 0x4, u8) == 0;
}

/* state: the options list (controller, vibration, sound, brightness / position, ...): up / down
 * choose of 5, confirm opens the entry's editor, the default button asks to restore the
 * defaults; in game the page buttons switch to the other menu pages */
void func_00393BD0(void *self) {
    static void (*const sEditors[5])(void *) = {
        func_00393880, func_00393480, func_003930C0, func_00392B50, func_00392670,
    };
    u8 *s = self;

    if (!SUB_FADING(s)) {
        if (D_0047E36C & MENU_CONFIRM) {
            if (OPT_CURSOR(s) < 5) {
                set_state(&SUB_STATE(s), sEditors[OPT_CURSOR(s)]);
            }
            SE(0x2B);
        } else if (D_0047E36C & MENU_UP) {
            if (OPT_CURSOR(s) != 0) {
                OPT_CURSOR(s)--;
            } else {
                OPT_CURSOR(s) = 4;
            }
            SE(0x2A);
        } else if (D_0047E36C & MENU_DOWN) {
            OPT_CURSOR(s)++;
            if (OPT_CURSOR(s) >= 5) {
                OPT_CURSOR(s) = 0;
            }
            SE(0x2A);
        } else if (D_0047E36C & MENU_DEFAULT) {
            func_00384A90((Task *)(s + 0x97764), 0x84);
            set_state(&SUB_STATE(s), func_00392360);
            SE(0x2B);
        } else if ((D_0047E36C & MENU_NEXT) && sub_ingame_menu(s)) {
            AT(s, 0xA8C50, u8) = 0;
            set_state(&SUB_STATE(s), func_00396900);
            set_state(&AT(s, 0x1714, PTMF), func_00396900);
            SE(0x87);
        } else if ((D_0047E36C & MENU_PREV) && sub_ingame_menu(s)) {
            set_state(&SUB_STATE(s), func_00394260);
            set_state(&AT(s, 0x1714, PTMF), func_00394260);
            SE(0x87);
        } else if (D_0047E36C & MENU_CANCEL) {
            SUB_CLOSE(s) = 1;
        }
    }
    func_00391450(s, 0);
    func_00384650((Task *)(s + 0x97868), 0x46, 0x186, 0x80, func_00384B00(s + 0x97868, 0x82), 0x80, 0x30, 0x10, 0x15);
}

extern u16 D_0044B570[2][4][7];   /* [special scene][controller layout]: the action names */
extern u16 D_0044B558[5];         /* the options' rows (y) */
extern u16 func_00382970(Task *t, s32 id, s32 glyphW);

static const char sPosX[] = "X : %d";
static const char sPosY[] = "Y : %d";

#define OPT(s, i)      AT(s, 0xA8C44 + (i), s8)   /* the options being edited (system data +0x30) */
#define OPT_VOLUME(s)  AT(s, 0xA8C4C, f32)

/* one line of text in the options' text task */
static void opt_text(u8 *s, s32 x, s32 y, s32 color, s32 id) {
    Task *t = (Task *)(s + 0x97868);

    func_00384650(t, x, y, color, func_00384B00(t, id), 0x80, 0x30, 0x10, 0x15);
}

/* a line centred on x = 0x160 */
static void opt_text_centred(u8 *s, s32 y, s32 id) {
    s32 x = 0x160 - (func_00382970((Task *)(s + 0x97868), id, 0x10) >> 1);

    opt_text(s, x, y, 0x80, id);
}

/* draw the options: the panels, the controller layout's action names, vibration, sound output,
 * volume bar, screen position, and the cursor bar; `editing` highlights the current entry */
void func_00391450(u8 *s, s32 editing) {
    u8 special = 0;
    u8 kind;
    u16 (*names)[7];
    s32 i, w;
    u16 bar;

    if (gProgress != NULL && AT(gProgress, 0x1FBEC1, u8) == 1) {
        special = 1;
    }
    if (AT(s, 0x4, u8) != 0) {
        SUB_KIND(s) = 0x8A;
    } else if (gProgress != NULL && AT(gProgress, 0x1FBEC1, u8) == 1) {
        SUB_KIND(s) = 0xA;
    } else {
        SUB_KIND(s) = 3;
    }
    kind = SUB_KIND(s);
    if (kind != 0xFF) {
        s8 *panel = D_0044C120[kind & 0x7F];

        for (i = 0; i < 4; i++, panel++) {
            if (*panel < 0) {
                break;
            }
            func_003858E0(s, (u8)*panel, 0x80, 0x30);
        }
    }

    /* controller layout */
    func_003854C0(s, 0x20, 0x54, 0x10, 0x80, 0);
    opt_text(s, 0x48, 0x5A, OPT_CURSOR(s) == 0 && editing ? 0x82 : 0x80, 0x60);
    names = D_0044B570[special];
    w = 0x160 - (func_00382970((Task *)(s + 0x97868), 0x61, 0x10) >> 1);
    opt_text(s, w, 0x5A, 0x80, names[OPT(s, 6)][0]);
    opt_text(s, 0x40, 0x74, 0x80, 0x65);
    opt_text(s, 0x60, 0x74, 0x80, names[OPT(s, 6)][1]);
    opt_text(s, 0x40, 0x94, 0x80, 0x66);
    opt_text(s, 0x60, 0x94, 0x80, names[OPT(s, 6)][2]);
    opt_text(s, 0x40, 0xB4, 0x80, 0x67);
    opt_text(s, 0x60, 0xB4, 0x80, names[OPT(s, 6)][3]);
    opt_text(s, 0x40, 0xD4, 0x80, 0x68);
    opt_text(s, 0x60, 0xD4, 0x80, names[OPT(s, 6)][4]);
    opt_text(s, 0x110, 0x74, 0x80, 0x69);
    opt_text(s, 0x130, 0x74, 0x80, 0x73);
    opt_text(s, 0x110, 0x94, 0x80, 0x6A);
    opt_text(s, 0x130, 0x94, 0x80, 0x74);
    opt_text(s, 0x110, 0xB4, 0x80, 0x6B);
    opt_text(s, 0x130, 0xB4, 0x80, names[OPT(s, 6)][5]);
    opt_text(s, 0x110, 0xD4, 0x80, 0x6C);
    opt_text(s, 0x130, 0xD4, 0x80, names[OPT(s, 6)][6]);

    /* vibration */
    func_003854C0(s, 0x20, 0xF4, 0x11, 0x80, 0);
    opt_text(s, 0x48, 0xFA, OPT_CURSOR(s) == 1 && editing ? 0x82 : 0x80, 0x77);
    opt_text_centred(s, 0xFA, OPT(s, 4) == 1 ? 0x78 : 0x79);

    /* sound output */
    func_003854C0(s, 0x20, 0x114, 0x12, 0x80, 0);
    opt_text(s, 0x48, 0x11A, OPT_CURSOR(s) == 2 && editing ? 0x82 : 0x80, 0x7A);
    opt_text_centred(s, 0x11A, OPT(s, 0) == 0 ? 0x7B : OPT(s, 0) == 1 ? 0x7C : 0x7D);

    /* volume: a bar from 0x118 to 0x198 */
    func_003854C0(s, 0x20, 0x134, 0x13, 0x80, 0);
    opt_text(s, 0x48, 0x13A, OPT_CURSOR(s) == 3 && editing ? 0x82 : 0x80, 0x7E);
    w = 0x118 - func_00382970((Task *)(s + 0x97868), 0x7F, 0x10);
    opt_text(s, w, 0x13A, 0x80, 0x7F);
    opt_text(s, 0x1A8, 0x13A, 0x80, 0x80);
    bar = (u32)(128.0f * OPT_VOLUME(s));
    func_003854C0(s, (u16)(bar + 0x118), 0x134, 0x19, 0x80, 0);

    /* screen position */
    func_003854C0(s, 0x20, 0x154, 0x14, 0x80, 0);
    opt_text(s, 0x48, 0x15A, OPT_CURSOR(s) == 4 && editing ? 0x82 : 0x80, 0x81);
    func_00384800((Task *)(s + 0x97868), 0x11C, 0x15A, 0x80, sPosX, OPT(s, 2));
    func_00384800((Task *)(s + 0x97868), 0x170, 0x15A, 0x80, sPosY, OPT(s, 3));

    /* the cursor bar */
    func_003854C0(s, 0x20, D_0044B558[OPT_CURSOR(s)], 0x1C, 0x80, 0);
}

void func_003977D0(void *s);

/* draw state: the screen is up; run it until it asks to close (+0xA8DE4, or in game the
 * menu button: Progress flag 4), then fade out (func_003977D0) */
void func_00397D60(void *self) {
    u8 *s = self;
    Progress *p = gProgress;

    if (p != NULL && AT(s, 0x4, u8) != 0) {
        Progress_ClearFlag(p, 4);
    }
    ptmf_scall(s, &SUB_STATE(s));
    if (SUB_CLOSE(s) != 1 && (p == NULL || !Progress_TestFlag(p, 4))) {
        return;
    }
    if (p != NULL && AT(p, 0x1FBEC1, u8) == 0 && AT(s, 0x4, u8) == 0) {
        AT(s, 0xA8C67, u8) = SUB_KIND(s);   /* reopen on this page */
    }
    SUB_FADING(s) = 1;
    SUB_FADE(s) = 0;
    SUB_FADESTEP(s) = 0x20;
    set_state(&SUB_DRAW(s), func_003977D0);
    if (p != NULL) {
        Progress_ClearFlag(p, 4);
    }
    if (AT(s, 0xA8C68, u8) == 0) {
        SE(0x95);
    }
}

extern VObject *D_0044E4F8;
void func_003974A0(void *s);

/* draw state: fade back to black over the screen; at black restore the menu's background
 * texture (the screens that replaced it) and fade the game back in (func_003974A0) */
void func_003977D0(void *self) {
    u8 *s = self;
    u8 mode;

    SUB_FADE(s) += SUB_FADESTEP(s);
    if (SUB_FADE(s) < 0x80) {
        ptmf_scall(s, &SUB_STATE(s));
    } else {
        mode = AT(s, 0x4, u8);
        if (mode != 0 && mode != 5) {
            sub_free_vram();
            sub_load(s, sSubBack, s + 0x11F40);
        }
        D_0047B350 = 1;
        AT(s, 0x16F8, u8) = 1;
        SUB_FADE(s) = 0x80;
        SUB_FADESTEP(s) = -0x10;
        mode = AT(s, 0x4, u8);
        if (mode == 0xB || mode == 8 || (u32)(mode - 0xD) < 3) {
            VCALL(D_0044E4F8, 0x30, void (*)(VObject *, s32))(D_0044E4F8, 0);
        }
        if (AT(s, 0x4, u8) == 8) {
            SUB_FADE(s) = 0;
            Progress_SetFlag(gProgress, 8);
        }
        set_state(&SUB_DRAW(s), func_003974A0);
    }
    sub_draw_fade(s);
    if (gProgress != NULL) {
        Progress_ClearFlag(gProgress, 4);
    }
}


/* draw state: fade the menu back in (once the background texture is loaded); at the end the
 * screen is closed (in game: Progress flag 4) and the next update opens it again
 * (func_00398850) */
void func_003974A0(void *self) {
    u8 *s = self;
    f32 vol;

    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x6000000) != 2) {
        SUB_FADE(s) += SUB_FADESTEP(s);
    }
    if (SUB_FADE(s) < 0) {
        SUB_FADE(s) = 0;
        VCALL(D_0044E4E8, 0x10, void (*)(VObject *, void *, s32))(D_0044E4E8, s + 0x11F40, 0x19);
        set_state(&SUB_DRAW(s), func_00398850);
        if (gProgress != NULL) {
            Progress_SetFlag(gProgress, 4);
        } else {
            AT(s, 0xA8DE5, u8) = 0;
        }
        vol = 1.0f;
    } else {
        sub_draw_fade(s);
        if (gProgress != NULL) {
            Progress_ClearFlag(gProgress, 4);
        }
        vol = 100.0f * (f32)(0xFF - SUB_FADE(s)) / 255.0f / 100.0f;
    }
    sub_set_volume(s, vol);
}

extern void func_002BC460(void *card, s32 arg);
extern void func_002BCAC0(void *card);

/* state: the save-data screen (the card UI at +0xA8AC0) for loading; when it finishes
 * (+0xA8AC4 < 0) the screen closes */
void func_00391250(void *self) {
    u8 *s = self;

    SUB_KIND(s) = 0x85;
    if (AT(s, 0xA8AC4, s32) >= 0) {
        func_002BCAC0(s + 0xA8AC0);
        return;
    }
    if (!SUB_FADING(s)) {
        SUB_CLOSE(s) = 1;
    }
    func_002BC460(s + 0xA8AC0, 1);
}
