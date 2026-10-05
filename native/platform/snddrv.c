/* Capcom's IOP sound driver (SNDDRV.IRX "ver311", T. Masuda, Dec 2004) on PC: its RPC servers
 * 0x77777777 (commands, 0x20-byte argument blocks) and 0x77777778 (transfers into sound
 * memory), ported from the module (handlers at IRX 0xA7B4 / 0xB018). The sound memory and
 * voices are the SPU2 emulation in spu2.c. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iop_mem.h"
#include "snd.h"

unsigned char iop_ram[IOP_RAM_SIZE];
unsigned char spu_ram[SPU_RAM_SIZE];
static unsigned sHeap = 0x100000;   /* the IOP heap (below: the driver's own data) */

void *iop_ptr(unsigned addr, unsigned n) {
    if (addr >= IOP_RAM_SIZE || n > IOP_RAM_SIZE - addr) {
        return NULL;
    }
    return iop_ram + addr;
}

void iop_write(unsigned addr, const void *src, unsigned n) {
    void *p = iop_ptr(addr, n);

    if (p == NULL || src == NULL) {
        fprintf(stderr, "iop: bad write of %u bytes to %08X\n", n, addr);
        return;
    }
    memcpy(p, src, n);
}

unsigned iop_alloc(unsigned size) {
    unsigned a = (sHeap + 0x3F) & ~0x3Fu;

    if (a + size > IOP_RAM_SIZE) {
        fprintf(stderr, "iop: heap full (%u bytes)\n", size);
        return 0;
    }
    sHeap = a + size;
    return a;
}

/* ---- the driver's state ---- */

/* the banks (IRX D_0000DFC4): the EE's bank block (command 0xA), 0xB4 bytes, 32 of them */
typedef struct Bank {
    char name[0x80];
    unsigned hd;      /* +0x80 its header (IOP memory) */
    unsigned buf;     /* +0x84 the IOP transfer buffer */
    unsigned spu;     /* +0x88 its samples (sound memory) */
    unsigned seq;     /* +0x8C its sequence (IOP memory), or 0 */
    unsigned sdt;     /* +0x90 its sound table (IOP memory), or 0 */
    unsigned pad[5];
    unsigned owner;   /* +0xB0 -1: by slot */
} Bank;

static Bank sBanks[0x21];
static unsigned sEeState;      /* the EE's state block (command 4) */
static unsigned sResult[0x26];

/* ---- the sound effects (IRX 0x7AA0 and the voice table D_0000D670) ---- */

#define NV 48

typedef struct VoiceRec {
    unsigned char pri;          /* +0x0 the sound's priority (0: free) */
    unsigned char busy;         /* +0x1 */
    unsigned char placed;       /* +0x2 1: volumes from the EE's 3D placement */
    int bank, entry;            /* +0x4, +0x8 the sound playing (-1: none) */
    int pan;                    /* +0xC pan 0..127, or the 3D left << 16 | right */
    int vol;                    /* +0x10 its volume (14 bits) */
    int chvol;                  /* +0x14 the voice's own volume (0..255) */
    int lastL, lastR;           /* the volumes set */
} VoiceRec;

static VoiceRec sVoices[NV];
static int sMasterVol = 0xFF;   /* D_0000DF84 */
static int sStereo = 1;         /* D_0000DF80 (command 0x16) */
static int sAutoVoice;          /* D_0000D1BC */
static int sInited;
static int sLog;          /* HG_SNDLOG: the commands on stderr */

/* an SDT entry unpacked (IRX 0x39FC; the EE's func_0021F5C0), type 2 moved onto type 1's
   layout (0x41B8) */
typedef struct Entry {
    unsigned flags;             /* 1 even priority wins, 2 fixed pan, 4 continues, 0x10 any free
                                   voice, 0x20 empty */
    unsigned char bank, prog, split, voice, pri, pan;
    signed char vol;
    unsigned char note, x, layers, curve;
} Entry;

