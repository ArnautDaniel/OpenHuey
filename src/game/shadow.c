/* Character shadows: shadow volumes of per-bone boxes, counted in renderer layer 6.
 *
 * A model's shadow object (model +0x1D0, vtable D_0046B1C0) is queued in layer 6 once per
 * light that casts it (func_001F3530). Its draw (func_001F2B80) extrudes the bone boxes away
 * from the light and draws their volumes with a counter in the half-size layer-6 buffer: the
 * PS2 does it in the buffer's alpha, read through a palette that adds or takes 1 (lights +0x30),
 * starting at 0x7F; z-pass against the scene's depth copied down to half size. The light's
 * blocker quads' volumes count the other way (func_001F2060), so a character's shadow doesn't
 * fall where a wall already shades the light. Where the count ends above 0x7F the shadow's
 * colour is written (its opacity from the light's falloff), and layer 6's end blurs the buffer
 * and takes it off the screen. On PC glr keeps the count in a stencil buffer.
 *
 * The shadow object: +0xC the skeleton, +0x8 the bone boxes, +0x110..+0x11C the screen box of
 * this draw (1/16 pixels), +0x120 the box's vertices in the volume (bit 0 on the outline, 1 on
 * a lit face), +0x128 its lit faces, +0x12E its outline edges, +0x140 the vertices extruded,
 * +0x1C0 / +0x240 both projected (ints), +0x2C0 the light's offset, +0x2D0 the bone node,
 * +0x2D4 the colour, +0x2D8 the light, +0x2DC the model's layer, +0x2E0 bones left out.
 *
 * Bone boxes (+0x8): a count, then 0x20-byte entries from +0x10: the bone, the face count (6),
 * then offsets to the faces (0x30 each: 4 vertices, 4 edges, the normal at +0x20), the edges (12
 * x 2 vertices) and the vertices (8). */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"

extern f32 *func_0017CE80(void *skeleton, s32 bone);   /* bone node */
extern void func_0010E640(f32 *out, const f32 *v, f32 s);   /* libvu0: scale x, y, z */
extern void sceVu0FTOI4Vector(s32 *out, const f32 *in);
extern VObject *D_0044E570;   /* the nav mesh: +0x14 the floor height at a point on a triangle */
extern VObject *D_0044E4C8;   /* the lights */
extern VObject *D_0044E4F0;   /* the renderer */
extern VObject *D_0044E4B8;   /* the camera */

/* queue shadow `s` of its model's bone `bone` (on nav triangle `tri`, the light offset `light`,
 * the model's layer `layer`) for each light that casts it there, if the lights allow a shadow
 * at its foot (lights +0x2C) */
void func_001F3530(u8 *s, s32 tri, s32 bone, f32 *light, s32 layer) {
    f32 p[4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));
    s32 idx[3];
    s32 i;

    AT(s, 0x2D0, f32 *) = func_0017CE80(AT(s, 0xC, void *), bone);
    sceVu0CopyVector(p, AT(s, 0x2D0, f32 *) + 12);
    if (D_0044E570 != NULL) {
        VCALL(D_0044E570, 0x14, void (*)(VObject *, s32, f32 *))(D_0044E570, tri, p);
    } else {
        p[1] = 0.0f;
    }
    sceVu0CopyVector(q, AT(s, 0x2D0, f32 *) + 12);
    q[1] = p[1];
    if ((u8)VCALL(D_0044E4C8, 0x2C, s32 (*)(VObject *, s32, f32 *))(D_0044E4C8, tri, q) != 1) {
        return;
    }
    sceVu0CopyVector((f32 *)(s + 0x2C0), light);
    AT(s, 0x2DC, s32) = layer;
    VCALL(D_0044E4C8, 0x14, void (*)(VObject *, f32 *, s32, s32 *))(D_0044E4C8, AT(s, 0x2D0, f32 *) + 12, tri, idx);
    for (i = 0; i < 3; i++) {
        if (idx[i] >= 0) {
            AT(s, 0x2D8, s32) = idx[i];
            VCALL(D_0044E4F0, 0xC, void (*)(VObject *, u8 *, s32, s32))(D_0044E4F0, s, 6, 0);
        }
    }
}

