/* Mode 2 scene: the title screen and its menus (new game, load, options, extras), the opening
 * movie. A large object (0x140CE0 bytes from the scene heap). */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "task.h"
#include "scene_title.h"
#include "input.h"
#include "sound.h"
#include "globals.h"
#include "progress.h"
#include "actor.h"
#include "pursuer.h"
#include "bgm.h"
#include "bootcard.h"
#include "heap.h"
#include "item_classes.h"
#include "message.h"
#include "movie.h"
#include "renderer.h"
#include "scene_game_members.h"
#include "subscreen.h"
#include "libc.h"
#include "msl.h"

extern void *Scene_vtable[];
extern void *D_0046A040[];          /* SceneTitle */
extern void *D_0046D7D0[];          /* the message object */
extern void *D_0047A790[];          /* the title work */
extern void *D_0046A110[];
extern void *D_0046A090[], *D_0046A078[], *D_004699E0[], *D_004699C0[], *D_0046A068[];
extern void *D_0046C790[];          /* a pool entry */
extern const PTMF sSceneEntryState; /* virtual: vtable +0x10 */
extern const PTMF sGameStateNull;

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

s32 func_00384C50(u8 *g, s32 a1);
s32 func_00384C60(u8 *g);
void func_00384C70(u8 *g, s32 n);
s32 func_00384CB0(u8 *g, s32 n);

/* a pool entry: constructor / destructor */
void *PoolEntry_ctor(void *e) {
    AT(e, 0x0, void **) = D_0046C790;
    AT(e, 0x4, s32) = -1;
    AT(e, 0x8, u8) = 0;
    AT(e, 0x10, s64) = 0;
    return e;
}

void *PoolEntry_dtor(void *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x0, void **) = D_0046C790;
        if ((s16)flags > 0) {
            func_0025FEF0(e);
        }
    }
    return e;
}

/* the sub screen's base: a pool of 192 entries (global gSubPool) and its list */
void *SubScreenBase_ctor(SubScreen *w) {
    u8 *pool = w->pool;

    gSubScreen = (VObject *)w;
    w->vtbl = D_0046A090;
    gSubPool = pool;
    AT(pool, 0x0, void **) = D_0046A078;
    func_00100340(pool + 8, PoolEntry_ctor, PoolEntry_dtor, 0x18, 0xC0);
    AT(pool, 0x1208, void **) = D_004699E0;
    AT(pool, 0x120C, s32) = 0;
    AT(pool, 0x1210, s32) = 0;
    AT(pool, 0x1208, void **) = D_004699C0;
    AT(pool, 0x1214, s32) = 0;
    AT(pool, 0x1218, s32) = 0;
    AT(pool, 0x121C, s32) = 0;
    w->mode = 0;
    return w;
}

/* a text object: a Task and what it shows */
void *TextObj_ctor(u8 *o) {
    AT(o, 0x0, void **) = D_0046A068;
    Task_Construct((Task *)(o + 4));
    AT(o, 0x108, s32) = -1;
    AT(o, 0x10C, u8) = 0xFF;
    AT(o, 0x10E, u8) = 0xFF;
    return o;
}

/* constructor */
SceneTitle *SceneTitle_ctor(SceneTitle *t) {
    SubScreen *w;

    t->base.vtbl = Scene_vtable;
    ptmf_set(&t->base.state, &sSceneEntryState);
    gSceneTitle = t;
    gBootMessage = (VObject *)t->msg;
    t->base.vtbl = D_0046A040;
    AT(t->msg, 0, void **) = D_0046D7D0;
    Task_Construct(&t->task);
    w = &t->sub;
    SubScreenBase_ctor(w);
    w->vtbl = D_0047A790;
    Task_ctor(&w->ask);
    Task_ctor(&w->text);
    TextObj_ctor(w->textObj);
    BootCard_ctor(&w->card);
    gMusic = (VObject *)&t->bgm;
    t->bgm.vtbl = D_0046A110;
    t->unk11DA80 = 0;
    t->loaded = 0;
    t->returnTo = 0;
    t->extras = 0;
    t->seq = sGameStateNull;
    func_002D2370(gAdx, t->bgmWork);
    func_002E34D0((u8 *)&t->bgm);
    return t;
}

void func_002E34D0(u8 *p) {
    p[0x5] = 0xFF;
    p[0x4] = 0xFF;
    F(p, 0x8, u32) = 0;
    F(p, 0x10, f32) = 1.0f;
    F(p, 0xC, u32) = 0;
    F(p, 0x14, u32) = 0;
}

extern u8 D_0047B350;           /* the language */
void SceneTitle_StateStart(SceneTitle *t);

/* +0x10 entry: reset the message object and the sub screen, apply the options, then the title
 * sequence (SceneTitle_StateStart) */
void SceneTitle_StateEntry(SceneTitle *t) {
    u8 *sys = gSystemData;
    PTMF s = {0, -1, {(void *)SceneTitle_StateStart}};

    t->timer = 0;
    if (AT(sys, 0x2C, u32) & 0x100000) {
        t->cursor = 1;
    } else {
        t->cursor = 0;
    }
    t->next = 0;
    t->movieSkipped = 0;
    func_0026BCC0(t->msg);
    SubScreen_Start(&t->sub);
    SubScreen_ApplyOptions((VObject *)&t->sub);
    D_0047B350 = 2;
    t->extras = 0;
    if (AT(sys, 0x24, u32) & 1) {
        t->extras = 1;
    }
    t->demo = 0;
    if (ptmf_test(&s)) {
        t->base.state = s;
    }
}

void SceneTitle_StateAttractStart(SceneTitle *t);
void SceneTitle_StateTitle(SceneTitle *t);
void SceneTitle_SeqLoad(SceneTitle *t);

/* state: once the textures are loaded, either back to the menu (+0x14 = 2) or the title
 * (SceneTitle_StateTitle, with the +0x140CA4 object running SceneTitle_SeqLoad) */
void SceneTitle_StateStart(SceneTitle *t) {
    if (VCALL(gFileLoader, 0x24, s32 (*)(VObject *))(gFileLoader) == 2) {
        return;
    }
    if (t->returnTo == 2) {
        t->returnTo = 0;
        ptmf_set_fn(&t->base.state, SceneTitle_StateAttractStart);
        return;
    }
    ptmf_set_fn(&t->base.state, SceneTitle_StateTitle);
    ptmf_set_fn(&t->seq, SceneTitle_SeqLoad);
}

