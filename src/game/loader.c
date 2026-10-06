/* The file loader (system +0x319900, vtable 0x46A1E0, global gFileLoader). Files are named
 * "FOLDER\FILE" after the data folders of DATA.CVM; each folder's directory listing is loaded
 * once at startup (CRI ROFS). Reads go through CRI ADXF (replaced on PC by
 * native/platform/crifs.c, which reads the extracted folder).
 *
 * (was loading.c) The loading screen: a turning emblem (from GAME_FIX.GFM) and the "Now Loading"
 * box, drawn while a room loads. The emblem is a static model drawn through VU1.
 */
#include "common.h"
#include "game.h"
#include "globals.h"
#include "ptmf.h"
#include "actor.h"
#include "loader.h"
#include "cri/crifs.h"
#include "libc.h"
#include "msl.h"
#include "gs.h"
#include "sce/libvu0.h"
#include "hewie.h"
#include "renderer.h"
#include <stdint.h>
#include "text.h"
#include "gl2d.h"
#include "navmesh.h"
#include "progress.h"
#include "memcard.h"
#include "pursuer.h"
#include "effectmgr.h"
#include "charaction.h"
#include "music.h"
#include "camera.h"
#include "heap.h"
#include "char_load.h"
#include "creature.h"
#include "doors.h"
#include "event.h"
#include "gameover.h"
#include "items.h"
#include "model.h"
#include "movie.h"
#include "fiona.h"
#include "pause.h"
#include "placed.h"
#include "room_map.h"
#include "system.h"
#include "scene.h"
#include "scene_game.h"
#include "scene_title.h"
#include "draw_leaves.h"
#include "sce/eekernel.h"
#include "effects.h"
#include "sound.h"
#include "vecmath.h"
#include "daniella.h"
#include "pad.h"
#include "scene_boot.h"
#include "sce/iop.h"
#include "cri/adx.h"
#include "subscreen.h"
#include "input.h"
#include "sce/intc.h"
#include "sce/libmc.h"
#include "sce/libpad2.h"
#include "sce/sif.h"
#include "ps2hw.h"
#ifdef HG_NATIVE
#include <stdlib.h>
#include "glr.h"
#endif

#define LOADER_DIRS 208
#define LOADER_DIR_NAME(l, i) ((char *)(l) + 0x12810 + (i) * 0x104)
#define LOADER_DIR_LIST(l, i) ((u8 *)(l) + 0x25BC0 + (i) * 0x6A8)

extern const char D_0044F7F0[];   /* "." (the root) */

#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

void Lzss_Start(u8 *p);

extern void *Helper469D00_vtable[];
extern void *Bloom_vtable[];
#define PI 0x1.921fb6p+1f

#define TWO_PI 0x1.921fb6p+2f

extern void *D_00476F40[];
void *LoadingEmblem_dtor(u8 *o, s32 flags);

s32 Loader_CurrentDone(u8 *p);
s32 Loader_Slot28(u8 *p, s32 kind);
s32 Loader_Slot24(u8 *p);

/* an angle into -pi..pi */
static inline f32 wrap_angle(f32 a) {
    a = a - TWO_PI * (f32)(s32)(a / TWO_PI);
    if (!(a <= PI)) {
        a -= TWO_PI;
    }
    return a;
}

void Loading_DrawFrame(u8 *o, s32 frame);
u8 *gl_gfm_part(u8 *part, f32 (*mvp)[4], const u8 *tex, s32 csa);
s32 LoadingEmblem_Draw(u8 *o);

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
    while ((s16)ROFS_LoadDir(D_0044F7F0, l + 0x1FB80, 0x200) != 0) {
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
        } else if (msl_strcmp(dir, LOADER_DIR_NAME(l, i)) == 0) {
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
    while ((s16)ROFS_LoadDir(dir, LOADER_DIR_LIST(l, free), 0x23) != 0) {
    }
    msl_strcpy(LOADER_DIR_NAME(l, free), dir);
    return 0;
}

/* Entries of 0x128 bytes; +0x12804 current index, +0x12805 end index. */
/* 0x0016B420 */
s32 Loader_CurrentDone(u8 *p) {
    s32 v = FLD(p + p[0x12804] * 0x128, 0x8, s32);

    return v == 6 || v == 7;
}

