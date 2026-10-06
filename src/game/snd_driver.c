/* The sound driver object (system +0x395D40, vtable D_0046BF20; its sound interface at +4,
 * vtable D_0046BF2C, is the global gSound): banks of sounds loaded into the IOP sound
 * driver, sound effects (2D, or placed in 3D from the block at +0x10), sequences, volumes and
 * the output mode. It talks to SNDDRV.IRX through the EE sound library (snd_lib.c).
 *
 *   +0x10   the 3D block of the next sound (func_0021EB10; func_002FF4B0 fills it)
 *   +0x68   the output mode the sound was placed for
 *   +0x70   module ids (+0x70..+0x7C)
 *   +0x80   the IOP transfer buffer (0x4000 bytes)
 *   +0x84   8 banks, 0x10 each: +0 its header in IOP memory (bit 31: bank n's), +4 its sound
 *           table (or sequence), +8 its samples in sound memory, +0xC type (0 sounds,
 *           1 a sequence), +0xD loaded
 *   +0x104  the output mode (s8: 0 mono)
 *   +0x108  8 sample transfers, 0x18 each: +4 bytes left, +8 from (EE), +0xC to (sound
 *           memory), +0x10 running, +0x14 the IOP buffer
 *   +0x1C8  2 volumes (u16)
 *   +0x1CC  the sound volume, +0x1D0 the master volume, +0x1D4 the 3D sounds' volume, +0x1D8
 *           the 3D distance scale
 *   +0x7DC  the 3D curve tables of banks 4..7 */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "ptmf.h"
#include "sndlib.h"
#include "globals.h"
#include "memcard.h"
#include "navmesh.h"
#include "snd_driver.h"
#include "snd_lib.h"
#include "system.h"
#include "cri/adx.h"
#include "libc.h"
#include "msl.h"
#include "sce/iop.h"
#include "sce/sif.h"

extern u32 D_01970D40[8];      /* the call arguments */
extern u8 D_01970C80[0xB4];    /* a bank's description (command 0xA) */
extern u8 D_003D8990[8][2];    /* the positioned sounds: bank, sound - 0x18 */
extern u32 D_003D8930[8][3];   /* the banks' header / table sizes and sound memory addresses */

#define BANK(d, k) ((d) + 0x84 + (k) * 0x10)
#define XFER(d, k) ((d) + 0x108 + (k) * 0x18)
#define LOADED(d, k) AT(BANK(d, k), 0xD, u8)
#define BANK_TYPE(d, k) AT(BANK(d, k), 0xC, u8)

extern void *D_0046AF90[];
void *func_001BF800(u8 *o, s32 flags);

/* destructor (vtable D_0046AF90) */
void *func_001BF800(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AF90;
        gSound = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}
/* a volume 0..1 as 0..255 / 0..127 (float to int as the EE converts) */
static inline u32 vol_byte(f32 v) {
    return v >= 2147483648.0f ? ((s32)(v - 2147483648.0f) | 0x80000000) : (u32)(s32)v;
}

/* ---- the simple ones ---- */

/* the 3D block */
u8 *func_0020E870(u8 *d) {
    return d + 0x10;
}

/* bank `k` loaded */
u8 func_0020E880(u8 *d, u32 k) {
    return LOADED(d, k & 0xFF);
}

/* sample transfer `k` running */
u8 func_0020E8A0(u8 *d, u32 k) {
    return AT(XFER(d, k & 0xFF), 0x10, u8);
}

/* the output mode */
s8 func_0020E8C0(u8 *d) {
    return AT(d, 0x104, s8);
}

/* send the master volume (sound x master, 0..255) */
void func_0020E8D0(u8 *d) {
    s32 v = (s32)(255.0f * (AT(d, 0x1CC, f32) * AT(d, 0x1D0, f32)));

    if (v < 0) {
        v = 0;
    }
    if (v >= 0x100) {
        v = 0xFF;
    }
    D_01970D40[2] = v;
    func_0021FB70(0x240000, D_01970D40);
}

/* set the master volume (0..1) */
void func_0020E950(u8 *d, f32 v) {
    if (v < 0.0f) {
        v = 0.0f;
    }
    if (!(v <= 1.0f)) {
        v = 1.0f;
    }
    AT(d, 0x1D0, f32) = v;
    VCALL(d, 0x178, void (*)(u8 *))(d);
}

/* set the sound volume (0..1) */
void func_0020E9A0(u8 *d, f32 v) {
    if (v < 0.0f) {
        v = 0.0f;
    }
    if (!(v <= 1.0f)) {
        v = 1.0f;
    }
    AT(d, 0x1CC, f32) = v;
    VCALL(d, 0x178, void (*)(u8 *))(d);
}

