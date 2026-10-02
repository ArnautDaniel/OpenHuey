/* Tasks: message boxes and on-screen text (see include/task.h for the message byte code).
 * A task lays out a message, types it out a glyph at a time and handles page breaks, waits,
 * choices and nested tasks through its state; drawing builds GS packets in a renderer layer:
 * the box (a soft-edged dark rectangle), the font state, then a sprite per glyph. */
#include <stdarg.h>

#include "common.h"
#include "gs.h"
#include "ptmf.h"
#include "task.h"
#include "input.h"
#include "sound.h"


extern u8 *D_01991EC0[];        /* message tables, by language */
extern char D_01991ED0[][32];   /* parameter strings (code 0x08) */
extern char D_01991F50[][8];    /* names (code 0x13) */
extern u8 D_0047B350;           /* language */
extern u8 D_0044AD00[];         /* small font glyphs: high nibble blank columns, low nibble drop */
extern u16 D_0044B010[][2];     /* box position presets */
extern u8 D_0047B140[];         /* frames per glyph, by speed */
extern u8 D_0047B144[];         /* the choice cursor glyph */
extern u8 D_0047B148[];         /* the page arrow glyph */
extern void *D_0044E4E8;        /* texture cache */
extern void *D_0044E4F0;        /* renderer */
extern void *D_0044E9A0;        /* VRAM manager */

extern void func_00100490(void *p);   /* operator delete */
extern s32 func_0026ED98(char *buf, s32 n, const char *fmt, va_list ap);   /* vsnprintf */
extern f32 func_0031C058(f32 x);      /* cosf */
extern f32 func_0031C248(f32 x);      /* sinf */

void Task_DrawPage(Task *t);
void Task_StateChild(Task *t);
void Task_StateChoice(Task *t);
void Task_StateWaitButton(Task *t);
void Task_StateWaitFrames(Task *t);
void Task_StateNextPage(Task *t);
void Task_StateGlyphDelay(Task *t);
void Task_StatePause(Task *t);
void Task_StateType(Task *t);
u16 Task_LineWidth(Task *t, TextCursor *src);
void Task_Layout(Task *t);
void Task_CollectChoices(Task *t);
void Task_DrawGlyph(Task *t, s32 x, s32 y, s32 w, s32 h, s32 color, u8 *g);
void Task_BeginDraw(Task *t);
s32 TextCursor_Step(Task *t, TextCursor *c);

#define RENDERER_ALLOC(n, layer) \
    VCALL(D_0044E4F0, 0x10, u64 *(*)(void *, s32, s32))(D_0044E4F0, n, layer)
#define RENDERER_UPLOAD(slot, tex, layer) \
    VCALL(D_0044E4F0, 0x44, s32 (*)(void *, s32, u8 *, s32))(D_0044E4F0, slot, tex, layer)
#define TEXCACHE_SLOT(sel, group) \
    VCALL(D_0044E4E8, 0x8, s32 (*)(void *, s32, s32))(D_0044E4E8, sel, group)
#define TEXCACHE_TEX(sel, group) \
    VCALL(D_0044E4E8, 0xC, u8 *(*)(void *, s32, s32))(D_0044E4E8, sel, group)

/* a window-coordinate XYZ2 value (12.4 fixed point), farthest depth */
#define XYZ(x, y) ((u64)(u32)((x) << 4) | ((u64)(u32)((y) << 4) << 16) | 0xFFFFFFFF00000000ULL)

static inline void Task_SetState(Task *t, void (*state)(Task *)) {
    ptmf_set_fn(&t->state, (void *)state);
}

/* the text of message `id` (bit 15: from the language 0 table) */
static inline u8 *msg_text(u32 id) {
    u8 lang;
    u8 *table;

    if (id & 0x8000) {
        lang = 0;
        id &= 0x7FFF;
    } else {
        lang = D_0047B350;
    }
    table = D_01991EC0[lang];
    return table + *(u16 *)(table + (id & 0xFFFF) * 2 + 2);
}

/* advance width of the glyph at `p` */
static inline s32 glyph_w(Task *t, u8 *p) {
    s32 i;

    if (p[0] >= 0x1A && p[0] < 0x1D) {
        return t->glyphW;
    }
    i = p[0] == 0x1D ? p[1] + 0xE0 : p[0] - 0x20;
    return (16 - (D_0044AD00[i] >> 4)) * t->glyphW / 16 + 1;
}

/* copy a cursor (the depth as a byte: not the padding after it) */
static inline void cursor_copy(TextCursor *d, const TextCursor *s) {
    d->p = s->p;
    d->depth = s->depth;
    d->stack[0] = s->stack[0];
    d->stack[1] = s->stack[1];
    d->stack[2] = s->stack[2];
    d->stack[3] = s->stack[3];
}

/* box position preset `n` (bit 7: without the frame) */
static inline void set_position(Task *t, u8 n) {
    t->x = D_0044B010[n & 0x7F][0];
    t->y = D_0044B010[n & 0x7F][1];
    if (n & 0x80) {
        t->flags &= ~4;
    } else {
        t->flags |= 4;
    }
}