/* the outline of bone box `box` (of the boxes `base`) seen along `l` (box space, scaled to the
 * shadow's length): faces facing along it (n . l >= 0) are lit, an edge of an odd number of lit
 * faces is on the outline; vertices of lit faces and outline edges are extruded by -l */
void func_001F2900(u8 *s, u8 *base, u8 *box, f32 *l) {
    u8 *v = base + AT(box, 0x10, s32);
    u8 *e = base + AT(box, 0xC, s32);
    u8 *f = base + AT(box, 0x8, s32);
    s32 i;

    for (i = 0; i < 14; i++) {   /* the vertices' and faces' marks */
        AT(s, 0x120 + i, u8) = 0;
    }
    for (i = 0; i < 12; i++) {
        AT(s, 0x12E + i, u8) = 0;
    }
    for (i = 0; i < AT(box, 0x4, s32); i++, f += 0x30) {
        if (sceVu0InnerProduct(l, (f32 *)(f + 0x20)) < 0.0f) {
            continue;
        }
        AT(s, 0x128 + i, u8) = 1;
        AT(s, 0x12E + AT(f, 0x10, s32), u8) ^= 1;
        AT(s, 0x12E + AT(f, 0x14, s32), u8) ^= 1;
        AT(s, 0x12E + AT(f, 0x18, s32), u8) ^= 1;
        AT(s, 0x12E + AT(f, 0x1C, s32), u8) ^= 1;
        AT(s, 0x120 + AT(f, 0x0, s32), u8) |= 2;
        AT(s, 0x120 + AT(f, 0x4, s32), u8) |= 2;
        AT(s, 0x120 + AT(f, 0x8, s32), u8) |= 2;
        AT(s, 0x120 + AT(f, 0xC, s32), u8) |= 2;
    }
    for (i = 0; i < 12; i++, e += 8) {
        if (AT(s, 0x12E + i, u8) != 0) {
            AT(s, 0x120 + AT(e, 0x0, s32), u8) |= 1;
            AT(s, 0x120 + AT(e, 0x4, s32), u8) |= 1;
        }
    }
    for (i = 0; i < 8; i++, v += 0x10) {
        if (AT(s, 0x120 + i, u8) != 0) {
            sceVu0SubVector((f32 *)(s + 0x140 + i * 0x10), (f32 *)v, l);
        }
    }
}

/* project vertex `v` (the box's) with `scr` into `out` (screen ints, 1/16 pixels; z whole) and
 * grow the shadow's screen box; 0 if it is out of view (`clip`) */
static s32 shadow_project(u8 *s, f32 (*scr)[4], f32 (*clip)[4], f32 *v, s32 *out) {
    f32 t[4] __attribute__((aligned(16)));

    sceVu0ApplyMatrix(t, clip, v);
    if (!(t[0] <= t[3]) || t[0] < -t[3] || !(t[1] <= t[3]) || t[1] < -t[3] || !(t[2] <= t[3]) || t[2] < -t[3]) {
        return 0;
    }
    sceVu0ApplyMatrix(t, scr, v);
    t[3] = 1.0f / t[3];
    t[0] *= t[3];
    t[1] *= t[3];
    t[2] *= t[3];
    sceVu0FTOI4Vector(out, t);
    out[2] = out[2] / 16;
    if (out[0] < AT(s, 0x110, s32)) {
        AT(s, 0x110, s32) = out[0];
    }
    if (out[1] < AT(s, 0x114, s32)) {
        AT(s, 0x114, s32) = out[1];
    }
    if (AT(s, 0x118, s32) < out[0]) {
        AT(s, 0x118, s32) = out[0];
    }
    if (AT(s, 0x11C, s32) < out[1]) {
        AT(s, 0x11C, s32) = out[1];
    }
    return 1;
}

/* project the volume's vertices (box vertices `v`, then the extruded ones) with `scr` (the
 * half-size screen matrix of the bone) into +0x1C0 / +0x240; 0 if one is out of view (`clip`) */
s32 func_001F2560(u8 *s, f32 (*scr)[4], f32 (*clip)[4], f32 *v) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (AT(s, 0x120 + i, u8) != 0 && !shadow_project(s, scr, clip, v + i * 4, (s32 *)(s + 0x1C0 + i * 0x10))) {
            return 0;
        }
    }
    for (i = 0; i < 8; i++) {
        if (AT(s, 0x120 + i, u8) != 0 &&
            !shadow_project(s, scr, clip, (f32 *)(s + 0x140 + i * 0x10), (s32 *)(s + 0x240 + i * 0x10))) {
            return 0;
        }
    }
    return 1;
}