/* state: the title; run the title's own sequence (its state at +0x140CC0), then the
 * +0x140CA4 object */
void SceneTitle_StateTitle(SceneTitle *t) {
    PTMF *seq = &t->seq;

    if (ptmf_test(seq)) {
        ptmf_scall(t, seq);
    }
    func_002E3200(&t->bgm);
}

extern void *D_0046ECC0[];        /* SceneMovie */
extern const char D_0044E940[];   /* "SYSTEM\\LOOP_DEMO.SFD" */
void SceneTitle_StateAttract(SceneTitle *t);

#define SCENE_TABLE_SCENE(i) (*(Scene **)((u8 *)gSceneTable + 4 + (i) * 4))

/* start movie `name` as scene 1 at the title's movie volume (+0x140CCC, reset to 1) */
static inline void title_movie_start(SceneTitle *t, const char *name) {
    void *table = gSceneTable;
    VObject *heap = (VObject *)((u8 *)table + 0x10D9040);
    Scene *movie;
    void *mem;
    s32 ok;

    t->timer = 0;
    t->movieSkipped = 0;
    t->movieVolume = 1.0f;
    mem = VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, 0x600200);
    if (mem != NULL) {
        movie = __nw__FUiPv(0x600200, mem);
        if (movie != NULL) {
            func_002B70D0((Movie *)movie);
            movie->vtbl = D_0046ECC0;
        }
        SCENE_TABLE_SCENE(1) = movie;
        SCENE_TABLE_SCENE(1)->slot = 1;
        movie = SCENE_TABLE_SCENE(1);
        if (movie != NULL) {
            movie->request = SCENE_REQ_RUN;
            movie->status = 0;
            movie->waitFrames = 0;
            ok = 1;
        } else {
            ok = 0;
        }
    } else {
        ok = 0;
    }
    if (ok) {
        u8 *m = gMovie;
        f32 *v = &AT(m, 0x1D4, f32);

        func_002B6D10((Movie *)m, name, 1, 0);
        *v = t->movieVolume;
        if (*v < 0.0f) {
            *v = 0.0f;
        }
        if (!(*v <= 1.0f)) {
            *v = 1.0f;
        }
        func_002B6340((Movie *)m);
    }
}

/* ... then state `next` */
static inline void title_movie(SceneTitle *t, const char *name, void (*next)(SceneTitle *)) {
    title_movie_start(t, name);
    ptmf_set_fn(&t->base.state, next);
}

/* state: start the attract movie */
void SceneTitle_StateAttractStart(SceneTitle *t) {
    title_movie(t, D_0044E940, SceneTitle_StateAttract);
}

extern const char D_0044E888[];   /* "OPENING.SFD" */
void SceneTitle_StateOpening(SceneTitle *t);

/* state: a new game: the opening movie first */
void SceneTitle_StateNewGame(SceneTitle *t) {
    title_movie(t, D_0044E888, SceneTitle_StateOpening);
}

void SceneTitle_StateAttractFade(SceneTitle *t);

/* wait for the movie (Start, once it shows, skips it), then state `next` */
static inline void title_movie_wait(SceneTitle *t, void (*next)(SceneTitle *)) {
    if ((D_0047E37C & PAD_START) && gMovie != NULL && AT(gMovie, 0x1B4, u8)) {
        t->movieSkipped = 1;
    }
    if (gMovie != NULL && !t->movieSkipped) {
        return;
    }
    ptmf_set_fn(&t->base.state, next);
}

/* state: the attract movie */
void SceneTitle_StateAttract(SceneTitle *t) {
    title_movie_wait(t, SceneTitle_StateAttractFade);
}

void SceneTitle_StateOpeningFade(SceneTitle *t);

/* state: the opening movie */
void SceneTitle_StateOpening(SceneTitle *t) {
    title_movie_wait(t, SceneTitle_StateOpeningFade);
}

static const PTMF sSceneFinish = {0, 0x14, {(void *)0}};   /* virtual +0x14 */

/* the renderer's +0x7C: a rectangle (x, y, w, h; colour; layer ...) */
typedef void (*RectFn)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, u32, s32, s32, s32, s32);

/* fade the movie out (sound and picture, 1/30 a frame; at 0 its scene is finished); 0 once
 * it's gone */
static inline s32 title_movie_fade(SceneTitle *t) {
    u8 *m = gMovie;
    f32 *level = &t->movieVolume;
    f32 l;

    if (m == NULL) {
        return 0;
    }
    l = *level - 0x1.111112p-5f;
    *level = l;
    if (l < 0.0f) {
        l = 0.0f;
    }
    *level = l;
    if (l <= 0.0f) {
        Scene *s = SCENE_TABLE_SCENE(1);

        if (s != NULL) {
            ptmf_set(&s->state, &sSceneFinish);
            VCALL(SCENE_TABLE_SCENE(1), 0x14, void (*)(Scene *))(SCENE_TABLE_SCENE(1));
        }
    }
    {
        f32 *v = &AT(m, 0x1D4, f32);

        *v = *level;
        if (*level < 0.0f) {
            *v = 0.0f;
        }
        if (!(*v <= 1.0f)) {
            *v = 1.0f;
        }
    }
    m = gMovie;
    func_002B6340((Movie *)m);
    if (AT(m, 0x1B4, u8)) {
        u32 a = (u32)(127.0f * (1.0f - *level));

        if (a >= 0x80) {
            a = 0x7F;
        }
        VCALL(gRenderer, 0x7C, RectFn)(gRenderer, 0, 0, 0x200, 0x200, 0, 0, 0, 0, (a << 24) & 0xFF000000, -1, 0, 0x30, -1);
    }
    return 1;
}

/* state: fade the attract movie out, then the title */
void SceneTitle_StateAttractFade(SceneTitle *t) {
    if (!title_movie_fade(t)) {
        ptmf_set_fn(&t->base.state, SceneTitle_StateTitle);
        ptmf_set_fn(&t->seq, SceneTitle_SeqLoad);
    }
}

/* state: fade the opening out, then start the game (mode 3, stage 0x2A; flagged for the extra
 * mode) */
void SceneTitle_StateOpeningFade(SceneTitle *t) {
    if (!title_movie_fade(t)) {
        AT(gSystemData, 0x4, s32) = 3;
        AT(gSystemData, 0x10, s32) = t->next == 5 ? 0x4000002A : 0x2A;
        VCALL(t, 0x14, void (*)(SceneTitle *))(t);
    }
}

