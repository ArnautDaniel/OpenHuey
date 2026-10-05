/* The placed things (D_0044F260, Progress +0x6FC340, vtable D_0046F5C0): up to 128 actors of
 * 0x140 bytes in a block pool (+0xA040) - the items lying about in the rooms, of kinds 0..10.
 * Each: +0x20 kind, +0x28 active, +0x30 room, +0x34 nav triangle, +0x10 position, +0xE4 age.
 * Kinds 0, 2, 3, 5, 7 and 8 are kept with the save (Progress +0xA14: 60 entries of { kind,
 * room, triangle, x, z, age }); at most 10 of a kind lie about (kind 10: 5), the oldest going
 * when another comes. */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "sce/libvu0.h"

extern VObject *D_0044E570;   /* the nav mesh */
extern void *func_00120D60(void *pool, u32 i);   /* BlockPool: block i if in use */
extern void *func_00121370(u32 size, void *place);   /* placement new */
extern void *D_00469C20[];   /* Actor */
extern void *D_0046F5C0[], *D_00469A00[], *D_004699C0[], *D_004699E0[], *D_0046A950[];
extern void *D_0046F520[], *D_004727E0[], *D_00472840[], *D_004758A0[], *D_00475A80[], *D_00475960[],
    *D_00475900[], *D_004759C0[], *D_00475A20[], *D_00479E70[], *D_00479ED0[], *D_00479500[];
extern VObject *D_0044F260;
extern void func_00100490(void *p);   /* operator delete */
extern void func_00121360(void *p);   /* delete (the pool's: nothing) */

#define POOL(m) ((m) + 0xA040)
#define SAVED(p) ((u8 *)(p) + 0xA14)
#define NUM_SAVED 60

/* kinds kept with the save */
static s32 kept_kind(u32 k) {
    switch (k) {
    case 0: case 2: case 3: case 5: case 7: case 8:
        return 1;
    }
    return 0;
}

/* the active thing in block i (NULL: none or out of range) */
static u8 *placed_active(u8 *m, s32 i) {
    u8 *t = func_00120D60(POOL(m), i);

    if (t != NULL && AT(t, 0x28, u8) == 1) {
        return t;
    }
    return NULL;
}

/* destructor (the pool +0xA040 with it) */
void *func_002D0EE0(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = D_0046F5C0;
        AT(m, 0xA040, void **) = D_004699C0;
        AT(m, 0xA040, void **) = D_004699E0;
        AT(m, 0x0, void **) = D_0046A950;
        D_0044F260 = NULL;
        if ((s16)flags > 0) {
            func_00100490(m);
        }
    }
    return m;
}

/* +0x8 a new thing of `kind` (0..10) from the pool (NULL: none / full) */
void *func_002D6B00(u8 *m, u32 kind) {
    static void **const sClass[11] = {
        D_0046F520, D_004727E0, D_00472840, D_004758A0, D_00475A80, D_00475960,
        D_00475900, D_004759C0, D_00475A20, D_00479E70, D_00479ED0,
    };
    void *mem;
    u8 *t;

    if (kind >= 11) {
        return NULL;
    }
    mem = VCALL((VObject *)m, 0x28, void *(*)(VObject *, u32))((VObject *)m, 0x140);
    if (mem == NULL) {
        return NULL;
    }
    t = func_00121370(0x140, mem);
    if (t != NULL) {
        AT(t, 0x0, void **) = D_00469C20;
        AT(t, 0x20, u32) = kind;
        AT(t, 0x24, u32) = 0x01000000;
        AT(t, 0x0, void **) = sClass[kind];
    }
    return mem;
}

/* the destructor of a thing (D_00479500) */
void *func_002D6F60(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479500;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            func_00121360(o);
        }
    }
    return o;
}

/* +0x14 another of `kind` came: when 10 (kind 10: 5) lie about, the oldest of them is aged out
 * (+0xE4 -1) */
void func_002D6FD0(u8 *m, u32 kind) {
    s32 max = 10, n = 0, i;
    u8 *oldest = NULL;

    switch (kind) {
    case 10:
        max = 5;
        break;
    case 0: case 2: case 3: case 5: case 7: case 8:
        break;
    default:
        return;
    }
    for (i = 0; i < 0x80; i++) {
        u8 *t = placed_active(m, i);

        if (t == NULL || AT(t, 0x20, u32) != kind) {
            continue;
        }
        if (n == 0 || AT(oldest, 0xE4, u32) < AT(t, 0xE4, u32)) {
            oldest = t;
        }
        if (++n == max) {
            AT(oldest, 0xE4, s32) = -1;
            return;
        }
    }
}

