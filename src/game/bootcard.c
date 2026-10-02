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

#define AT32(p, off) AT(p, off, s32)

extern MemCard *D_0044FF00;
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

void func_002BF2F0(BootCard *b) {
    MemCard *mc = D_0044FF00;
    s32 st;

    if (b->state < 0) {
        return;
    }
    switch (b->state) {
    case 0:
        sys_copy(&b->saved, b->sys);
        b->saved.flags = b->sys->flags;
        func_00380B80(&b->task, 1, sFmtDec, 0xC5);   /* the space needed, in KB */
        b->port = 0;
        D_0047B260 = -1;
        D_0047B258[1] = -1;
        D_0047B258[0] = -1;
        MEMCARD_CHECK(mc, b->port);
        func_00384A00(&b->task, MSG_CHECKING, 1);
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
        func_00380B80(&b->task, 0, sSlot1);
        func_00384A00(&b->task, MSG_LOADED, 1);
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
            func_00380B80(&b->task, 0, sSlot1);
            func_00384A00(&b->task, MSG_NO_DATA, 1);
        } else if (st == MC_NO_CARD) {
            if (D_0047B260 == MC_NO_CARD) {
                func_00384A00(&b->task, MSG_NO_CARDS, 1);
            } else if (D_0047B260 == MC_NO_ROOM) {
                func_00380B80(&b->task, 0, sSlot1);
                func_00384A00(&b->task, MSG_NO_ROOM, 1);
            } else {
                b->state = 400;
                break;
            }
        } else if (st == MC_NO_ROOM) {
            if (D_0047B260 == MC_NO_CARD) {
                func_00380B80(&b->task, 0, sSlot2);
                func_00384A00(&b->task, MSG_NO_ROOM, 1);
            } else if (D_0047B260 == MC_NO_ROOM) {
                func_00380B80(&b->task, 0, sSlots12);
                func_00384A00(&b->task, MSG_NO_ROOM, 1);
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
            func_00380B80(&b->task, 0, sSlot2);
            func_00384A00(&b->task, MSG_LOADED, 1);
            b->timer = 0;
            b->state = 300;
            break;
        }
        sys_copy(b->sys, &b->saved);
        b->sys->flags = b->saved.flags;
        func_00380B80(&b->task, 0, D_0047B260 == 9 ? sSlots12 : sSlot2);
        func_00384A00(&b->task, MSG_NO_DATA, 1);
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
        func_003844E0(&b->task);
        b->state = -1;
        break;
    default:
        func_003844E0(&b->task);
        b->state = -1;
        break;
    }
    if (b->task.mode != 0) {
        func_00384BC0(&b->task);
    }
}

extern void *D_0046A058[];

/* constructor */
BootCard *func_002D0300(BootCard *b) {
    b->vtbl = D_0046A058;
    Task_Construct(&b->task);
    b->state = -1;
    return b;
}

