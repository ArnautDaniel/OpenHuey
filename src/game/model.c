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
