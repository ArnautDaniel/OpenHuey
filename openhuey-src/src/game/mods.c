/* Mods (mods.h, docs/library-and-mods.md).
 *
 *   mods/order.txt        on: <name> / off: <name>, a line each, in load order (the mods menu
 *                         writes it); a mod it doesn't name is on, after those it does
 *   mods/<folder>/mod.fs  mod: <name>  version: <major> <minor>   (or mods/<name>.zip: mod.fs at
 *                         its top, or in the one folder it holds)
 *                         requires: openhuey <major> <minor> (the mod API; also `hga`) / <mod> <major> <minor>
 *                         after: <mod> (loaded after it, if it's on)        \ comments
 */
#include "mods.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <SDL3/SDL.h>

#include "../core/files.h"
#include "../core/zip.h"

#define MODS 64
#define NEEDS 8

typedef struct Need {
    char name[64];
    int major, minor;
} Need;

typedef struct Mod {
    char name[64], dir[1024];   /* (a zip: the .zip's path) */
    Zip *zip;
    char prefix[256];           /* in the zip: where mod.fs is ("" or "folder/") */
    int major, minor;
    Need requires[NEEDS], after[NEEDS];
    int nrequires, nafter;
    int on, listed;     /* listed: its place in order.txt (+1), 0 not there */
    int named;          /* given on the command line (--mod): on for this run, not in order.txt */
    char why[160];      /* why it is off */
} Mod;

static Mod sMods[MODS];
static int sN;
static char sFolder[1024];   /* mods/ (empty: none - the tests) */

/* the mods menu's list (the title's MODS): the folder's mods in the order order.txt will have,
 * each on or off as the player set it - applied when OpenHuey next starts */
typedef struct MenuMod {
    char name[64], why[160];
    int on, was_on, ok;   /* ok: it loaded this run (was_on and nothing turned it off) */
} MenuMod;
static MenuMod sMenu[MODS];
static int sNMenu = -1;

/* the engine's own name in a manifest (`requires: openhuey 1 2`; `hga`, its name before, too) */
static int is_engine(const char *s) { return strcasecmp(s, "openhuey") == 0 || strcasecmp(s, "hga") == 0; }

static int good_name(const char *s) {   /* letters, digits, '-' (a vocabulary and a folder name) */
    if (*s == 0) {
        return 0;
    }
    for (; *s != 0; s++) {
        if (!isalnum((unsigned char)*s) && *s != '-') {
            return 0;
        }
    }
    return 1;
}

/* the words of a line, comments (\ to its end, ( ... )) left out */
static int words(char *line, char **w, int max) {
    int n = 0, paren = 0;
    char *save = NULL, *t;

    for (t = strtok_r(line, " \t\r\n", &save); t != NULL && n < max; t = strtok_r(NULL, " \t\r\n", &save)) {
        if (paren) {
            paren = strchr(t, ')') == NULL;
        } else if (strcmp(t, "\\") == 0) {
            break;
        } else if (strcmp(t, "(") == 0) {
            paren = 1;
        } else {
            w[n++] = t;
        }
    }
    return n;
}

static void need(Need *list, int *n, char **w, int nw) {
    if (*n < NEEDS && nw >= 2) {
        snprintf(list[*n].name, sizeof(list[0].name), "%s", w[1]);
        list[*n].major = nw > 2 ? atoi(w[2]) : 0;
        list[*n].minor = nw > 3 ? atoi(w[3]) : 0;
        (*n)++;
    }
}

/* a file of the mod's (`rel` from where its mod.fs is), whole, or NULL */
static char *mod_read(const Mod *m, const char *rel, size_t *size) {
    char path[1400];

    if (m->zip != NULL) {
        snprintf(path, sizeof(path), "%s%s", m->prefix, rel);
        return (char *)zip_read(m->zip, zip_find(m->zip, path), size);
    }
    snprintf(path, sizeof(path), "%s/%s", m->dir, rel);
    return SDL_LoadFile(path, size);   /* (NUL-terminated past its end; SDL_free) */
}

static void mod_free(const Mod *m, char *text) {
    if (m->zip != NULL) {
        free(text);
    } else {
        SDL_free(text);
    }
}

static int is_zip(const char *path) {
    size_t len = strlen(path);

    return len > 4 && strcasecmp(path + len - 4, ".zip") == 0;
}

