/* OpenGL 3.3 core: the functions past GL 1.1 are loaded at start-up (gl_load, after the context
 * exists) into pointers; the macros below let the code call them by their usual names. */
#ifndef GL_H
#define GL_H

#include <SDL3/SDL_opengl.h>

/* every GL function used beyond 1.1: X(type, name) */
#define GL_FUNCTIONS(X)                                              \
    X(PFNGLCREATESHADERPROC, glCreateShader)                         \
    X(PFNGLSHADERSOURCEPROC, glShaderSource)                         \
    X(PFNGLCOMPILESHADERPROC, glCompileShader)                       \
    X(PFNGLGETSHADERIVPROC, glGetShaderiv)                           \
    X(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog)                 \
    X(PFNGLDELETESHADERPROC, glDeleteShader)                         \
    X(PFNGLCREATEPROGRAMPROC, glCreateProgram)                       \
    X(PFNGLATTACHSHADERPROC, glAttachShader)                         \
    X(PFNGLLINKPROGRAMPROC, glLinkProgram)                           \
    X(PFNGLGETPROGRAMIVPROC, glGetProgramiv)                         \
    X(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog)               \
    X(PFNGLUSEPROGRAMPROC, glUseProgram)                             \
    X(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation)             \
    X(PFNGLUNIFORM1IPROC, glUniform1i)                               \
    X(PFNGLUNIFORM1FPROC, glUniform1f)                               \
    X(PFNGLUNIFORM4FPROC, glUniform4f)                               \
    X(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv)                 \
    X(PFNGLGENBUFFERSPROC, glGenBuffers)                             \
    X(PFNGLBINDBUFFERPROC, glBindBuffer)                             \
    X(PFNGLBUFFERDATAPROC, glBufferData)                             \
    X(PFNGLDELETEBUFFERSPROC, glDeleteBuffers)                       \
    X(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays)                   \
    X(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray)                   \
    X(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays)             \
    X(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray)   \
    X(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer)           \
    X(PFNGLACTIVETEXTUREPROC, glActiveTexture)                       \
    X(PFNGLBLENDFUNCSEPARATEPROC, glBlendFuncSeparate)               \
    X(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap)

#define GL_DECLARE(type, name) extern type p_##name;
GL_FUNCTIONS(GL_DECLARE)
#undef GL_DECLARE

#define glCreateShader p_glCreateShader
#define glShaderSource p_glShaderSource
#define glCompileShader p_glCompileShader
#define glGetShaderiv p_glGetShaderiv
#define glGetShaderInfoLog p_glGetShaderInfoLog
#define glDeleteShader p_glDeleteShader
#define glCreateProgram p_glCreateProgram
#define glAttachShader p_glAttachShader
#define glLinkProgram p_glLinkProgram
#define glGetProgramiv p_glGetProgramiv
#define glGetProgramInfoLog p_glGetProgramInfoLog
#define glUseProgram p_glUseProgram
#define glGetUniformLocation p_glGetUniformLocation
#define glUniform1i p_glUniform1i
#define glUniform1f p_glUniform1f
#define glUniform4f p_glUniform4f
#define glUniformMatrix4fv p_glUniformMatrix4fv
#define glGenBuffers p_glGenBuffers
#define glBindBuffer p_glBindBuffer
#define glBufferData p_glBufferData
#define glDeleteBuffers p_glDeleteBuffers
#define glGenVertexArrays p_glGenVertexArrays
#define glBindVertexArray p_glBindVertexArray
#define glDeleteVertexArrays p_glDeleteVertexArrays
#define glEnableVertexAttribArray p_glEnableVertexAttribArray
#define glVertexAttribPointer p_glVertexAttribPointer
#define glActiveTexture p_glActiveTexture
#define glBlendFuncSeparate p_glBlendFuncSeparate
#define glGenerateMipmap p_glGenerateMipmap

/* load the pointers (a GL context must be current); 0 if one is missing */
int gl_load(void);

#endif
