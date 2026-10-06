/* The file loader (system +0x319900, vtable 0x46A1E0, global gFileLoader). Files are named
 * "FOLDER\FILE" after the data folders of DATA.CVM; each folder's directory listing is loaded
 * once at startup (CRI ROFS). Reads go through CRI ADXF (replaced on PC by
 * native/platform/crifs.c, which reads the extracted folder). */
#include "common.h"
#include "game.h"
#include "globals.h"
#include "ptmf.h"
#include "actor.h"
#include "loader.h"
#include "cri/crifs.h"
#include "libc.h"
#include "msl.h"

#define LOADER_DIRS 208
#define LOADER_DIR_NAME(l, i) ((char *)(l) + 0x12810 + (i) * 0x104)
#define LOADER_DIR_LIST(l, i) ((u8 *)(l) + 0x25BC0 + (i) * 0x6A8)

extern const char D_0044F7F0[];   /* "." (the root) */

#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

void Lzss_Start(u8 *p);

/* init: 256 empty request slots, the root's listing, the folder slots' listing buffers */
/* 0x0016C530 */
void Loader_Init(u8 *l) {
    s32 i;

    AT(l, 0x12808, s32) = 0;
    AT(l, 0x12805, u8) = 0;
    AT(l, 0x12804, u8) = 0;
    for (i = 0; i < 256; i++) {
        u8 *r = l + 8 + i * 0x128;

        AT(r, 0x0, s32) = 0;
        AT(r, 0x4, s32) = 0;
        AT(r, 0x8, s32) = 0;
        AT(r, 0xC, s32) = 0;
        AT(r, 0x10, s32) = 0x10000000;
        AT(r, 0x18, s32) = 0;
        AT(r, 0x14, s32) = 0;
        AT(r, 0x1C, u8) = 0;    /* file name */
        AT(r, 0x11C, u8) = 0;
        AT(r, 0x120, s32) = 0;
    }
    while ((s16)func_001E7380(D_0044F7F0, l + 0x1FB80, 0x200) != 0) {
    }
    for (i = 0; i < LOADER_DIRS; i++) {
        AT(l, 0x1280C + i * 0x104, u8 *) = LOADER_DIR_LIST(l, i);
        *LOADER_DIR_LIST(l, i) = 0;
    }
}

/* Register data folder `dir`: index + 1 if it already is, 0 once its listing is loaded, -1 if
 * there's no free slot. */
/* 0x0016B2B0 */
s32 Loader_RegisterDir(u8 *l, const char *dir) {
    s32 i, free = -1;

    if (dir == NULL) {
        return -1;
    }
    for (i = 0; i < LOADER_DIRS; i++) {
        if (LOADER_DIR_NAME(l, i)[0] == 0) {
            if (free < 0) {
                free = i;
            }
        } else if (func_00118278(dir, LOADER_DIR_NAME(l, i)) == 0) {
            return i + 1;
        }
    }
    if (free < 0) {
        for (i = 0; i < LOADER_DIRS; i++) {
            if (LOADER_DIR_NAME(l, i)[0] == 0) {
                free = i;
                break;
            }
        }
    }
    if (free < 0) {
        return -1;
    }
    while ((s16)func_001E7380(dir, LOADER_DIR_LIST(l, free), 0x23) != 0) {
    }
    func_001183C0(LOADER_DIR_NAME(l, free), dir);
    return 0;
}

