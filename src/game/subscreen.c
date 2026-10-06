/* The sub screen: the menu system of the title and of the in-game menus (vtable D_0047A790, at
 * SceneTitle +0x74A80; code 0x384C00..0x3A0000). Its textures are SUBSCR\SUBBASE.TEX and
 * SUBSCR\SUBBACK.TEX. */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "progress.h"
#include "task.h"
#include "subscreen.h"
#include "input.h"
#include "sound.h"
#include "gs.h"
#include "texcache.h"


extern VObject *gFileLoader;
extern u8 *D_0044E978;          /* the system data; +0x30 the options */
extern VObject *D_0044E7A8;     /* the pad actuator (vibration) */
extern VObject *D_00456DF0;
extern void *D_0044E958;        /* the movie playing */
extern void *D_0044E980;        /* the ADX sound system (music) */
extern void *D_0046C790[];      /* a pool entry */
extern void func_002B6340(void *movie);   /* apply the movie volume */
extern void func_002D1FD0(void *bgm);     /* apply the music volume */
extern void func_00120EC0(void *pool, u8 *base, u32 size, u32 n, u8 *used);

void func_00396900(SubScreen *s);
void SubScreen_DrawLoadWait(SubScreen *s);
void SubScreen_Open(SubScreen *s);

static const char sSubBase[] = "SUBSCR\\SUBBASE.TEX";
static const char sSubBack[] = "SUBSCR\\SUBBACK.TEX";

/* placement new (pool entries) */
void *func_0025FF00(u32 size, void *p) {
    return p;
}

/* the entries' pool: the first 64 entries constructed again, the tables cleared, the block
 * pool (+0x1208) over all 192 */
void SubPool_Reset(u8 *pool) {
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
void SubScreen_Start(SubScreen *s) {
    VObject *ld = gFileLoader;
    s32 i;

    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sSubBase, s->baseTex, 0x6000000, 0);
    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sSubBack, s->pageTex, 0x6000000, 0);
    SubPool_Reset(s->pool);
    for (i = 0; i < 9; i++) {
        s->unk97740[i] = 0;
    }
    for (i = 0; i < 0x80; i++) {
        s->unk15F8[i] = 0;
    }
    s->mode = 0;
    s->frame = 0;
    s->showBehind = 1;
    s->unkA8C50 = 0;
    for (i = 0; i < 3; i++) {
        s->unkA8C51[i] = 0;
    }
    s->unkA8C55 = 0;
    s->optCursor = 0;
    s->open = 0;
    s->resumeKind = 0;
    s->kind = 0;
    ptmf_set_fn(&s->resume, func_00396900);
    ptmf_set_fn(&s->draw, SubScreen_DrawLoadWait);
}

/* the master volume (0..1): sound effects, voices (D_00456DF0), the movie, the music */
static void opt_apply_volume(VObject *snd, f32 vol) {
    VCALL(snd, 0xA8, void (*)(VObject *, f32))(snd, vol);
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
}

/* apply the options: controller layout, vibration, sound output, volume, screen position */
void SubScreen_ApplyOptions(VObject *s) {
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
    opt_apply_volume(o, vol);
    VCALL(D_0044E4F0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4F0, (s8)opt[2], (s8)opt[3]);
}

extern u16 D_0044B5E0[][6][2];   /* per controller layout: 6 x (button, its name's message) */
extern u8 D_0047E3C0[];          /* button mapping (+0xA: the six configurable actions) */
extern u8 D_0047B150[6];         /* the name slot of each action */
extern void Msg_SetName(void *self, s32 slot, s32 id);

/* +0x34 apply controller layout `type`: map the six actions and name their buttons */
void SubScreen_SetLayout(SubScreen *s, s32 type) {
    s32 i;

    for (i = 0; i < 6; i++) {
        D_0047E3C0[0xA + i] = D_0044B5E0[type][i][0];
        Msg_SetName(&s->ask, D_0047B150[i], D_0044B5E0[type][i][1]);
    }
}

extern void *func_00322570(u32 size, void *p);   /* placement new */
extern void func_00305380(void *p);
extern void SaveScreen_Init(void *card, void *buf, void *buf2);
extern void func_0038F7D0(void *s);
extern void func_00388D30(void *s);
extern void *D_00474020[];       /* the 0x15C helper's base vtable */
extern void *D_00474040[];
extern void *D_00474060[];

void Options_StateList(SubScreen *s);
void func_003912E0(SubScreen *s);
void func_003908B0(SubScreen *s);
void func_00390790(SubScreen *s);
void SubScreen_StateLoad(SubScreen *s);
void func_0038FBE0(SubScreen *s);
void func_0038E0C0(SubScreen *s);
void func_003894F0(SubScreen *s);
void func_0038D7E0(SubScreen *s);
void func_00388FF0(SubScreen *s);
void func_003888A0(SubScreen *s);
void func_00387F00(SubScreen *s);
void func_00386150(SubScreen *s);
void SubScreen_DrawFadeIn(SubScreen *s);
void func_00398100(SubScreen *s);

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


static void sub_load(SubScreen *s, const char *name, void *dst) {
    VObject *ld = gFileLoader;

    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, name, dst, 0x6000000, 0);
}

static void sub_free_vram(void) {
    VCALL(D_0044E4E8, 0x14, void (*)(VObject *, s32))(D_0044E4E8, 0x19);
}

static void sub_se(void) {
    Sound_Play(D_0044E560, SE_OPEN, SE_BANK_MENU);
}

/* the options being edited: a copy of the system data's */
static void sub_copy_options(SubScreen *s) {
    u8 *sys = D_0044E978;
    s32 i;

    for (i = 0; i < 7; i++) {
        s->opt[i] = sys[0x30 + i];
    }
    s->optVolume = AT(sys, 0x38, f32);
}