extern const char D_0044E8C0[];   /* "SYSTEM\\TITLE.TEX" */
extern const char D_0044E8E0[];   /* "SYSTEM\\TITLE_BACK.BIN" */
extern const char D_0044E900[];   /* "SYSTEM\\TITLE.HD" */
extern const char D_0044E910[];   /* "SYSTEM\\TITLE.SDT" */
extern const char D_0044E930[];   /* "SYSTEM\\TITLE.BD" */
void SceneTitle_SeqPressStart(SceneTitle *t);
void SceneTitle_SeqFadeIn(SceneTitle *t);

#define LOADER_SIZE(name) VCALL(gFileLoader, 0x30, s32 (*)(VObject *, const char *))(gFileLoader, name)
#define LOADER_LOAD(name, dst) VCALL(gFileLoader, 0x34, void (*)(VObject *, const char *, void *))(gFileLoader, name, dst)

/* load one part of the title sound bank (into bank 7 through the driver's method `m`) */
static inline void load_bank_part(VObject *snd, const char *name, s32 m) {
    s32 size = LOADER_SIZE(name);
    void *buf;

    if (size == 0) {
        return;
    }
    buf = func_00114DA8(0x40, size);
    if (buf == NULL) {
        return;
    }
    LOADER_LOAD(name, buf);
    VCALL(snd, m, void (*)(VObject *, s32, void *, s32))(snd, 7, buf, size);
    func_00114FD0(buf);
}

/* the title sequence, first: load the title picture and its sound bank, then the logo
 * (SceneTitle_SeqFadeIn, title music) or, after a game, the menu straight away (SceneTitle_SeqPressStart) */
void SceneTitle_SeqLoad(SceneTitle *t) {
    VObject *msg;
    VObject *snd;

    t->timer = 0;
    LOADER_LOAD(D_0044E8C0, t->titleTex);
    msg = gBootMessage;
    VCALL(msg, 0x8, void (*)(VObject *, s32, void *))(msg, 6, t->titleTex);
    VCALL(msg, 0x10, void (*)(VObject *, s32, void *, s32))(msg, 6, t->titleTex, 0);
    LOADER_LOAD(D_0044E8E0, t->backImage);
    snd = gSound;
    t->loaded = 1;
    VCALL(snd, 0xA0, void (*)(VObject *))(snd);
    VCALL(snd, 0xAC, void (*)(VObject *, f32))(snd, 1.0f);
    {
        f32 *v = &AT(gAdx, 0x120, f32);

        *v = 1.0f;
        if (*v < 0.0f) {
            *v = 0.0f;
        }
        if (!(*v <= 1.0f)) {
            *v = 1.0f;
        }
        func_002D1FD0(gAdx);
    }
    load_bank_part(snd, D_0044E900, 0x4C);
    load_bank_part(snd, D_0044E910, 0x50);
    load_bank_part(snd, D_0044E930, 0x58);
    {
        VObject *s = gSound;

        VCALL(s, 0x60, void (*)(VObject *, s32))(s, 7);
        VCALL(s, 0x7C, void (*)(VObject *, s32, s32))(s, 1, 0);
    }
    if (t->movieSkipped == 0 && t->returnTo == 0) {
        VObject *s;

        t->anim = 0;
        s = gSound;
        Sound_Play(s, 0, 7);
        if ((s8)VCALL(s, 0x6C, s32 (*)(VObject *))(s) != 0) {
            Sound_Play(snd, 1, 7);
        }
        ptmf_set_fn(&t->seq, SceneTitle_SeqFadeIn);
        return;
    }
    t->returnTo = 0;
    t->anim = 90000;
    ptmf_set_fn(&t->seq, SceneTitle_SeqPressStart);
}

void SceneTitle_PrepareBackground(SceneTitle *t);
void SceneTitle_DrawPicture(SceneTitle *t, f32 alpha);
void SceneTitle_DrawLogo(SceneTitle *t, f32 alpha, f32 scale);
void SceneTitle_DrawRect(SceneTitle *t, s32 u, s32 v, s32 w, s32 h, s32 x, s32 y, s32 clut, s32 fix, f32 alpha);

/* the title fading in over 150 frames: the picture, then (from frame 90) the logo coming in
 * from large; then the menu (SceneTitle_SeqPressStart) */
void SceneTitle_SeqFadeIn(SceneTitle *t) {
    s32 done = 0;
    f32 a, b, f;

    t->anim++;
    if (t->anim >= 0x97) {
        t->anim = 150;
        done = 1;
    }
    a = 1.0f;
    f = (f32)t->anim / 150.0f;
    if (f <= 1.0f) {
        a = f;
    }
    f = (f32)(t->anim - 90) / 60.0f;
    if (f < 0.0f) {
        f = 0.0f;
    }
    b = 1.0f;
    if (f <= 1.0f) {
        b = f;
    }
    if (!t->loaded) {
        return;
    }
    SceneTitle_PrepareBackground(t);
    SceneTitle_DrawPicture(t, a);
    SceneTitle_DrawLogo(t, b, 1.0f + 4.0f * func_0031C248(0x1.921fb6p+1f * (90.0f * (1.0f - b)) / 180.0f));
    if (done) {
        t->anim = 90000;
        ptmf_set_fn(&t->seq, SceneTitle_SeqPressStart);
    }
}

/* the title picture's two textures: their VRAM slots (+0x11DA84 / +0x11DA88; bit 31 just
 * assigned) and entries (+0x11DA8C / +0x11DA90) */
#define TITLE_TEX_SLOT(i) (t->texSlot[i])
#define TITLE_TEX(i) (t->tex[i])

/* make sure texture `i` of the title set (6) is in VRAM: 0 if it can't be uploaded */
static inline s32 title_texture(SceneTitle *t, VObject *msg, s32 i) {
    TITLE_TEX_SLOT(i) = VCALL(msg, 0x24, s32 (*)(VObject *, s32, s32))(msg, 6, i);
    if (TITLE_TEX_SLOT(i) == -1) {
        return 1;
    }
    TITLE_TEX(i) = VCALL(msg, 0x28, void *(*)(VObject *, s32, s32))(msg, 6, i);
    if (!(TITLE_TEX_SLOT(i) & 0x80000000)) {
        return 1;
    }
    TITLE_TEX_SLOT(i) &= 0x7FFFFFFF;
    return (u8)VCALL(gRenderer, 0x44, s32 (*)(VObject *, s32, void *, s32))(
        gRenderer, TITLE_TEX_SLOT(i), TITLE_TEX(i), 0);
}

