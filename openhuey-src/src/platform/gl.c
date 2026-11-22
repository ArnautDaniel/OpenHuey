#include "gl.h"

#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GL_DEFINE(type, name) type p_##name;
GL_CORE_FUNCTIONS(GL_DEFINE)
GL_DSA_FUNCTIONS(GL_DEFINE)
#undef GL_DEFINE

static int sLow;           /* the 3.3 path */
static int sAnisotropy;    /* anisotropic filtering there */
static int sMaxSamples = 1;

/* ---- the 3.3 path's own: the functions the fallbacks are made of (all in 3.3 core) ---- */

typedef void (APIENTRY *HGA_GLBINDTEXTURE)(GLenum, GLuint);
typedef void (APIENTRY *HGA_GLGENTEXTURES)(GLsizei, GLuint *);
typedef void (APIENTRY *HGA_GLTEXPARAMETERI)(GLenum, GLenum, GLint);
typedef void (APIENTRY *HGA_GLTEXIMAGE2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void *);
typedef void (APIENTRY *HGA_GLTEXSUBIMAGE2D)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void *);
typedef void (APIENTRY *HGA_GLCOPYTEXSUBIMAGE2D)(GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
typedef void (APIENTRY *HGA_GLREADPIXELS)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, void *);
typedef void (APIENTRY *HGA_GLDRAWBUFFER)(GLenum);
typedef void (APIENTRY *HGA_GLREADBUFFER)(GLenum);

#define GL_CLASSIC_FUNCTIONS(X) \
    X(HGA_GLBINDTEXTURE, glBindTexture)                                        \
    X(HGA_GLGENTEXTURES, glGenTextures)                                        \
    X(HGA_GLTEXPARAMETERI, glTexParameteri)                                    \
    X(HGA_GLTEXIMAGE2D, glTexImage2D)                                          \
    X(HGA_GLTEXSUBIMAGE2D, glTexSubImage2D)                                    \
    X(HGA_GLCOPYTEXSUBIMAGE2D, glCopyTexSubImage2D)                            \
    X(HGA_GLREADPIXELS, glReadPixels)                                          \
    X(HGA_GLDRAWBUFFER, glDrawBuffer)                                          \
    X(HGA_GLREADBUFFER, glReadBuffer)                                          \
    X(PFNGLACTIVETEXTUREPROC, glActiveTexture)                                 \
    X(PFNGLTEXIMAGE3DPROC, glTexImage3D)                                       \
    X(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap)                               \
    X(PFNGLGENSAMPLERSPROC, glGenSamplers)                                     \
    X(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers)                             \
    X(PFNGLFRAMEBUFFERTEXTUREPROC, glFramebufferTexture)                       \
    X(PFNGLFRAMEBUFFERTEXTURELAYERPROC, glFramebufferTextureLayer)             \
    X(PFNGLFRAMEBUFFERRENDERBUFFERPROC, glFramebufferRenderbuffer)             \
    X(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus)               \
    X(PFNGLBLITFRAMEBUFFERPROC, glBlitFramebuffer)                             \
    X(PFNGLDRAWBUFFERSPROC, glDrawBuffers)                                     \
    X(PFNGLCLEARBUFFERFVPROC, glClearBufferfv)                                 \
    X(PFNGLCLEARBUFFERFIPROC, glClearBufferfi)                                 \
    X(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers)                           \
    X(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer)                           \
    X(PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC, glRenderbufferStorageMultisample) \
    X(PFNGLGENBUFFERSPROC, glGenBuffers)                                       \
    X(PFNGLBINDBUFFERPROC, glBindBuffer)                                       \
    X(PFNGLBUFFERDATAPROC, glBufferData)                                       \
    X(PFNGLBUFFERSUBDATAPROC, glBufferSubData)                                 \
    X(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays)                             \
    X(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer)                     \
    X(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray)             \
    X(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation)                       \
    X(PFNGLUNIFORM1IPROC, glUniform1i)                                         \
    X(PFNGLUNIFORM1FPROC, glUniform1f)                                         \
    X(PFNGLUNIFORM2FPROC, glUniform2f)                                         \
    X(PFNGLUNIFORM3FPROC, glUniform3f)                                         \
    X(PFNGLUNIFORM4FPROC, glUniform4f)                                         \
    X(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv)                           \
    X(PFNGLUNIFORM1FVPROC, glUniform1fv)                                       \
    X(PFNGLUNIFORM3FVPROC, glUniform3fv)                                       \
    X(PFNGLUNIFORM4FVPROC, glUniform4fv)

#define GL_CLASSIC(type, name) static type c_##name;
GL_CLASSIC_FUNCTIONS(GL_CLASSIC)
#undef GL_CLASSIC

