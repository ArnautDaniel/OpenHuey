/* Character models (see docs/model_format.md): the model classes' construction. Model base
 * class vtable D_0046F9E0 (constructed through D_0046B210); Hewie's model D_0046B240. */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "navmesh.h"
#include "model.h"
#include "input.h"

extern void *D_00469D00[], *D_0046ADA0[], *D_0046B210[], *D_0046F9E0[], *D_0046B240[], *D_0046B0C0[];
extern void *gCharacters[6];
extern void *func_0016F740(void *p);
extern void func_00100340(void *array, void *(*ctor)(void *), void *(*dtor)(void *, s32), u32 size, u32 n);
extern void *func_001F7E40(void *, s32);
extern void *func_001706F0(void *);
void *func_0016FC80(void *e, s32 flags);

/* placement new (models) */
void *func_002DC6E0(u32 size, void *p) {
    return p;
}

/* the model's drawing object (vtable D_0046ADA0, on the overlay base D_00469D00) */
static inline void DrawObj_Init(u8 *o) {
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
}

void *func_0016F770(u8 *o) {
    DrawObj_Init(o);
    return o;
}

/* the model base's own fields (after its drawing object and +0x1D0 part) */
static inline void ModelBase_Zero(u8 *m) {
    s32 i, k;

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
}

/* the model base class's constructor */
void *func_0016F4B0(u8 *m) {
    AT(m, 0x0, void **) = D_0046B210;
    func_0016F770(m + 0x10);
    func_0016F740(m + 0x1D0);
    ModelBase_Zero(m);
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

extern void *D_0046B9B0[];
extern void *func_0016FCD0(u8 *m);

/* the dog model's constructor (the plain base by func_0016FCD0), kind `kind` (+0x890) */
void *func_00208210(u8 *m, u8 kind) {
    func_0016FCD0(m);
    AT(m, 0x0, void **) = D_0046B240;
    AT(m, 0x890, u8) = kind;
    func_00100340(m + 0x960, func_00208160, func_001F7E40, 0x90, 2);
    func_00100340(m + 0xA80, func_001706F0, func_0016FC80, 0x60, 2);
    return m;
}

/* the second dog model (D_0046B9B0, 0xB90 bytes) for character `slot` */
void func_003A0F90(Progress *p, u32 slot) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);
    u8 *m = func_002DC6E0(0xB90, VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, 0xB90));

    if (m != NULL) {
        func_00208210(m, 2);
        AT(m, 0x0, void **) = D_0046B9B0;
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

extern void *D_0046C160[], *D_0046B0E0[], *D_00470620[], *D_00472BD0[];
extern void *func_00208E30(void *, s32);
extern void *func_0016FB80(void *);
extern void *func_00170670(void *);
extern void *func_0016FC10(void *);
void *func_0016FBB0(void *e, s32 flags);
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

extern void *D_0046FF40[], *D_0046FF00[];

/* a room effect of the event command 0x7F (0xA0 bytes): its vtable */
void *func_00208EF0(void *p) {
    AT(p, 0x0, void **) = D_0046FF00;
    return p;
}

/* a room effect of the event command 0x86 (0xA0 bytes): its vtable */
void *func_00208ED0(void *p) {
    AT(p, 0x0, void **) = D_0046FF40;
    return p;
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

/* ---- Fiona's model in her clothes (vtable D_00470540, O_FIN, costumes 0..6): its own
 * methods. Its secondary motion: five spring sets - +0xD70 the 12 hair / clothes nodes at
 * +0x9B0, +0xE00 one node (+0xDB0), +0x1100 four hanging nodes (+0xE40) with six collision
 * spheres (+0xF80), +0x11A0 one node (+0x1140), +0x1230 one node (+0x11E0) ---- */

extern void func_002EE960(u8 *set);
extern void func_002EE690(u8 *col, s32 bone, f32 x, f32 y, f32 z, f32 r);
extern void func_001F1FE0(u8 *m);
extern void func_002EE8A0(u8 *springs);
extern void func_002EE900(u8 *springs);
extern void func_002EE840(u8 *springs);
extern const char D_0045E3C0[], D_0045E3E0[], D_0045E400[], D_0045E420[], D_0045E440[],
    D_0045E460[], D_0045E480[];   /* "O_FIN\FIN_00n.PCK" */
extern u8 D_0041A4B0[], D_0041A4C0[], D_0041A4D0[], D_0041A4E0[], D_0041A4F0[];
extern f32 *func_0017CE80(void *skeleton, s32 bone); /* bone node */
extern void func_001F1FD0(u8 *m);
extern void *D_00470440[], *D_004703B0[], *D_004703D0[];
extern void func_001002C0(void *array, void *(*dtor)(void *, s32), u32 size, u32 n);   /* __destroy_arr */
void *func_0016F9E0(u8 *m, s32 flags);

/* append a node to a spring set's node list (+0x30 head, +0x34 tail; the node's next +0x28,
 * prev +0x2C) */
static inline void spring_link(u8 *set, u8 *node) {
    if (AT(set, 0x30, u8 *) != NULL && AT(set, 0x34, u8 *) != NULL) {
        AT(AT(set, 0x34, u8 *), 0x28, u8 *) = node;
        AT(node, 0x28, u8 *) = NULL;
        AT(node, 0x2C, u8 *) = AT(set, 0x34, u8 *);
        AT(set, 0x34, u8 *) = node;
    } else {
        AT(set, 0x34, u8 *) = node;
        AT(set, 0x30, u8 *) = node;
        AT(node, 0x2C, u8 *) = NULL;
        AT(node, 0x28, u8 *) = NULL;
    }
}

/* a spring set's gravity (0, g, 0), damping and owner */
static inline void spring_set(u8 *set, u32 g, u32 damping, u8 *owner) {
    AT(set, 0x0, f32) = 0.0f;
    AT(set, 0x4, u32) = g;
    AT(set, 0x8, f32) = 0.0f;
    AT(set, 0x10, u32) = damping;
    AT(set, 0x14, u8 *) = owner;
    AT(set, 0x20, u8) = 0;
    AT(set, 0x1C, s32) = 0;
}

/* +0x84..+0x94: part roles */
s32 func_002F6E10(void) { return 3; }
s32 func_002F6E20(void) { return 7; }
s32 func_002F6E30(void) { return 0x1C; }
s32 func_002F6E40(void) { return 0x2C; }
s32 func_002F6E50(void) { return 0x17; }

/* +0xB0 the model file's buffer size */
u32 func_002F6DE0(void) {
    return 0x41000;
}

/* +0xBC */
void func_002F6DF0(u8 *m, s32 on, f32 x) {
    AT(m, 0x111C, f32) = x;
    AT(m, 0x1120, u8) = on;
}

/* +0xC0 the point at +0x1190 */
void func_002F6E00(u8 *m, f32 x, f32 y, f32 z) {
    AT(m, 0x1190, f32) = x;
    AT(m, 0x1194, f32) = y;
    AT(m, 0x1198, f32) = z;
}

/* +0xB8: 16 entries of the table D_0041A4B0 (+0x840 count, +0x844 table) */
void func_002F7A10(u8 *m) {
    AT(m, 0x840, s16) = 16;
    AT(m, 0x844, u8 *) = D_0041A4B0;
}

/* +0xC4 which of the swappable parts show (part flag 2 hides): 0..4 one of the four at
 * +0xD8 / +0xE0 / +0xE2 / +0xDA (0 none), 5..9 one set of the eight at +0x9E.. */
void func_002F6E60(u8 *m, s32 look) {
    u32 k = look & 0xFF;

    if (k < 5) {
        AT(m, 0xD8, u8) |= 2;
        AT(m, 0xE0, u8) |= 2;
        AT(m, 0xE2, u8) |= 2;
        AT(m, 0xDA, u8) |= 2;
        switch (k) {
        case 1: AT(m, 0xD8, u8) &= ~2; break;
        case 2: AT(m, 0xE0, u8) &= ~2; break;
        case 3: AT(m, 0xE2, u8) &= ~2; break;
        case 4: AT(m, 0xDA, u8) &= ~2; break;
        }
        return;
    }
    AT(m, 0x9E, u8) |= 2;
    AT(m, 0xA0, u8) |= 2;
    AT(m, 0xA2, u8) |= 2;
    AT(m, 0xC4, u8) |= 2;
    AT(m, 0xA4, u8) |= 2;
    AT(m, 0xA6, u8) |= 2;
    AT(m, 0xC6, u8) |= 2;
    AT(m, 0xC8, u8) |= 2;
    switch (k) {
    case 5:
        AT(m, 0x9E, u8) &= ~2;
        AT(m, 0xA0, u8) &= ~2;
        break;
    case 6: AT(m, 0xA2, u8) &= ~2; break;
    case 7: AT(m, 0xC4, u8) &= ~2; break;
    case 8:
        AT(m, 0xA4, u8) &= ~2;
        AT(m, 0xA6, u8) &= ~2;
        break;
    case 9:
        AT(m, 0xC6, u8) &= ~2;
        AT(m, 0xC8, u8) &= ~2;
        break;
    }
}

/* +0xA0 the model file of costume `n` */
const char *func_002F7030(void *m, u32 n) {
    switch (n) {
    case 0: return D_0045E3C0;
    case 1: return D_0045E3E0;
    case 2: return D_0045E400;
    case 3: return D_0045E420;
    case 4: return D_0045E440;
    case 5: return D_0045E460;
    case 6: return D_0045E480;
    }
    return NULL;
}

/* the one-node set +0x11A0: node +0x1140 on bone 0x19 */
void func_002F70C0(u8 *m) {
    u8 *node = m + 0x1140;

    func_002EE960(m + 0x11A0);
    spring_link(m + 0x11A0, node);
    AT(m, 0x11A0, f32) = 0.0f;
    AT(m, 0x11A4, f32) = 0.0f;
    AT(m, 0x11A8, f32) = 0.0f;
    AT(m, 0x11B0, f32) = 0.75f;
    AT(m, 0x11B4, u8 *) = m;
    AT(m, 0x11C0, u8) = 0;
    AT(m, 0x11BC, s32) = 0;
    AT(node, 0x40, u32) = 0x3F666666;   /* 0.9f */
    AT(node, 0x24, s32) = 0x19;
    AT(node, 0x20, u8) = 1;
    AT(node, 0x58, f32) = 1.0f;
    AT(node, 0x54, f32) = 1.0f;
    AT(node, 0x50, f32) = 1.0f;
}

/* the set +0x1100: 4 nodes (+0xE40) on bones 0x25..0x28 and 6 collision spheres (+0xF80,
 * chained by +0x2C from +0x1118) */
void func_002F7180(u8 *m) {
    static const s32 sColBones[6] = {0x1A, 0x2A, 0x21, 0x1B, 0x2B, 0x21};
    u8 *set = m + 0x1100;
    s32 i;

    func_002EE960(set);
    for (i = 0; i < 4; i++) {
        spring_link(set, m + 0xE40 + i * 0x50);
    }
    for (i = 0; i < 6; i++) {
        u8 *col = m + 0xF80 + i * 0x40;

        AT(col, 0x2C, u8 *) = NULL;
        if (AT(m, 0x1118, u8 *) == NULL) {
            AT(m, 0x1118, u8 *) = col;
        } else {
            u8 *last = AT(m, 0x1118, u8 *);

            while (AT(last, 0x2C, u8 *) != NULL) {
                last = AT(last, 0x2C, u8 *);
            }
            AT(last, 0x2C, u8 *) = col;
        }
    }
    spring_set(set, 0x3DCCCCCD /* 0.1f */, 0x3F4CCCCD /* 0.8f */, m);
    for (i = 0; i < 4; i++) {
        u8 *node = m + 0xE40 + i * 0x50;

        AT(node, 0x40, u32) = 0x3EE66666;   /* 0.45f */
        AT(node, 0x24, s32) = 0x25 + i;
        AT(node, 0x20, u8) = (i == 0);
    }
    for (i = 0; i < 5; i++) {
        func_002EE690(m + 0xF80 + i * 0x40, sColBones[i], 0.0f, 0.0f, 0.0f, 1.0f);
    }
    func_002EE690(m + 0x10C0, sColBones[5], 0.0f, 1.0f, 0.0f, 1.0f);
}

/* the set +0xD70: 12 nodes (+0x9B0) in 6 pairs - bones, kinds and tables per pair, the first
 * of each pair leading, the phases 1 / 2 x 2 pi / 18 */
void func_002F73C0(u8 *m) {
    static u8 *const sTables[6] = {D_0041A4C0, D_0041A4C0, D_0041A4D0, D_0041A4D0, D_0041A4E0, D_0041A4F0};
    static const s32 sBones[6] = {0xB, 0x11, 0xD, 0x13, 0xF, 0x15};
    static const u32 sPhase[2] = {0x3EB2B8C3, 0x3F32B8C3};
    u8 *set = m + 0xD70;
    s32 i, j;

    func_002EE960(set);
    for (i = 0; i < 12; i++) {
        spring_link(set, m + 0x9B0 + i * 0x50);
    }
    spring_set(set, 0x3E4CCCCD /* 0.2f */, 0x3F666666 /* 0.9f */, m);
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 2; j++) {
            u8 *node = m + 0x9B0 + (i * 2 + j) * 0x50;

            AT(node, 0x40, u32) = 0x3ED182AA;
            AT(node, 0x24, s32) = sBones[i] + j;
            AT(node, 0x44, s32) = (i & 1) ? 6 : 2;
            AT(node, 0x48, u8 *) = sTables[i];
            AT(node, 0x20, u8) = (j == 0);
            AT(node, 0x4C, u32) = sPhase[j];
        }
    }
}

/* her own setup: the five spring sets (the +0xE00 node on bone 0x29, the +0x1230 one on
 * 0x30), then a reset (+0x850) */
void func_002F7620(u8 *m) {
    u8 *node;

    func_002F73C0(m);
    func_002EE960(m + 0xE00);
    node = m + 0xDB0;
    spring_link(m + 0xE00, node);
    spring_set(m + 0xE00, 0x3DCCCCCD /* 0.1f */, 0x3F7D70A4 /* 0.99f */, m);
    AT(node, 0x40, f32) = 1.0f;
    AT(node, 0x24, s32) = 0x29;
    AT(node, 0x20, u8) = 1;
    func_002F7180(m);
    func_002F70C0(m);
    func_002EE960(m + 0x1230);
    node = m + 0x11E0;
    spring_link(m + 0x1230, node);
    spring_set(m + 0x1230, 0x3DCCCCCD /* 0.1f */, 0x3F7D70A4 /* 0.99f */, m);
    AT(node, 0x40, f32) = 1.0f;
    AT(node, 0x24, s32) = 0x30;
    AT(node, 0x20, u8) = 1;
    AT(m, 0x850, u8) = 1;
}

/* the five spring sets, a frame: one step, or after a reset (+0x850) the 4 hanging nodes
 * (+0xE40) put back under their anchors by their length (+0x40) along bone 0x21's Z axis, at
 * rest - 30 steps to settle (as Fiona's sheet model, func_002F81A0) */
