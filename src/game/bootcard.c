/* The boot memory card check (SceneBoot +0xC7440): look for the system data on the card in
 * slot 1, then slot 2; read it (it holds the options) or tell the player why it can't be.
 *
 * States: 0 / 1 / 2 check and read slot 1; 100 / 101 the same for slot 2; 200 a message is
 * up (an answer of 0 continues without the data, else checks again; replacing a card checks
 * again too); 300 loaded (a message for a few seconds); 400 done (-1 once the box is closed). */
#include "common.h"
#include "memcard.h"
#include "scene_boot.h"
#include "task.h"
#include "input.h"
#include "sound.h"
#include "gs.h"
#include "texcache.h"
#include "globals.h"
#include "ptmf.h"

#define AT32(p, off) AT(p, off, s32)

extern void func_00100490(void *p);   /* operator delete */
extern s32 D_0047B258[2];   /* check status per slot */
extern s32 D_0047B260;      /* slot 1's status; 9: its data couldn't be read */

static const char sFmtDec[] = "%d";
static const char sSlot1[] = "1";
static const char sSlot2[] = "2";
static const char sSlots12[] = "1,2";

/* messages */
#define MSG_CHECKING 0x10
#define MSG_NO_CARDS 0x11
#define MSG_NO_ROOM 0x12   /* in slot(s) %s */
#define MSG_NO_DATA 0x16
#define MSG_LOADED 0x17

extern u8 D_00463A50[];
void *func_0037E3F0(void);

static inline void sys_copy(SysData *d, const SysData *s) {
    s32 i;

    d->sum = s->sum;
    for (i = 0; i < 6; i++) {
        d->f[i] = s->f[i];
    }
    for (i = 0; i < 48; i++) {
        d->b[i] = s->b[i];
    }
}

/* the data read is valid: not flagged, and the byte sum matches */
static inline s32 sys_valid(SysData *s) {
    u8 *p = (u8 *)s;
    u32 sum = 0;
    s32 i;

    if (s->flags & 0x80) {
        return 0;
    }
    for (i = 4; i < 0x50; i++) {
        sum += p[i];
    }
    return s->sum == sum;
}

void BootCard_Check(BootCard *b) {
    MemCard *mc = gMemCard;
    s32 st;

    if (b->state < 0) {
        return;
    }
    switch (b->state) {
    case 0:
        sys_copy(&b->saved, b->sys);
        b->saved.flags = b->sys->flags;
        Msg_PrintfParam(&b->task, 1, sFmtDec, 0xC5);   /* the space needed, in KB */
        b->port = 0;
        D_0047B260 = -1;
        D_0047B258[1] = -1;
        D_0047B258[0] = -1;
        MEMCARD_CHECK(mc, b->port);
        Task_OpenAt(&b->task, MSG_CHECKING, 1);
        b->state++;
        /* fall through */
    case 1:
        st = mc->status;
        if (st < 0) {
            break;
        }
        D_0047B260 = st;
        D_0047B258[0] = st;
        if (st != MC_HAS_DATA) {
            b->port = 1;
            MEMCARD_CHECK(mc, b->port);
            b->state = 100;
            break;
        }
        MEMCARD_READ(mc, b->port, b->sys, 0, 0x50);
        b->state++;
        /* fall through */
    case 2:
        st = mc->status;
        if (st < 0) {
            break;
        }
        if (st != 0 || !sys_valid(b->sys)) {
            sys_copy(b->sys, &b->saved);
            D_0047B260 = 9;
            b->sys->flags = b->saved.flags;
            b->port = 1;
            MEMCARD_CHECK(mc, b->port);
            b->state = 100;
            goto slot2;
        }
        Msg_PrintfParam(&b->task, 0, sSlot1);
        Task_OpenAt(&b->task, MSG_LOADED, 1);
        b->timer = 0;
        b->state = 300;
        break;
    case 100:
    slot2:
        st = mc->status;
        if (st < 0) {
            break;
        }
        D_0047B258[1] = st;
        if (st == MC_HAS_DATA) {
            MEMCARD_READ(mc, b->port, b->sys, 0, 0x50);
            b->state++;
            goto slot2_read;
        }
        if (D_0047B260 == 9) {
            Msg_PrintfParam(&b->task, 0, sSlot1);
            Task_OpenAt(&b->task, MSG_NO_DATA, 1);
        } else if (st == MC_NO_CARD) {
            if (D_0047B260 == MC_NO_CARD) {
                Task_OpenAt(&b->task, MSG_NO_CARDS, 1);
            } else if (D_0047B260 == MC_NO_ROOM) {
                Msg_PrintfParam(&b->task, 0, sSlot1);
                Task_OpenAt(&b->task, MSG_NO_ROOM, 1);
            } else {
                b->state = 400;
                break;
            }
        } else if (st == MC_NO_ROOM) {
            if (D_0047B260 == MC_NO_CARD) {
                Msg_PrintfParam(&b->task, 0, sSlot2);
                Task_OpenAt(&b->task, MSG_NO_ROOM, 1);
            } else if (D_0047B260 == MC_NO_ROOM) {
                Msg_PrintfParam(&b->task, 0, sSlots12);
                Task_OpenAt(&b->task, MSG_NO_ROOM, 1);
            } else {
                b->state = 400;
                break;
            }
        } else {
            b->state = 400;
            break;
        }
        MEMCARD_CHECK(mc, b->port);
        b->state = 200;
        break;
    case 101:
    slot2_read:
        st = mc->status;
        if (st < 0) {
            break;
        }
        if (st == 0 && sys_valid(b->sys)) {
            Msg_PrintfParam(&b->task, 0, sSlot2);
            Task_OpenAt(&b->task, MSG_LOADED, 1);
            b->timer = 0;
            b->state = 300;
            break;
        }
        sys_copy(b->sys, &b->saved);
        b->sys->flags = b->saved.flags;
        Msg_PrintfParam(&b->task, 0, D_0047B260 == 9 ? sSlots12 : sSlot2);
        Task_OpenAt(&b->task, MSG_NO_DATA, 1);
        b->port = 0;
        MEMCARD_CHECK(mc, b->port);
        b->state = 200;
        goto asking;
    case 200:
    asking:
        if (b->task.mode == 0) {
            b->state = b->task.answer != 0 ? 0 : 400;
            break;
        }
        /* while the message is up, watch both slots for a card change */
        st = mc->status;
        if (st < 0) {
            break;
        }
        if (st != D_0047B258[b->port] || mc->error == 5) {
            b->state = 0;
            break;
        }
        b->port ^= 1;
        MEMCARD_CHECK(mc, b->port);
        break;
    case 300:
        b->timer++;
        if (b->task.mode != 0 && !(D_0047E37C & 8) && (u32)b->timer < 151) {
            break;
        }
        b->state = 400;
        Task_Close(&b->task);
        b->state = -1;
        break;
    default:
        Task_Close(&b->task);
        b->state = -1;
        break;
    }
    if (b->task.mode != 0) {
        Task_Run(&b->task);
    }
}

