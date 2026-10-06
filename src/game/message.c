/* On-screen messages (gBootMessage, vtable 0x46D7D0, SceneBoot's boot->msg...): 7 slots, each a
 * set of message textures (a .TEX file) shown one at a time on the renderer's layer 10.
 * Slot kinds (table D_003EA970): 1/2 = textures uploaded to VRAM (VRAM manager +0x5C),
 * 3 = a texture group of the slots object gTexCache. */
#include "common.h"
#include "game.h"


extern VObject *gRenderer;   /* the renderer */
extern VObject *gVram;   /* the VRAM manager */
extern VObject *gTexCache;   /* texture groups (Game +0x14E8C90) */
extern const u8 D_003EA970[]; /* per slot: kind, group, ? */
extern const u8 D_003EA971[];

typedef struct MsgSlot {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 index;
    /* 0x02 */ u8 cur;      /* texture shown (bit 7: hidden), 0xFF none */
    /* 0x04 */ u8 *data;
    /* 0x08 */ u32 *tex;    /* the .TEX file: count, then 16-byte entries from +0x10 */
    /* 0x0C */ u32 count;
    /* 0x10 */ s32 group;
    /* 0x14 */ s32 vram[4];
} MsgSlot;

#define MSG_SLOT(m, i) ((MsgSlot *)((u8 *)(m) + 4) + (u8)(i))
#define MSG_LAYER(m) AT(m, 0x104, s32)
#define MSG_LAST(m) AT(m, 0x100, u8)

static inline void Message_Reset(u8 *m) {
    s8 i;

    for (i = 0; i < 7; i++) {
        u8 *s = m + i * 0x24;

        AT(s, 0x18, s32) = -1;
        AT(s, 0x1C, s32) = -1;
        AT(s, 0x20, s32) = -1;
        AT(s, 0x24, s32) = -1;
        AT(s, 0x4, u8) = 0;
        AT(s, 0x6, u8) = 0xFF;
        AT(s, 0x8, s32) = 0;
        AT(s, 0x5, s8) = i;
    }
    AT(m, 0x104, s32) = VCALL(gRenderer, 0x3C, s32 (*)(VObject *, s32))(gRenderer, 10);
    AT(m, 0x100, u8) = 0xFF;
}

/* init */
void func_0026BCC0(u8 *m) {
    Message_Reset(m);
}

/* clear all slots (+0xC), then init */
void func_0026BC00(VObject *m) {
    s32 i;

    for (i = 0; i < 7; i++) {
        VCALL(m, 0xC, void (*)(VObject *, s32))(m, i & 0xFF);
    }
    Message_Reset((u8 *)m);
}

/* upload each texture of `tex` (VRAM manager +0x5C, placement `first`, else the other one) */
static inline s32 MsgSlot_Upload(MsgSlot *s, u32 *tex, s32 first) {
    VObject *v;
    u32 i;

    s->kind = 1;
    s->cur = 0xFF;
    s->data = NULL;
    s->tex = tex;
    s->count = *s->tex;
    s->group = -1;
    v = gVram;
    for (i = 0; i < s->count; i++) {
        u8 *e = (u8 *)s->tex + 0x10 + i * 0x10;

        s->vram[i] = VCALL(v, 0x5C, s32 (*)(VObject *, void *, s32))(v, e, first);
        if (s->vram[i] < 0) {
            s->vram[i] = VCALL(v, 0x5C, s32 (*)(VObject *, void *, s32))(v, e, !first);
        }
    }
    return 1;
}

s32 func_0026B690(MsgSlot *s, u32 *tex) { return MsgSlot_Upload(s, tex, 0); }
s32 func_0026B570(MsgSlot *s, u32 *tex) { return MsgSlot_Upload(s, tex, 1); }

#ifdef HG_NATIVE
#include <stdlib.h>
#include <string.h>

/* PC: the PS2 only needs the .TEX file until its textures are in VRAM, and the buffer it was
 * loaded into is then reused (a character's textures go bad once the next file loads). The PC
 * renderer decodes the textures from the file when it draws, so the slot keeps its own copy. */
static u32 *sTexCopy[64];

static u32 tex_file_size(const u32 *tex) {
    u32 n = tex[0], end = 0x10 + n * 0x10, i;

    for (i = 0; i < n; i++) {
        const u8 *e = (const u8 *)tex + 0x10 + i * 0x10;
        u32 e_end = 0x10 + i * 0x10 + AT(e, 0xC, u32) + (AT(e, 0x8, u16) + AT(e, 0xA, u16)) * 16;

        if (e_end > end) {
            end = e_end;
        }
    }
    return end;
}

static void tex_copy_free(MsgSlot *s) {
    s32 k;

    for (k = 0; k < 64; k++) {
        if (sTexCopy[k] != NULL && sTexCopy[k] == s->tex) {
            free(sTexCopy[k]);
            sTexCopy[k] = NULL;
            s->tex = NULL;
        }
    }
}

