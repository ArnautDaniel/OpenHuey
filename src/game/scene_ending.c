/* Mode 5 scene: the ending. The staff roll (STAFF_ROLL.SFD) as scene 1, then the results
 * screen: the ending's picture (SYSTEM\ENDING_A..D.TEX), the play record (MSG_END) with a
 * dog level and a type, the extras it unlocks, and the clear-data save (the sub screen's
 * mode 9). Built by Scene5_ctor (scene_title.c), 0x117540 bytes, vtable D_0047A330. */
#include <stdarg.h>
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "task.h"
#include "subscreen.h"
#include "texcache.h"
#include "input.h"
#include "sound.h"
#include "globals.h"
#include "bgm.h"
#include "bootcard.h"
#include "heap.h"
#include "message.h"
#include "movie.h"
#include "scene_title.h"
#include "libc.h"
#include "msl.h"

typedef struct SceneEnding {
    /* 0x00 */ Scene base;
    /* 0x14 */ s32 step;        /* within the running sequence */
    /* 0x18 */ s32 timer;
    /* 0x1C */ u8 picAlpha;     /* the ending's picture, 0..0x80 */
    /* 0x1D */ u8 textAlpha;    /* the play record */
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s32 news;        /* 1 the first clear, 2 a new extra (then message 0x13's width) */
    /* 0x24 */ s32 newsX;
    /* 0x28 */ u8 pad28[0x18];
    /* 0x40 */ s32 title;       /* 0..10, the TYPE earned (messages 0x12 / 8..0x11) */
    /* 0x44 */ s32 dogLevel;    /* 0..4: A..E */
    /* 0x48 */ Task task;
} SceneEnding;

_Static_assert(__builtin_offsetof(SceneEnding, task) == 0x48, "SceneEnding.task");

#define END_SUB(s)      ((SubScreen *)((u8 *)(s) + 0x180))
#define END_MSG(s)      ((u8 *)(s) + 0xA9180)       /* the message object (gBootMessage) */
#define END_PIC_FILE(s) ((u8 *)(s) + 0xA92C0)       /* ENDING_x.TEX */
#define END_TEXT(s)     ((u8 *)(s) + 0xEA300)       /* MSG_END.BIN */
#define END_FONT(s)     ((u8 *)(s) + 0xEB300)       /* MSG_END.TEX */
#define END_PIC_SLOT(s) AT(s, 0xEA2C0, s32)         /* the picture's VRAM slot */
#define END_PIC_TEX(s)  AT(s, 0xEA2C4, u8 *)        /* ... and .TEX entry */
#define END_BGM(s)      ((u8 *)(s) + 0x1174E4)
#define END_SEQ(s)      (&AT(s, 0x117500, PTMF))    /* the results' own state */
#define END_VOLUME(s)   AT(s, 0x11750C, f32)        /* the staff roll's volume */

extern void *Scene_vtable[];
extern void *D_0047A330[];          /* SceneEnding */
extern void *D_0047A790[];          /* the sub screen */
extern void *D_0046D7D0[], *D_0046A0D0[];   /* the message object, its base */
extern void *D_0046A110[], *D_0046A100[];   /* the BGM controller, its base */
extern void *D_0046ECC0[];          /* SceneMovie */
extern u8 D_0047B350;               /* the message language set */
extern u8 *D_01991EC4;              /* the message text */
extern const char D_004638F8[];     /* "STAFF_ROLL.SFD" */
extern const char D_00463840[], D_00463860[], D_00463880[], D_004638A0[];   /* "SYSTEM\\ENDING_A..D.TEX" */
extern const char D_004638C0[];     /* "SUBSCR\\MSG_END.BIN" */
extern const char D_004638E0[];     /* "SUBSCR\\MSG_END.TEX" */
extern const char D_00463760[];     /* "%02d:%02d:%02d" */
extern const char D_00463770[];     /* "%s" */
extern const char D_00463778[];     /* "%c" */
extern const char D_00463780[];     /* "%d" */
extern const char D_00463788[];     /* "%s%%" */

static const PTMF sSceneFinish = {0, 0x14, {(void *)0}};   /* virtual +0x14 */

