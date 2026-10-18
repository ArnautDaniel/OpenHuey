/* The game library (library.h, docs/library-and-mods.md). The import copies an opened disc
 * image's files - DATA.CVM's into library/data/, the disc's own (SYSTEM.CNF, MODULES/) into
 * library/, the executable's tables into library/exe/ (the executable itself not kept), the kinds
 * with a converter converted - into library.new/ first, writes the manifest and only then takes
 * the place of library/: a failed or cancelled import leaves the old one (or none). Read back,
 * library/data/ is a folder of the game's files as the disc layer has always read them, its
 * "../" the library itself. */
#define _FILE_OFFSET_BITS 64
#include "library.h"
#include "disc.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <SDL3/SDL.h>

#include "../data/tex.h"
#include "../data/texbank.h"
#include "../data/flacenc.h"
#include "../data/msgtext.h"
#include "../data/musicsrc.h"
#include "../data/pac.h"
#include "../data/exetables.h"

#define MUSIC_FORMAT 1             /* "convert music 1": ADX to FLAC, LOOPSTART / LOOPEND / STOPAT (musicsrc.h) */

#define LIBRARY_FORMAT 1           /* the manifest's first line: "hga-library 1" */
#define EXE_NAME "SLUS_210.75"     /* Haunting Ground (USA) */
#define EXE_CRC 0xA295AF2Bu
#define EXE_SIZE 3650608L

/* ---- where things are: beside the executable (a portable game folder); if that can't be
 * written, the user's own folder for programs' data ---- */

static char sBase[1024], sLibrary[1100], sUser[1100], sMods[1100];

static void folders(void) {
    char probe[1100];

    if (sBase[0] != 0) {
        return;
    }
    if (getenv("HGA_HOME") != NULL && getenv("HGA_HOME")[0] != 0) {   /* (tests, tools: another folder) */
        snprintf(sBase, sizeof(sBase), "%s/", getenv("HGA_HOME"));
    } else {
        snprintf(sBase, sizeof(sBase), "%s", SDL_GetBasePath() != NULL ? SDL_GetBasePath() : "./");
    }
    snprintf(probe, sizeof(probe), "%suser", sBase);
    if (!SDL_CreateDirectory(probe)) {   /* (not writable here: an installed copy) */
        char *pref = SDL_GetPrefPath("hg2", "hga");

        if (pref != NULL) {
            snprintf(sBase, sizeof(sBase), "%s", pref);
            SDL_free(pref);
        }
    }
    snprintf(sLibrary, sizeof(sLibrary), "%slibrary/", sBase);
    snprintf(sUser, sizeof(sUser), "%suser/", sBase);
    snprintf(sMods, sizeof(sMods), "%smods/", sBase);
}

const char *library_mods_dir(void) {
    folders();
    SDL_CreateDirectory(sMods);
    return sMods;
}

const char *library_dir(void) {
    folders();
    return sLibrary;
}

/* ---- user/: made when first asked for - with the saves and settings hga kept in the user's
 * folder before there was a library (copied over, once) ---- */

static int copy_file(const char *from, const char *to) {
    FILE *a = fopen(from, "rb"), *b;
    char buf[65536];
    size_t n;
    int ok = 1;

    if (a == NULL) {
        return 0;
    }
    if ((b = fopen(to, "wb")) == NULL) {
        fclose(a);
        return 0;
    }
    while ((n = fread(buf, 1, sizeof(buf), a)) > 0) {
        if (fwrite(buf, 1, n, b) != n) {
            ok = 0;
            break;
        }
    }
    fclose(a);
    ok = fclose(b) == 0 && ok;
    return ok;
}

static SDL_EnumerationResult move_one(void *user, const char *dir, const char *name) {
    char from[1300], to[1300];
    SDL_PathInfo info;

    (void)user;
    snprintf(from, sizeof(from), "%s%s", dir, name);
    snprintf(to, sizeof(to), "%s%s", sUser, name);
    if (SDL_GetPathInfo(from, &info) && info.type == SDL_PATHTYPE_FILE && strcmp(name, "disc.txt") != 0 &&
        !SDL_GetPathInfo(to, NULL)) {
        copy_file(from, to);
    }
    return SDL_ENUM_CONTINUE;
}

