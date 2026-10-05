/* The cutscene director (vtable D_0046ED30, global D_0044FE10, constructed by func_002D1000).
 * A cutscene ("name" +0x14) is a script (progress +0x16C0, +0x18 here), a per-frame signal
 * table (progress +0x26C0, +0x1C) and light / effect cues (progress +0x66C0), loaded at the
 * start, then a series of shots streamed in turn into two buffers (progress +0xA6C0 +
 * 0x60000 x buffer) while it plays. The script's header: +0x2 how many records, +0x4 + k x 4
 * object group k's list (offset; a count, then 0x10-byte entries named at +0x4); its records
 * (12 bytes from +0x1C, one per shot): +0x24 the actors in it (bit 0 the camera), +0x28 the
 * doors animated, +0x29 the object groups animated. A shot: offsets (0: none) to each actor's
 * keys (+0 + i x 4; the camera's: 32-byte keys from +0x10), the doors' (+0x80 + k x 4; 12-byte
 * keys from +0x20) and the groups' (+0xA0 + k x 4).
 *
 *   +0x4    no letterbox            +0x8    the next event (+0x30 runs it)
 *   +0xC    the frame, +0x10 last frame's
 *   +0x20 / +0x24 [8] / +0x44 [8]  the shot's camera / door / group keys
 *   +0x64   the buffer playing; +0x68 two buffers (12 bytes: the shot, its state 0 free /
 *           1 loading / 2 loaded / 3 playing, its load handle)
 *   +0x80   32 actor slots (12 bytes; slot 0 the camera): +0 in the cutscene, +1 animated,
 *           +2 left alone, +3 no model hook, +4 its state before (1 active, 0x80 shown), +8
 *           the character
 *   +0x200  status: 1 loading, 2 ready, 3 playing, 4 waiting for a shot, 5 over
 *   +0x204  keep going past the end, +0x205 start when ready
 *   +0x206  16 signals' counts, +0x216 the frame each was last seen
 *   +0x238 / +0x248  effect slot 0x1D's parameters at the start; +0x268 / +0x278 slot 0x1C's
 *   +0x298 / +0x299  the two fades' last levels, +0x29A the four fade levels
 *   +0x2A0  the state (a member function) */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "ptmf.h"
#include "sce/libvu0.h"

extern VObject *gCharacters[6];
extern VObject *gFileLoader;
extern VObject *D_0044FE10;   /* the director */
extern VObject *D_0044E4B8;   /* the camera */
extern VObject *D_0044E4C8;   /* the scene's lights */
extern u8 *D_0044E4C0;        /* the effects (SceneGame +0xF6CD30) */
extern VObject *D_0044E4F0;   /* the renderer */
extern VObject *D_0044E4F8;   /* the camera director */
extern VObject *D_0044E558;   /* the doors */
extern VObject *D_0044E7A8;   /* the screen fades */
extern VObject *D_00456DF8;   /* the room's placed objects */
extern const char D_0045D2A8[], D_0045D2B8[], D_0045D2C8[], D_0045D2D8[];   /* "%s\\CUT%03X.DP", "%s\\%s.DH", "%s\\MARK.BIN", "%s\\PARAMS.BIN" */
extern const PTMF D_00412920, D_00412930, D_00412940;   /* states: loading, first shot, playing */
extern void *D_0046EB40[], *D_0046EC60[];   /* effect classes for slots 0x1D / 0x1C */

extern s32 func_0026EDD0(char *buf, s32 size, const char *fmt, ...);   /* snprintf */
extern s32 func_001770D0(Progress *p, s32 kind);   /* the slot of character kind (0xFF) */
extern u8 func_00177160(Progress *p, u32 slot);    /* active */
extern s32 func_001771A0(Progress *p, u32 slot);   /* make active */
extern void func_0016D050(Progress *p, s32 slot);  /* its cutscene motion buffer */
extern void func_0016CF50(Progress *p, s32 slot);  /* ... given back */
extern s32 func_002CC5A0(u8 *d, s32 actor);        /* actor -> character kind */
extern void func_002CBFF0(u8 *d, s32 rec);         /* reset the slots of a record */
extern void func_001F4910(u8 *m);                  /* a model's motion reset */
extern void func_002DD040(u8 *m, s32 anim);
extern void func_002DD090(u8 *m);
extern void *func_00266C40(u8 *fx, s32 n);         /* effect slot n */
extern s32 func_00266C70(u8 *fx, s32 n, void *arg);
extern void *func_002672F0(u32 size, void *place);
extern f32 func_002E2D00(f32 angle);               /* wrap an angle */

u32 func_002C9930(u8 *d, s32 kind);
void func_002CBBE0(u8 *d);
void func_002CBD10(u8 *d);
void func_002CC130(u8 *d, s32 b, s32 rec);
extern void func_002670F0(u8 *fx, s32 n);          /* effect slot n gone */

