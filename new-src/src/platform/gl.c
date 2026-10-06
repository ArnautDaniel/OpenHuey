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
