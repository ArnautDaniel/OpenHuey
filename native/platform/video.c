/* Window and presentation: SDL3 shows the GS's displayed frame buffer every vblank.
 *
 *   HG_HEADLESS=1   no window (tests)
 *   HG_DUMP=dir     write every 30th displayed frame as dir/frame_NNNNN.ppm */
#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>

#include "gs_local.h"

#define MAXW 1024
#define MAXH 1024

static SDL_Window *sWindow;
static SDL_Renderer *sRenderer;
static SDL_Texture *sTexture;
static int sHeadless = -1;
static uint32_t sPixels[MAXW * MAXH];
static unsigned sFrame;

static void video_init(void) {
    sHeadless = getenv("HG_HEADLESS") != NULL;
    if (sHeadless) {
        return;
    }
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        fprintf(stderr, "video: SDL_Init failed: %s\n", SDL_GetError());
        sHeadless = 1;
        return;
    }
    sWindow = SDL_CreateWindow("Haunting Ground", 1280, 896, SDL_WINDOW_RESIZABLE);
    sRenderer = sWindow ? SDL_CreateRenderer(sWindow, NULL) : NULL;
    sTexture = sRenderer ? SDL_CreateTexture(sRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, MAXW, MAXH) : NULL;
    if (sTexture == NULL) {
        fprintf(stderr, "video: no window: %s\n", SDL_GetError());
        sHeadless = 1;
        return;
    }
    SDL_SetTextureScaleMode(sTexture, SDL_SCALEMODE_LINEAR);
    SDL_SetRenderLogicalPresentation(sRenderer, 640, 448, SDL_LOGICAL_PRESENTATION_LETTERBOX);
}

static void dump(int w, int h) {
    const char *dir = getenv("HG_DUMP");
    char path[512];
    FILE *f;
    int x, y;

    if (dir == NULL || sFrame % 30 != 0) {
        return;
    }
    snprintf(path, sizeof(path), "%s/frame_%05u.ppm", dir, sFrame);
    if ((f = fopen(path, "wb")) == NULL) {
        return;
    }
    fprintf(f, "P6\n%d %d\n255\n", w, h);
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            uint32_t p = sPixels[y * MAXW + x];
            unsigned char rgb[3] = {(unsigned char)p, (unsigned char)(p >> 8), (unsigned char)(p >> 16)};

            fwrite(rgb, 1, 3, f);
        }
    }
    fclose(f);
}

/* one vblank: show the displayed frame, handle window events */
void hg_frame(void) {
    SDL_Event e;
    int w = 0, h = 0;

    if (sHeadless < 0) {
        video_init();
    }
    gs_display(sPixels, MAXW, MAXH, &w, &h);
    dump(w, h);
    sFrame++;
    if (sHeadless || w <= 0 || h <= 0) {
        return;
    }
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            exit(0);
        }
    }
    SDL_UpdateTexture(sTexture, NULL, sPixels, MAXW * 4);
    SDL_SetRenderDrawColor(sRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sRenderer);
    {
        SDL_FRect src = {0, 0, (float)w, (float)h};

        SDL_RenderTexture(sRenderer, sTexture, &src, NULL);
    }
    SDL_RenderPresent(sRenderer);
}
