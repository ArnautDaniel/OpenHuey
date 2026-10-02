/* VIF1 + VU1 on PC: the 3D path. The game sends VIF1 streams (dma.c) that upload microprograms
 * (MPG), unpack vertex data into VU1 memory (UNPACK, with the write cycle, mask and row / column
 * modes) and start the microprograms (MSCAL / MSCNT); these transform, light and clip, and send
 * GIF packets to the GS (XGKICK, here straight into gs.c).
 *
 * The VU1 runs synchronously when started. Timing is modelled only where results depend on it:
 * an instruction waits for the vector registers it reads (FMAC / load latency 4), and Q / P
 * (DIV / EFU results) change only when their latency has passed, as pipelined microcode relies
 * on reading the previous Q. Flags are immediate. Floats follow the VU's rules: no infinities,
 * NaNs or denormals.
 *
 * HG_VU1DEBUG=1 reports unknown instructions and VIF codes once each. */
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gs_local.h"

/* ---- state ---- */

typedef union VuReg {
    float f[4];   /* x y z w */
    uint32_t u[4];
    int32_t i[4];
} VuReg;

static struct {
    uint64_t micro[2048];   /* 16 KB program memory (instruction pairs: lower | upper << 32) */
    VuReg mem[1024];        /* 16 KB data memory */
    VuReg vf[32];
    uint16_t vi[16];
    VuReg acc;
    float q, p, i, r;
    uint32_t mac, status, clip;
    uint32_t pc;            /* in instruction pairs */
    /* timing */
    uint64_t cyc;
    uint64_t vfReady[32];
    float qPending, pPending;
    uint64_t qReady, pReady;
    int qBusy, pBusy;
    /* a branch right after an instruction that wrote one of its VI registers reads the old
     * value (the VU's integer pipeline) */
    int viPrevReg;
    uint16_t viPrevOld;
    long runs, pairs;
} vu;

static inline uint16_t vi_branch(uint32_t r) {
    return (vu.viPrevReg == (int)r && r != 0) ? vu.viPrevOld : vu.vi[r];
}

static struct {
    uint32_t cl, wl, mode, mask;
    uint32_t row[4], col[4];
    uint32_t offset, base, tops, top, itops, itop, dbf;
} vif;

static int sDebug = -1;

static void debug_once(const char *what, uint32_t code) {
    static uint32_t seen[64];
    static int n;
    int k;

    if (sDebug < 0) {
        sDebug = getenv("HG_VU1DEBUG") != NULL;
    }
    if (!sDebug) {
        return;
    }
    for (k = 0; k < n; k++) {
        if (seen[k] == code) {
            return;
        }
    }
    if (n < 64) {
        seen[n++] = code;
    }
    fprintf(stderr, "vu1: %s %08x\n", what, code);
}

/* ---- VU floats ---- */

static inline float vuf(float x) {
    uint32_t u;

    memcpy(&u, &x, 4);
    if ((u & 0x7F800000) == 0x7F800000) {
        u = (u & 0x80000000) | 0x7F7FFFFF;   /* inf / NaN -> the largest value */
    } else if ((u & 0x7F800000) == 0) {
        u &= 0x80000000;                     /* denormal -> zero */
    }
    memcpy(&x, &u, 4);
    return x;
}

/* ---- XGKICK: GIF packets from VU1 memory to the GS (until the end of packet) ---- */

static void xgkick(uint32_t addr) {
    uint64_t buf[2 * 1024];
    int guard = 0;

    for (;;) {
        uint64_t lo = (uint64_t)vu.mem[addr & 0x3FF].u[0] | ((uint64_t)vu.mem[addr & 0x3FF].u[1] << 32);
        uint32_t nloop = (uint32_t)(lo & 0x7FFF), eop = (uint32_t)(lo >> 15) & 1;
        uint32_t flg = (uint32_t)(lo >> 58) & 3, nreg = (uint32_t)(lo >> 60) & 0xF, qw, k;

        if (nreg == 0) {
            nreg = 16;
        }
        qw = flg == 0 ? nloop * nreg : flg == 1 ? (nloop * nreg + 1) / 2 : nloop;
        if (qw + 1 > 1024) {
            qw = 1023;
        }
        for (k = 0; k <= qw; k++) {
            const VuReg *r = &vu.mem[(addr + k) & 0x3FF];

            buf[k * 2] = (uint64_t)r->u[0] | ((uint64_t)r->u[1] << 32);
            buf[k * 2 + 1] = (uint64_t)r->u[2] | ((uint64_t)r->u[3] << 32);
        }
        gs_gif(buf, qw + 1);
        addr += qw + 1;
        if (eop || ++guard > 1024) {
            break;
        }
    }
}

/* ---- flags ---- */

