/* OpenGL 4.6 core: the functions past GL 1.1 are loaded at start-up (gl_load, after the context
 * exists) into pointers; the macros below let the code call them by their usual names. */
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

/* every GL function used: X(type, name). The objects are made and changed by name, OpenGL 4.5 style
 * (direct state access): glCreate*, glNamed*, glTexture*, glVertexArray*, glProgramUniform*. */
#define GL_FUNCTIONS(X) \
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
    X(PFNGLPROGRAMUNIFORM1IPROC, glProgramUniform1i)                           \
    X(PFNGLPROGRAMUNIFORM1FPROC, glProgramUniform1f)                           \
    X(PFNGLPROGRAMUNIFORM2FPROC, glProgramUniform2f)                           \
    X(PFNGLPROGRAMUNIFORM3FPROC, glProgramUniform3f)                           \
    X(PFNGLPROGRAMUNIFORM4FPROC, glProgramUniform4f)                           \
    X(PFNGLPROGRAMUNIFORMMATRIX4FVPROC, glProgramUniformMatrix4fv)             \
    X(PFNGLCREATEBUFFERSPROC, glCreateBuffers)                                 \
    X(PFNGLNAMEDBUFFERSTORAGEPROC, glNamedBufferStorage)                       \
    X(PFNGLNAMEDBUFFERDATAPROC, glNamedBufferData)                             \
    X(PFNGLDELETEBUFFERSPROC, glDeleteBuffers)                                 \
    X(PFNGLCREATEVERTEXARRAYSPROC, glCreateVertexArrays)                       \
    X(PFNGLENABLEVERTEXARRAYATTRIBPROC, glEnableVertexArrayAttrib)             \
    X(PFNGLVERTEXARRAYATTRIBFORMATPROC, glVertexArrayAttribFormat)             \
    X(PFNGLVERTEXARRAYATTRIBBINDINGPROC, glVertexArrayAttribBinding)           \
    X(PFNGLVERTEXARRAYVERTEXBUFFERPROC, glVertexArrayVertexBuffer)             \
    X(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray)                             \
    X(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays)                       \
    X(PFNGLCREATETEXTURESPROC, glCreateTextures)                               \
    X(PFNGLTEXTURESTORAGE2DPROC, glTextureStorage2D)                           \
    X(PFNGLTEXTURESUBIMAGE2DPROC, glTextureSubImage2D)                         \
    X(PFNGLTEXTUREPARAMETERIPROC, glTextureParameteri)                         \
    X(PFNGLTEXTUREPARAMETERFPROC, glTextureParameterf)                         \
    X(PFNGLBINDTEXTUREUNITPROC, glBindTextureUnit)                             \
    X(PFNGLGENERATETEXTUREMIPMAPPROC, glGenerateTextureMipmap)                 \
    X(PFNGLCREATESAMPLERSPROC, glCreateSamplers)                               \
    X(PFNGLSAMPLERPARAMETERIPROC, glSamplerParameteri)                         \
    X(PFNGLSAMPLERPARAMETERFPROC, glSamplerParameterf)                         \
    X(PFNGLBINDSAMPLERPROC, glBindSampler)                                     \
    X(PFNGLDELETESAMPLERSPROC, glDeleteSamplers)                               \
    X(PFNGLCREATEFRAMEBUFFERSPROC, glCreateFramebuffers)                       \
    X(PFNGLNAMEDFRAMEBUFFERTEXTUREPROC, glNamedFramebufferTexture)             \
    X(PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC, glNamedFramebufferRenderbuffer)   \
    X(PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC, glCheckNamedFramebufferStatus)     \
    X(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer)                             \
    X(PFNGLBLITNAMEDFRAMEBUFFERPROC, glBlitNamedFramebuffer)                   \
    X(PFNGLCOPYTEXTURESUBIMAGE2DPROC, glCopyTextureSubImage2D)                 \
    X(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers)                       \
    X(PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC, glNamedFramebufferDrawBuffer)       \
    X(PFNGLCREATERENDERBUFFERSPROC, glCreateRenderbuffers)                     \
    X(PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC, glNamedRenderbufferStorageMultisample) \
    X(PFNGLDELETERENDERBUFFERSPROC, glDeleteRenderbuffers)                     \
    X(PFNGLCLEARNAMEDFRAMEBUFFERFVPROC, glClearNamedFramebufferfv)             \
    X(PFNGLCLEARNAMEDFRAMEBUFFERFIPROC, glClearNamedFramebufferfi)             \
    X(PFNGLCLEARBUFFERIVPROC, glClearBufferiv)                                 \
    X(PFNGLNAMEDBUFFERSUBDATAPROC, glNamedBufferSubData)                       \
    X(PFNGLCOPYIMAGESUBDATAPROC, glCopyImageSubData)                           \
    X(PFNGLREADNPIXELSPROC, glReadnPixels)                                     \
    X(PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback)                   \
    X(PFNGLBLENDFUNCIPROC, glBlendFunci) \
    X(PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC, glNamedFramebufferDrawBuffers) \
    X(PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC, glNamedFramebufferReadBuffer) \
    X(PFNGLCOLORMASKIPROC, glColorMaski) \
    X(PFNGLENABLEIPROC, glEnablei) \
    X(PFNGLDISABLEIPROC, glDisablei) \
    X(PFNGLPROGRAMUNIFORM3FVPROC, glProgramUniform3fv) \
    X(PFNGLPROGRAMUNIFORM4FVPROC, glProgramUniform4fv) \
    X(PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC, glNamedFramebufferTextureLayer) \
    X(PFNGLTEXTURESTORAGE3DPROC, glTextureStorage3D) \
    X(PFNGLPROGRAMUNIFORM1FVPROC, glProgramUniform1fv) \
    X(HGA_GLBLENDFUNC, glBlendFunc) \
    X(HGA_GLDELETETEXTURES, glDeleteTextures) \
    X(HGA_GLDEPTHFUNC, glDepthFunc) \
    X(HGA_GLDEPTHMASK, glDepthMask) \
    X(HGA_GLDEPTHRANGE, glDepthRange) \
    X(HGA_GLDISABLE, glDisable) \
    X(HGA_GLDRAWARRAYS, glDrawArrays) \
    X(HGA_GLENABLE, glEnable) \
    X(HGA_GLGETSTRING, glGetString) \
    X(HGA_GLPOLYGONOFFSET, glPolygonOffset) \
    X(HGA_GLSTENCILFUNC, glStencilFunc) \
    X(HGA_GLSTENCILOP, glStencilOp) \
    X(HGA_GLVIEWPORT, glViewport)

