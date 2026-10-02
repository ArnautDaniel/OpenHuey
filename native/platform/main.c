/* Native entry point: what the PS2 main (0x0020D860) does, minus the PS2 runtime setup. */
#include <stdio.h>

typedef void (*Ctor)(void);

/* Static constructor table (MW __sinit_*), run by the PS2 runtime's __init (0x00100290). Its two
 * halves are separate data files; on x86 alignment padding between them reads as NULL. */
extern Ctor D_00469460[];
extern Ctor D_0046969C[];

extern char gGame[];
extern void Game_Run(void *game);

int main(int argc, char **argv) {
    Ctor *c;

    (void)argc;
    (void)argv;
    for (c = D_00469460; c < D_0046969C; c++) {
        if (*c != NULL) {
            (*c)();
        }
    }
    Game_Run(gGame);
    return 0;
}
