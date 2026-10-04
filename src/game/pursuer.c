/* Pursuer: the stalkers' shared base class, code 0x278490..0x29FF10. See include/pursuer.h. */
#include "common.h"
#include "pursuer.h"

extern Character *gCharPartner;   /* Hewie */
extern VObject *D_0044E560;       /* sound driver */

/* nothing */
void func_00283EE0(Pursuer *p) {
}

extern void func_00125960(Character *c);

/* a character's step (base) */
void func_002992F0(Pursuer *p) {
    func_00125960(&p->c);
}

/* chasing Hewie? */
s32 func_0029A850(Pursuer *p) {
    return p->target == gCharPartner;
}

/* stop the pursuer's sounds (sound driver +0x10) */
void func_0029D3E0(Pursuer *p) {
    VCALL(D_0044E560, 0x10, void (*)(VObject *, s32, s32))(D_0044E560, 0, 0x400002);
}

/* clear a 3-word vector (third argument) */
void func_0029CE40(Pursuer *p, s32 a1, s32 *out) {
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
}

extern void func_00125D40(Character *c);

/* vtable +0x58: deactivate (Character part) */
void func_0029E600(Pursuer *p) {
    func_00125D40(&p->c);
}

extern void func_00127650(Character *c);

/* vtable +0x24 */
void func_0029EAA0(Pursuer *p) {
    func_00127650(&p->c);
}

/* vtable +0x10: nothing */
void func_0029EE00(Pursuer *p) {
}

/* vtable +0x1C4 */
void func_0028ED20(Pursuer *p) {
    PU(p, 0x16EE, u8) = 1;
    PU(p, 0x16F0, u8) = 1;
    p->c.unk100 = -1;
    p->c.unk104[0] = -1;
    p->c.a.unk2B = 0;
    p->c.a.unk2A = 0;
}
