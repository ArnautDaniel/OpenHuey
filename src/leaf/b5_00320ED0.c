#include "common.h"
#include "ptmf.h"
#include "progress.h"
#include "globals.h"
#include "actor.h"

/* Field access by byte offset into objects whose layout is not yet known. */
#define S16(p, off) (*(s16 *)((u8 *)(p) + (off)))
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))
#define S64(p, off) (*(s64 *)((u8 *)(p) + (off)))
#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
#define PTR(p, off) (*(void * *)((u8 *)(p) + (off)))
/* gFileLoader vtable +0xC: start loading file `name` into `dest` (flags 0x4000000) */
#define FILE_LOAD_ASYNC(name, dest) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, void *, void *, u32, s32))(gFileLoader, name, dest, 0x4000000, 0)

extern u8 D_0042B200[];
extern u8 D_0042B520[];
extern void *D_0042C200[];
extern void *D_0042C2F0[];
extern u8 D_0042C360[];
extern u8 D_0042C380[];
extern u8 D_0042C3C0[];
extern u8 D_0042C6A0[];
extern u8 D_0042C6E0[];
extern u8 D_0042C730[];
extern u8 D_0042C770[];
extern u8 D_0042C790[];
extern u8 D_0042C810[];
extern u8 D_0042C860[];
extern u8 D_0042C900[];
extern u8 D_0042C940[];
extern u8 D_0042C990[];
extern u8 D_0042C9D0[];
extern u8 D_0042CA20[];
extern u8 D_0042CB60[];
extern u8 D_0042CC00[];
extern u8 D_0042D0C0[];
extern void *D_0042E310[];
extern void *D_0042E3F0[];
extern u8 D_0042E410[];
extern u8 D_0042E4C0[];
extern u8 D_00460BE0[];
extern void *D_0047ADB4[];
extern void *D_0047ADB8[];
extern u32 D_0047E36C;

void *func_00320ED0(void) {
    return D_0042B200;
}

void *func_00320EE0(void) {
    return D_0042B520;
}

void *func_00320EF0(void *self, s32 i) {
    return D_0042C200[i];
}

void *func_00320F10(void) {
    return D_0042C360;
}

void *func_00320F20(void *self, s32 i) {
    return D_0042C2F0[i];
}

void *func_00321750(void) {
    return D_0042C380;
}

void *func_00321760(void) {
    return D_0042C3C0;
}

void func_00322A60(u8 *self) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        ((volatile u8 *)self)[0x118 + i] = 0xFF;
    }
    self[0x158] = 0;
}

void func_00322B30(u8 *self) {
    if ((D_0047E36C >> 5) & 1) {
        self[0x158] = 1;
    }
}

void func_00324790(u8 *self, s32 unused, u32 v) {
    S32(self, 0x1540) = v < 3 ? (s32)v : -1;
}

s32 func_003247C0(u8 *self) {
    return self[0x15AF] != 0;
}

/* Saves this enemy's state into gProgress slot `slot` (36-byte records at +0x878). */
void func_00324C00(u8 *self, s32 slot, u32 b12) {
    u8 *rec = (u8 *)gProgress + 0x878 + slot * 36;

    U32(rec, 0x0) = U32(self, 0x30);
    U32(rec, 0x4) = U32(self, 0x1540);
    U32(rec, 0x8) = U32(self, 0x34);
    S16(rec, 0xC) = S16(self, 0x1584);
    rec[0xE] = self[0x15AD];
    rec[0xF] = (u8)(S16(self, 0x1588) / 10);
    rec[0x10] = self[0x15AE];
    rec[0x11] = self[0x15AF];
    rec[0x12] = self[0x15A9] == 4 ? 0 : (u8)b12;
    rec[0x13] = self[0x15B0];
    rec[0x14] = self[0x15A2];
    U32(rec, 0x18) = U32(self, 0x14C8);
    F32(rec, 0x1C) = F32(self, 0x54);
    F32(rec, 0x20) = F32(self, 0x1568);
}

void *func_0032C350(void) {
    return D_0042C6A0;
}

void *func_0032C360(void) {
    return D_0042C6E0;
}

void func_0032C5E0(u8 *self) {
    PTR(self, 0x874) = D_0042C730;
}

void *func_0032C6F0(void) {
    return D_0042C770;
}

void *func_0032C700(void) {
    return D_0042C790;
}

void *func_0032C710(void) {
    return D_0042C810;
}

void *func_0032C720(void *self, s32 i) {
    return D_0047ADB4[i];
}

void *func_0032C740(void) {
    return D_0042C860;
}

void *func_0032C750(void *self, s32 i) {
    return D_0047ADB8[i];
}

void *func_0032CAD0(void) {
    return D_0042C900;
}

