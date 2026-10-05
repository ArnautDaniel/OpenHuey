/* CRI file system (ROFS directories + ADXF reads) on PC: files come from the extracted DATA.CVM
 * folder (hg_data_dir). The game names files "FOLDER\FILE"; the loader keeps one directory
 * listing buffer per folder, which here just holds the folder's name. */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SECTOR 2048

const char *hg_data_dir = HG_DEFAULT_DATA;   /* (native/CMakeLists.txt) */

/* listing buffer contents (our own format: the game only passes the buffer around) */
#define DIR_MAGIC 0x52494447u   /* "GDIR" */

typedef struct HostDir {
    unsigned magic;
    char name[64];
} HostDir;

/* ROFS_LoadDir(folder, listing, max entries): 0 = done */
int func_001E7380(const char *dir, void *list, int max) {
    HostDir *d = list;

    (void)max;
    d->magic = DIR_MAGIC;
    snprintf(d->name, sizeof(d->name), "%s", strcmp(dir, ".") == 0 ? "" : dir);
    return 0;
}

/* A file handle (ADXF): the game only checks it for NULL and passes it back. */
typedef struct HostFile {
    FILE *f;
    long size;        /* bytes */
    long pos;         /* sectors */
    long lastRead;    /* sectors */
    int stat;
} HostFile;

enum { ADXF_STAT_STOP = 1, ADXF_STAT_READING = 2, ADXF_STAT_READEND = 3, ADXF_STAT_ERROR = 4 };

/* open path, falling back to a case-insensitive match of the last component */
static FILE *open_ci(const char *dir, const char *name) {
    char path[1024];
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

/* for the sound streams (snd.h): a listing's folder, and opening a file in one */
const char *crifs_folder(const void *list) {
    const HostDir *d = list;

    return d != NULL && d->magic == DIR_MAGIC ? d->name : "";
}

FILE *crifs_open(const char *folder, const char *name) {
    char dir[1024];

    if (folder != NULL && folder[0] != 0) {
        snprintf(dir, sizeof(dir), "%s/%s", hg_data_dir, folder);
    } else {
        snprintf(dir, sizeof(dir), "%s", hg_data_dir);
    }
    return open_ci(dir, name);
}

/* ADXF open of `name` in the folder whose listing is `list` (NULL handle if missing) */
void *func_001C9438(const char *name, void *list) {
    const HostDir *d = list;
    char dir[1024];
    HostFile *h;
    FILE *f;

    if (d != NULL && d->magic == DIR_MAGIC && d->name[0] != 0) {
        snprintf(dir, sizeof(dir), "%s/%s", hg_data_dir, d->name);
    } else {
        snprintf(dir, sizeof(dir), "%s", hg_data_dir);
    }
    f = open_ci(dir, name);
    if (f == NULL) {
        fprintf(stderr, "crifs: can't open %s/%s\n", dir, name);
        return NULL;
    }
    h = calloc(1, sizeof(*h));
    h->f = f;
    fseek(f, 0, SEEK_END);
    h->size = ftell(f);
    h->stat = ADXF_STAT_STOP;
    fprintf(stderr, "crifs: open %s/%s (%ld bytes)\n", dir, name, h->size);
    return h;
}

/* ADXF_Close */
void func_001C9800(HostFile *h) {
    if (h != NULL) {
        fclose(h->f);
        free(h);
    }
}

/* ADXF_GetFsizeSct-based size in sectors */
int func_001CA0B8(HostFile *h) { return (int)((h->size + SECTOR - 1) / SECTOR); }
int ADXF_GetFsizeSct(HostFile *h) { return func_001CA0B8(h); }

/* PS2 buffer addresses may carry the uncached / accelerated segment bits */
static void *host_ptr(unsigned addr) { return (void *)(addr & 0x0FFFFFFFu); }

/* ADXF_ReadNw(h, sectors, buffer): reads complete immediately */
int ADXF_ReadNw(HostFile *h, int nsct, unsigned buf) {
    size_t n;

    fseek(h->f, h->pos * SECTOR, SEEK_SET);
    n = fread(host_ptr(buf), 1, (size_t)nsct * SECTOR, h->f);
    h->lastRead = (long)((n + SECTOR - 1) / SECTOR);
    h->pos += h->lastRead;
    h->stat = ADXF_STAT_READEND;
    return (int)h->lastRead;
}

int ADXF_ReadNw32(HostFile *h, int nsct, unsigned buf) { return ADXF_ReadNw(h, nsct, buf); }

/* ADXF_Seek(h, sector, whence) */
int ADXF_Seek(HostFile *h, int pos, int type) {
    if (type == 0) {
        h->pos = pos;
    } else if (type == 1) {
        h->pos += pos;
    } else {
        h->pos = func_001CA0B8(h) + pos;
    }
    return (int)h->pos;
}

int ADXF_Tell(HostFile *h) { return (int)h->pos; }
int ADXF_GetStat(HostFile *h) { return h != NULL ? h->stat : ADXF_STAT_ERROR; }
int ADXF_GetNumReadSct(HostFile *h) { return (int)h->lastRead; }
void ADXF_Stop(HostFile *h) { h->stat = ADXF_STAT_STOP; }
void ADXF_StopNw(HostFile *h) { h->stat = ADXF_STAT_STOP; }
