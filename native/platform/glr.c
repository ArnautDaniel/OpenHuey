/* The PC renderer: OpenGL 4.6 core with direct state access (see glr.h).
 *
 * The scene is drawn at the PS2's 640x448 times a render scale (glr_set_scale; HG_SCALE, the
 * options menu) into an offscreen target (glr_present), which is then scaled into the window;
 * the effects' work images stay at the PS2's sizes. Frame dumps read the scene at 640x448.
 *
 *   HG_GLDEBUG=1     once a second: the frame's draw count and its first vertex in clip space
 *                    (and, on glow frames, the glow buffer's brightest value and the bloom colour)
 *   HG_GLDEBUG_W=1   with it, the flags words of the first draws' vertices
 *   HG_POSTOFF=a,b   leave out these frame passes (bloom glow screen2 fog vignette), to compare */
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
    X(PFNGLPROGRAMUNIFORM2IPROC, glProgramUniform2i) \
    X(PFNGLCREATETEXTURESPROC, glCreateTextures) \
    X(PFNGLTEXTURESTORAGE2DPROC, glTextureStorage2D) \
    X(PFNGLTEXTURESUBIMAGE2DPROC, glTextureSubImage2D) \
    X(PFNGLTEXTUREPARAMETERIPROC, glTextureParameteri) \
    X(PFNGLBINDTEXTUREUNITPROC, glBindTextureUnit) \
    X(PFNGLDELETETEXTURESPROC, glDeleteTextures) \
    X(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers) \
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
    X(PFNGLBLENDFUNCSEPARATEPROC, glBlendFuncSeparate) \
    X(PFNGLBLENDEQUATIONPROC, glBlendEquation) \
    X(PFNGLBLENDCOLORPROC, glBlendColor) \
    X(PFNGLPOLYGONOFFSETPROC, glPolygonOffset) \
    X(PFNGLSTENCILFUNCPROC, glStencilFunc) \
    X(PFNGLSTENCILOPPROC, glStencilOp) \
    X(PFNGLSTENCILMASKPROC, glStencilMask) \
    X(PFNGLCOLORMASKPROC, glColorMask) \
    X(PFNGLSCISSORPROC, glScissor) \
    X(PFNGLCLEARNAMEDFRAMEBUFFERIVPROC, glClearNamedFramebufferiv) \
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

enum { POST_BLOOM = 1, POST_GLOW, POST_SCREEN2, POST_FOG, POST_VIGNETTE, POST_ALPHA_CLEAR, POST_CAUSTIC, POST_DOF, POST_MASK_CLEAR, POST_REFL, POST_SHADOW_BEGIN, POST_SHADOW_FILL, POST_IMAGE, POST_HAZE, POST_NEGATIVE, POST_ZOOM_BLUR, POST_PANIC, POST_MARKER };

