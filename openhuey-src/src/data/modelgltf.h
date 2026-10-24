/* A character model as glTF 2.0 (docs/library-and-mods.md): the library's form of a .PCK -
 * O_FIN/FIN_000.PCK -> O_FIN/FIN_000.model/: model.gltf + model.bin (the skinned mesh and its
 * skeleton, as data/model.h builds them) and motions.bin (the motion bank, as the game has it:
 * not converted - the engine plays its keys as the original does, which glTF's animations can't
 * say exactly).
 *
 *   nodes 0..n-1  the bones, at rest (translation, rotation: viewers see the model standing), each
 *                 its own rest angles exact in extras "hga": "bone <parent> <rx ry rz> <px py pz>"
 *   node n        the model: mesh 0 skinned by skin 0 (the bones, their inverse bind matrices)
 *   mesh 0        a primitive a draw (indexed), JOINTS_0 / WEIGHTS_0 the bones and their weights;
 *                 its extras "hga": "draw ..." (data/roomgltf.h's words, then where its vertices go)
 *   nodes n+1..   the faces and hands (the morphing parts, in their draws' order): mesh 1.. each,
 *   meshes 1..    skinned by skin 0, one primitive whose targets are the part's shapes 1.. (each
 *                 less the rest: exact - the game's keys are 1/4096 steps); its words go on
 *                 "morph <face 1 / hand 0> <hand pose 0 / 1> <shapes> <its draw's place>"
 *   materials     the model's textures (its .texbank beside, as `textures` names it)
 *   scene extras  "hga": "model <format> <bones> <vertices> <the bone table, 256 bytes in hex>"
 *                 (format 2; 1 had no faces' and hands' meshes - still read: their rest alone)
 *                 (a draw's words end with where its vertices start: the draws go in their passes'
 *                 order, not their vertices')
 * The .PCK's shadow volumes (its resource 2: hga doesn't draw them yet) are kept as they are,
 * shadows.bin. */
#ifndef MODELGLTF_H
#define MODELGLTF_H

#include <stddef.h>
#include <stdint.h>

#include "model.h"

#define MODELGLTF_FORMAT 2

/* the glTF's JSON (malloc'd) and binary (malloc'd: named `bin_name`); its images
 * "<textures><i>.png" ("../FIN_000.texbank/") */
char *modelgltf_write(const Model *m, const char *bin_name, const char *textures, uint8_t **bin, size_t *bin_size,
                      size_t *len);
/* read back with the motion bank (`motions`: malloc'd, taken; NULL none): 0 if it isn't one hga
 * wrote (m is then empty, motions freed) */
int modelgltf_read(Model *m, const char *json, size_t len, const uint8_t *bin, size_t bin_size, uint8_t *motions,
                   size_t motions_size);

#endif