extern void *D_0046A058[];

/* constructor */
BootCard *BootCard_ctor(BootCard *b) {
    b->vtbl = D_0046A058;
    Task_Construct(&b->task);
    b->state = -1;
    return b;
}

void *func_0037E3F0(void) {
    return D_00463A50;
}

/* destructor */
BootCard *BootCard_dtor(BootCard *b, s32 flags) {
    if (b != NULL) {
        b->vtbl = D_0046A058;
        if (&b->task != NULL && b->task.child != NULL) {
            Task_dtor(b->task.child, 1);
            b->task.child = NULL;
        }
        if ((s16)flags > 0) {
            func_00100490(b);
        }
    }
    return b;
}

/* ---- the save-data screen (the sub screen's load page; mode 6) ---- */

extern void SaveScreen_DrawPart(BootCard *b, s32 part);
void SaveScreen_DrawList(BootCard *b);
void SaveScreen_MergeSystem(BootCard *b, SysData *cur, SysData *loaded);

#define MEMCARD_POLL(mc, port) VCALL(mc, 0x20, void (*)(MemCard *, s32))(mc, port)

/* the save headers after the system data: 12 x 0x18, then the saves (0x1900 each) */
#define SAVE_HEADER(sys, i) ((u8 *)(sys) + 0x50 + (i) * 0x18)
#define SAVE_DATA_OFF 0x170
#define SAVE_SIZE 0x1900

/* the save screen is set up on the system data, with two work buffers */
void SaveScreen_Init(BootCard *b, void *buf0, void *buf1) {
    b->sys = (SysData *)((u8 *)gSystemData + 0x20);
    b->buf0 = buf0;
    b->buf1 = buf1;
}

/* the save screen's parts (texture group 0x19): texture, CLUT (0x80: blend with the alpha
 * channel as is), u, v, w, h, x, y, screen w, h */
extern s16 D_00412770[][10];

#ifdef HG_NATIVE
#include "gl2d.h"

/* draw part `part` of the save screen (layer 0x30): its w x h texels at u, v shown sw x sh at
 * x, y, palette e[1] & 0x7F; bit 0x80 subtracts it by its alpha, else blended */
void SaveScreen_DrawPart(BootCard *b, s32 part) {
    s16 *e = D_00412770[(u8)part];
    u8 *tex;
    s32 slot = TexCache_Resident(e[0], 0x19, 0x30, &tex);

    if (slot == -1) {
        return;
    }
    gl2d_sprite(0x30, e[6], e[7], e[6] + e[8], e[7] + e[9], tex, e[2], e[3], e[2] + e[4], e[3] + e[5], 0x80808080,
                e[1] & 0x7F, gl2d_blend((0x80ULL << 32) | (e[1] & 0x80 ? 0x42 : 0x44)));
}
#endif

/* draw the screen: the frame, the slot tabs (the current one lit), the help line (`flags` 1:
 * choosing the card, else the saves) and, with `flags` 4, the save list */
