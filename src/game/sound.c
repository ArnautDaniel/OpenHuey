/* The sound driver object (system +0x395D40, vtable SndDriver_vtable; its sound interface at +4,
 * vtable D_0046BF2C, is the global gSound): banks of sounds loaded into the IOP sound
 * driver, sound effects (2D, or placed in 3D from the block at +0x10), sequences, volumes and
 * the output mode. It talks to SNDDRV.IRX through the EE sound library (snd_lib.c).
 *
 *   +0x10   the 3D block of the next sound (SndLib_Place; Sound_SetPosition fills it)
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
 *   +0x7DC  the 3D curve tables of banks 4..7
 *
 * (was snd_lib.c) Capcom's EE sound library (PS2 0x0021EAC0..0x00220680, include/sndlib.h): calls
 * into the IOP sound driver SNDDRV.IRX by SIF RPC (server 0x77777777, a second one 0x77777778 for
 * the bank transfers), the EE -> IOP DMA, the RPC server the driver calls back (0x77777779), and
 * the 3D placement of a sound (its volume left/right from where it is and how far).
 *
 * (was snd_place.c) Placing a sound in 3D (PS2 0x002FF4B0..): the sound driver's 3D block (driver
 * +0x10, its interface's method +0xBC) gets the sound's position and the camera's, in the
 * camera's view (x mirrored), with the distance curves D_0041D820; then the sound is played.
 */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "ptmf.h"
#include "sndlib.h"
#include "globals.h"
#include "memcard.h"
#include "navmesh.h"
#include "sound.h"
#include "system.h"
#include "cri/adx.h"
#include "libc.h"
#include "msl.h"
#include "sce/iop.h"
#include "sce/sif.h"
#include "sce/libvu0.h"

extern u32 gSndCallArgs[8];      /* the call arguments */
extern u8 gSndCallBlock[0xB4];    /* a bank's description (command 0xA) */
extern u8 kPositionedSounds[8][2];    /* the positioned sounds: bank, sound - 0x18 */
extern u32 kSoundBankSizes[8][3];   /* the banks' header / table sizes and sound memory addresses */

#define BANK(d, k) ((d) + 0x84 + (k) * 0x10)
#define XFER(d, k) ((d) + 0x108 + (k) * 0x18)
#define LOADED(d, k) AT(BANK(d, k), 0xD, u8)
#define BANK_TYPE(d, k) AT(BANK(d, k), 0xC, u8)

extern void *D_0046AF90[];
void *SndDriverBase_dtor(u8 *o, s32 flags);

/* the RPC clients and the argument / result blocks */
extern SifClient gSndClient;   /* 0x77777777 */
extern SifClient gSndClient2;   /* 0x77777778 */
extern u32 gSndResults[0x26];   /* results */
extern u16 *gSndCurves[0x20];  /* per bank: each sound's 3D distance curve (the bank's table) */
extern SifDma gSndDma;
extern u8 gSndDriverState[0x80];    /* the driver's state, written by the IOP (uncached) */
extern u32 gSndTransferBits[4];      /* bit 0 / 1: a transfer (0x11 / 0x12) running (uncached) */
extern u32 D_01975700[8];
extern u32 gSndLastDma;         /* the last DMA's id */
extern u32 gSndIopState;         /* the IOP's copy of gSndDriverState */
extern void (*gSndCallback)(void *);
extern u32 gSoundOutput;         /* the output: 0 stereo, 1 / 2 the surround modes */
extern f32 D_003DF580[0x400], D_003E0580[0x400];   /* stereo pan: right / left by angle */
extern s32 kSurroundIndex[0x168];                      /* by degree: the surround tables' index */
extern f32 D_003E3B20[], D_003E40C0[], D_003E4660[], D_003E4C00[];
extern u16 D_0047B228;     /* the driver's argument 10 (1: it reads EE memory itself) */
extern u32 gSndServerReply;     /* the server's reply */
extern u8 gSndServerQueue[];   /* the server's queue */
extern u8 D_01976F50[];   /* its serve data */
extern u8 D_019726C0[];   /* its receive buffer */
extern s32 gSndServerThreadId;    /* the server thread */
extern s32 D_0047B224;
extern s32 gSndSemaParams[3];      /* the semaphore's parameters */
typedef struct EeThread {
    s32 status;
    void *func;
    void *stack;
    s32 stackSize;
    void *gp;
    s32 prio;
    s32 pad[3];
} EeThread;

extern EeThread gSndServerThread;
extern u8 D_01971680[];        /* the server thread's stack */
extern u8 _gp[];
extern u8 D_019756E4[], D_01976F24[];   /* the clients' server pointers (bound when set) */
extern u8 D_01970D80[0x600], D_01971380[0x10], D_019713C0[0x88], D_01971480[0x60];
extern const char str_0xN[];   /* "%x" (as the driver reads them) */
extern const char D_004572B8[];   /* "%d" */
extern u8 D_0041D820[];       /* the 3D distance curves */
void Sound_SetPosition(VObject *snd, f32 *pos);
void Sound_PlayAt(VObject *snd, u32 which, f32 *pos);
void Sound_PlayBankAt(VObject *snd, u32 id, u32 bank, f32 *pos, s32 vol, s32 pitch);

/* wait out the client's call in progress */
static inline void sif_delay(void) {
    volatile s32 n;

    for (n = 0x2710 - 1; n > 0; n--) {
    }
}

static inline void sif_wait(SifClient *cd) {
    while (sceSifCheckStatRpc(cd) == 1) {
        sif_delay();
    }
}

f32 SndLib_Distance(f32 *a, f32 *b);
u32 SndLib_Place(u8 *b, u32 word);
void SndTable_Unpack(u8 *e, u8 *out);
void SndLib_CallDone(void *p);
void SndLib_TransferDone2(u32 *flags);
void SndLib_TransferDone1(u32 *flags);
void *SndLib_ServerCall(u32 fno, void *buf);
void SndLib_ServerThread(void);

/* destructor (vtable D_0046AF90) */
/* 0x001BF800 */
void *SndDriverBase_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AF90;
        gSound = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(o);
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
/* 0x0020E870 */
u8 *SndDriver_Block3D(u8 *d) {
    return d + 0x10;
}

/* bank `k` loaded */
/* 0x0020E880 */
u8 SndDriver_BankLoaded(u8 *d, u32 k) {
    return LOADED(d, k & 0xFF);
}

/* sample transfer `k` running */
/* 0x0020E8A0 */
u8 SndDriver_TransferRunning(u8 *d, u32 k) {
    return AT(XFER(d, k & 0xFF), 0x10, u8);
}

/* the output mode */
/* 0x0020E8C0 */
s8 SndDriver_OutputMode(u8 *d) {
    return AT(d, 0x104, s8);
}

/* send the master volume (sound x master, 0..255) */
/* 0x0020E8D0 */
void SndDriver_SendMaster(u8 *d) {
    s32 v = (s32)(255.0f * (AT(d, 0x1CC, f32) * AT(d, 0x1D0, f32)));

    if (v < 0) {
        v = 0;
    }
    if (v >= 0x100) {
        v = 0xFF;
    }
    gSndCallArgs[2] = v;
    SndLib_Call(0x240000, gSndCallArgs);
}