void func_002F7780(u8 *o) {
    static const u16 sSets[5] = {0xD70, 0xE00, 0x1100, 0x11A0, 0x1230};
    f32 down[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    s32 n, i, k;
    u8 *p;

    if (AT(o, 0x850, u8) == 0) {
        n = 1;
    } else {
        sceVu0CopyVector(down, func_0017CE80(AT(AT(o, 0x1114, u8 *), 0x810, void *), 0x21) + 8);
        p = o + 0xE40;
        for (i = 0; i < 4; i++) {
            AT(p, 0x18, f32) = 0.0f;
            AT(p, 0x14, f32) = 0.0f;
            AT(p, 0x10, f32) = 0.0f;
            if (AT(p, 0x20, u8) != 0) {
                sceVu0CopyVector(at, func_0017CE80(AT(AT(o, 0x1114, u8 *), 0x810, void *), AT(p, 0x24, s32)) + 12);
            } else {
                sceVu0CopyVector(at, AT(p, 0x2C, f32 *));
            }
            sceVu0ScaleVector(d, down, -AT(p, 0x40, f32));
            sceVu0AddVector((f32 *)p, at, d);
            p += 0x50;
        }
        n = 0x1E;
    }
    for (k = 0; k < 5; k++) {
        func_002EE8A0(o + sSets[k]);
    }
    for (i = 0; i < n; i++) {
        for (k = 0; k < 5; k++) {
            func_002EE900(o + sSets[k]);
        }
    }
    for (k = 0; k < 5; k++) {
        func_002EE840(o + sSets[k]);
    }
    AT(o, 0x850, u8) = 0;
}

/* +0x10 */
void func_002F7910(u8 *m) {
    func_001F1FD0(m);
}

/* +0xC loaded: the base setup, the parts' roles, her own setup, per-part draw settings */
void func_002F7920(u8 *m) {
    static const u8 sParts[] = {0x9C, 0xA8, 0xAA, 0xAC, 0xB2, 0xB4, 0xB6, 0xB8, 0xE4, 0xE6, 0xE8};
    s32 i;

    func_001F1FE0(m);
    AT(m, 0x890, s32) = 2;
    AT(m, 0x894, s32) = 3;
    AT(m, 0x898, s32) = 4;
    AT(m, 0x89C, s32) = 5;
    AT(m, 0x8B8, s32) = 0x1E;
    AT(m, 0x8A0, s32) = 6;
    AT(m, 0x8A4, s32) = 7;
    AT(m, 0x8A8, s32) = 8;
    AT(m, 0x8AC, s32) = 9;
    AT(m, 0x8BC, s32) = 0x2E;
    AT(m, 0x8B0, s32) = 0x21;
    AT(m, 0x8B4, s32) = 0x17;
    func_002F7620(m);
    AT(m, 0x9C, u8) = 4;
    AT(m, 0x9D, u8) = 0xC0;
    for (i = 1; i < 11; i++) {
        AT(m, sParts[i], u8) = 4;
        AT(m, sParts[i] + 1, u8) = 0x40;
    }
}

extern void func_001F7AC0(u8 *m);

/* Fiona's sheet model (D_00470620): +0x10 */
void func_002F8320(u8 *m) {
    func_001F7AC0(m);
}

/* Fiona's sheet model (D_00470620): +0x8 destructor */
void *func_002F7A30(void *p, s32 flags) {
    u8 *m = p;

    if (m != NULL) {
        AT(m, 0x0, void **) = D_00470620;
        AT(m, 0x17B0, void **) = D_00470440;
        AT(m, 0x17B0, void **) = D_004703B0;
        func_001002C0(m + 0x1480, func_0016FBB0, 0x50, 4);
        AT(m, 0x1420, void **) = D_004703D0;
        AT(m, 0x1420, void **) = D_004703B0;
        func_001002C0(m + 0x9B0, func_00208E30, 0x50, 0x20);
        AT(m, 0x0, void **) = D_0046B0E0;
        func_0016F9E0(m, 0);
        if ((s16)flags > 0) {
            func_002DC6D0(m);
        }
    }
    return m;
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

/* ---- the human characters' model base (vtable D_0046B0E0): its own methods ---- */

extern void func_002118C0(u8 *m);
extern void func_002DCA70(u8 *m);
extern const char D_00456150[], D_00456170[], D_00456190[], D_004561B0[], D_004561D0[],
    D_004561F0[], D_00456210[];   /* "O_FIN\FIN_00n.MRK" */

/* +0x10 */
void func_001F1FD0(u8 *m) {
    func_002118C0(m);
}

/* +0x68: func_002DCA70, then +0x2C */
void func_001F1DA0(u8 *m) {
    func_002DCA70(m);
    VCALL(m, 0x2C, void (*)(u8 *))(m);
}

/* +0xA4 the marker file of kind `n` (0..6) */
const char *func_001F1F30(void *m, s32 n) {
    switch (n) {
    case 0: return D_00456150;
    case 1: return D_00456170;
    case 2: return D_00456190;
    case 3: return D_004561B0;
    case 4: return D_004561D0;
    case 5: return D_004561F0;
    case 6: return D_00456210;
    }
    return NULL;
}

/* empty hooks: +0xBC, +0xC0, +0xC8, +0xCC, +0xD0 */
void func_001F1FC0(u8 *m) {
}

void func_001F1F20(u8 *m) {
}

void func_001F1F00(u8 *m) {
}

void func_001F1EF0(u8 *m) {
}

void func_001F1EE0(u8 *m) {
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

/* the motion back to the default speeds (+0x87C / +0x880) from the start, its flags +0x85C /
   +0x85D cleared */
static inline void motion_defaults(u8 *m) {
    AT(m, 0x85C, u8) = 0;
    AT(m, 0x85D, u8) = 0;
    AT(m, 0x38, s32) = AT(m, 0x87C, s32);
    AT(m, 0x3C, s32) = AT(m, 0x87C, s32);
    AT(m, 0x40, s32) = 0;
    AT(m, 0x48, s32) = AT(m, 0x880, s32);
    AT(m, 0x4C, s32) = AT(m, 0x880, s32);
    AT(m, 0x50, s32) = 0;
}

/* play animation `anim` with these flags, variant and blend at the default speeds */
void func_002DE030(u8 *m, s32 anim, u32 flags, s32 variant, f32 blend) {
    motion_defaults(m);
    func_001F7890(m, anim, flags, variant, blend);
}

/* play animation `anim` (its flags from the table +0x874, 6 bytes per entry) from the start,
 * at the default speeds +0x87C / +0x880 */
void func_002DDE20(u8 *m, s32 anim, s32 variant) {
    s32 i = func_001F4710(m, anim);
    u32 flags = i != -1 ? AT(AT(m, 0x874, u8 *), i * 6 + 4, u16) : 0;

    motion_defaults(m);
    func_001F7890(m, anim, flags & 0xFFFF, variant, 0.0f);
}

/* func_002DDE20 blended in over `blend` frames */
void func_002DDC60(u8 *m, s32 anim, s32 blend, s32 variant) {
    s32 i = func_001F4710(m, anim);
    u32 flags = i != -1 ? AT(AT(m, 0x874, u8 *), i * 6 + 4, u16) : 0;

    motion_defaults(m);
    func_001F7890(m, anim, flags & 0xFFFF, variant, (f32)blend);
}

/* func_002DDC60 with flag 8 added to the animation's, the default variant */
void func_002DDBA0(u8 *m, s32 anim, s32 blend) {
    s32 i = func_001F4710(m, anim);
    u32 flags = i != -1 ? AT(AT(m, 0x874, u8 *), i * 6 + 4, u16) : 0;

    motion_defaults(m);
    func_001F7890(m, anim, (flags | 8) & 0xFFFF, -1, (f32)blend);
}

/* the same blended over the animation's own frames (the table's first s16) */
void func_002DDD20(u8 *m, s32 anim, s32 variant) {
    s32 i = func_001F4710(m, anim);
    f32 blend = (f32)(i != -1 ? AT(AT(m, 0x874, u8 *), i * 6, s16) : 0);
    u32 flags;

    i = func_001F4710(m, anim);
    flags = i != -1 ? AT(AT(m, 0x874, u8 *), i * 6 + 4, u16) : 0;
    motion_defaults(m);
    func_001F7890(m, anim, (flags | 8) & 0xFFFF, variant, blend);
}

/* the model's +0x6A4 object gets flag 0x40 */
void func_001F6E30(u8 *m) {
    AT(AT(m, 0x6A4, u8 *), 0x18, u32) |= 0x40;
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

#ifdef HG_NATIVE
    if (motion_entry(m, anim) == NULL) {   /* not loaded (event motions aren't on PC yet) */
        return;
    }
#endif
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

extern void func_001F4C10(u8 *m, u8 **motion, u8 **skel, s32 anim, s32 part);

/* start animation `anim` on part channel `ch` (0x60 bytes at +0x6B0: its two slots of 0x1C at
 * +0x1C, current +0x54 / previous +0x58, the animation +0x18), the old one blending out over
 * `blend` frames; the slot's old motion and skeleton are freed first */
void func_001F6E50(u8 *m, s32 anim, s32 part, u32 flags, u8 *ch, f32 blend) {
    u8 *cur;

    AT(ch, 0x8, f32) = blend;
    AT(ch, 0xC, f32) = blend;
    AT(ch, 0x10, f32) = 1.0f;
    AT(ch, 0x4, s32) = AT(ch, 0x0, s32);
    AT(ch, 0x0, s32) ^= 1;
    AT(ch, 0x54, u8 *) = ch + 0x1C + AT(ch, 0x0, s32) * 0x1C;
    AT(ch, 0x58, u8 *) = ch + 0x1C + AT(ch, 0x4, s32) * 0x1C;
    AT(ch, 0x14, s32) = AT(ch, 0x18, s32);
    AT(ch, 0x18, s32) = anim;
    AT(AT(ch, 0x54, u8 *), 0xC, u32) = flags & 0xFFFF;
    if (AT(AT(ch, 0x54, u8 *), 0xC, u32) & 8) {
        AT(AT(ch, 0x54, u8 *), 0xC, u32) |= 0x10;
        if (AT(ch, 0x58, u8 *) != NULL) {
            AT(AT(ch, 0x58, u8 *), 0xC, u32) |= 0x10;
        }
    }
    cur = AT(ch, 0x54, u8 *);
    if (AT(cur, 0x10, void *) != NULL) {
        func_00179BC0(D_004562B0, AT(cur, 0x10, void *));
        AT(AT(ch, 0x54, u8 *), 0x10, void *) = NULL;
    }
    cur = AT(ch, 0x54, u8 *);
    if (AT(cur, 0x14, u8 *) != NULL) {
        func_0017CED0(D_004562A8, AT(cur, 0x14, u8 *));
        AT(AT(ch, 0x54, u8 *), 0x14, u8 *) = NULL;
    }
    cur = AT(ch, 0x54, u8 *);
    func_001F4C10(m, (u8 **)(cur + 0x10), (u8 **)(cur + 0x14), anim, part);
    AT(AT(ch, 0x54, u8 *), 0x0, f32) = 0.0f;
    AT(AT(ch, 0x54, u8 *), 0x8, f32) = 1.0f;
    AT(AT(ch, 0x54, u8 *), 0x18, s32) = 0;
}

/* what an animation's entry covers: bit 0 its +0, bit 1 the body (+4), bits 2..4 the 3 parts */
static inline u32 motion_covers(const u8 *e) {
    u32 c = AT(e, 0x0, s32) != 0;

    c |= AT(e, 0x4, s32) != 0 ? 2 : 0;
    c |= AT(e, 0x8, s32) != 0 ? 4 : 0;
    c |= AT(e, 0xC, s32) != 0 ? 8 : 0;
    c |= AT(e, 0x10, s32) != 0 ? 0x10 : 0;
    return c;
}

/* the parts' animations queued while their channel was blending (+0x504.., 0x14 each) are
 * dropped: each part just keeps the queued one as its own (+0x4E4) */
static inline void motion_drop_queued_parts(u8 *m) {
    s32 k;

    for (k = 0; k < 3; k++) {
        if (AT(m, 0x504 + k * 0x14, u8)) {
            AT(m, 0x504 + k * 0x14, u8) = 0;
            AT(m, 0x4E4 + k * 4, s32) = AT(m, 0x508 + k * 0x14, s32);
        }
    }
}

/* play animation `anim` when the model is free for it: one covering the whole body replaces the
 * body's (unless already playing), or waits (+0x4F0..) while the body still blends; one covering
 * only parts goes to each such part, waiting (+0x504..) while that part's channel blends - a part
 * under a whole-body animation (playing or queued) just remembers it (+0x4E4) */
void func_001F7460(u8 *m, s32 anim, u32 flags, s32 variant, f32 blend) {
    u32 c = motion_covers(motion_entry(m, anim));
    s32 k;

    if (c == 0x1F || (c & 2) == 2) {
        if (AT(m, 0x55C, s32) == anim && AT(m, 0x560, s32) == variant) {
            return;
        }
        if (AT(m, 0x54C, f32) != 0.0f) {
            AT(m, 0x4F0, u8) = 1;
            AT(m, 0x4F4, s32) = anim;
            AT(m, 0x4F8, s16) = flags;
            AT(m, 0x4FC, f32) = blend;
            AT(m, 0x500, s32) = variant;
        } else {
            func_001F7890(m, anim, flags, variant, blend);
        }
        if (c == 0x1F) {
            motion_drop_queued_parts(m);
        }
        return;
    }
    for (k = 0; k < 3; k++) {
        if (AT(motion_entry(m, anim), 0x8 + k * 4, s32) == 0) {
            continue;
        }
        if (motion_covers(motion_entry(m, AT(m, 0x55C, s32))) == 0x1F
            || (AT(m, 0x4F0, u8) && motion_covers(motion_entry(m, AT(m, 0x4F4, s32))) == 0x1F)) {
            AT(m, 0x4E4 + k * 4, s32) = anim;
            continue;
        }
        if (AT(m, 0x6C8 + k * 0x60, s32) == anim) {
            continue;
        }
        if (AT(m, 0x6BC + k * 0x60, f32) == 0.0f) {
            func_001F7890(m, anim, flags, variant, blend);
            continue;
        }
        AT(m, 0x504 + k * 0x14, u8) = 1;
        AT(m, 0x508 + k * 0x14, s32) = anim;
        AT(m, 0x50C + k * 0x14, s16) = flags;
        AT(m, 0x510 + k * 0x14, f32) = blend;
        AT(m, 0x514 + k * 0x14, s32) = variant;
        AT(m, 0x4E4 + k * 4, s32) = anim;
    }
}

extern void func_001F5020(u8 *m, s32 slot);
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


/* the position of the model's reference bone (+0x8B0, in the skeleton +0x810): its matrix's
 * translation row */
void func_001F1DE0(u8 *m, f32 *out) {
    u8 *mtx = (u8 *)func_0017CE80(AT(m, 0x810, void *), AT(m, 0x8B0, s32));

    sceVu0CopyVector(out, (f32 *)(mtx + 0x30));
}


/* motion: clear flag 0x40 of the current track (+0x6A4, flags +0x18) */
void func_001F6E10(u8 *m) {
    AT(AT(m, 0x6A4, u8 *), 0x18, u32) &= ~0x40;
}


/* ---- stick gestures: each recognizer arms once the stick is centred (under 0.4) and fires
   when it is then pushed fully (over 0.99) in its direction (the angle from func_0031C5C0:
   0 forward, +-pi back) ---- */

static s32 gesture_step(s32 *state, f32 len, s32 aimed, s32 gesture) {
    if (*state == 0) {
        if (len < 0x1.99999ap-2f /* 0.4 */) {
            *state += 1;
        }
        return -1;
    }
    if (*state != 1 || len <= 0x1.fae148p-1f /* 0.99 */ || !aimed) {
        return -1;
    }
    *state = 0;
    return gesture;
}

/* 0: back (more than pi - 0.6 off forward) */
s32 func_001F1AD0(u8 *g, f32 len, f32 ang) {
    if (ang <= 0.0f) {
        ang = -ang;
    }
    return gesture_step(&AT(g, 0x28, s32), len, !(ang <= 0x1.4552e8p+1f /* pi - 0.6 */), 0);
}

/* 1: forward (within 0.6) */
s32 func_001F1A10(u8 *g, f32 len, f32 ang) {
    if (ang <= 0.0f) {
        ang = -ang;
    }
    return gesture_step(&AT(g, 0x2C, s32), len, ang < 0x1.333334p-1f /* 0.6 */, 1);
}

/* 2: R3 pressed with the stick centred */
s32 func_001F19C0(u8 *g, f32 len, f32 ang) {
    return len < 0x1.99999ap-2f /* 0.4 */ && (D_0047E37C & PAD_R3) ? 2 : -1;
}

/* 3: right (within 0.6 of +pi/2) */
s32 func_001F1900(u8 *g, f32 len, f32 ang) {
    return gesture_step(&AT(g, 0x34, s32), len,
                        !(ang <= 0x1.f10c38p-1f /* pi/2 - 0.6 */) && ang < 0x1.15dca8p+1f /* pi/2 + 0.6 */, 3);
}

/* 4: left (within 0.6 of -pi/2) */
s32 func_001F1840(u8 *g, f32 len, f32 ang) {
    return gesture_step(&AT(g, 0x38, s32), len,
                        !(ang <= -0x1.15dca8p+1f /* -pi/2 - 0.6 */) && ang < -0x1.f10c38p-1f /* -pi/2 + 0.6 */, 4);
}

extern const PTMF16 D_003D5C68[5];   /* the gesture recognizers (func_001F1AD0, func_001F1A10, ..) */
extern f32 func_0031C5C0(f32 x, f32 z);   /* atan2(x, z) */

/* a stick gesture, each frame: the recognizers (5, state +0x14/+0x28 each) are fed the
 * stick's length and direction in turn until one reports a gesture (not -1); the rest are
 * reset (+0x28). No stick: all reset. Returns the gesture, -1 = none. */
s32 func_001F1B90(u8 *g, f32 *stick) {
    f32 len, ang;
    s32 r = -1;
    u32 i;

    if (stick == NULL) {
        for (i = 0; i < 5; i++) {
            AT(g, 0x28 + i * 4, s32) = 0;
            AT(g, 0x14 + i * 4, s32) = 0;
        }
        return -1;
    }
    len = __builtin_sqrtf(sceVu0InnerProduct(stick, stick));
    ang = func_0031C5C0(stick[0], stick[2]);
    for (i = 0; i < 5; i++) {
        if (r == -1) {
            r = ptmf_scall_rff(g, &D_003D5C68[i].p, len, ang);
        } else {
            AT(g, 0x28 + i * 4, s32) = 0;
        }
    }
    return r;
}


extern void func_001F36B0(void *track, f32 *out, f32 t);   /* sample a track: out[4..7] = translation */

/* the root translation of slot `slot`'s animation `k` (0, 1: the layer) at its time + dt
 * (wrapped into the animation), scaled by the slot's weight; 0 if it has none */
static void motion_root(u8 *m, s32 slot, s32 k, f32 dt, f32 *out) {
    u8 *s = m + slot * 0xA0;
    f32 tmp[8] __attribute__((aligned(16)));
    void *anim = AT(s, 0x584 + k * 4, void *);
    s32 *track = AT(s, 0x59C + k * 4, s32 *);
    f32 t;

    if ((AT(s, 0x57C, u32) & 0x10) || anim == NULL || track == NULL || *track == 0) {
        return;
    }
    t = AT(s, 0x564 + k * 4, f32) + dt;
    if (t < 0.0f) {
        do {
            t += (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32);
        } while (t < 0.0f);
    }
    if (!(t < (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32))) {
        do {
            t -= (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32);
        } while (!(t < (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32)));
    }
    func_001F36B0(track, tmp, t);
    sceVu0ScaleVector(out, tmp + 4, AT(s, 0x574 + k * 4, f32));
}

/* the motion's root movement over dt: the current slot (+0x540) and the previous one
 * (+0x544), each blended with its layer by the track's weight (+0x6A4/+0x6A8 +0x1C), then
 * cross-faded by +0x550 while a fade runs (+0x54C > 0) */
void func_001F6370(u8 *m, f32 *out, f32 dt) {
    f32 cur[4] __attribute__((aligned(16))) = {0.0f, 0.0f, 0.0f, 0.0f};
    f32 prev[4] __attribute__((aligned(16))) = {0.0f, 0.0f, 0.0f, 0.0f};
    f32 layer[4] __attribute__((aligned(16)));
    s32 *l;

    motion_root(m, AT(m, 0x540, s32), 0, dt, cur);
    motion_root(m, AT(m, 0x544, s32), 0, dt, prev);
    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = 0.0f;
    l = AT(AT(m, 0x6A4, u8 *), 0x3C, s32 *);
    if (l != NULL && *l != 0) {
        layer[0] = layer[1] = layer[2] = layer[3] = 0.0f;
        motion_root(m, AT(m, 0x540, s32), 1, dt, layer);
        sceVu0InterVector(cur, cur, layer, AT(AT(m, 0x6A4, u8 *), 0x1C, f32));
    }
    l = AT(AT(m, 0x6A8, u8 *), 0x3C, s32 *);
    if (l != NULL && *l != 0) {
        layer[0] = layer[1] = layer[2] = layer[3] = 0.0f;
        motion_root(m, AT(m, 0x544, s32), 1, dt, layer);
        sceVu0InterVector(prev, prev, layer, AT(AT(m, 0x6A8, u8 *), 0x1C, f32));
    }
    if (AT(m, 0x54C, f32) <= 0.0f) {
        out[0] = cur[0];
        out[1] = cur[1];
        out[2] = cur[2];
    } else {
        sceVu0InterVector(out, prev, cur, AT(m, 0x550, f32));
    }
}


static const union { u32 u; f32 f; } kTrkRot = {0x38C90FDB},   /* 2 pi / 65536 */
    kTrkUnit = {0x38000100},                                  /* ~1 / 32767 */
    kTrkPi = {0x40490FDB}, kTrkTwoPi = {0x40C90FDB};

/* angle n moved by a turn so that it is within pi of cur */
static f32 track_unwrap(f32 n, f32 cur) {
    f32 d = n - cur;

    if (!((d <= 0.0f ? -d : d) <= kTrkPi.f)) {
        if (d <= 0.0f) {
            n += kTrkTwoPi.f;
        } else {
            n -= kTrkTwoPi.f;
        }
    }
    return n;
}

/* sample an animation track { keys, format (+ 0x10000: constant), key count } at time t:
 * out[0..2] rotation or position, out[4..6] position (formats 2, 4, 7), out[0..7] a matrix
 * row pair (6); between keys rotations and positions are interpolated (angles the short way) */
void func_001F36B0(void *track, f32 *out, f32 t) {
    u8 *trk = track;
    u32 flags = AT(trk, 0x4, u32);
    f32 base = (flags & 0x10000) ? 0.0f : (f32)(s32)t;
    s32 k = (s32)base;
    s16 *h;
    f32 *w;
    f32 frac;
    s32 k2;

    switch (flags & 0xFFFF) {
    case 0:
        h = AT(trk, 0x0, s16 *) + k * 3;
        out[0] = kTrkRot.f * (f32)h[0];
        out[1] = kTrkRot.f * (f32)h[1];
        out[2] = kTrkRot.f * (f32)h[2];
        break;
    case 1:
        h = AT(trk, 0x0, s16 *) + k * 3;
        out[0] = 0.00390625f * (f32)h[0];
        out[1] = 0.00390625f * (f32)h[1];
        out[2] = 0.00390625f * (f32)h[2];
        break;
    case 3:
        h = AT(trk, 0x0, s16 *) + k * 4;
        out[0] = kTrkUnit.f * (f32)h[0];
        out[1] = kTrkUnit.f * (f32)h[1];
        out[2] = kTrkUnit.f * (f32)h[2];
        out[3] = kTrkUnit.f * (f32)h[3];
        break;
    case 2:
    case 4:
        h = AT(trk, 0x0, s16 *) + k * 6;
        out[0] = kTrkRot.f * (f32)h[0];
        out[1] = kTrkRot.f * (f32)h[1];
        out[2] = kTrkRot.f * (f32)h[2];
        out[4] = 0.00390625f * (f32)h[3];
        out[5] = 0.00390625f * (f32)h[4];
        out[6] = 0.00390625f * (f32)h[5];
        break;
    case 5: {
        /* the original stores x and y unconverted (the s16 bits as a float) */
        union { s32 i; f32 f; } x, y;

        h = AT(trk, 0x0, s16 *) + k * 3;
        x.i = h[0];
        y.i = h[1];
        out[0] = x.f;
        out[1] = y.f;
        out[2] = kTrkUnit.f * (f32)h[2];
        break;
    }
    case 6:
        w = AT(trk, 0x0, f32 *) + k * 8;
        out[0] = w[0];
        out[1] = w[1];
        out[2] = w[2];
        out[3] = w[3];
        out[4] = w[4];
        out[5] = w[5];
        out[6] = w[6];
        out[7] = w[7];
        break;
    case 7:
        w = AT(trk, 0x0, f32 *) + k * 6;
        out[0] = w[0];
        out[1] = w[1];
        out[2] = w[2];
        out[4] = w[3];
        out[5] = w[4];
        out[6] = w[5];
        break;
    case 8:
    case 9:
        w = AT(trk, 0x0, f32 *) + k * 3;
        out[0] = w[0];
        out[1] = w[1];
        out[2] = w[2];
        break;
    }
    if (t == base || (AT(trk, 0x4, u32) & 0x10000)) {
        return;
    }
    frac = t - base;
    k2 = (s32)base + 1;
    if (!(k2 < AT(trk, 0x8, s32))) {
        k2 = 0;
    }
    switch (AT(trk, 0x4, u32) & 0xFFFF) {
    case 0: {
        f32 n0, n1, n2;

        h = AT(trk, 0x0, s16 *) + k2 * 3;
        n0 = track_unwrap(kTrkRot.f * (f32)h[0], out[0]);
        n1 = track_unwrap(kTrkRot.f * (f32)h[1], out[1]);
        n2 = track_unwrap(kTrkRot.f * (f32)h[2], out[2]);
        out[0] = out[0] + frac * (n0 - out[0]);
        out[1] = out[1] + frac * (n1 - out[1]);
        out[2] = out[2] + frac * (n2 - out[2]);
        break;
    }
    case 1:
        h = AT(trk, 0x0, s16 *) + k2 * 3;
        out[0] = out[0] + frac * (0.00390625f * (f32)h[0] - out[0]);
        out[1] = out[1] + frac * (0.00390625f * (f32)h[1] - out[1]);
        out[2] = out[2] + frac * (0.00390625f * (f32)h[2] - out[2]);
        break;
    case 2:
    case 4: {
        f32 n0, n1, n2;

        h = AT(trk, 0x0, s16 *) + k2 * 6;
        n0 = track_unwrap(kTrkRot.f * (f32)h[0], out[0]);
        n1 = track_unwrap(kTrkRot.f * (f32)h[1], out[1]);
        n2 = track_unwrap(kTrkRot.f * (f32)h[2], out[2]);
        out[0] = out[0] + frac * (n0 - out[0]);
        out[1] = out[1] + frac * (n1 - out[1]);
        out[2] = out[2] + frac * (n2 - out[2]);
        out[4] = out[4] + frac * (0.00390625f * (f32)h[3] - out[4]);
        out[5] = out[5] + frac * (0.00390625f * (f32)h[4] - out[5]);
        out[6] = out[6] + frac * (0.00390625f * (f32)h[5] - out[6]);
        break;
    }
    case 7:
        w = AT(trk, 0x0, f32 *) + k2 * 6;
        out[0] = out[0] + frac * (w[0] - out[0]);
        out[1] = out[1] + frac * (w[1] - out[1]);
        out[2] = out[2] + frac * (w[2] - out[2]);
        out[4] = out[4] + frac * (w[3] - out[4]);
        out[5] = out[5] + frac * (w[4] - out[5]);
        out[6] = out[6] + frac * (w[5] - out[6]);
        break;
    }
}


/* the current slot's root rotation (track 0, y) at its time + dt, scaled by its weight;
 * 0 without an animation */
f32 func_001F6140(u8 *m, f32 dt) {
    u8 *s = m + AT(m, 0x540, s32) * 0xA0;
    f32 tmp[8] __attribute__((aligned(16)));
    void *anim = AT(s, 0x584, void *);
    s32 *track = AT(s, 0x59C, s32 *);
    f32 t;

    if (anim == NULL || track == NULL || *track == 0) {
        return 0.0f;
    }
    t = AT(s, 0x564, f32) + dt;
    if (t < 0.0f) {
        do {
            t += (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32);
        } while (t < 0.0f);
    }
    if (!(t < (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32))) {
        do {
            t -= (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32);
        } while (!(t < (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32)));
    }
    func_001F36B0(track, tmp, t);
    return AT(s, 0x574, f32) * tmp[1];
}


/* the event flags of layer `layer`'s motion at its current frame + `dt` frames (m +0x4D4: per
 * motion of the library +0x4C4, one byte per frame); wrapped into the motion when `loop` and
 * the track loops (+0x18 bit 1), else 0 outside it */
s32 func_001F4770(u8 *m, s32 layer, s32 dt, u32 loop) {
    u8 *ev = AT(m, 0x4D4, u8 *);
    u8 *lib = AT(m, 0x4C4, u8 *);
    s32 id, idx = -1;
    u8 *trk, *anim, *bytes;
    s32 t, len;

    if (ev == NULL) {
        return 0;
    }
    id = AT(m, 0x55C + layer * 4, s32);
    if (lib != NULL) {
        u8 *tbl = lib + AT(lib, 0xC, s32);
        u32 n = AT(tbl, 0x0, u32);
        u32 i;

        for (i = 0; i < n; i++) {
            if (AT(tbl, 0x10 + i * 8, s32) == id) {
                idx = i;
                break;
            }
        }
    }
    if (idx == -1) {
        return 0;
    }
    trk = AT(m, 0x6A4, u8 *);
    t = (s32)((f32)dt + AT(trk, layer * 4, f32));
    anim = AT(trk, 0x20 + layer * 4, u8 *);
    len = AT(AT(anim, 0x4, u8 *), 0xC, s32);
    bytes = ev + AT(ev, 0x4 + AT(ev, 0x0, s32) * 4 + idx * 4, s32);
    if (AT(trk, 0x18, u32) & (loop & 1)) {
        f32 ft = (f32)t;

        if (ft < 0.0f) {
            do {
                ft += (f32)len;
            } while (ft < 0.0f);
        }
        while (!(ft < (f32)len)) {
            ft -= (f32)len;
        }
        return bytes[(s32)ft];
    }
    if (t < 0 || len - 1 < t) {
        return 0;
    }
    return bytes[t];
}


extern void func_001F5F70(u8 *m, f32 *out, void *trkA, void *trkB, f32 ta, f32 tb, f32 w);   /* a bone's position */
extern void func_0010E610(f32 *out, const f32 *a, const f32 *b, f32 t);   /* xyz lerp, w of a */

/* `base` + dt * `speed`, wrapped into an animation of `anim`'s length (0 without one) */
static f32 motion_time(u8 *anim, f32 base, f32 dt, f32 speed) {
    f32 t;

    if (anim == NULL) {
        return 0.0f;
    }
    t = base + dt * speed;
    if (t < 0.0f) {
        do {
            t += (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32);
        } while (t < 0.0f);
    }
    while (!(t < (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32))) {
        t -= (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32);
    }
    return t;
}

/* a foot (`left` or right) at the motion's time + dt: its position (out; z scaled by
 * `zscale`, w = 1), cross-faded from the previous track (+0x6A8) by +0x550, and whether it is
 * on the ground (the current track's contact channel +0x50); 0 without contact data */
s32 func_002DD420(u8 *m, f32 *out, s32 left, f32 dt, f32 zscale) {
    u8 *cur = AT(m, 0x6A4, u8 *);
    u8 *prev;
    f32 contact[8] __attribute__((aligned(16)));
    f32 pa[4] __attribute__((aligned(16)));
    f32 pb[4] __attribute__((aligned(16)));
    f32 c0, c1, p0, p1, d;
    s32 k;

    if (AT(cur, 0x50, void *) == NULL) {
        return 0;
    }
    d = (AT(cur, 0x18, u32) & 0x10) ? 0.0f : dt;
    c0 = motion_time(AT(cur, 0x20, u8 *), AT(cur, 0x0, f32), d, AT(cur, 0x10, f32));
    c1 = motion_time(AT(cur, 0x24, u8 *), AT(cur, 0x4, f32), d, AT(cur, 0x14, f32));
    prev = AT(m, 0x6A8, u8 *);
    if (AT(prev, 0x18, u32) & 0x10) {
        dt = 0.0f;
    }
    p0 = motion_time(AT(prev, 0x20, u8 *), AT(prev, 0x0, f32), dt, AT(prev, 0x10, f32));
    p1 = motion_time(AT(prev, 0x24, u8 *), AT(prev, 0x4, f32), dt, AT(prev, 0x14, f32));
    func_001F36B0(AT(cur, 0x50, void *), contact, c0);
    k = left != 0 ? 2 : 1;
    cur = AT(m, 0x6A4, u8 *);
    func_001F5F70(m, pa, AT(cur, 0x38 + k * 8, void *), AT(cur, 0x3C + k * 8, void *), c0, c1,
                  1.0f - AT(cur, 0x1C, f32));
    prev = AT(m, 0x6A8, u8 *);
    if (AT(prev, 0x20, void *) != NULL) {
        func_001F5F70(m, pb, AT(prev, 0x38 + k * 8, void *), AT(prev, 0x3C + k * 8, void *), p0, p1,
                      1.0f - AT(prev, 0x1C, f32));
        func_0010E610(pa, pb, pa, AT(m, 0x550, f32));
    }
    out[0] = pa[0];
    out[1] = pa[1];
    out[2] = pa[2] * zscale;
    AT(out, 0xC, u32) = 0x3F800000;   /* 1.0 */
    return contact[left] > 0.0f;
}


/* a bone's channel blended between two tracks: track A at ta and B at tb, out = B * w +
 * A * (1 - w) (xyz; format 2 tracks have a second vector at out + 0x10); with only one track,
 * that one (the original takes the second vector from A even when only B exists) */
void func_001F5F70(u8 *m, f32 *out, void *trackA, void *trackB, f32 ta, f32 tb, f32 w) {
    s32 *trkA = trackA, *trkB = trackB;
    f32 a[8] __attribute__((aligned(16)));
    f32 b[8] __attribute__((aligned(16)));

    a[0] = a[1] = a[2] = 0.0f;
    a[4] = a[5] = a[6] = 0.0f;
    b[0] = b[1] = b[2] = 0.0f;
    b[4] = b[5] = b[6] = 0.0f;
    if (trkA != NULL && *trkA != 0) {
        func_001F36B0(trkA, a, ta);
    }
    if (trkB != NULL && *trkB != 0) {
        func_001F36B0(trkB, b, tb);
    }
    if (trkA != NULL && *trkA != 0 && trkB != NULL && *trkB != 0) {
        func_0010E610(out, b, a, w);
        if (AT(trkA, 0x4, u16) == 2) {
            func_0010E610(out + 4, b + 4, a + 4, w);
        }
    } else if (trkA != NULL && *trkA != 0) {
        sceVu0CopyVector(out, a);
        if (AT(trkA, 0x4, u16) == 2) {
            sceVu0CopyVector(out + 4, a + 4);
        }
    } else {
        sceVu0CopyVector(out, b);
        if (AT(trkA, 0x4, u16) == 2) {
            sceVu0CopyVector(out + 4, a + 4);
        }
    }
}


extern u32 func_002DD860(void *motion, s32 left, f32 t);   /* foot planted (u8) */
extern void func_002DC710(u8 *m, f32 *p, u8 *a);          /* drop a point onto the floor */

/* the model's height above the character's floor (+0x804, smoothed 3:1 with last frame's
 * +0x8C0 unless +0x990 asks for a jump), each frame from the character's matrix and position
 * (kept at +0x7D0 / +0x800): the lower of the two feet put on the floor (feet in the air keep
 * the character's height; flag 0x100: planted feet only), or on slopes (track flag 4) two
 * points `back` / `front` along the step; nothing while both are 0 */
void func_00210E00(u8 *m, u8 *a, f32 back, f32 front) {
    f32 l[4] __attribute__((aligned(16)));
    f32 r[4] __attribute__((aligned(16)));

    sceVu0CopyMatrix((f32 (*)[4])(m + 0x7D0), (f32 (*)[4])(a + 0x60));
    sceVu0CopyVector((f32 *)(m + 0x800), (f32 *)(a + 0x10));
    AT(m, 0x80C, f32) = 1.0f;
    if (back == 0.0f && front == 0.0f) {
        AT(m, 0x8C0, f32) = AT(m, 0x804, f32);
        return;
    }
    if (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 4) {
        f32 n[4] __attribute__((aligned(16)));
        f32 v[4] __attribute__((aligned(16)));
        f32 ny, k;

        VCALL(D_0044E570, 0x2C, void (*)(NavMesh *, u32, f32 *))(D_0044E570, AT(a, 0x34, u32), n);
        ny = n[1];
        func_001F6370(m, v, 0.0f);
        k = v[2] * (ny * ny);
        l[0] = 0.0f;
        l[1] = 0.0f;
        l[2] = back * k;
        l[3] = 1.0f;
        r[0] = 0.0f;
        r[1] = 0.0f;
        r[2] = front * k;
        r[3] = 1.0f;
        sceVu0ApplyMatrix(l, (f32 (*)[4])(m + 0x7D0), l);
        sceVu0ApplyMatrix(r, (f32 (*)[4])(m + 0x7D0), r);
        func_002DC710(m, l, a);
        func_002DC710(m, r, a);
    } else {
        u32 dl = (u8)func_002DD420(m, l, 1, 0.0f, 1.0f);
        u32 dr = (u8)func_002DD420(m, r, 0, 0.0f, 1.0f);
        s32 useL = 1, useR = 1;

        sceVu0ApplyMatrix(l, (f32 (*)[4])(m + 0x7D0), l);
        sceVu0ApplyMatrix(r, (f32 (*)[4])(m + 0x7D0), r);
        if (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 0x100) {
            if (dl == 1 && AT(AT(m, 0x6A8, u8 *), 0x20, void *) != NULL) {
                dl = (u8)func_002DD860(m, 1, 0.0f);
            }
            if (dl == 0) {
                useL = 0;
            }
            if (dr == 1 && AT(AT(m, 0x6A8, u8 *), 0x20, void *) != NULL) {
                dr = (u8)func_002DD860(m, 0, 0.0f);
            }
            if (dr == 0) {
                useR = 0;
            }
        }
        if (useL == 1) {
            func_002DC710(m, l, a);
        } else {
            l[1] = AT(a, 0x14, f32);
        }
        if (useR == 1) {
            func_002DC710(m, r, a);
        } else {
            r[1] = AT(a, 0x14, f32);
        }
    }
    AT(m, 0x804, f32) = l[1] < r[1] ? l[1] : r[1];
    if (AT(m, 0x990, u8) == 0) {
        AT(m, 0x804, f32) = 0.25f * AT(m, 0x804, f32) + 0.75f * AT(m, 0x8C0, f32);
    } else {
        AT(m, 0x990, u8) = 0;
    }
    AT(m, 0x8C0, f32) = AT(m, 0x804, f32);
}


/* put point p on the floor: walk the nav mesh from the character's triangle towards it
 * (vtable +0x24: the edge crossed, 3 inside, 4 lost); in a triangle the mesh gives its height
 * (+0x14); at a wall (no neighbour, or a marked one (+0x18 bit 0x80) blocked by the
 * character's mask +0xC0) its height continues along the line to where it was crossed */
void func_002DC710(u8 *m, f32 *p, u8 *a) {
    NavMesh *nm;
    NavTri *t;
    u32 tri = AT(a, 0x34, u32);
    f32 hit[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 len;

    if (tri == (u32)-1) {
        return;
    }
    nm = D_0044E570;
    t = NavMesh_Tri(nm, tri);
    for (;;) {
        s32 e = VCALL(nm, 0x24, s32 (*)(NavMesh *, u32, f32 *, f32 *, f32 *))(nm, tri, hit, (f32 *)(a + 0x10), p);
        u8 mark;

        if (e == 3) {
            VCALL(nm, 0x14, void (*)(NavMesh *, u32, f32 *))(nm, tri, p);
            return;
        }
        if (e == 4) {
            VCALL(nm, 0x14, void (*)(NavMesh *, u32, f32 *))(nm, AT(a, 0x34, u32), p);
            return;
        }
        tri = t->adj[e];
        if (tri == (u32)-1) {
            break;
        }
        t = NavMesh_Tri(nm, tri);
        mark = (AT(nm, 0x18, u8 *) != NULL && tri < nm->numTris && nm->tris != NULL) ? AT(nm, 0x18, u8 *)[tri] : 0;
        if ((mark & 0x80) && (t->flags & AT(a, 0xC0, u32))) {
            break;
        }
    }
    sceVu0SubVector(d, p, (f32 *)(a + 0x10));
    len = __builtin_sqrtf(sceVu0InnerProduct(d, d));
    sceVu0SubVector(d, hit, (f32 *)(a + 0x10));
    sceVu0Normalize(d, d);
    p[1] = AT(a, 0x14, f32) + d[1] * len;
}


extern s32 D_004157A0[];   /* hand poses: per pose 5 key frames { s32, s32, f32 } */

/* the hands, when the motion moves on: the motion's hand pose (+0x874 table, byte 2: left pose
 * << 4 | right pose) is blended in over the next 5 frames as their events (bit 4 left, 8
 * right) say, forwards or backwards by the hand's state (+0x85C / +0x85D), into +0x38 / +0x48
 * (poses 0..6 as they are, others fall back to the defaults +0x87C / +0x880); a restarted
 * motion resets them */
void func_002DCB40(u8 *m) {
    u8 ev[5];
    s32 hand, i, k, idx, bit;
    u32 pose, on;
    u8 *t = AT(m, 0x6A4, u8 *);

    if (AT(t, 0x0, f32) == AT(t, 0x8, f32)) {
        return;
    }
    if (AT(t, 0x0, f32) == 0.0f) {
        AT(m, 0x85C, u8) = 0;
        AT(m, 0x85D, u8) = 0;
        AT(m, 0x38, s32) = AT(m, 0x87C, s32);
        AT(m, 0x3C, s32) = AT(m, 0x87C, s32);
        AT(m, 0x40, f32) = 0.0f;
        AT(m, 0x48, s32) = AT(m, 0x880, s32);
        AT(m, 0x4C, s32) = AT(m, 0x880, s32);
        AT(m, 0x50, f32) = 0.0f;
    }
    if (AT(m, 0x874, u8 *) == NULL) {
        return;
    }
    k = func_001F4710(m, AT(m, 0x55C, s32));
    if (k == -1) {
        return;
    }
    pose = AT(m, 0x874, u8 *)[k * 6 + 2];
    if (pose == 0) {
        return;
    }
    for (i = 0; i < 5; i++) {
        ev[i] = func_001F4770(m, 0, i, -2);
    }
    for (hand = 0; hand < 2; hand++) {
        s32 *tbl;
        s32 *dst = &AT(m, 0x38 + hand * 16, s32);

        if (hand == 0) {
            on = AT(m, 0x85C, u8);
            bit = 4;
            idx = (pose >> 4) & 0xF;
        } else {
            on = AT(m, 0x85D, u8);
            bit = 8;
            idx = pose & 0xF;
        }
        tbl = D_004157A0 + idx * 15;
        for (i = 0; i < 5; i++) {
            s32 *e = on ? tbl + i * 3 : tbl + (4 - i) * 3;

            if (!(ev[i] & bit)) {
                continue;
            }
            dst[0] = (idx < 7 || e[0] != 0) ? e[0] : AT(m, 0x87C + hand * 4, s32);
            dst[1] = (idx < 7 || e[1] != 0) ? e[1] : AT(m, 0x87C + hand * 4, s32);
            AT(dst, 0x8, f32) = AT(e, 0x8, f32);
            if (i == 0) {
                on = (on ^ 1) != 0;
            }
        }
        AT(m, 0x85C + hand, u8) = on;
    }
}


extern f32 D_00415B60[14];   /* the eyelids through a blink */
extern VObject *D_0044E550;  /* random numbers: +0x1C -> 0..1 */

/* the eyes, each frame: a blink plays the eyelid curve over 14 frames (+0x870 counts, both
 * lids +0x74 / +0x78); from frame 30 a new blink starts at random, surely by frame 150 */
void func_002DC960(u8 *m) {
    if (AT(m, 0x870, s32) < 14) {
        AT(m, 0x78, f32) = D_00415B60[AT(m, 0x870, s32)];
        AT(m, 0x74, f32) = D_00415B60[AT(m, 0x870, s32)];
    }
    if (AT(m, 0x870, s32) >= 30) {
        f32 r = VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550);

        if ((s32)((f32)(150 - AT(m, 0x870, s32)) * r) == 0) {
            AT(m, 0x870, s32) = 0;
        }
    }
    AT(m, 0x870, s32)++;
}


extern void func_001F4F40(u8 *m);
extern void func_001F4D70(u8 *m);
extern void func_001F5850(u8 *m);
extern void *func_001F56F0(u8 *m, void *a, void *b, f32 t);   /* the blend of two poses */
extern void func_001F5D70(u8 *m, void *skel);
extern void func_001F5130(u8 *m);

/* the motion player, each frame: start the queued motion (+0x4F0) and the queued layer
 * motions (+0x504, 3 x 0x14) once their fades are over (a layer waits a frame when the main
 * motion just started), free the faded-out layers' buffers, advance the tracks, then blend
 * the pose: each track's two animations, the current with the previous one (+0x550), the three
 * layers; the skeleton is built from it (+0x810), then vtable +0x18 and func_001F5130 */
void func_001F6AF0(u8 *m) {
    void *chains, *skels, *a, *b;
    s32 started = 0;
    s32 i;

    AT(AT(m, 0x6A4, u8 *), 0x18, u32) &= ~0x80;
    AT(AT(m, 0x704, u8 *), 0xC, u32) &= ~0x80;
    AT(AT(m, 0x764, u8 *), 0xC, u32) &= ~0x80;
    AT(AT(m, 0x7C4, u8 *), 0xC, u32) &= ~0x80;
    if (AT(m, 0x4F0, u8) != 0 && AT(m, 0x54C, f32) <= 0.0f) {
        func_001F7890(m, AT(m, 0x4F4, s32), AT(m, 0x4F8, u16), AT(m, 0x500, s32), AT(m, 0x4FC, f32));
        AT(m, 0x4F0, u8) = 0;
        started = 1;
        AT(AT(m, 0x6A4, u8 *), 0x18, u32) |= 0x80;
    }
    for (i = 0; i < 3; i++) {
        u8 *q = m + i * 0x14;

        if (AT(q, 0x504, u8) == 0 || !(AT(m, 0x6BC + i * 0x60, f32) <= 0.0f)) {
            continue;
        }
        if (!started) {
            func_001F7890(m, AT(q, 0x508, s32), AT(q, 0x50C, u16), AT(q, 0x514, s32), AT(q, 0x510, f32));
            AT(q, 0x504, u8) = 0;
            AT(AT(m, 0x704 + i * 0x60, u8 *), 0xC, u32) |= 0x80;
        } else {
            AT(q, 0x504, u8) = 0;
        }
    }
    chains = D_004562B0;
    skels = D_004562A8;
    for (i = 0; i < 3; i++) {
        u8 *l = m + i * 0x60;
        u8 *s;

        if (!(AT(l, 0x6BC, f32) <= 0.0f)) {
            continue;
        }
        s = l + AT(l, 0x6B4, s32) * 0x1C;
        if (AT(s, 0x6DC, void *) != NULL) {
            func_00179BC0(chains, AT(s, 0x6DC, void *));
            AT(s, 0x6DC, void *) = NULL;
        }
        if (AT(s, 0x6E0, void *) != NULL) {
            func_0017CED0(skels, AT(s, 0x6E0, u8 *));
            AT(s, 0x6E0, void *) = NULL;
        }
    }
    if (AT(m, 0x54C, f32) <= 0.0f) {
        func_001F5020(m, AT(m, 0x544, s32));
        if (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 8) {
            AT(AT(m, 0x6A4, u8 *), 0x18, u32) &= ~0x10;
        }
    }
    func_001F4F40(m);
    func_001F4D70(m);
    func_001F5850(m);
    a = func_001F56F0(m, AT(AT(m, 0x6A4, u8 *), 0x28, void *), AT(AT(m, 0x6A4, u8 *), 0x2C, void *),
                      1.0f - AT(AT(m, 0x6A4, u8 *), 0x1C, f32));
    b = func_001F56F0(m, AT(AT(m, 0x6A8, u8 *), 0x28, void *), AT(AT(m, 0x6A8, u8 *), 0x2C, void *),
                      1.0f - AT(AT(m, 0x6A8, u8 *), 0x1C, f32));
    AT(m, 0x6AC, void *) = func_001F56F0(m, a, b, AT(m, 0x550, f32));
    for (i = 0; i < 3; i++) {
        AT(m, 0x70C + i * 0x60, void *) = func_001F56F0(m, AT(AT(m, 0x704 + i * 0x60, u8 *), 0x14, void *),
                                                        AT(AT(m, 0x708 + i * 0x60, u8 *), 0x14, void *),
                                                        AT(m, 0x6C0 + i * 0x60, f32));
    }
    func_001F5D70(m, AT(m, 0x810, void *));
    VCALL(m, 0x18, void (*)(u8 *))(m);
    func_001F5130(m);
}


/* the current track's extra channels at its time: four (+0x68, 8 bytes apart) into +0x58..,
 * two (+0x88) into the hand poses +0x38 / +0x48 */
void func_001F4F40(u8 *m) {
    s32 i;

    for (i = 0; i < 4; i++) {
        s32 *trk = AT(AT(m, 0x6A4, u8 *), 0x68 + i * 8, s32 *);

        if (trk != NULL && *trk != 0) {
            func_001F36B0(trk, (f32 *)(m + 0x58 + i * 4), AT(AT(m, 0x6A4, u8 *), 0x0, f32));
        }
    }
    for (i = 0; i < 2; i++) {
        s32 *trk = AT(AT(m, 0x6A4, u8 *), 0x88 + i * 8, s32 *);

        if (trk != NULL && *trk != 0) {
            func_001F36B0(trk, (f32 *)(m + 0x38 + i * 16), AT(AT(m, 0x6A4, u8 *), 0x0, f32));
        }
    }
}


/* the length of an animation, in frames */
static f32 anim_frames(u8 *anim) {
    return (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32);
}

/* keep blended animations in step, each frame: in each of the two slots (0xA0 apart) the
 * second animation (+0x588) follows the first's phase and their speeds (+0x574 / +0x578)
 * meet by the blend +0x580; while the motion fades (+0x54C) between two step-synced tracks
 * (flag 2), the tracks' speeds (+0x10) meet the same way by +0x550 instead */
void func_001F4D70(u8 *m) {
    s32 i;

    if (AT(m, 0x54C, f32) != 0.0f) {
        u8 *prev = AT(m, 0x6A8, u8 *);
        u8 *cur = AT(m, 0x6A4, u8 *);

        if (AT(cur, 0x18, u32) & AT(prev, 0x18, u32) & 2) {
            f32 t = AT(m, 0x550, f32);
            f32 u = 1.0f - t;
            f32 lp = anim_frames(AT(prev, 0x20, u8 *));
            f32 lc = anim_frames(AT(cur, 0x20, u8 *));

            AT(cur, 0x10, f32) = u + (lc / lp) * t;
            AT(AT(m, 0x6A8, u8 *), 0x10, f32) = t + (lp / lc) * u;
            for (i = 0; i < 2; i++, m += 0xA0) {
                if (AT(m, 0x588, u8 *) == NULL) {
                    AT(m, 0x574, f32) = 1.0f;
                    continue;
                }
                AT(m, 0x568, f32) = anim_frames(AT(m, 0x588, u8 *)) * (AT(m, 0x564, f32) / anim_frames(AT(m, 0x584, u8 *)));
                AT(m, 0x578, f32) = AT(m, 0x574, f32) * anim_frames(AT(m, 0x588, u8 *)) / anim_frames(AT(m, 0x584, u8 *));
            }
            return;
        }
    }
    for (i = 0; i < 2; i++, m += 0xA0) {
        f32 b, l0, l1;

        if (AT(m, 0x588, u8 *) == NULL) {
            AT(m, 0x574, f32) = 1.0f;
            continue;
        }
        b = AT(m, 0x580, f32);
        l1 = anim_frames(AT(m, 0x588, u8 *));
        l0 = anim_frames(AT(m, 0x584, u8 *));
        AT(m, 0x574, f32) = b + (l0 / l1) * (1.0f - b);
        AT(m, 0x578, f32) = (1.0f - b) + (l1 / l0) * b;
        AT(m, 0x568, f32) = anim_frames(AT(m, 0x588, u8 *)) * (AT(m, 0x564, f32) / anim_frames(AT(m, 0x584, u8 *)));
    }
}

extern void func_001F5930(u8 *m, void *chain, void *anim, f32 t, f32 w);   /* pose a chain from an animation */

/* apply the animations to their bone chains: the layers' (3 x 2 at +0x6CC, 0x1C apart) and
 * the two slots' two animations (+0x584) */
void func_001F5850(u8 *m) {
    s32 i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            u8 *s = m + i * 0x60 + j * 0x1C + 0x6CC;

            if (AT(s, 0x10, void *) != NULL) {
                func_001F5930(m, AT(s, 0x14, void *), AT(s, 0x10, void *), AT(s, 0x0, f32), AT(s, 0x18, f32));
            }
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            u8 *s = m + i * 0xA0 + 0x564 + j * 4;

            if (AT(s, 0x20, void *) != NULL) {
                func_001F5930(m, AT(s, 0x28, void *), AT(s, 0x20, void *), AT(s, 0x0, f32), AT(s, 0x98, f32));
            }
        }
    }
}

extern void func_0025C440(void *tmp, u8 *out, u8 *a, u8 *b, f32 t);   /* blend two bones */

/* blend two bone lists { +0x4 first bone (next +0x48, id +0x40), +0x8 count }: the longer
 * one (by t, or the other by 1 - t) takes in each of its bones the other's bone with the same
 * id; returns the blended list (one missing: the other) */
void *func_001F56F0(u8 *m, void *listA, void *listB, f32 t) {
    u8 *a = listA, *b = listB;
    f32 tmp[4] __attribute__((aligned(16)));
    u8 *big, *small, *n;
    s32 i;

    tmp[3] = tmp[2] = tmp[1] = tmp[0] = 0.0f;
    if (a == NULL && b == NULL) {
        return NULL;
    }
    if (b == NULL) {
        return a;
    }
    if (a == NULL) {
        return b;
    }
    if (AT(a, 0x8, s32) < AT(b, 0x8, s32)) {
        big = b;
        small = a;
        t = 1.0f - t;
    } else {
        big = a;
        small = b;
    }
    n = AT(big, 0x4, u8 *);
    for (i = 0; i < AT(big, 0x8, s32); i++, n = AT(n, 0x48, u8 *)) {
        u8 *o = AT(small, 0x4, u8 *);
        s32 k;

        for (k = 0; k < AT(small, 0x8, s32); k++, o = AT(o, 0x48, u8 *)) {
            if (AT(n, 0x40, s32) == AT(o, 0x40, s32)) {
                func_0025C440(tmp, n, n, o, t);
                break;
            }
        }
    }
    return big;
}


extern void func_002E2E00(void *mat, f32 *rot, const f32 *trans);   /* bone matrix */
extern void func_0025C630(f32 *q, f32 *axis, f32 (*m)[4]);   /* rotation matrix to axis + angle */
extern void func_0025C6F0(f32 *q, const f32 *axis, f32 angle);   /* quaternion of a rotation */
extern void func_0025C770(const f32 *q, f32 (*m)[4]);           /* its matrix */

/* sample one track at `t` into the motion's rotation (+0x820) and translation (+0x830): kinds
 * 0 (rotation only: the bind translation), 2 / 7 (both), others (translation only: the bind
 * rotation) */
static void pose_sample(u8 *m, u8 *bones, u8 *trk, f32 t) {
    switch (AT(trk, 0x8, u16)) {
    case 7:
    case 2:
        func_001F36B0(trk + 4, (f32 *)(m + 0x820), t);
        break;
    case 0:
        func_001F36B0(trk + 4, (f32 *)(m + 0x820), t);
        sceVu0CopyVector((f32 *)(m + 0x830), (f32 *)(bones + AT(trk, 0x0, s32) * 0x70 + 0x20));
        break;
    default:
        sceVu0CopyVector((f32 *)(m + 0x820), (f32 *)(bones + AT(trk, 0x0, s32) * 0x70 + 0x10));
        func_001F36B0(trk + 4, (f32 *)(m + 0x830), t);
        break;
    }
}

/* pose a bone chain { +0x4 first bone (next +0x48, id +0x40) } from an animation { +0x4 first
 * track (bone +0x0, kind +0x8, next +0x10), +0x8 count } at frame `t`. With `w` > 0 each bone
 * not in the motion's fixed list (+0x844, +0x840 entries) is eased: turned from its pose a
 * frame earlier towards the new one by `w` (shortest way), its position mixed the same */
void func_001F5930(u8 *m, void *chain, void *anim, f32 t, f32 w) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB}, kNegPi = {0xC0490FDB};
    f32 axis[4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));
    f32 p0[4] __attribute__((aligned(16)));
    f32 p1[4] __attribute__((aligned(16)));
    f32 cur[4][4] __attribute__((aligned(16)));
    f32 r[4][4] __attribute__((aligned(16)));
    f32 prev[4][4] __attribute__((aligned(16)));
    u8 *bones = AT(m, 0x4C0, u8 *) + 0x10;
    u8 *b = AT(chain, 0x4, u8 *);
    u8 *trk = AT(anim, 0x4, u8 *);
    s32 i, k, ease;

    for (i = 0; i < AT(anim, 0x8, s32); i++) {
        if (AT(trk, 0x0, s32) >= 0) {
            pose_sample(m, bones, trk, t);
            func_002E2E00(b, (f32 *)(m + 0x820), (f32 *)(m + 0x830));
            ease = 1;
            for (k = 0; k < AT(m, 0x840, s16); k++) {
                if (AT(m, 0x844, s8 *)[k] == AT(b, 0x40, s32)) {
                    ease = 0;
                }
            }
            if (ease && !(w <= 0.0f)) {
                pose_sample(m, bones, trk, t - 1.0f);
                func_002E2E00(prev, (f32 *)(m + 0x820), (f32 *)(m + 0x830));
                q[3] = 0.0f;
                q[0] = 0.0f;
                q[2] = 0.0f;
                q[1] = 0.0f;
                sceVu0CopyMatrix(r, prev);
                sceVu0CopyMatrix(cur, (f32 (*)[4])b);
                sceVu0CopyVector(p0, r[3]);
                sceVu0CopyVector(p1, cur[3]);
                r[3][1] = 0.0f;
                r[3][0] = 0.0f;
                r[3][2] = 0.0f;
                cur[3][0] = 0.0f;
                cur[3][1] = 0.0f;
                cur[3][2] = 0.0f;
                sceVu0TransposeMatrix(r, r);
                sceVu0MulMatrix(r, cur, r);
                func_0025C630(q, axis, r);
                axis[3] = axis[3] * w;
                if (!(axis[3] <= kPi.f)) {
                    do {
                        axis[3] = axis[3] - kTwoPi.f;
                    } while (!(axis[3] <= kPi.f));
                }
                if (axis[3] < kNegPi.f) {
                    do {
                        axis[3] = axis[3] + kTwoPi.f;
                    } while (axis[3] < kNegPi.f);
                }
                func_0025C6F0(q, axis, axis[3]);
                func_0025C770(q, r);
                sceVu0InterVector(p0, p1, p0, w);
                sceVu0MulMatrix((f32 (*)[4])b, r, (f32 (*)[4])b);
                sceVu0CopyVector((f32 *)(b + 0x30), p0);
            }
        }
        trk = AT(trk, 0x10, u8 *);
        b = AT(b, 0x48, u8 *);
    }
}


