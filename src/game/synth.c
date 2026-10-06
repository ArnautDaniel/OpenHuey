/* The item synthesizer (the sub screen's modes 3 and 4, "SUBSCR\SYNSLOT"): a 0x15C-byte page
 * object at +0xA8C80 of the sub screen (subscreen.c sub_new_slots), base class D_00474020 with
 * the pot (mode 4, D_00474040) and the slot machine (mode 3, D_00474060). Fiona picks one of
 * six materials, the machine rolls ten rows of symbols; matching pairs among 22 cells (or the
 * wild symbol 5) count up four kinds of symbol, which pick the item made.
 *
 * The object: +0x4 its state (PTMF), +0x10 the row being rolled (-1 before the first) or the
 * list's cursor, +0x14 its message task, +0x118 the six materials' counts (then the found
 * item's places), +0x11E the material used, +0x11F the roll's flags (1 rolling, 2 all rows
 * done, 4 a row stopping), +0x120 the ten rows' symbols (-1 not rolled), +0x12A the reel
 * (D_00460430: 35 symbols each), +0x12B its position, +0x12C the step between symbols, +0x12D /
 * +0x12E the roll's timer and speed, +0x12F the 22 cells' symbols (-1 no match), +0x145 the
 * four symbols' counts, +0x14A the result (-1 none), +0x14B how many, +0x14C the item (big
 * endian), +0x150 the prompt's blink, +0x151 the found item's frame, +0x152 its sound played,
 * +0x158 "leave". */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "task.h"
#include "input.h"
#include "sound.h"
#include "texcache.h"
#include "globals.h"

extern void *D_00474020[];
extern u8 D_0047B350;
extern void func_00322560(void *p);                 /* delete (the sub screen's pool) */
extern u32 func_00260CF0(void *items, s32 id);      /* how many of an item */
extern s32 func_00260BB0(void *items, s32 id);      /* one of an item used */
extern const char D_00460408[];                     /* the count's format */
extern const char D_00460410[];                     /* "ITEM SYNTHESIZER(POT)" */
extern const PTMF D_0042C410, D_0042C420, D_0042C430, D_0042C440, D_0042C450;

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

/* base +0x8: destroy (the task's window freed); the object returned, not freed */
void *func_003224F0(u8 *o) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00474020;
        VCALL(o, 0x14, void (*)(u8 *))(o);
        if ((Task *)(o + 0x14) != NULL && SY_TASK(o)->child != NULL) {
            Task_dtor(SY_TASK(o)->child, 1);
            SY_TASK(o)->child = NULL;
        }
    }
    return o;
}

/* base +0x10: each frame, its state */
void func_00322A10(u8 *o) {
    D_0047B350 = 1;
    if (ptmf_test(SY_STATE(o))) {
        ptmf_scall(o, SY_STATE(o));
    }
}

/* a subclass's +0x8: destroy as the base (its task too), freed when `flags` > 0 */
static inline void *sy_dtor(u8 *o, void **vtbl, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = vtbl;
        if (o != NULL) {
            AT(o, 0x0, void **) = D_00474020;
            VCALL(o, 0x14, void (*)(u8 *))(o);
            Task_dtor(SY_TASK(o), -1);
        }
        if ((s16)flags > 0) {
            func_00322560(o);
        }
    }
    return o;
}

extern void *D_00474040[], *D_00474060[];

/* the pot +0x8 */
void *func_00322AA0(u8 *o, s32 flags) {
    return sy_dtor(o, D_00474040, flags);
}

/* the slot machine +0x8 */
void *func_00322C40(u8 *o, s32 flags) {
    return sy_dtor(o, D_00474060, flags);
}

/* the six materials' counts (items 0x70..0x75) into +0x118..; any at all */
s32 func_00322970(u8 *o) {
    u8 *items = (u8 *)gItems + 8;
    s32 any = 0, i;

    for (i = 0; i < 6; i++) {
        s8 n = func_00260CF0(items, i + 0x70);

        o[0x118 + i] = n;
        any = any || n != 0;
    }
    return any;
}

/* the pot +0xC: started - the counts taken, its state D_0042C410 */
void func_00322BD0(u8 *o) {
    SY_STEP(o) = 0;
    func_00322970(o);
    ptmf_set(SY_STATE(o), &D_0042C410);
}

/* the pot +0x10: its title (a debug line), then as the base */
void func_00322B80(u8 *o) {
    Task_Printf(SY_TASK(o), 0x32, 0x32, 0x80, D_00460410);
    func_00322A10(o);
}

/* the slot machine +0xC: started - the counts taken; the material list (D_0042C420) or, with
 * none, "nothing to use" (D_0042C430) */
void func_00324650(u8 *o) {
    SY_STEP(o) = 0;
    o[0x150] = 0;
    if (func_00322970(o)) {
        ptmf_set(SY_STATE(o), &D_0042C420);
    } else {
        ptmf_set(SY_STATE(o), &D_0042C430);
    }
}

/* the slot machine +0x10: as the base, then the start prompt blinking (alpha 0x40..0x80 over
 * 60 frames) */
