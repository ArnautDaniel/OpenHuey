/* CRI file system (ROFS directories + ADXF reads) on PC: files come from the game data (disc.c:
 * the disc image or the extracted DATA.CVM folder). The game names files "FOLDER\FILE"; the
 * loader keeps one directory listing buffer per folder, which here just holds the folder's name. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disc.h"

#define SECTOR 2048

/* listing buffer contents (our own format: the game only passes the buffer around) */
#define DIR_MAGIC 0x52494447u   /* "GDIR" */

typedef struct HostDir {
    unsigned magic;
    char name[64];
} HostDir;

/* ROFS_LoadDir(folder, listing, max entries): 0 = done */
int ROFS_LoadDir(const char *dir, void *list, int max) {
    HostDir *d = list;

    (void)max;
    d->magic = DIR_MAGIC;
    snprintf(d->name, sizeof(d->name), "%s", strcmp(dir, ".") == 0 ? "" : dir);
    return 0;
}

/* A file handle (ADXF): the game only checks it for NULL and passes it back. */
typedef struct HostFile {
    HgFile *f;
    long pos;         /* sectors */
    long lastRead;    /* sectors */
    int stat;
} HostFile;

enum { ADXF_STAT_STOP = 1, ADXF_STAT_READING = 2, ADXF_STAT_READEND = 3, ADXF_STAT_ERROR = 4 };

/* for the sound streams (snd.h): a listing's folder, and opening a file in one */
const char *crifs_folder(const void *list) {
    const HostDir *d = list;

    return d != NULL && d->magic == DIR_MAGIC ? d->name : "";
}

HgFile *crifs_open(const char *folder, const char *name) { return hg_file_open(folder, name); }

/* ADXF open of `name` in the folder whose listing is `list` (NULL handle if missing) */
void *ADXF_OpenInDir(const char *name, void *list) {
    const char *dir = crifs_folder(list);
    HostFile *h;
    HgFile *f = hg_file_open(dir, name);

    if (f == NULL) {
        fprintf(stderr, "crifs: can't open %s/%s\n", dir, name);
        return NULL;
    }
    h = calloc(1, sizeof(*h));
    h->f = f;
    h->stat = ADXF_STAT_STOP;
    fprintf(stderr, "crifs: open %s/%s (%ld bytes)\n", dir, name, hg_file_size(f));
    return h;
}

/* ADXF_Close */
void ADXF_Close(HostFile *h) {
    if (h != NULL) {
        hg_file_close(h->f);
        free(h);
    }
}

/* ADXF_GetFsizeSct-based size in sectors */
int ADXF_FileSectors(HostFile *h) { return (int)((hg_file_size(h->f) + SECTOR - 1) / SECTOR); }
int ADXF_GetFsizeSct(HostFile *h) { return ADXF_FileSectors(h); }

/* PS2 buffer addresses may carry the uncached / accelerated segment bits */
static void *host_ptr(unsigned addr) {
    /* (only those: a PC pointer may be anywhere, e.g. a big block high up) */
    if ((addr >> 28) == 2 || (addr >> 28) == 3) {
        addr &= 0x0FFFFFFFu;
    }
    return (void *)addr;
}

/* ADXF_ReadNw(h, sectors, buffer): reads complete immediately */
int ADXF_ReadNw(HostFile *h, int nsct, unsigned buf) {
    size_t n;

    hg_file_seek(h->f, h->pos * SECTOR);
    n = hg_file_read(h->f, host_ptr(buf), (size_t)nsct * SECTOR);
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
        h->pos = ADXF_FileSectors(h) + pos;
    }
    return (int)h->pos;
}

int ADXF_Tell(HostFile *h) { return (int)h->pos; }
int ADXF_GetStat(HostFile *h) { return h != NULL ? h->stat : ADXF_STAT_ERROR; }
int ADXF_GetNumReadSct(HostFile *h) { return (int)h->lastRead; }
void ADXF_Stop(HostFile *h) { h->stat = ADXF_STAT_STOP; }
void ADXF_StopNw(HostFile *h) { h->stat = ADXF_STAT_STOP; }