typedef struct GlrFrame {
    uint32_t overlay;   /* a full-screen tint over the frame (RGBA, alpha 0x80 = 1.0; 0: none) */
    uint32_t tint[3];   /* the fading layers' (0x0F, 0x1A, 0x23) tint when drawn this frame */
    int soft;           /* layer 0x1C was set up this frame */
    int shine;          /* layer 0x14 was, with this colour */
    uint32_t shineColor;
    int late;           /* layer 0x23's re-run of the two-colour effect (colours, blurred) */
    uint32_t lateColor[2];
    int lateBlur;
    int tinted[3];
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

void glr_prim2d(int layer, int kind, int n, const float *xy, const float *st, const uint8_t *rgba, const void *tex,
                int csa, uint32_t prim) {
    /* identity: the vertex shader takes PS2 clip space (GS units around 2048 / 2047), z 1 the
     * nearest; GL samples pixels at their centres (+0.5), the GS at whole coordinates */
    static const float kIdentity[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    static const float kNoSt[8] = {0};
    float xyzw[64][4], tst[64][2];
    uint8_t trgba[64][4];
    uint64_t tex0 = 1ull << 34 | (uint64_t)(csa & 0x1F) << 56;
    int k, keep = sLayer;

    prim = (tex != NULL ? 0x10 : 0) | (prim & ~0x10u) | GLR_PRIM_NOZW | GLR_PRIM_NOZT;
    if (n < 3 || n > 64) {
        return;
    }
    for (k = 0; k < n; k++) {
        xyzw[k][0] = (xy[k * 2] + 0.5f - 256.0f) / 2047.0f;
        xyzw[k][1] = (xy[k * 2 + 1] + 0.5f - 224.0f) / 2047.0f;
        xyzw[k][2] = 1.0f;
        xyzw[k][3] = 0.0f;   /* (flags: drawn) */
        tst[k][0] = tex != NULL ? st[k * 2] : 0.0f;
        tst[k][1] = tex != NULL ? st[k * 2 + 1] : 0.0f;
        memcpy(trgba[k], rgba + k * 4, 4);
    }
    sLayer = layer;
    if (kind == GLR_2D_FAN) {   /* each triangle (0, k - 1, k) as its own strip */
        for (k = 2; k < n; k++) {
            float fx[3][4];
            float fs[3][2];
            uint8_t fc[3][4];
            int j, idx[3] = {0, k - 1, k};

            for (j = 0; j < 3; j++) {
                memcpy(fx[j], xyzw[idx[j]], sizeof(fx[j]));
                memcpy(fs[j], tst[idx[j]], sizeof(fs[j]));
                memcpy(fc[j], trgba[idx[j]], sizeof(fc[j]));
            }
            glr_strip(kIdentity, 3, &fx[0][0], &fs[0][0], &fc[0][0], tex, tex0, prim);
        }
    } else {
        glr_strip(kIdentity, n, &xyzw[0][0], tex != NULL ? &tst[0][0] : kNoSt, &trgba[0][0], tex, tex0, prim);
    }
    sLayer = keep;
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
    sFrames[sBuilding].tinted[0] = sFrames[sBuilding].tinted[1] = sFrames[sBuilding].tinted[2] = 0;
    sFrames[sBuilding].soft = 0;
    sFrames[sBuilding].shine = 0;
    sFrames[sBuilding].late = 0;
}


/* the fading layers: which of 0x0F / 0x1A / 0x23 (-1 none) */
static int tint_index(int layer) {
    return layer == 0x0F ? 0 : layer == 0x1A ? 1 : layer == 0x23 ? 2 : -1;
}

void glr_tint_layer(int layer, uint32_t tint) {
    int k = tint_index(layer);

    if (k >= 0) {
        sFrames[sBuilding].tint[k] = tint;
        sFrames[sBuilding].tinted[k] = 1;
    }
}

void glr_late_layer(uint32_t add, uint32_t contrast, int blur) {
    sFrames[sBuilding].late = 1;
    sFrames[sBuilding].lateColor[0] = add;
    sFrames[sBuilding].lateColor[1] = contrast;
    sFrames[sBuilding].lateBlur = blur;
}

void glr_shine_layer(uint32_t rgba) {
    sFrames[sBuilding].shine = 1;
    sFrames[sBuilding].shineColor = rgba;
}

void glr_soft_layer(void) {
    sFrames[sBuilding].soft = 1;
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

/* room 0x61's haze (func_00374E50, its packet's layer 0x2A): phase of the columns' waves, and
 * how far right all of it is moved */
void glr_haze(float phase, float sway) {
    GlrDraw *d = put_post(POST_HAZE, 0x2A, 0, 0);

    d->mvp[0] = phase;
    d->mvp[1] = sway;
    d->mvp[2] = 2.0f;   /* the waves' base strength */
    d->mvp[3] = 0.0f;   /* over the screen at 0x48 */
}

/* the heat haze effect D_0047A2F0 (func_00370100, layer 0x2A): as room 0x61's with the waves'
 * base strength `size` and no sway, over the screen by a horizontal alpha ramp (0x20 at the
 * edges, 0x60 in the middle, halved) */
void glr_haze2(float phase, float size) {
    GlrDraw *d = put_post(POST_HAZE, 0x2A, 0, 0);

    d->mvp[0] = phase;
    d->mvp[1] = 0.0f;
    d->mvp[2] = size;
    d->mvp[3] = 1.0f;
}

/* the panic screens (func_0021E1B0 / func_0021D290 / func_0021D8F0, their packets' layer 0x2A):
 * the negative, the zoom blur, and the threshold overlay (index below `limit` black, else
 * grey 0xC0, over the screen at amount / 2 of 128) */
void glr_negative(void) {
    put_post(POST_NEGATIVE, 0x2A, 0, 0);
}

void glr_zoom_blur(void) {
    put_post(POST_ZOOM_BLUR, 0x2A, 0, 0);
}

void glr_panic(int limit, int amount) {
    put_post(POST_PANIC, 0x2A, (uint32_t)limit, (uint32_t)amount);
}

/* the marker glow (func_00301E70, layer 0xB): 32 spheres about a point (radius 10 + 0.3 k, the
 * later ones + `jitter`) counted where they show in front of the scene, blurred, and added over
 * the screen in orange at `fix` / 128. The point: game pixels (x, y), view depth z, `scale`
 * game pixels per unit there */
void glr_marker(float x, float y, float z, float scale, float jitter, int fix) {
    GlrDraw *d = put_post(POST_MARKER, 0xB, (uint32_t)fix, 0);

    d->mvp[0] = x;
    d->mvp[1] = y;
    d->mvp[2] = z;
    d->mvp[3] = scale;
    d->mvp[4] = jitter;
}

void glr_dof(float a, float from, float to, float b) {
    GlrDraw *d = put_post(POST_DOF, sLayer, 0, 0);

    d->mvp[0] = a;
    d->mvp[1] = from;
    d->mvp[2] = to;
    d->mvp[3] = b;
}

static int sReflFlip;   /* the reflection's mirror (renderer +0x8C): 0 left / right, 1 top / bottom */

void glr_refl_flip(int flip) {
    sReflFlip = flip;
}

void glr_mask_clear(void) {
    put_post(POST_MASK_CLEAR, sLayer, 0, 0);
}

void glr_refl(int prep, int fix, int flip, int masked, float dx) {
    GlrDraw *d = put_post(POST_REFL, sLayer, (uint32_t)fix, (prep != 0) | (flip != 0) << 1 | (masked != 0) << 2);

    d->mvp[0] = dx;
}

static int sShadowMark = -1, sShadowMarkV;   /* where this shadow's entries began (glr_shadow_cancel) */

void glr_shadow_begin(void) {
    GlrFrame *f = &sFrames[sBuilding];

    sShadowMark = f->nd;
    sShadowMarkV = f->nv;
    put_post(POST_SHADOW_BEGIN, sLayer, 0, 0);
}

void glr_shadow_cancel(void) {
    GlrFrame *f = &sFrames[sBuilding];

    if (sShadowMark >= 0 && sShadowMark <= f->nd) {
        f->nd = sShadowMark;
        f->nv = sShadowMarkV;
    }
    sShadowMark = -1;
}

/* a shadow volume quad (4 points in clip space, strip order), counting +1 (`inc`) or -1 */
void glr_shadow_quad(const float *xyz, int inc) {
    static const float kIdentity[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    float xyzw[4][4], st[4][2] = {{0}};
    uint8_t rgba[4][4] = {{0}};
    int k;

    for (k = 0; k < 4; k++) {
        uint32_t flags = k < 2 ? 0x8000 : 0;

        xyzw[k][0] = xyz[k * 3];
        xyzw[k][1] = xyz[k * 3 + 1];
        xyzw[k][2] = xyz[k * 3 + 2];
        memcpy(&xyzw[k][3], &flags, 4);
    }
    glr_strip(kIdentity, 4, &xyzw[0][0], &st[0][0], &rgba[0][0], NULL, 0,
              GLR_PRIM_STENCIL | (inc ? GLR_PRIM_STENCIL_INC : 0));
}

void glr_shadow_fill(float x0, float y0, float x1, float y1, uint32_t rgba) {
    GlrDraw *d = put_post(POST_SHADOW_FILL, sLayer, rgba, 0);

    d->mvp[0] = x0;
    d->mvp[1] = y0;
    d->mvp[2] = x1;
    d->mvp[3] = y1;
    sShadowMark = -1;
}

/* images the game sent to VRAM (renderer +0x40) that it then shows whole: the last one */
static uint32_t sImgAddr = 0xFFFFFFFF;
static uint8_t *sImgPixels;
static int sImgW, sImgH, sImgDirty;
static GLuint sImgTex;

void glr_vram_upload(uint32_t addr, const void *rgba, int w, int h) {
    if (w <= 0 || h <= 0 || w > 1024 || h > 1024) {
        return;
    }
    if (sImgPixels == NULL) {
        sImgPixels = malloc(1024 * 1024 * 4);
    }
    memcpy(sImgPixels, rgba, (size_t)w * h * 4);
    sImgAddr = addr;
    sImgW = w;
    sImgH = h;
    sImgDirty = 1;
}

void glr_vram_draw(uint32_t addr, int layer) {
    if (addr == sImgAddr) {
        put_post(POST_IMAGE, layer, 0, 0);
    }
}

/* an image sent straight into the frame (address 0x88000, the screen): it replaces it */
void glr_vram_blit(int layer) {
    put_post(POST_IMAGE, layer, 0, 1);
}

void glr_caustic_begin(void) {
    put_post(POST_ALPHA_CLEAR, sLayer, 0, 0);
}

void glr_caustic_glow(int aref) {
    put_post(POST_CAUSTIC, sLayer, 0, (uint32_t)aref);
}

void glr_screen2(uint32_t rgba, int contrast, int blur) {
    put_post(POST_SCREEN2, sLayer, rgba, (contrast != 0) | (blur != 0) << 1);
}

/* ---- GL objects ---- */

/* the scene's targets: the PS2's 640 x 448 times the render scale (glr_set_scale); the effects'
 * half-size images stay at the PS2's sizes */
static int sScale = 1, sWantScale = 1;
#define GLR_WIDTH (640 * sScale)
#define GLR_HEIGHT (448 * sScale)
#define GLR_GLOW_W 128   /* the renderer's work buffer (frame page 0x1F0) */
#define GLR_GLOW_H 112

static GLuint sMeshProg, sQuadProg, sFillProg, sVao, sQuadVao, sVbo;
static GLint sFillLoc;
static GLint sMvpLoc, sTexModeLoc, sTccLoc, sFbaLoc;
static GLuint sFbo, sColor, sDepth, sViewDepth, sGsTex;   /* sViewDepth: each pixel's view depth */
/* the glow buffer (B, kept between frames) with the scene's depth at its size, and the
 * scratch buffer its fade goes through (A) */
static GLuint sGlowFbo, sGlowB, sGlowDepth, sGlowAFbo, sGlowA, sPostProg;
static GLint sPostScaleLoc;
static GLint sPostModeLoc, sPostColorLoc, sPostColor2Loc, sPostRangeLoc, sPostFixLoc, sPostBandLoc, sPostOffLoc;
/* the bloom's half-size images: H, H2 and the next H */
#define GLR_HALF_W 256
#define GLR_HALF_H 224
static GLuint sHalf[3], sHalfFbo[3];
static GLuint sCopy, sCopyFbo;   /* a copy of the screen (passes that read it while drawing over it) */
/* the reflection (layer 0x17's draws, from the mirrored camera's half-size matrices), its
 * prepared half-size image, and the mask its quads mark (depth tested against the scene) */
static GLuint sRefl, sReflDepth, sReflFbo, sReflPrep, sReflPrepFbo, sMask, sMaskFbo;
/* layer 6 (shadows): its colour and depth-stencil, the scene's depth at half size in the middle
 * (where the camera's half-size matrices draw) */
static GLuint sShadowCol, sShadowDS, sShadowFbo;
static GLuint sDump, sDumpFbo;
static GLuint sTintHalf, sTintHalfFbo;
static GLuint sSoft, sSoftFbo;   /* layer 0x1C: its draws at 256 x 256 */   /* a fading layer's background: the screen halved before it */
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
     * of which the 512 x 448 around the centre is the screen (the game draws 512 wide and the
     * display stretches it to 640; the camera's aspect allows for that); y grows downwards,
     * larger z is nearer */
    "    gl_Position = vec4(p.x * (2047.0 / 256.0), -p.y * (2047.0 / 224.0), -p.z, p.w);\n"
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
    "uniform int uFba;\n"   /* the frame's alpha written with bit 7 set (GS FBA) */
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
    "    if (uFba != 0) oColor.a = 1.0;\n"
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
    "    oColor = vec4(texture(uTex, vUv).rgb, 0.0);\n"
    "}\n";

/* The post passes, in the GS's 8-bit arithmetic (colours 0..255, sprites modulated by their
 * colour with 0x80 = 1.0, blends (A - B) * FIX >> 7 + D clamped). Textures are sampled whole
 * texels except where noted; "H" is the half-size (256 x 224) copy of the screen.
 *
 * The glow (renderer +0x58), B the 128 x 112 glow buffer:
 *   0  B fades, through A: A = B * 0x40 >> 7, then B + ((A - B) * 0x40 >> 7)
 *   1  over the screen: B * 0x40 >> 7 added, B stretched with bilinear filtering
 * The screen bloom (func_002699D0):
 *   2  H = the screen at every other pixel (uFix < 0: only where the frame's alpha isn't 0,
 *      the bloom's alpha test)
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
 *   12 H where its alpha (the frame's) >= uFix, else nothing
 *   13 H + the sum of H * 0x40 >> 7 at the 4 diagonal neighbours
 *      (the caustic, func_0034E9E0: 2, 12, 13, then 6 at colour 0x80 and FIX 0x40, added)
 *   15 D + ((S at p - offset) - D) * 0x40 >> 7 (D the target's own image, S the other)
 *   16 the colour with the depth of field's alpha: 0x80, stepping 0x60 .. 0 over a .. from,
 *      0x20 .. 0x80 over to .. b (view depth, from the depth image; last band passed wins)
 *   17 over the screen by its alpha, stretched
 *      (the depth of field, func_002C6650: 2, eight 15s ping-ponging, 16, 17)
 * The reflecting floor (func_00317D40 / func_00316DE0), R the reflection's middle at half size:
 *   18 R blurred: 4 diagonal neighbours lerped in at 1/4 each, then where R's alpha isn't 0
 *      R lerped back in at 3/4 with its alpha (else alpha 0)
 *   21 that stretched over the screen, mirrored left / right (uOff.x 0; moved by uRange.x
 *      pixels) or top / bottom, only where its alpha isn't 0 and (uOff.y) the mask is marked;
 *      blended by FIX (constant colour)
 *   22 the screen halved and mirrored into the reflection's middle (its background, alpha 0)
 * The fading layers 0x0F / 0x1A / 0x23 (func_001B18E0 / func_001AB3F0 / func_001AC0D0, ends
 * func_001B1370 / func_001AAE80): the frame's alpha cleared and the screen halved at their
 * start, their draws marking alpha (FBA), then on the marked pixels (a copy of the screen):
 *   25 the tint's alpha a above 0x80: towards the tint by ((a + 0x81) & 0xFF) / 128; else
 *      towards the halved background times the tint by (0x80 - a) / 128
 * Layer 0x14 (func_001AF3B0): its draws mark the frame's alpha (cleared at its start), then
 *   29 the screen halved where that alpha is >= 0x80 and green >= 0x40 (the PS2 moves green's
 *      top bits into alpha - renderer +0x54 - and tests alpha >= 0x10)
 *   30 that + 8 neighbours at 1/2, then 6 at the renderer's colour (+0x304D58) and FIX 0x40,
 *      added - its bright parts glow
 * Layer 0x1C (func_001AB960): its draws mark the frame's alpha (cleared at its start), then
 *   27 the screen at 256 x 256 (every other column, 7 rows in 4) where that alpha isn't 0
 *   28 that stretched back over the screen by its alpha (bilinear) - its draws softened
 * Layer 6 (shadows), S its buffer's middle at half size:
 *   23 a flat colour (the shadow's colour where its volumes count)
 *   24 S + the sum of S * 0x10 >> 7 at 8 neighbours; then 6 at colour 0x80, FIX 0x40,
 *      subtracted from the screen
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
    "uniform float uS;\n"   /* the render scale */
    "uniform vec4 uBand;\n"   /* the depth of field: a, from, to, b */
    "uniform ivec2 uOff;\n"   /* a pass's offset (GL pixels) */
    "uniform sampler2D uTex2;\n"
    "uniform float uFix;\n"
    "out vec4 oColor;\n"
    "vec4 at(sampler2D t, ivec2 p) {\n"   /* 0..255; outside the image: nothing */
    "    ivec2 sz = textureSize(t, 0);\n"
    "    if (p.x < 0 || p.y < 0 || p.x >= sz.x || p.y >= sz.y) return vec4(0.0);\n"
    "    return round(texelFetch(t, p, 0) * 255.0);\n"
    "}\n"
    "vec4 refl(ivec2 q) {\n"   /* the reflection's middle at half size (0..255 x 0..223) */
    "    return round(texelFetch(uTex, ivec2(int((160.0 + float(q.x) * 1.25) * uS), int(float(112 + q.y) * uS)), 0) * 255.0);\n"
    "}\n"
    "float corner(vec2 q, vec2 c, vec2 a, vec2 b) {\n"   /* c's barycentric weight; 0 outside */
    "    float det = (a.y - b.y) * (c.x - b.x) + (b.x - a.x) * (c.y - b.y);\n"
    "    float wc = ((a.y - b.y) * (q.x - b.x) + (b.x - a.x) * (q.y - b.y)) / det;\n"
    "    float wa = ((b.y - c.y) * (q.x - b.x) + (c.x - b.x) * (q.y - b.y)) / det;\n"
    "    return wc >= 0.0 && wa >= 0.0 && wc + wa <= 1.0 ? wc : 0.0;\n"
    "}\n"
    "vec4 haze_at(vec2 q) {\n"   /* the screen at half size, point q (0..256 x 0..224) */
    "    vec2 sz = vec2(textureSize(uTex, 0));\n"
    "    return round(texture(uTex, vec2(q.x * 2.5 * uS, (448.0 - q.y * 2.0) * uS) / sz) * 255.0);\n"
    "}\n"
    "float haze_off(float k) {\n"   /* column k's move down (half-size pixels, in 1/16) */
    "    float a = uRange.x + k * 1.5707964;\n"
    "    return trunc(16.0 * sin(a) * (uBand.x + 0.5 * (1.0 + cos(a)))) / 16.0;\n"
    "}\n"
    "float marker_count(vec2 g) {\n"   /* the shells over game pixel g that show (0..32) */
    "    vec2 gl = vec2(g.x * 1.25, 448.0 - g.y) * uS;\n"
    "    float zs = texelFetch(uTex1, ivec2(clamp(gl, vec2(0.0), vec2(textureSize(uTex1, 0)) - 1.0)), 0).r;\n"
    "    float dist = length(g - uBand.xy);\n"
    "    for (int k = 0; k < 32; k++) {\n"
    "        float r = 10.0 + 0.3 * float(k) + (k > 0 ? uRange.x : 0.0);\n"
    "        float R = r * uBand.w;\n"
    "        if (dist < R && uBand.z - r * sqrt(1.0 - (dist / R) * (dist / R)) <= zs) return float(32 - k);\n"
    "    }\n"
    "    return 0.0;\n"
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
    "        vec2 t = (gl_FragCoord.xy - 0.5) * sz / (vec2(640.0, 448.0) * uS) + 0.5;\n"
    "        vec4 h = round(texture(uTex, t / sz) * 255.0);\n"
    "        c = uMode == 1 ? floor(h * 0.5) : floor(floor(h * uColor / 128.0) * uFix / 128.0);\n"
    "    } else if (uMode == 2) {\n"   /* the screen is 640 wide here, the game's 512 */
    "        c = at(uTex, ivec2(int(float(p.x * 2) * 1.25 * uS), int(float(p.y * 2 + 1) * uS)));\n"
    "        if (uFix < 0.0 && c.a == 0.0) c = vec4(0.0);\n"   /* the bloom's alpha test: frame alpha != 0 */
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
    "        vec2 q = vec2(gl_FragCoord.x * (512.0 / 640.0), 448.0 * uS - gl_FragCoord.y) / uS;\n"   /* the game's pixels */
    "        float o = uRange.x, w = 0.0;\n"
    "        w = max(w, corner(q, vec2(0.0, 0.0), vec2(256.0 + o, 0.0), vec2(0.0, 224.0 - o)));\n"
    "        w = max(w, corner(q, vec2(512.0, 0.0), vec2(256.0 + o, 0.0), vec2(512.0, 224.0 + o)));\n"
    "        w = max(w, corner(q, vec2(0.0, 448.0), vec2(256.0 - o, 448.0), vec2(0.0, 224.0 - o)));\n"
    "        w = max(w, corner(q, vec2(512.0, 448.0), vec2(256.0 - o, 448.0), vec2(512.0, 224.0 + o)));\n"
    "        vec4 d = at(uTex, p);\n"
    "        c = d + floor(-d * floor(uFix * w) / 128.0);\n"
    "    } else if (uMode == 12) {\n"   /* where the frame's alpha (0x80 = 255 here) >= uFix */
    "        c = at(uTex, p);\n"
    "        if (round(c.a * 128.0 / 255.0) < uFix) c = vec4(0.0);\n"
    "    } else if (uMode == 13) {\n"
    "        c = at(uTex, p);\n"
    "        c += floor(at(uTex, p + ivec2(-1, -1)) * 0.5); c += floor(at(uTex, p + ivec2(-1, 1)) * 0.5);\n"
    "        c += floor(at(uTex, p + ivec2(1, -1)) * 0.5);  c += floor(at(uTex, p + ivec2(1, 1)) * 0.5);\n"
    "    } else if (uMode == 15) {\n"   /* dst + ((src at p - off) - dst) * 0x40 >> 7 where covered */
    "        c = at(uTex, p);\n"
    "        ivec2 q = p - uOff;\n"
    "        ivec2 sz = textureSize(uTex1, 0);\n"
    "        if (q.x >= 0 && q.y >= 0 && q.x < sz.x && q.y < sz.y) c += floor((at(uTex1, q) - c) * 0.5);\n"
    "    } else if (uMode == 16) {\n"   /* the blur's colour with the depth band's alpha (0x80 = 128) */
    "        float d = texelFetch(uTex2, ivec2(int(float(p.x * 2) * 1.25 * uS), int(float(p.y * 2 + 1) * uS)), 0).r;\n"
    "        float a = 128.0;\n"
    "        for (int k = 0; k < 8; k++) {\n"
    "            float z = k < 4 ? uBand.x + float(k + 1) * 0.25 * (uBand.y - uBand.x)\n"
    "                            : uBand.z + float(k - 3) * 0.25 * (uBand.w - uBand.z);\n"
    "            if (d >= z) a = k < 4 ? float(96 - k * 32) : float((k - 3) * 32);\n"
    "        }\n"
    "        c = vec4(at(uTex, p).rgb, a);\n"
    "    } else if (uMode == 17) {\n"   /* over the screen by its alpha: (Cs - Cd) * As + Cd */
    "        vec2 sz = vec2(textureSize(uTex, 0));\n"
    "        vec2 t = (gl_FragCoord.xy - 0.5) * sz / (vec2(640.0, 448.0) * uS) + 0.5;\n"
    "        vec4 h = texture(uTex, t / sz) * 255.0;\n"
    "        oColor = vec4(h.rgb / 255.0, clamp(h.a / 128.0, 0.0, 1.0));\n"
    "        return;\n"
    "    } else if (uMode == 18) {\n"   /* the reflection prepared (see above) */
    "        c = refl(p);\n"
    "        vec4 r = c;\n"
    "        const ivec2 taps[4] = ivec2[4](ivec2(-1, 1), ivec2(1, 1), ivec2(-1, -1), ivec2(1, -1));\n"
    "        for (int k = 0; k < 4; k++) {\n"
    "            ivec2 q = p - taps[k];\n"
    "            if (q.x >= 0 && q.y >= 0 && q.x < 256 && q.y < 224) {\n"
    "                vec4 t = refl(q);\n"
    "                c = vec4(c.rgb + floor((t.rgb - c.rgb) * 0.25), t.a);\n"
    "            }\n"
    "        }\n"
    "        c = r.a != 0.0 ? vec4(c.rgb + floor((r.rgb - c.rgb) * 0.75), r.a) : vec4(c.rgb, 0.0);\n"
    "        oColor = clamp(c / 255.0, 0.0, 1.0);\n"
    "        return;\n"
    "    } else if (uMode == 21) {\n"   /* the prepared reflection stretched over the screen */
    "        vec2 g = vec2(gl_FragCoord.x * (512.0 / 640.0), 448.0 * uS - gl_FragCoord.y) / uS;\n"   /* the game's pixels */
    "        vec2 uv = uOff.x != 0 ? vec2(g.x * 0.5, (448.0 - g.y) * 0.5)\n"
    "                              : vec2((512.0 + uRange.x - g.x) * 0.5, g.y * 0.5);\n"
    "        if (uv.x < 0.0 || uv.x >= 256.0) discard;\n"
    "        if (uOff.y != 0 && texelFetch(uTex1, p, 0).r == 0.0) discard;\n"
    "        vec4 h = texture(uTex, vec2(uv.x, 224.0 - uv.y) / vec2(256.0, 224.0));\n"
    "        if (round(h.a * 255.0) == 0.0) discard;\n"
    "        oColor = vec4(h.rgb, 1.0);\n"
    "        return;\n"
    "    } else if (uMode == 22) {\n"   /* the screen mirrored into the reflection's middle (half size) */
    "        vec2 q = gl_FragCoord.xy - vec2(160.0, 112.0) * uS;\n"   /* 0 .. 320 x 0 .. 224, scaled */
    "        vec2 s = vec2(q.x * 2.0, q.y * 2.0);\n"
    "        if (uOff.x != 0) s.y = 448.0 * uS - 1.0 - s.y; else s.x = 640.0 * uS - 1.0 - s.x;\n"
    "        oColor = vec4(texelFetch(uTex, ivec2(s), 0).rgb, 0.0);\n"
    "        return;\n"
    "    } else if (uMode == 23) {\n"   /* a flat colour (uColor 0..255, alpha 0..128) */
    "        oColor = vec4(uColor.rgb / 255.0, clamp(uColor.a / 128.0, 0.0, 1.0));\n"
    "        return;\n"
    "    } else if (uMode == 24) {\n"   /* layer 6's buffer blurred: S + the sum of S >> 3 at 8 neighbours */
    "        c = refl(p);\n"
    "        const ivec2 taps[8] = ivec2[8](ivec2(-1, -1), ivec2(1, -1), ivec2(-1, 1), ivec2(1, 1),\n"
    "                                       ivec2(-2, 0), ivec2(2, 0), ivec2(0, -2), ivec2(0, 2));\n"
    "        for (int k = 0; k < 8; k++) {\n"
    "            ivec2 q = p + taps[k];\n"
    "            if (q.x >= 0 && q.y >= 0 && q.x < 256 && q.y < 224) c += floor(refl(q) / 8.0);\n"
    "        }\n"
    "    } else if (uMode == 25) {\n"   /* a fading layer's end, on the pixels it drew (alpha bit 7) */
    "        vec4 d = at(uTex, p);\n"
    "        if (d.a < 128.0) { oColor = d / 255.0; return; }\n"
    "        float a = uColor.a;\n"
    "        if (uOff.x != 0) {\n"   /* tint alpha above 0x80: towards the tint by (a + 0x81) & 0xFF */
    "            float as = mod(a + 129.0, 256.0);\n"
    "            c = vec4(d.rgb + floor((uColor.rgb - d.rgb) * as / 128.0), d.a);\n"
    "        } else {\n"   /* else: back to the background (times the tint) by 0x80 - a */
    "            vec2 sz = vec2(textureSize(uTex1, 0));\n"
    "            vec2 t = (gl_FragCoord.xy - 0.5) * sz / (vec2(640.0, 448.0) * uS) + 0.5;\n"
    "            vec4 bg = floor(round(texture(uTex1, t / sz) * 255.0) * vec4(uColor.rgb, 128.0) / 128.0);\n"
    "            c = vec4(d.rgb + floor((bg.rgb - d.rgb) * (128.0 - a) / 128.0), d.a);\n"
    "        }\n"
    "        oColor = clamp(c / 255.0, 0.0, 1.0);\n"
    "        return;\n"
    "    } else if (uMode == 26) {\n"   /* an uploaded image over the whole screen, by its alpha (0x80 = 1) */
    "        vec2 t = gl_FragCoord.xy / (vec2(640.0, 448.0) * uS);\n"
    "        vec4 c4 = texture(uTex, vec2(t.x, 1.0 - t.y));\n"
    "        oColor = vec4(c4.rgb, clamp(c4.a * 255.0 / 128.0, 0.0, 1.0));\n"
    "        return;\n"
    "    } else if (uMode == 27) {\n"   /* layer 0x1C: the screen at 256 x 256 where its alpha isn't 0 */
    "        float v = 255.0 - float(p.y);\n"   /* rows from the top */
    "        vec4 d = at(uTex, ivec2(int(float(p.x * 2) * 1.25 * uS), int((447.0 - 1.75 * v) * uS)));\n"
    "        oColor = d.a == 0.0 ? vec4(0.0) : d / 255.0;\n"
    "        return;\n"
    "    } else if (uMode == 28) {\n"   /* that stretched back over the screen (from 1 pixel left / up) */
    "        vec2 g = vec2(gl_FragCoord.x * (512.0 / 640.0), 448.0 * uS - gl_FragCoord.y) / uS;\n"
    "        vec2 uv = vec2((g.x + 1.0) * 256.0 / 513.0, (g.y + 1.0) * 256.0 / 449.0);\n"
    "        vec4 h = texture(uTex, vec2(uv.x, 256.0 - uv.y) / 256.0);\n"
    "        oColor = vec4(h.rgb, clamp(h.a * 255.0 / 128.0, 0.0, 1.0));\n"
    "        return;\n"
    "    } else if (uMode == 29) {\n"   /* layer 0x14: its bright pixels halved (alpha >= 0x80, green >= 0x40) */
    "        vec4 d = at(uTex, ivec2(int(float(p.x * 2) * 1.25 * uS), int(float(p.y * 2 + 1) * uS)));\n"
    "        c = d.a >= 128.0 && d.g >= 64.0 ? d : vec4(0.0);\n"
    "    } else if (uMode == 30) {\n"   /* H + the sum of H * 0x40 >> 7 at 8 neighbours */
    "        c = at(uTex, p);\n"
    "        c += floor(at(uTex, p + ivec2(-1, -1)) * 0.5); c += floor(at(uTex, p + ivec2(1, -1)) * 0.5);\n"
    "        c += floor(at(uTex, p + ivec2(-1, 1)) * 0.5);  c += floor(at(uTex, p + ivec2(1, 1)) * 0.5);\n"
    "        c += floor(at(uTex, p + ivec2(-2, 0)) * 0.5);  c += floor(at(uTex, p + ivec2(2, 0)) * 0.5);\n"
    "        c += floor(at(uTex, p + ivec2(0, -2)) * 0.5);  c += floor(at(uTex, p + ivec2(0, 2)) * 0.5);\n"
    "    } else if (uMode == 31) {\n"   /* the marked pixels (alpha bit 7), for the stencil */
    "        if (at(uTex, p).a < 128.0) discard;\n"
    "        oColor = vec4(0.0);\n"
    "        return;\n"
    "    } else if (uMode == 32) {\n"   /* room 0x61's haze (see glr_haze's pass) */
    "        vec2 g = vec2(gl_FragCoord.x * (512.0 / 640.0), 448.0 * uS - gl_FragCoord.y) / uS;\n"
    "        vec2 h = floor(g * 0.5) + 0.5;\n"
    "        vec4 d = at(uTex, p);\n"
    "        vec4 hc = haze_at(h);\n"
    "        vec4 b = hc;\n"
    "        float u = h.x - uRange.y;\n"
    "        if (u >= 0.0 && u <= 256.0) {\n"
    "            float f = u / 8.0, k = floor(f);\n"
    "            float v = h.y - mix(haze_off(k), haze_off(k + 1.0), f - k);\n"
    "            if (v >= 0.0 && v < 224.0) b = hc + floor((haze_at(vec2(u, v)) - hc) * 0.5);\n"
    "        }\n"
    "        float amt = uFix;\n"
    "        if (uBand.y > 0.0) amt = floor(floor(32.0 + 64.0 * (1.0 - abs(h.x - 128.0) / 128.0)) * 64.0 / 128.0);\n"
    "        c = d + floor((b - d) * amt / 128.0);\n"
    "    } else if (uMode == 36) {\n"   /* the marker glow (see glr_marker) */
    "        vec2 g = vec2(gl_FragCoord.x * (512.0 / 640.0), 448.0 * uS - gl_FragCoord.y) / uS;\n"
    "        vec2 h = (floor(g * 0.5) + 0.5) * 2.0;\n"   /* the half-size pixel */
    "        float w = marker_count(h);\n"
    "        w += floor(marker_count(h + vec2(2.0, 2.0)) * 64.0 / 128.0);\n"
    "        w += floor(marker_count(h + vec2(-2.0, 2.0)) * 48.0 / 128.0);\n"
    "        w += floor(marker_count(h + vec2(2.0, -2.0)) * 32.0 / 128.0);\n"
    "        w += floor(marker_count(h + vec2(-2.0, -2.0)) * 16.0 / 128.0);\n"
    "        vec4 d = at(uTex, p);\n"
    "        c = d + floor(floor(w * vec4(128.0, 96.0, 48.0, 0.0) / 128.0) * uFix / 128.0);\n"
    "    } else if (uMode == 33) {\n"   /* the negative: (0x80 - Cd) * 0x80 >> 7, clamped */
    "        c = max(vec4(0.0), 128.0 - at(uTex, p));\n"
    "    } else if (uMode == 34) {\n"   /* the screen copy 16 pixels bigger each way, bilinear, over it at 0x40 */
    "        vec2 g = vec2(gl_FragCoord.x * (512.0 / 640.0), 448.0 * uS - gl_FragCoord.y) / uS;\n"
    "        vec2 uv = vec2((g.x + 16.0) * 512.0 / 544.0, (g.y + 16.0) * 448.0 / 480.0);\n"
    "        vec2 sz = vec2(textureSize(uTex, 0));\n"
    "        vec4 z = round(texture(uTex, vec2(uv.x * 1.25 * uS, (448.0 - uv.y) * uS) / sz) * 255.0);\n"
    "        vec4 d = at(uTex, p);\n"
    "        c = d + floor((z - d) * 64.0 / 128.0);\n"
    "    } else if (uMode == 35) {\n"   /* panic: index = green bits 6-7 | GS alpha bits 0-5 */
    "        vec4 d = at(uTex, p);\n"
    "        float ga = round(d.a * 128.0 / 255.0);\n"
    "        float idx = floor(d.g / 64.0) * 64.0 + mod(ga, 64.0);\n"
    "        vec4 cs = idx < uFix ? vec4(0.0) : vec4(192.0);\n"
    "        c = d + floor((cs - d) * uRange.x / 128.0);\n"
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
    "        vec2 t = (gl_FragCoord.xy - 0.5) * sz / (vec2(640.0, 448.0) * uS) + 0.5;\n"
    "        vec4 cs = floor(round(texture(uTex, t / sz) * 255.0) * uColor / 128.0);\n"
    "        vec4 d = at(uTex1, p);\n"
    "        c = d + floor((d - cs) * uFix / 128.0);\n"
    "    }\n"
    "    oColor = vec4(clamp(c.rgb, 0.0, 255.0) / 255.0, uMode == 16 ? c.a / 255.0 : 1.0);\n"
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

static const uint8_t kFont[65][5] = {   /* 5 x 7 glyphs for ASCII 32..96, columns low bit at top */
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x1C, 0x22, 0x41, 0x00},
    {0x00, 0x41, 0x22, 0x1C, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x08, 0x08, 0x08, 0x08, 0x08},
    {0x00, 0x60, 0x60, 0x00, 0x00},
    {0x20, 0x10, 0x08, 0x04, 0x02},
    {0x3E, 0x51, 0x49, 0x45, 0x3E},
    {0x00, 0x42, 0x7F, 0x40, 0x00},
    {0x42, 0x61, 0x51, 0x49, 0x46},
    {0x21, 0x41, 0x45, 0x4B, 0x31},
    {0x18, 0x14, 0x12, 0x7F, 0x10},
    {0x27, 0x45, 0x45, 0x45, 0x39},
    {0x3C, 0x4A, 0x49, 0x49, 0x30},
    {0x01, 0x71, 0x09, 0x05, 0x03},
    {0x36, 0x49, 0x49, 0x49, 0x36},
    {0x06, 0x49, 0x49, 0x29, 0x1E},
    {0x00, 0x36, 0x36, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x08, 0x14, 0x22, 0x41, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x41, 0x22, 0x14, 0x08},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x7E, 0x11, 0x11, 0x11, 0x7E},
    {0x7F, 0x49, 0x49, 0x49, 0x36},
    {0x3E, 0x41, 0x41, 0x41, 0x22},
    {0x7F, 0x41, 0x41, 0x22, 0x1C},
    {0x7F, 0x49, 0x49, 0x49, 0x41},
    {0x7F, 0x09, 0x09, 0x09, 0x01},
    {0x3E, 0x41, 0x49, 0x49, 0x7A},
    {0x7F, 0x08, 0x08, 0x08, 0x7F},
    {0x00, 0x41, 0x7F, 0x41, 0x00},
    {0x20, 0x40, 0x41, 0x3F, 0x01},
    {0x7F, 0x08, 0x14, 0x22, 0x41},
    {0x7F, 0x40, 0x40, 0x40, 0x40},
    {0x7F, 0x02, 0x0C, 0x02, 0x7F},
    {0x7F, 0x04, 0x08, 0x10, 0x7F},
    {0x3E, 0x41, 0x41, 0x41, 0x3E},
    {0x7F, 0x09, 0x09, 0x09, 0x06},
    {0x3E, 0x41, 0x51, 0x21, 0x5E},
    {0x7F, 0x09, 0x19, 0x29, 0x46},
    {0x46, 0x49, 0x49, 0x49, 0x31},
    {0x01, 0x01, 0x7F, 0x01, 0x01},
    {0x3F, 0x40, 0x40, 0x40, 0x3F},
    {0x1F, 0x20, 0x40, 0x20, 0x1F},
    {0x3F, 0x40, 0x38, 0x40, 0x3F},
    {0x63, 0x14, 0x08, 0x14, 0x63},
    {0x07, 0x08, 0x70, 0x08, 0x07},
    {0x61, 0x51, 0x49, 0x45, 0x43},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x01, 0x02, 0x04, 0x00}
};

/* the PC options menu (options.c): lines of text in a panel over the window, `sel` lit */
static const char *const *sMenu;
static int sMenuN, sMenuSel;
static GLuint sFontTex, sTextProg, sTextVao, sTextVbo;
static GLint sTextColorLoc;

void glr_menu(const char *const *lines, int n, int sel) {
    sMenu = lines;
    sMenuN = n;
    sMenuSel = sel;
}

static const char *kTextVs =
    "#version 460 core\n"
    "layout(location = 0) in vec2 aPos;\n"
    "layout(location = 1) in vec2 aUv;\n"
    "out vec2 vUv;\n"
    "void main() {\n"
    "    vUv = aUv;\n"
    "    gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "}\n";

static const char *kTextFs =
    "#version 460 core\n"
    "in vec2 vUv;\n"
    "uniform sampler2D uTex;\n"
    "uniform vec4 uColor;\n"
    "out vec4 oColor;\n"
    "void main() {\n"
    "    oColor = vec4(uColor.rgb, uColor.a * texture(uTex, vUv).r);\n"
    "}\n";

static GLuint program(const char *vs, const char *fs);

static void font_make(void) {
    static uint8_t px[8][65 * 6];
    int c, x, y;

    for (c = 0; c < 65; c++) {
        for (x = 0; x < 5; x++) {
            for (y = 0; y < 7; y++) {
                px[y][c * 6 + x] = (kFont[c][x] >> y) & 1 ? 255 : 0;
            }
        }
    }
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sFontTex);
    p_glTextureStorage2D(sFontTex, 1, GL_R8, 65 * 6, 8);
    p_glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    p_glTextureSubImage2D(sFontTex, 0, 0, 0, 65 * 6, 8, GL_RED, GL_UNSIGNED_BYTE, px);
    p_glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    p_glTextureParameteri(sFontTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    p_glTextureParameteri(sFontTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    sTextProg = program(kTextVs, kTextFs);
    sTextColorLoc = p_glGetUniformLocation(sTextProg, "uColor");
    p_glCreateBuffers(1, &sTextVbo);
    p_glCreateVertexArrays(1, &sTextVao);
    p_glVertexArrayVertexBuffer(sTextVao, 0, sTextVbo, 0, 4 * sizeof(float));
    p_glEnableVertexArrayAttrib(sTextVao, 0);
    p_glVertexArrayAttribFormat(sTextVao, 0, 2, GL_FLOAT, GL_FALSE, 0);
    p_glVertexArrayAttribBinding(sTextVao, 0, 0);
    p_glEnableVertexArrayAttrib(sTextVao, 1);
    p_glVertexArrayAttribFormat(sTextVao, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));
    p_glVertexArrayAttribBinding(sTextVao, 1, 0);
}

/* the menu over the window (w x h pixels): a dark panel, its lines (the picked one yellow) */
static void menu_draw(int w, int h) {
    static float v[64 * 64 * 6 * 4];
    int k = h / 200 > 1 ? h / 200 : 1;   /* glyph pixels per font pixel */
    int cw = 6 * k, ch = 10 * k, maxLen = 0, i, n, line;
    int pw, ph, px0, py0;

    for (i = 0; i < sMenuN; i++) {
        n = (int)strlen(sMenu[i]);
        maxLen = n > maxLen ? n : maxLen;
    }
    pw = maxLen * cw + 4 * cw;
    ph = sMenuN * ch + 3 * ch;
    px0 = (w - pw) / 2;
    py0 = (h - ph) / 2;
    p_glBindFramebuffer(GL_FRAMEBUFFER, 0);
    p_glViewport(0, 0, w, h);
    p_glDisable(GL_DEPTH_TEST);
    p_glEnable(GL_BLEND);
    p_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    p_glEnable(GL_SCISSOR_TEST);
    p_glScissor(px0, py0, pw, ph);
    p_glUseProgram(sFillProg);
    p_glProgramUniform4f(sFillProg, sFillLoc, 0.0f, 0.0f, 0.0f, 0.8f);
    p_glBindVertexArray(sQuadVao);
    p_glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    p_glDisable(GL_SCISSOR_TEST);
    p_glUseProgram(sTextProg);
    p_glBindVertexArray(sTextVao);
    p_glBindTextureUnit(0, sFontTex);
    for (line = 0; line < sMenuN; line++) {
        const char *t = sMenu[line];
        int x = px0 + 2 * cw, y = py0 + ph - (line + 2) * ch;   /* GL rows from the bottom */

        for (i = n = 0; t[i] != 0 && n < 64 * 64; i++) {
            int c = (unsigned char)t[i];
            float x0, x1, y0, y1, u0, u1;
            float q[6][4];
            int j;

            c = c >= 'a' && c <= 'z' ? c - 32 : c;
            c = c < 32 || c > 96 ? 32 : c;
            x0 = (float)(x + i * cw) / w * 2.0f - 1.0f;
            x1 = (float)(x + i * cw + 6 * k) / w * 2.0f - 1.0f;
            y0 = (float)y / h * 2.0f - 1.0f;
            y1 = (float)(y + 8 * k) / h * 2.0f - 1.0f;
            u0 = (float)((c - 32) * 6) / (65 * 6);
            u1 = (float)((c - 32) * 6 + 6) / (65 * 6);
            {
                float tmp[6][4] = {{x0, y0, u0, 1}, {x1, y0, u1, 1}, {x0, y1, u0, 0},
                                   {x1, y0, u1, 1}, {x1, y1, u1, 0}, {x0, y1, u0, 0}};

                memcpy(q, tmp, sizeof(q));
            }
            for (j = 0; j < 6; j++) {
                memcpy(&v[(n * 6 + j) * 4], q[j], sizeof(q[j]));
            }
            n++;
        }
        if (n == 0) {
            continue;
        }
        p_glNamedBufferData(sTextVbo, (GLsizeiptr)n * 6 * 4 * sizeof(float), v, GL_STREAM_DRAW);
        if (line == sMenuSel) {
            p_glProgramUniform4f(sTextProg, sTextColorLoc, 1.0f, 0.85f, 0.3f, 1.0f);
        } else {
            p_glProgramUniform4f(sTextProg, sTextColorLoc, 0.85f, 0.85f, 0.85f, 1.0f);
        }
        p_glDrawArrays(GL_TRIANGLES, 0, n * 6);
    }
    p_glDisable(GL_BLEND);
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

/* the scene-size targets, made for the render scale (and made again when it changes) */
static void targets_make(void) {
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sColor);
    p_glTextureStorage2D(sColor, 1, GL_RGBA8, GLR_WIDTH, GLR_HEIGHT);
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sDepth);
    p_glTextureStorage2D(sDepth, 1, GL_DEPTH24_STENCIL8, GLR_WIDTH, GLR_HEIGHT);
    p_glCreateFramebuffers(1, &sFbo);
    p_glNamedFramebufferTexture(sFbo, GL_COLOR_ATTACHMENT0, sColor, 0);
    p_glNamedFramebufferTexture(sFbo, GL_DEPTH_STENCIL_ATTACHMENT, sDepth, 0);   /* stencil: layer 0x23's marks */
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sViewDepth);
    p_glTextureStorage2D(sViewDepth, 1, GL_R32F, GLR_WIDTH, GLR_HEIGHT);
    p_glNamedFramebufferTexture(sFbo, GL_COLOR_ATTACHMENT1, sViewDepth, 0);
    {
        static const GLenum kBufs[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

        p_glNamedFramebufferDrawBuffers(sFbo, 2, kBufs);
    }
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sRefl);
    p_glTextureStorage2D(sRefl, 1, GL_RGBA8, GLR_WIDTH, GLR_HEIGHT);
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sReflDepth);
    p_glTextureStorage2D(sReflDepth, 1, GL_DEPTH24_STENCIL8, GLR_WIDTH, GLR_HEIGHT);
    p_glCreateFramebuffers(1, &sReflFbo);
    p_glNamedFramebufferTexture(sReflFbo, GL_COLOR_ATTACHMENT0, sRefl, 0);
    p_glNamedFramebufferTexture(sReflFbo, GL_DEPTH_ATTACHMENT, sReflDepth, 0);
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sMask);
    p_glTextureStorage2D(sMask, 1, GL_RGBA8, GLR_WIDTH, GLR_HEIGHT);
    p_glCreateFramebuffers(1, &sMaskFbo);
    p_glNamedFramebufferTexture(sMaskFbo, GL_COLOR_ATTACHMENT0, sMask, 0);
    p_glNamedFramebufferTexture(sMaskFbo, GL_DEPTH_ATTACHMENT, sDepth, 0);
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sShadowCol);
    p_glTextureStorage2D(sShadowCol, 1, GL_RGBA8, GLR_WIDTH, GLR_HEIGHT);
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sShadowDS);
    p_glTextureStorage2D(sShadowDS, 1, GL_DEPTH24_STENCIL8, GLR_WIDTH, GLR_HEIGHT);
    p_glCreateFramebuffers(1, &sShadowFbo);
    p_glNamedFramebufferTexture(sShadowFbo, GL_COLOR_ATTACHMENT0, sShadowCol, 0);
    p_glNamedFramebufferTexture(sShadowFbo, GL_DEPTH_STENCIL_ATTACHMENT, sShadowDS, 0);
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sCopy);
    p_glTextureStorage2D(sCopy, 1, GL_RGBA8, GLR_WIDTH, GLR_HEIGHT);
    p_glCreateFramebuffers(1, &sCopyFbo);
    p_glNamedFramebufferTexture(sCopyFbo, GL_COLOR_ATTACHMENT0, sCopy, 0);
}

