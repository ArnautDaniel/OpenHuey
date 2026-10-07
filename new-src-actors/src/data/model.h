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
    const uint8_t *motions;  /* the motion bank (resource 3), or NULL */
    size_t motions_size;
} Model;

/* build from a .PCK (takes ownership of the buffer); 0 if malformed */
int model_build(Model *m, uint8_t *pck, size_t size);
void model_free(Model *m);

/* motions: by id (as the game numbers them, e.g. 0x0200), how many there are */
int model_motion_count(const Model *m);
int model_motion_id(const Model *m, int index);
/* where motion `index` is in the bank's id list (Motion_AnimIndex: what the motion tables go by) */
int model_motion_pos(const Model *m, int index);
int model_motion_find(const Model *m, int id);   /* the index, -1 if none */
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
/* the bones' origins (model space) of the last pose model_pose_blend / model_pose_root made */
const Vec3 *model_pose_origins(void);
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

#endif
