/* Mode 2 scene: the title screen and its menus (new game, load, options, extras), the opening
 * movie. A large object (0x140CE0 bytes from the scene heap). */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "task.h"
#include "input.h"
#include "sound.h"


/*
 * SceneTitle layout (what's known):
 *   0x000000 Scene
 *   0x000014 s32
 *   0x000024 the message object (vtable D_0046D7D0; gBootMessage points here)
 *   0x074940 u8
 *   0x074944 Task
 *   0x074A80 the title work (vtable D_0047A790, see func_002D04C0), with
 *            +0x097764 / +0x097868 Tasks, +0x097980 a text object (func_002D03A0),
 *            +0x0A8AC0 a memory card check (BootCard)
 *   0x11DA80 s32, 0x11DA94 u8
 *   0x11DAC0 the music stream's work buffer (0x231E4 bytes)
 *   0x140CA4 a small object (vtable D_0046A110, global D_0044E970), its state at +0x1C
 */
#define TITLE_TASK 0x74944
#define TITLE_WORK 0x74A80
#define TITLE_BGM_WORK 0x11DAC0
#define TITLE_UNK140CA4 0x140CA4
#define MENU_CURSOR AT(t, 0x21, s8)  /* the main menu's entry */
#define TITLE_NEXT AT(t, 0x20, u8)   /* what the title leads to (2 new game, 5 / 6 extras) */

extern void *Scene_vtable[];
extern void *D_0046A040[];          /* SceneTitle */
extern void *D_0046D7D0[];          /* the message object */
extern void *D_0047A790[];          /* the title work */
extern void *D_0046A110[];
extern void *D_0046A090[], *D_0046A078[], *D_004699E0[], *D_004699C0[], *D_0046A068[];
extern void *D_0046C790[];          /* a pool entry */
extern const PTMF sSceneEntryState; /* virtual: vtable +0x10 */
extern const PTMF sGameStateNull;
extern void *D_0044E968;            /* the title scene */
extern void *D_0044E970;
extern void *D_0044E980;            /* the ADX sound system */
extern void *D_0044E988, *D_0044E990;
extern void *gBootMessage;
extern void func_00100340(void *array, void *(*ctor)(void *), void *(*dtor)(void *, s32), u32 size, u32 n);   /* __construct_array */
extern void func_0025FEF0(void *p);   /* operator delete (pool entries) */
extern void func_002D2370(void *bgm, void *work);
extern void func_002E34D0(void *obj);
extern void *func_002D0300(void *card);   /* BootCard constructor */

/* a pool entry: constructor / destructor */
void *func_002D0570(void *e) {
    AT(e, 0x0, void **) = D_0046C790;
    AT(e, 0x4, s32) = -1;
    AT(e, 0x8, u8) = 0;
    AT(e, 0x10, s64) = 0;
    return e;
}

void *func_0012C500(void *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x0, void **) = D_0046C790;
        if ((s16)flags > 0) {
            func_0025FEF0(e);
        }
    }
    return e;
}

/* the title work's base: a pool of 192 entries (global D_0044E990) and its list */
void *func_002D04C0(u8 *w) {
    u8 *pool = w + 8;

    D_0044E988 = w;
    AT(w, 0x0, void **) = D_0046A090;
    D_0044E990 = pool;
    AT(pool, 0x0, void **) = D_0046A078;
    func_00100340(pool + 8, func_002D0570, func_0012C500, 0x18, 0xC0);
    AT(pool, 0x1208, void **) = D_004699E0;
    AT(pool, 0x120C, s32) = 0;
    AT(pool, 0x1210, s32) = 0;
    AT(pool, 0x1208, void **) = D_004699C0;
    AT(pool, 0x1214, s32) = 0;
    AT(pool, 0x1218, s32) = 0;
    AT(pool, 0x121C, s32) = 0;
    AT(w, 0x4, u8) = 0;
    return w;
}

/* a text object: a Task and what it shows */
void *func_002D03A0(u8 *o) {
    AT(o, 0x0, void **) = D_0046A068;
    Task_Construct((Task *)(o + 4));
    AT(o, 0x108, s32) = -1;
    AT(o, 0x10C, u8) = 0xFF;
    AT(o, 0x10E, u8) = 0xFF;
    return o;
}

/* constructor */
Scene *SceneTitle_ctor(Scene *t) {
    u8 *w;

    t->vtbl = Scene_vtable;
    ptmf_set(&t->state, &sSceneEntryState);
    D_0044E968 = t;
    gBootMessage = (u8 *)t + 0x24;
    t->vtbl = D_0046A040;
    AT(t, 0x24, void **) = D_0046D7D0;
    Task_Construct((Task *)((u8 *)t + TITLE_TASK));
    w = (u8 *)t + TITLE_WORK;
    func_002D04C0(w);
    AT(w, 0x0, void **) = D_0047A790;
    func_002D0440((Task *)(w + 0x97764));
    func_002D0440((Task *)(w + 0x97868));
    func_002D03A0(w + 0x97980);
    func_002D0300(w + 0xA8AC0);
    D_0044E970 = (u8 *)t + TITLE_UNK140CA4;
    AT(t, TITLE_UNK140CA4, void **) = D_0046A110;
    AT(t, 0x11DA80, s32) = 0;
    AT(t, 0x74940, u8) = 0;
    AT(t, 0x14, s32) = 0;
    AT(t, 0x11DA94, u8) = 0;
    AT(t, TITLE_UNK140CA4 + 0x1C, PTMF) = sGameStateNull;
    func_002D2370(D_0044E980, (u8 *)t + TITLE_BGM_WORK);
    func_002E34D0((u8 *)t + TITLE_UNK140CA4);
    return t;
}

extern u8 *D_0044E978;          /* the system data */
extern u8 D_0047B350;           /* the language */
extern void func_0026BCC0(void *msg);
extern void func_00399A10(void *sub);
extern void func_003921A0(void *sub);
void func_001306E0(Scene *t);

/* +0x10 entry: reset the message object and the sub screen, apply the options, then the title
 * sequence (func_001306E0) */
