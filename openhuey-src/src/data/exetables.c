/* The executable's tables: see exetables.h. Each table's address and size, as the engine has
 * read them: the sizes of the motion tables are their models' motion counts (6 bytes each). */
#include "exetables.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../core/files.h"

typedef struct Table {
    const char *name;
    uint32_t vaddr, vaddr_eu;   /* its address in SLUS_210.75, and in SLES_528.77 (the European disc's) */
    uint32_t size;              /* (0: a table of pointers to whole, below) */
    const uint8_t *bytes;   /* read: kept */
    size_t got;
    int tried;
} Table;

static Table sTables[] = {
    {"doors", 0x3DCFC0, 0x3DD010, 400 * 16, NULL, 0, 0},           /* kDoorDefs: two sides each, flags */
    {"rooms", 0x3D8BC0, 0x3D8C10, 0x110 * 0x40, NULL, 0, 0},       /* D_003D8BC0: the rooms' exits */
    {"obstacle-kinds", 0x47A938, 0x47CEA0, 0, NULL, 0, 0},         /* ObstacleKinds: pointers to each kind */
    {"butterflies", 0x412710, 0x410E80, 32, NULL, 0, 0},
    {"sound-curves", 0x41D820, 0x41A450, 0x600, NULL, 0, 0},       /* the 3D distance curves */
    {"sound-pan-right", 0x3DF580, 0x3DF5D0, 0x1004, NULL, 0, 0},   /* stereo pan by angle */
    {"sound-pan-left", 0x3E0580, 0x3E05D0, 0x1004, NULL, 0, 0},
    {"sound-positioned", 0x3D8990, 0x3D89E0, 16, NULL, 0, 0},      /* kPositionedSounds */
    /* the placement's output 2 (the game's own: SndDriver_Start) */
    {"sound-surround-left", 0x3E4C00, 0x3E4C50, 360 * 4, NULL, 0, 0},
    {"sound-surround-right", 0x3E4660, 0x3E46B0, 360 * 4, NULL, 0, 0},
    {"sound-surround-index", 0x3E3580, 0x3E35D0, 360 * 4, NULL, 0, 0}, /* kSurroundIndex: by degree */
    {"fiona-meet-offsets", 0x3B2460, 0x3B35B0, 16 * 12, NULL, 0, 0}, /* kFionaMeetOffsets: three floats a row */
    {"deal-rows", 0x3D6B20, 0x4465A0, 79 * 3, NULL, 0, 0},         /* D_003D6B20: three rooms a row */
    {"motions-fiona", 0x3D5CC0, 0x3D6DB0, 120 * 6, NULL, 0, 0},    /* the models' motion tables: fades, flags */
    {"motions-hewie", 0x3D5F90, 0x3D7080, 108 * 6, NULL, 0, 0},
    {"motions-debilitas", 0x3D89A0, 0x3D89F0, 85 * 6, NULL, 0, 0},
    {"motions-daniella", 0x419E60, 0x416A90, 80 * 6, NULL, 0, 0},
    {"motions-riccardo", 0x41A2C0, 0x416EF0, 82 * 6, NULL, 0, 0},
    {"motions-lorenzo", 0x4246C0, 0x41EB60, 74 * 6, NULL, 0, 0},
    {"motions-lorenzo2", 0x424490, 0x41E930, 81 * 6, NULL, 0, 0},
    {"motions-tintstalker", 0x42AED0, 0x425330, 82 * 6, NULL, 0, 0},
    {"motions-kind12", 0x443FB0, 0x43D640, 74 * 6, NULL, 0, 0},
    {"motions-hanging-lorenzo", 0x424250, 0x41E6F0, 5 * 6, NULL, 0, 0},
    {"motions-crow", 0x429C10, 0x4240B0, 3 * 6, NULL, 0, 0},           /* D_00429C10: its 3 motions (0x18 with its padding) */
    {"motions-homunculus", 0x42C730, 0x426B90, 10 * 6, NULL, 0, 0},
    /* D_004157A0: a pose's 5 keys { shape, shape, weight } (Motion_Hands) */
    {"hand-poses", 0x4157A0, 0x4123D0, 16 * 5 * 12, NULL, 0, 0},
    /* D_0044AD00: the small font's glyphs, by the glyph less $20 (its $1D ones from $E0): high
     * nibble the blank columns at its right, low nibble its drop */
    {"glyph-shapes", 0x44AD00, 0x444330, 0x100, NULL, 0, 0},
    {"box-places", 0x44B010, 0x444660, 8 * 4, NULL, 0, 0},         /* D_0044B010: the box's places (0x0A n): x, y */
    {"blink-curve", 0x415B60, 0x412790, 14 * 4, NULL, 0, 0},       /* D_00415B60: the eyelids through a blink */
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

static uint32_t table_at(const Exe *e, const Table *t) { return e->european ? t->vaddr_eu : t->vaddr; }

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
        const uint8_t *ptr = exe_at(e, table_at(e, find("obstacle-kinds")) + (uint32_t)k * 4, 4), *def;
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
    if ((p = exe_at(e, table_at(e, t), t->size)) == NULL || (out = malloc(t->size)) == NULL) {
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
        if (b != NULL && t->got < t->size) {   /* (shorter than the table: its readers would go past it) */
            fprintf(stderr, "exe: library/exe/%s.bin is %zu bytes, the table %u - the executable's used\n", name, t->got,
                    (unsigned)t->size);
            free(b);
            b = NULL;
        }
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
