/* Debug shortcuts for the PC build.
 *
 *   HG_ROOM=<room>   go into the game at room <room> (hex, e.g. 2A: where New Game starts)
 *                    instead of the title (the boot scene still runs: it loads the system
 *                    files)
 *   HG_NOPARTNER=1   no partner (Hewie) in the game scene */
#include <stdint.h>
#include <stdio.h>
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

/* HG_FREEPLAY: gameplay runs the world even while the opening keeps it stopped (Progress
 * flag 8), for looking at a room before its events work */
int32_t hg_debug_freeplay(void) {
    return getenv("HG_FREEPLAY") != NULL;
}

/* HG_NOCHARS: the characters don't update (to look at a room while their code is missing) */
int32_t hg_debug_nochars(void) {
    return getenv("HG_NOCHARS") != NULL;
}

/* an event script command whose C isn't written yet (func_002029B0): logged once, skipped */
void hg_debug_todo_opcode(int32_t op) {
    static uint8_t seen[256];

    if (!seen[op & 0xFF]) {
        seen[op & 0xFF] = 1;
        fprintf(stderr, "event: command 0x%02X not decompiled yet (skipped)\n", (int)op);
    }
}

/* an event script condition whose C isn't written yet (func_001FC760): logged once, false */
void hg_debug_todo_cond(int32_t op) {
    static uint8_t seen[256];

    if (!seen[op & 0xFF]) {
        seen[op & 0xFF] = 1;
        fprintf(stderr, "event: condition 0x%02X not decompiled yet (false)\n", (int)op);
    }
}

/* HG_EVLOG=1: each event script command as it runs (script object, offset, opcode, bytes) */
void hg_debug_evlog(const void *ev, const uint8_t *pc) {
    static int on = -1;

    if (on < 0) {
        on = getenv("HG_EVLOG") != NULL;
    }
    if (on) {
        fprintf(stderr, "ev %p: %02X %02X %02X %02X %02X\n", ev, pc[0], pc[1], pc[2], pc[3], pc[4]);
    }
}

/* HG_FLAGLOG=1: report when a watched progress flag changes (once per gameplay frame) */
void hg_debug_flaglog(int32_t flag, int32_t on) {
    static int32_t last[64], frame, init = -1;

    if (init < 0) {
        init = getenv("HG_FLAGLOG") != NULL;
        for (frame = 0; frame < 64; frame++) {
            last[frame] = -1;
        }
        frame = 0;
    }
    frame++;
    if (init && last[flag & 63] != on) {
        fprintf(stderr, "flag %d -> %d (gameplay frame %d)\n", (int)flag, (int)on, (int)frame);
        last[flag & 63] = on;
    }
}