#define SCRIPT(d) AT(d, 0x18, u8 *)
#define REC(d, r) (SCRIPT(d) + (r) * 12)     /* fields at +0x24 / +0x28 / +0x29 */
#define SLOT(d, i) ((d) + 0x80 + (i) * 12)
#define SLOT_CHAR(s) AT(s, 0x8, u8 *)
#define BUF(d, b) ((d) + 0x68 + (b) * 12)    /* +0 shot, +4 state, +8 handle */
#define SHOT(b) ((u8 *)gProgress + (b) * 0x60000 + 0xA6C0)
#define FRAME(d) AT(d, 0xC, s32)
#define LAST(d) AT(d, 0x10, s32)

/* the vtable's: the record at frame f (+0x1C), the key at f (+0x20), the shot after f's
   (+0x24), the length (+0x28), f's signal bits (+0x4C), the signal count since last frame */
#define REC_AT(d, f) VCALL((VObject *)(d), 0x1C, s32 (*)(u8 *, s32))(d, f)
#define KEY_AT(d, f) VCALL((VObject *)(d), 0x20, s32 (*)(u8 *, s32))(d, f)
#define LENGTH(d) VCALL((VObject *)(d), 0x28, s32 (*)(u8 *))(d)
#define SIGNALS(d, f) VCALL((VObject *)(d), 0x4C, u16 (*)(u8 *, s32))(d, f)
#define SIGNALED(d, bit, at) VCALL((VObject *)(d), 0x54, s32 (*)(u8 *, s32, s16 *))(d, bit, at)

/* a shot part: base + offset, 0 none */
static u8 *shot_part(u8 *base, u32 off) {
    s32 o = AT(base, off, s32);

    return o != 0 ? base + o : NULL;
}

/* all the records' actors / object groups */
static u32 script_actors(u8 *d) {
    u32 m = 0;
    s32 i;

    for (i = 0; i < AT(SCRIPT(d), 0x2, u16); i++) {
        m |= AT(REC(d, i), 0x24, u32);
    }
    return m;
}

static u8 script_groups(u8 *d) {
    u8 m = 0;
    s32 i;

    for (i = 0; i < AT(SCRIPT(d), 0x2, u16); i++) {
        m |= AT(REC(d, i), 0x29, u8);
    }
    return m;
}

/* object group k's list: a count, then 0x10-byte entries named at +0x4 */
static s32 *group_list(u8 *d, s32 k) {
    return (s32 *)(SCRIPT(d) + AT(SCRIPT(d), 0x4 + k * 4, s32));
}

static u8 *group_object(s32 *list, s32 i) {
    return VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, void *))(D_00456DF8, (u8 *)list + 4 + i * 0x10);
}

/* actor i's character kind, and that character's slot (0xFF none) */
static u32 actor_slot(u8 *d, s32 i) {
    return func_002C9930(d, func_002CC5A0(d, i));
}

#define NAME(d) AT(d, 0x14, const char *)

/* the file loader: `file` into dest under handle */
static void load(const char *file, void *dest, s32 handle) {
    VCALL(gFileLoader, 0xC, void (*)(VObject *, const char *, void *, s32, s32))(gFileLoader, file, dest, handle, 0);
}

/* buffer b's shot loading ("name\\CUTnnn.DP") */
static void shot_load(u8 *d, s32 b) {
    char file[0x20];

    func_0026EDD0(file, 0x20, D_0045D2A8, NAME(d), AT(BUF(d, b), 0x0, u32));
    load(file, SHOT(b), AT(BUF(d, b), 0x8, s32));
}

/* effect slot n of class vtbl, made if missing */
static void effect_need(u8 *fx, s32 n, void **vtbl) {
    VObject *pool = (VObject *)(fx + 0x1400);
    VObject **slot = &AT(fx, 0x1438 + n * 4, VObject *);
    void *mem;

    if (func_00266C40(fx, n) != NULL) {
        return;
    }
    if (*slot != NULL) {
        VCALL(pool, 0x14, void (*)(VObject *, void *))(pool, *slot);
        *slot = NULL;
    }
    mem = VCALL(pool, 0x10, void *(*)(VObject *, u32))(pool, 0xA0);
    if (mem != NULL) {
        VObject *e = func_002672F0(0xA0, mem);

        if (e != NULL) {
            e->vtbl = vtbl;
        }
        *slot = e;
        VCALL(*slot, 0xC, void (*)(VObject *))(*slot);
    }
}

/* ---- small methods ---- */

/* +0x34 status */
s32 func_002C9510(u8 *d) {
    return AT(d, 0x200, s32);
}

/* +0x3C status cleared */
void func_002CBFD0(u8 *d) {
    AT(d, 0x200, s32) = 0;
}

/* +0x8C the next event run */
void func_002C9520(u8 *d) {
    VCALL((VObject *)d, 0x30, void (*)(u8 *, s32))(d, AT(d, 0x8, s32));
    AT(d, 0x8, s32)++;
}

/* this frame's signal 2 */
s32 func_002C9560(u8 *d) {
    return (SIGNALS(d, FRAME(d)) & 4) != 0;
}

