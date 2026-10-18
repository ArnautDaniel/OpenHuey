/* Mods (mods.h, docs/library-and-mods.md).
 *
 *   mods/order.txt        on: <name> / off: <name>, a line each, in load order (the mods menu
 *                         will write it); a mod it doesn't name is on, after those it does
 *   mods/<folder>/mod.fs  mod: <name>  version: <major> <minor>
 *                         requires: hga <major> <minor> (the mod API) / <mod> <major> <minor>
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

#define MODS 64
#define NEEDS 8

typedef struct Need {
    char name[64];
    int major, minor;
} Need;

typedef struct Mod {
    char name[64], dir[1024];
    int major, minor;
    Need requires[NEEDS], after[NEEDS];
    int nrequires, nafter;
    int on, listed;     /* listed: its place in order.txt (+1), 0 not there */
    char why[160];      /* why it is off */
} Mod;

static Mod sMods[MODS];
static int sN;

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

static void add_mod(const char *dir) {   /* (dir: no "/" at its end) */
    char path[1100], line[512], *w[8];
    const char *base = strrchr(dir, '/');
    Mod *m;
    FILE *f;
    int i;

    snprintf(path, sizeof(path), "%s/mod.fs", dir);
    if (sN == MODS || (f = fopen(path, "r")) == NULL) {
        return;   /* (not a mod: no manifest) */
    }
    m = &sMods[sN];
    memset(m, 0, sizeof(*m));
    snprintf(m->dir, sizeof(m->dir), "%s", dir);
    snprintf(m->name, sizeof(m->name), "%s", base != NULL ? base + 1 : dir);
    m->on = 1;
    while (fgets(line, sizeof(line), f) != NULL) {
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
            fprintf(stderr, "mods: %s: \"%s\" isn't a word mod.fs knows\n", path, w[0]);
        }
    }
    fclose(f);
    if (!good_name(m->name) || strcasecmp(m->name, "hga") == 0) {
        fprintf(stderr, "mods: %s: its name \"%s\" can't be a mod's (letters, digits and -)\n", path, m->name);
        return;
    }
    for (i = 0; i < sN; i++) {
        if (strcasecmp(sMods[i].name, m->name) == 0) {
            fprintf(stderr, "mods: %s and %s are both \"%s\" - the first is used\n", sMods[i].dir, dir, m->name);
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
    if (name[0] != '.' && SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_DIRECTORY) {
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

    for (i = 0; i < sN; i++) {   /* (hga's first: a mod off for it is off for those needing it) */
        Mod *m = &sMods[i];

        for (k = 0; k < m->nrequires && m->on; k++) {
            const Need *r = &m->requires[k];

            if (strcasecmp(r->name, "hga") == 0 && (r->major != MODS_API_MAJOR || r->minor > MODS_API_MINOR)) {
                snprintf(m->why, sizeof(m->why), "it needs hga's mod API %d.%d (this hga has %d.%d)", r->major, r->minor,
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

                if (strcasecmp(r->name, "hga") == 0) {
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
        if (sMods[i].on && SDL_GetPathInfo(dir, NULL)) {
            files_layer_add(dir);
        }
    }
}

void mods_find(const char *folder, const char *const *names, int nnamed) {
    char dir[1100];
    int i, from_folder;

    sN = 0;
    if (folder != NULL) {
        SDL_EnumerateDirectory(folder, folder_one, NULL);
        qsort(sMods, (size_t)sN, sizeof(Mod), by_name);
        read_order(folder);
        qsort(sMods, (size_t)sN, sizeof(Mod), by_listed);
    }
    from_folder = sN;
    for (i = 0; i < nnamed; i++) {   /* (--mod: on, after the folder's) */
        size_t len;

        snprintf(dir, sizeof(dir), "%s", names[i]);
        len = strlen(dir);
        while (len > 1 && dir[len - 1] == '/') {
            dir[--len] = 0;
        }
        add_mod(dir);
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

void mods_load_scripts(Forth *f, const char *game_scripts) {
    char path[1300], line[200];
    int i, any = 0;

    for (i = 0; i < sN; i++) {
        Mod *m = &sMods[i];
        Vocab *v;

        if (!m->on) {
            continue;
        }
        snprintf(path, sizeof(path), "%s/scripts/%s.fs", m->dir, m->name);
        if (!SDL_GetPathInfo(path, NULL)) {
            fprintf(stderr, "mods: %s %d.%d on (files only)\n", m->name, m->major, m->minor);
            continue;
        }
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
        snprintf(path, sizeof(path), "%s/scripts", m->dir);
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

void bind_mods(Forth *f) {
    forth_prim(f, ".mods", p_mods);
    forth_prim(f, "mod-on?", p_mod_on);
    forth_constant(f, "mod-api", MODS_API_MAJOR * 100 + MODS_API_MINOR);
}