/* +0x7C: delete the child task */
static inline void delete_child(Task *t) {
    Task *c = t->child;

    if (c != NULL) {
        if (c->child != NULL) {
            Task_dtor(c->child, 1);
            c->child = NULL;
        }
        func_00100490(c);
        t->child = NULL;
    }
}

/* Destructor: deletes the chain of children (deepest first). */
Task *Task_dtor(Task *t, s32 flags) {
    Task *chain[7];
    s32 n;

    if (t != NULL) {
        chain[0] = t;
        for (n = 0; n < 6 && chain[n]->child != NULL; n++) {
            chain[n + 1] = chain[n]->child;
        }
        if (n == 6 && chain[6]->child != NULL) {
            Task_dtor(chain[6]->child, 1);
            chain[6]->child = NULL;
        }
        for (; n > 0; n--) {
            func_00100490(chain[n]);
            chain[n - 1]->child = NULL;
        }
        if ((s16)flags > 0) {
            func_00100490(t);
        }
    }
    return t;
}

/* line height */
s32 Task_LineHeight(Task *t) {
    return 25;
}

void Task_DrawBox(Task *t, s32 x, s32 y, s32 w, s32 h, s32 alpha, s32 layer);

/* the box frame, a little larger than the text */
void Task_BoxSize(Task *t) {
    s32 w, h;

    if (t->flags & 4) {
        w = t->w;
        if (w >= 0x19) {
            w -= 0x18;
        }
        h = t->h;
        if (h >= 0x11) {
            h -= 0x10;
        }
        Task_DrawBox(t, t->x, t->y + 6, w, h, 0x60, t->layer);
    }
}

/* copy message `id` to name `slot` (code 0x13; up to 7 bytes) */
void Msg_SetName(void *self, s32 slot, s32 id) {
    u8 *src = msg_text(id & 0xFFFF);
    char *dst = D_01991F50[slot & 0xFF];
    s32 i = 0;

    for (;;) {
        u8 c = src[i];

        if (c == 0 || c == 1) {
            break;
        }
        if ((c >= 0x1A && c < 0x1E) || c == 3) {
            dst[i] = c;
            i++;
        }
        dst[i] = src[i];
        i++;
        if (i >= 8) {
            break;
        }
    }
    dst[i] = 0;
}

/* copy system message `id` (0x100 + id of the language 0 table) to parameter string `slot` */
void Msg_SetParamSystem(void *self, s32 slot, s32 id) {
    u8 *src = msg_text((u16)((id & 0xFFFF) + 0x8100));
    char *dst = D_01991ED0[slot & 0xFF];
    s32 i = 0;

    for (;;) {
        u8 c = src[i];

        if (c == 0 || c == 1) {
            break;
        }
        if ((c >= 0x1A && c < 0x1E) || c == 8) {
            dst[i] = c;
            i++;
        }
        dst[i] = src[i];
        i++;
        if (i >= 31) {
            break;
        }
    }
    dst[i] = 0;
}

/* printf to parameter string `slot` */
void Msg_PrintfParam(void *self, s32 slot, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    func_0026ED98(D_01991ED0[slot & 0xFF], 0x80, fmt, ap);
    va_end(ap);
}

/* The box: a dark rectangle (centre x, y; w x h) whose edges and rounded corners fade out over
 * 32 pixels: a sprite, four gradient strips and four triangle fans in one packet. */