/* Task_MessageWidth's u16, masked as the original does (the empty asm keeps the mask: a
 * difftest stub returns all 32 bits) */
static inline u32 msg_width(Task *t, s32 id) {
    u32 w = Task_MessageWidth(t, id, 0x10);

    __asm__("" : "+r"(w));
    return w & 0xFFFF;
}

#define SCENE_TABLE_SCENE(i) (*(Scene **)((u8 *)gSceneTable + 4 + (i) * 4))
#define LOADER_LOAD(name, dst) VCALL(gFileLoader, 0x34, void (*)(VObject *, const char *, void *))(gFileLoader, name, dst)

void func_00372050(SceneEnding *s, u8 alpha);
void func_00372330(SceneEnding *s);
void func_003723A0(SceneEnding *s);
void func_00372690(SceneEnding *s);
void func_00372A90(SceneEnding *s);
void func_003738C0(SceneEnding *s);
void func_00373E50(SceneEnding *s);
void func_00373EB0(SceneEnding *s);
void func_003741A0(SceneEnding *s);
void func_00374260(SceneEnding *s);

/* destructor */
SceneEnding *func_00371E10(SceneEnding *s, s32 flags) {
    if (s != NULL) {
        u8 *bgm = END_BGM(s);
        u8 *msg = END_MSG(s);
        SubScreen *w = END_SUB(s);
        Task *task = &s->task;

        s->base.vtbl = D_0047A330;
        func_002E31D0((BgmCtl *)bgm);
        func_002D2330(gAdx);
        if (bgm != NULL) {
            AT(bgm, 0, void **) = D_0046A110;
            if (bgm != NULL) {
                AT(bgm, 0, void **) = D_0046A100;
                if (bgm != NULL) {
                    gMusic = NULL;
                }
            }
        }
        if (msg != NULL) {
            AT(msg, 0, void **) = D_0046D7D0;
            if (msg != NULL) {
                AT(msg, 0, void **) = D_0046A0D0;
                if (msg != NULL) {
                    gBootMessage = NULL;
                }
            }
        }
        if (w != NULL) {
            w->vtbl = D_0047A790;
            BootCard_dtor(&w->card, -1);
            TextObj_dtor(w->textObj, -1);
            Task_dtor(&w->text, -1);
            Task_dtor(&w->ask, -1);
            SubScreenBase_dtor(w, 0);
        }
        if (task != NULL && task->child != NULL) {
            Task_dtor(task->child, 1);
            task->child = NULL;
        }
        if (s != NULL) {
            s->base.vtbl = Scene_vtable;
        }
        if ((s16)flags > 0) {
            func_0011F9A0(s);
        }
    }
    return s;
}

/* format into `buf` (9 bytes) */
void func_00371FE0(SceneEnding *s, char *buf, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    func_0026ED98(buf, 9, fmt, ap);
    va_end(ap);
}

#ifdef HG_NATIVE
#include "gl2d.h"

/* the ending's picture over the whole screen (512 x 448 texels), at `alpha` 0..0x80 */
void func_00372050(SceneEnding *s, u8 alpha) {
    gl2d_sprite(0x30, 0, 0, 512, 448, END_PIC_TEX(s), 0.5f, 0.5f, 512.5f, 448.5f, ((u32)alpha << 24) | 0x808080, 0,
                0x40);
}
#endif

/* +0x14 finish: end the movie scene, drop the message set 6 and the textures (group 0x19),
 * reset the message object, ask to be finished */
void func_00372230(SceneEnding *s) {
    Scene *m = SCENE_TABLE_SCENE(1);
    VObject *msg;

    if (m != NULL) {
        ptmf_set(&m->state, &sSceneFinish);
        VCALL(SCENE_TABLE_SCENE(1), 0x14, void (*)(Scene *))(SCENE_TABLE_SCENE(1));
    }
    msg = gBootMessage;
    VCALL(msg, 0x14, void (*)(VObject *, s32))(msg, 6);
    VCALL(msg, 0xC, void (*)(VObject *, s32))(msg, 6);
    VCALL(gTexCache, 0x14, void (*)(VObject *, s32))(gTexCache, 0x19);
    func_0026BC00((VObject *)END_MSG(s));
    s->base.request = SCENE_REQ_FINISH;
}

