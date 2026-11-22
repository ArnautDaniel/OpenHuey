/* OpenGL: 4.6 core where the driver has it, else 3.3 core (the low-power path: older and smaller
 * GPUs - Intel's from Sandy Bridge, NVIDIA's and AMD's DX10 generation). The functions past GL 1.1
 * are loaded at start-up (gl_load, after the context exists) into pointers; the macros below let
 * the code call them by their usual names.
 *
 * The code is written 4.5 style (direct state access: glCreate*, glNamed*, glTexture*,
 * glVertexArray*, glProgramUniform*). On the 3.3 path those pointers are gl.c's own fallbacks,
 * which do the same with 3.3's functions (bind, change, put the binding back), and gl_program
 * rewrites the shaders for GLSL 3.30 - so the renderer has one way of doing everything. */
#ifndef GL_H
#define GL_H

#include <SDL3/SDL_opengl.h>

/* GL 1.0 / 1.1's, loaded too (nothing linked against libGL: SDL finds the driver's) */
typedef void (APIENTRY *HGA_GLBLENDFUNC)(GLenum, GLenum);
typedef void (APIENTRY *HGA_GLDELETETEXTURES)(GLsizei, const GLuint *);
typedef void (APIENTRY *HGA_GLDEPTHFUNC)(GLenum);
typedef void (APIENTRY *HGA_GLDEPTHMASK)(GLboolean);
typedef void (APIENTRY *HGA_GLDEPTHRANGE)(GLdouble, GLdouble);
typedef void (APIENTRY *HGA_GLDISABLE)(GLenum);
typedef void (APIENTRY *HGA_GLDRAWARRAYS)(GLenum, GLint, GLsizei);
typedef void (APIENTRY *HGA_GLENABLE)(GLenum);
typedef const GLubyte * (APIENTRY *HGA_GLGETSTRING)(GLenum);
typedef void (APIENTRY *HGA_GLPOLYGONOFFSET)(GLfloat, GLfloat);
typedef void (APIENTRY *HGA_GLSTENCILFUNC)(GLenum, GLint, GLuint);
typedef void (APIENTRY *HGA_GLSTENCILOP)(GLenum, GLenum, GLenum);
typedef void (APIENTRY *HGA_GLVIEWPORT)(GLint, GLint, GLsizei, GLsizei);
typedef void (APIENTRY *HGA_GLGETINTEGERV)(GLenum, GLint *);

/* the functions in GL 3.3 core, used as they are on both paths: X(type, name) */
#define GL_CORE_FUNCTIONS(X) \
    X(PFNGLCREATESHADERPROC, glCreateShader)                                   \
    X(PFNGLSHADERSOURCEPROC, glShaderSource)                                   \
    X(PFNGLCOMPILESHADERPROC, glCompileShader)                                 \
    X(PFNGLGETSHADERIVPROC, glGetShaderiv)                                     \
    X(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog)                           \
    X(PFNGLDELETESHADERPROC, glDeleteShader)                                   \
    X(PFNGLCREATEPROGRAMPROC, glCreateProgram)                                 \
    X(PFNGLATTACHSHADERPROC, glAttachShader)                                   \
    X(PFNGLDETACHSHADERPROC, glDetachShader)                                   \
    X(PFNGLLINKPROGRAMPROC, glLinkProgram)                                     \
    X(PFNGLGETPROGRAMIVPROC, glGetProgramiv)                                   \
    X(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog)                         \
    X(PFNGLUSEPROGRAMPROC, glUseProgram)                                       \
    X(PFNGLDELETEBUFFERSPROC, glDeleteBuffers)                                 \
    X(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray)                             \
    X(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays)                       \
    X(PFNGLSAMPLERPARAMETERIPROC, glSamplerParameteri)                         \
    X(PFNGLSAMPLERPARAMETERFPROC, glSamplerParameterf)                         \
    X(PFNGLBINDSAMPLERPROC, glBindSampler)                                     \
    X(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer)                             \
    X(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers)                       \
    X(PFNGLDELETERENDERBUFFERSPROC, glDeleteRenderbuffers)                     \
    X(PFNGLCLEARBUFFERIVPROC, glClearBufferiv)                                 \
    X(PFNGLCOLORMASKIPROC, glColorMaski)                                       \
    X(PFNGLENABLEIPROC, glEnablei)                                             \
    X(PFNGLBLENDCOLORPROC, glBlendColor)                                       \
    X(PFNGLBLENDEQUATIONPROC, glBlendEquation)                                 \
    X(PFNGLGETSTRINGIPROC, glGetStringi)                                       \
    X(HGA_GLGETINTEGERV, glGetIntegerv)                                        \
    X(HGA_GLBLENDFUNC, glBlendFunc)                                            \
    X(HGA_GLDELETETEXTURES, glDeleteTextures)                                  \
    X(HGA_GLDEPTHFUNC, glDepthFunc)                                            \
    X(HGA_GLDEPTHMASK, glDepthMask)                                            \
    X(HGA_GLDEPTHRANGE, glDepthRange)                                          \
    X(HGA_GLDISABLE, glDisable)                                                \
    X(HGA_GLDRAWARRAYS, glDrawArrays)                                          \
    X(HGA_GLENABLE, glEnable)                                                  \
    X(HGA_GLGETSTRING, glGetString)                                            \
    X(HGA_GLPOLYGONOFFSET, glPolygonOffset)                                    \
    X(HGA_GLSTENCILFUNC, glStencilFunc)                                        \
    X(HGA_GLSTENCILOP, glStencilOp)                                            \
    X(HGA_GLVIEWPORT, glViewport)

