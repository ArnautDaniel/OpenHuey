/* A human's legs on the floor (Fiona's and the stalkers' models: the original's
 * HumanModel_BodyFrames / HumanModel_PlantFeet / Model_LegIK, src/game/model.c). Each frame the
 * character asks (legs_fit, from Forth):
 *  - the body: the model put at the height of the floor under the lower of its feet, eased
 *    1:3 with the frame before (BodyFrames' +0x804 / +0x8C0); on a
 *    slope motion (its flag 4) the floor at two points `back` and `front` along its step instead;
 *    with flag $100 a foot not down keeps the character's height; back = front = 0: the model at
 *    the character's place;
 *  - then, once posed, the feet planted (PlantFeet): each foot put on the floor under it (on a
 *    slope its height above the model eased by how much the slope faces the way it goes:
 *    Model_SlopeKeep, eased 1:3), and the legs - two-bone chains hip, knee, ankle, with the
 *    foot - bent to reach them (IK2), the ankle and foot moved by how far the solved ankle went.
 * A frame the character doesn't ask, the model is at its place, unfitted. */
#ifndef HUMANLEGS_H
#define HUMANLEGS_H

#include "../data/model.h"
#include "../data/navmesh.h"

struct Actor;

typedef struct HumanLegs {
    int on;                 /* its bones are set (a human's skeleton) */
    int bones[2][4];        /* right, left: hip, knee, ankle, foot (+0x890.. / +0x8A0..) */
    /* asked this tick (legs_fit), and what that leaves for the frame's pose */
    int asked;
    int fitted;             /* the model at `height` this tick (else at the character's place) */
    int plant;              /* the feet planted this tick */
    float height;           /* +0x804: the model's height */
    float last;             /* +0x8C0: the height the frame before */
    float keep;             /* +0x8C4: how much of the feet's height a slope leaves (eased) */
    int jump;               /* +0x990: the next height not eased (as loaded) */
    int use[2];             /* each foot onto the floor (else at the character's height) */
    /* the floor the feet go onto: the character's triangle, place and floor mask */
    const NavMesh *nav;
    int tri;
    uint32_t mask;
    Vec3 at;
} HumanLegs;

/* its legs are a human's: bone chains from `right` and `left` (hip, knee, ankle, foot); -1 none.
 * The easing starts afresh (HumanModel_Loaded's FeetUp) */
void humanlegs_set(HumanLegs *h, int right, int left);
/* Motion_OntoFloor: point p (room) put on the floor, walking the mesh from triangle `tri` at `at`
 * toward it (mask: the marked triangles that stop it) */
void humanlegs_onto_floor(const NavMesh *nav, int tri, uint32_t mask, Vec3 at, float *p);
/* the floor's height under `at` on triangle `tri` (the nav mesh's +0x14) */
float humanlegs_floor_at(const NavMesh *nav, int tri, Vec3 at);
/* this frame (the character's +0x40, before its motion moves on): the body by its feet over
 * `nav` (tri -1: the model at its place, the feet not planted - Model_BodyFrames); plant 1 / 0
 * the feet planted or not once posed (+0x4C), -1 not asked */
void humanlegs_fit(struct Actor *a, const NavMesh *nav, int tri, uint32_t mask, int plant, float back, float front);
/* after its motion moved on this tick: the feet's contacts and the slope's share (PlantFeet's
 * part before the pose) */
void humanlegs_tick(struct Actor *a);
/* the feet planted on the posed skeleton (model space; place: model -> room, unplace its inverse) */
void humanlegs_pose(HumanLegs *h, const Model *m, Mat4 *world, const Mat4 *place, const Mat4 *unplace);

/* A human's head where it looks (HumanModel_AdjustBone, the human models' +0x14 as each bone is
 * posed): the head bone (+0x8B0) turned in its own frame by the tilt (-pitch about its X) and half
 * the turn (about its Y); the bone below it (+0x8B4) by the other half about the model's up, about
 * its own place. pitch / yaw: the motion's +0x854 / +0x858 (the character eases them). */
typedef struct HumanHead {
    int head, neck;         /* -1: none */
    float pitch, yaw;
} HumanHead;

/* the pose's per-bone hook (data/model.h model_pose_adjust; user: the HumanHead) */
void humanhead_adjust(int i, Mat4 *world, void *user);

#endif