const char *library_user_dir(void) {
    static int made;

    folders();
    if (!made) {
        char old[1100], mark[1200];

        made = 1;
        SDL_CreateDirectory(sUser);
        snprintf(mark, sizeof(mark), "%s.moved", sUser);
        if (!SDL_GetPathInfo(mark, NULL)) {   /* (once: the old folder's saves into user/) */
            char *pref = SDL_GetPrefPath("hg2", "new-src");
            FILE *f;

            if (pref != NULL) {
                snprintf(old, sizeof(old), "%s", pref);
                SDL_free(pref);
                if (strcmp(old, sUser) != 0) {
                    SDL_EnumerateDirectory(old, move_one, NULL);
                }
            }
            if ((f = fopen(mark, "w")) != NULL) {
                fprintf(f, "the saves of hga's earlier folder were copied here\n");
                fclose(f);
            }
        }
    }
    return sUser;
}

/* ---- the disc: Haunting Ground (USA), by its executable's checksum ---- */

static uint32_t crc32_of(uint32_t crc, const uint8_t *p, size_t n) {
    static uint32_t table[256];
    size_t i;

    if (table[1] == 0) {
        uint32_t k, c;
        int b;

        for (k = 0; k < 256; k++) {
            c = k;
            for (b = 0; b < 8; b++) {
                c = c & 1 ? 0xEDB88320u ^ (c >> 1) : c >> 1;
            }
            table[k] = c;
        }
    }
    crc = ~crc;
    for (i = 0; i < n; i++) {
        crc = table[(crc ^ p[i]) & 0xFF] ^ (crc >> 8);
    }
    return ~crc;
}

static int disc_is_ours(char *why, size_t n) {   /* (the disc opened) */
    size_t size;
    uint8_t *exe = disc_read_all("../" EXE_NAME, &size);
    uint32_t crc;

    if (exe == NULL) {
        snprintf(why, n, "this isn't the USA disc of Haunting Ground (no " EXE_NAME " on it: the European disc "
                         "is SLES_...)");
        return 0;
    }
    crc = crc32_of(0, exe, size);
    free(exe);
    if ((long)size != EXE_SIZE || crc != EXE_CRC) {
        snprintf(why, n, "this disc's " EXE_NAME " isn't the one hga knows (checksum %08X, not %08X)", crc, EXE_CRC);
        return 0;
    }
    return 1;
}

/* ---- the manifest: "hga-library 1", "disc <crc>", "convert <kind> <format>" each converter
 * used, then "file <size> <path>" each ---- */

static int library_ok(char *why, size_t n) {
    char path[2200], line[1200];
    FILE *f;
    int format = 0, files = 0;
    unsigned crc = 0;

    folders();
    snprintf(path, sizeof(path), "%smanifest.txt", sLibrary);
    if ((f = fopen(path, "r")) == NULL) {
        snprintf(why, n, "there is no library yet");
        return 0;
    }
    if (fgets(line, sizeof(line), f) == NULL || sscanf(line, "hga-library %d", &format) != 1 || format != LIBRARY_FORMAT) {
        snprintf(why, n, "the library's manifest isn't one this hga reads");
        fclose(f);
        return 0;
    }
    if (fgets(line, sizeof(line), f) == NULL || sscanf(line, "disc %x", &crc) != 1 || crc != EXE_CRC) {
        snprintf(why, n, "the library wasn't made from the USA disc");
        fclose(f);
        return 0;
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        long size;
        char name[1024];
        SDL_PathInfo info;

        line[strcspn(line, "\r\n")] = 0;
        if (sscanf(line, "file %ld %1023[^\n]", &size, name) != 2) {
            continue;
        }
        snprintf(path, sizeof(path), "%s%s", sLibrary, name);
        if (!SDL_GetPathInfo(path, &info) || info.type != SDL_PATHTYPE_FILE || (long)info.size != size) {
            snprintf(why, n, "the library's %s is missing or damaged", name);
            fclose(f);
            return 0;
        }
        files++;
    }
    fclose(f);
    if (files == 0) {
        snprintf(why, n, "the library's manifest lists no files");
        return 0;
    }
    return 1;
}

/* ---- removing a folder and all in it (a half import, the library replaced) ---- */

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