/* MAC flag bits for a written field (x = bit 3 .. w = bit 0): zero 0-3, sign 4-7 */
static void set_mac(const VuReg *d, uint32_t dest) {
    uint32_t mac = 0, st, f;

    for (f = 0; f < 4; f++) {
        uint32_t bit = 3 - f;

        if (!(dest & (8 >> f))) {
            continue;
        }
        if ((d->u[f] & 0x7FFFFFFF) == 0) {
            mac |= 1u << bit;
        }
        if (d->u[f] & 0x80000000) {
            mac |= 0x10u << bit;
        }
    }
    vu.mac = mac;
    st = vu.status & 0xFC0;
    if (mac & 0x000F) st |= 0x041;
    if (mac & 0x00F0) st |= 0x082;
    vu.status = st;
}

/* ---- the upper (FMAC) instruction ---- */

static float operand_bc(const VuReg *ft, uint32_t bc) { return ft->f[bc]; }

/* d = op(a, b) for the dest fields; kind: 0 add, 1 sub, 2 mul, 3 madd (acc + a*b), 4 msub
 * (acc - a*b), 5 max, 6 min */
static void fmac(VuReg *d, const VuReg *a, const float *b, uint32_t dest, int kind, int flags) {
    VuReg r = *d;
    uint32_t f;

    for (f = 0; f < 4; f++) {
        float x, y, v;

        if (!(dest & (8 >> f))) {
            continue;
        }
        x = vuf(a->f[f]);
        y = vuf(b[f]);
        switch (kind) {
        case 0: v = x + y; break;
        case 1: v = x - y; break;
        case 2: v = x * y; break;
        case 3: v = vuf(vu.acc.f[f]) + x * y; break;
        case 4: v = vuf(vu.acc.f[f]) - x * y; break;
        case 5: v = x > y ? x : y; break;
        default: v = x < y ? x : y; break;
        }
        r.f[f] = vuf(v);
    }
    *d = r;
    if (flags) {
        set_mac(d, dest);
    }
}

typedef struct Upper {
    int dst;        /* vf written (-1 none, 32 = ACC) */
    VuReg val;
    uint32_t dest;
} Upper;

