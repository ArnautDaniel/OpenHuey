#include "render.h"

#include "../platform/gl.h"
#include "font.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- shaders ----
 * Uniforms and samplers have fixed locations and bindings in the GLSL, so nothing is looked up:
 * the mesh shader's u_mvp is location 0, u_use_tex 1, u_solid_tex 2, its texture unit 0; the
 * 2D shader's projection is location 0, the font unit 0. */

enum { U_MVP = 0, U_USE_TEX = 1, U_SOLID_TEX = 2 };
enum { U_PROJ = 0 };

static const char *kMeshVs =
    "#version 460 core\n"
    "layout(location = 0) in vec3 a_pos;\n"
    "layout(location = 1) in vec2 a_st;\n"
    "layout(location = 2) in vec4 a_col;\n"
    "layout(location = 0) uniform mat4 u_mvp;\n"
    "out vec2 v_st;\n"
    "out vec4 v_col;\n"
    "void main() {\n"
    "    gl_Position = u_mvp * vec4(a_pos, 1.0);\n"
    "    v_st = a_st;\n"
    "    v_col = a_col * (255.0 / 128.0);\n"   /* 0x80 = 1.0 */
    "}\n";

/* the GS's modulate: texel times vertex colour; texels with no alpha are dropped */
static const char *kMeshFs =
    "#version 460 core\n"
    "in vec2 v_st;\n"
    "in vec4 v_col;\n"
    "layout(binding = 0) uniform sampler2D u_tex;\n"
    "layout(location = 1) uniform int u_use_tex;\n"
    "layout(location = 2) uniform int u_solid_tex;\n"   /* the texture's alpha is not transparency */
    "out vec4 o_color;\n"
    "void main() {\n"
    "    vec4 c = v_col;\n"
    "    if (u_use_tex != 0) {\n"
    "        vec4 t = texture(u_tex, v_st);\n"
    "        if (u_solid_tex != 0) t.a = 1.0;\n"
    "        c *= t;\n"
    "        if (c.a < 1.0 / 255.0) discard;\n"
    "    }\n"
    "    o_color = clamp(c, 0.0, 1.0);\n"
    "}\n";

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

static GLuint shader(GLenum type, const char *src) {
    GLuint s = glCreateShader(type);
    GLint ok;

    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];

        glGetShaderInfoLog(s, sizeof(log), NULL, log);
        fprintf(stderr, "render: shader: %s\n", log);
    }
    return s;
}

static GLuint program(const char *vs, const char *fs) {
    GLuint p = glCreateProgram(), v = shader(GL_VERTEX_SHADER, vs), f = shader(GL_FRAGMENT_SHADER, fs);
    GLint ok;

    glAttachShader(p, v);
    glAttachShader(p, f);
    glLinkProgram(p);
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[1024];

        glGetProgramInfoLog(p, sizeof(log), NULL, log);
        fprintf(stderr, "render: program: %s\n", log);
    }
    glDetachShader(p, v);
    glDetachShader(p, f);
    glDeleteShader(v);
    glDeleteShader(f);
    return p;
}

/* ---- state ---- */

typedef struct Vertex2d {
    float x, y, u, v;
    uint8_t rgba[4];
} Vertex2d;

static struct {
    GLuint mesh_prog;
    GLuint prog2d, vao2d, vbo2d, font;
    Vertex2d *v2d;
    int n2d, cap2d;
    int w, h;
} R;

#define FONT_CELL_W 6
#define FONT_CELL_H 8
#define FONT_ATLAS_W (FONT_COUNT * FONT_CELL_W)   /* 576: rows stay 4-byte aligned */

/* a vertex array; its attributes read buffer binding 0 (attached with glVertexArrayVertexBuffer) */
static GLuint vertex_array(void) {
    GLuint vao;

    glCreateVertexArrays(1, &vao);
    return vao;
}

static void attribute(GLuint vao, GLuint index, GLint size, GLenum type, GLboolean normalized, GLuint offset) {
    glEnableVertexArrayAttrib(vao, index);
    glVertexArrayAttribFormat(vao, index, size, type, normalized, offset);
    glVertexArrayAttribBinding(vao, index, 0);
}

