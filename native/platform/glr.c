/* The PC renderer: OpenGL 4.6 core with direct state access (see glr.h).
 *
 * The scene is drawn at the PS2's 640x448 into an offscreen target (glr_present), which is
 * then scaled into the window; frame dumps read the offscreen target.
 *
 *   HG_GLDEBUG=1     once a second: the frame's draw count and its first vertex in clip space
 *   HG_GLDEBUG_W=1   with it, the flags words of the first draws' vertices */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <GL/glcorearb.h>

#include "glr.h"

/* ---- the GL functions used (loaded through SDL) ---- */

#define GLR_FUNCS(X) \
    X(PFNGLCREATEBUFFERSPROC, glCreateBuffers) \
    X(PFNGLNAMEDBUFFERDATAPROC, glNamedBufferData) \
    X(PFNGLCREATEVERTEXARRAYSPROC, glCreateVertexArrays) \
    X(PFNGLVERTEXARRAYVERTEXBUFFERPROC, glVertexArrayVertexBuffer) \
    X(PFNGLENABLEVERTEXARRAYATTRIBPROC, glEnableVertexArrayAttrib) \
    X(PFNGLVERTEXARRAYATTRIBFORMATPROC, glVertexArrayAttribFormat) \
    X(PFNGLVERTEXARRAYATTRIBBINDINGPROC, glVertexArrayAttribBinding) \
    X(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray) \
    X(PFNGLCREATESHADERPROC, glCreateShader) \
    X(PFNGLSHADERSOURCEPROC, glShaderSource) \
    X(PFNGLCOMPILESHADERPROC, glCompileShader) \
    X(PFNGLGETSHADERIVPROC, glGetShaderiv) \
    X(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog) \
    X(PFNGLCREATEPROGRAMPROC, glCreateProgram) \
    X(PFNGLATTACHSHADERPROC, glAttachShader) \
    X(PFNGLLINKPROGRAMPROC, glLinkProgram) \
    X(PFNGLGETPROGRAMIVPROC, glGetProgramiv) \
    X(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog) \
    X(PFNGLUSEPROGRAMPROC, glUseProgram) \
    X(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation) \
    X(PFNGLPROGRAMUNIFORMMATRIX4FVPROC, glProgramUniformMatrix4fv) \
    X(PFNGLPROGRAMUNIFORM1IPROC, glProgramUniform1i) \
    X(PFNGLCREATETEXTURESPROC, glCreateTextures) \
    X(PFNGLTEXTURESTORAGE2DPROC, glTextureStorage2D) \
    X(PFNGLTEXTURESUBIMAGE2DPROC, glTextureSubImage2D) \
    X(PFNGLTEXTUREPARAMETERIPROC, glTextureParameteri) \
    X(PFNGLBINDTEXTUREUNITPROC, glBindTextureUnit) \
    X(PFNGLDELETETEXTURESPROC, glDeleteTextures) \
    X(PFNGLCREATEFRAMEBUFFERSPROC, glCreateFramebuffers) \
    X(PFNGLNAMEDFRAMEBUFFERTEXTUREPROC, glNamedFramebufferTexture) \
    X(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer) \
    X(PFNGLBLITNAMEDFRAMEBUFFERPROC, glBlitNamedFramebuffer) \
    X(PFNGLCLEARNAMEDFRAMEBUFFERFVPROC, glClearNamedFramebufferfv) \
    X(PFNGLREADPIXELSPROC, glReadPixels) \
    X(PFNGLVIEWPORTPROC, glViewport) \
    X(PFNGLENABLEPROC, glEnable) \
    X(PFNGLDISABLEPROC, glDisable) \
    X(PFNGLDEPTHFUNCPROC, glDepthFunc) \
    X(PFNGLDEPTHMASKPROC, glDepthMask) \
    X(PFNGLBLENDFUNCPROC, glBlendFunc) \
    X(PFNGLDRAWARRAYSPROC, glDrawArrays) \
    X(PFNGLPIXELSTOREIPROC, glPixelStorei) \
    X(PFNGLGETSTRINGPROC, glGetString) \
    X(PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback)