static void parse_entry(const unsigned char *e, Entry *o, int type2) {
    memset(o, 0, sizeof(*o));
    if (e[0] == 1 && !type2) {
        o->bank = e[1];
        o->flags |= (e[2] >> 7) & 1;
        o->pri = e[2] & 0x7F;
        o->flags |= (e[3] >> 5) & 4;
        o->flags |= (e[3] >> 3) & 8;
        o->voice = e[3] & 0x3F;
        o->flags |= (e[4] >> 6) & 2;
        o->pan = e[4] & 0x7F;
        o->vol = (signed char)e[5];
        o->note = e[6];
        o->x = (e[7] >> 4) & 0xF;
        o->layers = e[7] & 7;
        o->flags |= (e[7] * 2) & 0x10;
        o->prog = e[8] & 0x7F;
        o->split = e[9] & 0x7F;
        o->curve = e[0xA];
    } else if (e[0] == 2 && type2) {
        o->bank = e[2];
        o->flags |= (e[3] >> 7) & 1;
        o->pri = e[3] & 0x7F;
        o->flags |= (e[4] >> 5) & 4;
        o->flags |= (e[4] >> 3) & 8;
        o->voice = e[4] & 0x3F;
        o->flags |= (e[5] >> 6) & 2;
        o->pan = e[5] & 0x7F;
        o->vol = (signed char)e[6];
        o->note = e[7];
        o->x = (e[8] >> 4) & 0xF;
        o->layers = e[8] & 7;
        o->flags |= (e[8] * 2) & 0x10;
        o->prog = e[9] & 0x7F;
        o->split = e[0xA] & 0x7F;
        o->curve = e[0xB];
    } else if (e[0] == 0) {
        o->flags |= 0x20;
        o->flags |= (e[3] >> 5) & 4;
    }
}

/* a sample layer of a program's split (IRX 0x3CF4 reading the HD bank) */
typedef struct Layer {
    unsigned char note;         /* the sample's base note */
    signed char fine;
    unsigned char pan, vol;
    unsigned short adsr1, adsr2;
    unsigned char mix;          /* dry / wet left / right */
    unsigned ssa;               /* sound memory */
    unsigned short rate;
} Layer;

typedef struct Tone {
    unsigned char progVol, progPan;
    signed char progTrans, progFine;
    unsigned char splitVol, splitPan;
    signed char splitTrans, splitFine;
    int n;
    Layer l[16];
} Tone;

static unsigned rd32(const unsigned char *p) { return p[0] | p[1] << 8 | p[2] << 16 | (unsigned)p[3] << 24; }
static unsigned rd16(const unsigned char *p) { return p[0] | p[1] << 8; }

