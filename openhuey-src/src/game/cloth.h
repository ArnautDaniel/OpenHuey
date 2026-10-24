/* A model's hanging parts (the original's spring sets, src/game/model.c SpringSet_* and the
 * point classes; set up by FionaModel_Setup / EventHumanModel_Setup / DaniellaModel_Springs):
 * Fiona's slip and her skirt swaying from her hips, her hair, the bones that hang or bounce;
 * Daniella's hair and what hangs from her waist. Each part is a point with a velocity, stepped
 * once a game frame in the room (so it trails as she moves), kept at its length from what it
 * hangs from and pushed off spheres and capsules on her bones; then the part's bone is aimed at
 * it before the model is skinned. */
#ifndef CLOTH_H
#define CLOTH_H

#include "../core/mathx.h"

#define CLOTH_NODES 48
#define CLOTH_SETS 6
#define CLOTH_SPHERES 16

typedef enum ClothKind {
    CLOTH_SWAY,      /* swings from its hip bone's axis within a cone, kept to the outer side
                      * (SwayPoint: her skirt) */
    CLOTH_CHAIN,     /* the same, the side kept a second time (HairPoint2: the slip) */
    CLOTH_HANG,      /* hangs free, pushed off the spheres (HangPoint: her hair) */
    CLOTH_BONEHANG,  /* hangs from its bone, in front of it and near its X axis (BoneHangPoint) */
    CLOTH_SPRUNG,    /* sprung to its bone, kept close in its X / Y plane (SprungPoint) */
    CLOTH_COSTUME,   /* hangs as CLOTH_HANG, then kept within `cone` of its bone's X; a rigid one
                      * goes on straight from the one before (CostumeHangPoint: the costumes) */
    CLOTH_WIND,      /* the same, blown toward the set's wind instead of falling (WindHangPoint) */
    CLOTH_PAIRED,    /* hangs free, following its anchor's moves by `stiff`, kept near its partner in
                      * a strand beside it (HairPoint: Daniella's hair) */
    CLOTH_BENT,      /* hangs free, drawn along its pull bone's Z by `stiff` (HangingPart: what
                      * hangs from Daniella's waist) */
    CLOTH_TIED,      /* follows its anchor's moves (0.6), off the colliders, held about 1.9 from
                      * the node it is tied to by `stiff` (Part60: Riccardo's coat) */
    CLOTH_REACH,     /* drawn by `stiff` toward where its bone points, its length out (Part50) */
} ClothKind;

typedef struct ClothNode {
    ClothKind kind;
    Vec3 p, v;          /* where it is and its velocity (the room) */
    int bone;           /* the bone it moves */
    int anchored;       /* hangs from its bone's place; else from node `from` (-1: the one before) */
    int from;
    float len;          /* its length (a sprung one: its stiffness) */
    int along;          /* (sway / chain) the hip bone it swings from */
    Vec3 side;          /* (sway / chain) the outward side, in that bone's axes */
    float cone;         /* (sway / chain) how far off that bone's X axis it may swing */
    Vec3 scale;         /* (sprung) its bone's axes scaled */
    int rigid;          /* (costume / wind) on straight from the one before, not swinging */
    int partner;        /* (paired) the node beside it in the other strand; (tied) the node it is
                         * tied to, -1 none */
    float toward;       /* (paired) its bone's Y toward the partner (1) or away (-1) */
    float stiff;        /* (paired) how much it follows its anchor; (bent) its pull */
    Vec3 last_at;       /* (paired, tied) its anchor at the last step */
    int rest_bone;      /* (tied) settled along this bone's Z, `rest_way` (1 / -1) */
    float rest_way;
    int pull;           /* (bent) the bone it is drawn along */
} ClothNode;

typedef struct ClothSphere {   /* a sphere on a bone, or a capsule between two (Capsule) */
    int bone;
    Vec3 off;           /* on its bone */
    float r;
    Vec3 at;            /* this frame, in the room */
    int capsule;        /* its other end: */
    int bone2;
    Vec3 off2, at2;
} ClothSphere;

typedef struct ClothSet {
    Vec3 force;         /* subtracted from the velocity each step (hang, bone-hang); its y the
                         * pull along the hip bone for sway / chain */
    float damp;
    int first, n;       /* its nodes, in order (the later hang from the earlier) */
    int sphere, nspheres;
    int wind_bone;      /* (wind) the bone its wind comes from, -1 none */
    Vec3 wind_at;       /* (wind) the wind's point, in that bone's space */
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
    int settle_steps;          /* steps to settle afresh (30; Daniella's 50) */
    int back, crawl_set;       /* (Daniella) her back's five capsules, her hair's set (-1 none) */
    int back_pose;             /* those capsules: 0 standing, 1 flat, 2 bent (DaniellaModel +0x1578) */
} Cloth;

/* the parts of model `name` (O_FIS/FIS_000 her slip, O_FIN/... her clothes, O_FIH / O_FIO / O_FIB /
 * O_FIC her other costumes, O_DNL/... Daniella); NULL: none */
Cloth *cloth_for(const char *name);
/* (Daniella's) her back's capsules as the story sets them (its 0xCC op 4,
 * DaniellaModel_BackCapsules): 0 standing, 1 flat, 2 bent; another number - standing */
void cloth_back(Cloth *c, int pose);
/* (Daniella's) each frame by her animation (DaniellaModel_Vt3C): on her back ($1800..$1803) her
 * back's capsules bent and her hair falling forward (0.4 along her facing, yaw); off it, bent
 * ones standing again and her hair let go */
void cloth_watch(Cloth *c, int anim, float yaw);
/* settle afresh at the next pose (+0x850: the character put somewhere - Fiona_PlaceOn,
 * Pursuer_Place - or a scene's cut, its signal 0) */
void cloth_settle(Cloth *c);
/* a game frame has passed */
void cloth_tick(Cloth *c);
/* on the posed bones (`world`, model space; place: model -> room, unplace its inverse): the
 * steps due, then the bones aimed at the parts */
void cloth_pose(Cloth *c, Mat4 *world, int nbones, const Mat4 *place, const Mat4 *unplace);

#endif
