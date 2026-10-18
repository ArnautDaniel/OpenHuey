/* The game data (disc.h). Three forms:
 *   - the disc image: ISO 9660, DATA.CVM in its top folder (with MODULES/ and SLUS_210.75 beside it)
 *   - DATA.CVM itself: a 0x1800-byte "CVMH" header, then an ISO 9660 image of the game's folders
 *   - the folder DATA.CVM was extracted to (SYSTEM/ etc. in it)
 * An image's files are listed once; each open file then has its own handle on the image (the
 * sound thread's streams don't share a position with loads). The image is 4.5 GB: 64-bit offsets.
 * (After hg's native/platform/disc.c.) */
#define _FILE_OFFSET_BITS 64
#include "disc.h"
#include "library.h"

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <SDL3/SDL.h>

#define SECTOR 2048
#define CVM_HEADER 0x1800

#ifdef _WIN32
#define fseek64 _fseeki64
#define ftell64 _ftelli64
#else
#define fseek64 fseeko
#define ftell64 ftello
#endif

struct DiscFile {
    FILE *fp;
    int64_t base;   /* where the file starts in fp */
    long size, pos;
};

typedef struct Entry {
    char *path;     /* "FOLDER/NAME" */
    int64_t at;
    long size;
} Entry;

typedef struct Listing {
    Entry *e;
    int n, cap;
} Listing;

static char sImage[1024];     /* the .iso / DATA.CVM; "" for a folder */
static char sFolder[1024];    /* the extracted folder; for a DATA.CVM, the folder it is in */
static Listing sInside;       /* DATA.CVM's files */
static Listing sBeside;       /* the disc's own files, beside DATA.CVM (an .iso only) */
static char sError[256];

const char *disc_error(void) { return sError; }

static void norm(char *s) {   /* "a\b" -> "a/b" */
    for (; *s != 0; s++) {
        *s = *s == '\\' ? '/' : *s;
    }
}

static int read_at(FILE *fp, int64_t at, void *buf, size_t n) {
    return fseek64(fp, at, SEEK_SET) == 0 && fread(buf, 1, n, fp) == n;
}

static uint32_t le32(const uint8_t *p) { return p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24; }

static void listing_add(Listing *l, const char *path, int64_t at, long size) {
    if (l->n == l->cap) {
        l->cap = l->cap ? l->cap * 2 : 1024;
        l->e = realloc(l->e, sizeof(*l->e) * (size_t)l->cap);
    }
    l->e[l->n].path = strdup(path);
    norm(l->e[l->n].path);
    l->e[l->n].at = at;
    l->e[l->n].size = size;
    l->n++;
}

static void listing_clear(Listing *l) {
    int i;

    for (i = 0; i < l->n; i++) {
        free(l->e[i].path);
    }
    l->n = 0;
}

static const Entry *listing_find(const Listing *l, const char *path) {
    int i;

    for (i = 0; i < l->n; i++) {
        if (strcasecmp(l->e[i].path, path) == 0) {
            return &l->e[i];
        }
    }
    return NULL;
}

/* list the ISO 9660 folder at sector `lba` (`size` bytes) of the image at `base` into `l` */
static int walk(FILE *fp, int64_t base, uint32_t lba, uint32_t size, const char *prefix, int depth, Listing *l) {
    uint8_t *d;
    uint32_t i = 0;

    if (depth > 8 || size > (16u << 20) || (d = malloc(size)) == NULL) {
        return 0;
    }
    if (!read_at(fp, base + (int64_t)lba * SECTOR, d, size)) {
        free(d);
        return 0;
    }
    while (i < size) {
        const uint8_t *r = d + i;
        char name[256], path[1024];
        int nl;

        if (r[0] == 0) {   /* records don't cross sectors: the rest of this one is padding */
            i = (i / SECTOR + 1) * SECTOR;
            continue;
        }
        if (r[0] < 34 || i + r[0] > size) {
            break;
        }
        nl = r[32];
        if (!(nl == 1 && (r[33] == 0 || r[33] == 1))) {   /* (not "." / "..") */
            memcpy(name, r + 33, (size_t)nl);
            name[nl] = 0;
            if (strchr(name, ';') != NULL) {
                *strchr(name, ';') = 0;
            }
            if (nl > 0 && name[strlen(name) - 1] == '.') {   /* "NAME." : no extension */
                name[strlen(name) - 1] = 0;
            }
            snprintf(path, sizeof(path), "%s%s", prefix, name);
            if (r[25] & 2) {
                strcat(path, "/");
                walk(fp, base, le32(r + 2), le32(r + 10), path, depth + 1, l);
            } else {
                listing_add(l, path, base + (int64_t)le32(r + 2) * SECTOR, (long)le32(r + 10));
            }
        }
        i += r[0];
    }
    free(d);
    return 1;
}

