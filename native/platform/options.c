/* The PC options menu: ` (the key left of 1) opens it over the game, which gets no pad input
 * while it is open. Up / down pick a line, left / right (or Enter) change it, ` or Escape
 * close it. The settings are kept in <save dir>/options.cfg (HG_SAVE, else "save").
 *
 *   render scale  the scene drawn at 640 x 448 times 1..4, or fitted to the window
 *   display       a window or full screen */
#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>

#include "glr.h"

enum { OPT_SCALE, OPT_DISPLAY, OPT_CLOSE, OPT_COUNT };

static int sOpen, sSel, sLoaded;
static int sScale;        /* 0 fit the window, else 1..4 */
static int sFullscreen;

static const char *cfg_path(void) {
    static char path[1024];
    const char *dir = getenv("HG_SAVE");

    snprintf(path, sizeof(path), "%s/options.cfg", dir ? dir : "save");
    return path;
}

static void save(void) {
    FILE *f = fopen(cfg_path(), "w");

    if (f != NULL) {
        fprintf(f, "scale=%d\nfullscreen=%d\n", sScale, sFullscreen);
        fclose(f);
    }
}

static void apply(SDL_Window *w) {
    glr_set_scale(sScale);
    if (w != NULL) {
        SDL_SetWindowFullscreen(w, sFullscreen != 0);
    }
}

/* the saved settings, applied to window `w` (HG_SCALE wins over the saved scale) */
void hg_options_load(SDL_Window *w) {
    FILE *f = fopen(cfg_path(), "r");
    const char *env = getenv("HG_SCALE");
    char line[128];

    sLoaded = 1;
    if (f != NULL) {
        while (fgets(line, sizeof(line), f) != NULL) {
            sscanf(line, "scale=%d", &sScale);
            sscanf(line, "fullscreen=%d", &sFullscreen);
        }
        fclose(f);
    }
    if (env != NULL) {
        sScale = atoi(env);
    }
    sScale = sScale < 0 ? 0 : sScale > 4 ? 4 : sScale;
    apply(w);
}

int hg_options_open(void) {
    return sOpen;
}

static void change(SDL_Window *w, int d) {
    switch (sSel) {
    case OPT_SCALE:
        sScale = (sScale + d + 5) % 5;
        break;
    case OPT_DISPLAY:
        sFullscreen ^= 1;
        break;
    case OPT_CLOSE:
        sOpen = 0;
        return;
    }
    apply(w);
    save();
}

/* a key event; 1 if the menu took it */
int hg_options_key(SDL_Window *w, SDL_Scancode k) {
    if (k == SDL_SCANCODE_GRAVE) {
        sOpen ^= 1;
        sSel = 0;
        return 1;
    }
    if (!sOpen) {
        return 0;
    }
    switch (k) {
    case SDL_SCANCODE_ESCAPE:
        sOpen = 0;
        break;
    case SDL_SCANCODE_UP:
        sSel = (sSel + OPT_COUNT - 1) % OPT_COUNT;
        break;
    case SDL_SCANCODE_DOWN:
        sSel = (sSel + 1) % OPT_COUNT;
        break;
    case SDL_SCANCODE_LEFT:
        change(w, -1);
        break;
    case SDL_SCANCODE_RIGHT:
    case SDL_SCANCODE_RETURN:
        change(w, 1);
        break;
    default:
        break;
    }
    return 1;
}

/* this frame's menu for glr to draw (nothing when closed) */
void hg_options_draw(void) {
    static char lines[OPT_COUNT + 3][64];
    static const char *text[OPT_COUNT + 3];   /* glr keeps the pointer until it draws */
    int i;

    if (!sOpen) {
        return;
    }
    snprintf(lines[0], sizeof(lines[0]), "OPTIONS");
    if (sScale == 0) {
        snprintf(lines[1], sizeof(lines[1]), "RENDER SCALE: FIT WINDOW");
    } else {
        snprintf(lines[1], sizeof(lines[1]), "RENDER SCALE: %dX (%dX%d)", sScale, 640 * sScale, 448 * sScale);
    }
    snprintf(lines[2], sizeof(lines[2]), "DISPLAY: %s", sFullscreen ? "FULL SCREEN" : "WINDOW");
    snprintf(lines[3], sizeof(lines[3]), "CLOSE");
    snprintf(lines[4], sizeof(lines[4]), " ");
    snprintf(lines[5], sizeof(lines[5]), "UP/DOWN PICK  LEFT/RIGHT CHANGE  ` CLOSE");
    for (i = 0; i < OPT_COUNT + 3; i++) {
        text[i] = lines[i];
    }
    glr_menu(text, OPT_COUNT + 3, sSel + 1);
}