static void savescreen_draw(BootCard *b, u8 flags) {
    if (b->hidden == 1) {
        return;
    }
    SaveScreen_DrawPart(b, 0);
    SaveScreen_DrawPart(b, 1);
    if (b->port == 0) {
        SaveScreen_DrawPart(b, 2);
    } else {
        SaveScreen_DrawPart(b, 3);
    }
    if (flags & 1) {
        SaveScreen_DrawPart(b, 5);
    } else {
        SaveScreen_DrawPart(b, 4);
    }
    if (flags & 4) {
        SaveScreen_DrawList(b);
    }
}

void SaveScreen_Draw(BootCard *b, s32 flags) {
    savescreen_draw(b, flags);
}

static const char sFmtDate[] = "%02X/%02X/20%02X %02X:%02X";

/* the date and time a save was made (BCD: month, day, year, hour, minute) */
void SaveScreen_DrawDate(BootCard *b, u8 *h) {
    Task t;

    Task_Construct(&t);
    Task_Printf(&t, 0xA0, 0x52, 0x80, sFmtDate, h[0x11], h[0x10], h[0x12], h[0xE], h[0xD]);
    if (t.child != NULL) {
        Task_dtor(t.child, 1);
        t.child = NULL;
    }
}

/* the system data was read from the card: keep what was unlocked in either, and the options
 * and controls set now */
void SaveScreen_MergeSystem(BootCard *b, SysData *cur, SysData *loaded) {
    s32 i, j;

    AT32(b->sys, 0x4) = AT32(cur, 0x4) | AT32(loaded, 0x4);
    AT32(b->sys, 0x8) = AT32(cur, 0x8) | AT32(loaded, 0x8);
    AT32(b->sys, 0xC) = AT32(cur, 0xC) | AT32(loaded, 0xC);
    for (i = 0x10; i < 0x17; i++) {
        AT(b->sys, i, u8) = AT(cur, i, u8);
    }
    AT(b->sys, 0x18, f32) = AT(cur, 0x18, f32);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 12; j++) {
            AT(b->sys, 0x1C + i * 12 + j, u8) = AT(cur, 0x1C + i * 12 + j, u8);
        }
    }
}

/* where each save was made: room, area -> its name's message (0xFFF ends) */
extern u16 D_00412800[][3];

static const char sFmtSaveNo[] = "%02d:";
static const char sFmtTime[] = "%02d:%02d:%02d";

/* one line of save-list text */
static void list_text(Task *t, s32 x, s32 y, s32 color, s32 id) {
    Task_ShowText(t, x, y, color, Task_MessageText(t, id), 0x80, 0x30, 0x10, 0x15);
}

/* the name of the place a save was made in */
static u16 save_place(u8 *h) {
    s32 room = AT32(h, 4);
    s32 area = (s8)h[9];
    u16 *e;

    for (e = D_00412800[0]; e[0] != 0xFFF; e += 3) {
        if (room == e[0] && area == e[1]) {
            return e[2];
        }
    }
    return 0x85;
}

/* draw the save list: two columns of 6 (number, place / empty / broken), and for the chosen
 * one its details (place, thumbnail, play time) */
void SaveScreen_DrawList(BootCard *b) {
    Task t;
    u32 i;

    Task_Construct(&t);
    for (i = 0; i < 12; i++) {
        u8 *h = SAVE_HEADER(b->sys, i);
        s32 color = b->cursor == (s32)i ? 0x82 : 0x80;
        s32 y = (s32)i % 6 * 24 + 0x99;
        s32 x = (s32)i / 6 * 0xDA;

        Task_Printf(&t, x + 0x30, y, color, sFmtSaveNo, i + 1);
        if (b->slots[i] == 2) {
            list_text(&t, x + 0x4E, y, color, 0x2B);
            if (b->cursor == (s32)i) {
                list_text(&t, 0xA0, 0x52, 0x80, 0x2C);
            }
        } else if (b->slots[i] == 1) {
            list_text(&t, x + 0x4E, y, color, 0x29);
            if (b->cursor == (s32)i) {
                list_text(&t, 0xA0, 0x52, 0x80, 0x2A);
            }
        } else {
            list_text(&t, x + 0x4E, y, color, save_place(h));
            if (b->cursor == (s32)i) {
                if (h[0xA] != 0) {
                    SaveScreen_DrawPart(b, 6);
                }
                list_text(&t, 0xA0, 0x3A, 0x80, save_place(h));
                SaveScreen_DrawDate(b, h);
                list_text(&t, 0xA0, 0x6A, 0x80, 0x28);
                Task_Printf(&t, 0xFF, 0x6A, 0x80, sFmtTime, h[0x13], h[0x14], h[0x15]);
            }
        }
    }
    if (t.child != NULL) {
        Task_dtor(t.child, 1);
        t.child = NULL;
    }
}

/* a save header is valid: the byte sum of 4..0x17 matches, not flagged */
static inline s32 header_valid(u8 *h) {
    u32 sum = 0;
    s32 i;

    for (i = 4; i < 0x18; i++) {
        sum += h[i];
    }
    return AT32(h, 0) == sum && !(h[8] & 0x80);
}