/* the driver's own, behind the fallbacks' wrappers that keep track of the bindings */
static PFNGLUSEPROGRAMPROC sUseProgram;
static PFNGLBINDVERTEXARRAYPROC sBindVertexArray;
static PFNGLBINDFRAMEBUFFERPROC sBindFramebuffer;
static GLuint sProgram, sVertexArray, sDrawFbo, sReadFbo;   /* what is bound */

/* a texture unit no shader samples: the fallbacks bind there to change a texture */
#define SCRATCH_UNIT 40

/* what the fallbacks know of each object, by its name (GL's names are small numbers) */
typedef struct VaoAttr {
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLuint offset, binding;
    int set;
} VaoAttr;

#define VAO_ATTRS 8
#define VAO_BINDINGS 2

typedef struct VaoInfo {
    VaoAttr attr[VAO_ATTRS];
    GLuint buffer[VAO_BINDINGS];
    GLintptr offset[VAO_BINDINGS];
    GLsizei stride[VAO_BINDINGS];
} VaoInfo;

#define PROGRAM_LOCATIONS 256   /* the explicit locations the shaders use: below this */

typedef struct ProgramInfo {
    GLint location[PROGRAM_LOCATIONS];   /* the explicit one -> the program's (-1: none) */
} ProgramInfo;

static GLenum *sTextureTarget;
static GLuint sTextureN;
static VaoInfo **sVao;
static GLuint sVaoN;
static ProgramInfo **sProgramInfo;
static GLuint sProgramN;

/* a table by name grown to hold name (zeroed) */
static void *grow(void *table, GLuint *n, GLuint name, size_t size) {
    GLuint m = *n;

    if (name < m) {
        return table;
    }
    while (m <= name) {
        m = m < 64 ? 64 : m * 2;
    }
    table = realloc(table, (size_t)m * size);
    if (table == NULL) {
        fprintf(stderr, "gl: out of memory\n");
        abort();
    }
    memset((char *)table + (size_t)*n * size, 0, (size_t)(m - *n) * size);
    *n = m;
    return table;
}

static GLenum texture_target(GLuint t) {
    return t < sTextureN && sTextureTarget[t] != 0 ? sTextureTarget[t] : GL_TEXTURE_2D;
}

static VaoInfo *vao_info(GLuint vao) {
    sVao = grow(sVao, &sVaoN, vao, sizeof(*sVao));
    if (sVao[vao] == NULL) {
        sVao[vao] = calloc(1, sizeof(VaoInfo));
    }
    return sVao[vao];
}

static void APIENTRY use_program(GLuint p) {
    sProgram = p;
    sUseProgram(p);
}

static void APIENTRY bind_vertex_array(GLuint vao) {
    sVertexArray = vao;
    sBindVertexArray(vao);
}

static void APIENTRY bind_framebuffer(GLenum target, GLuint fbo) {
    if (target == GL_FRAMEBUFFER || target == GL_DRAW_FRAMEBUFFER) {
        sDrawFbo = fbo;
    }
    if (target == GL_FRAMEBUFFER || target == GL_READ_FRAMEBUFFER) {
        sReadFbo = fbo;
    }
    sBindFramebuffer(target, fbo);
}

/* a framebuffer bound to change it (draw), then the bindings put back */
static void fbo_edit(GLuint fbo) {
    sBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}
static void fbo_done(void) {
    sBindFramebuffer(GL_DRAW_FRAMEBUFFER, sDrawFbo);
    sBindFramebuffer(GL_READ_FRAMEBUFFER, sReadFbo);
}

static void texture_edit(GLuint t) {
    c_glActiveTexture(GL_TEXTURE0 + SCRATCH_UNIT);
    c_glBindTexture(texture_target(t), t);
}

/* ---- programs: the uniforms set through the program in use ---- */

static GLint location(GLuint p, GLint loc) {
    if (p < sProgramN && sProgramInfo[p] != NULL && loc >= 0 && loc < PROGRAM_LOCATIONS) {
        return sProgramInfo[p]->location[loc];
    }
    return -1;
}

#define PROGRAM_UNIFORM(name, call, params, args)                                         \
    static void APIENTRY name params {                                                    \
        GLint l = location(program, loc);                                                 \
        if (l < 0) {                                                                      \
            return;                                                                       \
        }                                                                                 \
        if (program != sProgram) {                                                        \
            sUseProgram(program);                                                         \
        }                                                                                 \
        call args;                                                                        \
        if (program != sProgram) {                                                        \
            sUseProgram(sProgram);                                                        \
        }                                                                                 \
    }
