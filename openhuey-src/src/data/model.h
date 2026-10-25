/* Character models (.PCK) and their motions. Format: docs/model_format.md in the repository.
 *
 * A .PCK holds resources: 0 the skeleton and meshes, 1 morphing parts (faces, hands), 2 shadow
 * volumes, 3 the motion bank. Mesh vertices are in model space in the bind pose and carry up
 * to four bone weights; a bone's skin matrix is its posed world matrix times its inverse bind
 * matrix. Motions replace bones' rest rotations (Euler X, Y, Z) and translations, a key a frame. */
#ifndef MODEL_H
#define MODEL_H

#include "../core/mathx.h"
#include "roommesh.h"

#include <stddef.h>
#include <stdint.h>

#define MODEL_MAX_BONES 128

typedef struct ModelBone {
    int parent;              /* -1: the root */
    float rest_rot[3];       /* Euler angles: about X, then Y, then Z */
    float rest_pos[3];
    Mat4 inv_bind;
} ModelBone;

typedef struct SkinVertex {
    Vec3 pos, normal;        /* bind pose, model space */
    float s, t;
    uint8_t bones[4];
    float weights[4];
} SkinVertex;

/* a morphing part (resource 1: a face, a hand) - its draw's vertices moved between its shapes
 * (the original's func_001BD650): the face by the model's face weights (data/model.h's
 * model_morph), a hand from one of its shapes to another by the model's hand pose */
typedef struct ModelMorph {
    int first, count;        /* its draw's vertices in v (a triangle list, in their rest shape) */
    int face;                /* 1 the face (record kind 1), 0 a hand */
    int hand;                /* (a hand) the hand pose that moves it: 0 for the second part, else 1
                              * (the original's +0x38 / +0x48) */
    int nshapes;             /* shape 0 the rest */
    Vec3 *dpos, *dnrm;       /* nshapes x count: each shape's vertices less the rest (shape 0's: 0) */
} ModelMorph;

typedef struct Model {
    uint8_t *pck;            /* the file (motions are read from it as they play) */
    size_t size;
    ModelBone bones[MODEL_MAX_BONES];
    int nbones;
    const uint8_t *bone_table;   /* motion track code + 1 -> bone */
    SkinVertex *v;           /* triangle lists, per draw */
    int nv;
    MeshDraw *d;
    int nd;
    ModelMorph *morphs;      /* its faces and hands (none: the library's older models) */
    int nmorphs;
    const uint8_t *motions;  /* the motion bank (resource 3), or NULL */
    size_t motions_size;
    /* a second bank, looked in after its own (the original's +0x4C8: a room's motions for a
     * scene, the story's 0xA6 / 0xA7), or NULL; its motions are numbered from MODEL_SECOND */
    const uint8_t *motions2;
    size_t motions2_size;
} Model;
#define MODEL_SECOND 0x10000

/* a model's face and hands (the original's model +0x58 / +0x38 / +0x48): the face's weight for
 * each shape (16), each hand from one shape to another by w. The face drawn (func_001BD650) is
 * its rest and the mouth (1) and eyelids (7, 8) - or, while a scene has the character (`all`: the
 * model's +0x30, set by Motion_Disable), every shape by its weight (func_001BCD30) */
typedef struct ModelFace {
    float w[16];
    struct {
        int a, b;
        float w;
    } hand[2];
    int all;
} ModelFace;
/* the morphing parts' vertices in `v` (a copy of the model's: nv of them) moved into the face's
 * shapes, from their rest shape; the other vertices are left as they are */
void model_morph(const Model *m, const ModelFace *f, SkinVertex *v);

/* build from a .PCK (takes ownership of the buffer); 0 if malformed */
int model_build(Model *m, uint8_t *pck, size_t size);
void model_free(Model *m);