#ifdef HG_NATIVE
extern void glr_shadow_begin(void);
extern void glr_shadow_quad(const f32 *xyz, s32 inc);
extern void glr_shadow_fill(f32 x0, f32 y0, f32 x1, f32 y1, u32 rgba);
extern void glr_shadow_cancel(void);

/* a projected point (1/16 pixels around 2048, Z) in clip space: as the camera's clip matrix
 * makes it (Z from zMin .. zMax to -1 .. 1) */
static void shadow_ndc(const s32 *p, f32 *out) {
    f32 zMin = AT(D_0044E4B8, 0x18, f32), zMax = AT(D_0044E4B8, 0x1C, f32);

    out[0] = ((f32)p[0] / 16.0f - 2048.0f) / 2047.0f;
    out[1] = ((f32)p[1] / 16.0f - 2048.0f) / 2047.0f;
    out[2] = (2.0f * (f32)p[2] - zMax - zMin) / (zMax - zMin);
}

/* lights +0x30 (func_001FA0E0): one quad of a shadow volume (a strip of 4 projected points),
 * counting +1 when its screen winding is negative (`flip` the other way); a degenerate one
 * counts nothing. 0: no room for it (never, on PC) */
s32 func_001FA0E0(VObject *l, s32 *p0, s32 *p1, s32 *p2, s32 *p3, s32 flip) {
    s32 cross = (p1[0] - p0[0]) * (p2[1] - p0[1]) - (p1[1] - p0[1]) * (p2[0] - p0[0]);
    f32 xyz[4][3];

    (void)l;
    if (cross == 0) {
        return 1;
    }
    shadow_ndc(p0, xyz[0]);
    shadow_ndc(p1, xyz[1]);
    shadow_ndc(p2, xyz[2]);
    shadow_ndc(p3, xyz[3]);
    glr_shadow_quad(&xyz[0][0], ((cross < 0 ? flip : flip ^ 1)) == 0);
    return 1;
}

/* the volumes of the light's blocker quads (lights +0x3C, 0x60 each: corners, the normal +0x40,
 * the middle +0x50; +0x40 the count of their vectors) facing away from it, extruded from the
 * light (5 above it) by its length and again by 1/5 of it, counting the other way. 0 (no
 * shadow) if one is partly out of view */