/* the last state: the clear-data save (the sub screen); once it's closed, back to the title
 * (mode 2) */
void func_00372330(SceneEnding *s) {
    SubScreen_Update(END_SUB(s));
    if (END_SUB(s)->open) {
        return;
    }
    AT(gSystemData, 0x4, s32) = 2;
    AT(gSystemData, 0x10, s32) = 0;
    VCALL(s, 0x14, void (*)(SceneEnding *))(s);
}

/* each frame of the results: the texture cache and message object's upkeep, the picture (set
 * 6, texture 0) sent to VRAM once it's loaded and drawn while it shows */
static inline void ending_picture(SceneEnding *s) {
    VObject *msg;

    VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
    msg = gBootMessage;
    VCALL(msg, 0x20, void (*)(VObject *))(msg);
    END_PIC_SLOT(s) = VCALL(msg, 0x24, s32 (*)(VObject *, s32, s32))(msg, 6, 0);
    if (END_PIC_SLOT(s) != -1) {
        END_PIC_TEX(s) = VCALL(msg, 0x28, u8 *(*)(VObject *, s32, s32))(msg, 6, 0);
        if (END_PIC_SLOT(s) & 0x80000000) {
            END_PIC_SLOT(s) &= 0x7FFFFFFF;
            VCALL(gRenderer, 0x44, s32 (*)(VObject *, s32, void *, s32))(gRenderer, END_PIC_SLOT(s),
                                                                          END_PIC_TEX(s), 0);
        }
    }
    if (s->picAlpha) {
        func_00372050(s, s->picAlpha);
    }
}

/* fade the picture out, then the clear-data save (sub screen mode 9) */
static inline void ending_fade_to_save(SceneEnding *s) {
    if (s->picAlpha) {
        s->picAlpha -= 2;
        return;
    }
    VCALL(gMusic, 0x8, void (*)(VObject *, s32, s32, s32, f32))(gMusic, 0xFF, 0, 0, 1.0f);
    END_SUB(s)->mode = SUB_MODE_SAVE_9;
    Task_Close(&s->task);
    ptmf_set_fn(END_SEQ(s), func_00372330);
}

/* titles 3..6 have a message of their own (func_003723A0): its width (message 0x13) kept */
static inline s32 ending_title_message(SceneEnding *s) {
    switch (s->title) {
    case 3:
    case 4:
    case 5:
    case 6:
        s->step = 0;
        s->textAlpha = 0;
        s->news = -(s32)msg_width(&s->task, 0x13);
        s->newsX = (u32)(s->news + 0x200) >> 1;
        Task_Close(&s->task);
        ptmf_set_fn(END_SEQ(s), func_003723A0);
        return 1;
    }
    return 0;
}

/* results: the picture fades in, the title's message (0x14..0x17 for titles 3..6), out */
void func_003723A0(SceneEnding *s) {
    s32 id = 0;

    ending_picture(s);
    Task_Run(&s->task);
    switch (s->step) {
    case 0:
        if (s->picAlpha == 0x80) {
            s->step++;
        } else {
            s->picAlpha += 2;
        }
        break;
    case 1:
        switch (s->title) {
        case 3:
            id = 0x14;
            break;
        case 4:
            id = 0x15;
            break;
        case 5:
            id = 0x16;
            break;
        case 6:
            id = 0x17;
            break;
        }
        Task_OpenAt(&s->task, id, 1);
        s->step++;
        break;
    case 2:
        if (s->task.mode) {
            return;
        }
        s->step++;
        break;
    case 3:
        ending_fade_to_save(s);
        break;
    }
}

/* results: the picture fades in; what was unlocked (0x1C the first clear, 0x1D, 0x1E); then
 * the title's message or the fade */
