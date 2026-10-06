#ifndef SND_LIB_H
#define SND_LIB_H

/* snd_lib.c: what other files call. */
#include "common.h"

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

#endif /* SND_LIB_H */