void Task_DrawBox(Task *t, s32 x, s32 y, s32 w, s32 h, s32 alpha, s32 layer) {
    static const f32 angles[5] = {0.0f, 0x1.921fb6p-2f, 0x1.921fb6p-1f, 0x1.2d97c8p+0f, 0x1.921fb6p+0f};
    u64 *p = RENDERER_ALLOC(0x47, layer);
    u64 rgba, *f;
    s32 top, bottom, left, right, k, i;

    if (p == NULL) {
        return;
    }
    rgba = (u64)((s64)alpha << 24);
    top = y + 0x720 - h / 2;
    bottom = y + 0x720 + h / 2;
    left = x + 0x700 - w / 2;
    right = x + 0x700 + w / 2;

    p[0] = DMA_TAG(DMA_CNT, 0x46, 0);
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x50000046;   /* VIF DIRECT */
    p[2] = 2 | (1ULL << 60);        /* GIF tag: 2 A+D */
    p[3] = 0xE;
    p[4] = 0x44;                    /* ALPHA_1: (Cs - Cd) * As + Cd */
    p[5] = GS_ALPHA_1;
    p[6] = 0x46;                    /* PRIM: sprite, blended */
    p[7] = GS_PRIM;
    p[8] = 1 | (0x44ULL << 56);     /* GIF tag: reglist RGBAQ XYZ2 XYZ2 NOP */
    p[9] = 0xF551;
    p[10] = rgba;
    p[11] = XYZ(left, top);
    p[12] = XYZ(right, bottom);
    p[13] = 0;

    /* the edges: blended strips (PRIM 0x4C), opaque inside, transparent 32 pixels out */
    for (k = 0; k < 4; k++) {
        f = p + 14 + k * 14;
        f[0] = 1 | (1ULL << 60);
        f[1] = 0xE;
        f[2] = 0x4C;
        f[3] = GS_PRIM;
        f[4] = 1 | (0x84ULL << 56);   /* reglist (RGBAQ XYZ2) x 4 */
        f[5] = 0x51515151;
    }
    f = p + 14;   /* top */
    f[6] = 0;
    f[7] = XYZ(left, top - 32);
    f[8] = rgba;
    f[9] = XYZ(left, top);
    f[10] = 0;
    f[11] = XYZ(right, top - 32);
    f[12] = rgba;
    f[13] = XYZ(right, top);
    f = p + 28;   /* right */
    f[6] = rgba;
    f[7] = XYZ(right, top);
    f[8] = 0;
    f[9] = XYZ(right + 32, top);
    f[10] = rgba;
    f[11] = XYZ(right, bottom);
    f[12] = 0;
    f[13] = XYZ(right + 32, bottom);
    f = p + 42;   /* left */
    f[6] = 0;
    f[7] = XYZ(left - 32, top);
    f[8] = rgba;
    f[9] = XYZ(left, top);
    f[10] = 0;
    f[11] = XYZ(left - 32, bottom);
    f[12] = rgba;
    f[13] = XYZ(left, bottom);
    f = p + 56;   /* bottom */
    f[6] = rgba;
    f[7] = XYZ(left, bottom);
    f[8] = 0;
    f[9] = XYZ(left, bottom + 32);
    f[10] = rgba;
    f[11] = XYZ(right, bottom);
    f[12] = 0;
    f[13] = XYZ(right, bottom + 32);

    /* the corners: fans (PRIM 0x4D) from the inner corner to a quarter circle, top left, top
     * right, bottom left, bottom right */
    for (k = 0; k < 4; k++) {
        s32 cx = (k & 1) ? right : left;
        s32 cy = (k & 2) ? bottom : top;

        f = p + 70 + k * 18;
        f[0] = 1 | (1ULL << 60);
        f[1] = 0xE;
        f[2] = 0x4D;
        f[3] = GS_PRIM;
        f[4] = (k == 3 ? 0x8001 : 1) | (0xC4ULL << 56);   /* reglist (RGBAQ XYZ2) x 6; EOP last */
        f[5] = 0x515151515151ULL;
        f[6] = rgba;
        f[7] = XYZ(cx, cy);
        for (i = 0; i < 5; i++) {
            s32 dx, dy;

            f[8 + i * 2] = 0;
            dx = (s32)(32.0f * func_0031C058(angles[i]));
            dy = (s32)(32.0f * func_0031C248(angles[i]));
            f[9 + i * 2] = XYZ((k & 1) ? cx + dx : cx - dx, (k & 2) ? cy + dy : cy - dy);
        }
    }
}

/* Draw glyph `g` as a w x h sprite at x, y. color: low 6 bits the CLUT, 0x40 / 0x80 blend
 * modes. Big-font glyphs (0x1A..0x1C) come from the language's font texture, uploaded when its
 * VRAM slot was reassigned. */
void Task_DrawGlyph(Task *t, s32 x, s32 y, s32 w, s32 h, s32 color, u8 *g) {
    u8 *font, *tex;
    s32 slot, u, v;
    u64 *p;

    if (t->fontSlot == -1) {
        return;
    }
    font = TEXCACHE_TEX(0, 0x14);
    if (g[0] >= 0x1A && g[0] < 0x1D) {
        u8 lang = D_0047B350;
        s32 group, idx;

        if (D_01991EC0[lang] == NULL) {
            return;
        }
        if (g[0] == 0x1C) {
            lang++;
        }
        group = lang + 0x14;
        slot = TEXCACHE_SLOT(0, group);
        if (slot == -1) {
            return;
        }
        tex = TEXCACHE_TEX(0, group);
        if (slot & 0x80000000) {
            slot &= 0x7FFFFFFF;
            if (!(u8)RENDERER_UPLOAD(slot, tex, t->layer)) {
                return;
            }
        }
        idx = g[1];
        if (g[0] == 0x1B) {
            idx += 0x100;
        }
        if (D_0047B350 == 2) {
            u = (idx & 0x1F) << 4;
            v = (idx & 0xFFE0) >> 1;
        } else {
            u = (idx & 0xF) << 4;
            v = idx & 0xFFF0;
        }
    } else {
        s32 idx = g[0] == 0x1D ? g[1] + 0xE0 : g[0] - 0x20;

        slot = t->fontSlot;
        tex = font;
        u = (idx & 0xF) << 4;
        v = idx & 0xFFF0;
    }

    p = RENDERER_ALLOC(8, t->layer);
    if (p == NULL) {
        return;
    }
    p[0] = DMA_TAG(DMA_CNT, 7, 0);
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x50000007;
    p[2] = 1 | (1ULL << 60);
    p[3] = 0xE;
    if (color & 0x80) {
        s32 c = color & 0x7F;

        p[4] = ((u64)t->alpha << 32) | (c >= 4 && c < 7 ? 0x44 : 0x62);
    } else if (color & 0x40) {
        p[4] = ((u64)t->alpha << 32) | 0x44;
    } else {
        p[4] = ((u64)t->alpha << 32) | 0x68;
    }
    p[5] = GS_ALPHA_1;
    p[6] = 0x8001 | (0x74ULL << 56);   /* reglist: TEX0 CLAMP RGBAQ UV XYZ2 UV XYZ2 */
    p[7] = 0x05353186;
    p[8] = VCALL(D_0044E9A0, 0x30, u64 (*)(void *, s32, s32, s32, s32, s32, s32))(
        D_0044E9A0, slot, color & 0x3F, tex[0], AT(tex, 4, u16), AT(tex, 6, u16), font[1]);
    p[9] = 0xA | ((u64)(s64)u << 4) | ((u64)(s64)(u + 15) << 14) | ((u64)(s64)v << 24)
           | ((u64)(s64)(v + 15) << 34);   /* CLAMP: region u..u+15, v..v+15 */
    p[10] = 0x80606060;
    p[11] = (u64)(u32)(u << 4) | ((u64)(u32)(v << 4) << 16);
    p[12] = (u64)(u32)(((x + 0x700) << 4) + 8) | ((u64)(u32)(((y + 0x720) << 4) + 8) << 16)
            | 0xFFFFFFFF00000000ULL;
    p[13] = (u64)(u32)((u + 16) << 4) | ((u64)(u32)((v + 16) << 4) << 16);
    p[14] = (u64)(u32)(((x + 0x700 + w) << 4) + 8) | ((u64)(u32)(((y + 0x720 + h) << 4) + 8) << 16)
            | 0xFFFFFFFF00000000ULL;
}

