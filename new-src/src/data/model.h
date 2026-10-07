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
int model_motion_find(const Model *m, int id);   /* the index, -1 if none */
int model_motion_frames(const Model *m, int index);

/* the skin matrices (model space) for a motion at a time in frames (fractions blend), or the
 * rest pose for index -1 */
void model_pose(const Model *m, int index, float frame, Mat4 *skin);
/* the same, and the root bone's posed position (model space) */
void model_pose_root(const Model *m, int index, float frame, Mat4 *skin, Vec3 *root);

#endif