/* the save slot screen's helper object (0x15C bytes at +0xA8C80) */
static void sub_new_slots(SubScreen *s, void **vtbl) {
    u8 *p = func_00322570(0x15C, s->page);

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
void SubScreen_Open(SubScreen *s) {
    u8 *sys;
    u8 n;

    s->open = 1;
    switch (s->mode) {
    case 0:
        sub_copy_options(s);
        if (gProgress != NULL && AT(gProgress, 0x1FBEC1, u8) == 1) {
            s->kind = 0xA;
            ptmf_set_fn(&s->state, Options_StateList);
        } else {
            func_00305380(s->textObj);
            s->kind = s->resumeKind;
            s->state = s->resume;
        }
        sub_se();
        break;
    case 1:
        sub_free_vram();
        sub_load(s, sSubSave, s->pageTex);
        SaveScreen_Init(&s->card, s->saveBuf0, s->saveBuf1);
        s->card.state = 0;
        s->card.hidden = 0;
        s->kind = 0x85;
        ptmf_set_fn(&s->state, func_003912E0);
        break;
    case 2:
        sub_free_vram();
        sub_load(s, sPlate, s->pageTex);
        s->unkA8C57 = 0;
        for (n = 0; n < 8; n++) {
            s->unkA8C58[n] = 0;
        }
        s->unkA8C60 = 0;
        s->kind = 0x84;
        ptmf_set_fn(&s->state, func_003908B0);
        sub_se();
        break;
    case 3:
        sub_free_vram();
        sub_load(s, sSynSlot, s->pageTex);
        sub_new_slots(s, D_00474060);
        s->kind = 0x86;
        ptmf_set_fn(&s->state, func_00390790);
        sub_se();
        break;
    case 4:
        sub_free_vram();
        sub_load(s, sSynSlot, s->pageTex);
        sub_new_slots(s, D_00474040);
        s->kind = 0x87;
        ptmf_set_fn(&s->state, func_00390790);
        sub_se();
        break;
    case 5:
        sub_copy_options(s);
        sub_se();
        s->kind = 0x8A;
        ptmf_set_fn(&s->state, Options_StateList);
        break;
    case 6:
        sub_free_vram();
        sub_load(s, sSubSave, s->pageTex);
        SaveScreen_Init(&s->card, NULL, NULL);
        s->card.state = 0;
        s->card.hidden = 0;
        s->kind = 0x85;
        ptmf_set_fn(&s->state, SubScreen_StateLoad);
        break;
    case 7:
        sub_free_vram();
        sub_load(s, sSubMg, s->pageTex);
        s->kind = 0x80;
        func_0038F7D0(s);
        ptmf_set_fn(&s->state, func_0038FBE0);
        break;
    case 8: {
        VObject *ld;
        Progress *p;

        sub_free_vram();
        ld = gFileLoader;
        VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sGalMovie, s->pageTex, 0x6000000, 0);
        p = gProgress;
        VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sThumbnail, (u8 *)p + 0x16C0, 0x6000000, 0);
        s->kind = 0x8B;
        s->page[0] = Progress_GetVar(p, 0x2B);
        ptmf_set_fn(&s->state, func_0038E0C0);
        break;
    }
    case 9:
        SaveScreen_Init(&s->card, s->saveBuf0, s->saveBuf1);
        s->card.state = 0;
        s->card.hidden = 1;
        s->kind = 0x85;
        ptmf_set_fn(&s->state, func_003912E0);
        break;
    case 10:
        /* the extras menu: entries 0..2, then 3 and 4 when unlocked, then 5 */
        sub_free_vram();
        sub_load(s, sSubMg, s->pageTex);
        s->kind = 0x80;
        s->page[0] = 0;
        s->page[1] = 1;
        s->page[2] = 2;
        n = 3;
        sys = D_0044E978;
        if ((AT(sys, 0x2C, u32) & 0x40000) != 0) {
            s->page[3] = 3;
            n++;
        }
        if ((AT(sys, 0x2C, u32) & 0x80000) != 0) {
            s->page[n] = 4;
            n++;
        }
        s->page[n] = 5;
        s->page[6] = n + 1;
        s->page[7] = 0;
        ptmf_set_fn(&s->state, func_003894F0);
        break;
    case 11:
        sub_free_vram();
        sub_load(s, sGalModel, s->pageTex);
        s->kind = 0x8C;
        s->page[0] = 0;
        ptmf_set_fn(&s->state, func_0038D7E0);
        break;
    case 12:
        s->kind = 0x80;
        func_00388D30(s);
        ptmf_set_fn(&s->state, func_00388FF0);
        break;
    case 13:
        sub_free_vram();
        sub_load(s, sGalMusic, s->pageTex);
        s->kind = 0x8D;
        s->page[0] = 0;
        s->page[2] = 0xFF;
        s->page[1] = 0xFF;
        ptmf_set_fn(&s->state, func_003888A0);
        break;
    case 14: {
        VObject *ld;
        void *buf;

        sub_free_vram();
        ld = gFileLoader;
        VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sGalArt, s->pageTex, 0x6000000, 0);
        s->kind = 0x8E;
        s->page[0] = 0;
        s->page[1] = 0;
        s->page[2] = 0;
        s->page[3] = 1;
        buf = VCALL((VObject *)gProgress, 0x88, void *(*)(VObject *))((VObject *)gProgress);
        VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sArtLen, buf, 0x6000000, 0);
        SUB_PAGE(s, 4, s16) = 0;
        SUB_PAGE(s, 6, s16) = 0;
        s->page[8] = 0;
        s->page[9] = 1;
        s->page[10] = 0;
        s->page[11] = 0;
        s->page[12] = 0;
        ptmf_set_fn(&s->state, func_00387F00);
        break;
    }
    case 15:
        sub_free_vram();
        sub_load(s, sGalType, s->pageTex);
        s->kind = 0x8F;
        s->page[0] = 0;
        ptmf_set_fn(&s->state, func_00386150);
        break;
    }
    s->showBehind = 1;
    s->fading = 1;
    s->fade = 0;
    s->fadeStep = 0x10;
    ptmf_set_fn(&s->draw, SubScreen_DrawFadeIn);
    if (s->mode == 7 || s->mode == 0xC || s->mode == 0xA) {
        s->fadeStep = 8;
        ptmf_set_fn(&s->draw, func_00398100);
    }
    if (gProgress != NULL) {
        Progress_ClearFlag(gProgress, 4);
    }
}

/* per frame: run the draw/step state (+0x16FC); true while the menu behind should be drawn */
s32 SubScreen_Update(SubScreen *s) {
    ptmf_scall(s, &s->draw);
    s->frame++;
    return s->showBehind;
}

/* state: wait for the textures, upload them (VRAM slots 0x18, 0x19), set the screen up */
void SubScreen_DrawLoadWait(SubScreen *s) {
    VObject *tc;

    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x6000000) == 2) {
        return;
    }
    tc = D_0044E4E8;
    VCALL(tc, 0x10, void (*)(VObject *, void *, s32))(tc, s->baseTex, 0x18);
    VCALL(tc, 0x10, void (*)(VObject *, void *, s32))(tc, s->pageTex, 0x19);
    SubScreen_Open(s);
}

extern s8 D_0044C120[][4];       /* per screen kind: up to 4 panels to draw while fading */
extern u8 D_0047B350;
extern void *D_0046F350[];       /* overlay vtable */
extern void *D_00469D00[];       /* its base */
extern void func_002CF390(void *ov, u32 rgba);
void SubScreen_DrawFadeFromBlack(SubScreen *s);


extern VObject *D_0044E9A0;   /* the VRAM manager */

/* a fixed piece of the screen (SUBBACK.TEX, VRAM group 0x19): texture, u, v, w, h, x, y */
extern u16 D_0044C160[][7];
/* a movable part (SUBBASE.TEX, group 0x18): u, v, w, h, screen w, h, CLUT, texture, blending
 * (0 normal, 1..3 fixed alpha variants) */
extern u16 D_0044C280[][9];

#ifdef HG_NATIVE
#include "gl2d.h"

/* draw panel `id` (D_0044C160: its w x h texels at u, v, at x, y) mixed over the screen by
 * fixed alpha `alpha`, in renderer layer `layer` */
void SubScreen_DrawPanel(void *s, s32 id, s32 alpha, s32 layer) {
    u16 *e = D_0044C160[(u8)id];
    u8 *tex;
    s32 slot = TexCache_Resident(e[0], 0x19, (u8)layer, &tex);

    if (slot == -1) {
        return;
    }
    gl2d_sprite((u8)layer, e[5], e[6], e[5] + e[3], e[6] + e[4], tex, e[1], e[2], e[1] + e[3], e[2] + e[4],
                0x80808080, 0, gl2d_blend(((u64)(u8)alpha << 32) | 0x64));
}

/* draw part `part` (D_0044C280) at x, y with alpha `alpha`, in layer 0x30 (0x33 with `top`):
 * its blending 0 normal, 1 subtracted / 2 mixed / 3 added by the fixed alpha */
void SubScreen_DrawPart(void *s, s32 x, s32 y, s32 part, s32 alpha, s32 top) {
    static const u8 kAlpha[4] = {0x44, 0x62, 0x64, 0x68};
    u16 *e = D_0044C280[(u8)part];
    s32 layer = top ? 0x33 : 0x30;
    u8 *tex;
    s32 slot = TexCache_Resident(e[7], 0x18, layer, &tex);
    s16 sx = x, sy = y;

    if (slot == -1) {
        return;
    }
    gl2d_sprite(layer, sx, sy, sx + e[4], sy + e[5], tex, e[0], e[1], e[0] + e[2], e[1] + e[3], 0x80808080, e[6],
                gl2d_blend(((u64)(u8)alpha << 32) | kAlpha[e[8] & 3]));
}
#else
void SubScreen_DrawPanel(void *s, s32 id, s32 alpha, s32 layer);
void SubScreen_DrawPart(void *s, s32 x, s32 y, s32 part, s32 alpha, s32 top);
#endif

