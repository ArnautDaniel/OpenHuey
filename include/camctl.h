#ifndef CAMCTL_H
#define CAMCTL_H

/* camctl.c: what other files call. */
#include "common.h"

/* camctl.c */
extern void CamDirector_NewRoom(u8 *d);
extern void CamDirector_RoomStart(u8 *d, s32 target);
extern void CamDirector_Ease(u8 *d);
extern void CamDirector_Track(u8 *d);
extern void CamDirector_Update(u8 *d);

#endif /* CAMCTL_H */
