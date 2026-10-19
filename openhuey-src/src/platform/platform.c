#include "platform.h"

#include "gl.h"

#include <stdio.h>
#include <string.h>

static SDL_Window *sWindow;
static SDL_GLContext sContext;

/* ---- the controls (platform.h): the actions' keys, as the scripts read them (keys.fs) ---- */
static const SDL_Scancode kActions[] = {
    SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D,   /* the left stick */
    SDL_SCANCODE_C, SDL_SCANCODE_X, SDL_SCANCODE_Z, SDL_SCANCODE_V,   /* circle, cross, square, triangle */
    SDL_SCANCODE_Q, SDL_SCANCODE_E, SDL_SCANCODE_RETURN, SDL_SCANCODE_TAB,   /* L1, R1, start, select */
    SDL_SCANCODE_I, SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_J, SDL_SCANCODE_F,   /* the right stick, R3 */
};
#define ACTIONS (int)(sizeof(kActions) / sizeof(kActions[0]))
static SDL_Scancode sBinding[ACTIONS][2];
static int sBindNext = -1;   /* the action the next key is bound to (-1 none) */

void platform_bindings_reset(void) {
    int i;

    for (i = 0; i < ACTIONS; i++) {
        sBinding[i][0] = kActions[i];
        sBinding[i][1] = kActions[i] == SDL_SCANCODE_X   ? SDL_SCANCODE_SPACE
                         : kActions[i] == SDL_SCANCODE_C ? SDL_SCANCODE_BACKSPACE   /* (as hg: native/platform/input.c) */
                                                         : SDL_SCANCODE_UNKNOWN;
    }
}

int platform_actions(void) { return ACTIONS; }
SDL_Scancode platform_action_key(int i) { return i >= 0 && i < ACTIONS ? kActions[i] : SDL_SCANCODE_UNKNOWN; }
SDL_Scancode platform_binding(int i, int which) {
    return i >= 0 && i < ACTIONS && (which == 0 || which == 1) ? sBinding[i][which] : SDL_SCANCODE_UNKNOWN;
}

void platform_bind(int i, int which, SDL_Scancode key) {
    int k, w;

    if (i < 0 || i >= ACTIONS || which < 0 || which > 1 || key < 0 || key >= SDL_SCANCODE_COUNT) {
        return;
    }
    if (which == 1 && key == sBinding[i][0]) {   /* (its first as its second too: no) */
        return;
    }
    if (key == SDL_SCANCODE_UNKNOWN) {   /* (only a second one is cleared: an action keeps a key) */
        if (which == 1) {
            sBinding[i][1] = SDL_SCANCODE_UNKNOWN;
        }
        return;
    }
    for (k = 0; k < ACTIONS; k++) {   /* (a key does one thing) */
        for (w = 0; w < 2; w++) {
            if (sBinding[k][w] == key && !(k == i && w == which)) {
                sBinding[k][w] = SDL_SCANCODE_UNKNOWN;
            }
        }
        if (sBinding[k][0] == SDL_SCANCODE_UNKNOWN && k != i) {   /* (its first gone: its second moves up) */
            sBinding[k][0] = sBinding[k][1];
            sBinding[k][1] = SDL_SCANCODE_UNKNOWN;
        }
    }
    if (which == 0 && sBinding[i][0] != key) {
        sBinding[i][1] = sBinding[i][0];
    }
    sBinding[i][which] = key;
}

void platform_bind_next(int i) { sBindNext = i >= 0 && i < ACTIONS ? i : -1; }
int platform_binding_wanted(void) { return sBindNext; }

static int is_action_key(SDL_Scancode k) {
    int i;

    for (i = 0; i < ACTIONS; i++) {
        if (kActions[i] == k) {
            return 1;
        }
    }
    return 0;
}

/* a key pressed: the actions it is bound to pressed, or the key itself if no action is it */
static void key_pressed(Input *in, SDL_Scancode k) {
    int i;

    in->typed[k] = true;
    for (i = 0; i < ACTIONS; i++) {
        if (sBinding[i][0] == k || sBinding[i][1] == k) {
            in->pressed[kActions[i]] = true;
        }
    }
    if (!is_action_key(k)) {
        in->pressed[k] = true;
    }
}

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
    platform_bindings_reset();
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
    in->fullscreen_toggle = 0;
    memset(in->typed, 0, sizeof(in->typed));
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_EVENT_QUIT:
            in->quit = 1;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (sBindNext >= 0 && !e.key.repeat) {   /* (the key wanted for an action: taken, nothing else) */
                if (e.key.scancode != SDL_SCANCODE_ESCAPE) {
                    platform_bind(sBindNext, 0, e.key.scancode);
                }
                sBindNext = -1;
            } else if ((e.key.scancode == SDL_SCANCODE_RETURN && (e.key.mod & SDL_KMOD_ALT)) ||
                       e.key.scancode == SDL_SCANCODE_F11) {
                if (!e.key.repeat) {
                    in->fullscreen_toggle = 1;
                }
            } else if (!e.key.repeat || e.key.scancode == SDL_SCANCODE_BACKSPACE) {
                key_pressed(in, e.key.scancode);
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
            sHeld[k] = (kb[k] && !is_action_key((SDL_Scancode)k)) || sPadHeld[k];
        }
        for (k = 0; k < ACTIONS; k++) {   /* (an action held: either of its keys) */
            if ((sBinding[k][0] != SDL_SCANCODE_UNKNOWN && kb[sBinding[k][0]]) ||
                (sBinding[k][1] != SDL_SCANCODE_UNKNOWN && kb[sBinding[k][1]])) {
                sHeld[kActions[k]] = true;
            }
        }
        in->keys = sHeld;
    }
    in->mouse_buttons = SDL_GetMouseState(NULL, NULL);
}

void platform_mouse_capture(int on) {
    if (sWindow != NULL) {
        SDL_SetWindowRelativeMouseMode(sWindow, on != 0);
    }
}

void platform_fullscreen(int on) {
    if (sWindow != NULL) {
        SDL_SetWindowFullscreen(sWindow, on != 0);   /* (no mode set: the desktop's, borderless) */
    }
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