/* the save read is valid: the byte sum of 4..0x18FF matches */
static inline s32 save_valid(u8 *d) {
    u32 sum = 0;
    s32 i;

    for (i = 4; i < SAVE_SIZE; i++) {
        sum += d[i];
    }
    return AT32(d, 0) == sum;
}

/* Load a game: choose the card (left / right), read its system data and save headers, choose a
 * save (two columns of 6), read and check it.
 * States: 0 / 1 start (message 0x20); 2 choosing the card; 3 checking it; 4 reading the
 * system data; 5 choosing the save; 6 / 7 reading it; 100 / 101 a message, then back to 1;
 * 150 loaded (-2 once its message is closed); 200 a question; 300 cancelled (-1). */
void SaveScreen_Load(BootCard *b) {
    MemCard *mc = gMemCard;
    u8 flags = 1;
    s32 st;
    u32 pad;

    if (b->state < 0) {
        return;
    }
    switch (b->state) {
    case 0:
        b->port = 0;
        b->state++;
        /* fall through */
    case 1:
        Task_Open(&b->task, 0x20);
        b->cursor = 0;
        b->state++;
        break;
    case 2:
        if (D_0047E36C & MENU_CONFIRM) {
            MEMCARD_CHECK(mc, b->port);
            Msg_PrintfParam(&b->task, 0, sFmtDec, b->port + 1);
            Task_Open(&b->task, 0x13);
            b->state++;
            Sound_PlaySE(SE_DECIDE);
        } else if (D_0047E36C & MENU_CANCEL) {
            b->state = 300;
        } else {
            pad = D_0047E36C;
            if ((pad & MENU_LEFT) || (pad & MENU_PREV)) {
                if (b->port != 0) {
                    b->port = 0;
                    Sound_PlaySE(SE_CURSOR);
                }
            } else if ((pad & MENU_RIGHT) || (pad & MENU_NEXT)) {
                if (b->port != 1) {
                    b->port = 1;
                    Sound_PlaySE(SE_CURSOR);
                }
            }
        }
        flags |= 2;
        break;
    case 3:
        st = mc->status;
        if (st < 0) {
            break;
        }
        if (st == MC_NO_CARD) {
            Task_Open(&b->task, 0x14);
            b->state = 101;
        } else if (st != MC_HAS_DATA) {
            Task_Open(&b->task, 0x21);
            b->state = 101;
        } else {
            sys_copy(&b->saved, b->sys);
            b->saved.flags = b->sys->flags;
            MEMCARD_READ(mc, b->port, b->sys, 0, SAVE_DATA_OFF);
            b->state++;
        }
        break;
    case 4: {
        s32 i;

        st = mc->status;
        if (st < 0) {
            break;
        }
        if (st != 0 || !sys_valid(b->sys)) {
            sys_copy(b->sys, &b->saved);
            b->sys->flags = b->saved.flags;
        } else {
            SaveScreen_MergeSystem(b, &b->saved, b->sys);
            b->cursor = b->sys->flags;
        }
        st = mc->status;
        if (st != 0) {
            if (st == 9) {
                Task_Open(&b->task, 0x26);
            } else {
                Task_Open(&b->task, 0x25);
            }
            Sound_PlaySE(SE_BUZZER);
            b->state = 101;
            break;
        }
        for (i = 0; i < 12; i++) {
            u8 *h = SAVE_HEADER(b->sys, i);

            if (!header_valid(h)) {
                b->slots[i] = 2;
            } else if (AT32(h, 4) == -1) {
                b->slots[i] = 1;
            } else {
                b->slots[i] = 0;
            }
        }
        Task_Open(&b->task, 0x22);
        MEMCARD_POLL(mc, b->port);
        b->state++;
        break;
    }
    case 5:
        st = mc->status;
        if (st == 0) {
            MEMCARD_POLL(mc, b->port);
        } else if (st > 0) {
            Task_Open(&b->task, 0x15);
            b->state = 101;
            break;
        }
        pad = D_0047E36C;
        if (pad & MENU_CONFIRM) {
            if (b->slots[b->cursor] == 0) {
                Task_Open(&b->task, 0x23);
                b->state++;
                Sound_PlaySE(SE_DECIDE);
            } else {
                Sound_PlaySE(SE_BUZZER);
            }
        } else if (pad & MENU_CANCEL) {
            b->state = 1;
            Sound_PlaySE(SE_CANCEL);
        } else {
            u32 row = (u32)b->cursor % 6;
            u32 col = (u32)b->cursor / 6 * 6;

            pad = D_0047E36C;
            if (pad & MENU_UP) {
                b->cursor = (row != 0 ? row - 1 : 5) + col;
                Sound_PlaySE(SE_CURSOR);
            } else if (pad & MENU_DOWN) {
                b->cursor = (row + 1 < 6 ? row + 1 : 0) + col;
                Sound_PlaySE(SE_CURSOR);
            } else if ((pad & MENU_LEFT) || (pad & MENU_RIGHT)) {
                b->cursor = ((u32)b->cursor + 6) % 12;
                Sound_PlaySE(SE_CURSOR);
            }
        }
        flags |= 4;
        break;
    case 6:
        st = mc->status;
        if (st >= 0) {
            if (st == 0) {
                MEMCARD_READ(mc, b->port, (u8 *)b->sys + SAVE_DATA_OFF, b->cursor * SAVE_SIZE + SAVE_DATA_OFF,
                             SAVE_SIZE);
                b->state++;
            } else {
                Task_Open(&b->task, 0x25);
                Sound_PlaySE(SE_BUZZER);
                b->state = 100;
            }
        }
        flags |= 4;
        break;
    case 7:
        flags |= 4;
        st = mc->status;
        if (st < 0) {
            break;
        }
        if (st == 0) {
            if (save_valid((u8 *)b->sys + SAVE_DATA_OFF)) {
                Task_Open(&b->task, 0x24);
                Sound_PlaySE(SE_LOADED);
                b->state = 150;
                break;
            }
            mc->status = 9;
        }
        if (mc->status == 9) {
            Task_Open(&b->task, 0x26);
        } else {
            Task_Open(&b->task, 0x25);
        }
        Sound_PlaySE(SE_BUZZER);
        b->state = 100;
        break;
    case 100:
        flags |= 4;
        /* fall through */
    case 101:
        if (!b->task.mode) {
            b->state = 1;
        }
        break;
    case 150:
        if (b->task.mode == 0) {
            Task_Close(&b->task);
            b->state = -2;
        }
        flags |= 4;
        break;
    case 200:
        if (b->task.mode) {
            break;
        }
        b->state = b->task.answer == 0 ? 300 : 1;
        break;
    default:
        Task_Close(&b->task);
        b->state = -1;
        break;
    }
    savescreen_draw(b, flags);
    if (b->task.mode) {
        Task_Run(&b->task);
    }
}

