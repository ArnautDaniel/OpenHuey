/* The EE side of the PS2's SIF (EE <-> IOP link) and the bits of the EE kernel the sound
 * library uses, on PC: RPC calls go straight to the IOP modules emulated here (the sound driver,
 * snddrv.c), DMA copies into the emulated IOP memory, and the IOP heap is a simple allocator in
 * it. The game's EE threads for RPC servers are not run: the IOP side calls their handler. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iop_mem.h"

/* sceSifClientData: only the server pointer (+0x24, nonzero once bound) is looked at by the
   game; here it holds the server id */
#define CD_SERVER(cd) (*(unsigned *)((char *)(cd) + 0x24))

/* sceSifBindRpc */
int func_0026FF08(void *cd, unsigned id, int mode) {
    (void)mode;
    CD_SERVER(cd) = id;
    return 0;
}

/* sceSifCheckStatRpc: calls finish at once */
int func_002702E8(void *cd) {
    (void)cd;
    return 0;
}

/* sceSifCallRpc */
int func_002700E8(void *cd, unsigned fno, unsigned mode, void *send, int ssize, void *recv, int rsize,
                  void (*end)(void *), void *endParam) {
    void *r = NULL;

    switch (CD_SERVER(cd)) {
    case 0x77777777:
        r = snddrv_rpc(fno, send, ssize);
        break;
    case 0x77777778:
        r = snddrv_rpc2(fno, send, ssize);
        break;
    default:
        fprintf(stderr, "sif: call to unknown server %08X (%08X)\n", CD_SERVER(cd), fno);
        break;
    }
    if (recv != NULL && rsize > 0) {
        if (r != NULL) {
            memcpy(recv, r, (size_t)rsize);
        } else {
            memset(recv, 0, (size_t)rsize);
        }
    }
    if ((mode & 1) && end != NULL) {
        end(endParam);
    }
    return 0;
}

/* the EE's own RPC server (registered by the game's server thread, which isn't run): the IOP
   side calls the game's handler SndLib_ServerCall directly */
void func_00270328(void *queue, int thread) { (void)queue; (void)thread; }
void func_002703C0(void *sd, unsigned id, void *func, void *buf, void *cfunc, void *cbuf, void *queue) {
    (void)sd; (void)id; (void)func; (void)buf; (void)cfunc; (void)cbuf; (void)queue;
}
void func_002707D8(void *queue) { (void)queue; }

/* EE kernel: threads and semaphores the sound library makes (not run) */
int CreateSema(void *s) { (void)s; return 1; }
int CreateThread(void *t) { (void)t; return 1; }
int func_0026D2E0(int thread, void *arg) { (void)thread; (void)arg; return 0; }   /* StartThread */

/* data cache write-back before a DMA: nothing on PC */
void func_0026CA98(unsigned start, unsigned end) { (void)start; (void)end; }
void func_0026CB18(unsigned start, unsigned end) { (void)start; (void)end; }

typedef struct SifDma {
    unsigned src, dest;
    int size, attr;
} SifDma;

/* an EE address as a pointer: the uncached / accelerated views (0x2..., 0x3...) of a buffer
   are the buffer */
static const void *ee_ptr(unsigned a) {
    if ((a >> 28) == 2 || (a >> 28) == 3) {
        a &= 0x0FFFFFFFu;
    }
    return (const void *)a;
}

/* sceSifSetDma: EE memory into IOP memory, done at once */
unsigned sceSifSetDma(SifDma *d, int n) {
    int i;

    for (i = 0; i < n; i++) {
        if (getenv("HG_SNDLOG")) {
            const unsigned char *q = ee_ptr(d[i].src);

            fprintf(stderr, "sif: dma %08X -> IOP %X (%X bytes) %02X %02X\n", d[i].src, d[i].dest, d[i].size, q[0], q[0x20]);
        }
        iop_write(d[i].dest, ee_ptr(d[i].src), (unsigned)d[i].size);
    }
    return 1;
}

unsigned isceSifSetDma(SifDma *d, int n) { return sceSifSetDma(d, n); }
int sceSifDmaStat(unsigned id) { (void)id; return -1; }
int isceSifDmaStat(unsigned id) { (void)id; return -1; }

/* sceSifAllocIopHeap / sceSifFreeIopHeap */
unsigned func_002744D8(unsigned size) { return iop_alloc(size); }
int func_00274640(unsigned addr) { (void)addr; return 0; }