static void upper_exec(uint32_t ins, Upper *out) {
    uint32_t dest = (ins >> 21) & 0xF, ft = (ins >> 16) & 0x1F, fs = (ins >> 11) & 0x1F, fd = (ins >> 6) & 0x1F;
    uint32_t op = ins & 0x3F, bc = ins & 3;
    const VuReg *s = &vu.vf[fs], *t = &vu.vf[ft];
    float b[4];
    int kind, toacc = 0, f;
    VuReg *d;

    out->dst = -1;
    out->dest = dest;
    if (op >= 0x3C) {
        uint32_t sp = ((ins >> 6) & 0x1F) << 2 | (ins & 3);

        out->val = vu.acc;
        d = &out->val;
        switch (sp) {
        case 0x00: case 0x01: case 0x02: case 0x03: kind = 0; goto acc_bc;
        case 0x04: case 0x05: case 0x06: case 0x07: kind = 1; goto acc_bc;
        case 0x08: case 0x09: case 0x0A: case 0x0B: kind = 3; goto acc_bc;
        case 0x0C: case 0x0D: case 0x0E: case 0x0F: kind = 4; goto acc_bc;
        case 0x18: case 0x19: case 0x1A: case 0x1B: kind = 2;
        acc_bc:
            for (f = 0; f < 4; f++) b[f] = operand_bc(t, sp & 3);
            fmac(d, s, b, dest, kind, 1);
            out->dst = 32;
            return;
        case 0x1C: kind = 2; goto acc_q;   /* MULAq */
        case 0x20: kind = 0; goto acc_q;   /* ADDAq */
        case 0x21: kind = 3; goto acc_q;
        case 0x24: kind = 1; goto acc_q;
        case 0x25: kind = 4;
        acc_q:
            for (f = 0; f < 4; f++) b[f] = vu.q;
            fmac(d, s, b, dest, kind, 1);
            out->dst = 32;
            return;
        case 0x1E: kind = 2; goto acc_i;   /* MULAi */
        case 0x22: kind = 0; goto acc_i;
        case 0x23: kind = 3; goto acc_i;
        case 0x26: kind = 1; goto acc_i;
        case 0x27: kind = 4;
        acc_i:
            for (f = 0; f < 4; f++) b[f] = vu.i;
            fmac(d, s, b, dest, kind, 1);
            out->dst = 32;
            return;
        case 0x28: kind = 0; goto acc_v;   /* ADDA */
        case 0x29: kind = 3; goto acc_v;
        case 0x2A: kind = 2; goto acc_v;
        case 0x2C: kind = 1; goto acc_v;
        case 0x2D: kind = 4;
        acc_v:
            fmac(d, s, t->f, dest, kind, 1);
            out->dst = 32;
            return;
        case 0x2E: {   /* OPMULA: ACC.xyz = fs.yzx * ft.zxy */
            float y[4] = {t->f[2], t->f[0], t->f[1], 0}, x[4] = {s->f[1], s->f[2], s->f[0], 0};
            VuReg xv;

            memcpy(xv.f, x, sizeof(x));
            fmac(d, &xv, y, 0xE, 2, 1);
            out->dst = 32;
            out->dest = 0xE;
            return;
        }
        case 0x10: case 0x11: case 0x12: case 0x13: {   /* ITOF0/4/12/15 */
            static const float sc[4] = {1.0f, 1.0f / 16, 1.0f / 4096, 1.0f / 32768};

            out->val = vu.vf[ft];
            for (f = 0; f < 4; f++) {
                if (dest & (8 >> f)) out->val.f[f] = (float)s->i[f] * sc[sp & 3];
            }
            out->dst = (int)ft;
            return;
        }
        case 0x14: case 0x15: case 0x16: case 0x17: {   /* FTOI0/4/12/15 */
            static const float sc[4] = {1.0f, 16.0f, 4096.0f, 32768.0f};

            out->val = vu.vf[ft];
            for (f = 0; f < 4; f++) {
                if (dest & (8 >> f)) {
                    double v = (double)vuf(s->f[f]) * sc[sp & 3];

                    out->val.i[f] = v >= 2147483647.0 ? 0x7FFFFFFF : v <= -2147483648.0 ? (int32_t)0x80000000 : (int32_t)v;
                }
            }
            out->dst = (int)ft;
            return;
        }
        case 0x1D:   /* ABS */
            out->val = vu.vf[ft];
            for (f = 0; f < 4; f++) {
                if (dest & (8 >> f)) out->val.u[f] = s->u[f] & 0x7FFFFFFF;
            }
            out->dst = (int)ft;
            return;
        case 0x1F: {   /* CLIP fs.xyz against |ft.w| */
            float w = fabsf(vuf(t->f[3]));
            uint32_t c = 0;

            if (vuf(s->f[0]) > w) c |= 0x01;
            if (vuf(s->f[0]) < -w) c |= 0x02;
            if (vuf(s->f[1]) > w) c |= 0x04;
            if (vuf(s->f[1]) < -w) c |= 0x08;
            if (vuf(s->f[2]) > w) c |= 0x10;
            if (vuf(s->f[2]) < -w) c |= 0x20;
            vu.clip = ((vu.clip << 6) | c) & 0xFFFFFF;
            return;
        }
        case 0x2F:   /* NOP */
            return;
        default:
            debug_once("upper", ins);
            return;
        }
    }
    out->val = vu.vf[fd];
    d = &out->val;
    if (op <= 0x1B) {   /* xxxbc */
        static const int kinds[7] = {0, 1, 3, 4, 5, 6, 2};

        kind = kinds[op >> 2];
        for (f = 0; f < 4; f++) b[f] = t->f[bc];
        fmac(d, s, b, dest, kind, kind < 5);
        out->dst = (int)fd;
        return;
    }
    switch (op) {
    case 0x1C: kind = 2; goto q;   /* MULq */
    case 0x20: kind = 0; goto q;   /* ADDq */
    case 0x21: kind = 3; goto q;
    case 0x24: kind = 1; goto q;
    case 0x25: kind = 4;
    q:
        for (f = 0; f < 4; f++) b[f] = vu.q;
        fmac(d, s, b, dest, kind, 1);
        break;
    case 0x1D: kind = 5; goto i;   /* MAXi */
    case 0x1E: kind = 2; goto i;   /* MULi */
    case 0x1F: kind = 6; goto i;   /* MINIi */
    case 0x22: kind = 0; goto i;
    case 0x23: kind = 3; goto i;
    case 0x26: kind = 1; goto i;
    case 0x27: kind = 4;
    i:
        for (f = 0; f < 4; f++) b[f] = vu.i;
        fmac(d, s, b, dest, kind, kind < 5);
        break;
    case 0x28: kind = 0; goto v;   /* ADD */
    case 0x29: kind = 3; goto v;   /* MADD */
    case 0x2A: kind = 2; goto v;   /* MUL */
    case 0x2B: kind = 5; goto v;   /* MAX */
    case 0x2C: kind = 1; goto v;   /* SUB */
    case 0x2D: kind = 4; goto v;   /* MSUB */
    case 0x2F: kind = 6;           /* MINI */
    v:
        fmac(d, s, t->f, dest, kind, kind < 5);
        break;
    case 0x2E: {   /* OPMSUB: fd.xyz = ACC - fs.yzx * ft.zxy */
        float y[4] = {t->f[2], t->f[0], t->f[1], 0};
        VuReg xv = {{s->f[1], s->f[2], s->f[0], 0}};

        fmac(d, &xv, y, 0xE, 4, 1);
        out->dest = 0xE;
        break;
    }
    default:
        debug_once("upper", ins);
        return;
    }
    out->dst = (int)fd;
    (void)toacc;
}

/* ---- the lower instruction ---- */

typedef struct Lower {
    int vfDst;        /* vf written (-1 none) */
    VuReg vfVal;
    uint32_t vfDest;
    int viDst;        /* vi written (-1 none) */
    uint16_t viVal;
    int branch;       /* 1: jump to target after the delay slot */
    uint32_t target;
} Lower;

