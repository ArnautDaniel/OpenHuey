#ifndef ITEMS_H
#define ITEMS_H

/* items.c: what other files call. */
#include "common.h"

/* items.c */
extern s32 func_00260690(u8 *items, u8 slot);   /* the item in equipment slot `slot` */
extern void func_00260EC0(u8 *o);
extern s32 func_00260BB0(u8 *o, s32 id);   /* one of an item used */
extern u32 func_00260CF0(u8 *o, s32 id);   /* how many */
extern void *func_00261090(u8 *items, u32 id, s32 n);   /* given */
extern s32 func_002604E0(u8 *items, u8 l, u8 i);   /* the item's word (+0x20) */
extern s32 func_00260630(u8 *items, u8 l, u8 i);   /* its equipment slot */
extern void func_00260170(u8 *o, s32 id, s32 arg);
extern void *func_00261040(u8 *items, const u8 *src);   /* a word plate added */
extern void func_00260840(u8 *items, u8 l, u8 i);   /* equip it */
extern void func_002607A0(u8 *items, u8 l, u8 i);   /* take it off */
extern s32 func_002606E0(u8 *items, u8 l, u8 i);   /* its equipment status: 1 equipped */
extern void func_00260A60(u8 *items, u8 slot);
extern void func_00260B00(u8 *items, u8 l, u8 i);   /* one of it used */
extern u32 func_00260DD0(u8 *items, u8 l, u8 i);   /* use the item */
extern void func_0025FF80(u8 *items, u8 l);   /* sort the list */
extern void func_00260250(u8 *items, u8 l, u8 i, void *arg);   /* start using it */
extern s32 func_00260420(u8 *items, u8 l, u8 i);   /* an item's kind (-1 none) */
extern u32 func_002603C0(u8 *items, u8 l, u8 i);
extern s32 func_00260480(u8 *items, u8 l, u8 i);   /* an item's id (-1 none) */
extern s32 func_00260300(u8 *items, u8 l, u8 i);   /* how many */
extern u8 func_00260360(u8 *items, u8 l, u8 i);   /* counted */
extern u64 *func_002602A0(u8 *items, u8 l, u8 i);   /* an item's data */
extern s32 func_00260540(u8 *items);   /* the word plates held */

#endif /* ITEMS_H */