extern const char str_BGM[], str_MAP[], D_0044F280[], D_0044F288[], D_0044F290[], D_0044F298[], D_0044F2A0[], D_0044F2A8[], D_0044F2B0[], D_0044F2B8[], D_0044F2C0[], D_0044F2C8[], D_0044F2D0[], D_0044F2D8[], D_0044F2E0[], D_0044F2E8[], D_0044F2F0[], D_0044F2F8[], str_DE0010[], str_DE0011[], str_DE0012[], str_DE0013[], str_DE0014[], str_DE0015[], str_DE0016[], str_DE0017[], str_DE0018[], str_DE0019[], str_DE001A[], str_DE001B[], str_DE001C[], str_DE001D[], str_DE001E[], str_DE001F[], str_DE0020[], str_DE0021[], str_DE0022[], str_DE0023[], str_DE0024[], str_DE0025[], str_DE0026[], str_DE0027[], str_DE0028[], str_DE0029[], str_DE002A[], str_DE002B[], str_DE002C[], str_DE002D[], str_DE002E[], str_DE002F[], str_DE0030[], str_DE0031[], str_DE0032[], str_DE0033[], str_DE0034[], str_DE0035[], str_DE0036[], str_DE0039[], str_DE003B[], str_DE003C[], str_DE003D[], str_DE003F[], str_DE0040[], str_DE0041[], str_DE0042[], str_DE0043[], str_DE0044[], str_DE0045[], str_DE0046[], str_DE0047[], str_DE0048[], str_DE0049[], D_0044F4B0[], str_DE100B[], str_DE1018[], str_DE101F[], str_DE1025[], str_DE102E[], str_DE1033[], str_DE1034[], str_DE1035[], str_DE2018[], str_DE201F[], str_DE2025[], str_DE2035[], str_DE3035[], str_DE4035[], D_0044F528[], D_0044F530[], D_0044F538[], D_0044F540[], D_0044F548[], D_0044F550[], D_0044F558[], D_0044F560[], str_EV0010[], str_EV0011[], str_EV0012[], str_EV0013[], str_EV0014[], str_EV0015[], str_EV0016[], str_EV0017[], str_EV0018[], str_EV0019[], str_EV0020[], str_EV0021[], str_EV0022[], str_EV0023[], str_EV0024[], str_EV0025[], str_EV0026[], str_EV0027[], str_EV0028[], str_EV0029[], str_EV0030[], str_EV0031[], str_EV1031[], str_EV0032[], str_EV0033[], str_EV1033[], str_EV0034[], str_EV1034[], str_ITEM00[], str_ITEM01[], str_ITEM02[], str_ITEM03[], str_ITEM04[], str_O_CRW[], str_O_DB0[], str_O_DB1[], str_O_DB2[], str_O_DNL[], str_O_DNT[], str_O_FIB[], str_O_FIC[], str_O_FIF[], str_O_FIH[], str_O_FIM[], str_O_FIN[], str_O_FIN_M[], str_O_FIO[], str_O_FIS[], str_O_FIW[], str_O_FS0[], str_O_FS1[], str_O_FS2[], str_O_GLM[], str_O_HED[], str_O_HEG[], str_O_HEW[], str_O_HMA[], str_O_HMB[], str_O_HND[], str_O_LRC[], str_O_LRF[], str_O_LRH[], str_O_LRM[], str_O_LRO[], str_O_LRY[], str_O_RBT[], str_O_RCG[], str_O_RCT[], str_O_SGM[], str_O_SHT[], str_O_WHC[], str_O_WIR[], str_ADX00[], str_ADX01[], str_ADX02[], str_SYSTEM[], str_SUBSCR[];

