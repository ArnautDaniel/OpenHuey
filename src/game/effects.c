/* The room's effects (made by SceneGame +0xF6CD30 from PAC section 13; 0xA0-byte objects):
 *   D_0046D750 a screen tint, D_0046D7B0 a screen blend, D_0046EB40 the fog (also sets the
 *   camera's depth range), D_0046EC60 a depth range.
 * Vtable: +0xC reset, +0x18 set from the room data (unaligned little-endian words). */
#include "common.h"
#include "game.h"

extern VObject *D_0044E4B8;   /* the camera */

static u32 rd32(const u8 *p) {
    return p[0] | (p[1] << 8) | (p[2] << 16) | ((u32)p[3] << 24);
}

/* tint +0xC */
void func_002674F0(u8 *e) {
    AT(e, 0x10, s32) = 0;
    AT(e, 0x14, s32) = 0;
    AT(e, 0x18, s32) = 0;
}

/* tint +0x18: two colours and a mode byte */
void func_00267370(u8 *e, const u8 *d) {
    if (d == NULL) {
        return;
    }
    AT(e, 0x10, u32) = rd32(d);
    AT(e, 0x14, u32) = rd32(d + 4);
    AT(e, 0x18, u32) = d[8];
}

/* blend +0xC */
void func_0026B480(u8 *e) {
    AT(e, 0x10, u32) = 0x80004080;
    AT(e, 0x14, s32) = 1;
    AT(e, 0x30, s32) = 0;
}

/* blend +0x18: colour and mode (mode 2 and 3/4 use fixed colours) */
void func_0026B240(u8 *e, const u8 *d) {
    if (d == NULL) {
        return;
    }
    AT(e, 0x10, u32) = rd32(d);
    AT(e, 0x14, u32) = d[4];
    if (AT(e, 0x14, u32) == 2) {
        AT(e, 0x10, u32) = 0x80004080;
    } else if (AT(e, 0x14, u32) - 3 < 2) {
        AT(e, 0x10, u32) = 0x40404040;
    }
}

/* fog +0xC */
void func_002BB390(u8 *e) {
    s32 i;

    AT(e, 0x10, u32) = 0x808080;
    AT(e, 0x14, u32) = 0xC0808080;
    AT(e, 0x50, f32) = 20.0f;
    AT(e, 0x54, f32) = 1000.0f;
    for (i = 0; i < 7; i++) {
        AT(e, 0x1C + i * 4, s32) = 0;
    }
}

/* fog +0x18: two colours, the depth range (also the camera's), a colour and 6 bytes */
void func_002BB030(u8 *e, const u8 *d) {
    VObject *cam;
    s32 i;

    if (d == NULL) {
        return;
    }
    cam = D_0044E4B8;
    AT(e, 0x10, u32) = rd32(d);
    AT(e, 0x14, u32) = rd32(d + 4);
    AT(e, 0x18, u32) = rd32(d + 8);
    AT(e, 0x50, f32) = AT(e, 0x18, f32);
    AT(e, 0x18, u32) = rd32(d + 12);
    AT(e, 0x54, f32) = AT(e, 0x18, f32);
    VCALL(cam, 0xC0, void (*)(VObject *, f32, f32))(cam, AT(e, 0x50, f32), AT(e, 0x54, f32));
    AT(e, 0x1C, u32) = rd32(d + 16);
    for (i = 0; i < 6; i++) {
        AT(e, 0x20 + i * 4, u32) = d[20 + i];
    }
}

/* depth range +0xC */
void func_002C6630(u8 *e) {
    AT(e, 0x50, f32) = 1.0f;
    AT(e, 0x54, f32) = 1.0f;
    AT(e, 0x58, f32) = 2000.0f;
    AT(e, 0x5C, f32) = 2000.0f;
}