static GLuint texture(GLenum format, int w, int h, GLenum data_format, const void *data, GLint filter, GLint wrap) {
    GLuint t;

    glCreateTextures(GL_TEXTURE_2D, 1, &t);
    glTextureStorage2D(t, 1, format, w, h);
    glTextureSubImage2D(t, 0, 0, 0, w, h, data_format, GL_UNSIGNED_BYTE, data);
    glTextureParameteri(t, GL_TEXTURE_MIN_FILTER, filter);
    glTextureParameteri(t, GL_TEXTURE_MAG_FILTER, filter);
    glTextureParameteri(t, GL_TEXTURE_WRAP_S, wrap);
    glTextureParameteri(t, GL_TEXTURE_WRAP_T, wrap);
    return t;
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
    R.font = texture(GL_R8, FONT_ATLAS_W, FONT_CELL_H, GL_RED, px, GL_NEAREST, GL_CLAMP_TO_EDGE);
}

int render_init(void) {
    R.mesh_prog = program(kMeshVs, kMeshFs);
    R.prog2d = program(k2dVs, k2dFs);

    glCreateBuffers(1, &R.vbo2d);
    R.vao2d = vertex_array();
    attribute(R.vao2d, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2d, x));
    attribute(R.vao2d, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2d, u));
    attribute(R.vao2d, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex2d, rgba));
    glVertexArrayVertexBuffer(R.vao2d, 0, R.vbo2d, 0, sizeof(Vertex2d));
    make_font();
    return 1;
}

void render_shutdown(void) {
    free(R.v2d);
    R.v2d = NULL;
}

/* ---- frames ---- */

void render_begin(int w, int h, Vec3 clear) {
    const float colour[4] = {clear.x, clear.y, clear.z, 1.0f};
    const float depth = 1.0f;

    R.w = w;
    R.h = h;
    R.n2d = 0;
    glViewport(0, 0, w, h);
    glDepthMask(GL_TRUE);
    glClearNamedFramebufferfv(0, GL_COLOR, 0, colour);
    glClearNamedFramebufferfv(0, GL_DEPTH, 0, &depth);
}

static void flush_2d(void) {
    Mat4 proj = mat4_ortho2d((float)R.w, (float)R.h);

    if (R.n2d == 0) {
        return;
    }
    glNamedBufferData(R.vbo2d, (GLsizeiptr)(R.n2d * sizeof(Vertex2d)), R.v2d, GL_STREAM_DRAW);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glProgramUniformMatrix4fv(R.prog2d, U_PROJ, 1, GL_FALSE, proj.m);
    glUseProgram(R.prog2d);
    glBindTextureUnit(0, R.font);
    glBindVertexArray(R.vao2d);
    glDrawArrays(GL_TRIANGLES, 0, R.n2d);
    glBindVertexArray(0);
    R.n2d = 0;
}

void render_end(void) {
    flush_2d();
}

/* ---- textures ---- */

GpuTexture render_texture(const uint8_t *rgba, int w, int h) {
    return texture(GL_RGBA8, w, h, GL_RGBA, rgba, GL_LINEAR, GL_REPEAT);
}

void render_texture_free(GpuTexture t) {
    if (t != 0) {
        glDeleteTextures(1, &t);
    }
}

/* ---- meshes ---- */

static void mesh_arrays(GpuMesh *g) {
    g->vao = vertex_array();
    attribute(g->vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(MeshVertex, x));
    attribute(g->vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(MeshVertex, s));
    attribute(g->vao, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(MeshVertex, rgba));
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
    int i;

    if (g->vao == 0) {
        return;
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glProgramUniformMatrix4fv(R.mesh_prog, U_MVP, 1, GL_FALSE, mvp->m);
    glUseProgram(R.mesh_prog);
    glBindVertexArray(g->vao);
    for (i = 0; i < nd; i++) {
        const MeshDraw *x = &d[i];
        GLuint tex = x->texture >= 0 && x->texture < ntextures ? textures[x->texture] : 0;

        if (x->group != 0 && !(groups[x->group >> 5] >> (x->group & 31) & 1)) {
            continue;
        }
        if (x->blend || x->additive) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, x->additive ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA);
        } else {
            glDisable(GL_BLEND);
        }
        glDepthMask(x->no_zwrite ? GL_FALSE : GL_TRUE);
        glProgramUniform1i(R.mesh_prog, U_USE_TEX, tex != 0);
        glProgramUniform1i(R.mesh_prog, U_SOLID_TEX, x->solid_tex);
        glBindTextureUnit(0, tex);
        glDrawArrays(GL_TRIANGLES, x->first, x->count);
    }
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
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