/* ---- saving (the sub screen's save page, mode 1) ---- */

#include "progress.h"

extern void func_002A76E0(u8 *p);         /* four bytes cleared */
extern void func_002A8060(u8 *p);         /* a fresh save's progress */
extern u32 D_0047ABF8;                    /* written over a header's sum while its save is written */
extern s32 D_0047B264;                    /* the last check's status */
extern s32 D_0047B268;                    /* the empty saves written so far */
extern const char D_0045D210[];           /* "%d" */
extern const char D_0045D220[], D_0045D230[];   /* "SUBSCR\\ICON.SYS", "SUBSCR\\ICON00.ICO" */
extern const char D_0045D248[], D_0045D258[];   /* the card's "icon.sys", "icon00.ico" */

#define MEMCARD_CREATE(mc, port, name, buf, size) \
    VCALL(mc, 0x10, void (*)(MemCard *, s32, const char *, void *, s32))(mc, port, name, buf, size)
#define MEMCARD_WRITE(mc, port, buf, off, size) \
    VCALL(mc, 0x14, void (*)(MemCard *, s32, void *, s32, s32))(mc, port, buf, off, size)
#define MEMCARD_FORMAT(mc, port) VCALL(mc, 0x1C, void (*)(MemCard *, s32))(mc, port)

/* the byte sum of `p`[4 .. n) */
static inline u32 bytes_sum(const u8 *p, s32 n) {
    u32 sum = 0;
    s32 i;

    for (i = 4; i < n; i++) {
        sum += p[i];
    }
    return sum;
}

/* the system data's sum made right */
static inline void sys_resum(SysData *s) {
    s->sum = bytes_sum((u8 *)s, 0x50);
}

/* each save's state from its header: 0 used, 1 empty, 2 broken */
static inline void slots_scan(BootCard *b) {
    s32 i;

    for (i = 0; i < 12; i++) {
        u8 *h = SAVE_HEADER(b->sys, i);

        if (!header_valid(h)) {
            b->slots[i] = 2;
        } else if (AT32(h, 4) == -1) {
            b->slots[i] = 1;
        } else {
            b->slots[i] = 0;
        }
    }
}

/* the system data as read kept (restored if what is read is bad) */
static inline void sys_keep(BootCard *b) {
    sys_copy(&b->saved, b->sys);
    b->saved.flags = b->sys->flags;
}

/* the game saved into save `cursor`: its header marked, the progress written (progress +0x70)
 * and its place kept in the system data (and system flag 0x100000 set); the sums of the system
 * data, the header and the save made right */
void func_002BD6A0(BootCard *b) {
    u8 *h;

    SAVE_HEADER(b->sys, b->cursor)[8] = 0;
    VCALL(gProgress, 0x70, void (*)(Progress *, u8))(gProgress, b->cursor);
    b->sys->flags = b->cursor;
    AT(gSystemData, 0x2C, u32) |= 0x100000;
    sys_resum(b->sys);
    h = SAVE_HEADER(b->sys, b->cursor);
    AT32(h, 0) = bytes_sum(h, 0x18);
    h = (u8 *)b->sys + SAVE_DATA_OFF;
    AT32(h, 0) = bytes_sum(h, SAVE_SIZE);
}

/* fresh game data: no last save; 12 empty headers; an empty save area (a new game's progress,
 * its sum left -1) */
