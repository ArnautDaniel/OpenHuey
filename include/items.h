#ifndef ITEMS_H
#define ITEMS_H

/* items.c: what other files call. */
#include "common.h"

/* items.c */
extern s32 Items_Equipped(u8 *items, u8 slot);   /* the item in equipment slot `slot` */
extern void Items_Update(u8 *o);
extern s32 Items_UseId(u8 *o, s32 id);   /* one of an item used */
extern u32 Items_Count(u8 *o, s32 id);   /* how many */
extern void *Items_Give(u8 *items, u32 id, s32 n);   /* given */
extern s32 Items_Field20(u8 *items, u8 l, u8 i);   /* the item's word (+0x20) */
extern s32 Items_KindSlot(u8 *items, u8 l, u8 i);   /* its equipment slot */
extern void Items_Notify(u8 *o, s32 id, s32 arg);
extern void *Items_NewItem3F(u8 *items, const u8 *src);   /* a word plate added */
extern void Items_Equip(u8 *items, u8 l, u8 i);   /* equip it */
extern void Items_Unequip(u8 *items, u8 l, u8 i);   /* take it off */
extern s32 Items_EquipState(u8 *items, u8 l, u8 i);   /* its equipment status: 1 equipped */
extern void Items_UseEquipped(u8 *items, u8 slot);
extern void Items_UseOne(u8 *items, u8 l, u8 i);   /* one of it used */
extern u32 Items_Use(u8 *items, u8 l, u8 i);   /* use the item */
extern void Items_Sort(u8 *items, u8 l);   /* sort the list */
extern void Items_StartUse(u8 *items, u8 l, u8 i, void *arg);   /* start using it */
extern s32 Items_Kind(u8 *items, u8 l, u8 i);   /* an item's kind (-1 none) */
extern u32 Items_Actions(u8 *items, u8 l, u8 i);
extern s32 Items_Id(u8 *items, u8 l, u8 i);   /* an item's id (-1 none) */
extern s32 Items_HowMany(u8 *items, u8 l, u8 i);   /* how many */
extern u8 Items_IsCounted(u8 *items, u8 l, u8 i);   /* counted */
extern u64 *Items_Data(u8 *items, u8 l, u8 i);   /* an item's data */
extern s32 Items_CountItem3F(u8 *items);   /* the word plates held */

#endif /* ITEMS_H */
