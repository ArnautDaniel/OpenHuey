/* Movies (CRI Sofdec .SFD): the player library wrapper (ADX sound system +0x7C44, global
 * D_0044FEF8, vtable D_0046C740) and the movie scene. The mwPly* calls are CRI's library
 * (native/platform/sofdec.c on PC). */
#include "common.h"
#include "ptmf.h"


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
    /* 0x1B5 */ u8 frameBuf;        /* class 0: the frame written next (0 / 1) */
    /* 0x1B6 */ u8 pad1B6[2];
    /* 0x1B8 */ void *frames;        /* class 0: two 256 x 224 frames the movie is decoded into */
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
    ptmf_set(&m->base.state, state);
}

static inline void Movie_SetStateFn(Movie *m, void (*fn)(Movie *)) {
    ptmf_set_fn(&m->base.state, (void *)fn);
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


/* ---- D_0046EA40 (room effect 1 of rooms 0x31 / 0x32): the TV showing the movie playing -
 * a quad on the screen's corners textured with the movie's current frame (func_0021E410) while
 * +0x10 (set by +0x18); its +0xC is func_002B6310 ---- */

extern void *D_0046EA40[], *D_0046D730[], *D_0046FC30[], *D_00469D00[];
extern void func_002672E0(void *p);   /* delete (effects' heap) */
extern void func_002E56C0(u8 *quad);
extern VObject *D_0044E9A0;

/* the movie's current frame copied into a texture page (`page` 2): its GS TEX0, or -1 when
 * there is none. (PC: no movie frames yet - CRI Sofdec is not available) */
#ifdef HG_NATIVE
extern void glr_todo(const char *what);

u64 func_0021E410(u8 *mv, s32 page) {
    VObject *r = D_0044E4F0;
    s32 buf = VCALL(r, 0x38, s32 (*)(VObject *))(r);

    VCALL(D_0044E9A0, 0x38, s32 (*)(VObject *, s32))(D_0044E9A0, buf);
    if (!(u8)VCALL(r, 0x94, s32 (*)(VObject *, s32, s32, s32, s32))(r, buf, mv[0] != 0 ? 4 : 3, page, 0)) {
        return (u64)-1;
    }
    glr_todo("movie frame as a texture (func_0021E410)");
    return (u64)-1;
}
#else
u64 func_0021E410(u8 *mv, s32 page);
#endif

/* +0x8 destructor */
void *func_002B60D0(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046EA40;
        if (o != NULL) {
            AT(o, 0x0, void **) = D_0046D730;
        }
        if ((s16)flags > 0) {
            func_002672E0(o);
        }
    }
    return o;
}

/* +0x10 update: the movie flag from the game mode (gProgress +0x54) */
void func_002B62D0(void) {
    *D_0045D1F0 = VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress);
}

/* a TV's +0x14 draw: its screen (corners (x0, top, z0) (x1, top, z1) (x0, bottom, z0) (x1,
 * bottom, z1)), 256 x 224 of the movie frame at half brightness, in layer 2 */
static inline __attribute__((always_inline)) void tv_draw(u8 *o, u32 x0, u32 x1, u32 top, u32 bottom, u32 z0,
                                                          u32 z1) {
    if (AT(o, 0x10, s32) != 0) {
        u64 tex = func_0021E410(D_0045D1F0, 2);
        struct {
            void **vtbl;
            s32 a;
            u64 tex;
            void *rec;
            s32 corners;
            f32 cx, cy;
            s32 layer;
            s16 count, cellX, cellY, cellW, cellH, texW, texH;
            s8 flags, frames, texId, texGroup, palette;
        } q __attribute__((aligned(16)));
        u32 c[16] __attribute__((aligned(16)));
        struct {
            s32 rgba[4];
            f32 pos[4];
            f32 w, h, turn;
            s32 frame;
        } r __attribute__((aligned(16)));

        if (tex == (u64)-1) {
            return;
        }
        r.rgba[3] = 0x80;
        r.rgba[0] = 0x40;
        r.rgba[1] = 0x40;
        r.rgba[2] = 0x40;
        c[0] = x0;
        c[8] = x0;
        c[1] = top;
        c[5] = top;
        c[2] = z0;
        c[10] = z0;
        r.pos[3] = 1.0f;
        r.w = 1.0f;
        q.vtbl = D_0046FC30;
        r.h = 1.0f;
        q.rec = &r;
        c[3] = 0x3F800000;
        q.corners = (s32)c;
        q.cellH = 0xE0;
        c[4] = x1;
        c[12] = x1;
        c[6] = z1;
        c[14] = z1;
        c[7] = 0x3F800000;
        c[9] = bottom;
        c[13] = bottom;
        c[11] = 0x3F800000;
        c[15] = 0x3F800000;
        q.a = -1;
        q.palette = -1;
        q.tex = tex;
        q.layer = 2;
        q.flags = 2;
        q.count = 1;
        q.frames = 1;
        q.cellW = 0x100;
        q.texW = 0x100;
        q.texH = 0x100;
        r.pos[0] = 0.0f;
        r.pos[1] = 0.0f;
        r.pos[2] = 0.0f;
        r.turn = 0.0f;
        r.frame = 0;
        q.cx = 0.0f;
        q.cy = 0.0f;
        q.cellX = 0;
        q.cellY = 0;
        q.texId = 0;
        q.texGroup = 0;
        func_002E56C0((u8 *)&q);
        q.vtbl = D_00469D00;
    }
    *D_0045D1F0 = 1;
}