void func_00372690(SceneEnding *s) {
    ending_picture(s);
    Task_Run(&s->task);
    switch (s->step) {
    case 0:
        if (s->picAlpha == 0x80) {
            s->step++;
        } else {
            s->picAlpha += 2;
        }
        break;
    case 1:
        if (s->news & 1) {
            Task_OpenAt(&s->task, 0x1C, 1);
            s->step++;
        } else {
            Task_OpenAt(&s->task, 0x1E, 1);
            s->step += 5;
        }
        break;
    case 2:
        if (s->task.mode) {
            return;
        }
        s->step++;
        break;
    case 3:
        Task_OpenAt(&s->task, 0x1D, 1);
        s->step++;
        break;
    case 4:
        if (s->task.mode) {
            return;
        }
        s->step++;
        break;
    case 5:
        Task_OpenAt(&s->task, 0x1E, 1);
        s->step++;
        break;
    case 6:
        if (s->task.mode) {
            return;
        }
        if (!ending_title_message(s)) {
            s->step++;
        }
        break;
    case 7:
        ending_fade_to_save(s);
        break;
    }
}

/* one value of the record, right-aligned at x 480 */
static inline void ending_value(SceneEnding *s, char *buf, s32 y, s32 color) {
    s32 x = 0x1E0 - (Text_LineWidth(&s->task, (u8 *)buf, 0x10) & 0xFFFF);

    Task_PrintfEx(&s->task, x, y, color, s->textAlpha, 0x33, D_00463770, buf);
}

/* one message of MSG_END, right-aligned at x 480 */
static inline void ending_label(SceneEnding *s, s32 id, s32 y, s32 color) {
    s32 x = 0x1E0 - msg_width(&s->task, id);

    Task_ShowText(&s->task, x, y, color, Task_MessageText(&s->task, id), s->textAlpha, 0x33, 0x10, 0x15);
}

static inline void bit_set(u32 *bits, s32 n) {
    bits[n >> 5] |= 1 << (n & 31);
}

static inline s32 bit_test(u32 *bits, s32 n) {
    return (bits[n >> 5] & (1 << (n & 31))) != 0;
}

/* the results: the picture fades in and half out, the record shows - ENDING, TIME, DOG LEVEL,
 * PANIC, ITEM, CRITICAL HEWIE INJURIES, ENEMIES DEFEATED and TYPE, the values the type came
 * from highlighted; a button, then the unlocks (the system data's +0x24 bits, func_00372690),
 * the title's message (func_003723A0) or the fade to the save */