#define GLR_DECLARE(type, name) static type p_##name;
GLR_FUNCS(GLR_DECLARE)

/* ---- the frame's strips ---- */

typedef struct GlrVertex {
    float x, y, z, w;
    float s, t;
    uint8_t rgba[4];
} GlrVertex;

typedef struct GlrDraw {
    float mvp[16];
    int first, n;
    const uint8_t *tex;
    uint64_t tex0;
    uint32_t prim;
} GlrDraw;

typedef struct GlrFrame {
    GlrVertex *v;
    int nv, capv;
    GlrDraw *d;
    int nd, capd;
} GlrFrame;

static GlrFrame sFrames[2];
static int sBuilding;   /* index of the frame being built; the other one is shown */

static void put_vertex(GlrFrame *f, const float *xyzw, const float *st, const uint8_t *rgba, int i) {
    GlrVertex *v = &f->v[f->nv++];

    v->x = xyzw[i * 4 + 0];
    v->y = xyzw[i * 4 + 1];
    v->z = xyzw[i * 4 + 2];
    v->w = xyzw[i * 4 + 3];
    v->s = st[i * 2 + 0];
    v->t = st[i * 2 + 1];
    memcpy(v->rgba, rgba + i * 4, 4);
}

/* The strip's w words are GS XYZ2 / XYZ3 flags, not coordinates: bit 15 (ADC) set means the
 * triangle ending at that vertex isn't drawn, which is how one VU1 batch holds many strips. The
 * strip becomes a triangle list without those. */
void glr_strip(const float mvp[16], int n, const float *xyzw, const float *st, const uint8_t *rgba,
               const void *tex, uint64_t tex0, uint32_t prim) {
    GlrFrame *f = &sFrames[sBuilding];
    GlrDraw *d;
    int i, max = (n - 2) * 3;

    if (n < 3) {
        return;
    }
    if (f->nv + max > f->capv) {
        f->capv = (f->nv + max) * 2 + 4096;
        f->v = realloc(f->v, f->capv * sizeof(GlrVertex));
    }
    if (f->nd + 1 > f->capd) {
        f->capd = f->capd * 2 + 256;
        f->d = realloc(f->d, f->capd * sizeof(GlrDraw));
    }
    d = &f->d[f->nd];
    d->first = f->nv;
    for (i = 2; i < n; i++) {
        uint32_t flags;

        memcpy(&flags, &xyzw[i * 4 + 3], 4);
        if (flags & 0x8000) {
            continue;
        }
        put_vertex(f, xyzw, st, rgba, i - 2);
        put_vertex(f, xyzw, st, rgba, i - 1);
        put_vertex(f, xyzw, st, rgba, i);
    }
    d->n = f->nv - d->first;
    if (d->n == 0) {
        return;
    }
    memcpy(d->mvp, mvp, sizeof(d->mvp));
    d->tex = tex;
    d->tex0 = tex0;
    d->prim = prim;
    f->nd++;
}

/* a draw path that still builds PS2 packets only: reported once, drawn as nothing */
void glr_todo(const char *what) {
    static const char *seen[64];
    static int n;
    int i;

    for (i = 0; i < n; i++) {
        if (seen[i] == what) {
            return;
        }
    }
    if (n < 64) {
        seen[n++] = what;
    }
    fprintf(stderr, "glr: %s not drawn with OpenGL yet\n", what);
}

void glr_end_frame(void) {
    sBuilding ^= 1;
    sFrames[sBuilding].nv = 0;
    sFrames[sBuilding].nd = 0;
}

/* ---- GL objects ---- */

#define GLR_WIDTH 640
#define GLR_HEIGHT 448

static GLuint sMeshProg, sQuadProg, sVao, sQuadVao, sVbo;
static GLint sMvpLoc, sTexModeLoc, sTccLoc;
static GLuint sFbo, sColor, sDepth, sGsTex;
static int sGsW, sGsH;

