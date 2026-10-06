#ifndef BOOTCARD_H
#define BOOTCARD_H

/* bootcard.c: what other files call. */
#include "common.h"

typedef struct BootCard BootCard;

/* bootcard.c */
extern void BootCard_Check(BootCard *b);
extern BootCard *BootCard_ctor(BootCard *b);   /* BootCard constructor */
extern BootCard *BootCard_dtor(BootCard *b, s32 flags);   /* BootCard destructor */
extern void SaveScreen_Init(BootCard *b, void *buf0, void *buf1);
extern void SaveScreen_Draw(BootCard *b, s32 flags);
extern void SaveScreen_Load(BootCard *b);
extern void func_002BDAB0(BootCard *b);   /* the card screens' step */

#endif /* BOOTCARD_H */