static int read_tone(unsigned hd, unsigned spu, int prog, int split, Tone *t) {
    const unsigned char *h = iop_ptr(hd, 0x40);
    const unsigned char *pc, *p, *sp, *ss, *sm, *va;
    unsigned progOff, ssetOff, smplOff, vagiOff;
    int i;

    memset(t, 0, sizeof(*t));
    if (h == NULL) {
        return 0;
    }
    progOff = rd32(h + 0x24);
    ssetOff = rd32(h + 0x28);
    smplOff = rd32(h + 0x2C);
    vagiOff = rd32(h + 0x30);
    pc = iop_ptr(hd + progOff, 0x10);
    if (pc == NULL || prog > (int)rd32(pc + 0xC)) {
        return 0;
    }
    p = iop_ptr(hd + progOff + rd32(pc + 0x10 + prog * 4), 0x24);
    if (p == NULL || rd32(pc + 0x10 + prog * 4) == 0xFFFFFFFF) {
        return 0;
    }
    t->progVol = p[6];
    t->progPan = p[7];
    t->progTrans = (signed char)p[8];
    t->progFine = (signed char)p[9];
    if (split >= p[4]) {
        return 0;
    }
    sp = p + rd32(p) + split * p[5];
    t->splitVol = sp[0x10];
    t->splitPan = sp[0x11];
    t->splitTrans = (signed char)sp[0x12];
    t->splitFine = (signed char)sp[0x13];
    {
        const unsigned char *sc = iop_ptr(hd + ssetOff, 0x10);
        unsigned so = rd32(sc + 0x10 + rd16(sp) * 4);

        ss = iop_ptr(hd + ssetOff + so, 4);
    }
    t->n = ss[3] > 16 ? 16 : ss[3];
    {
        const unsigned char *vc = iop_ptr(hd + vagiOff, 0x10);

        va = vc + 0x10 + (rd32(vc + 0xC) + 1) * 4;
    }
    for (i = 0; i < t->n; i++) {
        const unsigned char *mc = iop_ptr(hd + smplOff, 0x10);
        const unsigned char *v;

        sm = iop_ptr(hd + smplOff + rd32(mc + 0x10 + rd16(ss + 4 + i * 2) * 4), 0x2A);
        if (sm == NULL) {
            t->n = i;
            break;
        }
        t->l[i].note = sm[0xB];
        t->l[i].fine = (signed char)sm[0xC];
        t->l[i].pan = sm[0xD];
        t->l[i].vol = sm[0x10];
        t->l[i].adsr1 = rd16(sm + 0x12);
        t->l[i].adsr2 = rd16(sm + 0x14);
        t->l[i].mix = sm[0x29];
        v = va + rd16(sm) * 8;
        t->l[i].ssa = rd32(v) + spu;
        t->l[i].rate = rd16(v + 4);
    }
    return 1;
}

static unsigned short sd_voice(int v) {   /* IRX 0xB224: voice 0..47 as a libsd voice entry */
    return v < 24 ? (unsigned short)(v * 2) : (unsigned short)(1 | (v - 24) * 2);
}

static void set_volume(int v, int l, int r) {   /* IRX 0xB408 */
    sceSdSetParam(sd_voice(v) | 0x0000, (unsigned short)(l & 0x7FFF));
    sceSdSetParam(sd_voice(v) | 0x0100, (unsigned short)(r & 0x7FFF));
}

static void key(int on, unsigned core0, unsigned core1) {
    sceSdSetSwitch(on ? 0x1500 : 0x1600, core0);
    sceSdSetSwitch((on ? 0x1500 : 0x1600) | 1, core1);
}

/* the voice records of voices that have gone quiet are cleared (the driver's main loop) */
static void voices_sweep(void) {
    int v;

    for (v = 0; v < NV; v++) {
        VoiceRec *r = &sVoices[v];

        if (sceSdGetParam(sd_voice(v) | 0x0500) == 0) {
            r->busy = 0;
            r->pri = 0;
            r->bank = -1;
            r->entry = -1;
            r->vol = 0;
            r->pan = r->placed == 1 ? 0 : 0x40;
        } else {
            r->busy = 1;
        }
    }
}

static void snd_init_once(void) {
    int v;

    if (sInited) {
        return;
    }
    sInited = 1;
    sLog = getenv("HG_SNDLOG") != NULL;
    snd_init();
    for (v = 0; v < NV; v++) {
        sVoices[v].chvol = 0xFF;
        sVoices[v].bank = sVoices[v].entry = -1;
        sVoices[v].pan = 0x40;
    }
}