/* the fade: a black overlay over the menu for screens 0x80.., else the screen's panels
 * (D_0044C120) drawn at the fade's alpha */
static void sub_draw_fade(SubScreen *s) {
    u8 kind;

    kind = s->kind;
    if (kind & 0x80) {
        /* a full-screen black overlay */
        u8 ov[0x100] __attribute__((aligned(16)));

        AT(ov, 0x24, s32) = 0;
        AT(ov, 0x0, void **) = D_0046F350;
        AT(ov, 0x4, s32) = -1;
        AT(ov, 0x10, s32) = -1;
        AT(ov, 0x14, u8) = 0;
        func_002CF390(ov, (u32)s->fade << 24);
        VCALL(D_0044E4F0, 0xC, void (*)(VObject *, void *, s32, s32))(D_0044E4F0, ov, 0x31, 0);
        AT(ov, 0x0, void **) = D_00469D00;
    } else {
        u8 alpha = s->fade;

        if (alpha != 0 && kind != 0xFF) {
            s8 *panel = D_0044C120[kind & 0x7F];
            s32 i;

            for (i = 0; i < 4; i++, panel++) {
                if (*panel < 0) {
                    break;
                }
                SubScreen_DrawPanel(s, (u8)*panel, alpha, 0x31);
            }
        }
    }
}

/* the sound and music volume follow the fade (`vol` = 1 - fade / 255) */
static void sub_set_volume(SubScreen *s, f32 vol) {
    u8 *bgm;

    VCALL(D_0044E560, 0x94, void (*)(VObject *, s32))(D_0044E560, (u8)(0xFF - s->fade));
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
 * over (virtual +0x28, its texture uploaded) and the fade turns round (SubScreen_DrawFadeFromBlack) */
void SubScreen_DrawFadeIn(SubScreen *s) {
    f32 vol;

    s->fade += s->fadeStep;
    if (s->fade >= 0x80) {
        s->showBehind = 0;
        s->fade = 0x80;
        if (s->mode == 0
            || VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x6000000) != 2) {
            VCALL((VObject *)s, 0x28, void (*)(VObject *))((VObject *)s);
            if (s->mode != 0) {
                VCALL(D_0044E4E8, 0x10, void (*)(VObject *, void *, s32))(D_0044E4E8, s->pageTex, 0x19);
            }
            D_0047B350 = 2;
            s->fadeStep = -0x20;
            ptmf_set_fn(&s->draw, SubScreen_DrawFadeFromBlack);
        }
    }

    vol = 100.0f * (f32)(0xFF - s->fade) / 255.0f / 100.0f;
    sub_set_volume(s, vol);

    sub_draw_fade(s);
    if (gProgress != NULL) {
        Progress_ClearFlag(gProgress, 4);
    }
}

void SubScreen_DrawRun(SubScreen *s);

/* draw state: run the screen (+0x1708) while fading back from black; at 0 the fade is done
 * (SubScreen_DrawRun) */
void SubScreen_DrawFadeFromBlack(SubScreen *s) {

    s->fade += s->fadeStep;
    if (s->fade < 0) {
        s->fading = 0;
        s->fade = 0;
        s->quietClose = 0;
        s->close = 0;
        ptmf_set_fn(&s->draw, SubScreen_DrawRun);
    }
    ptmf_scall(s, &s->state);
    sub_draw_fade(s);
    if (gProgress != NULL) {
        Progress_ClearFlag(gProgress, 4);
    }
}

/* +0x28 the screen takes over */
void SubScreen_TakeOver(SubScreen *s) {
    s->tab = 0;
    s->unkA8DDE = 1;
}



void Options_StateLayout(SubScreen *s);
void Options_StateVibration(SubScreen *s);
void Options_StateSound(SubScreen *s);
void Options_StateVolume(SubScreen *s);
void Options_StatePosition(SubScreen *s);
void Options_StateDefaults(SubScreen *s);
void func_00394260(SubScreen *s);
void Options_Draw(SubScreen *s, s32 editing);


/* in-game, not in a special scene (gProgress +0x1FBEC1) */
static s32 sub_ingame_menu(SubScreen *s) {
    return gProgress != NULL && AT(gProgress, 0x1FBEC1, u8) == 0 && s->mode == 0;
}

/* state: the options list (controller, vibration, sound, brightness / position, ...): up / down
 * choose of 5, confirm opens the entry's editor, the default button asks to restore the
 * defaults; in game the page buttons switch to the other menu pages */
void Options_StateList(SubScreen *s) {
    static void (*const sEditors[5])(SubScreen *) = {
        Options_StateLayout, Options_StateVibration, Options_StateSound, Options_StateVolume, Options_StatePosition,
    };

    if (!s->fading) {
        if (D_0047E36C & MENU_CONFIRM) {
            if (s->optCursor < 5) {
                ptmf_set_fn(&s->state, sEditors[s->optCursor]);
            }
            Sound_PlaySE(SE_DECIDE);
        } else if (D_0047E36C & MENU_UP) {
            if (s->optCursor != 0) {
                s->optCursor--;
            } else {
                s->optCursor = 4;
            }
            Sound_PlaySE(SE_CURSOR);
        } else if (D_0047E36C & MENU_DOWN) {
            s->optCursor++;
            if (s->optCursor >= 5) {
                s->optCursor = 0;
            }
            Sound_PlaySE(SE_CURSOR);
        } else if (D_0047E36C & MENU_DEFAULT) {
            Task_Open(&s->ask, 0x84);
            ptmf_set_fn(&s->state, Options_StateDefaults);
            Sound_PlaySE(SE_DECIDE);
        } else if ((D_0047E36C & MENU_NEXT) && sub_ingame_menu(s)) {
            s->unkA8C50 = 0;
            ptmf_set_fn(&s->state, func_00396900);
            ptmf_set_fn(&s->resume, func_00396900);
            Sound_PlaySE(SE_PAGE);
        } else if ((D_0047E36C & MENU_PREV) && sub_ingame_menu(s)) {
            ptmf_set_fn(&s->state, func_00394260);
            ptmf_set_fn(&s->resume, func_00394260);
            Sound_PlaySE(SE_PAGE);
        } else if (D_0047E36C & MENU_CANCEL) {
            s->close = 1;
        }
    }
    Options_Draw(s, 0);
    Task_ShowText(&s->text, 0x46, 0x186, 0x80, Task_MessageText(&s->text, 0x82), 0x80, 0x30, 0x10, 0x15);
}

extern u16 D_0044B570[2][4][7];   /* [special scene][controller layout]: the action names */
extern u16 D_0044B558[5];         /* the options' rows (y) */

static const char sPosX[] = "X : %d";
static const char sPosY[] = "Y : %d";


/* one line of text in the options' text task */
static void opt_text(SubScreen *s, s32 x, s32 y, s32 color, s32 id) {
    Task *t = &s->text;

    Task_ShowText(t, x, y, color, Task_MessageText(t, id), 0x80, 0x30, 0x10, 0x15);
}

/* a line centred on x = 0x160 */
static void opt_text_centred(SubScreen *s, s32 y, s32 id) {
    s32 x = 0x160 - (Task_MessageWidth(&s->text, id, 0x10) >> 1);

    opt_text(s, x, y, 0x80, id);
}

/* draw the options: the panels, the controller layout's action names, vibration, sound output,
 * volume bar, screen position, and the cursor bar; `editing` highlights the current entry */
