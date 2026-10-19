/* The platform: one SDL3 window with an OpenGL 4.6 core context, and the input state for a frame. */
#ifndef PLATFORM_H
#define PLATFORM_H

#include <SDL3/SDL.h>

typedef struct Input {
    const bool *keys;            /* held now, by SDL scancode */
    bool pressed[SDL_SCANCODE_COUNT];   /* went down since the last game tick took them */
    float mouse_dx, mouse_dy;    /* mouse motion since then (pixels) */
    unsigned mouse_buttons;      /* held now: SDL_BUTTON_MASK(n) */
    char text[64];               /* text typed this frame (UTF-8) */
    int quit;                    /* the window was closed */
    float stick_x, stick_y;      /* a gamepad's left stick (-1..1, right / down; 0 within its dead zone) */
    int fullscreen_toggle;       /* F11 or Alt+Return this frame */
    bool typed[SDL_SCANCODE_COUNT];   /* the keys themselves pressed this frame, whatever they're bound to (the console) */
} Input;

/* open the window (hidden: nothing shown, for tests and screenshots); 0 on failure */
int platform_open(const char *title, int w, int h, int hidden);
void platform_close(void);
/* gather this frame's events into the input state */
void platform_poll(Input *in);
/* the drawable size in pixels */
void platform_size(int *w, int *h);
void platform_swap(void);
/* text input on (the console) or off */
void platform_text_input(int on);
/* the mouse held in the window, unseen, its motion relative (a first-person look) or not */
void platform_mouse_capture(int on);
/* the window fullscreen (the desktop's mode) or not */
void platform_fullscreen(int on);

/* ---- the controls: each of the game's actions is a key the scripts read (W, C, X ...: keys.fs),
 * held while either of the two keys bound to it is (the default: the key itself, and Space for X,
 * Backspace for C). Keys no action uses reach the scripts as they are; the actions' own keys only
 * through their bindings. The gamepad is not bound: the PS2's layout. ---- */
int platform_actions(void);                        /* how many */
SDL_Scancode platform_action_key(int i);           /* the key the scripts read for action i */
SDL_Scancode platform_binding(int i, int which);   /* its first (0) or second (1) key; 0 none */
/* bound: `key` its first, the first before its second (the key taken from any other action); a
 * second one cleared with key 0 and which 1 */
void platform_bind(int i, int which, SDL_Scancode key);
void platform_bindings_reset(void);
/* the next key pressed bound to action i, as its first (Escape: not); -1 none waited for */
void platform_bind_next(int i);
int platform_binding_wanted(void);

/* a gamepad's motors: the small one on or off, the large one's strength (0..1); none: nothing */
void platform_rumble(int small_on, float large);

#endif