/* Start drawing: make sure the small font is in VRAM (else nothing is drawn: fontSlot -1),
 * draw the frame, then the text state (blending, font TEX0, bilinear, sprites). */
void Task_BeginDraw(Task *t) {
    u8 *font;
    u64 *p;

    t->fontSlot = TEXCACHE_SLOT(0, 0x14);
    if (t->fontSlot == -1) {
        return;
    }
    font = TEXCACHE_TEX(0, 0x14);
    if (t->fontSlot & 0x80000000) {
        t->fontSlot &= 0x7FFFFFFF;
        if (!(u8)RENDERER_UPLOAD(t->fontSlot, font, t->layer)) {
            t->fontSlot = -1;
            return;
        }
    }
    Task_BoxSize(t);
    p = RENDERER_ALLOC(7, t->layer);
    if (p == NULL) {
        return;
    }
    p[0] = DMA_TAG(DMA_CNT, 6, 0);
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x50000006;
    p[2] = 0x8005 | (1ULL << 60);   /* GIF tag: 5 A+D, EOP */
    p[3] = 0xE;
    p[4] = ((u64)t->alpha << 32) | ((t->baseColor & 0x80) ? 0x62 : 0x68);
    p[5] = GS_ALPHA_1;
    p[6] = VCALL(D_0044E9A0, 0x2C, u64 (*)(void *, s32, s32, s32, s32))(
        D_0044E9A0, t->fontSlot, AT(font, 4, u16), AT(font, 6, u16), font[1]);
    p[7] = 0x06;                    /* TEX0_1 */
    p[8] = 0x60;                    /* TEX1_1: bilinear */
    p[9] = GS_TEX1_1;
    p[10] = (0x80ULL << 32) | 0x8080;
    p[11] = GS_TEXA;
    p[12] = 0x156;                  /* PRIM: sprite, textured, blended, UV */
    p[13] = GS_PRIM;
}

/* Step over control code / nested string handling at c->p: 0 handled (moved on), 1 a glyph,
 * 2 a big-font glyph (2 bytes); the caller moves past glyphs. */
s32 TextCursor_Step(Task *t, TextCursor *c) {
    u8 *p = c->p;

    switch (*p) {
    case 0x00:
        if (c->depth != 0) {
            c->depth--;
            c->p = c->stack[c->depth];
        }
        return 0;
    case 0x08:
        c->stack[c->depth] = p + 2;
        c->depth++;
        c->p = (u8 *)D_01991ED0[c->p[1]];
        return 0;
    case 0x13:
        c->stack[c->depth] = p + 2;
        c->depth++;
        c->p = (u8 *)D_01991F50[c->p[1]];
        return 0;
    case 0x11:
        c->p = msg_text((u16)(p[1] << 8 | p[2]));
        return 0;
    case 0x15:
        c->stack[c->depth] = p + 3;
        c->depth++;
        c->p = msg_text((u16)((u16)(c->p[1] << 8 | c->p[2]) + 0x8100));
        return 0;
    case 0x0B: case 0x0E: case 0x0F: case 0x19:
        c->p = p + 3;
        return 0;
    case 0x03: case 0x07: case 0x09: case 0x0A: case 0x0C: case 0x10:
        c->p = p + 2;
        return 0;
    case 0x1A: case 0x1B: case 0x1C: case 0x1D:
        return 2;
    }
    if (*p >= 0x1E) {
        return 1;
    }
    c->p = p + 1;
    return 0;
}