/* this frame's signals 3..4 */
s32 func_002C9590(u8 *d) {
    return (u8)VCALL((VObject *)d, 0x50, s32 (*)(u8 *, s32, s32, s32))(d, FRAME(d), 3, 4);
}

void func_002C95C0(u8 *d, u8 v) {
    AT(d, 0x4, u8) = v;
}

s32 func_002C9600(u8 *d) {
    return FRAME(d);
}

void func_002C9610(u8 *d, u8 v) {
    AT(d, 0x204, u8) = v;
}

/* the frame passed 17 before the end */
s32 func_002C9690(u8 *d) {
    s32 f = LENGTH(d) - 0x11;

    return LAST(d) < f && FRAME(d) >= f;
}

/* +0x8 the state run */
void func_002CBFE0(u8 *d) {
    ptmf_scall(d, &AT(d, 0x2A0, PTMF));
}

/* the slot of character kind `kind`, or of one of its variants */
u32 func_002C9930(u8 *d, s32 kind) {
    static const s8 sAlt[][4] = {
        { 2, 6, 7, 0x1B }, { 3, 0x22, 0x23, 0x24 }, { 10, 0x27, -1, -1 }, { 23, 0x17, 0x25, -1 },
    };
    Progress *p = gProgress;
    u32 s = (u8)func_001770D0(p, kind & 0xFF);
    u32 i, j;

    if (s != 0xFF) {
        return s;
    }
    for (i = 0; i < sizeof(sAlt) / sizeof(sAlt[0]); i++) {
        if (sAlt[i][0] != kind) {
            continue;
        }
        for (j = 1; j < 4 && sAlt[i][j] != -1 && s == 0xFF; j++) {
            s = (u8)func_001770D0(p, sAlt[i][j]);
        }
    }
    return s;
}

/* frame f's signals lo..hi as bits from 0 */
u16 func_002C9A80(u8 *d, s32 f, s8 lo, s8 hi) {
    u16 out = 0;
    s32 i;

    for (i = lo; i <= hi; i++) {
        if ((1 << i) & SIGNALS(d, f)) {
            out += (1 << (i - lo)) & 0xFFFF;
        }
    }
    return out;
}

/* +0x54: how often signal `bit` came since last frame (through this one); *at the last
   frame it came at */
s32 func_002C9B50(u8 *d, s32 bit, s16 *at) {
    s32 n = 0;
    s16 f;

    for (f = LAST(d) + 1; FRAME(d) >= f; f++) {
        if ((1 << bit) & SIGNALS(d, f)) {
            n++;
            if (at != NULL) {
                *at = f;
            }
        }
    }
    return n;
}

/* +0x4C: frame f's signal bits */
u16 func_002C9C10(u8 *d, s32 f) {
    if (f < 0 || f >= LENGTH(d)) {
        return 0;
    }
    return AT(d, 0x1C, u16 *)[f];
}

/* ---- the shot's parts ---- */

/* the letterbox: two black bars (layer 0x30) unless +0x4 */
void func_002C9EA0(u8 *d) {
    u64 *p;

    if (AT(d, 0x4, u8)) {
        return;
    }
    p = VCALL(D_0044E4F0, 0x10, u64 *(*)(VObject *, s32, s32))(D_0044E4F0, 7, 0x30);
    if (p == NULL) {
        return;
    }
    p[0] = 0x10000006;              /* DMA cnt 6 */
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x50000006;   /* VIF DIRECT 6 */
    p[2] = 1 | (0x10000000ULL << 32);   /* GIF tag: 1 A+D, EOP */
    p[3] = 0xE;
    p[4] = 6;                       /* PRIM: sprite */
    p[5] = 0;
    p[6] = 0x8001 | (0x64000000ULL << 32);   /* reglist x2: RGBAQ XYZ2 XYZ2 RGBAQ? */
    p[7] = 0xF55551;
    p[8] = 0x80000000ULL;           /* RGBAQ: black */
    p[9] = 0xFFFFFFFF72007000ULL;   /* top bar */
    p[10] = 0xFFFFFFFF75809000ULL;
    p[11] = 0xFFFFFFFF8A807000ULL;  /* bottom bar */
    p[12] = 0xFFFFFFFF8E009000ULL;
    p[13] = 0;
}

/* the object groups the script animates put back as they were defined */
void func_002C9C80(u8 *d) {
    u8 groups = script_groups(d);
    s32 k, i;

    for (k = 0; k < 8; k++) {
        s32 *list;

        if (!((1 << k) & groups)) {
            continue;
        }
        list = group_list(d, k);
        if (list == NULL) {
            continue;
        }
        for (i = 0; i < *list; i++) {
            u8 *o = group_object(list, i);

            if (o != NULL) {
                u8 *def = AT(o, 0x70, u8 *);

                sceVu0CopyVector((f32 *)(o + 0x10), (f32 *)(def + 0x10));
                sceVu0CopyVector((f32 *)(o + 0x20), (f32 *)(def + 0x20));
            }
        }
    }
}