/* the 4.1 - 4.5 functions: the driver's on the 4.6 path, gl.c's fallbacks on the 3.3 one */
#define GL_DSA_FUNCTIONS(X) \
    X(PFNGLPROGRAMUNIFORM1IPROC, glProgramUniform1i)                           \
    X(PFNGLPROGRAMUNIFORM1FPROC, glProgramUniform1f)                           \
    X(PFNGLPROGRAMUNIFORM2FPROC, glProgramUniform2f)                           \
    X(PFNGLPROGRAMUNIFORM3FPROC, glProgramUniform3f)                           \
    X(PFNGLPROGRAMUNIFORM4FPROC, glProgramUniform4f)                           \
    X(PFNGLPROGRAMUNIFORMMATRIX4FVPROC, glProgramUniformMatrix4fv)             \
    X(PFNGLPROGRAMUNIFORM1FVPROC, glProgramUniform1fv)                         \
    X(PFNGLPROGRAMUNIFORM3FVPROC, glProgramUniform3fv)                         \
    X(PFNGLPROGRAMUNIFORM4FVPROC, glProgramUniform4fv)                         \
    X(PFNGLCREATEBUFFERSPROC, glCreateBuffers)                                 \
    X(PFNGLNAMEDBUFFERSTORAGEPROC, glNamedBufferStorage)                       \
    X(PFNGLNAMEDBUFFERDATAPROC, glNamedBufferData)                             \
    X(PFNGLNAMEDBUFFERSUBDATAPROC, glNamedBufferSubData)                       \
    X(PFNGLCREATEVERTEXARRAYSPROC, glCreateVertexArrays)                       \
    X(PFNGLENABLEVERTEXARRAYATTRIBPROC, glEnableVertexArrayAttrib)             \
    X(PFNGLVERTEXARRAYATTRIBFORMATPROC, glVertexArrayAttribFormat)             \
    X(PFNGLVERTEXARRAYATTRIBBINDINGPROC, glVertexArrayAttribBinding)           \
    X(PFNGLVERTEXARRAYVERTEXBUFFERPROC, glVertexArrayVertexBuffer)             \
    X(PFNGLCREATETEXTURESPROC, glCreateTextures)                               \
    X(PFNGLTEXTURESTORAGE2DPROC, glTextureStorage2D)                           \
    X(PFNGLTEXTURESTORAGE3DPROC, glTextureStorage3D)                           \
    X(PFNGLTEXTURESUBIMAGE2DPROC, glTextureSubImage2D)                         \
    X(PFNGLTEXTUREPARAMETERIPROC, glTextureParameteri)                         \
    X(PFNGLTEXTUREBUFFERPROC, glTextureBuffer)                                 \
    X(PFNGLBINDTEXTUREUNITPROC, glBindTextureUnit)                             \
    X(PFNGLGENERATETEXTUREMIPMAPPROC, glGenerateTextureMipmap)                 \
    X(PFNGLCOPYTEXTURESUBIMAGE2DPROC, glCopyTextureSubImage2D)                 \
    X(PFNGLCREATESAMPLERSPROC, glCreateSamplers)                               \
    X(PFNGLCREATEFRAMEBUFFERSPROC, glCreateFramebuffers)                       \
    X(PFNGLNAMEDFRAMEBUFFERTEXTUREPROC, glNamedFramebufferTexture)             \
    X(PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC, glNamedFramebufferTextureLayer)   \
    X(PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC, glNamedFramebufferRenderbuffer)   \
    X(PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC, glCheckNamedFramebufferStatus)     \
    X(PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC, glNamedFramebufferDrawBuffer)       \
    X(PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC, glNamedFramebufferDrawBuffers)     \
    X(PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC, glNamedFramebufferReadBuffer)       \
    X(PFNGLBLITNAMEDFRAMEBUFFERPROC, glBlitNamedFramebuffer)                   \
    X(PFNGLCLEARNAMEDFRAMEBUFFERFVPROC, glClearNamedFramebufferfv)             \
    X(PFNGLCLEARNAMEDFRAMEBUFFERFIPROC, glClearNamedFramebufferfi)             \
    X(PFNGLCREATERENDERBUFFERSPROC, glCreateRenderbuffers)                     \
    X(PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC, glNamedRenderbufferStorageMultisample) \
    X(PFNGLCOPYIMAGESUBDATAPROC, glCopyImageSubData)                           \
    X(PFNGLREADNPIXELSPROC, glReadnPixels)