void func_00130810(Scene *t) {
    u8 *sys = D_0044E978;
    PTMF s = {0, -1, {(void *)func_001306E0}};

    AT(t, 0x18, s32) = 0;
    if (AT(sys, 0x2C, u32) & 0x100000) {
        AT(t, 0x21, u8) = 1;
    } else {
        AT(t, 0x21, u8) = 0;
    }
    AT(t, 0x20, u8) = 0;
    AT(t, 0x22, u8) = 0;
    func_0026BCC0((u8 *)t + 0x24);
    func_00399A10((u8 *)t + TITLE_WORK);
    func_003921A0((u8 *)t + TITLE_WORK);
    D_0047B350 = 2;
    AT(t, 0x11DA94, u8) = 0;
    if (AT(sys, 0x24, u32) & 1) {
        AT(t, 0x11DA94, u8) = 1;
    }
    AT(t, 0x140CD0, u8) = 0;
    if (ptmf_test(&s)) {
        t->state = s;
    }
}

extern VObject *gFileLoader;
void func_00130520(Scene *t);
void func_0012FB50(Scene *t);
void func_0012F720(Scene *t);

/* state: once the textures are loaded, either back to the menu (+0x14 = 2) or the title
 * (func_0012FB50, with the +0x140CA4 object running func_0012F720) */
void func_001306E0(Scene *t) {
    if (VCALL(gFileLoader, 0x24, s32 (*)(VObject *))(gFileLoader) == 2) {
        return;
    }
    if (AT(t, 0x14, s32) == 2) {
        AT(t, 0x14, s32) = 0;
        ptmf_set_fn(&t->state, func_00130520);
        return;
    }
    ptmf_set_fn(&t->state, func_0012FB50);
    ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012F720);
}

extern void func_002E3200(void *obj);

/* state: the title; run the title's own sequence (its state at +0x140CC0), then the
 * +0x140CA4 object */
void func_0012FB50(Scene *t) {
    PTMF *seq = &AT(t, TITLE_UNK140CA4 + 0x1C, PTMF);

    if (ptmf_test(seq)) {
        ptmf_scall(t, seq);
    }
    func_002E3200((u8 *)t + TITLE_UNK140CA4);
}

extern void *D_0044E960;          /* the scene table: scenes[] at +4, the scene heap at +0x10D9040 */
extern void *D_0044E958;          /* the movie playing */
extern void *D_0046ECC0[];        /* SceneMovie */
extern const char D_0044E940[];   /* "SYSTEM\\LOOP_DEMO.SFD" */
extern void *__nw__FUiPv(u32 size, void *p);
extern void *func_002B70D0(void *movie);
extern void func_002B6D10(void *movie, const char *path, s32 mode, s32 keep);
extern void func_002B6340(void *movie);
void func_00130460(Scene *t);

#define SCENE_TABLE_SCENE(i) (*(Scene **)((u8 *)D_0044E960 + 4 + (i) * 4))

/* start movie `name` as scene 1 at the title's movie volume (+0x140CCC, reset to 1) */
static inline void title_movie_start(Scene *t, const char *name) {
    void *table = D_0044E960;
    VObject *heap = (VObject *)((u8 *)table + 0x10D9040);
    Scene *movie;
    void *mem;
    s32 ok;

    AT(t, 0x18, s32) = 0;
    AT(t, 0x22, u8) = 0;
    AT(t, 0x140CCC, f32) = 1.0f;
    mem = VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, 0x600200);
    if (mem != NULL) {
        movie = __nw__FUiPv(0x600200, mem);
        if (movie != NULL) {
            func_002B70D0(movie);
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
        u8 *m = D_0044E958;
        f32 *v = &AT(m, 0x1D4, f32);

        func_002B6D10(m, name, 1, 0);
        *v = AT(t, 0x140CCC, f32);
        if (*v < 0.0f) {
            *v = 0.0f;
        }
        if (!(*v <= 1.0f)) {
            *v = 1.0f;
        }
        func_002B6340(m);
    }
}

/* ... then state `next` */
static inline void title_movie(Scene *t, const char *name, void (*next)(Scene *)) {
    title_movie_start(t, name);
    ptmf_set_fn(&t->state, next);
}

/* state: start the attract movie */
void func_00130520(Scene *t) {
    title_movie(t, D_0044E940, func_00130460);
}

extern const char D_0044E888[];   /* "OPENING.SFD" */
void func_0012CA70(Scene *t);

/* state: a new game: the opening movie first */
void func_0012CB30(Scene *t) {
    title_movie(t, D_0044E888, func_0012CA70);
}

void func_00130170(Scene *t);

/* wait for the movie (Start, once it shows, skips it), then state `next` */
static inline void title_movie_wait(Scene *t, void (*next)(Scene *)) {
    if ((D_0047E37C & PAD_START) && D_0044E958 != NULL && AT(D_0044E958, 0x1B4, u8)) {
        AT(t, 0x22, u8) = 1;
    }
    if (D_0044E958 != NULL && !AT(t, 0x22, u8)) {
        return;
    }
    ptmf_set_fn(&t->state, next);
}

/* state: the attract movie */
void func_00130460(Scene *t) {
    title_movie_wait(t, func_00130170);
}

void func_0012C7B0(Scene *t);

/* state: the opening movie */
void func_0012CA70(Scene *t) {
    title_movie_wait(t, func_0012C7B0);
}

extern VObject *D_0044E4F0;      /* the renderer */
static const PTMF sSceneFinish = {0, 0x14, {(void *)0}};   /* virtual +0x14 */

/* the renderer's +0x7C: a rectangle (x, y, w, h; colour; layer ...) */
typedef void (*RectFn)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, u32, s32, s32, s32, s32);

/* fade the movie out (sound and picture, 1/30 a frame; at 0 its scene is finished); 0 once
 * it's gone */
static inline s32 title_movie_fade(Scene *t) {
    u8 *m = D_0044E958;
    f32 *level = &AT(t, 0x140CCC, f32);
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
    m = D_0044E958;
    func_002B6340(m);
    if (AT(m, 0x1B4, u8)) {
        u32 a = (u32)(127.0f * (1.0f - *level));

        if (a >= 0x80) {
            a = 0x7F;
        }
        VCALL(D_0044E4F0, 0x7C, RectFn)(D_0044E4F0, 0, 0, 0x200, 0x200, 0, 0, 0, 0, (a << 24) & 0xFF000000, -1, 0, 0x30, -1);
    }
    return 1;
}

/* state: fade the attract movie out, then the title */
void func_00130170(Scene *t) {
    if (!title_movie_fade(t)) {
        ptmf_set_fn(&t->state, func_0012FB50);
        ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012F720);
    }
}

/* state: fade the opening out, then start the game (mode 3, stage 0x2A; flagged for the extra
 * mode) */
void func_0012C7B0(Scene *t) {
    if (!title_movie_fade(t)) {
        AT(D_0044E978, 0x4, s32) = 3;
        AT(D_0044E978, 0x10, s32) = TITLE_NEXT == 5 ? 0x4000002A : 0x2A;
        VCALL(t, 0x14, void (*)(Scene *))(t);
    }
}

