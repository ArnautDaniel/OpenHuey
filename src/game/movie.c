/* Movies (CRI Sofdec .SFD): the player library wrapper (ADX sound system +0x7C44, global
 * D_0044FEF8, vtable D_0046C740) and the movie scene. The mwPly* calls are CRI's library
 * (native/platform/sofdec.c on PC). */
#include "common.h"
#include "ptmf.h"

#define AT(p, off, type) (*(type *)((u8 *)(p) + (off)))

typedef struct MovieLib {
    /* 0x00 */ void **vtbl;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ u8 init[0x20];     /* mwPlyInitSfdFx parameters: f32 fps, s32 1, s32 1, u8 1 */
    /* 0x28 */ u8 create[0x30];   /* creation parameters (mwPlyCreateSofdec) */
} MovieLib;
_Static_assert(__builtin_offsetof(MovieLib, create) == 0x28, "MovieLib.create");

#define CPRM(lib, off, type) (*(type *)((lib)->create + (off) - 0x28))

extern void *D_0046C740[];
extern void *D_0046AED0[];
extern MovieLib *D_0044FEF8;
extern void func_00100490(void *p);   /* operator delete */
extern void func_00115D20(void *p, s32 c, u32 n);   /* memset */
extern void mwPlyInitSfdFx(void *prm);
extern s32 mwPlyCalcWorkCprmSfd(void *cprm);
extern void func_0023AE40(void);       /* mwPlyFinishSfdFx */
extern void *func_00238BF0(void *cprm);   /* mwPlyCreateSofdec */

/* +0x8 */
MovieLib *func_0020E740(MovieLib *lib, s32 flags) {
    if (lib != NULL) {
        lib->vtbl = D_0046C740;
        if (lib != NULL) {
            lib->vtbl = D_0046AED0;
            lib->unk4 = 0;
            if (lib != NULL) {
                D_0044FEF8 = NULL;
            }
        }
        if ((s16)flags > 0) {
            func_00100490(lib);
        }
    }
    return lib;
}

/* +0xC set up the library: 59.94 fields a second; default creation parameters (512 x 512 at
 * most, 6 Mbit/s) */
void func_002266F0(MovieLib *lib) {
    func_00115D20(lib->init, 0, 0x20);
    *(f32 *)(lib->init + 0x0) = 0x1.df851ep+5f;   /* 59.94 */
    *(s32 *)(lib->init + 0x4) = 1;
    *(s32 *)(lib->init + 0x8) = 1;
    lib->init[0xC] = 1;
    mwPlyInitSfdFx(lib->init);
    func_00115D20(lib->create, 0, 0x30);
    CPRM(lib, 0x48, s32) = 0;
    CPRM(lib, 0x28, s32) = 1;
    CPRM(lib, 0x2C, s32) = 6000000;
    CPRM(lib, 0x30, s32) = 0x200;
    CPRM(lib, 0x34, s32) = 0x200;
    CPRM(lib, 0x38, s32) = 3;
    CPRM(lib, 0x3C, s32) = 3;
    CPRM(lib, 0x44, s32) = 0;
    CPRM(lib, 0x40, void *) = NULL;
}

/* +0x10 the work buffer (NULL: keep) */
void func_002266D0(MovieLib *lib, void *work) {
    if (work != NULL) {
        CPRM(lib, 0x40, void *) = work;
    }
}

/* +0x14 shut the library down */
void func_00226680(MovieLib *lib) {
    func_0023AE40();
    func_00115D20(lib->init, 0, 0x20);
    func_00115D20(lib->create, 0, 0x30);
}

/* +0x18 create a player (NULL without a work buffer or on failure) */
void *func_00226640(MovieLib *lib) {
    if (CPRM(lib, 0x40, void *) == NULL) {
        return NULL;
    }
    return func_00238BF0(lib->create);
}

/* +0x1C the movie's size and kind */
void func_00226620(MovieLib *lib, s32 w, s32 h, s32 a, s32 b) {
    CPRM(lib, 0x30, s32) = w;
    CPRM(lib, 0x34, s32) = h;
    CPRM(lib, 0x48, s32) = a;
    CPRM(lib, 0x4C, u8) = b;
}

/* +0x20 the work buffer size, rounded up to 64 bytes */
s32 func_002265D0(MovieLib *lib) {
    CPRM(lib, 0x44, s32) = mwPlyCalcWorkCprmSfd(lib->create);
    CPRM(lib, 0x44, s32) = (CPRM(lib, 0x44, s32) & ~0x3F) + ((CPRM(lib, 0x44, s32) & 0x3F) ? 0x40 : 0);
    return CPRM(lib, 0x44, s32);
}