/* set the master volume (0..1) */
/* 0x0020E950 */
void SndDriver_SetMaster(u8 *d, f32 v) {
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
/* 0x0020E9A0 */
void SndDriver_SetSound(u8 *d, f32 v) {
    if (v < 0.0f) {
        v = 0.0f;
    }
    if (!(v <= 1.0f)) {
        v = 1.0f;
    }
    AT(d, 0x1CC, f32) = v;
    VCALL(d, 0x178, void (*)(u8 *))(d);
}

/* play positioned sound `which` (kPositionedSounds: footsteps and the like) at the 3D block */
/* 0x0020E9F0 */
void SndDriver_PlayPositioned(u8 *d, u32 which) {
    u8 bank = kPositionedSounds[which & 0xFF][0];
    u8 sound = kPositionedSounds[which & 0xFF][1] + 0x18;
    u8 v;

    if (!LOADED(d, bank)) {
        return;
    }
    if (BANK_TYPE(d, bank) == 1) {
        return;
    }
    gSndCallArgs[0] = bank;
    gSndCallArgs[2] = 0x2D000000;
    gSndCallArgs[3] = (sound << 24) & 0xFF000000;
    gSndCallArgs[4] = bank;
    gSndCallArgs[6] = (u32)(d + 0x10);
    AT(d, 0x68, s32) = AT(d, 0x104, s8);
    AT(d, 0x10, u8) = vol_byte(127.0f * AT(d, 0x1D4, f32));
    v = AT(d, 0x10, u8);
    if (v >= 0x80) {
        v = 0x7F;
    }
    AT(d, 0x10, u8) = v;
    AT(d, 0x14, f32) = AT(d, 0x1D8, f32);
    SndLib_Call(0x260000, gSndCallArgs);
}

/* stop every voice (command 0x35, 2) */
/* 0x0020EB30 */
void SndDriver_StopVoices(u8 *d) {
    gSndCallArgs[2] = 0;
    gSndCallArgs[3] = 0xFFFFFF;
    gSndCallArgs[4] = 2;
    gSndCallArgs[5] = 0xFF;
    SndLib_Call(0x350000, gSndCallArgs);
}

/* stop the positioned sounds but those of banks 3 and 4 */
/* 0x0020EB70 */
void SndDriver_StopPlacedButBanks(u8 *d) {
    u32 i;

    gSndCallArgs[2] = 0;
    gSndCallArgs[3] = 0;
    for (i = 0; i < 8; i++) {
        if (i != 3 && i != 4) {
            gSndCallArgs[3] |= 1 << kPositionedSounds[i][1];
        }
    }
    gSndCallArgs[4] = 0x80000000;
    SndLib_Call(0x290000, gSndCallArgs);
}

/* stop all the positioned sounds */
/* 0x0020EC00 */
void SndDriver_StopPlaced(u8 *d) {
    u32 i;

    gSndCallArgs[2] = 0;
    gSndCallArgs[3] = 0;
    for (i = 0; i < 8; i++) {
        gSndCallArgs[3] |= 1 << kPositionedSounds[i][1];
    }
    gSndCallArgs[4] = 0x80000000;
    SndLib_Call(0x290000, gSndCallArgs);
}

/* the positioned sounds' state `s` (command 0x35, 2) */
/* 0x0020EC70 */
void SndDriver_PlacedState(u8 *d, u32 s) {
    u32 i;

    gSndCallArgs[2] = 0;
    gSndCallArgs[3] = 0;
    for (i = 0; i < 8; i++) {
        gSndCallArgs[3] |= 1 << kPositionedSounds[i][1];
    }
    gSndCallArgs[4] = 2;
    gSndCallArgs[5] = s & 0xFF;
    SndLib_Call(0x350000, gSndCallArgs);
}

/* bank `k`'s file loaded (the loader's state 2) */
/* 0x0020ECF0 */
s32 SndDriver_BankFileLoaded(u8 *d, u32 k) {
    return VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, (k & 0xFF) | 0x05000000) == 2;
}

/* drop all 8 banks' files */
/* 0x0020ED30 */
void SndDriver_DropFiles(u8 *d) {
    s32 i;

    for (i = 0; i < 8; i++) {
        VCALL(d, 0x14C, void (*)(u8 *, s32))(d, i);
    }
}

/* drop bank `k`'s file (loader +0x18), its transfer off */
/* 0x0020EDF0 */
void SndDriver_DropFile(u8 *d, u32 k) {
    VCALL(gFileLoader, 0x18, void (*)(VObject *, u32))(gFileLoader, (k & 0xFF) | 0x05000000);
    AT(XFER(d, k & 0xFF), 0x10, u8) = 0;
}

/* (loader +0x14) */
/* 0x0020EE60 */
void SndDriver_CancelFile(u8 *d, u32 k) {
    VCALL(gFileLoader, 0x14, void (*)(VObject *, u32))(gFileLoader, (k & 0xFF) | 0x05000000);
    AT(XFER(d, k & 0xFF), 0x10, u8) = 0;
}

/* load file `file` into `buf` as part `part` (0 header, 2 table, 3 samples; 1 a sequence) of
   bank `k`: the loader's request id, 0 if not queued */
/* 0x0020EED0 */
s32 SndDriver_LoadPart(u8 *d, const char *file, u32 k, u32 part, void *buf) {
    if ((part & 0xFF) >= 4 || buf == NULL) {
        return 0;
    }
    return VCALL(gFileLoader, 0xC, s32 (*)(VObject *, const char *, u32, u32, void *))(
        gFileLoader, file, ((part & 0xFF) << 28) | 0x80000000 | ((k & 0xFF) << 24), (k & 0xFF) | 0x05000000, buf);
}

/* volume `ch` (0, 1) to `v` (14 bits) */
/* 0x0020EF50 */
void SndDriver_SetVolume(u8 *d, u32 ch, u32 v) {
    ch &= 0xFF;
    if (ch < 2) {
        AT(d, 0x1C8 + ch * 2, u16) = v & 0x3FFF;
        gSndCallArgs[2] = ch != 0;
        gSndCallArgs[3] = 3;
        gSndCallArgs[5] = gSndCallArgs[4] = AT(d, 0x1C8 + ch * 2, u16);
        gSndCallArgs[6] = 0;
        SndLib_Call(0x150000, gSndCallArgs);
    }
}

/* 0x0020EFD0 */
u16 SndDriver_GetVolume(u8 *d, u32 ch) {
    ch &= 0xFF;
    return ch < 2 ? AT(d, 0x1C8 + ch * 2, u16) : 0;
}

/* bank `k` still loading (its file not done, or its samples going over) */
/* 0x0020F000 */
s32 SndDriver_BankLoading(u8 *d, u32 k) {
    u8 running = AT(XFER(d, k & 0xFF), 0x10, u8);

    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, (k & 0xFF) | 0x05000000) == 3
        && running == 0) {
        return 0;
    }
    return 1;
}

/* the output mode (0 mono) */
/* 0x0020F070 */
void SndDriver_SetOutputMode(u8 *d, s8 mode) {
    AT(d, 0x104, s8) = mode;
    if (mode == 0) {
        gSndCallArgs[2] = 0;
        ADXT_SetOutputMono(1);
    } else {
        gSndCallArgs[2] = 1;
        ADXT_SetOutputMono(0);
    }
    SndLib_Call(0x160000, gSndCallArgs);
}

/* unload bank `k` (a sequence stopped first) */
/* 0x0020F0D0 */
void SndDriver_Unload(u8 *d, u32 k) {
    if (k >= 8) {
        return;
    }
    if (!LOADED(d, k)) {
        if (AT(XFER(d, k), 0x10, u8) == 1) {
            AT(XFER(d, k), 0x10, u8) = 0;
        }
        return;
    }
    gSndCallArgs[0] = k;
    if (BANK_TYPE(d, k) != 0) {
        gSndCallArgs[2] = 1;
        gSndCallArgs[3] = 0xB;
        SndLib_Call(0x1A0000, gSndCallArgs);
    }
    SndLib_Call(k | 0xB0000, gSndCallBlock);
    LOADED(d, k) = 0;
}

extern const char str_DUMMY[];   /* "DUMMY" */

/* register bank `k` with the driver (reloaded if it was): its header, table / sequence and
   samples; a sequence bank is started on its channel set up */