PROGRAM_UNIFORM(f_ProgramUniform1i, c_glUniform1i, (GLuint program, GLint loc, GLint a), (l, a))
PROGRAM_UNIFORM(f_ProgramUniform1f, c_glUniform1f, (GLuint program, GLint loc, GLfloat a), (l, a))
PROGRAM_UNIFORM(f_ProgramUniform2f, c_glUniform2f, (GLuint program, GLint loc, GLfloat a, GLfloat b), (l, a, b))
PROGRAM_UNIFORM(f_ProgramUniform3f, c_glUniform3f, (GLuint program, GLint loc, GLfloat a, GLfloat b, GLfloat c), (l, a, b, c))
PROGRAM_UNIFORM(f_ProgramUniform4f, c_glUniform4f, (GLuint program, GLint loc, GLfloat a, GLfloat b, GLfloat c, GLfloat d),
                (l, a, b, c, d))
PROGRAM_UNIFORM(f_ProgramUniformMatrix4fv, c_glUniformMatrix4fv,
                (GLuint program, GLint loc, GLsizei n, GLboolean transpose, const GLfloat *v), (l, n, transpose, v))
PROGRAM_UNIFORM(f_ProgramUniform1fv, c_glUniform1fv, (GLuint program, GLint loc, GLsizei n, const GLfloat *v), (l, n, v))
PROGRAM_UNIFORM(f_ProgramUniform3fv, c_glUniform3fv, (GLuint program, GLint loc, GLsizei n, const GLfloat *v), (l, n, v))
PROGRAM_UNIFORM(f_ProgramUniform4fv, c_glUniform4fv, (GLuint program, GLint loc, GLsizei n, const GLfloat *v), (l, n, v))
#undef PROGRAM_UNIFORM

/* ---- buffers: changed bound to the copy target (no vertex array's state) ---- */