s32 func_001F2060(u8 *s) {
    VObject *lights = D_0044E4C8, *cam = D_0044E4B8;
    s32 n = VCALL(lights, 0x40, s32 (*)(VObject *))(lights);
    f32 scr[4][4] __attribute__((aligned(16)));
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 rec[12] __attribute__((aligned(16)));
    f32 lp[4] __attribute__((aligned(16)));
    f32 len;
    s32 pass, k;

    if (n == 0) {
        return 1;
    }
    VCALL(cam, 0x4C, void (*)(VObject *, f32 (*)[4]))(cam, scr);
    VCALL(cam, 0x58, void (*)(VObject *, f32 (*)[4]))(cam, clip);
    VCALL(lights, 0x1C, void (*)(f32 *, VObject *, s32))(rec, lights, AT(s, 0x2D8, s32));
    lp[0] = rec[0];
    lp[1] = rec[1] + 5.0f;
    lp[2] = rec[2];
    lp[3] = rec[3];
    len = rec[11];
    for (pass = 0; pass < 2; pass++, len = 0.2f * rec[11]) {
        u8 *q = VCALL(lights, 0x3C, u8 *(*)(VObject *))(lights);

        for (k = 0; k < n; k += 6, q += 0x60) {
            f32 d[4] __attribute__((aligned(16)));
            f32 e[4][4] __attribute__((aligned(16)));
            f32 pt[8][4] __attribute__((aligned(16)));
            s32 sc[8][4];
            s32 i;

            sceVu0SubVector(d, lp, (f32 *)(q + 0x50));
            sceVu0Normalize(d, d);
            func_0010E640(d, d, len);
            if (!(sceVu0InnerProduct(d, (f32 *)(q + 0x40)) < 0.0f)) {
                continue;
            }
            for (i = 0; i < 4; i++) {
                sceVu0SubVector(e[i], lp, (f32 *)(q + i * 0x10));
                sceVu0Normalize(e[i], e[i]);
                func_0010E640(e[i], e[i], len);
            }
            for (i = 0; i < 8; i++) {
                f32 t[4] __attribute__((aligned(16)));

                if (i >= 4) {
                    sceVu0SubVector(pt[i], (f32 *)(q + (i - 4) * 0x10), e[i - 4]);
                } else {
                    sceVu0CopyVector(pt[i], (f32 *)(q + i * 0x10));
                }
                sceVu0ApplyMatrix(t, clip, pt[i]);
                if (!(t[0] <= t[3]) || t[0] < -t[3] || !(t[1] <= t[3]) || t[1] < -t[3] || !(t[2] <= t[3]) ||
                    t[2] < -t[3]) {
                    return 0;
                }
            }
            for (i = 0; i < 8; i++) {
                f32 t[4] __attribute__((aligned(16)));

                sceVu0ApplyMatrix(t, scr, pt[i]);
                t[3] = 1.0f / t[3];
                t[0] *= t[3];
                t[1] *= t[3];
                t[2] *= t[3];
                sceVu0FTOI4Vector(sc[i], t);
                sc[i][2] = sc[i][2] / 16;
            }
            if (!func_001FA0E0(lights, sc[0], sc[1], sc[4], sc[5], 1) || !func_001FA0E0(lights, sc[1], sc[3], sc[5], sc[7], 1) ||
                !func_001FA0E0(lights, sc[3], sc[2], sc[7], sc[6], 1) || !func_001FA0E0(lights, sc[2], sc[0], sc[6], sc[4], 1) ||
                !func_001FA0E0(lights, sc[4], sc[5], sc[6], sc[7], 1)) {
                return 0;
            }
        }
    }
    return 1;
}

/* the shadow's draw (+0xC, layer 6): its opacity from the light (record +0x1C: position, +0x1C
 * strength, +0x24 / +0x28 falloff, +0x2C length; out of range or under 2: none), scaled in the
 * tinted layers; then the bone boxes' volumes and the blockers', and the colour where they
 * count. 0: nothing drawn */