/* depth range +0x18 */
void func_002C6540(u8 *e, const f32 *d) {
    if (d == NULL) {
        return;
    }
    AT(e, 0x50, f32) = d[0];
    AT(e, 0x54, f32) = d[1];
    AT(e, 0x58, f32) = d[2];
    AT(e, 0x5C, f32) = d[3];
}


extern VObject *D_0044FE10;    /* +0x80: the character in slot i (0xFF none) */
extern void *gCharacters[6];
extern void func_001267F0(void *c, s32 light);
extern s32 func_00126800(void *c);

/* +0x10 each frame: the characters this light is on (+0x20 per slot) get its light group
 * (slot << 16 | 0xB); the others it had go back to the default (0xA) */
void func_002BB280(u8 *o) {
    s32 i;

    for (i = 0; i < 6; i++) {
        u32 k = D_0044FE10 != NULL ? (u8)VCALL(D_0044FE10, 0x80, s32 (*)(VObject *, s32))(D_0044FE10, i & 0xFF) : (u8)i;
        s32 group = (i << 16) | 0xB;

        if (k == 0xFF || gCharacters[k] == NULL) {
            continue;
        }
        if (AT(o, 0x20 + i * 4, s32) != 0) {
            func_001267F0(gCharacters[k], group);
        } else if (func_00126800(gCharacters[k]) == group) {
            func_001267F0(gCharacters[k], 0xA);
        }
    }
}


/* +0x10 each frame: a pulsing colour (+0x10 RGBA, direction +0x30): modes 3 / 4 a grey
 * breathing between 0x20 and 0x80, mode 2 red and alpha between 0x80 and 0xC0 */
void func_0026B350(u8 *o) {
    s32 mode = AT(o, 0x14, s32);

    if (mode == 2) {
        if (AT(o, 0x30, s32) == 0) {
            AT(o, 0x10, u32) += 0x10000010;
            if (AT(o, 0x10, u8) >= 0xC0) {
                AT(o, 0x10, u32) = 0xC00040C0;
                AT(o, 0x30, s32) = 1;
            }
        } else {
            AT(o, 0x10, u32) += 0xEFFFFFF0;
            if (AT(o, 0x10, u8) < 0x81) {
                AT(o, 0x10, u32) = 0x80004080;
                AT(o, 0x30, s32) = 0;
            }
        }
    } else if ((u32)(mode - 3) < 2) {
        if (AT(o, 0x30, s32) == 0) {
            AT(o, 0x10, u32) += 0x01010101;
            if (AT(o, 0x10, u8) >= 0x80) {
                AT(o, 0x10, u32) = 0x80808080;
                AT(o, 0x30, s32) = 1;
            }
        } else {
            AT(o, 0x10, u32) += 0xFEFEFEFF;
            if (AT(o, 0x10, u8) < 0x21) {
                AT(o, 0x10, u32) = 0x20202020;
                AT(o, 0x30, s32) = 0;
            }
        }
    }
}


/* +0x10 for effects that don't change */
void func_002674E0(u8 *o) {
}


extern void *D_0046EB60[], *D_00469D00[];
extern void func_002BC000(void *drawer, u32 c0, u32 c1, s32 layer, f32 a, f32 b);

/* +0x14 draw (the fog): a temporary drawer object paints its colours (+0x10, +0x14) over the
 * range +0x50..+0x54 in layer 0x21 (9 while +0x1C) */
void func_002BB1A0(u8 *o) {
    u8 drawer[0x20] __attribute__((aligned(16)));

    AT(drawer, 0x0, void **) = D_0046EB60;
    AT(drawer, 0x4, s32) = -1;
    func_002BC000(drawer, AT(o, 0x10, u32), AT(o, 0x14, u32), AT(o, 0x1C, s32) != 0 ? 9 : 0x21,
                  AT(o, 0x50, f32), AT(o, 0x54, f32));
    AT(drawer, 0x0, void **) = D_00469D00;
}


extern VObject *D_0044E4F0;   /* the renderer */

