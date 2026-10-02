/* Hewie: the partner dog (vtable 0x46A120). */
#include "common.h"
#include "hewie.h"
#include "progress.h"

extern VObject *gBootMessage;      /* message display, also used in game */
extern Progress *gProgress;

#define MOTION_U8(m, off) (*((u8 *)(m) + (off)))

extern void *D_0046A120[];   /* Hewie vtable */
extern void *D_00469C60[];   /* Character vtable */
extern void *D_00469C20[];   /* Actor base vtable */
extern void func_00124E40(Actor *a);

/* vtable +0x8: destructor (nothing to free: he lives inside the scene). */
Hewie *func_00130A70(Hewie *h, s32 flags) {
    if (h != NULL) {
        h->c.a.vtbl = D_0046A120;
        h->c.a.vtbl = D_00469C60;
        h->c.a.vtbl = D_00469C20;
        if ((s16)flags > 0) {
            func_00124E40(&h->c.a);
        }
    }
    return h;
}

/* vtable +0x10 */
void func_00168A00(Hewie *h) {
}

extern void func_00125D40(Character *c);

/* vtable +0x58: deactivate (Character part only). */
void func_00166140(Hewie *h) {
    func_00125D40(&h->c);
}

/* vtable +0x98 */
s32 func_00130AE0(Hewie *h) {
    return 1;
}

extern void func_00126910(Character *c);
extern void func_00126810(Character *c);

/* vtable +0x4C: disable (as Character), animation paused. */
void func_00165CD0(Hewie *h) {
    func_00126910(&h->c);
    MOTION_U8(h->c.motion, 0x4D8) = 1;
}

extern void func_00125BE0(Character *c);
extern void func_00130AF0(Hewie *h, s32 action, s32 arg);
extern s32 func_0013B2C0(Hewie *h, s32 arg);

/* vtable +0x60: forget path/movement state, then his default action. */
void func_00165D00(Hewie *h) {
    func_00125BE0(&h->c);
    func_00130AF0(h, 0, 0);
}

extern void func_00127650(Character *c);

/* vtable +0x24: save the previous frame's state. */
void func_00168360(Hewie *h) {
    func_00127650(&h->c);
    HW(h, 0xF354C, f32) = h->c.a.angle[1];
    HW(h, 0xF3568, s32) = HW(h, 0xF3564, s32);
    HW(h, 0xF366C, u8) = HW(h, 0xF366D, u8);
}

/* vtable +0x20: release his message slot and stop his animation player, if set up. */
void func_00168680(Hewie *h) {
    if (h->c.a.unkD0) {
        VCALL(gBootMessage, 0xC, void (*)(VObject *, u32))(gBootMessage, h->c.msgSlot);
        h->c.a.unkD0 = 0;
    }
    if (h->c.a.unkD1) {
        VCALL(h->c.motion, 0x10, void (*)(void *))(h->c.motion);
        h->c.a.unkD1 = 0;
    }
}

/* vtable +0x2C: room setup done - (outside the special mode, a pending +0x80 call), then put
 * his animation player on his triangle. */
void func_00168600(Hewie *h) {
    if (*((u8 *)gProgress + 0x1FBEC1) == 0 && h->c.unkE4 == 1) {
        VCALL(h, 0x80, void (*)(Hewie *))(h);
    }
    VCALL(h->c.motion, 0x38, void (*)(void *, s32, u32, s32))(h->c.motion, h->c.unk152C, h->c.a.navTri, 0x1D);
}

/* vtable +0x50: halt (as Character), animation running, then back to his default action. */
void func_00165C50(Hewie *h) {
    func_00126810(&h->c);
    MOTION_U8(h->c.motion, 0x4D8) = 0;
    VCALL(h->c.motion, 0x50, void (*)(void *, Hewie *))(h->c.motion, h);
    func_00130AF0(h, func_0013B2C0(h, 0), 0);
}
