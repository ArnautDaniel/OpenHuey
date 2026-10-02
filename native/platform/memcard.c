/* Memory card: the PC build keeps saves as host files. The game's card manager
 * (src/game/memcard.c) runs a state per request; its libmc states are replaced here.
 *
 * Slot 1 is the directory $HG_SAVE (default ./save), holding the game data file as
 * BASLUS-21075HG/BASLUS-21075HG like on the card; slot 2 is empty. Every state finishes in
 * one frame. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AT(p, off, type) (*(type *)((uint8_t *)(p) + (off)))

/* MemCard fields (see src/game/memcard.c) */
#define MC_STATUS(mc) AT(mc, 0x04, int32_t)
#define MC_ERROR(mc) AT(mc, 0x08, int32_t)
#define MC_PORT(mc) AT(mc, 0x20, int32_t)
#define MC_BUF(mc) AT(mc, 0x3C, void *)
#define MC_OFFSET(mc) AT(mc, 0x40, int32_t)
#define MC_SIZE(mc) AT(mc, 0x44, int32_t)

static void finish(void *mc, int32_t status) {
    MC_STATUS(mc) = status;
    memset((uint8_t *)mc + 0x14, 0, 12);   /* state: none */
}

static const char *save_path(void) {
    static char path[1024];
    const char *dir = getenv("HG_SAVE");

    snprintf(path, sizeof(path), "%s/BASLUS-21075HG/BASLUS-21075HG", dir ? dir : "save");
    return path;
}

/* memory card setup (system +0x390, PS2 0x00226570): MCMAN / MCSERV modules, sceMcInit */
void func_00226570(void *mc) { (void)mc; }

/* check (PS2 0x00226220): slot 1 has room, with or without game data; slot 2 is empty */
void func_00226220(void *mc) {
    FILE *f;

    MC_ERROR(mc) = 0;
    if (MC_PORT(mc) != 0) {
        finish(mc, 4);
        return;
    }
    f = fopen(save_path(), "rb");
    if (f != NULL) {
        fclose(f);
    }
    finish(mc, f != NULL ? 1 : 0);
}

/* read part of the game data file (PS2 0x00225C40) */
void func_00225C40(void *mc) {
    FILE *f = MC_PORT(mc) == 0 ? fopen(save_path(), "rb") : NULL;
    int ok = 0;

    if (f != NULL) {
        ok = fseek(f, MC_OFFSET(mc), SEEK_SET) == 0
             && fread(MC_BUF(mc), 1, (size_t)MC_SIZE(mc), f) == (size_t)MC_SIZE(mc);
        fclose(f);
    }
    finish(mc, ok ? 0 : 9);
}
