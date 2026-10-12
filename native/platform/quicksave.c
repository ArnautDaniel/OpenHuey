/* Quicksave and the room display, for testing on PC.
 *
 *   F5   quicksave: the game's own save (SceneGame_Save: room, entry, Fiona's place and heading,
 *        the characters, the progress, the sub screen) taken in play and written to
 *        <save dir>/quicksave.bin (HG_SAVE, else "save"). The resident save the game keeps
 *        for continuing (gGamePtr +0x70 headers, +0x190 save) is left as it was.
 *   F9   quickload: the scenes end as on a soft reset and the game scene starts from the
 *        quicksave as from Load Game. Works in play and on the title.
 *   HG_QUICKLOAD=1   start from the quicksave instead of the title (after the boot scene)
 *   HG_QUICKSAVE_AT=n / HG_QUICKLOAD_AT=n   (tests) F5 / F9 at video frame n
 *
 * In play the room (hex, as HG_ROOM takes it) is shown in the top right corner. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>

#include "glr.h"

#define AT(p, off, type) (*(type *)((uint8_t *)(p) + (off)))
#define VCALL(o, off, type) ((type)(AT(*(void ***)(o), off, void *)))

#define SAVE_OFF 0x190    /* the save in the resident data (gGamePtr) */
#define SAVE_SIZE 0x1900
#define HEADERS_OFF 0x70  /* 12 save headers, 0x18 each */
#define SG_PROGRESS 0x40  /* the game scene's Progress (gProgress) */

extern uint8_t *gGamePtr;
extern void *gProgress;
extern void SceneGame_Save(void *g, uint32_t slot);
extern int32_t SceneGame_InPlay(void *g);

static int sSaveAsked, sLoadAsked, sLoading;
static int sRoom = -1;
static char sToast[64];
static unsigned sToastUntil;
extern unsigned hg_video_frame;   /* video.c */

static const char *quick_path(void) {
    static char path[1024];
    const char *dir = getenv("HG_SAVE");

    snprintf(path, sizeof(path), "%s/quicksave.bin", dir ? dir : "save");
    return path;
}

static void toast(const char *msg) {
    snprintf(sToast, sizeof(sToast), "%s", msg);
    sToastUntil = hg_video_frame + 120;
    fprintf(stderr, "quicksave: %s\n", msg);
}

/* a key (video.c); 1 if it was ours */
int hg_quick_key(SDL_Scancode k) {
    if (k == SDL_SCANCODE_F5) {
        sSaveAsked = 1;
        return 1;
    }
    if (k == SDL_SCANCODE_F9) {
        sLoadAsked = 1;
        return 1;
    }
    return 0;
}

/* the game scene, when it is up and in play (not loading a room, in a menu or leaving a room) */
static void *game_in_play(int32_t mode) {
    void *g;

    if (mode != 3 || gProgress == NULL) {
        return NULL;
    }
    g = (uint8_t *)gProgress - SG_PROGRESS;
    if (AT(g, 0x1053450, int32_t) != 0 || AT(g, 0x1053454, int32_t) != 0 || AT(g, 0x1053458, void *) != NULL
        || !SceneGame_InPlay(g) || AT(g, 0x44, int32_t) != 0) {   /* a room loading (PTMF), a menu, leaving */
        return NULL;
    }
    return g;
}

static void quicksave(void *g) {
    static uint8_t keep[SAVE_SIZE], keepHeader[0x18];
    FILE *f;
    int ok = 0;

    memcpy(keep, gGamePtr + SAVE_OFF, SAVE_SIZE);
    memcpy(keepHeader, gGamePtr + HEADERS_OFF, 0x18);
    SceneGame_Save(g, 0);
    {
        char dir[1024];
        const char *d = getenv("HG_SAVE");

        snprintf(dir, sizeof(dir), "%s", d ? d : "save");
        SDL_CreateDirectory(dir);
    }
    if ((f = fopen(quick_path(), "wb")) != NULL) {
        ok = fwrite(gGamePtr + SAVE_OFF, 1, SAVE_SIZE, f) == SAVE_SIZE;
        ok = fclose(f) == 0 && ok;
    }
    memcpy(gGamePtr + SAVE_OFF, keep, SAVE_SIZE);
    memcpy(gGamePtr + HEADERS_OFF, keepHeader, 0x18);
    if (ok) {
        char msg[64];

        snprintf(msg, sizeof(msg), "QUICKSAVED (ROOM %02X)", sRoom);
        toast(msg);
    } else {
        toast("QUICKSAVE FAILED");
    }
}