static void targets_free(void) {
    GLuint tex[] = {sColor, sDepth, sViewDepth, sRefl, sReflDepth, sMask, sShadowCol, sShadowDS, sCopy};
    GLuint fbo[] = {sFbo, sReflFbo, sMaskFbo, sShadowFbo, sCopyFbo};

    p_glDeleteTextures((GLsizei)(sizeof(tex) / sizeof(tex[0])), tex);
    p_glDeleteFramebuffers((GLsizei)(sizeof(fbo) / sizeof(fbo[0])), fbo);
}

/* the render scale: the scene drawn at 640 x 448 times `scale` (1 the PS2's; 0 to fit the
 * window), from the next frame */
void glr_set_scale(int scale) {
    sWantScale = scale;
}

int glr_scale(void) {
    return sWantScale;
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
    sFbaLoc = p_glGetUniformLocation(sMeshProg, "uFba");
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


    sPostProg = program(kQuadVs, kPostFs);
    sPostModeLoc = p_glGetUniformLocation(sPostProg, "uMode");
    sPostColorLoc = p_glGetUniformLocation(sPostProg, "uColor");
    sPostFixLoc = p_glGetUniformLocation(sPostProg, "uFix");
    sPostColor2Loc = p_glGetUniformLocation(sPostProg, "uColor2");
    sPostRangeLoc = p_glGetUniformLocation(sPostProg, "uRange");
    sPostBandLoc = p_glGetUniformLocation(sPostProg, "uBand");
    sPostOffLoc = p_glGetUniformLocation(sPostProg, "uOff");
    sPostScaleLoc = p_glGetUniformLocation(sPostProg, "uS");
    p_glProgramUniform1f(sPostProg, sPostScaleLoc, 1.0f);
    p_glProgramUniform1i(sPostProg, p_glGetUniformLocation(sPostProg, "uTex2"), 2);
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
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sReflPrep);
    p_glTextureStorage2D(sReflPrep, 1, GL_RGBA8, GLR_HALF_W, GLR_HALF_H);
    p_glTextureParameteri(sReflPrep, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    p_glTextureParameteri(sReflPrep, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    p_glTextureParameteri(sReflPrep, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    p_glTextureParameteri(sReflPrep, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    p_glCreateFramebuffers(1, &sReflPrepFbo);
    p_glNamedFramebufferTexture(sReflPrepFbo, GL_COLOR_ATTACHMENT0, sReflPrep, 0);
    targets_make();
    font_make();
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sTintHalf);
    p_glTextureStorage2D(sTintHalf, 1, GL_RGBA8, GLR_HALF_W, GLR_HALF_H);
    p_glTextureParameteri(sTintHalf, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    p_glTextureParameteri(sTintHalf, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    p_glTextureParameteri(sTintHalf, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    p_glTextureParameteri(sTintHalf, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    p_glCreateFramebuffers(1, &sTintHalfFbo);
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sSoft);
    p_glTextureStorage2D(sSoft, 1, GL_RGBA8, 256, 256);
    p_glTextureParameteri(sSoft, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    p_glTextureParameteri(sSoft, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    p_glTextureParameteri(sSoft, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    p_glTextureParameteri(sSoft, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    p_glCreateFramebuffers(1, &sSoftFbo);
    p_glNamedFramebufferTexture(sSoftFbo, GL_COLOR_ATTACHMENT0, sSoft, 0);
    p_glNamedFramebufferTexture(sTintHalfFbo, GL_COLOR_ATTACHMENT0, sTintHalf, 0);
    {   /* frame dumps: the scene brought to 640 x 448 */
        p_glCreateTextures(GL_TEXTURE_2D, 1, &sDump);
        p_glTextureStorage2D(sDump, 1, GL_RGBA8, 640, 448);
        p_glCreateFramebuffers(1, &sDumpFbo);
        p_glNamedFramebufferTexture(sDumpFbo, GL_COLOR_ATTACHMENT0, sDump, 0);
    }
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sGlowB);
    p_glTextureStorage2D(sGlowB, 1, GL_RGBA8, GLR_GLOW_W, GLR_GLOW_H);
    p_glTextureParameteri(sGlowB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    p_glTextureParameteri(sGlowB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    p_glTextureParameteri(sGlowB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    p_glTextureParameteri(sGlowB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    p_glCreateTextures(GL_TEXTURE_2D, 1, &sGlowDepth);
    p_glTextureStorage2D(sGlowDepth, 1, GL_DEPTH24_STENCIL8, GLR_GLOW_W, GLR_GLOW_H);
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

static void run_post(const GlrDraw *d);

/* layer 0x23's end, after its fading: on what it drew (marked in the frame's alpha, made a
 * stencil), the room's two-colour effect again (effect 0x1F's colours, their alpha scaled by
 * the tint) and the frame's fog - it was drawn after them */
static void late_end(const GlrFrame *f) {
    static const GLint kZero = 0;
    GlrDraw g;
    int k;

    if (!f->late) {
        return;
    }
    p_glClearNamedFramebufferiv(sFbo, GL_STENCIL, 0, &kZero);
    p_glBlitNamedFramebuffer(sFbo, sCopyFbo, 0, 0, GLR_WIDTH, GLR_HEIGHT, 0, 0, GLR_WIDTH, GLR_HEIGHT,
                             GL_COLOR_BUFFER_BIT, GL_NEAREST);
    p_glBindFramebuffer(GL_FRAMEBUFFER, sFbo);
    p_glViewport(0, 0, GLR_WIDTH, GLR_HEIGHT);
    p_glDisable(GL_DEPTH_TEST);
    p_glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    p_glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    p_glEnable(GL_STENCIL_TEST);
    p_glStencilFunc(GL_ALWAYS, 1, 0xFF);
    p_glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    p_glUseProgram(sPostProg);
    p_glBindVertexArray(sQuadVao);
    p_glProgramUniform1i(sPostProg, sPostModeLoc, 31);
    p_glBindTextureUnit(0, sCopy);
    p_glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    p_glStencilFunc(GL_EQUAL, 1, 0xFF);
    p_glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    memset(&g, 0, sizeof(g));
    for (k = 0; k < 2; k++) {   /* added, then contrasted */
        if ((f->lateColor[k] >> 24) == 0) {
            continue;
        }
        g.post = POST_SCREEN2;
        g.tex0 = f->lateColor[k];
        g.prim = (uint32_t)(k == 1) | (uint32_t)(f->lateBlur != 0) << 1;
        run_post(&g);
    }
    for (k = f->nd - 1; k >= 0; k--) {   /* the frame's fog again */
        if (f->d[k].post == POST_FOG) {
            run_post(&f->d[k]);
            break;
        }
    }
    p_glDisable(GL_STENCIL_TEST);
}

static int cmp_order(const void *a, const void *b) {
    long long x = *(const long long *)a, y = *(const long long *)b;

    return x < y ? -1 : x > y;
}

/* the 3D strips' state */
static void mesh_state(void) {
    p_glBindFramebuffer(GL_FRAMEBUFFER, sFbo);
    p_glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    p_glProgramUniform1i(sMeshProg, sFbaLoc, 0);
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

/* HG_POSTOFF names pass `kind` */
static int post_off(int kind) {
    static const char *const kNames[] = {"", "bloom", "glow", "screen2", "fog", "vignette", "alphaclear", "caustic", "dof", "maskclear", "refl", "shadowbegin", "shadowfill", "image", "haze"};
    const char *off = getenv("HG_POSTOFF");

    return off != NULL && kind < (int)(sizeof(kNames) / sizeof(kNames[0])) && strstr(off, kNames[kind]) != NULL;
}

/* layer 0x14's end: the bright parts of what it drew, blurred and added in colour `col` */
static void shine_end(uint32_t col) {
    p_glDisable(GL_DEPTH_TEST);
    p_glDisable(GL_BLEND);
    p_glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    p_glUseProgram(sPostProg);
    p_glBindVertexArray(sQuadVao);
    post(29, sHalfFbo[0], GLR_HALF_W, GLR_HALF_H, sColor, 0);
    post(30, sHalfFbo[1], GLR_HALF_W, GLR_HALF_H, sHalf[0], 0);
    p_glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
    p_glProgramUniform4f(sPostProg, sPostColorLoc, (float)(col & 0xFF), (float)((col >> 8) & 0xFF),
                         (float)((col >> 16) & 0xFF), 0.0f);
    p_glProgramUniform1f(sPostProg, sPostFixLoc, 64.0f);
    p_glEnable(GL_BLEND);
    p_glBlendFunc(GL_ONE, GL_ONE);
    post(6, sFbo, GLR_WIDTH, GLR_HEIGHT, sHalf[1], 0);
    p_glDisable(GL_BLEND);
}

/* layer 0x1C's start (func_001AB960): the frame's alpha cleared, so its draws mark it */
static void soft_begin(void) {
    static const float kNone[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    p_glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
    p_glClearNamedFramebufferfv(sFbo, GL_COLOR, 0, kNone);
    p_glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

/* its end: what it drew, brought down to 256 x 256 and blended back over the screen by its
 * alpha - softened */
static void soft_end(void) {
    p_glDisable(GL_DEPTH_TEST);
    p_glDisable(GL_BLEND);
    p_glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    p_glUseProgram(sPostProg);
    p_glBindVertexArray(sQuadVao);
    post(27, sSoftFbo, 256, 256, sColor, 0);
    p_glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
    p_glEnable(GL_BLEND);
    p_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    post(28, sFbo, GLR_WIDTH, GLR_HEIGHT, sSoft, 0);
    p_glDisable(GL_BLEND);
}

/* a fading layer's start: the frame's alpha cleared, the screen halved for its background */
static void tint_begin(void) {
    static const float kNone[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    p_glDisable(GL_DEPTH_TEST);
    p_glDisable(GL_BLEND);
    p_glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    p_glUseProgram(sPostProg);
    p_glBindVertexArray(sQuadVao);
    p_glProgramUniform1f(sPostProg, sPostFixLoc, 0.0f);
    post(2, sTintHalfFbo, GLR_HALF_W, GLR_HALF_H, sColor, 0);
    p_glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
    p_glClearNamedFramebufferfv(sFbo, GL_COLOR, 0, kNone);
}

/* a fading layer's end (tint `t`): its marked pixels faded or tinted */
static void tint_end(uint32_t t) {
    p_glDisable(GL_DEPTH_TEST);
    p_glDisable(GL_BLEND);
    p_glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    p_glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
    p_glBlitNamedFramebuffer(sFbo, sCopyFbo, 0, 0, GLR_WIDTH, GLR_HEIGHT, 0, 0, GLR_WIDTH, GLR_HEIGHT,
                             GL_COLOR_BUFFER_BIT, GL_NEAREST);
    p_glUseProgram(sPostProg);
    p_glBindVertexArray(sQuadVao);
    p_glProgramUniform4f(sPostProg, sPostColorLoc, (float)(t & 0xFF), (float)((t >> 8) & 0xFF),
                         (float)((t >> 16) & 0xFF), (float)(t >> 24));
    p_glProgramUniform2i(sPostProg, sPostOffLoc, t >= 0x81000000u, 0);
    post(25, sFbo, GLR_WIDTH, GLR_HEIGHT, sCopy, sTintHalf);
}

/* layer 6's end (func_001B4F30's second packet): its buffer blurred and taken off the screen
 * at half (ALPHA (0 - Cs) * FIX + Cd, FIX 0x40) */
static void shadow_end(void) {
    if (post_off(POST_SHADOW_FILL)) {
        return;
    }
    p_glDisable(GL_DEPTH_TEST);
    p_glDisable(GL_BLEND);
    p_glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    p_glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
    p_glUseProgram(sPostProg);
    p_glBindVertexArray(sQuadVao);
    post(24, sHalfFbo[0], GLR_HALF_W, GLR_HALF_H, sShadowCol, 0);
    p_glProgramUniform4f(sPostProg, sPostColorLoc, 128.0f, 128.0f, 128.0f, 0.0f);
    p_glProgramUniform1f(sPostProg, sPostFixLoc, 64.0f);
    p_glEnable(GL_BLEND);
    p_glBlendFunc(GL_ONE, GL_ONE);
    p_glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
    post(6, sFbo, GLR_WIDTH, GLR_HEIGHT, sHalf[0], 0);
    p_glBlendEquation(GL_FUNC_ADD);
    p_glDisable(GL_BLEND);
}

/* a pass over the frame (the shader's modes, kPostFs) */
static void run_post(const GlrDraw *d) {
    uint32_t rgba = (uint32_t)d->tex0;

    if (post_off(d->post)) {
        return;
    }

    p_glDisable(GL_DEPTH_TEST);
    p_glDisable(GL_BLEND);
    p_glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    p_glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, d->post == POST_FOG);
    p_glUseProgram(sPostProg);
    p_glBindVertexArray(sQuadVao);
    switch (d->post) {
    case POST_ALPHA_CLEAR: {   /* the frame's alpha to 0 (a sprite writing only alpha) */
        static const float kNone[4] = {0.0f, 0.0f, 0.0f, 0.0f};

        p_glBindFramebuffer(GL_FRAMEBUFFER, sFbo);
        p_glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
        p_glClearNamedFramebufferfv(sFbo, GL_COLOR, 0, kNone);
        break;
    }
    case POST_CAUSTIC:   /* func_0034E9E0: the halved screen where alpha >= args, blurred, added at 1/2 */
        p_glProgramUniform1f(sPostProg, sPostFixLoc, 0.0f);
        post(2, sHalfFbo[0], GLR_HALF_W, GLR_HALF_H, sColor, 0);
        p_glProgramUniform1f(sPostProg, sPostFixLoc, (float)d->prim);
        post(12, sHalfFbo[1], GLR_HALF_W, GLR_HALF_H, sHalf[0], 0);
        post(13, sHalfFbo[2], GLR_HALF_W, GLR_HALF_H, sHalf[1], 0);
        p_glProgramUniform4f(sPostProg, sPostColorLoc, 128.0f, 128.0f, 128.0f, 0.0f);
        p_glProgramUniform1f(sPostProg, sPostFixLoc, 64.0f);
        p_glEnable(GL_BLEND);
        p_glBlendFunc(GL_ONE, GL_ONE);
        post(6, sFbo, GLR_WIDTH, GLR_HEIGHT, sHalf[2], 0);
        break;
    case POST_DOF: {   /* func_002C6650: the halved screen blurred, over it by the depth band's alpha */
        /* (+-1, +-1), then (-+1, 0), (0, -+1): the game's pixel offsets, y down */
        static const int kOff[8][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        int x = 0, y = 1, z = 2, k, t;   /* x: the "0x1B8" image, y: "0x1D4", z spare */

        p_glProgramUniform1f(sPostProg, sPostFixLoc, 0.0f);
        post(2, sHalfFbo[x], GLR_HALF_W, GLR_HALF_H, sColor, 0);
        p_glBlitNamedFramebuffer(sHalfFbo[x], sHalfFbo[y], 0, 0, GLR_HALF_W, GLR_HALF_H, 0, 0, GLR_HALF_W,
                                 GLR_HALF_H, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        for (k = 0; k < 8; k++) {   /* into x from y, then into y from x, ... */
            int dst = k & 1 ? y : x, src = k & 1 ? x : y;

            p_glProgramUniform2i(sPostProg, sPostOffLoc, kOff[k][0], -kOff[k][1]);
            post(15, sHalfFbo[z], GLR_HALF_W, GLR_HALF_H, sHalf[dst], sHalf[src]);
            t = z;
            z = dst;
            if (k & 1) {
                y = t;
            } else {
                x = t;
            }
        }
        p_glProgramUniform4f(sPostProg, sPostBandLoc, d->mvp[0], d->mvp[1], d->mvp[2], d->mvp[3]);
        p_glBindTextureUnit(2, sViewDepth);
        post(16, sHalfFbo[z], GLR_HALF_W, GLR_HALF_H, sHalf[y], 0);
        p_glEnable(GL_BLEND);
        p_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        post(17, sFbo, GLR_WIDTH, GLR_HEIGHT, sHalf[z], 0);
        break;
    }
    case POST_IMAGE:   /* an uploaded image (glr_vram_draw) over the screen, by its alpha */
        if (sImgDirty) {
            if (sImgTex != 0) {
                p_glDeleteTextures(1, &sImgTex);
            }
            p_glCreateTextures(GL_TEXTURE_2D, 1, &sImgTex);
            p_glTextureStorage2D(sImgTex, 1, GL_RGBA8, sImgW, sImgH);
            p_glTextureParameteri(sImgTex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            p_glTextureParameteri(sImgTex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            p_glTextureParameteri(sImgTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            p_glTextureParameteri(sImgTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            p_glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            p_glTextureSubImage2D(sImgTex, 0, 0, 0, sImgW, sImgH, GL_RGBA, GL_UNSIGNED_BYTE, sImgPixels);
            sImgDirty = 0;
        }
        if (sImgTex != 0) {
            if (!(d->prim & 1)) {   /* shown by its alpha; else written as it is */
                p_glEnable(GL_BLEND);
                p_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            post(26, sFbo, GLR_WIDTH, GLR_HEIGHT, sImgTex, 0);
        }
        break;
    case POST_SHADOW_BEGIN: {   /* the count back to 0x7F */
        static const GLint kStart = 0x7F;

        p_glClearNamedFramebufferiv(sShadowFbo, GL_STENCIL, 0, &kStart);
        break;
    }
    case POST_SHADOW_FILL: {   /* the colour where the count is above 0x7F, within the box */
        int x0 = (int)((d->mvp[0] - 1728.0f) * sScale), x1 = (int)((d->mvp[2] - 1728.0f) * sScale);
        int y0 = (int)((2272.0f - d->mvp[3]) * sScale), y1 = (int)((2272.0f - d->mvp[1]) * sScale);

        p_glBindFramebuffer(GL_FRAMEBUFFER, sShadowFbo);
        p_glViewport(0, 0, GLR_WIDTH, GLR_HEIGHT);
        p_glEnable(GL_SCISSOR_TEST);
        p_glScissor(x0, y0, x1 - x0 + sScale, y1 - y0 + sScale);
        p_glEnable(GL_STENCIL_TEST);
        p_glStencilFunc(GL_LESS, 0x7F, 0xFF);
        p_glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        p_glEnable(GL_BLEND);
        p_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        p_glProgramUniform4f(sPostProg, sPostColorLoc, (float)(rgba & 0xFF), (float)((rgba >> 8) & 0xFF),
                             (float)((rgba >> 16) & 0xFF), (float)(rgba >> 24));
        p_glProgramUniform1i(sPostProg, sPostModeLoc, 23);
        p_glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        p_glDisable(GL_STENCIL_TEST);
        p_glDisable(GL_SCISSOR_TEST);
        break;
    }
    case POST_MASK_CLEAR: {
        static const float kNone[4] = {0.0f, 0.0f, 0.0f, 0.0f};

        p_glClearNamedFramebufferfv(sMaskFbo, GL_COLOR, 0, kNone);
        break;
    }
    case POST_REFL: {   /* func_00317D40 / func_00316DE0 */
        float fix = (float)(int32_t)rgba / 128.0f;

        if (d->prim & 1) {
            post(18, sReflPrepFbo, GLR_HALF_W, GLR_HALF_H, sRefl, 0);
        }
        p_glProgramUniform2i(sPostProg, sPostOffLoc, (d->prim >> 1) & 1, (d->prim >> 2) & 1);
        p_glProgramUniform2f(sPostProg, sPostRangeLoc, d->mvp[0], 0.0f);
        p_glEnable(GL_BLEND);
        p_glBlendColor(fix, fix, fix, fix);
        p_glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR);
        post(21, sFbo, GLR_WIDTH, GLR_HEIGHT, sReflPrep, sMask);
        break;
    }
    case POST_HAZE:   /* func_00374E50: the screen halved, blended half with itself in 33 wavering
                       * columns, then that over the screen at 0x48 / 128 */
        p_glBlitNamedFramebuffer(sFbo, sCopyFbo, 0, 0, GLR_WIDTH, GLR_HEIGHT, 0, 0, GLR_WIDTH, GLR_HEIGHT,
                                 GL_COLOR_BUFFER_BIT, GL_NEAREST);
        p_glProgramUniform1f(sPostProg, sPostFixLoc, 72.0f);
        p_glProgramUniform2f(sPostProg, sPostRangeLoc, d->mvp[0], d->mvp[1]);
        p_glProgramUniform4f(sPostProg, sPostBandLoc, d->mvp[2], d->mvp[3], 0.0f, 0.0f);
        post(32, sFbo, GLR_WIDTH, GLR_HEIGHT, sCopy, 0);
        break;
    case POST_MARKER:   /* func_00301E70 (see glr_marker) */
        p_glBlitNamedFramebuffer(sFbo, sCopyFbo, 0, 0, GLR_WIDTH, GLR_HEIGHT, 0, 0, GLR_WIDTH, GLR_HEIGHT,
                                 GL_COLOR_BUFFER_BIT, GL_NEAREST);
        p_glProgramUniform1f(sPostProg, sPostFixLoc, (float)(int32_t)rgba);
        p_glProgramUniform2f(sPostProg, sPostRangeLoc, d->mvp[4], 0.0f);
        p_glProgramUniform4f(sPostProg, sPostBandLoc, d->mvp[0], d->mvp[1], d->mvp[2], d->mvp[3]);
        post(36, sFbo, GLR_WIDTH, GLR_HEIGHT, sCopy, sViewDepth);
        break;
    case POST_NEGATIVE:   /* func_0021E1B0: the screen's negative, 0x80 - each channel */
        p_glBlitNamedFramebuffer(sFbo, sCopyFbo, 0, 0, GLR_WIDTH, GLR_HEIGHT, 0, 0, GLR_WIDTH, GLR_HEIGHT,
                                 GL_COLOR_BUFFER_BIT, GL_NEAREST);
        post(33, sFbo, GLR_WIDTH, GLR_HEIGHT, sCopy, 0);
        break;
    case POST_ZOOM_BLUR:   /* func_0021D290: the screen copied, drawn 16 pixels bigger each way
                            * over itself at half */
        p_glBlitNamedFramebuffer(sFbo, sCopyFbo, 0, 0, GLR_WIDTH, GLR_HEIGHT, 0, 0, GLR_WIDTH, GLR_HEIGHT,
                                 GL_COLOR_BUFFER_BIT, GL_NEAREST);
        post(34, sFbo, GLR_WIDTH, GLR_HEIGHT, sCopy, 0);
        break;
    case POST_PANIC:   /* func_0021D8F0: the screen's index (green's top bits in the alpha byte)
                        * through palette 5 (below `limit` black, else grey 0xC0), over the
                        * screen at amount / 2 */
        p_glBlitNamedFramebuffer(sFbo, sCopyFbo, 0, 0, GLR_WIDTH, GLR_HEIGHT, 0, 0, GLR_WIDTH, GLR_HEIGHT,
                                 GL_COLOR_BUFFER_BIT, GL_NEAREST);
        p_glProgramUniform1f(sPostProg, sPostFixLoc, (float)(int32_t)rgba);
        p_glProgramUniform2f(sPostProg, sPostRangeLoc, (float)((int32_t)d->prim >> 1), 0.0f);
        post(35, sFbo, GLR_WIDTH, GLR_HEIGHT, sCopy, 0);
        break;
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
        p_glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);   /* alpha: the fog's */
        post(10, sFbo, GLR_WIDTH, GLR_HEIGHT, sViewDepth, 0);
        break;
    }
    case POST_BLOOM:   /* func_002699D0: H blurred, into the glow buffer and over the screen */
        p_glProgramUniform1f(sPostProg, sPostFixLoc, -1.0f);   /* mode 2 with the alpha test */
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
        p_glProgramUniform1f(sPostProg, sPostFixLoc, 0.0f);   /* mode 2 without the alpha test */
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
    static const float kBlack[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    static const float kFar = 1.0f;
    static const float kFarView[4] = {1e30f, 0.0f, 0.0f, 0.0f};   /* nothing drawn: beyond the fog */
    const GlrFrame *f = &sFrames[sBuilding ^ 1];
    int i, depthDirty = 1;   /* the scene's depth changed since the glow buffer's copy */
    int bloomMask = 0, maskCleared = 0;   /* layer 0x26 used this frame; its alpha cleared */
    int reflStarted = 0;                  /* layer 0x17's reflection begun this frame */
    int inShadow = 0;                     /* within layer 6 or 0x0D (the layer, else 0) */
    int tintLayer = -1;                   /* within this fading layer */
    int inSoft = 0;                       /* within layer 0x1C */
    int inShine = 0;                      /* within layer 0x14 */

    {   /* the render scale: as set, or (0) the window's height in 448s, at most 4 */
        int want = sWantScale > 0 ? sWantScale : outH > 0 ? (outH + 447) / 448 : 1;

        want = want < 1 ? 1 : want > 4 ? 4 : want;
        if (want != sScale) {
            targets_free();
            sScale = want;
            targets_make();
            p_glProgramUniform1f(sPostProg, sPostScaleLoc, (float)sScale);
        }
    }
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
                        fprintf(stderr, "glr: pass %d in layer %d, colour %08X, args %u (%g %g %g %g)\n", f->d[k].post,
                                f->d[k].layer, (uint32_t)f->d[k].tex0, f->d[k].prim, f->d[k].mvp[0], f->d[k].mvp[1],
                                f->d[k].mvp[2], f->d[k].mvp[3]);
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
        for (i = 0; i < f->nd; i++) {
            bloomMask |= f->d[i].layer == 0x26 && !f->d[i].post;
        }
        p_glNamedBufferData(sVbo, (GLsizeiptr)f->nv * sizeof(GlrVertex), f->v, GL_STREAM_DRAW);
        mesh_state();
        for (i = 0; i < f->nd; i++) {
            const GlrDraw *d = &f->d[order[i] & 0xFFFFFFFF];
            int mode = 0;

            if (inShine && d->layer != 0x14) {
                shine_end(f->shineColor);
                mesh_state();
                inShine = 0;
            }
            if (!inShine && d->layer == 0x14 && f->shine) {
                soft_begin();   /* (the same start: the frame's alpha cleared) */
                inShine = 1;
            }
            if (inSoft && d->layer != 0x1C) {
                soft_end();
                mesh_state();
                inSoft = 0;
            }
            if (!inSoft && d->layer == 0x1C && f->soft) {
                soft_begin();
                inSoft = 1;
            }
            if (tintLayer >= 0 && d->layer != tintLayer) {   /* a fading layer's end */
                tint_end(f->tint[tint_index(tintLayer)]);
                if (tintLayer == 0x23) {
                    late_end(f);
                }
                mesh_state();
                tintLayer = -1;
            }
            if (tintLayer < 0 && tint_index(d->layer) >= 0 && f->tinted[tint_index(d->layer)]) {
                tint_begin();
                mesh_state();
                tintLayer = d->layer;
            }
            if (inShadow && d->layer != inShadow) {
                shadow_end();
                mesh_state();
                inShadow = 0;
            }
            if ((d->layer == 6 || d->layer == 0x0D) && !inShadow) {   /* their start (func_001B4F30 / _4330) */
                static const float kNone[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                static const float kFar1 = 1.0f;

                p_glClearNamedFramebufferfv(sShadowFbo, GL_COLOR, 0, kNone);
                p_glClearNamedFramebufferfv(sShadowFbo, GL_DEPTH, 0, &kFar1);
                p_glBlitNamedFramebuffer(sFbo, sShadowFbo, 0, 0, GLR_WIDTH, GLR_HEIGHT, 160 * sScale, 112 * sScale,
                                         480 * sScale, 336 * sScale,
                                         GL_DEPTH_BUFFER_BIT, GL_NEAREST);
                inShadow = d->layer;
            }
            if (d->post) {
                run_post(d);
                mesh_state();
                continue;
            }
            if (d->prim & GLR_PRIM_STENCIL) {   /* a shadow volume quad: counted where in front */
                p_glBindFramebuffer(GL_FRAMEBUFFER, sShadowFbo);
                p_glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
                p_glDepthMask(GL_FALSE);
                p_glDepthFunc(GL_LEQUAL);
                p_glDisable(GL_BLEND);
                p_glEnable(GL_STENCIL_TEST);
                p_glStencilFunc(GL_ALWAYS, 0, 0xFF);
                p_glStencilOp(GL_KEEP, GL_KEEP, d->prim & GLR_PRIM_STENCIL_INC ? GL_INCR : GL_DECR);
                p_glProgramUniform1i(sMeshProg, sTexModeLoc, 0);
                p_glProgramUniformMatrix4fv(sMeshProg, sMvpLoc, 1, GL_FALSE, d->mvp);
                p_glDrawArrays(GL_TRIANGLES, d->first, d->n);
                p_glDisable(GL_STENCIL_TEST);
                p_glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                p_glDepthFunc(GL_LESS);
                p_glBindFramebuffer(GL_FRAMEBUFFER, sFbo);
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
                /* the frame keeps the source alpha, as the GS writes it */
                if (d->prim & GLR_PRIM_FIXB) {   /* Cs * FIX / 128 + Cd (SUB: Cd - Cs * FIX / 128) */
                    float fix = (float)(d->prim >> 24) / 128.0f;

                    p_glBlendEquation(d->prim & GLR_PRIM_SUB ? GL_FUNC_REVERSE_SUBTRACT : GL_FUNC_ADD);

                    p_glBlendColor(fix, fix, fix, fix);
                    p_glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_ONE, GL_ONE, GL_ZERO);
                } else {
                    p_glBlendFuncSeparate(GL_SRC_ALPHA, d->prim & GLR_PRIM_ADD ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA,
                                          GL_ONE, GL_ZERO);
                }
            } else {
                p_glDisable(GL_BLEND);
            }
            {
                /* layers 0x25 / 0x26 once layer 0x26 is used: the bloom's mask - only the frame's
                 * alpha is written, with bit 7 set (func_001B1E50 clears it first) */
                int mask = bloomMask && (d->layer == 0x25 || d->layer == 0x26);

                if (mask && !maskCleared) {
                    static const float kNone[4] = {0.0f, 0.0f, 0.0f, 0.0f};

                    p_glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
                    p_glClearNamedFramebufferfv(sFbo, GL_COLOR, 0, kNone);
                    maskCleared = 1;
                }
                p_glColorMaski(0, !mask, !mask, !mask, GL_TRUE);
                p_glProgramUniform1i(sMeshProg, sFbaLoc, mask || d->layer == tintLayer);
            }
            if (d->prim & GLR_PRIM_NOZT) {
                p_glDisable(GL_DEPTH_TEST);
            } else {
                p_glEnable(GL_DEPTH_TEST);
            }
            p_glDepthMask(d->prim & GLR_PRIM_NOZW ? GL_FALSE : GL_TRUE);
            p_glColorMaski(1, d->prim & GLR_PRIM_NOZW ? GL_FALSE : GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
            p_glProgramUniformMatrix4fv(sMeshProg, sMvpLoc, 1, GL_FALSE, d->mvp);
            if (d->layer == 6 || d->layer == 0x0D) {   /* their other draws: into the buffer, tested, no depth */
                p_glBindFramebuffer(GL_FRAMEBUFFER, sShadowFbo);
                p_glDepthMask(GL_FALSE);
                p_glDrawArrays(GL_TRIANGLES, d->first, d->n);
                p_glBindFramebuffer(GL_FRAMEBUFFER, sFbo);
                continue;
            }
            if (d->layer == 0x17 || (d->prim & GLR_PRIM_MASK)) {
                if (d->layer == 0x17 && !reflStarted) {
                    /* layer 0x17's start (func_001B0D40): the reflection cleared, the screen
                     * so far halved and mirrored into it as its background */
                    static const float kNone[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                    static const float kFar1 = 1.0f;

                    p_glClearNamedFramebufferfv(sReflFbo, GL_COLOR, 0, kNone);
                    p_glClearNamedFramebufferfv(sReflFbo, GL_DEPTH, 0, &kFar1);
                    p_glBlitNamedFramebuffer(sFbo, sCopyFbo, 0, 0, GLR_WIDTH, GLR_HEIGHT, 0, 0, GLR_WIDTH, GLR_HEIGHT,
                                             GL_COLOR_BUFFER_BIT, GL_NEAREST);
                    p_glDisable(GL_DEPTH_TEST);
                    p_glDisable(GL_BLEND);
                    p_glUseProgram(sPostProg);
                    p_glBindVertexArray(sQuadVao);
                    p_glBindFramebuffer(GL_FRAMEBUFFER, sReflFbo);
                    p_glViewport(160 * sScale, 112 * sScale, 320 * sScale, 224 * sScale);
                    p_glProgramUniform1i(sPostProg, sPostModeLoc, 22);
                    p_glProgramUniform2i(sPostProg, sPostOffLoc, sReflFlip, 0);
                    p_glBindTextureUnit(0, sCopy);
                    p_glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                    mesh_state();
                    if (mode != 0) {
                        GLuint t = texture_for((const TexEntry *)d->tex, (int)(d->tex0 >> 56) & 0x1F);

                        p_glBindTextureUnit(0, t);
                    }
                    if (d->prim & 0x40) {
                        p_glEnable(GL_BLEND);
                        p_glDisablei(GL_BLEND, 1);
                    }
                    reflStarted = 1;
                }
                if (d->prim & GLR_PRIM_MASK) {   /* a quad marking the mask where it's in front */
                    p_glBindFramebuffer(GL_FRAMEBUFFER, sMaskFbo);
                    p_glDepthFunc(GL_LEQUAL);
                    p_glDepthMask(GL_FALSE);
                    p_glEnable(GL_POLYGON_OFFSET_FILL);
                    p_glPolygonOffset(-1.0f, -4.0f);
                    p_glProgramUniform1i(sMeshProg, sTexModeLoc, 0);
                    p_glDrawArrays(GL_TRIANGLES, d->first, d->n);
                    p_glDisable(GL_POLYGON_OFFSET_FILL);
                    p_glDepthFunc(GL_LESS);
                } else {
                    p_glBindFramebuffer(GL_FRAMEBUFFER, sReflFbo);
                    p_glDrawArrays(GL_TRIANGLES, d->first, d->n);
                }
                p_glBindFramebuffer(GL_FRAMEBUFFER, sFbo);
                continue;
            }
            p_glDrawArrays(GL_TRIANGLES, d->first, d->n);
            if (d->prim & GLR_PRIM_SUB) {
                p_glBlendEquation(GL_FUNC_ADD);
            }
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
        if (inShadow) {
            shadow_end();
        }
        if (tintLayer >= 0) {
            tint_end(f->tint[tint_index(tintLayer)]);
            if (tintLayer == 0x23) {
                late_end(f);
            }
        }
        if (inSoft) {
            soft_end();
        }
        if (inShine) {
            shine_end(f->shineColor);
        }
        p_glDisable(GL_BLEND);
        p_glDepthMask(GL_TRUE);
        p_glDisable(GL_DEPTH_TEST);
        p_glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
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
        if (sMenu != NULL) {
            menu_draw(outW, outH);
        }
    }
    sMenu = NULL;   /* the options menu gives itself again each frame it is open */
}

void glr_read_pixels(uint32_t *out, int w, int h) {
    int y;

    (void)w;
    /* the scene at 640 x 448 whatever the render scale */
    p_glBlitNamedFramebuffer(sFbo, sDumpFbo, 0, 0, GLR_WIDTH, GLR_HEIGHT, 0, 0, 640, 448, GL_COLOR_BUFFER_BIT,
                             GL_LINEAR);
    p_glBindFramebuffer(GL_FRAMEBUFFER, sDumpFbo);
    p_glPixelStorei(GL_PACK_ALIGNMENT, 4);
    p_glReadPixels(0, 0, 640, 448, GL_RGBA, GL_UNSIGNED_BYTE, out);
    /* GL rows run bottom-up */
    for (y = 0; y < h / 2 && y < 448 / 2; y++) {
        uint32_t tmp[640];

        memcpy(tmp, out + y * 640, sizeof(tmp));
        memcpy(out + y * 640, out + (447 - y) * 640, sizeof(tmp));
        memcpy(out + (447 - y) * 640, tmp, sizeof(tmp));
    }
}