/* draw the title background (TITLE_BACK.BIN, 640 x 448) after loading the title textures */
void SceneTitle_PrepareBackground(SceneTitle *t) {
    VObject *msg;

    VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
    msg = gBootMessage;
    VCALL(msg, 0x20, void (*)(VObject *))(msg);
    if (!title_texture(t, msg, 0)) {
        return;
    }
    if (!title_texture(t, msg, 1)) {
        return;
    }
    VCALL(gRenderer, 0x4C, s32 (*)(VObject *, void *, s32, s32, s32))(
        gRenderer, t->backImage, 0x280, 0x1C0, 0);
}

#ifdef HG_NATIVE
#include "gl2d.h"

/* a title sprite (layer 0x30, blended): x0, y0 .. x1, y1 showing texels u0, v0 .. u1, v1 of
 * `tex` (palette `csa`) at `alpha` 0..1 */
static void title_sprite(const u8 *tex, f32 x0, f32 y0, f32 x1, f32 y1, f32 u0, f32 v0, f32 u1, f32 v1, s32 csa,
                         f32 alpha) {
    gl2d_sprite(0x30, x0, y0, x1, y1, tex, u0, v0, u1, v1, ((u32)(u8)(u32)(128.0f * alpha) << 24) | 0x808080, csa,
                0x40);
}

/* the title picture (512 x 336 of texture 0) at y 48, `alpha` 0..1 */
void SceneTitle_DrawPicture(SceneTitle *t, f32 alpha) {
    title_sprite(TITLE_TEX(0), 0, 48, 512, 384, 0, 0, 512, 336, 0, alpha);
}

/* the logo (texture 0, from v 336), centred on (256, 216), `alpha` 0..1, `scale` x its size
 * (512 x 176) */
void SceneTitle_DrawLogo(SceneTitle *t, f32 alpha, f32 scale) {
    s32 hh = (s32)(88.0f * scale), hw = (s32)(256.0f * scale);

    title_sprite(TITLE_TEX(0), 256 - hw, 216 - hh, 256 + hw, 216 + hh, 0, 336, 512, 512, 0, alpha);
}

/* Draw the w x h rectangle at u, v of title texture 1 (CLUT `clut`) at screen x, y, `alpha`
 * 0..1 (as vertex alpha, or with `fix` as fixed alpha, added). */
void SceneTitle_DrawRect(SceneTitle *t, s32 u, s32 v, s32 w, s32 h, s32 x, s32 y, s32 clut, s32 fix, f32 alpha) {
    u8 a8;

    if (alpha < 0.0f) {
        alpha = 0.0f;
    }
    if (!(alpha <= 1.0f)) {
        alpha = 1.0f;
    }
    a8 = (u8)(u32)(128.0f * alpha);
    if ((u8)fix == 0) {
        gl2d_sprite(0x30, x, y, x + w, y + h, TITLE_TEX(1), u, v, u + w, v + h, ((u32)a8 << 24) | 0x808080, clut, 0x40);
    } else {
        gl2d_sprite(0x30, x, y, x + w, y + h, TITLE_TEX(1), u, v, u + w, v + h, 0x80808080, clut,
                    gl2d_blend(((u64)a8 << 32) | 0x68));
    }
}
#endif

/* "PRESS START BUTTON" */
void SceneTitle_DrawPressStart(SceneTitle *t, f32 alpha) {
    SceneTitle_DrawRect(t, 0, 0xC0, 0x170, 0x20, 0x48, 0x188, 2, 0, alpha);
}

/* the pulsing part of it */
void SceneTitle_DrawPressStartGlow(SceneTitle *t, f32 alpha) {
    SceneTitle_DrawRect(t, 0, 0xA0, 0xC0, 0x20, 0xA0, 0x130, 3, 0, alpha);
}

/* (the scene) its part +0x97980's func_00303E60 */
s32 func_00384C50(u8 *g, s32 a1) {
    return ((s32 (*)(u8 *, s32))func_00303E60)(g + 0x97980, a1);   /* (void: v0 as it was left) */
}

s32 func_00384C60(u8 *g) {
    return AT(g, 0x97A8F, s8);
}

/* bit n of the scene's 0x97740 bitmap set / tested */
void func_00384C70(u8 *g, s32 n) {
    AT(g, 0x97740 + (n >> 5) * 4, u32) |= 1u << (n & 0x1F);
}

s32 func_00384CB0(u8 *g, s32 n) {
    return (AT(g, 0x97740 + (n >> 5) * 4, u32) & (1u << (n & 0x1F))) != 0;
}

void SceneTitle_SeqToMenu(SceneTitle *t);
void SceneTitle_SeqLeave(SceneTitle *t);

/* the title waiting for Start; idle for 900 frames: the attract movie */
void SceneTitle_SeqPressStart(SceneTitle *t) {
    VObject *snd;

    t->anim = (t->anim + 5000) % 360000;
    if (!t->loaded) {
        return;
    }
    SceneTitle_PrepareBackground(t);
    SceneTitle_DrawPicture(t, 1.0f);
    SceneTitle_DrawLogo(t, 1.0f, 1.0f);
    SceneTitle_DrawPressStart(t, 1.0f);
    SceneTitle_DrawPressStartGlow(t, 0.5f * (1.0f + func_0031C248(0x1.921fb6p+1f * (180.0f - (f32)t->anim / 1000.0f) / 180.0f)));
    if ((D_0047E36C & MENU_CONFIRM) || (D_0047E37C & PAD_START)) {
        ptmf_set_fn(&t->seq, SceneTitle_SeqToMenu);
        t->timer = 0;
        snd = gSound;
        Sound_Play(snd, 2, 7);
        if ((s8)VCALL(snd, 0x6C, s32 (*)(VObject *))(snd) != 0) {
            Sound_Play(snd, 3, 7);
        }
        return;
    }
    if (++t->timer < 0x385) {
        return;
    }
    t->next = t->next ? 0 : 1;
    ptmf_set_fn(&t->seq, SceneTitle_SeqLeave);
}

void SceneTitle_DrawBackImage(SceneTitle *t, f32 alpha, f32 scroll);
void SceneTitle_DrawMenuPanel(SceneTitle *t, f32 alpha, f32 open);
void SceneTitle_SeqMenu(SceneTitle *t);

