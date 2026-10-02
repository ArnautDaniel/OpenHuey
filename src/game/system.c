/* The game's system object (Game +0x69AC0, vtable 0x46ADF0, ticked every frame) and the
 * objects it embeds: file loader, ... Constructors so far. */
#include "common.h"
#include "game.h"
#include "ptmf.h"

#define AT(p, off, type) (*(type *)((u8 *)(p) + (off)))

extern void *D_0044E978;   /* the Game (set by its base constructor) */
extern void *D_0046BEE0[];

/* Game's base class constructor. */
void *func_0020E7F0(Game *game) {
    D_0044E978 = game;
    game->vtbl = D_0046BEE0;
    game->nextMode = 0;
    game->softResetEnabled = 0;
    game->modeParam = 0;
    return game;
}

/* Element constructors for two arrays in the +0x395D40 object. */
void *func_0020E7D0(void *e) {
    AT(e, 0x8, s32) = 0;
    AT(e, 0x4, s32) = 0;
    AT(e, 0x0, s32) = 0;
    AT(e, 0xC, u8) = 0;
    AT(e, 0xD, u8) = 0;
    return e;
}

void *func_0020E7B0(void *e) {
    AT(e, 0x4, s32) = 0;
    AT(e, 0x0, s32) = 0;
    AT(e, 0xC, s32) = 0;
    AT(e, 0x8, s32) = 0;
    AT(e, 0x14, s32) = 0;
    AT(e, 0x10, u8) = 0;
    return e;
}

/* Global pointers to the parts (set by the constructor). */
extern void *D_0044F7F8;    /* the system object */
extern void *D_0044FEB0;    /* +0x40 */
extern void *D_0044FF00;    /* +0x390 */
extern void *D_0044E4F0;    /* +0x460 */
extern void *D_0044E980;    /* +0x305280 */
extern void *D_0044FEF8;    /* +0x305280 +0x7C44 */
extern void *D_0044E9A0;    /* +0x30CF40 */
extern void *gFileLoader;   /* +0x319900 */
extern void *D_0044E560;    /* +0x395D44 */

extern void *D_0046ADF0[], *D_0046ADD0[], *D_0046ADB0[], *D_0046ADC4[], *D_0046AD88[], *D_0046AE90[], *D_0046AEB4[];
extern void *D_0046AC50[], *D_0046AF00[], *D_0046AF0C[], *D_0046C740[], *D_0046B050[], *D_0046A1E0[];
extern void *D_0046BF20[], *D_0046BF2C[];
extern u8 D_0047E360[], D_0047E3C0[16], D_0047E3D0[16];
extern const PTMF sGameStateNull;

extern void *func_00115D20(void *p, s32 c, u32 n);   /* memset */
extern void func_002D4680(void *obj, void *arg);
extern void func_002D4630(void *obj);
extern void func_001B80C0(void *obj);
extern void func_00100340(void *array, void *(*ctor)(void *), void (*dtor)(void *, s32), u32 size, u32 count);   /* __construct_array */
extern void func_001BEC10(void *, s32), func_001BECA0(void *, s32);   /* the element destructors */

/* System object constructor. */
void *func_0020E340(u8 *s) {
    u8 *p;
    s32 i;

    AT(s, 0x0, void **) = D_0046ADF0;
    D_0044F7F8 = s;
    AT(s, 0x20, void **) = D_0046AD88;
    D_0044FEB0 = s + 0x40;
    AT(s, 0x40, void **) = D_0046ADD0;
    func_002D4680(s + 0x40, D_0047E360);
    for (i = 0; i < 16; i++) {
        D_0047E3C0[i] = i;
    }
    for (i = 0; i < 16; i++) {
        D_0047E3D0[i] = i;
    }
    AT(s, 0x40, void **) = D_0046ADB0;
    AT(s, 0x58, void **) = D_0046ADC4;
    func_002D4630(s + 0x300);

    D_0044FF00 = s + 0x390;
    AT(s, 0x390, void **) = D_0046AE90;
    AT(s, 0x39C, void **) = D_0046AEB4;
    D_0044E4F0 = s + 0x460;
    AT(s, 0x3A4, PTMF) = sGameStateNull;
    AT(s, 0x460, void **) = D_0046AC50;
    func_001B80C0(s + 0x460);

    p = s + 0x305280;
    D_0044E980 = p;
    AT(p, 0x0, void **) = D_0046AF00;
    AT(p, 0x4, s32) = 0;
    AT(p, 0x8, s32) = 0;
    AT(p, 0xC, s32) = 0;
    AT(p, 0x10, u8) = 0;
    AT(p, 0x124, void **) = D_0046AF0C;
    AT(p, 0x7C48, u8) = 0;
    D_0044FEF8 = p + 0x7C44;
    AT(p, 0x7C44, void **) = D_0046C740;
    func_00115D20(p + 0x7C4C, 0, 0x20);
    func_00115D20(p + 0x7C6C, 0, 0x30);

    D_0044E9A0 = s + 0x30CF40;
    AT(s, 0x30CF40, void **) = D_0046B050;
    gFileLoader = s + 0x319900;
    AT(s, 0x319900, void **) = D_0046A1E0;

    p = s + 0x395D40;
    D_0044E560 = p + 4;
    AT(p, 0x0, void **) = D_0046BF20;
    AT(p, 0x4, void **) = D_0046BF2C;
    func_00100340(p + 0x84, func_0020E7D0, func_001BEC10, 0x10, 8);
    func_00100340(p + 0x108, func_0020E7B0, func_001BECA0, 0x18, 8);
    AT(p, 0x80, s32) = 0;
    AT(p, 0x104, s8) = -1;
    AT(p, 0x7EC, s32) = 0x100;
    AT(p, 0x7F0, s32) = 0x100;
    AT(p, 0x7F4, s32) = 0x80;
    AT(p, 0x7F8, s32) = 0x80;
    AT(p, 0x7DC, u8 *) = p + 0x1DC;
    AT(p, 0x7E0, u8 *) = p + 0x3DC;
    AT(p, 0x7E4, u8 *) = p + 0x5DC;
    AT(p, 0x7E8, u8 *) = p + 0x6DC;
    return s;
}
