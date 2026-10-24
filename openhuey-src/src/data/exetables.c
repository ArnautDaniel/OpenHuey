/* The executable's tables: see exetables.h. Each table's address and size, as the engine has
 * read them: the sizes of the motion tables are their models' motion counts (6 bytes each). */
#include "exetables.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../core/files.h"

typedef struct Table {
    const char *name;
    uint32_t vaddr, size;   /* (size 0: a table of pointers to whole, below) */
    const uint8_t *bytes;   /* read: kept */
    size_t got;
    int tried;
} Table;

static Table sTables[] = {
    {"doors", 0x3DCFC0, 400 * 16, NULL, 0, 0},              /* kDoorDefs: two sides each, flags */
    {"rooms", 0x3D8BC0, 0x110 * 0x40, NULL, 0, 0},          /* D_003D8BC0: the rooms' exits */
    {"obstacle-kinds", 0x47A938, 0, NULL, 0, 0},            /* ObstacleKinds: pointers to each kind */
    {"butterflies", 0x412710, 32, NULL, 0, 0},
    {"sound-curves", 0x41D820, 0x600, NULL, 0, 0},          /* the 3D distance curves */
    {"sound-pan-right", 0x3DF580, 0x1004, NULL, 0, 0},      /* stereo pan by angle */
    {"sound-pan-left", 0x3E0580, 0x1004, NULL, 0, 0},
    {"sound-positioned", 0x3D8990, 16, NULL, 0, 0},         /* kPositionedSounds */
    {"fiona-meet-offsets", 0x3B2460, 16 * 12, NULL, 0, 0},  /* kFionaMeetOffsets: three floats a row */
    {"deal-rows", 0x3D6B20, 79 * 3, NULL, 0, 0},            /* D_003D6B20: three rooms a row */
    {"motions-fiona", 0x3D5CC0, 120 * 6, NULL, 0, 0},       /* the models' motion tables: fades, flags */
    {"motions-hewie", 0x3D5F90, 108 * 6, NULL, 0, 0},
    {"motions-debilitas", 0x3D89A0, 85 * 6, NULL, 0, 0},
    {"motions-daniella", 0x419E60, 80 * 6, NULL, 0, 0},
    {"motions-riccardo", 0x41A2C0, 82 * 6, NULL, 0, 0},
    {"motions-lorenzo", 0x4246C0, 74 * 6, NULL, 0, 0},
    {"motions-lorenzo2", 0x424490, 81 * 6, NULL, 0, 0},
    {"motions-tintstalker", 0x42AED0, 82 * 6, NULL, 0, 0},
    {"motions-kind12", 0x443FB0, 74 * 6, NULL, 0, 0},
    {"motions-hanging-lorenzo", 0x424250, 5 * 6, NULL, 0, 0},
    {"motions-crow", 0x429C10, 16 * 6, NULL, 0, 0},
    {"motions-homunculus", 0x42C730, 10 * 6, NULL, 0, 0},
    {"hand-poses", 0x4157A0, 16 * 5 * 12, NULL, 0, 0},     /* D_004157A0: a pose's 5 keys { shape, shape, weight } (Motion_Hands) */
    {"blink-curve", 0x415B60, 14 * 4, NULL, 0, 0},         /* D_00415B60: the eyelids through a blink (Motion_Eyes) */
};
#define NTABLES (int)(sizeof(sTables) / sizeof(sTables[0]))

static Exe sExe;

void exetables_use_exe(Exe *e) {
    exe_free(&sExe);
    sExe = *e;
    memset(e, 0, sizeof(*e));
}

const char *exetables_name(int i) { return i >= 0 && i < NTABLES ? sTables[i].name : NULL; }

static Table *find(const char *name) {
    int i;

    for (i = 0; i < NTABLES; i++) {
        if (strcmp(sTables[i].name, name) == 0) {
            return &sTables[i];
        }
    }
    return NULL;
}

static int32_t s32(const uint8_t *p) {
    int32_t v;

    memcpy(&v, p, 4);
    return v;
}

/* obstacle-kinds made whole: each kind's definition (two floats, a count n, then n parts of
 * four words) one after another, in the order of the pointers (to the first null) */
static uint8_t *obstacle_kinds(const Exe *e, size_t *size) {
    uint8_t *out = NULL;
    size_t n = 0;
    int k;

    for (k = 0; k < 64; k++) {
        const uint8_t *ptr = exe_at(e, 0x47A938 + (uint32_t)k * 4, 4), *def;
        int parts;
        size_t len;

        if (ptr == NULL || s32(ptr) == 0 || (def = exe_at(e, (uint32_t)s32(ptr), 12)) == NULL) {
            break;
        }
        parts = s32(def + 8);
        len = 12 + 16 * (size_t)(parts >= 1 && parts <= 3 ? parts : 0);
        if ((def = exe_at(e, (uint32_t)s32(ptr), len)) == NULL) {
            break;
        }
        out = realloc(out, n + len);
        memcpy(out + n, def, len);
        n += len;
    }
    *size = n;
    return out;
}

uint8_t *exetables_extract(const Exe *e, const char *name, size_t *size) {
    const Table *t = find(name);
    const uint8_t *p;
    uint8_t *out;

    if (t == NULL) {
        return NULL;
    }
    if (t->size == 0) {
        return obstacle_kinds(e, size);
    }
    if ((p = exe_at(e, t->vaddr, t->size)) == NULL || (out = malloc(t->size)) == NULL) {
        return NULL;
    }
    memcpy(out, p, t->size);
    *size = t->size;
    return out;
}

const uint8_t *exetable(const char *name, size_t *size) {
    Table *t = find(name);
    char path[128];

    if (t == NULL) {
        return NULL;
    }
    if (!t->tried) {   /* (once: the library's or a mod's file, else the executable) */
        uint8_t *b;

        t->tried = 1;
        snprintf(path, sizeof(path), "../exe/%s.bin", name);
        b = files_read(path, &t->got);
        if (b == NULL && sExe.data != NULL) {
            b = exetables_extract(&sExe, name, &t->got);
        }
        t->bytes = b;
    }
    if (t->bytes != NULL && size != NULL) {
        *size = t->got;
    }
    return t->bytes;
}