/* the volume pair of a voice from its volume (IRX 0x4244 / 0x7AA0's tail) */
static void voice_lr(int v, int vol14, int *l, int *r) {
    VoiceRec *rec = &sVoices[v];
    int x = (int)((unsigned)((((unsigned)(((vol14 << 8) / 255) * sMasterVol) >> 8) << 8) / 255) * rec->chvol) >> 8;

    if (rec->placed) {
        int L = (short)(rec->pan >> 16), R = (short)rec->pan;

        *l = L * (short)x / 8192;
        *r = R * (short)x / 8192;
        return;
    }
    {
        int pan = sStereo ? rec->pan : 0x40;

        if (pan >= 0 && pan < 0x40) {
            *l = x;
            *r = x * (pan * 2) >> 7;
        } else if (pan > 0x40 && pan < 0x80) {
            if (pan == 0x7F) {
                pan = 0x80;
            }
            *r = x;
            *l = x * (0x100 - pan * 2) >> 7;
        } else {
            *l = *r = x;
        }
    }
}

/* the entry index of sound `id` in table `t` */
static int find_sound(const unsigned char *t, unsigned id, int type2) {
    unsigned n = 0;
    int i = 0;
    Entry e;

    while (n < id) {
        parse_entry(t + i * 16, &e, type2);
        if (!(e.flags & 4)) {
            n++;
        }
        i++;
        if (i > 0x4000) {
            return -1;
        }
    }
    return i;
}

/* command 0x26: play a sound (IRX 0x7AA0). a[0] bank (| 0x8000: by owner), a[2] flags | id,
   a[3] voice << 24 | pitch << 16 | vol << 8 (offsets / overrides by the flags), a[4] bank of
   the samples (flag 0x04000000), a[5] the 3D left << 16 | right */