void Options_Draw(SubScreen *s, s32 editing) {
    u8 special = 0;
    u8 kind;
    u16 (*names)[7];
    s32 i, w;
    u16 bar;

    if (gProgress != NULL && AT(gProgress, 0x1FBEC1, u8) == 1) {
        special = 1;
    }
    if (s->mode != 0) {
        s->kind = 0x8A;
    } else if (gProgress != NULL && AT(gProgress, 0x1FBEC1, u8) == 1) {
        s->kind = 0xA;
    } else {
        s->kind = 3;
    }
    kind = s->kind;
    if (kind != 0xFF) {
        s8 *panel = D_0044C120[kind & 0x7F];

        for (i = 0; i < 4; i++, panel++) {
            if (*panel < 0) {
                break;
            }
            SubScreen_DrawPanel(s, (u8)*panel, 0x80, 0x30);
        }
    }

    /* controller layout */
    SubScreen_DrawPart(s, 0x20, 0x54, 0x10, 0x80, 0);
    opt_text(s, 0x48, 0x5A, s->optCursor == 0 && editing ? 0x82 : 0x80, 0x60);
    names = D_0044B570[special];
    w = 0x160 - (Task_MessageWidth(&s->text, 0x61, 0x10) >> 1);
    opt_text(s, w, 0x5A, 0x80, names[s->opt[6]][0]);
    opt_text(s, 0x40, 0x74, 0x80, 0x65);
    opt_text(s, 0x60, 0x74, 0x80, names[s->opt[6]][1]);
    opt_text(s, 0x40, 0x94, 0x80, 0x66);
    opt_text(s, 0x60, 0x94, 0x80, names[s->opt[6]][2]);
    opt_text(s, 0x40, 0xB4, 0x80, 0x67);
    opt_text(s, 0x60, 0xB4, 0x80, names[s->opt[6]][3]);
    opt_text(s, 0x40, 0xD4, 0x80, 0x68);
    opt_text(s, 0x60, 0xD4, 0x80, names[s->opt[6]][4]);
    opt_text(s, 0x110, 0x74, 0x80, 0x69);
    opt_text(s, 0x130, 0x74, 0x80, 0x73);
    opt_text(s, 0x110, 0x94, 0x80, 0x6A);
    opt_text(s, 0x130, 0x94, 0x80, 0x74);
    opt_text(s, 0x110, 0xB4, 0x80, 0x6B);
    opt_text(s, 0x130, 0xB4, 0x80, names[s->opt[6]][5]);
    opt_text(s, 0x110, 0xD4, 0x80, 0x6C);
    opt_text(s, 0x130, 0xD4, 0x80, names[s->opt[6]][6]);

    /* vibration */
    SubScreen_DrawPart(s, 0x20, 0xF4, 0x11, 0x80, 0);
    opt_text(s, 0x48, 0xFA, s->optCursor == 1 && editing ? 0x82 : 0x80, 0x77);
    opt_text_centred(s, 0xFA, s->opt[4] == 1 ? 0x78 : 0x79);

    /* sound output */
    SubScreen_DrawPart(s, 0x20, 0x114, 0x12, 0x80, 0);
    opt_text(s, 0x48, 0x11A, s->optCursor == 2 && editing ? 0x82 : 0x80, 0x7A);
    opt_text_centred(s, 0x11A, s->opt[0] == 0 ? 0x7B : s->opt[0] == 1 ? 0x7C : 0x7D);

    /* volume: a bar from 0x118 to 0x198 */
    SubScreen_DrawPart(s, 0x20, 0x134, 0x13, 0x80, 0);
    opt_text(s, 0x48, 0x13A, s->optCursor == 3 && editing ? 0x82 : 0x80, 0x7E);
    w = 0x118 - Task_MessageWidth(&s->text, 0x7F, 0x10);
    opt_text(s, w, 0x13A, 0x80, 0x7F);
    opt_text(s, 0x1A8, 0x13A, 0x80, 0x80);
    bar = (u32)(128.0f * s->optVolume);
    SubScreen_DrawPart(s, (u16)(bar + 0x118), 0x134, 0x19, 0x80, 0);

    /* screen position */
    SubScreen_DrawPart(s, 0x20, 0x154, 0x14, 0x80, 0);
    opt_text(s, 0x48, 0x15A, s->optCursor == 4 && editing ? 0x82 : 0x80, 0x81);
    Task_Printf(&s->text, 0x11C, 0x15A, 0x80, sPosX, s->opt[2]);
    Task_Printf(&s->text, 0x170, 0x15A, 0x80, sPosY, s->opt[3]);

    /* the cursor bar */
    SubScreen_DrawPart(s, 0x20, D_0044B558[s->optCursor], 0x1C, 0x80, 0);
}

void SubScreen_DrawFadeOut(SubScreen *s);

/* draw state: the screen is up; run it until it asks to close (+0xA8DE4, or in game the
 * menu button: Progress flag 4), then fade out (SubScreen_DrawFadeOut) */
void SubScreen_DrawRun(SubScreen *s) {
    Progress *p = gProgress;

    if (p != NULL && s->mode != 0) {
        Progress_ClearFlag(p, 4);
    }
    ptmf_scall(s, &s->state);
    if (s->close != 1 && (p == NULL || !Progress_TestFlag(p, 4))) {
        return;
    }
    if (p != NULL && AT(p, 0x1FBEC1, u8) == 0 && s->mode == 0) {
        s->resumeKind = s->kind;   /* reopen on this page */
    }
    s->fading = 1;
    s->fade = 0;
    s->fadeStep = 0x20;
    ptmf_set_fn(&s->draw, SubScreen_DrawFadeOut);
    if (p != NULL) {
        Progress_ClearFlag(p, 4);
    }
    if (s->quietClose == 0) {
        Sound_PlaySE(SE_CLOSE);
    }
}

extern VObject *D_0044E4F8;
void SubScreen_DrawFadeBack(SubScreen *s);

/* draw state: fade back to black over the screen; at black restore the menu's background
 * texture (the screens that replaced it) and fade the game back in (SubScreen_DrawFadeBack) */
void SubScreen_DrawFadeOut(SubScreen *s) {
    u8 mode;

    s->fade += s->fadeStep;
    if (s->fade < 0x80) {
        ptmf_scall(s, &s->state);
    } else {
        mode = s->mode;
        if (mode != 0 && mode != 5) {
            sub_free_vram();
            sub_load(s, sSubBack, s->pageTex);
        }
        D_0047B350 = 1;
        s->showBehind = 1;
        s->fade = 0x80;
        s->fadeStep = -0x10;
        mode = s->mode;
        if (mode == 0xB || mode == 8 || (u32)(mode - 0xD) < 3) {
            VCALL(D_0044E4F8, 0x30, void (*)(VObject *, s32))(D_0044E4F8, 0);
        }
        if (s->mode == 8) {
            s->fade = 0;
            Progress_SetFlag(gProgress, 8);
        }
        ptmf_set_fn(&s->draw, SubScreen_DrawFadeBack);
    }
    sub_draw_fade(s);
    if (gProgress != NULL) {
        Progress_ClearFlag(gProgress, 4);
    }
}


/* draw state: fade the menu back in (once the background texture is loaded); at the end the
 * screen is closed (in game: Progress flag 4) and the next update opens it again
 * (SubScreen_Open) */
void SubScreen_DrawFadeBack(SubScreen *s) {
    f32 vol;

    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x6000000) != 2) {
        s->fade += s->fadeStep;
    }
    if (s->fade < 0) {
        s->fade = 0;
        VCALL(D_0044E4E8, 0x10, void (*)(VObject *, void *, s32))(D_0044E4E8, s->pageTex, 0x19);
        ptmf_set_fn(&s->draw, SubScreen_Open);
        if (gProgress != NULL) {
            Progress_SetFlag(gProgress, 4);
        } else {
            s->open = 0;
        }
        vol = 1.0f;
    } else {
        sub_draw_fade(s);
        if (gProgress != NULL) {
            Progress_ClearFlag(gProgress, 4);
        }
        vol = 100.0f * (f32)(0xFF - s->fade) / 255.0f / 100.0f;
    }
    sub_set_volume(s, vol);
}