static inline int32_t sext(uint32_t v, int bits) {
    return (int32_t)(v << (32 - bits)) >> (32 - bits);
}

static void efu(int op, const VuReg *s, uint32_t fsf) {
    float x = vuf(s->f[0]), y = vuf(s->f[1]), z = vuf(s->f[2]), w = vuf(s->f[3]);
    float a = vuf(s->f[fsf]), r = 0;
    int lat = 12;

    switch (op) {
    case 0x70: r = x * x + y * y + z * z; lat = 11; break;                       /* ESADD */
    case 0x71: r = 1.0f / (x * x + y * y + z * z); lat = 18; break;               /* ERSADD */
    case 0x72: r = sqrtf(x * x + y * y + z * z); lat = 18; break;                 /* ELENG */
    case 0x73: r = 1.0f / sqrtf(x * x + y * y + z * z); lat = 24; break;          /* ERLENG */
    case 0x74: r = atanf(y / x); lat = 54; break;                                 /* EATANxy */
    case 0x75: r = atanf(z / x); lat = 54; break;                                 /* EATANxz */
    case 0x76: r = x + y + z + w; lat = 12; break;                                /* ESUM */
    case 0x78: r = sqrtf(fabsf(a)); lat = 12; break;                              /* ESQRT */
    case 0x79: r = 1.0f / sqrtf(fabsf(a)); lat = 18; break;                       /* ERSQRT */
    case 0x7A: r = 1.0f / a; lat = 12; break;                                     /* ERCPR */
    case 0x7C: r = sinf(a); lat = 29; break;                                      /* ESIN */
    case 0x7D: r = atanf(a); lat = 54; break;                                     /* EATAN */
    case 0x7E: r = expf(-a); lat = 44; break;                                     /* EEXP */
    }
    if (vu.pBusy && vu.cyc < vu.pReady) {
        vu.cyc = vu.pReady;   /* the EFU is busy: stall */
    }
    if (vu.pBusy) {
        vu.p = vu.pPending;
    }
    vu.pPending = vuf(r);
    vu.pReady = vu.cyc + (uint64_t)lat;
    vu.pBusy = 1;
}

static void fdiv(float r, int lat) {
    if (vu.qBusy && vu.cyc < vu.qReady) {
        vu.cyc = vu.qReady;   /* the divider is busy: stall */
    }
    if (vu.qBusy) {
        vu.q = vu.qPending;
    }
    vu.qPending = vuf(r);
    vu.qReady = vu.cyc + (uint64_t)lat;
    vu.qBusy = 1;
}