/* the data folders registered at startup after ST_000..ST_108, in the original order */
static const char *const sLoaderDirs[] = {
    str_BGM /* BGM */, str_MAP /* MAP */, D_0044F280 /* DE0000 */, D_0044F288 /* DE0001 */,
    D_0044F290 /* DE0002 */, D_0044F298 /* DE0003 */, D_0044F2A0 /* DE0004 */,
    D_0044F2A8 /* DE0005 */, D_0044F2B0 /* DE0006 */, D_0044F2B8 /* DE0007 */,
    D_0044F2C0 /* DE0008 */, D_0044F2C8 /* DE0009 */, D_0044F2D0 /* DE000A */,
    D_0044F2D8 /* DE000B */, D_0044F2E0 /* DE000C */, D_0044F2E8 /* DE000D */,
    D_0044F2F0 /* DE000E */, D_0044F2F8 /* DE000F */, str_DE0010 /* DE0010 */,
    str_DE0011 /* DE0011 */, str_DE0012 /* DE0012 */, str_DE0013 /* DE0013 */,
    str_DE0014 /* DE0014 */, str_DE0015 /* DE0015 */, str_DE0016 /* DE0016 */,
    str_DE0017 /* DE0017 */, str_DE0018 /* DE0018 */, str_DE0019 /* DE0019 */,
    str_DE001A /* DE001A */, str_DE001B /* DE001B */, str_DE001C /* DE001C */,
    str_DE001D /* DE001D */, str_DE001E /* DE001E */, str_DE001F /* DE001F */,
    str_DE0020 /* DE0020 */, str_DE0021 /* DE0021 */, str_DE0022 /* DE0022 */,
    str_DE0023 /* DE0023 */, str_DE0024 /* DE0024 */, str_DE0025 /* DE0025 */,
    str_DE0026 /* DE0026 */, str_DE0027 /* DE0027 */, str_DE0028 /* DE0028 */,
    str_DE0029 /* DE0029 */, str_DE002A /* DE002A */, str_DE002B /* DE002B */,
    str_DE002C /* DE002C */, str_DE002D /* DE002D */, str_DE002E /* DE002E */,
    str_DE002F /* DE002F */, str_DE0030 /* DE0030 */, str_DE0031 /* DE0031 */,
    str_DE0032 /* DE0032 */, str_DE0033 /* DE0033 */, str_DE0034 /* DE0034 */,
    str_DE0035 /* DE0035 */, str_DE0036 /* DE0036 */, str_DE0039 /* DE0039 */,
    str_DE003B /* DE003B */, str_DE003C /* DE003C */, str_DE003D /* DE003D */,
    str_DE003F /* DE003F */, str_DE0040 /* DE0040 */, str_DE0041 /* DE0041 */,
    str_DE0042 /* DE0042 */, str_DE0043 /* DE0043 */, str_DE0044 /* DE0044 */,
    str_DE0045 /* DE0045 */, str_DE0046 /* DE0046 */, str_DE0047 /* DE0047 */,
    str_DE0048 /* DE0048 */, str_DE0049 /* DE0049 */, D_0044F4B0 /* DE1000 */,
    str_DE100B /* DE100B */, str_DE1018 /* DE1018 */, str_DE101F /* DE101F */,
    str_DE1025 /* DE1025 */, str_DE102E /* DE102E */, str_DE1033 /* DE1033 */,
    str_DE1034 /* DE1034 */, str_DE1035 /* DE1035 */, str_DE2018 /* DE2018 */,
    str_DE201F /* DE201F */, str_DE2025 /* DE2025 */, str_DE2035 /* DE2035 */,
    str_DE3035 /* DE3035 */, str_DE4035 /* DE4035 */, D_0044F528 /* EV0002 */,
    D_0044F530 /* EV0003 */, D_0044F538 /* EV0004 */, D_0044F540 /* EV0005 */,
    D_0044F548 /* EV0006 */, D_0044F550 /* EV0007 */, D_0044F558 /* EV0008 */,
    D_0044F560 /* EV0009 */, str_EV0010 /* EV0010 */, str_EV0011 /* EV0011 */,
    str_EV0012 /* EV0012 */, str_EV0013 /* EV0013 */, str_EV0014 /* EV0014 */,
    str_EV0015 /* EV0015 */, str_EV0016 /* EV0016 */, str_EV0017 /* EV0017 */,
    str_EV0018 /* EV0018 */, str_EV0019 /* EV0019 */, str_EV0020 /* EV0020 */,
    str_EV0021 /* EV0021 */, str_EV0022 /* EV0022 */, str_EV0023 /* EV0023 */,
    str_EV0024 /* EV0024 */, str_EV0025 /* EV0025 */, str_EV0026 /* EV0026 */,
    str_EV0027 /* EV0027 */, str_EV0028 /* EV0028 */, str_EV0029 /* EV0029 */,
    str_EV0030 /* EV0030 */, str_EV0031 /* EV0031 */, str_EV1031 /* EV1031 */,
    str_EV0032 /* EV0032 */, str_EV0033 /* EV0033 */, str_EV1033 /* EV1033 */,
    str_EV0034 /* EV0034 */, str_EV1034 /* EV1034 */, str_ITEM00 /* ITEM00 */,
    str_ITEM01 /* ITEM01 */, str_ITEM02 /* ITEM02 */, str_ITEM03 /* ITEM03 */,
    str_ITEM04 /* ITEM04 */, str_O_CRW /* O_CRW */, str_O_DB0 /* O_DB0 */,
    str_O_DB1 /* O_DB1 */, str_O_DB2 /* O_DB2 */, str_O_DNL /* O_DNL */, str_O_DNT /* O_DNT */,
    str_O_FIB /* O_FIB */, str_O_FIC /* O_FIC */, str_O_FIF /* O_FIF */, str_O_FIH /* O_FIH */,
    str_O_FIM /* O_FIM */, str_O_FIN /* O_FIN */, str_O_FIN_M /* O_FIN_M */,
    str_O_FIO /* O_FIO */, str_O_FIS /* O_FIS */, str_O_FIW /* O_FIW */, str_O_FS0 /* O_FS0 */,
    str_O_FS1 /* O_FS1 */, str_O_FS2 /* O_FS2 */, str_O_GLM /* O_GLM */, str_O_HED /* O_HED */,
    str_O_HEG /* O_HEG */, str_O_HEW /* O_HEW */, str_O_HMA /* O_HMA */, str_O_HMB /* O_HMB */,
    str_O_HND /* O_HND */, str_O_LRC /* O_LRC */, str_O_LRF /* O_LRF */, str_O_LRH /* O_LRH */,
    str_O_LRM /* O_LRM */, str_O_LRO /* O_LRO */, str_O_LRY /* O_LRY */, str_O_RBT /* O_RBT */,
    str_O_RCG /* O_RCG */, str_O_RCT /* O_RCT */, str_O_SGM /* O_SGM */, str_O_SHT /* O_SHT */,
    str_O_WHC /* O_WHC */, str_O_WIR /* O_WIR */, str_ADX00 /* ADX00 */, str_ADX01 /* ADX01 */,
    str_ADX02 /* ADX02 */, str_SYSTEM /* SYSTEM */, str_SUBSCR /* SUBSCR */,
};

