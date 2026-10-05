/* The PC renderer: OpenGL 4.6 core with direct state access (see glr.h).
 *
 * The scene is drawn at the PS2's 640x448 into an offscreen target (glr_present), which is
 * then scaled into the window; frame dumps read the offscreen target.
 *
 *   HG_GLDEBUG=1     once a second: the frame's draw count and its first vertex in clip space
 *                    (and, on glow frames, the glow buffer's brightest value and the bloom colour)
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
    X(PFNGLPROGRAMUNIFORM4FPROC, glProgramUniform4f) \
    X(PFNGLPROGRAMUNIFORM1FPROC, glProgramUniform1f) \
    X(PFNGLPROGRAMUNIFORM2FPROC, glProgramUniform2f) \
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
    X(PFNGLBLENDEQUATIONPROC, glBlendEquation) \
    X(PFNGLDISABLEIPROC, glDisablei) \
    X(PFNGLCOLORMASKIPROC, glColorMaski) \
    X(PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC, glNamedFramebufferDrawBuffers) \
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
    int layer;   /* the renderer layer it was sent in (glr_layer; -1: none) */
    int post;    /* not a strip but a pass over the frame (POST_*; colour tex0, arguments prim) */
} GlrDraw;

enum { POST_BLOOM = 1, POST_GLOW, POST_SCREEN2, POST_FOG, POST_VIGNETTE };

typedef struct GlrFrame {
    uint32_t overlay;   /* a full-screen tint over the frame (RGBA, alpha 0x80 = 1.0; 0: none) */
    GlrVertex *v;
    int nv, capv;
    GlrDraw *d;
    int nd, capd;
} GlrFrame;

static GlrFrame sFrames[2];
static int sBuilding;   /* index of the frame being built; the other one is shown */
static int sLayer = -1;

void glr_layer(int layer) {
    sLayer = layer;
}

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
    d->layer = sLayer;
    d->post = 0;
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
    sFrames[sBuilding].overlay = 0;
}


void glr_overlay(uint32_t rgba) {
    sFrames[sBuilding].overlay = rgba;
}

static int sGlowClear = 1;   /* clear the glow buffer at the next present */

/* a pass over the frame, in `layer` */
static GlrDraw *put_post(int kind, int layer, uint64_t rgba, uint32_t args) {
    GlrFrame *f = &sFrames[sBuilding];
    GlrDraw *d;

    if (f->nd + 1 > f->capd) {
        f->capd = f->capd * 2 + 256;
        f->d = realloc(f->d, f->capd * sizeof(GlrDraw));
    }
    d = &f->d[f->nd++];
    memset(d, 0, sizeof(*d));
    d->post = kind;
    d->layer = layer;
    d->tex0 = rgba;
    d->prim = args;
    return d;
}

/* the fog (func_002BB3E0, which the game draws in its own layer): colours c0 / c1 (low / high
 * word of tex0), view depths in mvp[0] / mvp[1] */
void glr_fog(uint32_t c0, uint32_t c1, float nearZ, float farZ) {
    GlrDraw *d = put_post(POST_FOG, sLayer, c0 | (uint64_t)c1 << 32, 0);

    d->mvp[0] = nearZ;
    d->mvp[1] = farZ;
}

void glr_glow(void) {
    put_post(POST_GLOW, 0x29, 0, 0);   /* the original's packet goes in layer 0x29 */
}

void glr_glow_clear(void) {
    sGlowClear = 1;
}

void glr_bloom(uint32_t rgba, int subtract) {
    put_post(POST_BLOOM, sLayer, rgba, subtract != 0);
}

void glr_vignette(int strength, int offset) {
    put_post(POST_VIGNETTE, 0x2A, (uint32_t)strength, (uint32_t)offset);   /* its packet's layer */
}

void glr_screen2(uint32_t rgba, int contrast, int blur) {
    put_post(POST_SCREEN2, sLayer, rgba, (contrast != 0) | (blur != 0) << 1);
}

/* ---- GL objects ---- */

#define GLR_WIDTH 640
#define GLR_HEIGHT 448
#define GLR_GLOW_W 128   /* the renderer's work buffer (frame page 0x1F0) */
#define GLR_GLOW_H 112

static GLuint sMeshProg, sQuadProg, sFillProg, sVao, sQuadVao, sVbo;
static GLint sFillLoc;
static GLint sMvpLoc, sTexModeLoc, sTccLoc;
static GLuint sFbo, sColor, sDepth, sViewDepth, sGsTex;   /* sViewDepth: each pixel's view depth */
/* the glow buffer (B, kept between frames) with the scene's depth at its size, and the
 * scratch buffer its fade goes through (A) */