/* 0x0020F1B0 */
void SndDriver_Register(u8 *d, u32 k) {
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
    msl_snprintf((char *)gSndCallBlock, 0x80, str_DUMMY);
    AT(gSndCallBlock, 0x84, u32) = AT(d, 0x80, u32);
    AT(gSndCallBlock, 0x98, u32) = 0x4000;
    AT(gSndCallBlock, 0x88, u32) = AT(b, 0x8, u32);
    hd = AT(b, 0x0, u32);
    if (hd & 0x80000000) {
        hd = AT(BANK(d, hd & 0x7FFFFFFF), 0x0, u32);
    }
    AT(gSndCallBlock, 0x80, u32) = hd;
    if (BANK_TYPE(d, k) == 0) {
        AT(gSndCallBlock, 0x8C, u32) = 0;
        AT(gSndCallBlock, 0x90, u32) = AT(b, 0x4, u32);
        SndLib_Call(k | 0xA0000, gSndCallBlock);
    } else {
        AT(gSndCallBlock, 0x8C, u32) = AT(b, 0x4, u32);
        AT(gSndCallBlock, 0x90, u32) = 0;
        AT(gSndCallBlock, 0xA8, u8) = k;
        AT(gSndCallBlock, 0xA9, u8) = 0;
        AT(gSndCallBlock, 0xAA, u8) = k;
        AT(gSndCallBlock, 0xAB, u8) = 0;
        SndLib_Call(k | 0xA0000, gSndCallBlock);
        gSndCallArgs[0] = k;
        SndLib_Call(0xC0000, gSndCallArgs);
        SndLib_Call(0xD0000, gSndCallArgs);
        SndLib_Call(0x170000, gSndCallArgs);
        gSndCallArgs[2] = 0xFF;
        SndLib_Call(0x1B0000, gSndCallArgs);
        gSndCallArgs[2] = k;
        gSndCallArgs[3] = 1;
        SndLib_Call(0x380000, gSndCallArgs);
    }
    LOADED(d, k) = 1;
}

/* the next 0x4000 bytes of a sample transfer (`x`: +0x108..) into sound memory: 1 while it
   goes on (or the last chunk is still on its way), 0 once done */
/* 0x0020F3D0 */
s32 SndDriver_TransferChunk(u8 *x) {
    u32 n;

    if (AT(x, 0x10, u8) == 0) {
        return 1;
    }
    if (SndLib_TransferBusy(0x120000, 1) != 0) {
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
    SndLib_DmaToIop(AT(x, 0x8, u32), AT(x, 0x14, u32), n, 0, 0);
    gSndCallArgs[2] = AT(x, 0x14, u32);
    gSndCallArgs[3] = AT(x, 0xC, u32);
    gSndCallArgs[5] = 1;
    gSndCallArgs[4] = n;
    SndLib_Transfer(0x120000, gSndCallArgs);
    AT(x, 0x8, u32) += n;
    AT(x, 0xC, u32) += n;
    return 1;
}

/* queue bank `k`'s samples (`size` bytes at `src`) for transfer (SndDriver_Frame does it) */
/* 0x0020F4D0 */
void SndDriver_QueueSamples(u8 *d, u32 k, u32 src, u32 size) {
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
    AT(gSndCallBlock, 0x84, u32) = AT(d, 0x80, u32);
    AT(gSndCallBlock, 0x98, u32) = 0x4000;
    AT(gSndCallBlock, 0x88, u32) = spu;
}

/* bank `k`'s samples (`size` bytes at `src`) into sound memory now */
/* 0x0020F570 */
void SndDriver_SamplesNow(u8 *d, u32 k, u32 src, u32 size) {
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
        SndLib_DmaToIop(src, AT(d, 0x80, u32), n, 0, 0);
        gSndCallArgs[2] = AT(d, 0x80, u32);
        gSndCallArgs[5] = 1;
        gSndCallArgs[3] = to;
        gSndCallArgs[4] = n;
        SndLib_Transfer(0x120000, gSndCallArgs);
        while (SndLib_TransferBusy(0x120000, 1) != 0) {
        }
    }
    AT(gSndCallBlock, 0x84, u32) = AT(d, 0x80, u32);
    AT(gSndCallBlock, 0x98, u32) = 0x4000;
    AT(gSndCallBlock, 0x88, u32) = spu;
}

/* bank `k`'s sequence (`size` bytes at `src`) into IOP memory */
/* 0x0020F6C0 */
void SndDriver_Sequence(u8 *d, u32 k, u32 src, u32 size) {
    u32 to;

    if (k >= 8 || src == 0 || size == 0) {
        return;
    }
    to = AT(BANK(d, k), 0x4, u32);
    if (to != 0) {
        SndLib_DmaToIop(src, to, size, 0, 0);
        BANK_TYPE(d, k) = 1;
    }
}

/* bank `k`'s sound table (`size` bytes at `src`) into IOP memory, its 3D curves kept */
/* 0x0020F740 */
void SndDriver_Table(u8 *d, u32 k, u32 src, u32 size) {
    u32 to;

    if (k >= 8 || src == 0 || size == 0) {
        return;
    }
    to = AT(BANK(d, k), 0x4, u32);
    if (to != 0) {
        SndTable_Count((u8 *)src, size);
        SndTable_Curves(k, (u8 *)src, size, AT(d, 0x7DC + (k - 4) * 4, u16 *));
        SndLib_DmaToIop(src, to, size, 0, 0);
        BANK_TYPE(d, k) = 0;
    }
}

/* bank `k`'s header (`size` bytes at `src`) into IOP memory */
/* 0x0020F810 */
void SndDriver_Header(u8 *d, u32 k, u32 src, u32 size) {
    u32 to;

    if (k >= 8 || src == 0 || size == 0) {
        return;
    }
    to = AT(BANK(d, k), 0x0, u32);
    if (to != 0) {
        SndLib_DmaToIop(src, to, size, 0, 0);
    }
}

/* ---- sequences (a bank of type 1) ---- */

/* its tempo (0x21) */
/* 0x0020F870 */
u16 SndDriver_SeqTempo(u8 *d, u32 k) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return 0;
    }
    gSndCallArgs[0] = k;
    return *(u16 *)SndLib_Call(0x210000, gSndCallArgs);
}

/* 0x0020F8E0 */
s32 SndDriver_SetSeqTempo(u8 *d, u32 k, u32 tempo) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return -1;
    }
    gSndCallArgs[0] = k;
    gSndCallArgs[2] = tempo & 0xFFFF;
    return (s32)SndLib_Call(0x200000, gSndCallArgs);
}

/* its volume (0x1F) */
/* 0x0020F950 */
u8 SndDriver_SeqVolume(u8 *d, u32 k) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return 0;
    }
    gSndCallArgs[0] = k;
    return *(u8 *)SndLib_Call(0x1F0000, gSndCallArgs);
}

/* 0x0020F9C0 */
s32 SndDriver_SetSeqVolume(u8 *d, u32 k, u32 v) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return -1;
    }
    gSndCallArgs[0] = k;
    gSndCallArgs[2] = v & 0xFF;
    return (s32)SndLib_Call(0x1E0000, gSndCallArgs);
}

/* 0x0020FA30 */
s32 SndDriver_SeqParam2(u8 *d, u32 k, u32 a, u32 b) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return -1;
    }
    gSndCallArgs[0] = k;
    gSndCallArgs[3] = b & 0xFF;
    gSndCallArgs[2] = a & 0xFF;
    return (s32)SndLib_Call(0x1C0000, gSndCallArgs);
}

/* 0x0020FAB0 */
s32 SndDriver_SeqParam1(u8 *d, u32 k, u32 a) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return -1;
    }
    gSndCallArgs[0] = k;
    gSndCallArgs[2] = a & 0xFF;
    return (s32)SndLib_Call(0x1B0000, gSndCallArgs);
}

/* SndDriver_SeqParam2 with the second argument read from `p` */
/* 0x0020FB20 */
s32 SndDriver_SeqParamPtr(u8 *d, u32 k, s32 *p) {
    if (p == NULL) {
        return -1;
    }
    return VCALL(d, 0xF0, s32 (*)(u8 *, u32, s32))(d, k, *p);
}