static void lower_exec(uint32_t ins, Lower *out) {
    uint32_t op = ins >> 25, dest = (ins >> 21) & 0xF, ft = (ins >> 16) & 0x1F, fs = (ins >> 11) & 0x1F;
    uint32_t id = (ins >> 6) & 0x1F, it = ft & 0xF, is = fs & 0xF;
    int32_t imm11 = sext(ins & 0x7FF, 11);
    uint32_t imm15 = (ins & 0x7FF) | (((ins >> 21) & 0xF) << 11);
    uint32_t imm24 = ins & 0xFFFFFF, imm12 = (ins & 0x7FF) | (((ins >> 21) & 1) << 11);
    uint32_t fsf = (ins >> 21) & 3, ftf = (ins >> 23) & 3, f;
    const VuReg *s = &vu.vf[fs], *t = &vu.vf[ft];

    out->vfDst = -1;
    out->viDst = -1;
    out->branch = 0;
    out->vfDest = dest;
#define VI_SET(r, v) do { out->viDst = (int)(r); out->viVal = (uint16_t)(v); } while (0)
#define VF_SET(r, val) do { out->vfDst = (int)(r); out->vfVal = (val); } while (0)
#define MEM(a) vu.mem[(a) & 0x3FF]
    switch (op) {
    case 0x00: {   /* LQ */
        VF_SET(ft, MEM(vu.vi[is] + imm11));
        return;
    }
    case 0x01: {   /* SQ: fs to (it + imm) */
        VuReg *m = &MEM(vu.vi[it] + imm11);

        for (f = 0; f < 4; f++) if (dest & (8 >> f)) m->u[f] = s->u[f];
        return;
    }
    case 0x04: {   /* ILW */
        const VuReg *m = &MEM(vu.vi[is] + imm11);

        for (f = 0; f < 4; f++) if (dest & (8 >> f)) { VI_SET(it, m->u[f]); break; }
        return;
    }
    case 0x05: {   /* ISW */
        VuReg *m = &MEM(vu.vi[is] + imm11);

        for (f = 0; f < 4; f++) if (dest & (8 >> f)) m->u[f] = vu.vi[it];
        return;
    }
    case 0x08: VI_SET(it, vu.vi[is] + imm15); return;   /* IADDIU */
    case 0x09: VI_SET(it, vu.vi[is] - imm15); return;   /* ISUBIU */
    case 0x10: VI_SET(1, (vu.clip & 0xFFFFFF) == imm24); return;              /* FCEQ */
    case 0x11: vu.clip = imm24; return;                                        /* FCSET */
    case 0x12: VI_SET(1, (vu.clip & imm24) != 0); return;                      /* FCAND */
    case 0x13: VI_SET(1, ((vu.clip | imm24) & 0xFFFFFF) == 0xFFFFFF); return;  /* FCOR */
    case 0x14: VI_SET(it, (vu.status & 0xFFF) == imm12); return;               /* FSEQ */
    case 0x15: vu.status = (vu.status & 0x3F) | (imm12 & 0xFC0); return;       /* FSSET */
    case 0x16: VI_SET(it, vu.status & imm12); return;                          /* FSAND */
    case 0x17: VI_SET(it, (vu.status | imm12) & 0xFFF); return;                /* FSOR */
    case 0x18: VI_SET(it, (vu.mac & 0xFFFF) == vu.vi[is]); return;             /* FMEQ */
    case 0x1A: VI_SET(it, vu.mac & vu.vi[is]); return;                         /* FMAND */
    case 0x1B: VI_SET(it, (vu.mac | vu.vi[is]) & 0xFFFF); return;              /* FMOR */
    case 0x1C: VI_SET(it, vu.clip & 0xFFF); return;                            /* FCGET */
    case 0x20: out->branch = 1; out->target = vu.pc + 1 + (uint32_t)imm11; return;   /* B */
    case 0x21: VI_SET(it, vu.pc + 2); out->branch = 1; out->target = vu.pc + 1 + (uint32_t)imm11; return;   /* BAL */
    case 0x24: out->branch = 1; out->target = vi_branch(is); return;                    /* JR */
    case 0x25: VI_SET(it, vu.pc + 2); out->branch = 1; out->target = vi_branch(is); return;   /* JALR */
    case 0x28: case 0x29: case 0x2C: case 0x2D: case 0x2E: case 0x2F: {
        int16_t a = (int16_t)vi_branch(is), b = (int16_t)vi_branch(it);
        int take;

        switch (op) {
        case 0x28: take = a == b; break;   /* IBEQ */
        case 0x29: take = a != b; break;   /* IBNE */
        case 0x2C: take = a < 0; break;    /* IBLTZ */
        case 0x2D: take = a > 0; break;    /* IBGTZ */
        case 0x2E: take = a <= 0; break;   /* IBLEZ */
        default: take = a >= 0; break;     /* IBGEZ */
        }
        if (take) {
            out->branch = 1;
            out->target = vu.pc + 1 + (uint32_t)imm11;
        }
        return;
    }
    case 0x40:
        break;
    default:
        debug_once("lower", ins);
        return;
    }
    /* special */
    switch (ins & 0x3F) {
    case 0x30: VI_SET(id, vu.vi[is] + vu.vi[it]); return;          /* IADD */
    case 0x31: VI_SET(id, vu.vi[is] - vu.vi[it]); return;          /* ISUB */
    case 0x32: VI_SET(it, vu.vi[is] + sext(id, 5)); return;        /* IADDI */
    case 0x34: VI_SET(id, vu.vi[is] & vu.vi[it]); return;          /* IAND */
    case 0x35: VI_SET(id, vu.vi[is] | vu.vi[it]); return;          /* IOR */
    default:
        if ((ins & 0x3F) < 0x3C) {
            debug_once("lower", ins);
            return;
        }
    }
    switch (((ins >> 6) & 0x1F) << 2 | (ins & 3)) {
    case 0x30:   /* MOVE (with no dest: NOP) */
        if (dest) VF_SET(ft, *s);
        return;
    case 0x31: {   /* MR32 */
        VuReg r = {{0}};

        r.u[0] = s->u[1]; r.u[1] = s->u[2]; r.u[2] = s->u[3]; r.u[3] = s->u[0];
        VF_SET(ft, r);
        return;
    }
    case 0x34:   /* LQI */
        VF_SET(ft, MEM(vu.vi[is]));
        if (is) VI_SET(is, vu.vi[is] + 1);
        return;
    case 0x35: {   /* SQI */
        VuReg *m = &MEM(vu.vi[it]);

        for (f = 0; f < 4; f++) if (dest & (8 >> f)) m->u[f] = s->u[f];
        if (it) VI_SET(it, vu.vi[it] + 1);
        return;
    }
    case 0x36:   /* LQD */
        if (is) VI_SET(is, vu.vi[is] - 1);
        VF_SET(ft, MEM((uint16_t)(vu.vi[is] - 1)));
        return;
    case 0x37: {   /* SQD */
        VuReg *m = &MEM((uint16_t)(vu.vi[it] - 1));

        for (f = 0; f < 4; f++) if (dest & (8 >> f)) m->u[f] = s->u[f];
        if (it) VI_SET(it, vu.vi[it] - 1);
        return;
    }
    case 0x38: {   /* DIV */
        float a = vuf(s->f[fsf]), b = vuf(t->f[ftf]);

        fdiv(b != 0 ? a / b : (((s->u[fsf] ^ t->u[ftf]) & 0x80000000) ? -3.4028235e38f : 3.4028235e38f), 7);
        return;
    }
    case 0x39: fdiv(sqrtf(fabsf(vuf(t->f[ftf]))), 7); return;   /* SQRT */
    case 0x3A: {   /* RSQRT */
        float b = sqrtf(fabsf(vuf(t->f[ftf])));

        fdiv(b != 0 ? vuf(s->f[fsf]) / b : 3.4028235e38f, 13);
        return;
    }
    case 0x3B:   /* WAITQ */
        if (vu.qBusy && vu.cyc < vu.qReady) vu.cyc = vu.qReady;
        return;
    case 0x3C: VI_SET(it, s->u[fsf]); return;   /* MTIR */
    case 0x3D: {   /* MFIR */
        VuReg r = vu.vf[ft];

        for (f = 0; f < 4; f++) if (dest & (8 >> f)) r.i[f] = (int16_t)vu.vi[is];
        VF_SET(ft, r);
        return;
    }
    case 0x3E: {   /* ILWR */
        const VuReg *m = &MEM(vu.vi[is]);

        for (f = 0; f < 4; f++) if (dest & (8 >> f)) { VI_SET(it, m->u[f]); break; }
        return;
    }
    case 0x3F: {   /* ISWR */
        VuReg *m = &MEM(vu.vi[is]);

        for (f = 0; f < 4; f++) if (dest & (8 >> f)) m->u[f] = vu.vi[it];
        return;
    }
    case 0x40: case 0x41: {   /* RNEXT / RGET */
        uint32_t x;
        VuReg r = vu.vf[ft];

        memcpy(&x, &vu.r, 4);
        if ((((ins >> 6) & 0x1F) << 2 | (ins & 3)) == 0x40) {
            x = (x & 0x7FFFFF) | 0x3F800000;
            x = ((x << 1) | (((x >> 4) ^ (x >> 22)) & 1)) & 0x7FFFFF;
            x |= 0x3F800000;
            memcpy(&vu.r, &x, 4);
        }
        for (f = 0; f < 4; f++) if (dest & (8 >> f)) r.u[f] = x;
        VF_SET(ft, r);
        return;
    }
    case 0x42: {   /* RINIT */
        uint32_t x = (s->u[fsf] & 0x7FFFFF) | 0x3F800000;

        memcpy(&vu.r, &x, 4);
        return;
    }
    case 0x43: {   /* RXOR */
        uint32_t x;

        memcpy(&x, &vu.r, 4);
        x = ((x ^ s->u[fsf]) & 0x7FFFFF) | 0x3F800000;
        memcpy(&vu.r, &x, 4);
        return;
    }
    case 0x64: {   /* MFP */
        VuReg r = vu.vf[ft];

        for (f = 0; f < 4; f++) if (dest & (8 >> f)) r.f[f] = vu.p;
        VF_SET(ft, r);
        return;
    }
    case 0x68: VI_SET(it, vif.top); return;    /* XTOP */
    case 0x69: VI_SET(it, vif.itop); return;   /* XITOP */
    case 0x6C: xgkick(vu.vi[is]); return;      /* XGKICK */
    case 0x6B: case 0x6D: return;              /* XITOP / XTOP on VU0, ignored */
    case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76:
    case 0x78: case 0x79: case 0x7A: case 0x7C: case 0x7D: case 0x7E:
        efu(((ins >> 6) & 0x1F) << 2 | (ins & 3), s, fsf);
        return;
    case 0x7B:   /* WAITP */
        if (vu.pBusy && vu.cyc < vu.pReady) vu.cyc = vu.pReady;
        return;
    default:
        debug_once("lower", ins);
        return;
    }
#undef VI_SET
#undef VF_SET
#undef MEM
}

