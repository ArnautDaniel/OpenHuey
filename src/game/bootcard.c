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

extern MemCard *D_0044FF00;
extern s32 D_0047B258[2];   /* check status per slot */
extern s32 D_0047B260;      /* slot 1's status; 9: its data couldn't be read */
extern u32 D_0047E37C;      /* pad buttons pressed */

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