static const char *kMeshVs =
    "#version 460 core\n"
    "layout(location = 0) in vec4 aPos;\n"
    "layout(location = 1) in vec2 aSt;\n"
    "layout(location = 2) in vec4 aCol;\n"
    "uniform mat4 uMvp;\n"
    "out vec4 vCol;\n"
    "out vec2 vSt;\n"
    "void main() {\n"
    "    vec4 p = uMvp * vec4(aPos.xyz, 1.0);\n"
    /* PS2 clip space: y grows downwards, larger z is nearer */
    "    gl_Position = vec4(p.x, -p.y, -p.z, p.w);\n"
    "    vCol = aCol * (255.0 / 128.0);\n"
    "    vSt = aSt;\n"
    "}\n";

/* the GS texture function: modulate (vertex colour times texel, 0x80 = 1.0) or decal; TCC
 * takes the texel's alpha (0x80 = 1.0 too). Fully transparent texels are dropped (the
 * game's alpha test). */
static const char *kMeshFs =
    "#version 460 core\n"
    "in vec4 vCol;\n"
    "in vec2 vSt;\n"
    "uniform sampler2D uTex;\n"
    "uniform int uTexMode;\n"   /* 0 none, 1 modulate, 2 decal */
    "uniform int uTcc;\n"
    "out vec4 oColor;\n"
    "void main() {\n"
    "    vec4 c = vCol;\n"
    "    if (uTexMode != 0) {\n"
    "        vec4 t = texture(uTex, vSt);\n"
    "        t.a *= 255.0 / 128.0;\n"
    "        c.rgb = uTexMode == 1 ? c.rgb * t.rgb : t.rgb;\n"
    "        if (uTcc != 0) {\n"
    "            c.a = uTexMode == 1 ? c.a * t.a : t.a;\n"
    "        }\n"
    "        if (c.a < 1.0 / 255.0) {\n"
    "            discard;\n"
    "        }\n"
    "    }\n"
    "    oColor = clamp(c, 0.0, 1.0);\n"
    "}\n";

/* a full-screen triangle pair from gl_VertexID, showing the software GS frame */
static const char *kQuadVs =
    "#version 460 core\n"
    "out vec2 vUv;\n"
    "void main() {\n"
    "    vec2 p = vec2((gl_VertexID & 1) != 0 ? 1.0 : -1.0, (gl_VertexID & 2) != 0 ? 1.0 : -1.0);\n"
    "    vUv = vec2(p.x * 0.5 + 0.5, 0.5 - p.y * 0.5);\n"
    "    gl_Position = vec4(p, 0.0, 1.0);\n"
    "}\n";

static const char *kQuadFs =
    "#version 460 core\n"
    "in vec2 vUv;\n"
    "uniform sampler2D uTex;\n"
    "out vec4 oColor;\n"
    "void main() {\n"
    "    oColor = vec4(texture(uTex, vUv).rgb, 1.0);\n"
    "}\n";

/* ---- textures: decoded from the game's .TEX entries in memory ---- */

/* a .TEX entry (as src/game/renderer.c's TexHeader): the image (rows of w pixels in `psm`)
 * then the CLUT (a 16 x 16 block of `cpsm` colours as uploaded; indexed formats look entries up
 * in the GS's CSM1 order), both from the entry */
typedef struct TexEntry {
    uint8_t psm, cpsm, pad[2];
    uint16_t w, h, imageQwc, clutQwc;
    int32_t data;
} TexEntry;

enum { PSMCT32 = 0x00, PSMCT24 = 0x01, PSMCT16 = 0x02, PSMT8 = 0x13, PSMT4 = 0x14 };

static uint32_t ct16(uint16_t c) {   /* alpha bit set: 0x80 */
    return (uint32_t)(c & 0x1F) << 3 | (uint32_t)((c >> 5) & 0x1F) << 11 | (uint32_t)((c >> 10) & 0x1F) << 19 |
           (c & 0x8000 ? 0x80000000u : 0);
}