static void remove_tree(const char *dir) {   /* (dir ends with "/") */
    if (SDL_GetPathInfo(dir, NULL)) {
        SDL_EnumerateDirectory(dir, remove_one, NULL);
        SDL_RemovePath(dir);
    }
}

/* ---- the progress window (none headless: stderr) ---- */

typedef struct Progress {
    SDL_Window *win;
    SDL_Renderer *ren;
    int headless, cancelled, last_pct;
    long long done, total;
    Uint64 drawn;
} Progress;

static void progress_open(Progress *pr, int headless) {
    memset(pr, 0, sizeof(*pr));
    pr->headless = headless;
    pr->last_pct = -1;
    if (!headless && SDL_Init(SDL_INIT_VIDEO)) {
        SDL_CreateWindowAndRenderer("Haunting Ground - importing the disc", 640, 160, 0, &pr->win, &pr->ren);
    }
}

static void progress_draw(Progress *pr, const char *name) {
    int pct = pr->total > 0 ? (int)(pr->done * 100 / pr->total) : 0;
    SDL_Event e;
    SDL_FRect bar = {20, 90, 600, 28}, fill;
    char line[200];

    if (pr->win == NULL) {
        if (pct != pr->last_pct && pct % 5 == 0) {
            fprintf(stderr, "hga: importing the disc ... %d%%\n", pct);
        }
        pr->last_pct = pct;
        return;
    }
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT || e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            pr->cancelled = 1;
        }
    }
    if (SDL_GetTicks() - pr->drawn < 33 && pct != 100) {
        return;
    }
    pr->drawn = SDL_GetTicks();
    SDL_SetRenderDrawColor(pr->ren, 16, 14, 12, 255);
    SDL_RenderClear(pr->ren);
    SDL_SetRenderScale(pr->ren, 2, 2);
    SDL_SetRenderDrawColor(pr->ren, 216, 208, 200, 255);
    SDL_RenderDebugText(pr->ren, 10, 10, "Importing Haunting Ground");
    SDL_SetRenderScale(pr->ren, 1, 1);
    snprintf(line, sizeof(line), "%.70s", name);
    SDL_RenderDebugText(pr->ren, 20, 60, line);
    snprintf(line, sizeof(line), "%d%%  (%lld of %lld MB)", pct, pr->done >> 20, pr->total >> 20);
    SDL_RenderDebugText(pr->ren, 20, 130, line);
    SDL_SetRenderDrawColor(pr->ren, 70, 64, 58, 255);
    SDL_RenderRect(pr->ren, &bar);
    fill = bar;
    fill.w = bar.w * (float)pct / 100.0f;
    SDL_SetRenderDrawColor(pr->ren, 200, 150, 70, 255);
    SDL_RenderFillRect(pr->ren, &fill);
    SDL_RenderPresent(pr->ren);
}

static void progress_close(Progress *pr) {
    if (pr->ren != NULL) {
        SDL_DestroyRenderer(pr->ren);
    }
    if (pr->win != NULL) {
        SDL_DestroyWindow(pr->win);
    }
}

/* ---- the import ---- */

typedef struct Item {
    char path[512];   /* as the disc layer names it: "SYSTEM/X.TEX", "../SLUS_210.75" */
    long size;
} Item;

typedef struct Items {
    Item *e;
    int n, cap;
} Items;

static void items_add(Items *l, const char *path, long size) {
    if (l->n == l->cap) {
        l->cap = l->cap ? l->cap * 2 : 2048;
        l->e = realloc(l->e, sizeof(*l->e) * (size_t)l->cap);
    }
    snprintf(l->e[l->n].path, sizeof(l->e[l->n].path), "%s", path);
    l->e[l->n].size = size;
    l->n++;
}

/* the disc's own files to keep: the executable, SYSTEM.CNF, its modules (not DATA.CVM - its
 * files are listed apart - nor the padding files) */
static int keep_beside(const char *name) {
    return strcasecmp(name, "DATA.CVM") != 0 && strstr(name, "DUMMY") == NULL && strncasecmp(name, "cvm_tool", 8) != 0;
}