/* the ISO 9660 image at `base`: its top folder's sector and size; 0 if it isn't one */
static int iso_root(FILE *fp, int64_t base, uint32_t *lba, uint32_t *size) {
    uint8_t pvd[SECTOR];

    if (!read_at(fp, base + 16 * SECTOR, pvd, SECTOR) || pvd[0] != 1 || memcmp(pvd + 1, "CD001", 5) != 0) {
        return 0;
    }
    *lba = le32(pvd + 156 + 2);
    *size = le32(pvd + 156 + 10);
    return 1;
}

static void forget(void) {
    listing_clear(&sInside);
    listing_clear(&sBeside);
    sImage[0] = sFolder[0] = 0;
}

static int is_dir(const char *path) {
    SDL_PathInfo info;

    return SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_DIRECTORY;
}

static void folder_of(char *out, size_t n, const char *path) {
    char *slash;

    snprintf(out, n, "%s", path);
    slash = strrchr(out, '/') > strrchr(out, '\\') ? strrchr(out, '/') : strrchr(out, '\\');
    if (slash != NULL) {
        *slash = 0;
    } else {
        snprintf(out, n, ".");
    }
}

/* an image file: the disc (DATA.CVM inside it) or DATA.CVM */
static int open_image(const char *path) {
    FILE *fp = fopen(path, "rb");
    uint8_t magic[4];
    uint32_t lba, size;
    int64_t cvm = -1;

    if (fp == NULL) {
        snprintf(sError, sizeof(sError), "can't open \"%s\"", path);
        return 0;
    }
    if (read_at(fp, 0, magic, 4) && memcmp(magic, "CVMH", 4) == 0) {
        cvm = 0;
        folder_of(sFolder, sizeof(sFolder), path);   /* (its neighbours: the disc's other files) */
    } else if (iso_root(fp, 0, &lba, &size)) {
        const Entry *e;

        walk(fp, 0, lba, size, "", 0, &sBeside);
        if ((e = listing_find(&sBeside, "DATA.CVM")) == NULL) {
            snprintf(sError, sizeof(sError), "\"%s\" is a disc image, but not of Haunting Ground (no DATA.CVM)", path);
            fclose(fp);
            return 0;
        }
        cvm = e->at;
    } else {
        snprintf(sError, sizeof(sError), "\"%s\" isn't a disc image (.iso) or a DATA.CVM", path);
        fclose(fp);
        return 0;
    }
    if (!read_at(fp, cvm, magic, 4) || memcmp(magic, "CVMH", 4) != 0 || !iso_root(fp, cvm + CVM_HEADER, &lba, &size)) {
        snprintf(sError, sizeof(sError), "the DATA.CVM in \"%s\" can't be read", path);
        fclose(fp);
        return 0;
    }
    walk(fp, cvm + CVM_HEADER, lba, size, "", 0, &sInside);
    fclose(fp);
    snprintf(sImage, sizeof(sImage), "%s", path);
    return 1;
}

int disc_open(const char *path) {
    int i;

    forget();
    sError[0] = 0;
    if (path == NULL || path[0] == 0) {
        snprintf(sError, sizeof(sError), "no game data given");
        return 0;
    }
    if (is_dir(path)) {
        char probe[1100];

        snprintf(probe, sizeof(probe), "%s/SYSTEM", path);
        if (!is_dir(probe)) {
            snprintf(sError, sizeof(sError), "\"%s\" isn't the extracted DATA.CVM folder (no SYSTEM in it)", path);
            return 0;
        }
        snprintf(sFolder, sizeof(sFolder), "%s", path);
        fprintf(stderr, "disc: the folder %s\n", path);
        return 1;
    }
    if (!open_image(path)) {
        forget();
        return 0;
    }
    for (i = 0; i < sInside.n && strncasecmp(sInside.e[i].path, "SYSTEM/", 7) != 0; i++) {
    }
    if (i == sInside.n) {
        snprintf(sError, sizeof(sError), "\"%s\" isn't Haunting Ground (no SYSTEM folder in its DATA.CVM)", path);
        forget();
        return 0;
    }
    fprintf(stderr, "disc: %s (%d files)\n", path, sInside.n);
    return 1;
}