/* the main menu's entries (texture 1, CLUT 1): New Game, Load Game, Options and, once the game
 * has been cleared (+0x11DA94), two more; with `stagger` the k-th at alpha - k / 8 */
static inline void menu_entries(SceneTitle *t, f32 alpha, s32 stagger, s32 all) {
    SceneTitle_DrawRect(t, 0, 0x00, 0xC0, 0x20, 0x20, 0x40, 1, 0, alpha);
    SceneTitle_DrawRect(t, 0, 0x20, 0xC0, 0x20, 0x20, 0x70, 1, 0, stagger ? -0.125f + alpha : alpha);
    SceneTitle_DrawRect(t, 0, 0x40, 0xC0, 0x20, 0x20, 0xA0, 1, 0, stagger ? -0.25f + alpha : alpha);
    if (!all) {
        return;
    }
    SceneTitle_DrawRect(t, 0, 0x60, 0xC0, 0x20, 0x20, 0xD0, 1, 0, stagger ? -0.375f + alpha : alpha);
    SceneTitle_DrawRect(t, 0, 0x80, 0xC0, 0x20, 0x20, 0x100, 1, 0, stagger ? -0.5f + alpha : alpha);
}

/* the title turning into the main menu: (1) a 30-frame fade of the title layer, (2) the menu
 * coming in, (3) then the menu (SceneTitle_SeqMenu) */
void SceneTitle_SeqToMenu(SceneTitle *t) {
    f32 f;

    if (!t->loaded) {
        return;
    }
    SceneTitle_PrepareBackground(t);
    switch (t->timer) {
    case 0:
        t->anim = 0;
        t->timer++;
        /* fall through */
    case 1:
        SceneTitle_DrawPicture(t, 1.0f);
        SceneTitle_DrawLogo(t, 1.0f, 1.0f);
        SceneTitle_DrawPressStart(t, 1.0f);
        SceneTitle_DrawPressStartGlow(t, 1.0f);
        f = (f32)t->anim++ / 30.0f;
        if (t->anim >= 0x1F) {
            t->anim = 0;
            f = 1.0f;
            t->timer++;
        }
        SceneTitle_DrawBackImage(t, f, 1.0f);
        break;
    case 2:
        f = (f32)t->anim++ / 30.0f;
        if (t->anim >= 0x1F) {
            t->anim = 0;
            f = 1.0f;
            t->timer++;
        }
        SceneTitle_DrawBackImage(t, 1.0f, 1.0f - f);
        SceneTitle_DrawMenuPanel(t, f, f);
        menu_entries(t, 1.5f * f, 1, t->extras);
        break;
    case 3:
        SceneTitle_DrawBackImage(t, 1.0f, 0.0f);
        SceneTitle_DrawMenuPanel(t, 1.0f, 1.0f);
        menu_entries(t, 1.0f, 0, t->extras);
        t->anim = 90000;
        ptmf_set_fn(&t->seq, SceneTitle_SeqMenu);
        break;
    }
}

#ifdef HG_NATIVE
/* The title background (TITLE_BACK.BIN as sent by the renderer's +0x4C: 4-bit 640 x 448 at
 * block 0x3400), the screen's 512 of it scrolled left by scroll x 128 pixels, `alpha` 0..1. */
void SceneTitle_DrawBackImage(SceneTitle *t, f32 alpha, f32 scroll) {
    s32 s = (u8)(u32)(128.0f * scroll);

    title_sprite(gl2d_image(0x3400), 0, 0, 512, 448, s, 0, s + 0x200, 448, 0, alpha);
}
#endif

#ifdef HG_NATIVE
/* The menu's panel: two bands (texture 1, v 224..256, CLUT 1) across the screen at y 16 and
 * y 400, shaded from one side to the other (the lower one mirrored), opening with `open` 0..1
 * (the left side first, then the right), `alpha` 0..1. */
void SceneTitle_DrawMenuPanel(SceneTitle *t, f32 alpha, f32 open) {
    u32 left, right;
    f32 f, xy[8], st[8];
    u8 c[16];
    s32 band, k;
    const u8 *tex = TITLE_TEX(1);

    f = 2.0f * open;
    if (!(f <= 1.0f)) {
        f = 1.0f;
    }
    left = ((u32)(u8)(u32)(128.0f * f * alpha) << 24) | 0x808080;
    f = 2.0f * (open - 0.5f);
    if (f <= 1.0f) {
        if (f < 0.0f) {
            f = 0.0f;
        }
    } else {
        f = 1.0f;
    }
    right = ((u32)(u8)(u32)(128.0f * f * alpha) << 24) | 0x808080;
    for (band = 0; band < 2; band++) {
        f32 y0 = band == 0 ? 16 : 400;

        for (k = 0; k < 4; k++) {   /* strip: (0, top) (0, bottom) (512, top) (512, bottom) */
            xy[k * 2] = k < 2 ? 0 : 512;
            xy[k * 2 + 1] = y0 + (k & 1) * 32;
            st[k * 2] = (k < 2 ? 0.0f : 512.0f) / AT(tex, 4, u16);
            st[k * 2 + 1] = ((k & 1) ? 256.0f : 224.0f) / AT(tex, 6, u16);
        }
        gl2d_colors(c, band == 0 ? left : right, 2);
        gl2d_colors(c + 8, band == 0 ? right : left, 2);
        glr_prim2d(0x30, GLR_2D_STRIP, 4, xy, st, c, tex, 1, 0x40);
    }
}
#endif

void SceneTitle_DrawMenu(SceneTitle *t, f32 alpha);
void func_0012E270(SceneTitle *t);
void SceneTitle_SeqLoadGame(SceneTitle *t);
void SceneTitle_SeqOptions(SceneTitle *t);

#define BGM_WANT(track, pause, restart, level) \
    VCALL(gMusic, 0x8, void (*)(void *, s32, s32, s32, f32))(gMusic, track, pause, restart, level)

/* the main menu: up / down choose (wrapping), cancel goes back to the title, confirm or Start
 * takes the entry; 900 idle frames: the attract movie */
