/* Capcom's IOP sound driver (SNDDRV.IRX "ver311", T. Masuda, Dec 2004) on PC: its RPC servers
 * 0x77777777 (commands, 0x20-byte argument blocks) and 0x77777778 (transfers into sound
 * memory), ported from the module (handlers at IRX 0xA7B4 / 0xB018). The sound memory and
 * voices are the SPU2 emulation in spu2.c. */
#include <stdio.h>
#include <string.h>

#include "iop_mem.h"

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

/* the command server */
void *snddrv_rpc(unsigned fno, void *args, int size) {
    unsigned *a = args;
    unsigned cmd = fno >> 16;

    (void)size;
    memset(sResult, 0, sizeof(sResult));
    switch (cmd) {
    case 0x04:   /* the EE's state block: where the driver's copy is */
        sEeState = a[2];
        sResult[0] = iop_alloc(0x80);
        break;
    case 0x0A: { /* a bank: slot (fno & 0x7FFF) or, with 0x8000, the first free from 4 */
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

        if (p != NULL && to < SPU_RAM_SIZE && n <= SPU_RAM_SIZE - to) {
            memcpy(spu_ram + to, p, n);
        }
    }
    return sResult;
}
