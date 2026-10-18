#include "platform.h"

#include "gl.h"

#include <stdio.h>
#include <string.h>

static SDL_Window *sWindow;
static SDL_GLContext sContext;

/* ---- a gamepad, played as the keyboard: its buttons hold the keys the scripts read for the pad's
 * (scripts/keys.fs: as hg's native/platform/input.c lays them out), the d-pad the arrows, the left
 * stick W A S D past half way - and its own reading for Fiona's walk ---- */
static SDL_Gamepad *sPad;
static bool sHeld[SDL_SCANCODE_COUNT];   /* the keyboard's and the pad's, together */
static bool sPadHeld[SDL_SCANCODE_COUNT];
static const struct { SDL_GamepadButton button; SDL_Scancode key; } kPadKeys[] = {
    {SDL_GAMEPAD_BUTTON_EAST, SDL_SCANCODE_C},          /* circle: the action */
    {SDL_GAMEPAD_BUTTON_WEST, SDL_SCANCODE_Z},          /* square */
    {SDL_GAMEPAD_BUTTON_NORTH, SDL_SCANCODE_V},         /* triangle */
    {SDL_GAMEPAD_BUTTON_SOUTH, SDL_SCANCODE_X},         /* cross: run */
    {SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, SDL_SCANCODE_Q},  /* L1 */
    {SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, SDL_SCANCODE_E}, /* R1 */
    {SDL_GAMEPAD_BUTTON_START, SDL_SCANCODE_RETURN},
    {SDL_GAMEPAD_BUTTON_BACK, SDL_SCANCODE_TAB},        /* select: the menu */
    {SDL_GAMEPAD_BUTTON_DPAD_UP, SDL_SCANCODE_UP},
    {SDL_GAMEPAD_BUTTON_DPAD_DOWN, SDL_SCANCODE_DOWN},
    {SDL_GAMEPAD_BUTTON_DPAD_LEFT, SDL_SCANCODE_LEFT},
    {SDL_GAMEPAD_BUTTON_DPAD_RIGHT, SDL_SCANCODE_RIGHT},
};
#define STICK_DEAD 0.25f   /* (the stick's dead zone) */
#define STICK_KEY 0.5f     /* (past this, it holds a key) */

static SDL_Scancode pad_key(SDL_GamepadButton b) {
    size_t i;

    for (i = 0; i < sizeof(kPadKeys) / sizeof(kPadKeys[0]); i++) {
        if (kPadKeys[i].button == b) {
            return kPadKeys[i].key;
        }
    }
    return SDL_SCANCODE_UNKNOWN;
}

static float stick_axis(SDL_GamepadAxis a) {
    float v = (float)SDL_GetGamepadAxis(sPad, a) / 32767.0f;

    return v > -STICK_DEAD && v < STICK_DEAD ? 0.0f : v < -1.0f ? -1.0f : v;
}

/* the pad's held keys this frame; a stick key newly held counts as pressed */
static void pad_poll(Input *in) {
    static const struct { SDL_Scancode key; int axis; float sign; } kStickKeys[] = {
        {SDL_SCANCODE_A, 0, -1.0f}, {SDL_SCANCODE_D, 0, 1.0f}, {SDL_SCANCODE_W, 1, -1.0f}, {SDL_SCANCODE_S, 1, 1.0f},
    };
    size_t i;

    memset(sPadHeld, 0, sizeof(sPadHeld));
    in->stick_x = in->stick_y = 0.0f;
    if (sPad == NULL) {
        return;
    }
    for (i = 0; i < sizeof(kPadKeys) / sizeof(kPadKeys[0]); i++) {
        sPadHeld[kPadKeys[i].key] = SDL_GetGamepadButton(sPad, kPadKeys[i].button);
    }
    in->stick_x = stick_axis(SDL_GAMEPAD_AXIS_LEFTX);
    in->stick_y = stick_axis(SDL_GAMEPAD_AXIS_LEFTY);
    for (i = 0; i < sizeof(kStickKeys) / sizeof(kStickKeys[0]); i++) {
        float v = (kStickKeys[i].axis == 0 ? in->stick_x : in->stick_y) * kStickKeys[i].sign;
        bool was = sHeld[kStickKeys[i].key];

        sPadHeld[kStickKeys[i].key] = v > STICK_KEY;
        if (sPadHeld[kStickKeys[i].key] && !was) {
            in->pressed[kStickKeys[i].key] = true;
        }
    }
}

void platform_rumble(int small_on, float large) {
    if (sPad != NULL) {   /* (held a little past the next tick: renewed each one) */
        SDL_RumbleGamepad(sPad, small_on ? 0xFFFF : 0, (Uint16)(large * 65535.0f), 100);
    }
}

int platform_open(const char *title, int w, int h, int hidden) {
    SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | (hidden ? SDL_WINDOW_HIDDEN : 0);

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        fprintf(stderr, "platform: SDL_Init: %s\n", SDL_GetError());
        return 0;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
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
        fprintf(stderr, "platform: no OpenGL 4.6 context: %s\n", SDL_GetError());
        return 0;
    }
    SDL_GL_SetSwapInterval(1);
    if (!gl_load()) {
        return 0;
    }
    gl_debug_output();
    fprintf(stderr, "platform: OpenGL %s (%s)\n", (const char *)glGetString(GL_VERSION), (const char *)glGetString(GL_RENDERER));
    return 1;
}

void platform_close(void) {
    if (sPad != NULL) {
        SDL_CloseGamepad(sPad);
    }
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

    /* (presses and mouse motion add up until a game tick takes them: a frame can come without a
     * tick, and a press then must not be lost) */
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
        case SDL_EVENT_GAMEPAD_ADDED:
            if (sPad == NULL) {
                sPad = SDL_OpenGamepad(e.gdevice.which);
            }
            break;
        case SDL_EVENT_GAMEPAD_REMOVED:
            if (sPad != NULL && SDL_GetGamepadID(sPad) == e.gdevice.which) {
                SDL_CloseGamepad(sPad);
                sPad = NULL;
            }
            break;
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            if (pad_key((SDL_GamepadButton)e.gbutton.button) != SDL_SCANCODE_UNKNOWN) {
                in->pressed[pad_key((SDL_GamepadButton)e.gbutton.button)] = true;
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
    {
        const bool *kb = SDL_GetKeyboardState(NULL);
        int k;

        pad_poll(in);
        for (k = 0; k < SDL_SCANCODE_COUNT; k++) {
            sHeld[k] = kb[k] || sPadHeld[k];
        }
        in->keys = sHeld;
    }
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