static uint32_t clut_entry(const TexEntry *t, const uint8_t *clut, int i, int n) {
    int x, y, k;

    if (n == 256) {   /* CSM1: entries 8..15 and 16..23 of each 32 swapped */
        x = i % 8 + ((i / 16) % 2) * 8;
        y = (i / 32) * 2 + (i / 8) % 2;
    } else {
        x = i & 7;
        y = i >> 3;
    }
    k = y * 16 + x;
    if (t->cpsm == PSMCT16) {
        return k * 2 < t->clutQwc * 16 ? ct16((uint16_t)(clut[k * 2] | clut[k * 2 + 1] << 8)) : 0;
    }
    if (k * 4 >= t->clutQwc * 16) {
        return 0;
    }
    return (uint32_t)clut[k * 4] | (uint32_t)clut[k * 4 + 1] << 8 | (uint32_t)clut[k * 4 + 2] << 16 |
           (uint32_t)clut[k * 4 + 3] << 24;
}

/* RGBA8 texels (alpha as the GS keeps it, 0x80 = 1.0); 0 for a format not handled */
static int tex_decode(const TexEntry *t, uint32_t *out) {
    const uint8_t *img = (const uint8_t *)t + t->data, *clut = img + t->imageQwc * 16;
    uint32_t pal[256];
    int w = t->w, h = t->h, i, n;

    switch (t->psm) {
    case PSMT8:
    case PSMT4:
        n = t->psm == PSMT8 ? 256 : 16;
        for (i = 0; i < n; i++) {
            pal[i] = clut_entry(t, clut, i, n);
        }
        for (i = 0; i < w * h; i++) {
            out[i] = t->psm == PSMT8 ? pal[img[i]] : pal[(img[i >> 1] >> ((i & 1) * 4)) & 0xF];
        }
        return 1;
    case PSMCT32:
        memcpy(out, img, (size_t)w * h * 4);
        return 1;
    case PSMCT24:
        for (i = 0; i < w * h; i++) {
            out[i] = (uint32_t)img[i * 3] | (uint32_t)img[i * 3 + 1] << 8 | (uint32_t)img[i * 3 + 2] << 16 | 0x80000000u;
        }
        return 1;
    case PSMCT16:
        for (i = 0; i < w * h; i++) {
            out[i] = ct16((uint16_t)(img[i * 2] | img[i * 2 + 1] << 8));
        }
        return 1;
    }
    return 0;
}

typedef struct GlrTex {
    const TexEntry *entry;   /* NULL: free */
    uint32_t check;          /* the entry's contents when decoded (a room's file reloads in place) */
    GLuint tex;
} GlrTex;

static GlrTex sTex[1024];
static uint32_t *sTexels;

static uint32_t tex_check(const TexEntry *t) {
    const uint8_t *img = (const uint8_t *)t + t->data;
    uint32_t c = 2166136261u;
    int i;

    for (i = 0; i < 16; i++) {
        c = (c ^ ((const uint8_t *)t)[i]) * 16777619u;
    }
    for (i = 0; i < 64 && i < t->imageQwc * 16; i++) {
        c = (c ^ img[i]) * 16777619u;
    }
    return c;
}