static GLuint sGlowFbo, sGlowB, sGlowDepth, sGlowAFbo, sGlowA, sPostProg;
static GLint sPostModeLoc, sPostColorLoc, sPostColor2Loc, sPostRangeLoc, sPostFixLoc;
/* the bloom's half-size images: H, H2 and the next H */
#define GLR_HALF_W 256
#define GLR_HALF_H 224
static GLuint sHalf[3], sHalfFbo[3];
static GLuint sCopy, sCopyFbo;   /* a copy of the screen (passes that read it while drawing over it) */
static int sGsW, sGsH;

static const char *kMeshVs =
    "#version 460 core\n"
    "layout(location = 0) in vec4 aPos;\n"
    "layout(location = 1) in vec2 aSt;\n"
    "layout(location = 2) in vec4 aCol;\n"
    "uniform mat4 uMvp;\n"
    "out vec4 vCol;\n"
    "out vec2 vSt;\n"
    "out float vDepth;\n"
    "void main() {\n"
    "    vec4 p = uMvp * vec4(aPos.xyz, 1.0);\n"
    "    vDepth = p.w;\n"
    /* PS2 clip space spans the GS's whole 4096 x 4096 drawing space (-1..1 = 2048 -+ 2047),
     * of which the 640 x 448 around the centre is the screen; y grows downwards, larger z is
     * nearer */
    "    gl_Position = vec4(p.x * (2047.0 / 320.0), -p.y * (2047.0 / 224.0), -p.z, p.w);\n"
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
    "in float vDepth;\n"
    "layout(location = 0) out vec4 oColor;\n"
    "layout(location = 1) out float oDepth;\n"   /* the view depth, for the fog pass */
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
    "    oDepth = vDepth;\n"
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

/* The post passes, in the GS's 8-bit arithmetic (colours 0..255, sprites modulated by their
 * colour with 0x80 = 1.0, blends (A - B) * FIX >> 7 + D clamped). Textures are sampled whole
 * texels except where noted; "H" is the half-size (256 x 224) copy of the screen.
 *
 * The glow (renderer +0x58), B the 128 x 112 glow buffer:
 *   0  B fades, through A: A = B * 0x40 >> 7, then B + ((A - B) * 0x40 >> 7)
 *   1  over the screen: B * 0x40 >> 7 added, B stretched with bilinear filtering
 * The screen bloom (func_002699D0):
 *   2  H = the screen at every other pixel
 *   3  H2 = the sum of H * 0x40 >> 7 at 8 offsets: (+-1, +-1), (+-2, 0), (0, +-2)
 *   4  H += the sum of H2 * 0x10 >> 7 at 8 offsets: (+-3, 0), (0, +-3), (+-2, +-2)
 *   5  B += (H * 0x40 >> 7) * 0x20 >> 7, H at every other pixel
 *   6  over the screen: (H * colour >> 7) * fix >> 7, H stretched with bilinear filtering;
 *      added or subtracted (the blend)
 * The two-colour screen effect (func_002685F0), H2 into the second half-size image:
 *   7  H2 = H + the sum of H * 0x20 >> 7 at 8 offsets: (+-1, +-1), (+-2, 0), (0, +-2)
 *   8  H2 = H + 8 x (H * 0x20 >> 7)
 *   9  the screen (a copy) D + ((D - (H2 * colour >> 7)) * fix >> 7), H2 stretched as in 6
 *      (mode 6 does its other way, added)
 * The fog (func_002BB3E0; the game maps its Z buffer through a colour ramp):
 *   10 the fog colour and alpha at each pixel's view depth, blended over it
 * The vignette (func_0021C840), uFix its strength, uRange.x its offset:
 *   11 the screen (a copy) D + (-D * a >> 7), a = strength x the corner's weight in one of
 *      four gouraud triangles, each from a screen corner to the middles of its two edges
 *      (moved by the offset) */