static int quick_exists(void) {
    FILE *f = fopen(quick_path(), "rb");

    if (f == NULL) {
        return 0;
    }
    fclose(f);
    return 1;
}

/* the quicksave into the resident save */
static int quick_read(void) {
    FILE *f = fopen(quick_path(), "rb");
    int ok;

    if (f == NULL) {
        return 0;
    }
    ok = fread(gGamePtr + SAVE_OFF, 1, SAVE_SIZE, f) == SAVE_SIZE;
    fclose(f);
    return ok;
}

/* once a frame, before the scenes run (Game_StateMain): the room shown, F5 done; 1 when F9 asks
 * for the scenes to end (system.c then ends them and starts the game scene from the quicksave) */
int hg_quick_frame(int32_t mode) {
    void *g = game_in_play(mode);
    static long saveAt = -2, loadAt = -2;

    if (saveAt == -2) {
        saveAt = getenv("HG_QUICKSAVE_AT") ? atol(getenv("HG_QUICKSAVE_AT")) : -1;
        loadAt = getenv("HG_QUICKLOAD_AT") ? atol(getenv("HG_QUICKLOAD_AT")) : -1;
    }
    if (saveAt >= 0 && hg_video_frame >= (unsigned long)saveAt) {
        saveAt = -1;
        sSaveAsked = 1;
    }
    if (loadAt >= 0 && hg_video_frame >= (unsigned long)loadAt) {
        loadAt = -1;
        sLoadAsked = 1;
    }
    if (mode == 3 && gProgress != NULL) {
        sRoom = VCALL(gProgress, 0xC, int32_t (*)(void *))(gProgress);
    } else {
        sRoom = -1;
    }
    if (sSaveAsked) {
        sSaveAsked = 0;
        if (g != NULL) {
            quicksave(g);
        } else {
            toast(mode == 3 ? "CAN'T QUICKSAVE NOW" : "QUICKSAVE: NOT IN PLAY");
        }
    }
    if (sLoadAsked) {
        sLoadAsked = 0;
        if (sLoading) {
            return 0;
        }
        if (mode != 2 && mode != 3) {
            toast("QUICKLOAD: NOT YET");
        } else if (!quick_exists()) {
            toast("NO QUICKSAVE");
        } else {
            sLoading = 1;
            return 1;
        }
    }
    return 0;
}

/* the next scene is chosen (Game_StartNextScene): after F9, or with HG_QUICKLOAD at the title,
 * the game scene from the quicksave */
void hg_quick_next_scene(int32_t *mode, int32_t *param) {
    static int startDone;

    if (!startDone && *mode == 2 && getenv("HG_QUICKLOAD") != NULL) {
        startDone = 1;
        sLoading = 1;
    }
    if (!sLoading) {
        return;
    }
    sLoading = 0;
    if (!quick_read()) {
        toast("QUICKLOAD FAILED");
        return;
    }
    *mode = 3;
    *param = -1;   /* ENTRY_NEW: from the resident save */
    toast("QUICKLOADED");
}

/* this frame's debug lines (video.c, before glr_present) */
void hg_quick_draw(void) {
    static char room[32];
    static const char *lines[2];
    int n = 0;

    if (sRoom >= 0) {
        snprintf(room, sizeof(room), "ROOM %02X", sRoom);
        lines[n++] = room;
    }
    if (sToast[0] != 0 && hg_video_frame < sToastUntil) {
        lines[n++] = sToast;
    }
    if (n > 0) {
        glr_hud(lines, n);
    }
}