void SceneTitle_SeqMenu(SceneTitle *t) {
    s8 old;
    s8 n;

    BGM_WANT(0x33, 0, 0, 1.0f);
    SceneTitle_DrawMenu(t, 1.0f);
    old = t->cursor;
    n = t->extras ? 5 : 3;
    if (D_0047E36C & MENU_UP) {
        t->cursor = old - 1;
        if (t->cursor < 0) {
            t->cursor = n - 1;
        }
    } else if (D_0047E36C & MENU_DOWN) {
        t->cursor++;
        if (n - 1 < t->cursor) {
            t->cursor = 0;
        }
    }
    if (old != t->cursor) {
        t->timer = 0;
        Sound_PlaySE(SE_CURSOR);
        t->anim = 90000;
    }
    if (D_0047E36C & MENU_CANCEL) {
        Sound_PlaySE(SE_CANCEL);
        t->timer = 0;
        BGM_WANT(0xFF, 0, 0, 1.0f);
        ptmf_set_fn(&t->seq, func_0012E270);
        return;
    }
    if (old == t->cursor && ((D_0047E36C & MENU_CONFIRM) || (D_0047E37C & PAD_START))) {
        BGM_WANT(0xFF, 0, 0, 1.0f);
        switch (t->cursor) {
        case 0:
            Sound_PlaySE(SE_DECIDE);
            t->next = 2;
            break;
        case 1:
            Sound_PlaySE(SE_DECIDE);
            t->sub.mode = 6;   /* the sub screen: load a game */
            ptmf_set_fn(&t->seq, SceneTitle_SeqLoadGame);
            return;
        case 2:
            t->sub.mode = 5;   /* the sub screen: options */
            ptmf_set_fn(&t->seq, SceneTitle_SeqOptions);
            return;
        case 3:
            Sound_PlaySE(SE_DECIDE);
            t->next = 5;
            break;
        case 4:
            Sound_PlaySE(SE_DECIDE);
            t->next = 6;
            break;
        default: {
            VObject *snd = gSound;

            t->next = 2;
            Sound_Play(snd, 0, 6);
            if ((s8)VCALL(snd, 0x6C, s32 (*)(VObject *))(snd) != 0) {
                Sound_Play(snd, 1, 6);
            }
            break;
        }
        }
        ptmf_set_fn(&t->seq, SceneTitle_SeqLeave);
        return;
    }
    if (++t->timer < 0x385) {
        return;
    }
    BGM_WANT(0xFF, 0, 0, 1.0f);
    t->next = t->next ? 0 : 1;
    ptmf_set_fn(&t->seq, SceneTitle_SeqLeave);
}

/* the highlighted entry k: drawn again in CLUT 0 (`a`, fixed alpha), its description shown */
static inline void menu_highlight(SceneTitle *t, s32 k, f32 a) {
    SceneTitle_DrawRect(t, 0, k * 0x20, 0xC0, 0x20, 0x20, 0x40 + k * 0x30, 0, 1, a);
    Task_Open(&t->task, 2 + k);
}

/* draw the main menu at `alpha`: the panel and entries, the chosen one pulsing with its
 * description below */
void SceneTitle_DrawMenu(SceneTitle *t, f32 alpha) {
    f32 pulse;
    u8 bit;

    D_0047B350 = 2;
    t->anim = (t->anim + 5000) % 360000;
    if (!t->loaded) {
        return;
    }
    SceneTitle_PrepareBackground(t);
    pulse = 0.5f * (1.0f + func_0031C248(0x1.921fb6p+1f * (180.0f - (f32)t->anim / 1000.0f) / 180.0f));
    bit = 1 << t->cursor;
    SceneTitle_DrawBackImage(t, alpha, 0.0f);
    SceneTitle_DrawMenuPanel(t, alpha, 1.0f);
    SceneTitle_DrawRect(t, 0, 0x00, 0xC0, 0x20, 0x20, 0x40, 1, 0, alpha);
    SceneTitle_DrawRect(t, 0, 0x20, 0xC0, 0x20, 0x20, 0x70, 1, 0, alpha);
    SceneTitle_DrawRect(t, 0, 0x40, 0xC0, 0x20, 0x20, 0xA0, 1, 0, alpha);
    if (t->extras) {
        SceneTitle_DrawRect(t, 0, 0x60, 0xC0, 0x20, 0x20, 0xD0, 1, 0, alpha);
        SceneTitle_DrawRect(t, 0, 0x80, 0xC0, 0x20, 0x20, 0x100, 1, 0, alpha);
    }
    if (bit & 1) {
        menu_highlight(t, 0, pulse * alpha);
    }
    if (bit & 2) {
        menu_highlight(t, 1, pulse * alpha);
    }
    if (bit & 4) {
        menu_highlight(t, 2, pulse * alpha);
    }
    if (t->extras) {
        if (bit & 8) {
            menu_highlight(t, 3, pulse * alpha);
        }
        if (bit & 0x10) {
            menu_highlight(t, 4, pulse * alpha);
        }
    }
    Task_Run(&t->task);
}

void SceneTitle_StateDemoStart(SceneTitle *t);
void SceneTitle_StateNewGame(SceneTitle *t);

/* leave the title once the music has stopped: release the title textures and sound bank, then
 * what the title leads to (t->next): 0 the attract movie, 1 the opening, 2 / 5 a new game,
 * 3 / 6 straight into game mode 3 (stage -1 / 0x37), 4 nothing */
void SceneTitle_SeqLeave(SceneTitle *t) {
    VObject *snd;

    if (!(u8)func_002D20D0(gAdx)) {
        return;
    }
    if (t->loaded) {
        VObject *msg = gBootMessage;

        VCALL(msg, 0x14, void (*)(VObject *, s32))(msg, 6);
        VCALL(msg, 0xC, void (*)(VObject *, s32))(msg, 6);
        t->loaded = 0;
    }
    snd = gSound;
    VCALL(snd, 0x10, void (*)(VObject *, s32, s32))(snd, 0, 0xF000);
    VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 7);
    switch (t->next) {
    case 0:
        ptmf_set_fn(&t->base.state, SceneTitle_StateAttractStart);
        break;
    case 1:
        ptmf_set_fn(&t->base.state, SceneTitle_StateDemoStart);
        break;
    case 2:
        ptmf_set_fn(&t->base.state, SceneTitle_StateNewGame);
        break;
    case 3:
        AT(gSystemData, 0x4, s32) = 3;
        AT(gSystemData, 0x10, s32) = -1;
        VCALL(t, 0x14, void (*)(SceneTitle *))(t);
        break;
    case 4:
        break;
    case 5:
        ptmf_set_fn(&t->base.state, SceneTitle_StateNewGame);
        break;
    case 6:
        AT(gSystemData, 0x4, s32) = 3;
        AT(gSystemData, 0x10, s32) = 0x37;
        VCALL(t, 0x14, void (*)(SceneTitle *))(t);
        break;
    }
}

