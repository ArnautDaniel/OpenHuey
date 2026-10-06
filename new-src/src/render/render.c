#include "render.h"

#include "../platform/gl.h"
#include "font.h"
#include "post.h"

#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

RenderSettings gRender = {
    .msaa = 4, .scale = 1.0f, .aspect = 0, .anisotropy = 16.0f,
    .ssao = 1, .ssao_radius = 20.0f, .ssao_strength = 0.9f,
    .bloom = 1, .bloom_threshold = 0.9f, .bloom_strength = 0.12f,
    .fog = 1, .fog_density = 0.0012f, .fog_start = 150.0f, .fog_color = {0.05f, 0.055f, 0.07f},
    .exposure = 1.0f, .tonemap = 1, .saturation = 1.0f, .contrast = 1.0f,
    .vignette = 0.35f, .grain = 0.025f,
    .shadows = 1,
    .light_dir = {0.35f, 0.85f, 0.4f}, .light_color = {0.55f, 0.52f, 0.48f}, .ambient = {0.16f, 0.17f, 0.2f},
    .rim = 0.35f,
    .room_fog = 1, .room_tint = 1, .room_bloom = 1,
};

RoomLook gRoomLook;

/* ---- the scene's shader ----
 * Fixed locations: u_mvp 0, u_use_tex 1, u_solid_tex 2, u_lit 3, u_coverage 4, u_light_dir 5,
 * u_light_color 6, u_ambient 7, u_eye 8, u_rim 9; the texture on unit 0. */

enum { U_MVP, U_USE_TEX, U_SOLID_TEX, U_LIT, U_COVERAGE, U_LIGHT_DIR, U_LIGHT_COLOR, U_AMBIENT, U_EYE, U_RIM, U_MASK };

static const char *kMeshVs =
    "#version 460 core\n"
    "layout(location = 0) in vec3 a_pos;\n"
    "layout(location = 1) in vec2 a_st;\n"
    "layout(location = 2) in vec4 a_col;\n"
    "layout(location = 3) in vec3 a_normal;\n"
    "layout(location = 0) uniform mat4 u_mvp;\n"
    "out vec2 v_st;\n"
    "out vec4 v_col;\n"
    "out vec3 v_normal;\n"
    "out vec3 v_pos;\n"
    "void main() {\n"
    "    gl_Position = u_mvp * vec4(a_pos, 1.0);\n"
    "    v_st = a_st;\n"
    "    v_col = a_col * (255.0 / 128.0);\n"   /* 0x80 = 1.0 */
    "    v_normal = a_normal;\n"
    "    v_pos = a_pos;\n"
    "}\n";

/* the GS's modulate (texel times vertex colour), done on linear values; characters lit by a key
 * light, the ambient and a rim; cut-out edges antialiased by alpha to coverage */
static const char *kMeshFs =
    "#version 460 core\n"
    "in vec2 v_st;\n"
    "in vec4 v_col;\n"
    "in vec3 v_normal;\n"
    "in vec3 v_pos;\n"
    "layout(binding = 0) uniform sampler2D u_tex;\n"
    "layout(location = 1) uniform int u_use_tex;\n"
    "layout(location = 2) uniform int u_solid_tex;\n"
    "layout(location = 3) uniform int u_lit;\n"
    "layout(location = 4) uniform int u_coverage;\n"
    "layout(location = 5) uniform vec3 u_light_dir;\n"
    "layout(location = 6) uniform vec3 u_light_color;\n"
    "layout(location = 7) uniform vec3 u_ambient;\n"
    "layout(location = 8) uniform vec3 u_eye;\n"
    "layout(location = 9) uniform float u_rim;\n"
    "layout(location = 10) uniform float u_mask;\n"   /* the bloom mask: 1 for its own draws */
    "layout(location = 0) out vec4 o_color;\n"
    "layout(location = 1) out float o_mask;\n"
    "void main() {\n"
    "    vec4 c = vec4(pow(max(v_col.rgb, 0.0), vec3(2.2)), v_col.a);\n"
    "    if (u_use_tex != 0) {\n"
    "        vec4 t = texture(u_tex, v_st);\n"   /* sRGB texture: already linear */
    "        if (u_solid_tex != 0) t.a = 1.0;\n"
    "        c *= t;\n"
    "    }\n"
    "    if (u_lit != 0) {\n"
    "        vec3 n = normalize(v_normal), v = normalize(u_eye - v_pos);\n"
    "        float key = max(dot(n, normalize(u_light_dir)), 0.0);\n"
    "        float rim = pow(1.0 - max(dot(n, v), 0.0), 3.0) * u_rim;\n"
    "        c.rgb *= u_ambient * 2.0 + u_light_color * key * 2.0;\n"
    "        c.rgb += rim * (u_ambient + u_light_color) * c.a;\n"
    "    }\n"
    "    if (u_use_tex != 0) {\n"
    "        if (u_coverage != 0) {\n"   /* a sharp edge where alpha crosses the cut-off */
    "            c.a = clamp((c.a - 0.02) / max(fwidth(c.a), 1e-4) + 0.5, 0.0, 1.0);\n"
    "            if (c.a <= 0.0) discard;\n"
    "        } else if (c.a < 1.0 / 255.0) {\n"
    "            discard;\n"
    "        }\n"
    "    }\n"
    "    o_color = c;\n"
    "    o_mask = u_mask;\n"
    "}\n";

