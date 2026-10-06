#include "platform.h"

#include "gl.h"

#include <stdio.h>
#include <string.h>

static SDL_Window *sWindow;
static SDL_GLContext sContext;

int platform_open(const char *title, int w, int h, int hidden) {
    SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | (hidden ? SDL_WINDOW_HIDDEN : 0);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "platform: SDL_Init: %s\n", SDL_GetError());
        return 0;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    sWindow = SDL_CreateWindow(title, w, h, flags);
    if (sWindow == NULL) {
        fprintf(stderr, "platform: SDL_CreateWindow: %s\n", SDL_GetError());
        return 0;
    }
    sContext = SDL_GL_CreateContext(sWindow);
    if (sContext == NULL) {
        fprintf(stderr, "platform: no OpenGL 3.3 context: %s\n", SDL_GetError());
        return 0;
    }
    SDL_GL_SetSwapInterval(1);
    if (!gl_load()) {
        return 0;
    }
    return 1;
}

void platform_close(void) {
    if (sContext != NULL) {
        SDL_GL_DestroyContext(sContext);
    }
    if (sWindow != NULL) {
        SDL_DestroyWindow(sWindow);
    }
    SDL_Quit();
}

void platform_poll(Input *in) {
    SDL_Event e;

    memset(in->pressed, 0, sizeof(in->pressed));
    in->mouse_dx = in->mouse_dy = 0.0f;
    in->text[0] = 0;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_EVENT_QUIT:
            in->quit = 1;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (!e.key.repeat || e.key.scancode == SDL_SCANCODE_BACKSPACE) {
                in->pressed[e.key.scancode] = true;
            }
            break;
        case SDL_EVENT_MOUSE_MOTION:
            in->mouse_dx += e.motion.xrel;
            in->mouse_dy += e.motion.yrel;
            break;
        case SDL_EVENT_TEXT_INPUT:
            if (strlen(in->text) + strlen(e.text.text) < sizeof(in->text)) {
                strcat(in->text, e.text.text);
            }
            break;
        }
    }
    in->keys = SDL_GetKeyboardState(NULL);
    in->mouse_buttons = SDL_GetMouseState(NULL, NULL);
}

void platform_size(int *w, int *h) {
    SDL_GetWindowSizeInPixels(sWindow, w, h);
}

void platform_swap(void) {
    SDL_GL_SwapWindow(sWindow);
}

void platform_text_input(int on) {
    if (on) {
        SDL_StartTextInput(sWindow);
    } else {
        SDL_StopTextInput(sWindow);
    }
}
