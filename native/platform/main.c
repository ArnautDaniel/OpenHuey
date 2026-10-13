/* Native entry point: what the PS2 main (0x0020D860) does, minus the PS2 runtime setup. */
#include <stdio.h>
#include <stdlib.h>

#include "disc.h"

typedef void (*Ctor)(void);

/* Static constructor table (MW __sinit_*), run by the PS2 runtime's __init (0x00100290). Its two
 * halves are separate data files; on x86 alignment padding between them reads as NULL. */
extern Ctor Sinit_Table[];
extern Ctor D_0046969C[];

extern char gGame[];
extern void Game_Run(void *game);
extern int hg_slus_load(void);   /* slus.c */

int main(int argc, char **argv) {
    Ctor *c;

    /* the game data: the argument, $HG_DATA, the disc chosen last time, the extracted folder next
       to the repository, or one the player picks */
    if (!hg_disc_choose(argc > 1 ? argv[1] : getenv("HG_DATA"), HG_DEFAULT_DATA)) {
        return 1;
    }
    /* the game's data from the disc's SLUS_210.75 (unless the build has it) */
    if (!hg_slus_load()) {
        return 1;
    }
    for (c = Sinit_Table; c < D_0046969C; c++) {
        if (*c != NULL) {
            (*c)();
        }
    }
    Game_Run(gGame);
    return 0;
}