/* play positioned sound `which` (D_003D8990: footsteps and the like) at the 3D block */
void func_0020E9F0(u8 *d, u32 which) {
    u8 bank = D_003D8990[which & 0xFF][0];
    u8 sound = D_003D8990[which & 0xFF][1] + 0x18;
    u8 v;

    if (!LOADED(d, bank)) {
        return;
    }
    if (BANK_TYPE(d, bank) == 1) {
        return;
    }
    D_01970D40[0] = bank;
    D_01970D40[2] = 0x2D000000;
    D_01970D40[3] = (sound << 24) & 0xFF000000;
    D_01970D40[4] = bank;
    D_01970D40[6] = (u32)(d + 0x10);
    AT(d, 0x68, s32) = AT(d, 0x104, s8);
    AT(d, 0x10, u8) = vol_byte(127.0f * AT(d, 0x1D4, f32));
    v = AT(d, 0x10, u8);
    if (v >= 0x80) {
        v = 0x7F;
    }
    AT(d, 0x10, u8) = v;
    AT(d, 0x14, f32) = AT(d, 0x1D8, f32);
    func_0021FB70(0x260000, D_01970D40);
}

/* stop every voice (command 0x35, 2) */
void func_0020EB30(u8 *d) {
    D_01970D40[2] = 0;
    D_01970D40[3] = 0xFFFFFF;
    D_01970D40[4] = 2;
    D_01970D40[5] = 0xFF;
    func_0021FB70(0x350000, D_01970D40);
}

/* stop the positioned sounds but those of banks 3 and 4 */
void func_0020EB70(u8 *d) {
    u32 i;

    D_01970D40[2] = 0;
    D_01970D40[3] = 0;
    for (i = 0; i < 8; i++) {
        if (i != 3 && i != 4) {
            D_01970D40[3] |= 1 << D_003D8990[i][1];
        }
    }
    D_01970D40[4] = 0x80000000;
    func_0021FB70(0x290000, D_01970D40);
}

/* stop all the positioned sounds */
void func_0020EC00(u8 *d) {
    u32 i;

    D_01970D40[2] = 0;
    D_01970D40[3] = 0;
    for (i = 0; i < 8; i++) {
        D_01970D40[3] |= 1 << D_003D8990[i][1];
    }
    D_01970D40[4] = 0x80000000;
    func_0021FB70(0x290000, D_01970D40);
}

/* the positioned sounds' state `s` (command 0x35, 2) */
void func_0020EC70(u8 *d, u32 s) {
    u32 i;

    D_01970D40[2] = 0;
    D_01970D40[3] = 0;
    for (i = 0; i < 8; i++) {
        D_01970D40[3] |= 1 << D_003D8990[i][1];
    }
    D_01970D40[4] = 2;
    D_01970D40[5] = s & 0xFF;
    func_0021FB70(0x350000, D_01970D40);
}

/* bank `k`'s file loaded (the loader's state 2) */
s32 func_0020ECF0(u8 *d, u32 k) {
    return VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, (k & 0xFF) | 0x05000000) == 2;
}

/* drop all 8 banks' files */
void func_0020ED30(u8 *d) {
    s32 i;

    for (i = 0; i < 8; i++) {
        VCALL(d, 0x14C, void (*)(u8 *, s32))(d, i);
    }
}

/* drop bank `k`'s file (loader +0x18), its transfer off */
void func_0020EDF0(u8 *d, u32 k) {
    VCALL(gFileLoader, 0x18, void (*)(VObject *, u32))(gFileLoader, (k & 0xFF) | 0x05000000);
    AT(XFER(d, k & 0xFF), 0x10, u8) = 0;
}

/* (loader +0x14) */
void func_0020EE60(u8 *d, u32 k) {
    VCALL(gFileLoader, 0x14, void (*)(VObject *, u32))(gFileLoader, (k & 0xFF) | 0x05000000);
    AT(XFER(d, k & 0xFF), 0x10, u8) = 0;
}

/* load file `file` into `buf` as part `part` (0 header, 2 table, 3 samples; 1 a sequence) of
   bank `k`: the loader's request id, 0 if not queued */
s32 func_0020EED0(u8 *d, const char *file, u32 k, u32 part, void *buf) {
    if ((part & 0xFF) >= 4 || buf == NULL) {
        return 0;
    }
    return VCALL(gFileLoader, 0xC, s32 (*)(VObject *, const char *, u32, u32, void *))(
        gFileLoader, file, ((part & 0xFF) << 28) | 0x80000000 | ((k & 0xFF) << 24), (k & 0xFF) | 0x05000000, buf);
}