static u32 *tex_copy(MsgSlot *s, u32 *tex) {
    u32 size;
    u32 *c;
    s32 k;

    tex_copy_free(s);
    if (tex == NULL || tex[0] == 0 || tex[0] > 64) {
        return tex;
    }
    size = tex_file_size(tex);
    c = malloc(size);
    if (c == NULL) {
        return tex;
    }
    memcpy(c, tex, size);
    for (k = 0; k < 64; k++) {
        if (sTexCopy[k] == NULL) {
            sTexCopy[k] = c;
            return c;
        }
    }
    free(c);
    return tex;
}
#endif

/* +0x8 set slot `i`'s textures (a .TEX file); 1 if done */
s32 func_0026BB00(u8 *m, s32 i, u32 *tex) {
    MsgSlot *s = MSG_SLOT(m, i);

#ifdef HG_NATIVE
    tex = tex_copy(s, tex);
#endif

    s->kind = D_003EA970[(u8)i * 3];
    switch (s->kind) {
    case 1:
        return (u8)func_0026B690(s, tex);
    case 2:
        return (u8)func_0026B570(s, tex);
    case 3:
        s->kind = 3;
        s->cur = 0xFF;
        s->data = NULL;
        s->tex = tex;
        s->count = *s->tex;
        s->group = D_003EA971[s->index * 3];
        VCALL(gTexCache, 0x10, void (*)(VObject *, u32 *, s32))(gTexCache, tex, s->group);
        return 1;
    }
    return 0;
}

static inline void MsgSlot_Clear(MsgSlot *s) {
    s->vram[0] = -1;
    s->vram[1] = -1;
    s->vram[2] = -1;
    s->vram[3] = -1;
    s->kind = 0;
    s->cur = 0xFF;
    s->data = NULL;
}

/* +0xC release slot `i` */
void func_0026B9C0(u8 *m, s32 i) {
    MsgSlot *s = MSG_SLOT(m, i);
    VObject *v;
    u32 j;

#ifdef HG_NATIVE
    if (s->kind != 0) {
        tex_copy_free(s);
    }
#endif

    switch (s->kind) {
    case 1:
        v = gVram;
        for (j = 0; j < s->count; j++) {
            if (s->vram[j] != -1) {
                VCALL(v, 0x1C, void (*)(VObject *, s32))(v, s->vram[j]);
            }
        }
        MsgSlot_Clear(s);
        break;
    case 3:
        VCALL(gTexCache, 0x14, void (*)(VObject *, s32))(gTexCache, s->group);
        MsgSlot_Clear(s);
        break;
    }
}

/* +0x10 show texture `sel` of slot `i` with `data` (bit 7 of sel: hidden) */
s32 func_0026B950(u8 *m, s32 i, u8 *data, s32 sel) {
    MsgSlot *s = MSG_SLOT(m, i);

    if (s->kind != 0 && (u8)sel < s->count && !((u8)sel & 0x80)) {
        s->data = data;
        s->cur = sel;
        return 1;
    }
    return 0;
}

/* +0x14 show nothing */
void func_0026B4A0(u8 *m, s32 i) {
    MSG_SLOT(m, i)->cur = 0xFF;
    MSG_SLOT(m, i)->data = NULL;
}

/* +0x18 unhide */
void func_0026B4D0(u8 *m, s32 i) {
    MsgSlot *s = MSG_SLOT(m, i);

    if (s->data != NULL && s->cur != 0xFF) {
        s->cur &= 0x7F;
    }
}

/* +0x1C hide */
void func_0026B520(u8 *m, s32 i) {
    MsgSlot *s = MSG_SLOT(m, i);

    if (s->data != NULL && s->cur != 0xFF) {
        s->cur |= 0x80;
    }
}

/* +0x20 */
void func_001762A0(u8 *m) {
    MSG_LAST(m) = 0xFF;
}

/* +0x24 what to draw texture `sel` of slot `i` with: the layer (bit 31: slot changed since last
 * time) when it is the one shown, else its VRAM entry (kind 1) or texture group id (kind 3) */
s32 func_0026B860(u8 *m, s32 i, s32 sel, s32 unused) {
    MsgSlot *s = MSG_SLOT(m, i);
    s32 r;

    (void)unused;
    if (s->kind == 0) {
        return -1;
    }
    r = -1;
    if (s->data != NULL && !(s->cur & 0x80) && s->cur == sel) {
        r = MSG_LAYER(m);
        if (r != -1) {
            if (MSG_LAST(m) != (u8)i) {
                MSG_LAST(m) = i;
                r |= 0x80000000;
            }
            return r;
        }
    }
    switch (s->kind) {
    case 1:
        return s->vram[sel];
    case 3:
        return VCALL(gTexCache, 0x8, s32 (*)(VObject *, s32, s32, s32))(gTexCache, sel, s->group, s->cur);
    }
    return r;
}

/* +0x28 the texture entry for texture `sel` of slot `i` (the shown data, or the .TEX entry) */
u8 *func_0026B7B0(u8 *m, s32 i, u32 sel) {
    MsgSlot *s = MSG_SLOT(m, i);

    if (s->kind == 0) {
        return NULL;
    }
    if (s->data != NULL && !(s->cur & 0x80) && s->cur == sel && MSG_LAYER(m) != -1) {
        return s->data + 0x10;
    }
    if (s->kind == 3 && sel < s->count) {
        return (u8 *)s->tex + 0x10 + sel * 0x10;
    }
    return NULL;
}