/* where a zip's mod.fs is: at its top, else in a folder (the shortest such path) */
static int zip_prefix(const Zip *z, char *prefix, size_t n) {
    int i, best = -1;

    for (i = 0; i < zip_count(z); i++) {
        const char *name = zip_name(z, i), *slash = strrchr(name, '/');

        if (strcasecmp(slash != NULL ? slash + 1 : name, "mod.fs") == 0 &&
            (best < 0 || strlen(name) < strlen(zip_name(z, best)))) {
            best = i;
        }
    }
    if (best < 0) {
        return 0;
    }
    snprintf(prefix, n, "%.*s", (int)(strlen(zip_name(z, best)) - 6), zip_name(z, best));
    return 1;
}

static void add_mod(const char *dir) {   /* (a folder - no "/" at its end - or a .zip) */
    char *w[8], *text, *line, *save = NULL;
    const char *base = strrchr(dir, '/');
    Mod *m;
    size_t size;
    int i;

    if (sN == MODS) {
        return;
    }
    m = &sMods[sN];
    memset(m, 0, sizeof(*m));
    snprintf(m->dir, sizeof(m->dir), "%s", dir);
    snprintf(m->name, sizeof(m->name), "%s", base != NULL ? base + 1 : dir);
    if (is_zip(dir)) {
        m->name[strlen(m->name) - 4] = 0;
        if ((m->zip = zip_open(dir)) == NULL) {
            return;
        }
        if (!zip_prefix(m->zip, m->prefix, sizeof(m->prefix))) {
            fprintf(stderr, "mods: %s has no mod.fs - not a mod\n", dir);
            zip_close(m->zip);
            return;
        }
    }
    if ((text = mod_read(m, "mod.fs", &size)) == NULL) {
        zip_close(m->zip);
        return;   /* (not a mod: no manifest) */
    }
    m->on = 1;
    for (line = strtok_r(text, "\n", &save); line != NULL; line = strtok_r(NULL, "\n", &save)) {
        int n = words(line, w, 8);

        if (n == 0) {
            continue;
        }
        if (n >= 2 && strcmp(w[0], "mod:") == 0) {
            snprintf(m->name, sizeof(m->name), "%s", w[1]);
        } else if (n >= 2 && strcmp(w[0], "version:") == 0) {
            m->major = atoi(w[1]);
            m->minor = n > 2 ? atoi(w[2]) : 0;
        } else if (strcmp(w[0], "requires:") == 0) {
            need(m->requires, &m->nrequires, w, n);
        } else if (strcmp(w[0], "after:") == 0) {
            need(m->after, &m->nafter, w, n);
        } else {
            fprintf(stderr, "mods: %s's mod.fs: \"%s\" isn't a word mod.fs knows\n", dir, w[0]);
        }
    }
    mod_free(m, text);
    if (!good_name(m->name) || is_engine(m->name)) {
        fprintf(stderr, "mods: %s: its name \"%s\" can't be a mod's (letters, digits and -)\n", dir, m->name);
        zip_close(m->zip);
        return;
    }
    for (i = 0; i < sN; i++) {
        if (strcasecmp(sMods[i].name, m->name) == 0) {
            fprintf(stderr, "mods: %s and %s are both \"%s\" - the first is used\n", sMods[i].dir, dir, m->name);
            zip_close(m->zip);
            return;
        }
    }
    sN++;
}

static Mod *named(const char *name) {
    int i;

    for (i = 0; i < sN; i++) {
        if (strcasecmp(sMods[i].name, name) == 0) {
            return &sMods[i];
        }
    }
    return NULL;
}

static SDL_EnumerationResult folder_one(void *user, const char *dir, const char *name) {
    char path[1100];
    SDL_PathInfo info;

    snprintf(path, sizeof(path), "%s%s", dir, name);
    if (name[0] != '.' && SDL_GetPathInfo(path, &info) &&
        (info.type == SDL_PATHTYPE_DIRECTORY || (info.type == SDL_PATHTYPE_FILE && is_zip(name)))) {
        add_mod(path);
    }
    return SDL_ENUM_CONTINUE;
}

static int by_name(const void *a, const void *b) { return strcasecmp(((const Mod *)a)->name, ((const Mod *)b)->name); }
static int by_listed(const void *a, const void *b) {   /* (those order.txt names first, in its order) */
    const Mod *x = a, *y = b;

    if ((x->listed != 0) != (y->listed != 0)) {
        return x->listed != 0 ? -1 : 1;
    }
    return x->listed != y->listed ? x->listed - y->listed : strcasecmp(x->name, y->name);
}

