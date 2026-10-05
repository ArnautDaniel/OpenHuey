/* The file loader (system +0x319900, vtable 0x46A1E0, global gFileLoader). Files are named
 * "FOLDER\FILE" after the data folders of DATA.CVM; each folder's directory listing is loaded
 * once at startup (CRI ROFS). Reads go through CRI ADXF (replaced on PC by
 * native/platform/crifs.c, which reads the extracted folder). */
#include "common.h"
#include "game.h"


#define LOADER_DIRS 208
#define LOADER_DIR_NAME(l, i) ((char *)(l) + 0x12810 + (i) * 0x104)
#define LOADER_DIR_LIST(l, i) ((u8 *)(l) + 0x25BC0 + (i) * 0x6A8)

extern s32 func_001E7380(const char *dir, void *list, s32 max);   /* ROFS_LoadDir (s16: 0 = done) */
extern s32 func_00118278(const char *a, const char *b);             /* strcmp */
extern char *func_001183C0(char *dst, const char *src);             /* strcpy */
extern s32 func_0026EDD0(char *buf, s32 size, const char *fmt, ...);   /* snprintf */
extern const char D_0044F7F0[];   /* "." (the root) */
extern VObject *D_0044F7F8;       /* the system object */
extern VObject *D_0044E560;       /* the sound driver */