/* ---- the movie scene: plays a .SFD (CAPCOM.SFD at boot, the opening ...) ---- */

#include "game.h"

typedef struct Movie {
    /* 0x000 */ Scene base;
    /* 0x014 */ VObject *ply;        /* the player (mwPly handle, NULL: none) */
    /* 0x018 */ u8 *frame;           /* the current frame: image, w at +0x20, h +0x24, no. +0x38 */
    /* 0x01C */ u8 frm1C[0x20 - 0x1C];
    /* 0x020 */ s32 frameW;
    /* 0x024 */ s32 frameH;
    /* 0x028 */ u8 frm28[0x38 - 0x28];
    /* 0x038 */ s32 frameNo;
    /* 0x03C */ u8 frm3C[0xA0 - 0x3C];
    /* 0x0A0 */ u8 stat;             /* the player's status: 2 playing, 3 / 4 ended */
    /* 0x0A1 */ u8 padA1[3];
    /* 0x0A4 */ void *work;          /* the work buffer, when allocated */
    /* 0x0A8 */ char name[0x100];    /* the file, empty: none */
    /* 0x1A8 */ s32 dir;             /* its folder (loader handle), 0: current */
    /* 0x1AC */ u8 keepRenderer;
    /* 0x1AD */ u8 pad1AD[3];
    /* 0x1B0 */ s32 time;
    /* 0x1B4 */ u8 hasFrame;
    /* 0x1B5 */ u8 unk1B5;
    /* 0x1B6 */ u8 pad1B6[2];
    /* 0x1B8 */ void *unk1B8;
    /* 0x1BC */ u8 mode;
    /* 0x1BD */ u8 pad1BD[3];
    /* 0x1C0 */ s32 shownFrame;      /* -1 none */
    /* 0x1C4 */ u8 loop;
    /* 0x1C5 */ u8 pad1C5[3];
    /* 0x1C8 */ f32 volume[5];       /* multiplied together */
} Movie;

_Static_assert(__builtin_offsetof(Movie, name) == 0xA8, "Movie.name");
_Static_assert(__builtin_offsetof(Movie, volume) == 0x1C8, "Movie.volume");

extern void *Scene_vtable[];
extern void *D_0046EA60[];       /* Movie */
extern void *D_0046ECC0[];       /* SceneMovie (the boot logo) */
extern Movie *D_0044E958;        /* the movie playing */
extern u8 *D_0044E978;           /* +0x38: the master volume */
extern u8 *gProgress;            /* +0x9F0: the movie volume option */
extern u8 *D_0045D1F0;
extern VObject *gFileLoader;
extern VObject *D_0044E4F0;      /* renderer */
extern void func_0011F9A0(void *p);           /* delete (scene heap) */
extern void *func_00114DA8(s32 align, s32 size);   /* memalign */
extern void func_00114FD0(void *p);           /* free */
extern char *func_001183C0(char *d, const char *s);   /* strcpy */
extern char *func_00118978(char *d, const char *s, s32 n);   /* strncpy */
extern f32 func_0031C830(f32 x);              /* log10f */
extern void func_001E7430(s32 a, s32 dir);    /* CRI file system: the current folder */
extern s32 func_0023C480(VObject *ply);       /* mwPly: playing time */
extern void func_00239828(VObject *ply, void *frame);   /* mwPly: get the current frame */
extern void func_0023A180(VObject *ply);      /* mwPly: release the current frame */
extern s32 func_0023B4B0(VObject *ply);
extern void func_0023CA88(VObject *ply, s32 mode);

void func_002B6E50(Movie *m);
void func_002B69B0(Movie *m);

static const PTMF sMovieEntry = {0, 0x10, {(void *)0}};   /* virtual +0x10 */

static inline void Movie_SetState(Movie *m, const PTMF *state) {
    PTMF s = *state;

    if (ptmf_test(&s)) {
        m->base.state = s;
    }
}

static inline void Movie_SetStateFn(Movie *m, void (*fn)(Movie *)) {
    PTMF s = {0, -1, {(void *)fn}};

    if (ptmf_test(&s)) {
        m->base.state = s;
    }
}

/* the output level in 0.1 dB, -96 dB for silence */
static inline s32 movie_level(Movie *m) {
    f32 v = m->volume[4] * (m->volume[3] * (m->volume[1] * (m->volume[0] * m->volume[2])));
    s32 db;

    if (v == 0.0f) {
        db = -960;
    } else {
        db = (s32)(100.0f * func_0031C830(v));
    }
    if (db > 0) {
        db = 0;
    }
    if (db < -960) {
        db = -960;
    }
    return db;
}


/* clear the flag */
void func_002B6310(Movie *m) {
    *D_0045D1F0 = 0;
    AT(m, 0x10, s32) = 0;
}

