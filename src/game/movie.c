/* Movies (CRI Sofdec .SFD): the player library wrapper (ADX sound system +0x7C44, global
 * gMovieLib, vtable MovieLib_vtable) and the movie scene. The mwPly* calls are CRI's library
 * (native/platform/sofdec.c on PC). */
#include "common.h"
#include "ptmf.h"
#include "globals.h"
#include "progress.h"
#include "memcard.h"
#include "navmesh.h"
#include "actor.h"
#include "pursuer.h"
#include "effects.h"
#include "heap.h"
#include "movie.h"
#include "scene_game_members.h"
#include "cri/adx.h"
#include "cri/sofdec.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif
#include "libc.h"
#include "msl.h"
#include "sce/eekernel.h"

typedef struct MovieLib {
    /* 0x00 */ void **vtbl;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ u8 init[0x20];     /* mwPlyInitSfdFx parameters: f32 fps, s32 1, s32 1, u8 1 */
    /* 0x28 */ u8 create[0x30];   /* creation parameters (mwPlyCreateSofdec) */
} MovieLib;
_Static_assert(__builtin_offsetof(MovieLib, create) == 0x28, "MovieLib.create");

#define CPRM(lib, off, type) (*(type *)((lib)->create + (off) - 0x28))

extern void *MovieLib_vtable[];
extern void *D_0046AED0[];

void *MovieLibBase_dtor(u8 *o, s32 flags);

extern s32 pstr_This_CFT_function_doesn_t_support_the_fu, D_003E5264;
extern void *D_01976F98;
extern const u8 str_CRI_CFT_PS2EE_Ver_1_57_Build_Sep_17_2004[];
extern s64 gTimerRate;   /* the timer's rate (ticks a second) */
void Cft_SetValue0(s32 v);
s32 Cft_GetValue0(void);
void Cft_SetValue1(s32 v);
s32 Cft_GetValue1(void);
void Sofdec_SetValue(s32 v);
s32 Sofdec_GetValue(void);
void Cft_Init(void);
s64 Ticks_Convert(s64 ticks);
f32 Ticks_ToMicros(s32 ticks);
f32 Ticks_ToMillis(s32 ticks);
f32 Ticks_ToSeconds(s32 ticks);
void Ticks_SetRate(s64 rate);
void Measure_Clear(u8 *s);
void Measure_Add(u8 *s, s64 v);

void Cft_Noop(void);
void Cft_Stub1(void);
void Cft_Stub2(void);
void Cft_Stub3(void);
void Cft_Stub4(void);
void Cft_Stub5(void);
void Cft_Stub6(void);
void Cft_Stub7(void);
void Cft_Stub8(void);
void Cft_Stub9(void);
void Cft_Stub10(void);
void Cft_Stub11(void);

extern u8 D_003E5268[];
extern u8 D_003E5270[];
extern u8 D_003E88D0[];
extern u8 D_003E9F18[];
extern u8 D_004574B8[];
extern u8 D_00457558[];
extern u8 D_004575C0[];
extern u8 D_00457ED0[];
extern u8 D_00459930[];
extern u8 str_CRI_SFH_PS2EE_Ver_1_19_Build_Sep_17_2004[];
extern u8 str_CRI_SFX_PS2EE_Ver_2_08_Build_Sep_17_2004[];
extern u8 str_CRI_SUD_PS2EE_Ver_0_05_Build_Sep_17_2004[];
void *Cft_ValuePtr(void);
void *Sofdec_Data3E5270(void);
void *Sofdec_Data4574B8(void);
void *Sofdec_Data457558(void);
void *Sofdec_Data4575C0(void);
void *Sofdec_Data457ED0(void);
void *Sofdec_Data3E88D0(void);
void *Sofdec_Data3E9F18(void);
void *Sofdec_Data459930(void);
void *Sfh_Version(void);
void *Sfx_Version(void);
void *Sud_Version(void);

void TvScreenA_SetParams(u8 *self, u8 *src);

extern u8 *gMovieFlag;
#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

void TvScreenB_SetParams(u8 *p, const u8 *src);
void TvScreenB_Start(u8 *p);

/* destructor (vtable D_0046AED0) */
/* 0x001BF660 */
void *MovieLibBase_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AED0;
        AT(o, 0x4, u8) = 0;
        gMovieLib = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}