/* wrap an angle into -pi..pi (in place) */
static void wrap_pi(f32 *a) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB}, kNegPi = {0xC0490FDB};
    f32 v = *a;

    while (!(v <= kPi.f)) {
        v -= kTwoPi.f;
    }
    while (v < kNegPi.f) {
        v += kTwoPi.f;
    }
    *a = v;
}

/* a bone matrix from Euler angles `rot` (X, then Y, then Z; wrapped into -pi..pi in place) and
 * the position `trans` */
void func_002E2E00(void *mat, f32 *rot, const f32 *trans) {
    f32 (*m)[4] = mat;

    wrap_pi(&rot[0]);
    wrap_pi(&rot[1]);
    wrap_pi(&rot[2]);
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixX(m, m, rot[0]);
    sceVu0RotMatrixY(m, m, rot[1]);
    sceVu0RotMatrixZ(m, m, rot[2]);
    sceVu0TransMatrix(m, m, (f32 *)trans);
}


/* the bone with id `id` in a bone list { +0x4 first (next +0x48, id +0x40), +0x8 count } */
static u8 *bone_find(u8 *list, s32 id) {
    u8 *b;
    s32 i;

    if (list == NULL) {
        return NULL;
    }
    b = AT(list, 0x4, u8 *);
    for (i = 0; i < AT(list, 0x8, s32); i++) {
        if (id == AT(b, 0x40, s32)) {
            return b;
        }
        b = AT(b, 0x48, u8 *);
    }
    return NULL;
}