extern const char str_ST_N[];   /* "ST_%03X" */

/* register a folder, idling the system (+0x1C) until there is room */
static inline void Loader_AddDir(u8 *l, VObject *sys, const char *dir) {
    do {
        VCALL(sys, 0x1C, void (*)(VObject *))(sys);
    } while (Loader_RegisterDir(l, dir) < 0);
}

/* Register all data folders: the stages ST_000..ST_108, then the rest. */
/* 0x00169680 */
void Loader_RegisterAll(u8 *l) {
    VObject *sys = gSystem;
    char name[0x100];
    u32 i;

    for (i = 0; i < 0x110; i += 8) {
        func_0026EDD0(name, sizeof(name), str_ST_N, i);
        Loader_AddDir(l, sys, name);
    }
    for (i = 0; i < sizeof(sLoaderDirs) / sizeof(sLoaderDirs[0]); i++) {
        Loader_AddDir(l, sys, sLoaderDirs[i]);
    }
}

/* a request: the folder's listing (+0xC) and the file name (+0x20) */
#define REQ_DIR(q) AT(q, 0xC, void *)
#define REQ_NAME(q) ((char *)(q) + 0x20)

#define ADXF_STAT_READEND 3
#define ADXF_STAT_ERROR 4

/* Split "FOLDER\FILE" into the folder's listing (+0x3C lookup; none: the root) and the name. */
/* 0x001694D0 */
void Loader_SplitPath(u8 *q, const char *path) {
    char dir[0x100];
    s32 i;

    REQ_DIR(q) = NULL;
    dir[0] = 0;
    for (i = 0; i < 0x100 && path[i] != 0; i++) {
        if (path[i] == '\\') {
            func_001183C0(REQ_NAME(q), path + i + 1);
            func_00118978(dir, path, i);
            dir[i] = 0;
            break;
        }
    }
    if (dir[0] != 0) {
        REQ_DIR(q) = VCALL(gFileLoader, 0x3C, void *(*)(VObject *, const char *))(gFileLoader, dir);
    } else {
        REQ_DIR(q) = VCALL(gFileLoader, 0x3C, void *(*)(VObject *, const char *))(gFileLoader, NULL);
        func_001183C0(REQ_NAME(q), path);
    }
}