s32 func_001F2B80(u8 *s) {
    VObject *lights = D_0044E4C8, *cam = D_0044E4B8;
    f32 rec[12] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 scr[4][4] __attribute__((aligned(16)));
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 fall = 1.0f, f;
    u8 *base;
    u32 a;
    s32 h;

    VCALL(lights, 0x1C, void (*)(f32 *, VObject *, s32))(rec, lights, AT(s, 0x2D8, s32));
    if (rec[11] < 1.0f) {
        return 0;
    }
    sceVu0AddVector(d, AT(s, 0x2D0, f32 *) + 12, (f32 *)(s + 0x2C0));
    d[3] = 0.0f;
    d[0] = rec[0] - d[0];
    d[1] = rec[1] - d[1];
    d[2] = rec[2] - d[2];
    if (rec[10] != 0.0f) {
        f32 r = rec[10] - (d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);

        if (r <= 0.0f) {
            return 0;
        }
        r = rec[9] * (r * rec[9]);
        r = r * r;
        fall = r * r;
    }
    f = 64.0f * fall * rec[7];
    a = f >= 2147483648.0f ? (u32)(s32)(f - 2147483648.0f) | 0x80000000u : (u32)(s32)f;
    if (a < 2) {
        return 0;
    }
    if (a > 0x80) {
        a = 0x80;
    }
    switch (AT(s, 0x2DC, s32)) {   /* the tinted layers dim it as they dim the screen */
    case 0x11:
        a = a * (VCALL(D_0044E4F0, 0x68, u32 (*)(VObject *))(D_0044E4F0) >> 25) >> 7;
        break;
    case 0x0F:
    case 0x23:
    case 0x1A: {
        u32 t = VCALL(D_0044E4F0, 0x74, u32 (*)(VObject *))(D_0044E4F0) >> 24;

        if (t < 0x80) {
            a = a * t >> 7;
        }
        break;
    }
    }
    AT(s, 0x2D4, u32) = a << 24 | 0x808080;
    glr_shadow_begin();
    AT(s, 0x110, s32) = 0xFFF0;
    AT(s, 0x114, s32) = 0xFFF0;
    AT(s, 0x118, s32) = 0;
    AT(s, 0x11C, s32) = 0;
    VCALL(cam, 0x4C, void (*)(VObject *, f32 (*)[4]))(cam, scr);
    VCALL(cam, 0x58, void (*)(VObject *, f32 (*)[4]))(cam, clip);
    base = AT(s, 0x8, u8 *);
    if (base == NULL) {
        glr_shadow_cancel();
        return 0;
    }
    for (h = 0; h < AT(base, 0x0, s32); h++) {
        u8 *box = base + 0x10 + h * 0x20;
        u8 *f = base + AT(box, 0x8, s32);
        u8 *edges = base + AT(box, 0xC, s32);
        f32 b[4][4] __attribute__((aligned(16)));
        f32 bs[4][4] __attribute__((aligned(16)));
        f32 bc[4][4] __attribute__((aligned(16)));
        f32 inv[4][4] __attribute__((aligned(16)));
        f32 l[4] __attribute__((aligned(16)));
        s32 i, k;

        if (AT(s, 0x2E0, u32) & (1u << h)) {
            continue;
        }
        sceVu0CopyMatrix(b, (f32 (*)[4])func_0017CE80(AT(s, 0xC, void *), AT(box, 0x0, s32)));
        sceVu0MulMatrix(bs, scr, b);
        sceVu0MulMatrix(bc, clip, b);
        sceVu0InversMatrix(inv, b);
        sceVu0Normalize(l, d);
        sceVu0ApplyMatrix(l, inv, l);
        func_0010E640(l, l, rec[11]);
        func_001F2900(s, base, box, l);
        if (func_001F2560(s, bs, bc, (f32 *)(base + AT(box, 0x10, s32))) == 0) {
            glr_shadow_cancel();
            return 0;
        }
        for (i = 0; i < 6; i++, f += 0x30) {
            s32 *v = (s32 *)f;

            if (AT(s, 0x128 + i, u8) == 0) {
                continue;
            }
            if (!func_001FA0E0(lights, (s32 *)(s + 0x1C0 + v[0] * 0x10), (s32 *)(s + 0x1C0 + v[1] * 0x10),
                               (s32 *)(s + 0x1C0 + v[3] * 0x10), (s32 *)(s + 0x1C0 + v[2] * 0x10), 0) ||
                !func_001FA0E0(lights, (s32 *)(s + 0x240 + v[1] * 0x10), (s32 *)(s + 0x240 + v[0] * 0x10),
                               (s32 *)(s + 0x240 + v[2] * 0x10), (s32 *)(s + 0x240 + v[3] * 0x10), 0)) {
                glr_shadow_cancel();
            return 0;
            }
            for (k = 0; k < 4; k++) {   /* the face's outline edges: their sides */
                s32 en = v[4 + k], p, q;

                if (AT(s, 0x12E + en, u8) == 0) {
                    continue;
                }
                p = AT(edges + en * 8, 0x0, s32);
                q = AT(edges + en * 8, 0x4, s32);
                if (p == v[k]) {   /* along the face's winding */
                    s32 t = p;

                    p = q;
                    q = t;
                }
                if (!func_001FA0E0(lights, (s32 *)(s + 0x1C0 + p * 0x10), (s32 *)(s + 0x1C0 + q * 0x10),
                                   (s32 *)(s + 0x240 + p * 0x10), (s32 *)(s + 0x240 + q * 0x10), 0)) {
                    glr_shadow_cancel();
            return 0;
                }
            }
        }
    }
    if (!(u8)func_001F2060(s) || AT(s, 0x110, s32) >= AT(s, 0x118, s32)) {
        glr_shadow_cancel();
        return 0;
    }
    glr_shadow_fill((f32)AT(s, 0x110, s32) / 16.0f, (f32)AT(s, 0x114, s32) / 16.0f, (f32)AT(s, 0x118, s32) / 16.0f,
                    (f32)AT(s, 0x11C, s32) / 16.0f, AT(s, 0x2D4, u32));
    return 1;
}
#endif