/* +0x8 */
/* 0x0020E740 */
MovieLib *MovieLib_dtor(MovieLib *lib, s32 flags) {
    if (lib != NULL) {
        lib->vtbl = MovieLib_vtable;
        if (lib != NULL) {
            lib->vtbl = D_0046AED0;
            lib->unk4 = 0;
            if (lib != NULL) {
                gMovieLib = NULL;
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
/* 0x002266F0 */
void MovieLib_Setup(MovieLib *lib) {
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

/* (possibly dead code: nothing in the game references it) */
/* 0x00226790 */
void Cft_SetValue0(s32 v) {
    pstr_This_CFT_function_doesn_t_support_the_fu = v;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x002267A0 */
s32 Cft_GetValue0(void) {
    return pstr_This_CFT_function_doesn_t_support_the_fu;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x002267B0 */
void Cft_SetValue1(s32 v) {
    D_003E5264 = v;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x002267C0 */
s32 Cft_GetValue1(void) {
    return D_003E5264;
}

/* D_01976F98 = str_CRI_CFT_PS2EE_Ver_1_57_Build_Sep_17_2004, Cft_ValuePtr's value 0, then 0x80 */
/* (possibly dead code: nothing in the game references it) */
/* 0x002267D0 */
void Cft_Init(void) {
    D_01976F98 = (void *)str_CRI_CFT_PS2EE_Ver_1_57_Build_Sep_17_2004;
    AT(Cft_ValuePtr(), 0x0, s32) = 0;
    Sofdec_SetValue(0x80);
}

/* 0x00226808 */
void Cft_Noop(void) {
}

/* 0x00226810 */
void *Cft_ValuePtr(void) {
    return D_003E5268;
}

/* the value behind Cft_ValuePtr set / read */
/* 0x00226820 */
void Sofdec_SetValue(s32 v) {
    AT(Cft_ValuePtr(), 0x0, s32) = v;
}

/* 0x00226848 */
s32 Sofdec_GetValue(void) {
    return AT(Cft_ValuePtr(), 0x0, s32);
}

/* (possibly dead code: nothing in the game references it) */
/* 0x00226868 */
void Cft_Stub1(void) {
}

/* (possibly dead code: nothing in the game references it) */
/* 0x00226870 */
void Cft_Stub2(void) {
}

/* (possibly dead code: nothing in the game references it) */
/* 0x00226878 */
void Cft_Stub3(void) {
}

/* (possibly dead code: nothing in the game references it) */
/* 0x00226880 */
void Cft_Stub4(void) {
}

/* (possibly dead code: nothing in the game references it) */
/* 0x00226888 */
void Cft_Stub5(void) {
}

/* (possibly dead code: nothing in the game references it) */
/* 0x00226890 */
void Cft_Stub6(void) {
}

/* (possibly dead code: nothing in the game references it) */
/* 0x00226898 */
void Cft_Stub7(void) {
}

/* (possibly dead code: nothing in the game references it) */
/* 0x002268A0 */
void Cft_Stub8(void) {
}

/* (possibly dead code: nothing in the game references it) */
/* 0x002268A8 */
void Cft_Stub9(void) {
}

/* (possibly dead code: nothing in the game references it) */
/* 0x002268B0 */
void Cft_Stub10(void) {
}

/* (possibly dead code: nothing in the game references it) */
/* 0x002268B8 */
void Cft_Stub11(void) {
}

/* 0x0022A6F8 */
void *Sofdec_Data3E5270(void) {
    return D_003E5270;
}

/* 0x00230B48 */
void *Sofdec_Data4574B8(void) {
    return D_004574B8;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x00233720 */
void *Sofdec_Data457558(void) {
    return D_00457558;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x00236B70 */
void *Sofdec_Data4575C0(void) {
    return D_004575C0;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x0023AA68 */
void *Sofdec_Data457ED0(void) {
    return D_00457ED0;
}

/* 0x0023AA78 */
void *Sofdec_Data3E88D0(void) {
    return D_003E88D0;
}

/* 0x0023FFA0 */
void *Sofdec_Data3E9F18(void) {
    return D_003E9F18;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x00246A80 */
void *Sofdec_Data459930(void) {
    return D_00459930;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x002577D8 */
void *Sfh_Version(void) {
    return str_CRI_SFH_PS2EE_Ver_1_19_Build_Sep_17_2004;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x00259EE8 */
void *Sfx_Version(void) {
    return str_CRI_SFX_PS2EE_Ver_2_08_Build_Sep_17_2004;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x0025B828 */
void *Sud_Version(void) {
    return str_CRI_SUD_PS2EE_Ver_0_05_Build_Sep_17_2004;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x0025C2F8 */
s64 Ticks_Convert(s64 ticks) {
    return func_0011CE88(ticks, gTimerRate);
}

/* in microseconds, milliseconds, seconds */
/* (possibly dead code: nothing in the game references it) */
/* 0x0025C320 */
f32 Ticks_ToMicros(s32 ticks) {
    return (f32)ticks * 1000000.0f / (f32)(s32)gTimerRate;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x0025C360 */
f32 Ticks_ToMillis(s32 ticks) {
    return (f32)ticks * 1000.0f / (f32)(s32)gTimerRate;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x0025C3A0 */
f32 Ticks_ToSeconds(s32 ticks) {
    return (f32)ticks / (f32)(s32)gTimerRate;
}

/* 0x0025C3D0 */
void Ticks_SetRate(s64 rate) {
    gTimerRate = rate;
}

/* a measure: { sum, min, max, count } cleared / one more value */
/* (possibly dead code: nothing in the game references it) */
/* 0x0025C3E0 */
void Measure_Clear(u8 *s) {
    AT(s, 0x18, s32) = 0;
    AT(s, 0x8, s64) = (s64)((u64)-1 >> 1);
    AT(s, 0x0, s64) = 0;
    AT(s, 0x10, s64) = 0;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x0025C400 */
void Measure_Add(u8 *s, s64 v) {
    s64 lo = AT(s, 0x8, s64), hi = AT(s, 0x10, s64);

    if (hi < v) {
        hi = v;
    }
    if (v < lo) {
        lo = v;
    }
    AT(s, 0x18, s32)++;
    AT(s, 0x0, s64) += v;
    AT(s, 0x8, s64) = lo;
    AT(s, 0x10, s64) = hi;
}

/* +0x10 the work buffer (NULL: keep) */
/* 0x002266D0 */
void MovieLib_SetWork(MovieLib *lib, void *work) {
    if (work != NULL) {
        CPRM(lib, 0x40, void *) = work;
    }
}

/* +0x14 shut the library down */
/* 0x00226680 */
void MovieLib_Shutdown_14(MovieLib *lib) {
    func_0023AE40();
    func_00115D20(lib->init, 0, 0x20);
    func_00115D20(lib->create, 0, 0x30);
}

/* +0x18 create a player (NULL without a work buffer or on failure) */
/* 0x00226640 */
void *MovieLib_CreatePlayer(MovieLib *lib) {
    if (CPRM(lib, 0x40, void *) == NULL) {
        return NULL;
    }
    return func_00238BF0(lib->create);
}

/* +0x1C the movie's size and kind */
/* 0x00226620 */
void MovieLib_MovieSize(MovieLib *lib, s32 w, s32 h, s32 a, s32 b) {
    CPRM(lib, 0x30, s32) = w;
    CPRM(lib, 0x34, s32) = h;
    CPRM(lib, 0x48, s32) = a;
    CPRM(lib, 0x4C, u8) = b;
}

/* +0x20 the work buffer size, rounded up to 64 bytes */
/* 0x002265D0 */
s32 MovieLib_WorkSize(MovieLib *lib) {
    CPRM(lib, 0x44, s32) = mwPlyCalcWorkCprmSfd(lib->create);
    CPRM(lib, 0x44, s32) = (CPRM(lib, 0x44, s32) & ~0x3F) + ((CPRM(lib, 0x44, s32) & 0x3F) ? 0x40 : 0);
    return CPRM(lib, 0x44, s32);
}

/* ---- the movie scene: plays a .SFD (CAPCOM.SFD at boot, the opening ...) ---- */

#include "game.h"

_Static_assert(__builtin_offsetof(Movie, name) == 0xA8, "Movie.name");
_Static_assert(__builtin_offsetof(Movie, volume) == 0x1C8, "Movie.volume");

extern void *Scene_vtable[];
extern void *Movie_vtable[];       /* Movie */
extern void *MovieScene_vtable[];       /* SceneMovie (the boot logo) */
extern u8 *gMovieFlag;

void Movie_Finish(Movie *m);
void Movie_StateOpening(Movie *m);

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

/* ---- TvScreenA_vtable (room effect 1 of rooms 0x31 / 0x32): the TV showing the movie playing -
 * a quad on the screen's corners textured with the movie's current frame (Movie_PageTex0) while
 * +0x10 (set by +0x18); its +0xC is TvScreenA_Start ---- */

extern void *TvScreenA_vtable[], *D_0046D730[], *QuadDrawer_vtable[], *Helper469D00_vtable[];

/* the movie's current frame copied into a texture page (`page` 2): its GS TEX0, or -1 when
 * there is none. (PC: no movie frames yet - CRI Sofdec is not available) */
#ifdef HG_NATIVE

/* 0x0021E410 */
u64 Movie_PageTex0(u8 *mv, s32 page) {
    VObject *r = gRenderer;
    s32 buf = VCALL(r, 0x38, s32 (*)(VObject *))(r);

    VCALL(gVram, 0x38, s32 (*)(VObject *, s32))(gVram, buf);
    if (!(u8)VCALL(r, 0x94, s32 (*)(VObject *, s32, s32, s32, s32))(r, buf, mv[0] != 0 ? 4 : 3, page, 0)) {
        return (u64)-1;
    }
    glr_todo("movie frame as a texture (Movie_PageTex0)");
    return (u64)-1;
}
#else
u64 Movie_PageTex0(u8 *mv, s32 page);
#endif

/* +0x8 destructor */
/* 0x002B60D0 */
void *TvScreenA_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = TvScreenA_vtable;
        if (o != NULL) {
            AT(o, 0x0, void **) = D_0046D730;
        }
        if ((s16)flags > 0) {
            RoomEffects_delete(o);
        }
    }
    return o;
}

/* 0x002B6130 */
void TvScreenA_SetParams(u8 *self, u8 *src) {
    *(u32 *)(self + 0x10) = *src;
}

/* +0x10 update: the movie flag from the game mode (gProgress +0x54) */
/* 0x002B62D0 */
void TvScreenA_Update(void) {
    *gMovieFlag = VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress);
}

/* a TV's +0x14 draw: its screen (corners (x0, top, z0) (x1, top, z1) (x0, bottom, z0) (x1,
 * bottom, z1)), 256 x 224 of the movie frame at half brightness, in layer 2 */
static inline __attribute__((always_inline)) void tv_draw(u8 *o, u32 x0, u32 x1, u32 top, u32 bottom, u32 z0,
                                                          u32 z1) {
    if (AT(o, 0x10, s32) != 0) {
        u64 tex = Movie_PageTex0(gMovieFlag, 2);
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
        q.vtbl = QuadDrawer_vtable;
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
        Drawer_Submit((u8 *)&q);
        q.vtbl = Helper469D00_vtable;
    }
    *gMovieFlag = 1;
}

/* +0x14 draw: the screen at x -2.61 .. -0.12, z 5.79 .. 7.53, y 2.06 .. 4.39 */
/* 0x002B6140 */
void TvScreenA_Draw(u8 *o) {
    tv_draw(o, 0xC0274A23, 0xBDF93DD9, 0x408C872B, 0x400401A3, 0x40B93A93, 0x40F0D014);
}

/* ---- TvScreenB_vtable: another TV (as TvScreenA_vtable; its +0xC TvScreenB_Start, +0x18 TvScreenB_SetParams) ---- */

extern void *TvScreenB_vtable[];

/* +0x8 destructor */
/* 0x002D76B0 */
void *TvScreenB_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = TvScreenB_vtable;
        if (o != NULL) {
            AT(o, 0x0, void **) = D_0046D730;
        }
        if ((s16)flags > 0) {
            RoomEffects_delete(o);
        }
    }
    return o;
}

/* 0x002D7710 */
void TvScreenB_SetParams(u8 *p, const u8 *src) { F(p, 0x10, u32) = *src; }

/* +0x10 update */
/* 0x002D78B0 */
void TvScreenB_Update(void) {
    *gMovieFlag = VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress);
}

/* 0x002D78F0 */
void TvScreenB_Start(u8 *p) {
    *gMovieFlag = 0;
    F(p, 0x10, u32) = 0;
}

/* +0x14 draw: the screen at x 20.8 .. 22.96, z 7.73 .. 9.80, y 9.80 .. 12.05 */
/* 0x002D7720 */
void TvScreenB_Draw(u8 *o) {
    tv_draw(o, 0x41A66AE8, 0x41B7AE14, 0x4140B924, 0x411CDB23, 0x40F7573F, 0x411CDD2F);
}

/* clear the flag */
/* 0x002B6310 */
void TvScreenA_Start(Movie *m) {
    *gMovieFlag = 0;
    AT(m, 0x10, s32) = 0;
}

/* +0x18 */
/* 0x002B6330 */
s32 Movie_Shot(Movie *m) {
    return 0;
}

/* apply the volume */
/* 0x002B6340 */
void Movie_ApplyVolume(Movie *m) {
    s32 db = movie_level(m);

    if (m->ply != NULL) {
        VCALL(m->ply, 0x2C, void (*)(VObject *, s32))(m->ply, db);
    }
}

/* the player's status: 2 playing, 1 other, -1 ended or none */
/* 0x002B6410 */
s32 Movie_Status(Movie *m) {
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
/* 0x002B64A0 */
void Movie_Draw(Movie *m) {
}

/* +0x20 draw the frame (full screen, layer 3) */
/* 0x002B64B0 */
void Movie_TakeFrame(Movie *m) {
    VCALL(gRenderer, 0x40, void (*)(VObject *, u8 *, s32, s32, s32, s32))(
        gRenderer, m->frame, 0x88000, m->frameW, m->frameH, 3);
}

/* the frame shown, -1 none */
/* 0x002B64F0 */
s32 Movie_FrameShown(Movie *m) {
    if (m->hasFrame) {
        return m->shownFrame;
    }
    return -1;
}

/* +0x1C per frame: take the decoded frame and draw it; at the end, finish (or loop) */
/* 0x002B6510 */
void Movie_Frame(Movie *m) {
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

/* restart the file; 2 / 0 / -1 as Movie_Status */
/* 0x002B6640 */
s32 Movie_Restart(Movie *m) {
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
        AT(VCALL(gRenderer, 0x2C, u8 *(*)(VObject *))(gRenderer), 0x1C, u8) = 1;
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
extern const PTMF Movie_StateWaitFirst_ptmf;   /* Movie_StateWaitFirst */
extern const PTMF Movie_StatePausedFirst_ptmf;   /* Movie_StatePausedFirst */
extern const PTMF D_004126F8;   /* virtual +0x1C */

/* state: paused on the first frame until the event says go (mode 1, event command 0xCF) - then
 * it is shown, the movie resumed with its sound, and played */
/* 0x002B6710 */
void Movie_StatePausedFirst(Movie *m) {
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
/* 0x002B68B0 */
void Movie_StateWaitFirst(Movie *m) {
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
    Movie_SetState(m, &Movie_StatePausedFirst_ptmf);
}

/* state: the file opening; then played at once (mode 1) or held on its first frame */
/* 0x002B69B0 */
void Movie_StateOpening(Movie *m) {
    m->stat = VCALL(m->ply, 0x20, s32 (*)(VObject *))(m->ply);
    if (m->stat == 4) {
        movie_retry(m);
        return;
    }
    if (m->stat == 1) {
        return;
    }
    if (m->mode != 1) {
        Movie_SetState(m, &Movie_StateWaitFirst_ptmf);
        return;
    }
    movie_take_screen(m);
    movie_sound_on(m);
    Movie_SetState(m, &D_004126C8);
}

/* state: start the file */
/* 0x002B6BB0 */
void Movie_StateStart(Movie *m) {
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
    Movie_SetStateFn(m, Movie_StateOpening);
}

/* set the file to play (folder\name, or just a name in the current folder) */
/* 0x002B6D10 */
void Movie_SetFile(Movie *m, const char *path, s32 mode, s32 keep) {
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
/* 0x002B6E50 */
void Movie_Finish(Movie *m) {
    m->dir = 0;
    m->name[0] = 0;
    m->shownFrame = -1;
    m->base.request = SCENE_REQ_FINISH;
}

/* +0x10 entry: a player with its own work buffer, then start */
/* 0x002B6E70 */
void Movie_Entry(Movie *m) {
    MovieLib *lib = gMovieLib;
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
    lib = gMovieLib;
    VCALL(lib, 0x10, void (*)(MovieLib *, void *))(lib, m->work);
    m->ply = VCALL(lib, 0x18, VObject *(*)(MovieLib *))(lib);
    if (m->ply == NULL) {
        VCALL(m, 0x14, void (*)(Movie *))(m);
        return;
    }
    Movie_SetStateFn(m, Movie_StateStart);
}

/* +0x8 */
/* 0x002B6FC0 */
Movie *Movie_dtor(Movie *m, s32 flags) {
    if (m != NULL) {
        m->base.vtbl = Movie_vtable;
        if (!m->keepRenderer) {
            AT(VCALL(gRenderer, 0x2C, u8 *(*)(VObject *))(gRenderer), 0x1C, u8) = 0;
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
            gMovie = NULL;
        }
        if (m != NULL) {
            m->base.vtbl = Scene_vtable;
        }
        if ((s16)flags > 0) {
            SceneHeap_delete(m);
        }
    }
    return m;
}

/* constructor */
/* 0x002B70D0 */
Movie *Movie_ctor(Movie *m) {
    m->base.vtbl = Scene_vtable;
    Movie_SetState(m, &sMovieEntry);
    gMovie = m;
    m->base.vtbl = Movie_vtable;
    m->work = NULL;
    m->ply = NULL;
    m->name[0] = 0;
    m->keepRenderer = 0;
    m->time = 0;
    m->shownFrame = -1;
    m->frames = NULL;
    m->volume[2] = *(f32 *)(gGamePtr + 0x38);
    if (gProgress != NULL) {
        m->volume[3] = *(f32 *)((u8 *)gProgress + 0x9F0);
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
/* 0x002C8C10 */
Movie *MovieScene_dtor(Movie *m, s32 flags) {
    if (m != NULL) {
        m->base.vtbl = MovieScene_vtable;
        Movie_dtor(m, 0);
        if ((s16)flags > 0) {
            SceneHeap_delete(m);
        }
    }
    return m;
}

/* +0x10 entry */
/* 0x002C8C70 */
void MovieScene_Entry(Movie *m) {
    MovieLib *lib = gMovieLib;

    VCALL(lib, 0x1C, void (*)(MovieLib *, s32, s32, s32, s32))(lib, 0x200, 0x1C0, 0x11, 0);
    if (VCALL(lib, 0x20, s32 (*)(MovieLib *))(lib) == 0) {
        VCALL(m, 0x14, void (*)(Movie *))(m);
        return;
    }
    lib = gMovieLib;
    VCALL(lib, 0x10, void (*)(MovieLib *, void *))(lib, (u8 *)m + 0x200);
    m->ply = VCALL(lib, 0x18, VObject *(*)(MovieLib *))(lib);
    if (m->ply == NULL) {
        VCALL(m, 0x14, void (*)(Movie *))(m);
        return;
    }
    Movie_SetStateFn(m, Movie_StateStart);
}

/* ---- movie class 0 (MovieOwnBuf_vtable): a movie shown on something in the game - decoded at 256 x
 * 224 into two frames in progress memory (+0xCA6C0, 0x38000 each), drawn by the game itself ---- */

extern void *MovieOwnBuf_vtable[];
extern const PTMF Movie_StateStart_ptmf6;   /* Movie_StateStart */

/* +0x8 (the frames aren't its own) */
/* 0x002FEC50 */
Movie *MovieOwnBuf_dtor(Movie *m, s32 flags) {
    if (m != NULL) {
        m->base.vtbl = MovieOwnBuf_vtable;
        m->frames = NULL;
        Movie_dtor(m, 0);
        if ((s16)flags > 0) {
            SceneHeap_delete(m);
        }
    }
    return m;
}

/* +0x24 */
/* 0x002FECC0 */
void MovieOwnBuf_Draw(Movie *m) {
}

/* +0x20 the frame into the next of the two */
/* 0x002FECD0 */
void MovieOwnBuf_TakeFrame(Movie *m) {
    func_002410B0(m->ply, &m->frame, (u8 *)m->frames + m->frameBuf * 0x38000);
    FlushCache(0);
    m->frameBuf ^= 1;
}

/* +0x10 entry: a 256 x 224 player with its own work buffer, then start */
/* 0x002FED30 */
void MovieOwnBuf_Entry(Movie *m) {
    MovieLib *lib = gMovieLib;
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
    lib = gMovieLib;
    VCALL(lib, 0x10, void (*)(MovieLib *, void *))(lib, m->work);
    m->ply = VCALL(lib, 0x18, VObject *(*)(MovieLib *))(lib);
    if (m->ply == NULL) {
        VCALL(m, 0x14, void (*)(Movie *))(m);
    }
    m->frames = (u8 *)gProgress + 0xCA6C0;
    func_0023E878(m->ply, 0x10, 0x20, 0);
    Movie_SetState(m, &Movie_StateStart_ptmf6);
}

/* ---- movie classes 1..6 (Progress_PlayMovie's `kind`): movies the game draws itself, decoded into
 * two frames (+0x1B8, `frameBuf` the one written next). Each frame drawn is sent to VRAM 0xC0000
 * and most put it on the screen as a sprite. Their +0x18 fills in a frame description for
 * whoever shows the movie (the game over screen, GameOver_DrawMovieFrame): w, h, the frames, the frame
 * written next, plain - and returns `loop`. ---- */

extern void *MovieBlended_vtable[], *MovieOpaque_vtable[], *MovieHalf_vtable[], *MovieCopied_vtable[], *MovieAdded_vtable[], *MovieSmall_vtable[];
extern const PTMF Movie_StateStart_ptmf, Movie_StateStart_ptmf2, Movie_StateStart_ptmf3, Movie_StateStart_ptmf4, Movie_StateStart_ptmf5, Movie_StateStart_ptmf7;   /* Movie_StateStart */
#ifdef HG_NATIVE
#define MOVIE_UNCACHED(p) ((void *)(p))
#else
#define MOVIE_UNCACHED(p) ((void *)((u32)(p) | 0x30000000))   /* uncached accelerated */
#endif

static inline __attribute__((always_inline)) Movie *movie_dtor(Movie *m, s32 flags, void **vtbl) {
    if (m != NULL) {
        m->base.vtbl = vtbl;
        m->frames = NULL;
        Movie_dtor(m, 0);
        if ((s16)flags > 0) {
            SceneHeap_delete(m);
        }
    }
    return m;
}

/* +0x10 entry: a w x h player of kind `k` with its own work buffer, the frames at gProgress
 * + `at`, then start */
static inline __attribute__((always_inline)) void movie_entry(Movie *m, s32 w, s32 h, s32 k, s32 f, u32 at,
                                                              s32 setting, const PTMF *start) {
    MovieLib *lib = gMovieLib;
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
    lib = gMovieLib;
    VCALL(lib, 0x10, void (*)(MovieLib *, void *))(lib, m->work);
    m->ply = VCALL(lib, 0x18, VObject *(*)(MovieLib *))(lib);
    if (m->ply == NULL) {
        VCALL(m, 0x14, void (*)(Movie *))(m);
    }
    m->frames = (u8 *)gProgress + at;
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
    VCALL(gRenderer, 0x40, void (*)(VObject *, u8 *, s32, s32, s32, s32))(
        gRenderer, (u8 *)m->frames + (m->frameBuf ^ 1) * size, 0xC0000, m->frameW, h, layer);
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
    if (gTexCache != NULL) {
        VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
    }
}

/* ---- class 1 (MovieBlended_vtable): 256 x 224, over the screen by its alpha ---- */

/* +0x8 */
/* 0x002BA1B0 */
Movie *MovieBlended_dtor(Movie *m, s32 flags) {
    return movie_dtor(m, flags, MovieBlended_vtable);
}

/* +0x10 */
/* 0x002BA4F0 */
void MovieBlended_Entry(Movie *m) {
    movie_entry(m, 0x100, 0xE0, 0x31, 1, 0xCA6C0, 1, &Movie_StateStart_ptmf);
}

/* +0x18 */
/* 0x002BA220 */
u8 MovieBlended_Shot(Movie *m, u8 *shot) {
    return movie_shot(m, shot, 0);
}

/* +0x20 */
/* 0x002BA490 */
void MovieBlended_TakeFrame(Movie *m) {
    movie_take(m, 0x38000, 1, 0);
}

#ifdef HG_NATIVE
/* +0x24 draw: the frame over the screen, blended (layer 0x2C) */
/* 0x002BA270 */
void MovieBlended_Draw(Movie *m) {
    movie_send(m, 0x38000, m->frameH, 0x2B);
    movie_sprite(0x2C, 0x156, 0x72007000);
    texcache_done();
}
#endif

/* ---- class 2 (MovieOpaque_vtable): 512 x 224 frames; put over the screen, opaque, only while
 * progress flag 0x29 ---- */

/* +0x8 */
/* 0x002BA670 */
Movie *MovieOpaque_dtor(Movie *m, s32 flags) {
    return movie_dtor(m, flags, MovieOpaque_vtable);
}

/* +0x10 */
/* 0x002BA9F0 */
void MovieOpaque_Entry(Movie *m) {
    movie_entry(m, 0x200, 0xE0, 0x11, 1, 0xCA6C0, 0, &Movie_StateStart_ptmf2);
}

/* +0x18 */
/* 0x002BA6E0 */
u8 MovieOpaque_Shot(Movie *m, u8 *shot) {
    return movie_shot(m, shot, 0);
}

/* +0x20 */
/* 0x002BA980 */
void MovieOpaque_TakeFrame(Movie *m) {
    movie_take(m, 0x70000, 0, 1);
}

#ifdef HG_NATIVE
/* +0x24 draw: the frame over the screen, opaque, only while progress flag 0x29 is set */
/* 0x002BA730 */
void MovieOpaque_Draw(Movie *m) {
    movie_send(m, 0x70000, m->frameH, 0x2B);
    if ((Progress_TestFlag(gProgress, 0x29) & 0xFF) == 0) {
        return;
    }
    movie_sprite(0x2C, 0x116, 0x72007000);
    texcache_done();
}
#endif

/* ---- class 3 (MovieHalf_vtable): 256 x 448 decoded, half of it shown (layer 3), over the screen
 * by its alpha (layer 4) ---- */

/* +0x8 */
/* 0x002BAB50 */
Movie *MovieHalf_dtor(Movie *m, s32 flags) {
    return movie_dtor(m, flags, MovieHalf_vtable);
}

/* +0x10 */
/* 0x002BAE70 */
void MovieHalf_Entry(Movie *m) {
    movie_entry(m, 0x100, 0x1C0, 0x21, 1, 0xCA6C0, 0, &Movie_StateStart_ptmf3);
}

/* +0x20 */
/* 0x002BAE10 */
void MovieHalf_TakeFrame(Movie *m) {
    movie_take(m, 0x38000, 1, 0);
}

#ifdef HG_NATIVE
/* +0x24 draw: half the frame over the screen, blended (layer 4) */
/* 0x002BABC0 */
void MovieHalf_Draw(Movie *m) {
    movie_send(m, 0x38000, m->frameH / 2, 3);
    movie_sprite(4, 0x156, 0x72007000);
    texcache_done();
}
#endif

/* ---- class 4 (MovieCopied_vtable): 256 x 224, plain (layer 6) ---- */

/* +0x8 */
/* 0x002C61E0 */
Movie *MovieCopied_dtor(Movie *m, s32 flags) {
    return movie_dtor(m, flags, MovieCopied_vtable);
}

/* +0x10 */
/* 0x002C6380 */
void MovieCopied_Entry(Movie *m) {
    movie_entry(m, 0x100, 0xE0, 0x11, 0, 0xCA6C0, 0, &Movie_StateStart_ptmf4);
}

/* +0x18 */
/* 0x002C6250 */
u8 MovieCopied_Shot(Movie *m, u8 *shot) {
    return movie_shot(m, shot, 1);
}

/* +0x20 the frame copied */
/* 0x002C6320 */
void MovieCopied_TakeFrame(Movie *m) {
    func_00115B68(MOVIE_UNCACHED((u8 *)m->frames + m->frameBuf * 0x38000), m->frame, 0x38000);
    m->frameBuf ^= 1;
}

/* +0x24 */
/* 0x002C62A0 */
void MovieCopied_Draw(Movie *m) {
    movie_send(m, 0x38000, m->frameH, 6);
    texcache_done();
}

/* ---- class 5 (MovieAdded_vtable): as class 4 but over the screen with ALPHA 0x2A ---- */

/* +0x8 */
/* 0x002C87C0 */
Movie *MovieAdded_dtor(Movie *m, s32 flags) {
    return movie_dtor(m, flags, MovieAdded_vtable);
}

/* +0x10 */
/* 0x002C8AB0 */
void MovieAdded_Entry(Movie *m) {
    movie_entry(m, 0x100, 0xE0, 0x11, 0, 0x16C0, 0, &Movie_StateStart_ptmf5);
}

/* +0x20 the frame copied */
/* 0x002C8A50 */
void MovieAdded_TakeFrame(Movie *m) {
    func_00115B68(MOVIE_UNCACHED((u8 *)m->frames + m->frameBuf * 0x38000), m->frame, 0x38000);
    m->frameBuf ^= 1;
}

#ifdef HG_NATIVE
/* +0x24 draw: the frame over the screen; its blend (GS ALPHA 0x2A: (0 - 0) * FIX + Cs) leaves
 * it as it is, so straight */
/* 0x002C8830 */
void MovieAdded_Draw(Movie *m) {
    movie_send(m, 0x38000, m->frameH, 0x2B);
    movie_sprite(0x2C, 0x116, 0x72007000);
    texcache_done();
}
#endif

/* ---- class 6 (MovieSmall_vtable): a 256 x 64 strip at screen (128, 176) .. (512, 272) (layers 0x2D /
 * 0x2E) ---- */

/* +0x8 */
/* 0x0032E430 */
Movie *MovieSmall_dtor(Movie *m, s32 flags) {
    return movie_dtor(m, flags, MovieSmall_vtable);
}

/* +0x10 */
/* 0x0032E730 */
void MovieSmall_Entry(Movie *m) {
    movie_entry(m, 0x100, 0x40, 0x41, 1, 0x1AA6C0, 0, &Movie_StateStart_ptmf7);
}

/* +0x20 */
/* 0x0032E6C0 */
void MovieSmall_TakeFrame(Movie *m) {
    movie_take(m, 0x10000, 1, 1);
}

#ifdef HG_NATIVE
/* +0x24 draw: the frame in a small rectangle (screen 0x740..0x8C0 x 0x7D0..0x830 in GS units:
 * not drawn yet, glr_todo), blended (layer 0x2E) */
/* 0x0032E4A0 */
void MovieSmall_Draw(Movie *m) {
    movie_send(m, 0x10000, m->frameH, 0x2D);
    movie_sprite(0x2E, 0x156, 0x7D007400);
    texcache_done();
}
#endif