/* Store the parameters, then tail-call virtual +0xC (Reset) with them. */
/* LZSS decompression: +0x18 source (4-byte header), +0x1C destination.
 * Flag bit 1 = literal byte, 0 = 16-bit back-reference (len = low 4 bits + 2,
 * distance = high 12 bits); a zero reference ends the stream. */
/* 0x001695D0 */
void Lzss_Start(u8 *p) {
    u8 *src = FLD(p, 0x18, u8 *) + 4;
    u8 *dst = FLD(p, 0x1C, u8 *);
    u32 bits = 1;
    u32 flags = 0;

    for (;;) {
        bits--;
        flags >>= 1;
        if (bits == 0) {
            flags = *src++;
            bits = 8;
        }
        if (flags & 1) {
            *dst++ = *src++;
        } else {
            u32 w = src[0] | (src[1] << 8);
            u32 dist, len;

            src += 2;
            if (w == 0) {
                return;
            }
            len = (w & 0xF) + 2;
            dist = w >> 4;
            do {
                *dst = *(dst - dist);
                dst++;
            } while (--len != 0);
        }
    }
}

/* +0x2C size of file `path` in sectors (0: not found) */
/* 0x00169450 */
s32 Loader_FileSectors(VObject *l, const char *path) {
    u8 *q = func_00100660(0x128);
    void *f;
    s32 n;

    Loader_SplitPath(q, path);
    f = func_001C9438(REQ_NAME(q), REQ_DIR(q));
    func_00100490(q);
    if (f == NULL) {
        return 0;
    }
    n = func_001CA0B8(f);
    func_001C9800(f);
    return n;
}

/* +0x30 size of file `path` in bytes (whole sectors) */
/* 0x00169420 */
u32 Loader_FileSize(VObject *l, const char *path) {
    return VCALL(l, 0x2C, s32 (*)(VObject *, const char *))(l, path) << 11;
}

/* +0x34 load all of file `path` into `dst` (waiting; retried on read errors); returns its size */
/* 0x001692F0 */
u32 Loader_LoadNow(VObject *l, const char *path, u32 dst) {
    u8 *q = func_00100660(0x128);
    void *f;
    s32 st;
    u32 size;

    Loader_SplitPath(q, path);
    do {
        f = func_001C9438(REQ_NAME(q), REQ_DIR(q));
    } while (f == NULL);
    func_00100490(q);
    if (f == NULL) {
        return 0;
    }
    size = func_001CA0B8(f);
    ADXF_Seek(f, 0, 0);
    dst |= UNCACHED_BIT;   /* uncached */
    ADXF_ReadNw(f, size, dst);
    do {
        st = ADXF_GetStat(f);
        if (st == ADXF_STAT_ERROR) {
            ADXF_Seek(f, 0, 0);
            ADXF_ReadNw(f, func_001CA0B8(f), dst);
        }
    } while (st != ADXF_STAT_READEND);
    size = func_001CA0B8(f) << 11;
    func_001C9800(f);
    return size;
}

/* +0x3C the listing of registered folder `dir` (NULL: the root's); NULL if not registered */
/* 0x0016B1F0 */
void *Loader_DirListing(u8 *l, const char *dir) {
    s32 i;

    if (dir == NULL) {
        return l + 0x1FB80;
    }
    for (i = 0; i < LOADER_DIRS; i++) {
        if (func_00118278(dir, LOADER_DIR_NAME(l, i)) == 0) {
            return LOADER_DIR_LIST(l, i);
        }
    }
    return NULL;
}

/* ---- asynchronous requests ---- */

/* a request (ring of 256 at +4; read index +0x12804, write index +0x12805) */
typedef struct LoadReq {
    /* 0x000 */ u32 unk0;
    /* 0x004 */ s32 state;      /* 0 free, 1/2 open, 3 read, 4 reading, 5 sound driver busy, 6/7 stop */
    /* 0x008 */ void *file;
    /* 0x00C */ void *dir;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ s32 group;      /* the requester (cancelled together: +0x14); 0x10000000 none */
    /* 0x018 */ u32 dst;
    /* 0x01C */ s32 notify;     /* hand the data on when done */
    /* 0x020 */ char name[0x100];
    /* 0x120 */ u8 kind;        /* 0: callback (Lzss_Start); else a sound bank part (| 0x80) */
    /* 0x121 */ u8 bank;
    /* 0x122 */ u8 pad122[2];
    /* 0x124 */ u32 size;
} LoadReq;