#define GL_DECLARE(type, name) extern type p_##name;
GL_FUNCTIONS(GL_DECLARE)
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
#define glProgramUniform1i p_glProgramUniform1i
#define glProgramUniform1f p_glProgramUniform1f
#define glProgramUniform2f p_glProgramUniform2f
#define glProgramUniform3f p_glProgramUniform3f
#define glProgramUniform4f p_glProgramUniform4f
#define glProgramUniformMatrix4fv p_glProgramUniformMatrix4fv
#define glCreateBuffers p_glCreateBuffers
#define glNamedBufferStorage p_glNamedBufferStorage
#define glNamedBufferData p_glNamedBufferData
#define glDeleteBuffers p_glDeleteBuffers
#define glCreateVertexArrays p_glCreateVertexArrays
#define glEnableVertexArrayAttrib p_glEnableVertexArrayAttrib
#define glVertexArrayAttribFormat p_glVertexArrayAttribFormat
#define glVertexArrayAttribBinding p_glVertexArrayAttribBinding
#define glVertexArrayVertexBuffer p_glVertexArrayVertexBuffer
#define glBindVertexArray p_glBindVertexArray
#define glDeleteVertexArrays p_glDeleteVertexArrays
#define glCreateTextures p_glCreateTextures
#define glTextureStorage2D p_glTextureStorage2D
#define glTextureSubImage2D p_glTextureSubImage2D
#define glTextureParameteri p_glTextureParameteri
#define glTextureParameterf p_glTextureParameterf
#define glBindTextureUnit p_glBindTextureUnit
#define glGenerateTextureMipmap p_glGenerateTextureMipmap
#define glCreateSamplers p_glCreateSamplers
#define glSamplerParameteri p_glSamplerParameteri
#define glSamplerParameterf p_glSamplerParameterf
#define glBindSampler p_glBindSampler
#define glDeleteSamplers p_glDeleteSamplers
#define glCreateFramebuffers p_glCreateFramebuffers
#define glNamedFramebufferTexture p_glNamedFramebufferTexture
#define glNamedFramebufferRenderbuffer p_glNamedFramebufferRenderbuffer
#define glCheckNamedFramebufferStatus p_glCheckNamedFramebufferStatus
#define glBindFramebuffer p_glBindFramebuffer
#define glBlitNamedFramebuffer p_glBlitNamedFramebuffer
#define glCopyTextureSubImage2D p_glCopyTextureSubImage2D
#define glDeleteFramebuffers p_glDeleteFramebuffers
#define glNamedFramebufferDrawBuffer p_glNamedFramebufferDrawBuffer
#define glCreateRenderbuffers p_glCreateRenderbuffers
#define glNamedRenderbufferStorageMultisample p_glNamedRenderbufferStorageMultisample
#define glDeleteRenderbuffers p_glDeleteRenderbuffers
#define glClearNamedFramebufferfv p_glClearNamedFramebufferfv
#define glClearNamedFramebufferfi p_glClearNamedFramebufferfi
#define glClearBufferiv p_glClearBufferiv
#define glNamedBufferSubData p_glNamedBufferSubData
#define glCopyImageSubData p_glCopyImageSubData
#define glReadnPixels p_glReadnPixels
#define glDebugMessageCallback p_glDebugMessageCallback
#define glBlendFunci p_glBlendFunci
#define glNamedFramebufferDrawBuffers p_glNamedFramebufferDrawBuffers
#define glNamedFramebufferReadBuffer p_glNamedFramebufferReadBuffer
#define glColorMaski p_glColorMaski
#define glEnablei p_glEnablei
#define glDisablei p_glDisablei
#define glProgramUniform3fv p_glProgramUniform3fv
#define glProgramUniform4fv p_glProgramUniform4fv
#define glNamedFramebufferTextureLayer p_glNamedFramebufferTextureLayer
#define glTextureStorage3D p_glTextureStorage3D
#define glProgramUniform1fv p_glProgramUniform1fv

/* load the pointers (a GL context must be current); 0 if one is missing */
int gl_load(void);
/* print the driver's errors and warnings (GL debug output) to stderr */
void gl_debug_output(void);
/* a program from vertex and fragment shader sources (errors printed); 0 on failure */
unsigned int gl_program(const char *vs, const char *fs, const char *name);

#endif
