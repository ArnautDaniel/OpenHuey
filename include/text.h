#ifndef TEXT_H
#define TEXT_H

#include "common.h"
#include "ptmf.h"

/*
 * Messages are byte code from per-language tables (gMessageTables[lang]: u16 offsets, then the
 * texts). Bytes >= 0x1E are glyphs of the small font; 0x1A..0x1C + a byte are glyphs of the
 * big fonts (texcache groups 0x14 + language), 0x1D + a byte a small-font glyph >= 0xE0.
 * Control codes (operand bytes after them):
 *   0x00 end (or back to the enclosing string)   0x01 new line      0x02 page break
 *   0x03 c colour       0x04 default colour       0x08 n parameter string n (gMessageParams)
 *   0x09 n wait n frames                          0x0A n position preset n (bit 7: no frame)
 *   0x0B w h fixed box size (w glyphs, h lines)   0x0C n speed       0x0D full speed
 *   0x0E f choice (options follow)                0x0F hi lo option leading to message hi:lo
 *   0x10 n close after n frames                   0x11 hi lo continue with message hi:lo
 *   0x12 centred line   0x13 n name n (gMessageNames)   0x14 right-aligned line
 *   0x15 hi lo insert system message 0x100 + hi:lo   0x16 / 0x18 / 0x17 furigana: base text,
 *   reading, end   0x19 a b wait for the child task
 */
typedef struct TextCursor {
    /* 0x00 */ u8 *p;
    /* 0x04 */ u8 depth;
    /* 0x08 */ u8 *stack[4];   /* where to continue after nested strings */
} TextCursor;

/* A message box / text drawer with its own state machine (0x104 bytes). */
typedef struct Task {
    /* 0x00 */ s32 fontSlot;      /* the small font's VRAM slot (texcache), -1 none */
    /* 0x04 */ PTMF state;        /* initially Task_StateIdle */
    /* 0x10 */ u8 mode;           /* 0 off, 1 typing, 2 choice, 3 shown at once, 4 opening */
    /* 0x11 */ u8 flags;          /* 1 cancel picks the last option, 2 last option first,
                                     4 frame, 0x80 prepared (Task_Prepare) */
    /* 0x12 */ u16 id;            /* message, 0xFFFF none */
    /* 0x14 */ u8 color;          /* colour at the page start */
    /* 0x15 */ u8 baseColor;
    /* 0x16 */ u8 alpha;
    /* 0x17 */ u8 layer;          /* renderer layer */
    /* 0x18 */ u8 speed;          /* index into the frames-per-glyph table Text_GlyphFrames */
    /* 0x19 */ u8 pad19;
    /* 0x1A */ s16 x;             /* box centre, screen */
    /* 0x1C */ s16 y;
    /* 0x1E */ u16 w;             /* text size */
    /* 0x20 */ u16 h;
    /* 0x22 */ u16 glyphW;
    /* 0x24 */ u16 glyphH;
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ u8 *page;          /* start of the page shown */
    /* 0x2C */ u16 shown;         /* steps of the page shown so far */
    /* 0x2E */ u8 wait;           /* frames */
    /* 0x2F */ u8 pad2F;
    /* 0x30 */ TextCursor cur;    /* the typing position */
    /* 0x48 */ u8 answer;         /* chosen option */
    /* 0x49 */ u8 nOptions;
    /* 0x4A */ s16 optX[8];
    /* 0x5A */ s16 optY[8];
    /* 0x6A */ u16 optId[8];      /* message an option leads to, 0xFFFF: close */
    /* 0x7A */ u8 pad7A[2];
    /* 0x7C */ struct Task *child; /* owned, deleted with the task */
    /* 0x80 */ s32 frames;
    /* 0x84 */ char text[0x80];   /* formatted text (Task_PrintfEx / Task_Printf) */
} Task;

_Static_assert(__builtin_offsetof(Task, cur) == 0x30, "Task.cur");
_Static_assert(__builtin_offsetof(Task, child) == 0x7C, "Task.child");
_Static_assert(sizeof(Task) == 0x104, "Task size");

extern const PTMF sTaskIdleState;   /* { 0, -1, Task_StateIdle } */

/* task.c */
extern Task *Task_dtor(Task *t, s32 flags);
extern void Msg_SetName(void *self, s32 slot, s32 id);
extern void Msg_SetParamSystem(void *self, s32 slot, s32 id);
extern void Msg_PrintfParam(void *self, s32 slot, const char *fmt, ...);
extern void Task_DrawBox(Task *t, s32 x, s32 y, s32 w, s32 h, s32 alpha, s32 layer);
extern u32 Text_LineWidth(Task *t, u8 *text, s32 glyphW);   /* (a u16, masked here as the original does) */
extern u16 Task_MessageWidth(Task *t, s32 id, s32 glyphW);
extern void Task_StateIdle(Task *t);
extern void Task_OpenDefault(Task *t);
extern void Task_Close(Task *t);
extern void Task_ShowMessage(Task *t, s32 id, s32 color, s32 alpha, s32 layer);
extern void Task_ShowText(Task *t, s32 x, s32 y, s32 color, u8 *text, s32 alpha, s32 layer, s32 glyphW, s32 glyphH);
extern void Task_PrintfEx(Task *t, s32 x, s32 y, s32 color, s32 alpha, s32 layer, const char *fmt, ...);
extern void Task_Printf(Task *t, s32 x, s32 y, s32 color, const char *fmt, ...);
extern void Task_ShowPrepared(Task *t);
extern void Task_Prepare(Task *t, s32 id);
extern void Task_OpenAt(Task *t, s32 id, s32 pos);
extern void Task_Open(Task *t, s32 id);
extern u8 *Task_MessageText(void *self, s32 id);
extern void Task_Draw(Task *t);
extern void Task_Update(Task *t);
extern void Task_Run(Task *t);
extern Task *Task_ctor(Task *t);   /* constructor */

/* text.c */
extern void *DimMessage_ctor(u8 *p);

/* the constructor (Task_ctor), as the original inlines it into the owners' */
static inline void Task_Construct(Task *t) {
    PTMF s = sTaskIdleState;

    t->id = 0xFFFF;
    t->child = NULL;
    t->mode = 0;
    t->flags = 0;
    if (ptmf_test(&s)) {
        t->state = s;
    }
}

/* ---- (was message.h) ---- */

/* message.c: what other files call. */

typedef struct VObject VObject;

/* message.c */
extern void Message_Init(u8 *m);
extern void Message_ClearAll(VObject *m);

#endif /* TEXT_H */
