/* The game data (disc.h). Three forms are accepted:
 *   - the disc image: ISO 9660, with DATA.CVM in its top folder
 *   - DATA.CVM itself: a 0x1800-byte "CVMH" header, then an ISO 9660 image of the game's folders
 *   - the folder DATA.CVM was extracted to (SYSTEM/ etc. in it)
 * For the first two the files of the inner image are listed once; each open file then has its
 * own handle on the image (so the sound thread's streams don't share a position with loads). The
 * image is 4.5 GB: offsets into it are 64-bit. */
#define _FILE_OFFSET_BITS 64
#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <SDL3/SDL.h>

#include "disc.h"

#define SECTOR 2048
#define CVM_HEADER 0x1800

#ifdef _WIN32
#define fseek64 _fseeki64
#else
#define fseek64 fseeko
#endif

struct HgFile {
    FILE *fp;
    int64_t base;   /* where the file starts in fp */
    long size, pos;
};

typedef struct Entry {
    char *path;     /* "FOLDER/NAME" */
    int64_t at;
    long size;
} Entry;

static char sImage[1024];     /* the .iso / DATA.CVM; "" for a folder */
static char sFolder[1024];    /* the extracted folder */
static Entry *sFiles;
static int sNumFiles, sCapFiles;
static char sError[256];
/* the game's executable SLUS_210.75 (slus.c reads its data): in the disc image, else a file */
static char sExe[1024];
static int64_t sExeAt = -1;
static long sExeSize;

const char *hg_disc_error(void) { return sError; }

/* "a\b" -> "a/b" */
static void norm(char *s) {
    for (; *s != 0; s++) {
        *s = *s == '\\' ? '/' : *s;
    }
}

static int read_at(FILE *fp, int64_t at, void *buf, size_t n) {
    return fseek64(fp, at, SEEK_SET) == 0 && fread(buf, 1, n, fp) == n;
}

static uint32_t le32(const uint8_t *p) { return p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24; }

static void add_file(const char *path, int64_t at, long size) {
    if (sNumFiles == sCapFiles) {
        sCapFiles = sCapFiles ? sCapFiles * 2 : 1024;
        sFiles = realloc(sFiles, sizeof(*sFiles) * (size_t)sCapFiles);
    }
    sFiles[sNumFiles].path = strdup(path);
    norm(sFiles[sNumFiles].path);
    sFiles[sNumFiles].at = at;
    sFiles[sNumFiles].size = size;
    sNumFiles++;
}

/* list the ISO 9660 directory at sector `lba` (`size` bytes) of the image at `base`; `cb` gets
 * each file. 0 if the directory couldn't be read. */
typedef void (*FileCb)(const char *path, int64_t at, long size, void *arg);