/* +0x18 */
s32 func_002B6330(Movie *m) {
    return 0;
}

/* apply the volume */
void func_002B6340(Movie *m) {
    s32 db = movie_level(m);

    if (m->ply != NULL) {
        VCALL(m->ply, 0x2C, void (*)(VObject *, s32))(m->ply, db);
    }
}

/* the player's status: 2 playing, 1 other, -1 ended or none */
s32 func_002B6410(Movie *m) {
    if (m->ply == NULL) {
        return -1;
    }
    switch (m->stat) {
    case 2:
        return func_0023B4B0(m->ply) ? 2 : 1;
    case 4:
        return -1;
    case 3:
        return -1;
    }
    return 1;
}

/* +0x24 */
void func_002B64A0(Movie *m) {
}

/* +0x20 draw the frame (full screen, layer 3) */
void func_002B64B0(Movie *m) {
    VCALL(D_0044E4F0, 0x40, void (*)(VObject *, u8 *, s32, s32, s32, s32))(
        D_0044E4F0, m->frame, 0x88000, m->frameW, m->frameH, 3);
}

/* the frame shown, -1 none */
s32 func_002B64F0(Movie *m) {
    if (m->hasFrame) {
        return m->shownFrame;
    }
    return -1;
}

/* +0x1C per frame: take the decoded frame and draw it; at the end, finish (or loop) */
void func_002B6510(Movie *m) {
    s32 t = func_0023C480(m->ply);

    if (t != m->time) {
        m->time = t;
    }
    func_00239828(m->ply, &m->frame);
    if (m->frame != NULL) {
        m->shownFrame = m->frameNo;
        VCALL(m, 0x20, void (*)(Movie *))(m);
        m->hasFrame = 1;
        func_0023A180(m->ply);
    }
    m->stat = VCALL(m->ply, 0x20, s32 (*)(VObject *))(m->ply);
    if ((u8)(m->stat - 3) < 2) {
        if (!m->loop) {
            VCALL(m, 0x14, void (*)(Movie *))(m);
        } else if (m->hasFrame) {
            m->shownFrame = -1;
            VCALL(m, 0x24, void (*)(Movie *))(m);
        }
    } else if (m->hasFrame) {
        VCALL(m, 0x24, void (*)(Movie *))(m);
    }
}

/* restart the file; 2 / 0 / -1 as func_002B6410 */
s32 func_002B6640(Movie *m) {
    if (m->ply == NULL || m->work == NULL) {
        VCALL(m, 0x14, void (*)(Movie *))(m);
        return -1;
    }
    m->stat = VCALL(m->ply, 0x20, s32 (*)(VObject *))(m->ply);
    if (m->stat == 2) {
        return func_0023B4B0(m->ply);
    }
    if (m->stat == 1) {
        return 0;
    }
    if (m->dir != 0) {
        func_001E7430(0, m->dir);
    }
    VCALL(m->ply, 0x18, void (*)(VObject *, char *))(m->ply, m->name);
    m->hasFrame = 0;
    m->unk1B5 = 0;
    return 0;
}

/* state: start the file */
void func_002B6BB0(Movie *m) {
    if (m->name[0] == 0) {
        return;
    }
    if (m->dir != 0) {
        func_001E7430(0, m->dir);
    }
    func_0023CA88(m->ply, 2);
    VCALL(m->ply, 0x18, void (*)(VObject *, char *))(m->ply, m->name);
    m->hasFrame = 0;
    m->unk1B5 = 0;
    m->volume[4] = 0.0f;
    {
        s32 db = movie_level(m);

        if (m->ply != NULL) {
            VCALL(m->ply, 0x2C, void (*)(VObject *, s32))(m->ply, db);
        }
    }
    Movie_SetStateFn(m, func_002B69B0);
}

/* set the file to play (folder\name, or just a name in the current folder) */
void func_002B6D10(Movie *m, const char *path, s32 mode, s32 keep) {
    char dir[256];
    s32 i;

    if (path[0] == 0) {
        return;
    }
    m->dir = 0;
    dir[0] = 0;
    for (i = 0; i < 256; i++) {
        if (path[i] == 0) {
            break;
        }
        if (path[i] == '\\') {
            func_001183C0(m->name, path + i + 1);
            func_00118978(dir, path, i);
            dir[i] = 0;
            break;
        }
    }
    if (dir[0] != 0) {
        m->dir = VCALL(gFileLoader, 0x3C, s32 (*)(VObject *, char *))(gFileLoader, dir);
        m->keepRenderer = keep;
    } else {
        m->dir = VCALL(gFileLoader, 0x3C, s32 (*)(VObject *, char *))(gFileLoader, NULL);
        func_001183C0(m->name, path);
        m->keepRenderer = keep;
    }
    m->time = 0;
    m->mode = mode;
    m->loop = 0;
}