/* ---- a door's shadow (doors +0x190, class D_0046D800): the light through a doorway, as the
 * door's lit face swept away from the light into a box, its four sides drawn in layer 6's
 * buffer shading from the door's colour (+0x54: 0x808080 and the strength) to nothing at the
 * far end. +0x10 the door's matrix, +0x50 its lit face (0 or 6: four corners from +0x50 in
 * D_003EC080, its normal at +4), +0x60 the light (5 above it), +0x70 how far the box goes ---- */

extern f32 D_003EC080[][4], D_003EC0C0[4], D_003EC0D0[4], D_003EC130[4];
extern VObject *D_0044E4F0;   /* the renderer */

/* the shadow from light `l`: 1 if there is one (not out of the light's reach), with its
 * strength (the light's, its falloff at the doorway, the angle it meets the door, the scene's
 * brightness) */
s32 func_002786D0(u8 *o, s32 l) {
    VObject *lights = D_0044E4C8;
    f32 rec[12] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 power, reach, range, len, fall = 1.0f, c, k, *amb;
    s32 a;

    VCALL(lights, 0x1C, void (*)(f32 *, VObject *, s32))(rec, lights, l);
    power = rec[7];
    reach = rec[9];
    range = rec[10];
    len = rec[11];
    AT(o, 0x60, f32) = rec[0];
    AT(o, 0x64, f32) = 5.0f + rec[1];
    AT(o, 0x68, f32) = rec[2];
    AT(o, 0x6C, f32) = rec[3];
    sceVu0ApplyMatrix(d, (f32 (*)[4])(o + 0x10), D_003EC0D0);
    sceVu0SubVector(d, (f32 *)(o + 0x60), d);
    sceVu0ApplyMatrix(n, (f32 (*)[4])(o + 0x10), D_003EC0C0);
    if (sceVu0InnerProduct(d, n) < 0.0f) {
        AT(o, 0x50, s32) = 0;
    } else {
        AT(o, 0x50, s32) = 6;
        sceVu0ApplyMatrix(d, (f32 (*)[4])(o + 0x10), D_003EC130);
        sceVu0SubVector(d, (f32 *)(o + 0x60), d);
    }
    if (range != 0.0f) {
        f32 d2 = 0.0f + (0.0f + d[1] * d[1] + d[0] * d[0]) + d[2] * d[2];
        f32 r = range - d2;

        if (r < 0.0f) {
            return 0;
        }
        r = r * reach;
        r = reach * r;
        r = r * r;
        fall = r * r;
    }
    sceVu0Normalize(d, d);
    sceVu0ApplyMatrix(n, (f32 (*)[4])(o + 0x10), D_003EC080[AT(o, 0x50, s32) + 4]);
    sceVu0Normalize(n, n);
    c = -sceVu0InnerProduct(d, n);
    k = 4.0f * (c * c);
    amb = VCALL(lights, 0x28, f32 *(*)(VObject *))(lights);
    a = (s32)(k * (fall * power) * (64.0f + (amb[2] + (amb[0] + amb[1])) / 3.0f));
    AT(o, 0x54, s32) = a;
    if (AT(o, 0x54, s32) <= 0) {
        return 0;
    }
    if (AT(o, 0x54, s32) >= 0x81) {
        AT(o, 0x54, s32) = 0x80;
    }
    AT(o, 0x54, u32) = (u32)AT(o, 0x54, s32) << 24 | 0x808080;
    AT(o, 0x70, f32) = 0.75f * len;
    return 1;
}

/* the door's shadow from each of the (up to 3) lights reaching `pos` (lights +0x2C on, +0x14
 * the lights of the spot), queued in renderer layer 6 when func_002786D0 finds one */
void func_00278D60(u8 *o, void *model, f32 *pos, f32 *rot) {
    VObject *lights = D_0044E4C8;
    s32 l[3];
    s32 i;

    if ((VCALL(lights, 0x2C, s32 (*)(VObject *))(lights) & 0xFF) != 1) {
        return;
    }
    VCALL(lights, 0x14, void (*)(VObject *, f32 *, void *, s32 *))(lights, pos, model, l);
    for (i = 0; i < 3; i++) {
        VObject *r = D_0044E4F0;

        if (l[i] < 0) {
            continue;
        }
        sceVu0UnitMatrix((f32 (*)[4])(o + 0x10));
        sceVu0RotMatrix((f32 (*)[4])(o + 0x10), (f32 (*)[4])(o + 0x10), rot);
        sceVu0TransMatrix((f32 (*)[4])(o + 0x10), (f32 (*)[4])(o + 0x10), pos);
        if ((func_002786D0(o, l[i]) & 0xFF) == 1) {
            VCALL(r, 0xC, void (*)(VObject *, u8 *, s32, s32))(r, o, 6, 0);
        }
    }
}