static int walk(FILE *fp, int64_t base, uint32_t lba, uint32_t size, const char *prefix, int depth, FileCb cb,
                void *arg) {
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
                walk(fp, base, le32(r + 2), le32(r + 10), path, depth + 1, cb, arg);
            } else {
                cb(path, base + (int64_t)le32(r + 2) * SECTOR, (long)le32(r + 10), arg);
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

static void list_cb(const char *path, int64_t at, long size, void *arg) {
    (void)arg;
    add_file(path, at, size);
}

typedef struct Find {
    const char *want;
    int64_t at;
    long size;
} Find;

static void find_cb(const char *path, int64_t at, long size, void *arg) {
    Find *f = arg;

    if (strcasecmp(path, f->want) == 0) {
        f->at = at;
        f->size = size;
    }
}

static void clear(void) {
    int i;

    for (i = 0; i < sNumFiles; i++) {
        free(sFiles[i].path);
    }
    sNumFiles = 0;
    sImage[0] = sFolder[0] = sExe[0] = 0;
    sExeAt = -1;
}

static void find_exe(const char *path);

static int is_dir(const char *path) {
    SDL_PathInfo info;

    return SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_DIRECTORY;
}

/* an image file: the disc (DATA.CVM inside it) or DATA.CVM */
static int open_image(const char *path) {
    FILE *fp = fopen(path, "rb");
    uint8_t magic[4];
    uint32_t lba, size;
    int64_t cvm = -1;
    Find f = {"DATA.CVM", -1, 0};

    if (fp == NULL) {
        snprintf(sError, sizeof(sError), "can't open \"%s\"", path);
        return 0;
    }
    if (read_at(fp, 0, magic, 4) && memcmp(magic, "CVMH", 4) == 0) {
        cvm = 0;
    } else if (iso_root(fp, 0, &lba, &size)) {
        Find exe = {"SLUS_210.75", -1, 0};

        walk(fp, 0, lba, size, "", 0, find_cb, &f);
        walk(fp, 0, lba, size, "", 0, find_cb, &exe);
        if (exe.at >= 0) {
            snprintf(sExe, sizeof(sExe), "%s", path);
            sExeAt = exe.at;
            sExeSize = exe.size;
        }
        if (f.at < 0) {
            snprintf(sError, sizeof(sError), "\"%s\" is a disc image, but not of Haunting Ground (no DATA.CVM)", path);
            fclose(fp);
            return 0;
        }
        cvm = f.at;
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
    walk(fp, cvm + CVM_HEADER, lba, size, "", 0, list_cb, NULL);
    fclose(fp);
    snprintf(sImage, sizeof(sImage), "%s", path);
    return 1;
}

int hg_disc_open(const char *path) {
    int i;

    clear();
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
        find_exe(path);
        fprintf(stderr, "disc: the folder %s\n", path);
        return 1;
    }
    if (!open_image(path)) {
        clear();
        return 0;
    }
    for (i = 0; i < sNumFiles && strncasecmp(sFiles[i].path, "SYSTEM/", 7) != 0; i++) {
    }
    if (i == sNumFiles) {
        snprintf(sError, sizeof(sError), "\"%s\" isn't Haunting Ground (no SYSTEM folder in its DATA.CVM)", path);
        clear();
        return 0;
    }
    {
        /* (a DATA.CVM: SLUS_210.75 beside it) */
        char dir[1024], *slash;

        snprintf(dir, sizeof(dir), "%s", path);
        slash = strrchr(dir, '/') > strrchr(dir, '\\') ? strrchr(dir, '/') : strrchr(dir, '\\');
        if (slash != NULL) {
            *slash = 0;
            find_exe(dir);
        } else {
            find_exe(".");
        }
    }
    fprintf(stderr, "disc: %s (%d files)\n", path, sNumFiles);
    return 1;
}

/* SLUS_210.75 for the data in a DATA.CVM or a folder: $HG_SLUS, else next to it or in the folder
 * above (where the disc's files sit when they were all copied off it) */
static void find_exe(const char *path) {
    char dir[1024], probe[1100], *slash;
    const char *env = getenv("HG_SLUS");
    int i;

    if (sExeAt >= 0) {
        return;
    }
    snprintf(dir, sizeof(dir), "%s", path);
    for (i = 0; i < 2; i++) {
        SDL_PathInfo info;

        if (env != NULL && env[0] != 0) {
            snprintf(probe, sizeof(probe), "%s", env);
        } else {
            snprintf(probe, sizeof(probe), "%s/SLUS_210.75", dir);
        }
        if (SDL_GetPathInfo(probe, &info) && info.type == SDL_PATHTYPE_FILE) {
            snprintf(sExe, sizeof(sExe), "%s", probe);
            sExeAt = 0;
            sExeSize = (long)info.size;
            return;
        }
        /* up a folder */
        while (dir[0] != 0 && (dir[strlen(dir) - 1] == '/' || dir[strlen(dir) - 1] == '\\')) {
            dir[strlen(dir) - 1] = 0;
        }
        slash = strrchr(dir, '/');
        if (strrchr(dir, '\\') > slash) {
            slash = strrchr(dir, '\\');
        }
        if (slash == NULL) {
            return;
        }
        *slash = 0;
    }
}

HgFile *hg_disc_exe(void) {
    HgFile *h;
    FILE *fp;

    if (sExeAt < 0 || (fp = fopen(sExe, "rb")) == NULL) {
        return NULL;
    }
    h = calloc(1, sizeof(*h));
    h->fp = fp;
    h->base = sExeAt;
    h->size = sExeSize;
    return h;
}

const char *hg_disc_exe_path(void) { return sExe; }

/* the extracted folder: open dir/name, else a case-insensitive match of name */
static FILE *open_ci(const char *dir, const char *name) {
    char path[1300];
    DIR *dp;
    struct dirent *de;
    FILE *f;

    snprintf(path, sizeof(path), "%s/%s", dir, name);
    if ((f = fopen(path, "rb")) != NULL) {
        return f;
    }
    if ((dp = opendir(dir)) == NULL) {
        return NULL;
    }
    f = NULL;
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

HgFile *hg_file_open(const char *folder, const char *name) {
    char want[1024];
    HgFile *h;
    FILE *fp;
    int i;

    if (folder != NULL && folder[0] != 0) {
        snprintf(want, sizeof(want), "%s/%s", folder, name);
    } else {
        snprintf(want, sizeof(want), "%s", name);
    }
    norm(want);
    if (sImage[0] != 0) {
        for (i = 0; i < sNumFiles && strcasecmp(sFiles[i].path, want) != 0; i++) {
        }
        if (i == sNumFiles || (fp = fopen(sImage, "rb")) == NULL) {
            return NULL;
        }
        h = calloc(1, sizeof(*h));
        h->fp = fp;
        h->base = sFiles[i].at;
        h->size = sFiles[i].size;
        return h;
    }
    {
        /* the folder: each part but the last as given (the game's names match the extraction),
           the last case-insensitively */
        char dir[1300], *slash = strrchr(want, '/');

        if (slash != NULL) {
            *slash = 0;
            snprintf(dir, sizeof(dir), "%s/%s", sFolder, want);
            fp = open_ci(dir, slash + 1);
        } else {
            fp = open_ci(sFolder, want);
        }
    }
    if (fp == NULL) {
        return NULL;
    }
    h = calloc(1, sizeof(*h));
    h->fp = fp;
    fseek64(fp, 0, SEEK_END);
    h->size = (long)ftell(fp);
    return h;
}

size_t hg_file_read(HgFile *f, void *buf, size_t n) {
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

int hg_file_seek(HgFile *f, long pos) {
    if (pos < 0) {
        return -1;
    }
    f->pos = pos;
    return 0;
}

long hg_file_size(const HgFile *f) { return f->size; }
long hg_file_tell(const HgFile *f) { return f->pos; }

void hg_file_close(HgFile *f) {
    if (f != NULL) {
        fclose(f->fp);
        free(f);
    }
}

const char *hg_save_dir(void) {
    static char dir[1024];

    if (dir[0] == 0) {
        const char *env = getenv("HG_SAVE");
        char *pref;

        if (env != NULL && env[0] != 0) {
            snprintf(dir, sizeof(dir), "%s", env);
        } else if (is_dir("save")) {   /* (a "save" folder where hg is started: the developers') */
            snprintf(dir, sizeof(dir), "save");
        } else if ((pref = SDL_GetPrefPath("OpenHuey", "hg")) != NULL) {
            snprintf(dir, sizeof(dir), "%s", pref);
            SDL_free(pref);
            if (dir[0] != 0 && (dir[strlen(dir) - 1] == '/' || dir[strlen(dir) - 1] == '\\')) {
                dir[strlen(dir) - 1] = 0;
            }
        } else {
            snprintf(dir, sizeof(dir), "save");
        }
        SDL_CreateDirectory(dir);
    }
    return dir;
}

/* ---- choosing the disc ---- */

/* the disc last chosen (<save dir>/disc.txt) */
static const char *remembered(void) {
    static char path[1024];
    char file[1100];
    FILE *f;

    snprintf(file, sizeof(file), "%s/disc.txt", hg_save_dir());
    if ((f = fopen(file, "r")) == NULL) {
        return NULL;
    }
    if (fgets(path, sizeof(path), f) == NULL) {
        path[0] = 0;
    }
    fclose(f);
    path[strcspn(path, "\r\n")] = 0;
    return path[0] != 0 ? path : NULL;
}

static void remember(const char *path) {
    char file[1100];
    FILE *f;

    snprintf(file, sizeof(file), "%s/disc.txt", hg_save_dir());
    if ((f = fopen(file, "w")) != NULL) {
        fprintf(f, "%s\n", path);
        fclose(f);
    }
}

typedef struct Pick {
    SDL_AtomicInt done;
    char path[1024];
} Pick;

/* (may be called on another thread) */
static void pick_cb(void *arg, const char *const *files, int filter) {
    Pick *p = arg;

    (void)filter;
    if (files == NULL) {
        fprintf(stderr, "disc: the file dialog failed: %s\n", SDL_GetError());
    } else if (files[0] != NULL) {
        snprintf(p->path, sizeof(p->path), "%s", files[0]);
    }
    SDL_SetAtomicInt(&p->done, 1);
}

/* ask the player for the disc image, until one works or they cancel; `why` (if set) is said first */
static int pick(const char *why) {
    static const SDL_DialogFileFilter filters[] = {
        {"Haunting Ground disc image (.iso)", "iso;cvm"},
        {"All files", "*"},
    };

    if (getenv("HG_HEADLESS") != NULL || !SDL_Init(SDL_INIT_VIDEO)) {
        return 0;
    }
    if (why != NULL) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Haunting Ground", why, NULL);
    }
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
        if (hg_disc_open(p.path)) {
            remember(p.path);
            return 1;
        }
        snprintf(msg, sizeof(msg), "That file can't be used: %s.\n\nChoose the disc image (.iso) of Haunting Ground (USA).",
                 hg_disc_error());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Haunting Ground", msg, NULL);
    }
}

int hg_disc_choose(const char *given, const char *fallback) {
    const char *r;
    char why[1400], *whyp = NULL;

    if (given != NULL && given[0] != 0) {
        if (hg_disc_open(given)) {
            return 1;
        }
        fprintf(stderr, "hg: %s\n", hg_disc_error());
        return 0;
    }
    if ((r = remembered()) != NULL) {
        if (hg_disc_open(r)) {
            return 1;
        }
        fprintf(stderr, "hg: the disc used last time can't be read (%s)\n", hg_disc_error());
        snprintf(why, sizeof(why), "The disc image used last time isn't there any more:\n%s\n\nPlease choose it again.", r);
        whyp = why;
    }
    if (fallback != NULL && fallback[0] != 0 && hg_disc_open(fallback)) {
        return 1;
    }
    if (pick(whyp)) {
        return 1;
    }
    fprintf(stderr, "hg: no game data. Pass the Haunting Ground disc image (.iso), its DATA.CVM or the folder\n"
                    "DATA.CVM was extracted to as the first argument (or in $HG_DATA).\n");
    return 0;
}