/* 0x0016B470 */
s32 Loader_Slot28(u8 *p, s32 kind) {
    u8 end = p[0x12805];
    u8 i = p[0x12804];

    if (i == end) {
        return 3;
    }
    for (; i != end; i++) {
        if (FLD(p + i * 0x128, 0x18, s32) == kind) {
            return 2;
        }
    }
    return 3;
}

/* 0x0016B510 */
s32 Loader_Slot24(u8 *p) {
    if (p[0x12804] == p[0x12805]) {
        return 3;
    }
    return 2;
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
        msl_snprintf(name, sizeof(name), str_ST_N, i);
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
            msl_strcpy(REQ_NAME(q), path + i + 1);
            msl_strncpy(dir, path, i);
            dir[i] = 0;
            break;
        }
    }
    if (dir[0] != 0) {
        REQ_DIR(q) = VCALL(gFileLoader, 0x3C, void *(*)(VObject *, const char *))(gFileLoader, dir);
    } else {
        REQ_DIR(q) = VCALL(gFileLoader, 0x3C, void *(*)(VObject *, const char *))(gFileLoader, NULL);
        msl_strcpy(REQ_NAME(q), path);
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
    u8 *q = __nw__FUi(0x128);
    void *f;
    s32 n;

    Loader_SplitPath(q, path);
    f = ADXF_OpenInDir(REQ_NAME(q), REQ_DIR(q));
    __dl__FPv(q);
    if (f == NULL) {
        return 0;
    }
    n = ADXF_FileSectors(f);
    ADXF_Close(f);
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
    u8 *q = __nw__FUi(0x128);
    void *f;
    s32 st;
    u32 size;

    Loader_SplitPath(q, path);
    do {
        f = ADXF_OpenInDir(REQ_NAME(q), REQ_DIR(q));
    } while (f == NULL);
    __dl__FPv(q);
    if (f == NULL) {
        return 0;
    }
    size = ADXF_FileSectors(f);
    ADXF_Seek(f, 0, 0);
    dst |= UNCACHED_BIT;   /* uncached */
    ADXF_ReadNw(f, size, dst);
    do {
        st = ADXF_GetStat(f);
        if (st == ADXF_STAT_ERROR) {
            ADXF_Seek(f, 0, 0);
            ADXF_ReadNw(f, ADXF_FileSectors(f), dst);
        }
    } while (st != ADXF_STAT_READEND);
    size = ADXF_FileSectors(f) << 11;
    ADXF_Close(f);
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
        if (msl_strcmp(dir, LOADER_DIR_NAME(l, i)) == 0) {
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
    ADXF_Close(q->file);
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
            q->file = ADXF_OpenInDir(q->name, q->dir);
            if (q->file != NULL) {
                q->size = ADXF_FileSectors(q->file) << 11;
            }
            if (q->file == NULL) {
                return;
            }
            q->state = 3;
            /* fallthrough */
        case 3:
            n = 0;
            if (q->file != NULL && (n = ADXF_FileSectors(q->file)) != 0) {
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
    LoadReq *t = __nw__FUi(0x128), *q;
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
                __dl__FPv(t);
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
    __dl__FPv(t);
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
            __dl__FPv(l);
        }
    }
    return l;
}

