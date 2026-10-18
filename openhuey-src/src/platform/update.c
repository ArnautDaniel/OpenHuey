/* The updater (update.h). The release (.github/workflows/hga-release.yml) carries version.txt:
 *     openhuey-version <build>                        (the build: "0.1.1-c6b7113")
 *     <archive name> <size in bytes>                  (one line each platform's archive)
 * A build other than this one's is newer (the release is rolling: whatever master built last).
 * Its archive goes into update/ beside the executable, is unpacked there, and each thing at its
 * top - the program, scripts/, docs/, the libraries - takes the place of the one beside the
 * executable, which is moved into update/old/ first (a running program or library can be moved,
 * on Windows too, though not removed): if one can't be put in place, the moved ones go back.
 * update/ is removed by the next start. */
#include "update.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>

#ifdef _WIN32
#define PLATFORM_ARCHIVE "-windows-x64.zip"
#define ARCHIVE_EXT ".zip"
#define PROGRAM "openhuey.exe"
#else
#define PLATFORM_ARCHIVE "-linux-x86_64.tar.gz"
#define ARCHIVE_EXT ".tar.gz"
#define PROGRAM "openhuey"
#endif
#define RELEASE_URL "https://github.com/ArnautDaniel/OpenHuey/releases/download/openhuey-latest/"
#define MOST_ENTRIES 64

enum { U_IDLE, U_CHECKING, U_DOWNLOADING, U_UNPACKING, U_INSTALLING, U_CURRENT, U_DONE, U_FAILED };

static SDL_AtomicInt sState;
/* what the page shows, each written by the update's thread before the state that shows it (the
 * page reads them every frame: never one being written) */
static char sFound[300];              /* the newest build: from "downloading" on, or "up to date" */
static char sWhy[600];                /* why it failed */
static char sDir[1024], sStage[1100]; /* the executable's folder; update/ in it */
static char sArchive[1600];           /* the download */
static long long sTotal;              /* its size (version.txt's) */
static int sVerbose;                  /* (--update: progress on stderr) */

static int enabled(void) {
#ifdef HGA_UPDATER
    return 1;
#else
    return getenv("OPENHUEY_UPDATE_URL") != NULL;
#endif
}

static const char *base_url(void) {
    const char *url = getenv("OPENHUEY_UPDATE_URL");

    return url != NULL && url[0] != 0 ? url : RELEASE_URL;
}

static void folders(void) {
    const char *dir = getenv("OPENHUEY_UPDATE_DIR");   /* (the tests: another folder updated) */

    if (dir != NULL && dir[0] != 0) {
        snprintf(sDir, sizeof(sDir), "%s/", dir);
    } else {
        snprintf(sDir, sizeof(sDir), "%s", SDL_GetBasePath() != NULL ? SDL_GetBasePath() : "./");
    }
    snprintf(sStage, sizeof(sStage), "%supdate/", sDir);
}

static void say(const char *fmt, const char *arg) {
    snprintf(sWhy, sizeof(sWhy), fmt, arg);
}

/* a program run and waited for: its exit status (Windows: no console window, so no status - what
 * it should have made is checked instead) */
static int run(const char *const *args) {
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_Process *p;
    int status = -1;

    SDL_SetPointerProperty(props, SDL_PROP_PROCESS_CREATE_ARGS_POINTER, (void *)args);
    SDL_SetNumberProperty(props, SDL_PROP_PROCESS_CREATE_STDOUT_NUMBER, SDL_PROCESS_STDIO_NULL);
#ifdef _WIN32
    SDL_SetBooleanProperty(props, SDL_PROP_PROCESS_CREATE_BACKGROUND_BOOLEAN, true);
#endif
    p = SDL_CreateProcessWithProperties(props);
    SDL_DestroyProperties(props);
    if (p == NULL) {
        return -1;
    }
    SDL_WaitProcess(p, true, &status);
    SDL_DestroyProcess(p);
    return status;
}

static long long file_size(const char *path) {
    SDL_PathInfo info;

    return SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE ? (long long)info.size : -1;
}