/* 0x0020FB50 */
s32 SndDriver_SeqParam2B(u8 *d, u32 k, u32 a, u32 b) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return -1;
    }
    gSndCallArgs[2] = k;
    gSndCallArgs[3] = a;
    gSndCallArgs[4] = b & 0xFF;
    return (s32)SndLib_Call(0x230000, gSndCallArgs);
}

/* 0x0020FBD0 */
s32 SndDriver_SeqParam1B(u8 *d, u32 k, u32 a) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return -1;
    }
    gSndCallArgs[2] = k;
    gSndCallArgs[3] = a & 0xFF;
    return (s32)SndLib_Call(0x380000, gSndCallArgs);
}

/* restart from `pos`, keeping its volume and tempo */
/* 0x0020FC40 */
void SndDriver_SeqRestart(u8 *d, u32 k, u32 pos) {
    u8 vol;
    u32 tempo;

    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return;
    }
    gSndCallArgs[0] = k;
    vol = *(u8 *)SndLib_Call(0x1F0000, gSndCallArgs);
    tempo = *SndLib_Call(0x210000, gSndCallArgs);
    gSndCallArgs[2] = pos;
    SndLib_Call(0x180000, gSndCallArgs);
    gSndCallArgs[2] = vol;
    SndLib_Call(0x1E0000, gSndCallArgs);
    gSndCallArgs[2] = tempo;
    SndLib_Call(0x200000, gSndCallArgs);
}

/* stop */
/* 0x0020FD20 */
void SndDriver_SeqStop(u8 *d, u32 k) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return;
    }
    gSndCallArgs[0] = k;
    gSndCallArgs[2] = 0;
    gSndCallArgs[3] = 0xB;
    SndLib_Call(0x1A0000, gSndCallArgs);
}

/* play (from `pos` unless negative) with `mode` */
/* 0x0020FD90 */
void SndDriver_SeqPlay(u8 *d, u32 k, u32 pos, u32 mode) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 0) {
        return;
    }
    gSndCallArgs[0] = k;
    if (!(pos & 0x80000000)) {
        gSndCallArgs[2] = pos;
        SndLib_Call(0x180000, gSndCallArgs);
    }
    gSndCallArgs[2] = mode & 0xFF;
    SndLib_Call(0x190000, gSndCallArgs);
}

/* ---- sound effects ---- */

/* stop sound `id` of bank `k` */
/* 0x0020FE30 */
void SndDriver_StopSound(u8 *d, u32 id, u32 k) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 1) {
        return;
    }
    gSndCallArgs[0] = k;
    gSndCallArgs[2] = id | 0x80000000;
    SndLib_Call(0x280000, gSndCallArgs);
}

/* play sound `id` of bank `k` placed by the 3D block (`vol`, `pitch`: offsets; id bit 30:
   also by the progress's +0x1118 volume) */
/* 0x0020FEA0 */
void SndDriver_PlayPlaced(u8 *d, u32 id, u32 k, s8 vol, s8 pitch) {
    u8 v;

    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 1) {
        return;
    }
    gSndCallArgs[0] = k;
    gSndCallArgs[2] = (id & 0xFFFFFF) | 0x2C500000;
    if (!(id & 0x80000000)) {
        gSndCallArgs[2] |= 0x80000000;
    }
    gSndCallArgs[4] = k;
    gSndCallArgs[3] = ((vol << 8) & 0xFF00) | ((pitch << 16) & 0xFF0000);
    gSndCallArgs[6] = (u32)(d + 0x10);
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
    SndLib_Call(0x260000, gSndCallArgs);
}

/* play sound `id` of bank `k` unplaced */
/* 0x00210070 */
void SndDriver_Play(u8 *d, u32 id, u32 k) {
    k &= 0xFF;
    if (!LOADED(d, k) || BANK_TYPE(d, k) == 1) {
        return;
    }
    gSndCallArgs[0] = k;
    gSndCallArgs[2] = (id & 0xFFFFFF) | 0x04000000;
    if (!(id & 0x80000000)) {
        gSndCallArgs[2] |= 0x80000000;
    }
    gSndCallArgs[4] = k;
    gSndCallArgs[3] = 0x7840;
    SndLib_Call(0x260000, gSndCallArgs);
}

/* stop the voices in masks `a`, `b` */
/* 0x00210120 */
void SndDriver_StopMasks(u8 *d, u32 a, u32 b) {
    gSndCallArgs[2] = a & 0xFFFFFF;
    gSndCallArgs[3] = b & 0xFFFFFF;
    gSndCallArgs[4] = 0x80000000;
    SndLib_Call(0x290000, gSndCallArgs);
}

/* stop all voices */
/* 0x00210160 */
void SndDriver_StopAll(u8 *d) {
    VCALL(d, 0xD0, void (*)(u8 *, u32, u32))(d, 0xFFFFFF, 0xFFFFFF);
}

/* ---- life ---- */

/* free the IOP buffers */
/* 0x00210180 */
void SndDriver_FreeIop(u8 *d) {
    u32 i;

    if (AT(d, 0x80, u32) != 0) {
        sceSifFreeIopHeap(AT(d, 0x80, u32));
        AT(d, 0x80, u32) = 0;
    }
    for (i = 0; i < 8; i++) {
        u8 *b = BANK(d, i);

        if (AT(b, 0x0, u32) != 0) {
            if (!(AT(b, 0x0, u32) & 0x80000000)) {
                sceSifFreeIopHeap(AT(b, 0x0, u32));
            }
            AT(b, 0x0, u32) = 0;
        }
        if (AT(b, 0x4, u32) != 0) {
            sceSifFreeIopHeap(AT(b, 0x4, u32));
            AT(b, 0x4, u32) = 0;
        }
    }
}

/* a frame: the first sample transfer in progress goes on; once it is done its bank (of sounds)
   is registered */
/* 0x00210230 */
void SndDriver_Frame(u8 *d) {
    u32 i;

    for (i = 0; i < 8; i++) {
        if (AT(XFER(d, i), 0x10, u8) != 0) {
            if (!(SndDriver_TransferChunk(XFER(d, i)) & 0xFF) && BANK_TYPE(d, i) == 0) {
                VCALL(d, 0x128, void (*)(u8 *, u32))(d, i);
            }
            return;
        }
    }
}

extern const char str_MODHSYN_IRX[], str_MODMIDI_IRX[], str_MODMSIN_IRX[], str_SNDDRV_IRX[];   /* the modules */
extern char D_01970B10[0x100];   /* the driver's arguments */
extern u8 gSndDriverArgs[0x1C];      /* ... before formatting */

