/* Window and presentation: an SDL3 window with an OpenGL 4.6 core context; every vblank the
 * renderer (glr.c) draws the last complete frame - the software GS frame underneath, the GL
 * strips over it - and it is shown scaled to the window.
 *
 *   HG_HEADLESS=1   a hidden window (tests): nothing shown, frames can still be dumped
 *   HG_DUMP=dir     write every 30th frame as dir/frame_NNNNN.ppm
 *   HG_MAXFRAMES=n  exit after n frames (profiling, tests) */
#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>

#include "glr.h"
#include "gs_local.h"

#define MAXW 1024
#define MAXH 1024
#define OUTW 640
#define OUTH 448

static SDL_Window *sWindow;
static SDL_GLContext sContext;
static int sHeadless = -1;
static int sGl;
static uint32_t sPixels[MAXW * MAXH];
static uint32_t sOut[OUTW * OUTH];
unsigned hg_video_frame;

static void video_init(void) {
    sHeadless = getenv("HG_HEADLESS") != NULL;
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        fprintf(stderr, "video: SDL_Init failed: %s\n", SDL_GetError());
        return;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    sWindow = SDL_CreateWindow("Haunting Ground", 1280, 896,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | (sHeadless ? SDL_WINDOW_HIDDEN : 0));
    sContext = sWindow ? SDL_GL_CreateContext(sWindow) : NULL;
    if (sContext == NULL) {
        fprintf(stderr, "video: no OpenGL 4.6 context: %s\n", SDL_GetError());
        return;
    }
    SDL_GL_SetSwapInterval(0);
    sGl = glr_init();
}

static void dump(void) {
    const char *dir = getenv("HG_DUMP");
    char path[512];
    FILE *f;
    int i;

    if (dir == NULL || hg_video_frame % 30 != 0 || !sGl) {
        return;
    }
    glr_read_pixels(sOut, OUTW, OUTH);
    snprintf(path, sizeof(path), "%s/frame_%05u.ppm", dir, hg_video_frame);
    if ((f = fopen(path, "wb")) == NULL) {
        return;
    }
    fprintf(f, "P6\n%d %d\n255\n", OUTW, OUTH);
    for (i = 0; i < OUTW * OUTH; i++) {
        unsigned char rgb[3] = {(unsigned char)sOut[i], (unsigned char)(sOut[i] >> 8), (unsigned char)(sOut[i] >> 16)};

        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
}

/* one vblank: show the last complete frame, handle window events */
void hg_frame(void) {
    SDL_Event e;
    int w = 0, h = 0, ww = 0, wh = 0;

    if (sHeadless < 0) {
        video_init();
    }
    gs_display(sPixels, MAXW, MAXH, &w, &h);
    if (getenv("HG_GSDEBUG") && hg_video_frame % 60 == 0) {
        extern unsigned gs_stat_prims, gs_stat_pixels, gs_stat_written;
        extern void vu1_stats(long *runs, long *pairs);
        long runs, pairs;

        vu1_stats(&runs, &pairs);
        fprintf(stderr, "frame %u: %u prim kicks, %u pixels, %u written; VU1 %ld runs, %ld instructions\n",
                hg_video_frame, gs_stat_prims, gs_stat_pixels, gs_stat_written, runs, pairs);
    }
    if (sGl) {
        if (!sHeadless) {
            SDL_GetWindowSizeInPixels(sWindow, &ww, &wh);
        }
        glr_present(sPixels, MAXW, w, h, ww, wh);
        dump();
        if (!sHeadless) {
            SDL_GL_SwapWindow(sWindow);
        }
    }
    hg_video_frame++;
    {
        static long maxFrames = -1;

        if (maxFrames < 0) {
            const char *m = getenv("HG_MAXFRAMES");

            maxFrames = m ? atol(m) : 0;
        }
        if (maxFrames > 0 && hg_video_frame >= (unsigned)maxFrames) {
            exit(0);
        }
    }
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            exit(0);
        }
    }
}
