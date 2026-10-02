/* Character models (see docs/model_format.md): the model classes' construction. Model base
 * class vtable D_0046F9E0 (constructed through D_0046B210); Hewie's model D_0046B240. */
#include "common.h"
#include "game.h"
#include "progress.h"

extern void *D_00469D00[], *D_0046ADA0[], *D_0046B210[], *D_0046F9E0[], *D_0046B240[], *D_0046B0C0[];
extern void *gCharacters[6];
extern void *func_0016F740(void *p);
extern void func_00100340(void *array, void *(*ctor)(void *), void *(*dtor)(void *, s32), u32 size, u32 n);
extern void *func_001F7E40(void *, s32);
extern void *func_001706F0(void *);
extern void *func_0016FC80(void *, s32);

/* placement new (models) */
void *func_002DC6E0(u32 size, void *p) {
    return p;
}

/* the model's drawing object (vtable D_0046ADA0, on the overlay base D_00469D00) */
void *func_0016F770(u8 *o) {
    s32 i;

    AT(o, 0x0, void **) = D_00469D00;
    AT(o, 0x4, s32) = -1;
    AT(o, 0x0, void **) = D_0046ADA0;
    AT(o, 0xC, s32) = 0;
    AT(o, 0x10, s32) = 0;
    AT(o, 0x8, s32) = 0;
    AT(o, 0x14, u8) = 0xFF;
    AT(o, 0x28, s32) = 0;
    AT(o, 0x2C, s32) = 0;
    AT(o, 0x30, s32) = 0;
    AT(o, 0x38, s32) = 0;
    AT(o, 0x3C, s32) = 0;
    AT(o, 0x40, s32) = 0;
    for (i = 0; i < 16; i++) {
        AT(o, 0x48 + i * 4, s32) = 0;
    }
    for (i = 0; i < 0x70; i++) {
        AT(o, 0x88 + i, u8) = 0;
    }
    AT(o, 0x20, u8) = 0;
    AT(o, 0x24, s32) = 0;
    return o;
}

/* the model base class's constructor */
void *func_0016F4B0(u8 *m) {
    s32 i, k;

    AT(m, 0x0, void **) = D_0046B210;
    func_0016F770(m + 0x10);
    func_0016F740(m + 0x1D0);
    for (i = 0; i < 2; i++) {
        AT(m, 0x584 + i * 0xA0, s32) = 0;
        AT(m, 0x58C + i * 0xA0, s32) = 0;
        AT(m, 0x594 + i * 0xA0, s32) = 0;
        AT(m, 0x588 + i * 0xA0, s32) = 0;
        AT(m, 0x590 + i * 0xA0, s32) = 0;
        AT(m, 0x598 + i * 0xA0, s32) = 0;
        for (k = 0; k < 24; k++) {
            AT(m, 0x59C + i * 0xA0 + k * 4, s32) = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        AT(m, 0x6DC + i * 0x60, s32) = 0;
        AT(m, 0x6E0 + i * 0x60, s32) = 0;
        AT(m, 0x6F8 + i * 0x60, s32) = 0;
        AT(m, 0x6FC + i * 0x60, s32) = 0;
    }
    AT(m, 0x4C0, s32) = 0;
    AT(m, 0x4C4, s32) = 0;
    AT(m, 0x4C8, s32) = 0;
    AT(m, 0x4CC, s32) = 0;
    AT(m, 0x4D0, s32) = 0;
    AT(m, 0x4D4, s32) = 0;
    AT(m, 0x4D9, u8) = 0;
    AT(m, 0x840, u16) = 0;
    AT(m, 0x844, s32) = 0;
    AT(m, 0x0, void **) = D_0046F9E0;
    return m;
}

/* an element of Hewie's model's first array (0x90 bytes) */
void *func_00208160(void *p) {
    u8 *e = p;

    AT(e, 0x58, void **) = D_0046B0C0;
    return e;
}

/* the partner's (Hewie's) model: allocated from the scene heap, put at character `slot` +0xF0 */
void func_003A10B0(Progress *p, u32 slot) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);
    u8 *m = func_002DC6E0(0xB90, VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, 0xB90));

    if (m != NULL) {
        func_0016F4B0(m);
        AT(m, 0x0, void **) = D_0046B240;
        AT(m, 0x890, u8) = 0;
        func_00100340(m + 0x960, func_00208160, func_001F7E40, 0x90, 2);
        func_00100340(m + 0xA80, func_001706F0, func_0016FC80, 0x60, 2);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

extern void *D_0046C160[], *D_0046B0E0[], *D_00470620[], *D_00472BD0[];
extern void *func_00208E30(void *, s32);
extern void *func_0016FB80(void *);
extern void *func_00170670(void *);
extern void *func_0016FC10(void *);
extern void *func_0016FBB0(void *, s32);
extern void *func_0016FB90(void *);
extern void *func_00170650(void *);

/* the human characters' model base: the model, two of 0x60 at +0x8D0 / +0x930, kind +0x9A0 */
void *func_00170690(u8 *m, s32 kind) {
    func_0016F4B0(m);
    AT(m, 0x0, void **) = D_0046C160;
    func_001706F0(m + 0x8D0);
    func_001706F0(m + 0x930);
    AT(m, 0x0, void **) = D_0046B0E0;
    AT(m, 0x9A0, u8) = kind;
    return m;
}

/* an element of Fiona's model's first array (0x50 bytes) */
void *func_00208E90(void *p) {
    u8 *e = p;

    AT(e, 0x30, void **) = D_00472BD0;
    return e;
}

/* the player's (Fiona's) model (0x1820 bytes), put at character `slot` +0xF0 */
void func_003A1860(Progress *p, u32 slot) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);
    u8 *m = func_002DC6E0(0x1820, VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, 0x1820));

    if (m != NULL) {
        u8 *e;

        func_00170690(m, 1);
        AT(m, 0x0, void **) = D_00470620;
        func_00100340(m + 0x9B0, func_00208E90, func_00208E30, 0x50, 0x20);
        func_0016FB80(m + 0x13B0);
        func_00170670(m + 0x13F0);
        func_0016FB80(m + 0x1440);
        func_00100340(m + 0x1480, func_0016FC10, func_0016FBB0, 0x50, 4);
        for (e = m + 0x15C0; e < m + 0x1740; e += 0x40) {
            func_0016FB90(e);
        }
        func_0016FB80(m + 0x1740);
        func_00170650(m + 0x1780);
        func_0016FB80(m + 0x17E0);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* ---- Fiona's model (vtable D_00470620): her files (the game starts with her in a sheet,
 * O_FIS) ---- */