_Static_assert(sizeof(LoadReq) == 0x128, "LoadReq");

#define LOADER_REQ(l, i) ((LoadReq *)((u8 *)(l) + 4) + (u8)(i))
#define LOADER_RD(l) AT(l, 0x12804, u8)
#define LOADER_WR(l) AT(l, 0x12805, u8)

static inline void LoadReq_Clear(LoadReq *q) {
    *(volatile s32 *)&q->state = 0;   /* (also when it already is: the PS2 code stores it) */
    q->file = NULL;
    q->dir = NULL;
    q->unk10 = 0;
    q->group = 0x10000000;
    q->notify = 0;
    q->dst = 0;
    q->name[0] = 0;
    q->kind = 0;
    q->size = 0;
}

/* finish the request at the read index and move on */
static inline void Loader_Retire(u8 *l, LoadReq *q) {
    func_001C9800(q->file);
    LoadReq_Clear(q);
    LOADER_RD(l)++;
}

/* Per-frame tick: advance the request at the read index (open, read, wait, hand sound data to
 * the sound driver: +0x4C / +0x54 / +0x50 / +0x5C by kind, +0x70 = driver busy). */
/* 0x0016BFB0 */
void Loader_Tick(u8 *l) {
    VObject *drv = gSound;
    LoadReq *q;
    s32 n, st;

    for (;;) {
        if (LOADER_RD(l) == LOADER_WR(l)) {
            return;
        }
        if (LOADER_REQ(l, LOADER_WR(l) - 1)->state == 0) {   /* the newest request was dropped */
            LOADER_WR(l)--;
            continue;
        }
        q = LOADER_REQ(l, LOADER_RD(l));
        switch (q->state) {
        case 0:
            LoadReq_Clear(q);
            LOADER_RD(l)++;
            continue;
        case 1:
            q->state = 2;
            /* fallthrough */
        case 2:
            q->file = func_001C9438(q->name, q->dir);
            if (q->file != NULL) {
                q->size = func_001CA0B8(q->file) << 11;
            }
            if (q->file == NULL) {
                return;
            }
            q->state = 3;
            /* fallthrough */
        case 3:
            n = 0;
            if (q->file != NULL && (n = func_001CA0B8(q->file)) != 0) {
                n = ADXF_ReadNw(q->file, n, q->dst);
            }
            if (n == 0) {
                return;
            }
            q->state = 4;
            return;
        case 4:
            ADXF_GetStat(q->file);
            st = ADXF_GetStat(q->file);
            if (st == 1) {
                q->state = 3;
                continue;
            }
            if (st != ADXF_STAT_READEND) {
                return;
            }
            if (q->notify != 0 && q->size != 0) {
                if (q->kind == 0) {
                    Lzss_Start((u8 *)q);
                } else {
                    if ((VCALL(drv, 0x70, s32 (*)(VObject *, s32))(drv, q->bank) & 0xFF) == 1) {
                        return;
                    }
                    switch (q->kind & ~0x80) {
                    case 0:
                        VCALL(drv, 0x4C, void (*)(VObject *, s32, u32, u32))(drv, q->bank, q->dst, q->size);
                        break;
                    case 1:
                        VCALL(drv, 0x54, void (*)(VObject *, s32, u32, u32))(drv, q->bank, q->dst, q->size);
                        break;
                    case 2:
                        VCALL(drv, 0x50, void (*)(VObject *, s32, u32, u32))(drv, q->bank, q->dst, q->size);
                        break;
                    case 3:
                        VCALL(drv, 0x5C, void (*)(VObject *, s32, u32, u32))(drv, q->bank, q->dst, q->size);
                        q->state = 5;
                        break;
                    }
                }
            }
            if (q->state == 5) {
                return;
            }
            Loader_Retire(l, q);
            continue;
        case 5:
            if (VCALL(drv, 0x70, s32 (*)(VObject *, s32))(drv, q->bank) & 0xFF) {
                return;
            }
            Loader_Retire(l, q);
            continue;
        case 6:
            ADXF_StopNw(q->file);
            q->state = 7;
            /* fallthrough */
        case 7:
            ADXF_GetStat(q->file);
            st = ADXF_GetStat(q->file);
            if (st == ADXF_STAT_READEND || st == 1) {
                Loader_Retire(l, q);
                continue;
            }
            return;
        default:
            LoadReq_Clear(q);
            LOADER_RD(l)++;
            return;
        }
    }
}