extern void *func_00114DA8(s32 align, s32 size);   /* memalign */
extern void func_00114FD0(void *p);                /* free */
extern void func_002D1FD0(void *bgm);
extern const char D_0044E8C0[];   /* "SYSTEM\\TITLE.TEX" */
extern const char D_0044E8E0[];   /* "SYSTEM\\TITLE_BACK.BIN" */
extern const char D_0044E900[];   /* "SYSTEM\\TITLE.HD" */
extern const char D_0044E910[];   /* "SYSTEM\\TITLE.SDT" */
extern const char D_0044E930[];   /* "SYSTEM\\TITLE.BD" */
void func_0012F290(Scene *t);
void func_0012F500(Scene *t);

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
 * (func_0012F500, title music) or, after a game, the menu straight away (func_0012F290) */
void func_0012F720(Scene *t) {
    VObject *msg;
    VObject *snd;

    AT(t, 0x18, s32) = 0;
    LOADER_LOAD(D_0044E8C0, (u8 *)t + 0x140);
    msg = gBootMessage;
    VCALL(msg, 0x8, void (*)(VObject *, s32, void *))(msg, 6, (u8 *)t + 0x140);
    VCALL(msg, 0x10, void (*)(VObject *, s32, void *, s32))(msg, 6, (u8 *)t + 0x140, 0);
    LOADER_LOAD(D_0044E8E0, (u8 *)t + 0x51140);
    snd = D_0044E560;
    AT(t, 0x74940, u8) = 1;
    VCALL(snd, 0xA0, void (*)(VObject *))(snd);
    VCALL(snd, 0xAC, void (*)(VObject *, f32))(snd, 1.0f);
    {
        f32 *v = &AT(D_0044E980, 0x120, f32);

        *v = 1.0f;
        if (*v < 0.0f) {
            *v = 0.0f;
        }
        if (!(*v <= 1.0f)) {
            *v = 1.0f;
        }
        func_002D1FD0(D_0044E980);
    }
    load_bank_part(snd, D_0044E900, 0x4C);
    load_bank_part(snd, D_0044E910, 0x50);
    load_bank_part(snd, D_0044E930, 0x58);
    {
        VObject *s = D_0044E560;

        VCALL(s, 0x60, void (*)(VObject *, s32))(s, 7);
        VCALL(s, 0x7C, void (*)(VObject *, s32, s32))(s, 1, 0);
    }
    if (AT(t, 0x22, u8) == 0 && AT(t, 0x14, s32) == 0) {
        VObject *s;

        AT(t, 0x1C, s32) = 0;
        s = D_0044E560;
        Sound_Play(s, 0, 7);
        if ((s8)VCALL(s, 0x6C, s32 (*)(VObject *))(s) != 0) {
            Sound_Play(snd, 1, 7);
        }
        ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012F500);
        return;
    }
    AT(t, 0x14, s32) = 0;
    AT(t, 0x1C, s32) = 90000;
    ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012F290);
}

extern f32 func_0031C248(f32 x);   /* sinf */
void func_0012D7F0(Scene *t);
void func_0037E950(Scene *t, f32 alpha);
void func_0037E6A0(Scene *t, f32 alpha, f32 scale);


/* the title fading in over 150 frames: the picture, then (from frame 90) the logo coming in
 * from large; then the menu (func_0012F290) */
void func_0012F500(Scene *t) {
    s32 done = 0;
    f32 a, b, f;

    AT(t, 0x1C, s32)++;
    if (AT(t, 0x1C, s32) >= 0x97) {
        AT(t, 0x1C, s32) = 150;
        done = 1;
    }
    a = 1.0f;
    f = (f32)AT(t, 0x1C, s32) / 150.0f;
    if (f <= 1.0f) {
        a = f;
    }
    f = (f32)(AT(t, 0x1C, s32) - 90) / 60.0f;
    if (f < 0.0f) {
        f = 0.0f;
    }
    b = 1.0f;
    if (f <= 1.0f) {
        b = f;
    }
    if (!AT(t, 0x74940, u8)) {
        return;
    }
    func_0012D7F0(t);
    func_0037E950(t, a);
    func_0037E6A0(t, b, 1.0f + 4.0f * func_0031C248(0x1.921fb6p+1f * (90.0f * (1.0f - b)) / 180.0f));
    if (done) {
        AT(t, 0x1C, s32) = 90000;
        ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012F290);
    }
}

extern VObject *D_0044E4E8;   /* the texture cache */

/* the title picture's two textures: their VRAM slots (+0x11DA84 / +0x11DA88; bit 31 just
 * assigned) and entries (+0x11DA8C / +0x11DA90) */
#define TITLE_TEX_SLOT(i) AT(t, 0x11DA84 + (i) * 4, s32)
#define TITLE_TEX(i) AT(t, 0x11DA8C + (i) * 4, void *)

/* make sure texture `i` of the title set (6) is in VRAM: 0 if it can't be uploaded */
static inline s32 title_texture(Scene *t, VObject *msg, s32 i) {
    TITLE_TEX_SLOT(i) = VCALL(msg, 0x24, s32 (*)(VObject *, s32, s32))(msg, 6, i);
    if (TITLE_TEX_SLOT(i) == -1) {
        return 1;
    }
    TITLE_TEX(i) = VCALL(msg, 0x28, void *(*)(VObject *, s32, s32))(msg, 6, i);
    if (!(TITLE_TEX_SLOT(i) & 0x80000000)) {
        return 1;
    }
    TITLE_TEX_SLOT(i) &= 0x7FFFFFFF;
    return (u8)VCALL(D_0044E4F0, 0x44, s32 (*)(VObject *, s32, void *, s32))(
        D_0044E4F0, TITLE_TEX_SLOT(i), TITLE_TEX(i), 0);
}

/* draw the title background (TITLE_BACK.BIN, 640 x 448) after loading the title textures */
void func_0012D7F0(Scene *t) {
    VObject *msg;

    VCALL(D_0044E4E8, 0x18, void (*)(VObject *))(D_0044E4E8);
    msg = gBootMessage;
    VCALL(msg, 0x20, void (*)(VObject *))(msg);
    if (!title_texture(t, msg, 0)) {
        return;
    }
    if (!title_texture(t, msg, 1)) {
        return;
    }
    VCALL(D_0044E4F0, 0x4C, s32 (*)(VObject *, void *, s32, s32, s32))(
        D_0044E4F0, (u8 *)t + 0x51140, 0x280, 0x1C0, 0);
}