/* Collect the options of a choice (code 0x0F) on the page: where each is drawn, what it leads to. */
void Task_CollectChoices(Task *t) {
    TextCursor c;
    s32 hidden = 0, done = 0, x, y, n, i;

    c.p = t->page;
    c.depth = 0;
    x = t->x - (t->w >> 1);
    t->nOptions = 0;
    y = t->y - (t->h >> 1);
    for (i = 0; i < 8; i++) {
        t->optId[i] = 0xFFFF;
    }
    do {
        u8 *p = c.p;

        switch (*p) {
        case 0x18:
            c.p = p + 1;
            hidden = 1;
            break;
        case 0x17:
            c.p = p + 1;
            hidden = 0;
            break;
        case 0x14:
            c.p = p + 1;
            x = t->x + (t->w >> 1);
            x -= (u16)Task_LineWidth(t, &c);
            break;
        case 0x12:
            c.p = p + 1;
            x = t->x - ((u16)Task_LineWidth(t, &c) >> 1);
            break;
        case 0x0F:
            t->optX[t->nOptions] = x - t->glyphW;
            t->optY[t->nOptions] = y;
            t->optId[t->nOptions] = c.p[1] << 8 | c.p[2];
            t->nOptions++;
            c.p += 3;
            break;
        case 0x01:
            x = t->x - (t->w >> 1);
            y += (u8)Task_LineHeight(t);
            c.p += 1;
            break;
        case 0x00:
            if (c.depth != 0) {
                c.depth--;
                c.p = c.stack[c.depth];
            } else {
                done = 1;
            }
            break;
        default:
            n = (u8)TextCursor_Step(t, &c);
            if (n != 0) {
                if (!hidden) {
                    x += glyph_w(t, c.p);
                }
                c.p += n;
            }
            break;
        }
    } while (!done);
}

/* Width of the line at `src` (to its end, a new line, a page break or a choice). */
u16 Task_LineWidth(Task *t, TextCursor *src) {
    TextCursor c = *src;
    s32 hidden = 0, done = 0, n;
    u16 w = 0;

    do {
        u8 *p = c.p;

        switch (*p) {
        case 0x17:
            c.p = p + 1;
            hidden = 0;
            break;
        case 0x18:
            c.p = p + 1;
            hidden = 1;
            break;
        case 0x19: case 0x10: case 0x0E: case 0x02: case 0x01:
            done = 1;
            break;
        case 0x00:
            if (c.depth != 0) {
                c.depth--;
                c.p = c.stack[c.depth];
            } else {
                done = 1;
            }
            break;
        default:
            n = (u8)TextCursor_Step(t, &c);
            if (n != 0) {
                if (!hidden) {
                    w += glyph_w(t, c.p);
                }
                c.p += n;
            }
            break;
        }
    } while (!done);
    return w;
}

/* width of a line of `text` at glyph width `glyphW` */
u16 Text_LineWidth(Task *t, u8 *text, s32 glyphW) {
    TextCursor c;

    t->glyphW = glyphW;
    c.p = text;
    c.depth = 0;
    return Task_LineWidth(t, &c);
}

/* width of the first line of message `id` at glyph width `glyphW` */
u16 Task_MessageWidth(Task *t, s32 id, s32 glyphW) {
    TextCursor c;

    c.p = msg_text(id & 0xFFFF);
    t->glyphW = glyphW;
    c.depth = 0;
    return Task_LineWidth(t, &c);
}

/* Lay out the page at the typing position: the text's size (widest line, line heights). */
void Task_Layout(Task *t) {
    TextCursor c;
    s32 hidden = 0, done = 0, lineW = 0, w = 0, linesH = 0, h = 0, n;

    cursor_copy(&c, &t->cur);

    do {
        u8 *p = c.p;

        switch (*p) {
        case 0x17:
            c.p = p + 1;
            hidden = 0;
            break;
        case 0x18:
            c.p = p + 1;
            hidden = 1;
            break;
        case 0x0A:
            set_position(t, p[1]);
            c.p += 2;
            break;
        case 0x01:
            if (p[1] == 2) {
                lineW += t->glyphW;
            }
            if (w < lineW) {
                w = lineW;
            }
            lineW = 0;
            linesH += (u8)Task_LineHeight(t);
            c.p += 1;
            break;
        case 0x0B:
            w = p[1] * t->glyphW;
            h = c.p[2] * (u8)Task_LineHeight(t);
            done = 1;
            break;
        case 0x00:
            if (c.depth != 0) {
                c.depth--;
                c.p = c.stack[c.depth];
                break;
            }
            /* fall through */
        case 0x02: case 0x19: case 0x10:
            if (lineW != 0) {
                if (w < lineW) {
                    w = lineW;
                }
                linesH += (u8)Task_LineHeight(t);
            }
            if (h < linesH) {
                h = linesH;
            }
            done = 1;
            break;
        default:
            n = (u8)TextCursor_Step(t, &c);
            if (n != 0) {
                if (!hidden) {
                    lineW += glyph_w(t, c.p);
                }
                c.p += n;
            }
            break;
        }
    } while (!done);
    t->w = w;
    t->h = h;
}

/* Draw the page: the first `shown` steps of it, with the page arrow and the choice cursor.
 * Furigana (0x16 base 0x18 reading 0x17) is drawn small, centred above its base text. */