#define LOADER_LASTID(l) AT(l, 0x12808, s32)

/* +0xC queue loading `path` into `dst` (flags `flags`): with `buf`, `dst` says what to do with
 * the data once loaded (bit 31: a sound bank part, kind bits 28-29, bank bits 24-27) and `buf`
 * is where it goes. Returns the request id (an equal request already queued: its id). */
/* 0x0016BBD0 */
s32 Loader_Queue(u8 *l, const char *path, u32 dst, s32 flags, u32 buf) {
    LoadReq *t = func_00100660(0x128), *q;
    u32 i;
    s32 id;

    t->state = 0;
    t->file = NULL;
    t->dir = NULL;
    t->unk10 = 0;
    t->group = 0x10000000;
    t->notify = 0;
    t->dst = 0;
    t->name[0] = 0;
    t->kind = 0;
    t->size = 0;
    t->group = flags;
    Loader_SplitPath((u8 *)t, path);
    if (buf != 0) {
        if (dst & 0x80000000) {
            t->notify = -1;
            t->kind = ((dst >> 28) & 3) | 0x80;
            t->bank = (dst >> 24) & 0xF;
        } else {
            t->notify = dst | UNCACHED_BIT;
        }
        t->dst = buf | UNCACHED_BIT;
    } else {
        t->dst = dst | UNCACHED_BIT;
        t->notify = 0;
    }
    /* the same request queued already? (from the write index back to the read index) */
    if (LOADER_RD(l) != LOADER_WR(l)) {
        for (i = LOADER_WR(l); i != LOADER_RD(l); i = (i - 1) & 0xFF) {
            q = LOADER_REQ(l, i);
            if (t->group == q->group && t->dir == q->dir && t->dst == q->dst && t->notify == q->notify &&
                t->kind == q->kind && t->bank == q->bank) {
                func_00100490(t);
                return q->unk10;
            }
        }
    }
    if (LOADER_WR(l) + 1 == LOADER_RD(l)) {   /* full: work until there is room */
        do {
            Loader_Tick(l);
        } while (LOADER_WR(l) + 1 == LOADER_RD(l));
    }
    while (++LOADER_LASTID(l) == 0) {
    }
    t->unk10 = LOADER_LASTID(l);
    t->unk0 = 3;
    t->state = 1;
    q = LOADER_REQ(l, LOADER_WR(l)++);
    q->unk0 = t->unk0;
    q->state = t->state;
    q->file = t->file;
    q->dir = t->dir;
    q->unk10 = t->unk10;
    q->group = t->group;
    q->dst = t->dst;
    q->notify = t->notify;
    for (i = 0; i < 0x80; i++) {
        q->name[i * 2] = t->name[i * 2];
        q->name[i * 2 + 1] = t->name[i * 2 + 1];
    }
    q->kind = t->kind;
    q->bank = t->bank;
    q->size = t->size;
    id = t->unk10;
    func_00100490(t);
    return id;
}

extern void *Loader_vtable[], *D_0046A220[];

/* the request in progress: stopped (with or without waiting) or, before it reads, dropped */
static void LoadReq_Cancel(LoadReq *q, s32 wait) {
    if ((u32)q->state < 6) {
        if ((u32)q->state < 3) {
            LoadReq_Clear(q);
        } else {
            q->state = 6;
            if (wait) {
                ADXF_Stop(q->file);
            } else {
                ADXF_StopNw(q->file);
            }
        }
    }
}

