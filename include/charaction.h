#ifndef CHARACTION_H
#define CHARACTION_H

#include "common.h"

/* a character action (Character +0x14E8: current, +0x1508: saved); state 7 = held by
 * another character's relation */
typedef struct CharAction {
    s32 state, a, b, c, d;
    f32 e;
    s32 f;
    u8 g, h;
    u16 i;
} CharAction;

static inline void char_set_action(u8 *c, const CharAction *act) {
    AT(c, 0x14E8, s32) = act->state;
    AT(c, 0x14EC, s32) = act->a;
    AT(c, 0x14F0, s32) = act->b;
    AT(c, 0x14F4, s32) = act->c;
    AT(c, 0x14F8, s32) = act->d;
    AT(c, 0x14FC, f32) = act->e;
    AT(c, 0x1500, s32) = act->f;
    AT(c, 0x1504, u8) = act->g;
    AT(c, 0x1505, u8) = act->h;
    AT(c, 0x1506, u16) = act->i;
}

#endif
