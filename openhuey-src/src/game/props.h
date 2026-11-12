/* Props (mod API 1.5, docs/modding.md "Props"): a mod's own models - any glTF read through the
 * files layer (data/propgltf.h) - placed in a room, turned, scaled, shown or not, drawn in the
 * room's 3D pass after the things lying about, lit by the room's lights as the characters are.
 * None of the game's own: with no mod, there are none. */
#ifndef PROPS_H
#define PROPS_H

#include "../core/mathx.h"
#include "../render/render.h"

#define MAX_PROPS 64

typedef struct Prop {
    int used, shown;
    int room;            /* the room it is drawn in (-1: whichever is played - one held, say) */
    GpuMesh gpu;
    MeshDraw *d;
    int nd;
    GpuTexture *tex;
    int ntex;
    Vec3 lo, hi;         /* its bounds, as authored */
    Vec3 pos, rot;       /* placed: turned about X, then Y, then Z (as the room's objects), then moved */
    float scale;
    char path[128];
} Prop;

/* a prop from the glTF (or .glb) at `path` (as the files layer has it: a mod's library/data/...);
 * its buffers and images beside it. Not shown until placed. -1 if it can't be read (stderr says
 * why) or there's no slot left */
int props_load(const char *path);
void props_free(int i);
void props_clear(void);              /* every one gone (a new game's scripts) */
Prop *props_get(int i);              /* NULL if i isn't one */
Mat4 props_matrix(const Prop *p);    /* its model to the room */
Vec3 props_center(const Prop *p);    /* its middle in the room */

#endif