#define GL_DECLARE(type, name) extern type p_##name;
GL_CORE_FUNCTIONS(GL_DECLARE)
GL_DSA_FUNCTIONS(GL_DECLARE)
#undef GL_DECLARE

#define glBlendFunc p_glBlendFunc
#define glDeleteTextures p_glDeleteTextures
#define glDepthFunc p_glDepthFunc
#define glDepthMask p_glDepthMask
#define glDepthRange p_glDepthRange
#define glDisable p_glDisable
#define glDrawArrays p_glDrawArrays
#define glEnable p_glEnable
#define glGetString p_glGetString
#define glPolygonOffset p_glPolygonOffset
#define glStencilFunc p_glStencilFunc
#define glStencilOp p_glStencilOp
#define glViewport p_glViewport
#define glCreateShader p_glCreateShader
#define glShaderSource p_glShaderSource
#define glCompileShader p_glCompileShader
#define glGetShaderiv p_glGetShaderiv
#define glGetShaderInfoLog p_glGetShaderInfoLog
#define glDeleteShader p_glDeleteShader
#define glCreateProgram p_glCreateProgram
#define glAttachShader p_glAttachShader
#define glDetachShader p_glDetachShader
#define glLinkProgram p_glLinkProgram
#define glGetProgramiv p_glGetProgramiv
#define glGetProgramInfoLog p_glGetProgramInfoLog
#define glUseProgram p_glUseProgram
#define glDeleteBuffers p_glDeleteBuffers
#define glBindVertexArray p_glBindVertexArray
#define glDeleteVertexArrays p_glDeleteVertexArrays
#define glSamplerParameteri p_glSamplerParameteri
#define glSamplerParameterf p_glSamplerParameterf
#define glBindSampler p_glBindSampler
#define glBindFramebuffer p_glBindFramebuffer
#define glDeleteFramebuffers p_glDeleteFramebuffers
#define glDeleteRenderbuffers p_glDeleteRenderbuffers
#define glClearBufferiv p_glClearBufferiv
#define glColorMaski p_glColorMaski
#define glEnablei p_glEnablei
#define glBlendColor p_glBlendColor
#define glBlendEquation p_glBlendEquation
#define glGetStringi p_glGetStringi
#define glGetIntegerv p_glGetIntegerv
#define glProgramUniform1i p_glProgramUniform1i
#define glProgramUniform1f p_glProgramUniform1f
#define glProgramUniform2f p_glProgramUniform2f
#define glProgramUniform3f p_glProgramUniform3f
#define glProgramUniform4f p_glProgramUniform4f
#define glProgramUniformMatrix4fv p_glProgramUniformMatrix4fv
#define glProgramUniform1fv p_glProgramUniform1fv
#define glProgramUniform3fv p_glProgramUniform3fv
#define glProgramUniform4fv p_glProgramUniform4fv
#define glCreateBuffers p_glCreateBuffers
#define glNamedBufferStorage p_glNamedBufferStorage
#define glNamedBufferData p_glNamedBufferData
#define glNamedBufferSubData p_glNamedBufferSubData
#define glCreateVertexArrays p_glCreateVertexArrays
#define glEnableVertexArrayAttrib p_glEnableVertexArrayAttrib
#define glVertexArrayAttribFormat p_glVertexArrayAttribFormat
#define glVertexArrayAttribBinding p_glVertexArrayAttribBinding
#define glVertexArrayVertexBuffer p_glVertexArrayVertexBuffer
#define glCreateTextures p_glCreateTextures
#define glTextureStorage2D p_glTextureStorage2D
#define glTextureStorage3D p_glTextureStorage3D
#define glTextureSubImage2D p_glTextureSubImage2D
#define glTextureParameteri p_glTextureParameteri
#define glTextureBuffer p_glTextureBuffer
#define glBindTextureUnit p_glBindTextureUnit
#define glGenerateTextureMipmap p_glGenerateTextureMipmap
#define glCopyTextureSubImage2D p_glCopyTextureSubImage2D
#define glCreateSamplers p_glCreateSamplers
#define glCreateFramebuffers p_glCreateFramebuffers
#define glNamedFramebufferTexture p_glNamedFramebufferTexture
#define glNamedFramebufferTextureLayer p_glNamedFramebufferTextureLayer
#define glNamedFramebufferRenderbuffer p_glNamedFramebufferRenderbuffer
#define glCheckNamedFramebufferStatus p_glCheckNamedFramebufferStatus
#define glNamedFramebufferDrawBuffer p_glNamedFramebufferDrawBuffer
#define glNamedFramebufferDrawBuffers p_glNamedFramebufferDrawBuffers
#define glNamedFramebufferReadBuffer p_glNamedFramebufferReadBuffer
#define glBlitNamedFramebuffer p_glBlitNamedFramebuffer
#define glClearNamedFramebufferfv p_glClearNamedFramebufferfv
#define glClearNamedFramebufferfi p_glClearNamedFramebufferfi
#define glCreateRenderbuffers p_glCreateRenderbuffers
#define glNamedRenderbufferStorageMultisample p_glNamedRenderbufferStorageMultisample
#define glCopyImageSubData p_glCopyImageSubData
#define glReadnPixels p_glReadnPixels

/* load the pointers (a GL context must be current): fallbacks 0 the driver's 4.5+ functions (0 if
 * one is missing), 1 gl.c's 3.3 fallbacks for them. 0 if a needed function is missing */
int gl_load(int fallbacks);
/* 1 on the 3.3 path (gl.c's fallbacks, GLSL 3.30) */
int gl_low(void);
/* the driver's errors and warnings (GL debug output) printed to stderr, where it has it */
void gl_debug_output(void);
/* a program from vertex and fragment shader sources (errors printed); 0 on failure. The sources
 * are GLSL 4.60 (explicit uniform locations and sampler bindings): on the 3.3 path rewritten
 * for 3.30, the locations then mapped by the glProgramUniform fallbacks */
unsigned int gl_program(const char *vs, const char *fs, const char *name);
/* anisotropic filtering there (4.6 core; on 3.3 an extension) */
int gl_anisotropy(void);
/* the most samples a multisampled target may have */
int gl_max_samples(void);

#endif
