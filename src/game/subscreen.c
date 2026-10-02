/* The sub screen: the menu system of the title and of the in-game menus (vtable D_0047A790, at
 * SceneTitle +0x74A80; code 0x384C00..0x3A0000). Its textures are SUBSCR\SUBBASE.TEX and
 * SUBSCR\SUBBACK.TEX. */
#include "common.h"
#include "game.h"
#include "ptmf.h"

#define AT(p, off, type) (*(type *)((u8 *)(p) + (off)))

extern VObject *gFileLoader;
extern u8 *D_0044E978;          /* the system data; +0x30 the options */
extern VObject *D_0044E7A8;     /* the pad actuator (vibration) */
extern VObject *D_0044E560;     /* the sound driver */
extern VObject *D_00456DF0;
extern void *D_0044E958;        /* the movie playing */
extern void *D_0044E980;        /* the ADX sound system (music) */
extern VObject *D_0044E4F0;     /* the renderer */
extern void *D_0046C790[];      /* a pool entry */
extern void func_002B6340(void *movie);   /* apply the movie volume */
extern void func_002D1FD0(void *bgm);     /* apply the music volume */
extern void func_00120EC0(void *pool, u8 *base, u32 size, u32 n, u8 *used);

void func_00396900(void *s);
void func_00399920(void *s);

static const char sSubBase[] = "SUBSCR\\SUBBASE.TEX";
static const char sSubBack[] = "SUBSCR\\SUBBACK.TEX";

static inline void set_state(PTMF *dst, void (*fn)(void *)) {
    PTMF s = {0, -1, {(void *)fn}};

    if (ptmf_test(&s)) {
        *dst = s;
    }
}

/* placement new (pool entries) */
void *func_0025FF00(u32 size, void *p) {
    return p;
}

/* the entries' pool: the first 64 entries constructed again, the tables cleared, the block
 * pool (+0x1208) over all 192 */
void func_00264900(u8 *pool) {
    s32 i, j;

    for (i = 0; i < 0x40; i++) {
        u8 *e = func_0025FF00(0x18, pool + 8 + i * 0x18);

        if (e != NULL) {
            AT(e, 0x0, void **) = D_0046C790;
            AT(e, 0x4, s32) = -1;
            AT(e, 0x8, u8) = 0;
            AT(e, 0x10, s64) = 0;
        }
    }
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 0x40; i++) {
            AT(pool, 0x12E0 + j * 0x100 + i * 4, s32) = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        AT(pool, 0x15E0 + i * 4, s32) = 0;
    }
    func_00120EC0(pool + 0x1208, pool + 8, 0x18, 0xC0, pool + 0x1220);
}

/* start: load the textures, reset */
void func_00399A10(u8 *s) {
    VObject *ld = gFileLoader;
    s32 i;

    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sSubBase, s + 0x1740, 0x6000000, 0);
    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sSubBack, s + 0x11F40, 0x6000000, 0);
    func_00264900(s + 8);
    for (i = 0; i < 9; i++) {
        AT(s, 0x97740 + i * 4, s32) = 0;
    }
    for (i = 0; i < 0x80; i++) {
        AT(s, 0x15F8 + i * 2, u16) = 0;
    }
    AT(s, 0x4, u8) = 0;
    AT(s, 0xA8DE0, s32) = 0;
    AT(s, 0x16F8, u8) = 1;
    AT(s, 0xA8C50, u8) = 0;
    for (i = 0; i < 3; i++) {
        AT(s, 0xA8C51 + i, u8) = 0;
    }
    AT(s, 0xA8C55, u8) = 0;
    AT(s, 0xA8C56, u8) = 0;
    AT(s, 0xA8DE5, u8) = 0;
    AT(s, 0xA8C67, u8) = 0;
    AT(s, 0xA8C66, u8) = 0;
    set_state(&AT(s, 0x1714, PTMF), func_00396900);
    set_state(&AT(s, 0x16FC, PTMF), func_00399920);
}

/* apply the options: controller layout, vibration, sound output, volume, screen position */
void func_003921A0(VObject *s) {
    u8 *opt = D_0044E978 + 0x30;
    VObject *o;
    f32 vol;

    VCALL(s, 0x34, void (*)(VObject *, s32))(s, (s8)opt[6]);
    o = D_0044E7A8;
    VCALL(o, 0x10, void (*)(VObject *))(o);
    if ((s8)opt[4] == 1) {
        VCALL(o, 0x28, void (*)(VObject *, s32))(o, 1);
    } else {
        VCALL(o, 0x28, void (*)(VObject *, s32))(o, 0);
    }
    o = D_0044E560;
    VCALL(o, 0x68, void (*)(VObject *, s32))(o, (s8)opt[0]);
    vol = *(f32 *)(opt + 8);
    VCALL(o, 0xA8, void (*)(VObject *, f32))(o, vol);
    if (D_00456DF0 != NULL) {
        VCALL(D_00456DF0, 0x20, void (*)(VObject *, f32))(D_00456DF0, vol);
    }
    if (D_0044E958 != NULL) {
        f32 *v = &AT(D_0044E958, 0x1D0, f32);

        *v = vol;
        if (vol < 0.0f) {
            *v = 0.0f;
        }
        if (!(*v <= 1.0f)) {
            *v = 1.0f;
        }
        func_002B6340(D_0044E958);
    }
    if (D_0044E980 != NULL) {
        f32 *v = &AT(D_0044E980, 0x11C, f32);

        *v = vol;
        if (vol < 0.0f) {
            *v = 0.0f;
        }
        if (!(*v <= 1.0f)) {
            *v = 1.0f;
        }
        func_002D1FD0(D_0044E980);
    }
    VCALL(D_0044E4F0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4F0, (s8)opt[2], (s8)opt[3]);
}

extern u16 D_0044B5E0[][6][2];   /* per controller layout: 6 x (button, its name's message) */
extern u8 D_0047E3C0[];          /* button mapping (+0xA: the six configurable actions) */
extern u8 D_0047B150[6];         /* the name slot of each action */
extern void func_00380990(void *self, s32 slot, s32 id);

/* +0x34 apply controller layout `type`: map the six actions and name their buttons */
void func_003913B0(u8 *s, s32 type) {
    s32 i;

    for (i = 0; i < 6; i++) {
        D_0047E3C0[0xA + i] = D_0044B5E0[type][i][0];
        func_00380990(s + 0x97764, D_0047B150[i], D_0044B5E0[type][i][1]);
    }
}