static GLuint texture_for(const TexEntry *t) {
    uint32_t h = (uint32_t)(uintptr_t)t * 2654435761u >> 22, check = tex_check(t), i;
    GlrTex *e = NULL;

    for (i = 0; i < 1024; i++) {
        e = &sTex[(h + i) & 1023];
        if (e->entry == t || e->entry == NULL) {
            break;
        }
    }
    if (e->entry == t && e->check == check) {
        return e->tex;
    }
    if (e->tex != 0) {   /* changed, or the table is full: the slot is reused */
        p_glDeleteTextures(1, &e->tex);
        e->tex = 0;
    }
    e->entry = t;
    e->check = check;
    if (t->w == 0 || t->h == 0 || t->w > 1024 || t->h > 1024) {
        return 0;
    }
    if (sTexels == NULL) {
        sTexels = malloc(1024 * 1024 * sizeof(uint32_t));
    }
    if (!tex_decode(t, sTexels)) {
        fprintf(stderr, "glr: texture format 0x%02X not handled\n", t->psm);
        return 0;
    }
    if (getenv("HG_TEXDUMP")) {   /* each decoded texture as <dir>/tex_<address>.ppm */
        char path[512];
        FILE *fp;
        int k;

        snprintf(path, sizeof(path), "%s/tex_%p.ppm", getenv("HG_TEXDUMP"), (const void *)t);
        if ((fp = fopen(path, "wb")) != NULL) {
            fprintf(fp, "P6\n%d %d\n255\n", t->w, t->h);
            for (k = 0; k < t->w * t->h; k++) {
                fwrite(&sTexels[k], 1, 3, fp);
            }
            fclose(fp);
        }
    }
    p_glCreateTextures(GL_TEXTURE_2D, 1, &e->tex);
    p_glTextureStorage2D(e->tex, 1, GL_RGBA8, t->w, t->h);
    p_glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    p_glTextureSubImage2D(e->tex, 0, 0, 0, t->w, t->h, GL_RGBA, GL_UNSIGNED_BYTE, sTexels);
    p_glTextureParameteri(e->tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    p_glTextureParameteri(e->tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return e->tex;
}

static GLuint shader(GLenum type, const char *src) {
    GLuint s = p_glCreateShader(type);
    GLint ok;
    char log[1024];

    p_glShaderSource(s, 1, &src, NULL);
    p_glCompileShader(s);
    p_glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        p_glGetShaderInfoLog(s, sizeof(log), NULL, log);
        fprintf(stderr, "glr: shader: %s\n", log);
    }
    return s;
}

static GLuint program(const char *vs, const char *fs) {
    GLuint p = p_glCreateProgram();
    GLint ok;
    char log[1024];

    p_glAttachShader(p, shader(GL_VERTEX_SHADER, vs));
    p_glAttachShader(p, shader(GL_FRAGMENT_SHADER, fs));
    p_glLinkProgram(p);
    p_glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        p_glGetProgramInfoLog(p, sizeof(log), NULL, log);
        fprintf(stderr, "glr: link: %s\n", log);
    }
    return p;
}

static void APIENTRY debug_cb(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei len,
                                const GLchar *msg, const void *user) {
    (void)source; (void)id; (void)len; (void)user;
    if (type == GL_DEBUG_TYPE_ERROR || severity == GL_DEBUG_SEVERITY_HIGH) {
        fprintf(stderr, "gl: %s\n", msg);
    }
}