/* motions: by id (as the game numbers them, e.g. 0x0200), how many there are */
int model_motion_count(const Model *m);
int model_motion_id(const Model *m, int index);
/* where motion `index` is in the bank's id list (Motion_AnimIndex: what the motion tables go by) */
int model_motion_pos(const Model *m, int index);
int model_motion_find(const Model *m, int id);   /* the index (its own bank, then the second), -1 if none */
int model_motion_frames(const Model *m, int index);

/* the skin matrices (model space) for a motion at a time in frames (fractions blend), or the
 * rest pose for index -1 */
void model_pose(const Model *m, int index, float frame, Mat4 *skin);
/* the motion's root movement at a time: its per-frame turn about y (radians) and step (model
 * space: forward is +z) - the special track -1 that moves the character itself (the original's
 * Motion_RootRotation / Motion_RootTranslation); 0 if it has none */
int model_root_delta(const Model *m, int index, float frame, float *turn, Vec3 *step);
/* the same, and the root bone's posed position (model space) */
void model_pose_root(const Model *m, int index, float frame, Mat4 *skin, Vec3 *root);
/* cross-faded from motion `prev` at its time, weight w (1: all of it .. 0: none) */
void model_pose_blend(const Model *m, int index, float frame, int prev, float prev_frame, float w, Mat4 *skin,
                      Vec3 *root);
/* a special track (`code` < 0: -1 the root's movement, -5 / -6 the front / hind feet's contact,
 * ...) of a motion at a time: its three values (raw); 0 if the motion hasn't it */
int model_track_raw(const Model *m, int index, int code, float frame, float out[3]);
/* ... converted as the original's Track_Sample (a place track's keys / 256) */
int model_track(const Model *m, int index, int code, float frame, float out[3]);
/* a track exactly as Track_Sample gives it (all its formats: a key a frame, blended between
 * frames for rotations and places, the others stepped; format 5's first two the s16s' bits as an
 * int): which of out[0..7] it set (bit i: out[i]; 0 the motion hasn't it) */
int model_track_sample(const Model *m, int index, int code, float frame, float out[8]);
/* the bones' origins (model space) of the last pose model_pose_blend / model_pose_root made */
const Vec3 *model_pose_origins(void);
const Vec3 (*model_pose_axes(void))[3];   /* the last pose's bones' axes (x, y, z; model space) */
/* which of a motion's five parts it has (bit p: part p - 0 the special channels, 1 the body,
 * 2..4 parts that can play motions of their own: Hewie's ears, tail, jaw) */
int model_motion_parts(const Model *m, int index);
/* posing from layers: each a motion (cross-faded from the one before) for some of the parts */
typedef struct ModelLayer {
    int index;
    float frame;
    int prev;
    float prev_frame, w;
    int mask;        /* the parts it poses */
    /* each one's variant (the original's second track in a motion slot): blended in by its share
     * (1 - the slot's weight +0x1C; 0: none) */
    int var;
    float var_frame, var_w;
    int prev_var;
    float prev_var_frame, prev_var_w;
} ModelLayer;
void model_pose_layers(const Model *m, const ModelLayer *layers, int nlayers, Mat4 *skin, Vec3 *root);
/* bones turned as the next pose is made (where a head looks: the original's DogModel_AdjustBone) */
typedef struct ModelTurn {
    int bone;
    float pitch, yaw;
} ModelTurn;
void model_pose_turns(const ModelTurn *t, int n);
/* a hook on the next poses: the bones' world matrices (model space) once built, before skinning */
typedef void (*ModelPoseHook)(Mat4 *world, int nbones, void *user);
void model_pose_hook(ModelPoseHook hook, void *user);
/* a hook on the next poses for each bone as it is posed (parents first, before its children: the
 * original's model +0x14, AdjustBone, from Skel_WorldPose): bone `i`'s world matrix (model space;
 * the others posed so far in `world`) changed as the model's class does */
typedef void (*ModelBoneAdjust)(int i, Mat4 *world, void *user);
void model_pose_adjust(ModelBoneAdjust adjust, void *user);

#endif