/* a save entry emptied */
void func_002A7700(s32 *e) {
    e[0] = -1;
    e[1] = -1;
    e[2] = -1;
    e[3] = 0;
    e[4] = 0;
    e[5] = 0;
}

/* +0x1C back from the save */
void func_002D7120(u8 *m) {
    s32 *e = (s32 *)SAVED(gProgress);
    s32 i;

    for (i = 0; i < NUM_SAVED && e[0] != -1; i++, e += 6) {
        u8 *t = VCALL((VObject *)m, 0x8, u8 *(*)(VObject *, s32))((VObject *)m, e[0]);

        if (t != NULL) {
            VCALL((VObject *)t, 0xC, void (*)(VObject *))((VObject *)t);
            AT(t, 0x28, u8) = 1;
            AT(t, 0x30, s32) = e[1];
            AT(t, 0x34, s32) = e[2];
            AT(t, 0x10, s32) = e[3];
            AT(t, 0x18, s32) = e[4];
            AT(t, 0xE4, s32) = e[5];
        }
    }
}

/* +0x18 into the save */
void func_002D71F0(u8 *m) {
    s32 *e = (s32 *)SAVED(gProgress);
    s32 n = 0, i;

    for (i = 0; i < 0x80; i++) {
        u8 *t = placed_active(m, i);

        if (t != NULL && kept_kind(AT(t, 0x20, u32))) {
            e[0] = AT(t, 0x20, u32);
            n++;
            e[1] = AT(t, 0x30, s32);
            e[2] = AT(t, 0x34, s32);
            e[3] = AT(t, 0x10, s32);
            e[4] = AT(t, 0x18, s32);
            e[5] = AT(t, 0xE4, s32);
            e += 6;
        }
    }
    for (; n < NUM_SAVED; n++, e += 6) {
        func_002A7700(e);
    }
}

/* +0x10 the first thing of `kind` from block `from` on (NULL: none) */
void *func_002D7320(u8 *m, u32 kind, s32 from) {
    s32 i;

    if (from < 0 || from >= 0x80) {
        return NULL;
    }
    for (i = from; i < 0x80; i++) {
        u8 *t = VCALL((VObject *)m, 0xC, u8 *(*)(VObject *, s32))((VObject *)m, i);

        if (t != NULL && AT(t, 0x20, u32) == kind) {
            return t;
        }
    }
    return NULL;
}

/* +0xC the thing in block i, if active */
void *func_002D73D0(u8 *m, s32 i) {
    u8 *t;

    if (i < 0 || i >= 0x80) {
        return NULL;
    }
    t = func_00120D60(POOL(m), i);
    if (t == NULL || AT(t, 0x28, u8) == 0) {
        return NULL;
    }
    return t;
}

/* +0x24 everything gone */
void func_002D7450(u8 *m) {
    s32 i;

    for (i = 0; i < 0x80; i++) {
        VObject *t = func_00120D60(POOL(m), i);

        if (t != NULL) {
            VCALL((VObject *)POOL(m), 0x14, void (*)(VObject *, void *))((VObject *)POOL(m), t);
            if (t != NULL) {
                VCALL(t, 0x8, void (*)(VObject *, s32))(t, 1);
            }
        }
    }
}

/* +0x20: put the kept kinds lying in the current room onto their nav mesh triangle (+0x34;
 * position +0x10, copied to +0x40) */
void func_002D69E0(u8 *mgr) {
    s32 room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    VObject *nav = D_0044E570;
    s32 i;

    for (i = 0; i < 0x80; i++) {
        u8 *t = placed_active(mgr, i);

        if (t != NULL && kept_kind(AT(t, 0x20, u32))) {
            if (AT(t, 0x30, s32) == room && AT(t, 0x34, s32) != -1) {
                VCALL(nav, 0x14, void (*)(VObject *, s32, f32 *))(nav, AT(t, 0x34, s32), (f32 *)(t + 0x10));
                sceVu0CopyVector((f32 *)(t + 0x40), (f32 *)(t + 0x10));
            }
        }
    }
}