static void read_order(const char *folder) {
    char path[1100], line[512], *w[4];
    FILE *f;
    int k = 0;

    snprintf(path, sizeof(path), "%sorder.txt", folder);
    if ((f = fopen(path, "r")) == NULL) {
        return;
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        int n = words(line, w, 4);
        Mod *m = n >= 2 ? named(w[1]) : NULL;

        if (m != NULL && strcmp(w[0], "on:") == 0) {
            m->listed = ++k;
        } else if (m != NULL && strcmp(w[0], "off:") == 0) {
            m->listed = ++k;
            m->on = 0;
            snprintf(m->why, sizeof(m->why), "turned off");
        }
    }
    fclose(f);
}

/* off: those whose needs aren't met (a mod off can turn off those needing it - until none) */
static void check_needs(void) {
    int changed = 1, i, k;

    for (i = 0; i < sN; i++) {   /* (OpenHuey's first: a mod off for it is off for those needing it) */
        Mod *m = &sMods[i];

        for (k = 0; k < m->nrequires && m->on; k++) {
            const Need *r = &m->requires[k];

            if (is_engine(r->name) && (r->major != MODS_API_MAJOR || r->minor > MODS_API_MINOR)) {
                snprintf(m->why, sizeof(m->why), "it needs OpenHuey's mod API %d.%d (this OpenHuey has %d.%d)", r->major, r->minor,
                         MODS_API_MAJOR, MODS_API_MINOR);
                m->on = 0;
            }
        }
    }
    while (changed) {
        changed = 0;
        for (i = 0; i < sN; i++) {
            Mod *m = &sMods[i];

            for (k = 0; k < m->nrequires && m->on; k++) {
                const Need need = m->requires[k], *r = &need;
                Mod *o;

                if (is_engine(r->name)) {
                    continue;
                }
                if ((o = named(r->name)) == NULL || !o->on) {
                    snprintf(m->why, sizeof(m->why), "it needs the mod %s, %s", r->name, o == NULL ? "not there" : "off");
                    m->on = 0;
                } else if (o->major < r->major || (o->major == r->major && o->minor < r->minor)) {
                    snprintf(m->why, sizeof(m->why), "it needs %s %d.%d or newer (there is %d.%d)", r->name, r->major,
                             r->minor, o->major, o->minor);
                    m->on = 0;
                }
                changed |= !m->on;
            }
        }
    }
}

/* each mod after those it names in after: (moved down until it is - a circle is left as it is) */
static void order_after(void) {
    int pass, moved = 1, i, k, j;

    for (pass = 0; moved && pass < sN * sN + 1; pass++) {
        moved = 0;
        for (i = 0; i < sN && !moved; i++) {
            for (k = 0; k < sMods[i].nafter && !moved; k++) {
                for (j = i + 1; j < sN; j++) {
                    if (strcasecmp(sMods[j].name, sMods[i].after[k].name) == 0) {
                        Mod m = sMods[i];

                        memmove(&sMods[i], &sMods[i + 1], sizeof(Mod) * (size_t)(j - i));
                        sMods[j] = m;
                        moved = 1;
                        break;
                    }
                }
            }
        }
    }
    if (moved) {
        fprintf(stderr, "mods: their after: lines go round in a circle; the order is left as it was\n");
    }
}

static void layer_files(void) {   /* (the mods on: their library/ over the data, in load order) */
    char dir[1100];
    int i;

    files_layers_clear();
    for (i = 0; i < sN; i++) {
        snprintf(dir, sizeof(dir), "%s/library", sMods[i].dir);
        if (sMods[i].on && sMods[i].zip != NULL) {
            files_layer_add_zip(sMods[i].zip, sMods[i].prefix);
        } else if (sMods[i].on && SDL_GetPathInfo(dir, NULL)) {
            files_layer_add(dir);
        }
    }
}

void mods_find(const char *folder, const char *const *names, int nnamed) {
    char dir[1100];
    int i, from_folder;

    for (i = 0; i < sN; i++) {   /* (found again: the tests) */
        zip_close(sMods[i].zip);
    }
    files_layers_clear();
    sN = 0;
    if (folder != NULL) {
        SDL_EnumerateDirectory(folder, folder_one, NULL);
        qsort(sMods, (size_t)sN, sizeof(Mod), by_name);
        read_order(folder);
        qsort(sMods, (size_t)sN, sizeof(Mod), by_listed);
    }
    from_folder = sN;
    snprintf(sFolder, sizeof(sFolder), "%s", folder != NULL ? folder : "");
    sNMenu = -1;
    for (i = 0; i < nnamed; i++) {   /* (--mod: on, after the folder's) */
        size_t len;

        snprintf(dir, sizeof(dir), "%s", names[i]);
        len = strlen(dir);
        while (len > 1 && dir[len - 1] == '/') {
            dir[--len] = 0;
        }
        add_mod(dir);
        if (sN > from_folder) {
            sMods[sN - 1].named = 1;
        }
        if (sN == from_folder) {
            fprintf(stderr, "mods: %s isn't a mod (no mod.fs in it)\n", names[i]);
        }
        from_folder = sN;
    }
    check_needs();
    order_after();
    for (i = 0; i < sN; i++) {
        if (!sMods[i].on) {
            fprintf(stderr, "mods: %s is off: %s\n", sMods[i].name, sMods[i].why);
        }
    }
    layer_files();
}