/* the skeleton's world pose: each bone of `skel` (in parent-first order; parent matrix +0x44) is
 * its parent's matrix (the first: the model's, +0x7D0) times its local pose - from the blended
 * animation (+0x6AC), else the first layer that animates it (+0x70C, 3 x 0x60), else the bind
 * pose (the skeleton's record +0x10 / +0x20) - and, unless +0x4D8, reported to +0x14 */
void func_001F5D70(u8 *m, void *skel) {
    f32 bind[4][4] __attribute__((aligned(16)));
    u8 *rec = AT(m, 0x4C0, u8 *) + 0x10;
    u8 *b = AT(skel, 0x4, u8 *);
    u8 *local;
    s32 i, k;

    for (i = 0; i < AT(skel, 0x8, s32); i++) {
        local = bone_find(AT(m, 0x6AC, u8 *), AT(b, 0x40, s32));
        for (k = 0; local == NULL && k < 3; k++) {
            local = bone_find(AT(m, 0x70C + k * 0x60, u8 *), AT(b, 0x40, s32));
        }
        if (local != NULL) {
            if (i == 0) {
                sceVu0MulMatrix((f32 (*)[4])b, (f32 (*)[4])(m + 0x7D0), (f32 (*)[4])local);
            } else {
                sceVu0MulMatrix((f32 (*)[4])b, (f32 (*)[4])AT(b, 0x44, u8 *), (f32 (*)[4])local);
            }
        } else {
            func_002E2E00(bind, (f32 *)(rec + 0x10), (f32 *)(rec + 0x20));
            if (i == 0) {
                sceVu0MulMatrix((f32 (*)[4])b, (f32 (*)[4])(m + 0x7D0), bind);
            } else {
                sceVu0MulMatrix((f32 (*)[4])b, (f32 (*)[4])AT(b, 0x44, u8 *), bind);
            }
        }
        if (AT(m, 0x4D8, u8) == 0) {
            VCALL(m, 0x14, void (*)(u8 *, s32, u8 *, u8 *))(m, i, b, local);
        }
        b = AT(b, 0x48, u8 *);
        rec += 0x70;
    }
}


/* +0x14 per posed bone (the look-at): bone +0x8B0 is turned by -+0x854 about X and half of
 * +0x858 about Y in its own frame, bone +0x8B4 by half of +0x858 about Y about its own
 * position */
void func_002117C0(u8 *m, s32 i, f32 (*b)[4], void *local) {
    f32 r[4][4] __attribute__((aligned(16)));

    if (i == AT(m, 0x8B0, s32)) {
        sceVu0UnitMatrix(r);
        sceVu0RotMatrixX(r, r, -AT(m, 0x854, f32));
        sceVu0RotMatrixY(r, r, 0.5f * AT(m, 0x858, f32));
        sceVu0MulMatrix(b, b, r);
    }
    if (i == AT(m, 0x8B4, s32)) {
        sceVu0UnitMatrix(r);
        sceVu0RotMatrixY(r, r, 0.5f * AT(m, 0x858, f32));
        sceVu0CopyVector(r[3], b[3]);
        b[3][2] = 0.0f;
        b[3][1] = 0.0f;
        b[3][0] = 0.0f;
        sceVu0MulMatrix(b, r, b);
    }
}


/* (does nothing) */
void func_001F1ED0(void) {
}


/* frames in an animation */
static s32 anim_len(void *anim) {
    return AT(AT(anim, 0x4, u8 *), 0xC, s32);
}

/* advance a play time { +0x0 time, prev (`prev`), speed (`speed`) } of animation `anim` by
 * its speed: looping (`flags` bit 0) wraps it into 0..frames (`wrapped` when it went past the
 * end), else it is clamped to 0..frames - 1 (`wrapped` at the end), the overshoot carried to
 * `carry` with `carryFlags` bit 9 while the motion fades (`fading`), else `carry` cleared */
static s32 anim_step(f32 *t, f32 *prev, f32 speed, u32 flags, void *anim, f32 *carry, u32 carryFlags, f32 fading) {
    s32 len = anim_len(anim), wrapped = 0;
    f32 v, v0, last;

    *prev = *t;
    *t = *t + speed;
    if (flags & 1) {
        v = *t;
        while (v < 0.0f) {
            v = v + (f32)len;
        }
        while (!(v < (f32)len)) {
            v = v - (f32)len;
            wrapped = 1;
        }
        *t = v;
        *carry = 0.0f;
        return wrapped;
    }
    v0 = *t;
    v = v0 < 0.0f ? 0.0f : v0;
    last = (f32)len - 1.0f;
    if (!(v <= last)) {
        v = last;
        wrapped = 1;
    }
    *t = v;
    if ((carryFlags & 0x200) && wrapped && fading != 0.0f) {
        *carry = *carry + (v0 - *t);
    } else {
        *carry = 0.0f;
    }
    return wrapped;
}

/* a fade's weight: smoothstep of the frames left / its length */
static f32 fade_weight(f32 left, f32 len) {
    f32 x, xx;

    if (len == 0.0f) {
        return 0.0f;
    }
    x = left / len;
    xx = x * x;
    return 3.0f * xx * (1.0f - x) + xx * x;
}

/* advance the motion a frame (unless the current slot's flags +0x18 have 0x40): both
 * animations of the current slot (+0x6A4) and, while fading (+0x54C), the previous one (+0x6A8)
 * - each pair { time, -, prev, -, speed, ... anim +0x20, carry +0x98 }; the first animation sets
 * the slot's flags 0x20 (wrapped) and 0x400 (at its end) - then the fade (+0x54C counts down,
 * weight +0x550), and the 3 layers' two animations (+0x6CC, 0x1C each) and fades (+0x6B8) */
void func_001F5130(u8 *m) {
    u8 *slot = NULL, *a;
    s32 k, j, i, w;

    if (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 0x40) {
        return;
    }
    for (k = 0; k < 2; k++) {
        if (k != 0 && AT(m, 0x54C, f32) == 0.0f) {
            break;
        }
        slot = k == 0 ? AT(m, 0x6A4, u8 *) : AT(m, 0x6A8, u8 *);
        for (j = 0; j < 2; j++) {
            a = slot + j * 4;
            if (AT(a, 0x20, void *) == NULL || (AT(slot, 0x18, u32) & 0x10)) {
                continue;
            }
            w = anim_step(&AT(a, 0x0, f32), &AT(a, 0x8, f32), AT(a, 0x10, f32), AT(slot, 0x18, u32), AT(a, 0x20, void *),
                          &AT(a, 0x98, f32), AT(slot, 0x18, u32), AT(m, 0x54C, f32));
            if (j == 0) {
                if (w) {
                    AT(slot, 0x18, u32) |= 0x20;
                } else {
                    AT(slot, 0x18, u32) &= ~0x20;
                }
                if (!(AT(a, 0x0, f32) < (f32)(anim_len(AT(a, 0x20, void *)) - 1))) {
                    AT(slot, 0x18, u32) |= 0x400;
                } else {
                    AT(slot, 0x18, u32) &= ~0x400;
                }
            }
        }
    }
    if (!(AT(m, 0x54C, f32) <= 0.0f)) {
        AT(m, 0x54C, f32) = AT(m, 0x54C, f32) - 1.0f;
    }
    AT(m, 0x550, f32) = fade_weight(AT(m, 0x54C, f32), AT(m, 0x548, f32));
    for (i = 0; i < 3; i++) {
        u8 *l = m + i * 0x60;

        for (j = 0; j < 2; j++) {
            a = l + j * 0x1C + 0x6CC;
            if (AT(a, 0x10, void *) == NULL) {
                continue;
            }
            /* (the carry tests the motion slot's flags, as the original) */
            if (anim_step(&AT(a, 0x0, f32), &AT(a, 0x4, f32), AT(a, 0x8, f32), AT(a, 0xC, u32), AT(a, 0x10, void *),
                          &AT(a, 0x18, f32), AT(slot, 0x18, u32), AT(m, 0x54C, f32))) {
                AT(a, 0xC, u32) |= 0x20;
            } else {
                AT(a, 0xC, u32) &= ~0x20;
            }
        }
        if (!(AT(l, 0x6BC, f32) <= 0.0f)) {
            AT(l, 0x6BC, f32) = AT(l, 0x6BC, f32) - 1.0f;
        }
        AT(l, 0x6C0, f32) = fade_weight(AT(l, 0x6BC, f32), AT(l, 0x6B8, f32));
    }
}


extern void func_00211530(u8 *m, f32 *right, f32 *left, f32 height);   /* bend the legs to the feet */

/* a foot bone's position (row 3 of its matrix) into `out`; with the slope weight `k` < 1 its
 * height above the floor is eased: out = position (+0x800) + side (+0x7F0) x k x (its offset
 * along it) + forward (+0x7D0) x (its offset along that) */
static void foot_point(u8 *m, s32 bone, f32 *out, f32 k) {
    f32 v[4] __attribute__((aligned(16)));
    f32 a, b, c, d;

    sceVu0CopyVector(out, func_0017CE80(AT(m, 0x810, void *), bone) + 12);
    if (k < 1.0f) {
        a = sceVu0InnerProduct((f32 *)(m + 0x7D0), (f32 *)(m + 0x800));
        b = sceVu0InnerProduct((f32 *)(m + 0x7F0), (f32 *)(m + 0x800));
        c = sceVu0InnerProduct((f32 *)(m + 0x7D0), out);
        d = sceVu0InnerProduct((f32 *)(m + 0x7F0), out);
        sceVu0CopyVector(out, (f32 *)(m + 0x800));
        sceVu0ScaleVector(v, (f32 *)(m + 0x7F0), k * (d - b));
        sceVu0AddVector(out, out, v);
        sceVu0ScaleVector(v, (f32 *)(m + 0x7D0), c - a);
        sceVu0AddVector(out, out, v);
    }
}