/* this frame's keys of the shot's object groups (+0x44: a count, then 0x1C-byte entries named
   at +0xC with their keys at +8: 0x18 bytes, angles and a position) */
void func_002C9FB0(u8 *d) {
    s32 k;

    for (k = 0; k < 8; k++) {
        u32 *tr;
        u32 i;

        if (!(AT(REC(d, REC_AT(d, FRAME(d))), 0x29, u8) & (1 << k))) {
            continue;
        }
        tr = AT(d, 0x44 + k * 4, u32 *);
        for (i = 0; i < *tr; i++) {
            u8 *e = (u8 *)tr + 4 + i * 0x1C;
            u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, void *))(D_00456DF8, e + 0xC);

            if (o != NULL) {
                f32 *key = (f32 *)((u8 *)tr + AT(e, 0x8, s32) + KEY_AT(d, FRAME(d)) * 0x18);

                AT(o, 0x10, f32) = func_002E2D00(key[0]);
                AT(o, 0x14, f32) = func_002E2D00(key[1]);
                AT(o, 0x18, f32) = func_002E2D00(key[2]);
                AT(o, 0x20, f32) = key[3];
                AT(o, 0x24, f32) = key[4];
                AT(o, 0x28, f32) = key[5];
            }
        }
    }
}

/* this frame's lights and effects from the cues (progress +0x66C0: a count, then 0x30-byte
 * cues from +0x10, for record +0x2: type +0x1 - 0 the lights (colour +0x10, ambient +0x14,
 * their parameters; a second colour +0x24 if any), 1 / 2 effect slot 0x1C / 0x1D fed +0x10);
 * slot 0x1D otherwise as at the start (+0x238) */
void func_002CA130(u8 *d) {
    f32 col[4] __attribute__((aligned(16))) = { 0 };
    f32 amb[4] __attribute__((aligned(16))) = { 0 };
    VObject *lights = D_0044E4C8;
    u8 *cues = (u8 *)gProgress + 0x66C0;
    u8 *c = cues + 0x10;
    u8 *fx;
    s32 i;

    VCALL(lights, 0x48, void (*)(VObject *, s32, f32 *, f32))(lights, 0, amb, 0.0f);
    fx = D_0044E4C0;
    func_002670F0(fx, 0x1C);
    if (AT(d, 0x238, u8)) {
        effect_need(fx, 0x1D, D_0046EB40);
        func_00266C70(fx, 0x1D, d + 0x248);
    } else {
        func_002670F0(fx, 0x1D);
    }
    for (i = 0; i < AT(cues, 0x0, s32); i++, c += 0x30) {
        if (AT(c, 0x2, u16) != REC_AT(d, FRAME(d))) {
            continue;
        }
        switch (AT(c, 0x1, u8)) {
        case 0:
            col[0] = (f32)c[0x10];
            col[1] = (f32)c[0x11];
            col[2] = (f32)c[0x12];
            amb[0] = (f32)c[0x14];
            amb[1] = (f32)c[0x15];
            amb[2] = (f32)c[0x16];
            VCALL(lights, 0x48, void (*)(VObject *, s32, f32 *, f32))(lights, 1, amb, AT(c, 0x18, f32));
            VCALL(lights, 0x4C, void (*)(VObject *, s32, s32, f32 *, f32, f32))(lights, 0, 1, col, AT(c, 0x1C, f32),
                                                                                 AT(c, 0x20, f32));
            if (c[0x24] != 0 || c[0x25] != 0 || c[0x26] != 0) {
                col[0] = (f32)c[0x24];
                col[1] = (f32)c[0x25];
                col[2] = (f32)c[0x26];
                VCALL(lights, 0x4C, void (*)(VObject *, s32, s32, f32 *, f32, f32))(lights, 1, 1, col,
                                                                                     AT(c, 0x28, f32), AT(c, 0x2C, f32));
            }
            break;
        case 1:
            effect_need(fx, 0x1C, D_0046EC60);
            func_00266C70(fx, 0x1C, c + 0x10);
            break;
        case 2:
            effect_need(fx, 0x1D, D_0046EB40);
            func_00266C70(fx, 0x1D, c + 0x10);
            break;
        }
    }
}

/* this frame's camera key, when the camera is in the shot: position, target, two values
   (camera +0x1C, +0x28, +0x5C, +0x70) */
void func_002CA790(u8 *d) {
    f32 *k;

    if (!(AT(REC(d, REC_AT(d, FRAME(d))), 0x24, u32) & 1)) {
        return;
    }
    k = (f32 *)(AT(d, 0x20, u8 *) + 0x10 + (KEY_AT(d, FRAME(d)) << 5));
    VCALL(D_0044E4B8, 0x1C, void (*)(VObject *, f32, f32, f32))(D_0044E4B8, k[0], k[1], k[2]);
    VCALL(D_0044E4B8, 0x28, void (*)(VObject *, f32, f32, f32))(D_0044E4B8, k[3], k[4], k[5]);
    VCALL(D_0044E4B8, 0x5C, void (*)(VObject *, f32))(D_0044E4B8, k[6]);
    VCALL(D_0044E4B8, 0x70, void (*)(VObject *, f32))(D_0044E4B8, k[7]);
}