/* start: load the sound modules and the driver, connect, set it up; IOP memory for the banks */
/* 0x002102E0 */
void SndDriver_Start(u8 *d) {
    u32 i;

    AT(d, 0x70, s32) = Iop_LoadModule(d, str_MODHSYN_IRX, 0, 0, 0);
    AT(d, 0x74, s32) = Iop_LoadModule(d, str_MODMIDI_IRX, 0, 0, 0);
    AT(d, 0x78, s32) = Iop_LoadModule(d, str_MODMSIN_IRX, 0, 0, 0);
    SndLib_Clear();
    SndLib_StartServer(0xA, 0x2000);
    AT(gSndDriverArgs, 0x0, s32) = -1;
    AT(gSndDriverArgs, 0x4, s32) = 0x1FFFFF;
    AT(gSndDriverArgs, 0x8, s32) = 0;
    AT(gSndDriverArgs, 0xC, s32) = 0x1047;
    AT(gSndDriverArgs, 0x10, s16) = 0x1B;
    AT(gSndDriverArgs, 0x12, s16) = 0x1B;
    AT(gSndDriverArgs, 0x14, s16) = 0x1B;
    AT(gSndDriverArgs, 0x16, s16) = 0x1B;
    AT(gSndDriverArgs, 0x18, s16) = 0x17;
    AT(gSndDriverArgs, 0x1A, s16) = 1;
    AT(d, 0x7C, s32) = Iop_LoadModule(d, str_SNDDRV_IRX, SndLib_DriverArgs(D_01970B10, gSndDriverArgs), (s32)D_01970B10, 0);
    SndLib_Bind();
    SndLib_SendState();
    gSndCallArgs[2] = 1;
    SndLib_Call(0x80000, gSndCallArgs);
    gSndCallArgs[2] = 0;
    gSndCallArgs[3] = 3;
    gSndCallArgs[4] = 0;
    gSndCallArgs[5] = 0;
    gSndCallArgs[6] = 1;
    SndLib_Call(0x150000, gSndCallArgs);
    gSndCallArgs[2] = 1;
    gSndCallArgs[4] = 0;
    gSndCallArgs[3] = 3;
    gSndCallArgs[6] = 1;
    gSndCallArgs[5] = 0;
    SndLib_Call(0x150000, gSndCallArgs);
    AT(d, 0x1CA, s16) = 0;
    AT(d, 0x1C8, s16) = 0;
    VCALL(d, 0x130, void (*)(u8 *, s32))(d, 1);
    gSndCallArgs[2] = 0;
    gSndCallArgs[3] = 0xFFFFFF;
    SndLib_Call(0x70000, gSndCallArgs);
    AT(d, 0x80, u32) = sceSifAllocIopHeap(0x4000);
    for (i = 0; i < 8; i++) {
        u8 *b = BANK(d, i);
        u32 hd = kSoundBankSizes[i][0];

        if (hd != 0) {
            if (!(hd & 0x80000000)) {
                if (AT(b, 0x0, u32) == 0) {
                    AT(b, 0x0, u32) = sceSifAllocIopHeap(hd);
                }
            } else {
                AT(b, 0x0, u32) = hd;
            }
        }
        if (kSoundBankSizes[i][1] != 0 && AT(b, 0x4, u32) == 0) {
            AT(b, 0x4, u32) = sceSifAllocIopHeap(kSoundBankSizes[i][1]);
        }
        AT(b, 0x8, u32) = kSoundBankSizes[i][2];
    }
    AT(d, 0x1D0, f32) = 1.0f;
    VCALL(d, 0x170, void (*)(u8 *, f32))(d, 1.0f);
    AT(d, 0x1D4, f32) = 1.0f;
    AT(d, 0x1D8, f32) = 1000.0f;
    SndLib_Call(0x360002, NULL);
}

/* ---- the 3D placement ---- */

/* |a - b| */
/* 0x0021EAC0 */
f32 SndLib_Distance(f32 *a, f32 *b) {
    f32 x = a[0] - b[0];
    f32 y = a[1] - b[1];
    f32 z = a[2] - b[2];

    return __builtin_sqrtf(x * x + y * y + z * z);
}

/* The sound's place: the block (driver +0x10, set by Sound_SetPosition) holds its volume (+0x0,
 * 0..127), the distance scale (+0x4), the sound's position (+0x10), the listener's (+0x20),
 * the listener's facing (+0x30 from +0x40), the distance curves (+0x50: 10 (volume %,
 * distance) pairs each, 0x50 bytes), the sound's curve (+0x54, +8 with +0x55 == 1, +4 from
 * behind) and how its sides combine (+0x58: 0 both the average, 1 both positive, else as
 * placed). The result: left volume << 16 | right (0x2000 full, by the output's pan tables). */
/* 0x0021EB10 */
u32 SndLib_Place(u8 *b, u32 word) {
    f32 vol = AT(b, 0x0, u8) / 127.0f;
    s16 l = 0, r = 0;
    s32 at, facing, rel, deg, k, i, v = 0;
    f32 dist;
    s32 *curve;

    if (b + 0x10 == NULL) {
        return 0;
    }
    at = (s32)(0x1.45f306p+9f /* 4096 / 2pi */ * msl_atan2f(AT(b, 0x10, f32) - AT(b, 0x20, f32),
                                                                  AT(b, 0x18, f32) - AT(b, 0x28, f32))) & 0xFFF;
    dist = SndLib_Distance((f32 *)(b + 0x20), (f32 *)(b + 0x10));
    facing = (s32)(0x1.45f306p+9f * msl_atan2f(AT(b, 0x30, f32) - AT(b, 0x40, f32),
                                                  AT(b, 0x38, f32) - AT(b, 0x48, f32))) & 0xFFF;
    rel = (((facing - at + 0x1000) & 0xFFF) / 2 + 0x200) & 0x7FF;
    k = AT(b, 0x54, u8);
    if (AT(b, 0x55, u8) == 1) {
        k = (u8)(k + 8);
    }
    if (rel > 0x400) {
        k = (u8)(k + 4);
    }
    curve = (s32 *)(AT(b, 0x50, u8 *) + k * 0x50);
    for (i = 1; i < 16; i++) {
        f32 d;

        if (curve[i * 2 + 1] == 0) {
            v = 0;
            break;
        }
        d = AT(b, 0x4, f32) * (f32)curve[i * 2 + 1] / 100.0f;
        if (dist < d) {
            f32 d0 = AT(b, 0x4, f32) * (f32)curve[i * 2 - 1] / 100.0f;

            v = (s32)((f32)curve[i * 2 - 2] + (f32)(curve[i * 2] - curve[i * 2 - 2]) * (dist - d0) / (d - d0));
            break;
        }
    }
    v = (s16)((v << 13) / 100);
    if (gSoundOutput == 0) {
        if (rel > 0x400) {
            l = (s32)(vol * ((f32)v * -D_003E0580[0x800 - rel]));
            r = (s32)(vol * ((f32)v * D_003DF580[0x800 - rel]));
        } else {
            l = (s32)(vol * ((f32)v * D_003E0580[rel]));
            r = (s32)(vol * ((f32)v * D_003DF580[rel]));
        }
    } else {
        deg = rel * 360 / 2048;
        if (gSoundOutput == 1) {
            l = (s32)(vol * ((f32)v * D_003E40C0[kSurroundIndex[deg]]));
            r = (s32)(vol * ((f32)v * D_003E3B20[kSurroundIndex[deg]]));
        } else if (gSoundOutput == 2) {
            l = (s32)(vol * ((f32)v * D_003E4C00[kSurroundIndex[deg]]));
            r = (s32)(vol * ((f32)v * D_003E4660[kSurroundIndex[deg]]));
        }
    }
    switch (AT(b, 0x58, s32)) {
    case 0:
        l = r = ((l < 0 ? -l : l) + (r < 0 ? -r : r)) / 2;
        break;
    case 1:
        l = l < 0 ? -l : l;
        r = r < 0 ? -r : r;
        break;
    }
    (void)word;
    return ((u32)(s32)l << 16) | (u32)(s32)r;
}

/* the driver asks: 1 copy EE memory to it (or, with argument 10 set, just the address), 2..5
   its state words into gSndDriverState (+0x30, +0x20, +0x40, +0x50) */
/* 0x0021F0E0 */
void *SndLib_ServerCall(u32 fno, void *buf) {
    u8 *st = UNCACHED(gSndDriverState);

    switch (fno & 0xFFFF0000) {
    case 0x50000:
        AT(st, 0x50, u32) = AT(buf, 0x8, u32);
        gSndServerReply = 0;
        return &gSndServerReply;
    case 0x40000:
        AT(st, 0x40, u32) = AT(buf, 0x8, u32);
        gSndServerReply = 0;
        return &gSndServerReply;
    case 0x30000:
        AT(st, 0x20, u32) = AT(buf, 0x8, u32);
        gSndServerReply = 0;
        return &gSndServerReply;
    case 0x20000:
        AT(st, 0x30, u32) = AT(buf, 0x8, u32);
        gSndServerReply = 0;
        return &gSndServerReply;
    case 0x10000:
        if (D_0047B228 == 1) {
            return AT(buf, 0x8, void *);
        }
        gSndServerReply = SndLib_DmaToIop(AT(buf, 0x8, u32), AT(buf, 0xC, u32), AT(buf, 0x10, u32), AT(buf, 0x14, s32),
                                   AT(buf, 0x18, s32));
        return &gSndServerReply;
    }
    return NULL;
}