/* ---- state ---- */

typedef struct Vertex2d {
    float x, y, u, v;
    uint8_t rgba[4];
} Vertex2d;

static struct {
    GLuint mesh_prog, sampler;
    float sampler_aniso;
    GLuint prog2d, vao2d, vbo2d, font, blob;
    Vertex2d *v2d;
    int n2d, cap2d;
    int w, h;                   /* the window */
    int px, py, pw, ph;         /* the picture's rectangle in it (GL: from the bottom left) */
    Mat4 proj;
    float znear, zfar;
} R;

#define FONT_CELL_W 6
#define FONT_CELL_H 8
#define FONT_ATLAS_W (FONT_COUNT * FONT_CELL_W)   /* 576: rows stay 4-byte aligned */

static const char *k2dVs =
    "#version 460 core\n"
    "layout(location = 0) in vec2 a_pos;\n"
    "layout(location = 1) in vec2 a_uv;\n"
    "layout(location = 2) in vec4 a_col;\n"
    "layout(location = 0) uniform mat4 u_proj;\n"
    "out vec2 v_uv;\n"
    "out vec4 v_col;\n"
    "void main() {\n"
    "    gl_Position = u_proj * vec4(a_pos, 0.0, 1.0);\n"
    "    v_uv = a_uv;\n"
    "    v_col = a_col;\n"
    "}\n";

static const char *k2dFs =   /* the font is a coverage mask */
    "#version 460 core\n"
    "in vec2 v_uv;\n"
    "in vec4 v_col;\n"
    "layout(binding = 0) uniform sampler2D u_font;\n"
    "out vec4 o_color;\n"
    "void main() {\n"
    "    o_color = vec4(v_col.rgb, v_col.a * texture(u_font, v_uv).r);\n"
    "}\n";

static void attribute(GLuint vao, GLuint index, GLint size, GLenum type, GLboolean normalized, GLuint offset) {
    glEnableVertexArrayAttrib(vao, index);
    glVertexArrayAttribFormat(vao, index, size, type, normalized, offset);
    glVertexArrayAttribBinding(vao, index, 0);
}