static SDL_EnumerationResult remove_one(void *user, const char *dir, const char *name) {
    char path[1300];
    SDL_PathInfo info;

    (void)user;
    snprintf(path, sizeof(path), "%s%s", dir, name);
    if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_DIRECTORY) {
        strcat(path, "/");
        SDL_EnumerateDirectory(path, remove_one, NULL);
    }
    SDL_RemovePath(path);
    return SDL_ENUM_CONTINUE;
}

static void remove_tree(const char *dir) {
    if (SDL_GetPathInfo(dir, NULL)) {
        SDL_EnumerateDirectory(dir, remove_one, NULL);
        SDL_RemovePath(dir);
    }
}

/* ---- the steps ---- */

static int download(const char *url, const char *to) {
    const char *args[] = {"curl", "-fsSL", "--retry", "2", "-o", to, url, NULL};

    SDL_RemovePath(to);
    return run(args) == 0 && file_size(to) > 0;
}

/* version.txt read: the newest build, its archive for this platform and that's size */
static int check(char *build, size_t nbuild, char *archive, size_t narchive) {
    char url[1400], path[1200], line[600];
    FILE *f;
    size_t ends = strlen(PLATFORM_ARCHIVE);

    snprintf(url, sizeof(url), "%sversion.txt", base_url());
    snprintf(path, sizeof(path), "%sversion.txt", sStage);
    if (!download(url, path)) {
        say("the newest version can't be found (is curl there, and the internet?)%s", "");
        return 0;
    }
    build[0] = archive[0] = 0;
    if ((f = fopen(path, "r")) == NULL) {
        say("update/version.txt can't be read%s", "");
        return 0;
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        char name[300];
        long long size;

        if (sscanf(line, "openhuey-version %299s", name) == 1) {
            snprintf(build, nbuild, "%s", name);
        } else if (sscanf(line, "%299s %lld", name, &size) == 2 && strlen(name) > ends &&
                   strcmp(name + strlen(name) - ends, PLATFORM_ARCHIVE) == 0) {
            snprintf(archive, narchive, "%s", name);
            sTotal = size;
        }
    }
    fclose(f);
    if (build[0] == 0 || archive[0] == 0) {
        say("the release has no build for this computer%s", "");
        return 0;
    }
    return 1;
}

/* the names at the archive's top (in update/<its folder>/) */
typedef struct Entries {
    char name[MOST_ENTRIES][256];
    int n;
} Entries;

static SDL_EnumerationResult list_one(void *user, const char *dir, const char *name) {
    Entries *e = user;

    (void)dir;
    if (e->n < MOST_ENTRIES) {
        snprintf(e->name[e->n++], sizeof(e->name[0]), "%s", name);
    }
    return SDL_ENUM_CONTINUE;
}

static int the_players(const char *name) {   /* (never replaced: theirs) */
    return strcmp(name, "library") == 0 || strcmp(name, "user") == 0 || strcmp(name, "mods") == 0 ||
           strcmp(name, "update") == 0;
}

/* each thing unpacked put in place of the executable's folder's, the old moved into update/old/;
 * all moved back if one can't be */
static int install(const char *unpacked) {
    static Entries e;
    char old[1200], from[2048], to[2048], kept[2048];
    int i, j, has_program = 0;

    e.n = 0;
    SDL_EnumerateDirectory(unpacked, list_one, &e);
    for (i = 0; i < e.n; i++) {
        has_program |= strcmp(e.name[i], PROGRAM) == 0;
    }
    if (!has_program) {
        say("the download holds no %s", PROGRAM);
        return 0;
    }
    snprintf(old, sizeof(old), "%sold/", sStage);
    remove_tree(old);
    SDL_CreateDirectory(old);
    for (i = 0; i < e.n; i++) {
        if (the_players(e.name[i])) {
            continue;
        }
        snprintf(from, sizeof(from), "%s%s", unpacked, e.name[i]);
        snprintf(to, sizeof(to), "%s%s", sDir, e.name[i]);
        snprintf(kept, sizeof(kept), "%s%s", old, e.name[i]);
        if ((SDL_GetPathInfo(to, NULL) && !SDL_RenamePath(to, kept)) || !SDL_RenamePath(from, to)) {
            say("OpenHuey's folder can't be changed (%s) - nothing was", SDL_GetError());
            if (SDL_GetPathInfo(kept, NULL)) {   /* (this one's old back) */
                SDL_RenamePath(kept, to);
            }
            for (j = i - 1; j >= 0; j--) {   /* (and those before) */
                if (the_players(e.name[j])) {
                    continue;
                }
                snprintf(to, sizeof(to), "%s%s", sDir, e.name[j]);
                snprintf(kept, sizeof(kept), "%s%s", old, e.name[j]);
                remove_tree(to);
                SDL_RemovePath(to);
                if (SDL_GetPathInfo(kept, NULL)) {
                    SDL_RenamePath(kept, to);
                }
            }
            return 0;
        }
    }
    remove_tree(old);   /* (Windows: the running program's stays, for the next start) */
    return 1;
}