/* Forth's reader (forth.h): a zipped mod's scripts, "<the .zip>!/<path in it>" */
static char *zip_reader(void *ctx, const char *path, size_t *len) {
    int i;

    (void)ctx;
    for (i = 0; i < sN; i++) {
        size_t n = strlen(sMods[i].dir);

        if (sMods[i].zip != NULL && strncmp(path, sMods[i].dir, n) == 0 && strncmp(path + n, "!/", 2) == 0) {
            return (char *)zip_read(sMods[i].zip, zip_find(sMods[i].zip, path + n + 2), len);
        }
    }
    return NULL;
}

void mods_load_scripts(Forth *f, const char *game_scripts) {
    char path[1300], line[200], *text;
    size_t size;
    int i, any = 0;

    f->reader = zip_reader;

    for (i = 0; i < sN; i++) {
        Mod *m = &sMods[i];
        Vocab *v;

        if (!m->on) {
            continue;
        }
        snprintf(path, sizeof(path), "scripts/%s.fs", m->name);
        if ((text = mod_read(m, path, &size)) == NULL) {
            fprintf(stderr, "mods: %s %d.%d on (files only)\n", m->name, m->major, m->minor);
            continue;
        }
        mod_free(m, text);
        for (v = f->vocabs; v != NULL && strcasecmp(v->name, m->name) != 0; v = v->next) {
        }
        snprintf(line, sizeof(line), "%s/%s.fs", game_scripts, m->name);
        if (v != NULL || SDL_GetPathInfo(line, NULL)) {
            snprintf(m->why, sizeof(m->why), "its name is one of the game's vocabularies");
            fprintf(stderr, "mods: %s is off: %s\n", m->name, m->why);
            m->on = 0;
            layer_files();
            continue;
        }
        if (!any) {
            fprintf(stderr, "mods: a mod's scripts can do anything the game's can - load only mods you trust\n");
            any = 1;
        }
        if (m->zip != NULL) {
            snprintf(path, sizeof(path), "%s!/%sscripts", m->dir, m->prefix);
        } else {
            snprintf(path, sizeof(path), "%s/scripts", m->dir);
        }
        forth_add_root(f, path);
        snprintf(line, sizeof(line), "USING: %s ;", m->name);   /* (as any vocabulary: from its file) */
        if (forth_eval(f, line, strlen(line), m->name) != 0) {
            snprintf(m->why, sizeof(m->why), "its scripts failed to load (what it made before the error stays)");
            fprintf(stderr, "mods: %s is off: %s\n", m->name, m->why);
            m->on = 0;
            layer_files();
            continue;
        }
        fprintf(stderr, "mods: %s %d.%d on\n", m->name, m->major, m->minor);
    }
}

int mods_count(void) { return sN; }

const char *mods_name(int i, int *on, const char **why) {
    if (i < 0 || i >= sN) {
        return NULL;
    }
    *on = sMods[i].on;
    *why = sMods[i].why;
    return sMods[i].name;
}

static void p_mods(Forth *f, Word *w) {   /* ( -- ) the mods, in load order: on or off and why */
    int i;

    (void)w;
    if (sN == 0) {
        forth_printf(f, "no mods\n");
    }
    for (i = 0; i < sN; i++) {
        forth_printf(f, "%-24s %d.%-4d %s%s\n", sMods[i].name, sMods[i].major, sMods[i].minor,
                     sMods[i].on ? "on" : "off: ", sMods[i].on ? "" : sMods[i].why);
    }
}

static void p_mod_on(Forth *f, Word *w) {   /* ( addr len -- flag ) is the mod of that name on? */
    Cell n = forth_pop(f), a = forth_pop(f);
    char name[64];
    Mod *m;

    (void)w;
    snprintf(name, sizeof(name), "%.*s", (int)n, (const char *)a);
    m = named(name);
    forth_push(f, m != NULL && m->on ? -1 : 0);
}

/* ---- the mods menu ---- */

