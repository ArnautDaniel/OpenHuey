/* Native entry point: what the PS2 main (0x0020D860) does, minus the PS2 runtime setup. */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*Ctor)(void);

/* Static constructor table (MW __sinit_*), run by the PS2 runtime's __init (0x00100290). Its two
 * halves are separate data files; on x86 alignment padding between them reads as NULL. */
extern Ctor D_00469460[];
extern Ctor D_0046969C[];

extern char gGame[];
extern const char *hg_data_dir;   /* crifs.c */
extern void Game_Run(void *game);
extern void gs_init(void);

int main(int argc, char **argv) {
    Ctor *c;

    /* the extracted DATA.CVM folder: argument, $HG_DATA, or the one next to the repository */
    if (argc > 1) {
        hg_data_dir = argv[1];
    } else if (getenv("HG_DATA") != NULL) {
        hg_data_dir = getenv("HG_DATA");
    }
    {
        char probe[1024];
        DIR *d;

        snprintf(probe, sizeof(probe), "%s/SYSTEM", hg_data_dir);
        d = opendir(probe);
        if (d == NULL) {
            fprintf(stderr,
                    "hg: no game data in \"%s\" (expected the extracted DATA.CVM folder, with SYSTEM/ in it).\n"
                    "Pass the folder as the first argument or set HG_DATA.\n",
                    hg_data_dir);
            return 1;
        }
        closedir(d);
    }
    gs_init();
    for (c = D_00469460; c < D_0046969C; c++) {
        if (*c != NULL) {
            (*c)();
        }
    }
    Game_Run(gGame);
    return 0;
}
