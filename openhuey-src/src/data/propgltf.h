/* A prop: any glTF 2.0 model (docs/modding.md "Props") - a mod's own object, made in Blender or
 * any tool, not one of the game's. Read as authored: its nodes' transforms applied, Y up, one unit
 * a game unit; each triangle primitive a draw, lit by the room's lights; its base colour - the
 * material's texture (a PNG) and factor - and its vertex colours; OPAQUE, MASK or BLEND as
 * its material says. A .gltf with its buffers and images beside it (or as data: URIs), or a .glb.
 * No skin, no animations (a mod moves the whole prop). */
#ifndef PROPGLTF_H
#define PROPGLTF_H

#include <stddef.h>
#include <stdint.h>

#include "roommesh.h"

typedef struct PropImage {
    uint8_t *rgba;   /* w x h, malloc'd */
    int w, h;
} PropImage;

typedef struct PropMesh {
    MeshVertex *v;   /* a triangle list */
    int nv;
    MeshDraw *d;     /* a draw a primitive (texture: an index in images, -1 none) */
    int nd;
    PropImage *images;
    int nimages;
    float lo[3], hi[3];   /* its bounds */
} PropMesh;

/* reads a file next to the glTF (its buffers and images): `rel` is the URI as written, relative
 * to the glTF's folder; malloc'd bytes, NULL if missing */
typedef uint8_t *(*PropRead)(void *user, const char *rel, size_t *size);

/* the glTF (or .glb) in `data`; 0 with a reason on stderr if it can't be read (m is then empty) */
int propgltf_read(PropMesh *m, const uint8_t *data, size_t size, PropRead read, void *user);
void propgltf_free(PropMesh *m);

#endif