static void play_sound(const unsigned *a) {
    unsigned w = a[2], arg = a[3];
    int type2 = (a[0] & 0x8000) != 0;
    int bank = (int)(a[0] & 0x7FFF), first, nent, k, j;
    const unsigned char *t;
    unsigned id = w & 0xFFFF;
    int ext = (w & 0x20000000) != 0;

    if (type2) {
        for (k = 0; k < 0x20; k++) {
            if (sBanks[k].owner == (a[0] & 0x7FFF)) {
                break;
            }
        }
        bank = k;
    }
    if (sLog) {
        fprintf(stderr, "snddrv: play bank %d hd %X spu %X sdt %X id %u\n", bank, bank < 0x20 ? sBanks[bank].hd : 0,
                bank < 0x20 ? sBanks[bank].spu : 0, bank < 0x20 ? sBanks[bank].sdt : 0, id);
    }
    if (bank >= 0x20 || sBanks[bank].sdt == 0) {
        return;
    }
    t = iop_ptr(sBanks[bank].sdt, 0x10);
    if (t == NULL) {
        return;
    }
    first = find_sound(t, id, type2);
    if (first < 0) {
        return;
    }
    for (nent = 1;; nent++) {
        Entry e;

        parse_entry(t + (first + nent) * 16, &e, type2);
        if (!(e.flags & 4)) {
            break;
        }
    }
    sAutoVoice = 0;
    for (j = 0; j < nent; j++) {
        Entry e;
        Tone tone;
        int v0, layers, l;
        unsigned on0 = 0, on1 = 0;

        parse_entry(t + (first + j) * 16, &e, type2);
        if (sLog) {
            const unsigned char *x = t + (first + j) * 16;

            fprintf(stderr, "snddrv:  entry %d: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X bank %d prog %d split %d voice %d pri %d (voice pri %d)\n",
                    first + j, x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], x[9], x[10], x[11], e.bank, e.prog,
                    e.split, e.voice, e.pri, sVoices[e.voice < NV ? e.voice : 0].pri);
        }
        if ((type2 ? 2 : 1) != t[(first + j) * 16] || (e.flags & 0x20)) {
            continue;
        }
        if (w & 0x04000000) {
            e.bank = (unsigned char)a[4];
        }
        if (!type2 && e.bank >= 0x20) {
            continue;
        }
        /* the voice */
        if (ext && (w & 0x01000000)) {
            v0 = (int)((arg >> 24) & 0x3F) + sAutoVoice;
            if (v0 >= NV) {
                v0 = NV - 1;
            }
            sAutoVoice++;
        } else if (!(e.flags & 0x10)) {
            v0 = e.voice >= NV ? NV - 1 : e.voice;
        } else {
            for (v0 = NV - 1; v0 >= 0 && sceSdGetParam(sd_voice(v0) | 0x0500) != 0; v0--) {
            }
            if (v0 < 0) {
                continue;
            }
        }
        if (ext && (w & 0x02000000)) {
            v0 += (signed char)(arg >> 24);
            if (v0 >= NV) {
                v0 = NV - 1;
            } else if (v0 < 0) {
                v0 = 0;
            }
        }
        if (e.pri < sVoices[v0].pri || (e.pri == sVoices[v0].pri && !(e.flags & 1))) {
            continue;
        }
        if (!read_tone(sBanks[e.bank].hd, sBanks[e.bank].spu, e.prog, e.split, &tone)) {
            if (sLog) {
                fprintf(stderr, "snddrv:  no tone (bank %d hd %X)\n", e.bank, sBanks[e.bank].hd);
            }
            continue;
        }
        layers = e.layers + 1;
        if (layers > tone.n) {
            layers = tone.n;
        }
        /* the old sounds off first */
        if (w & 0x80000000) {
            for (l = 0; l < layers; l++) {
                int v = v0 + l;

                if (v < 24) {
                    on0 |= 1u << v;
                } else if (v < NV) {
                    on1 |= 1u << (v - 24);
                }
            }
            key(0, on0, on1);
        }
        for (l = 0; l < layers && v0 + l < NV; l++) {
            Layer *ly = &tone.l[l];
            int v = v0 + l, pan, note, fine, vol, p, s, q, L, R;
            unsigned pitch;

            sceSdSetAddr(sd_voice(v) | 0x2040, ly->ssa & 0x7FFFF0);
            /* pan */
            if (e.flags & 2) {
                pan = e.pan;
            } else {
                pan = (signed char)(ly->pan - 0x40) + (signed char)(tone.progPan - 0x40) + (signed char)(tone.splitPan - 0x40) + 0x40;
                if (pan < 0) {
                    pan = 0;
                } else if (pan >= 0x80) {
                    pan = 0x7F;
                }
                if (ext && (w & 0x10000)) {
                    pan = arg & 0x7F;
                } else if (ext && (w & 0x20000)) {
                    pan += (signed char)arg;
                    if (pan >= 0x80) {
                        pan = 0x7F;
                    } else if (pan < 0) {
                        pan = 0;
                    }
                }
            }
            /* pitch */
            if (ext && (w & 0x200000)) {
                note = (arg >> 16) & 0x7F;
            } else {
                note = tone.progTrans + ly->note + tone.splitTrans + e.note;
                if (ext && (w & 0x400000)) {
                    note += (signed char)(arg >> 16);
                    if (note >= 0x80) {
                        note = 0x7F;
                    }
                    if (note < 0) {
                        note = 0;
                    }
                }
                note &= 0xFFFF;
            }
            fine = ly->fine + tone.progFine + tone.splitFine;
            if (ext && (w & 0x800000)) {
                int x = (a[4] >> 8) & 0xFF;

                fine += (signed char)(x * 0x319C / 10000);
            }
            pitch = sceSdNote2Pitch(ly->note, 0, (unsigned short)note, (short)fine);
            pitch = pitch * ly->rate / 48000;
            sceSdSetParam(sd_voice(v) | 0x0200, (unsigned short)pitch);
            sceSdSetParam(sd_voice(v) | 0x0300, ly->adsr1);
            sceSdSetParam(sd_voice(v) | 0x0400, ly->adsr2);
            /* volume: program x split x sample, the entry's offset spread over them */
            p = tone.progVol;
            s = tone.splitVol;
            q = ly->vol;
            if (e.vol > 0) {
                if (p + e.vol <= 0x80) {
                    p += e.vol;
                } else {
                    int rest = e.vol - (0x80 - p);

                    p = 0x80;
                    if (s + rest <= 0x80) {
                        s += rest;
                    } else {
                        q += rest - (0x80 - s);
                        s = 0x80;
                        if (q > 0x80) {
                            q = 0x80;
                        }
                    }
                }
            } else if (e.vol < 0) {
                if (p + e.vol >= 0) {
                    p += e.vol;
                } else {
                    int rest = e.vol + p;

                    p = 0;
                    if (s + rest >= 0) {
                        s += rest;
                    } else {
                        q += rest + s;
                        s = 0;
                        if (q < 0) {
                            q = 0;
                        }
                    }
                }
            }
            if (ext && (w & 0x40000)) {
                unsigned x = (arg >> 8) & 0x7F;

                vol = (int)((unsigned long long)(x * x * x) * 0xC0C0C0C1u >> 32 >> 7);
            } else if (ext && (w & 0x80000)) {
                vol = q * (p * s) / 170 + ((signed char)(arg >> 8) << 5);
                if ((unsigned)vol >= 0x4000) {
                    vol = 0x3FFF;
                }
            } else if (ext && (w & 0x100000)) {
                unsigned x = (unsigned)(q * (p * s) / 170) << 8;

                vol = (int)((x + (x >> 7) * (signed char)(arg >> 8)) >> 8);
                if ((unsigned)vol >= 0x4000) {
                    vol = 0x3FFF;
                }
            } else {
                vol = (int)((unsigned long long)(unsigned)(q * (p * s)) * 0xC0C0C0C1u >> 32 >> 7);
            }
            sVoices[v].vol = vol;
            if (ext && (w & 0x08000000)) {
                sVoices[v].placed = 1;
                sVoices[v].pan = (int)a[5];
            } else {
                sVoices[v].placed = 0;
                sVoices[v].pan = pan;
            }
            voice_lr(v, vol, &L, &R);
            if (ext && (w & 0x08000000) && !(w & 0x80000000) && ((L ^ sVoices[v].lastL) & 0x80000000)) {
                L = (L + sVoices[v].lastL) / 2;
                R = (R + sVoices[v].lastR) / 2;
            }
            set_volume(v, L, R);
            sVoices[v].lastL = L;
            sVoices[v].lastR = R;
            /* the dry / wet mix (IRX 0xBB20): not modelled without reverb */
            sVoices[v].pri = e.pri;
        }
        if (w & 0x80000000) {
            for (l = 0; l < layers && v0 + l < NV; l++) {
                sVoices[v0 + l].bank = bank;
                sVoices[v0 + l].entry = first;
            }
            key(1, on0, on1);
        }
    }
}