extern void SaveScreen_Draw(void *card, s32 arg);
extern void SaveScreen_Load(void *card);

/* state: the save-data screen (the card UI at +0xA8AC0) for loading; when it finishes
 * (+0xA8AC4 < 0) the screen closes */
void SubScreen_StateLoad(SubScreen *s) {

    s->kind = 0x85;
    if (s->card.state >= 0) {
        SaveScreen_Load(&s->card);
        return;
    }
    if (!s->fading) {
        s->close = 1;
    }
    SaveScreen_Draw(&s->card, 1);
}

/* the blinking of the editors' arrows: |0x80 - (frame * 4 & 0xFF)| */
static u8 opt_blink(SubScreen *s) {
    s32 a = 0x80 - (u8)(s->frame << 2);

    return a > 0 ? (u8)a : (u8)-a;
}

/* an editor's frame: the options with the entry lit, blinking arrows either side of the value,
 * the help line */
static void opt_editor_draw(SubScreen *s, s32 xl, s32 xr, s32 y) {
    u8 a;

    Options_Draw(s, 1);
    a = opt_blink(s);
    SubScreen_DrawPart(s, xl, y, 0x1A, a, 0);
    SubScreen_DrawPart(s, xr, y, 0x1B, a, 0);
    opt_text(s, 0x46, 0x186, 0x80, 0x83);
}

/* in-game menu button: closes the editor like cancel */
static s32 opt_cancelled(void) {
    return (D_0047E36C & MENU_CANCEL) || (gProgress != NULL && Progress_TestFlag(gProgress, 4));
}

/* leave an editor: back to the list */
static void opt_back(SubScreen *s, s32 se) {
    ptmf_set_fn(&s->state, Options_StateList);
    Sound_PlaySE(se);
}

/* editing the controller layout (4 types): left / right choose, confirm applies it, cancel
 * (or the in-game menu button) restores it */
void Options_StateLayout(SubScreen *s) {
    s8 *opt = (s8 *)D_0044E978 + 0x30;

    if (!s->fading) {
        if (D_0047E36C & MENU_LEFT) {
            if (s->opt[6] != 0) {
                s->opt[6]--;
            } else {
                s->opt[6] = 3;
            }
            Sound_PlaySE(SE_CURSOR);
        } else if (D_0047E36C & MENU_RIGHT) {
            s->opt[6]++;
            if (s->opt[6] >= 4) {
                s->opt[6] = 0;
            }
            Sound_PlaySE(SE_CURSOR);
        }
        if (D_0047E36C & MENU_CONFIRM) {
            opt[6] = s->opt[6];
            VCALL((VObject *)s, 0x34, void (*)(VObject *, s32))((VObject *)s, s->opt[6]);
            opt_back(s, 0x2B);
        } else if (opt_cancelled()) {
            s->opt[6] = opt[6];
            opt_back(s, 0x2C);
        }
    }
    opt_editor_draw(s, 0x120, 0x180, 0x54);
}

/* the vibration's on / off: the actuator's +0x28 */
#define VIB_ENABLE(o, on) VCALL(o, 0x28, void (*)(VObject *, s32))(o, on)


/* editing the vibration: left / right toggle it (turning it on buzzes the pad) */
void Options_StateVibration(SubScreen *s) {
    s8 *opt = (s8 *)D_0044E978 + 0x30;
    VObject *o;

    if (!s->fading) {
        if ((D_0047E36C & MENU_LEFT) || (D_0047E36C & MENU_RIGHT)) {
            o = D_0044E7A8;
            if (s->opt[4] == 1) {
                s->opt[4] = 0;
                VCALL(o, 0x10, void (*)(VObject *))(o);
                VIB_ENABLE(o, 0);
            } else {
                s->opt[4] = 1;
                VCALL(o, 0x10, void (*)(VObject *))(o);
                VIB_ENABLE(o, 1);
                VCALL(o, 0x14, void (*)(VObject *, s32, s32, s32))(o, 0, 1, 4);
                VCALL(o, 0x18, void (*)(VObject *, s32, s32, s32))(o, 0, 0x80, 4);
            }
            Sound_PlaySE(SE_CURSOR);
        }
        if (D_0047E36C & MENU_CONFIRM) {
            o = D_0044E7A8;
            opt[4] = s->opt[4];
            VCALL(o, 0x10, void (*)(VObject *))(o);
            VIB_ENABLE(o, opt[4] == 1);
            opt_back(s, 0x2B);
        } else if (opt_cancelled()) {
            o = D_0044E7A8;
            s->opt[4] = opt[4];
            VCALL(o, 0x10, void (*)(VObject *))(o);
            VIB_ENABLE(o, opt[4] == 1);
            opt_back(s, 0x2C);
        }
    }
    opt_editor_draw(s, 0xE0, 0x1C0, 0xF4);
}

/* the sound driver's output mode (0 mono, 1 stereo, 2 surround) */
#define SND_OUTPUT(o, mode) VCALL(o, 0x68, void (*)(VObject *, s32))(o, mode)

/* editing the sound output: left / right cycle it (heard at once) */
void Options_StateSound(SubScreen *s) {
    s8 *opt = (s8 *)D_0044E978 + 0x30;

    if (!s->fading) {
        if (D_0047E36C & MENU_LEFT) {
            s->opt[0] = s->opt[0] == 0 ? 2 : s->opt[0] == 1 ? 0 : 1;
            SND_OUTPUT(D_0044E560, s->opt[0]);
            Sound_PlaySE(SE_CURSOR);
        } else if (D_0047E36C & MENU_RIGHT) {
            s->opt[0] = s->opt[0] == 0 ? 1 : s->opt[0] == 1 ? 2 : 0;
            SND_OUTPUT(D_0044E560, s->opt[0]);
            Sound_PlaySE(SE_CURSOR);
        }
        if (D_0047E36C & MENU_CONFIRM) {
            opt[0] = s->opt[0];
            opt_back(s, 0x2B);
        } else if (opt_cancelled()) {
            s->opt[0] = opt[0];
            SND_OUTPUT(D_0044E560, s->opt[0]);
            opt_back(s, 0x2C);
        }
    }
    opt_editor_draw(s, 0xE0, 0x1C0, 0x114);
}

/* editing the volume: left / right in steps of 1/64 (heard at once) */
void Options_StateVolume(SubScreen *s) {
    u8 *opt = (u8 *)D_0044E978 + 0x30;
    VObject *snd;
    f32 v;

    if (!s->fading) {
        if (D_0047E36C & MENU_LEFT) {
            if (!(s->optVolume < 0.0f)) {
                s->optVolume = v = s->optVolume - 0.015625f;
                if (v < 0.0f) {
                    s->optVolume = 0.0f;
                }
                snd = D_0044E560;
                VCALL(snd, 0xA8, void (*)(VObject *, f32))(snd, s->optVolume);
                Sound_Play(snd, SE_CURSOR, SE_BANK_MENU);
            }
        } else if (D_0047E36C & MENU_RIGHT) {
            if (s->optVolume < 1.0f) {
                s->optVolume = v = s->optVolume + 0.015625f;
                if (!(v < 1.0f)) {
                    s->optVolume = 1.0f;
                }
                snd = D_0044E560;
                VCALL(snd, 0xA8, void (*)(VObject *, f32))(snd, s->optVolume);
                Sound_Play(snd, SE_CURSOR, SE_BANK_MENU);
            }
        }
        if (D_0047E36C & MENU_CONFIRM) {
            snd = D_0044E560;
            v = s->optVolume;
            AT(opt, 8, f32) = v;
            opt_apply_volume(snd, v);
            Sound_Play(snd, SE_DECIDE, SE_BANK_MENU);
            ptmf_set_fn(&s->state, Options_StateList);
        } else if (opt_cancelled()) {
            snd = D_0044E560;
            s->optVolume = AT(opt, 8, f32);
            opt_apply_volume(snd, AT(opt, 8, f32));
            Sound_Play(snd, SE_CANCEL, SE_BANK_MENU);
            ptmf_set_fn(&s->state, Options_StateList);
        }
    }
    opt_editor_draw(s, 0xE0, 0x1C0, 0x134);
}