/* +0x14 draw: the screen at x -2.61 .. -0.12, z 5.79 .. 7.53, y 2.06 .. 4.39 */
void func_002B6140(u8 *o) {
    tv_draw(o, 0xC0274A23, 0xBDF93DD9, 0x408C872B, 0x400401A3, 0x40B93A93, 0x40F0D014);
}

/* ---- D_0046F5F0: another TV (as D_0046EA40; its +0xC func_002D78F0, +0x18 func_002D7710) ---- */

extern void *D_0046F5F0[];

/* +0x8 destructor */
void *func_002D76B0(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046F5F0;
        if (o != NULL) {
            AT(o, 0x0, void **) = D_0046D730;
        }
        if ((s16)flags > 0) {
            func_002672E0(o);
        }
    }
    return o;
}

/* +0x10 update */
void func_002D78B0(void) {
    *D_0045D1F0 = VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress);
}

/* +0x14 draw: the screen at x 20.8 .. 22.96, z 7.73 .. 9.80, y 9.80 .. 12.05 */
void func_002D7720(u8 *o) {
    tv_draw(o, 0x41A66AE8, 0x41B7AE14, 0x4140B924, 0x411CDB23, 0x40F7573F, 0x411CDD2F);
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
    m->frameBuf = 0;
    return 0;
}

/* restart the file after an error */
static inline void movie_retry(Movie *m) {
    if (m->dir != 0) {
        func_001E7430(0, m->dir);
    }
    VCALL(m->ply, 0x18, void (*)(VObject *, char *))(m->ply, m->name);
}

/* the renderer told the screen is the movie's (unless it keeps its own) */
static inline void movie_take_screen(Movie *m) {
    if (!m->keepRenderer) {
        AT(VCALL(D_0044E4F0, 0x2C, u8 *(*)(VObject *))(D_0044E4F0), 0x1C, u8) = 1;
    }
}

/* the sound up to the volume */
static inline void movie_sound_on(Movie *m) {
    s32 db;

    m->volume[4] = 1.0f;
    db = movie_level(m);
    if (m->ply != NULL) {
        VCALL(m->ply, 0x2C, void (*)(VObject *, s32))(m->ply, db);
    }
}

extern const PTMF D_004126C8;   /* virtual +0x1C: playing */
extern const PTMF D_004126D8;   /* func_002B68B0 */
extern const PTMF D_004126E8;   /* func_002B6710 */
extern const PTMF D_004126F8;   /* virtual +0x1C */

/* state: paused on the first frame until the event says go (mode 1, event command 0xCF) - then
 * it is shown, the movie resumed with its sound, and played */
void func_002B6710(Movie *m) {
    if (m->mode != 1) {
        return;
    }
    movie_take_screen(m);
    VCALL(m, 0x20, void (*)(Movie *))(m);
    m->hasFrame = 1;
    func_0023A180(m->ply);
    VCALL(m, 0x24, void (*)(Movie *))(m);
    VCALL(m->ply, 0x28, void (*)(VObject *, s32))(m->ply, 0);
    movie_sound_on(m);
    Movie_SetState(m, &D_004126F8);
}

/* state: wait for the first frame, then pause on it */
void func_002B68B0(Movie *m) {
    m->stat = VCALL(m->ply, 0x20, s32 (*)(VObject *))(m->ply);
    if (m->stat == 4) {
        movie_retry(m);
        return;
    }
    func_00239828(m->ply, &m->frame);
    if (m->frame == NULL) {
        return;
    }
    m->shownFrame = m->frameNo;
    VCALL(m->ply, 0x28, void (*)(VObject *, s32))(m->ply, 1);
    Movie_SetState(m, &D_004126E8);
}