/* put the feet on the floor (`on`; off: the weight +0x8C4 back to 1): the weight eases 3:1
 * towards +0x44's for the actor `a`; each foot (left bone +0x8AC, right +0x89C) is dropped onto
 * the floor unless (track flags 0x100 without 4) it isn't planted (+0x64), in which case it keeps
 * the actor's height; then the legs are bent to them */
void func_00211190(u8 *m, s32 on, u8 *a) {
    f32 l[4] __attribute__((aligned(16)));
    f32 r[4] __attribute__((aligned(16)));
    f32 k;
    u32 dl, dr, flags;
    s32 useL, useR;

    if (on == 0) {
        AT(m, 0x8C4, f32) = 1.0f;
        return;
    }
    k = VCALL(m, 0x44, f32 (*)(u8 *, u8 *))(m, a);
    k = 0.75f * AT(m, 0x8C4, f32) + 0.25f * k;
    foot_point(m, AT(m, 0x8AC, s32), l, k);
    dl = (u8)VCALL(m, 0x64, s32 (*)(u8 *, s32, s32))(m, 1, -1);
    foot_point(m, AT(m, 0x89C, s32), r, k);
    dr = (u8)VCALL(m, 0x64, s32 (*)(u8 *, s32, s32))(m, 0, -1);
    useL = 1;
    useR = 1;
    flags = AT(AT(m, 0x6A4, u8 *), 0x18, u32);
    if (!(flags & 4) && (flags & 0x100)) {
        if (dl == 1 && AT(AT(m, 0x6A8, u8 *), 0x20, void *) != NULL) {
            dl = (u8)func_002DD860(m, 1, 0.0f);
        }
        if (dl == 0) {
            useL = 0;
        }
        if (dr == 1 && AT(AT(m, 0x6A8, u8 *), 0x20, void *) != NULL) {
            dr = (u8)func_002DD860(m, 0, 0.0f);
        }
        if (dr == 0) {
            useR = 0;
        }
    }
    if (useL == 1) {
        func_002DC710(m, l, a);
    } else {
        l[1] = AT(a, 0x14, f32);
    }
    if (useR == 1) {
        func_002DC710(m, r, a);
    } else {
        r[1] = AT(a, 0x14, f32);
    }
    func_00211530(m, r, l, AT(m, 0x804, f32));
    AT(m, 0x8C4, f32) = k;
}


/* +0x44 how much of the feet's height to keep on a slope (track flag 4; else 1): 1 - (1 -
 * ny^2) x |the actor's facing (+0x60 matrix) along the floor's downhill direction|, where ny
 * is the up part of the floor normal under it (+0x34) */
f32 func_002DD980(u8 *m, u8 *a) {
    f32 rot[4][4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 k = 1.0f, d;

    if (!(AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 4)) {
        return 1.0f;
    }
    VCALL(D_0044E570, 0x2C, void (*)(NavMesh *, u32, f32 *))(D_0044E570, AT(a, 0x34, u32), n);
    if (n[1] < 1.0f) {
        k = n[1] * n[1];
        n[1] = 0.0f;
        sceVu0Normalize(n, n);
        v[2] = 1.0f;
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[3] = 0.0f;   /* unset in the original */
        sceVu0CopyMatrix(rot, (f32 (*)[4])(a + 0x60));
        sceVu0ApplyMatrix(v, rot, v);
        d = sceVu0InnerProduct(v, n);
        if (!(d <= 0.0f)) {
            d = sceVu0InnerProduct(v, n);
        } else {
            d = -sceVu0InnerProduct(v, n);
        }
        k = 1.0f - (1.0f - k) * d;
    }
    return k;
}


/* +0x64 is foot `foot` (channel of the contact track +0x50) down `ofs` frames from now in the
 * current slot's animation (the time wrapped into it) */
s32 func_002DCDE0(u8 *m, s32 foot, s32 ofs) {
    f32 c[4] __attribute__((aligned(16)));
    u8 *slot = AT(m, 0x6A4, u8 *);
    f32 t, len;

    c[3] = 0.0f;
    c[2] = 0.0f;
    c[1] = 0.0f;
    c[0] = 0.0f;
    if (AT(slot, 0x20, void *) == NULL) {
        return 0;
    }
    t = AT(slot, 0x0, f32) + (f32)ofs;
    len = (f32)AT(AT(AT(slot, 0x20, u8 *), 0x4, u8 *), 0xC, s32);
    while (t < 0.0f) {
        t = t + len;
    }
    while (!(t < len)) {
        t = t - len;
    }
    if (AT(slot, 0x50, void *) != NULL && AT(AT(slot, 0x50, u8 *), 0x0, void *) != NULL) {
        func_001F36B0(AT(slot, 0x50, void *), c, t);
    }
    return !(c[foot] <= 0.0f);
}


/* set up a two-bone IK solver: chain root / middle / end of `skel`, the two bone lengths, -1 */
extern void func_001F1250(u8 *ik, void *skel, s32 root, s32 mid, s32 end, f32 len1, f32 len2, f32 bend);

#define BIND_X(m, bone) AT(AT(m, 0x4C0, u8 *) + 0x10 + (bone) * 0x70, 0x20, f32)

/* one leg's IK (solver `ik`, its end bone's target at ik + 0x20, chain end node ik + 0x54):
 * the target is the floor point `foot` less the foot's offset from the ankle (`toFoot`), at
 * the chain end's height + foot height - `height` */
static void leg_target(u8 *ik, const f32 *foot, const f32 *toFoot, f32 height) {
    AT(ik, 0x20, f32) = foot[0] - toFoot[0];
    AT(ik, 0x24, f32) = AT(AT(ik, 0x54, u8 *), 0x34, f32) + foot[1] - height;
    AT(ik, 0x28, f32) = foot[2] - toFoot[2];
}

/* bend the legs so the feet reach `right` / `left` with the body `height` lower: two-bone IK
 * from hip to ankle (right +0x890 / +0x894 / +0x898, left +0x8A0 / +0x8A4 / +0x8A8), then the
 * ankle and foot (+0x89C / +0x8AC) moved by how far the solved ankle went */
void func_00211530(u8 *m, f32 *right, f32 *left, f32 height) {
    f32 dr[4] __attribute__((aligned(16)));
    f32 dl[4] __attribute__((aligned(16)));
    f32 er[4] __attribute__((aligned(16)));
    f32 el[4] __attribute__((aligned(16)));
    f32 *a, *b;

    func_001F1250(m + 0x8D0, AT(m, 0x810, void *), AT(m, 0x890, s32), AT(m, 0x894, s32), AT(m, 0x898, s32),
                  BIND_X(m, AT(m, 0x894, s32)), BIND_X(m, AT(m, 0x898, s32)), -1.0f);
    func_001F1250(m + 0x930, AT(m, 0x810, void *), AT(m, 0x8A0, s32), AT(m, 0x8A4, s32), AT(m, 0x8A8, s32),
                  BIND_X(m, AT(m, 0x8A4, s32)), BIND_X(m, AT(m, 0x8A8, s32)), -1.0f);
    a = func_0017CE80(AT(m, 0x810, void *), AT(m, 0x898, s32));
    b = func_0017CE80(AT(m, 0x810, void *), AT(m, 0x89C, s32));
    sceVu0SubVector(dr, b + 12, a + 12);
    a = func_0017CE80(AT(m, 0x810, void *), AT(m, 0x8A8, s32));
    b = func_0017CE80(AT(m, 0x810, void *), AT(m, 0x8AC, s32));
    sceVu0SubVector(dl, b + 12, a + 12);
    sceVu0CopyVector(er, (f32 *)(AT(m, 0x924, u8 *) + 0x30));
    sceVu0CopyVector(el, (f32 *)(AT(m, 0x984, u8 *) + 0x30));
    leg_target(m + 0x8D0, right, dr, height);
    leg_target(m + 0x930, left, dl, height);
    VCALL(m + 0x928, 0x8, void (*)(u8 *))(m + 0x8D0);
    VCALL(m + 0x988, 0x8, void (*)(u8 *))(m + 0x930);
    a = func_0017CE80(AT(m, 0x810, void *), AT(m, 0x898, s32));
    b = func_0017CE80(AT(m, 0x810, void *), AT(m, 0x89C, s32));
    sceVu0SubVector(dr, (f32 *)(m + 0x8F0), er);
    sceVu0AddVector(a + 12, a + 12, dr);
    sceVu0AddVector(b + 12, b + 12, dr);
    a = func_0017CE80(AT(m, 0x810, void *), AT(m, 0x8A8, s32));
    b = func_0017CE80(AT(m, 0x810, void *), AT(m, 0x8AC, s32));
    sceVu0SubVector(dl, (f32 *)(m + 0x950), el);
    sceVu0AddVector(a + 12, a + 12, dl);
    sceVu0AddVector(b + 12, b + 12, dl);
}


/* set up a two-bone IK solver { +0x0 root position, +0x10 middle, +0x20 end (the target), +0x40
 * lengths and bend, +0x4C / +0x50 / +0x54 the chain's nodes }: from the bones root / mid / end
 * of `skel` */
void func_001F1250(u8 *ik, void *skel, s32 root, s32 mid, s32 end, f32 len1, f32 len2, f32 bend) {
    f32 *r = func_0017CE80(skel, root);
    f32 *mi = func_0017CE80(skel, mid);
    f32 *e = func_0017CE80(skel, end);

    AT(ik, 0x4C, f32 *) = r;
    AT(ik, 0x50, f32 *) = mi;
    AT(ik, 0x54, f32 *) = e;
    sceVu0CopyVector((f32 *)ik, AT(ik, 0x4C, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x10), AT(ik, 0x50, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x20), AT(ik, 0x54, f32 *) + 12);
    AT(ik, 0x40, f32) = len1;
    AT(ik, 0x44, f32) = len2;
    AT(ik, 0x48, f32) = bend;
}


extern s32 func_001F10C0(u8 *ik, f32 *root, f32 *mid, f32 *end, f32 *pole, f32 len1, f32 len2, f32 bend);   /* 0: solved */
extern void func_001F0F40(u8 *ik);   /* turn the chain's bones to the solution */

/* +0x8 solve: from the chain root's position (and its Z axis as the bend direction, +0x30),
 * place the middle joint for the end target; a target out of reach is pulled in to the
 * chain's full length along its direction; then the bones follow */
void func_001F0E50(u8 *ik) {
    f32 d[4] __attribute__((aligned(16)));
    f32 len;

    sceVu0CopyVector((f32 *)ik, AT(ik, 0x4C, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x10), AT(ik, 0x50, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x30), AT(ik, 0x4C, f32 *) + 8);
    if (func_001F10C0(ik, (f32 *)ik, (f32 *)(ik + 0x10), (f32 *)(ik + 0x20), (f32 *)(ik + 0x30), AT(ik, 0x40, f32),
                      AT(ik, 0x44, f32), AT(ik, 0x48, f32)) != 0) {
        sceVu0SubVector(d, (f32 *)(ik + 0x20), (f32 *)ik);
        sceVu0Normalize(d, d);
        len = AT(ik, 0x40, f32) + AT(ik, 0x44, f32);
        AT(ik, 0x20, f32) = 0.0f + AT(ik, 0x0, f32) + d[0] * len;
        AT(ik, 0x24, f32) = 0.0f + AT(ik, 0x4, f32) + d[1] * len;
        AT(ik, 0x28, f32) = 0.0f + AT(ik, 0x8, f32) + d[2] * len;
    }
    func_001F0F40(ik);
}


/* place the middle joint `mid` of a two-bone chain (lengths len1, len2) from `root` towards
 * `end`, bent to the side of `pole` x direction (scaled by `bend`); 1 when `end` is out of
 * reach (the chain then points straight at it). (The EE's square root takes |x|.) */
s32 func_001F10C0(u8 *ik, f32 *root, f32 *mid, f32 *end, f32 *pole, f32 len1, f32 len2, f32 bend) {
    f32 d[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 dist, a, h, l1;
    s32 out = 0;

    sceVu0SubVector(d, end, root);
    dist = __builtin_sqrtf(__builtin_fabsf(sceVu0InnerProduct(d, d)));
    sceVu0Normalize(d, d);
    if (!(dist <= len1 + len2)) {
        dist = len1 + len2;
        out = 1;
    }
    sceVu0OuterProduct(side, pole, d);
    sceVu0Normalize(side, side);
    l1 = len1 * len1;
    side[0] = side[0] * bend;
    side[1] = side[1] * bend;
    side[2] = side[2] * bend;
    a = (l1 - len2 * len2 + dist * dist) / (2.0f * dist);
    h = __builtin_sqrtf(__builtin_fabsf(l1 - a * a));
    mid[0] = root[0] + a * d[0] + h * side[0];
    mid[1] = root[1] + a * d[1] + h * side[1];
    mid[2] = root[2] + a * d[2] + h * side[2];
    return out;
}


/* a bone's matrix aimed from `from` to `to`: X along it, Z from the pole (+0x30) made square
 * to it, Y = Z x X, all unit, at `from` */
static void ik_aim(f32 (*m)[4], const f32 *from, const f32 *to, const f32 *pole) {
    sceVu0UnitMatrix(m);
    m[0][0] = to[0] - from[0];
    m[0][1] = to[1] - from[1];
    m[0][2] = to[2] - from[2];
    sceVu0CopyVector(m[2], (f32 *)pole);
    sceVu0OuterProduct(m[1], m[2], m[0]);
    sceVu0OuterProduct(m[2], m[0], m[1]);
    sceVu0Normalize(m[0], m[0]);
    sceVu0Normalize(m[1], m[1]);
    sceVu0Normalize(m[2], m[2]);
    sceVu0TransMatrix(m, m, (f32 *)from);
}

/* turn the chain's bones to the solved joints: the root (+0x4C) towards the middle, the
 * middle (+0x50) towards the end */
void func_001F0F40(u8 *ik) {
    ik_aim((f32 (*)[4])AT(ik, 0x4C, u8 *), (f32 *)ik, (f32 *)(ik + 0x10), (f32 *)(ik + 0x30));
    ik_aim((f32 (*)[4])AT(ik, 0x50, u8 *), (f32 *)(ik + 0x10), (f32 *)(ik + 0x20), (f32 *)(ik + 0x30));
}

/* turn bone `m` so its X runs `from` -> `to`, keeping its Z side, at `from` */
static inline void ik_turn(f32 (*m)[4], const f32 *from, const f32 *to) {
    m[0][0] = to[0] - from[0];
    m[0][1] = to[1] - from[1];
    m[0][2] = to[2] - from[2];
    sceVu0OuterProduct(m[1], m[2], m[0]);
    sceVu0OuterProduct(m[2], m[0], m[1]);
    sceVu0Normalize(m[0], m[0]);
    sceVu0Normalize(m[1], m[1]);
    sceVu0Normalize(m[2], m[2]);
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
    sceVu0TransMatrix(m, m, (f32 *)from);
}

/* +0xC solve keeping the bend: the bend direction (+0x30) square to the root -> target line
 * in the plane of the root's Z, the knee kept on the side it is bent to now; a target out of
 * reach is pulled in; then the root and middle bones turn to the joints and the end bone is
 * moved onto the target */
void func_001F0AF0(u8 *ik) {
    f32 d[4] __attribute__((aligned(16)));
    f32 z[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    f32 a[4] __attribute__((aligned(16)));
    f32 len;

    sceVu0CopyVector((f32 *)ik, AT(ik, 0x4C, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x10), AT(ik, 0x50, f32 *) + 12);
    sceVu0CopyVector(z, AT(ik, 0x4C, f32 *) + 8);
    sceVu0SubVector(a, (f32 *)(ik + 0x20), AT(ik, 0x4C, f32 *) + 12);
    sceVu0OuterProduct(b, z, a);
    sceVu0OuterProduct((f32 *)(ik + 0x30), a, b);
    sceVu0Normalize((f32 *)(ik + 0x30), (f32 *)(ik + 0x30));
    sceVu0SubVector(a, AT(ik, 0x50, f32 *) + 12, AT(ik, 0x4C, f32 *) + 12);
    sceVu0SubVector(b, AT(ik, 0x54, f32 *) + 12, AT(ik, 0x50, f32 *) + 12);
    sceVu0OuterProduct(z, a, b);
    if (func_001F10C0(ik, (f32 *)ik, (f32 *)(ik + 0x10), (f32 *)(ik + 0x20), (f32 *)(ik + 0x30), AT(ik, 0x40, f32),
                      AT(ik, 0x44, f32), sceVu0InnerProduct(AT(ik, 0x4C, f32 *) + 8, z) <= 0.0f ? 1.0f : -1.0f) != 0) {
        sceVu0SubVector(d, (f32 *)(ik + 0x20), (f32 *)ik);
        sceVu0Normalize(d, d);
        len = AT(ik, 0x40, f32) + AT(ik, 0x44, f32);
        AT(ik, 0x20, f32) = 0.0f + AT(ik, 0x0, f32) + d[0] * len;
        AT(ik, 0x24, f32) = 0.0f + AT(ik, 0x4, f32) + d[1] * len;
        AT(ik, 0x28, f32) = 0.0f + AT(ik, 0x8, f32) + d[2] * len;
    }
    ik_turn((f32 (*)[4])AT(ik, 0x4C, u8 *), (f32 *)ik, (f32 *)(ik + 0x10));
    ik_turn((f32 (*)[4])AT(ik, 0x50, u8 *), (f32 *)(ik + 0x10), (f32 *)(ik + 0x20));
    sceVu0CopyVector(AT(ik, 0x54, f32 *) + 12, (f32 *)(ik + 0x20));
}


/* ---- the three-bone IK solver (vtable D_0046B0C0, derived from the two-bone one; 0x90 bytes):
 * a dog's leg - root +0x0, knee +0x10, hock +0x60, foot +0x20 (the target), the bend
 * direction +0x30; lengths +0x40 / +0x44 / +0x70, the two bends +0x48 / +0x74, how far round
 * from the knee's to the hock's direction the lower leg points +0x7C, the leg's full reach
 * +0x80; bones +0x4C / +0x50 / +0x78 / +0x54 ---- */

extern f32 func_0031C3C0(f32 x);   /* acosf */
extern f32 func_0031C248(f32 x);   /* sinf */

/* out = a + b * s */
static void ik_madd(f32 *out, const f32 *a, const f32 *b, f32 s) {
    out[0] = 0.0f + a[0] + b[0] * s;
    out[1] = 0.0f + a[1] + b[1] * s;
    out[2] = 0.0f + a[2] + b[2] * s;
}

/* out = a + b * s (a method of the solver) */
void func_001F17F0(u8 *ik, f32 *out, const f32 *a, const f32 *b, f32 s) {
    ik_madd(out, a, b, s);
}

/* v's component along unit `axis` (*along) and its distance from it (*off) */
void func_001F1430(u8 *ik, const f32 *axis, const f32 *v, f32 *off, f32 *along) {
    *along = sceVu0InnerProduct((f32 *)axis, (f32 *)v);
    *off = __builtin_sqrtf(__builtin_fabsf(0.0f + sceVu0InnerProduct((f32 *)v, (f32 *)v) - *along * *along));
}

/* how far round from b to c (by the angles they make with a) a lies: ang(a,b) / (ang(a,b) + ang(a,c)) */
f32 func_001F1330(u8 *ik, f32 *a, f32 *b, f32 *c) {
    f32 la = __builtin_sqrtf(__builtin_fabsf(sceVu0InnerProduct(a, a)));
    f32 lb = __builtin_sqrtf(__builtin_fabsf(sceVu0InnerProduct(b, b)));
    f32 lc = __builtin_sqrtf(__builtin_fabsf(sceVu0InnerProduct(c, c)));
    f32 ab = func_0031C3C0(sceVu0InnerProduct(a, b) / (la * lb));
    f32 ac = func_0031C3C0(sceVu0InnerProduct(a, c) / (la * lc));

    return ab / (ab + ac);
}

/* the unit vector `t` of the way along the arc from unit `p` to unit `q`, `w` apart */
static void slerp(f32 *out, const f32 *p, const f32 *q, f32 w, f32 t) {
    f32 s = func_0031C248(w);
    f32 inv, sp, sq;

    if (s == 0.0f) {
        out[0] = p[0];
        out[1] = p[1];
        out[2] = p[2];
        return;
    }
    inv = 1.0f / s;
    sp = func_0031C248(w * (1.0f - t));
    sq = func_0031C248(w * t);
    out[0] = inv * (q[0] * sq + p[0] * sp);
    out[1] = inv * (q[1] * sq + p[1] * sp);
    out[2] = inv * (q[2] * sq + p[2] * sp);
}

/* the unit vector `t` of the way round from `a` to `b`, through `mid` when a and b lie on its
   either side (the arc then goes round by way of it) */
void func_001F14A0(u8 *ik, f32 *out, f32 *a, f32 *mid, f32 *b, f32 t) {
    f32 ca[4] __attribute__((aligned(16)));
    f32 cb[4] __attribute__((aligned(16)));
    f32 w1, w2, sum;

    sceVu0OuterProduct(ca, mid, a);
    sceVu0OuterProduct(cb, mid, b);
    if (!(sceVu0InnerProduct(ca, cb) < 0.0f)) {
        slerp(out, a, b, func_0031C3C0(sceVu0InnerProduct(a, b)), t);
        return;
    }
    w1 = func_0031C3C0(sceVu0InnerProduct(a, mid));
    w2 = func_0031C3C0(sceVu0InnerProduct(mid, b));
    sum = w1 + w2;
    if (t < w1 / sum) {
        slerp(out, a, mid, w1, t * sum / w1);
    } else {
        slerp(out, mid, b, w2, (t * sum - w1) / w2);
    }
}

/* place the knee `knee` and the hock `hock` of a leg from `root` to `foot`: the lower leg's
 * direction is `t` of the way round from where the knee (as a two-bone chain root / knee+hock)
 * to where the hock (as root+knee / hock) would put it, by way of the line to the root; a foot
 * out of reach (+0x80) is pulled in first (returns 1) */
s32 func_001F0600(u8 *ik, f32 *root, f32 *knee, f32 *hock, f32 *foot, f32 *pole, f32 len1, f32 len2,
                  f32 len3, f32 bend1, f32 bend2, f32 t) {
    f32 up[4] __attribute__((aligned(16)));
    f32 dk[4] __attribute__((aligned(16)));
    f32 dh[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    f32 dist;
    s32 r1, r2, out = 0;

    sceVu0SubVector(dk, foot, root);
    up[0] = -dk[0];
    up[1] = -dk[1];
    up[2] = -dk[2];
    up[3] = 0.0f;
    r1 = func_001F10C0(ik, root, knee, foot, pole, len1, len2 + len3, bend1);
    sceVu0SubVector(dk, knee, foot);
    sceVu0Normalize(dk, dk);
    r2 = func_001F10C0(ik, root, hock, foot, pole, len1 + len2, len3, bend2);
    sceVu0SubVector(dh, hock, foot);
    sceVu0Normalize(dh, dh);
    dist = __builtin_sqrtf(__builtin_fabsf(sceVu0InnerProduct(up, up)));
    sceVu0Normalize(up, up);
    if (r1 == 0 && r2 == 0) {
        func_001F14A0(ik, dir, dk, up, dh, t);
    } else {
        sceVu0CopyVector(dir, dk);
    }
    if (!(dist <= AT(ik, 0x80, f32))) {
        out = 1;
        ik_madd(foot, root, up, -AT(ik, 0x80, f32));
    }
    ik_madd(hock, foot, dir, len3);
    func_001F10C0(ik, root, knee, hock, pole, len1, len2, bend1);
    return out;
}

/* turn the bones to the solved joints: root -> knee, knee -> hock, hock -> foot */
void func_001F08C0(u8 *ik) {
    ik_aim((f32 (*)[4])AT(ik, 0x4C, u8 *), (f32 *)ik, (f32 *)(ik + 0x10), (f32 *)(ik + 0x30));
    ik_aim((f32 (*)[4])AT(ik, 0x50, u8 *), (f32 *)(ik + 0x10), (f32 *)(ik + 0x60), (f32 *)(ik + 0x30));
    ik_aim((f32 (*)[4])AT(ik, 0x78, u8 *), (f32 *)(ik + 0x60), (f32 *)(ik + 0x20), (f32 *)(ik + 0x30));
}

/* set up: from the bones root / knee / hock / foot of `skel`, the lengths, the bends, the reach */
void func_001F04B0(u8 *ik, void *skel, s32 root, s32 knee, s32 hock, s32 foot, f32 len1, f32 len2, f32 len3,
                   f32 bend1, f32 bend2, f32 reach) {
    f32 *r = func_0017CE80(skel, root);
    f32 *k = func_0017CE80(skel, knee);
    f32 *h = func_0017CE80(skel, hock);
    f32 *f = func_0017CE80(skel, foot);

    AT(ik, 0x4C, f32 *) = r;
    AT(ik, 0x50, f32 *) = k;
    AT(ik, 0x78, f32 *) = h;
    AT(ik, 0x54, f32 *) = f;
    sceVu0CopyVector((f32 *)ik, AT(ik, 0x4C, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x10), AT(ik, 0x50, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x60), AT(ik, 0x78, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x20), AT(ik, 0x54, f32 *) + 12);
    AT(ik, 0x40, f32) = len1;
    AT(ik, 0x44, f32) = len2;
    AT(ik, 0x70, f32) = len3;
    AT(ik, 0x48, f32) = bend1;
    AT(ik, 0x74, f32) = bend2;
    AT(ik, 0x7C, f32) = 0.5f;
    AT(ik, 0x80, f32) = reach;
}

/* +0x8 solve with the current lower-leg blend */
void func_001F0450(u8 *ik) {
    func_001F0600(ik, (f32 *)ik, (f32 *)(ik + 0x10), (f32 *)(ik + 0x60), (f32 *)(ik + 0x20), (f32 *)(ik + 0x30),
                  AT(ik, 0x40, f32), AT(ik, 0x44, f32), AT(ik, 0x70, f32), AT(ik, 0x48, f32), AT(ik, 0x74, f32),
                  AT(ik, 0x7C, f32));
    func_001F08C0(ik);
}

/* +0xC solve keeping the leg's pose: the bend direction is the root's Z; the blend +0x7C is
 * taken from the current pose (where the hock lies between the knee's and the hock's
 * two-bone solutions, seen from the foot); the target is solved in the plane through the
 * root square to the bend direction, with the hock-to-foot distance from that axis as the
 * lower leg, and moved back out along it; then the bones follow and the foot bone goes onto
 * the target */
void func_001F0250(u8 *ik) {
    f32 kneeAt[4] __attribute__((aligned(16)));
    f32 hockAt[4] __attribute__((aligned(16)));
    f32 flat[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 *pole = (f32 *)(ik + 0x30);
    f32 *footAt = AT(ik, 0x54, f32 *) + 12;
    f32 off, along;

    sceVu0CopyVector((f32 *)ik, AT(ik, 0x4C, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x10), AT(ik, 0x50, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x60), AT(ik, 0x78, f32 *) + 12);
    sceVu0CopyVector(pole, AT(ik, 0x4C, f32 *) + 8);
    sceVu0SubVector(v, footAt, AT(ik, 0x78, f32 *) + 12);
    func_001F1430(ik, pole, v, &off, &along);
    func_001F17F0(ik, flat, (f32 *)(ik + 0x20), pole, -along);
    func_001F10C0(ik, (f32 *)ik, kneeAt, footAt, pole, AT(ik, 0x40, f32), AT(ik, 0x44, f32) + AT(ik, 0x70, f32),
                  AT(ik, 0x48, f32));
    func_001F10C0(ik, (f32 *)ik, hockAt, footAt, pole, AT(ik, 0x40, f32) + AT(ik, 0x44, f32), AT(ik, 0x70, f32),
                  AT(ik, 0x74, f32));
    sceVu0SubVector(v, AT(ik, 0x78, f32 *) + 12, footAt);
    sceVu0SubVector(kneeAt, kneeAt, footAt);
    sceVu0SubVector(hockAt, hockAt, footAt);
    AT(ik, 0x7C, f32) = func_001F1330(ik, v, kneeAt, hockAt);
    func_001F0600(ik, (f32 *)ik, (f32 *)(ik + 0x10), (f32 *)(ik + 0x60), flat, pole, AT(ik, 0x40, f32),
                  AT(ik, 0x44, f32), off, AT(ik, 0x48, f32), AT(ik, 0x74, f32), AT(ik, 0x7C, f32));
    func_001F17F0(ik, (f32 *)(ik + 0x20), flat, pole, along);
    func_001F08C0(ik);
    sceVu0CopyVector(footAt, (f32 *)(ik + 0x20));
}


extern void func_002EE8A0(u8 *springs);   /* begin a step */
extern void func_002EE900(u8 *springs);   /* one step */
extern void func_002EE840(u8 *springs);   /* finish */

/* the four spring systems (+0x13B0, +0x1440, +0x1740, +0x17E0), a frame: one step, or after a
 * reset (+0x850) - the 4 hanging points (+0x1480, 0x50 each) put back under their anchors (a
 * bone +0x24 when +0x20, else the point +0x2C) by their length (+0x40) along bone 0x35's
 * Z axis, at rest - 30 steps to settle */
void func_002F81A0(u8 *o) {
    f32 down[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    s32 n, i;
    u8 *p;

    if (AT(o, 0x850, u8) == 0) {
        n = 1;
    } else {
        sceVu0CopyVector(down, func_0017CE80(AT(AT(o, 0x1754, u8 *), 0x810, void *), 0x35) + 8);
        p = o + 0x1480;
        for (i = 0; i < 4; i++) {
            AT(p, 0x18, f32) = 0.0f;
            AT(p, 0x14, f32) = 0.0f;
            AT(p, 0x10, f32) = 0.0f;
            if (AT(p, 0x20, u8) != 0) {
                sceVu0CopyVector(at, func_0017CE80(AT(AT(o, 0x1754, u8 *), 0x810, void *), AT(p, 0x24, s32)) + 12);
            } else {
                sceVu0CopyVector(at, AT(p, 0x2C, f32 *));
            }
            sceVu0ScaleVector(d, down, -AT(p, 0x40, f32));
            sceVu0AddVector((f32 *)p, at, d);
            p += 0x50;
        }
        n = 0x1E;
    }
    func_002EE8A0(o + 0x13B0);
    func_002EE8A0(o + 0x1440);
    func_002EE8A0(o + 0x1740);
    func_002EE8A0(o + 0x17E0);
    for (i = 0; i < n; i++) {
        func_002EE900(o + 0x13B0);
        func_002EE900(o + 0x1440);
        func_002EE900(o + 0x1740);
        func_002EE900(o + 0x17E0);
    }
    func_002EE840(o + 0x13B0);
    func_002EE840(o + 0x1440);
    func_002EE840(o + 0x1740);
    func_002EE840(o + 0x17E0);
    AT(o, 0x850, u8) = 0;
}

/* A spring system: point masses (first +0x18, next +0x2C) and the links between them (first
 * +0x30, next +0x28), each with its vtable at +0x30 of itself. */

/* begin a frame: each point (+0xC) with the system's +0x14 */
void func_002EE8A0(u8 *s) {
    u8 *p;

    for (p = AT(s, 0x18, u8 *); p != NULL; p = AT(p, 0x2C, u8 *)) {
        VCALL(p + 0x30, 0xC, void (*)(u8 *, void *))(p, AT(s, 0x14, void *));
    }
}

/* one step: each link (+0x10) */
void func_002EE900(u8 *s) {
    u8 *l;

    for (l = AT(s, 0x30, u8 *); l != NULL; l = AT(l, 0x28, u8 *)) {
        VCALL(l + 0x30, 0x10, void (*)(u8 *, u8 *))(l, s);
    }
}

/* finish the frame: each link (+0x14) */
void func_002EE840(u8 *s) {
    u8 *l;

    for (l = AT(s, 0x30, u8 *); l != NULL; l = AT(l, 0x28, u8 *)) {
        VCALL(l + 0x30, 0x14, void (*)(u8 *, u8 *))(l, s);
    }
}


/* +0xC of a point fixed to a bone: its position (+0x0) is the bone (+0x28) of the model's
 * skeleton applied to its offset (+0x10) */
void func_002EE570(u8 *p, u8 *model) {
    f32 m[4][4] __attribute__((aligned(16)));

    sceVu0CopyMatrix(m, (f32 (*)[4])func_0017CE80(AT(model, 0x810, void *), AT(p, 0x28, s32)));
    sceVu0ApplyMatrix((f32 *)p, m, (f32 *)(p + 0x10));
}


extern f32 func_0031C3C0(f32 x);   /* acosf */
extern f32 func_0031C248(f32 x);   /* sinf */

/* +0x10 step of a hanging point (hair / cloth) { +0x0 position, +0x10 velocity, +0x20 anchored to
 * a bone (+0x24) else to the point +0x2C, +0x40 length, +0x44 the bone it hangs along, +0x48
 * its side axis, +0x4C the cone angle } in system `s` (+0x4 gravity, +0x10 damping): pulled
 * along the hanging bone's X axis, damped, moved; then kept at its length from the anchor,
 * within the cone around that axis and off the back of the plane of the axis and the side
 * axis, its velocity following the corrections */
void func_00315F00(u8 *l, u8 *s) {
    f32 anchor[4] __attribute__((aligned(16)));
    f32 prev[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 x[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 *a = func_0017CE80(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(l, 0x24, s32));
    f32 *b = func_0017CE80(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(l, 0x44, s32));
    f32 ang, sa, sl, sr, inv, k1, k2;

    if (AT(l, 0x20, u8) != 0) {
        sceVu0CopyVector(anchor, a + 12);
    } else {
        sceVu0CopyVector(anchor, AT(l, 0x2C, f32 *));
    }
    sceVu0CopyVector(prev, (f32 *)l);
    sceVu0ScaleVector(d, b, AT(s, 0x4, f32));
    sceVu0AddVector((f32 *)(l + 0x10), (f32 *)(l + 0x10), d);
    sceVu0ScaleVector((f32 *)(l + 0x10), (f32 *)(l + 0x10), AT(s, 0x10, f32));
    sceVu0AddVector((f32 *)l, (f32 *)l, (f32 *)(l + 0x10));
    sceVu0SubVector(d, (f32 *)l, anchor);
    sceVu0Normalize(d, d);
    ang = func_0031C3C0(sceVu0InnerProduct(d, b));
    if (!(ang <= AT(l, 0x4C, f32))) {
        sa = func_0031C248(ang);
        if (!(sa <= 0.0f)) {
            sl = func_0031C248(AT(l, 0x4C, f32));
            sr = func_0031C248(ang - AT(l, 0x4C, f32));
            inv = 1.0f / sa;
            d[0] = inv * (sr * b[0] + sl * d[0]);
            d[1] = inv * (sr * b[1] + sl * d[1]);
            d[2] = inv * (sr * b[2] + sl * d[2]);
        }
    }
    sceVu0ScaleVector(d, d, AT(l, 0x40, f32));
    sceVu0AddVector((f32 *)l, anchor, d);
    sceVu0SubVector((f32 *)(l + 0x10), (f32 *)l, prev);
    sceVu0CopyVector(x, b);
    sceVu0ApplyMatrix(side, (f32 (*)[4])b, AT(l, 0x48, f32 *));
    k1 = sceVu0InnerProduct(d, x);
    k2 = sceVu0InnerProduct(d, side);
    sceVu0CopyVector(prev, (f32 *)l);
    if (k2 < 0.0f) {
        k2 = 0.0f;
    }
    sceVu0ScaleVector(x, x, k1);
    sceVu0ScaleVector(side, side, k2);
    sceVu0AddVector(d, x, side);
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, AT(l, 0x40, f32));
    sceVu0AddVector((f32 *)l, anchor, d);
    sceVu0SubVector(d, (f32 *)l, prev);
    sceVu0AddVector((f32 *)(l + 0x10), (f32 *)(l + 0x10), d);
}


/* +0x10 step of a point hanging from a bone (+0x24) { +0x0 position, +0x10 velocity, +0x40
 * length } in system `s` (+0x0 force, subtracted; +0x10 damping): moved, then kept at its
 * length from the bone, on the front side of the bone's Y axis and within 30 degrees of its X
 * axis; its velocity is how far it went */
void func_002EE970(u8 *p, u8 *s) {
    static const union { u32 u; f32 f; } kCone = {0x3F060A92};   /* 30 degrees */
    f32 m[4][4] __attribute__((aligned(16)));
    f32 prev[4] __attribute__((aligned(16)));
    f32 anchor[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 k, ang, sl, sr, inv;

    sceVu0CopyVector(prev, (f32 *)p);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), (f32 *)s);
    sceVu0ScaleVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), AT(s, 0x10, f32));
    sceVu0AddVector((f32 *)p, (f32 *)p, (f32 *)(p + 0x10));
    sceVu0CopyMatrix(m, (f32 (*)[4])func_0017CE80(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32)));
    sceVu0CopyVector(anchor, m[3]);
    sceVu0SubVector(d, (f32 *)p, anchor);
    k = sceVu0InnerProduct(d, m[1]);
    if (k < 0.0f) {
        sceVu0ScaleVector(v, m[1], k);
        sceVu0SubVector(d, d, v);
    }
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
    ang = func_0031C3C0(sceVu0InnerProduct(d, m[0]));
    if (!(ang <= kCone.f)) {
        sl = func_0031C248(kCone.f);
        sr = func_0031C248(ang - kCone.f);
        inv = 1.0f / func_0031C248(ang);
        d[0] = inv * (sr * m[0][0] + sl * d[0]);
        d[1] = inv * (sr * m[0][1] + sl * d[1]);
        d[2] = inv * (sr * m[0][2] + sl * d[2]);
    }
    sceVu0AddVector((f32 *)p, anchor, d);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)p, prev);
}


/* +0x10 step of a free hanging point { +0x0 position, +0x10 velocity, +0x20 anchored to a bone
 * (+0x24) else the point +0x2C, +0x40 length } in system `s` (+0x0 force, +0x10 damping, +0x18
 * the colliders, +0x20 / +0x1C a floor height): pushed by each collider (+0x8, strength 1) and
 * up off the floor, damped, moved, then kept at its length from the anchor; its velocity is
 * how far it went */
void func_002ECE50(u8 *p, u8 *s) {
    f32 prev[4] __attribute__((aligned(16)));
    f32 anchor[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u8 *c;

    sceVu0CopyVector(prev, (f32 *)p);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), (f32 *)s);
    for (c = AT(s, 0x18, u8 *); c != NULL; c = AT(c, 0x2C, u8 *)) {
        VCALL(c + 0x30, 0x8, void (*)(u8 *, f32 *, u8 *, f32))(c, d, p, 1.0f);
        sceVu0AddVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), d);
    }
    if (AT(s, 0x20, u8) != 0 && AT(p, 0x4, f32) < AT(s, 0x1C, f32)) {
        AT(p, 0x14, f32) = AT(p, 0x14, f32) + (AT(s, 0x1C, f32) - AT(p, 0x4, f32));
    }
    sceVu0ScaleVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), AT(s, 0x10, f32));
    sceVu0AddVector((f32 *)p, (f32 *)p, (f32 *)(p + 0x10));
    if (AT(p, 0x20, u8) != 0) {
        sceVu0CopyVector(anchor, func_0017CE80(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32)) + 12);
    } else {
        sceVu0CopyVector(anchor, AT(p, 0x2C, f32 *));
    }
    sceVu0SubVector(d, (f32 *)p, anchor);
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
    sceVu0AddVector((f32 *)p, anchor, d);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)p, prev);
}