/* the server thread */
/* 0x0021F210 */
void SndLib_ServerThread(void) {
    sceSifSetRpcQueue(gSndServerQueue, gSndServerThreadId);
    func_002703C0(D_01976F50, 0x77777779, SndLib_ServerCall, D_019726C0, NULL, NULL, gSndServerQueue);
    sceSifRpcLoop(gSndServerQueue);
}

/* the end of a nowait call (the SIF interrupt): interrupts back on */
/* 0x0021F280 */
void SndLib_CallDone(void *p) {
    EE_SYNC_EI();
}

/* the end of a transfer 0x12 / 0x11: its running bit off */
/* 0x0021F290 */
void SndLib_TransferDone2(u32 *flags) {
    *flags &= ~2;
    EE_SYNC_EI();
}

/* 0x0021F2B0 */
void SndLib_TransferDone1(u32 *flags) {
    *flags &= ~1;
    EE_SYNC_EI();
}

/* transfer 0x110000 / 0x120000 still running (poll 0: wait for it) */
/* 0x0021F2D0 */
s32 SndLib_TransferBusy(u32 cmd, s32 poll) {
    volatile u32 *flags = UNCACHED(gSndTransferBits);
    u32 bit;

    if (cmd == 0x120000) {
        bit = 2;
    } else if (cmd == 0x110000) {
        bit = 1;
    } else {
        return -1;
    }
    if (poll == 0) {
        while (*flags & bit) {
        }
        return 0;
    }
    return (*flags & bit) != 0;
}

/* the number of sounds in table `t` (`size` bytes; -1 for less than an entry) */
/* 0x0021F3D0 */
s32 SndTable_Count(u8 *t, s32 size) {
    u8 e[0x14] __attribute__((aligned(16)));
    s32 n, i, k;

    if (size < 0x10) {
        return -1;
    }
    for (n = 0;; n++) {
        /* (the n-th sound's first entry, counted from the start each time) */
        for (k = 0, i = 0; k < n; i++) {
            SndTable_Unpack(t + i * 0x10, e);
            if (!(AT(e, 0x0, s32) & 4)) {
                k++;
            }
        }
        if (i * 0x10 >= size) {
            return n;
        }
    }
}

/* bank `bank`'s 3D curves: each sound's (+0xE of its first entry) into `out`, kept for the
   bank (gSndCurves) */
/* 0x0021F4A0 */
void SndTable_Curves(s32 bank, u8 *t, s32 size, u16 *out) {
    u8 e[0x14] __attribute__((aligned(16)));
    u8 f[0x14] __attribute__((aligned(16)));
    u16 *o = out;
    s32 n, i, k;

    if (size < 0x10) {
        return;
    }
    for (n = 0;; n++) {
        for (k = 0, i = 0; k < n; i++) {
            SndTable_Unpack(t + i * 0x10, e);
            if (!(AT(e, 0x0, s32) & 4)) {
                k++;
            }
        }
        if (i * 0x10 >= size) {
            break;
        }
        SndTable_Unpack(t + i * 0x10, f);
        *o++ = f[0xE];
    }
    gSndCurves[bank] = out;
}

/* ---- the driver's sound tables (the SDT files: 16 bytes an entry, an entry with flag 4 goes on
   the sound before it) ---- */

/* entry `e` of a sound table, unpacked (type 1: the 11-byte layout, type 2: 12 bytes with a
   16-bit number) */
/* 0x0021F5C0 */
void SndTable_Unpack(u8 *e, u8 *out) {
    msl_memset(out, 0, 0x14);
    switch (e[0]) {
    case 1:
        out[4] = e[1];
        AT(out, 0x0, s32) |= (e[2] >> 7) & 1;
        out[8] = e[2] & 0x7F;
        AT(out, 0x0, s32) |= (e[3] >> 5) & 4;
        AT(out, 0x0, s32) |= (e[3] >> 3) & 8;
        AT(out, 0x0, s32) |= e[0] != 0 ? 0 : 0x20;
        out[7] = e[3] & 0x3F;
        AT(out, 0x0, s32) |= (e[4] >> 6) & 2;
        out[9] = e[4] & 0x7F;
        out[0xA] = e[5];
        out[0xB] = e[6];
        out[0xC] = (e[7] >> 4) & 0xF;
        out[0xD] = e[7] & 7;
        AT(out, 0x0, s32) |= (e[7] * 2) & 0x10;
        out[5] = e[8] & 0x7F;
        out[6] = e[9] & 0x7F;
        out[0xE] = e[0xA];
        break;
    case 2:
        AT(out, 0x4, s32) = (e[1] << 8) | e[2];
        AT(out, 0x0, s32) |= (e[3] >> 7) & 1;
        out[0xB] = e[3] & 0x7F;
        AT(out, 0x0, s32) |= (e[4] >> 5) & 4;
        AT(out, 0x0, s32) |= (e[4] >> 3) & 8;
        AT(out, 0x0, s32) |= e[0] != 0 ? 0 : 0x20;
        out[0xA] = e[4] & 0x3F;
        AT(out, 0x0, s32) |= (e[5] >> 6) & 2;
        out[0xC] = e[5] & 0x7F;
        out[0xD] = e[6];
        out[0xE] = e[7];
        out[0xF] = (e[8] >> 4) & 0xF;
        out[0x10] = e[8] & 7;
        AT(out, 0x0, s32) |= (e[8] * 2) & 0x10;
        out[8] = e[9] & 0x7F;
        out[9] = e[0xA] & 0x7F;
        out[0x11] = e[0xB];
        break;
    }
}

/* ---- SIF transfers and calls ---- */

/* copy `size` bytes of EE memory at `src` to IOP memory at `dest` and wait for it (inIrq: from
   an interrupt handler): 0, or -1 too big, -2 bad mode, -3 not queued, -4 */
/* 0x0021F840 */
s32 SndLib_DmaToIop(u32 src, u32 dest, u32 size, s32 attr, s32 inIrq) {
    s32 r = 0;

    if (size >= 0xFFFF1) {
        return -1;
    }
    if (inIrq == 1) {
        do {
            gSndDma.src = src;
            gSndDma.dest = dest;
            gSndDma.size = size;
            gSndDma.attr = attr;
            func_0026CB18(src, src + size);
            gSndLastDma = isceSifSetDma(&gSndDma, 1);
        } while (gSndLastDma == 0);
    } else if (inIrq == 0) {
        do {
            gSndDma.src = src;
            gSndDma.dest = dest;
            gSndDma.size = size;
            gSndDma.attr = attr;
            FlushDCacheRange(src, src + size);
            gSndLastDma = sceSifSetDma(&gSndDma, 1);
        } while (gSndLastDma == 0);
    } else {
        return -2;
    }
    if (gSndLastDma == 0) {
        return -3;
    }
    for (;;) {
        if (inIrq == 1) {
            if (isceSifDmaStat(gSndLastDma) < 0) {
                break;
            }
        } else if (inIrq == 0) {
            if (sceSifDmaStat(gSndLastDma) < 0) {
                break;
            }
        } else {
            return -4;
        }
    }
    return r;
}

/* the transfer commands (0x110000 / 0x120000) on the second server, not waited for */
/* 0x0021F9F0 */
void *SndLib_Transfer(u32 cmd, void *args) {
    u32 *flags = UNCACHED(gSndTransferBits);

    sif_wait(&gSndClient2);
    switch (cmd & 0xFFFF0000) {
    case 0x110000:
    case 0x120000:
        if ((cmd & 0xFFFF0000) == 0x110000) {
            gSndCallback = (void (*)(void *))SndLib_TransferDone1;
            *flags |= 1;
        } else {
            gSndCallback = (void (*)(void *))SndLib_TransferDone2;
            *flags |= 2;
        }
        while (sceSifCallRpc(&gSndClient2, cmd, 1, args, 0x20, D_01975700, 4, gSndCallback, flags) != 0) {
        }
        break;
    }
    return D_01975700;
}

