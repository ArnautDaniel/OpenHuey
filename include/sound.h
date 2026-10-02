#ifndef SOUND_H
#define SOUND_H

/* The sound driver (global D_0044E560): banks of sound effects played by id. */
#include "common.h"
#include "game.h"

extern VObject *D_0044E560;   /* the sound driver */

/* banks */
#define SE_BANK_MENU 5    /* the system / menu sounds */

/* menu sounds (bank 5) */
#define SE_LOADED 0x0E    /* a game was loaded */
#define SE_CURSOR 0x2A    /* the cursor moved */
#define SE_DECIDE 0x2B    /* confirm */
#define SE_CANCEL 0x2C    /* back */
#define SE_BUZZER 0x85    /* not possible / an error */
#define SE_PAGE   0x87    /* the in-game menu turned a page */
#define SE_OPEN   0x94    /* the sub screen opens */
#define SE_CLOSE  0x95    /* the sub screen closes */

/* play sound `id` of `bank` (driver vtable +0x14) */
static inline void Sound_Play(VObject *snd, s32 id, s32 bank) {
    VCALL(snd, 0x14, void (*)(VObject *, s32, s32))(snd, id, bank);
}

/* a menu sound */
static inline void Sound_PlaySE(s32 id) {
    Sound_Play(D_0044E560, id, SE_BANK_MENU);
}

#endif /* SOUND_H */