void func_00372A90(SceneEnding *s) {
    u8 *sys = gSystemData;
    u8 *st = sys + 0x190;
    char buf[12];
    u8 a;
    s32 i, id = 0, color;

    ending_picture(s);
    a = s->textAlpha;
    if (a) {
        Task_ShowText(&s->task, 0x20, 0x60, 0, Task_MessageText(&s->task, 0), a, 0x33, 0x10, 0x15);
        for (i = 1; i < 8; i++) {
            Task_ShowText(&s->task, 0x20, 0x60 + i * 0x20, 0, Task_MessageText(&s->task, i), s->textAlpha, 0x33,
                          0x10, 0x15);
        }

        /* the ending (A..D) */
        color = 0;
        switch (AT(st, 0x111, u8)) {
        case 0:
            id = 0x18;
            break;
        case 1:
            id = 0x19;
            break;
        case 2:
            id = 0x1A;
            if (s->title == 2) {
                color = 2;
            }
            break;
        case 3:
            id = 0x1B;
            break;
        }
        ending_label(s, id, 0x60, color);

        /* the play time */
        func_00371FE0(s, buf, D_00463760, AT(st, 0x100C, u8), AT(st, 0x100D, u8), AT(st, 0x100E, u8));
        switch (s->title) {
        case 6:
        case 9:
            color = 2;
            break;
        case 10:
            color = 1;
            break;
        default:
            color = 0;
            break;
        }
        ending_value(s, buf, 0x80, color);

        /* the dog level */
        {
            s8 c = 0;

            switch (s->dogLevel) {
            case 0:
                c = 'A';
                break;
            case 1:
                c = 'B';
                break;
            case 2:
                c = 'C';
                break;
            case 3:
                c = 'D';
                break;
            case 4:
                c = 'E';
                break;
            }
            switch (s->title) {
            case 1:
                color = 1;
                break;
            case 3:
            case 7:
                color = 2;
                break;
            default:
                color = 0;
                break;
            }
            func_00371FE0(s, buf, D_00463778, c);
            ending_value(s, buf, 0xA0, color);
        }

        /* panic, items, critical Hewie injuries, enemies defeated */
        func_00371FE0(s, buf, D_00463780, AT(st, 0x100A, s16));
        ending_value(s, buf, 0xC0, s->title == 8);

        /* the percentage */
        {
            static const union { u32 u; f32 f; } k100 = {0x42C80000};   /* 100 */
            char pct[2] = "%";
            s32 x;

            func_00371FE0(s, buf, D_00463780,
                          (s32)(k100.f * (f32)AT(st, 0x1006, s16) / (f32)AT(st, 0x1008, s16)));
            x = (Text_LineWidth(&s->task, (u8 *)buf, 0x10) & 0xFFFF) +
                (Text_LineWidth(&s->task, (u8 *)pct, 0x10) & 0xFFFF);
            Task_PrintfEx(&s->task, 0x1E0 - x, 0xE0, s->title == 4 ? 2 : 0, s->textAlpha, 0x33, D_00463788, buf);
        }

        func_00371FE0(s, buf, D_00463780, AT(st, 0x1002, s16));
        ending_value(s, buf, 0x100, s->title == 3 ? 2 : 0);
        func_00371FE0(s, buf, D_00463780, AT(st, 0x1004, s16));
        ending_value(s, buf, 0x120, s->title == 5 ? 2 : 0);

        /* the type */
        switch (s->title) {
        case 0:
            id = 0x12;
            break;
        case 1:
            id = 8;
            break;
        case 2:
            id = 9;
            break;
        case 3:
            id = 0xA;
            break;
        case 4:
            id = 0xB;
            break;
        case 5:
            id = 0xC;
            break;
        case 6:
            id = 0xD;
            break;
        case 7:
            id = 0xE;
            break;
        case 8:
            id = 0xF;
            break;
        case 9:
            id = 0x10;
            break;
        case 10:
            id = 0x11;
            break;
        }
        ending_label(s, id, 0x140, 0);
    }

    switch (s->step) {
    case 0:
        if (s->picAlpha == 0x80) {
            s->step++;
            s->timer = 0;
        } else {
            s->picAlpha += 2;
        }
        break;
    case 1:
        s->timer++;
        if (s->timer < 90) {
            return;
        }
        s->step++;
        break;
    case 2:
        if (s->picAlpha == 0x40) {
            s->step++;
            s->timer = 0;
        } else {
            s->picAlpha -= 2;
        }
        break;
    case 3:
        if (s->textAlpha == 0x80) {
            s->step++;
        } else {
            s->textAlpha += 4;
        }
        break;
    case 4:
        s->timer++;
        if (s->timer < 150) {
            return;
        }
        s->step++;
        break;
    case 5:
        if ((D_0047E374 & PAD_CIRCLE) || (D_0047E374 & PAD_START)) {
            s->step++;
        }
        break;
    case 6: {
        u32 *unlock = &AT(sys, 0x24, u32);
        u32 *flags = &AT(sys, 0x2C, u32);

        if (s->textAlpha) {
            s->textAlpha -= 8;
            break;
        }
        s->news = 0;
        if (!bit_test(unlock, 0)) {
            s->news = 1;
            *unlock |= 1;
        }
        switch (AT(st, 0x111, u8)) {
        case 0:
            if (!bit_test(unlock, 1)) {
                s->news |= 2;
                *unlock |= 2;
            }
            if (AT(st, 0x78, u32) & 0x8000) {
                *flags |= 0x800000;
            }
            break;
        case 1:
            *unlock |= 4;
            if (AT(st, 0x78, u32) & 0x8000) {
                *flags |= 0x1000000;
            }
            break;
        case 2:
            if (!bit_test(unlock, 3)) {
                s->news |= 2;
                *unlock |= 8;
            }
            break;
        case 3:
            *unlock |= 0x10;
            break;
        }
        if (s->news == 0) {
            if (!(*flags & 0x400000)) {
                if (AT(st, 0x78, u32) & 0x8000) {
                    if (bit_test(unlock, 3 + 5) == 1 || bit_test(unlock, 7 + 5) == 1 || s->title == 3 ||
                        s->title == 7) {
                        s->news |= 2;
                    }
                }
            } else if (!bit_test(unlock, 3 + 5) && !bit_test(unlock, 7 + 5) && (s->title == 3 || s->title == 7)) {
                s->news |= 2;
            }
        }
        if (AT(st, 0x78, u32) & 0x8000) {
            *flags |= 0x400000;
        }
        bit_set(unlock, s->title + 5);
        if (s->news) {
            s->step = 0;
            s->textAlpha = 0;
            Task_Close(&s->task);
            ptmf_set_fn(END_SEQ(s), func_00372690);
        } else if (!ending_title_message(s)) {
            s->step++;
        }
        break;
    }
    case 7:
        ending_fade_to_save(s);
        break;
    }
}