/* volume `ch` (0, 1) to `v` (14 bits) */
void func_0020EF50(u8 *d, u32 ch, u32 v) {
    ch &= 0xFF;
    if (ch < 2) {
        AT(d, 0x1C8 + ch * 2, u16) = v & 0x3FFF;
        D_01970D40[2] = ch != 0;
        D_01970D40[3] = 3;
        D_01970D40[5] = D_01970D40[4] = AT(d, 0x1C8 + ch * 2, u16);
        D_01970D40[6] = 0;
        func_0021FB70(0x150000, D_01970D40);
    }
}

u16 func_0020EFD0(u8 *d, u32 ch) {
    ch &= 0xFF;
    return ch < 2 ? AT(d, 0x1C8 + ch * 2, u16) : 0;
}

/* bank `k` still loading (its file not done, or its samples going over) */
s32 func_0020F000(u8 *d, u32 k) {
    u8 running = AT(XFER(d, k & 0xFF), 0x10, u8);

    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, (k & 0xFF) | 0x05000000) == 3
        && running == 0) {
        return 0;
    }
    return 1;
}

/* the output mode (0 mono) */
void func_0020F070(u8 *d, s8 mode) {
    AT(d, 0x104, s8) = mode;
    if (mode == 0) {
        D_01970D40[2] = 0;
        func_001D4750(1);
    } else {
        D_01970D40[2] = 1;
        func_001D4750(0);
    }
    func_0021FB70(0x160000, D_01970D40);
}

/* unload bank `k` (a sequence stopped first) */
void func_0020F0D0(u8 *d, u32 k) {
    if (k >= 8) {
        return;
    }
    if (!LOADED(d, k)) {
        if (AT(XFER(d, k), 0x10, u8) == 1) {
            AT(XFER(d, k), 0x10, u8) = 0;
        }
        return;
    }
    D_01970D40[0] = k;
    if (BANK_TYPE(d, k) != 0) {
        D_01970D40[2] = 1;
        D_01970D40[3] = 0xB;
        func_0021FB70(0x1A0000, D_01970D40);
    }
    func_0021FB70(k | 0xB0000, D_01970C80);
    LOADED(d, k) = 0;
}

extern const char D_00457258[];   /* "DUMMY" */

/* register bank `k` with the driver (reloaded if it was): its header, table / sequence and
   samples; a sequence bank is started on its channel set up */
void func_0020F1B0(u8 *d, u32 k) {
    u8 *b;
    u32 hd;

    if (k >= 8) {
        return;
    }
    b = BANK(d, k);
    if (AT(b, 0x4, u32) == 0 || AT(b, 0x8, u32) == 0) {
        return;
    }
    if (LOADED(d, k)) {
        VCALL(d, 0x12C, void (*)(u8 *, u32))(d, k);
    }
    func_0026EDD0((char *)D_01970C80, 0x80, D_00457258);
    AT(D_01970C80, 0x84, u32) = AT(d, 0x80, u32);
    AT(D_01970C80, 0x98, u32) = 0x4000;
    AT(D_01970C80, 0x88, u32) = AT(b, 0x8, u32);
    hd = AT(b, 0x0, u32);
    if (hd & 0x80000000) {
        hd = AT(BANK(d, hd & 0x7FFFFFFF), 0x0, u32);
    }
    AT(D_01970C80, 0x80, u32) = hd;
    if (BANK_TYPE(d, k) == 0) {
        AT(D_01970C80, 0x8C, u32) = 0;
        AT(D_01970C80, 0x90, u32) = AT(b, 0x4, u32);
        func_0021FB70(k | 0xA0000, D_01970C80);
    } else {
        AT(D_01970C80, 0x8C, u32) = AT(b, 0x4, u32);
        AT(D_01970C80, 0x90, u32) = 0;
        AT(D_01970C80, 0xA8, u8) = k;
        AT(D_01970C80, 0xA9, u8) = 0;
        AT(D_01970C80, 0xAA, u8) = k;
        AT(D_01970C80, 0xAB, u8) = 0;
        func_0021FB70(k | 0xA0000, D_01970C80);
        D_01970D40[0] = k;
        func_0021FB70(0xC0000, D_01970D40);
        func_0021FB70(0xD0000, D_01970D40);
        func_0021FB70(0x170000, D_01970D40);
        D_01970D40[2] = 0xFF;
        func_0021FB70(0x1B0000, D_01970D40);
        D_01970D40[2] = k;
        D_01970D40[3] = 1;
        func_0021FB70(0x380000, D_01970D40);
    }
    LOADED(d, k) = 1;
}

