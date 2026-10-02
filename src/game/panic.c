/* Fiona's panic (SceneGame +0x7F8): +0x4 the level (0..100; at 100 she panics), made of a
 * lasting part (+0x1C) and a passing one (+0xC); each frame the fear inputs (+0x10.. +0x2C,
 * mode +0x1) are folded in and cleared. */
#include "common.h"
#include "game.h"
#include "progress.h"

extern VObject *D_0044E988;   /* the item manager */
extern u8 *gCharPlayer;
extern void func_002EF580(u8 *o);
extern void func_002EFBE0(u8 *o);

typedef union {
    u32 u;
    f32 f;
} F32Bits;

#define PANIC_MAX 100.0f


/* the per-frame update */
void func_002F0500(u8 *o) {
    static const F32Bits k06 = {0x3F19999A}, k08 = {0x3F4CCCCD}, k16 = {0x3FCCCCCD},
                         k09 = {0x3F666666};
    Progress *p = gProgress;
    f32 scale, a, b, c, d;

    if (Progress_TestFlag(p, 8)) {
        AT(o, 0x34, s32) = 0;
    }
    func_002EF580(o);
    if (AT(o, 0x8, s16) != 0) {
        AT(o, 0x8, s16)--;
    }
    if (AT(o, 0x4, f32) < PANIC_MAX) {
        func_002EFBE0(o);
        scale = AT(p, 0x9EC, s32) != 0 ? AT(p, 0x9E8, f32) : 1.0f;
        a = AT(o, 0x10, f32);
        b = AT(o, 0x28, f32);
        c = AT(o, 0x18, f32);
        d = AT(o, 0x20, f32);
        switch (AT(o, 0x1, u8)) {
        case 0:
            d = d + AT(o, 0x24, f32);
            a = a + AT(o, 0x14, f32);
            b = b + AT(o, 0x2C, f32);
            break;
        case 1:
            d = d + k06.f * AT(o, 0x24, f32);
            a = a + k06.f * AT(o, 0x14, f32);
            b = b + 2.0f * AT(o, 0x2C, f32);
            break;
        case 2:
            d = d + k08.f * AT(o, 0x24, f32);
            a = a + k08.f * AT(o, 0x14, f32);
            b = b + k16.f * AT(o, 0x2C, f32);
            break;
        case 3:
            d = d + k09.f * AT(o, 0x24, f32);
            a = a + k09.f * AT(o, 0x14, f32);
            b = b + 1.5f * AT(o, 0x2C, f32);
            break;
        }
        d = d * scale;
        a = a * scale;
        switch (VCALL(D_0044E988, 0x10, s32 (*)(VObject *, s32))(D_0044E988, 1)) {
        case 0x86:
        case 0x87:
            b = b * 1.25f;
            break;
        case 0x88:
            b = b * 2.0f;
            break;
        }
        AT(o, 0xC, f32) = AT(o, 0xC, f32) - c;
        if (AT(o, 0xC, f32) < 0.0f) {
            AT(o, 0xC, f32) = 0.0f;
        }
        AT(o, 0x1C, f32) = AT(o, 0x1C, f32) - b;
        if (AT(o, 0x1C, f32) < 0.0f) {
            AT(o, 0x1C, f32) = 0.0f;
        }
        AT(o, 0x1C, f32) = AT(o, 0x1C, f32) + d;
        if (a <= 0.0f) {
            AT(o, 0x4, f32) = AT(o, 0x1C, f32) + AT(o, 0xC, f32);
            if (!(AT(o, 0x4, f32) < PANIC_MAX)) {
                AT(o, 0x4, f32) = 99.0f;
                AT(o, 0x1C, f32) = 99.0f;
            }
        } else {
            AT(o, 0x1C, f32) = AT(o, 0x1C, f32) + AT(o, 0xC, f32);
            AT(o, 0xC, f32) = a;
            AT(o, 0x4, f32) = AT(o, 0x1C, f32) + AT(o, 0xC, f32);
        }
        /* she can only reach 100 under flag 0x15, or as Hewie's partner away from her room */
        if ((u8)Progress_TestFlag(p, 0x15) == 1 ||
            (AT(p, 0x1FBEC1, u8) == 1 &&
             AT(gCharPlayer, 0x30, s32) != VCALL(p, 0xC, s32 (*)(Progress *))(p))) {
            if (!(AT(o, 0x4, f32) < PANIC_MAX)) {
                AT(o, 0x4, f32) = 99.0f;
                AT(o, 0x1C, f32) = 99.0f;
                AT(o, 0xC, f32) = 0.0f;
            }
        }
    }
    AT(o, 0x1, u8) = 0;
    AT(o, 0x10, f32) = 0.0f;
    AT(o, 0x14, f32) = 0.0f;
    AT(o, 0x20, f32) = 0.0f;
    AT(o, 0x24, f32) = 0.0f;
    AT(o, 0x28, f32) = 0.0f;
    AT(o, 0x2C, f32) = 0.0f;
    AT(o, 0x20, f32) = 0.0f;
    AT(o, 0x24, f32) = 0.0f;
    AT(o, 0x18, f32) = 0.0f;
}