static SDL_EnumerationResult beside_one(void *user, const char *dir, const char *name) {   /* (a DATA.CVM's folder) */
    Items *l = user;
    char path[1300], rel[600];
    SDL_PathInfo info;
    const char *base = disc_folder();

    snprintf(path, sizeof(path), "%s%s", dir, name);
    if (!SDL_GetPathInfo(path, &info) || !keep_beside(name)) {
        return SDL_ENUM_CONTINUE;
    }
    snprintf(rel, sizeof(rel), "../%s", path + strlen(base) + 1);
    if (info.type == SDL_PATHTYPE_DIRECTORY) {
        if (strcasecmp(name, "MODULES") == 0) {
            strcat(path, "/");
            SDL_EnumerateDirectory(path, beside_one, user);
        }
    } else if (info.type == SDL_PATHTYPE_FILE && (strchr(rel + 3, '/') != NULL || strcasecmp(name, EXE_NAME) == 0 ||
                                                  strcasecmp(name, "SYSTEM.CNF") == 0)) {
        items_add(l, rel, (long)info.size);
    }
    return SDL_ENUM_CONTINUE;
}

static void list_items(Items *l) {
    char path[600];
    long size;
    int i;

    for (i = 0; disc_entry(i, path, sizeof(path), &size); i++) {
        if (strncmp(path, "../", 3) != 0 || keep_beside(path + 3)) {
            items_add(l, path, size);
        }
    }
    if (disc_entries() > 0 && disc_folder()[0] != 0) {   /* (a DATA.CVM: the disc's files from its folder) */
        int beside = 0;

        for (i = 0; i < l->n; i++) {
            beside |= strncmp(l->e[i].path, "../", 3) == 0;
        }
        if (!beside) {
            char dir[1100];

            snprintf(dir, sizeof(dir), "%s/", disc_folder());
            SDL_EnumerateDirectory(dir, beside_one, l);
        }
    }
}

/* where a disc file goes in the library: DATA.CVM's under data/, the disc's own at the top */
static void library_path(char *out, size_t n, const char *root, const char *path) {
    if (strncmp(path, "../", 3) == 0) {
        snprintf(out, n, "%s%s", root, path + 3);
    } else {
        snprintf(out, n, "%sdata/%s", root, path);
    }
}

static int make_parent(const char *path) {
    char dir[1300], *slash;

    snprintf(dir, sizeof(dir), "%s", path);
    slash = strrchr(dir, '/');
    if (slash == NULL) {
        return 1;
    }
    *slash = 0;
    return SDL_CreateDirectory(dir);
}

/* a kind of DATA.CVM's file the library holds converted, read whole: a .TEX bank (its PNGs,
 * texbank.h), an .ADX track (a FLAC) */
static int has_kind(const Item *it, const char *ext) {
    size_t len = strlen(it->path);

    return strncmp(it->path, "../", 3) != 0 && len > 4 && strcasecmp(it->path + len - 4, ext) == 0 && it->size > 0 &&
           it->size < (64L << 20);
}
static int is_messages(const Item *it) { return has_kind(it, ".BIN") && strncasecmp(it->path, "SUBSCR/MSG_", 11) == 0; }
/* (a room's .PAC is copied as it is, its text beside it as well) */
static int is_exe(const Item *it) { return strcasecmp(it->path, "../" EXE_NAME) == 0; }
static int is_converted(const Item *it) {
    return has_kind(it, ".TEX") || has_kind(it, ".ADX") || has_kind(it, ".PAC") || is_messages(it) || is_exe(it);
}

/* the executable: its tables (exetables.h) in exe/, each a file - the executable itself not kept */
static int convert_exe(const char *root, Items *made, const uint8_t *data, size_t size, char *why, size_t n) {
    char rel[200], to[1300];
    uint8_t *copy = malloc(size), *t;
    size_t tsize;
    Exe e;
    int i, ok = 1;
    FILE *f;

    if (copy == NULL) {
        return 0;
    }
    memcpy(copy, data, size);
    if (!exe_take(&e, copy, size)) {
        snprintf(why, n, "the disc's " EXE_NAME " isn't an executable hga reads");
        return 0;
    }
    for (i = 0; ok && exetables_name(i) != NULL; i++) {
        if ((t = exetables_extract(&e, exetables_name(i), &tsize)) == NULL) {
            snprintf(why, n, "the executable's table %s isn't where it should be", exetables_name(i));
            ok = 0;
            break;
        }
        snprintf(rel, sizeof(rel), "exe/%s.bin", exetables_name(i));
        snprintf(to, sizeof(to), "%s%s", root, rel);
        ok = make_parent(to) && (f = fopen(to, "wb")) != NULL;
        if (ok) {
            ok = fwrite(t, 1, tsize, f) == tsize;
            ok = fclose(f) == 0 && ok;
        }
        free(t);
        if (!ok) {
            snprintf(why, n, "%s can't be written", to);
        } else {
            items_add(made, rel, (long)tsize);
        }
    }
    exe_free(&e);
    return ok;
}

