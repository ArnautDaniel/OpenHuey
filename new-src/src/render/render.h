/* The renderer: OpenGL 4.6, written 4.5+ style (direct state access).
 *
 * A frame:
 *   render_begin       the window's size; picks the picture's rectangle (all of the window, or
 *                      a 4:3 one in the middle) and readies the scene targets
 *   render_camera      the camera for the 3D pass
 *   render_mesh ...    the scene: into an HDR (16-bit float) target, multisampled
 *   render_post        resolve, then ambient occlusion, bloom, fog, exposure, tone mapping,
 *                      grading, vignette and grain - onto the window
 *   render_rect/text   2D on top (the console, menus, hints)
 *   render_end
 *
 * Colours: the game's textures and vertex colours are display (gamma) values. They are turned
 * linear for lighting and blending, so with every effect off the picture matches the original;
 * vertex colours and texture alpha keep the PS2's 0x80 = 1.0. */
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

/* what the picture looks like: changed freely (from Forth: the `gfx` fields); takes effect on
 * the next frame */
typedef struct RenderSettings {
    int32_t msaa;            /* samples: 1 (off), 2, 4, 8 */
    float scale;             /* render resolution relative to the window: 0.5 .. 2 */
    int32_t aspect;          /* 0: fill the window (wider screens see more), 1: the original 4:3 */
    float anisotropy;        /* texture filtering: 1 .. 16 */
    int32_t ssao;            /* ambient occlusion */
    float ssao_radius;       /* room units */
    float ssao_strength;     /* 0 .. 1 */
    int32_t bloom;
    float bloom_threshold;   /* brightness where glow starts (linear) */
    float bloom_strength;
    int32_t fog;
    float fog_density;       /* per room unit */
    float fog_start;         /* room units from the eye */
    Vec3 fog_color;          /* display colour */
    float exposure;
    int32_t tonemap;         /* 0 clip, 1 soft shoulder (keeps the original look), 2 filmic (ACES) */
    float saturation;
    float contrast;
    float vignette;          /* 0 .. 1 */
    float grain;             /* 0 .. 0.1 */
    int32_t shadows;         /* soft contact shadows under characters */
    Vec3 light_dir;          /* characters' key light: the direction it comes from (world) */
    Vec3 light_color;        /* linear */
    Vec3 ambient;            /* linear */
    float rim;               /* rim light on characters' edges */
    int32_t room_fog;        /* the room's own fog, tint and bloom (part of the original look) */
    int32_t room_tint;
    int32_t room_bloom;
    int32_t debug;           /* show a buffer instead: 1 occlusion, 2 bloom, 3 depth (bands of 100 units),
                              * 4 the bloom mask */
} RenderSettings;

extern RenderSettings gRender;

/* the room's own look (PAC section 13, src/game/effects.c RoomEffects_TakeRoom): fog, the
 * two-colour tint and the bloom, all worked on display values as the PS2 did. Colours: r g b
 * with 1.0 = 0x80 (fog: 1.0 = 0xFF), a = the strength */
typedef struct RoomLook {
    int32_t has_fog;
    float fog_near_color[4], fog_far_color[4];   /* the ramp's ends; a: how opaque (0x80 = 1) */
    float fog_near, fog_far;                     /* view distance where it starts / ends */
    int32_t has_tint;
    float tint_glow[4];                          /* added: the screen blurred, times this */
    float tint_contrast[4];                      /* pushed away from: its blurred copy, times this */
    int32_t tint_sharp;                          /* not blurred first */
    int32_t has_bloom;
    float bloom[4];                              /* the masked areas' glow */
    int32_t bloom_subtract;
} RoomLook;

extern RoomLook gRoomLook;

int render_init(void);
void render_shutdown(void);

/* the frame: the window's size in pixels; `clear` the background (display colour) */
void render_begin(int window_w, int window_h, Vec3 clear);
/* the picture's aspect (width / height) - for the camera's projection */
float render_aspect(void);
/* the camera (for lighting, ambient occlusion and fog): projection, view, eye, depth range */
void render_camera(const Mat4 *proj, const Mat4 *view, Vec3 eye, float znear, float zfar);
/* finish the 3D scene: post effects onto the window */
void render_post(void);
void render_end(void);

GpuTexture render_texture(const uint8_t *rgba, int w, int h);
void render_texture_free(GpuTexture t);

void render_mesh_upload(GpuMesh *g, const MeshVertex *v, int n);
/* replace a mesh's vertices (meshes that change every frame: skinned characters) */
void render_mesh_update(GpuMesh *g, const MeshVertex *v, int n);
void render_mesh_free(GpuMesh *g);
/* draw a mesh's draws; textures[i] for texture index i; groups: bit g shows group g (0 always) */
void render_mesh(const GpuMesh *g, const Mat4 *mvp, const MeshDraw *d, int nd, const GpuTexture *textures,
                 int ntextures, const uint32_t groups[8]);

/* the contact-shadow texture (a soft round blob) */
GpuTexture render_blob_texture(void);

/* 2D, in window pixels from the top left; colours 0xRRGGBBAA */
void render_rect(float x, float y, float w, float h, uint32_t rgba);
void render_text(float x, float y, float scale, uint32_t rgba, const char *s, int n);
float render_text_width(float scale, int chars);
float render_line_height(float scale);

/* save the last drawn frame as a PNG; 0 on failure */
int render_screenshot(const char *path);

#endif