extern VObject *D_0044E9A0;   /* the VRAM manager */

/* the start of a title sprite packet (layer 0x30): Z writes off, blending, a textured sprite
 * of the title texture 0 with clamp `clamp`; returns the packet (NULL: no room) */
static inline u64 *title_sprite(Scene *t, u64 clamp) {
    u64 *p = VCALL(D_0044E4F0, 0x10, u64 *(*)(VObject *, s32, s32))(D_0044E4F0, 0xC, 0x30);
    u8 *tex;

    if (p == NULL) {
        return NULL;
    }
    p[0] = 0x1000000B;              /* DMA cnt 11 */
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x5000000B;   /* VIF DIRECT 11 */
    p[2] = 0x8003 | (1ULL << 60);   /* GIF tag: 3 A+D, EOP */
    p[3] = 0xE;
    p[4] = (1ULL << 32) | 0x310000A0;   /* ZBUF_1: Z24 at 0xA0, masked */
    p[5] = 0x4E;
    p[6] = 0x44;                    /* ALPHA_1 */
    p[7] = 0x42;
    p[8] = 0x156;                   /* PRIM: sprite, textured, blended, UV */
    p[9] = 0;
    p[10] = 0x8001 | (0x84ULL << 56);   /* reglist: CLAMP TEX0 RGBAQ UV XYZ2 UV XYZ2 NOP */
    p[11] = 0xF5353168;
    p[12] = clamp;
    tex = TITLE_TEX(0);
    p[13] = VCALL(D_0044E9A0, 0x28, u64 (*)(VObject *, s32, s32, s32, s32, s32))(
        D_0044E9A0, TITLE_TEX_SLOT(0), tex[0], AT(tex, 4, u16), AT(tex, 6, u16), tex[1]);
    return p;
}

/* the end of it: Z writes on again */
static inline void title_sprite_end(u64 *p) {
    p[20] = 0x8001 | (1ULL << 60);
    p[21] = 0xE;
    p[22] = 0x310000A0;
    p[23] = 0x4E;
}

/* the title picture (512 x 336 of texture 0), `alpha` 0..1 */
void func_0037E950(Scene *t, f32 alpha) {
    u64 *p = title_sprite(t, (0x53CULL << 32) | 0x7FC00A);

    if (p == NULL) {
        return;
    }
    p[14] = ((u64)(u8)(u32)(128.0f * alpha) << 24) | 0x808080;
    p[15] = 0x80008;
    p[16] = 0xFFFFFFFF75007000ULL;
    p[17] = 0x15082008;
    p[18] = 0xFFFFFFFF8A009000ULL;
    p[19] = 0;
    title_sprite_end(p);
}

/* the logo (texture 0, from v 336), centred, `alpha` 0..1, `scale` x its size (512 x 176) */
void func_0037E6A0(Scene *t, f32 alpha, f32 scale) {
    u64 *p = title_sprite(t, (0x7FDULL << 32) | 0x507FC00A);
    s32 hh, hw;

    if (p == NULL) {
        return;
    }
    p[14] = ((u64)(u8)(u32)(128.0f * alpha) << 24) | 0x808080;
    p[15] = 0x15080008;
    hh = (s32)(88.0f * scale);
    hw = (s32)(256.0f * scale);
    p[16] = (u64)(u32)((0x800 - hw) << 4) | ((u64)(u32)((0x7F8 - hh) << 4) << 16) | 0xFFFFFFFF00000000ULL;
    p[17] = 0x20082008;
    p[18] = (u64)(u32)((0x800 + hw) << 4) | ((u64)(u32)((0x7F8 + hh) << 4) << 16) | 0xFFFFFFFF00000000ULL;
    p[19] = 0;
    title_sprite_end(p);
}

/* Draw the w x h rectangle at u, v of title texture 1 (CLUT `clut`) at screen x, y, `alpha`
 * 0..1 (as vertex alpha, or with `fix` as fixed alpha). */
void func_0012D140(Scene *t, s32 u, s32 v, s32 w, s32 h, s32 x, s32 y, s32 clut, s32 fix, f32 alpha) {
    u64 *p;
    u8 *tex;
    u8 a8;

    if (alpha < 0.0f) {
        alpha = 0.0f;
    }
    if (!(alpha <= 1.0f)) {
        alpha = 1.0f;
    }
    p = VCALL(D_0044E4F0, 0x10, u64 *(*)(VObject *, s32, s32))(D_0044E4F0, 0xD, 0x30);
    if (p == NULL) {
        return;
    }
    p[0] = 0x1000000C;
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x5000000C;
    p[2] = 0x8005 | (1ULL << 60);
    p[3] = 0xE;
    p[4] = (1ULL << 32) | 0x310000A0;   /* ZBUF_1: masked */
    p[5] = 0x4E;
    p[6] = 0x156;
    p[7] = 0;
    tex = TITLE_TEX(1);
    p[8] = VCALL(D_0044E9A0, 0x2C, u64 (*)(VObject *, s32, s32, s32, s32))(
        D_0044E9A0, TITLE_TEX_SLOT(1), AT(tex, 4, u16), AT(tex, 6, u16), tex[1]);
    p[9] = 0x06;                    /* TEX0_1 */
    tex = TITLE_TEX(1);
    p[10] = VCALL(D_0044E9A0, 0x34, u64 (*)(VObject *, s32, s32, s32, s32))(
        D_0044E9A0, TITLE_TEX_SLOT(1), clut, tex[0], tex[1]);
    p[11] = 0x16;                   /* TEX2_1 */
    a8 = (u8)(u32)(128.0f * alpha);
    if ((u8)fix == 0) {
        p[12] = 0x44;
    } else {
        p[12] = ((u64)a8 << 32) | 0x68;
    }
    p[13] = 0x42;
    p[14] = 0x8001 | (0x64ULL << 56);   /* reglist: CLAMP RGBAQ UV XYZ2 UV XYZ2 */
    p[15] = 0x535318;
    p[16] = 0xA | ((u64)(s64)u << 4) | ((u64)(s64)(u + w - 1) << 14) | ((u64)(s64)v << 24)
            | ((u64)(s64)(v + h - 1) << 34);
    if ((u8)fix == 0) {
        p[17] = ((u64)a8 << 24) | 0x808080;
    } else {
        p[17] = 0x80808080;
    }
    p[18] = (u64)(s64)((u << 4) + 8) | ((u64)(s64)((v << 4) + 8) << 16);
    p[19] = (u64)(u32)((x + 0x700) << 4) | ((u64)(u32)((y + 0x720) << 4) << 16) | 0xFFFFFFFF00000000ULL;
    p[20] = (u64)(s64)(((u + w) << 4) + 8) | ((u64)(s64)(((v + h) << 4) + 8) << 16);
    p[21] = (u64)(u32)((x + 0x700 + w) << 4) | ((u64)(u32)((y + 0x720 + h) << 4) << 16) | 0xFFFFFFFF00000000ULL;
    p[22] = 0x8001 | (1ULL << 60);
    p[23] = 0xE;
    p[24] = 0x310000A0;
    p[25] = 0x4E;
}