extern void sceVu0CopyVector(f32 *dst, const f32 *src);
extern const char D_0045E520[];   /* "O_FIS\FIS_000.PCK" */
extern const char D_0045E500[];   /* "O_FIN\FIN_000.MRK" */
extern const char D_0045E540[];   /* "O_FIS\FIS_000.TEX" */
extern const char D_0045E560[];   /* "O_FIS\FIS_200.TEX" (the names are returned as such) */

/* +0xA0 the model file */
const char *func_002F7B80(void) {
    return D_0045E520;
}

/* +0xA4 the marker file */
const char *func_002F7B70(void) {
    return D_0045E500;
}

/* +0xA8 the textures */
const char *func_002F7B30(void) {
    return D_0045E540;
}

/* +0xAC the second texture set */
const char *func_002F7B40(void) {
    return D_0045E560;
}

/* +0xB0 the model file's buffer size */
u32 func_002F7B50(void) {
    return 0x41000;
}

/* +0xC8 set the vector at +0x1740 */
void func_002F7B60(u8 *m, const f32 *v) {
    sceVu0CopyVector((f32 *)(m + 0x1740), v);
}

/* ---- Hewie's model (vtable D_0046B240): his files by costume 0..4 ---- */

extern const char D_00456360[], D_00456380[], D_004563A0[], D_004563C0[], D_004563E0[];   /* HEW_00n.PCK */
extern const char D_004562C0[], D_004562E0[], D_00456300[], D_00456320[], D_00456340[];   /* HEW_00n.MRK */
extern const char D_00456400[];   /* O_HEW\HEW_000.TEX */

/* +0xA0 the model file of costume `n` */
const char *func_001F8090(void *m, s32 n) {
    switch (n) {
    case 0: return D_00456360;
    case 1: return D_00456380;
    case 2: return D_004563A0;
    case 3: return D_004563C0;
    case 4: return D_004563E0;
    }
    return NULL;
}

/* +0xA4 the marker file of costume `n` */
const char *func_001F8010(void *m, s32 n) {
    switch (n) {
    case 0: return D_004562C0;
    case 1: return D_004562E0;
    case 2: return D_00456300;
    case 3: return D_00456320;
    case 4: return D_00456340;
    }
    return NULL;
}

/* +0xA8 the textures (all costumes) */
const char *func_001F7F00(void) {
    return D_00456400;
}

extern void func_001F1FE0(u8 *m);
extern void func_002F80D0(u8 *m);

/* Fiona's model, once loaded: the base setup, the parts' roles (+0x890..: which mesh part is
 * which), her own setup, and per-part draw settings (4, 0x40) */
void func_002F8330(u8 *m) {
    static const u8 sParts[] = {0x98, 0x9A, 0x9C, 0x9E, 0xA0, 0xA2, 0xA6, 0xA8, 0xAA, 0xC0, 0xC2, 0xC4};
    s32 i;

    func_001F1FE0(m);
    AT(m, 0x890, s32) = 2;
    AT(m, 0x894, s32) = 3;
    AT(m, 0x898, s32) = 4;
    AT(m, 0x89C, s32) = 5;
    AT(m, 0x8B8, s32) = 0x32;
    AT(m, 0x8A0, s32) = 6;
    AT(m, 0x8A4, s32) = 7;
    AT(m, 0x8A8, s32) = 8;
    AT(m, 0x8AC, s32) = 9;
    AT(m, 0x8BC, s32) = 0x42;
    AT(m, 0x8B0, s32) = 0x35;
    AT(m, 0x8B4, s32) = 0x2B;
    func_002F80D0(m);
    for (i = 0; i < 12; i++) {
        AT(m, sParts[i], u8) = 4;
        AT(m, sParts[i] + 1, u8) = 0x40;
    }
}

extern void func_002118D0(u8 *m);

/* character model setup common to Fiona and the partner: base setup, defaults, then the
 * class's own reset (vt+0xB8) and two slot inits (vt+0xC4, slots 0 and 5) */