/* a fog drawer: colours, range, queued with the renderer in `layer` */
void func_002BC000(void *drawer, u32 c0, u32 c1, s32 layer, f32 a, f32 b) {
    u8 *d = drawer;

    AT(d, 0x8, u32) = c0;
    AT(d, 0xC, u32) = c1;
    AT(d, 0x10, f32) = a;
    AT(d, 0x14, f32) = b;
    VCALL(D_0044E4F0, 0xC, void (*)(VObject *, void *, s32, s32))(D_0044E4F0, d, layer, 0);
}


extern void *D_0046D7A0[];
extern void func_0026B180(void *drawer, u32 rgba, s32 layer, s32 add);

/* +0x14 draw (a screen tint): when its colour (+0x10) has alpha, a temporary drawer paints it
 * in layer 0x28 (added for modes 1 and 4, +0x14) */
void func_0026B2C0(u8 *o) {
    u8 drawer[0x20] __attribute__((aligned(16)));

    AT(drawer, 0x0, void **) = D_0046D7A0;
    AT(drawer, 0x4, s32) = -1;
    if (AT(o, 0x10, u32) & 0xFF000000) {
        func_0026B180(drawer, AT(o, 0x10, u32), 0x28, AT(o, 0x14, s32) == 1 || AT(o, 0x14, s32) == 4);
    }
    AT(drawer, 0x0, void **) = D_00469D00;
}


extern void *D_0046D790[];
extern void func_00269940(void *drawer, u32 rgba, s32 which, s32 arg);

/* +0x14 draw of a two-colour screen effect: a temporary drawer paints each colour (+0x10 first,
 * then +0x14) that has alpha, both with +0x18 */
void func_002673E0(u8 *o) {
    u8 drawer[0x20] __attribute__((aligned(16)));

    AT(drawer, 0x0, void **) = D_0046D790;
    AT(drawer, 0x4, s32) = -1;
    if (AT(o, 0x10, u32) & 0xFF000000) {
        func_00269940(drawer, AT(o, 0x10, u32), 0, AT(o, 0x18, s32));
    }
    if (AT(o, 0x14, u32) & 0xFF000000) {
        func_00269940(drawer, AT(o, 0x14, u32), 1, AT(o, 0x18, s32));
    }
    AT(drawer, 0x0, void **) = D_00469D00;
}



/* set a screen-colour drawer's colour, layer and argument and hand it to the renderer (+0xC,
 * priority 0x20) */
void func_00269940(void *d, u32 rgba, s32 which, s32 arg) {
    AT(d, 0x8, u32) = rgba;
    AT(d, 0xC, s32) = which;
    AT(d, 0x10, s32) = arg;
    VCALL(D_0044E4F0, 0xC, void (*)(VObject *, void *, s32, s32))(D_0044E4F0, d, 0x20, 0);
}


/* hand a drawer to the renderer (+0xC) in its layer (+0x20); one flagged +0x32 bit 7 then
 * also has the renderer run +0x58 */
void func_002E56C0(u8 *d) {
    VObject *r = D_0044E4F0;

    VCALL(r, 0xC, void (*)(VObject *, void *, s32, s32))(r, d, AT(d, 0x20, s32), 0);
    if (AT(d, 0x32, u8) & 0x80) {
        VCALL(r, 0x58, void (*)(VObject *))(r);
    }
}


extern s32 func_002E3500(u8 *d);
extern s32 func_002E4760(u8 *d);

/* +0xC draw of the quad drawer: flagged ones (+0x32 bit 7) outside layer 0x17 by
 * func_002E3500, the rest by func_002E4760 */
s32 func_002E5660(u8 *d) {
    if ((AT(d, 0x32, u8) & 0x80) && AT(d, 0x20, s32) != 0x17) {
        return (u8)func_002E3500(d);
    }
    return (u8)func_002E4760(d);
}