/* +0x14 finish: end the movie scene, drop the sub screen's textures (group 0x19), reset the
 * message object, ask to be finished */
void SceneTitle_Finish(SceneTitle *t) {
    Scene *s = SCENE_TABLE_SCENE(1);

    if (s != NULL) {
        ptmf_set(&s->state, &sSceneFinish);
        VCALL(SCENE_TABLE_SCENE(1), 0x14, void (*)(Scene *))(SCENE_TABLE_SCENE(1));
    }
    VCALL(gTexCache, 0x14, void (*)(VObject *, s32))(gTexCache, 0x19);
    func_0026BC00((VObject *)t->msg);
    t->base.request = SCENE_REQ_FINISH;
}

extern void *D_0046A100[], *D_0046A0D0[];

/* the text object: release (its loads, its textures: group 0x28) */
void TextObj_Release(u8 *o) {
    VCALL(gFileLoader, 0x14, void (*)(VObject *, u32))(gFileLoader, 0x6000000);
    VCALL(gTexCache, 0x14, void (*)(VObject *, s32))(gTexCache, 0x28);
    AT(o, 0x108, s32) = -1;
    AT(o, 0x10C, u8) = 0xFF;
    AT(o, 0x10E, u8) = 0xFF;
}

/* the text object: destructor */
void *TextObj_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        Task *t;

        AT(o, 0x0, void **) = D_0046A068;
        TextObj_Release(o);
        t = (Task *)(o + 4);
        if (t != NULL && t->child != NULL) {
            Task_dtor(t->child, 1);
            t->child = NULL;
        }
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* the entry pool's destructor body: its list, its 192 entries, the global */
static inline void Pool_Destroy(u8 *pool) {
    AT(pool, 0x0, void **) = D_0046A078;
    if (pool + 0x1208 != NULL) {
        AT(pool, 0x1208, void **) = D_004699C0;
        if (pool + 0x1208 != NULL) {
            AT(pool, 0x1208, void **) = D_004699E0;
        }
    }
    func_001002C0(pool + 8, PoolEntry_dtor, 0x18, 0xC0);
    if (pool != NULL) {
        gSubPool = NULL;
    }
}

/* the entry pool (gSubPool): destructor */
void *func_00130920(u8 *pool, s32 flags) {
    if (pool != NULL) {
        Pool_Destroy(pool);
        if ((s16)flags > 0) {
            func_00100490(pool);
        }
    }
    return pool;
}

/* the sub screen's base: destructor (the pool and its entries) */
void *SubScreenBase_dtor(SubScreen *w, s32 flags) {
    if (w != NULL) {
        u8 *pool = w->pool;

        w->vtbl = D_0046A090;
        if (pool != NULL) {
            Pool_Destroy(pool);
        }
        if (w != NULL) {
            gSubScreen = NULL;
        }
        if ((s16)flags > 0) {
            func_00100490(w);
        }
    }
    return w;
}

/* destructor */
SceneTitle *SceneTitle_dtor(SceneTitle *t, s32 flags) {
    if (t != NULL) {
        SubScreen *w;
        Task *task;

        t->base.vtbl = D_0046A040;
        func_002E31D0(&t->bgm);
        func_002D2330(gAdx);
        if (&t->bgm != NULL) {
            t->bgm.vtbl = D_0046A110;
            if (&t->bgm != NULL) {
                t->bgm.vtbl = D_0046A100;
                if (&t->bgm != NULL) {
                    gMusic = NULL;
                }
            }
        }
        w = &t->sub;
        if (w != NULL) {
            w->vtbl = D_0047A790;
            BootCard_dtor(&w->card, -1);
            TextObj_dtor(w->textObj, -1);
            Task_dtor(&w->text, -1);
            Task_dtor(&w->ask, -1);
            SubScreenBase_dtor(w, 0);
        }
        task = &t->task;
        if (task != NULL && task->child != NULL) {
            Task_dtor(task->child, 1);
            task->child = NULL;
        }
        if (t->msg != NULL) {
            AT(t->msg, 0, void **) = D_0046D7D0;
            if (t->msg != NULL) {
                AT(t->msg, 0, void **) = D_0046A0D0;
                if (t->msg != NULL) {
                    gBootMessage = NULL;
                }
            }
        }
        if (&t->returnTo != NULL) {   /* (MW tests the address of the next base) */
            gSceneTitle = NULL;
        }
        if (t != NULL) {
            t->base.vtbl = Scene_vtable;
        }
        if ((s16)flags > 0) {
            func_0011F9A0(t);
        }
    }
    return t;
}

extern const char *D_003B0050[7];   /* "SYSTEM\\PLAY_DEMO_1.SFD" .. _7 */
void SceneTitle_StateDemo(SceneTitle *t);
void SceneTitle_StateDemoFade(SceneTitle *t);

/* state: the next of the seven gameplay demos */
void SceneTitle_StateDemoStart(SceneTitle *t) {
    title_movie_start(t, D_003B0050[t->demo]);
    t->demo = (u8)(t->demo + 1) % 7;
    ptmf_set_fn(&t->base.state, SceneTitle_StateDemo);
}

/* state: the demo */
void SceneTitle_StateDemo(SceneTitle *t) {
    title_movie_wait(t, SceneTitle_StateDemoFade);
}

/* state: fade the demo out, then the title (as after the attract movie) */
void SceneTitle_StateDemoFade(SceneTitle *t) {
    if (!title_movie_fade(t)) {
        ptmf_set_fn(&t->base.state, SceneTitle_StateTitle);
        ptmf_set_fn(&t->seq, SceneTitle_SeqLoad);
    }
}

/* state: the options screen (over the menu); back to the menu when it closes */
void SceneTitle_SeqOptions(SceneTitle *t) {
    if (t->sub.showBehind) {
        SceneTitle_DrawMenu(t, 1.0f);
    }
    SubScreen_Update(&t->sub);
    if (!t->sub.open) {
        ptmf_set_fn(&t->seq, SceneTitle_SeqMenu);
        t->timer = 0;
    }
}

/* state: the load screen; when it closes, leave the title with the loaded game (next 3) or go
 * back to the menu */
void SceneTitle_SeqLoadGame(SceneTitle *t) {
    if (t->sub.showBehind && t->sub.card.state != -2) {
        SceneTitle_DrawMenu(t, 1.0f);
    }
    SubScreen_Update(&t->sub);
    if (t->sub.open) {
        return;
    }
    if (t->sub.card.state == -2) {
        t->next = 3;
        ptmf_set_fn(&t->seq, SceneTitle_SeqLeave);
    } else {
        ptmf_set_fn(&t->seq, SceneTitle_SeqMenu);
        t->timer = 0;
    }
}

