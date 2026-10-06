/* Debug shortcuts for the PC build.
 *
 *   HG_ROOM=<room>   go into the game at room <room> (hex, e.g. 2A: where New Game starts)
 *                    instead of the title (the boot scene still runs: it loads the system
 *                    files); HG_ROOM=END: the ending (staff roll, results) instead
 *   HG_FASTBOOT=1    the boot scene skips its logos, the Capcom movie and the caution screen
 *                    (its loading steps still run)
 *   HG_NOPARTNER=1   no partner (Hewie) in the game scene
 *   HG_WATCHDOG=<s>  after <s> seconds print a backtrace and stop (endless loops)
 *   HG_EVSTUCK=1     stop with the script bytes when an event script steps onto a byte that
 *                    isn't a command (src/game/event_cmd.c EventCmd_Skip) */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hg_debug_next_scene(int32_t *mode, int32_t *param) {
    static int done;
    const char *room = getenv("HG_ROOM");

    if (done || room == NULL || *mode != 2) {
        return;
    }
    done = 1;
    if (strcmp(room, "END") == 0) {
        *mode = 5;
        return;
    }
    *mode = 3;
    *param = (int32_t)strtol(room, NULL, 16);
}

/* HG_FASTBOOT=1: boot step `fn` (a logo, the Capcom movie or the caution screen) is skipped */
int32_t hg_debug_skip_boot_step(const void *fn) {
    extern char SceneBoot_StepCri[], SceneBoot_StepDolby[], SceneBoot_StepCapcom[], SceneBoot_StepCaution[];
    const char *v = getenv("HG_FASTBOOT");

    if (v == NULL || v[0] == 0 || v[0] == '0') {
        return 0;
    }
    return fn == SceneBoot_StepCri || fn == SceneBoot_StepDolby || fn == SceneBoot_StepCapcom || fn == SceneBoot_StepCaution;
}

/* HG_NOPARTNER=1: no partner (Hewie) in the game scene (his class isn't fully decompiled) */
int32_t hg_debug_no_partner(void) {
    const char *v = getenv("HG_NOPARTNER");

    return v != NULL && v[0] != 0 && v[0] != '0';
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

/* an event script command whose C isn't written yet (EventCmd_Run): logged once, skipped */
void hg_debug_todo_opcode(int32_t op) {
    static uint8_t seen[256];

    if (!seen[op & 0xFF]) {
        seen[op & 0xFF] = 1;
        fprintf(stderr, "event: command 0x%02X not decompiled yet (skipped)\n", (int)op);
    }
}

/* an event script condition whose C isn't written yet (EventCond_Eval): logged once, false */
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
        fprintf(stderr, "ev %p pc %p: %02X %02X %02X %02X %02X\n", ev, (const void *)pc, pc[0], pc[1], pc[2], pc[3], pc[4]);
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

/* HG_WATCHDOG=<seconds>: after that long print where the game is (a backtrace) and stop - for
 * finding endless loops */
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>

static void watchdog_fire(int sig) {
    void *frames[32];
    int n = backtrace(frames, 32);

    (void)sig;
    fprintf(stderr, "\nwatchdog: still running, at:\n");
    backtrace_symbols_fd(frames, n, 2);
    _exit(3);
}

__attribute__((constructor)) static void watchdog_init(void) {
    const char *s = getenv("HG_WATCHDOG");

    if (s != NULL && atoi(s) > 0) {
        signal(SIGALRM, watchdog_fire);
        alarm((unsigned)atoi(s));
    }
}

/* HG_ROOMLOG=1: room changes and character placements (include/common.h ROOMLOG) */
#include <stdarg.h>
int hg_roomlog_on(void) {
    static int on = -1;

    if (on < 0) {
        on = getenv("HG_ROOMLOG") != NULL;
    }
    return on;
}

void hg_roomlog(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fputs("roomlog: ", stderr);
    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
    va_end(ap);
}
