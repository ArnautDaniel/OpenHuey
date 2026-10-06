/* The renderer: OpenGL 3.3. A frame is: render_begin, 3D draws (meshes), then 2D draws (text,
 * rectangles; queued and drawn together at render_end).
 *
 * Colours follow the PS2's convention where the game data does: vertex colours and texture
 * alpha use 0x80 for 1.0, and a texel is multiplied by the vertex colour. */
#ifndef RENDER_H
#define RENDER_H

#include "../core/mathx.h"
#include "../data/roommesh.h"

#include <stdint.h>

typedef unsigned int GpuTexture;   /* a GL texture name; 0 = none */

typedef struct GpuMesh {
    unsigned int vao, vbo;
    int nv;
} GpuMesh;

int render_init(void);
void render_shutdown(void);

void render_begin(int w, int h, Vec3 clear);
void render_end(void);

GpuTexture render_texture(const uint8_t *rgba, int w, int h);
void render_texture_free(GpuTexture t);

void render_mesh_upload(GpuMesh *g, const MeshVertex *v, int n);
void render_mesh_free(GpuMesh *g);
/* draw a mesh's draws; textures[i] for texture index i; groups: bit g shows group g (0 always) */
void render_mesh(const GpuMesh *g, const Mat4 *mvp, const MeshDraw *d, int nd, const GpuTexture *textures,
                 int ntextures, const uint32_t groups[8]);

/* 2D, in pixels from the top left; colours 0xRRGGBBAA */
void render_rect(float x, float y, float w, float h, uint32_t rgba);
void render_text(float x, float y, float scale, uint32_t rgba, const char *s, int n);
float render_text_width(float scale, int chars);
float render_line_height(float scale);

/* save the last drawn frame as a PNG; 0 on failure */
int render_screenshot(const char *path);

#endif