/* ---- the sub screen's destructor and scene mode 5 (2026-10-05) ---- */

extern void *D_0047A330[], *D_0046A058[], *D_0046A078[], *D_0046A090[], *D_004699C0[], *D_004699E0[];

static inline void task_end_child(Task *t) {
    if (t != NULL && t->child != NULL) {
        Task_dtor(t->child, 1);
        t->child = NULL;
    }
}

/* the sub screen (D_0047A790): its load / save screens, text object and two text tasks, then
 * the base (D_0046A090): the pool's entries, the globals gSubPool / gSubScreen cleared */
void *func_002D0110(SubScreen *w, s32 flags) {
    if (w != NULL) {
        u8 *o = (u8 *)w;

        w->vtbl = D_0047A790;
        AT(o, 0xA8AC0, void **) = D_0046A058;
        Task_dtor((Task *)(o + 0xA8AD8), -1);
        AT(o, 0x97980, void **) = D_0046A068;
        TextObj_Release(o + 0x97980);
        Task_dtor((Task *)(o + 0x97984), -1);
        task_end_child(&w->text);
        task_end_child(&w->ask);
        w->vtbl = D_0046A090;
        AT(o, 0x8, void **) = D_0046A078;
        AT(o, 0x1210, void **) = D_004699C0;
        AT(o, 0x1210, void **) = D_004699E0;
        func_001002C0(o + 0x10, PoolEntry_dtor, 0x18, 0xC0);
        gSubPool = NULL;
        gSubScreen = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return w;
}

/* scene mode 5, the ending (0x117540 bytes; vtable D_0047A330, scene_ending.c): the sub screen
 * (+0x180) for the clear-data save, the message object (+0xA9180), the BGM (+0x1174E4) and a
 * task (+0x48) */
void *Scene5_ctor(u8 *s) {
    SubScreen *w = (SubScreen *)(s + 0x180);

    AT(s, 0x0, void **) = Scene_vtable;
    ptmf_set((PTMF *)(s + 0x4), &sSceneEntryState);
    AT(s, 0x0, void **) = D_0047A330;
    Task_Construct((Task *)(s + 0x48));
    SubScreenBase_ctor(w);
    w->vtbl = D_0047A790;
    Task_ctor(&w->ask);
    Task_ctor(&w->text);
    TextObj_ctor(w->textObj);
    BootCard_ctor(&w->card);
    gBootMessage = (VObject *)(s + 0xA9180);
    AT(s, 0xA9180, void **) = D_0046D7D0;
    gMusic = (VObject *)(s + 0x1174E4);
    AT(s, 0x1174E4, void **) = D_0046A110;
    AT(s, 0x117500, PTMF) = sGameStateNull;
    func_002D2370(gAdx, s + 0xF4300);
    func_002E34D0(s + 0x1174E4);
    return s;
}

extern void func_0012DA20(SceneTitle *t, f32 alpha);   /* the menu, at `alpha` */
extern const PTMF D_003B0250;

/* back from the menu to the title over 16 frames: the menu fading out (func_0012DA20) and a
 * black screen lifting while the picture, logo and PRESS START come back; then D_003B0250 */
void func_0012E270(SceneTitle *t) {
    f32 f = (f32)t->timer++ / 15.0f;
    s32 done = 0;
    u32 a;

    if (t->timer >= 0x10) {
        f = 1.0f;
        done = 1;
    }
    func_0012DA20(t, 1.0f - f);
    a = (u32)(127.0f * f);
    if (a >= 0x80) {
        a = 0x7F;
    }
    VCALL(gRenderer, 0x7C, RectFn)(gRenderer, 0, 0, 0x200, 0x200, 0, 0, 0, 0, (a << 24) & 0xFF000000, -1, 0, 0x30, -1);
    SceneTitle_DrawPicture(t, f);
    SceneTitle_DrawLogo(t, f, 1.0f);
    SceneTitle_DrawPressStart(t, f);
    if (done) {
        t->timer = 0;
        t->anim = 0;
        ptmf_set(&t->seq, &D_003B0250);
    }
}

/* the title menu at `alpha`: the language 2, the glow's phase on; with the files loaded the
 * background and panel, the items (NEW GAME / LOAD GAME / OPTIONS, and the two extras once
 * the game is finished) and the cursor's item lit by a pulsing added copy */
void func_0012DA20(SceneTitle *t, f32 alpha) {
    static const s16 kY[5] = {0x40, 0x70, 0xA0, 0xD0, 0x100};
    f32 glow;
    u8 sel;
    s32 i;

    D_0047B350 = 2;
    t->anim = (t->anim + 5000) % 360000;
    if (!t->loaded) {
        return;
    }
    SceneTitle_PrepareBackground(t);
    glow = 0.5f * (1.0f + func_0031C248(0x1.921fb6p+1f * (180.0f - (f32)t->anim / 1000.0f) / 180.0f));
    sel = 1 << t->cursor;
    SceneTitle_DrawBackImage(t, alpha, 0.0f);
    SceneTitle_DrawMenuPanel(t, alpha, 1.0f);
    for (i = 0; i < 3; i++) {
        SceneTitle_DrawRect(t, 0, i * 0x20, 0xC0, 0x20, 0x20, kY[i], 1, 0, alpha);
    }
    if (t->extras) {
        SceneTitle_DrawRect(t, 0, 0x60, 0xC0, 0x20, 0x20, kY[3], 1, 0, alpha);
        SceneTitle_DrawRect(t, 0, 0x80, 0xC0, 0x20, 0x20, kY[4], 1, 0, alpha);
    }
    for (i = 0; i < 3; i++) {
        if (sel & (1 << i)) {
            SceneTitle_DrawRect(t, 0, i * 0x20, 0xC0, 0x20, 0x20, kY[i], 0, 1, glow * alpha);
        }
    }
    if (!t->extras) {
        return;
    }
    if (sel & 0x8) {
        SceneTitle_DrawRect(t, 0, 0x60, 0xC0, 0x20, 0x20, kY[3], 0, 1, glow * alpha);
    }
    if (sel & 0x10) {
        SceneTitle_DrawRect(t, 0, 0x80, 0xC0, 0x20, 0x20, kY[4], 0, 1, glow * alpha);
    }
}