/* registers an instruction pair reads (for the latency stalls) */
static void pair_reads(uint32_t up, uint32_t lo, int *regs, int *n) {
    *n = 0;
    regs[(*n)++] = (int)((up >> 11) & 0x1F);
    regs[(*n)++] = (int)((up >> 16) & 0x1F);
    if ((lo >> 25) == 0x40 || (lo >> 25) == 0x01) {
        regs[(*n)++] = (int)((lo >> 11) & 0x1F);
        regs[(*n)++] = (int)((lo >> 16) & 0x1F);
    }
}

/* ---- running a microprogram ---- */

static void vu1_run(uint32_t start) {
    int delay = 0, ending = 0;
    uint32_t target = 0;
    long steps = 0;

    vu.pc = start & 0x7FF;
    vu.viPrevReg = -1;
    vu.runs++;
    vu.vf[0].f[0] = vu.vf[0].f[1] = vu.vf[0].f[2] = 0.0f;
    vu.vf[0].f[3] = 1.0f;
    for (;;) {
        uint64_t pair = vu.micro[vu.pc & 0x7FF];
        uint32_t lo = (uint32_t)pair, up = (uint32_t)(pair >> 32);
        Upper u;
        Lower l = {-1, {{0}}, 0, -1, 0, 0, 0};
        int regs[4], nr, k, isLoadImm = (up >> 31) & 1, endBit = (up >> 30) & 1;
        int branchSlot = delay;

        /* wait for the registers read */
        pair_reads(up, isLoadImm ? 0 : lo, regs, &nr);
        for (k = 0; k < nr; k++) {
            if (regs[k] && vu.vfReady[regs[k]] > vu.cyc) {
                vu.cyc = vu.vfReady[regs[k]];
            }
        }
        if (vu.qBusy && vu.cyc >= vu.qReady) { vu.q = vu.qPending; vu.qBusy = 0; }
        if (vu.pBusy && vu.cyc >= vu.pReady) { vu.p = vu.pPending; vu.pBusy = 0; }

        upper_exec(up, &u);
        if (!isLoadImm) {
            lower_exec(lo, &l);
        }
        /* commit: lower first, the upper's result wins a shared register */
        if (l.vfDst > 0) {
            VuReg *d = &vu.vf[l.vfDst];
            uint32_t f;

            for (f = 0; f < 4; f++) if (l.vfDest & (8 >> f)) d->u[f] = l.vfVal.u[f];
            vu.vfReady[l.vfDst] = vu.cyc + 4;
        }
        vu.viPrevReg = -1;
        if (l.viDst > 0) {
            vu.viPrevReg = l.viDst;
            vu.viPrevOld = vu.vi[l.viDst];
            vu.vi[l.viDst] = l.viVal;
        }
        if (u.dst == 32) {
            vu.acc = u.val;
        } else if (u.dst > 0) {
            vu.vf[u.dst] = u.val;
            vu.vfReady[u.dst] = vu.cyc + 4;
        }
        if (isLoadImm) {
            memcpy(&vu.i, &lo, 4);
        }
        vu.vf[0].f[0] = vu.vf[0].f[1] = vu.vf[0].f[2] = 0.0f;
        vu.vf[0].f[3] = 1.0f;
        vu.vi[0] = 0;
        vu.cyc++;
        vu.pairs++;

        if (ending) {
            vu.pc++;
            break;
        }
        if (branchSlot) {
            vu.pc = target & 0x7FF;
            delay = 0;
        } else {
            vu.pc++;
        }
        if (l.branch) {
            delay = 1;
            target = l.target;
        }
        if (endBit) {
            ending = 1;
        }
        if (++steps > 4000000) {
            fprintf(stderr, "vu1: runaway microprogram (start %03x)\n", start);
            break;
        }
    }
}

