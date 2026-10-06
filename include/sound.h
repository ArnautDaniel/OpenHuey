#ifndef SOUND_H
#define SOUND_H

/* The sound driver (global gSound): banks of sound effects played by id. */
#include "common.h"
#include "game.h"
#include "globals.h"

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
    Sound_Play(gSound, id, SE_BANK_MENU);
}

/* ---- (was snd_lib.h) ---- */

/* snd_lib.c: what other files call. */

/* snd_lib.c */
extern s32 SndTable_Count(u8 *t, s32 size);
extern void SndTable_Curves(s32 bank, u8 *t, s32 size, u16 *out);
extern s32 SndLib_DmaToIop(u32 src, u32 dest, u32 size, s32 attr, s32 inIrq);   /* EE -> IOP DMA */
extern s32 SndLib_TransferBusy(u32 cmd, s32 poll);   /* a transfer still running */
extern void *SndLib_Transfer(u32 cmd, void *args);   /* ... without waiting */
extern u32 *SndLib_Call(u32 cmd, void *args);   /* call the driver */
extern void SndLib_StartServer(s32 prio, s32 stack);
extern void SndLib_SendState(void);
extern void SndLib_Bind(void);
extern void SndLib_Clear(void);
extern s8 SndLib_DriverArgs(char *out, u8 *p);

/* ---- (was snd_place.h) ---- */

/* snd_place.c: what other files call. */

typedef struct VObject VObject;

/* snd_place.c */
extern void Sound_PlayAt(VObject *snd, u32 which, f32 *pos);
extern void Sound_PlayBankAt(VObject *snd, u32 id, u32 bank, f32 *pos, s32 vol, s32 pitch);

/* ---- (was snd_driver.h) ---- */

/* snd_driver.c: what other files call. */

/* snd_driver.c */
extern void SndDriver_FreeIop(u8 *d);
extern void SndDriver_Frame(u8 *d);   /* sound driver tick */
extern void SndDriver_Start(u8 *d);
extern u8 *SndDriver_dtor(u8 *d, s32 flags);

#endif /* SOUND_H */