/* the eight corners: the lit face, then each swept away from the light by +0x70; 0 if one is
 * out of view */
static inline __attribute__((always_inline)) s32 door_shadow_corners(u8 *o, VObject *cam, f32 (*p)[4]) {
    f32 clip[4][4] __attribute__((aligned(16)));
    s32 i;

    VCALL(cam, 0x58, void (*)(VObject *, f32 (*)[4]))(cam, clip);
    for (i = 0; i < 8; i++) {
        f32 v[4] __attribute__((aligned(16)));

        if (i < 4) {
            sceVu0ApplyMatrix(p[i], (f32 (*)[4])(o + 0x10), D_003EC080[AT(o, 0x50, s32) + i]);
        } else {
            sceVu0SubVector(p[i], (f32 *)(o + 0x60), p[i - 4]);
            sceVu0Normalize(p[i], p[i]);
            func_0010E640(p[i], p[i], AT(o, 0x70, f32));
            sceVu0SubVector(p[i], p[i - 4], p[i]);
        }
        sceVu0ApplyMatrix(v, clip, p[i]);
        if (!(v[0] <= v[3]) || v[0] < -v[3] || !(v[1] <= v[3]) || v[1] < -v[3] || !(v[2] <= v[3]) || v[2] < -v[3]) {
            return 0;
        }
    }
    return 1;
}

/* the corners on the half-size screen (1/16 pixels, Z) */
static inline __attribute__((always_inline)) void door_shadow_project(VObject *cam, f32 (*p)[4], s32 (*s)[4]) {
    f32 m[4][4] __attribute__((aligned(16))) = {{0}};   /* (an out buffer; cleared so it starts as the original's) */
    s32 i;

    VCALL(cam, 0x4C, void (*)(VObject *, f32 (*)[4]))(cam, m);
    for (i = 0; i < 8; i++) {
        f32 v[4] __attribute__((aligned(16)));
        f32 q;

        sceVu0ApplyMatrix(v, m, p[i]);
        q = 1.0f / v[3];
        v[0] = v[0] * q;
        v[1] = v[1] * q;
        v[3] = q;
        v[2] = v[2] * q;
        sceVu0FTOI4Vector(s[i], v);
        s[i][2] = s[i][2] / 16;
    }
}

#ifndef HG_NATIVE
/* one side of the box (a strip p0 p1 at the door, p2 p3 at the far end), if it faces the
 * screen: shaded from the colour to 0. 0 if the packet didn't fit */
s32 func_002784F0(u8 *o, s32 *p0, s32 *p1, s32 *p2, s32 *p3) {
    u64 *g;

    if ((p1[0] - p0[0]) * (p2[1] - p0[1]) - (p1[1] - p0[1]) * (p2[0] - p0[0]) >= 0) {
        return 1;
    }
    g = VCALL(D_0044E4F0, 0x14, u64 *(*)(VObject *, s32))(D_0044E4F0, 0xB);
    if (g == NULL) {
        return 0;
    }
    g[0] = 0x1000000A;
    AT(g, 0x8, s32) = 0;
    AT(g, 0xC, s32) = 0x5000000A;
    g[2] = 0x8009 | (u64)0x10000000 << 32;
    g[3] = 0xE;
    g[4] = 0x4C;   /* PRIM: strip, gouraud, blended */
    g[5] = 0;
    AT(g, 0x30, u32) = AT(o, 0x54, u32);
    AT(g, 0x34, f32) = 1.0f;
    g[7] = 1;
    g[8] = (u64)((s64)p0[0] | (s64)p0[1] << 16 | (s64)p0[2] << 32);
    g[9] = 0xD;
    AT(g, 0x50, u32) = AT(o, 0x54, u32);
    AT(g, 0x54, f32) = 1.0f;
    g[11] = 1;
    g[12] = (u64)((s64)p1[0] | (s64)p1[1] << 16 | (s64)p1[2] << 32);
    g[13] = 0xD;
    AT(g, 0x70, u32) = 0;
    AT(g, 0x74, f32) = 1.0f;
    g[15] = 1;
    g[16] = (u64)((s64)p2[0] | (s64)p2[1] << 16 | (s64)p2[2] << 32);
    g[17] = 5;
    AT(g, 0x90, u32) = 0;
    AT(g, 0x94, f32) = 1.0f;
    g[19] = 1;
    g[20] = (u64)((s64)p3[0] | (s64)p3[1] << 16 | (s64)p3[2] << 32);
    g[21] = 5;
    return 1;
}