#define SCREEN_POS(x, y) VCALL(D_0044E4F0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4F0, x, y)

/* editing the screen position: the D-pad moves it (-32..32 each way, seen at once) */
void Options_StatePosition(SubScreen *s) {
    s8 *opt = (s8 *)D_0044E978 + 0x30;
    s32 a;

    if (!s->fading) {
        u32 pad = D_0047E36C;

        if (pad & MENU_UP) {
            if (s->opt[3] >= -0x1F) {
                s->opt[3]--;
                SCREEN_POS(s->opt[2], s->opt[3]);
                Sound_PlaySE(SE_CURSOR);
            }
        } else if (pad & MENU_DOWN) {
            if (s->opt[3] < 0x20) {
                s->opt[3]++;
                SCREEN_POS(s->opt[2], s->opt[3]);
                Sound_PlaySE(SE_CURSOR);
            }
        }
        pad = D_0047E36C;
        if (pad & MENU_LEFT) {
            if (s->opt[2] >= -0x1F) {
                s->opt[2]--;
                SCREEN_POS(s->opt[2], s->opt[3]);
                Sound_PlaySE(SE_CURSOR);
            }
        } else if (pad & MENU_RIGHT) {
            if (s->opt[2] < 0x20) {
                s->opt[2]++;
                SCREEN_POS(s->opt[2], s->opt[3]);
                Sound_PlaySE(SE_CURSOR);
            }
        }
        if (D_0047E36C & MENU_CONFIRM) {
            opt[2] = s->opt[2];
            opt[3] = s->opt[3];
            opt_back(s, 0x2B);
        } else if (opt_cancelled()) {
            s->opt[2] = opt[2];
            s->opt[3] = opt[3];
            SCREEN_POS(opt[2], opt[3]);
            opt_back(s, 0x2C);
        }
    }
    Options_Draw(s, 1);
    /* the corner marks pulse */
    a = 0x40 - ((s->frame << 1) & 0x7F);
    a = (a > 0 ? a : -a) + 0x20;
    SubScreen_DrawPart(s, 0x10, 0x10, 0x15, (u8)a, 0);
    SubScreen_DrawPart(s, 0x1D2, 0x10, 0x16, (u8)a, 0);
    SubScreen_DrawPart(s, 0x10, 0x190, 0x17, (u8)a, 0);
    SubScreen_DrawPart(s, 0x1D2, 0x190, 0x18, (u8)a, 0);
    opt_text(s, 0x46, 0x186, 0x80, 0x83);
}

/* the "restore the defaults?" question (message 0x84): on yes the options go back to mono,
 * vibration on, full volume, centred, layout A, and are applied */
void Options_StateDefaults(SubScreen *s) {
    Task *t = &s->ask;

    Options_Draw(s, 0);
    if (t->mode) {
        Task_Run(t);
        return;
    }
    if (t->answer == 0) {
        s8 *sys = D_0044E978;
        VObject *o;

        sys[0x36] = 0;
        sys[0x34] = 1;
        sys[0x30] = 1;
        AT(sys, 0x38, f32) = 1.0f;
        sys[0x32] = 0;
        sys[0x33] = 0;
        sub_copy_options(s);
        VCALL((VObject *)s, 0x34, void (*)(VObject *, s32))((VObject *)s, sys[0x36]);
        o = D_0044E7A8;
        VCALL(o, 0x10, void (*)(VObject *))(o);
        VIB_ENABLE(o, sys[0x34] == 1);
        o = D_0044E560;
        SND_OUTPUT(o, sys[0x30]);
        opt_apply_volume(o, AT(sys, 0x38, f32));
        SCREEN_POS(sys[0x32], sys[0x33]);
    }
    ptmf_set_fn(&s->state, Options_StateList);
}


/* add `id` to the list at +0x15F8 (128 u16, 0-terminated): 1 if added, 0 if already there or
 * the list is full */
s32 func_00394200(SubScreen *s, u32 id) {
    u16 *list = &AT(s, 0x15F8, u16);
    u32 i;

    for (i = 0; i < 0x80; i++) {
        if (list[i] == (u16)id) {
            return 0;
        }
        if (list[i] == 0) {
            list[i] = id;
            return 1;
        }
    }
    return 0;
}


/* +0x24 the in-game tab, drawn each gameplay frame: base parts 0xC (at y 0x60) and 0xB (at y
 * 0x8C) slide in from the left (state 2) as page[0x15C] grows to 0x40, stay (3) and slide out
 * to the right (4), fading with the slide */
void func_00384CE0(SubScreen *s) {
    u8 t = s->page[0x15C];

    switch (s->tab) {
    case 2:
        SubScreen_DrawPart(s, (u16)(0xAC - (0x40 - t)), 0x60, 0xC, (u8)(t * 2), 0);
        SubScreen_DrawPart(s, (u16)(0xC0 - (0x40 - s->page[0x15C])), 0x8C, 0xB, s->page[0x15C], 0);
        break;
    case 3:
        SubScreen_DrawPart(s, 0xAC, 0x60, 0xC, 0x80, 0);
        SubScreen_DrawPart(s, 0xC0, 0x8C, 0xB, 0x40, 0);
        break;
    case 4:
        SubScreen_DrawPart(s, (u16)(0xEC - t), 0x60, 0xC, (u8)(t * 2), 0);
        SubScreen_DrawPart(s, (u16)(0x100 - s->page[0x15C]), 0x8C, 0xB, s->page[0x15C], 0);
        break;
    }
}

/* ---- text helpers left (2026-10-05); the sub-screen's text task at +0x97868 ---- */

extern s32 func_0026EDD0(char *buf, s32 size, const char *fmt, ...);   /* snprintf */
extern s32 func_0038A2C0(void *s, u32 k);   /* (a u8) */
extern s32 func_003941C0(u8 *o);   /* the file's entries (+0x15F8) */
extern const char D_00463A60[];

#define SUB_TEXT(s) ((Task *)((u8 *)(s) + 0x97868))

/* the page counter at the top right: "<page + 1> / <count>" (D_00463A60) and its caption
 * (message 0x1C6), faded by +0xA8C62 */
void func_0037E480(u8 *s) {
    char buf[16];
    u8 *msg;

    func_0026EDD0(buf, 10, D_00463A60, s[0xA8C81] + 1, func_0038A2C0(s, s[0xA8C80]) & 0xFF);
    Task_ShowText(SUB_TEXT(s), 0x1B8, 0x10, 0, (u8 *)buf, 0x80 - AT(s, 0xA8C62, s16), 0x33, 0x10, 0x15);
    msg = Task_MessageText(SUB_TEXT(s), 0x1C6);
    Task_ShowText(SUB_TEXT(s), 0x160, 0x10, 0, msg, 0x80 - AT(s, 0xA8C62, s16), 0x33, 0x10, 0x15);
}

/* the two lines of the current entry (+0x15F8, two message ids per entry +0xA8C55; 0 none) at
 * x 0x2D, y 0x54 / 0xF4 */