static void step(int state) {
    SDL_SetAtomicInt(&sState, state);
    if (sVerbose && state == U_DOWNLOADING) {
        fprintf(stderr, "openhuey: downloading %s (%lld MB) ...\n", sFound, sTotal >> 20);
    } else if (sVerbose && state == U_UNPACKING) {
        fprintf(stderr, "openhuey: unpacking it ...\n");
    } else if (sVerbose && state == U_INSTALLING) {
        fprintf(stderr, "openhuey: putting it in place ...\n");
    }
}

/* the whole update: its end state */
static int update(void) {
    char build[300], archive[300], url[1700], unpacked[1500];
    const char *tar[] = {"tar", "-xzf", sArchive, "-C", sStage, NULL};

#ifdef _WIN32
    tar[1] = "-xf";   /* (Windows' tar: zip as well) */
#endif
    folders();
    step(U_CHECKING);
    remove_tree(sStage);
    if (!SDL_CreateDirectory(sStage)) {
        say("OpenHuey's folder can't be written (%s)", SDL_GetError());
        return U_FAILED;
    }
    if (!check(build, sizeof(build), archive, sizeof(archive))) {
        return U_FAILED;
    }
    snprintf(sFound, sizeof(sFound), "%s", build);
    if (strcmp(build, HGA_BUILD) == 0) {
        remove_tree(sStage);
        return U_CURRENT;
    }
    step(U_DOWNLOADING);
    snprintf(sArchive, sizeof(sArchive), "%s%s", sStage, archive);
    snprintf(url, sizeof(url), "%s%s", base_url(), archive);
    if (!download(url, sArchive) || file_size(sArchive) != sTotal) {
        say("%s couldn't be downloaded", archive);
        return U_FAILED;
    }
    step(U_UNPACKING);
    snprintf(unpacked, sizeof(unpacked), "%s%.*s/", sStage, (int)(strlen(archive) - strlen(ARCHIVE_EXT)), archive);   /* (its folder: its name) */
    run(tar);
    if (!SDL_GetPathInfo(unpacked, NULL)) {
        say("%s couldn't be unpacked (is tar there?)", archive);
        return U_FAILED;
    }
    step(U_INSTALLING);
    if (!install(unpacked)) {
        return U_FAILED;
    }
    SDL_RemovePath(sArchive);
    return U_DONE;
}

static int SDLCALL update_thread(void *unused) {
    int end;

    (void)unused;
    end = update();
    SDL_SetAtomicInt(&sState, end);
    return 0;
}

/* ---- `openhuey --update`, the start-up's tidying ---- */

int update_run(void) {
    int end;

    if (!enabled()) {
        fprintf(stderr, "openhuey: this is a developer's build (%s): it isn't updated - pull and build it again,\n"
                        "or download a release: https://github.com/ArnautDaniel/OpenHuey/releases\n", HGA_BUILD);
        return 1;
    }
    sVerbose = 1;
    fprintf(stderr, "openhuey: this is %s; looking for a newer one ...\n", HGA_BUILD);
    end = update();
    if (end == U_CURRENT) {
        fprintf(stderr, "openhuey: up to date (%s is the newest)\n", sFound);
    } else if (end == U_DONE) {
        fprintf(stderr, "openhuey: updated to %s - start it again to play that one\n", sFound);
    } else {
        fprintf(stderr, "openhuey: it can't be updated: %s\n", sWhy);
    }
    return end == U_FAILED;
}