/* the next 0x4000 bytes of a sample transfer (`x`: +0x108..) into sound memory: 1 while it
   goes on (or the last chunk is still on its way), 0 once done */
s32 func_0020F3D0(u8 *x) {
    u32 n;

    if (AT(x, 0x10, u8) == 0) {
        return 1;
    }
    if (func_0021F2D0(0x120000, 1) != 0) {
        return 1;
    }
    n = AT(x, 0x4, u32);
    if (n == 0) {
        AT(x, 0x10, u8) = 0;
        return 0;
    }
    if (n >= 0x4000) {
        AT(x, 0x4, u32) -= 0x4000;
        n = 0x4000;
    } else {
        AT(x, 0x4, u32) = 0;
    }
    func_0021F840(AT(x, 0x8, u32), AT(x, 0x14, u32), n, 0, 0);
    D_01970D40[2] = AT(x, 0x14, u32);
    D_01970D40[3] = AT(x, 0xC, u32);
    D_01970D40[5] = 1;
    D_01970D40[4] = n;
    func_0021F9F0(0x120000, D_01970D40);
    AT(x, 0x8, u32) += n;
    AT(x, 0xC, u32) += n;
    return 1;
}

/* queue bank `k`'s samples (`size` bytes at `src`) for transfer (func_00210230 does it) */
void func_0020F4D0(u8 *d, u32 k, u32 src, u32 size) {
    u8 *x;
    u32 spu;

    if (k >= 8 || src == 0 || size == 0 || AT(d, 0x80, u32) == 0) {
        return;
    }
    x = XFER(d, k);
    if (AT(x, 0x10, u8) != 0) {
        return;
    }
    spu = AT(BANK(d, k), 0x8, u32);
    if (spu == 0) {
        return;
    }
    AT(x, 0x4, u32) = size;
    AT(x, 0x8, u32) = src;
    AT(x, 0xC, u32) = spu;
    AT(x, 0x10, u8) = 1;
    AT(x, 0x14, u32) = AT(d, 0x80, u32);
    AT(D_01970C80, 0x84, u32) = AT(d, 0x80, u32);
    AT(D_01970C80, 0x98, u32) = 0x4000;
    AT(D_01970C80, 0x88, u32) = spu;
}

/* bank `k`'s samples (`size` bytes at `src`) into sound memory now */
void func_0020F570(u8 *d, u32 k, u32 src, u32 size) {
    u32 spu, to, left, n;

    if (k >= 8 || src == 0 || size == 0 || AT(d, 0x80, u32) == 0) {
        return;
    }
    spu = AT(BANK(d, k), 0x8, u32);
    if (spu == 0) {
        return;
    }
    for (to = spu, left = size; left != 0; src += n, to += n) {
        if (left >= 0x4000) {
            n = 0x4000;
            left -= 0x4000;
        } else {
            n = left;
            left = 0;
        }
        func_0021F840(src, AT(d, 0x80, u32), n, 0, 0);
        D_01970D40[2] = AT(d, 0x80, u32);
        D_01970D40[5] = 1;
        D_01970D40[3] = to;
        D_01970D40[4] = n;
        func_0021F9F0(0x120000, D_01970D40);
        while (func_0021F2D0(0x120000, 1) != 0) {
        }
    }
    AT(D_01970C80, 0x84, u32) = AT(d, 0x80, u32);
    AT(D_01970C80, 0x98, u32) = 0x4000;
    AT(D_01970C80, 0x88, u32) = spu;
}

/* bank `k`'s sequence (`size` bytes at `src`) into IOP memory */
void func_0020F6C0(u8 *d, u32 k, u32 src, u32 size) {
    u32 to;

    if (k >= 8 || src == 0 || size == 0) {
        return;
    }
    to = AT(BANK(d, k), 0x4, u32);
    if (to != 0) {
        func_0021F840(src, to, size, 0, 0);
        BANK_TYPE(d, k) = 1;
    }
}

/* bank `k`'s sound table (`size` bytes at `src`) into IOP memory, its 3D curves kept */
void func_0020F740(u8 *d, u32 k, u32 src, u32 size) {
    u32 to;

    if (k >= 8 || src == 0 || size == 0) {
        return;
    }
    to = AT(BANK(d, k), 0x4, u32);
    if (to != 0) {
        func_0021F3D0((u8 *)src, size);
        func_0021F4A0(k, (u8 *)src, size, AT(d, 0x7DC + (k - 4) * 4, u16 *));
        func_0021F840(src, to, size, 0, 0);
        BANK_TYPE(d, k) = 0;
    }
}

