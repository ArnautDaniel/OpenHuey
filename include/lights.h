#ifndef LIGHTS_H
#define LIGHTS_H

/* shadow.c: what other files call. */
#include "common.h"

/* shadow.c */
extern void Shadow_Queue(u8 *s, s32 tri, s32 bone, f32 *light, s32 layer);   /* the shadow drawer */
extern void DoorShadow_Queue(u8 *o, u32 tri, f32 *pos, f32 *rot);

/* lights.c */
extern void Lights_RoomStart(u8 *o);
extern void Lights_ReleaseVram(u8 *o);

#endif /* LIGHTS_H */
