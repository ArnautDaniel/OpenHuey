/* Mode 2 scene: the title screen and its menus (new game, load, options, extras), the opening
 * movie. A large object (0x140CE0 bytes from the scene heap). */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "task.h"

#define AT(p, off, type) (*(type *)((u8 *)(p) + (off)))

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

static inline void set_state(PTMF *dst, const PTMF *state) {
    PTMF s = *state;

    if (ptmf_test(&s)) {
        *dst = s;
    }
}

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
    set_state(&t->state, &sSceneEntryState);
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

static inline void set_state_fn(PTMF *dst, void *fn) {
    PTMF s = {0, -1, {fn}};

    if (ptmf_test(&s)) {
        *dst = s;
    }
}

/* state: once the textures are loaded, either back to the menu (+0x14 = 2) or the title
 * (func_0012FB50, with the +0x140CA4 object running func_0012F720) */
void func_001306E0(Scene *t) {
    if (VCALL(gFileLoader, 0x24, s32 (*)(VObject *))(gFileLoader) == 2) {
        return;
    }
    if (AT(t, 0x14, s32) == 2) {
        AT(t, 0x14, s32) = 0;
        set_state_fn(&t->state, func_00130520);
        return;
    }
    set_state_fn(&t->state, func_0012FB50);
    set_state_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012F720);
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

/* state: start the attract movie (as scene 1) at the title's movie volume */
void func_00130520(Scene *t) {
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

        func_002B6D10(m, D_0044E940, 1, 0);
        *v = AT(t, 0x140CCC, f32);
        if (*v < 0.0f) {
            *v = 0.0f;
        }
        if (!(*v <= 1.0f)) {
            *v = 1.0f;
        }
        func_002B6340(m);
    }
    set_state_fn(&t->state, func_00130460);
}

extern u32 D_0047E37C;      /* pad buttons pressed */
void func_00130170(Scene *t);

/* state: the attract movie; Start (once it shows) skips it */
void func_00130460(Scene *t) {
    if ((D_0047E37C & 8) && D_0044E958 != NULL && AT(D_0044E958, 0x1B4, u8)) {
        AT(t, 0x22, u8) = 1;
    }
    if (D_0044E958 != NULL && !AT(t, 0x22, u8)) {
        return;
    }
    set_state_fn(&t->state, func_00130170);
}

extern VObject *D_0044E4F0;      /* the renderer */
static const PTMF sSceneFinish = {0, 0x14, {(void *)0}};   /* virtual +0x14 */

/* the renderer's +0x7C: a rectangle (x, y, w, h; colour; layer ...) */
typedef void (*RectFn)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, u32, s32, s32, s32, s32);

/* state: fade the attract movie out (sound and picture, 1/30 a frame), then the title */
void func_00130170(Scene *t) {
    u8 *m = D_0044E958;
    f32 *level = &AT(t, 0x140CCC, f32);
    f32 l;

    if (m == NULL) {
        set_state_fn(&t->state, func_0012FB50);
        set_state_fn(&AT(t, TITLE_UNK140CA4 + 0x1C, PTMF), func_0012F720);
        return;
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
            set_state(&s->state, &sSceneFinish);
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
}