void Task_DrawPage(Task *t) {
    TextCursor c, r;
    s32 x, y, saveX = 0, saveY = 0, ruby = 0, baseW = 0, rubyW, n;
    u8 color;
    u16 i;

    c.p = t->page;
    c.depth = 0;
    y = t->y - (t->h >> 1);
    x = t->x - (t->w >> 1);
    color = t->color;
    y += 4;
    Task_BeginDraw(t);
    if (t->shown != 0) {
        i = 0;
        do {
            u8 *p = c.p;

            switch (*p) {
            case 0x18:
                cursor_copy(&r, &c);
                ruby = 1;
                saveX = x;
                saveY = y;
                rubyW = 0;
                while (*r.p != 0x17) {
                    n = (u8)TextCursor_Step(t, &r);
                    if (n != 0) {
                        rubyW += glyph_w(t, r.p) * 5 / 8;
                        r.p += n;
                    }
                }
                x = x - ((u16)baseW >> 1) - ((u16)rubyW >> 1);
                y -= 13;
                c.p += 1;
                break;
            case 0x17:
                y = saveY;
                ruby = 0;
                c.p = p + 1;
                x = saveX;
                break;
            case 0x16:
                baseW = 0;
                c.p += 1;
                break;
            case 0x12:
                c.p += 1;
                x = t->x - ((u16)Task_LineWidth(t, &c) >> 1);
                break;
            case 0x14:
                c.p += 1;
                x = t->x + (t->w >> 1);
                x -= (u16)Task_LineWidth(t, &c);
                break;
            case 0x04:
                color = t->baseColor & 0x80;
                c.p += 1;
                break;
            case 0x03:
                color = (t->baseColor & 0x80) | p[1];
                c.p += 2;
                break;
            case 0x02:
                if (t->mode != 3) {
                    s32 b = 32 - (t->frames & 0x3F);

                    b = b > 0 ? b * 2 : -b * 2;
                    Task_DrawGlyph(t, x, y + (b + 32) / 6, t->glyphW, t->glyphH, 8, D_0047B148);
                }
                c.p += 1;
                break;
            case 0x01:
                c.p = p + 1;
                if (p[1] != 2) {
                    x = t->x - (t->w >> 1);
                    y += (u8)Task_LineHeight(t);
                }
                break;
            default:
                n = (u8)TextCursor_Step(t, &c);
                if (n == 0) {
                    break;
                }
                if (!ruby) {
                    u8 *g = c.p;
                    s32 dy;

                    if (g[0] >= 0x1A && g[0] < 0x1D) {
                        dy = 0;
                    } else {
                        s32 idx = g[0] == 0x1D ? g[1] + 0xE0 : g[0] - 0x20;

                        dy = (D_0044AD00[idx] & 0xF) * t->glyphW / 16;
                    }
                    Task_DrawGlyph(t, x, y + dy, t->glyphW, t->glyphH, color, g);
                    x += glyph_w(t, c.p);
                } else {
                    Task_DrawGlyph(t, x, y, 10, 12, color, c.p);
                    x += glyph_w(t, c.p) * 5 / 8;
                }
                baseW = (u16)(baseW + glyph_w(t, c.p));
                c.p += n;
                break;
            }
            i++;
        } while (i != t->shown);
    }
    if (t->mode == 2) {
        Task_DrawGlyph(t, t->optX[t->answer] - 4, t->optY[t->answer] + 4, t->glyphW, t->glyphH, 9, D_0047B144);
    }
}

/* the state of a task with nothing to do */
void Task_StateIdle(Task *t) {
}

/* state: run the child task until it's done, then close */
void Task_StateChild(Task *t) {
    Task *c = t->child;

    c->frames++;
    ptmf_scall(c, &c->state);
    if (c->mode != 0 && c->mode != 4) {
        Task_DrawPage(c);
    }
    if (t->child->mode != 0) {
        return;
    }
    t->mode = 0;
    t->flags = 0;
    Task_SetState(t, Task_StateIdle);
    c = t->child;
    if (c != NULL) {
        if (c->child != NULL) {
            Task_dtor(c->child, 1);
            c->child = NULL;
        }
        func_00100490(c);
    }
    t->child = NULL;
}

/* state: choose an option. Confirm opens the message it leads to (or closes the box); cancel,
 * when allowed, picks the last option; up / down go through the options, left / right through
 * the ones on the same line. */
void Task_StateChoice(Task *t) {
    u32 b = D_0047E36C;
    u8 old;

    if (b & MENU_CONFIRM) {
        u16 id = t->optId[t->answer];

        if (id == 0xFFFF) {
            t->mode = 0;
            t->flags = 0;
            Task_SetState(t, Task_StateIdle);
        } else {
            s16 x = t->x, y = t->y;

            t->id = id;
            Task_OpenDefault(t);
            t->x = x;
            t->y = y;
        }
        Sound_PlaySE(SE_DECIDE);
        return;
    }
    if ((t->flags & 1) && (b & MENU_CANCEL)) {
        if (t->answer == t->nOptions - 1) {
            return;
        }
        t->answer = t->nOptions - 1;
        Sound_PlaySE(SE_CANCEL);
        return;
    }
    old = t->answer;
    if (b & (MENU_RIGHT | MENU_LEFT)) {
        u8 row[8];
        s32 n = 0, pos = 0, i;
        s16 rowY = t->optY[old];

        for (i = 0; i < t->nOptions; i++) {
            if (t->optY[i] == rowY) {
                if (i == t->answer) {
                    pos = n;
                }
                row[n++] = i;
            }
        }
        if (b & MENU_RIGHT) {
            if (n < 2) {
                t->answer++;
                if (t->answer >= t->nOptions) {
                    t->answer = 0;
                }
            } else if (pos + 1 == n) {
                t->answer = row[0];
            } else {
                t->answer = row[pos + 1];
            }
        } else if (n < 2) {
            if (t->answer != 0) {
                t->answer--;
            } else {
                t->answer = t->nOptions - 1;
            }
        } else if (pos == 0) {
            t->answer = row[n - 1];
        } else {
            t->answer = row[pos - 1];
        }
    } else if (b & MENU_UP) {
        if (old != 0) {
            t->answer--;
        } else {
            t->answer = t->nOptions - 1;
        }
    } else if (b & MENU_DOWN) {
        t->answer++;
        if (t->answer >= t->nOptions) {
            t->answer = 0;
        }
    }
    if (t->answer != old) {
        Sound_PlaySE(SE_CURSOR);
    }
}