#else
extern void glr_strip(const f32 *mvp, s32 n, const f32 *xyzw, const f32 *st, const u8 *rgba, const void *tex,
                      u64 tex0, u32 prim);

/* PC: one side of the box, as the PS2 draws it, a gouraud strip into layer 6's buffer */
static void door_shadow_side(u8 *o, s32 *p0, s32 *p1, s32 *p2, s32 *p3) {
    static const f32 kIdentity[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    s32 *p[4] = {p0, p1, p2, p3};
    f32 xyzw[4][4], st[4][2] = {{0}};
    u8 rgba[4][4];
    s32 k;

    if ((p1[0] - p0[0]) * (p2[1] - p0[1]) - (p1[1] - p0[1]) * (p2[0] - p0[0]) >= 0) {
        return;
    }
    for (k = 0; k < 4; k++) {
        u32 c = k < 2 ? AT(o, 0x54, u32) : 0;
        u32 flags = k < 2 ? 0x8000 : 0;

        shadow_ndc(p[k], xyzw[k]);
        AT(&xyzw[k][3], 0, u32) = flags;
        rgba[k][0] = (u8)c;
        rgba[k][1] = (u8)(c >> 8);
        rgba[k][2] = (u8)(c >> 16);
        rgba[k][3] = (u8)(c >> 24);
    }
    glr_strip(kIdentity, 4, &xyzw[0][0], &st[0][0], &rgba[0][0], NULL, 0, 0x8 | 0x40);
}

#endif

/* +0xC draw (layer 6): the box's sides into the shadow buffer (page 0x180, 256 x 224); 0 if it
 * is partly out of view or a packet didn't fit. (PC: gouraud strips into layer 6's buffer) */
s32 func_002789B0(u8 *o) {
    VObject *cam = D_0044E4B8;
    f32 p[8][4] __attribute__((aligned(16)));
    s32 s[8][4] __attribute__((aligned(16)));
#ifndef HG_NATIVE
    u64 *g;
#endif

    if (!door_shadow_corners(o, cam, p)) {
        return 0;
    }
    door_shadow_project(cam, p, s);
#ifdef HG_NATIVE
    door_shadow_side(o, s[0], s[1], s[4], s[5]);
    door_shadow_side(o, s[1], s[3], s[5], s[7]);
    door_shadow_side(o, s[3], s[2], s[7], s[6]);
    door_shadow_side(o, s[2], s[0], s[6], s[4]);
    return 1;
#else
    g = VCALL(D_0044E4F0, 0x14, u64 *(*)(VObject *, s32))(D_0044E4F0, 5);
    if (g == NULL) {
        return 0;
    }
    g[0] = 0x10000004;
    AT(g, 0x8, s32) = 0;
    AT(g, 0xC, s32) = 0x50000004;
    g[2] = 0x8003 | (u64)0x10000000 << 32;
    g[3] = 0xE;
    g[4] = 0x5000F;   /* TEST */
    g[5] = 0x47;
    g[6] = 0x40180;   /* FRAME: the shadow buffer */
    g[7] = 0x4C;
    g[8] = 0xFF0000 | (u64)0xDF0000 << 32;   /* SCISSOR 0..255 x 0..223 */
    g[9] = 0x40;
    if (!(func_002784F0(o, s[0], s[1], s[4], s[5]) & 0xFF)) {
        return 0;
    }
    if (!(func_002784F0(o, s[1], s[3], s[5], s[7]) & 0xFF)) {
        return 0;
    }
    if (!(func_002784F0(o, s[3], s[2], s[7], s[6]) & 0xFF)) {
        return 0;
    }
    return (func_002784F0(o, s[2], s[0], s[6], s[4]) & 0xFF) != 0;
#endif
}