static void menu_init(void) {
    int i;

    if (sNMenu >= 0) {
        return;
    }
    sNMenu = 0;
    for (i = 0; i < sN; i++) {
        MenuMod *mm;
        int turned_off = strcmp(sMods[i].why, "turned off") == 0;

        if (sMods[i].named) {
            continue;
        }
        mm = &sMenu[sNMenu++];
        snprintf(mm->name, sizeof(mm->name), "%s", sMods[i].name);
        snprintf(mm->why, sizeof(mm->why), "%s", sMods[i].on || turned_off ? "" : sMods[i].why);
        mm->on = mm->was_on = sMods[i].on || !turned_off;   /* (off for a reason of its own: still asked for) */
        mm->ok = sMods[i].on;
    }
}

static int menu_index(Forth *f) {
    Cell i = forth_pop(f);

    menu_init();
    if (i < 0 || i >= sNMenu) {
        forth_error(f, "mods menu: no mod %ld (there are %d)", (long)i, sNMenu);
    }
    return (int)i;
}

static void p_menu_count(Forth *f, Word *w) {   /* ( -- n ) the folder's mods (the menu's rows) */
    (void)w;
    menu_init();
    forth_push(f, sNMenu);
}

static void p_menu_mod(Forth *f, Word *w) {   /* ( i -- addr len on? ) a row: its name, asked on */
    int i = menu_index(f);

    (void)w;
    forth_push(f, (Cell)sMenu[i].name);
    forth_push(f, (Cell)strlen(sMenu[i].name));
    forth_push(f, sMenu[i].on ? -1 : 0);
}

static void p_menu_why(Forth *f, Word *w) {   /* ( i -- addr len ) why it didn't load this run ("": it did, or turned off) */
    int i = menu_index(f);

    (void)w;
    forth_push(f, (Cell)sMenu[i].why);
    forth_push(f, (Cell)strlen(sMenu[i].why));
}

static void p_menu_switch(Forth *f, Word *w) {   /* ( i -- ) on <-> off */
    int i = menu_index(f);

    (void)w;
    sMenu[i].on = !sMenu[i].on;
}

static void p_menu_move(Forth *f, Word *w) {   /* ( i delta -- i' ) moved up (-1) or down (1) the load order */
    Cell delta = forth_pop(f);
    int i = menu_index(f), j = i + (delta < 0 ? -1 : 1);

    (void)w;
    if (j >= 0 && j < sNMenu) {
        MenuMod t = sMenu[i];

        sMenu[i] = sMenu[j];
        sMenu[j] = t;
        i = j;
    }
    forth_push(f, i);
}

static void p_menu_changed(Forth *f, Word *w) {   /* ( -- flag ) the menu differs from this run's */
    int i, k = 0, changed = 0;

    (void)w;
    menu_init();
    for (i = 0; i < sN && !changed; i++) {
        if (sMods[i].named) {
            continue;
        }
        changed = strcmp(sMods[i].name, sMenu[k].name) != 0 || sMenu[k].on != sMenu[k].was_on;
        k++;
    }
    forth_push(f, changed ? -1 : 0);
}

static void p_menu_save(Forth *f, Word *w) {   /* ( -- flag ) mods/order.txt written from the menu */
    char path[1100];
    FILE *o;
    int i, ok;

    (void)w;
    menu_init();
    snprintf(path, sizeof(path), "%sorder.txt", sFolder);
    if (sFolder[0] == 0 || (o = fopen(path, "w")) == NULL) {
        forth_push(f, 0);
        return;
    }
    fprintf(o, "\\ the mods' load order, each on or off (written by OpenHuey's mods menu; docs/library-and-mods.md)\n");
    for (i = 0; i < sNMenu; i++) {
        fprintf(o, "%s %s\n", sMenu[i].on ? "on:" : "off:", sMenu[i].name);
    }
    ok = fclose(o) == 0;
    forth_push(f, ok ? -1 : 0);
}

void bind_mods(Forth *f) {
    forth_prim(f, "mods-menu-count", p_menu_count);
    forth_prim(f, "mods-menu-mod", p_menu_mod);
    forth_prim(f, "mods-menu-why", p_menu_why);
    forth_prim(f, "mods-menu-switch", p_menu_switch);
    forth_prim(f, "mods-menu-move", p_menu_move);
    forth_prim(f, "mods-menu-changed?", p_menu_changed);
    forth_prim(f, "mods-menu-save", p_menu_save);
    forth_prim(f, ".mods", p_mods);
    forth_prim(f, "mod-on?", p_mod_on);
    forth_constant(f, "mod-api", MODS_API_MAJOR * 100 + MODS_API_MINOR);
}