/* destructor (vtable D_00476F40) */
/* 0x0033D990 */
void *LoadingEmblem_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00476F40;
        AT(o, 0x0, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

#ifdef HG_NATIVE
/* Draw the loading emblem `o` (renderer object: +0x10 position, +0x20 rotation, +0x30.. the
 * parts' extra rotations, +0x70 1: only the last part): GAME_FIX.GFM's first model, texture 2
 * of group 0x10, its parts chained, each turned and moved relative to the previous (palette:
 * the part's +0x4). */
/* 0x0033D9F0 */
s32 LoadingEmblem_Draw(u8 *o) {
    VObject *tc = gTexCache, *cam;
    f32 world[4][4] __attribute__((aligned(16)));
    f32 view[4][4] __attribute__((aligned(16)));
    f32 rel[4][4] __attribute__((aligned(16)));
    f32 extra[4][4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    f32 trans[4] __attribute__((aligned(16)));
    f32 rot[4] __attribute__((aligned(16)));
    s32 count, n;
    u8 *texh, *model, *part;

    if (VCALL(tc, 0x8, s32 (*)(VObject *, s32, s32))(tc, 2, 0x10) == -1) {
        return 0;
    }
    texh = VCALL(tc, 0xC, u8 *(*)(VObject *, s32, s32))(tc, 2, 0x10);
    model = VCALL(gGamePtr, 0x1C, u8 *(*)(VObject *))((VObject *)gGamePtr);
    if (AT(model, 0x0, s32) <= 0) {
        return 1;
    }
    part = model + AT(model, 0x0, s32);
    n = 0;
    count = AT(part, 0x8, s32);
    if (count == 0) {
        return 1;
    }
    cam = gCamera;
    do {
        u8 *next;

        count--;
        if (n == 0) {
            sceVu0CopyVector(rot, (f32 *)(o + 0x20));
            sceVu0UnitMatrix(m);
            sceVu0RotMatrix(m, m, rot);
            sceVu0CopyVector(trans, (f32 *)(o + 0x10));
        } else {
            sceVu0AddVector(rot, rot, (f32 *)(part + 0x30));
            rot[0] = wrap_angle(rot[0]);
            rot[1] = wrap_angle(rot[1]);
            rot[2] = wrap_angle(rot[2]);
            sceVu0UnitMatrix(rel);
            sceVu0RotMatrix(rel, rel, rot);
            sceVu0UnitMatrix(extra);
            if (n > 0) {
                sceVu0RotMatrix(extra, extra, (f32 *)(o + 0x30 + (n - 1) * 0x10));
            }
            sceVu0MulMatrix(rel, rel, extra);
            sceVu0MulMatrix(m, m, rel);
            sceVu0AddVector(trans, trans, (f32 *)(part + 0x40));
        }
        n++;
        if (AT(o, 0x70, s32) != 0 && count != 0) {
            next = part + AT(part, 0x1C, s32) + AT(part, 0x0, s32);   /* (only the last part drawn) */
        } else {
            sceVu0TransMatrix(world, m, trans);
            VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, view);
            sceVu0MulMatrix(view, view, world);
            next = gl_gfm_part(part, view, texh, AT(part, 0x4, s32));
        }
        part = (u8 *)(((uintptr_t)next + 15) & ~(uintptr_t)15);
    } while (count != 0);
    return 1;
}
#endif

/* the loading screen, frame `frame`: the camera at a fixed spot, the emblem (`o`, a model object)
 * turning (its rotation from the frame count) drawn in layers 1 and 0x26, the screen dimmed, and
 * the box for the "Now Loading" message (0x808B) */
/* 0x0033E2A0 */
void Loading_DrawFrame(u8 *o, s32 frame) {
    VObject *tc = gTexCache, *cam = gCamera, *r;
    u8 dim[0x10] __attribute__((aligned(16)));
    Task t;
    s32 box[13];
    s32 w, x;
    f32 a;

    VCALL(tc, 0x18, void (*)(VObject *))(tc);
    VCALL(cam, 0x1C, void (*)(VObject *, f32, f32, f32))(cam, 0x1.0e9ed6p+6f, 0x1.0948dep+5f, 0x1.12a69ap+6f);
    VCALL(cam, 0x5C, void (*)(VObject *, f32))(cam, 0x1.0c1524p-1f);   /* 30 degrees */
    VCALL(cam, 0x28, void (*)(VObject *, f32, f32, f32))(cam, -0x1.6a315cp+4f, 0x1.cacac0p+3f, 0x1.a37d6cp+2f);
    VCALL(cam, 0x14, void (*)(VObject *))(cam);

    AT(o, 0x10, f32) = 0.0f;
    AT(o, 0x14, f32) = 0.0f;
    AT(o, 0x18, f32) = 0.0f;
    AT(o, 0x1C, f32) = 1.0f;
    AT(o, 0x20, f32) = 0.0f;
    AT(o, 0x24, f32) = 0.0f;
    AT(o, 0x28, f32) = 0.0f;
    AT(o, 0x2C, f32) = 0.0f;
    AT(o, 0x30, f32) = Angle_Wrap((f32)frame * 0x1.45b3d6p-6f);
    AT(o, 0x34, f32) = 0.0f;
    AT(o, 0x38, f32) = 0.0f;
    AT(o, 0x3C, f32) = 0.0f;
    AT(o, 0x40, f32) = Angle_Wrap((f32)frame * 0x1.750728p-6f);
    AT(o, 0x44, f32) = 0.0f;
    AT(o, 0x48, f32) = 0.0f;
    AT(o, 0x4C, f32) = 0.0f;
    a = (f32)frame * 0x1.b43958p-6f;
    AT(o, 0x50, f32) = Angle_Wrap(a);
    AT(o, 0x54, f32) = 0.0f;
    AT(o, 0x58, f32) = 0.0f;
    AT(o, 0x5C, f32) = 0.0f;
    AT(o, 0x60, f32) = 0.0f;
    AT(o, 0x64, f32) = Angle_Wrap(a);
    AT(o, 0x68, f32) = 0.0f;
    AT(o, 0x6C, f32) = 0.0f;
    AT(o, 0x70, s32) = 0;
    r = gRenderer;
    VCALL(r, 0xC, void (*)(VObject *, void *, s32, s32))(r, o, 1, 0);
    AT(o, 0x70, s32) = 1;
    VCALL(r, 0xC, void (*)(VObject *, void *, s32, s32))(r, o, 0x26, 0);

    AT(dim, 0x0, void **) = Bloom_vtable;
    AT(dim, 0x4, s32) = -1;
    Bloom_Start(dim, 0x5A623C32, 0x28, 0);
    VCALL(tc, 0x18, void (*)(VObject *))(tc);

    Task_Construct(&t);
    w = Task_MessageWidth(&t, 0x808B, 0x10);
    x = 0x1B0;
    if (w >= 0x69) {
        x -= (w - 0x68) >> 1;
    }
    x -= w >> 1;
    box[5] = 0;
    box[2] = w + 0x28;
    box[0] = x - 0x14;
    box[3] = 0x20;
    box[1] = 0x198;
    box[7] = 0x20;
    box[4] = 0xFF;
    box[6] = 0x90;
    box[8] = 0x80808080;
    box[9] = 2;
    box[10] = 0x10;
    box[11] = 0x30;
    box[12] = 6;
    VCALL(r, 0x80, void (*)(VObject *, s32 *))(r, box);
    if (t.child != NULL) {
        Task_dtor(t.child, 1);
        t.child = NULL;
    }
    AT(dim, 0x0, void **) = Helper469D00_vtable;
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
            __dl__FPv(l);
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
            ADXF_Close(q->file);
        }
    }
}