/* +0x14 each frame playing: a flash at signal 0, the camera, the doors, the object groups,
 * the actors' motions, lights and effects, the two fades (+0x70 / +0x74: only on a new frame,
 * 0x2D0 frames; reset on a held frame), the signals counted */
void func_002CA8C0(u8 *d) {
    u32 a, b;
    s32 i;

    if (SIGNALED(d, 0, NULL) > 0) {
        VCALL(D_0044E4F0, 0x5C, void (*)(VObject *))(D_0044E4F0);
    }
    func_002CA790(d);
    for (i = 0; i < 8; i++) {
        if (AT(REC(d, REC_AT(d, FRAME(d))), 0x28, u8) & (1 << i)) {
            u8 *keys = AT(d, 0x24 + i * 4, u8 *) + 0x20;

            VCALL(D_0044E558, 0x78, void (*)(VObject *, u32, f32))(D_0044E558, i & 0xFF,
                                                                   AT(keys + KEY_AT(d, FRAME(d)) * 12, 0x4, f32));
        }
    }
    func_002C9FB0(d);
    for (i = 1; i < 0x20; i++) {
        u8 *s = SLOT(d, i);

        if (s[0] != 0 && s[1] != 0) {
            func_002DD090(AT(SLOT_CHAR(s), 0xF0, u8 *));
        }
    }
    func_002CA130(d);
    a = (u8)VCALL((VObject *)d, 0x70, s32 (*)(u8 *))(d);
    b = (u8)VCALL((VObject *)d, 0x74, s32 (*)(u8 *))(d);
    if (LAST(d) != FRAME(d)) {
        if (a != AT(d, 0x298, u8)) {
            VCALL(D_0044E7A8, 0x14, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 2, a != 0, 0x2D0);
        }
        if (b != AT(d, 0x299, u8)) {
            VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, u8, s32))(D_0044E7A8, 2, AT(d, 0x29A + b, u8), 0x2D0);
        }
    } else {
        a = b = 0;
        VCALL(D_0044E7A8, 0x14, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 2, 0, 1);
        VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 2, 0, 1);
    }
    AT(d, 0x298, u8) = a;
    AT(d, 0x299, u8) = b;
    for (i = 0; i < 0x10; i++) {
        AT(d, 0x206 + i, s8) += (s8)SIGNALED(d, i, &AT(d, 0x216 + i * 2, s16));
    }
}

/* +0x48 the end: the last record's slots reset, the object groups back, the actors' cutscene
 * motion buffers given back, the actors released, the fades off, the effects as at the start,
 * the lights' ambient off */
void func_002CAB90(u8 *d) {
    f32 zero[4] __attribute__((aligned(16)));
    u8 *fx;
    s32 i;

    func_002CBFF0(d, REC_AT(d, LENGTH(d) - 1));
    func_002C9C80(d);
    for (i = 1; i < 0x20; i++) {
        u32 s;

        if (i == 2 || !((1 << i) & script_actors(d)) || SLOT(d, i)[2] != 0) {
            continue;
        }
        s = actor_slot(d, i);
        if (s != 0xFF) {
            func_0016CF50(gProgress, s);
        }
    }
    func_002CBBE0(d);
    VCALL(D_0044E7A8, 0x14, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 2, 0, 1);
    VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 2, 0, 1);
    fx = D_0044E4C0;
    if (AT(d, 0x238, u8)) {
        effect_need(fx, 0x1D, D_0046EB40);
        func_00266C70(fx, 0x1D, d + 0x248);
    } else {
        func_002670F0(fx, 0x1D);
    }
    if (AT(d, 0x268, u8)) {
        effect_need(fx, 0x1C, D_0046EC60);
        func_00266C70(fx, 0x1C, d + 0x278);
    } else {
        func_002670F0(fx, 0x1C);
    }
    zero[0] = zero[1] = zero[2] = 0.0f;
    VCALL(D_0044E4C8, 0x48, void (*)(VObject *, s32, f32 *, f32))(D_0044E4C8, 0, zero, 0.0f);
}


/* each frame playing: the shots streamed - past the end (unless +0x204) over; a buffer
 * loaded; a new shot swaps buffers; the playing one's shot loaded: it starts (the other
 * freed); not yet: wait (status 4), starting its load if it isn't loading; the other buffer
 * preloads the next shot (+0x24); on a cut, the renderer +0x1C */