/* call the driver: command `cmd` (high 16 bits; low bits its argument) with the 0x20-byte block
   `args`; the result block. The sound commands (0x26, 0x27 a list of 0x20-byte ones) with a 3D
   block (+0x18) get their volumes placed (+0x14) first. 0x36 / 0x37 set / read the output mode
   here. */
/* 0x0021FB70 */
u32 *SndLib_Call(u32 cmd, void *args) {
    u32 n, i;
    u8 *a;

    sif_wait(&gSndClient);
    switch (cmd & 0xFFFF0000) {
    case 0x370000:
        gSndResults[0] = gSoundOutput;
        break;
    case 0x360000:
        if (gSoundOutput < 3) {
            gSoundOutput = cmd & 0xFF;
        } else {
            gSoundOutput = 0;
        }
        break;
    case 0x270000:
        n = cmd & 0xFF;
        if (n == 0) {
            break;
        }
        for (i = 0, a = args; i < n; i++, a += 0x20) {
            u32 w = AT(a, 0x8, u32);

            if ((w & 0x20000000) == 0x20000000 && (w & 0x08000000) == 0x08000000 && AT(a, 0x18, u8 *) != NULL) {
                u16 *curves = gSndCurves[AT(a, 0x0, u16)];

                if (curves != NULL) {
                    AT(AT(a, 0x18, u8 *), 0x54, u8) = curves[w & 0xFFFF];
                }
                AT(a, 0x14, u32) = SndLib_Place(AT(a, 0x18, u8 *), w);
            }
        }
        while (sceSifCallRpc(&gSndClient, cmd, 0, args, n << 5, gSndResults, 4, NULL, NULL) != 0) {
        }
        break;
    case 0x260000: {
        u32 w = AT(args, 0x8, u32);

        if ((w & 0x20000000) == 0x20000000 && (w & 0x08000000) == 0x08000000 && AT(args, 0x18, u8 *) != NULL) {
            u16 *curves = gSndCurves[AT(args, 0x0, u16)];

            if (curves != NULL) {
                AT(AT(args, 0x18, u8 *), 0x54, u8) = curves[w & 0xFFFF];
            }
            AT(args, 0x14, u32) = SndLib_Place(AT(args, 0x18, u8 *), w);
        }
        while (sceSifCallRpc(&gSndClient, cmd, 0, args, 0x20, gSndResults, 8, SndLib_CallDone, NULL) != 0) {
        }
        break;
    }
    case 0x320000:
    case 0x300000:
    case 0x2E0000:
    case 0x2D0000:
    case 0x2C0000:
    case 0x0:
        while (sceSifCallRpc(&gSndClient, cmd, 0, args, 0, gSndResults, 4, NULL, NULL) != 0) {
        }
        break;
    case 0x2B0000:
        while (sceSifCallRpc(&gSndClient, cmd, 1, args, 0x88, gSndResults, 4, SndLib_CallDone, NULL) != 0) {
        }
        break;
    case 0x310000:
    case 0x2A0000:
        while (sceSifCallRpc(&gSndClient, cmd, 0, args, 0x88, gSndResults, 4, NULL, NULL) != 0) {
        }
        break;
    case 0xA0000:
        while (sceSifCallRpc(&gSndClient, cmd, 0, args, 0xB4, gSndResults, 4, NULL, NULL) != 0) {
        }
        break;
    case 0x60000:
        while (sceSifCallRpc(&gSndClient, cmd, 0, args, 0, gSndResults, 8, NULL, NULL) != 0) {
        }
        break;
    case 0x20000:
    case 0x10000:
    case 0x130000:
    case 0x2F0000:
    case 0xB0000:
    case 0x90000:
        while (sceSifCallRpc(&gSndClient, cmd, 0, args, 0, gSndResults, 4, NULL, NULL) != 0) {
        }
        break;
    case 0x140000:
        while (sceSifCallRpc(&gSndClient, cmd, 0, args, 0, gSndResults, 0x1C, NULL, NULL) != 0) {
        }
        break;
    case 0x50000:
        while (sceSifCallRpc(&gSndClient, cmd, 0, args, 0, gSndResults, 0x98, NULL, NULL) != 0) {
        }
        break;
    default:
        while (sceSifCallRpc(&gSndClient, cmd, 0, args, 0x20, gSndResults, 4, NULL, NULL) != 0) {
        }
        break;
    }
    return gSndResults;
}

/* start the EE server (thread priority `prio`, stack `stack` bytes) */
/* 0x00220150 */
void SndLib_StartServer(s32 prio, s32 stack) {
    gSndSemaParams[2] = 0;
    gSndSemaParams[1] = 1;
    D_0047B224 = CreateSema(gSndSemaParams);
    gSndServerThread.func = SndLib_ServerThread;
    gSndServerThread.stack = D_01971680;
    gSndServerThread.stackSize = stack;
    gSndServerThread.prio = prio;
    gSndServerThread.gp = _gp;
    gSndServerThread.pad[2] = 0;
    gSndServerThreadId = CreateThread(&gSndServerThread);
    if (gSndServerThreadId >= 0) {
        Kernel_StartThread(gSndServerThreadId, NULL);
    }
}

/* give the driver the EE state block: it answers where its copy is, which gets the block */
/* 0x00220210 */
void SndLib_SendState(void) {
    gSndCallArgs[2] = (u32)UNCACHED(gSndDriverState);
    gSndIopState = *SndLib_Call(0x40000, gSndCallArgs);
    SndLib_DmaToIop(gSndCallArgs[2], gSndIopState, 0x80, 0, 0);
}

/* bind the driver's two servers (retrying until they are up) */
/* 0x00220270 */
void SndLib_Bind(void) {
    do {
        sceSifBindRpc(&gSndClient, 0x77777777, 0);
        sif_delay();
    } while (AT(D_019756E4, 0, u32) == 0);
    do {
        sceSifBindRpc(&gSndClient2, 0x77777778, 0);
        sif_delay();
    } while (AT(D_01976F24, 0, u32) == 0);
}

/* clear the library's blocks */
/* 0x00220340 */
void SndLib_Clear(void) {
    msl_memset(gSndCallBlock, 0, 0xB4);
    msl_memset(gSndCallArgs, 0, 0x20);
    msl_memset(D_01970D80, 0, 0x600);
    msl_memset(D_01971380, 0, 0x10);
    msl_memset(D_019713C0, 0, 0x88);
    msl_memset(D_01971480, 0, 0x60);
    msl_memset(gSndCurves, 0, 0x80);
    msl_memset(&gSndDma, 0, 0x10);
    gSndLastDma = 0;
    msl_memset(UNCACHED(gSndDriverState), 0, 0x80);
    gSndIopState = 0;
    msl_memset(UNCACHED(gSndTransferBits), 0, 0x10);
}

/* the driver's start arguments into `out` ("a\0b\0..."; three addresses, then numbers - the
   last (+0x1A) kept: 1 the driver reads EE memory itself); their length, 0 past 250 */
/* 0x00220440 */
s8 SndLib_DriverArgs(char *out, u8 *p) {
    char s[10][0x10];
    char *e = out;
    s32 i, n;

    msl_snprintf(s[0], 0x10, str_0xN, AT(p, 0x0, u32));
    msl_snprintf(s[1], 0x10, str_0xN, AT(p, 0x4, u32));
    msl_snprintf(s[2], 0x10, str_0xN, AT(p, 0x8, u32));
    msl_snprintf(s[3], 0x10, D_004572B8, AT(p, 0xC, u32));
    for (i = 0; i < 6; i++) {
        msl_snprintf(s[4 + i], 0x10, D_004572B8, AT(p, 0x10 + i * 2, u16));
    }
    D_0047B228 = AT(p, 0x1A, u16);
    for (i = 0; i < 10; i++) {
        e = msl_strchr(msl_strcpy(e, s[i]), 0) + 1;
    }
    n = (e - out) & 0xFF;
    if (n >= 0xFB) {
        n = 0;
    }
    return n;
}

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