/* "PRESS START BUTTON" */
void func_0037EB80(Scene *t, f32 alpha) {
    func_0012D140(t, 0, 0xC0, 0x170, 0x20, 0x48, 0x188, 2, 0, alpha);
}

/* the pulsing part of it */
void func_0037EBC0(Scene *t, f32 alpha) {
    func_0012D140(t, 0, 0xA0, 0xC0, 0x20, 0xA0, 0x130, 3, 0, alpha);
}

void func_0012EE30(Scene *t);
void func_0012E450(Scene *t);

/* the title waiting for Start; idle for 900 frames: the attract movie */
void func_0012F290(Scene *t) {
    VObject *snd;

    AT(t, 0x1C, s32) = (AT(t, 0x1C, s32) + 5000) % 360000;
    if (!AT(t, 0x74940, u8)) {
        return;
    }
    func_0012D7F0(t);
    func_0037E950(t, 1.0f);
    func_0037E6A0(t, 1.0f, 1.0f);
    func_0037EB80(t, 1.0f);
    func_0037EBC0(t, 0.5f * (1.0f + func_0031C248(0x1.921fb6p+1f * (180.0f - (f32)AT(t, 0x1C, s32) / 1000.0f) / 180.0f)));
    if ((D_0047E36C & MENU_CONFIRM) || (D_0047E37C & PAD_START)) {
        ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012EE30);
        AT(t, 0x18, s32) = 0;
        snd = D_0044E560;
        Sound_Play(snd, 2, 7);
        if ((s8)VCALL(snd, 0x6C, s32 (*)(VObject *))(snd) != 0) {
            Sound_Play(snd, 3, 7);
        }
        return;
    }
    if (++AT(t, 0x18, s32) < 0x385) {
        return;
    }
    AT(t, 0x20, u8) = AT(t, 0x20, u8) ? 0 : 1;
    ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012E450);
}

void func_0012D530(Scene *t, f32 alpha, f32 scroll);
void func_0012CCF0(Scene *t, f32 alpha, f32 open);
void func_0012E8C0(Scene *t);

/* the main menu's entries (texture 1, CLUT 1): New Game, Load Game, Options and, once the game
 * has been cleared (+0x11DA94), two more; with `stagger` the k-th at alpha - k / 8 */
static inline void menu_entries(Scene *t, f32 alpha, s32 stagger, s32 all) {
    func_0012D140(t, 0, 0x00, 0xC0, 0x20, 0x20, 0x40, 1, 0, alpha);
    func_0012D140(t, 0, 0x20, 0xC0, 0x20, 0x20, 0x70, 1, 0, stagger ? -0.125f + alpha : alpha);
    func_0012D140(t, 0, 0x40, 0xC0, 0x20, 0x20, 0xA0, 1, 0, stagger ? -0.25f + alpha : alpha);
    if (!all) {
        return;
    }
    func_0012D140(t, 0, 0x60, 0xC0, 0x20, 0x20, 0xD0, 1, 0, stagger ? -0.375f + alpha : alpha);
    func_0012D140(t, 0, 0x80, 0xC0, 0x20, 0x20, 0x100, 1, 0, stagger ? -0.5f + alpha : alpha);
}

/* the title turning into the main menu: (1) a 30-frame fade of the title layer, (2) the menu
 * coming in, (3) then the menu (func_0012E8C0) */
void func_0012EE30(Scene *t) {
    f32 f;

    if (!AT(t, 0x74940, u8)) {
        return;
    }
    func_0012D7F0(t);
    switch (AT(t, 0x18, s32)) {
    case 0:
        AT(t, 0x1C, s32) = 0;
        AT(t, 0x18, s32)++;
        /* fall through */
    case 1:
        func_0037E950(t, 1.0f);
        func_0037E6A0(t, 1.0f, 1.0f);
        func_0037EB80(t, 1.0f);
        func_0037EBC0(t, 1.0f);
        f = (f32)AT(t, 0x1C, s32)++ / 30.0f;
        if (AT(t, 0x1C, s32) >= 0x1F) {
            AT(t, 0x1C, s32) = 0;
            f = 1.0f;
            AT(t, 0x18, s32)++;
        }
        func_0012D530(t, f, 1.0f);
        break;
    case 2:
        f = (f32)AT(t, 0x1C, s32)++ / 30.0f;
        if (AT(t, 0x1C, s32) >= 0x1F) {
            AT(t, 0x1C, s32) = 0;
            f = 1.0f;
            AT(t, 0x18, s32)++;
        }
        func_0012D530(t, 1.0f, 1.0f - f);
        func_0012CCF0(t, f, f);
        menu_entries(t, 1.5f * f, 1, AT(t, 0x11DA94, u8));
        break;
    case 3:
        func_0012D530(t, 1.0f, 0.0f);
        func_0012CCF0(t, 1.0f, 1.0f);
        menu_entries(t, 1.0f, 0, AT(t, 0x11DA94, u8));
        AT(t, 0x1C, s32) = 90000;
        ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012E8C0);
        break;
    }
}

/* The title background (TITLE_BACK.BIN as uploaded by the renderer's +0x4C: 4-bit 640 x 448
 * at block 0x3400, its CLUT from the renderer's VRAM entry), `alpha` 0..1, scrolled left by
 * scroll x 128 pixels. */
