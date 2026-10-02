/* DMA on PC: walk a source chain (the tags the game builds) and hand the data to the GIF
 * (channel 2) or to VIF1 (channel 1: VIF codes; DIRECT data goes to the GIF). VU1 (UNPACK / MPG /
 * MSCAL: the 3D path) isn't emulated yet: its data is skipped and reported once.
 *
 * Addresses in tags are 28 bits: the native build is 32-bit with its data below 0x10000000. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gs_local.h"

static uint32_t *sWords;   /* the chain's data as one stream (VIF: words; GIF: quadwords) */
static size_t sCount, sCap;

static void push_words(const uint32_t *w, size_t n) {
    if (sCount + n > sCap) {
        sCap = (sCount + n) * 2 + 1024;
        sWords = realloc(sWords, sCap * 4);
    }
    memcpy(sWords + sCount, w, n * 4);
    sCount += n;
}

static void *host(uint32_t addr) { return (void *)(uintptr_t)(addr & 0x0FFFFFF0u); }

/* ---- VIF1 ---- */

static void vif1_run(const uint32_t *w, size_t n) {
    static int warned;
    size_t i = 0;

    while (i < n) {
        uint32_t code = w[i++], cmd = (code >> 24) & 0x7F, imm = code & 0xFFFF, num = (code >> 16) & 0xFF;

        switch (cmd) {
        case 0x20:            /* STMASK */
            i += 1;
            break;
        case 0x30: case 0x31: /* STROW / STCOL */
            i += 4;
            break;
        case 0x4A:            /* MPG: microprogram (num 64-bit instructions) */
            i += (num ? num : 256) * 2;
            break;
        case 0x50: case 0x51: { /* DIRECT / DIRECTHL: quadwords to the GIF, 128-bit aligned */
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
            if (cmd >= 0x60) {   /* UNPACK: num vectors of vn+1 elements of 32 >> vl bits */
                uint32_t vl = cmd & 3, vn = (cmd >> 2) & 3, cnt = num ? num : 256;
                uint32_t bits = (32u >> vl) * (vn + 1) * cnt;

                i += (bits + 31) / 32;
                if (!warned) {
                    warned = 1;
                    fprintf(stderr, "dma: VIF1 UNPACK (VU1 geometry) not emulated yet; skipped\n");
                }
            }
            /* NOP, STCYCL, OFFSET, BASE, ITOP, STMOD, MSKPATH3, MARK, FLUSH*, MSCAL*, MSCNT */
            break;
        }
    }
}

/* ---- chain walking ---- */

void dma_send(int chan, uint32_t tagaddr, int tte) {
    uint32_t stack[2], sp = 0, guard = 0;
    uint32_t addr = tagaddr;
    int done = 0;

    sCount = 0;
    while (!done && guard++ < 1000000) {
        const uint64_t *tag = host(addr);
        uint32_t qwc = (uint32_t)(tag[0] & 0xFFFF), id = (uint32_t)(tag[0] >> 28) & 7;
        uint32_t taddr = (uint32_t)(tag[0] >> 32) & 0x7FFFFFF0u;
        uint32_t data = addr + 16, next = 0;
        static const uint32_t pad[2];

        if (chan == 1) {
            push_words(pad, 2);   /* the DMA tag's half keeps the VIF stream quadword-aligned */
            if (tte) {
                push_words((const uint32_t *)tag + 2, 2);
            } else {
                push_words(pad, 2);
            }
        }
        switch (id) {
        case 0:   /* refe */
            data = taddr;
            done = 1;
            break;
        case 1:   /* cnt */
            next = data + qwc * 16;
            break;
        case 2:   /* next */
            next = taddr;
            break;
        case 3: case 4:   /* ref / refs */
            data = taddr;
            next = addr + 16;
            break;
        case 5:   /* call */
            if (sp < 2) {
                stack[sp++] = data + qwc * 16;
            }
            next = taddr;
            break;
        case 6:   /* ret */
            if (sp > 0) {
                next = stack[--sp];
            } else {
                done = 1;
            }
            break;
        case 7:   /* end */
            done = 1;
            break;
        }
        if (qwc) {
            push_words(host(data), qwc * 4);
        }
        addr = next;
    }
    if (chan == 2) {
        gs_gif((const uint64_t *)sWords, (uint32_t)(sCount / 4));
    } else if (chan == 1) {
        vif1_run(sWords, sCount);
    }
}