/* state: the file opening; then played at once (mode 1) or held on its first frame */
void func_002B69B0(Movie *m) {
    m->stat = VCALL(m->ply, 0x20, s32 (*)(VObject *))(m->ply);
    if (m->stat == 4) {
        movie_retry(m);
        return;
    }
    if (m->stat == 1) {
        return;
    }
    if (m->mode != 1) {
        Movie_SetState(m, &D_004126D8);
        return;
    }
    movie_take_screen(m);
    movie_sound_on(m);
    Movie_SetState(m, &D_004126C8);
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
    m->frameBuf = 0;
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
        if (m->frames != NULL) {
            func_00114FD0(m->frames);
            m->frames = NULL;
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
    m->frames = NULL;
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

/* ---- movie class 0 (D_00470E80): a movie shown on something in the game - decoded at 256 x
 * 224 into two frames in progress memory (+0xCA6C0, 0x38000 each), drawn by the game itself ---- */

extern void *D_00470E80[];
extern const PTMF D_0041CA80;   /* func_002B6BB0 */
extern void func_0023E878(VObject *ply, u32 a, u32 b, s32 c);   /* Sofdec */
extern void func_002410B0(VObject *ply, u8 **frame, void *dst);   /* the frame copied out */
extern void FlushCache(s32 mode);

/* +0x8 (the frames aren't its own) */
Movie *func_002FEC50(Movie *m, s32 flags) {
    if (m != NULL) {
        m->base.vtbl = D_00470E80;
        m->frames = NULL;
        func_002B6FC0(m, 0);
        if ((s16)flags > 0) {
            func_0011F9A0(m);
        }
    }
    return m;
}

/* +0x24 */
void func_002FECC0(Movie *m) {
}

/* +0x20 the frame into the next of the two */
void func_002FECD0(Movie *m) {
    func_002410B0(m->ply, &m->frame, (u8 *)m->frames + m->frameBuf * 0x38000);
    FlushCache(0);
    m->frameBuf ^= 1;
}

/* +0x10 entry: a 256 x 224 player with its own work buffer, then start */
void func_002FED30(Movie *m) {
    MovieLib *lib = D_0044FEF8;
    s32 size;

    VCALL(lib, 0x1C, void (*)(MovieLib *, s32, s32, s32, s32))(lib, 0x100, 0xE0, 0x31, 1);
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
    }
    m->frames = gProgress + 0xCA6C0;
    func_0023E878(m->ply, 0x10, 0x20, 0);
    Movie_SetState(m, &D_0041CA80);
}

/* ---- movie classes 1..6 (func_001768B0's `kind`): movies the game draws itself, decoded into
 * two frames (+0x1B8, `frameBuf` the one written next). Each frame drawn is sent to VRAM 0xC0000
 * and most put it on the screen as a sprite. Their +0x18 fills in a frame description for
 * whoever shows the movie (the game over screen, func_002F0940): w, h, the frames, the frame
 * written next, plain - and returns `loop`. ---- */

extern void *D_0046EAB0[], *D_0046EAE0[], *D_0046EB10[], *D_0046EC30[], *D_0046EC90[], *D_00474F80[];
extern const PTMF D_00412730, D_00412740, D_00412750, D_004128D0, D_004128E0, D_0042E428;   /* func_002B6BB0 */
extern VObject *D_0044E4E8;   /* the texture cache */
extern void *func_00115B68(void *d, const void *s, u32 n);   /* memcpy */
extern s32 Progress_TestFlag(void *p, u32 id);
#ifdef HG_NATIVE
extern void glr_vram_draw(u32 addr, s32 layer);   /* native/platform/glr.c */
extern void glr_vram_blit(s32 layer);
#define MOVIE_UNCACHED(p) ((void *)(p))
#else
#define MOVIE_UNCACHED(p) ((void *)((u32)(p) | 0x30000000))   /* uncached accelerated */
#endif

static inline __attribute__((always_inline)) Movie *movie_dtor(Movie *m, s32 flags, void **vtbl) {
    if (m != NULL) {
        m->base.vtbl = vtbl;
        m->frames = NULL;
        func_002B6FC0(m, 0);
        if ((s16)flags > 0) {
            func_0011F9A0(m);
        }
    }
    return m;
}

/* +0x10 entry: a w x h player of kind `k` with its own work buffer, the frames at gProgress
 * + `at`, then start */
static inline __attribute__((always_inline)) void movie_entry(Movie *m, s32 w, s32 h, s32 k, s32 f, u32 at,
                                                              s32 setting, const PTMF *start) {
    MovieLib *lib = D_0044FEF8;
    s32 size;

    VCALL(lib, 0x1C, void (*)(MovieLib *, s32, s32, s32, s32))(lib, w, h, k, f);
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
    }
    m->frames = gProgress + at;
    if (setting) {
        func_0023E878(m->ply, 0x10, 0x20, 0);
    }
    Movie_SetState(m, start);
}

