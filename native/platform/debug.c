/* Debug shortcuts for the PC build.
 *
 *   HG_ROOM=<room>   go into the game at room <room> (hex, e.g. 2A: where New Game starts)
 *                    instead of the title (the boot scene still runs: it loads the system
 *                    files)
 *   HG_NOPARTNER=1   no partner (Hewie) in the game scene */
#include <stdint.h>
#include <stdlib.h>

void hg_debug_next_scene(int32_t *mode, int32_t *param) {
    static int done;
    const char *room = getenv("HG_ROOM");

    if (done || room == NULL || *mode != 2) {
        return;
    }
    done = 1;
    *mode = 3;
    *param = (int32_t)strtol(room, NULL, 16);
}

/* HG_NOPARTNER=1: no partner (Hewie) in the game scene (his class isn't fully decompiled) */
int32_t hg_debug_no_partner(void) {
    return getenv("HG_NOPARTNER") != NULL;
}
