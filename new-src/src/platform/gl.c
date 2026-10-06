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