/* +0x18 the frame description (when looping), `loop` back */
static inline __attribute__((always_inline)) u8 movie_shot(Movie *m, u8 *shot, u8 plain) {
    if (m->loop) {
        AT(shot, 0x0, s16) = m->frameW;
        AT(shot, 0x2, s16) = m->frameH;
        AT(shot, 0x4, void *) = m->frames;
        AT(shot, 0x8, u8) = m->frameBuf;
        AT(shot, 0x9, u8) = plain;
    }
    return m->loop;
}

/* +0x20 the decoded frame copied into the next of the two (`size` bytes each) */
static inline __attribute__((always_inline)) void movie_take(Movie *m, u32 size, s32 flush, s32 mark) {
    func_002410B0(m->ply, &m->frame, (u8 *)m->frames + m->frameBuf * size);
    if (flush) {
        FlushCache(0);
    }
    m->frameBuf ^= 1;
    if (mark && !m->hasFrame) {
        m->hasFrame = 1;
    }
}

/* the last frame written into VRAM 0xC0000, h rows, on layer `layer` */
static inline __attribute__((always_inline)) void movie_send(Movie *m, u32 size, s32 h, s32 layer) {
    VCALL(D_0044E4F0, 0x40, void (*)(VObject *, u8 *, s32, s32, s32, s32))(
        D_0044E4F0, (u8 *)m->frames + (m->frameBuf ^ 1) * size, 0xC0000, m->frameW, h, layer);
}

#ifdef HG_NATIVE
/* the sprite showing the frame sent to VRAM 0xC0000 at the screen rectangle xy0 .. xy1 (GS
 * XYZ2) on `layer`, with OpenGL: over the screen by its alpha when the prim blends (ABE),
 * else straight */
static inline void movie_sprite(s32 layer, u64 prim, u32 xy0) {
    if (xy0 != 0x72007000) {
        glr_todo("movie: a sprite not over the whole screen");
    } else if (prim & 0x40) {
        glr_vram_draw(0xC0000, layer);
    } else {
        glr_vram_blit(layer);
    }
}
#endif

static inline void texcache_done(void) {
    if (D_0044E4E8 != NULL) {
        VCALL(D_0044E4E8, 0x18, void (*)(VObject *))(D_0044E4E8);
    }
}

/* ---- class 1 (D_0046EAB0): 256 x 224, over the screen by its alpha ---- */

/* +0x8 */
Movie *func_002BA1B0(Movie *m, s32 flags) {
    return movie_dtor(m, flags, D_0046EAB0);
}

/* +0x10 */
void func_002BA4F0(Movie *m) {
    movie_entry(m, 0x100, 0xE0, 0x31, 1, 0xCA6C0, 1, &D_00412730);
}

/* +0x18 */
u8 func_002BA220(Movie *m, u8 *shot) {
    return movie_shot(m, shot, 0);
}

/* +0x20 */
void func_002BA490(Movie *m) {
    movie_take(m, 0x38000, 1, 0);
}

#ifdef HG_NATIVE
/* +0x24 draw: the frame over the screen, blended (layer 0x2C) */
void func_002BA270(Movie *m) {
    movie_send(m, 0x38000, m->frameH, 0x2B);
    movie_sprite(0x2C, 0x156, 0x72007000);
    texcache_done();
}
#endif

/* ---- class 2 (D_0046EAE0): 512 x 224 frames; put over the screen, opaque, only while
 * progress flag 0x29 ---- */

/* +0x8 */
Movie *func_002BA670(Movie *m, s32 flags) {
    return movie_dtor(m, flags, D_0046EAE0);
}

/* +0x10 */
void func_002BA9F0(Movie *m) {
    movie_entry(m, 0x200, 0xE0, 0x11, 1, 0xCA6C0, 0, &D_00412740);
}

/* +0x18 */
u8 func_002BA6E0(Movie *m, u8 *shot) {
    return movie_shot(m, shot, 0);
}

/* +0x20 */
void func_002BA980(Movie *m) {
    movie_take(m, 0x70000, 0, 1);
}