int glr_init(void) {
#define GLR_LOAD(type, name) \
    if ((p_##name = (type)SDL_GL_GetProcAddress(#name)) == NULL) { \
        fprintf(stderr, "glr: no %s\n", #name); \
        return 0; \
    }
    GLR_FUNCS(GLR_LOAD)
#undef GLR_LOAD

    p_glEnable(GL_DEBUG_OUTPUT);
    p_glDebugMessageCallback(debug_cb, NULL);

    sMeshProg = program(kMeshVs, kMeshFs);
    sMvpLoc = p_glGetUniformLocation(sMeshProg, "uMvp");
    sTexModeLoc = p_glGetUniformLocation(sMeshProg, "uTexMode");
    sTccLoc = p_glGetUniformLocation(sMeshProg, "uTcc");
    sQuadProg = program(kQuadVs, kQuadFs);

    p_glCreateBuffers(1, &sVbo);
    p_glCreateVertexArrays(1, &sVao);
    p_glVertexArrayVertexBuffer(sVao, 0, sVbo, 0, sizeof(GlrVertex));
    p_glEnableVertexArrayAttrib(sVao, 0);
    p_glVertexArrayAttribFormat(sVao, 0, 4, GL_FLOAT, GL_FALSE, 0);
    p_glVertexArrayAttribBinding(sVao, 0, 0);
    p_glEnableVertexArrayAttrib(sVao, 1);
    p_glVertexArrayAttribFormat(sVao, 1, 2, GL_FLOAT, GL_FALSE, 16);
    p_glVertexArrayAttribBinding(sVao, 1, 0);
    p_glEnableVertexArrayAttrib(sVao, 2);
    p_glVertexArrayAttribFormat(sVao, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 24);
    p_glVertexArrayAttribBinding(sVao, 2, 0);
    p_glCreateVertexArrays(1, &sQuadVao);

    p_glCreateTextures(GL_TEXTURE_2D, 1, &sColor);
    p_glTextureStorage2D(sColor, 1, GL_RGBA8, GLR_WIDTH, GLR_HEIGHT);
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sDepth);
    p_glTextureStorage2D(sDepth, 1, GL_DEPTH_COMPONENT24, GLR_WIDTH, GLR_HEIGHT);
    p_glCreateFramebuffers(1, &sFbo);
    p_glNamedFramebufferTexture(sFbo, GL_COLOR_ATTACHMENT0, sColor, 0);
    p_glNamedFramebufferTexture(sFbo, GL_DEPTH_ATTACHMENT, sDepth, 0);
    return 1;
}

/* the software GS frame as a texture (re-created when its size changes) */
static void upload_gs(const uint32_t *px, int pitch, int w, int h) {
    if (w != sGsW || h != sGsH) {
        if (sGsTex) {
            p_glDeleteTextures(1, &sGsTex);
        }
        p_glCreateTextures(GL_TEXTURE_2D, 1, &sGsTex);
        p_glTextureStorage2D(sGsTex, 1, GL_RGBA8, w, h);
        p_glTextureParameteri(sGsTex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        p_glTextureParameteri(sGsTex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        sGsW = w;
        sGsH = h;
    }
    p_glPixelStorei(GL_UNPACK_ROW_LENGTH, pitch);
    p_glTextureSubImage2D(sGsTex, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, px);
    p_glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
}

void glr_present(const uint32_t *gsPixels, int pitch, int w, int h, int outW, int outH) {
    static const float kBlack[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    static const float kFar = 1.0f;
    const GlrFrame *f = &sFrames[sBuilding ^ 1];
    int i;

    p_glBindFramebuffer(GL_FRAMEBUFFER, sFbo);
    p_glViewport(0, 0, GLR_WIDTH, GLR_HEIGHT);
    p_glClearNamedFramebufferfv(sFbo, GL_COLOR, 0, kBlack);
    p_glClearNamedFramebufferfv(sFbo, GL_DEPTH, 0, &kFar);

    /* underneath: what the software GS drew (2D paths not ported yet) */
    if (w > 0 && h > 0) {
        upload_gs(gsPixels, pitch, w, h);
        p_glDisable(GL_DEPTH_TEST);
        p_glUseProgram(sQuadProg);
        p_glBindTextureUnit(0, sGsTex);
        p_glBindVertexArray(sQuadVao);
        p_glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    if (getenv("HG_GLDEBUG")) {
        static unsigned n;

        if (n++ % 60 == 0) {
            fprintf(stderr, "glr: %d draws, %d vertices\n", f->nd, f->nv);
            if (f->nd > 0) {
                const float *m = f->d[0].mvp;
                const GlrVertex *v = &f->v[f->d[0].first];
                float c[4];
                int k;

                for (k = 0; k < 4; k++) {
                    c[k] = m[k] * v->x + m[4 + k] * v->y + m[8 + k] * v->z + m[12 + k];
                }
                fprintf(stderr, "glr: v0 (%g %g %g) -> clip (%g %g %g %g)\n", v->x, v->y, v->z, c[0], c[1], c[2], c[3]);
                if (getenv("HG_GLDEBUG_W")) {
                    int s2, j;

                    for (s2 = 0; s2 < 4 && s2 < f->nd; s2++) {
                        fprintf(stderr, "glr: draw %d n %d w:", s2, f->d[s2].n);
                        for (j = 0; j < f->d[s2].n && j < 24; j++) {
                            union { float f; unsigned u; } w = { f->v[f->d[s2].first + j].w };
                            fprintf(stderr, " %08X", w.u);
                        }
                        fprintf(stderr, "\n");
                    }
                }
            }
        }
    }

    /* the 3D strips */
    if (f->nd > 0) {
        p_glNamedBufferData(sVbo, (GLsizeiptr)f->nv * sizeof(GlrVertex), f->v, GL_STREAM_DRAW);
        p_glEnable(GL_DEPTH_TEST);
        p_glDepthFunc(GL_LESS);
        p_glDepthMask(GL_TRUE);
        p_glUseProgram(sMeshProg);
        p_glBindVertexArray(sVao);
        p_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for (i = 0; i < f->nd; i++) {
            const GlrDraw *d = &f->d[i];
            int mode = 0;

            if ((d->prim & 0x10) && d->tex != NULL) {   /* TME */
                GLuint t = texture_for((const TexEntry *)d->tex);
                uint32_t tfx = (uint32_t)(d->tex0 >> 35) & 3;

                mode = t == 0 ? 0 : tfx == 1 ? 2 : 1;
                p_glBindTextureUnit(0, t);
                p_glProgramUniform1i(sMeshProg, sTccLoc, (int)(d->tex0 >> 34) & 1);
            }
            p_glProgramUniform1i(sMeshProg, sTexModeLoc, mode);
            if (d->prim & 0x40) {   /* ABE */
                p_glEnable(GL_BLEND);
            } else {
                p_glDisable(GL_BLEND);
            }
            p_glProgramUniformMatrix4fv(sMeshProg, sMvpLoc, 1, GL_FALSE, d->mvp);
            p_glDrawArrays(GL_TRIANGLES, d->first, d->n);
        }
        p_glDisable(GL_BLEND);
        p_glDisable(GL_DEPTH_TEST);
    }

    /* into the window, 4:3 letterboxed (nothing when there is no window) */
    if (outW > 0 && outH > 0) {
        int vw = outW, vh = outW * 3 / 4, x, y;

        if (vh > outH) {
            vh = outH;
            vw = outH * 4 / 3;
        }
        x = (outW - vw) / 2;
        y = (outH - vh) / 2;
        p_glBindFramebuffer(GL_FRAMEBUFFER, 0);
        p_glViewport(0, 0, outW, outH);
        p_glClearNamedFramebufferfv(0, GL_COLOR, 0, kBlack);
        p_glBlitNamedFramebuffer(sFbo, 0, 0, 0, GLR_WIDTH, GLR_HEIGHT, x, y, x + vw, y + vh, GL_COLOR_BUFFER_BIT,
                                 GL_LINEAR);
    }
}

void glr_read_pixels(uint32_t *out, int w, int h) {
    int y;

    (void)w;
    p_glBindFramebuffer(GL_FRAMEBUFFER, sFbo);
    p_glPixelStorei(GL_PACK_ALIGNMENT, 4);
    p_glReadPixels(0, 0, GLR_WIDTH, GLR_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, out);
    /* GL rows run bottom-up */
    for (y = 0; y < h / 2 && y < GLR_HEIGHT / 2; y++) {
        uint32_t tmp[GLR_WIDTH];

        memcpy(tmp, out + y * GLR_WIDTH, sizeof(tmp));
        memcpy(out + y * GLR_WIDTH, out + (GLR_HEIGHT - 1 - y) * GLR_WIDTH, sizeof(tmp));
        memcpy(out + (GLR_HEIGHT - 1 - y) * GLR_WIDTH, tmp, sizeof(tmp));
    }
}
