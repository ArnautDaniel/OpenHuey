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