void func_0037E580(u8 *s) {
    s32 i;

    for (i = 0; i < 2; i++) {
        u16 id = AT(s, 0x15F8 + (s[0xA8C55] * 2 + i) * 2, u16);

        if (id != 0) {
            Task_ShowText(SUB_TEXT(s), 0x2D, 0x54 + i * 0xA0, 0x80, Task_MessageText(SUB_TEXT(s), id), 0x80, 0x30,
                          0x10, 0x15);
        }
    }
}

/* the file's last page: its entries (+0x15F8, func_003941C0) two to a page */
u8 func_0037E650(u8 *s) {
    return (u8)(((s32)func_003941C0(s) - 1) / 2);
}

extern void func_00260A60(void *items, s32 k);

/* its items (+0x8) func_00260A60 with 2 */
void func_00384C20(u8 *o) {
    func_00260A60(o + 0x8, 2);
}

/* ---- the in-game menu's item page ---- */

extern u32 func_002605F0(u8 *items, u8 l);                    /* how many items list `l` holds */
extern s32 func_00260420(u8 *items, u8 l, u8 i);              /* an item's kind (-1 none) */
extern s32 func_00260480(u8 *items, u8 l, u8 i);              /* an item's id (-1 none) */
extern void func_00260250(u8 *items, u8 l, u8 i, void *arg);   /* start using it */
extern void func_002600C0(void);
extern void func_0025FF80(u8 *items, u8 l);                    /* sort the list */
extern void func_003949B0(SubScreen *s, s32 a);               /* the item grid */
extern const PTMF D_0044B200, D_0044B210, D_0044B220, D_0044B230, D_0044B240, D_0044B250;

#define SUB_LIST(s) ((s)->unkA8C50)              /* the item list shown: 0 items, 1 / 2 the others */
#define SUB_CURSOR(s) ((s)->unkA8C51[SUB_LIST(s)])   /* its cursor: 16 a page, rows of 8 */

/* The item page: the cursor moves along its row of 8 (up / down, round) and between the rows
 * and pages (left / right, round); confirm uses the item (kind 7: a question, D_0044B200; else
 * started, D_0044B210); list 1 sorts with the default button; next / previous turn to the
 * other lists and then the map page (D_0044B220 / D_0044B230) or the page before (D_0044B240 /
 * D_0044B250); cancel closes. Then its panels, the grid and the help line. */
void func_00396900(SubScreen *s) {
    u8 *items = s->pool;

    if (!s->fading) {
        u32 n = func_002605F0(items, SUB_LIST(s));
        s32 last = n != 0 ? (s32)(n - 1) / 16 : 0;
        u32 pad = D_0047E36C;
        u8 *cur = &SUB_CURSOR(s);
        s32 c = *cur;
        u8 old = c;

        if (pad & MENU_CONFIRM) {
            if (func_00260480(items, SUB_LIST(s), SUB_CURSOR(s)) != -1) {
                if (func_00260420(items, SUB_LIST(s), SUB_CURSOR(s)) == 7) {
                    Task_Open(&s->ask, func_00260480(items, SUB_LIST(s), SUB_CURSOR(s)) & 0xFFFF);
                    ptmf_set(&s->state, &D_0044B200);
                } else {
                    s->padA8C54 = 0;
                    s->page[0x15C] = 0;
                    func_002600C0();
                    func_00260250(items, SUB_LIST(s), SUB_CURSOR(s), (u8 *)s + 0x94F40);
                    ptmf_set(&s->state, &D_0044B210);
                }
                Sound_PlaySE(SE_DECIDE);
            }
        } else if (pad & MENU_UP) {
            *cur = c != 0 ? (c - 1) % 8 + (c >> 3) * 8 : 7;
        } else if (pad & MENU_DOWN) {
            *cur = (c + 1) % 8 + (c >> 3) * 8;
        } else if (pad & MENU_LEFT) {
            if (c < 8) {
                *cur = c + ((last + 1) * 16 - 8);
            } else {
                *cur -= 8;
            }
        } else if (pad & MENU_RIGHT) {
            if (last < (c + 8) / 16) {
                *cur = c % 8;
            } else {
                *cur += 8;
            }
        } else if (SUB_LIST(s) == 1 && (pad & MENU_DEFAULT)) {
            func_0025FF80(items, SUB_LIST(s));
            Sound_PlaySE(SE_DECIDE);
        } else if (D_0047E36C & MENU_NEXT) {
            if (SUB_LIST(s) < 2) {
                SUB_LIST(s)++;
                old = SUB_CURSOR(s);
            } else {
                func_00305380(s->textObj);
                ptmf_set(&s->state, &D_0044B220);
                ptmf_set(&s->resume, &D_0044B230);
            }
            Sound_PlaySE(SE_PAGE);
        } else if (D_0047E36C & MENU_PREV) {
            if (SUB_LIST(s) != 0) {
                SUB_LIST(s)--;
                old = SUB_CURSOR(s);
            } else {
                ptmf_set(&s->state, &D_0044B240);
                ptmf_set(&s->resume, &D_0044B250);
            }
            Sound_PlaySE(SE_PAGE);
        } else if (D_0047E36C & MENU_CANCEL) {
            s->close = 1;
        }
        if (old != SUB_CURSOR(s)) {
            Sound_PlaySE(SE_CURSOR);
        }
    }
    if (SUB_LIST(s) == 0) {
        s->kind = 0;
    } else {
        s->kind = SUB_LIST(s) == 1 ? 8 : 9;
    }
    if (s->kind != 0xFF) {
        s8 *panel = D_0044C120[s->kind & 0x7F];
        s32 i;

        for (i = 0; i < 4 && panel[i] >= 0; i++) {
            SubScreen_DrawPanel(s, (u8)panel[i], 0x80, 0x30);
        }
    }
    func_003949B0(s, 1);
    Task_ShowText(&s->text, 0x46, 0x186, 0x80, Task_MessageText(&s->text, SUB_LIST(s) == 1 ? 0x5B : 0x5D), 0x80, 0x30,
                  0x10, 0x15);
}

extern s32 func_002604E0(u8 *items, u8 l, u8 i);   /* the item's word (+0x20) */
extern s32 func_002606E0(u8 *items, u8 l, u8 i);   /* its equipment status: 1 equipped */
extern s32 func_00260300(u8 *items, u8 l, u8 i);   /* how many */
extern u8 func_00260360(u8 *items, u8 l, u8 i);    /* counted */
extern const char D_00463FD8[];   /* "%2d/%2d" */
extern const char D_00464218[];   /* "%s" */
extern const char D_00464230[];   /* "x%2d" */

/* The item grid of the list shown: the page (16 places, two columns of 8 down the screen) the
 * cursor is on (past the last: the last item), its number of the pages and, with more than
 * one, the two arrows blinking; each item's icon (by kind), name (message 0x8100 + id; the
 * word plate's word in parameter 3) in grey or, equipped, highlighted, and its count; the
 * cursor. */