/* +0x8 a sphere collider { +0x0 centre, +0x20 radius, +0x24 falloff }: the push `out` on the
 * point `at` (strength `k`): inside the sphere its offset from the centre x (1 - distance x
 * falloff) x k, else none */
void func_002EE5C0(u8 *c, f32 *out, f32 *at, f32 k) {
    f32 d[4] __attribute__((aligned(16)));
    f32 dd = 0.0f;

    sceVu0SubVector(d, at, (f32 *)c);
    dd = sceVu0InnerProduct(d, d);
    if (dd < AT(c, 0x20, f32) * AT(c, 0x20, f32) && !(dd <= 0.0f)) {
        sceVu0ScaleVector(out, d, (1.0f - __builtin_sqrtf(__builtin_fabsf(dd)) * AT(c, 0x24, f32)) * k);
        return;
    }
    out[3] = 0.0f;
    out[2] = 0.0f;
    out[1] = 0.0f;
    out[0] = 0.0f;
}


/* +0x10 step of a point sprung to a bone (+0x24) { +0x0 position, +0x10 velocity, +0x40
 * stiffness } in system `s` (+0x10 damping): pulled to the bone, damped, moved, then flattened
 * into the bone's X / Y plane and kept within -0.2..0.2 along X and -0.2..0.3 along Y of it */
void func_002EEDA0(u8 *p, u8 *s) {
    static const union { u32 u; f32 f; } k03 = {0x3E99999A}, k02 = {0x3E4CCCCD}, kn02 = {0xBE4CCCCD};
    f32 *b = func_0017CE80(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32));
    f32 prev[4] __attribute__((aligned(16)));
    f32 o[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 x, y;

    sceVu0CopyVector(prev, (f32 *)p);
    sceVu0CopyVector(o, b + 12);
    sceVu0SubVector(d, o, (f32 *)p);
    sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
    sceVu0AddVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), d);
    sceVu0ScaleVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), AT(s, 0x10, f32));
    sceVu0AddVector((f32 *)p, (f32 *)p, (f32 *)(p + 0x10));
    sceVu0SubVector(d, (f32 *)p, b + 12);
    sceVu0ScaleVector(v, b + 8, sceVu0InnerProduct(d, b + 8));
    sceVu0SubVector(d, d, v);
    x = sceVu0InnerProduct(d, b);
    y = sceVu0InnerProduct(d, b + 4);
    if (!(y <= k03.f)) {
        y = k03.f;
    }
    if (y < kn02.f) {
        y = kn02.f;
    }
    if (!(x <= k02.f)) {
        x = k02.f;
    }
    if (x < kn02.f) {
        x = kn02.f;
    }
    sceVu0ScaleVector(d, b, x);
    sceVu0ScaleVector(v, b + 4, y);
    sceVu0AddVector(d, d, v);
    sceVu0AddVector((f32 *)p, b + 12, d);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)p, prev);
}


/* +0x14 of a hanging point: its anchor bone (+0x24) is aimed at it - X from the anchor to the
 * point, Y the hanging bone's (+0x44) side axis (+0x48), made square, at the anchor */