void update_cleanup(void) {
    if (enabled()) {
        folders();
        remove_tree(sStage);
    }
}

/* ---- the words ---- */

static void p_updater(Forth *f, Word *w) {   /* ( -- flag ) this build updates itself (a release's) */
    (void)w;
    forth_push(f, enabled() ? -1 : 0);
}

static void p_update_start(Forth *f, Word *w) {   /* ( -- ) the update begun, its own thread (unless one is going) */
    SDL_Thread *t;
    int s = SDL_GetAtomicInt(&sState);

    (void)f;
    (void)w;
    if (!enabled() || (s != U_IDLE && s < U_CURRENT) || s == U_DONE) {   /* (updated: once is enough) */
        return;
    }
    sFound[0] = sWhy[0] = 0;   /* (no thread running now) */
    SDL_SetAtomicInt(&sState, U_CHECKING);
    if ((t = SDL_CreateThread(update_thread, "update", NULL)) == NULL) {
        say("it can't be started (%s)", SDL_GetError());
        SDL_SetAtomicInt(&sState, U_FAILED);
        return;
    }
    SDL_DetachThread(t);
}

/* ( -- n ) 0 not begun, 1 looking, 2 downloading, 3 unpacking, 4 putting in place, 5 up to date,
 * 6 updated, 7 failed */
static void p_update_state(Forth *f, Word *w) {
    (void)w;
    forth_push(f, SDL_GetAtomicInt(&sState));
}

static void p_update_progress(Forth *f, Word *w) {   /* ( -- done total ) the download's bytes */
    long long have = SDL_GetAtomicInt(&sState) == U_DOWNLOADING ? file_size(sArchive) : 0;

    (void)w;
    forth_push(f, (Cell)(have > 0 ? have : 0));
    forth_push(f, (Cell)sTotal);
}

static void p_update_text(Forth *f, Word *w) {   /* ( -- addr len ) the build found, or why it failed */
    int s = SDL_GetAtomicInt(&sState);
    const char *text = s == U_FAILED ? sWhy : s >= U_DOWNLOADING && s <= U_DONE ? sFound : "";

    (void)w;
    forth_push(f, (Cell)text);
    forth_push(f, (Cell)strlen(text));
}

static void p_update_restart(Forth *f, Word *w) {   /* ( -- flag ) the program updated started (then `bye`) */
    char program[1200];
    const char *args[] = {program, NULL};
    SDL_PropertiesID props;
    SDL_Process *p;

    (void)w;
    snprintf(program, sizeof(program), "%s%s", sDir, PROGRAM);
    props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_PROCESS_CREATE_ARGS_POINTER, (void *)args);
    SDL_SetBooleanProperty(props, SDL_PROP_PROCESS_CREATE_BACKGROUND_BOOLEAN, true);
    p = SDL_GetAtomicInt(&sState) == U_DONE ? SDL_CreateProcessWithProperties(props) : NULL;
    SDL_DestroyProperties(props);
    forth_push(f, p != NULL ? -1 : 0);
    if (p != NULL) {
        SDL_DestroyProcess(p);
    }
}

static void p_build(Forth *f, Word *w) {   /* ( -- addr len ) this build: its version and commit */
    static const char b[] = HGA_BUILD;

    (void)w;
    forth_push(f, (Cell)b);
    forth_push(f, (Cell)(sizeof(b) - 1));
}

void bind_update(Forth *f) {
    forth_prim(f, "updater?", p_updater);
    forth_prim(f, "update-start", p_update_start);
    forth_prim(f, "update-state", p_update_state);
    forth_prim(f, "update-progress", p_update_progress);
    forth_prim(f, "update-text", p_update_text);
    forth_prim(f, "update-restart", p_update_restart);
    forth_prim(f, "openhuey-build", p_build);
}
