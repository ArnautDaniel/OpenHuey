/* A model's hanging parts (the original's spring sets, src/game/model.c SpringSet_* and the
 * point classes; set up by FionaModel_Setup / EventHumanModel_Setup): Fiona's slip and her
 * skirt swaying from her hips, her hair, the bones that hang or bounce. Each part is a point
 * with a velocity, stepped once a game frame in the room (so it trails as she moves), kept at
 * its length from what it hangs from and pushed off spheres on her bones; then the part's bone
 * is aimed at it before the model is skinned. */
#ifndef CLOTH_H
#define CLOTH_H

#include "../core/mathx.h"

#define CLOTH_NODES 48
#define CLOTH_SETS 6
#define CLOTH_SPHERES 12

typedef enum ClothKind {
    CLOTH_SWAY,      /* swings from its hip bone's axis within a cone, kept to the outer side
                      * (SwayPoint: her skirt) */
    CLOTH_CHAIN,     /* the same, the side kept a second time (HairPoint2: the slip) */
    CLOTH_HANG,      /* hangs free, pushed off the spheres (HangPoint: her hair) */
    CLOTH_BONEHANG,  /* hangs from its bone, in front of it and near its X axis (BoneHangPoint) */
    CLOTH_SPRUNG,    /* sprung to its bone, kept close in its X / Y plane (SprungPoint) */
} ClothKind;

typedef struct ClothNode {
    ClothKind kind;
    Vec3 p, v;          /* where it is and its velocity (the room) */
    int bone;           /* the bone it moves */
    int anchored;       /* hangs from its bone's place; else from the node before it */
    float len;          /* its length (a sprung one: its stiffness) */
    int along;          /* (sway / chain) the hip bone it swings from */
    Vec3 side;          /* (sway / chain) the outward side, in that bone's axes */
    float cone;         /* (sway / chain) how far off that bone's X axis it may swing */
    Vec3 scale;         /* (sprung) its bone's axes scaled */
} ClothNode;

typedef struct ClothSphere {
    int bone;
    Vec3 off;           /* on its bone */
    float r;
    Vec3 at;            /* this frame, in the room */
} ClothSphere;

typedef struct ClothSet {
    Vec3 force;         /* subtracted from the velocity each step (hang, bone-hang); its y the
                         * pull along the hip bone for sway / chain */
    float damp;
    int first, n;       /* its nodes, in order (the later hang from the earlier) */
    int sphere, nspheres;
} ClothSet;

typedef struct Cloth {
    ClothSet sets[CLOTH_SETS];
    int nsets;
    ClothNode nodes[CLOTH_NODES];
    int nnodes;
    ClothSphere spheres[CLOTH_SPHERES];
    int nspheres;
    int hang_set, hang_bone;   /* reset: that set's nodes put under their anchors along the
                                * bone's Z axis (her hair under her head) */
    int reset;                 /* settle afresh (30 steps) at the next pose */
    int pending;               /* game frames to step at the next pose */
    Vec3 root;                 /* where the model was at the last steps */
} Cloth;

/* the parts of model `name` (O_FIS/FIS_000 her slip, O_FIN/... her clothes); NULL: none */
Cloth *cloth_for(const char *name);
/* a game frame has passed */
void cloth_tick(Cloth *c);
/* on the posed bones (`world`, model space; place: model -> room, unplace its inverse): the
 * steps due, then the bones aimed at the parts */
void cloth_pose(Cloth *c, Mat4 *world, int nbones, const Mat4 *place, const Mat4 *unplace);

#endif