void func_00315E00(u8 *p, u8 *s) {
    f32 *a = func_0017CE80(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32));
    f32 *b = func_0017CE80(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x44, s32));
    f32 anchor[4] __attribute__((aligned(16)));

    if (AT(p, 0x20, u8) != 0) {
        sceVu0CopyVector(anchor, a + 12);
    } else {
        sceVu0CopyVector(anchor, AT(p, 0x2C, f32 *));
    }
    sceVu0SubVector(a, (f32 *)p, anchor);
    sceVu0ApplyMatrix(a + 4, (f32 (*)[4])b, AT(p, 0x48, f32 *));
    sceVu0OuterProduct(a + 8, a, a + 4);
    sceVu0OuterProduct(a + 4, a + 8, a);
    sceVu0Normalize(a, a);
    sceVu0Normalize(a + 4, a + 4);
    sceVu0Normalize(a + 8, a + 8);
    sceVu0CopyVector(a + 12, anchor);
}


/* +0x14 of a chained point: its bone (+0x24) is aimed at it - X from the anchor (the bone's own
 * position, or the point it hangs from, +0x2C) to the point, keeping the Y axis of the anchor's
 * bone, made square, at the anchor */
void func_002EE710(u8 *p, u8 *s) {
    f32 *a = func_0017CE80(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32));
    f32 anchor[4] __attribute__((aligned(16)));
    f32 x[4] __attribute__((aligned(16)));
    f32 up[4] __attribute__((aligned(16)));

    if (AT(p, 0x20, u8) != 0) {
        sceVu0CopyVector(anchor, a + 12);
        sceVu0SubVector(x, (f32 *)p, anchor);
        sceVu0CopyVector(up, a + 4);
    } else {
        sceVu0CopyVector(anchor, AT(p, 0x2C, f32 *));
        sceVu0SubVector(x, (f32 *)p, anchor);
        sceVu0CopyVector(up, func_0017CE80(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(AT(p, 0x2C, u8 *), 0x24, s32)) + 4);
    }
    sceVu0CopyVector(a, x);
    sceVu0OuterProduct(a + 8, a, up);
    sceVu0OuterProduct(a + 4, a + 8, a);
    sceVu0Normalize(a, a);
    sceVu0Normalize(a + 4, a + 4);
    sceVu0Normalize(a + 8, a + 8);
    sceVu0CopyVector(a + 12, anchor);
}


/* +0x14 of a sprung point: its bone (+0x24) moves to it, its axes scaled by +0x50 / +0x54 /
 * +0x58 */
void func_002EED20(u8 *p, u8 *s) {
    f32 *b = func_0017CE80(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32));

    sceVu0ScaleVector(b, b, AT(p, 0x50, f32));
    sceVu0ScaleVector(b + 4, b + 4, AT(p, 0x54, f32));
    sceVu0ScaleVector(b + 8, b + 8, AT(p, 0x58, f32));
    sceVu0CopyVector(b + 12, (f32 *)p);
}


extern void func_001F6870(u8 *m, s32 layer, s32 a, s32 b, f32 *light);   /* queue the model's drawing */

/* draw the model in `layer` (a, b: the character's draw parameters) with its light (+0x6C) */
void func_002DDAC0(u8 *m, s32 layer, s32 a, s32 b) {
    f32 light[4] __attribute__((aligned(16)));

    VCALL(m, 0x6C, void (*)(u8 *, f32 *))(m, light);
    func_001F6870(m, layer, a, b, light);
}


extern VObject *gBootMessage;   /* (also the characters' texture sets: +0x24 slot, +0x28 entry) */
extern VObject *D_0044E4E8;     /* the texture cache */
extern VObject *D_0044E4F0;     /* the renderer */
extern VObject *D_0044E4B8;     /* the camera */
extern void func_001F3530(u8 *shadow, s32 a, s32 b, f32 *light, s32 layer);   /* the shadow drawer */

#ifdef HG_NATIVE
#include <stdio.h>
#include <stdlib.h>

extern void glr_strip(const f32 *mvp, s32 n, const f32 *xyzw, const f32 *st, const u8 *rgba, const void *tex,
                      u64 tex0, u32 prim);
extern void glr_todo(const char *what);

/* ---- PC: a character model drawn with OpenGL (what the model drawer's VU1 packets do) ----
 *
 * Resource 0 (+0x4C0, docs/model_format.md): bone records (0x70, the inverse bind matrix at
 * +0x30), the skinned parts (mesh table) and the rigid ones; resource 1 (+0x4D0) the morphing
 * parts. A vertex is in model space; bone b moves it by (inverse bind b) x (b's world matrix,
 * the skeleton at +0x810). Skinning is done here on the CPU. */

typedef struct {
    f32 *xyzw, *st;
    u8 *rgba;
    s32 cap;
} ModelBuf;

static ModelBuf sMb;

/* Lighting, as the character microprograms do it (tools/vudis.py, e.g. the chain D_003A38C0):
 * the model's light set comes from the scene's lights (+0x10, func_001FAA00, as the drawer
 * func_001BDF80 asks for it: at its root bone +0x2C, on nav triangle +0x28) - per light a
 * direction (columns of the transposed matrix, the 4th row -dir.L), a colour, a falloff; the
 * 4th colour row is the ambient. A vertex at world P with normal N gets
 *   min(ambient + sum_i colour_i * max(dir_i.N^, 0) * max(1 + falloff_i * (dir_i.P - dir_i.L_i), 0), 128)
 * (0x80 = 1.0 against the texture), alpha 127. */
extern VObject *D_0044E4C8;   /* the scene's lights */
static f32 sLDir[4][4] __attribute__((aligned(16)));
static f32 sLCol[4][4] __attribute__((aligned(16)));
static f32 sLFall[4];

static void light_setup(u8 *m) {
    f32 *root = func_0017CE80(AT(m, 0x18, void *), AT(m, 0x2C, s32));

    VCALL(D_0044E4C8, 0x10, void (*)(VObject *, f32 *, s32, f32 (*)[4], f32 (*)[4], f32 *, f32 (*)[4]))(
        D_0044E4C8, root != NULL ? root + 12 : NULL, AT(m, 0x28, s32), sLDir, sLCol, sLFall, NULL);
    {
        static s32 sDbg = -1, sN;

        if (sDbg < 0) {
            sDbg = getenv("HG_LIGHTDEBUG") != NULL;
        }
        if (sDbg && (sN++ % 120) == 0) {
            fprintf(stderr, "light: model %p tri %d bone %d lights %d amb %.1f %.1f %.1f\n", (void *)m, AT(m, 0x28, s32),
                    AT(m, 0x2C, s32), AT(D_0044E4C8, 0x10, s32), sLCol[3][0], sLCol[3][1], sLCol[3][2]);
            fprintf(stderr, "  col0 %.1f %.1f %.1f col1 %.1f %.1f %.1f col2 %.1f %.1f %.1f fall %.4f %.4f %.4f\n",
                    sLCol[0][0], sLCol[0][1], sLCol[0][2], sLCol[1][0], sLCol[1][1], sLCol[1][2], sLCol[2][0],
                    sLCol[2][1], sLCol[2][2], sLFall[0], sLFall[1], sLFall[2]);
        }
    }
}

/* a vertex colour for world position `p` and normal `n` (not unit) */
static u32 light_rgba(const f32 *n, const f32 *p) {
    f32 len2 = n[0] * n[0] + n[1] * n[1] + n[2] * n[2];
    f32 r = len2 > 0.0f ? 1.0f / __builtin_sqrtf(len2) : 0.0f;
    f32 c[3];
    s32 i, k;
    u32 out = 0x7F000000u;

    for (k = 0; k < 3; k++) {
        c[k] = sLCol[3][k];
    }
    for (i = 0; i < 3; i++) {
        f32 d = (sLDir[0][i] * n[0] + sLDir[1][i] * n[1] + sLDir[2][i] * n[2]) * r;
        f32 f = 1.0f + sLFall[i] * (sLDir[0][i] * p[0] + sLDir[1][i] * p[1] + sLDir[2][i] * p[2] + sLDir[3][i]);
        f32 l = (d > 0.0f ? d : 0.0f) * (f > 0.0f ? f : 0.0f);

        for (k = 0; k < 3; k++) {
            c[k] += sLCol[i][k] * l;
        }
    }
    for (k = 0; k < 3; k++) {
        s32 v = (s32)(c[k] < 128.0f ? c[k] : 128.0f);

        if (v < 0) {
            v = 0;
        }
        out |= (u32)v << (k * 8);
    }
    return out;
}

/* a normal (3 x s16 / 32768) turned by a matrix's 3 x 3, scaled by w, added to `acc` */
static void normal_add(f32 *acc, f32 (*m)[4], const s16 *n, f32 w) {
    f32 x = n[0] / 32768.0f, y = n[1] / 32768.0f, z = n[2] / 32768.0f;
    s32 k;

    for (k = 0; k < 3; k++) {
        acc[k] += w * (m[0][k] * x + m[1][k] * y + m[2][k] * z);
    }
}

static void mb_reserve(s32 n) {
    if (n > sMb.cap) {
        sMb.cap = n;
        sMb.xyzw = realloc(sMb.xyzw, n * 16);
        sMb.st = realloc(sMb.st, n * 8);
        sMb.rgba = realloc(sMb.rgba, n * 4);
    }
}

/* bone `b`'s skinning matrix */
static void bone_skin(u8 *m, s32 b, f32 (*out)[4]) {
    sceVu0MulMatrix(out, (f32 (*)[4])func_0017CE80(AT(m, 0x810, void *), b),
                    (f32 (*)[4])(AT(m, 0x4C0, u8 *) + 0x10 + b * 0x70 + 0x30));
}

/* the part's texture (.TEX entry): from the character's texture set (gBootMessage slot +0x24;
 * 0x24-byte slots from +0x4: the .TEX file at +0x8, its count at +0xC - the original only
 * needs the VRAM entry the upload went to, the PC decodes the entry itself) */
static void *model_tex(u8 *m, s32 tex) {
    u8 *slot = (u8 *)gBootMessage + 4 + AT(m, 0x24, u8) * 0x24;

    if (tex < 0 || AT(slot, 0x0, u8) == 0 || AT(slot, 0x8, u8 *) == NULL || (u32)tex >= AT(slot, 0xC, u32)) {
        return NULL;
    }
    return AT(slot, 0x8, u8 *) + 0x10 + tex * 0x10;
}

static void model_emit(u8 *m, const f32 *mvp, s32 n, s32 tex, s32 flags) {
    /* second-pass parts (flags bit 0) are cut out by their texture's alpha */
    glr_strip(mvp, n, sMb.xyzw, sMb.st, sMb.rgba, model_tex(m, tex), flags & 1 ? 1ULL << 34 : 0, 0x1C);
}

static void vtx_set(s32 i, const f32 *p, u16 u, u16 v, u32 noTri) {
    sMb.xyzw[i * 4] = p[0];
    sMb.xyzw[i * 4 + 1] = p[1];
    sMb.xyzw[i * 4 + 2] = p[2];
    AT(&sMb.xyzw[i * 4 + 3], 0, u32) = noTri ? 0x8000 : 0;
    sMb.st[i * 2] = u / 32768.0f;
    sMb.st[i * 2 + 1] = v / 32768.0f;
    AT(sMb.rgba, i * 4, u32) = 0x80808080;
}

static void gl_skinned_parts(u8 *m, const f32 *mvp) {
    u8 *r0 = AT(m, 0x4C0, u8 *);
    u8 *mt = r0 + AT(r0, 0x4, s32);
    f32 pal[32][4][4] __attribute__((aligned(16)));
    s32 i, k, j;

    for (i = 0; i < AT(mt, 0x0, s32); i++) {
        u8 *rec = mt + 0x10 + i * 0x30;
        s32 n = AT(rec, 0x0, s32), npal = AT(rec, 0x24, s32), infl = AT(rec, 0x2C, s32);
        const s32 *start = (const s32 *)(rec + AT(rec, 0x4, s32));
        const s16 *d = (const s16 *)(rec + AT(rec, 0x4, s32) + 0x10);
        const u16 *uv = (const u16 *)(rec + AT(rec, 0x8, s32));
        const s16 *nrm = (const s16 *)(rec + AT(rec, 0xC, s32));
        const u16 *w = (const u16 *)(rec + AT(rec, 0x10, s32));
        const u8 *bi = rec + AT(rec, 0x14, s32);
        const u8 *fl = rec + AT(rec, 0x18, s32);
        const u8 *pb = rec + AT(rec, 0x28, s32);
        s32 x = start[0], y = start[1], z = start[2];

        if (n <= 0 || npal > 32 || infl < 1 || infl > 4) {
            continue;
        }
        for (j = 0; j < npal; j++) {
            bone_skin(m, pb[j], pal[j]);
        }
        mb_reserve(n);
        for (k = 0; k < n; k++) {
            f32 v[4] = {0, 0, 0, 1}, o[3] = {0, 0, 0}, nn[3] = {0, 0, 0}, t[4], ws = 0.0f;

            x += d[k * 3];
            y += d[k * 3 + 1];
            z += d[k * 3 + 2];
            v[0] = x / 4096.0f;
            v[1] = y / 4096.0f;
            v[2] = z / 4096.0f;
            for (j = 0; j < infl; j++) {
                ws += w[k * infl + j];
            }
            for (j = 0; j < infl; j++) {
                f32 wt = ws == 0.0f ? (j == 0 ? 1.0f : 0.0f) : w[k * infl + j] / 32768.0f;
                s32 s = bi[k * infl + j] / 4;

                if (wt == 0.0f || s >= npal) {
                    continue;
                }
                sceVu0ApplyMatrix(t, pal[s], v);
                o[0] += t[0] * wt;
                o[1] += t[1] * wt;
                o[2] += t[2] * wt;
                normal_add(nn, pal[s], nrm + k * 3, wt);
            }
            vtx_set(k, o, uv[k * 2], uv[k * 2 + 1], fl[k] & 1);
            AT(sMb.rgba, k * 4, u32) = light_rgba(nn, o);
        }
        model_emit(m, mvp, n, AT(rec, 0x1C, s32), AT(rec, 0x20, s32));
    }
}

static void gl_rigid_parts(u8 *m, const f32 *mvp) {
    u8 *r0 = AT(m, 0x4C0, u8 *);
    u8 *rt = r0 + AT(r0, 0x8, s32);
    f32 b[4][4] __attribute__((aligned(16)));
    s32 i, k;

    if (AT(r0, 0x8, s32) == 0) {
        return;
    }
    for (i = 0; i < AT(rt, 0x0, s32); i++) {
        u8 *rec = rt + 0x10 + i * 0x20;
        s32 n = AT(rec, 0x0, s32);
        const s32 *start = (const s32 *)(rec + AT(rec, 0x4, s32));
        const s16 *d = (const s16 *)(rec + AT(rec, 0x4, s32) + 0x10);
        const u16 *uv = (const u16 *)(rec + AT(rec, 0x8, s32));
        const s16 *nrm = (const s16 *)(rec + AT(rec, 0xC, s32));
        const u8 *fl = rec + AT(rec, 0x10, s32);
        s32 x = start[0], y = start[1], z = start[2];

        if (n <= 0) {
            continue;
        }
        bone_skin(m, AT(rec, 0x1C, s32), b);
        mb_reserve(n);
        for (k = 0; k < n; k++) {
            f32 v[4], t[4];

            x += d[k * 3];
            y += d[k * 3 + 1];
            z += d[k * 3 + 2];
            v[0] = x / 4096.0f;
            v[1] = y / 4096.0f;
            v[2] = z / 4096.0f;
            v[3] = 1.0f;
            sceVu0ApplyMatrix(t, b, v);
            vtx_set(k, t, uv[k * 2], uv[k * 2 + 1], fl[k] & 1);
            {
                f32 nn[3] = {0, 0, 0};

                normal_add(nn, b, nrm + k * 3, 1.0f);
                AT(sMb.rgba, k * 4, u32) = light_rgba(nn, t);
            }
        }
        model_emit(m, mvp, n, AT(rec, 0x14, s32), 0);
    }
}

/* a morph shape's position and normal (shape table entry: offsets from it), into acc by w */
static void morph_add(u8 *rec, s32 shape, s32 k, f32 w, f32 *pos, f32 *nrm) {
    u8 *e = rec + AT(rec, 0x10, s32) + shape * 8;
    const s16 *p = (const s16 *)(e + AT(e, 0x0, s32)) + k * 3;
    const s16 *n = (const s16 *)(e + AT(e, 0x4, s32)) + k * 3;
    s32 j;

    for (j = 0; j < 3; j++) {
        pos[j] += w * (f32)p[j];
        nrm[j] += w * (f32)n[j];
    }
}

/* morphing parts (resource 1, as func_001BD650 picks them): the face (kind 1) blends the rest
 * shape with shapes 1 (mouth), 7 and 8 (eyes) by the model's weights (+0x58 + 4 x shape; the
 * rest gets what is left); the hands (kind 0) blend two shapes - the second part by +0x38 /
 * +0x3C / +0x40 (shapes, weight), the others by +0x48 / +0x4C / +0x50 */
static void gl_morph_parts(u8 *m, const f32 *mvp) {
    u8 *r1 = AT(m, 0x4D0, u8 *);
    f32 b[4][4] __attribute__((aligned(16)));
    s32 i, k;

    if (r1 == NULL) {
        return;
    }
    for (i = 0; i < AT(r1, 0x0, s32); i++) {
        u8 *rec = r1 + 0x10 + i * 0x40;
        s32 n = AT(rec, 0x4, s32), nshape = AT(rec, 0x0, s32);
        const u16 *uv = (const u16 *)(rec + AT(rec, 0x8, s32));
        const u32 *fl = (const u32 *)(rec + AT(rec, 0xC, s32));
        const s32 *base = (const s32 *)(rec + 0x30);
        s32 sh[4], ns = 0;
        f32 w[4];

        if (n <= 0) {
            continue;
        }
        if (AT(rec, 0x1C, s32) != 0) {
            const f32 *fw = (const f32 *)(m + 0x58);

            sh[0] = 0; sh[1] = 1; sh[2] = 7; sh[3] = 8;
            w[1] = fw[1];
            w[2] = fw[7];
            w[3] = fw[8];
            w[0] = 1.0f - w[1] - w[2] - w[3];
            ns = nshape > 8 ? 4 : 1;
        } else {
            u8 *h = m + (i == 1 ? 0x38 : 0x48);

            sh[0] = AT(h, 0x0, s32);
            sh[1] = AT(h, 0x4, s32);
            w[1] = AT(h, 0x8, f32);
            w[0] = 1.0f - w[1];
            ns = sh[0] >= 0 && sh[0] < nshape && sh[1] >= 0 && sh[1] < nshape ? 2 : 0;
            if (ns == 0) {
                sh[0] = 0;
                w[0] = 1.0f;
                ns = 1;
            }
        }
        if (ns == 1) {
            w[0] = 1.0f;
        }
        bone_skin(m, AT(rec, 0x14, s32), b);
        mb_reserve(n);
        for (k = 0; k < n; k++) {
            f32 v[4], t[4], p[3] = {0, 0, 0}, nn[3] = {0, 0, 0}, wn[3] = {0, 0, 0};
            s32 j;

            for (j = 0; j < ns; j++) {
                morph_add(rec, sh[j], k, w[j], p, nn);
            }
            v[0] = (base[0] + p[0]) / 4096.0f;
            v[1] = (base[1] + p[1]) / 4096.0f;
            v[2] = (base[2] + p[2]) / 4096.0f;
            v[3] = 1.0f;
            sceVu0ApplyMatrix(t, b, v);
            vtx_set(k, t, uv[k * 2], uv[k * 2 + 1], fl[k] & 0x8000);
            for (j = 0; j < 3; j++) {
                wn[j] = b[0][j] * nn[0] + b[1][j] * nn[1] + b[2][j] * nn[2];
            }
            AT(sMb.rgba, k * 4, u32) = light_rgba(wn, t);
        }
        model_emit(m, mvp, n, AT(rec, 0x18, s32), AT(rec, 0x20, s32));
    }
}

/* PC stand-in for the shadow volumes (resource 2, cast from the room's lights - TODO): a soft
 * dark disc on the floor under the root bone, at the character's floor point (+0x800) */
static void gl_blob_shadow(u8 *m, const f32 *mvp) {
    enum { SEG = 16 };
    static f32 xyzw[SEG * 3 * 4], st[SEG * 3 * 2];
    static u8 rgba[SEG * 3 * 4];
    const f32 *root = func_0017CE80(AT(m, 0x810, void *), 0) + 12;
    f32 r = 3.0f, y = AT(m, 0x800 + 4, f32) + 1.2f;   /* above the floor point (world y up; the floor mesh is a little above the walk mesh) */
    s32 i, k;


    for (i = 0; i < SEG; i++) {
        f32 a0 = (f32)i * (6.2831853f / SEG), a1 = (f32)(i + 1) * (6.2831853f / SEG);
        f32 px[3] = {root[0], root[0] + r * __builtin_cosf(a0), root[0] + r * __builtin_cosf(a1)};
        f32 pz[3] = {root[2], root[2] + r * __builtin_sinf(a0), root[2] + r * __builtin_sinf(a1)};

        for (k = 0; k < 3; k++) {
            s32 v = i * 3 + k;

            xyzw[v * 4] = px[k];
            xyzw[v * 4 + 1] = y;
            xyzw[v * 4 + 2] = pz[k];
            AT(&xyzw[v * 4 + 3], 0, u32) = k < 2 ? 0x8000 : 0;   /* one triangle per three */
            st[v * 2] = 0.0f;
            st[v * 2 + 1] = 0.0f;
            AT(rgba, v * 4, u32) = k == 0 ? 0x60000000u : 0x00000000u;   /* centre dark, edge clear */
        }
    }
    glr_strip(mvp, SEG * 3, xyzw, st, rgba, NULL, 0, 0x4C);
}