static const char *kPostFs =
    "#version 460 core\n"
    "uniform sampler2D uTex;\n"
    "uniform sampler2D uTex1;\n"
    "uniform int uMode;\n"
    "uniform vec4 uColor;\n"   /* 0..255 */
    "uniform vec4 uColor2;\n"
    "uniform vec2 uRange;\n"
    "uniform float uFix;\n"
    "out vec4 oColor;\n"
    "vec4 at(sampler2D t, ivec2 p) {\n"   /* 0..255; outside the image: nothing */
    "    ivec2 sz = textureSize(t, 0);\n"
    "    if (p.x < 0 || p.y < 0 || p.x >= sz.x || p.y >= sz.y) return vec4(0.0);\n"
    "    return round(texelFetch(t, p, 0) * 255.0);\n"
    "}\n"
    "float corner(vec2 q, vec2 c, vec2 a, vec2 b) {\n"   /* c's barycentric weight; 0 outside */
    "    float det = (a.y - b.y) * (c.x - b.x) + (b.x - a.x) * (c.y - b.y);\n"
    "    float wc = ((a.y - b.y) * (q.x - b.x) + (b.x - a.x) * (q.y - b.y)) / det;\n"
    "    float wa = ((b.y - c.y) * (q.x - b.x) + (c.x - b.x) * (q.y - b.y)) / det;\n"
    "    return wc >= 0.0 && wa >= 0.0 && wc + wa <= 1.0 ? wc : 0.0;\n"
    "}\n"
    "void main() {\n"
    "    ivec2 p = ivec2(gl_FragCoord.xy);\n"
    "    vec4 c;\n"
    "    if (uMode == 0) {\n"
    "        vec4 b = at(uTex, p);\n"
    "        vec4 a = floor(b * 0.5);\n"
    "        c = b + floor((a - b) * 0.5);\n"
    "    } else if (uMode == 1 || uMode == 6) {\n"
    "        vec2 sz = vec2(textureSize(uTex, 0));\n"
    "        vec2 t = (gl_FragCoord.xy - 0.5) * sz / vec2(640.0, 448.0) + 0.5;\n"
    "        vec4 h = round(texture(uTex, t / sz) * 255.0);\n"
    "        c = uMode == 1 ? floor(h * 0.5) : floor(floor(h * uColor / 128.0) * uFix / 128.0);\n"
    "    } else if (uMode == 2) {\n"   /* the screen is 640 wide here, the game's 512 */
    "        c = at(uTex, ivec2(int(float(p.x * 2) * 1.25), p.y * 2 + 1));\n"
    "    } else if (uMode == 3) {\n"
    "        c = vec4(0.0);\n"
    "        c += floor(at(uTex, p + ivec2(-1, -1)) * 0.5); c += floor(at(uTex, p + ivec2(1, -1)) * 0.5);\n"
    "        c += floor(at(uTex, p + ivec2(-1, 1)) * 0.5);  c += floor(at(uTex, p + ivec2(1, 1)) * 0.5);\n"
    "        c += floor(at(uTex, p + ivec2(-2, 0)) * 0.5);  c += floor(at(uTex, p + ivec2(2, 0)) * 0.5);\n"
    "        c += floor(at(uTex, p + ivec2(0, -2)) * 0.5);  c += floor(at(uTex, p + ivec2(0, 2)) * 0.5);\n"
    "    } else if (uMode == 4) {\n"
    "        c = at(uTex, p);\n"
    "        c += floor(at(uTex1, p + ivec2(-3, 0)) / 8.0);  c += floor(at(uTex1, p + ivec2(3, 0)) / 8.0);\n"
    "        c += floor(at(uTex1, p + ivec2(0, -3)) / 8.0);  c += floor(at(uTex1, p + ivec2(0, 3)) / 8.0);\n"
    "        c += floor(at(uTex1, p + ivec2(-2, -2)) / 8.0); c += floor(at(uTex1, p + ivec2(2, 2)) / 8.0);\n"
    "        c += floor(at(uTex1, p + ivec2(2, -2)) / 8.0);  c += floor(at(uTex1, p + ivec2(-2, 2)) / 8.0);\n"
    "    } else if (uMode == 5) {\n"
    "        c = at(uTex, p) + floor(floor(at(uTex1, ivec2(p.x * 2, p.y * 2 + 1)) * 0.5) * 0.25);\n"
    "    } else if (uMode == 10) {\n"   /* fog: colours uColor .. uColor2 (alpha 0x80 = full) over uRange */
    "        float d = texelFetch(uTex, p, 0).r;\n"
    "        if (!(uRange.y > uRange.x) || d <= uRange.x) discard;\n"
    "        vec4 fc = mix(uColor, uColor2, clamp((d - uRange.x) / (uRange.y - uRange.x), 0.0, 1.0));\n"
    "        oColor = vec4(fc.rgb / 255.0, clamp(fc.a / 128.0, 0.0, 1.0));\n"
    "        return;\n"
    "    } else if (uMode == 11) {\n"
    "        vec2 q = vec2(gl_FragCoord.x * (512.0 / 640.0), 448.0 - gl_FragCoord.y);\n"   /* the game's pixels */
    "        float o = uRange.x, w = 0.0;\n"
    "        w = max(w, corner(q, vec2(0.0, 0.0), vec2(256.0 + o, 0.0), vec2(0.0, 224.0 - o)));\n"
    "        w = max(w, corner(q, vec2(512.0, 0.0), vec2(256.0 + o, 0.0), vec2(512.0, 224.0 + o)));\n"
    "        w = max(w, corner(q, vec2(0.0, 448.0), vec2(256.0 - o, 448.0), vec2(0.0, 224.0 - o)));\n"
    "        w = max(w, corner(q, vec2(512.0, 448.0), vec2(256.0 - o, 448.0), vec2(512.0, 224.0 + o)));\n"
    "        vec4 d = at(uTex, p);\n"
    "        c = d + floor(-d * floor(uFix * w) / 128.0);\n"
    "    } else if (uMode == 7) {\n"
    "        c = at(uTex, p);\n"
    "        c += floor(at(uTex, p + ivec2(-1, -1)) * 0.25); c += floor(at(uTex, p + ivec2(1, -1)) * 0.25);\n"
    "        c += floor(at(uTex, p + ivec2(-1, 1)) * 0.25);  c += floor(at(uTex, p + ivec2(1, 1)) * 0.25);\n"
    "        c += floor(at(uTex, p + ivec2(-2, 0)) * 0.25);  c += floor(at(uTex, p + ivec2(2, 0)) * 0.25);\n"
    "        c += floor(at(uTex, p + ivec2(0, -2)) * 0.25);  c += floor(at(uTex, p + ivec2(0, 2)) * 0.25);\n"
    "    } else if (uMode == 8) {\n"
    "        c = at(uTex, p) + 8.0 * floor(at(uTex, p) * 0.25);\n"
    "    } else {\n"
    "        vec2 sz = vec2(textureSize(uTex, 0));\n"
    "        vec2 t = (gl_FragCoord.xy - 0.5) * sz / vec2(640.0, 448.0) + 0.5;\n"
    "        vec4 cs = floor(round(texture(uTex, t / sz) * 255.0) * uColor / 128.0);\n"
    "        vec4 d = at(uTex1, p);\n"
    "        c = d + floor((d - cs) * uFix / 128.0);\n"
    "    }\n"
    "    oColor = vec4(clamp(c.rgb, 0.0, 255.0) / 255.0, 1.0);\n"
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