/* command 0x28: stop sound a[2] & 0xFFFF of bank a[0] (its voices released) */
static void stop_sound(const unsigned *a) {
    int type2 = (a[0] & 0x8000) != 0;
    int bank = (int)(a[0] & 0x7FFF), first, v;
    unsigned off0 = 0, off1 = 0;
    const unsigned char *t;

    if (bank >= 0x20 || sBanks[bank].sdt == 0 || (t = iop_ptr(sBanks[bank].sdt, 0x10)) == NULL) {
        return;
    }
    first = find_sound(t, a[2] & 0xFFFF, type2);
    for (v = 0; v < NV; v++) {
        if (sVoices[v].bank == bank && sVoices[v].entry == first) {
            if (v < 24) {
                off0 |= 1u << v;
            } else {
                off1 |= 1u << (v - 24);
            }
        }
    }
    key(0, off0, off1);
}

/* the command server */
void *snddrv_rpc(unsigned fno, void *args, int size) {
    unsigned *a = args;
    unsigned cmd = fno >> 16;
    int v;

    (void)size;
    snd_init_once();
    voices_sweep();
    if (sLog) {
        fprintf(stderr, "snddrv: %02X.%04X %08X %08X %08X %08X\n", cmd, fno & 0xFFFF, a ? a[0] : 0, a ? a[2] : 0,
                a ? a[3] : 0, a ? a[4] : 0);
    }
    memset(sResult, 0, sizeof(sResult));
    switch (cmd) {
    case 0x04:   /* the EE's state block: where the driver's copy is */
        sEeState = a[2];
        sResult[0] = iop_alloc(0x80);
        break;
    case 0x0A: { /* a bank: slot (fno & 0x7FFF) */
        unsigned k = fno & 0x7FFF;

        if (args != NULL && k < 0x20) {
            memcpy(&sBanks[k], args, 0xB4);
            sBanks[k].owner = (unsigned)-1;
        }
        break;
    }
    case 0x0B:
        if ((fno & 0x7FFF) < 0x20) {
            memset(&sBanks[fno & 0x7FFF], 0, sizeof(Bank));
        }
        break;
    case 0x16:   /* the output: 1 stereo */
        if (a[2] <= 1) {
            sStereo = (int)a[2];
        }
        break;
    case 0x24:   /* the sound effects' master volume (0..255), applied to the voices playing */
        if (a[2] < 0x100) {
            sMasterVol = (int)a[2];
            for (v = 0; v < NV; v++) {
                if (sVoices[v].busy) {
                    int L, R;

                    voice_lr(v, sVoices[v].vol, &L, &R);
                    set_volume(v, L, R);
                }
            }
        }
        break;
    case 0x26:
        play_sound(a);
        break;
    case 0x27: { /* a list of sounds (fno & 0xFF of them, 0x20 bytes each) */
        unsigned n = fno & 0xFF, i;

        for (i = 0; i < n; i++) {
            play_sound(a + i * 8);
        }
        break;
    }
    case 0x28:
        stop_sound(a);
        break;
    case 0x29:   /* the voices in masks a[2] / a[3] off */
        key(0, a[2] & 0xFFFFFF, a[3] & 0xFFFFFF);
        break;
    case 0x35:   /* the voices in masks a[2] / a[3]: a[4] 0 volumes a[5], 1 re-volume, 2 their
                    own volume a[5] */
        for (v = 0; v < NV; v++) {
            unsigned m = v < 24 ? a[2] : a[3];
            int L, R;

            if (!(m & (1u << (v % 24)))) {
                continue;
            }
            switch (a[4]) {
            case 0:
                set_volume(v, (int)(a[5] >> 16), (int)(a[5] & 0xFFFF));
                break;
            case 2:
                sVoices[v].chvol = (signed char)a[5] & 0xFF;
                /* fall through */
            case 1:
                voice_lr(v, sVoices[v].vol, &L, &R);
                set_volume(v, L, R);
                break;
            }
        }
        break;
    default:
        break;
    }
    return sResult;
}

/* the transfer server: 0x12 copies the IOP buffer into sound memory */
void *snddrv_rpc2(unsigned fno, void *args, int size) {
    unsigned *a = args;

    (void)size;
    memset(sResult, 0, sizeof(sResult));
    if ((fno >> 16) == 0x12 && a != NULL) {
        unsigned from = a[2], to = a[3], n = a[4];
        void *p = iop_ptr(from, n);

        if (sLog) {
            fprintf(stderr, "snddrv: transfer %X bytes IOP %X -> sound %X (%02X %02X %02X %02X)\n", n, from, to,
                    p ? ((unsigned char *)p)[0] : 0, p ? ((unsigned char *)p)[1] : 0, p ? ((unsigned char *)p)[0x20] : 0,
                    p ? ((unsigned char *)p)[0x21] : 0);
        }
        if (p != NULL && to < SPU_RAM_SIZE && n <= SPU_RAM_SIZE - to) {
            memcpy(spu_ram + to, p, n);
        }
    }
    return sResult;
}