void func_003244D0(u8 *o) {
    s32 t;
    f32 f;

    func_00322A10(o);
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

/* state: "nothing to put in" - message 3; cancel or confirm leaves */
void func_00323040(u8 *o) {
    Task_Open(SY_TASK(o), 3);
    Task_Run(SY_TASK(o));
    if (D_0047E36C & (MENU_CANCEL | MENU_CONFIRM)) {
        o[0x158] = 1;
    }
}

/* the material list: a frame as long as the materials there are, each with its count, the
 * cursor (+0x10) round them; message 2 while choosing. Confirm on one there is: its place
 * (0..5), cancel: "leave"; -1 for none chosen (or no materials) */
s8 func_00322580(u8 *o) {
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
    if (D_0047E36C & MENU_UP) {
        if (--SY_STEP(o) < 0) {
            SY_STEP(o) = last;
        }
    } else if (D_0047E36C & MENU_DOWN) {
        if (last < ++SY_STEP(o)) {
            SY_STEP(o) = 0;
        }
    }
    if (old != SY_STEP(o)) {
        Sound_PlaySE(SE_CURSOR);
    } else if (D_0047E36C & MENU_CANCEL) {
        o[0x158] = 1;
    } else if ((D_0047E36C & MENU_CONFIRM) && (s8)o[0x118 + sel] > 0) {
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

/* state: choosing the material; once chosen, one of it used, the rolling begins (D_0042C440) */
void func_00324410(u8 *o) {
    s8 m = func_00322580(o);

    if (m >= 0) {
        func_00260BB0((u8 *)gItems + 8, m + 0x70);
        o[0x11E] = m;
        o[0x11F] = 0;
        SY_STEP(o) = -1;
        ptmf_set(SY_STATE(o), &D_0042C440);
    }
}

/* the found item's step: the panel over the rows fading in (3 frames) and out (12; then bit 0,
 * "up"); when `shown`, the item's panel after it, fading out from frame 9 (then bit 7, "done"
 * - and with nothing shown, done as soon as up); counted to 50 */
u8 func_00322CD0(u8 *o, s32 shown) {
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

/* the result for the four symbols' counts (+0x145..): the more symbols and kinds the better
 * (0 best); -1 for none */
s32 func_00323890(u8 *o) {
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

/* draws: a row's symbol and a cell's (GL) */
void func_00323510(u8 *o, u8 row, u8 v, s32 a);
void func_003230B0(u8 *o, u8 k, u8 v);

extern const u8 D_00460430[][35];      /* the reels */
extern const u8 D_004605C0[][11];      /* per material: the reels' chances (%) */
extern const u8 D_00460608[11];        /* the reels picked by them (the last: none picked) */
extern const u8 D_00460620[][6];       /* per material: the first row's symbols' chances (%) */
extern const u8 D_00460650[22][2];     /* the cells: the two rows they compare */
extern const s32 D_00460680[][9];      /* per result: 8 items (-1 none), its amounts' row */
extern const s8 D_00460A18[][4];       /* the amounts by how many symbols matched */

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
 * (D_0042C450) */
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
    o[0x14A] = res = func_00323890(o);
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
            u8 *items = (u8 *)gItems + 8;
            s32 list[8], k = 0, j, id;

            for (j = 0; j < 8; j++) {
                id = D_00460680[(s8)o[0x14A]][j];
                if (id == -1) {
                    continue;
                }
                if ((u32)id >= 0x80 && (u32)id < 0x90 && (u8)func_00260CF0(items, id)) {
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
    ptmf_set(SY_STATE(o), &D_0042C450);
}

/* state: the rolling. First the top row's symbol drawn by the material's chances
 * (D_00460620); then each row in turn: its reel picked (D_004605C0 / D_00460608) at a random
 * place, spun (a symbol every 5 frames, speeding through 8 levels every 31) until confirm or
 * the top speed stops it, easing onto the next symbol; after the ninth, sy_finish. The rows
 * drawn (the rolling one with the next symbol coming in) and message 0x10 */
void func_00323A70(u8 *o) {
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
                    o[0x12A] = D_00460608[i];
                    found = 1;
                    break;
                }
            }
            if (!found) {
                o[0x12A] = D_00460608[10];
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
            if ((D_0047E36C & MENU_CONFIRM) || (s8)o[0x12E] == 8) {
                Sound_Play(gSound, 2, 6);
                o[0x11F] |= 4;
            }
        }
    }
    for (r = 0; r < 10; r++) {
        s8 v = o[0x120 + r];

        if (v >= 0) {
            func_00323510(o, r, v, 0);
        } else if (r == SY_STEP(o) + 1) {
            s8 c;

            func_00323510(o, r, SY_REEL(o, o[0x12B]), 0);
            c = o[0x12C];
            if (c != 0) {
                u8 next = (s8)o[0x12B] + 1;

                if (next >= 0x23) {
                    next = 0;
                }
                if (SY_REEL(o, next) != SY_REEL(o, o[0x12B])) {
                    func_00323510(o, r, SY_REEL(o, next), (u8)c);
                }
            }
        }
    }
    Task_Open(SY_TASK(o), 0x10);
    Task_Run(SY_TASK(o));
}

#ifdef HG_NATIVE
#include "gl2d.h"

/* a symbol's palette (symbols 0..5; others 1) */
static s32 sy_clut(u8 v) {
    static const u8 kClut[6] = {6, 3, 4, 5, 0, 7};

    return v < 6 ? kClut[v] : 1;
}

/* row `row`'s symbol `v` (32 x 32 at D_004608F0 + 16, the texture's column 0x60 + 64 x `a`: `a`
 * the frame of it sliding in), with OpenGL */
void func_00323510(u8 *o, u8 row, u8 v, s32 a) {
    extern const s16 D_004608F0[][2];
    u8 *tex;
    s32 x = D_004608F0[row][0] + 0x10, y = D_004608F0[row][1] + 0x10, u = (u8)a * 64 + 0x60;

    if (TexCache_Resident(4, 0x19, 0x30, &tex) == -1) {
        return;
    }
    gl2d_sprite(0x30, x, y, x + 0x20, y + 0x20, tex, u, 0, u + 0x20, 0x20, 0x80808080, sy_clut(v), 0x40);
}

/* cell `k`'s symbol `v` (D_00460920: its texels u, v, w, h, place x, y and flips), with OpenGL */
void func_003230B0(u8 *o, u8 k, u8 v) {
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