#define ANY_BUTTON 0xFC01   /* the buttons that close a message */

/* state: close when a button is pressed */
void Task_StateWaitButton(Task *t) {
    if (D_0047E37C & ANY_BUTTON) {
        t->mode = 0;
        t->flags = 0;
        Task_SetState(t, Task_StateIdle);
    }
}

/* state: close after `wait` frames */
void Task_StateWaitFrames(Task *t) {
    if (--t->wait == 0) {
        t->mode = 0;
        t->flags = 0;
        Task_SetState(t, Task_StateIdle);
    }
}

/* state: next page when a button is pressed */
void Task_StateNextPage(Task *t) {
    if (D_0047E37C & ANY_BUTTON) {
        t->page = t->cur.p;
        t->shown = 0;
        t->color = t->baseColor;
        Task_Layout(t);
        Task_SetState(t, Task_StateType);
    }
}

/* state: the delay after a glyph (cancel skips it) */
void Task_StateGlyphDelay(Task *t) {
    if (D_0047E36C & MENU_CANCEL) {
        t->wait = 0;
        Task_StateType(t);
    } else if (--t->wait == 0) {
        Task_SetState(t, Task_StateType);
    }
}

/* state: a pause (code 0x09) */
void Task_StatePause(Task *t) {
    if (--t->wait == 0) {
        Task_SetState(t, Task_StateType);
    }
}

/* state: type the text out until a delay, a pause, a page break or the end (holding cancel
 * types everything at once). */
void Task_StateType(Task *t) {
    s32 stop = 0, hidden = 0, n;

    if (t->mode == 4) {
        t->mode = 1;
    }
    do {
        u8 *p = t->cur.p;

        switch (*p) {
        case 0x00:
            if (t->cur.depth != 0) {
                t->cur.depth--;
                t->cur.p = t->cur.stack[t->cur.depth];
                break;
            }
            if (t->mode == 2) {
                Task_SetState(t, Task_StateChoice);
            } else if (t->mode == 3) {
                Task_SetState(t, Task_StateIdle);
            } else {
                Task_SetState(t, Task_StateWaitButton);
            }
            stop = 1;
            break;
        case 0x02:
            t->cur.p += 1;
            if (t->mode == 3) {
                Task_SetState(t, Task_StateIdle);
            } else {
                Task_SetState(t, Task_StateNextPage);
            }
            stop = 1;
            break;
        case 0x03:
            t->baseColor = (t->baseColor & 0x80) | p[1];
            t->cur.p += 2;
            break;
        case 0x04:
            t->baseColor &= 0x80;
            t->cur.p += 1;
            break;
        case 0x09:
            t->wait = p[1];
            t->cur.p += 2;
            Task_SetState(t, Task_StatePause);
            stop = 1;
            break;
        case 0x0A:
            set_position(t, p[1]);
            t->cur.p += 2;
            break;
        case 0x0C:
            t->speed = p[1];
            t->cur.p += 2;
            break;
        case 0x0D:
            t->speed = 0;
            t->cur.p += 1;
            break;
        case 0x0E:
            Task_CollectChoices(t);
            t->mode = 2;
            t->flags |= (u8)((t->cur.p[1] & 3) ^ 1);
            t->speed = 0;
            if (t->flags & 2) {
                t->answer = t->nOptions - 1;
            } else {
                t->answer = 0;
            }
            t->cur.p += 3;
            break;
        case 0x10:
            t->wait = p[1];
            if (t->wait != 0) {
                Task_SetState(t, Task_StateWaitFrames);
            } else {
                Task_SetState(t, Task_StateIdle);
            }
            stop = 1;
            break;
        case 0x17:
            hidden = 0;
            t->cur.p += 1;
            break;
        case 0x18:
            hidden = 1;
            t->cur.p += 1;
            break;
        case 0x19:
            Task_SetState(t, Task_StateChild);
            t->cur.p += 3;
            stop = 1;
            break;
        default:
            n = (u8)TextCursor_Step(t, &t->cur);
            if (n != 0) {
                t->cur.p += n;
                if (!(D_0047E36C & MENU_CANCEL) && !hidden) {
                    t->wait = D_0047B140[t->speed];
                    if (t->wait != 0) {
                        Task_SetState(t, Task_StateGlyphDelay);
                        stop = 1;
                    }
                }
            }
            break;
        }
        t->shown++;
    } while (!stop);
}