/* ---- VIF1 ---- */

static void vif_write_field(VuReg *m, int f, uint32_t data, uint32_t cyclePos, int masked) {
    uint32_t mk = masked ? (vif.mask >> ((cyclePos > 3 ? 3 : cyclePos) * 8 + f * 2)) & 3 : 0;

    switch (mk) {
    case 0:
        if (vif.mode == 1) {
            data += vif.row[f];
        } else if (vif.mode == 2) {
            data += vif.row[f];
            vif.row[f] = data;
        }
        m->u[f] = data;
        break;
    case 1: m->u[f] = vif.row[f]; break;
    case 2: m->u[f] = vif.col[cyclePos > 3 ? 3 : cyclePos]; break;
    default: break;   /* write-protected */
    }
}

/* UNPACK: returns the words of data used */
static size_t vif_unpack(uint32_t cmd, uint32_t imm, uint32_t num, const uint32_t *w, size_t avail) {
    uint32_t vl = cmd & 3, vn = (cmd >> 2) & 3, masked = (cmd >> 4) & 1;
    uint32_t usn = (imm >> 14) & 1, addr = (imm & 0x3FF) + (((imm >> 15) & 1) ? vif.tops : 0);
    uint32_t cnt = num ? num : 256, elems = vn + 1, bits = 32u >> vl;
    uint32_t cl = vif.cl, wl = vif.wl, n, dataN = 0, bitpos = 0, total;
    const uint8_t *bytes = (const uint8_t *)w;

    if (wl == 0) {
        wl = cl = 1;
    }
    /* vectors that consume data */
    for (n = 0; n < cnt; n++) {
        if (wl <= cl || (n % wl) < cl) {
            dataN++;
        }
    }
    total = (vn == 3 && vl == 3) ? dataN * 16 : dataN * elems * bits;
    if ((size_t)((total + 31) / 32) > avail) {
        return avail;
    }
    for (n = 0; n < cnt; n++) {
        uint32_t dst, cyc = n % wl;
        VuReg *m;
        int f, fromData = wl <= cl || cyc < cl;
        uint32_t v[4] = {0, 0, 0, 0};

        dst = wl <= cl ? addr + (n / wl) * cl + cyc : addr + n;
        m = &vu.mem[dst & 0x3FF];
        if (fromData) {
            if (vn == 3 && vl == 3) {   /* V4-5: RGBA 5551 */
                uint32_t d = bytes[bitpos / 8] | ((uint32_t)bytes[bitpos / 8 + 1] << 8);

                v[0] = (d & 0x1F) << 3;
                v[1] = ((d >> 5) & 0x1F) << 3;
                v[2] = ((d >> 10) & 0x1F) << 3;
                v[3] = (d & 0x8000) ? 0x80 : 0;
                bitpos += 16;
            } else {
                for (f = 0; f < (int)elems; f++) {
                    uint32_t x;

                    if (bits == 32) {
                        memcpy(&x, bytes + bitpos / 8, 4);
                    } else if (bits == 16) {
                        x = bytes[bitpos / 8] | ((uint32_t)bytes[bitpos / 8 + 1] << 8);
                        x = usn ? x : (uint32_t)(int32_t)(int16_t)x;
                    } else {
                        x = bytes[bitpos / 8];
                        x = usn ? x : (uint32_t)(int32_t)(int8_t)x;
                    }
                    v[f] = x;
                    bitpos += bits;
                }
                if (vn == 0) {
                    v[1] = v[2] = v[3] = v[0];
                }
            }
            for (f = 0; f < 4; f++) {
                if (vn == 0 || vn == 3 || f <= (int)vn) {
                    vif_write_field(m, f, v[f], cyc, (int)masked);
                } else if (masked) {
                    /* V2 / V3: the missing fields only by the mask (row / column) */
                    uint32_t mk = (vif.mask >> ((cyc > 3 ? 3 : cyc) * 8 + f * 2)) & 3;

                    if (mk == 1 || mk == 2) {
                        vif_write_field(m, f, 0, cyc, 1);
                    }
                }
            }
        } else {
            /* filling: no data, the mask says what (row / column) */
            for (f = 0; f < 4; f++) {
                uint32_t mk = (vif.mask >> ((cyc > 3 ? 3 : cyc) * 8 + f * 2)) & 3;

                if (mk == 1) m->u[f] = vif.row[f];
                else if (mk == 2) m->u[f] = vif.col[cyc > 3 ? 3 : cyc];
            }
        }
    }
    return (total + 31) / 32;
}