/* the record's DOG LEVEL (0 A .. 4 E): +0xFFC, less +0xFFE in bands of 20, plus +0x1000 in
 * bands of 10 (the two Hewie values the game scene keeps at the ending, func_0039E380) */
static inline s32 ending_dog_level(u8 *st) {
    s16 saves = AT(st, 0xFFE, s16), escapes = AT(st, 0x1000, s16);
    s16 a, b;
    s32 v;

    if (saves < 20) {
        a = 0;
    } else if (saves < 40) {
        a = 1;
    } else if (saves < 60) {
        a = 2;
    } else if (saves < 80) {
        a = 3;
    } else {
        a = 4;
    }
    if (escapes < 10) {
        b = 0;
    } else if (escapes < 20) {
        b = 1;
    } else if (escapes < 30) {
        b = 2;
    } else {
        b = 3;
    }
    v = b + (AT(st, 0xFFC, s16) + 1 - a);
    if (v < 7) {
        return 4;
    }
    if (v >= 10) {
        return 0;
    }
    return 10 - v;
}

/* the TYPE the record earns (0..10, the first that applies): +0xFFE 100 or more; ending C;
 * dog level A with no critical Hewie injuries (+0x1002); items 80% or more (+0x1006 of
 * +0x1008); 30 or more enemies defeated (+0x1004); under 3 hours (+0x100C); dog level A or B;
 * panic 30 or more (+0x100A); under 6 hours; under 20 hours (0); else 10 */
static inline s32 ending_title(SceneEnding *s, u8 *st) {
    static const union { u32 u; f32 f; } k100 = {0x42C80000};   /* 100 */
    s32 pct = (s32)(k100.f * (f32)AT(st, 0x1006, s16) / (f32)AT(st, 0x1008, s16));
    u8 hours;

    if (AT(st, 0xFFE, s16) >= 100) {
        return 1;
    }
    if (AT(st, 0x111, u8) == 2) {
        return 2;
    }
    if (s->dogLevel == 0 && AT(st, 0x1002, s16) == 0) {
        return 3;
    }
    if (pct >= 80) {
        return 4;
    }
    if (AT(st, 0x1004, s16) >= 30) {
        return 5;
    }
    hours = AT(st, 0x100C, u8);
    if (hours < 3) {
        return 6;
    }
    if ((u32)s->dogLevel < 2) {
        return 7;
    }
    if (AT(st, 0x100A, s16) >= 30) {
        return 8;
    }
    if (hours < 6) {
        return 9;
    }
    if (hours < 20) {
        return 0;
    }
    return 10;
}

/* the results' setup: the sub screen and message object reset, the ending's picture and
 * MSG_END loaded, the dog level and type worked out, the results music (0x33); then func_00372A90 */
