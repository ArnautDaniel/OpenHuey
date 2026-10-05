/* Helpers for the items' "use" methods (vtable +0x3C; see src/game/item_classes.c). A use
 * returns what the menu does next: 0 nothing happens, 1 used up, 2 a flag set, 4 an event
 * started (Fiona's state 5), 5 she / Hewie play an action, 8 a sound only. */
#ifndef ITEM_H
#define ITEM_H

#include "common.h"
#include "game.h"
#include "progress.h"

extern VObject *D_0044E4D0;   /* the events */
extern u32 func_00177BF0(Progress *p, u32 door, u32 slot);   /* the door's state (u8) */

/* character `c` plays event `ev` (who: 0 Fiona, 1 Hewie): its state 5 (0, ev) */
static inline void item_event(VObject *ev_mgr, s32 who, s32 ev, u8 *c) {
    AT(c, 0x14E8, s32) = 5;
    AT(c, 0x14EC, s32) = 0;
    AT(c, 0x14F0, s32) = ev;
    VCALL(ev_mgr, 0x18, void (*)(VObject *, s32, s32, u8 *))(ev_mgr, who, ev, c);
}

/* Fiona stands at event spot `spot` (in the room being played) */
static inline s32 item_at_spot(VObject *ev_mgr, u8 *fiona, s32 spot) {
    return VCALL(ev_mgr, 0x10, s32 (*)(VObject *, u8 *, s32, s32))(ev_mgr, fiona + 0x10, spot, -1) != 0;
}

#endif