/* bank `k`'s header (`size` bytes at `src`) into IOP memory */
void func_0020F810(u8 *d, u32 k, u32 src, u32 size) {
    u32 to;

    if (k >= 8 || src == 0 || size == 0) {
        return;
    }
    to = AT(BANK(d, k), 0x0, u32);
    if (to != 0) {
        func_0021F840(src, to, size, 0, 0);
    }
}

/* ---- sequences (a bank of type 1) ---- */

/* its tempo (0x21) */
u16 func_0020F870(u8 *d, u32 k) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return 0;
    }
    D_01970D40[0] = k;
    return *(u16 *)func_0021FB70(0x210000, D_01970D40);
}

s32 func_0020F8E0(u8 *d, u32 k, u32 tempo) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return -1;
    }
    D_01970D40[0] = k;
    D_01970D40[2] = tempo & 0xFFFF;
    return (s32)func_0021FB70(0x200000, D_01970D40);
}

/* its volume (0x1F) */
u8 func_0020F950(u8 *d, u32 k) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return 0;
    }
    D_01970D40[0] = k;
    return *(u8 *)func_0021FB70(0x1F0000, D_01970D40);
}

s32 func_0020F9C0(u8 *d, u32 k, u32 v) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return -1;
    }
    D_01970D40[0] = k;
    D_01970D40[2] = v & 0xFF;
    return (s32)func_0021FB70(0x1E0000, D_01970D40);
}

s32 func_0020FA30(u8 *d, u32 k, u32 a, u32 b) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return -1;
    }
    D_01970D40[0] = k;
    D_01970D40[3] = b & 0xFF;
    D_01970D40[2] = a & 0xFF;
    return (s32)func_0021FB70(0x1C0000, D_01970D40);
}

s32 func_0020FAB0(u8 *d, u32 k, u32 a) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return -1;
    }
    D_01970D40[0] = k;
    D_01970D40[2] = a & 0xFF;
    return (s32)func_0021FB70(0x1B0000, D_01970D40);
}

/* func_0020FA30 with the second argument read from `p` */
s32 func_0020FB20(u8 *d, u32 k, s32 *p) {
    if (p == NULL) {
        return -1;
    }
    return VCALL(d, 0xF0, s32 (*)(u8 *, u32, s32))(d, k, *p);
}

s32 func_0020FB50(u8 *d, u32 k, u32 a, u32 b) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return -1;
    }
    D_01970D40[2] = k;
    D_01970D40[3] = a;
    D_01970D40[4] = b & 0xFF;
    return (s32)func_0021FB70(0x230000, D_01970D40);
}

s32 func_0020FBD0(u8 *d, u32 k, u32 a) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return -1;
    }
    D_01970D40[2] = k;
    D_01970D40[3] = a & 0xFF;
    return (s32)func_0021FB70(0x380000, D_01970D40);
}

/* restart from `pos`, keeping its volume and tempo */
void func_0020FC40(u8 *d, u32 k, u32 pos) {
    u8 vol;
    u32 tempo;

    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return;
    }
    D_01970D40[0] = k;
    vol = *(u8 *)func_0021FB70(0x1F0000, D_01970D40);
    tempo = *func_0021FB70(0x210000, D_01970D40);
    D_01970D40[2] = pos;
    func_0021FB70(0x180000, D_01970D40);
    D_01970D40[2] = vol;
    func_0021FB70(0x1E0000, D_01970D40);
    D_01970D40[2] = tempo;
    func_0021FB70(0x200000, D_01970D40);
}

/* stop */
void func_0020FD20(u8 *d, u32 k) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return;
    }
    D_01970D40[0] = k;
    D_01970D40[2] = 0;
    D_01970D40[3] = 0xB;
    func_0021FB70(0x1A0000, D_01970D40);
}

/* play (from `pos` unless negative) with `mode` */
void func_0020FD90(u8 *d, u32 k, u32 pos, u32 mode) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return;
    }
    D_01970D40[0] = k;
    if (!(pos & 0x80000000)) {
        D_01970D40[2] = pos;
        func_0021FB70(0x180000, D_01970D40);
    }
    D_01970D40[2] = mode & 0xFF;
    func_0021FB70(0x190000, D_01970D40);
}

/* ---- sound effects ---- */

/* stop sound `id` of bank `k` */
void func_0020FE30(u8 *d, u32 id, u32 k) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 1) {
        return;
    }
    D_01970D40[0] = k;
    D_01970D40[2] = id | 0x80000000;
    func_0021FB70(0x280000, D_01970D40);
}