/* ---- files ---- */

/* in a folder: open dir/name, else name matched without regard to case */
static FILE *open_ci(const char *dir, const char *name) {
    char path[1300];
    DIR *dp;
    struct dirent *de;
    FILE *f;

    snprintf(path, sizeof(path), "%s/%s", dir, name);
    if ((f = fopen(path, "rb")) != NULL || (dp = opendir(dir)) == NULL) {
        return f;
    }
    while ((de = readdir(dp)) != NULL) {
        if (strcasecmp(de->d_name, name) == 0) {
            snprintf(path, sizeof(path), "%s/%s", dir, de->d_name);
            f = fopen(path, "rb");
            break;
        }
    }
    closedir(dp);
    return f;
}

static DiscFile *handle(FILE *fp, int64_t base, long size) {
    DiscFile *h = calloc(1, sizeof(*h));

    if (h == NULL) {
        fclose(fp);
        return NULL;
    }
    h->fp = fp;
    h->base = base;
    h->size = size;
    return h;
}

/* `path` (folders by "/") in the folder `root` */
static DiscFile *open_in_folder(const char *root, const char *path) {
    char dir[1300], want[1024], *slash;
    FILE *fp;

    snprintf(want, sizeof(want), "%s", path);
    slash = strrchr(want, '/');
    if (slash != NULL) {   /* (the folders as given - the game's names match the extraction - the
                              file itself without regard to case) */
        *slash = 0;
        snprintf(dir, sizeof(dir), "%s/%s", root, want);
        fp = open_ci(dir, slash + 1);
    } else {
        fp = open_ci(root, want);
    }
    if (fp == NULL) {
        return NULL;
    }
    fseek64(fp, 0, SEEK_END);
    return handle(fp, 0, (long)ftell64(fp));
}

static DiscFile *open_in_image(const Listing *l, const char *path) {
    const Entry *e = listing_find(l, path);
    FILE *fp;

    if (e == NULL || (fp = fopen(sImage, "rb")) == NULL) {
        return NULL;
    }
    return handle(fp, e->at, e->size);
}

DiscFile *disc_file_open(const char *name) {
    char path[1024];
    int beside;

    snprintf(path, sizeof(path), "%s", name);
    norm(path);
    beside = strncmp(path, "../", 3) == 0;
    if (sImage[0] == 0) {   /* a folder: "../" is its parent */
        return sFolder[0] != 0 ? open_in_folder(sFolder, path) : NULL;
    }
    if (!beside) {
        return open_in_image(&sInside, path);
    }
    if (sBeside.n > 0) {   /* the disc's own files */
        return open_in_image(&sBeside, path + 3);
    }
    return open_in_folder(sFolder, path + 3);   /* (a DATA.CVM: the files beside it) */
}

DiscFile *disc_file_open_in(const char *folder, const char *name) {
    char path[1024], parent[1100], *slash;

    snprintf(path, sizeof(path), "%s", name);
    norm(path);
    if (strncmp(path, "../", 3) != 0) {
        return open_in_folder(folder, path);
    }
    snprintf(parent, sizeof(parent), "%s", folder);
    slash = strrchr(parent, '/');
    if (slash == NULL) {
        return NULL;
    }
    *slash = 0;
    return open_in_folder(parent, path + 3);
}

size_t disc_file_read(DiscFile *f, void *buf, size_t n) {
    size_t got;

    if (f->pos >= f->size) {
        return 0;
    }
    if (n > (size_t)(f->size - f->pos)) {
        n = (size_t)(f->size - f->pos);
    }
    if (fseek64(f->fp, f->base + f->pos, SEEK_SET) != 0) {
        return 0;
    }
    got = fread(buf, 1, n, f->fp);
    f->pos += (long)got;
    return got;
}

void disc_file_seek(DiscFile *f, long pos) { f->pos = pos < 0 ? 0 : pos; }
long disc_file_size(const DiscFile *f) { return f->size; }

void disc_file_close(DiscFile *f) {
    if (f != NULL) {
        fclose(f->fp);
        free(f);
    }
}

unsigned char *disc_read_all(const char *path, size_t *size) { return disc_file_read_all(disc_file_open(path), size); }