void func_0012D530(Scene *t, f32 alpha, f32 scroll) {
    VObject *r = D_0044E4F0;
    u64 *p = VCALL(r, 0x10, u64 *(*)(VObject *, s32, s32))(r, 0xC, 0x30);
    u64 tex0;
    s32 s;

    if (p == NULL) {
        return;
    }
    p[0] = 0x1000000B;
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x5000000B;
    p[2] = 0x8003 | (1ULL << 60);
    p[3] = 0xE;
    p[4] = (1ULL << 32) | 0x310000A0;   /* ZBUF_1: masked */
    p[5] = 0x4E;
    p[6] = 0x44;
    p[7] = 0x42;
    p[8] = 0x156;
    p[9] = 0;
    p[10] = 0x8001 | (0x84ULL << 56);   /* reglist: CLAMP TEX0 RGBAQ UV XYZ2 UV XYZ2 NOP */
    p[11] = 0xF5353168;
    p[12] = (0x6FCULL << 32) | 0x9FC00A;
    tex0 = VCALL(D_0044E9A0, 0x38, u64 (*)(VObject *, s32))(
        D_0044E9A0, VCALL(r, 0x38, s32 (*)(VObject *))(r));
    /* its CLUT; the texture: PSMT4 1024 x 512 at 0x3400, 640 wide, TCC */
    p[13] = (tex0 & 0xFFFFFFE000000000ULL) | (6ULL << 32) | 0x6942B400;
    s = (u8)(u32)(128.0f * scroll);
    p[14] = ((u64)(u8)(u32)(128.0f * alpha) << 24) | 0x808080;
    p[15] = (u64)(s64)((s << 4) + 8) | 0x80000;
    p[16] = 0xFFFFFFFF72007000ULL;
    p[17] = (u64)(s64)(((s + 0x200) << 4) + 8) | 0x1C080000;
    p[18] = 0xFFFFFFFF8E009000ULL;
    p[19] = 0;
    title_sprite_end(p);
}

/* The menu's panel: two bands (texture 1, CLUT 1) shaded left to right, opening with `open`
 * 0..1 (the left side first, then the right), `alpha` 0..1. */
void func_0012CCF0(Scene *t, f32 alpha, f32 open) {
    u64 *p = VCALL(D_0044E4F0, 0x10, u64 *(*)(VObject *, s32, s32))(D_0044E4F0, 0x1C, 0x30);
    u64 left, right;
    u8 *tex;
    f32 f;

    if (p == NULL) {
        return;
    }
    p[0] = 0x1000001B;
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x5000001B;
    p[2] = 0x801A | (1ULL << 60);   /* GIF tag: 26 A+D, EOP */
    p[3] = 0xE;
    p[4] = (1ULL << 32) | 0x310000A0;
    p[5] = 0x4E;
    p[6] = 0x15C;                   /* PRIM: triangle strip, Gouraud, textured, blended, UV */
    p[7] = 0;
    tex = TITLE_TEX(1);
    p[8] = VCALL(D_0044E9A0, 0x2C, u64 (*)(VObject *, s32, s32, s32, s32))(
        D_0044E9A0, TITLE_TEX_SLOT(1), AT(tex, 4, u16), AT(tex, 6, u16), tex[1]);
    p[9] = 0x06;
    tex = TITLE_TEX(1);
    p[10] = VCALL(D_0044E9A0, 0x34, u64 (*)(VObject *, s32, s32, s32, s32))(
        D_0044E9A0, TITLE_TEX_SLOT(1), 1, tex[0], tex[1]);
    p[11] = 0x16;
    p[12] = 0x44;
    p[13] = 0x42;
    f = 2.0f * open;
    p[14] = (0x3FCULL << 32) | 0xE07FC00A;
    p[15] = 0x08;                   /* CLAMP_1 */
    if (!(f <= 1.0f)) {
        f = 1.0f;
    }
    left = ((u64)(u8)(u32)(128.0f * f * alpha) << 24) | 0x808080;
    f = 2.0f * (open - 0.5f);
    if (f <= 1.0f) {
        if (f < 0.0f) {
            f = 0.0f;
        }
    } else {
        f = 1.0f;
    }
    /* RGBAQ 1, UV 3, XYZ3 0xD (no kick), XYZ2 5 */
    p[16] = left;
    p[17] = 1;
    p[18] = 0x0E080008;
    p[19] = 3;
    p[20] = 0xFFFFFFFF73007000ULL;
    p[21] = 0xD;
    p[22] = 0x10080008;
    p[23] = 3;
    p[24] = 0xFFFFFFFF75007000ULL;
    p[25] = 0xD;
    right = ((u64)(u8)(u32)(128.0f * f * alpha) << 24) | 0x808080;
    p[26] = right;
    p[27] = 1;
    p[28] = 0x0E082008;
    p[29] = 3;
    p[30] = 0xFFFFFFFF73009000ULL;
    p[31] = 5;
    p[32] = 0x10082008;
    p[33] = 3;
    p[34] = 0xFFFFFFFF75009000ULL;
    p[35] = 5;
    p[36] = 0x0E080008;
    p[37] = 3;
    p[38] = 0xFFFFFFFF8B007000ULL;
    p[39] = 0xD;
    p[40] = 0x10080008;
    p[41] = 3;
    p[42] = 0xFFFFFFFF8D007000ULL;
    p[43] = 0xD;
    p[44] = left;
    p[45] = 1;
    p[46] = 0x0E082008;
    p[47] = 3;
    p[48] = 0xFFFFFFFF8B009000ULL;
    p[49] = 5;
    p[50] = 0x10082008;
    p[51] = 3;
    p[52] = 0xFFFFFFFF8D009000ULL;
    p[53] = 5;
    p[54] = 0x310000A0;
    p[55] = 0x4E;
}

void func_0012DDB0(Scene *t, f32 alpha);
void func_0012E270(Scene *t);
void func_0012E780(Scene *t);
void func_0012E6C0(Scene *t);

#define BGM_WANT(track, pause, restart, level) \
    VCALL(D_0044E970, 0x8, void (*)(void *, s32, s32, s32, f32))(D_0044E970, track, pause, restart, level)


/* the main menu: up / down choose (wrapping), cancel goes back to the title, confirm or Start
 * takes the entry; 900 idle frames: the attract movie */