static void make_font(void) {
    static uint8_t px[FONT_CELL_H][FONT_ATLAS_W];
    int c, x, y;

    for (c = 0; c < FONT_COUNT; c++) {
        for (x = 0; x < 5; x++) {
            for (y = 0; y < 7; y++) {
                px[y][c * FONT_CELL_W + x] = (kFont5x7[c][x] >> y) & 1 ? 255 : 0;
            }
        }
    }
    glCreateTextures(GL_TEXTURE_2D, 1, &R.font);
    glTextureStorage2D(R.font, 1, GL_R8, FONT_ATLAS_W, FONT_CELL_H);
    glTextureSubImage2D(R.font, 0, 0, 0, FONT_ATLAS_W, FONT_CELL_H, GL_RED, GL_UNSIGNED_BYTE, px);
    glTextureParameteri(R.font, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(R.font, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(R.font, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(R.font, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

/* a soft round shadow: white, its alpha falling off from the middle */
static void make_blob(void) {
    static uint8_t px[64][64][4];
    int x, y;

    for (y = 0; y < 64; y++) {
        for (x = 0; x < 64; x++) {
            float dx = (x + 0.5f) / 32.0f - 1.0f, dy = (y + 0.5f) / 32.0f - 1.0f, r = sqrtf(dx * dx + dy * dy);
            float a = r >= 1.0f ? 0.0f : (1.0f - r) * (1.0f - r) * (3.0f - 2.0f * (1.0f - r));

            px[y][x][0] = px[y][x][1] = px[y][x][2] = 255;
            px[y][x][3] = (uint8_t)(a * 128.0f);   /* (0x80 = opaque) */
        }
    }
    R.blob = render_texture(&px[0][0][0], 64, 64);
}

int render_init(void) {
    R.mesh_prog = gl_program(kMeshVs, kMeshFs, "mesh");
    R.prog2d = gl_program(k2dVs, k2dFs, "2d");
    if (R.mesh_prog == 0 || R.prog2d == 0) {
        return 0;
    }
    /* the scene's textures: trilinear and anisotropic, repeating */
    glCreateSamplers(1, &R.sampler);
    glSamplerParameteri(R.sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(R.sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(R.sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(R.sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glCreateBuffers(1, &R.vbo2d);
    glCreateVertexArrays(1, &R.vao2d);
    attribute(R.vao2d, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2d, x));
    attribute(R.vao2d, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2d, u));
    attribute(R.vao2d, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex2d, rgba));
    glVertexArrayVertexBuffer(R.vao2d, 0, R.vbo2d, 0, sizeof(Vertex2d));
    make_font();
    make_blob();
    post_init();
    return 1;
}

void render_shutdown(void) {
    free(R.v2d);
    R.v2d = NULL;
}

/* ---- frames ---- */

static int clampi(int x, int lo, int hi) {
    return x < lo ? lo : x > hi ? hi : x;
}

static float lin(float c) {
    return powf(c, 2.2f);
}

void render_begin(int w, int h, Vec3 clear) {
    RenderSettings *s = &gRender;
    int samples = s->msaa >= 8 ? 8 : s->msaa >= 4 ? 4 : s->msaa >= 2 ? 2 : 1;
    float scale = s->scale < 0.25f ? 0.25f : s->scale > 2.0f ? 2.0f : s->scale;
    const float black[4] = {0, 0, 0, 1};

    R.w = w;
    R.h = h;
    R.n2d = 0;
    /* the picture: the whole window, or the largest 4:3 rectangle in its middle */
    R.pw = w;
    R.ph = h;
    if (s->aspect == 1) {
        if (w * 3 > h * 4) {
            R.pw = h * 4 / 3;
        } else {
            R.ph = w * 3 / 4;
        }
    }
    R.px = (w - R.pw) / 2;
    R.py = (h - R.ph) / 2;
    glClearNamedFramebufferfv(0, GL_COLOR, 0, black);   /* (the bars, when there are any) */
    if (s->anisotropy != R.sampler_aniso) {
        R.sampler_aniso = s->anisotropy;
        glSamplerParameterf(R.sampler, GL_TEXTURE_MAX_ANISOTROPY, s->anisotropy < 1.0f ? 1.0f : s->anisotropy);
    }
    post_begin(clampi((int)(R.pw * scale), 16, 8192), clampi((int)(R.ph * scale), 16, 8192), samples,
               vec3(lin(clear.x), lin(clear.y), lin(clear.z)));
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
}

float render_aspect(void) {
    int w = R.w, h = R.h;

    if (gRender.aspect == 1) {
        return 4.0f / 3.0f;
    }
    return h > 0 ? (float)w / (float)h : 1.0f;
}

void render_camera(const Mat4 *proj, const Mat4 *view, Vec3 eye, float znear, float zfar) {
    Vec3 l = vec3_norm(gRender.light_dir);

    (void)view;
    R.proj = *proj;
    R.znear = znear;
    R.zfar = zfar;
    glProgramUniform3f(R.mesh_prog, U_LIGHT_DIR, l.x, l.y, l.z);
    glProgramUniform3f(R.mesh_prog, U_LIGHT_COLOR, gRender.light_color.x, gRender.light_color.y, gRender.light_color.z);
    glProgramUniform3f(R.mesh_prog, U_AMBIENT, gRender.ambient.x, gRender.ambient.y, gRender.ambient.z);
    glProgramUniform3f(R.mesh_prog, U_EYE, eye.x, eye.y, eye.z);
    glProgramUniform1f(R.mesh_prog, U_RIM, gRender.rim);
}

void render_post(void) {
    PostCamera cam = {R.proj, R.znear, R.zfar};

    post_finish(&cam, &gRender, R.px, R.py, R.pw, R.ph, (float)(SDL_GetTicks() % 100000) / 1000.0f);
    glViewport(0, 0, R.w, R.h);
}

static void flush_2d(void) {
    Mat4 proj = mat4_ortho2d((float)R.w, (float)R.h);

    if (R.n2d == 0) {
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, R.w, R.h);
    glNamedBufferData(R.vbo2d, (GLsizeiptr)(R.n2d * sizeof(Vertex2d)), R.v2d, GL_STREAM_DRAW);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glProgramUniformMatrix4fv(R.prog2d, 0, 1, GL_FALSE, proj.m);
    glUseProgram(R.prog2d);
    glBindTextureUnit(0, R.font);
    glBindSampler(0, 0);
    glBindVertexArray(R.vao2d);
    glDrawArrays(GL_TRIANGLES, 0, R.n2d);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    R.n2d = 0;
}

void render_end(void) {
    flush_2d();
}

/* ---- textures: sRGB (the game's colours are display values), with mipmaps ---- */

GpuTexture render_texture(const uint8_t *rgba, int w, int h) {
    GLuint t;
    int levels = 1, m = w > h ? w : h;

    while (m > 1) {
        m >>= 1;
        levels++;
    }
    glCreateTextures(GL_TEXTURE_2D, 1, &t);
    glTextureStorage2D(t, levels, GL_SRGB8_ALPHA8, w, h);
    glTextureSubImage2D(t, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
    glGenerateTextureMipmap(t);
    return t;
}

void render_texture_free(GpuTexture t) {
    if (t != 0) {
        glDeleteTextures(1, &t);
    }
}

GpuTexture render_blob_texture(void) {
    return R.blob;
}

/* ---- meshes ---- */

static void mesh_arrays(GpuMesh *g) {
    glCreateVertexArrays(1, &g->vao);
    attribute(g->vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(MeshVertex, x));
    attribute(g->vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(MeshVertex, s));
    attribute(g->vao, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(MeshVertex, rgba));
    attribute(g->vao, 3, 3, GL_FLOAT, GL_FALSE, offsetof(MeshVertex, nx));
    glVertexArrayVertexBuffer(g->vao, 0, g->vbo, 0, sizeof(MeshVertex));
}

/* a mesh that never changes: immutable storage */
void render_mesh_upload(GpuMesh *g, const MeshVertex *v, int n) {
    glCreateBuffers(1, &g->vbo);
    glNamedBufferStorage(g->vbo, (GLsizeiptr)((size_t)n * sizeof(MeshVertex)), v, 0);
    mesh_arrays(g);
    g->nv = n;
}

/* a mesh refilled every frame (skinned characters): storage that can be replaced */
void render_mesh_update(GpuMesh *g, const MeshVertex *v, int n) {
    if (g->vao == 0) {
        glCreateBuffers(1, &g->vbo);
        mesh_arrays(g);
    }
    glNamedBufferData(g->vbo, (GLsizeiptr)((size_t)n * sizeof(MeshVertex)), v, GL_STREAM_DRAW);
    g->nv = n;
}

void render_mesh_free(GpuMesh *g) {
    if (g->vbo != 0) {
        glDeleteBuffers(1, &g->vbo);
        glDeleteVertexArrays(1, &g->vao);
    }
    memset(g, 0, sizeof(*g));
}

void render_mesh(const GpuMesh *g, const Mat4 *mvp, const MeshDraw *d, int nd, const GpuTexture *textures,
                 int ntextures, const uint32_t groups[8]) {
    int i, multisampled = gRender.msaa > 1;

    if (g->vao == 0) {
        return;
    }
    glProgramUniformMatrix4fv(R.mesh_prog, U_MVP, 1, GL_FALSE, mvp->m);
    glUseProgram(R.mesh_prog);
    glDisable(GL_BLEND);   /* (for both targets: only the colour one ever blends) */
    glBindSampler(0, R.sampler);
    glBindVertexArray(g->vao);
    for (i = 0; i < nd; i++) {
        const MeshDraw *x = &d[i];
        GLuint tex = x->texture >= 0 && x->texture < ntextures ? textures[x->texture] : 0;
        int coverage = multisampled && tex != 0 && !x->blend && !x->additive;

        if (x->group != 0 && !(groups[x->group >> 5] >> (x->group & 31) & 1)) {
            continue;
        }
        if (x->mask) {   /* the bloom mask: marks where it shows, draws no colour */
            glDisable(GL_BLEND);
            glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        } else if (x->blend || x->additive) {
            glEnablei(GL_BLEND, 0);
            glBlendFunc(GL_SRC_ALPHA, x->additive ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA);
        } else {
            glDisable(GL_BLEND);
        }
        if (coverage) {
            glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        } else {
            glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        }
        glDepthMask(x->no_zwrite || x->mask ? GL_FALSE : GL_TRUE);
        glProgramUniform1f(R.mesh_prog, U_MASK, x->mask ? 1.0f : 0.0f);
        glProgramUniform1i(R.mesh_prog, U_USE_TEX, tex != 0);
        glProgramUniform1i(R.mesh_prog, U_SOLID_TEX, x->solid_tex);
        glProgramUniform1i(R.mesh_prog, U_LIT, x->lit);
        glProgramUniform1i(R.mesh_prog, U_COVERAGE, coverage);
        glBindTextureUnit(0, tex);
        glDrawArrays(GL_TRIANGLES, x->first, x->count);
        if (x->mask) {
            glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        }
    }
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

/* ---- 2D ---- */

static void quad(float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, uint32_t rgba) {
    Vertex2d q[4];
    static const int kOrder[6] = {0, 1, 2, 0, 2, 3};
    int i;

    if (R.n2d + 6 > R.cap2d) {
        R.cap2d = R.cap2d * 2 + 6 * 256;
        R.v2d = realloc(R.v2d, (size_t)R.cap2d * sizeof(Vertex2d));
    }
    q[0] = (Vertex2d){x0, y0, u0, v0, {0}};
    q[1] = (Vertex2d){x1, y0, u1, v0, {0}};
    q[2] = (Vertex2d){x1, y1, u1, v1, {0}};
    q[3] = (Vertex2d){x0, y1, u0, v1, {0}};
    for (i = 0; i < 4; i++) {
        q[i].rgba[0] = (uint8_t)(rgba >> 24);
        q[i].rgba[1] = (uint8_t)(rgba >> 16);
        q[i].rgba[2] = (uint8_t)(rgba >> 8);
        q[i].rgba[3] = (uint8_t)rgba;
    }
    for (i = 0; i < 6; i++) {
        R.v2d[R.n2d++] = q[kOrder[i]];
    }
}

void render_rect(float x, float y, float w, float h, uint32_t rgba) {
    /* the middle of the solid block glyph (127) */
    float u = ((FONT_COUNT - 1) * FONT_CELL_W + 2.5f) / FONT_ATLAS_W, v = 3.5f / FONT_CELL_H;

    quad(x, y, x + w, y + h, u, v, u, v, rgba);
}

void render_text(float x, float y, float scale, uint32_t rgba, const char *s, int n) {
    int i;

    for (i = 0; i < n && s[i] != 0; i++) {
        int c = (unsigned char)s[i];

        if (c >= FONT_FIRST && c < FONT_FIRST + FONT_COUNT && c != ' ') {
            float u0 = (float)((c - FONT_FIRST) * FONT_CELL_W) / FONT_ATLAS_W;
            float u1 = (float)((c - FONT_FIRST + 1) * FONT_CELL_W) / FONT_ATLAS_W;

            quad(x, y, x + FONT_CELL_W * scale, y + FONT_CELL_H * scale, u0, 0.0f, u1, 1.0f, rgba);
        }
        x += FONT_CELL_W * scale;
    }
}

float render_text_width(float scale, int chars) {
    return (float)chars * FONT_CELL_W * scale;
}

float render_line_height(float scale) {
    return (FONT_CELL_H + 2) * scale;
}

/* ---- screenshots: a PNG written by hand (uncompressed deflate blocks) ---- */

static uint32_t crc32_update(uint32_t crc, const uint8_t *p, size_t n) {
    static uint32_t table[256];
    size_t i;

    if (table[1] == 0) {
        uint32_t c, k, j;

        for (k = 0; k < 256; k++) {
            for (c = k, j = 0; j < 8; j++) {
                c = c & 1 ? 0xEDB88320u ^ (c >> 1) : c >> 1;
            }
            table[k] = c;
        }
    }
    crc = ~crc;
    for (i = 0; i < n; i++) {
        crc = table[(crc ^ p[i]) & 0xFF] ^ (crc >> 8);
    }
    return ~crc;
}

static void put32(uint8_t *p, uint32_t x) {
    p[0] = (uint8_t)(x >> 24);
    p[1] = (uint8_t)(x >> 16);
    p[2] = (uint8_t)(x >> 8);
    p[3] = (uint8_t)x;
}

static void chunk(FILE *fp, const char *type, const uint8_t *data, size_t n) {
    uint8_t head[8], tail[4];
    uint32_t crc;

    put32(head, (uint32_t)n);
    memcpy(head + 4, type, 4);
    crc = crc32_update(0, head + 4, 4);
    crc = crc32_update(crc, data, n);
    put32(tail, crc);
    fwrite(head, 1, 8, fp);
    fwrite(data, 1, n, fp);
    fwrite(tail, 1, 4, fp);
}

int render_screenshot(const char *path) {
    int w = R.w, h = R.h, y;
    size_t row = (size_t)w * 3 + 1, raw_n = row * (size_t)h, blocks = (raw_n + 65534) / 65535;
    uint8_t *px = malloc((size_t)w * h * 4), *raw = malloc(raw_n), *z = malloc(raw_n + blocks * 5 + 6);
    uint8_t ihdr[13];
    uint32_t a = 1, b = 0;
    size_t i, zn = 0;
    FILE *fp = fopen(path, "wb");

    if (fp == NULL || px == NULL || raw == NULL || z == NULL) {
        free(px);
        free(raw);
        free(z);
        if (fp != NULL) {
            fclose(fp);
        }
        return 0;
    }
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glReadnPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, (GLsizei)((size_t)w * h * 4), px);
    for (y = 0; y < h; y++) {   /* GL's rows go up; PNG's go down */
        const uint8_t *src = px + (size_t)(h - 1 - y) * w * 4;
        uint8_t *dst = raw + (size_t)y * row;
        int x;

        dst[0] = 0;   /* (no filter) */
        for (x = 0; x < w; x++) {
            memcpy(dst + 1 + x * 3, src + x * 4, 3);
        }
    }
    z[zn++] = 0x78;
    z[zn++] = 0x01;
    for (i = 0; i < raw_n; i += 65535) {
        size_t n = raw_n - i < 65535 ? raw_n - i : 65535;

        z[zn++] = i + n == raw_n;   /* the last block? */
        z[zn++] = (uint8_t)n;
        z[zn++] = (uint8_t)(n >> 8);
        z[zn++] = (uint8_t)~n;
        z[zn++] = (uint8_t)(~n >> 8);
        memcpy(z + zn, raw + i, n);
        zn += n;
    }
    for (i = 0; i < raw_n; i++) {   /* Adler-32 */
        a = (a + raw[i]) % 65521;
        b = (b + a) % 65521;
    }
    put32(z + zn, b << 16 | a);
    zn += 4;
    put32(ihdr, (uint32_t)w);
    put32(ihdr + 4, (uint32_t)h);
    ihdr[8] = 8;    /* bits */
    ihdr[9] = 2;    /* RGB */
    ihdr[10] = ihdr[11] = ihdr[12] = 0;
    fwrite("\x89PNG\r\n\x1a\n", 1, 8, fp);
    chunk(fp, "IHDR", ihdr, 13);
    chunk(fp, "IDAT", z, zn);
    chunk(fp, "IEND", NULL, 0);
    fclose(fp);
    free(px);
    free(raw);
    free(z);
    return 1;
}