/* destructor */
BootCard *func_0012C730(BootCard *b, s32 flags) {
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

extern void *D_0044E978;    /* the system data */
extern void func_002BC040(BootCard *b, s32 part);
void func_002BC530(BootCard *b);
void func_002BC9C0(BootCard *b, SysData *cur, SysData *loaded);

#define MEMCARD_POLL(mc, port) VCALL(mc, 0x20, void (*)(MemCard *, s32))(mc, port)

/* the save headers after the system data: 12 x 0x18, then the saves (0x1900 each) */
#define SAVE_HEADER(sys, i) ((u8 *)(sys) + 0x50 + (i) * 0x18)
#define SAVE_DATA_OFF 0x170
#define SAVE_SIZE 0x1900

/* the save screen is set up on the system data, with two work buffers */
void func_002BFB00(BootCard *b, void *buf0, void *buf1) {
    b->sys = (SysData *)((u8 *)D_0044E978 + 0x20);
    b->buf0 = buf0;
    b->buf1 = buf1;
}

extern VObject *D_0044E9A0;   /* the VRAM manager */

/* the save screen's parts (texture group 0x19): texture, CLUT (0x80: blend with the alpha
 * channel as is), u, v, w, h, x, y, screen w, h */
extern s16 D_00412770[][10];

/* draw part `part` of the save screen (layer 0x30) */
void func_002BC040(BootCard *b, s32 part) {
    s16 *e = D_00412770[(u8)part];
    u8 *tex;
    s32 slot = TexCache_Resident(e[0], 0x19, 0x30, &tex);
    s32 u, v, w, h, x, y, sw, sh;
    u64 *p;

    if (slot == -1) {
        return;
    }
    p = VCALL(D_0044E4F0, 0x10, u64 *(*)(VObject *, s32, s32))(D_0044E4F0, 0xC, 0x30);
    if (p == NULL) {
        return;
    }
    p[0] = 0x1000000B;              /* DMA cnt 11 */
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x5000000B;   /* VIF DIRECT 11 */
    p[2] = 5 | (1ULL << 60);        /* GIF tag: 5 A+D, EOP */
    p[3] = 0xE;
    if (e[1] & 0x80) {
        p[4] = (0x80ULL << 32) | 0x42;   /* ALPHA_1: (Cs - Cd) * As + Cd, ... */
    } else {
        p[4] = (0x80ULL << 32) | 0x44;
    }
    p[5] = 0x42;
    p[6] = VCALL(D_0044E9A0, 0x2C, u64 (*)(VObject *, s32, s32, s32, s32))(
        D_0044E9A0, slot, AT(tex, 4, u16), AT(tex, 6, u16), tex[1]);   /* TEX0_1: loads the CLUT */
    p[7] = 0x6;
    p[8] = 0x60;
    p[9] = 0x14;
    p[10] = (0x80ULL << 32) | 0x8080;  /* TEXA */
    p[11] = 0x3B;
    p[12] = 0x156;                  /* PRIM: sprite, textured, blended, UV */
    p[13] = 0;
    u = e[2];
    v = e[3];
    w = e[4];
    h = e[5];
    x = e[6];
    y = e[7];
    sw = e[8];
    sh = e[9];
    p[14] = 0x8001 | (0x84ULL << 56);   /* reglist: TEX0 CLAMP RGBAQ UV XYZ2 UV XYZ2 NOP */
    p[15] = GIF_REGS_TEX_SPRITE;
    p[16] = VCALL(D_0044E9A0, 0x30, u64 (*)(VObject *, s32, s32, s32, s32, s32, s32))(
        D_0044E9A0, slot, e[1] & 0x7F, tex[0], AT(tex, 4, u16), AT(tex, 6, u16), tex[1]);
    p[17] = gs_clamp_region(u, v, w, h);
    p[18] = 0x80808080 | (1ULL << 32);
    p[19] = gs_uv(u, v);
    p[20] = gs_xyz2(x + 0x700, y + 0x720);
    p[21] = gs_uv(u + w, v + h);
    p[22] = gs_xyz2(x + sw + 0x700, y + sh + 0x720);
    p[23] = 0;
}

/* draw the screen: the frame, the slot tabs (the current one lit), the help line (`flags` 1:
 * choosing the card, else the saves) and, with `flags` 4, the save list */
static void savescreen_draw(BootCard *b, u8 flags) {
    if (b->hidden == 1) {
        return;
    }
    func_002BC040(b, 0);
    func_002BC040(b, 1);
    if (b->port == 0) {
        func_002BC040(b, 2);
    } else {
        func_002BC040(b, 3);
    }
    if (flags & 1) {
        func_002BC040(b, 5);
    } else {
        func_002BC040(b, 4);
    }
    if (flags & 4) {
        func_002BC530(b);
    }
}

void func_002BC460(BootCard *b, s32 flags) {
    savescreen_draw(b, flags);
}

static const char sFmtDate[] = "%02X/%02X/20%02X %02X:%02X";

/* the date and time a save was made (BCD: month, day, year, hour, minute) */
void func_0037EC70(BootCard *b, u8 *h) {
    Task t;

    Task_Construct(&t);
    func_00384800(&t, 0xA0, 0x52, 0x80, sFmtDate, h[0x11], h[0x10], h[0x12], h[0xE], h[0xD]);
    if (t.child != NULL) {
        Task_dtor(t.child, 1);
        t.child = NULL;
    }
}

/* the system data was read from the card: keep what was unlocked in either, and the options
 * and controls set now */
void func_002BC9C0(BootCard *b, SysData *cur, SysData *loaded) {
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
    func_00384650(t, x, y, color, func_00384B00(t, id), 0x80, 0x30, 0x10, 0x15);
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
void func_002BC530(BootCard *b) {
    Task t;
    u32 i;

    Task_Construct(&t);
    for (i = 0; i < 12; i++) {
        u8 *h = SAVE_HEADER(b->sys, i);
        s32 color = b->cursor == (s32)i ? 0x82 : 0x80;
        s32 y = (s32)i % 6 * 24 + 0x99;
        s32 x = (s32)i / 6 * 0xDA;

        func_00384800(&t, x + 0x30, y, color, sFmtSaveNo, i + 1);
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
                    func_002BC040(b, 6);
                }
                list_text(&t, 0xA0, 0x3A, 0x80, save_place(h));
                func_0037EC70(b, h);
                list_text(&t, 0xA0, 0x6A, 0x80, 0x28);
                func_00384800(&t, 0xFF, 0x6A, 0x80, sFmtTime, h[0x13], h[0x14], h[0x15]);
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
void func_002BCAC0(BootCard *b) {
    MemCard *mc = D_0044FF00;
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
        func_00384A90(&b->task, 0x20);
        b->cursor = 0;
        b->state++;
        break;
    case 2:
        if (D_0047E36C & MENU_CONFIRM) {
            MEMCARD_CHECK(mc, b->port);
            func_00380B80(&b->task, 0, sFmtDec, b->port + 1);
            func_00384A90(&b->task, 0x13);
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
            func_00384A90(&b->task, 0x14);
            b->state = 101;
        } else if (st != MC_HAS_DATA) {
            func_00384A90(&b->task, 0x21);
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
            func_002BC9C0(b, &b->saved, b->sys);
            b->cursor = b->sys->flags;
        }
        st = mc->status;
        if (st != 0) {
            if (st == 9) {
                func_00384A90(&b->task, 0x26);
            } else {
                func_00384A90(&b->task, 0x25);
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
        func_00384A90(&b->task, 0x22);
        MEMCARD_POLL(mc, b->port);
        b->state++;
        break;
    }
    case 5:
        st = mc->status;
        if (st == 0) {
            MEMCARD_POLL(mc, b->port);
        } else if (st > 0) {
            func_00384A90(&b->task, 0x15);
            b->state = 101;
            break;
        }
        pad = D_0047E36C;
        if (pad & MENU_CONFIRM) {
            if (b->slots[b->cursor] == 0) {
                func_00384A90(&b->task, 0x23);
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
                func_00384A90(&b->task, 0x25);
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
                func_00384A90(&b->task, 0x24);
                Sound_PlaySE(SE_LOADED);
                b->state = 150;
                break;
            }
            mc->status = 9;
        }
        if (mc->status == 9) {
            func_00384A90(&b->task, 0x26);
        } else {
            func_00384A90(&b->task, 0x25);
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
            func_003844E0(&b->task);
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
        func_003844E0(&b->task);
        b->state = -1;
        break;
    }
    savescreen_draw(b, flags);
    if (b->task.mode) {
        func_00384BC0(&b->task);
    }
}