void func_001F1FE0(u8 *m) {
    func_002118D0(m);
    AT(m, 0x87C, s32) = 1;
    AT(m, 0x880, s32) = 1;
    AT(m, 0x860, f32) = 0.0f;
    AT(m, 0x864, f32) = 14.0f;
    AT(m, 0x868, f32) = 0.0f;
    ((void (*)(u8 *))AT(AT(m, 0, u8 *), 0xB8, void *))(m);
    ((void (*)(u8 *, s32))AT(AT(m, 0, u8 *), 0xC4, void *))(m, 0);
    ((void (*)(u8 *, s32))AT(AT(m, 0, u8 *), 0xC4, void *))(m, 5);
}

extern void func_002DE0A0(u8 *m);

/* base setup, then the class's post-setup hook (vt+0x54) */
void func_002118D0(u8 *m) {
    func_002DE0A0(m);
    ((void (*)(u8 *))AT(AT(m, 0, u8 *), 0x54, void *))(m);
}

extern void func_001F7C40(u8 *m);

void func_002DE0A0(u8 *m) {
    func_001F7C40(m);
    AT(m, 0x87C, s32) = 0;
    AT(m, 0x880, s32) = 0;
    AT(m, 0x854, s32) = 0;
    AT(m, 0x858, s32) = 0;
    AT(m, 0x870, s32) = 0;
    ((void (*)(u8 *))AT(AT(m, 0, u8 *), 0xB4, void *))(m);
}

extern void *D_004562A8;
extern u8 *func_0017D000(void *pool, s32 nBones);   /* allocate a skeleton */
extern f32 *func_0017CE80(void *skeleton, s32 bone); /* bone node */
extern s32 func_0017CE60(u8 *node, f32 *parent);
extern void func_001F4910(u8 *m);

/* build the model's skeleton from its bone table (+0x4C0: count, then 0x70-byte bones whose
 * +0x10 is the parent index, -1 for the root) */
void func_001F7C40(u8 *m) {
    u8 *skel;
    u8 *node;
    u8 *bone;
    s32 i;

    AT(m, 0x4C8, s32) = 0;
    skel = func_0017D000(D_004562A8, AT(AT(m, 0x4C0, u8 *), 0, s32));
    node = AT(skel, 4, u8 *);
    bone = AT(m, 0x4C0, u8 *) + 0x10;
    for (i = 0; i < AT(skel, 8, s32); i++) {
        AT(node, 0x40, s32) = i;
        if (AT(bone, 0, s32) >= 0)
            func_0017CE60(node, func_0017CE80(skel, AT(bone, 0, s32)));
        node = AT(node, 0x48, u8 *);
        bone += 0x70;
    }
    AT(m, 0x810, u8 *) = skel;
    AT(m, 0x18, u8 *) = AT(m, 0x810, u8 *);
    AT(m, 0x1C, u8 *) = AT(m, 0x4C0, u8 *);
    AT(m, 0x20, u8 *) = AT(m, 0x4D0, u8 *);
    AT(m, 0x1DC, u8 *) = AT(m, 0x810, u8 *);
    AT(m, 0x1D8, u8 *) = AT(m, 0x4CC, u8 *);
    AT(m, 0x4D8, u8) = 0;
    func_001F4910(m);
}

extern void *D_004562B0;
extern void func_0017CED0(void *pool, u8 *skel);   /* free a skeleton */
extern void func_00179BC0(void *pool, void *p);     /* free into D_004562B0 */

/* reset the model's motion state: the two motion slots (+0x564, 0xA0 each; current +0x540,
 * next +0x544) and the three blend channels (+0x6B0, 0x60 each), freeing their skeletons and
 * buffers */