/* init: 256 empty request slots, the root's listing, the folder slots' listing buffers */
void func_0016C530(u8 *l) {
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
s32 func_0016B2B0(u8 *l, const char *dir) {
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

extern const char D_0044F270[], D_0044F278[], D_0044F280[], D_0044F288[], D_0044F290[], D_0044F298[], D_0044F2A0[], D_0044F2A8[], D_0044F2B0[], D_0044F2B8[], D_0044F2C0[], D_0044F2C8[], D_0044F2D0[], D_0044F2D8[], D_0044F2E0[], D_0044F2E8[], D_0044F2F0[], D_0044F2F8[], D_0044F300[], D_0044F308[], D_0044F310[], D_0044F318[], D_0044F320[], D_0044F328[], D_0044F330[], D_0044F338[], D_0044F340[], D_0044F348[], D_0044F350[], D_0044F358[], D_0044F360[], D_0044F368[], D_0044F370[], D_0044F378[], D_0044F380[], D_0044F388[], D_0044F390[], D_0044F398[], D_0044F3A0[], D_0044F3A8[], D_0044F3B0[], D_0044F3B8[], D_0044F3C0[], D_0044F3C8[], D_0044F3D0[], D_0044F3D8[], D_0044F3E0[], D_0044F3E8[], D_0044F3F0[], D_0044F3F8[], D_0044F400[], D_0044F408[], D_0044F410[], D_0044F418[], D_0044F420[], D_0044F428[], D_0044F430[], D_0044F438[], D_0044F440[], D_0044F448[], D_0044F450[], D_0044F458[], D_0044F460[], D_0044F468[], D_0044F470[], D_0044F478[], D_0044F480[], D_0044F488[], D_0044F490[], D_0044F498[], D_0044F4A0[], D_0044F4A8[], D_0044F4B0[], D_0044F4B8[], D_0044F4C0[], D_0044F4C8[], D_0044F4D0[], D_0044F4D8[], D_0044F4E0[], D_0044F4E8[], D_0044F4F0[], D_0044F4F8[], D_0044F500[], D_0044F508[], D_0044F510[], D_0044F518[], D_0044F520[], D_0044F528[], D_0044F530[], D_0044F538[], D_0044F540[], D_0044F548[], D_0044F550[], D_0044F558[], D_0044F560[], D_0044F568[], D_0044F570[], D_0044F578[], D_0044F580[], D_0044F588[], D_0044F590[], D_0044F598[], D_0044F5A0[], D_0044F5A8[], D_0044F5B0[], D_0044F5B8[], D_0044F5C0[], D_0044F5C8[], D_0044F5D0[], D_0044F5D8[], D_0044F5E0[], D_0044F5E8[], D_0044F5F0[], D_0044F5F8[], D_0044F600[], D_0044F608[], D_0044F610[], D_0044F618[], D_0044F620[], D_0044F628[], D_0044F630[], D_0044F638[], D_0044F640[], D_0044F648[], D_0044F650[], D_0044F658[], D_0044F660[], D_0044F668[], D_0044F670[], D_0044F678[], D_0044F680[], D_0044F688[], D_0044F690[], D_0044F698[], D_0044F6A0[], D_0044F6A8[], D_0044F6B0[], D_0044F6B8[], D_0044F6C0[], D_0044F6C8[], D_0044F6D0[], D_0044F6D8[], D_0044F6E0[], D_0044F6E8[], D_0044F6F0[], D_0044F6F8[], D_0044F700[], D_0044F708[], D_0044F710[], D_0044F718[], D_0044F720[], D_0044F728[], D_0044F730[], D_0044F738[], D_0044F740[], D_0044F748[], D_0044F750[], D_0044F758[], D_0044F760[], D_0044F768[], D_0044F770[], D_0044F778[], D_0044F780[], D_0044F788[], D_0044F790[], D_0044F798[], D_0044F7A0[], D_0044F7A8[], D_0044F7B0[], D_0044F7B8[], D_0044F7C0[], D_0044F7C8[];

/* the data folders registered at startup after ST_000..ST_108, in the original order */
static const char *const sLoaderDirs[] = {
    D_0044F270 /* BGM */, D_0044F278 /* MAP */, D_0044F280 /* DE0000 */, D_0044F288 /* DE0001 */,
    D_0044F290 /* DE0002 */, D_0044F298 /* DE0003 */, D_0044F2A0 /* DE0004 */,
    D_0044F2A8 /* DE0005 */, D_0044F2B0 /* DE0006 */, D_0044F2B8 /* DE0007 */,
    D_0044F2C0 /* DE0008 */, D_0044F2C8 /* DE0009 */, D_0044F2D0 /* DE000A */,
    D_0044F2D8 /* DE000B */, D_0044F2E0 /* DE000C */, D_0044F2E8 /* DE000D */,
    D_0044F2F0 /* DE000E */, D_0044F2F8 /* DE000F */, D_0044F300 /* DE0010 */,
    D_0044F308 /* DE0011 */, D_0044F310 /* DE0012 */, D_0044F318 /* DE0013 */,
    D_0044F320 /* DE0014 */, D_0044F328 /* DE0015 */, D_0044F330 /* DE0016 */,
    D_0044F338 /* DE0017 */, D_0044F340 /* DE0018 */, D_0044F348 /* DE0019 */,
    D_0044F350 /* DE001A */, D_0044F358 /* DE001B */, D_0044F360 /* DE001C */,
    D_0044F368 /* DE001D */, D_0044F370 /* DE001E */, D_0044F378 /* DE001F */,
    D_0044F380 /* DE0020 */, D_0044F388 /* DE0021 */, D_0044F390 /* DE0022 */,
    D_0044F398 /* DE0023 */, D_0044F3A0 /* DE0024 */, D_0044F3A8 /* DE0025 */,
    D_0044F3B0 /* DE0026 */, D_0044F3B8 /* DE0027 */, D_0044F3C0 /* DE0028 */,
    D_0044F3C8 /* DE0029 */, D_0044F3D0 /* DE002A */, D_0044F3D8 /* DE002B */,
    D_0044F3E0 /* DE002C */, D_0044F3E8 /* DE002D */, D_0044F3F0 /* DE002E */,
    D_0044F3F8 /* DE002F */, D_0044F400 /* DE0030 */, D_0044F408 /* DE0031 */,
    D_0044F410 /* DE0032 */, D_0044F418 /* DE0033 */, D_0044F420 /* DE0034 */,
    D_0044F428 /* DE0035 */, D_0044F430 /* DE0036 */, D_0044F438 /* DE0039 */,
    D_0044F440 /* DE003B */, D_0044F448 /* DE003C */, D_0044F450 /* DE003D */,
    D_0044F458 /* DE003F */, D_0044F460 /* DE0040 */, D_0044F468 /* DE0041 */,
    D_0044F470 /* DE0042 */, D_0044F478 /* DE0043 */, D_0044F480 /* DE0044 */,
    D_0044F488 /* DE0045 */, D_0044F490 /* DE0046 */, D_0044F498 /* DE0047 */,
    D_0044F4A0 /* DE0048 */, D_0044F4A8 /* DE0049 */, D_0044F4B0 /* DE1000 */,
    D_0044F4B8 /* DE100B */, D_0044F4C0 /* DE1018 */, D_0044F4C8 /* DE101F */,
    D_0044F4D0 /* DE1025 */, D_0044F4D8 /* DE102E */, D_0044F4E0 /* DE1033 */,
    D_0044F4E8 /* DE1034 */, D_0044F4F0 /* DE1035 */, D_0044F4F8 /* DE2018 */,
    D_0044F500 /* DE201F */, D_0044F508 /* DE2025 */, D_0044F510 /* DE2035 */,
    D_0044F518 /* DE3035 */, D_0044F520 /* DE4035 */, D_0044F528 /* EV0002 */,
    D_0044F530 /* EV0003 */, D_0044F538 /* EV0004 */, D_0044F540 /* EV0005 */,
    D_0044F548 /* EV0006 */, D_0044F550 /* EV0007 */, D_0044F558 /* EV0008 */,
    D_0044F560 /* EV0009 */, D_0044F568 /* EV0010 */, D_0044F570 /* EV0011 */,
    D_0044F578 /* EV0012 */, D_0044F580 /* EV0013 */, D_0044F588 /* EV0014 */,
    D_0044F590 /* EV0015 */, D_0044F598 /* EV0016 */, D_0044F5A0 /* EV0017 */,
    D_0044F5A8 /* EV0018 */, D_0044F5B0 /* EV0019 */, D_0044F5B8 /* EV0020 */,
    D_0044F5C0 /* EV0021 */, D_0044F5C8 /* EV0022 */, D_0044F5D0 /* EV0023 */,
    D_0044F5D8 /* EV0024 */, D_0044F5E0 /* EV0025 */, D_0044F5E8 /* EV0026 */,
    D_0044F5F0 /* EV0027 */, D_0044F5F8 /* EV0028 */, D_0044F600 /* EV0029 */,
    D_0044F608 /* EV0030 */, D_0044F610 /* EV0031 */, D_0044F618 /* EV1031 */,
    D_0044F620 /* EV0032 */, D_0044F628 /* EV0033 */, D_0044F630 /* EV1033 */,
    D_0044F638 /* EV0034 */, D_0044F640 /* EV1034 */, D_0044F648 /* ITEM00 */,
    D_0044F650 /* ITEM01 */, D_0044F658 /* ITEM02 */, D_0044F660 /* ITEM03 */,
    D_0044F668 /* ITEM04 */, D_0044F670 /* O_CRW */, D_0044F678 /* O_DB0 */,
    D_0044F680 /* O_DB1 */, D_0044F688 /* O_DB2 */, D_0044F690 /* O_DNL */, D_0044F698 /* O_DNT */,
    D_0044F6A0 /* O_FIB */, D_0044F6A8 /* O_FIC */, D_0044F6B0 /* O_FIF */, D_0044F6B8 /* O_FIH */,
    D_0044F6C0 /* O_FIM */, D_0044F6C8 /* O_FIN */, D_0044F6D0 /* O_FIN_M */,
    D_0044F6D8 /* O_FIO */, D_0044F6E0 /* O_FIS */, D_0044F6E8 /* O_FIW */, D_0044F6F0 /* O_FS0 */,
    D_0044F6F8 /* O_FS1 */, D_0044F700 /* O_FS2 */, D_0044F708 /* O_GLM */, D_0044F710 /* O_HED */,
    D_0044F718 /* O_HEG */, D_0044F720 /* O_HEW */, D_0044F728 /* O_HMA */, D_0044F730 /* O_HMB */,
    D_0044F738 /* O_HND */, D_0044F740 /* O_LRC */, D_0044F748 /* O_LRF */, D_0044F750 /* O_LRH */,
    D_0044F758 /* O_LRM */, D_0044F760 /* O_LRO */, D_0044F768 /* O_LRY */, D_0044F770 /* O_RBT */,
    D_0044F778 /* O_RCG */, D_0044F780 /* O_RCT */, D_0044F788 /* O_SGM */, D_0044F790 /* O_SHT */,
    D_0044F798 /* O_WHC */, D_0044F7A0 /* O_WIR */, D_0044F7A8 /* ADX00 */, D_0044F7B0 /* ADX01 */,
    D_0044F7B8 /* ADX02 */, D_0044F7C0 /* SYSTEM */, D_0044F7C8 /* SUBSCR */,
};

extern const char D_0044F268[];   /* "ST_%03X" */

/* register a folder, idling the system (+0x1C) until there is room */
static inline void Loader_AddDir(u8 *l, VObject *sys, const char *dir) {
    do {
        VCALL(sys, 0x1C, void (*)(VObject *))(sys);
    } while (func_0016B2B0(l, dir) < 0);
}

/* Register all data folders: the stages ST_000..ST_108, then the rest. */
void func_00169680(u8 *l) {
    VObject *sys = D_0044F7F8;
    char name[0x100];
    u32 i;

    for (i = 0; i < 0x110; i += 8) {
        func_0026EDD0(name, sizeof(name), D_0044F268, i);
        Loader_AddDir(l, sys, name);
    }
    for (i = 0; i < sizeof(sLoaderDirs) / sizeof(sLoaderDirs[0]); i++) {
        Loader_AddDir(l, sys, sLoaderDirs[i]);
    }
}

/* a request: the folder's listing (+0xC) and the file name (+0x20) */
#define REQ_DIR(q) AT(q, 0xC, void *)
#define REQ_NAME(q) ((char *)(q) + 0x20)

extern VObject *gFileLoader;
extern char *func_00118978(char *dst, const char *src, u32 n);   /* strncpy */
extern void *func_00100660(u32 size);                          /* operator new */
extern void func_00100490(void *p);                            /* operator delete */
extern void *func_001C9438(const char *name, void *dir);         /* ADXF open in a folder */
extern void func_001C9800(void *f);                              /* ADXF close */
extern s32 func_001CA0B8(void *f);                               /* ADXF file size (sectors) */
extern void ADXF_Seek(void *f, s32 pos, s32 type);
extern s32 ADXF_ReadNw(void *f, s32 nsct, u32 buf);
extern s32 ADXF_GetStat(void *f);

#define ADXF_STAT_READEND 3
#define ADXF_STAT_ERROR 4

/* Split "FOLDER\FILE" into the folder's listing (+0x3C lookup; none: the root) and the name. */
void func_001694D0(u8 *q, const char *path) {
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

/* +0x2C size of file `path` in sectors (0: not found) */
s32 func_00169450(VObject *l, const char *path) {
    u8 *q = func_00100660(0x128);
    void *f;
    s32 n;

    func_001694D0(q, path);
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
u32 func_00169420(VObject *l, const char *path) {
    return VCALL(l, 0x2C, s32 (*)(VObject *, const char *))(l, path) << 11;
}

/* +0x34 load all of file `path` into `dst` (waiting; retried on read errors); returns its size */
u32 func_001692F0(VObject *l, const char *path, u32 dst) {
    u8 *q = func_00100660(0x128);
    void *f;
    s32 st;
    u32 size;

    func_001694D0(q, path);
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
void *func_0016B1F0(u8 *l, const char *dir) {
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
    /* 0x120 */ u8 kind;        /* 0: callback (func_001695D0); else a sound bank part (| 0x80) */
    /* 0x121 */ u8 bank;
    /* 0x122 */ u8 pad122[2];
    /* 0x124 */ u32 size;
} LoadReq;

_Static_assert(sizeof(LoadReq) == 0x128, "LoadReq");

#define LOADER_REQ(l, i) ((LoadReq *)((u8 *)(l) + 4) + (u8)(i))
#define LOADER_RD(l) AT(l, 0x12804, u8)
#define LOADER_WR(l) AT(l, 0x12805, u8)

extern void func_001695D0(LoadReq *q);
extern void ADXF_StopNw(void *f);

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
void func_0016BFB0(u8 *l) {
    VObject *drv = D_0044E560;
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
                    func_001695D0(q);
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
s32 func_0016BBD0(u8 *l, const char *path, u32 dst, s32 flags, u32 buf) {
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
    func_001694D0((u8 *)t, path);
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
            func_0016BFB0(l);
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

extern void ADXF_Stop(void *f);
extern void *D_0046A1E0[], *D_0046A220[];

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
void func_0016B8E0(u8 *l, s32 group) {
    LoadReq *q = LOADER_REQ(l, LOADER_RD(l));

    if (q->group == group) {
        LoadReq_Cancel(q, 0);
    }
    Loader_DropGroup(l, group);
}

/* +0x18 the same, waiting for the read in progress to stop */
void func_0016B750(u8 *l, s32 group) {
    LoadReq *q = LOADER_REQ(l, LOADER_RD(l));

    if (q->group == group) {
        LoadReq_Cancel(q, 1);
    }
    Loader_DropGroup(l, group);
}

/* +0x1C cancel everything */
void func_0016B600(u8 *l) {
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
void func_0016BA70(u8 *l, s32 id) {
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
s32 func_0016B550(u8 *l, s32 id) {
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

void *func_0016C840(VObject *l, s32 flags) {
    if (l != NULL) {
        LoaderBase_Destroy(l);
        if ((s16)flags > 0) {
            func_00100490(l);
        }
    }
    return l;
}

/* the loader: destructor */
void *func_00169280(VObject *l, s32 flags) {
    if (l != NULL) {
        *(void ***)l = D_0046A1E0;
        if (l != NULL) {
            LoaderBase_Destroy(l);
        }
        if ((s16)flags > 0) {
            func_00100490(l);
        }
    }
    return l;
}