/* +0x14 finish */
void func_002B6E50(Movie *m) {
    m->dir = 0;
    m->name[0] = 0;
    m->shownFrame = -1;
    m->base.request = SCENE_REQ_FINISH;
}

/* +0x10 entry: a player with its own work buffer, then start */
void func_002B6E70(Movie *m) {
    MovieLib *lib = D_0044FEF8;
    s32 size;

    VCALL(lib, 0x1C, void (*)(MovieLib *, s32, s32, s32, s32))(lib, 0x200, 0x1C0, 0x11, 0);
    size = VCALL(lib, 0x20, s32 (*)(MovieLib *))(lib);
    if (size == 0) {
        VCALL(m, 0x14, void (*)(Movie *))(m);
        return;
    }
    m->work = func_00114DA8(0x40, size);
    if (m->work == NULL) {
        VCALL(m, 0x14, void (*)(Movie *))(m);
        return;
    }
    lib = D_0044FEF8;
    VCALL(lib, 0x10, void (*)(MovieLib *, void *))(lib, m->work);
    m->ply = VCALL(lib, 0x18, VObject *(*)(MovieLib *))(lib);
    if (m->ply == NULL) {
        VCALL(m, 0x14, void (*)(Movie *))(m);
        return;
    }
    Movie_SetStateFn(m, func_002B6BB0);
}

/* +0x8 */
Movie *func_002B6FC0(Movie *m, s32 flags) {
    if (m != NULL) {
        m->base.vtbl = D_0046EA60;
        if (!m->keepRenderer) {
            AT(VCALL(D_0044E4F0, 0x2C, u8 *(*)(VObject *))(D_0044E4F0), 0x1C, u8) = 0;
        }
        if (m->ply != NULL) {
            VCALL(m->ply, 0x14, void (*)(VObject *))(m->ply);
            m->ply = NULL;
        }
        if (m->work != NULL) {
            func_00114FD0(m->work);
            m->work = NULL;
        }
        m->name[0] = 0;
        m->hasFrame = 0;
        if (m->unk1B8 != NULL) {
            func_00114FD0(m->unk1B8);
            m->unk1B8 = NULL;
        }
        if (&m->ply != NULL) {
            D_0044E958 = NULL;
        }
        if (m != NULL) {
            m->base.vtbl = Scene_vtable;
        }
        if ((s16)flags > 0) {
            func_0011F9A0(m);
        }
    }
    return m;
}

/* constructor */
Movie *func_002B70D0(Movie *m) {
    m->base.vtbl = Scene_vtable;
    Movie_SetState(m, &sMovieEntry);
    D_0044E958 = m;
    m->base.vtbl = D_0046EA60;
    m->work = NULL;
    m->ply = NULL;
    m->name[0] = 0;
    m->keepRenderer = 0;
    m->time = 0;
    m->shownFrame = -1;
    m->unk1B8 = NULL;
    m->volume[2] = *(f32 *)(D_0044E978 + 0x38);
    if (gProgress != NULL) {
        m->volume[3] = *(f32 *)(gProgress + 0x9F0);
    } else {
        m->volume[3] = 1.0f;
    }
    m->volume[1] = 1.0f;
    m->volume[0] = 1.0f;
    m->volume[4] = 0.0f;
    return m;
}

/* ---- SceneMovie (the boot logo): its work buffer is part of the object (+0x200) ---- */

/* +0x8 */
Movie *func_002C8C10(Movie *m, s32 flags) {
    if (m != NULL) {
        m->base.vtbl = D_0046ECC0;
        func_002B6FC0(m, 0);
        if ((s16)flags > 0) {
            func_0011F9A0(m);
        }
    }
    return m;
}

/* +0x10 entry */
void func_002C8C70(Movie *m) {
    MovieLib *lib = D_0044FEF8;

    VCALL(lib, 0x1C, void (*)(MovieLib *, s32, s32, s32, s32))(lib, 0x200, 0x1C0, 0x11, 0);
    if (VCALL(lib, 0x20, s32 (*)(MovieLib *))(lib) == 0) {
        VCALL(m, 0x14, void (*)(Movie *))(m);
        return;
    }
    lib = D_0044FEF8;
    VCALL(lib, 0x10, void (*)(MovieLib *, void *))(lib, (u8 *)m + 0x200);
    m->ply = VCALL(lib, 0x18, VObject *(*)(MovieLib *))(lib);
    if (m->ply == NULL) {
        VCALL(m, 0x14, void (*)(Movie *))(m);
        return;
    }
    Movie_SetStateFn(m, func_002B6BB0);
}