/* a message table as text (msgtext.h): "data/<path less its extension><suffix>" */
static int convert_messages(const Item *it, const char *root, Items *made, const uint8_t *table, size_t size,
                            const char *suffix, char *why, size_t n) {
    char rel[700], to[1300], about[700];
    char *text;
    size_t len;
    FILE *f;
    int ok;

    snprintf(about, sizeof(about), "%s%s", it->path, strcmp(suffix, ".txt") == 0 ? "" : ": the room's text (section 10)");
    if ((text = msgtext_write(table, size, about, &len)) == NULL) {
        return -1;   /* (not a table: copied as it is) */
    }
    snprintf(rel, sizeof(rel), "data/%.*s%s", (int)strlen(it->path) - 4, it->path, suffix);
    snprintf(to, sizeof(to), "%s%s", root, rel);
    ok = make_parent(to) && (f = fopen(to, "wb")) != NULL;
    if (ok) {
        ok = fwrite(text, 1, len, f) == len;
        ok = fclose(f) == 0 && ok;
    }
    free(text);
    if (!ok) {
        snprintf(why, n, "%s can't be written", to);
        return 0;
    }
    items_add(made, rel, (long)len);
    return 1;
}

/* an ADX as a FLAC beside where it was: the same samples, once through; its loop in tags */
static int convert_music(const Item *it, const char *root, Items *made, const uint8_t *adx, size_t size, char *why,
                         size_t n) {
    char rel[700], to[1300], start[48], end[48], stop[48];
    const char *tags[3] = {start, end, stop};
    int16_t *pcm;
    long frames, loop_start, loop_end, stop_at;
    int ch, rate, ok;
    uint8_t *flac;
    size_t flen;
    FILE *f;

    if (!musicsrc_adx_pcm(adx, size, &pcm, &frames, &ch, &rate, &loop_start, &loop_end, &stop_at)) {
        return -1;   /* (not an ADX: copied as it is) */
    }
    snprintf(start, sizeof(start), "LOOPSTART=%ld", loop_start);
    snprintf(end, sizeof(end), "LOOPEND=%ld", loop_end);
    snprintf(stop, sizeof(stop), "STOPAT=%ld", stop_at);
    flac = flacenc(pcm, frames, ch, rate, tags, 3, &flen);
    free(pcm);
    snprintf(rel, sizeof(rel), "data/%.*s.flac", (int)strlen(it->path) - 4, it->path);
    snprintf(to, sizeof(to), "%s%s", root, rel);
    ok = flac != NULL && make_parent(to) && (f = fopen(to, "wb")) != NULL;
    if (ok) {
        ok = fwrite(flac, 1, flen, f) == flen;
        ok = fclose(f) == 0 && ok;
    }
    free(flac);
    if (!ok) {
        snprintf(why, n, "%s can't be written", to);
        return 0;
    }
    items_add(made, rel, (long)flen);
    return 1;
}

typedef struct Made {   /* the files written, for the manifest (paths in the library) */
    Items *l;
    char dir[600];
} Made;

static void made_one(void *user, const char *name, long size) {
    Made *m = user;
    char rel[1200];

    snprintf(rel, sizeof(rel), "%s%s", m->dir, name);
    items_add(m->l, rel, size);
}

static int convert_bank(const Item *it, const char *root, Items *made, uint8_t *bank, char *why, size_t n) {
    char to[1300];
    Made m = {made, ""};
    size_t len = strlen(it->path) - 4;

    snprintf(m.dir, sizeof(m.dir), "data/%.*s.texbank/", (int)len, it->path);
    snprintf(to, sizeof(to), "%s%s", root, m.dir);
    if (!make_parent(to) || !texbank_convert(bank, (size_t)it->size, to, made_one, &m)) {
        snprintf(why, n, "%s can't be written", to);
        return 0;
    }
    return 1;
}