void func_002CAFB0(u8 *d) {
    s32 b;

    AT(d, 0x200, s32) = 3;
    if (!AT(d, 0x204, u8) && (FRAME(d) >= LENGTH(d) || FRAME(d) < 0)) {
        AT(d, 0x200, s32) = 5;
        return;
    }
    for (b = 0; b < 2; b++) {
        if (AT(BUF(d, b), 0x4, s32) == 1 &&
            VCALL(gFileLoader, 0x28, s32 (*)(VObject *, s32))(gFileLoader, AT(BUF(d, b), 0x8, s32)) == 3) {
            AT(BUF(d, b), 0x4, s32) = 2;
        }
    }
    if (REC_AT(d, LAST(d)) != REC_AT(d, FRAME(d)) && LAST(d) >= 0 && FRAME(d) >= 0) {
        AT(d, 0x64, s32) ^= 1;
    }
    b = AT(d, 0x64, s32);
    if (AT(BUF(d, b), 0x4, s32) == 2 && AT(BUF(d, b), 0x0, s32) == REC_AT(d, FRAME(d))) {
        AT(BUF(d, b), 0x4, s32) = 3;
        func_002CC130(d, AT(d, 0x64, s32), AT(BUF(d, AT(d, 0x64, s32)), 0x0, s32));
        AT(BUF(d, AT(d, 0x64, s32) ^ 1), 0x4, s32) = 0;
        AT(BUF(d, AT(d, 0x64, s32) ^ 1), 0x0, s32) = -1;
    } else if (AT(BUF(d, b), 0x4, s32) != 0 && AT(BUF(d, b), 0x0, s32) == REC_AT(d, FRAME(d))) {
        if (AT(BUF(d, AT(d, 0x64, s32)), 0x4, s32) == 1 &&
            AT(BUF(d, AT(d, 0x64, s32)), 0x0, s32) == REC_AT(d, FRAME(d))) {
            AT(d, 0x200, s32) = 4;
        }
    } else {
        b = AT(d, 0x64, s32);
        if (AT(BUF(d, b), 0x4, s32) == 1) {   /* loading another shot: cancelled */
            VCALL(gFileLoader, 0x14, void (*)(VObject *, s32))(gFileLoader, AT(BUF(d, b), 0x8, s32));
            AT(BUF(d, AT(d, 0x64, s32)), 0x4, s32) = 0;
        } else {
            AT(BUF(d, AT(d, 0x64, s32)), 0x0, s32) = REC_AT(d, FRAME(d));
            AT(BUF(d, AT(d, 0x64, s32)), 0x4, s32) = 1;
            b = AT(d, 0x64, s32);
            shot_load(d, b);
        }
        AT(d, 0x200, s32) = 4;
    }
    b = AT(d, 0x64, s32) ^ 1;
    if (AT(BUF(d, b), 0x0, s32) != VCALL((VObject *)d, 0x24, s32 (*)(u8 *, s32))(d, FRAME(d)) ||
        AT(BUF(d, b), 0x4, s32) == 0) {
        b = AT(d, 0x64, s32) ^ 1;
        if (AT(BUF(d, b), 0x4, s32) == 1) {
            VCALL(gFileLoader, 0x14, void (*)(VObject *, s32))(gFileLoader, AT(BUF(d, b), 0x8, s32));
            AT(BUF(d, AT(d, 0x64, s32) ^ 1), 0x4, s32) = 0;
        } else {
            AT(BUF(d, AT(d, 0x64, s32) ^ 1), 0x0, s32) = VCALL((VObject *)d, 0x24, s32 (*)(u8 *, s32))(d, FRAME(d));
            AT(BUF(d, AT(d, 0x64, s32) ^ 1), 0x4, s32) = 1;
            b = AT(d, 0x64, s32) ^ 1;
            shot_load(d, b);
        }
    }
    if (REC_AT(d, FRAME(d)) > 0 && REC_AT(d, LAST(d)) != REC_AT(d, FRAME(d))) {
        VCALL(D_0044E4F0, 0x1C, void (*)(VObject *))(D_0044E4F0);
    }
}

/* state: the first shot loading - loaded: ready (status 2); with +0x205 it starts at once:
 * the actors cast, the camera director told (+0x14), effect slots 0x1D / 0x1C's parameters
 * kept, the fades from 0, status 3 */