void func_002BD8C0(BootCard *b) {
    u8 *d;
    s32 i;

    b->sys->flags = 0;
    sys_resum(b->sys);
    for (i = 0; i < 12; i++) {
        u8 *h = SAVE_HEADER(b->sys, i);

        AT32(h, 4) = -1;
        h[8] = 0;
        h[9] = 0xFF;
        h[0xA] = 0;
        h[0xB] = 0xFF;
        func_002A76E0(h + 0x13);
        AT32(h, 0) = bytes_sum(h, 0x18);
    }
    d = (u8 *)b->sys + SAVE_DATA_OFF;
    for (i = 0; i < 5; i++) {
        AT32(d, 4 + i * 4) = -1;
    }
    d[0x18F0] = 0;
    for (i = 0; i < 6; i++) {
        d[0x18 + i] = 0xFF;
        d[0x1E + i] = 0;
    }
    for (i = 0; i < 8; i++) {
        AT32(d, 0x30 + i * 4) = 0;
    }
    func_002A8060(d + 0x50);
    AT32(d, 0) = -1;
}

/* Save the game. Messages open at the screen's place (`hidden`: 1 for a save without the list -
 * only the system data is written).
 * States: 0 the icons loaded; 1 start ("hidden": which card, 50); 2 choosing the card; 3
 * checking it (no data: 4, "create it?"; unformatted: 5, "format?" then 6 / 7 formatting); 8
 * the 12 empty saves written, then the icon (9) and icon.sys (10); 11 the system data read; 12
 * choosing the save (13: "overwrite?"); 14..17 written in turn: the system data, the save's
 * header sum spoiled (D_0047ABF8), the save, its header; 18 done; 50 "which card"; 100 / 101 a message, then back to 1; 150
 * saved; 200 "quit?"; 300 cancelled / finished (-1) */
