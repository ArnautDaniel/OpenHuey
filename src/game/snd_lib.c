/* Capcom's EE sound library (PS2 0x0021EAC0..0x00220680, include/sndlib.h): calls into the IOP
 * sound driver SNDDRV.IRX by SIF RPC (server 0x77777777, a second one 0x77777778 for the bank
 * transfers), the EE -> IOP DMA, the RPC server the driver calls back (0x77777779), and the 3D
 * placement of a sound (its volume left/right from where it is and how far). */
#include "common.h"
#include "sndlib.h"
#include "snd_lib.h"
#include "libc.h"
#include "msl.h"
#include "sce/sif.h"

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
    at = (s32)(0x1.45f306p+9f /* 4096 / 2pi */ * func_0031C5C0(AT(b, 0x10, f32) - AT(b, 0x20, f32),
                                                                  AT(b, 0x18, f32) - AT(b, 0x28, f32))) & 0xFFF;
    dist = SndLib_Distance((f32 *)(b + 0x20), (f32 *)(b + 0x10));
    facing = (s32)(0x1.45f306p+9f * func_0031C5C0(AT(b, 0x30, f32) - AT(b, 0x40, f32),
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

/* ---- the driver's sound tables (the SDT files: 16 bytes an entry, an entry with flag 4 goes on
   the sound before it) ---- */

/* entry `e` of a sound table, unpacked (type 1: the 11-byte layout, type 2: 12 bytes with a
   16-bit number) */
/* 0x0021F5C0 */
void SndTable_Unpack(u8 *e, u8 *out) {
    func_00115D20(out, 0, 0x14);
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
            func_0026CA98(src, src + size);
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

/* wait out the client's call in progress */
static inline void sif_delay(void) {
    volatile s32 n;

    for (n = 0x2710 - 1; n > 0; n--) {
    }
}

static inline void sif_wait(SifClient *cd) {
    while (func_002702E8(cd) == 1) {
        sif_delay();
    }
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
        while (func_002700E8(&gSndClient2, cmd, 1, args, 0x20, D_01975700, 4, gSndCallback, flags) != 0) {
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
        while (func_002700E8(&gSndClient, cmd, 0, args, n << 5, gSndResults, 4, NULL, NULL) != 0) {
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
        while (func_002700E8(&gSndClient, cmd, 0, args, 0x20, gSndResults, 8, SndLib_CallDone, NULL) != 0) {
        }
        break;
    }
    case 0x320000:
    case 0x300000:
    case 0x2E0000:
    case 0x2D0000:
    case 0x2C0000:
    case 0x0:
        while (func_002700E8(&gSndClient, cmd, 0, args, 0, gSndResults, 4, NULL, NULL) != 0) {
        }
        break;
    case 0x2B0000:
        while (func_002700E8(&gSndClient, cmd, 1, args, 0x88, gSndResults, 4, SndLib_CallDone, NULL) != 0) {
        }
        break;
    case 0x310000:
    case 0x2A0000:
        while (func_002700E8(&gSndClient, cmd, 0, args, 0x88, gSndResults, 4, NULL, NULL) != 0) {
        }
        break;
    case 0xA0000:
        while (func_002700E8(&gSndClient, cmd, 0, args, 0xB4, gSndResults, 4, NULL, NULL) != 0) {
        }
        break;
    case 0x60000:
        while (func_002700E8(&gSndClient, cmd, 0, args, 0, gSndResults, 8, NULL, NULL) != 0) {
        }
        break;
    case 0x20000:
    case 0x10000:
    case 0x130000:
    case 0x2F0000:
    case 0xB0000:
    case 0x90000:
        while (func_002700E8(&gSndClient, cmd, 0, args, 0, gSndResults, 4, NULL, NULL) != 0) {
        }
        break;
    case 0x140000:
        while (func_002700E8(&gSndClient, cmd, 0, args, 0, gSndResults, 0x1C, NULL, NULL) != 0) {
        }
        break;
    case 0x50000:
        while (func_002700E8(&gSndClient, cmd, 0, args, 0, gSndResults, 0x98, NULL, NULL) != 0) {
        }
        break;
    default:
        while (func_002700E8(&gSndClient, cmd, 0, args, 0x20, gSndResults, 4, NULL, NULL) != 0) {
        }
        break;
    }
    return gSndResults;
}

/* ---- the EE's own RPC server (0x77777779), which the driver calls ---- */

extern u16 D_0047B228;     /* the driver's argument 10 (1: it reads EE memory itself) */
extern u32 gSndServerReply;     /* the server's reply */

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

extern u8 gSndServerQueue[];   /* the server's queue */
extern u8 D_01976F50[];   /* its serve data */
extern u8 D_019726C0[];   /* its receive buffer */
extern s32 gSndServerThreadId;    /* the server thread */

/* the server thread */
/* 0x0021F210 */
void SndLib_ServerThread(void) {
    func_00270328(gSndServerQueue, gSndServerThreadId);
    func_002703C0(D_01976F50, 0x77777779, SndLib_ServerCall, D_019726C0, NULL, NULL, gSndServerQueue);
    func_002707D8(gSndServerQueue);
}

typedef struct EeThread {
    s32 status;
    void *func;
    void *stack;
    s32 stackSize;
    void *gp;
    s32 prio;
    s32 pad[3];
} EeThread;

extern s32 D_0047B224;
extern s32 gSndSemaParams[3];      /* the semaphore's parameters */
extern EeThread gSndServerThread;
extern u8 D_01971680[];        /* the server thread's stack */
extern u8 _gp[];

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
        func_0026D2E0(gSndServerThreadId, NULL);
    }
}

extern u32 gSndCallArgs[8];   /* the call arguments */

/* give the driver the EE state block: it answers where its copy is, which gets the block */
/* 0x00220210 */
void SndLib_SendState(void) {
    gSndCallArgs[2] = (u32)UNCACHED(gSndDriverState);
    gSndIopState = *SndLib_Call(0x40000, gSndCallArgs);
    SndLib_DmaToIop(gSndCallArgs[2], gSndIopState, 0x80, 0, 0);
}

extern u8 D_019756E4[], D_01976F24[];   /* the clients' server pointers (bound when set) */

/* bind the driver's two servers (retrying until they are up) */
/* 0x00220270 */
void SndLib_Bind(void) {
    do {
        func_0026FF08(&gSndClient, 0x77777777, 0);
        sif_delay();
    } while (AT(D_019756E4, 0, u32) == 0);
    do {
        func_0026FF08(&gSndClient2, 0x77777778, 0);
        sif_delay();
    } while (AT(D_01976F24, 0, u32) == 0);
}

extern u8 gSndCallBlock[0xB4], D_01970D80[0x600], D_01971380[0x10], D_019713C0[0x88], D_01971480[0x60];

/* clear the library's blocks */
/* 0x00220340 */
void SndLib_Clear(void) {
    func_00115D20(gSndCallBlock, 0, 0xB4);
    func_00115D20(gSndCallArgs, 0, 0x20);
    func_00115D20(D_01970D80, 0, 0x600);
    func_00115D20(D_01971380, 0, 0x10);
    func_00115D20(D_019713C0, 0, 0x88);
    func_00115D20(D_01971480, 0, 0x60);
    func_00115D20(gSndCurves, 0, 0x80);
    func_00115D20(&gSndDma, 0, 0x10);
    gSndLastDma = 0;
    func_00115D20(UNCACHED(gSndDriverState), 0, 0x80);
    gSndIopState = 0;
    func_00115D20(UNCACHED(gSndTransferBits), 0, 0x10);
}

extern const char str_0xN[];   /* "%x" (as the driver reads them) */
extern const char D_004572B8[];   /* "%d" */

/* the driver's start arguments into `out` ("a\0b\0..."; three addresses, then numbers - the
   last (+0x1A) kept: 1 the driver reads EE memory itself); their length, 0 past 250 */
/* 0x00220440 */
s8 SndLib_DriverArgs(char *out, u8 *p) {
    char s[10][0x10];
    char *e = out;
    s32 i, n;

    func_0026EDD0(s[0], 0x10, str_0xN, AT(p, 0x0, u32));
    func_0026EDD0(s[1], 0x10, str_0xN, AT(p, 0x4, u32));
    func_0026EDD0(s[2], 0x10, str_0xN, AT(p, 0x8, u32));
    func_0026EDD0(s[3], 0x10, D_004572B8, AT(p, 0xC, u32));
    for (i = 0; i < 6; i++) {
        func_0026EDD0(s[4 + i], 0x10, D_004572B8, AT(p, 0x10 + i * 2, u16));
    }
    D_0047B228 = AT(p, 0x1A, u16);
    for (i = 0; i < 10; i++) {
        e = func_001180E8(func_001183C0(e, s[i]), 0) + 1;
    }
    n = (e - out) & 0xFF;
    if (n >= 0xFB) {
        n = 0;
    }
    return n;
}