void func_002CB500(u8 *d) {
    u8 *fx, *e;

    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, s32))(gFileLoader, AT(BUF(d, AT(d, 0x64, s32)), 0x8, s32)) != 3) {
        AT(d, 0x200, s32) = 1;
        return;
    }
    AT(d, 0x200, s32) = 2;
    if (!AT(d, 0x205, u8)) {
        return;
    }
    AT(BUF(d, AT(d, 0x64, s32)), 0x4, s32) = 2;
    AT(d, 0x2A0, PTMF) = D_00412940;
    func_002CBD10(d);
    if (D_0044E4F8 != NULL) {
        VCALL(D_0044E4F8, 0x14, void (*)(VObject *))(D_0044E4F8);
    }
    fx = D_0044E4C0;
    AT(d, 0x238, u8) = 0;
    e = func_00266C40(fx, 0x1D);
    if (e != NULL) {
        AT(d, 0x238, u8) = 1;
        AT(d, 0x248, s32) = AT(e, 0x10, s32);
        AT(d, 0x24C, s32) = AT(e, 0x14, s32);
        AT(d, 0x250, f32) = AT(e, 0x50, f32);
        AT(d, 0x254, f32) = AT(e, 0x54, f32);
        AT(d, 0x258, s32) = 0;
        AT(d, 0x25C, u8) = 0;
        AT(d, 0x25D, u8) = 0;
        AT(d, 0x25E, u8) = 0;
        AT(d, 0x25F, u8) = 0;
        AT(d, 0x260, u8) = 0;
        AT(d, 0x261, u8) = 0;
    }
    AT(d, 0x268, u8) = 0;
    e = func_00266C40(fx, 0x1C);
    if (e != NULL) {
        AT(d, 0x268, u8) = 1;
        AT(d, 0x278, f32) = AT(e, 0x50, f32);
        AT(d, 0x27C, f32) = AT(e, 0x54, f32);
        AT(d, 0x280, f32) = AT(e, 0x58, f32);
        AT(d, 0x284, f32) = AT(e, 0x5C, f32);
    }
    AT(d, 0x298, u8) = 0;
    AT(d, 0x299, u8) = 0;
    AT(d, 0x29A, u8) = 0;
    AT(d, 0x29B, u8) = 0x55;
    AT(d, 0x29C, u8) = 0xAA;
    AT(d, 0x29D, u8) = 0xFF;
    AT(d, 0x200, s32) = 3;
}

/* state: the script loading - loaded: the actors (not Hewie's slot 2, not left alone) get
 * their cutscene motion buffers (and +0x54), shot 0 starts loading, then the first-shot state */
void func_002CB6C0(u8 *d) {
    s32 i;

    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, s32))(gFileLoader, 0x10000000) == 3) {
        for (i = 1; i < 0x20; i++) {
            u32 s;

            if (i == 2 || !((1 << i) & script_actors(d)) || SLOT(d, i)[2] != 0) {
                continue;
            }
            s = actor_slot(d, i);
            if (s != 0xFF) {
                func_0016D050(gProgress, s);
                VCALL(gCharacters[s], 0x54, void (*)(VObject *))(gCharacters[s]);
            }
        }
        AT(BUF(d, AT(d, 0x64, s32)), 0x0, s32) = 0;
        AT(BUF(d, AT(d, 0x64, s32)), 0x4, s32) = 1;
        i = AT(d, 0x64, s32);
        shot_load(d, i);
        AT(d, 0x2A0, PTMF) = D_00412930;
    }
    AT(d, 0x200, s32) = 1;
}

/* +0x10 start the cutscene named +0x14: frames before the start, the buffers free (handles
 * 0x07000000 / 1), the signals cleared; the script, signal table and cues loading (handle
 * 0x10000000) and given to the director (+0x18); the slots' +2 / +3 cleared; status loading */
void func_002CB9A0(u8 *d) {
    u8 *p = (u8 *)gProgress;
    char file[0x20];
    s32 i;

    LAST(d) = -2;
    FRAME(d) = -1;
    AT(d, 0x8, s32) = 0;
    AT(d, 0x205, u8) = 0;
    AT(d, 0x64, s32) = 0;
    AT(d, 0x68, s32) = -1;
    AT(d, 0x6C, s32) = 0;
    AT(d, 0x70, s32) = 0x07000000;
    AT(d, 0x74, s32) = -1;
    AT(d, 0x78, s32) = 0;
    AT(d, 0x7C, s32) = 0x07000001;
    for (i = 0; i < 0x10; i++) {
        AT(d, 0x206 + i, s8) = 0;
        AT(d, 0x216 + i * 2, s16) = -1;
    }
    func_0026EDD0(file, 0x20, D_0045D2B8, NAME(d), NAME(d));
    load(file, p + 0x16C0, 0x10000000);
    func_0026EDD0(file, 0x20, D_0045D2C8, NAME(d));
    load(file, p + 0x26C0, 0x10000000);
    func_0026EDD0(file, 0x20, D_0045D2D8, NAME(d));
    load(file, p + 0x66C0, 0x10000000);
    VCALL(D_0044FE10, 0x18, void (*)(VObject *, void *, void *))(D_0044FE10, p + 0x16C0, p + 0x26C0);
    for (i = 0; i < 0x20; i++) {
        SLOT(d, i)[2] = 0;
        SLOT(d, i)[3] = 0;
    }
    AT(d, 0x200, s32) = 1;
    AT(d, 0x2A0, PTMF) = D_00412920;
}

/* the actors released: each back to how it was (shown +0xE0 from +4's 0x80, active +0x29 from
 * its 1), +0x50, the model hook off (+0x30, unless +3), its motion reset */