/* play sound `id` of bank `k` placed by the 3D block (`vol`, `pitch`: offsets; id bit 30:
   also by the progress's +0x1118 volume) */
void func_0020FEA0(u8 *d, u32 id, u32 k, s8 vol, s8 pitch) {
    u8 v;

    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 1) {
        return;
    }
    D_01970D40[0] = k;
    D_01970D40[2] = (id & 0xFFFFFF) | 0x2C500000;
    if (!(id & 0x80000000)) {
        D_01970D40[2] |= 0x80000000;
    }
    D_01970D40[4] = k;
    D_01970D40[3] = ((vol << 8) & 0xFF00) | ((pitch << 16) & 0xFF0000);
    D_01970D40[6] = (u32)(d + 0x10);
    AT(d, 0x68, s32) = AT(d, 0x104, s8);
    AT(d, 0x10, u8) = vol_byte(127.0f * AT(d, 0x1D4, f32));
    if (id & 0x40000000) {
        AT(d, 0x10, u8) = vol_byte(127.0f * AT(d, 0x1D4, f32) * AT(gProgress, 0x1118, f32));
    }
    v = AT(d, 0x10, u8);
    if (v >= 0x80) {
        v = 0x7F;
    }
    AT(d, 0x10, u8) = v;
    AT(d, 0x14, f32) = AT(d, 0x1D8, f32);
    func_0021FB70(0x260000, D_01970D40);
}

/* play sound `id` of bank `k` unplaced */
void func_00210070(u8 *d, u32 id, u32 k) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 1) {
        return;
    }
    D_01970D40[0] = k;
    D_01970D40[2] = (id & 0xFFFFFF) | 0x04000000;
    if (!(id & 0x80000000)) {
        D_01970D40[2] |= 0x80000000;
    }
    D_01970D40[4] = k;
    D_01970D40[3] = 0x7840;
    func_0021FB70(0x260000, D_01970D40);
}

/* stop the voices in masks `a`, `b` */
void func_00210120(u8 *d, u32 a, u32 b) {
    D_01970D40[2] = a & 0xFFFFFF;
    D_01970D40[3] = b & 0xFFFFFF;
    D_01970D40[4] = 0x80000000;
    func_0021FB70(0x290000, D_01970D40);
}

/* stop all voices */
void func_00210160(u8 *d) {
    VCALL(d, 0xD0, void (*)(u8 *, u32, u32))(d, 0xFFFFFF, 0xFFFFFF);
}

/* ---- life ---- */

/* free the IOP buffers */
void func_00210180(u8 *d) {
    u32 i;

    if (AT(d, 0x80, u32) != 0) {
        func_00274640(AT(d, 0x80, u32));
        AT(d, 0x80, u32) = 0;
    }
    for (i = 0; i < 8; i++) {
        u8 *b = BANK(d, i);

        if (AT(b, 0x0, u32) != 0) {
            if (!(AT(b, 0x0, u32) & 0x80000000)) {
                func_00274640(AT(b, 0x0, u32));
            }
            AT(b, 0x0, u32) = 0;
        }
        if (AT(b, 0x4, u32) != 0) {
            func_00274640(AT(b, 0x4, u32));
            AT(b, 0x4, u32) = 0;
        }
    }
}

/* a frame: the first sample transfer in progress goes on; once it is done its bank (of sounds)
   is registered */
void func_00210230(u8 *d) {
    u32 i;

    for (i = 0; i < 8; i++) {
        if (AT(XFER(d, i), 0x10, u8) != 0) {
            if (!(func_0020F3D0(XFER(d, i)) & 0xFF) && BANK_TYPE(d, i) == 0) {
                VCALL(d, 0x128, void (*)(u8 *, u32))(d, i);
            }
            return;
        }
    }
}

extern const char D_00457260[], D_00457270[], D_00457280[], D_00457290[];   /* the modules */
extern char D_01970B10[0x100];   /* the driver's arguments */
extern u8 D_01970C40[0x1C];      /* ... before formatting */

