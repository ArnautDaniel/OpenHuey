/* Helpers for the items' "use" methods (vtable +0x3C; see src/game/item_classes.c). A use
 * returns what the menu does next: 0 nothing happens, 1 used up, 2 a flag set, 4 an event
 * started (Fiona's state 5), 5 she / Hewie play an action, 8 a sound only. */
#ifndef ITEM_H
#define ITEM_H

#include "common.h"
#include "game.h"
#include "progress.h"
#include "globals.h"
#include "actor.h"

extern u32 func_00177BF0(Progress *p, u32 door, u32 slot);   /* the door's state (u8) */

/* character `c` plays event `ev` (who: 0 Fiona, 1 Hewie): its state 5 (0, ev) */
static inline void item_event(VObject *ev_mgr, s32 who, s32 ev, Character *c) {
    AT(c, 0x14E8, s32) = 5;
    AT(c, 0x14EC, s32) = 0;
    AT(c, 0x14F0, s32) = ev;
    VCALL(ev_mgr, 0x18, void (*)(VObject *, s32, s32, Character *))(ev_mgr, who, ev, c);
}

/* Fiona stands at event spot `spot` (in the room being played) */
static inline s32 item_at_spot(VObject *ev_mgr, Character *fiona, s32 spot) {
    return VCALL(ev_mgr, 0x10, s32 (*)(VObject *, u8 *, s32, s32))(ev_mgr, (u8 *)fiona + 0x10, spot, -1) != 0;
}


/* Fiona at event spot `spot` of room `room` */
static inline s32 item_room_spot(Progress *p, s32 room, s32 spot) {
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) != room) {
        return 0;
    }
    return item_at_spot(gEvents, gCharPlayer, spot);
}

/* laid on the altar (spot 0x1B of room 0xC0): the events' +0x30 (1, the item's id), event 0x14,
   flag 0x18 */
static inline s32 item_offer(Progress *p, void *o) {
    VObject *ev_mgr;

    if (!item_room_spot(p, 0xC0, 0x1B)) {
        return 0;
    }
    ev_mgr = gEvents;
    VCALL(ev_mgr, 0x30, void (*)(VObject *, s32, s32))(ev_mgr, 1, AT(o, 0x4, s32));
    item_event(ev_mgr, 0, 0x14, gCharPlayer);
    Progress_SetFlag(p, 0x18);
    return 4;
}

/* door `door` of the room is open (state bit 4) */
static inline s32 item_door_open(Progress *p, u32 door) {
    return func_00177BF0(p, door, 0) & 0xFF & 4;
}

extern u32 func_00178610(Progress *p, u32 route);

/* door `door` open and route `route` taken */
static inline s32 item_door_route(Progress *p, u32 door, u32 route) {
    return item_door_open(p, door) && func_00178610(p, route) != 0;
}

/* Fiona's event `ev`, then flag 0x18; 4 */
static inline s32 item_event_flag(Progress *p, s32 ev) {
    item_event(gEvents, 0, ev, gCharPlayer);
    Progress_SetFlag(p, 0x18);
    return 4;
}

/* gCharPlayer +0x1AD5F4: f32 clamped to 0..100; +0x1AD5F8: s32 clamped to 0..1800 */
static inline void item_composure(f32 df) {
    u8 *g = (u8 *)gCharPlayer;
    f32 f = AT(g, 0x1AD5F4, f32) + df;

    AT(g, 0x1AD5F4, f32) = f;
    if (f < 0.0f) {
        AT(g, 0x1AD5F4, f32) = 0.0f;
    } else if (!(f <= 100.0f)) {
        AT(g, 0x1AD5F4, f32) = 100.0f;
    }
}

static inline void item_meters(f32 df, s32 di) {
    u8 *g = (u8 *)gCharPlayer;

    item_composure(df);
    AT(g, 0x1AD5F8, s32) += di;
    if (AT(g, 0x1AD5F8, s32) < 0) {
        AT(g, 0x1AD5F8, s32) = 0;
    } else if (AT(g, 0x1AD5F8, s32) > 1800) {
        AT(g, 0x1AD5F8, s32) = 1800;
    }
}

/* Hewie's trust (Progress +0xFB6) changed by `d`, kept within 0..10000 */
static inline void item_trust(s32 d) {
    s16 v;

    *(s16 *)((u8 *)gProgress + 0xFB6) += d;
    v = *(s16 *)((u8 *)gProgress + 0xFB6);
    if (v < 0) {
        *(s16 *)((u8 *)gProgress + 0xFB6) = 0;
    } else if (v >= 10001) {
        *(s16 *)((u8 *)gProgress + 0xFB6) = 10000;
    }
}

/* Hewie's health up by `d`, at most full; 1 if it is full */
static inline s32 item_hewie_heal(s32 d) {
    AT(gCharPartner, 0x14C8, s32) += d;
    if (AT(gCharPartner, 0x14C8, s32) >= AT(gCharPartner, 0x14CC, s32)) {
        AT(gCharPartner, 0x14C8, s32) = AT(gCharPartner, 0x14CC, s32);
    }
    return AT(gCharPartner, 0x14C8, s32) == AT(gCharPartner, 0x14CC, s32);
}

/* Fiona hands Hewie a treat (event 0x8E) and he takes it (`hewie_ev`); +0x40: he's at hand */
static inline s32 item_give_hewie(VObject *ev_mgr, s32 hewie_ev) {
    item_event(ev_mgr, 0, 0x8E, gCharPlayer);
    item_event(ev_mgr, 1, hewie_ev, gCharPartner);
    return 5;
}

#endif
