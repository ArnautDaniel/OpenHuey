/* Lorenzo in his chair (the original's LorenzoModel, src/game/lorenzo.c): his model isn't a
 * human's - no feet planted - but sits on the floor as a chair does. Each frame the character asks
 * (chair_fit, from Forth: Pursuer_ModelUpdate's +0x40 with his stance's two points):
 *  - on the floor (his height on the mesh) the body is tilted along the floor from the point `back`
 *    behind him to him, and the chair's front (bone $A and what hangs from it) along the floor from
 *    him to the point `front` ahead (LorenzoModel_BodyFrames: the model's matrix +0x7D0 and the
 *    front frame +0xC70, Chair_Frame); off it, both as he stands;
 *  - as the skeleton is posed (LorenzoModel_AdjustBone, the model's +0x14 per bone): bone 0 takes
 *    the body's tilt, bone $A the front's in its place, and the head - bones $B, $12, $13 - turns
 *    about the front frame's axes by the look's tilt and turn (+0x854 / +0x858, Mtx_TurnTwo:
 *    $B 0.3 of the turn; $12 0.4 of the tilt and 0.3 of the turn about a frame kept upright from
 *    $B; $13 0.2 and 0.4 from $12).
 * (Mtx_TurnTwo clears the bone's place and turns it by matrices whose place row the original
 * leaves unset - stack garbage; here each head bone turns about its own place.) */
#ifndef CHAIR_H
#define CHAIR_H

#include "../core/mathx.h"
#include "../data/navmesh.h"

#include <stdint.h>

struct Actor;

typedef struct Chair {
    int on;            /* his model (O_LRO) */
    int asked;         /* chair_fit this tick */
    int fitted;        /* the poses till the next tick by it (else as he stands, the head ahead) */
    Mat4 body, front;  /* the tilts, model space (the original's +0x7D0 and +0xC70 less his heading) */
    float pitch, yaw;  /* the look's tilt and turn (+0x854 / +0x858) */
} Chair;

void chair_set(Chair *c, int on);
/* this frame: on the floor from triangle `tri` (mask: the floor that stops him), the floor `front`
 * ahead and `back` behind (0 0: as he stands); the head's tilt and turn */
void chair_fit(struct Actor *a, const NavMesh *nav, int tri, uint32_t mask, float front, float back, float pitch,
               float yaw);
/* a tick has passed: what was asked holds for its poses */
void chair_tick(Chair *c);
/* the pose's per-bone hook (data/model.h model_pose_adjust; user: the Chair) */
void chair_adjust(int i, Mat4 *world, void *user);

#endif