void func_0012E8C0(Scene *t) {
    s8 old;
    s8 n;

    BGM_WANT(0x33, 0, 0, 1.0f);
    func_0012DDB0(t, 1.0f);
    old = MENU_CURSOR;
    n = AT(t, 0x11DA94, u8) ? 5 : 3;
    if (D_0047E36C & MENU_UP) {
        MENU_CURSOR = old - 1;
        if (MENU_CURSOR < 0) {
            MENU_CURSOR = n - 1;
        }
    } else if (D_0047E36C & MENU_DOWN) {
        MENU_CURSOR++;
        if (n - 1 < MENU_CURSOR) {
            MENU_CURSOR = 0;
        }
    }
    if (old != MENU_CURSOR) {
        AT(t, 0x18, s32) = 0;
        Sound_PlaySE(SE_CURSOR);
        AT(t, 0x1C, s32) = 90000;
    }
    if (D_0047E36C & MENU_CANCEL) {
        Sound_PlaySE(SE_CANCEL);
        AT(t, 0x18, s32) = 0;
        BGM_WANT(0xFF, 0, 0, 1.0f);
        ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012E270);
        return;
    }
    if (old == MENU_CURSOR && ((D_0047E36C & MENU_CONFIRM) || (D_0047E37C & PAD_START))) {
        BGM_WANT(0xFF, 0, 0, 1.0f);
        switch (MENU_CURSOR) {
        case 0:
            Sound_PlaySE(SE_DECIDE);
            TITLE_NEXT = 2;
            break;
        case 1:
            Sound_PlaySE(SE_DECIDE);
            AT(t, TITLE_WORK + 4, u8) = 6;   /* the sub screen: load a game */
            ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012E780);
            return;
        case 2:
            AT(t, TITLE_WORK + 4, u8) = 5;   /* the sub screen: options */
            ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012E6C0);
            return;
        case 3:
            Sound_PlaySE(SE_DECIDE);
            TITLE_NEXT = 5;
            break;
        case 4:
            Sound_PlaySE(SE_DECIDE);
            TITLE_NEXT = 6;
            break;
        default: {
            VObject *snd = D_0044E560;

            TITLE_NEXT = 2;
            Sound_Play(snd, 0, 6);
            if ((s8)VCALL(snd, 0x6C, s32 (*)(VObject *))(snd) != 0) {
                Sound_Play(snd, 1, 6);
            }
            break;
        }
        }
        ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012E450);
        return;
    }
    if (++AT(t, 0x18, s32) < 0x385) {
        return;
    }
    BGM_WANT(0xFF, 0, 0, 1.0f);
    TITLE_NEXT = TITLE_NEXT ? 0 : 1;
    ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012E450);
}

/* the highlighted entry k: drawn again in CLUT 0 (`a`, fixed alpha), its description shown */
static inline void menu_highlight(Scene *t, s32 k, f32 a) {
    func_0012D140(t, 0, k * 0x20, 0xC0, 0x20, 0x20, 0x40 + k * 0x30, 0, 1, a);
    func_00384A90((Task *)((u8 *)t + TITLE_TASK), 2 + k);
}

/* draw the main menu at `alpha`: the panel and entries, the chosen one pulsing with its
 * description below */
void func_0012DDB0(Scene *t, f32 alpha) {
    f32 pulse;
    u8 bit;

    D_0047B350 = 2;
    AT(t, 0x1C, s32) = (AT(t, 0x1C, s32) + 5000) % 360000;
    if (!AT(t, 0x74940, u8)) {
        return;
    }
    func_0012D7F0(t);
    pulse = 0.5f * (1.0f + func_0031C248(0x1.921fb6p+1f * (180.0f - (f32)AT(t, 0x1C, s32) / 1000.0f) / 180.0f));
    bit = 1 << MENU_CURSOR;
    func_0012D530(t, alpha, 0.0f);
    func_0012CCF0(t, alpha, 1.0f);
    func_0012D140(t, 0, 0x00, 0xC0, 0x20, 0x20, 0x40, 1, 0, alpha);
    func_0012D140(t, 0, 0x20, 0xC0, 0x20, 0x20, 0x70, 1, 0, alpha);
    func_0012D140(t, 0, 0x40, 0xC0, 0x20, 0x20, 0xA0, 1, 0, alpha);
    if (AT(t, 0x11DA94, u8)) {
        func_0012D140(t, 0, 0x60, 0xC0, 0x20, 0x20, 0xD0, 1, 0, alpha);
        func_0012D140(t, 0, 0x80, 0xC0, 0x20, 0x20, 0x100, 1, 0, alpha);
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
    if (AT(t, 0x11DA94, u8)) {
        if (bit & 8) {
            menu_highlight(t, 3, pulse * alpha);
        }
        if (bit & 0x10) {
            menu_highlight(t, 4, pulse * alpha);
        }
    }
    func_00384BC0((Task *)((u8 *)t + TITLE_TASK));
}

extern s32 func_002D20D0(void *bgm);
void func_0012FF60(Scene *t);
void func_0012CB30(Scene *t);

/* leave the title once the music has stopped: release the title textures and sound bank, then
 * what the title leads to (TITLE_NEXT): 0 the attract movie, 1 the opening, 2 / 5 a new game,
 * 3 / 6 straight into game mode 3 (stage -1 / 0x37), 4 nothing */
void func_0012E450(Scene *t) {
    VObject *snd;

    if (!(u8)func_002D20D0(D_0044E980)) {
        return;
    }
    if (AT(t, 0x74940, u8)) {
        VObject *msg = gBootMessage;

        VCALL(msg, 0x14, void (*)(VObject *, s32))(msg, 6);
        VCALL(msg, 0xC, void (*)(VObject *, s32))(msg, 6);
        AT(t, 0x74940, u8) = 0;
    }
    snd = D_0044E560;
    VCALL(snd, 0x10, void (*)(VObject *, s32, s32))(snd, 0, 0xF000);
    VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 7);
    switch (TITLE_NEXT) {
    case 0:
        ptmf_set_fn(&t->state, func_00130520);
        break;
    case 1:
        ptmf_set_fn(&t->state, func_0012FF60);
        break;
    case 2:
        ptmf_set_fn(&t->state, func_0012CB30);
        break;
    case 3:
        AT(D_0044E978, 0x4, s32) = 3;
        AT(D_0044E978, 0x10, s32) = -1;
        VCALL(t, 0x14, void (*)(Scene *))(t);
        break;
    case 4:
        break;
    case 5:
        ptmf_set_fn(&t->state, func_0012CB30);
        break;
    case 6:
        AT(D_0044E978, 0x4, s32) = 3;
        AT(D_0044E978, 0x10, s32) = 0x37;
        VCALL(t, 0x14, void (*)(Scene *))(t);
        break;
    }
}

extern void func_0026BC00(void *msg);

/* +0x14 finish: end the movie scene, drop the sub screen's textures (group 0x19), reset the
 * message object, ask to be finished */
void func_0012E1B0(Scene *t) {
    Scene *s = SCENE_TABLE_SCENE(1);

    if (s != NULL) {
        ptmf_set(&s->state, &sSceneFinish);
        VCALL(SCENE_TABLE_SCENE(1), 0x14, void (*)(Scene *))(SCENE_TABLE_SCENE(1));
    }
    VCALL(D_0044E4E8, 0x14, void (*)(VObject *, s32))(D_0044E4E8, 0x19);
    func_0026BC00((u8 *)t + 0x24);
    t->request = SCENE_REQ_FINISH;
}