unsigned char *disc_file_read_all(DiscFile *f, size_t *size) {
    unsigned char *buf;
    long n;

    if (f == NULL) {
        return NULL;
    }
    n = f->size;
    buf = malloc((size_t)n + 1);
    if (buf == NULL || disc_file_read(f, buf, (size_t)n) != (size_t)n) {
        free(buf);
        disc_file_close(f);
        return NULL;
    }
    disc_file_close(f);
    buf[n] = 0;
    if (size != NULL) {
        *size = (size_t)n;
    }
    return buf;
}

DiscFile *disc_exe(void) {
    const char *env = getenv("HG_EXE");
    DiscFile *f;
    FILE *fp;

    if (env != NULL && env[0] != 0 && (fp = fopen(env, "rb")) != NULL) {
        fseek64(fp, 0, SEEK_END);
        return handle(fp, 0, (long)ftell64(fp));
    }
    if ((f = disc_file_open("../SLUS_210.75")) != NULL) {
        return f;
    }
    return sImage[0] == 0 && sFolder[0] != 0 ? open_in_folder(sFolder, "SLUS_210.75") : NULL;
}

/* ---- the files there are (an image's: the import copies them) ---- */

int disc_is_image(void) { return sImage[0] != 0; }
int disc_entries(void) { return sInside.n + sBeside.n; }

/* entry i: its path ("../" for the disc's own files, beside DATA.CVM) and size; 0 past them */
int disc_entry(int i, char *path, size_t n, long *size) {
    const Entry *e;

    if (i < 0 || i >= sInside.n + sBeside.n) {
        return 0;
    }
    if (i < sInside.n) {
        e = &sInside.e[i];
        snprintf(path, n, "%s", e->path);
    } else {
        e = &sBeside.e[i - sInside.n];
        snprintf(path, n, "../%s", e->path);
    }
    *size = e->size;
    return 1;
}

const char *disc_folder(void) { return sFolder; }   /* (a DATA.CVM's: the folder it is in) */

/* ---- the player's folder ---- */

const char *disc_user_dir(void) { return library_user_dir(); }   /* (the library's user/: platform/library.c) */

/* ---- choosing the disc (the library's import: platform/library.c) ---- */

typedef struct Pick {
    SDL_AtomicInt done;
    char path[1024];
} Pick;

static void pick_cb(void *arg, const char *const *files, int filter) {   /* (maybe on another thread) */
    Pick *p = arg;

    (void)filter;
    if (files == NULL) {
        fprintf(stderr, "disc: the file dialog failed: %s\n", SDL_GetError());
    } else if (files[0] != NULL) {
        snprintf(p->path, sizeof(p->path), "%s", files[0]);
    }
    SDL_SetAtomicInt(&p->done, 1);
}

/* ask the player for the disc image, until one works or they cancel; `why` (if set) said first */
int disc_pick(const char *why) {
    static const SDL_DialogFileFilter filters[] = {
        {"Haunting Ground disc image (.iso)", "iso;cvm"},
        {"All files", "*"},
    };

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return 0;
    }
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Haunting Ground",
                             why != NULL ? why
                                         : "Choose your copy of Haunting Ground (USA): its disc image (.iso).",
                             NULL);
    for (;;) {
        SDL_PropertiesID props = SDL_CreateProperties();
        Pick p;
        char msg[512];

        memset(&p, 0, sizeof(p));
        SDL_SetPointerProperty(props, SDL_PROP_FILE_DIALOG_FILTERS_POINTER, (void *)filters);
        SDL_SetNumberProperty(props, SDL_PROP_FILE_DIALOG_NFILTERS_NUMBER, SDL_arraysize(filters));
        SDL_SetStringProperty(props, SDL_PROP_FILE_DIALOG_TITLE_STRING, "Choose your Haunting Ground disc image");
        SDL_ShowFileDialogWithProperties(SDL_FILEDIALOG_OPENFILE, pick_cb, &p, props);
        while (!SDL_GetAtomicInt(&p.done)) {
            SDL_Event e;

            SDL_WaitEventTimeout(&e, 50);
        }
        SDL_DestroyProperties(props);
        if (p.path[0] == 0) {
            return 0;
        }
        if (disc_open(p.path)) {
            return 1;
        }
        snprintf(msg, sizeof(msg), "That file can't be used: %s.\n\nChoose the disc image (.iso) of Haunting Ground (USA).",
                 disc_error());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Haunting Ground", msg, NULL);
    }
}