#ifdef HG_NATIVE
/* One part of a GAME_FIX.GFM model with OpenGL, as the static model microprogram (D_003AC3F0)
 * draws it: `nv` vertices of UVs (V2-16, / 32768), colours (V4-8, 0x80 = 1.0), positions (V3-16
 * running differences from the part's start, +0x20; / 16) and strip flags (S-8: set, the
 * triangle ending there is skipped), one textured, blended triangle strip; `mvp` the camera's
 * +0x48 by the part's world matrix, `tex` its .TEX entry with palette `csa`. Returns the end of
 * its flags (the next part follows, 16-byte aligned). */
u8 *gl_gfm_part(u8 *part, f32 (*mvp)[4], const u8 *tex, s32 csa) {
    s32 nv = AT(part, 0x0, s32), i;
    const u16 *uv = (const u16 *)(part + AT(part, 0x10, s32));
    const u8 *col = part + AT(part, 0x14, s32);
    const s16 *pos = (const s16 *)(part + AT(part, 0x18, s32));
    u8 *flags = part + AT(part, 0x1C, s32);
    s32 row[3];
    f32 *xyzw, *st;

    if (nv <= 0) {
        return flags;
    }
    row[0] = AT(part, 0x20, s32);
    row[1] = AT(part, 0x24, s32);
    row[2] = AT(part, 0x28, s32);
    xyzw = malloc((u32)nv * 16);
    st = malloc((u32)nv * 8);
    for (i = 0; i < nv; i++) {
        u32 fl = flags[i] ? 0x8000 : 0;

        row[0] += pos[i * 3];
        row[1] += pos[i * 3 + 1];
        row[2] += pos[i * 3 + 2];
        xyzw[i * 4] = (f32)row[0] / 16.0f;
        xyzw[i * 4 + 1] = (f32)row[1] / 16.0f;
        xyzw[i * 4 + 2] = (f32)row[2] / 16.0f;
        AT(&xyzw[i * 4 + 3], 0, u32) = fl;
        st[i * 2] = (f32)uv[i * 2] / 32768.0f;
        st[i * 2 + 1] = (f32)uv[i * 2 + 1] / 32768.0f;
    }
    glr_strip((const f32 *)mvp, nv, xyzw, st, col, tex, 1ull << 34 | (u64)(csa & 0x1F) << 56, 0x10 | 0x40);
    free(xyzw);
    free(st);
    return flags + nv;
}
#endif
