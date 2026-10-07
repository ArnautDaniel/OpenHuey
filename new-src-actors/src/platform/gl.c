#include "gl.h"

#include <SDL3/SDL.h>
#include <stdio.h>

#define GL_DEFINE(type, name) type p_##name;
GL_FUNCTIONS(GL_DEFINE)
#undef GL_DEFINE

int gl_load(void) {
    int ok = 1;

#define LOAD_FN(type, name)                                            \
    p_##name = (type)SDL_GL_GetProcAddress(#name);                     \
    if (p_##name == NULL) {                                            \
        fprintf(stderr, "gl: missing %s\n", #name);                    \
        ok = 0;                                                        \
    }
    GL_FUNCTIONS(LOAD_FN)
#undef LOAD_FN
    return ok;
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
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debug_message, NULL);
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

unsigned int gl_program(const char *vs, const char *fs, const char *name) {
    GLuint p = glCreateProgram(), v = shader(GL_VERTEX_SHADER, vs, name), f = shader(GL_FRAGMENT_SHADER, fs, name);
    GLint ok;

    glAttachShader(p, v);
    glAttachShader(p, f);
    glLinkProgram(p);
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    glDetachShader(p, v);
    glDetachShader(p, f);
    glDeleteShader(v);
    glDeleteShader(f);
    if (!ok) {
        char log[2048];

        glGetProgramInfoLog(p, sizeof(log), NULL, log);
        fprintf(stderr, "gl: program %s: %s\n", name, log);
        return 0;
    }
    return p;
}