/* palette entry i of n (16-colour palettes: palette `csa` of those in the 16-wide CLUT image,
 * each an 8 x 2 block) */
static uint32_t clut_entry(const TexEntry *t, const uint8_t *clut, int i, int n, int csa) {
    int x, y, k;

    if (n == 256) {   /* CSM1: entries 8..15 and 16..23 of each 32 swapped */
        x = i % 8 + ((i / 16) % 2) * 8;
        y = (i / 32) * 2 + (i / 8) % 2;
    } else {
        x = (i & 7) + (csa & 1) * 8;
        y = (i >> 3) + (csa >> 1) * 2;
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
static int tex_decode(const TexEntry *t, uint32_t *out, int csa) {
    const uint8_t *img = (const uint8_t *)t + t->data, *clut = img + t->imageQwc * 16;
    uint32_t pal[256];
    int w = t->w, h = t->h, i, n;

    switch (t->psm) {
    case PSMT8:
    case PSMT4:
        n = t->psm == PSMT8 ? 256 : 16;
        for (i = 0; i < n; i++) {
            pal[i] = clut_entry(t, clut, i, n, csa);
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
    int csa;                 /* its palette (16-colour textures) */
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

static GLuint texture_for(const TexEntry *t, int csa) {
    uint32_t h = ((uint32_t)(uintptr_t)t + (uint32_t)csa * 0x9E37u) * 2654435761u >> 22, check = tex_check(t), i;
    GlrTex *e = NULL;

    for (i = 0; i < 1024; i++) {
        e = &sTex[(h + i) & 1023];
        if ((e->entry == t && e->csa == csa) || e->entry == NULL) {
            break;
        }
    }
    if (e->entry == t && e->csa == csa && e->check == check) {
        return e->tex;
    }
    if (e->tex != 0) {   /* changed, or the table is full: the slot is reused */
        p_glDeleteTextures(1, &e->tex);
        e->tex = 0;
    }
    e->entry = t;
    e->csa = csa;
    e->check = check;
    if (t->w == 0 || t->h == 0 || t->w > 1024 || t->h > 1024) {
        return 0;
    }
    if (sTexels == NULL) {
        sTexels = malloc(1024 * 1024 * sizeof(uint32_t));
    }
    if (!tex_decode(t, sTexels, csa)) {
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

/* a full-screen fill in one colour (the overlay) */
static const char *kFillFs =
    "#version 460 core\n"
    "in vec2 vUv;\n"
    "uniform vec4 uColor;\n"
    "out vec4 oColor;\n"
    "void main() {\n"
    "    oColor = uColor;\n"
    "}\n";

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
    int i;

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
    sFillProg = program(kQuadVs, kFillFs);
    sFillLoc = p_glGetUniformLocation(sFillProg, "uColor");

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
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sViewDepth);
    p_glTextureStorage2D(sViewDepth, 1, GL_R32F, GLR_WIDTH, GLR_HEIGHT);
    p_glNamedFramebufferTexture(sFbo, GL_COLOR_ATTACHMENT1, sViewDepth, 0);
    {
        static const GLenum kBufs[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

        p_glNamedFramebufferDrawBuffers(sFbo, 2, kBufs);
    }

    sPostProg = program(kQuadVs, kPostFs);
    sPostModeLoc = p_glGetUniformLocation(sPostProg, "uMode");
    sPostColorLoc = p_glGetUniformLocation(sPostProg, "uColor");
    sPostFixLoc = p_glGetUniformLocation(sPostProg, "uFix");
    sPostColor2Loc = p_glGetUniformLocation(sPostProg, "uColor2");
    sPostRangeLoc = p_glGetUniformLocation(sPostProg, "uRange");
    p_glProgramUniform1i(sPostProg, p_glGetUniformLocation(sPostProg, "uTex1"), 1);
    for (i = 0; i < 3; i++) {
        p_glCreateTextures(GL_TEXTURE_2D, 1, &sHalf[i]);
        p_glTextureStorage2D(sHalf[i], 1, GL_RGBA8, GLR_HALF_W, GLR_HALF_H);
        p_glTextureParameteri(sHalf[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        p_glTextureParameteri(sHalf[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        p_glTextureParameteri(sHalf[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        p_glTextureParameteri(sHalf[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        p_glCreateFramebuffers(1, &sHalfFbo[i]);
        p_glNamedFramebufferTexture(sHalfFbo[i], GL_COLOR_ATTACHMENT0, sHalf[i], 0);
    }
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sCopy);
    p_glTextureStorage2D(sCopy, 1, GL_RGBA8, GLR_WIDTH, GLR_HEIGHT);
    p_glCreateFramebuffers(1, &sCopyFbo);
    p_glNamedFramebufferTexture(sCopyFbo, GL_COLOR_ATTACHMENT0, sCopy, 0);
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sGlowB);
    p_glTextureStorage2D(sGlowB, 1, GL_RGBA8, GLR_GLOW_W, GLR_GLOW_H);
    p_glTextureParameteri(sGlowB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    p_glTextureParameteri(sGlowB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    p_glTextureParameteri(sGlowB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    p_glTextureParameteri(sGlowB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sGlowDepth);
    p_glTextureStorage2D(sGlowDepth, 1, GL_DEPTH_COMPONENT24, GLR_GLOW_W, GLR_GLOW_H);
    p_glCreateFramebuffers(1, &sGlowFbo);
    p_glNamedFramebufferTexture(sGlowFbo, GL_COLOR_ATTACHMENT0, sGlowB, 0);
    p_glNamedFramebufferTexture(sGlowFbo, GL_DEPTH_ATTACHMENT, sGlowDepth, 0);
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sGlowA);
    p_glTextureStorage2D(sGlowA, 1, GL_RGBA8, GLR_GLOW_W, GLR_GLOW_H);
    p_glCreateFramebuffers(1, &sGlowAFbo);
    p_glNamedFramebufferTexture(sGlowAFbo, GL_COLOR_ATTACHMENT0, sGlowA, 0);
    return 1;
}

/* one post pass (kPostFs mode) into `fbo` (w x h) from `t0` / `t1` */
static void post(int mode, GLuint fbo, int w, int h, GLuint t0, GLuint t1) {
    p_glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    p_glViewport(0, 0, w, h);
    p_glProgramUniform1i(sPostProg, sPostModeLoc, mode);
    p_glBindTextureUnit(0, t0);
    p_glBindTextureUnit(1, t1);
    p_glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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

static int cmp_order(const void *a, const void *b) {
    long long x = *(const long long *)a, y = *(const long long *)b;

    return x < y ? -1 : x > y;
}

/* the 3D strips' state */
static void mesh_state(void) {
    p_glBindFramebuffer(GL_FRAMEBUFFER, sFbo);
    p_glColorMaski(1, GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
    p_glViewport(0, 0, GLR_WIDTH, GLR_HEIGHT);
    p_glEnable(GL_DEPTH_TEST);
    p_glDepthFunc(GL_LESS);
    p_glDepthMask(GL_TRUE);
    p_glDisable(GL_BLEND);
    p_glUseProgram(sMeshProg);
    p_glBindVertexArray(sVao);
}

static void post_color(uint32_t rgba) {
    p_glProgramUniform4f(sPostProg, sPostColorLoc, (float)(rgba & 0xFF), (float)((rgba >> 8) & 0xFF),
                         (float)((rgba >> 16) & 0xFF), 0.0f);
    p_glProgramUniform1f(sPostProg, sPostFixLoc, (float)(rgba >> 25));   /* FIX: alpha / 2 */
}

static void glow_buffer_update(void) {   /* A -> B */
    p_glBlitNamedFramebuffer(sGlowAFbo, sGlowFbo, 0, 0, GLR_GLOW_W, GLR_GLOW_H, 0, 0, GLR_GLOW_W, GLR_GLOW_H,
                             GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

/* a pass over the frame (the shader's modes, kPostFs) */
static void run_post(const GlrDraw *d) {
    uint32_t rgba = (uint32_t)d->tex0;

    p_glDisable(GL_DEPTH_TEST);
    p_glDisable(GL_BLEND);
    p_glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    p_glUseProgram(sPostProg);
    p_glBindVertexArray(sQuadVao);
    switch (d->post) {
    case POST_VIGNETTE:
        p_glBlitNamedFramebuffer(sFbo, sCopyFbo, 0, 0, GLR_WIDTH, GLR_HEIGHT, 0, 0, GLR_WIDTH, GLR_HEIGHT,
                                 GL_COLOR_BUFFER_BIT, GL_NEAREST);
        p_glProgramUniform1f(sPostProg, sPostFixLoc, (float)(int32_t)rgba);
        p_glProgramUniform2f(sPostProg, sPostRangeLoc, (float)(int32_t)d->prim, 0.0f);
        post(11, sFbo, GLR_WIDTH, GLR_HEIGHT, sCopy, 0);
        break;
    case POST_FOG: {   /* func_002BB3E0: (fog - screen) * fog alpha + screen, by view depth */
        uint32_t c1 = (uint32_t)(d->tex0 >> 32);

        p_glProgramUniform4f(sPostProg, sPostColor2Loc, (float)(c1 & 0xFF), (float)((c1 >> 8) & 0xFF),
                             (float)((c1 >> 16) & 0xFF), (float)(c1 >> 24));
        p_glProgramUniform4f(sPostProg, sPostColorLoc, (float)(rgba & 0xFF), (float)((rgba >> 8) & 0xFF),
                             (float)((rgba >> 16) & 0xFF), (float)(rgba >> 24));
        p_glProgramUniform2f(sPostProg, sPostRangeLoc, d->mvp[0], d->mvp[1]);
        p_glEnable(GL_BLEND);
        p_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        post(10, sFbo, GLR_WIDTH, GLR_HEIGHT, sViewDepth, 0);
        break;
    }
    case POST_BLOOM:   /* func_002699D0: H blurred, into the glow buffer and over the screen */
        post(2, sHalfFbo[0], GLR_HALF_W, GLR_HALF_H, sColor, 0);
        post(3, sHalfFbo[1], GLR_HALF_W, GLR_HALF_H, sHalf[0], 0);
        post(4, sHalfFbo[2], GLR_HALF_W, GLR_HALF_H, sHalf[0], sHalf[1]);
        post(5, sGlowAFbo, GLR_GLOW_W, GLR_GLOW_H, sGlowB, sHalf[2]);
        glow_buffer_update();
        post_color(rgba);
        p_glEnable(GL_BLEND);
        p_glBlendFunc(GL_ONE, GL_ONE);
        p_glBlendEquation(d->prim & 1 ? GL_FUNC_REVERSE_SUBTRACT : GL_FUNC_ADD);
        post(6, sFbo, GLR_WIDTH, GLR_HEIGHT, sHalf[2], 0);
        p_glBlendEquation(GL_FUNC_ADD);
        break;
    case POST_GLOW:   /* renderer +0x58: B fades through A, then is added over the screen */
        post(0, sGlowAFbo, GLR_GLOW_W, GLR_GLOW_H, sGlowB, 0);
        glow_buffer_update();
        p_glEnable(GL_BLEND);
        p_glBlendFunc(GL_ONE, GL_ONE);
        post(1, sFbo, GLR_WIDTH, GLR_HEIGHT, sGlowB, 0);
        break;
    case POST_SCREEN2:   /* func_002685F0: H brightened (blurred: args bit 1), added or contrasted */
        post(2, sHalfFbo[0], GLR_HALF_W, GLR_HALF_H, sColor, 0);
        post(d->prim & 2 ? 7 : 8, sHalfFbo[1], GLR_HALF_W, GLR_HALF_H, sHalf[0], 0);
        post_color(rgba);
        if (d->prim & 1) {
            p_glBlitNamedFramebuffer(sFbo, sCopyFbo, 0, 0, GLR_WIDTH, GLR_HEIGHT, 0, 0, GLR_WIDTH, GLR_HEIGHT,
                                     GL_COLOR_BUFFER_BIT, GL_NEAREST);
            post(9, sFbo, GLR_WIDTH, GLR_HEIGHT, sHalf[1], sCopy);
        } else {
            p_glEnable(GL_BLEND);
            p_glBlendFunc(GL_ONE, GL_ONE);
            post(6, sFbo, GLR_WIDTH, GLR_HEIGHT, sHalf[1], 0);
        }
        break;
    }
    p_glDisable(GL_BLEND);
}

void glr_present(const uint32_t *gsPixels, int pitch, int w, int h, int outW, int outH) {
    static const float kBlack[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    static const float kFar = 1.0f;
    static const float kFarView[4] = {1e30f, 0.0f, 0.0f, 0.0f};   /* nothing drawn: beyond the fog */
    const GlrFrame *f = &sFrames[sBuilding ^ 1];
    int i, depthDirty = 1;   /* the scene's depth changed since the glow buffer's copy */

    if (sGlowClear) {
        p_glClearNamedFramebufferfv(sGlowFbo, GL_COLOR, 0, kBlack);
        sGlowClear = 0;
    }

    p_glBindFramebuffer(GL_FRAMEBUFFER, sFbo);
    p_glViewport(0, 0, GLR_WIDTH, GLR_HEIGHT);
    p_glClearNamedFramebufferfv(sFbo, GL_COLOR, 0, kBlack);
    p_glClearNamedFramebufferfv(sFbo, GL_DEPTH, 0, &kFar);
    p_glClearNamedFramebufferfv(sFbo, GL_COLOR, 1, kFarView);
    p_glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);   /* only the strips write it */

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
            {   /* draws per renderer layer */
                int byLayer[54] = {0}, k;

                for (k = 0; k < f->nd; k++) {
                    byLayer[f->d[k].layer + 1]++;
                }
                fprintf(stderr, "glr: layers");
                for (k = 0; k < 54; k++) {
                    if (byLayer[k]) {
                        fprintf(stderr, " %d:%d", k - 1, byLayer[k]);
                    }
                }
                fprintf(stderr, "\n");
            }
            {   /* the frame's passes; the glow buffer's brightest channel (before this frame) */
                static uint8_t px[GLR_GLOW_W * GLR_GLOW_H * 4];
                int k, m = 0;

                for (k = 0; k < f->nd; k++) {
                    if (f->d[k].post) {
                        fprintf(stderr, "glr: pass %d in layer %d, colour %08X, args %u\n", f->d[k].post,
                                f->d[k].layer, (uint32_t)f->d[k].tex0, f->d[k].prim);
                    }
                }
                p_glBindFramebuffer(GL_FRAMEBUFFER, sGlowFbo);
                p_glReadPixels(0, 0, GLR_GLOW_W, GLR_GLOW_H, GL_RGBA, GL_UNSIGNED_BYTE, px);
                p_glBindFramebuffer(GL_FRAMEBUFFER, sFbo);
                for (k = 0; k < (int)sizeof(px); k++) {
                    m = (k & 3) != 3 && px[k] > m ? px[k] : m;
                }
                fprintf(stderr, "glr: glow buffer max %d\n", m);
            }
            if (f->nd > 0 && f->nv > 0 && !f->d[0].post) {
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

    /* the strips and passes, in renderer layer order (as sent within a layer) */
    if (f->nd > 0) {
        static long long *order;
        static int cap;

        if (f->nd > cap) {
            cap = f->nd * 2;
            order = realloc(order, cap * sizeof(*order));
        }
        for (i = 0; i < f->nd; i++) {   /* no layer: the 3D scene's (1) */
            order[i] = (long long)(f->d[i].layer < 0 ? 1 : f->d[i].layer) << 32 | i;
        }
        qsort(order, f->nd, sizeof(*order), cmp_order);
        p_glNamedBufferData(sVbo, (GLsizeiptr)f->nv * sizeof(GlrVertex), f->v, GL_STREAM_DRAW);
        mesh_state();
        for (i = 0; i < f->nd; i++) {
            const GlrDraw *d = &f->d[order[i] & 0xFFFFFFFF];
            int mode = 0;

            if (d->post) {
                run_post(d);
                mesh_state();
                continue;
            }
            if ((d->prim & 0x10) && d->tex != NULL) {   /* TME */
                GLuint t = texture_for((const TexEntry *)d->tex, (int)(d->tex0 >> 56) & 0x1F);   /* CSA */
                uint32_t tfx = (uint32_t)(d->tex0 >> 35) & 3;

                mode = t == 0 ? 0 : tfx == 1 ? 2 : 1;
                p_glBindTextureUnit(0, t);
                p_glProgramUniform1i(sMeshProg, sTccLoc, (int)(d->tex0 >> 34) & 1);
            }
            p_glProgramUniform1i(sMeshProg, sTexModeLoc, mode);
            if (d->prim & 0x40) {   /* ABE */
                p_glEnable(GL_BLEND);
                p_glDisablei(GL_BLEND, 1);
                p_glBlendFunc(GL_SRC_ALPHA, d->prim & GLR_PRIM_ADD ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA);
            } else {
                p_glDisable(GL_BLEND);
            }
            p_glDepthMask(d->prim & GLR_PRIM_NOZW ? GL_FALSE : GL_TRUE);
            p_glColorMaski(1, d->prim & GLR_PRIM_NOZW ? GL_FALSE : GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
            p_glProgramUniformMatrix4fv(sMeshProg, sMvpLoc, 1, GL_FALSE, d->mvp);
            p_glDrawArrays(GL_TRIANGLES, d->first, d->n);
            if (d->prim & GLR_PRIM_GLOW) {
                /* again into the glow buffer, against the scene's depth at its size (the game
                 * copies its Z buffer down to 128 x 112 for these) */
                if (depthDirty) {
                    p_glBlitNamedFramebuffer(sFbo, sGlowFbo, 0, 0, GLR_WIDTH, GLR_HEIGHT, 0, 0, GLR_GLOW_W,
                                             GLR_GLOW_H, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
                    depthDirty = 0;
                }
                p_glBindFramebuffer(GL_FRAMEBUFFER, sGlowFbo);
                p_glViewport(0, 0, GLR_GLOW_W, GLR_GLOW_H);
                p_glDrawArrays(GL_TRIANGLES, d->first, d->n);
                p_glBindFramebuffer(GL_FRAMEBUFFER, sFbo);
                p_glViewport(0, 0, GLR_WIDTH, GLR_HEIGHT);
            } else if (!(d->prim & GLR_PRIM_NOZW)) {
                depthDirty = 1;
            }
        }
        p_glDisable(GL_BLEND);
        p_glDepthMask(GL_TRUE);
        p_glDisable(GL_DEPTH_TEST);
    }

    /* the overlay (screen fades) */
    if (f->overlay >> 24) {
        float a = (float)(f->overlay >> 24) / 128.0f;

        p_glEnable(GL_BLEND);
        p_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        p_glUseProgram(sFillProg);
        p_glProgramUniform4f(sFillProg, sFillLoc, (float)(f->overlay & 0xFF) / 255.0f,
                             (float)((f->overlay >> 8) & 0xFF) / 255.0f, (float)((f->overlay >> 16) & 0xFF) / 255.0f,
                             a > 1.0f ? 1.0f : a);
        p_glBindVertexArray(sQuadVao);
        p_glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        p_glDisable(GL_BLEND);
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