#ifdef HG_NATIVE
/* +0x24 draw: the frame over the screen, opaque, only while progress flag 0x29 is set */
void func_002BA730(Movie *m) {
    movie_send(m, 0x70000, m->frameH, 0x2B);
    if ((Progress_TestFlag(gProgress, 0x29) & 0xFF) == 0) {
        return;
    }
    movie_sprite(0x2C, 0x116, 0x72007000);
    texcache_done();
}
#endif

/* ---- class 3 (D_0046EB10): 256 x 448 decoded, half of it shown (layer 3), over the screen
 * by its alpha (layer 4) ---- */

/* +0x8 */
Movie *func_002BAB50(Movie *m, s32 flags) {
    return movie_dtor(m, flags, D_0046EB10);
}

/* +0x10 */
void func_002BAE70(Movie *m) {
    movie_entry(m, 0x100, 0x1C0, 0x21, 1, 0xCA6C0, 0, &D_00412750);
}

/* +0x20 */
void func_002BAE10(Movie *m) {
    movie_take(m, 0x38000, 1, 0);
}

#ifdef HG_NATIVE
/* +0x24 draw: half the frame over the screen, blended (layer 4) */
void func_002BABC0(Movie *m) {
    movie_send(m, 0x38000, m->frameH / 2, 3);
    movie_sprite(4, 0x156, 0x72007000);
    texcache_done();
}
#endif

/* ---- class 4 (D_0046EC30): 256 x 224, plain (layer 6) ---- */

/* +0x8 */
Movie *func_002C61E0(Movie *m, s32 flags) {
    return movie_dtor(m, flags, D_0046EC30);
}

/* +0x10 */
void func_002C6380(Movie *m) {
    movie_entry(m, 0x100, 0xE0, 0x11, 0, 0xCA6C0, 0, &D_004128D0);
}

/* +0x18 */
u8 func_002C6250(Movie *m, u8 *shot) {
    return movie_shot(m, shot, 1);
}

/* +0x20 the frame copied */
void func_002C6320(Movie *m) {
    func_00115B68(MOVIE_UNCACHED((u8 *)m->frames + m->frameBuf * 0x38000), m->frame, 0x38000);
    m->frameBuf ^= 1;
}

/* +0x24 */
void func_002C62A0(Movie *m) {
    movie_send(m, 0x38000, m->frameH, 6);
    texcache_done();
}

/* ---- class 5 (D_0046EC90): as class 4 but over the screen with ALPHA 0x2A ---- */

/* +0x8 */
Movie *func_002C87C0(Movie *m, s32 flags) {
    return movie_dtor(m, flags, D_0046EC90);
}

/* +0x10 */
void func_002C8AB0(Movie *m) {
    movie_entry(m, 0x100, 0xE0, 0x11, 0, 0x16C0, 0, &D_004128E0);
}

/* +0x20 the frame copied */
void func_002C8A50(Movie *m) {
    func_00115B68(MOVIE_UNCACHED((u8 *)m->frames + m->frameBuf * 0x38000), m->frame, 0x38000);
    m->frameBuf ^= 1;
}

#ifdef HG_NATIVE
/* +0x24 draw: the frame over the screen; its blend (GS ALPHA 0x2A: (0 - 0) * FIX + Cs) leaves
 * it as it is, so straight */
void func_002C8830(Movie *m) {
    movie_send(m, 0x38000, m->frameH, 0x2B);
    movie_sprite(0x2C, 0x116, 0x72007000);
    texcache_done();
}
#endif

/* ---- class 6 (D_00474F80): a 256 x 64 strip at screen (128, 176) .. (512, 272) (layers 0x2D /
 * 0x2E) ---- */

/* +0x8 */
Movie *func_0032E430(Movie *m, s32 flags) {
    return movie_dtor(m, flags, D_00474F80);
}

/* +0x10 */
void func_0032E730(Movie *m) {
    movie_entry(m, 0x100, 0x40, 0x41, 1, 0x1AA6C0, 0, &D_0042E428);
}

/* +0x20 */
void func_0032E6C0(Movie *m) {
    movie_take(m, 0x10000, 1, 1);
}

#ifdef HG_NATIVE
/* +0x24 draw: the frame in a small rectangle (screen 0x740..0x8C0 x 0x7D0..0x830 in GS units:
 * not drawn yet, glr_todo), blended (layer 0x2E) */
void func_0032E4A0(Movie *m) {
    movie_send(m, 0x10000, m->frameH, 0x2D);
    movie_sprite(0x2E, 0x156, 0x7D007400);
    texcache_done();
}
#endif