static int copy_item(const Item *it, const char *root, Items *made, Progress *pr, char *why, size_t n) {
    static uint8_t buf[1 << 20];
    char to[1300], rel[1300];
    DiscFile *f = disc_file_open(it->path);
    FILE *out = NULL;
    uint8_t *bank = NULL;
    size_t got, have = 0;

    if (f == NULL) {
        snprintf(why, n, "the disc's %s can't be read", it->path);
        return 0;
    }
    library_path(to, sizeof(to), root, it->path);
    if (is_converted(it)) {
        bank = malloc((size_t)it->size);
    }
    if (bank == NULL && (!make_parent(to) || (out = fopen(to, "wb")) == NULL)) {
        snprintf(why, n, "%s can't be written", to);
        disc_file_close(f);
        return 0;
    }
    while ((got = disc_file_read(f, buf, sizeof(buf))) > 0) {
        if (bank != NULL) {
            got = got < (size_t)it->size - have ? got : (size_t)it->size - have;
            memcpy(bank + have, buf, got);
            have += got;
        } else if (fwrite(buf, 1, got, out) != got) {
            snprintf(why, n, "%s can't be written (is the disc full?)", to);
            fclose(out);
            disc_file_close(f);
            return 0;
        }
        pr->done += (long long)got;
        progress_draw(pr, it->path);
        if (pr->cancelled) {
            snprintf(why, n, "the import was cancelled");
            if (out != NULL) {
                fclose(out);
            }
            free(bank);
            disc_file_close(f);
            return 0;
        }
    }
    disc_file_close(f);
    if (bank != NULL) {   /* (not one after all: copied as it is) */
        int ok = have == (size_t)it->size ? -1 : 0;

        if (ok && has_kind(it, ".TEX") && tex_count(bank, have) > 0) {
            ok = convert_bank(it, root, made, bank, why, n);
        } else if (ok && has_kind(it, ".ADX")) {
            ok = convert_music(it, root, made, bank, have, why, n);
        } else if (ok && is_exe(it)) {
            ok = convert_exe(root, made, bank, have, why, n);
        } else if (ok && is_messages(it)) {
            ok = convert_messages(it, root, made, bank, have, ".txt", why, n);
        } else if (ok && has_kind(it, ".PAC")) {
            Pac pac = {bank, have};
            size_t tsize;
            const uint8_t *table = pac_section(&pac, PAC_TEXT, &tsize);

            if (table != NULL && convert_messages(it, root, made, table, tsize, ".messages.txt", why, n) == 0) {
                ok = 0;
            }
        }
        if (ok == 0 && have != (size_t)it->size) {
            snprintf(why, n, "the disc's %s can't be read whole", it->path);
        }
        if (ok >= 0) {
            free(bank);
            return ok;
        }
        if (!make_parent(to) || (out = fopen(to, "wb")) == NULL || fwrite(bank, 1, have, out) != have) {
            snprintf(why, n, "%s can't be written", to);
            if (out != NULL) {
                fclose(out);
            }
            free(bank);
            return 0;
        }
        free(bank);
    }
    if (fclose(out) != 0) {
        snprintf(why, n, "%s can't be written", to);
        return 0;
    }
    library_path(rel, sizeof(rel), "", it->path);
    items_add(made, rel, it->size);
    return 1;
}

/* the manifest: "convert <kind> <format>" for each converter used */
static int write_manifest(const char *root, const Items *made) {
    char path[1300];
    FILE *f;
    int i;

    snprintf(path, sizeof(path), "%smanifest.txt", root);
    if ((f = fopen(path, "w")) == NULL) {
        return 0;
    }
    fprintf(f, "hga-library %d\n", LIBRARY_FORMAT);
    fprintf(f, "disc %08X\n", EXE_CRC);
    fprintf(f, "convert textures %d\n", TEXBANK_FORMAT);
    fprintf(f, "convert music %d\n", MUSIC_FORMAT);
    fprintf(f, "convert messages %d\n", MSGTEXT_FORMAT);
    fprintf(f, "convert exe %d\n", EXETABLES_FORMAT);
    for (i = 0; i < made->n; i++) {
        fprintf(f, "file %ld %s\n", made->e[i].size, made->e[i].path);
    }
    return fclose(f) == 0;
}