/* start: load the sound modules and the driver, connect, set it up; IOP memory for the banks */
void func_002102E0(u8 *d) {
    u32 i;

    AT(d, 0x70, s32) = func_001BC0F0(d, D_00457260, 0, 0, 0);
    AT(d, 0x74, s32) = func_001BC0F0(d, D_00457270, 0, 0, 0);
    AT(d, 0x78, s32) = func_001BC0F0(d, D_00457280, 0, 0, 0);
    func_00220340();
    func_00220150(0xA, 0x2000);
    AT(D_01970C40, 0x0, s32) = -1;
    AT(D_01970C40, 0x4, s32) = 0x1FFFFF;
    AT(D_01970C40, 0x8, s32) = 0;
    AT(D_01970C40, 0xC, s32) = 0x1047;
    AT(D_01970C40, 0x10, s16) = 0x1B;
    AT(D_01970C40, 0x12, s16) = 0x1B;
    AT(D_01970C40, 0x14, s16) = 0x1B;
    AT(D_01970C40, 0x16, s16) = 0x1B;
    AT(D_01970C40, 0x18, s16) = 0x17;
    AT(D_01970C40, 0x1A, s16) = 1;
    AT(d, 0x7C, s32) = func_001BC0F0(d, D_00457290, func_00220440(D_01970B10, D_01970C40), (s32)D_01970B10, 0);
    func_00220270();
    func_00220210();
    D_01970D40[2] = 1;
    func_0021FB70(0x80000, D_01970D40);
    D_01970D40[2] = 0;
    D_01970D40[3] = 3;
    D_01970D40[4] = 0;
    D_01970D40[5] = 0;
    D_01970D40[6] = 1;
    func_0021FB70(0x150000, D_01970D40);
    D_01970D40[2] = 1;
    D_01970D40[4] = 0;
    D_01970D40[3] = 3;
    D_01970D40[6] = 1;
    D_01970D40[5] = 0;
    func_0021FB70(0x150000, D_01970D40);
    AT(d, 0x1CA, s16) = 0;
    AT(d, 0x1C8, s16) = 0;
    VCALL(d, 0x130, void (*)(u8 *, s32))(d, 1);
    D_01970D40[2] = 0;
    D_01970D40[3] = 0xFFFFFF;
    func_0021FB70(0x70000, D_01970D40);
    AT(d, 0x80, u32) = func_002744D8(0x4000);
    for (i = 0; i < 8; i++) {
        u8 *b = BANK(d, i);
        u32 hd = D_003D8930[i][0];

        if (hd != 0) {
            if (!(hd & 0x80000000)) {
                if (AT(b, 0x0, u32) == 0) {
                    AT(b, 0x0, u32) = func_002744D8(hd);
                }
            } else {
                AT(b, 0x0, u32) = hd;
            }
        }
        if (D_003D8930[i][1] != 0 && AT(b, 0x4, u32) == 0) {
            AT(b, 0x4, u32) = func_002744D8(D_003D8930[i][1]);
        }
        AT(b, 0x8, u32) = D_003D8930[i][2];
    }
    AT(d, 0x1D0, f32) = 1.0f;
    VCALL(d, 0x170, void (*)(u8 *, f32))(d, 1.0f);
    AT(d, 0x1D4, f32) = 1.0f;
    AT(d, 0x1D8, f32) = 1000.0f;
    func_0021FB70(0x360002, NULL);
}

extern void *D_0046BF20[], *D_0046BF2C[], *D_0046AF90[], *D_0046AD88[];

/* destructor */
u8 *func_0020E000(u8 *d, s32 flags) {
    if (d != NULL) {
        AT(d, 0x0, void **) = D_0046BF20;
        AT(d, 0x4, void **) = D_0046BF2C;
        func_001002C0(d + 0x108, (void * (*)(void *, s32))func_001BECA0, 0x18, 8);
        func_001002C0(d + 0x84, (void * (*)(void *, s32))func_001BEC10, 0x10, 8);
        if (d + 4 != NULL) {
            AT(d, 0x4, void **) = D_0046AF90;
            if (d + 4 != NULL) {
                gSound = NULL;
            }
        }
        if (d != NULL) {
            AT(d, 0x0, void **) = D_0046AD88;
        }
        if ((s16)flags > 0) {
            func_00100490(d);
        }
    }
    return d;
}

/* ---- the sound interface (at +4, gSound): each method the driver's ---- */

#define THUNK(name, ret, target, params, args) \
    ret name params { return target args; }
#define THUNKV(name, target, params, args) \
    void name params { target args; }