void func_001F4910(u8 *m) {
    void *bufs = D_004562B0;
    void *skels = D_004562A8;
    u8 *slot;
    u8 *ch;
    s32 i, j, k;

    AT(m, 0x540, s32) = 0;
    AT(m, 0x544, s32) = AT(m, 0x540, s32) ^ 1;
    for (i = 0; i < 2; i++) {
        slot = m + 0x564 + i * 0xA0;
        for (j = 0; j < 2; j++) {
            u8 *s = slot + j * 4;

            func_0017CED0(skels, AT(s, 0x28, u8 *));
            AT(s, 0x28, s32) = 0;
            func_00179BC0(bufs, AT(s, 0x20, void *));
            func_00179BC0(bufs, AT(s, 0x30, void *));
            AT(s, 0x20, s32) = 0;
            AT(s, 0x30, s32) = 0;
            AT(slot, 0x1C, s32) = 0;
            AT(m, 0x55C + j * 4, s32) = -1;
            AT(m, 0x554 + j * 4, s32) = -1;
        }
        for (k = 0; k < 24; k++) {
            AT(slot, 0x38 + k * 4, s32) = 0;
        }
    }
    AT(m, 0x548, s32) = 0;
    AT(m, 0x54C, s32) = 0;
    AT(m, 0x550, s32) = 0;
    for (i = 0; i < 3; i++) {
        ch = m + 0x6B0 + i * 0x60;
        AT(ch, 0x0, s32) = 0;
        AT(ch, 0x4, s32) = AT(ch, 0x0, s32) ^ 1;
        AT(ch, 0x54, u8 *) = ch + AT(ch, 0x0, s32) * 0x1C + 0x1C;
        AT(ch, 0x58, u8 *) = ch + AT(ch, 0x4, s32) * 0x1C + 0x1C;
        AT(ch, 0x18, s32) = -1;
        AT(ch, 0x14, s32) = -1;
        AT(ch, 0x8, s32) = 0;
        AT(ch, 0xC, s32) = 0;
        AT(ch, 0x10, s32) = 0;
        for (j = 0; j < 2; j++) {
            u8 *s = ch + j * 0x1C;

            func_0017CED0(skels, AT(s, 0x30, u8 *));
            AT(s, 0x30, s32) = 0;
            func_00179BC0(bufs, AT(s, 0x2C, void *));
            AT(s, 0x2C, s32) = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        AT(m, 0x4DC + i * 4, s32) = -1;
        AT(m, 0x4F0 + i * 0x14, u8) = 0;
    }
    AT(m, 0x4EC, s32) = -1;
    AT(m, 0x6A4, u8 *) = m + 0x564 + AT(m, 0x540, s32) * 0xA0;
    AT(m, 0x6A8, u8 *) = m + 0x564 + AT(m, 0x544, s32) * 0xA0;
    AT(AT(m, 0x6A4, u8 *), 0x18, s32) = 16;
    AT(AT(m, 0x6A8, u8 *), 0x18, s32) = 16;
}

extern u8 D_003D5CC0[];

/* vt+0xB8: the character's table at +0x874 */
void func_001F1D90(u8 *m) {
    AT(m, 0x874, u8 *) = D_003D5CC0;
}

/* vt+0xC4 (the slot is ignored here) */
void func_00211160(u8 *m, s32 slot) {
    AT(m, 0x990, u8) = 1;
    AT(m, 0x8C4, f32) = 1.0f;
}

extern u8 D_0041A500[];

/* Fiona's vt+0xC4: 16 entries of the table D_0041A500 (+0x840 count, +0x844 table) */
void func_002F8430(u8 *m, s32 slot) {
    AT(m, 0x840, s16) = 16;
    AT(m, 0x844, u8 *) = D_0041A500;
}

/* empty character hook (vt+0xC4 of the base) */
void func_001F1F10(u8 *m, s32 slot) {
}

extern void func_002F7E90(u8 *m);
extern void func_002EE960(u8 *p);
extern void func_002F7C50(u8 *m);
extern void func_002F7B90(u8 *m);

/* Fiona's own setup: append her node (+0x13F0; next +0x28, prev +0x2C) to her list
 * (+0x1470 head, +0x1474 tail) and set her defaults */
void func_002F80D0(u8 *m) {
    u8 *node = m + 0x13F0;

    func_002F7E90(m);
    func_002EE960(m + 0x1440);
    if (AT(m, 0x1470, u8 *) != NULL && AT(m, 0x1474, u8 *) != NULL) {
        AT(AT(m, 0x1474, u8 *), 0x28, u8 *) = node;
        AT(node, 0x28, u8 *) = NULL;
        AT(node, 0x2C, u8 *) = AT(m, 0x1474, u8 *);
        AT(m, 0x1474, u8 *) = node;
    } else {
        AT(m, 0x1474, u8 *) = node;
        AT(m, 0x1470, u8 *) = node;
        AT(node, 0x2C, u8 *) = NULL;
        AT(node, 0x28, u8 *) = NULL;
    }
    AT(m, 0x1440, f32) = 0.0f;
    AT(m, 0x1444, u32) = 0x3DCCCCCD;   /* 0.1f (ee-gcc rounds the literal down) */
    AT(m, 0x1448, f32) = 0.0f;
    AT(m, 0x1450, u32) = 0x3F7D70A4;   /* 0.99f */
    AT(m, 0x1454, u8 *) = m;
    AT(m, 0x1460, u8) = 0;
    AT(m, 0x145C, s32) = 0;
    AT(m, 0x1430, f32) = 1.0f;
    AT(m, 0x1414, s32) = 0x3D;
    AT(m, 0x1410, u8) = 1;
    func_002F7C50(m);
    func_002F7B90(m);
    AT(m, 0x850, u8) = 1;
}

extern u8 D_0041A510[], D_0041A520[], D_0041A530[], D_0041A540[], D_0041A550[], D_0041A560[];

/* Fiona's 32 secondary-motion nodes (+0x9B0, 0x50 each; on the list +0x13E0/+0x13E4): 8
 * groups of 4 (the hair and clothes?), each node: +0x20 first of its group, +0x24 bone,
 * +0x40 weight, +0x44 kind, +0x48 table, +0x4C phase */
void func_002F7E90(u8 *m) {
    static u8 *const sTables[8] = {
        D_0041A560, D_0041A510, D_0041A510, D_0041A540,
        D_0041A550, D_0041A520, D_0041A520, D_0041A530,
    };
    static const s32 sKinds[8] = {2, 2, 6, 6, 2, 2, 6, 6};
    union { u32 u; f32 f; } twoPi = {0x40C90FDB};
    s32 i, g;

    func_002EE960(m + 0x13B0);
    for (i = 0; i < 32; i++) {
        u8 *node = m + 0x9B0 + i * 0x50;

        if (AT(m, 0x13E0, u8 *) != NULL && AT(m, 0x13E4, u8 *) != NULL) {
            AT(AT(m, 0x13E4, u8 *), 0x28, u8 *) = node;
            AT(node, 0x28, u8 *) = NULL;
            AT(node, 0x2C, u8 *) = AT(m, 0x13E4, u8 *);
            AT(m, 0x13E4, u8 *) = node;
        } else {
            AT(m, 0x13E4, u8 *) = node;
            AT(m, 0x13E0, u8 *) = node;
            AT(node, 0x2C, u8 *) = NULL;
            AT(node, 0x28, u8 *) = NULL;
        }
    }
    AT(m, 0x13B0, f32) = 0.0f;
    AT(m, 0x13B4, u32) = 0x3ECCCCCD;   /* 0.4f */
    AT(m, 0x13B8, f32) = 0.0f;
    AT(m, 0x13C0, u32) = 0x3F19999A;   /* 0.6f */
    AT(m, 0x13C4, u8 *) = m;
    AT(m, 0x13D0, u8) = 0;
    AT(m, 0x13CC, s32) = 0;
    for (i = 0; i < 4; i++) {
        f32 phase = twoPi.f * (f32)(i + 1) / 18.0f;

        for (g = 0; g < 8; g++) {
            u8 *node = m + 0x9B0 + (g * 4 + i) * 0x50;

            AT(node, 0x40, f32) = 1.0f;
            AT(node, 0x24, s32) = i + 0xB + g * 4;
            AT(node, 0x44, s32) = sKinds[g];
            AT(node, 0x48, u8 *) = sTables[g];
            AT(node, 0x20, u8) = (i == 0);
            AT(node, 0x4C, f32) = phase;
        }
    }
}

/* clear a secondary-motion set (its node list +0x30/+0x34) */
void func_002EE960(u8 *set) {
    AT(set, 0x34, s32) = 0;
    AT(set, 0x30, s32) = 0;
    AT(set, 0x18, s32) = 0;
}

extern void func_002EE690(u8 *col, s32 bone, f32 x, f32 y, f32 z, f32 r);

/* Fiona's second secondary-motion set (+0x1740): 4 nodes (+0x1480) on bones 0x39..0x3C, and
 * 6 collision spheres (+0x15C0, 0x40 each, chained by +0x2C from +0x1758) */
void func_002F7C50(u8 *m) {
    static const s32 sColBones[6] = {0x2E, 0x3E, 0x35, 0x2F, 0x3F, 0x35};
    s32 i;

    func_002EE960(m + 0x1740);
    for (i = 0; i < 4; i++) {
        u8 *node = m + 0x1480 + i * 0x50;

        if (AT(m, 0x1770, u8 *) != NULL && AT(m, 0x1774, u8 *) != NULL) {
            AT(AT(m, 0x1774, u8 *), 0x28, u8 *) = node;
            AT(node, 0x28, u8 *) = NULL;
            AT(node, 0x2C, u8 *) = AT(m, 0x1774, u8 *);
            AT(m, 0x1774, u8 *) = node;
        } else {
            AT(m, 0x1774, u8 *) = node;
            AT(m, 0x1770, u8 *) = node;
            AT(node, 0x2C, u8 *) = NULL;
            AT(node, 0x28, u8 *) = NULL;
        }
    }
    for (i = 0; i < 6; i++) {
        u8 *col = m + 0x15C0 + i * 0x40;

        AT(col, 0x2C, u8 *) = NULL;
        if (AT(m, 0x1758, u8 *) == NULL) {
            AT(m, 0x1758, u8 *) = col;
        } else {
            u8 *last = AT(m, 0x1758, u8 *);

            while (AT(last, 0x2C, u8 *) != NULL) {
                last = AT(last, 0x2C, u8 *);
            }
            AT(last, 0x2C, u8 *) = col;
        }
    }
    AT(m, 0x1740, f32) = 0.0f;
    AT(m, 0x1744, u32) = 0x3DCCCCCD;   /* 0.1f */
    AT(m, 0x1748, f32) = 0.0f;
    AT(m, 0x1750, u32) = 0x3F4CCCCD;   /* 0.8f */
    AT(m, 0x1754, u8 *) = m;
    AT(m, 0x1760, u8) = 0;
    AT(m, 0x175C, s32) = 0;
    for (i = 0; i < 4; i++) {
        u8 *node = m + 0x1480 + i * 0x50;

        AT(node, 0x40, u32) = 0x3EE66666;   /* 0.45f */
        AT(node, 0x24, s32) = 0x39 + i;
        AT(node, 0x20, u8) = (i == 0);
    }
    for (i = 0; i < 5; i++) {
        func_002EE690(m + 0x15C0 + i * 0x40, sColBones[i], 0.0f, 0.0f, 0.0f, 1.0f);
    }
    func_002EE690(m + 0x1700, sColBones[5], 0.0f, 1.0f, 0.0f, 1.0f);
}

/* set a collision sphere: centre (bone-local, w 1), radius and its inverse, bone */
void func_002EE690(u8 *col, s32 bone, f32 x, f32 y, f32 z, f32 r) {
    AT(col, 0x10, f32) = x;
    AT(col, 0x14, f32) = y;
    AT(col, 0x18, f32) = z;
    AT(col, 0x1C, f32) = 1.0f;
    AT(col, 0x20, f32) = r;
    AT(col, 0x24, f32) = 1.0f / r;
    AT(col, 0x28, s32) = bone;
}

/* Fiona's third secondary-motion set (+0x17E0): one node (+0x1780) on bone 0x2D */
void func_002F7B90(u8 *m) {
    u8 *node = m + 0x1780;

    func_002EE960(m + 0x17E0);
    if (AT(m, 0x1810, u8 *) != NULL && AT(m, 0x1814, u8 *) != NULL) {
        AT(AT(m, 0x1814, u8 *), 0x28, u8 *) = node;
        AT(node, 0x28, u8 *) = NULL;
        AT(node, 0x2C, u8 *) = AT(m, 0x1814, u8 *);
        AT(m, 0x1814, u8 *) = node;
    } else {
        AT(m, 0x1814, u8 *) = node;
        AT(m, 0x1810, u8 *) = node;
        AT(node, 0x2C, u8 *) = NULL;
        AT(node, 0x28, u8 *) = NULL;
    }
    AT(m, 0x17E0, f32) = 0.0f;
    AT(m, 0x17E4, f32) = 0.0f;
    AT(m, 0x17E8, f32) = 0.0f;
    AT(m, 0x17F0, f32) = 0.75f;
    AT(m, 0x17F4, u8 *) = m;
    AT(m, 0x1800, u8) = 0;
    AT(m, 0x17FC, s32) = 0;
    AT(m, 0x17C0, u32) = 0x3F666666;   /* 0.9f */
    AT(m, 0x17A4, s32) = 0x2D;
    AT(m, 0x17A0, u8) = 1;
    AT(m, 0x17D8, f32) = 1.0f;
    AT(m, 0x17D4, f32) = 1.0f;
    AT(m, 0x17D0, f32) = 1.0f;
}

extern s32 func_001F4710(u8 *m, s32 anim);   /* the animation's index in the table +0x874 (-1) */
extern void func_001F7890(u8 *m, s32 anim, u32 flags, s32 variant, f32 blend);

/* play animation `anim` (its flags from the table +0x874, 6 bytes per entry) from the start,
 * at the default speeds +0x87C / +0x880 */
void func_002DDE20(u8 *m, s32 anim, s32 variant) {
    s32 i = func_001F4710(m, anim);
    u32 flags = i != -1 ? AT(AT(m, 0x874, u8 *), i * 6 + 4, u16) : 0;

    AT(m, 0x85C, u8) = 0;
    AT(m, 0x85D, u8) = 0;
    AT(m, 0x38, s32) = AT(m, 0x87C, s32);
    AT(m, 0x3C, s32) = AT(m, 0x87C, s32);
    AT(m, 0x40, s32) = 0;
    AT(m, 0x48, s32) = AT(m, 0x880, s32);
    AT(m, 0x4C, s32) = AT(m, 0x880, s32);
    AT(m, 0x50, s32) = 0;
    func_001F7890(m, anim, flags & 0xFFFF, variant, 0.0f);
}

/* the index of animation `anim` in the motion file (+0x4C4: at its +0xC a table: count, then
 * 8-byte entries from +0x10 starting with the id); -1: not there */
s32 func_001F4710(u8 *m, s32 anim) {
    u8 *mtn = AT(m, 0x4C4, u8 *);
    u8 *tbl;
    u32 i;

    if (mtn == NULL) {
        return -1;
    }
    tbl = mtn + AT(mtn, 0xC, u32);
    for (i = 0; i < AT(tbl, 0, u32); i++) {
        if (AT(tbl, 0x10 + i * 8, s32) == anim) {
            return i;
        }
    }
    return -1;
}

/* the entry (0x14 bytes from +0x10: +0x4 the body's motion, +0x8.. the 3 parts') of animation
 * `anim` in motion file `mtn` (the last match in its id table); NULL: not there */
u8 *func_001F4B80(u8 *m, u8 *mtn, s32 anim) {
    u8 *tbl;
    s32 j = -1;
    u32 i;

    if (mtn == NULL) {
        return NULL;
    }
    tbl = mtn + AT(mtn, 0xC, u32);
    for (i = 0; i < AT(tbl, 0, u32); i++) {
        if (AT(tbl, 0x10 + i * 8, s32) == anim) {
            j = AT(tbl, 0x14 + i * 8, s32);
        }
    }
    if (j == -1) {
        return NULL;
    }
    return mtn + j * 0x14 + 0x10;
}

extern void func_001F71E0(u8 *m, s32 anim, s32 part, u32 flags, s32 variant, f32 blend);
extern void func_001F6E50(u8 *m, s32 anim, s32 part, u32 flags, u8 *channel, f32 blend);
extern void func_001F7460(u8 *m, s32 anim, u32 flags, s32 variant, f32 blend);

/* animation `anim`'s entry, from the main motion file (+0x4C4) or else the second (+0x4C8) */
static u8 *motion_entry(u8 *m, s32 anim) {
    u8 *e = func_001F4B80(m, AT(m, 0x4C4, u8 *), anim);

    if (e == NULL) {
        e = func_001F4B80(m, AT(m, 0x4C8, u8 *), anim);
    }
    return e;
}

/* start animation `anim`: the body (if the animation has it), then each of the 3 parts that
 * the animation covers (blend channels +0x6B0); a part it doesn't cover keeps / resumes its
 * own animation (+0x4E4) */
void func_001F7890(u8 *m, s32 anim, u32 flags, s32 variant, f32 blend) {
    s32 k;

    AT(m, 0x4E0, s32) = AT(m, 0x4DC, s32);
    AT(m, 0x4DC, s32) = anim;
    if (AT(motion_entry(m, anim), 0x4, s32) != 0) {
        func_001F71E0(m, anim, 1, flags, variant, blend);
    } else {
        for (k = 0; k < 3; k++) {
            if (AT(motion_entry(m, anim), 0x8 + k * 4, s32) != 0) {
                AT(m, 0x4E4 + k * 4, s32) = anim;
            }
        }
    }
    for (k = 0; k < 3; k++) {
        if (AT(motion_entry(m, anim), 0x8 + k * 4, s32) != 0) {
            func_001F6E50(m, anim, k + 2, flags, m + k * 0x60 + 0x6B0, blend);
            if (AT(m, 0x504 + k * 0x14, u8)) {
                AT(m, 0x504 + k * 0x14, u8) = 0;
                AT(m, 0x4E4 + k * 4, s32) = AT(m, 0x508 + k * 0x14, s32);
            }
        } else {
            s32 own = AT(m, 0x4E4 + k * 4, s32);

            if (own != -1 && own != AT(m, 0x6C8 + k * 0x60, s32)) {
                func_001F7460(m, own, flags, -1, blend);
            }
        }
    }
}

extern void func_001F5020(u8 *m, s32 slot);
extern void func_001F4C10(u8 *m, u8 **motion, u8 **skel, s32 anim, s32 part);
extern void func_001F6FD0(u8 *m, s32 anim, s32 variant);

#define MOTION_SLOT(m, i) ((m) + 0x564 + (i) * 0xA0)

/* the frame count of a slot's motion (its +0x4 header, +0xC) */
static s32 motion_frames(u8 *motion) {
    return AT(AT(motion, 0x4, u8 *), 0xC, s32);
}

/* start body animation `anim` (with `variant`, -1: none) in the other motion slot, cross-fading
 * from the current one over `blend`; synced animations (flag 2 in both) keep the phase */
void func_001F71E0(u8 *m, s32 anim, s32 part, u32 flags, s32 variant, f32 blend) {
    u8 *cur;
    u8 *prev;

    AT(m, 0x548, f32) = blend;
    AT(m, 0x54C, f32) = blend;
    AT(m, 0x550, f32) = 1.0f;
    AT(m, 0x544, s32) = AT(m, 0x540, s32);
    AT(m, 0x540, s32) ^= 1;
    AT(m, 0x6A4, u8 *) = MOTION_SLOT(m, AT(m, 0x540, s32));
    AT(m, 0x6A8, u8 *) = MOTION_SLOT(m, AT(m, 0x544, s32));
    AT(m, 0x554, s32) = AT(m, 0x55C, s32);
    AT(m, 0x55C, s32) = anim;
    AT(m, 0x558, s32) = AT(m, 0x560, s32);
    AT(m, 0x560, s32) = variant;
    cur = AT(m, 0x6A4, u8 *);
    AT(cur, 0x18, u32) = flags & 0xFFFF;
    if (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 8) {
        AT(AT(m, 0x6A4, u8 *), 0x18, u32) |= 0x10;
        if (AT(m, 0x6A8, u8 *) != NULL) {
            AT(AT(m, 0x6A8, u8 *), 0x18, u32) |= 0x10;
        }
    }
    func_001F5020(m, AT(m, 0x540, s32));
    cur = AT(m, 0x6A4, u8 *);
    func_001F4C10(m, (u8 **)(cur + 0x20), (u8 **)(cur + 0x28), anim, part);
    cur = AT(m, 0x6A4, u8 *);
    prev = AT(m, 0x6A8, u8 *);
    if (AT(cur, 0x18, u32) & AT(prev, 0x18, u32) & 2) {
        u8 *old = MOTION_SLOT(m, AT(m, 0x544, s32));

        AT(cur, 0x0, f32) = (f32)motion_frames(AT(cur, 0x20, u8 *)) *
                            (AT(old, 0x0, f32) / (f32)motion_frames(AT(old, 0x20, u8 *)));
    } else {
        AT(cur, 0x0, f32) = 0.0f;
    }
    AT(AT(m, 0x6A4, u8 *), 0x8, f32) = -1.0f;
    AT(AT(m, 0x6A4, u8 *), 0x10, f32) = 1.0f;
    AT(AT(m, 0x6A4, u8 *), 0x98, s32) = 0;
    if (variant != -1) {
        cur = AT(m, 0x6A4, u8 *);
        func_001F4C10(m, (u8 **)(cur + 0x24), (u8 **)(cur + 0x2C), variant, part);
        cur = AT(m, 0x6A4, u8 *);
        prev = AT(m, 0x6A8, u8 *);
        if (AT(cur, 0x18, u32) & AT(prev, 0x18, u32) & 2) {
            u8 *old = MOTION_SLOT(m, AT(m, 0x544, s32));

            AT(cur, 0x4, f32) = (f32)motion_frames(AT(cur, 0x24, u8 *)) *
                                (AT(old, 0x0, f32) / (f32)motion_frames(AT(old, 0x20, u8 *)));
        } else {
            AT(cur, 0x4, f32) = 0.0f;
        }
        AT(AT(m, 0x6A4, u8 *), 0x8, f32) = -1.0f;
        AT(AT(m, 0x6A4, u8 *), 0x14, f32) = 1.0f;
        AT(AT(m, 0x6A4, u8 *), 0x9C, s32) = 0;
    }
    func_001F6FD0(m, anim, variant);
}

/* free motion slot `i`'s two tracks (skeleton +0x28, data +0x20 / +0x30) and clear their
 * 12 keys (+0x38, 8 apart) */
void func_001F5020(u8 *m, s32 i) {
    void *skels = D_004562A8;
    void *bufs = D_004562B0;
    u8 *slot = MOTION_SLOT(m, i);
    s32 j, k;

    for (j = 0; j < 2; j++) {
        u8 *t = slot + j * 4;

        if (AT(t, 0x28, u8 *) != NULL) {
            func_0017CED0(skels, AT(t, 0x28, u8 *));
            AT(t, 0x28, s32) = 0;
        }
        if (AT(t, 0x20, void *) != NULL) {
            func_00179BC0(bufs, AT(t, 0x20, void *));
            AT(t, 0x20, s32) = 0;
        }
        if (AT(t, 0x30, void *) != NULL) {
            func_00179BC0(bufs, AT(t, 0x30, void *));
            AT(t, 0x30, s32) = 0;
        }
        for (k = 0; k < 12; k++) {
            if (AT(t, 0x38 + k * 8, s32) != 0) {
                AT(t, 0x38 + k * 8, s32) = 0;
            }
        }
    }
}

extern u8 *func_00179CD0(void *pool, s32 n);   /* allocate a chain of n entries */
extern void func_001F40F0(u8 *key, s32 a, u8 *data, s32 b);

/* set up a motion track of animation `anim`'s part `part` (1 the body, 2.. the 3 parts): a
 * chain of per-bone keys (*keys) and a skeleton (*skel), each bone with its index in the
 * model's bone table */
void func_001F4C10(u8 *m, u8 **keys, u8 **skel, s32 anim, s32 part) {
    u8 *e = motion_entry(m, anim);
    u8 *mot = e + AT(e, part * 4, u32);
    u8 *tracks;
    u8 *key;
    u8 *node;
    s32 i;

    *keys = func_00179CD0(D_004562B0, AT(mot, 0, s32));
    *skel = func_0017D000(D_004562A8, AT(mot, 0, s32));
    node = AT(*skel, 0x4, u8 *);
    key = AT(*keys, 0x4, u8 *);
    tracks = mot + AT(mot, 0x8, u32);
    for (i = 0; i < AT(*keys, 0x8, s32); i++) {
        u8 *bones;
        s32 bone;

        func_001F40F0(key + 4, AT(tracks, 0x4, s32), tracks + AT(tracks, 0x8, u32), AT(mot, 0x4, s32));
        bones = AT(m, 0x4C0, u8 *);
        bone = AT(bones, 0xC, u32) != 0 ? AT(bones + AT(bones, 0xC, u32) + AT(tracks, 0, u32), 1, s8) : 0;
        AT(key, 0x0, s32) = bone;
        AT(node, 0x40, s32) = bone;
        key = AT(key, 0x10, u8 *);
        node = AT(node, 0x48, u8 *);
        tracks += 0xC;
    }
}

/* the event keys of the current motion slot's animation and variant (-1: none): a chain per
 * track (+0x30 / +0x34) of the animation's event part, and the 12 event tracks (+0x38 + k * 8:
 * the key of track -1 - k, if any) */
void func_001F6FD0(u8 *m, s32 anim, s32 variant) {
    s32 ids[2];
    void *bufs = D_004562B0;
    s32 t, k, i;

    ids[0] = anim;
    ids[1] = variant;
    for (t = 0; t < 2; t++) {
        u8 **chain;
        u8 *e;
        u8 *evp;
        u8 *key;
        u8 *tracks;

        if (ids[t] == -1) {
            continue;
        }
        chain = (u8 **)(AT(m, 0x6A4, u8 *) + t * 4 + 0x30);
        e = motion_entry(m, ids[t]);
        evp = e + AT(e, 0, u32);
        *chain = func_00179CD0(bufs, AT(evp, 0, s32));
        key = AT(*chain, 0x4, u8 *);
        tracks = evp + AT(evp, 0x8, u32);
        for (i = 0; i < AT(*chain, 0x8, s32); i++) {
            func_001F40F0(key + 4, AT(tracks, 0x4, s32), tracks + AT(tracks, 0x8, u32), AT(evp, 0x4, s32));
            AT(key, 0x0, s32) = AT(tracks, 0, s32);
            key = AT(key, 0x10, u8 *);
            tracks += 0xC;
        }
        {
            u8 *c = AT(AT(m, 0x6A4, u8 *) + t * 4, 0x30, u8 *);

            for (k = 0; k < 12; k++) {
                u8 *x;

                AT(AT(m, 0x6A4, u8 *) + t * 4 + k * 8, 0x38, u8 *) = NULL;
                x = AT(c, 0x4, u8 *);
                for (i = 0; i < AT(c, 0x8, s32); i++) {
                    if (AT(x, 0, s32) == -1 - k) {
                        AT(AT(m, 0x6A4, u8 *) + t * 4 + k * 8, 0x38, u8 *) = x + 4;
                    }
                    x = AT(x, 0x10, u8 *);
                }
            }
        }
    }
}