void func_002BDAB0(BootCard *b) {
    MemCard *mc = gMemCard;
    u8 flags = 0;
    s32 st;
    u32 pad;

    if (b->state < 0) {
        return;
    }
    switch (b->state) {
    case 0: {
        VObject *ld = gFileLoader;

        AT(gSystemData, 0xC, u8) = 0;
        b->port = 0;
        VCALL(ld, 0x34, void (*)(VObject *, const char *, void *))(ld, D_0045D220, b->buf0);
        VCALL(ld, 0x34, void (*)(VObject *, const char *, void *))(ld, D_0045D230, b->buf1);
        b->state++;
    }
        /* fall through */
    case 1:
        b->cursor = 0;
        D_0047B268 = 0;
        Msg_PrintfParam(&b->task, 1, D_0045D210, 0xC5);
        if (b->hidden != 0) {
            Task_OpenAt(&b->task, 0x3E, (u8)b->hidden);
            b->state = 50;
        } else {
            Task_Open(&b->task, 0x30);
            b->state++;
        }
        break;
    case 2:
        if (D_0047E36C & MENU_CONFIRM) {
            MEMCARD_CHECK(mc, b->port);
            Msg_PrintfParam(&b->task, 0, D_0045D210, b->port + 1);
            Task_OpenAt(&b->task, 0x13, (u8)b->hidden);
            b->state++;
            Sound_PlaySE(SE_DECIDE);
        } else if (D_0047E36C & MENU_CANCEL) {
            Task_OpenAt(&b->task, 0x3D, (u8)b->hidden);
            b->state = 200;
            Sound_PlaySE(SE_CANCEL);
        } else {
            pad = D_0047E36C;
            if ((pad & MENU_LEFT) || (pad & MENU_PREV)) {
                if (b->port != 0) {
                    b->port = 0;
                    Sound_PlaySE(SE_CURSOR);
                }
            } else if ((pad & MENU_RIGHT) || (pad & MENU_NEXT)) {
                if (b->port != 1) {
                    b->port = 1;
                    Sound_PlaySE(SE_CURSOR);
                }
            }
        }
        flags |= 2;
        break;
    case 3:
        st = mc->status;
        if (st < 0) {
            break;
        }
        D_0047B264 = st;
        if (st == MC_NO_CARD) {
            Task_OpenAt(&b->task, 0x14, (u8)b->hidden);
            b->state = 101;
        } else if (st == MC_NO_ROOM) {
            Task_OpenAt(&b->task, 0x31, (u8)b->hidden);
            b->state = 101;
        } else if (st != MC_HAS_DATA) {
            Task_OpenAt(&b->task, 0x32, (u8)b->hidden);
            MEMCARD_POLL(mc, b->port);
            b->state++;
        } else {
            sys_keep(b);
            MEMCARD_READ(mc, b->port, b->sys, 0, b->hidden == 0 ? SAVE_DATA_OFF : 0x50);
            b->state = 11;
        }
        break;
    case 4:
        st = mc->status;
        if (st < 0) {
            break;
        }
        if (b->task.mode == 0) {
            b->cursor = b->task.answer;
            if (b->cursor == 1) {
                b->state = 1;
            } else if (mc->status != 0) {
                Task_OpenAt(&b->task, 0x34, (u8)b->hidden);
                Sound_PlaySE(SE_BUZZER);
                b->state = 101;
            } else {
                func_002BD8C0(b);
                if (D_0047B264 == MC_UNFORMATTED) {
                    Task_OpenAt(&b->task, 0x35, (u8)b->hidden);
                    MEMCARD_POLL(mc, b->port);
                    b->state++;
                } else {
                    Task_OpenAt(&b->task, 0x33, (u8)b->hidden);
                    MEMCARD_CREATE(mc, b->port, func_0037E3F0(), b->sys, SAVE_DATA_OFF);
                    b->state = 8;
                }
            }
        } else if (st != 0) {
            Task_OpenAt(&b->task, 0x15, (u8)b->hidden);
            b->state = 101;
        } else {
            MEMCARD_POLL(mc, b->port);
        }
        break;
    case 5:
        st = mc->status;
        if (st < 0) {
            break;
        }
        if (b->task.mode == 0) {
            b->cursor = b->task.answer;
            if (b->cursor == 1) {
                b->state = 1;
            } else if (mc->status != 0) {
                Task_OpenAt(&b->task, 0x37, (u8)b->hidden);
                Sound_PlaySE(SE_BUZZER);
                b->state = 101;
            } else {
                Task_OpenAt(&b->task, 0x36, (u8)b->hidden);
                MEMCARD_CHECK(mc, b->port);
                b->state++;
            }
        } else if (st != 0) {
            Task_OpenAt(&b->task, 0x15, (u8)b->hidden);
            b->state = 101;
        } else {
            MEMCARD_POLL(mc, b->port);
        }
        break;
    case 6:
        st = mc->status;
        if (st < 0) {
            break;
        }
        if (st == MC_UNFORMATTED) {
            MEMCARD_FORMAT(mc, b->port);
            b->state++;
        } else if (st == MC_NO_CARD) {
            Task_OpenAt(&b->task, 0x14, (u8)b->hidden);
            b->state = 101;
        } else {
            Task_OpenAt(&b->task, 0x15, (u8)b->hidden);
            b->state = 101;
        }
        break;
    case 7:
        st = mc->status;
        if (st < 0) {
            break;
        }
        if (st == 0) {
            Task_OpenAt(&b->task, 0x33, (u8)b->hidden);
            MEMCARD_CREATE(mc, b->port, func_0037E3F0(), b->sys, SAVE_DATA_OFF);
            b->state++;
        } else {
            Task_OpenAt(&b->task, 0x37, (u8)b->hidden);
            Sound_PlaySE(SE_BUZZER);
            b->state = 101;
        }
        break;
    case 8:
        st = mc->status;
        if (st < 0) {
            break;
        }
        if (st != 0) {
            Task_OpenAt(&b->task, 0x34, (u8)b->hidden);
            Sound_PlaySE(SE_BUZZER);
            b->state = 101;
        } else if ((u32)D_0047B268 < 12) {
            MEMCARD_WRITE(mc, b->port, (u8 *)b->sys + SAVE_DATA_OFF, D_0047B268 * SAVE_SIZE + SAVE_DATA_OFF,
                          SAVE_SIZE);
            D_0047B268++;
        } else {
            MEMCARD_CREATE(mc, b->port, D_0045D258, b->buf1, 0x1CF58);
            b->state++;
        }
        break;
    case 9:
        st = mc->status;
        if (st < 0) {
            break;
        }
        if (st == 0) {
            MEMCARD_CREATE(mc, b->port, D_0045D248, b->buf0, 0x3C4);
            b->state++;
        } else {
            Task_OpenAt(&b->task, 0x34, (u8)b->hidden);
            Sound_PlaySE(SE_BUZZER);
            b->state = 101;
        }
        break;
    case 10:
        st = mc->status;
        if (st < 0) {
            break;
        }
        if (st != 0) {
            Task_OpenAt(&b->task, 0x34, (u8)b->hidden);
            Sound_PlaySE(SE_BUZZER);
            b->state = 101;
        } else if (b->hidden == 0) {
            sys_keep(b);
            MEMCARD_READ(mc, b->port, b->sys, 0, SAVE_DATA_OFF);
            b->state++;
        } else {
            Task_OpenAt(&b->task, 0x3A, (u8)b->hidden);
            Sound_Play(gSound, 0xD, SE_BANK_MENU);
            b->state = 150;
        }
        break;
    case 11:
        st = mc->status;
        if (st < 0) {
            break;
        }
        if (st != 0 || !sys_valid(b->sys)) {
            sys_copy(b->sys, &b->saved);
            b->sys->flags = b->saved.flags;
        } else {
            SaveScreen_MergeSystem(b, &b->saved, b->sys);
            b->cursor = b->sys->flags;
        }
        if (b->hidden != 0) {
            sys_resum(b->sys);
            Task_OpenAt(&b->task, 0x33, (u8)b->hidden);
            MEMCARD_WRITE(mc, b->port, b->sys, 0, 0x50);
            b->state = 18;
            break;
        }
        if (mc->status != 0) {
            Task_OpenAt(&b->task, 0x15, (u8)b->hidden);
            Sound_PlaySE(SE_BUZZER);
            b->state = 101;
            break;
        }
        slots_scan(b);
        Task_OpenAt(&b->task, 0x38, (u8)b->hidden);
        MEMCARD_POLL(mc, b->port);
        b->state++;
        /* fall through */
    case 12:
        st = mc->status;
        if (st == 0) {
            MEMCARD_POLL(mc, b->port);
        } else if (st > 0) {
            Task_OpenAt(&b->task, 0x15, (u8)b->hidden);
            b->state = 101;
            break;
        }
        pad = D_0047E36C;
        if (pad & MENU_CONFIRM) {
            if (b->slots[b->cursor] == 0) {
                Task_OpenAt(&b->task, 0x3C, (u8)b->hidden);
                MEMCARD_POLL(mc, b->port);
                b->state++;
            } else {
                Task_OpenAt(&b->task, 0x33, (u8)b->hidden);
                func_002BD6A0(b);
                slots_scan(b);
                b->state = 14;
            }
            Sound_PlaySE(SE_DECIDE);
        } else if (pad & MENU_CANCEL) {
            b->state = 1;
            Sound_PlaySE(SE_CANCEL);
        } else {
            u32 row = (u32)b->cursor % 6;
            u32 col = (u32)b->cursor / 6 * 6;

            pad = D_0047E36C;
            if (pad & MENU_UP) {
                b->cursor = (row != 0 ? row - 1 : 5) + col;
                Sound_PlaySE(SE_CURSOR);
            } else if (pad & MENU_DOWN) {
                b->cursor = (row + 1 < 6 ? row + 1 : 0) + col;
                Sound_PlaySE(SE_CURSOR);
            } else if ((pad & MENU_LEFT) || (pad & MENU_RIGHT)) {
                b->cursor = ((u32)b->cursor + 6) % 12;
                Sound_PlaySE(SE_CURSOR);
            }
        }
        flags |= 4;
        break;
    case 13:
        st = mc->status;
        if (st >= 0) {
            if (b->task.mode) {
                if (st != 0) {
                    Task_OpenAt(&b->task, 0x15, (u8)b->hidden);
                    b->state = 101;
                } else {
                    MEMCARD_POLL(mc, b->port);
                }
            } else if (b->task.answer == 1) {
                Task_OpenAt(&b->task, 0x38, (u8)b->hidden);
                MEMCARD_POLL(mc, b->port);
                b->state = 12;
            } else if (st != 0) {
                Task_OpenAt(&b->task, 0x3B, (u8)b->hidden);
                Sound_PlaySE(SE_BUZZER);
                b->state = 100;
            } else {
                Task_OpenAt(&b->task, 0x33, (u8)b->hidden);
                func_002BD6A0(b);
                slots_scan(b);
                b->state++;
            }
        }
        flags |= 4;
        break;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        flags |= 4;
        st = mc->status;
        if (st < 0) {
            break;
        }
        if (st != 0) {
            Task_OpenAt(&b->task, 0x3B, (u8)b->hidden);
            Sound_PlaySE(SE_BUZZER);
            b->state = 100;
            break;
        }
        switch (b->state) {
        case 14:
            MEMCARD_WRITE(mc, b->port, b->sys, 0, 0x50);
            break;
        case 15:
            MEMCARD_WRITE(mc, b->port, &D_0047ABF8, b->cursor * 0x18 + 0x50, 4);
            break;
        case 16:
            MEMCARD_WRITE(mc, b->port, (u8 *)b->sys + SAVE_DATA_OFF, b->cursor * SAVE_SIZE + SAVE_DATA_OFF,
                          SAVE_SIZE);
            break;
        case 17:
            MEMCARD_WRITE(mc, b->port, SAVE_HEADER(b->sys, b->cursor), b->cursor * 0x18 + 0x50, 0x18);
            break;
        case 18:
            Task_OpenAt(&b->task, 0x3A, (u8)b->hidden);
            Sound_Play(gSound, 0xD, SE_BANK_MENU);
            b->state = 150;
            goto done;
        }
        b->state++;
        break;
    case 50:
        if (b->task.mode) {
            break;
        }
        b->port = b->task.answer;
        if (b->port == 2) {
            Task_OpenAt(&b->task, 0x3F, (u8)b->hidden);
            b->state = 200;
            break;
        }
        MEMCARD_CHECK(mc, b->port);
        Msg_PrintfParam(&b->task, 0, D_0045D210, b->port + 1);
        Task_OpenAt(&b->task, 0x13, (u8)b->hidden);
        b->state = 3;
        break;
    case 100:
        flags |= 4;
        /* fall through */
    case 101:
        if (!b->task.mode) {
            b->state = 1;
        }
        break;
    case 150:
        if (b->task.mode == 0) {
            b->state = 300;
        }
        flags |= 4;
        break;
    case 200:
        if (b->task.mode) {
            break;
        }
        b->state = b->task.answer == 0 ? 300 : 1;
        break;
    default:
        AT(gSystemData, 0xC, u8) = 1;
        Task_Close(&b->task);
        b->state = -1;
        break;
    }
done:
    savescreen_draw(b, flags);
    if (b->task.mode) {
        Task_Run(&b->task);
    }
}
