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

/* has the card been swapped since the check (PS2 0x00225770, sceMcGetInfo): never on PC */
void func_00225770(void *mc) {
    finish(mc, 0);
}

#include <sys/stat.h>
#include <sys/types.h>

#define MC_NAME(mc) AT(mc, 0x38, const char *)

/* the save's directory (slot 1), made if it isn't there yet */
static const char *save_dir(void) {
    static char path[1024];
    const char *dir = getenv("HG_SAVE");

    snprintf(path, sizeof(path), "%s", dir ? dir : "save");
    mkdir(path, 0777);
    snprintf(path, sizeof(path), "%s/BASLUS-21075HG", dir ? dir : "save");
    mkdir(path, 0777);
    return path;
}

/* write part of the game data file (PS2 0x00225950: open, seek, write, close) */
void func_00225950(void *mc) {
    FILE *f = NULL;
    int ok = 0;

    if (MC_PORT(mc) == 0) {
        save_dir();
        f = fopen(save_path(), "r+b");
        if (f == NULL) {
            f = fopen(save_path(), "w+b");
        }
    }
    if (f != NULL) {
        ok = fseek(f, MC_OFFSET(mc), SEEK_SET) == 0
             && fwrite(MC_BUF(mc), 1, (size_t)MC_SIZE(mc), f) == (size_t)MC_SIZE(mc);
        ok = fclose(f) == 0 && ok;
    }
    finish(mc, ok ? 0 : 9);
}

/* make a file of the save (PS2 0x00225F30: the directory, then /BASLUS-21075HG/<+0x38> written
 * whole from the buffer) */
void func_00225F30(void *mc) {
    char path[1200];
    FILE *f = NULL;
    int ok = 0;

    if (MC_PORT(mc) == 0 && MC_NAME(mc) != NULL) {
        snprintf(path, sizeof(path), "%s/%s", save_dir(), MC_NAME(mc));
        f = fopen(path, "wb");
    }
    if (f != NULL) {
        ok = fwrite(MC_BUF(mc), 1, (size_t)MC_SIZE(mc), f) == (size_t)MC_SIZE(mc);
        ok = fclose(f) == 0 && ok;
    }
    finish(mc, ok ? 0 : 6);
}

/* format the card (PS2 0x00225860, sceMcFormat): the host folder needs none, and saves are
 * never wiped - done */
void func_00225860(void *mc) {
    save_dir();
    finish(mc, 0);
}

/* sceMcEnd: nothing to release on the PC */
int sceMcEnd(void) { return 1; }