/* the disc opened imported; the reason in `why` if not */
static int import(int headless, char *why, size_t n) {
    char fresh[1200], old[1200];
    Items l = {0}, made = {0};
    Progress pr;
    int i, ok = 1;

    folders();
    if (!disc_is_image()) {
        snprintf(why, n, "only a disc image (.iso) or its DATA.CVM can be imported");
        return 0;
    }
    if (!disc_is_ours(why, n)) {
        return 0;
    }
    list_items(&l);
    snprintf(fresh, sizeof(fresh), "%slibrary.new/", sBase);
    snprintf(old, sizeof(old), "%slibrary.old/", sBase);
    remove_tree(fresh);
    SDL_CreateDirectory(fresh);
    progress_open(&pr, headless);
    for (i = 0; i < l.n; i++) {
        pr.total += l.e[i].size;
    }
    for (i = 0; i < l.n && ok; i++) {
        ok = copy_item(&l.e[i], fresh, &made, &pr, why, n);
    }
    progress_close(&pr);
    if (ok && !write_manifest(fresh, &made)) {
        snprintf(why, n, "the library's manifest can't be written");
        ok = 0;
    }
    free(l.e);
    free(made.e);
    if (!ok) {
        remove_tree(fresh);
        return 0;
    }
    remove_tree(old);   /* (the library replaced: the old one out of the way first, then gone) */
    if (SDL_GetPathInfo(sLibrary, NULL) && !SDL_RenamePath(sLibrary, old)) {
        snprintf(why, n, "the old library can't be replaced (%s)", SDL_GetError());
        return 0;
    }
    if (!SDL_RenamePath(fresh, sLibrary)) {
        snprintf(why, n, "the library can't be put in place (%s)", SDL_GetError());
        return 0;
    }
    remove_tree(old);
    fprintf(stderr, "hga: the disc is imported into %s\n", sLibrary);
    return 1;
}

static int open_library(void) {
    char data[1200];

    snprintf(data, sizeof(data), "%sdata", sLibrary);
    return disc_open(data);
}

int library_import_from(const char *disc, int headless) {
    char why[600];

    if (!disc_open(disc)) {
        fprintf(stderr, "hga: %s\n", disc_error());
        return 0;
    }
    if (!import(headless, why, sizeof(why))) {
        fprintf(stderr, "hga: the disc can't be imported: %s\n", why);
        return 0;
    }
    return 1;
}

int library_choose(const char *given, const char *fallback, int headless) {
    char why[600], msg[900];
    const char *say = NULL;

    if (given != NULL && given[0] != 0) {   /* (a disc or folder named: read as it is) */
        if (disc_open(given)) {
            return 1;
        }
        fprintf(stderr, "hga: %s\n", disc_error());
        return 0;
    }
    if (library_ok(why, sizeof(why))) {
        if (open_library()) {
            return 1;
        }
        snprintf(why, sizeof(why), "the library can't be read (%s)", disc_error());
    }
    if (SDL_GetPathInfo(sLibrary, NULL)) {   /* (there is one, but it won't do) */
        fprintf(stderr, "hga: %s\n", why);
        snprintf(msg, sizeof(msg), "The game's library can't be used: %s.\n\nPlease choose the disc image again "
                                   "to import it.", why);
        say = msg;
    }
    if (fallback != NULL && fallback[0] != 0 && disc_open(fallback)) {   /* (a developer's build) */
        return 1;
    }
    if (headless) {
        fprintf(stderr, "hga: no game data. Import your disc first: hga --import <Haunting Ground.iso>\n"
                        "(or name a disc image or extracted folder to read it as it is)\n");
        return 0;
    }
    for (;;) {
        if (!disc_pick(say != NULL ? say
                                   : "Choose your copy of Haunting Ground (USA): its disc image (.iso).\n\n"
                                     "Its files are imported into the game's folder once; the disc isn't "
                                     "needed after that.")) {
            return 0;
        }
        if (import(0, why, sizeof(why)) && open_library()) {
            return 1;
        }
        snprintf(msg, sizeof(msg), "The disc couldn't be imported: %s.", why);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Haunting Ground", msg, NULL);
        say = "Choose the disc image (.iso) of Haunting Ground (USA).";
    }
}
