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

typedef int s32;

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
#include "port.h"
#ifndef _WIN32
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
#endif

#ifdef _WIN32
/* Windows: on a crash, the exception, where it happened and the code addresses on the stack
 * (i686-w64-mingw32-addr2line -fe hg.exe <address> names them) */
static LONG WINAPI crash_report(EXCEPTION_POINTERS *e) {
    MEMORY_BASIC_INFORMATION mi;
    DWORD *sp = (DWORD *)e->ContextRecord->Esp;
    HMODULE self = GetModuleHandleA(NULL);
    int i, n = 0;

    fprintf(stderr, "\nhg crashed: exception 0x%08lX at 0x%08lX (data 0x%08lX)\ncode addresses on the stack:\n",
            e->ExceptionRecord->ExceptionCode, e->ContextRecord->Eip,
            e->ExceptionRecord->NumberParameters > 1 ? (unsigned long)e->ExceptionRecord->ExceptionInformation[1] : 0UL);
    for (i = 0; i < 2048 && n < 24; i++) {
        if (VirtualQuery(sp + i, &mi, sizeof(mi)) == 0 || !(mi.Protect & (PAGE_READWRITE | PAGE_READONLY))) {
            break;
        }
        if (VirtualQuery((void *)(uintptr_t)sp[i], &mi, sizeof(mi)) != 0 && mi.AllocationBase == (void *)self &&
            (mi.Protect & (PAGE_EXECUTE_READ | PAGE_EXECUTE | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) {
            fprintf(stderr, "  0x%08lX\n", sp[i]);
            n++;
        }
    }
    fprintf(stderr, "top of the stack:");
    for (i = 0; i < 8; i++) {
        char mod[260] = "?";
        HMODULE h = NULL;

        if (VirtualQuery(sp + i, &mi, sizeof(mi)) == 0) {
            break;
        }
        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                               (LPCSTR)(uintptr_t)sp[i], &h) && h != NULL) {
            GetModuleFileNameA(h, mod, sizeof(mod));
        } else {
            mod[0] = 0;
        }
        fprintf(stderr, "\n  [esp+%d] 0x%08lX %s", i * 4, sp[i], mod);
    }
    fputc('\n', stderr);
    fflush(stderr);
    return EXCEPTION_EXECUTE_HANDLER;
}

__attribute__((constructor)) static void crash_report_init(void) {
    SetUnhandledExceptionFilter(crash_report);
}
#endif

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

/* HG_EVLOG=1: the event scripts (include/common.h EVLOG); each line with a frame count */
static long sEvFrame;
void hg_evlog_frame(void) { sEvFrame++; }
int hg_evlog_on(void) {
    static int on = -1;

    if (on < 0) {
        on = getenv("HG_EVLOG") != NULL;
    }
    return on;
}

void hg_evlog(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "evlog %ld: ", sEvFrame);
    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
    va_end(ap);
}

/* a load queued (Loader_Queue) into memory that can't be a buffer - the program's code or the
 * first page - is a wrong pointer somewhere: say which file and who asked, and refuse it (the
 * PS2 would have overwritten whatever was there; here it is a crash a frame later) */
s32 hg_loader_check(const char *path, unsigned to) {
#ifdef _WIN32
    MEMORY_BASIC_INFORMATION mi;
    int code = VirtualQuery((void *)(uintptr_t)to, &mi, sizeof(mi)) != 0 &&
               (mi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY));
#else
    extern char __executable_start[], etext[];
    int code = to >= (unsigned)__executable_start && to < (unsigned)etext;
#endif

    if (to >= 0x1000 && !code) {
        return 1;
    }
    fprintf(stderr, "loader: \"%s\" queued into %s at 0x%08X, refused; asked from:\n", path,
            to < 0x1000 ? "nowhere" : "the code", to);
    hg_backtrace(0, 16);
    return 0;
}