static void APIENTRY f_CreateBuffers(GLsizei n, GLuint *b) {
    GLsizei i;

    c_glGenBuffers(n, b);
    for (i = 0; i < n; i++) {
        c_glBindBuffer(GL_COPY_WRITE_BUFFER, b[i]);   /* (made: a name only until bound) */
    }
    c_glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

static void APIENTRY f_NamedBufferStorage(GLuint b, GLsizeiptr size, const void *data, GLbitfield flags) {
    c_glBindBuffer(GL_COPY_WRITE_BUFFER, b);
    c_glBufferData(GL_COPY_WRITE_BUFFER, size, data, flags & GL_DYNAMIC_STORAGE_BIT ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    c_glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

static void APIENTRY f_NamedBufferData(GLuint b, GLsizeiptr size, const void *data, GLenum usage) {
    c_glBindBuffer(GL_COPY_WRITE_BUFFER, b);
    c_glBufferData(GL_COPY_WRITE_BUFFER, size, data, usage);
    c_glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

static void APIENTRY f_NamedBufferSubData(GLuint b, GLintptr offset, GLsizeiptr size, const void *data) {
    c_glBindBuffer(GL_COPY_WRITE_BUFFER, b);
    c_glBufferSubData(GL_COPY_WRITE_BUFFER, offset, size, data);
    c_glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

/* ---- vertex arrays: the format and the buffer kept apart (as 4.3's), each attribute's pointer
 * set again once both are known ---- */

static void vao_apply(GLuint vao) {
    VaoInfo *v = vao_info(vao);
    int i;

    sBindVertexArray(vao);
    for (i = 0; i < VAO_ATTRS; i++) {
        const VaoAttr *a = &v->attr[i];

        if (a->set && a->binding < VAO_BINDINGS && v->buffer[a->binding] != 0) {
            c_glBindBuffer(GL_ARRAY_BUFFER, v->buffer[a->binding]);
            c_glVertexAttribPointer((GLuint)i, a->size, a->type, a->normalized, v->stride[a->binding],
                                    (const void *)(v->offset[a->binding] + (GLintptr)a->offset));
        }
    }
    c_glBindBuffer(GL_ARRAY_BUFFER, 0);
    sBindVertexArray(sVertexArray);
}

static void APIENTRY f_CreateVertexArrays(GLsizei n, GLuint *a) {
    GLsizei i;

    c_glGenVertexArrays(n, a);
    for (i = 0; i < n; i++) {
        memset(vao_info(a[i]), 0, sizeof(VaoInfo));   /* (a name used before: forgotten) */
        sBindVertexArray(a[i]);
    }
    sBindVertexArray(sVertexArray);
}

static void APIENTRY f_EnableVertexArrayAttrib(GLuint vao, GLuint index) {
    sBindVertexArray(vao);
    c_glEnableVertexAttribArray(index);
    sBindVertexArray(sVertexArray);
}

static void APIENTRY f_VertexArrayAttribFormat(GLuint vao, GLuint index, GLint size, GLenum type, GLboolean normalized,
                                               GLuint offset) {
    VaoInfo *v = vao_info(vao);

    if (index < VAO_ATTRS) {
        v->attr[index].size = size;
        v->attr[index].type = type;
        v->attr[index].normalized = normalized;
        v->attr[index].offset = offset;
        v->attr[index].set = 1;
        vao_apply(vao);
    }
}

static void APIENTRY f_VertexArrayAttribBinding(GLuint vao, GLuint index, GLuint binding) {
    if (index < VAO_ATTRS) {
        vao_info(vao)->attr[index].binding = binding;
        vao_apply(vao);
    }
}

static void APIENTRY f_VertexArrayVertexBuffer(GLuint vao, GLuint binding, GLuint buffer, GLintptr offset, GLsizei stride) {
    VaoInfo *v = vao_info(vao);

    if (binding < VAO_BINDINGS) {
        v->buffer[binding] = buffer;
        v->offset[binding] = offset;
        v->stride[binding] = stride;
        vao_apply(vao);
    }
}

/* ---- textures: each one's target remembered (the binds need it) ---- */

static void APIENTRY f_CreateTextures(GLenum target, GLsizei n, GLuint *t) {
    GLsizei i;

    c_glGenTextures(n, t);
    for (i = 0; i < n; i++) {
        sTextureTarget = grow(sTextureTarget, &sTextureN, t[i], sizeof(*sTextureTarget));
        sTextureTarget[t[i]] = target;
        texture_edit(t[i]);   /* (made, with its target) */
    }
}

/* a sized format's pixel layout (none are sent: any that fits it) */
static void format_of(GLenum internal, GLenum *format, GLenum *type) {
    switch (internal) {
    case GL_R8: *format = GL_RED; *type = GL_UNSIGNED_BYTE; break;
    case GL_RG8: *format = GL_RG; *type = GL_UNSIGNED_BYTE; break;
    case GL_R16F: *format = GL_RED; *type = GL_FLOAT; break;
    case GL_RGBA16F: *format = GL_RGBA; *type = GL_FLOAT; break;
    case GL_R11F_G11F_B10F: *format = GL_RGB; *type = GL_FLOAT; break;
    case GL_DEPTH_COMPONENT32F: *format = GL_DEPTH_COMPONENT; *type = GL_FLOAT; break;
    case GL_DEPTH32F_STENCIL8: *format = GL_DEPTH_STENCIL; *type = GL_FLOAT_32_UNSIGNED_INT_24_8_REV; break;
    default: *format = GL_RGBA; *type = GL_UNSIGNED_BYTE; break;   /* (RGBA8, SRGB8_ALPHA8) */
    }
}

/* immutable storage's rule kept: its levels and no more (else a one-level texture with the
 * default mipmapped filter is incomplete - black) */
static void APIENTRY f_TextureStorage2D(GLuint t, GLsizei levels, GLenum internal, GLsizei w, GLsizei h) {
    GLenum format, type, target = texture_target(t);
    GLsizei i;

    format_of(internal, &format, &type);
    texture_edit(t);
    for (i = 0; i < levels; i++) {
        c_glTexImage2D(target, i, (GLint)internal, w > 1 ? w : 1, h > 1 ? h : 1, 0, format, type, NULL);
        w >>= 1;
        h >>= 1;
    }
    c_glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
    c_glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, levels - 1);
}

static void APIENTRY f_TextureStorage3D(GLuint t, GLsizei levels, GLenum internal, GLsizei w, GLsizei h, GLsizei d) {
    GLenum format, type, target = texture_target(t);
    GLsizei i;

    format_of(internal, &format, &type);
    texture_edit(t);
    for (i = 0; i < levels; i++) {   /* (an array: its layers stay) */
        c_glTexImage3D(target, i, (GLint)internal, w > 1 ? w : 1, h > 1 ? h : 1, d, 0, format, type, NULL);
        w >>= 1;
        h >>= 1;
    }
    c_glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
    c_glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, levels - 1);
}

static void APIENTRY f_TextureSubImage2D(GLuint t, GLint level, GLint x, GLint y, GLsizei w, GLsizei h, GLenum format,
                                         GLenum type, const void *pixels) {
    texture_edit(t);
    c_glTexSubImage2D(texture_target(t), level, x, y, w, h, format, type, pixels);
}

static void APIENTRY f_TextureParameteri(GLuint t, GLenum pname, GLint value) {
    texture_edit(t);
    c_glTexParameteri(texture_target(t), pname, value);
}

static void APIENTRY f_BindTextureUnit(GLuint unit, GLuint t) {
    c_glActiveTexture(GL_TEXTURE0 + unit);
    if (t == 0) {   /* (none: whichever kind was there) */
        c_glBindTexture(GL_TEXTURE_2D, 0);
        c_glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    } else {
        c_glBindTexture(texture_target(t), t);
    }
}

static void APIENTRY f_GenerateTextureMipmap(GLuint t) {
    texture_edit(t);
    c_glGenerateMipmap(texture_target(t));
}

static void APIENTRY f_CopyTextureSubImage2D(GLuint t, GLint level, GLint xoff, GLint yoff, GLint x, GLint y, GLsizei w,
                                             GLsizei h) {
    texture_edit(t);   /* (from the framebuffer bound to read, as the 4.5 one) */
    c_glCopyTexSubImage2D(texture_target(t), level, xoff, yoff, x, y, w, h);
}

static void APIENTRY f_CreateSamplers(GLsizei n, GLuint *s) {
    GLsizei i;

    c_glGenSamplers(n, s);
    for (i = 0; i < n; i++) {   /* (made by binding, to be safe with any driver) */
        glBindSampler(SCRATCH_UNIT, s[i]);
    }
    glBindSampler(SCRATCH_UNIT, 0);
}

/* ---- framebuffers and renderbuffers ---- */

static void APIENTRY f_CreateFramebuffers(GLsizei n, GLuint *f) {
    GLsizei i;

    c_glGenFramebuffers(n, f);
    for (i = 0; i < n; i++) {
        fbo_edit(f[i]);
    }
    fbo_done();
}

static void APIENTRY f_NamedFramebufferTexture(GLuint f, GLenum attachment, GLuint t, GLint level) {
    fbo_edit(f);
    c_glFramebufferTexture(GL_DRAW_FRAMEBUFFER, attachment, t, level);
    fbo_done();
}

static void APIENTRY f_NamedFramebufferTextureLayer(GLuint f, GLenum attachment, GLuint t, GLint level, GLint layer) {
    fbo_edit(f);
    c_glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, attachment, t, level, layer);
    fbo_done();
}

static void APIENTRY f_NamedFramebufferRenderbuffer(GLuint f, GLenum attachment, GLenum target, GLuint r) {
    fbo_edit(f);
    c_glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, attachment, target, r);
    fbo_done();
}

static GLenum APIENTRY f_CheckNamedFramebufferStatus(GLuint f, GLenum target) {
    GLenum status;

    (void)target;
    fbo_edit(f);
    status = c_glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    fbo_done();
    return status;
}

static void APIENTRY f_NamedFramebufferDrawBuffer(GLuint f, GLenum buf) {
    fbo_edit(f);
    c_glDrawBuffer(buf);
    fbo_done();
}

static void APIENTRY f_NamedFramebufferDrawBuffers(GLuint f, GLsizei n, const GLenum *bufs) {
    fbo_edit(f);
    c_glDrawBuffers(n, bufs);
    fbo_done();
}

static void APIENTRY f_NamedFramebufferReadBuffer(GLuint f, GLenum buf) {
    sBindFramebuffer(GL_READ_FRAMEBUFFER, f);
    c_glReadBuffer(buf);
    fbo_done();
}

static void APIENTRY f_BlitNamedFramebuffer(GLuint from, GLuint to, GLint x0, GLint y0, GLint x1, GLint y1, GLint dx0,
                                            GLint dy0, GLint dx1, GLint dy1, GLbitfield mask, GLenum filter) {
    sBindFramebuffer(GL_READ_FRAMEBUFFER, from);
    sBindFramebuffer(GL_DRAW_FRAMEBUFFER, to);
    c_glBlitFramebuffer(x0, y0, x1, y1, dx0, dy0, dx1, dy1, mask, filter);
    fbo_done();
}

static void APIENTRY f_ClearNamedFramebufferfv(GLuint f, GLenum buffer, GLint drawbuffer, const GLfloat *value) {
    fbo_edit(f);
    c_glClearBufferfv(buffer, drawbuffer, value);
    fbo_done();
}

static void APIENTRY f_ClearNamedFramebufferfi(GLuint f, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) {
    fbo_edit(f);
    c_glClearBufferfi(buffer, drawbuffer, depth, stencil);
    fbo_done();
}

static void APIENTRY f_CreateRenderbuffers(GLsizei n, GLuint *r) {
    GLsizei i;

    c_glGenRenderbuffers(n, r);
    for (i = 0; i < n; i++) {
        c_glBindRenderbuffer(GL_RENDERBUFFER, r[i]);
    }
    c_glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

static void APIENTRY f_NamedRenderbufferStorageMultisample(GLuint r, GLsizei samples, GLenum internal, GLsizei w,
                                                           GLsizei h) {
    c_glBindRenderbuffer(GL_RENDERBUFFER, r);
    c_glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internal, w, h);
    c_glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/* a colour texture's part copied into another's: blitted between two framebuffers of its own */
static void APIENTRY f_CopyImageSubData(GLuint src, GLenum src_target, GLint src_level, GLint sx, GLint sy, GLint sz,
                                        GLuint dst, GLenum dst_target, GLint dst_level, GLint dx, GLint dy, GLint dz,
                                        GLsizei w, GLsizei h, GLsizei d) {
    static GLuint from, to;

    (void)src_target; (void)dst_target; (void)sz; (void)dz; (void)d;
    if (from == 0) {
        c_glGenFramebuffers(1, &from);
        c_glGenFramebuffers(1, &to);
    }
    sBindFramebuffer(GL_READ_FRAMEBUFFER, from);
    c_glFramebufferTexture(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, src, src_level);
    c_glReadBuffer(GL_COLOR_ATTACHMENT0);
    sBindFramebuffer(GL_DRAW_FRAMEBUFFER, to);
    c_glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, dst, dst_level);
    c_glDrawBuffer(GL_COLOR_ATTACHMENT0);
    c_glBlitFramebuffer(sx, sy, sx + w, sy + h, dx, dy, dx + w, dy + h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    c_glFramebufferTexture(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0);   /* (let go: either may be deleted) */
    c_glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0);
    fbo_done();
}

static void APIENTRY f_ReadnPixels(GLint x, GLint y, GLsizei w, GLsizei h, GLenum format, GLenum type, GLsizei size,
                                   void *data) {
    (void)size;
    c_glReadPixels(x, y, w, h, format, type, data);
}

/* ---- loading ---- */

static int has_extension(const char *name) {
    GLint n = 0, i;

    glGetIntegerv(GL_NUM_EXTENSIONS, &n);
    for (i = 0; i < n; i++) {
        const char *e = (const char *)glGetStringi(GL_EXTENSIONS, (GLuint)i);

        if (e != NULL && strcmp(e, name) == 0) {
            return 1;
        }
    }
    return 0;
}

int gl_load(int fallbacks) {
    int ok = 1;

#define LOAD_FN(type, name)                                            \
    p_##name = (type)SDL_GL_GetProcAddress(#name);                     \
    if (p_##name == NULL) {                                            \
        fprintf(stderr, "gl: missing %s\n", #name);                    \
        ok = 0;                                                        \
    }
    GL_CORE_FUNCTIONS(LOAD_FN)
#undef LOAD_FN
    if (!ok) {
        return 0;
    }
    sLow = fallbacks;
    if (!fallbacks) {
#define LOAD_FN(type, name)                                            \
    p_##name = (type)SDL_GL_GetProcAddress(#name);                     \
    if (p_##name == NULL) {                                            \
        fprintf(stderr, "gl: missing %s\n", #name);                    \
        ok = 0;                                                        \
    }
        GL_DSA_FUNCTIONS(LOAD_FN)
#undef LOAD_FN
        sAnisotropy = 1;   /* (4.6 core) */
    } else {
#define LOAD_FN(type, name)                                            \
    c_##name = (type)SDL_GL_GetProcAddress(#name);                     \
    if (c_##name == NULL) {                                            \
        fprintf(stderr, "gl: missing %s\n", #name);                    \
        ok = 0;                                                        \
    }
        GL_CLASSIC_FUNCTIONS(LOAD_FN)
#undef LOAD_FN
        p_glProgramUniform1i = f_ProgramUniform1i;
        p_glProgramUniform1f = f_ProgramUniform1f;
        p_glProgramUniform2f = f_ProgramUniform2f;
        p_glProgramUniform3f = f_ProgramUniform3f;
        p_glProgramUniform4f = f_ProgramUniform4f;
        p_glProgramUniformMatrix4fv = f_ProgramUniformMatrix4fv;
        p_glProgramUniform1fv = f_ProgramUniform1fv;
        p_glProgramUniform3fv = f_ProgramUniform3fv;
        p_glProgramUniform4fv = f_ProgramUniform4fv;
        p_glCreateBuffers = f_CreateBuffers;
        p_glNamedBufferStorage = f_NamedBufferStorage;
        p_glNamedBufferData = f_NamedBufferData;
        p_glNamedBufferSubData = f_NamedBufferSubData;
        p_glCreateVertexArrays = f_CreateVertexArrays;
        p_glEnableVertexArrayAttrib = f_EnableVertexArrayAttrib;
        p_glVertexArrayAttribFormat = f_VertexArrayAttribFormat;
        p_glVertexArrayAttribBinding = f_VertexArrayAttribBinding;
        p_glVertexArrayVertexBuffer = f_VertexArrayVertexBuffer;
        p_glCreateTextures = f_CreateTextures;
        p_glTextureStorage2D = f_TextureStorage2D;
        p_glTextureStorage3D = f_TextureStorage3D;
        p_glTextureSubImage2D = f_TextureSubImage2D;
        p_glTextureParameteri = f_TextureParameteri;
        p_glBindTextureUnit = f_BindTextureUnit;
        p_glGenerateTextureMipmap = f_GenerateTextureMipmap;
        p_glCopyTextureSubImage2D = f_CopyTextureSubImage2D;
        p_glCreateSamplers = f_CreateSamplers;
        p_glCreateFramebuffers = f_CreateFramebuffers;
        p_glNamedFramebufferTexture = f_NamedFramebufferTexture;
        p_glNamedFramebufferTextureLayer = f_NamedFramebufferTextureLayer;
        p_glNamedFramebufferRenderbuffer = f_NamedFramebufferRenderbuffer;
        p_glCheckNamedFramebufferStatus = f_CheckNamedFramebufferStatus;
        p_glNamedFramebufferDrawBuffer = f_NamedFramebufferDrawBuffer;
        p_glNamedFramebufferDrawBuffers = f_NamedFramebufferDrawBuffers;
        p_glNamedFramebufferReadBuffer = f_NamedFramebufferReadBuffer;
        p_glBlitNamedFramebuffer = f_BlitNamedFramebuffer;
        p_glClearNamedFramebufferfv = f_ClearNamedFramebufferfv;
        p_glClearNamedFramebufferfi = f_ClearNamedFramebufferfi;
        p_glCreateRenderbuffers = f_CreateRenderbuffers;
        p_glNamedRenderbufferStorageMultisample = f_NamedRenderbufferStorageMultisample;
        p_glCopyImageSubData = f_CopyImageSubData;
        p_glReadnPixels = f_ReadnPixels;
        /* the bindings the fallbacks change and put back: kept track of */
        sUseProgram = p_glUseProgram;
        sBindVertexArray = p_glBindVertexArray;
        sBindFramebuffer = p_glBindFramebuffer;
        p_glUseProgram = use_program;
        p_glBindVertexArray = bind_vertex_array;
        p_glBindFramebuffer = bind_framebuffer;
        sAnisotropy = has_extension("GL_ARB_texture_filter_anisotropic") || has_extension("GL_EXT_texture_filter_anisotropic");
    }
    if (ok) {
        GLint samples = 1;

        glGetIntegerv(GL_MAX_SAMPLES, &samples);
        sMaxSamples = samples < 1 ? 1 : samples;
    }
    return ok;
}

int gl_low(void) {
    return sLow;
}

int gl_anisotropy(void) {
    return sAnisotropy;
}

int gl_max_samples(void) {
    return sMaxSamples;
}

static void APIENTRY debug_message(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei len,
                                   const GLchar *msg, const void *user) {
    (void)source; (void)id; (void)len; (void)user;
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        return;
    }
    fprintf(stderr, "gl: %s%s\n", type == GL_DEBUG_TYPE_ERROR ? "error: " : "", msg);
}

void gl_debug_output(void) {
    PFNGLDEBUGMESSAGECALLBACKPROC callback = (PFNGLDEBUGMESSAGECALLBACKPROC)SDL_GL_GetProcAddress("glDebugMessageCallback");

    if (callback == NULL || (sLow && !has_extension("GL_KHR_debug"))) {   /* (4.3 core; on 3.3 an extension) */
        return;
    }
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    callback(debug_message, NULL);
}

/* ---- shaders ---- */

/* GLSL 4.60 to 3.30: the version line, and the explicit uniform locations and sampler bindings
 * (4.30 / 4.20) taken out - each kept, by name, for once the program is linked. A declaration
 * written `layout(location = N) uniform TYPE NAME...` or `layout(binding = N) uniform TYPE NAME;`
 * (the inputs' and outputs' locations are 3.30's own: left) */
typedef struct Rewritten {
    char *src;
    struct {
        char name[48];
        int at, binding;   /* the location or the unit; binding: a sampler's unit */
    } decl[192];
    int n;
} Rewritten;

static int read_decl(const char *p, const char *what, int *at, const char **name, int *len) {
    size_t wl = strlen(what);
    const char *q;

    if (strncmp(p, what, wl) != 0) {
        return 0;
    }
    q = p + wl;
    *at = (int)strtol(q, (char **)&q, 10);
    if (strncmp(q, ") uniform ", 10) != 0) {
        return 0;
    }
    q += 10;
    while (*q != ' ' && *q != 0) {   /* the type */
        q++;
    }
    while (*q == ' ') {
        q++;
    }
    *name = q;
    while ((*q >= 'a' && *q <= 'z') || (*q >= 'A' && *q <= 'Z') || (*q >= '0' && *q <= '9') || *q == '_') {
        q++;
    }
    *len = (int)(q - *name);
    return *len > 0;
}

static int rewrite(const char *src, Rewritten *r) {
    size_t n = strlen(src);
    const char *p = src, *nl;
    char *out;

    r->src = out = malloc(n + 64);
    if (out == NULL) {
        return 0;
    }
    nl = strchr(p, '\n');
    if (strncmp(p, "#version", 8) == 0 && nl != NULL) {
        p = nl + 1;
    }
    out += sprintf(out, "#version 330 core\n");
    while (*p != 0) {
        int at, len, binding = 0;
        const char *name;

        if ((read_decl(p, "layout(location = ", &at, &name, &len) ||
             (binding = read_decl(p, "layout(binding = ", &at, &name, &len))) && r->n < 192 && len < 48) {
            memcpy(r->decl[r->n].name, name, (size_t)len);
            r->decl[r->n].name[len] = 0;
            r->decl[r->n].at = at;
            r->decl[r->n].binding = binding;
            r->n++;
            p = strchr(p, ')') + 2;   /* (from "uniform") */
            continue;
        }
        *out++ = *p++;
    }
    *out = 0;
    return 1;
}

static GLuint shader(GLenum type, const char *src, const char *name) {
    GLuint s = glCreateShader(type);
    GLint ok;

    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[2048];

        glGetShaderInfoLog(s, sizeof(log), NULL, log);
        fprintf(stderr, "gl: shader %s (%s): %s\n", name, type == GL_VERTEX_SHADER ? "vertex" : "fragment", log);
    }
    return s;
}

/* the 3.3 path: the declarations' locations looked up, the samplers' units set */
static void map_locations(GLuint p, const Rewritten *rw, int nrw) {
    ProgramInfo *info;
    int k, i;

    sProgramInfo = grow(sProgramInfo, &sProgramN, p, sizeof(*sProgramInfo));
    if (sProgramInfo[p] == NULL) {
        sProgramInfo[p] = malloc(sizeof(ProgramInfo));
    }
    info = sProgramInfo[p];
    for (i = 0; i < PROGRAM_LOCATIONS; i++) {
        info->location[i] = -1;
    }
    sUseProgram(p);
    for (k = 0; k < nrw; k++) {
        for (i = 0; i < rw[k].n; i++) {
            GLint l = c_glGetUniformLocation(p, rw[k].decl[i].name);

            if (rw[k].decl[i].binding) {
                if (l >= 0) {
                    c_glUniform1i(l, rw[k].decl[i].at);
                }
            } else if (rw[k].decl[i].at >= 0 && rw[k].decl[i].at < PROGRAM_LOCATIONS) {
                info->location[rw[k].decl[i].at] = l;
            }
        }
    }
    sUseProgram(sProgram);
}

unsigned int gl_program(const char *vs, const char *fs, const char *name) {
    static Rewritten rw[2];
    GLuint p, v, f;
    GLint ok;

    if (sLow) {
        rw[0].n = rw[1].n = 0;
        if (!rewrite(vs, &rw[0]) || !rewrite(fs, &rw[1])) {
            free(rw[0].src);
            return 0;
        }
        vs = rw[0].src;
        fs = rw[1].src;
    }
    p = glCreateProgram();
    v = shader(GL_VERTEX_SHADER, vs, name);
    f = shader(GL_FRAGMENT_SHADER, fs, name);
    glAttachShader(p, v);
    glAttachShader(p, f);
    glLinkProgram(p);
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    glDetachShader(p, v);
    glDetachShader(p, f);
    glDeleteShader(v);
    glDeleteShader(f);
    if (sLow) {
        if (ok) {
            map_locations(p, rw, 2);
        }
        free(rw[0].src);
        free(rw[1].src);
        rw[0].src = rw[1].src = NULL;
    }
    if (!ok) {
        char log[2048];

        glGetProgramInfoLog(p, sizeof(log), NULL, log);
        fprintf(stderr, "gl: program %s: %s\n", name, log);
        return 0;
    }
    return p;
}