static void vif_start(uint32_t addr, int cont) {
    vif.top = vif.tops;
    vif.itop = vif.itops;
    vif.dbf ^= 1;
    vif.tops = vif.dbf ? vif.base + vif.offset : vif.base;
    vu1_run(cont ? vu.pc : addr);
}

void vif1_run(const uint32_t *w, size_t n) {
    size_t i = 0;

    while (i < n) {
        uint32_t code = w[i++], cmd = (code >> 24) & 0x7F, imm = code & 0xFFFF, num = (code >> 16) & 0xFF;

        switch (cmd) {
        case 0x00: break;                                           /* NOP */
        case 0x01: vif.cl = imm & 0xFF; vif.wl = (imm >> 8) & 0xFF; break;   /* STCYCL */
        case 0x02: vif.offset = imm & 0x3FF; vif.dbf = 0; vif.tops = vif.base; break;   /* OFFSET */
        case 0x03: vif.base = imm & 0x3FF; break;                  /* BASE */
        case 0x04: vif.itops = imm & 0x3FF; break;                 /* ITOP */
        case 0x05: vif.mode = imm & 3; break;                      /* STMOD */
        case 0x06: case 0x07: break;                                /* MSKPATH3, MARK */
        case 0x10: case 0x11: case 0x13: break;                     /* FLUSHE / FLUSH / FLUSHA */
        case 0x14: case 0x15: vif_start(imm, 0); break;             /* MSCAL / MSCALF */
        case 0x17: vif_start(0, 1); break;                          /* MSCNT */
        case 0x20:                                                  /* STMASK */
            if (i < n) vif.mask = w[i];
            i += 1;
            break;
        case 0x30: case 0x31: {                                     /* STROW / STCOL */
            uint32_t *dst = cmd == 0x30 ? vif.row : vif.col, k;

            for (k = 0; k < 4 && i + k < n; k++) dst[k] = w[i + k];
            i += 4;
            break;
        }
        case 0x4A: {                                                /* MPG */
            uint32_t cnt = num ? num : 256, k;

            for (k = 0; k < cnt && i + 1 < n; k++, i += 2) {
                vu.micro[(imm + k) & 0x7FF] = (uint64_t)w[i] | ((uint64_t)w[i + 1] << 32);
            }
            break;
        }
        case 0x50: case 0x51: {                                     /* DIRECT / DIRECTHL */
            uint32_t q = imm ? imm : 65536;

            i = (i + 3) & ~(size_t)3;
            if (i + q * 4 > n) {
                q = (uint32_t)((n - i) / 4);
            }
            gs_gif((const uint64_t *)(w + i), q);
            i += q * 4;
            break;
        }
        default:
            if (cmd >= 0x60) {
                i += vif_unpack(cmd, imm, num, w + i, n - i);
            } else {
                debug_once("vif code", code);
            }
            break;
        }
    }
}

/* statistics for the frame (HG_GSDEBUG) */
void vu1_stats(long *runs, long *pairs) {
    *runs = vu.runs;
    *pairs = vu.pairs;
    vu.runs = vu.pairs = 0;
}