static void gl_draw_model(u8 *m) {
    f32 clip[4][4] __attribute__((aligned(16)));

    if (AT(m, 0x4C0, u8 *) == NULL || AT(m, 0x810, u8 *) == NULL) {
        return;
    }
    VCALL(D_0044E4B8, 0x48, void (*)(VObject *, f32 (*)[4]))(D_0044E4B8, clip);
    light_setup(m);
    gl_skinned_parts(m, &clip[0][0]);
    gl_rigid_parts(m, &clip[0][0]);
    gl_morph_parts(m, &clip[0][0]);
}
#endif

/* queue the model for drawing in `layer` (a, b: the character's draw parameters, kept at
 * +0x28 / +0x2C; layer 0x14 none): the model drawer (+0x10) at its root bone; layer 0xB draws
 * it twice (masks in layers 10 / 11 around it, mode +0x34 4 then 8); then its shadow (+0x1D0)
 * unless +0x4D9 or in layers 0x14 / 0x17 / 0x1C */
void func_001F6870(u8 *m, s32 layer, s32 a, s32 b, f32 *light) {
    VObject *r;
    u64 *p;

    if (layer == 0x14) {
        AT(m, 0x28, s32) = -1;
        AT(m, 0x2C, s32) = -1;
    } else {
        AT(m, 0x28, s32) = a;
        AT(m, 0x2C, s32) = b;
    }
#ifdef HG_NATIVE
    gl_draw_model(m);
    if (AT(m, 0x4D9, u8) == 0 && layer != 0x17 && layer != 0x14 && layer != 0x1C) {
        f32 clip[4][4] __attribute__((aligned(16)));

        VCALL(D_0044E4B8, 0x48, void (*)(VObject *, f32 (*)[4]))(D_0044E4B8, clip);
        gl_blob_shadow(m, &clip[0][0]);
    }
    return;
#endif
    if ((u16)layer == 0xB) {
        r = D_0044E4F0;
        AT(m, 0x34, u32) = (layer & 0xFFFF0000) | 4;
        VCALL(r, 0xC, void (*)(VObject *, void *, s32, f32 *))(r, m + 0x10, 0xA,
                                                              func_0017CE80(AT(m, 0x810, void *), 0) + 12);
        p = VCALL(r, 0x10, u64 *(*)(VObject *, s32, s32))(r, 3, 0xB);
        p[0] = 0x10000002;
        AT(p, 0x8, u32) = 0;
        AT(p, 0xC, u32) = 0x50000002;
        p[2] = 0x8001 | (0x10000000ULL << 32);
        p[3] = 0xE;
        p[4] = 0x310000A0 | (1ULL << 32);
        p[5] = 0x4E;
        VCALL(D_0044E4E8, 0x18, void (*)(VObject *))(D_0044E4E8);
        VCALL(gBootMessage, 0x20, void (*)(VObject *))(gBootMessage);
        AT(m, 0x34, u32) = 8;
        VCALL(r, 0xC, void (*)(VObject *, void *, s32, f32 *))(r, m + 0x10, 0xB, NULL);
        p = VCALL(r, 0x10, u64 *(*)(VObject *, s32, s32))(r, 4, 0xB);
        p[0] = 0x10000003;
        AT(p, 0x8, u32) = 0;
        AT(p, 0xC, u32) = 0x50000003;
        p[2] = 0x8002 | (0x10000000ULL << 32);
        p[3] = 0xE;
        p[4] = 0x310000A0;
        p[5] = 0x4E;
        p[6] = 0x5000F;
        p[7] = 0x47;
        AT(m, 0x34, u32) = 0;
    } else {
        r = D_0044E4F0;
        VCALL(r, 0xC, void (*)(VObject *, void *, s32, f32 *))(r, m + 0x10, layer,
                                                              func_0017CE80(AT(m, 0x810, void *), 0) + 12);
    }
    if (AT(m, 0x4D9, u8) == 0 && layer != 0x17 && layer != 0x14 && layer != 0x1C) {
        func_001F3530(m + 0x1D0, a, b, light, layer);
    }
}


/* `m` = a turn of `a` about Y */
void func_002E3190(f32 (*m)[4], f32 a) {
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixY(m, m, a);
}



/* reset the play state before a new animation (as func_002DDE20): +0x85C / +0x85D off, the
 * speeds +0x38 / +0x3C and +0x48 / +0x4C from the defaults +0x87C / +0x880, +0x40 / +0x50 zero */
static void motion_reset_play(u8 *m) {
    motion_defaults(m);
}

/* play animation `anim` (variant `variant`) blending in over the frames the table (+0x874)
 * gives at +0, with its flags (+4); an animation not in the table cuts in at once */
void func_002DDED0(u8 *m, s32 anim, s32 variant) {
    s32 i;
    f32 blend;
    u32 flags;

    if (func_001F4710(m, anim) == -1) {
        motion_reset_play(m);
        func_001F7890(m, anim, 0, -1, 0.0f);
        return;
    }
    i = func_001F4710(m, anim);
    blend = (f32)(i != -1 ? AT(AT(m, 0x874, u8 *), i * 6, s16) : 0);
    i = func_001F4710(m, anim);
    flags = i != -1 ? AT(AT(m, 0x874, u8 *), i * 6 + 4, u16) : 0;
    motion_reset_play(m);
    func_001F7890(m, anim, flags & 0xFFFF, variant, blend);
}

/* play animation `anim` driven from outside: both motion layers' flag 0x10 (no own time) */
void func_002DD040(u8 *m, s32 anim) {
    func_002DDED0(m, anim, 0);
    AT(AT(m, 0x6A4, u8 *), 0x18, u32) |= 0x10;
    AT(AT(m, 0x6A8, u8 *), 0x18, u32) |= 0x10;
}

extern VObject *D_0044FE10;   /* the cutscene director */

/* the motion's time (+0x6A4 +0) = the director's frame (+0x20), shared by the three blend
   channels' time pointers (+0x704, 0x60 apart) */
void func_002DD090(u8 *m) {
    s32 i;

    *AT(m, 0x6A4, f32 *) = (f32)VCALL(D_0044FE10, 0x20, s32 (*)(VObject *))(D_0044FE10);
    for (i = 0; i < 3; i++) {
        f32 *t = AT(m, 0x704 + i * 0x60, f32 *);

        if (t != NULL) {
            *t = *AT(m, 0x6A4, f32 *);
        }
    }
}


/* is foot `foot` down (the contact track +0x50, channel `foot`) `ofs` frames from now in the
 * previous slot's animation (+0x6A8; the time wrapped into it) */
u32 func_002DD860(void *motion, s32 foot, f32 ofs) {
    f32 c[4] __attribute__((aligned(16)));
    u8 *slot = AT(motion, 0x6A8, u8 *);
    f32 t, len;

    c[3] = 0.0f;
    c[2] = 0.0f;
    c[1] = 0.0f;
    c[0] = 0.0f;
    if (AT(slot, 0x20, void *) == NULL) {
        return 0;
    }
    t = AT(slot, 0x0, f32) + ofs;
    len = (f32)AT(AT(AT(slot, 0x20, u8 *), 0x4, u8 *), 0xC, s32);
    while (t < 0.0f) {
        t = t + len;
    }
    while (!(t < len)) {
        t = t - len;
    }
    if (AT(slot, 0x50, void *) != NULL && AT(AT(slot, 0x50, u8 *), 0x0, void *) != NULL) {
        func_001F36B0(AT(slot, 0x50, void *), c, t);
    }
    return !(c[foot] <= 0.0f);
}

/* ---- the stalkers' and event characters' models (built by the loaders func_0016F420 ..
 * func_00170FB0 for func_00171160) ---- */

extern void *D_0046B1C0[], *D_0046B0D0[], *D_004703B0[];
extern void func_00100490(void *p);   /* operator delete */
extern void func_002DC6D0(void *p);   /* model delete */

/* the human characters' model base before its kind (vtable D_0046B210): drawing object, the
   +0x1D0 part, the model fields */
void *func_0016FCD0(u8 *m) {
    AT(m, 0x0, void **) = D_0046B210;
    DrawObj_Init(m + 0x10);
    AT(m, 0x1D0, void **) = D_00469D00;
    AT(m, 0x1D4, s32) = -1;
    AT(m, 0x1D0, void **) = D_0046B1C0;
    AT(m, 0x4B0, s32) = 0;
    ModelBase_Zero(m);
    return m;
}

/* the human model base with its two 0x60 parts' vtables (+0x928 / +0x988) */
void *func_0016FC30(u8 *m) {
    func_0016FCD0(m);
    AT(m, 0x0, void **) = D_0046C160;
    AT(m, 0x928, void **) = D_0046B0D0;
    AT(m, 0x988, void **) = D_0046B0D0;
    return m;
}

/* its destructor */
void *func_0016F9E0(u8 *m, s32 flags) {
    return HumanModel_Destroy(m, flags);
}

/* destructors of the model's parts and array elements: the element's vtable (at +0 or +0x30)
   back to its base, then (flags > 0) delete */
static inline void *Part_Destroy(u8 *e, s32 at, void **vt, void **base, s32 flags) {
    if (e != NULL) {
        AT(e, at, void **) = vt;
        AT(e, at, void **) = base;
        if ((s16)flags > 0) {
            func_00100490(e);
        }
    }
    return e;
}

void *func_0016F680(void *e, s32 flags) {
    return Part_Destroy(e, 0x0, D_0046B1C0, D_00469D00, flags);
}

void *func_0016F6E0(void *e, s32 flags) {
    return Part_Destroy(e, 0x0, D_0046ADA0, D_00469D00, flags);
}

void *func_0016F990(void *e, s32 flags) {
    if (e != NULL && (s16)flags > 0) {
        func_00100490(e);
    }
    return e;
}

void *func_0016FC80(void *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x58, void **) = D_0046B0D0;
        if ((s16)flags > 0) {
            func_00100490(e);
        }
    }
    return e;
}

/* the three-bone IK solver's destructor */
void *func_001F7E40(void *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x58, void **) = D_0046B0C0;
        AT(e, 0x58, void **) = D_0046B0D0;
        if ((s16)flags > 0) {
            func_00100490(e);
        }
    }
    return e;
}

extern void *D_004702B0[], *D_00473810[], *D_00470700[], *D_00470440[], *D_004703D0[],
    *D_00472350[], *D_004737F0[], *D_00470460[], *D_00472C10[];

void *func_0016FBB0(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, D_004702B0, D_004703B0, flags);
}

void *func_0016FB00(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, D_00473810, D_004703B0, flags);
}

void *func_00170080(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, D_00470700, D_004703B0, flags);
}

void *func_00170290(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, D_00470440, D_004703B0, flags);
}

void *func_001702F0(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, D_004703D0, D_004703B0, flags);
}

void *func_001709D0(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, D_00472350, D_004703B0, flags);
}

void *func_00170CB0(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, D_004737F0, D_004703B0, flags);
}

void *func_00170EB0(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, D_00470460, D_004703B0, flags);
}

void *func_00170F30(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, D_00472C10, D_004703B0, flags);
}

/* the base model's destructor (vtable D_0046B210 down to its parts) */
void *func_0016F5D0(void *p, s32 flags) {
    u8 *m = p;

    if (m != NULL) {
        AT(m, 0x0, void **) = D_0046B210;
        AT(m, 0x1D0, void **) = D_0046B1C0;
        AT(m, 0x1D0, void **) = D_00469D00;
        AT(m, 0x10, void **) = D_0046ADA0;
        AT(m, 0x10, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(m);
        }
    }
    return m;
}

/* the human-with-kind model's destructor (vtable D_0046B0E0, then the human base) */
void *func_00170350(void *p, s32 flags) {
    u8 *m = p;

    if (m != NULL) {
        AT(m, 0x0, void **) = D_0046B0E0;
        HumanModel_Destroy(m, flags);
    }
    return m;
}

extern void *D_00470540[], *D_004703A0[];
extern void *func_00170460(void *);
extern void *func_00170080(void *, s32);
extern void func_001002C0(void *array, void *(*dtor)(void *, s32), u32 size, u32 n);   /* __destroy_arr */

/* a human event character's model (vtable D_00470540, 0x1270 bytes) of `kind`: twelve 0x50
   parts at +0x9B0, four at +0xE40, six 0x40 parts at +0xF80 and the single parts between */
void *func_001700E0(u8 *m, s32 kind) {
    u8 *e;

    func_0016FC30(m);
    AT(m, 0x0, void **) = D_0046B0E0;
    AT(m, 0x9A0, u8) = kind;
    AT(m, 0x0, void **) = D_00470540;
    func_00100340(m + 0x9B0, func_00170460, func_00170080, 0x50, 0xC);
    AT(m, 0xDA4, s32) = 0;
    AT(m, 0xDA0, s32) = 0;
    AT(m, 0xDE0, void **) = D_004703D0;
    AT(m, 0xE34, s32) = 0;
    AT(m, 0xE30, s32) = 0;
    func_00100340(m + 0xE40, func_0016FC10, func_0016FBB0, 0x50, 4);
    for (e = m + 0xF80; e < m + 0x1100; e += 0x40) {
        AT(e, 0x30, void **) = D_004703A0;
    }
    AT(m, 0x1134, s32) = 0;
    AT(m, 0x1130, s32) = 0;
    AT(m, 0x1170, void **) = D_00470440;
    AT(m, 0x11D4, s32) = 0;
    AT(m, 0x11D0, s32) = 0;
    AT(m, 0x1210, void **) = D_004703D0;
    AT(m, 0x1264, s32) = 0;
    AT(m, 0x1260, s32) = 0;
    return m;
}

/* its destructor */
void *func_0016FF50(void *p, s32 flags) {
    u8 *m = p;

    if (m != NULL) {
        AT(m, 0x0, void **) = D_00470540;
        AT(m, 0x1210, void **) = D_004703D0;
        AT(m, 0x1210, void **) = D_004703B0;
        AT(m, 0x1170, void **) = D_00470440;
        AT(m, 0x1170, void **) = D_004703B0;
        func_001002C0(m + 0xE40, func_0016FBB0, 0x50, 4);
        AT(m, 0xDE0, void **) = D_004703D0;
        AT(m, 0xDE0, void **) = D_004703B0;
        func_001002C0(m + 0x9B0, func_00170080, 0x50, 0xC);
        AT(m, 0x0, void **) = D_0046B0E0;
        func_0016F9E0(m, 0);
        if ((s16)flags > 0) {
            func_002DC6D0(m);
        }
    }
    return m;
}

/* ---- the model loaders: a model of the kind's size from the scene heap (+0x6FBF00), built,
   and put at character `slot` +0xF0 (NULL when the heap is full) ---- */

static inline u8 *Model_New(Progress *p, u32 size) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);

    return func_002DC6E0(size, VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, size));
}

extern void *D_00474460[], *D_00473BD0[], *D_00476F50[], *D_0046F9E0[], *D_00479740[],
    *D_00472700[], *D_00471CE0[], *D_00471DA0[], *D_00471C20[], *D_00470480[], *D_004702D0[],
    *D_0046C0A0[];
extern void *func_0016FB60(void *);
extern void *func_0016FAE0(void *);
extern void *func_00170A30(void *);
extern void *func_00170D10(void *);
extern void *func_00170F90(void *);
extern void *func_00170F10(void *);

/* kind 33 */
void func_0016F420(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x890);

    if (m != NULL) {
        func_0016F4B0(m);
        AT(m, 0x0, void **) = D_00474460;
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kinds 23 / 37 */
void func_0016F860(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x1310);

    if (m != NULL) {
        u8 *e;

        func_0016FC30(m);
        AT(m, 0x0, void **) = D_00473BD0;
        func_00100340(m + 0x9A0, func_0016FC10, func_0016FBB0, 0x50, 4);
        for (e = m + 0xAE0; e < m + 0xBE0; e += 0x40) {
            func_0016FB90(e);
        }
        func_0016FB80(m + 0xBE0);
        func_00100340(m + 0xC20, func_0016FB60, func_0016FB00, 0x60, 0xC);
        func_0016FB80(m + 0x10A0);
        for (e = m + 0x10E0; e < m + 0x1310; e += 0x70) {
            func_0016FAE0(e);
        }
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kind 18 */
void func_0016FEC0(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x1270);

    if (m != NULL) {
        func_001700E0(m, 5);
        AT(m, 0x0, void **) = D_00476F50;
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kinds 14 / 15 */
void func_00170480(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x890);

    if (m != NULL) {
        func_0016F4B0(m);
        AT(m, 0x0, void **) = D_00472700;
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kind 13 */
void func_00170510(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x1270);

    if (m != NULL) {
        u8 *e;

        func_00170690(m, 0);
        AT(m, 0x0, void **) = D_00470540;
        func_00100340(m + 0x9B0, func_00170460, func_00170080, 0x50, 0xC);
        func_0016FB80(m + 0xD70);
        func_00170670(m + 0xDB0);
        func_0016FB80(m + 0xE00);
        func_00100340(m + 0xE40, func_0016FC10, func_0016FBB0, 0x50, 4);
        for (e = m + 0xF80; e < m + 0x1100; e += 0x40) {
            func_0016FB90(e);
        }
        func_0016FB80(m + 0x1100);
        func_00170650(m + 0x1140);
        func_0016FB80(m + 0x11A0);
        func_00170670(m + 0x11E0);
        func_0016FB80(m + 0x1230);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* the plain model (vtable D_0046F9E0): most event characters */
void func_00170710(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x890);

    if (m != NULL) {
        func_0016FCD0(m);
        AT(m, 0x0, void **) = D_0046F9E0;
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kind 12 */
void func_001707A0(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x9A0);

    if (m != NULL) {
        func_0016FC30(m);
        AT(m, 0x0, void **) = D_00479740;
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kind 11 (Lorenzo) */
void func_00170830(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0xD00);

    if (m != NULL) {
        u8 *e;

        func_0016F4B0(m);
        AT(m, 0x0, void **) = D_00471DA0;
        func_00100340(m + 0x890, func_0016FC10, func_0016FBB0, 0x50, 6);
        for (e = m + 0xA70; e < m + 0xC70; e += 0x40) {
            func_0016FB90(e);
        }
        func_0016FB80(m + 0xCC0);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kinds 10 / 39 (the second Lorenzo) */
void func_00170910(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x1160);

    if (m != NULL) {
        func_0016FC30(m);
        AT(m, 0x0, void **) = D_00471CE0;
        func_00100340(m + 0x9A0, func_00170A30, func_001709D0, 0x50, 0x18);
        func_0016FB80(m + 0x1120);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kind 9 */
void func_00170A50(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x1500);

    if (m != NULL) {
        u8 *e;

        func_0016FC30(m);
        AT(m, 0x0, void **) = D_00471C20;
        func_00100340(m + 0x9A0, func_0016FC10, func_0016FBB0, 0x50, 6);
        for (e = m + 0xB80; e < m + 0xD00; e += 0x40) {
            func_0016FB90(e);
        }
        func_0016FB80(m + 0xD00);
        func_00100340(m + 0xD40, func_00170A30, func_001709D0, 0x50, 0x18);
        func_0016FB80(m + 0x14C0);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kind 4 (Riccardo) */
void func_00170B60(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x1490);

    if (m != NULL) {
        u8 *e;

        func_0016FC30(m);
        AT(m, 0x0, void **) = D_00470480;
        func_00100340(m + 0x9A0, func_0016FC10, func_0016FBB0, 0x50, 4);
        for (e = m + 0xAE0; e < m + 0xBE0; e += 0x40) {
            func_0016FB90(e);
        }
        func_0016FB80(m + 0xBE0);
        func_00100340(m + 0xC20, func_0016FB60, func_0016FB00, 0x60, 0xC);
        func_0016FB80(m + 0x10A0);
        for (e = m + 0x10E0; e < m + 0x1310; e += 0x70) {
            func_0016FAE0(e);
        }
        func_00100340(m + 0x1310, func_00170D10, func_00170CB0, 0x50, 4);
        func_0016FB80(m + 0x1450);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kinds 3 / 34..36 (Daniella) */
void func_00170D30(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x1580);

    if (m != NULL) {
        u8 *e;

        func_0016FC30(m);
        AT(m, 0x0, void **) = D_004702D0;
        func_0016FB80(m + 0x9A0);
        func_0016FB80(m + 0x9E0);
        func_0016FB80(m + 0xA20);
        func_0016FB80(m + 0xA60);
        func_00100340(m + 0xAA0, func_00170F90, func_00170F30, 0x50, 6);
        for (e = m + 0xC80; e < m + 0xD60; e += 0x70) {
            func_0016FAE0(e);
        }
        for (e = m + 0xD60; e < m + 0xDE0; e += 0x40) {
            func_0016FB90(e);
        }
        func_00100340(m + 0xDE0, func_00170F10, func_00170EB0, 0x70, 0xA);
        for (e = m + 0x1240; e < m + 0x1470; e += 0x70) {
            func_0016FAE0(e);
        }
        func_00170650(m + 0x1470);
        func_00100340(m + 0x14D0, func_00170670, func_001702F0, 0x50, 2);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kinds 2 / 6 / 7 / 27 (the Debilitas kind) */
void func_00170FB0(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0xBA0);

    if (m != NULL) {
        u8 *e;

        func_0016FC30(m);
        AT(m, 0x0, void **) = D_0046C0A0;
        func_00100340(m + 0x9A0, func_0016FC10, func_0016FBB0, 0x50, 4);
        func_0016FB80(m + 0xAE0);
        for (e = m + 0xB20; e < m + 0xBA0; e += 0x40) {
            func_0016FB90(e);
        }
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}