void *func_0032CAE0(void) {
    return D_0042C940;
}

s32 func_0032CD00(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00460BE0, dest);
}

void *func_0032CF10(void) {
    return D_0042C990;
}

void *func_0032CF20(void) {
    return D_0042C9D0;
}

void func_0032D270(u8 *self, s32 a, f32 x, f32 y) {
    S32(self, 0x1624) = 0;
    S32(self, 0x1628) = 1;
    S32(self, 0x162C) = 60;
    F32(self, 0x1634) = F32(self, 0x10);
    F32(self, 0x1638) = F32(self, 0x18);
    F32(self, 0x163C) = x;
    F32(self, 0x1640) = y;
    self[0x16A8] = 1;
    self[0x16A9] = 1;
    S32(self, 0x1630) = a;
}

void func_0032D3E0(u8 *self, s32 a, f32 x, f32 y) {
    S32(self, 0x1624) = 0;
    S32(self, 0x1628) = 1;
    S32(self, 0x162C) = 60;
    F32(self, 0x1634) = F32(self, 0x10);
    F32(self, 0x1638) = F32(self, 0x18);
    F32(self, 0x163C) = x;
    F32(self, 0x1640) = y;
    self[0x16A8] = 1;
    self[0x16A9] = 1;
    S32(self, 0x1630) = a;
}

/* ---- character 0x1A (an animal in the garden): +0x1624 state, +0x1628 just entered it,
 * +0x162C timer, +0x1630 its sound, +0x1634 / +0x1638 home (x, z), +0x163C / +0x1640 where it
 * runs off to; +0x16A8 / +0x16A9 its feet down last frame. D_01991600: who it watches ---- */

extern u8 D_01991600[];
extern f32 func_002E2D00(f32 angle);   /* wrapped into -pi..pi */
extern f32 func_002E2BC0(const f32 *v);   /* heading of v */
extern void func_002DE030(void *motion, s32 anim, s32 blend, s32 loop, f32 speed);
extern void func_001F6370(void *motion, f32 *out, f32 t);
extern void func_00125900(void *c);
extern void func_00122C20(void *a, s32 sound, s32 a2, s32 a3, s32 a4, void *a5);
extern void sceVu0SubVector(f32 *out, const f32 *a, const f32 *b);
extern void sceVu0AddVector(f32 *out, const f32 *a, const f32 *b);
extern void sceVu0ScaleVector(f32 *out, const f32 *v, f32 s);
extern void sceVu0CopyVector(f32 *out, const f32 *v);
extern void sceVu0ApplyMatrix(f32 *out, f32 (*m)[4], const f32 *v);
extern f32 sceVu0InnerProduct(const f32 *a, const f32 *b);

#define CHAR_POS(c) ((f32 *)((u8 *)(c) + 0x10))
#define CHAR_ON(c) (*((u8 *)(c) + 0x28))
#define CHAR_ROOM(c) S32(c, 0x30)
#define CUR_ROOM() VCALL(gProgress, 0xC, s32 (*)(void *))(gProgress)

static f32 animal_rnd(void) {
    return VCALL(gRandom, 0x20, f32 (*)(void *))(gRandom);
}

/* the squared distance to the nearest of Fiona, Hewie and the pursuer (those two when active
 * and in the room), whose position goes into out */