void func_002CBBE0(u8 *d) {
    s32 i;

    for (i = 1; i < 0x20; i++) {
        u8 *s = SLOT(d, i);
        u8 *c;

        if (s[0] == 0) {
            continue;
        }
        c = SLOT_CHAR(s);
        AT(AT(c, 0xF0, u8 *), 0x4C8, s32) = 0;
        AT(c, 0xE0, u8) = (s[4] & 0x80) ? 1 : 0;
        VCALL((VObject *)SLOT_CHAR(s), 0x50, void (*)(void *))(SLOT_CHAR(s));
        if (s[3] != 0) {
            VCALL(AT(SLOT_CHAR(s), 0xF0, VObject *), 0x30, void (*)(VObject *))(AT(SLOT_CHAR(s), 0xF0, VObject *));
        }
        func_001F4910(AT(SLOT_CHAR(s), 0xF0, u8 *));
        AT(SLOT_CHAR(s), 0x29, u8) = (s[4] & 0x7F) == 1 ? 0 : 1;
    }
}

/* the cast: each actor in the script gets its character (left-alone ones: their motion
 * buffer back instead); actors 1..25 are made active and taken (+0); its state before kept
 * (+4), +0x4C, the model hook on (+0x2C, unless +3) */
void func_002CBD10(u8 *d) {
    Progress *p = gProgress;
    s32 i;

    for (i = 1; i < 0x20; i++) {
        u8 *s = SLOT(d, i);
        u32 k;

        s[0] = 0;
        s[1] = 0;
        s[4] = 0;
        if (!((1 << i) & script_actors(d))) {
            continue;
        }
        k = func_002C9930(d, func_002CC5A0(d, i) & 0xFF);
        if (k == 0xFF) {
            continue;
        }
        if (s[2] != 0) {
            func_0016CF50(p, k);
            continue;
        }
        SLOT_CHAR(s) = (u8 *)gCharacters[k];
        if ((u32)(i - 1) < 25) {
            if (!func_00177160(p, k)) {
                func_001771A0(p, k);
            }
            s[0] = 1;
        }
        if (AT(SLOT_CHAR(s), 0x28, u8)) {
            s[4] = 1;
        }
        if (AT(SLOT_CHAR(s), 0xE0, u8)) {
            s[4] |= 0x80;
        }
        VCALL((VObject *)SLOT_CHAR(s), 0x4C, void (*)(void *))(SLOT_CHAR(s));
        if (s[3] == 0) {
            VCALL(AT(SLOT_CHAR(s), 0xF0, VObject *), 0x2C, void (*)(VObject *))(AT(SLOT_CHAR(s), 0xF0, VObject *));
        }
    }
}

/* shot buffer b's shot `rec` starts: the camera's keys (from the playing buffer); each
 * actor in it (1..25) driven by its keys (hidden flag off, animated, motion reset, keys
 * +0x4C8, shown, animation 0x8000 from outside), those out of it back to their own motion;
 * the doors' keys; the object groups in it shown with their keys, the others hidden */
void func_002CC130(u8 *d, s32 b, s32 rec) {
    u8 *shot = SHOT(b);
    u8 groups;
    s32 i, k;

    for (i = 0; i < 0x20; i++) {
        u8 *s = SLOT(d, i);

        if (s[0] == 0 && i != 0) {
            continue;
        }
        if (AT(REC(d, rec), 0x24, u32) & (1 << i)) {
            if (i == 0) {
                AT(d, 0x20, u8 *) = shot_part(SHOT(AT(d, 0x64, s32)), 0);
            } else if (i <= 25) {
                AT(SLOT_CHAR(s), 0x29, u8) = 0;
                s[1] = 1;
                func_001F4910(AT(SLOT_CHAR(s), 0xF0, u8 *));
                AT(AT(SLOT_CHAR(s), 0xF0, u8 *), 0x4C8, u8 *) = shot_part(shot, i * 4);
                AT(SLOT_CHAR(s), 0xE0, u8) = 1;
                func_002DD040(AT(SLOT_CHAR(s), 0xF0, u8 *), 0x8000);
            }
        } else if (i != 0) {
            AT(SLOT_CHAR(s), 0x29, u8) = 1;
            s[1] = 0;
            func_001F4910(AT(SLOT_CHAR(s), 0xF0, u8 *));
        }
    }
    for (k = 0; k < 8; k++) {
        if (AT(REC(d, rec), 0x28, u8) & (1 << k)) {
            AT(d, 0x24 + k * 4, u8 *) = shot_part(shot, 0x80 + k * 4);
        }
    }
    for (k = 0; k < 8; k++) {
        s32 *list;
        u8 hide;

        if (!(AT(REC(d, rec), 0x29, u8) & (1 << k))) {
            groups = script_groups(d);
            if (!((1 << k) & groups)) {
                continue;
            }
            hide = 1;
        } else {
            AT(d, 0x44 + k * 4, u8 *) = shot_part(shot, 0xA0 + k * 4);
            hide = 0;
        }
        list = group_list(d, k);
        if (list == NULL) {
            continue;
        }
        for (i = 0; i < *list; i++) {
            u8 *o = group_object(list, i);

            if (o != NULL) {
                o[0] = hide;
            }
        }
    }
}