void func_003949B0(SubScreen *s, s32 a) {
    u8 *items = s->pool;
    u32 n = func_002605F0(items, SUB_LIST(s));
    s32 last = n != 0 ? (s32)(n - 1) / 16 : 0;
    s32 i;

    if (last < (s32)(SUB_CURSOR(s) >> 4)) {
        SUB_CURSOR(s) = func_002605F0(items, SUB_LIST(s)) - 1;
    }
    Task_Printf(&s->text, 0x186, 0x176, 0x80, D_00463FD8, (SUB_CURSOR(s) >> 4) + 1, last + 1);
    if (last != 0) {
        s32 b = 0x80 - ((s->frame << 2) & 0xFF);
        u8 alpha = b > 0 ? b : -b;

        SubScreen_DrawPart(s, 0x168, 0x170, 0x1A, alpha, 0);
        SubScreen_DrawPart(s, 0x1B3, 0x170, 0x1B, alpha, 0);
    }
    for (i = 0; i < 16; i++) {
        u8 k = i + (SUB_CURSOR(s) >> 4) * 16;
        s32 id = func_00260480(items, SUB_LIST(s), k);
        u8 color = (s8)func_002606E0(items, SUB_LIST(s), k) == 1 ? 0x82 : 0x80;

        if (id != -1) {
            s32 x = (i / 8) * 0xDA, y = (i % 8) * 35;

            SubScreen_DrawPart(s, (u16)(x + 0x20), (u16)(y + 0x5E), (u8)func_00260420(items, SUB_LIST(s), k), 0x80, 0);
            if (id == 0x3F) {
                const u8 *w = (const u8 *)func_002604E0(items, SUB_LIST(s), k);
                char name[9];
                s32 j;

                for (j = 0; j < 8; j++) {
                    name[j] = w[j];
                }
                name[8] = 0;
                Msg_PrintfParam(&s->text, 3, D_00464218, name);
            }
            Task_ShowText(&s->text, x + 0x46, y + 0x64, color, Task_MessageText(&s->text, (id + 0x8100) & 0xFFFF), 0x80,
                          0x30, 0x10, 0x15);
            if (func_00260360(items, SUB_LIST(s), k) == 1) {
                Task_Printf(&s->text, x + 0xDE, y + 0x64, color, D_00464230, func_00260300(items, SUB_LIST(s), k));
            }
        }
        if (SUB_CURSOR(s) % 16 == i) {
            SubScreen_DrawPart(s, (u16)((i / 8) * 0xDA + 0x20), (u16)((i % 8) * 35 + 0x5E), 0x1C, 0x80, 0);
        }
    }
}

/* ---- the in-game menu's map page ---- */

extern void func_00304F50(u8 *m);   /* the map, each frame */
extern u8 func_00303F00(void);      /* the maps the player has (bits) */
extern const PTMF D_0044B388, D_0044B398, D_0044B3A8, D_0044B3B8;

/* draw the screen kind `kind`'s panels */
static inline void sub_panels(SubScreen *s) {
    if (s->kind != 0xFF) {
        s8 *panel = D_0044C120[s->kind & 0x7F];
        s32 i;

        for (i = 0; i < 4 && panel[i] >= 0; i++) {
            SubScreen_DrawPanel(s, (u8)panel[i], 0x80, 0x30);
        }
    }
}

/* The map page: next / previous turn to the page after (D_0044B388 / D_0044B398) or back to
 * the item lists' last (D_0044B3A8 / D_0044B3B8); cancel closes. Its panels, the map and,
 * when there are other maps to turn to, the two arrows blinking. */
void func_00394670(SubScreen *s) {
    if (!s->fading) {
        if (D_0047E36C & MENU_NEXT) {
            ptmf_set(&s->state, &D_0044B388);
            ptmf_set(&s->resume, &D_0044B398);
            Sound_PlaySE(SE_PAGE);
        } else if (D_0047E36C & MENU_PREV) {
            SUB_LIST(s) = 2;
            ptmf_set(&s->state, &D_0044B3A8);
            ptmf_set(&s->resume, &D_0044B3B8);
            Sound_PlaySE(SE_PAGE);
        } else if (D_0047E36C & MENU_CANCEL) {
            s->close = 1;
        }
    }
    s->kind = 1;
    sub_panels(s);
    func_00304F50(s->textObj);
    {
        u8 maps = func_00303F00();

        if (maps != 0 && !(maps == 4 && AT(s->textObj, 0x10C, s8) == 2)) {
            s32 b = 0x80 - ((s->frame << 2) & 0xFF);
            u8 alpha = b > 0 ? b : -b;

            SubScreen_DrawPart(s, 0xA, 0xF0, 0x1A, alpha, 0);
            SubScreen_DrawPart(s, 0x1D6, 0xF0, 0x1B, alpha, 0);
        }
    }
}

/* ---- the in-game menu's file page ---- */

extern const PTMF D_0044B3C8, D_0044B3D8, D_0044B3E8, D_0044B3F8;

#define SUB_FILE_PAGE(s) ((s)->unkA8C55)

/* The file page: left / right turn its pages (round), next / previous turn to the page after
 * (D_0044B3C8 / D_0044B3D8) or back to the map (D_0044B3E8 / D_0044B3F8); cancel closes. Its
 * panels, the page number and, with more than one, the arrows blinking; the file. */
void func_00394260(SubScreen *s) {
    u8 last = func_0037E650((u8 *)s);

    if (!s->fading) {
        u8 old = SUB_FILE_PAGE(s);

        if (D_0047E36C & MENU_LEFT) {
            if (old != 0) {
                SUB_FILE_PAGE(s)--;
            } else {
                SUB_FILE_PAGE(s) = last;
            }
        } else if (D_0047E36C & MENU_RIGHT) {
            if (old < last) {
                SUB_FILE_PAGE(s)++;
            } else {
                SUB_FILE_PAGE(s) = 0;
            }
        } else if (D_0047E36C & MENU_NEXT) {
            ptmf_set(&s->state, &D_0044B3C8);
            ptmf_set(&s->resume, &D_0044B3D8);
            Sound_PlaySE(SE_PAGE);
        } else if (D_0047E36C & MENU_PREV) {
            func_00305380(s->textObj);
            ptmf_set(&s->state, &D_0044B3E8);
            ptmf_set(&s->resume, &D_0044B3F8);
            Sound_PlaySE(SE_PAGE);
        } else if (D_0047E36C & MENU_CANCEL) {
            s->close = 1;
        }
        if (old != SUB_FILE_PAGE(s)) {
            Sound_PlaySE(SE_CURSOR);
        }
    }
    s->kind = 2;
    sub_panels(s);
    Task_Printf(&s->text, 0x186, 0x186, 0x80, D_00463FD8, SUB_FILE_PAGE(s) + 1, last + 1);
    if (last != 0) {
        s32 b = 0x80 - ((s->frame << 2) & 0xFF);
        u8 alpha = b > 0 ? b : -b;

        SubScreen_DrawPart(s, 0x168, 0x180, 0x1A, alpha, 0);
        SubScreen_DrawPart(s, 0x1B3, 0x180, 0x1B, alpha, 0);
    }
    func_0037E580((u8 *)s);
}

/* ---- costumes ---- */

extern s32 func_00260690(u8 *items, u8 slot);   /* the item in equipment slot `slot` */

/* the costume worn (equipment slot 2, items 0x90..0x9B) as 0..8; -1 none */
s32 func_00385370(SubScreen *s) {
    static const s8 kIndex[12] = {0, 1, 2, 3, 4, 5, -1, 6, 7, -1, -1, 8};
    u32 k = func_00260690(s->pool, 2) - 0x90;

    return k < 12 ? kIndex[k] : -1;
}

/* the costume worn as its model variant 0x31..0x33; 0 none */
s32 func_00385410(SubScreen *s) {
    static const u8 kModel[12] = {0x31, 0x32, 0x33, 0x31, 0x32, 0x33, 0, 0x32, 0x33, 0, 0, 0x33};
    u32 k = func_00260690(s->pool, 2) - 0x90;

    return k < 12 ? kModel[k] : 0;
}

extern void func_00385C30(SubScreen *s);
extern const PTMF D_0044C108;

/* state: a question being asked (its page drawn, func_00385C30); once answered, D_0044C108 */
void func_00385F60(SubScreen *s) {
    func_00385C30(s);
    Task_Run(&s->ask);
    if (AT(&s->ask, 0x10, u8) == 0) {
        ptmf_set(&s->state, &D_0044C108);
    }
}