/* Open message `id` (0xFFFF: the next page at the typing position) in the default box. */
void Task_OpenDefault(Task *t) {
    t->mode = 4;
    if (t->id == 0xFFFF) {
        t->page = t->cur.p;
    } else {
        u8 *text = msg_text(t->id);

        t->cur.p = text;
        t->page = text;
    }
    t->cur.depth = 0;
    t->glyphW = 16;
    t->glyphH = 21;
    t->x = 0x100;
    t->y = 0x172;
    t->flags = 4;
    Task_Layout(t);
    t->shown = 0;
    t->wait = 0;
    t->baseColor = 0;
    t->color = 0;
    t->alpha = 0x80;
    t->layer = 0x30;
    t->speed = 0;
    Task_SetState(t, Task_StateType);
}

/* close */
void Task_Close(Task *t) {
    delete_child(t);
    t->mode = 0;
    t->flags = 0;
    Task_SetState(t, Task_StateIdle);
}

/* Show message `id` at once (0x4000 / 0x2000: in language 2 / 1). */
void Task_ShowMessage(Task *t, s32 id, s32 color, s32 alpha, s32 layer) {
    u8 lang = D_0047B350;

    if (id & 0x4000) {
        D_0047B350 = 2;
    } else if (id & 0x2000) {
        D_0047B350 = 1;
    }
    Task_Open(t, id & 0x1FFF);
    t->baseColor = color;
    t->color = color;
    t->alpha = alpha;
    t->layer = layer;
    Task_StateType(t);
    Task_DrawPage(t);
    D_0047B350 = lang;
}

/* Show `text` at once with its top left at x, y (no frame). */
void Task_ShowText(Task *t, s32 x, s32 y, s32 color, u8 *text, s32 alpha, s32 layer, s32 glyphW, s32 glyphH) {
    t->mode = 1;
    t->cur.p = text;
    t->page = text;
    t->cur.depth = 0;
    t->glyphW = glyphW;
    t->glyphH = glyphH;
    Task_Layout(t);
    t->x = x + (t->w >> 1);
    t->y = y + (t->h >> 1) - 4;
    t->shown = 0;
    t->wait = 0;
    t->flags = 0;
    t->baseColor = color;
    t->color = color;
    t->alpha = alpha;
    t->layer = layer;
    t->speed = 0;
    Task_StateType(t);
    Task_DrawPage(t);
}

/* printf at x, y */
void Task_PrintfEx(Task *t, s32 x, s32 y, s32 color, s32 alpha, s32 layer, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    func_0026ED98(t->text, 0x80, fmt, ap);
    va_end(ap);
    Task_ShowText(t, x, y, color, (u8 *)t->text, alpha, layer, 0x10, 0x15);
}

/* printf at x, y (layer 0x30) */
void Task_Printf(Task *t, s32 x, s32 y, s32 color, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    func_0026ED98(t->text, 0x80, fmt, ap);
    va_end(ap);
    Task_ShowText(t, x, y, color, (u8 *)t->text, 0x80, 0x30, 0x10, 0x15);
}

/* show a message prepared with Task_Prepare, all at once */
void Task_ShowPrepared(Task *t) {
    if (t->flags & 0x80) {
        t->page = t->cur.p;
        t->shown = 0;
        t->color = t->baseColor;
        Task_Layout(t);
        t->mode = 3;
        Task_StateType(t);
    }
}

/* prepare message `id` without showing it (0xFFFF: close) */
void Task_Prepare(Task *t, s32 id) {
    u8 *text;

    if ((id & 0xFFFF) == 0xFFFF) {
        Task_Close(t);
        return;
    }
    t->id = id;
    text = msg_text(t->id);
    t->cur.p = text;
    t->page = text;
    t->cur.depth = 0;
    t->glyphW = 16;
    t->glyphH = 21;
    t->x = 0x100;
    t->y = 0x16B;
    t->wait = 0;
    t->flags = 0x80;
    t->baseColor = 0;
    t->color = 0;
    t->alpha = 0x80;
    t->layer = 0x30;
    t->speed = 0;
    t->mode = 0;
}

/* open message `id` at position preset `pos` */
void Task_OpenAt(Task *t, s32 id, s32 pos) {
    Task_Open(t, id);
    set_position(t, pos);
}

/* open message `id` */
void Task_Open(Task *t, s32 id) {
    t->id = id;
    delete_child(t);
    Task_OpenDefault(t);
}

/* the text of message `id` */
u8 *Task_MessageText(void *self, s32 id) {
    return msg_text(id & 0xFFFF);
}

/* draw (nothing while closed or opening) */
void Task_Draw(Task *t) {
    if (t->mode != 0 && t->mode != 4) {
        Task_DrawPage(t);
    }
}

/* update: count the frame, run the state */
void Task_Update(Task *t) {
    t->frames++;
    ptmf_scall(t, &t->state);
}

/* update and draw */
void Task_Run(Task *t) {
    t->frames++;
    ptmf_scall(t, &t->state);
    if (t->mode != 0 && t->mode != 4) {
        Task_DrawPage(t);
    }
}

/* constructor */
Task *Task_ctor(Task *t) {
    t->id = 0xFFFF;
    t->child = NULL;
    t->mode = 0;
    t->flags = 0;
    Task_SetState(t, Task_StateIdle);
    return t;
}