/* the queued requests (behind the one in progress) of `group` dropped */
static void Loader_DropGroup(u8 *l, s32 group) {
    u8 i;

    if (LOADER_RD(l) == LOADER_WR(l)) {
        return;
    }
    for (i = LOADER_RD(l) + 1; i != LOADER_WR(l); i++) {
        LoadReq *q = LOADER_REQ(l, i);

        if (q->group == group) {
            LoadReq_Clear(q);
        }
    }
}

/* +0x14 cancel the requests of `group`: the one in progress is stopped (or dropped if it
 * hasn't started reading), the queued ones dropped */
/* 0x0016B8E0 */
void Loader_CancelGroup(u8 *l, s32 group) {
    LoadReq *q = LOADER_REQ(l, LOADER_RD(l));

    if (q->group == group) {
        LoadReq_Cancel(q, 0);
    }
    Loader_DropGroup(l, group);
}

/* +0x18 the same, waiting for the read in progress to stop */
/* 0x0016B750 */
void Loader_CancelGroupWait(u8 *l, s32 group) {
    LoadReq *q = LOADER_REQ(l, LOADER_RD(l));

    if (q->group == group) {
        LoadReq_Cancel(q, 1);
    }
    Loader_DropGroup(l, group);
}

/* +0x1C cancel everything */
/* 0x0016B600 */
void Loader_CancelAll(u8 *l) {
    u8 i;

    LoadReq_Cancel(LOADER_REQ(l, LOADER_RD(l)), 0);
    if (LOADER_RD(l) == LOADER_WR(l)) {
        return;
    }
    for (i = LOADER_RD(l) + 1; i != LOADER_WR(l); i++) {
        LoadReq_Clear(LOADER_REQ(l, i));
    }
}

/* +0x10 cancel request `id` (+0x10 of the request; 0 none) */
/* 0x0016BA70 */
void Loader_Cancel(u8 *l, s32 id) {
    LoadReq *q;
    u8 i;

    if (id == 0) {
        return;
    }
    q = LOADER_REQ(l, LOADER_RD(l));
    if (q->unk10 == id) {
        LoadReq_Cancel(q, 0);
        return;
    }
    for (i = LOADER_RD(l); i != LOADER_WR(l); i++) {
        q = LOADER_REQ(l, i);
        if (q->unk10 == id) {
            LoadReq_Clear(q);
            return;
        }
    }
}

/* +0x20 the state of request `id`: ADXF's (2 while it isn't open yet), 3 (done) if it is no
 * longer queued */
/* 0x0016B550 */
s32 Loader_State(u8 *l, s32 id) {
    u8 i;

    for (i = LOADER_RD(l); i != LOADER_WR(l); i++) {
        LoadReq *q = LOADER_REQ(l, i);

        if (q->unk10 == id) {
            return q->file != NULL ? ADXF_GetStat(q->file) : 2;
        }
    }
    return 3;
}

/* the loader base: destructor */
static inline void LoaderBase_Destroy(VObject *l) {
    *(void ***)l = D_0046A220;
    if (l != NULL) {
        gFileLoader = NULL;
    }
}

/* 0x0016C840 */
void *LoaderBase_dtor(VObject *l, s32 flags) {
    if (l != NULL) {
        LoaderBase_Destroy(l);
        if ((s16)flags > 0) {
            func_00100490(l);
        }
    }
    return l;
}

/* the loader: destructor */
/* 0x00169280 */
void *Loader_dtor(VObject *l, s32 flags) {
    if (l != NULL) {
        *(void ***)l = Loader_vtable;
        if (l != NULL) {
            LoaderBase_Destroy(l);
        }
        if ((s16)flags > 0) {
            func_00100490(l);
        }
    }
    return l;
}

/* close the files of all 256 request slots */
/* 0x0016BF50 */
void Loader_CloseAll(u8 *l) {
    s32 i;

    for (i = 0; i < 256; i++) {
        LoadReq *q = (LoadReq *)(l + 4) + i;

        if (q->file != NULL) {
            func_001C9800(q->file);
        }
    }
}
