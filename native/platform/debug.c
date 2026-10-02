/* Debug shortcuts for the PC build.
 *
 *   HG_ROOM=<room>   start in the game at room <room> (hex, e.g. 2A: where New Game starts),
 *                    skipping the boot checks and the title */
#include <stdint.h>
#include <stdlib.h>

void hg_debug_next_scene(int32_t *mode, int32_t *param) {
    static int done;
    const char *room = getenv("HG_ROOM");

    if (done || room == NULL || *mode != 1) {
        return;
    }
    done = 1;
    *mode = 3;
    *param = (int32_t)strtol(room, NULL, 16);
}