/* positioned sound `which` (kPositionedSounds: footsteps and the like) at `pos` */
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

extern void *SndDriver_vtable[], *D_0046BF2C[], *D_0046AF90[], *D_0046AD88[];

/* destructor */
/* 0x0020E000 */
u8 *SndDriver_dtor(u8 *d, s32 flags) {
    if (d != NULL) {
        AT(d, 0x0, void **) = SndDriver_vtable;
        AT(d, 0x4, void **) = D_0046BF2C;
        __destroy_arr(d + 0x108, (void * (*)(void *, s32))IopArray_dtor, 0x18, 8);
        __destroy_arr(d + 0x84, (void * (*)(void *, s32))IopBuffers_dtor, 0x10, 8);
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
            __dl__FPv(d);
        }
    }
    return d;
}

/* ---- the sound interface (at +4, gSound): each method the driver's ---- */

#define THUNK(name, ret, target, params, args) \
    ret name params { return target args; }
#define THUNKV(name, target, params, args) \
    void name params { target args; }

THUNK(SndDriver_dtor_thunk, u8 *, SndDriver_dtor, (u8 *s, s32 f), (s - 4, f))
THUNKV(SndDriver_StopAll_thunk, SndDriver_StopAll, (u8 *s), (s - 4))
THUNKV(SndDriver_StopMasks_thunk, SndDriver_StopMasks, (u8 *s, u32 a, u32 b), (s - 4, a, b))
THUNKV(SndDriver_Play_thunk, SndDriver_Play, (u8 *s, u32 id, u32 k), (s - 4, id, k))
THUNKV(SndDriver_StopSound_thunk, SndDriver_StopSound, (u8 *s, u32 id, u32 k), (s - 4, id, k))
THUNKV(SndDriver_SeqPlay_thunk, SndDriver_SeqPlay, (u8 *s, u32 k, u32 pos, u32 m), (s - 4, k, pos, m))
THUNKV(SndDriver_SeqStop_thunk, SndDriver_SeqStop, (u8 *s, u32 k), (s - 4, k))
THUNKV(SndDriver_SeqRestart_thunk, SndDriver_SeqRestart, (u8 *s, u32 k, u32 pos), (s - 4, k, pos))
THUNK(SndDriver_SeqParam1B_thunk, s32, SndDriver_SeqParam1B, (u8 *s, u32 k, u32 a), (s - 4, k, a))
THUNK(SndDriver_SeqParam2B_thunk, s32, SndDriver_SeqParam2B, (u8 *s, u32 k, u32 a, u32 b), (s - 4, k, a, b))
THUNK(SndDriver_SeqParamPtr_thunk, s32, SndDriver_SeqParamPtr, (u8 *s, u32 k, s32 *p), (s - 4, k, p))
THUNK(SndDriver_SeqParam1_thunk, s32, SndDriver_SeqParam1, (u8 *s, u32 k, u32 a), (s - 4, k, a))
THUNK(SndDriver_SeqParam2_thunk, s32, SndDriver_SeqParam2, (u8 *s, u32 k, u32 a, u32 b), (s - 4, k, a, b))
THUNK(SndDriver_SetSeqVolume_thunk, s32, SndDriver_SetSeqVolume, (u8 *s, u32 k, u32 v), (s - 4, k, v))
THUNK(SndDriver_SeqVolume_thunk, u8, SndDriver_SeqVolume, (u8 *s, u32 k), (s - 4, k))
THUNK(SndDriver_SetSeqTempo_thunk, s32, SndDriver_SetSeqTempo, (u8 *s, u32 k, u32 t), (s - 4, k, t))
THUNK(SndDriver_SeqTempo_thunk, u16, SndDriver_SeqTempo, (u8 *s, u32 k), (s - 4, k))
THUNKV(SndDriver_Header_thunk, SndDriver_Header, (u8 *s, u32 k, u32 src, u32 n), (s - 4, k, src, n))
THUNKV(SndDriver_Table_thunk, SndDriver_Table, (u8 *s, u32 k, u32 src, u32 n), (s - 4, k, src, n))
THUNKV(SndDriver_Sequence_thunk, SndDriver_Sequence, (u8 *s, u32 k, u32 src, u32 n), (s - 4, k, src, n))
THUNKV(SndDriver_SamplesNow_thunk, SndDriver_SamplesNow, (u8 *s, u32 k, u32 src, u32 n), (s - 4, k, src, n))
THUNKV(SndDriver_QueueSamples_thunk, SndDriver_QueueSamples, (u8 *s, u32 k, u32 src, u32 n), (s - 4, k, src, n))
THUNKV(SndDriver_Register_thunk, SndDriver_Register, (u8 *s, u32 k), (s - 4, k))
THUNKV(SndDriver_Unload_thunk, SndDriver_Unload, (u8 *s, u32 k), (s - 4, k))
THUNKV(SndDriver_SetOutputMode_thunk, SndDriver_SetOutputMode, (u8 *s, s8 m), (s - 4, m))
THUNK(SndDriver_OutputMode_thunk, s8, SndDriver_OutputMode, (u8 *s), (s - 4))
THUNK(SndDriver_TransferRunning_thunk, u8, SndDriver_TransferRunning, (u8 *s, u32 k), (s - 4, k))
THUNK(SndDriver_BankLoading_thunk, s32, SndDriver_BankLoading, (u8 *s, u32 k), (s - 4, k))
THUNK(SndDriver_GetVolume_thunk, u16, SndDriver_GetVolume, (u8 *s, u32 ch), (s - 4, ch))
THUNKV(SndDriver_SetVolume_thunk, SndDriver_SetVolume, (u8 *s, u32 ch, u32 v), (s - 4, ch, v))
THUNK(SndDriver_LoadPart_thunk, s32, SndDriver_LoadPart, (u8 *s, const char *f, u32 k, u32 p, void *buf), (s - 4, f, k, p, buf))
THUNKV(SndDriver_CancelFile_thunk, SndDriver_CancelFile, (u8 *s, u32 k), (s - 4, k))
THUNKV(SndDriver_DropFile_thunk, SndDriver_DropFile, (u8 *s, u32 k), (s - 4, k))
THUNKV(SndDriver_DropFiles_thunk, SndDriver_DropFiles, (u8 *s), (s - 4))
THUNK(SndDriver_BankFileLoaded_thunk, s32, SndDriver_BankFileLoaded, (u8 *s, u32 k), (s - 4, k))
THUNKV(SndDriver_PlacedState_thunk, SndDriver_PlacedState, (u8 *s, u32 st), (s - 4, st))
THUNKV(SndDriver_StopPlaced_thunk, SndDriver_StopPlaced, (u8 *s), (s - 4))
THUNKV(SndDriver_StopPlacedButBanks_thunk, SndDriver_StopPlacedButBanks, (u8 *s), (s - 4))
THUNKV(SndDriver_StopVoices_thunk, SndDriver_StopVoices, (u8 *s), (s - 4))
THUNK(SndDriver_BankLoaded_thunk, u8, SndDriver_BankLoaded, (u8 *s, u32 k), (s - 4, k))
THUNKV(SndDriver_SetSound_thunk, SndDriver_SetSound, (u8 *s, f32 v), (s - 4, v))
THUNKV(SndDriver_SetMaster_thunk, SndDriver_SetMaster, (u8 *s, f32 v), (s - 4, v))
THUNKV(SndDriver_SendMaster_thunk, SndDriver_SendMaster, (u8 *s), (s - 4))
THUNKV(SndDriver_PlayPlaced_thunk, SndDriver_PlayPlaced, (u8 *s, u32 id, u32 k, s8 v, s8 p), (s - 4, id, k, v, p))
THUNKV(SndDriver_PlayPositioned_thunk, SndDriver_PlayPositioned, (u8 *s, u32 w), (s - 4, w))
THUNK(SndDriver_Block3D_thunk, u8 *, SndDriver_Block3D, (u8 *s), (s - 4))