THUNK(func_002108E0, u8 *, func_0020E000, (u8 *s, s32 f), (s - 4, f))
THUNKV(func_002108D0, func_00210160, (u8 *s), (s - 4))
THUNKV(func_002108C0, func_00210120, (u8 *s, u32 a, u32 b), (s - 4, a, b))
THUNKV(func_002108B0, func_00210070, (u8 *s, u32 id, u32 k), (s - 4, id, k))
THUNKV(func_002108A0, func_0020FE30, (u8 *s, u32 id, u32 k), (s - 4, id, k))
THUNKV(func_00210890, func_0020FD90, (u8 *s, u32 k, u32 pos, u32 m), (s - 4, k, pos, m))
THUNKV(func_00210880, func_0020FD20, (u8 *s, u32 k), (s - 4, k))
THUNKV(func_00210870, func_0020FC40, (u8 *s, u32 k, u32 pos), (s - 4, k, pos))
THUNK(func_00210860, s32, func_0020FBD0, (u8 *s, u32 k, u32 a), (s - 4, k, a))
THUNK(func_00210840, s32, func_0020FB50, (u8 *s, u32 k, u32 a, u32 b), (s - 4, k, a, b))
THUNK(func_00210850, s32, func_0020FB20, (u8 *s, u32 k, s32 *p), (s - 4, k, p))
THUNK(func_00210830, s32, func_0020FAB0, (u8 *s, u32 k, u32 a), (s - 4, k, a))
THUNK(func_00210820, s32, func_0020FA30, (u8 *s, u32 k, u32 a, u32 b), (s - 4, k, a, b))
THUNK(func_00210810, s32, func_0020F9C0, (u8 *s, u32 k, u32 v), (s - 4, k, v))
THUNK(func_00210800, u8, func_0020F950, (u8 *s, u32 k), (s - 4, k))
THUNK(func_002107F0, s32, func_0020F8E0, (u8 *s, u32 k, u32 t), (s - 4, k, t))
THUNK(func_002107E0, u16, func_0020F870, (u8 *s, u32 k), (s - 4, k))
THUNKV(func_002107D0, func_0020F810, (u8 *s, u32 k, u32 src, u32 n), (s - 4, k, src, n))
THUNKV(func_002107C0, func_0020F740, (u8 *s, u32 k, u32 src, u32 n), (s - 4, k, src, n))
THUNKV(func_002107B0, func_0020F6C0, (u8 *s, u32 k, u32 src, u32 n), (s - 4, k, src, n))
THUNKV(func_002107A0, func_0020F570, (u8 *s, u32 k, u32 src, u32 n), (s - 4, k, src, n))
THUNKV(func_00210790, func_0020F4D0, (u8 *s, u32 k, u32 src, u32 n), (s - 4, k, src, n))
THUNKV(func_00210780, func_0020F1B0, (u8 *s, u32 k), (s - 4, k))
THUNKV(func_00210770, func_0020F0D0, (u8 *s, u32 k), (s - 4, k))
THUNKV(func_00210760, func_0020F070, (u8 *s, s8 m), (s - 4, m))
THUNK(func_00210750, s8, func_0020E8C0, (u8 *s), (s - 4))
THUNK(func_00210740, u8, func_0020E8A0, (u8 *s, u32 k), (s - 4, k))
THUNK(func_00210730, s32, func_0020F000, (u8 *s, u32 k), (s - 4, k))
THUNK(func_00210720, u16, func_0020EFD0, (u8 *s, u32 ch), (s - 4, ch))
THUNKV(func_00210710, func_0020EF50, (u8 *s, u32 ch, u32 v), (s - 4, ch, v))
THUNK(func_00210700, s32, func_0020EED0, (u8 *s, const char *f, u32 k, u32 p, void *buf), (s - 4, f, k, p, buf))
THUNKV(func_002106E0, func_0020EE60, (u8 *s, u32 k), (s - 4, k))
THUNKV(func_002106D0, func_0020EDF0, (u8 *s, u32 k), (s - 4, k))
THUNKV(func_002106F0, func_0020ED30, (u8 *s), (s - 4))
THUNK(func_002106C0, s32, func_0020ECF0, (u8 *s, u32 k), (s - 4, k))
THUNKV(func_002106B0, func_0020EC70, (u8 *s, u32 st), (s - 4, st))
THUNKV(func_002106A0, func_0020EC00, (u8 *s), (s - 4))
THUNKV(func_00210690, func_0020EB70, (u8 *s), (s - 4))
THUNKV(func_00210680, func_0020EB30, (u8 *s), (s - 4))
THUNK(func_00210670, u8, func_0020E880, (u8 *s, u32 k), (s - 4, k))
THUNKV(func_00210650, func_0020E9A0, (u8 *s, f32 v), (s - 4, v))
THUNKV(func_00210640, func_0020E950, (u8 *s, f32 v), (s - 4, v))
THUNKV(func_00210630, func_0020E8D0, (u8 *s), (s - 4))
THUNKV(func_00210620, func_0020FEA0, (u8 *s, u32 id, u32 k, s8 v, s8 p), (s - 4, id, k, v, p))
THUNKV(func_00210660, func_0020E9F0, (u8 *s, u32 w), (s - 4, w))
THUNK(func_00210610, u8 *, func_0020E870, (u8 *s), (s - 4))