f32 func_0032CF40(u8 *self, f32 *out) {
    f32 d[4] __attribute__((aligned(16)));
    f32 pf[4] __attribute__((aligned(16)));
    f32 ph[4] __attribute__((aligned(16)));
    f32 pp[4] __attribute__((aligned(16)));
    f32 df, dh, dp;

    sceVu0SubVector(d, CHAR_POS(self), CHAR_POS(gCharPlayer));
    df = sceVu0InnerProduct(d, d);
    sceVu0CopyVector(pf, CHAR_POS(gCharPlayer));
#ifdef HG_NATIVE
    if (gCharPartner != NULL && CHAR_ON(gCharPartner) && CHAR_ROOM(gCharPartner) == CUR_ROOM()) {   /* (no Hewie: HG_NOPARTNER) */
#else
    if (CHAR_ON(gCharPartner) && CHAR_ROOM(gCharPartner) == CUR_ROOM()) {
#endif
        sceVu0SubVector(d, CHAR_POS(self), CHAR_POS(gCharPartner));
        dh = sceVu0InnerProduct(d, d);
        sceVu0CopyVector(ph, CHAR_POS(gCharPartner));
    } else {
        dh = 1.0e8f;
    }
    if (gCharPursuer != NULL && CHAR_ON(gCharPursuer) && CHAR_ROOM(gCharPursuer) == CUR_ROOM()) {
        sceVu0SubVector(d, CHAR_POS(self), CHAR_POS(gCharPursuer));
        dp = sceVu0InnerProduct(d, d);
        sceVu0CopyVector(pp, CHAR_POS(gCharPursuer));
    } else {
        dp = 1.0e8f;
    }
    sceVu0CopyVector(out, pf);
    if (!(df <= dh)) {
        sceVu0CopyVector(out, ph);
        df = dh;
    }
    if (!(df <= dp)) {
        sceVu0CopyVector(out, pp);
        df = dp;
    }
    return df;
}

/* turn 0.3 of the way towards (x, z) */
static inline __attribute__((always_inline)) void animal_face(u8 *self, f32 x, f32 z) {
    static const union { u32 u; f32 f; } k03 = {0x3E99999A};
    f32 t[4] __attribute__((aligned(16)));
    f32 a, d;

    t[0] = x;
    t[1] = 0.0f;
    t[2] = z;
    t[3] = 1.0f;
    sceVu0SubVector(t, t, CHAR_POS(self));
    a = func_002E2D00(func_002E2BC0(t));
    d = func_002E2D00(a - func_002E2D00(F32(self, 0x54)));
    F32(self, 0x54) = func_002E2D00(0.0f + F32(self, 0x54) + k03.f * d);
}

#define ANIMAL_CLIP_DONE(self) ((S32(PTR(PTR(self, 0xF0), 0x6A4), 0x18) & 0x20) != 0)

static inline void animal_go(u8 *self, s32 state) {
    S32(self, 0x1624) = state;
    S32(self, 0x1628) = 1;
}

/* a frame of its behaviour: 0 grazing (turned to whoever it watches, 1..2 s), 1 looking up, 4
 * turning home (5 frames), 5 going back to grazing; 2 alert (someone within 30: meant to face
 * away from them, but the original takes the point's y for its z), 3 running off (within 20) to +0x163C / +0x1640 - 1 once there (its sound
 * +0x1630 + 1 played) */
s32 func_0032D430(u8 *self) {
    static const union { u32 u; f32 f; } k08 = {0x3F4CCCCD};
    f32 who[4] __attribute__((aligned(16)));
    f32 d = func_0032CF40(self, who);

    if (S32(self, 0x1624) != 3 && d < 400.0f) {
        animal_go(self, 3);
    }
    if (S32(self, 0x1624) != 3 && S32(self, 0x1624) != 2 && d < 900.0f) {
        animal_go(self, 2);
    }
    switch (S32(self, 0x1624)) {
    case 0:
        if (S32(self, 0x1628) != 0) {
            func_002DE030(PTR(self, 0xF0), 0x9000, 1, -1, 5.0f);
            S32(self, 0x162C) = (s32)(60.0f * (1.0f + animal_rnd()));
            S32(self, 0x1628) = 0;
        }
        animal_face(self, F32(D_01991600, 0), F32(D_01991600, 8));
        S32(self, 0x162C) -= 1;
        if (S32(self, 0x162C) < 0) {
            if (d <= 2500.0f || !(animal_rnd() < k08.f)) {
                S32(self, 0x1624) = 1;
            } else {
                S32(self, 0x1624) = 4;
            }
            S32(self, 0x1628) = 1;
            S32(self, 0x162C) = 0;
        }
        break;
    case 1:
        if (S32(self, 0x1628) != 0) {
            func_002DE030(PTR(self, 0xF0), 0x9001, 1, -1, 5.0f);
            S32(self, 0x1628) = 0;
        }
        if (ANIMAL_CLIP_DONE(self) && animal_rnd() < 0.5f) {
            animal_go(self, 0);
        }
        break;
    case 4:
        if (S32(self, 0x1628) != 0) {
            S32(self, 0x162C) = 5;
            S32(self, 0x1628) = 0;
        }
        animal_face(self, F32(self, 0x1634), F32(self, 0x1638));
        if (--S32(self, 0x162C) < 0) {
            animal_go(self, 5);
        }
        break;
    case 5:
        if (S32(self, 0x1628) != 0) {
            func_002DE030(PTR(self, 0xF0), 0x9002, 1, -1, 5.0f);
            S32(self, 0x1628) = 0;
        }
        if (ANIMAL_CLIP_DONE(self)) {
            animal_go(self, 0);
        }
        break;
    case 2: {
        f32 away[4] __attribute__((aligned(16)));

        if (S32(self, 0x1628) != 0) {
            func_002DE030(PTR(self, 0xF0), 0x9002, 1, -1, 5.0f);
            S32(self, 0x1628) = 0;
            func_0032CF40(self, (f32 *)D_01991600);
        }
        sceVu0SubVector(away, CHAR_POS(self), (f32 *)D_01991600);
        sceVu0ScaleVector(away, away, 100.0f);
        sceVu0AddVector(away, away, CHAR_POS(self));
        animal_face(self, away[0], away[1]);   /* (sic: y for z - the game's own slip) */
        if (ANIMAL_CLIP_DONE(self)) {
            animal_go(self, 0);
        }
        break;
    }
    case 3: {
        f32 dz, dx;

        if (S32(self, 0x1628) != 0) {
            func_002DE030(PTR(self, 0xF0), 0x9003, 1, -1, 5.0f);
            S32(self, 0x1628) = 0;
        }
        animal_face(self, F32(self, 0x163C), F32(self, 0x1640));
        dz = F32(self, 0x1640) - F32(self, 0x18);
        dx = F32(self, 0x163C) - F32(self, 0x10);
        if (dz * dz + dx * dx < 1.0f) {
            func_00122C20(self, S32(self, 0x1630) + 1, 6, 0, 0, NULL);
            return 1;
        }
        break;
    }
    }
    return 0;
}

/* a frame of it (func_0032D430) moved by its animation's root motion, a footstep sound
 * (+0x1630, unless -1) as either foot comes down; 1 once it has run off */
static inline __attribute__((always_inline)) s32 animal_step(u8 *self) {
    f32 d[4] __attribute__((aligned(16)));
    void *m;
    u8 done = func_0032D430(self) & 0xFF;
    u8 l, r;

    func_001F6370(PTR(self, 0xF0), d, 0.0f);
    func_00125900(self);
    sceVu0ApplyMatrix(d, (f32 (*)[4])(self + 0x60), d);
    sceVu0AddVector(CHAR_POS(self), CHAR_POS(self), d);
    m = PTR(self, 0xF0);
    l = VCALL(m, 0x64, s32 (*)(void *, s32, s32))(m, 1, 0) & 0xFF;
    m = PTR(self, 0xF0);
    r = VCALL(m, 0x64, s32 (*)(void *, s32, s32))(m, 0, 0) & 0xFF;
    if (((l == 1 && self[0x16A8] == 0) || (r == 1 && self[0x16A9] == 0)) && S32(self, 0x1630) != -1) {
        func_00122C20(self, S32(self, 0x1630), 6, 0, 0, NULL);
    }
    self[0x16A8] = l;
    self[0x16A9] = r;
    return done;
}

/* the two animals' copies (started by func_0032D270 / func_0032D3E0) */
s32 func_0032D150(u8 *self) {
    return animal_step(self);
}

s32 func_0032D2C0(u8 *self) {
    return animal_step(self);
}

void *func_0032DC50(void) {
    return D_0042CA20;
}

void *func_0032DC60(void) {
    return D_0042CB60;
}

void *func_0032DC70(void) {
    return D_0042CC00;
}

void *func_0032DC80(void) {
    return D_0042D0C0;
}

void *func_0032DC90(void *self, s32 i) {
    return D_0042E310[i];
}

void *func_0032DCB0(void) {
    return D_0042E410;
}

void *func_0032DCC0(void *self, s32 i) {
    return D_0042E3F0[i];
}

void func_0032F4E0(u8 *self) {
    S32(self, 0xFC8) = 0;
    self[0xFCC] = 0;
    S32(self, 0xFC0) = 0;
    S32(self, 0xFC4) = 0;

    S64(self, 0xC18) = -1;
    S32(self, 0xC24) = 0;
    S32(self, 0xC28) = 0;
    S32(self, 0xC2C) = 0;
    S32(self, 0xC30) = 25;
    S16(self, 0xC34) = 0x10;
    S16(self, 0xC36) = 0x20;
    S16(self, 0xC38) = 0x40;
    S16(self, 0xC3A) = 0x20;
    S16(self, 0xC3C) = 0x20;
    S16(self, 0xC3E) = 0x200;
    S16(self, 0xC40) = 0x100;
    self[0xC42] = 0x40;
    self[0xC43] = 1;
    self[0xC44] = 1;
    self[0xC45] = 0x10;
    self[0xC46] = 0xFF;

    S64(self, 0xC50) = -1;
    S32(self, 0xC5C) = 0;
    S32(self, 0xC60) = 0;
    S32(self, 0xC64) = 0;
    S32(self, 0xC68) = 25;
    S16(self, 0xC6C) = 0x10;
    S16(self, 0xC6E) = 0xE;
    S16(self, 0xC70) = 0x6E;
    S16(self, 0xC72) = 4;
    S16(self, 0xC74) = 4;
    S16(self, 0xC76) = 0x200;
    S16(self, 0xC78) = 0x100;
    self[0xC7A] = 0x40;
    self[0xC7B] = 1;
    self[0xC7C] = 1;
    self[0xC7D] = 0x10;
    self[0xC7E] = 0xFF;
}

void *func_0032F6C0(void) {
    return D_0042E4C0;
}
