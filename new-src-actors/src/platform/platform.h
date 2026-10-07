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

#endif