void func_003738C0(SceneEnding *s) {
    u8 *st = gSystemData + 0x190;
    VObject *msg;
    VObject *snd;

    SubScreen_Start(END_SUB(s));
    func_0026BCC0(END_MSG(s));
    switch (AT(st, 0x111, u8)) {
    case 0:
        LOADER_LOAD(D_00463840, END_PIC_FILE(s));
        break;
    case 1:
        LOADER_LOAD(D_00463860, END_PIC_FILE(s));
        break;
    case 2:
        LOADER_LOAD(D_00463880, END_PIC_FILE(s));
        break;
    case 3:
        LOADER_LOAD(D_004638A0, END_PIC_FILE(s));
        break;
    }
    msg = gBootMessage;
    VCALL(msg, 0x8, void (*)(VObject *, s32, void *))(msg, 6, END_PIC_FILE(s));
    VCALL(msg, 0x10, void (*)(VObject *, s32, void *, s32))(msg, 6, END_PIC_FILE(s), 0);
    LOADER_LOAD(D_004638C0, END_TEXT(s));
    D_01991EC4 = END_TEXT(s);
    LOADER_LOAD(D_004638E0, END_FONT(s));
    VCALL(gTexCache, 0x10, void (*)(VObject *, void *, s32))(gTexCache, END_FONT(s), 0x15);
    D_0047B350 = 1;
    s->dogLevel = ending_dog_level(st);
    s->title = ending_title(s, st);
    s->step = 0;
    s->picAlpha = 0;
    s->textAlpha = 0;
    s->news = 0;
    snd = gSound;
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
    }
    func_002D1FD0(gAdx);
    VCALL(gMusic, 0x8, void (*)(VObject *, s32, s32, s32, f32))(gMusic, 0x33, 0, 0, 1.0f);
    ptmf_set_fn(END_SEQ(s), func_00372A90);
}

/* state: the results (their own state at +0x117500), then the BGM */
void func_00373E50(SceneEnding *s) {
    PTMF *seq = END_SEQ(s);

    if (ptmf_test(seq)) {
        ptmf_scall(s, seq);
    }
    func_002E3200((BgmCtl *)END_BGM(s));
}

/* state: fade the staff roll out (sound and picture, 1/30 a frame; at 0 its scene is
 * finished); once it's gone, the results (func_003738C0) */
void func_00373EB0(SceneEnding *s) {
    u8 *m = gMovie;
    f32 l;

    if (m == NULL) {
        ptmf_set_fn(&s->base.state, func_00373E50);
        ptmf_set_fn(END_SEQ(s), func_003738C0);
        return;
    }
    l = END_VOLUME(s) - 0x1.111112p-5f;
    END_VOLUME(s) = l;
    if (l < 0.0f) {
        l = 0.0f;
    }
    END_VOLUME(s) = l;
    if (l <= 0.0f) {
        Scene *sc = SCENE_TABLE_SCENE(1);

        if (sc != NULL) {
            ptmf_set(&sc->state, &sSceneFinish);
            VCALL(SCENE_TABLE_SCENE(1), 0x14, void (*)(Scene *))(SCENE_TABLE_SCENE(1));
        }
    }
    {
        f32 *v = &AT(m, 0x1D4, f32);

        *v = END_VOLUME(s);
        if (END_VOLUME(s) < 0.0f) {
            *v = 0.0f;
        }
        if (!(*v <= 1.0f)) {
            *v = 1.0f;
        }
    }
    m = gMovie;
    func_002B6340((Movie *)m);
    if (AT(m, 0x1B4, u8)) {
        u32 a = (u32)(127.0f * (1.0f - END_VOLUME(s)));

        if (a >= 0x80) {
            a = 0x7F;
        }
        VCALL(gRenderer, 0x7C, void (*)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, u32, s32, s32, s32,
                                         s32))(gRenderer, 0, 0, 0x200, 0x200, 0, 0, 0, 0, (a << 24) & 0xFF000000, -1,
                                               0, 0x30, -1);
    }
}

/* state: the staff roll (Start, once it shows, skips it), then its fade */
void func_003741A0(SceneEnding *s) {
    s32 skip = 0;

    if ((D_0047E37C & PAD_START) && gMovie != NULL && AT(gMovie, 0x1B4, u8)) {
        skip = 1;
    }
    if (gMovie != NULL && !skip) {
        return;
    }
    ptmf_set_fn(&s->base.state, func_00373EB0);
}

/* state: start the staff roll as scene 1 at full volume */
void func_00374260(SceneEnding *s) {
    void *table = gSceneTable;
    VObject *heap = (VObject *)((u8 *)table + 0x10D9040);
    Scene *movie;
    void *mem;
    s32 ok;

    END_VOLUME(s) = 1.0f;
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
        func_002B6D10(gMovie, D_004638F8, 1, 0);
    }
    ptmf_set_fn(&s->base.state, func_003741A0);
}

/* +0x10 entry: the staff roll */
void func_003743B0(SceneEnding *s) {
    ptmf_set_fn(&s->base.state, func_00374260);
}