extern void func_00100490(void *p);   /* operator delete */
extern void func_0011F9A0(void *p);   /* operator delete (scene heap) */
extern void func_001002C0(void *array, void *(*dtor)(void *, s32), u32 size, u32 n);   /* __destroy_arr */
extern void func_002E31D0(void *bgmctl);
extern void func_002D2330(void *bgm);
extern void *func_0012C730(void *card, s32 flags);   /* BootCard destructor */
extern void *D_0046A100[], *D_0046A0D0[];

/* the text object: release (its loads, its textures: group 0x28) */
void func_00304EF0(u8 *o) {
    VCALL(gFileLoader, 0x14, void (*)(VObject *, u32))(gFileLoader, 0x6000000);
    VCALL(D_0044E4E8, 0x14, void (*)(VObject *, s32))(D_0044E4E8, 0x28);
    AT(o, 0x108, s32) = -1;
    AT(o, 0x10C, u8) = 0xFF;
    AT(o, 0x10E, u8) = 0xFF;
}

/* the text object: destructor */
void *func_0012C6B0(u8 *o, s32 flags) {
    if (o != NULL) {
        Task *t;

        AT(o, 0x0, void **) = D_0046A068;
        func_00304EF0(o);
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

/* the title work's base: destructor (the pool and its entries) */
void *func_0012C420(u8 *w, s32 flags) {
    if (w != NULL) {
        u8 *pool = w + 8;

        AT(w, 0x0, void **) = D_0046A090;
        if (pool != NULL) {
            AT(pool, 0x0, void **) = D_0046A078;
            if (pool + 0x1208 != NULL) {
                AT(pool, 0x1208, void **) = D_004699C0;
                if (pool + 0x1208 != NULL) {
                    AT(pool, 0x1208, void **) = D_004699E0;
                }
            }
            func_001002C0(pool + 8, func_0012C500, 0x18, 0xC0);
            if (pool != NULL) {
                D_0044E990 = NULL;
            }
        }
        if (w != NULL) {
            D_0044E988 = NULL;
        }
        if ((s16)flags > 0) {
            func_00100490(w);
        }
    }
    return w;
}

/* destructor */
Scene *func_0012C220(Scene *t, s32 flags) {
    if (t != NULL) {
        u8 *w;
        Task *task;

        t->vtbl = D_0046A040;
        func_002E31D0((u8 *)t + TITLE_UNK140CA4);
        func_002D2330(D_0044E980);
        if ((u8 *)t + TITLE_UNK140CA4 != NULL) {
            AT(t, TITLE_UNK140CA4, void **) = D_0046A110;
            if ((u8 *)t + TITLE_UNK140CA4 != NULL) {
                AT(t, TITLE_UNK140CA4, void **) = D_0046A100;
                if ((u8 *)t + TITLE_UNK140CA4 != NULL) {
                    D_0044E970 = NULL;
                }
            }
        }
        w = (u8 *)t + TITLE_WORK;
        if (w != NULL) {
            AT(w, 0x0, void **) = D_0047A790;
            func_0012C730(w + 0xA8AC0, -1);
            func_0012C6B0(w + 0x97980, -1);
            Task_dtor((Task *)(w + 0x97868), -1);
            Task_dtor((Task *)(w + 0x97764), -1);
            func_0012C420(w, 0);
        }
        task = (Task *)((u8 *)t + TITLE_TASK);
        if (task != NULL && task->child != NULL) {
            Task_dtor(task->child, 1);
            task->child = NULL;
        }
        if ((u8 *)t + 0x24 != NULL) {
            AT(t, 0x24, void **) = D_0046D7D0;
            if ((u8 *)t + 0x24 != NULL) {
                AT(t, 0x24, void **) = D_0046A0D0;
                if ((u8 *)t + 0x24 != NULL) {
                    gBootMessage = NULL;
                }
            }
        }
        if ((u8 *)t + 0x14 != NULL) {
            D_0044E968 = NULL;
        }
        if (t != NULL) {
            t->vtbl = Scene_vtable;
        }
        if ((s16)flags > 0) {
            func_0011F9A0(t);
        }
    }
    return t;
}

extern const char *D_003B0050[7];   /* "SYSTEM\\PLAY_DEMO_1.SFD" .. _7 */
void func_0012FEA0(Scene *t);
void func_0012FBB0(Scene *t);

/* state: the next of the seven gameplay demos */
void func_0012FF60(Scene *t) {
    title_movie_start(t, D_003B0050[AT(t, 0x140CD0, u8)]);
    AT(t, 0x140CD0, u8) = (u8)(AT(t, 0x140CD0, u8) + 1) % 7;
    ptmf_set_fn(&t->state, func_0012FEA0);
}

/* state: the demo */
void func_0012FEA0(Scene *t) {
    title_movie_wait(t, func_0012FBB0);
}

/* state: fade the demo out, then the title (as after the attract movie) */
void func_0012FBB0(Scene *t) {
    if (!title_movie_fade(t)) {
        ptmf_set_fn(&t->state, func_0012FB50);
        ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012F720);
    }
}

extern s32 func_003999C0(void *sub);

#define SUB_OPEN    AT(t, TITLE_WORK + 0xA8DE5, u8)   /* the sub screen is open */
#define SUB_LOADED  AT(t, TITLE_WORK + 0xA8AC4, s32)  /* the load screen's result, -2 a game loaded */

/* state: the options screen (over the menu); back to the menu when it closes */
void func_0012E6C0(Scene *t) {
    if (AT(t, TITLE_WORK + 0x16F8, u8)) {
        func_0012DDB0(t, 1.0f);
    }
    func_003999C0((u8 *)t + TITLE_WORK);
    if (!SUB_OPEN) {
        ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012E8C0);
        AT(t, 0x18, s32) = 0;
    }
}

/* state: the load screen; when it closes, leave the title with the loaded game (next 3) or go
 * back to the menu */
void func_0012E780(Scene *t) {
    if (AT(t, TITLE_WORK + 0x16F8, u8) && SUB_LOADED != -2) {
        func_0012DDB0(t, 1.0f);
    }
    func_003999C0((u8 *)t + TITLE_WORK);
    if (SUB_OPEN) {
        return;
    }
    if (SUB_LOADED == -2) {
        TITLE_NEXT = 3;
        ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012E450);
    } else {
        ptmf_set_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012E8C0);
        AT(t, 0x18, s32) = 0;
    }
}
