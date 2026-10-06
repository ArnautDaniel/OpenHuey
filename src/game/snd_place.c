/* Placing a sound in 3D (PS2 0x002FF4B0..): the sound driver's 3D block (driver +0x10, its
 * interface's method +0xBC) gets the sound's position and the camera's, in the camera's view
 * (x mirrored), with the distance curves D_0041D820; then the sound is played. */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "snd_place.h"

extern u8 D_0041D820[];       /* the 3D distance curves */

/* fill the 3D block for a sound at `pos` */
/* 0x002FF4B0 */
void Sound_SetPosition(VObject *snd, f32 *pos) {
    u8 *b = VCALL(snd, 0xBC, u8 *(*)(VObject *))(snd);
    VObject *cam = gCamera;
    f32 m[4][4] __attribute__((aligned(16)));
    s32 i;

    VCALL(cam, 0x60, void (*)(VObject *, f32 (*)[4]))(cam, m);
    sceVu0CopyVector((f32 *)(b + 0x10), pos);
    AT(b, 0x1C, f32) = 1.0f;
    VCALL(cam, 0x20, void (*)(VObject *, f32 *))(cam, (f32 *)(b + 0x20));
    VCALL(cam, 0x2C, void (*)(VObject *, f32 *))(cam, (f32 *)(b + 0x30));
    VCALL(cam, 0x20, void (*)(VObject *, f32 *))(cam, (f32 *)(b + 0x40));
    for (i = 0; i < 4; i++) {
        sceVu0ApplyMatrix((f32 *)(b + 0x10 + i * 0x10), m, (f32 *)(b + 0x10 + i * 0x10));
    }
    for (i = 0; i < 4; i++) {
        AT(b, 0x10 + i * 0x10, f32) = AT(b, 0x10 + i * 0x10, f32) * -1.0f;
    }
    for (i = 0; i < 4; i++) {
        AT(b, 0x1C + i * 0x10, f32) = 1.0f;
    }
    AT(b, 0x50, u8 *) = D_0041D820;
    AT(b, 0x54, u8) = 0;
    AT(b, 0x55, u8) = 0;
}

/* positioned sound `which` (D_003D8990: footsteps and the like) at `pos` */
/* 0x002FF600 */
void Sound_PlayAt(VObject *snd, u32 which, f32 *pos) {
    Sound_SetPosition(snd, pos);
    VCALL(snd, 0xB8, void (*)(VObject *, u32))(snd, which);
}

/* sound `id` of bank `bank` at `pos` (`vol`, `pitch` offsets) */
/* 0x002FF650 */
void Sound_PlayBankAt(VObject *snd, u32 id, u32 bank, f32 *pos, s32 vol, s32 pitch) {
    Sound_SetPosition(snd, pos);
    VCALL(snd, 0xB4, void (*)(VObject *, u32, u32, s32, s32))(snd, id, bank, vol, pitch);
}
