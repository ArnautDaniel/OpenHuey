#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

/* ---- (was camctl.h) ---- */

/* camctl.c: what other files call. */

/* camctl.c */
extern void CamDirector_NewRoom(u8 *d);
extern void CamDirector_RoomStart(u8 *d, s32 target);
extern void CamDirector_Ease(u8 *d);
extern void CamDirector_Track(u8 *d);
extern void CamDirector_Update(u8 *d);

typedef struct VObject VObject;

/* camera.c */
extern void *Camera_ctor(VObject *o);   /* +0x14D9B00 */

#endif /* CAMERA_H */
