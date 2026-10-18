/* A dog's legs on the floor (Hewie's model: the original's DogModel_PlantFeet / LegIK): a foot
 * down this frame and the last stays where it landed in the room, one lifting eases back to the
 * animation, and the legs bend to reach the feet - the front ones as three-bone chains (bones
 * 8..11, 12..15), the hind ones as two-bone (0x15..0x17, 0x1A..0x1C). */
#ifndef DOGLEGS_H
#define DOGLEGS_H

#include "../data/model.h"

typedef struct DogLegs {
    int on;                 /* the actor's legs are fitted */
    int init;               /* the feet placed once */
    int down[4], down_was[4];   /* the motion says each foot is down (now, the frame before) */
    int planted[4], was[4];
    int len[4], left[4];    /* easing a lifted foot back: over len frames, left to go */
    float foot[4][3];       /* where each foot is in the room */
    float off[4][3];
} DogLegs;

void doglegs_reset(DogLegs *d);
/* the legs fitted to the feet, on the posed skeleton (model space; place: model -> room,
 * unplace its inverse) */
void doglegs_pose(DogLegs *d, const Model *m, Mat4 *world, const Mat4 *place, const Mat4 *unplace);

#endif
